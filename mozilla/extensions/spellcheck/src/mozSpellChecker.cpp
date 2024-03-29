/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* ***** BEGIN LICENSE BLOCK *****
 * Version: MPL 1.1/GPL 2.0/LGPL 2.1
 *
 * The contents of this file are subject to the Mozilla Public License Version
 * 1.1 (the "License"); you may not use this file except in compliance with
 * the License. You may obtain a copy of the License at
 * http://www.mozilla.org/MPL/
 *
 * Software distributed under the License is distributed on an "AS IS" basis,
 * WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
 * for the specific language governing rights and limitations under the
 * License.
 *
 * The Original Code is Mozilla Spellchecker Component.
 *
 * The Initial Developer of the Original Code is
 * David Einstein.
 * Portions created by the Initial Developer are Copyright (C) 2001
 * the Initial Developer. All Rights Reserved.
 *
 * Contributor(s): David Einstein Deinst@world.std.com
 *
 * Alternatively, the contents of this file may be used under the terms of
 * either the GNU General Public License Version 2 or later (the "GPL"), or
 * the GNU Lesser General Public License Version 2.1 or later (the "LGPL"),
 * in which case the provisions of the GPL or the LGPL are applicable instead
 * of those above. If you wish to allow use of your version of this file only
 * under the terms of either the GPL or the LGPL, and not to allow others to
 * use your version of this file under the terms of the MPL, indicate your
 * decision by deleting the provisions above and replace them with the notice
 * and other provisions required by the GPL or the LGPL. If you do not delete
 * the provisions above, a recipient may use your version of this file under
 * the terms of any one of the MPL, the GPL or the LGPL.
 *
 * ***** END LICENSE BLOCK ***** */


#include "mozSpellChecker.h"
#include "nsIServiceManager.h"
#include "mozISpellI18NManager.h"

NS_IMPL_ISUPPORTS1(mozSpellChecker, nsISpellChecker)

mozSpellChecker::mozSpellChecker()
{
  NS_INIT_ISUPPORTS();
  mDictionaryName.SetLength(0);
  
  nsresult rv;

  mPersonalDictionary = do_GetService("@mozilla.org/spellchecker/personaldictionary;1",&rv);
  if (NS_FAILED(rv)) {
    NS_ERROR("Could not get personal Dictionary");
  }
  mSpellCheckingEngine = do_GetService("@mozilla.org/spellchecker/myspell;1",&rv);
  if (NS_FAILED(rv)) {
    NS_ERROR("Could not get spell checker");
  }
  mPersonalDictionary->Init();
  mSpellCheckingEngine->SetPersonalDictionary(mPersonalDictionary);
}

mozSpellChecker::~mozSpellChecker()
{
  if(mPersonalDictionary){
    //    mPersonalDictionary->Save();
    mPersonalDictionary->EndSession();
  }
  mSpellCheckingEngine = nsnull;
  mPersonalDictionary = nsnull;
}

NS_IMETHODIMP 
mozSpellChecker::SetDocument(nsITextServicesDocument *aDoc, PRBool aFromStartofDoc)
{
  mTsDoc = aDoc;
  mFromStart = aFromStartofDoc;
  return NS_OK;
}


NS_IMETHODIMP 
mozSpellChecker::NextMisspelledWord(nsString *aWord, nsStringArray *aSuggestions)
{
  if(!aSuggestions||!mConverter)
    return NS_ERROR_NULL_POINTER;

  PRUint32 selOffset;
  PRInt32 begin,end;
  nsresult result;
  result = SetupDoc(&selOffset);
  PRBool isMisspelled,done;
  if (NS_FAILED(result))
    return result;

  while( NS_SUCCEEDED(mTsDoc->IsDone(&done)) && !done )
    {
      nsString str;
      result = mTsDoc->GetCurrentTextBlock(&str);
  
      if (NS_FAILED(result))
        return result;
      do{
        result = mConverter->FindNextWord(str.get(),str.Length(),selOffset,&begin,&end);
        if(NS_SUCCEEDED(result)&&(begin != -1)){
          nsString currWord;
          currWord=Substring(str,begin,end-begin);
          result = CheckWord(&currWord,&isMisspelled,aSuggestions);
          if(isMisspelled){
              *aWord = currWord;
              mTsDoc->SetSelection(begin, end-begin);
              mTsDoc->ScrollSelectionIntoView();
              return NS_OK;
          }
        }
        selOffset = end;
      }while(end != -1);
      mTsDoc->NextBlock();
      selOffset=0;
    }
  return NS_OK;
}

NS_IMETHODIMP 
mozSpellChecker::CheckWord(const nsString *aWord, PRBool *aIsMisspelled, nsStringArray *aSuggestions)
{
  nsresult result;
  PRBool correct;
  if(!mSpellCheckingEngine)
    return NS_ERROR_NULL_POINTER;
  *aIsMisspelled = PR_FALSE;
  result = mSpellCheckingEngine->Check(aWord->get(),&correct);
  if(NS_FAILED(result))
    return result;
  if(!correct){
    if(aSuggestions){
      PRUint32 count,i;
      PRUnichar **words;
      nsAutoString temp;
      
      mSpellCheckingEngine->Suggest(aWord->get(),&words,&count);
      for(i=0;i<count;i++){
        temp.Assign(words[i]);
        aSuggestions->AppendString(temp);
      }
      NS_FREE_XPCOM_ALLOCATED_POINTER_ARRAY(count, words);
    }
    if(aIsMisspelled){
      *aIsMisspelled = PR_TRUE;
    }
  }
  return NS_OK;
}

NS_IMETHODIMP 
mozSpellChecker::Replace(const nsString *aOldWord, const nsString *aNewWord, PRBool aAllOccurrences)
{
  if(!mConverter)
    return NS_ERROR_NULL_POINTER;

  if(aAllOccurrences){
    PRUint32 selOffset;
    PRInt32 startBlock,currentBlock,currOffset;
    PRInt32 begin,end;
    PRBool done;
    nsresult result;
    nsAutoString str;

    // find out where we are
    result = SetupDoc(&selOffset);
    if(NS_FAILED(result))
      return result;
    result = GetCurrentBlockIndex(mTsDoc,&startBlock);
    if(NS_FAILED(result))
      return result;

    //start at the beginning
    result = mTsDoc->FirstBlock();
    currOffset=0;
    currentBlock = 0;
    while( NS_SUCCEEDED(mTsDoc->IsDone(&done)) && !done )
      {
        result = mTsDoc->GetCurrentTextBlock(&str);
        do{
          result = mConverter->FindNextWord(str.get(),str.Length(),currOffset,&begin,&end);
          if(NS_SUCCEEDED(result)&&(begin != -1)){
            if(aOldWord->Equals(Substring(str,begin,end-begin))){
              // if we are before the current selection point but in the same block
              // move the selection point forwards
              if((currentBlock == startBlock)&&(begin < (PRInt32) selOffset)){
                selOffset += (aNewWord->Length()-aOldWord->Length());
                if(selOffset < 0) selOffset=0;
              }
              mTsDoc->SetSelection(begin, end-begin);
              mTsDoc->InsertText(aNewWord);
              mTsDoc->GetCurrentTextBlock(&str);
              end += (aNewWord->Length()-aOldWord->Length());  // recursion was cute in GEB, not here.
            }
          }
          currOffset = end;
        }while(currOffset != -1);
        mTsDoc->NextBlock();
        currentBlock++;
        currOffset=0;          
      }

    // We are done replacing.  Put the selection point back where we found  it (or equivalent);
    result = mTsDoc->FirstBlock();
    currentBlock = 0;
    while(( NS_SUCCEEDED(mTsDoc->IsDone(&done)) && !done ) &&(currentBlock < startBlock)){
      mTsDoc->NextBlock();
    }
    if( NS_SUCCEEDED(mTsDoc->IsDone(&done)) && !done ){
      nsString str;                                // regenerate the offset table 
      result = mTsDoc->GetCurrentTextBlock(&str);  // this seems necessary.  I'm not 100% sure why
      mTsDoc->SetSelection(selOffset,0);
    }
  }
  else{
    mTsDoc->InsertText(aNewWord);
  }
  return NS_OK;
}

NS_IMETHODIMP 
mozSpellChecker::IgnoreAll(const nsString *aWord)
{
  if(mPersonalDictionary){
    mPersonalDictionary->IgnoreWord(aWord->get());
  }
  return NS_OK;
}

NS_IMETHODIMP 
mozSpellChecker::AddWordToPersonalDictionary(const nsString *aWord)
{
  nsresult res;
  PRUnichar empty=0;
  if(!aWord||!mPersonalDictionary)
    return NS_ERROR_NULL_POINTER;
  res = mPersonalDictionary->AddWord(aWord->get(),&empty);
  return res;
}

NS_IMETHODIMP 
mozSpellChecker::RemoveWordFromPersonalDictionary(const nsString *aWord)
{
  nsresult res;
  PRUnichar empty=0;
  if(!aWord||!mPersonalDictionary)
    return NS_ERROR_NULL_POINTER;
  res = mPersonalDictionary->RemoveWord(aWord->get(),&empty);
  return res;
}

NS_IMETHODIMP 
mozSpellChecker::GetPersonalDictionary(nsStringArray *aWordList)
{
  nsAutoString temp;
  PRUint32 count,i;
  PRUnichar **words;
  
  if(!aWordList || !mPersonalDictionary)
    return NS_ERROR_NULL_POINTER;
  mPersonalDictionary->GetWordList(&words,&count);
  for(i=0;i<count;i++){
    temp.Assign(words[i]);
    aWordList->AppendString(temp);
  }
  NS_FREE_XPCOM_ALLOCATED_POINTER_ARRAY(count, words);

  return NS_OK;
}

NS_IMETHODIMP 
mozSpellChecker::GetDictionaryList(nsStringArray *aDictionaryList)
{
  nsAutoString temp;
  PRUint32 count,i;
  PRUnichar **words;
  
  if(!aDictionaryList || !mSpellCheckingEngine)
    return NS_ERROR_NULL_POINTER;
  mSpellCheckingEngine->GetDictionaryList(&words,&count);
  for(i=0;i<count;i++){
    temp.Assign(words[i]);
    aDictionaryList->AppendString(temp);
  }
  NS_FREE_XPCOM_ALLOCATED_POINTER_ARRAY(count, words);

  return NS_OK;
}

NS_IMETHODIMP 
mozSpellChecker::GetCurrentDictionary(nsString *aDictionary)
{
  NS_ENSURE_ARG_POINTER(aDictionary);
  nsXPIDLString dictname;
  nsresult res;
  res=mSpellCheckingEngine->GetDictionary(getter_Copies(dictname));
  if(NS_SUCCEEDED(res))
    *aDictionary = dictname;
  return NS_OK;
}

NS_IMETHODIMP 
mozSpellChecker::SetCurrentDictionary(const nsString *aDictionary)
{
  NS_ENSURE_ARG_POINTER(aDictionary);

  if(!mSpellCheckingEngine)
    return NS_ERROR_NULL_POINTER;
 
  nsresult res;
  res = mSpellCheckingEngine->SetDictionary(aDictionary->get());
  if(NS_FAILED(res)){
    NS_WARNING("Dictionary load failed");
    return res;
  }
  nsXPIDLString language;
  
  nsCOMPtr<mozISpellI18NManager> serv(do_GetService("@mozilla.org/spellchecker/i18nmanager;1", &res));
  if(serv && NS_SUCCEEDED(res)){
    res = serv->GetUtil(language.get(),getter_AddRefs(mConverter));
  }
  return res;
}

nsresult
mozSpellChecker::SetupDoc(PRUint32 *outBlockOffset)
{
  nsresult  rv;

  nsITextServicesDocument::TSDBlockSelectionStatus blockStatus;
  PRInt32 selOffset;
  PRInt32 selLength;
  *outBlockOffset = 0;

  if (!mFromStart) 
  {
    rv = mTsDoc->LastSelectedBlock(&blockStatus, &selOffset, &selLength);
    if (NS_SUCCEEDED(rv) && (blockStatus != nsITextServicesDocument::eBlockNotFound))
    {
      switch (blockStatus)
      {
        case nsITextServicesDocument::eBlockOutside:  // No TB in S, but found one before/after S.
        case nsITextServicesDocument::eBlockPartial:  // S begins or ends in TB but extends outside of TB.
          // the TS doc points to the block we want.
          *outBlockOffset = selOffset + selLength;
          break;
                    
        case nsITextServicesDocument::eBlockInside:  // S extends beyond the start and end of TB.
          // we want the block after this one.
          rv = mTsDoc->NextBlock();
          *outBlockOffset = 0;
          break;
                
        case nsITextServicesDocument::eBlockContains: // TB contains entire S.
          *outBlockOffset = selOffset + selLength;
          break;
        
        case nsITextServicesDocument::eBlockNotFound: // There is no text block (TB) in or before the selection (S).
        default:
          NS_NOTREACHED("Shouldn't ever get this status");
      }
    }
    else  //failed to get last sel block. Just start at beginning
    {
      rv = mTsDoc->FirstBlock();
      *outBlockOffset = 0;
    }
  
  }
  else // we want the first block
  {
    rv = mTsDoc->FirstBlock();
    mFromStart = PR_FALSE;
  }
  return rv;
}


// utility method to discover which block we're in. The TSDoc interface doesn't give
// us this, because it can't assume a read-only document.
// shamelessly stolen from nsTextServicesDocument
nsresult
mozSpellChecker::GetCurrentBlockIndex(nsITextServicesDocument *aDoc, PRInt32 *outBlockIndex)
{
  PRInt32  blockIndex = 0;
  PRBool   isDone = PR_FALSE;
  nsresult result = NS_OK;

  do
  {
    aDoc->PrevBlock();

    result = aDoc->IsDone(&isDone);

    if (!isDone)
      blockIndex ++;

  } while (NS_SUCCEEDED(result) && !isDone);
  
  *outBlockIndex = blockIndex;

  return result;
}
