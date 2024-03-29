/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* ***** BEGIN LICENSE BLOCK *****
 * Version: NPL 1.1/GPL 2.0/LGPL 2.1
 *
 * The contents of this file are subject to the Netscape Public License
 * Version 1.1 (the "License"); you may not use this file except in
 * compliance with the License. You may obtain a copy of the License at
 * http://www.mozilla.org/NPL/
 *
 * Software distributed under the License is distributed on an "AS IS" basis,
 * WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
 * for the specific language governing rights and limitations under the
 * License.
 *
 * The Original Code is mozilla.org code.
 *
 * The Initial Developer of the Original Code is 
 * Netscape Communications Corporation.
 * Portions created by the Initial Developer are Copyright (C) 1998
 * the Initial Developer. All Rights Reserved.
 *
 * Contributor(s):
 *   Pierre Phaneuf <pp@ludusdesign.com>
 *
 * Alternatively, the contents of this file may be used under the terms of
 * either the GNU General Public License Version 2 or later (the "GPL"), or 
 * the GNU Lesser General Public License Version 2.1 or later (the "LGPL"),
 * in which case the provisions of the GPL or the LGPL are applicable instead
 * of those above. If you wish to allow use of your version of this file only
 * under the terms of either the GPL or the LGPL, and not to allow others to
 * use your version of this file under the terms of the NPL, indicate your
 * decision by deleting the provisions above and replace them with the notice
 * and other provisions required by the GPL or the LGPL. If you do not delete
 * the provisions above, a recipient may use your version of this file under
 * the terms of any one of the NPL, the GPL or the LGPL.
 *
 * ***** END LICENSE BLOCK ***** */

#include "nsID.h"
#include "nsCRT.h"
#include "nsReadableUtils.h"
#include "nsIInputStream.h"
#include "nsIUnicharInputStream.h"
#include "pratom.h"
#include "nsEnumeratorUtils.h"
#include "nsReadableUtils.h"
#include "nsPrintfCString.h"

#define PL_ARENA_CONST_ALIGN_MASK 3
#include "nsPersistentProperties.h"
#include "nsIProperties.h"
#include "nsProperties.h"

struct propertyTableEntry : public PLDHashEntryHdr
{
    // both of these are arena-allocated
    const char *mKey;
    const PRUnichar *mValue;
};

static PRUnichar*
ArenaStrdup(const nsAFlatString& aString, PLArenaPool* aArena)
{
    void *mem;
    // add one to include the null terminator
    PRInt32 len = (aString.Length()+1) * sizeof(PRUnichar);
    PL_ARENA_ALLOCATE(mem, aArena, len);
    NS_ASSERTION(mem, "Couldn't allocate space!\n");
    if (mem) {
        memcpy(mem, aString.get(), len);
    }
    return NS_STATIC_CAST(PRUnichar*, mem);
}

static char*
ArenaStrdup(const nsAFlatCString& aString, PLArenaPool* aArena)
{
    void *mem;
    // add one to include the null terminator
    PRInt32 len = (aString.Length()+1) * sizeof(char);
    PL_ARENA_ALLOCATE(mem, aArena, len);
    NS_ASSERTION(mem, "Couldn't allocate space!\n");
    if (mem)
        memcpy(mem, aString.get(), len);
    return NS_STATIC_CAST(char*, mem);
}

PR_STATIC_CALLBACK(PRBool)
matchPropertyKeys(PLDHashTable*, const PLDHashEntryHdr* aHdr,
                  const void *key)
{
  const propertyTableEntry* entry =
    NS_STATIC_CAST(const propertyTableEntry*,aHdr);
  const char *keyValue = NS_STATIC_CAST(const char*,key);
    
  return (strcmp(entry->mKey, keyValue)==0);
}

PR_STATIC_CALLBACK(const void*)
getPropertyKey(PLDHashTable*, PLDHashEntryHdr* aHdr)
{
  propertyTableEntry* entry =
    NS_STATIC_CAST(propertyTableEntry*, aHdr);

  return entry->mKey;
}

struct PLDHashTableOps property_HashTableOps = {
    PL_DHashAllocTable,
    PL_DHashFreeTable,
    getPropertyKey,
    PL_DHashStringKey,
    matchPropertyKeys,
    PL_DHashMoveEntryStub,
    PL_DHashClearEntryStub,
    PL_DHashFinalizeStub,
    nsnull,
};

nsPersistentProperties::nsPersistentProperties()
{
  mIn = nsnull;
  mSubclass = NS_STATIC_CAST(nsIPersistentProperties*, this);
  PL_DHashTableInit(&mTable, &property_HashTableOps, nsnull,
                    sizeof(propertyTableEntry), 20);

  PL_INIT_ARENA_POOL(&mArena, "PersistentPropertyArena", 2048);
  
}

nsPersistentProperties::~nsPersistentProperties()
{
    PL_FinishArenaPool(&mArena);
    PL_DHashTableFinish(&mTable);
}

NS_METHOD
nsPersistentProperties::Create(nsISupports *aOuter, REFNSIID aIID, void **aResult)
{
    if (aOuter)
        return NS_ERROR_NO_AGGREGATION;
    nsPersistentProperties* props = new nsPersistentProperties();
    if (props == nsnull)
        return NS_ERROR_OUT_OF_MEMORY;
    NS_ADDREF(props);
    nsresult rv = props->QueryInterface(aIID, aResult);
    NS_RELEASE(props);
    return rv;
}

NS_IMPL_THREADSAFE_ISUPPORTS2(nsPersistentProperties, nsIPersistentProperties, nsIProperties)

NS_IMETHODIMP
nsPersistentProperties::Load(nsIInputStream *aIn)
{
  PRInt32  c;
  nsresult ret = NS_NewUTF8ConverterStream(&mIn, aIn, 0);
  
  if (ret != NS_OK) {
    NS_WARNING("NS_NewUTF8ConverterStream failed");
    return NS_ERROR_FAILURE;
  }
  c = Read();
  while (1) {
    c = SkipWhiteSpace(c);
    if (c < 0) {
      break;
    }
    else if ((c == '#') || (c == '!')) {
      c = SkipLine(c);
      continue;
    }
    else {
      nsAutoString key;
      while ((c >= 0) && (c != '=') && (c != ':')) {
        key.Append(PRUnichar(c));
        c = Read();
      }
      if (c < 0) {
        break;
      }
      static const char trimThese[] = " \t";
      key.Trim(trimThese, PR_FALSE, PR_TRUE);
      c = Read();
      nsAutoString value;
      PRUint32 state  = 0;
      PRUnichar uchar = 0;
      while ((c >= 0) && (c != '\r') && (c != '\n')) {
        switch(state) {
          case 0:
           if (c == '\\') {
             c = Read();
             switch(c) {
               case '\r':
               case '\n':
                 c = SkipWhiteSpace(c);
                 value.Append((PRUnichar) c);
                 break;
               case 'u':
               case 'U':
                 state = 1;
                 uchar=0;
                 break;
               case 't':
                 value.Append(PRUnichar('\t'));
                 break;
               case 'n':
                 value.Append(PRUnichar('\n'));
                 break;
               case 'r':
                 value.Append(PRUnichar('\r'));
                 break;
               default:
                 value.Append((PRUnichar) c);
             } // switch(c)
           } else {
             value.Append((PRUnichar) c);
           }
           c = Read();
           break;
         case 1:
         case 2:
         case 3:
         case 4:
           if(('0' <= c) && (c <= '9')) {
              uchar = (uchar << 4) | (c - '0');
              state++;
              c = Read();
           } else if(('a' <= c) && (c <= 'f')) {
              uchar = (uchar << 4) | (c - 'a' + 0x0a);
              state++;
              c = Read();
           } else if(('A' <= c) && (c <= 'F')) {
              uchar = (uchar << 4) | (c - 'A' + 0x0a);
              state++;
              c = Read();
           } else {
             value.Append((PRUnichar) uchar);
             state = 0;
           }
           break;
         case 5:
           value.Append((PRUnichar) uchar);
           state = 0;
        }
      }
      if(state != 0) {
        value.Append((PRUnichar) uchar);
        state = 0;
      }

      value.Trim(trimThese, PR_TRUE, PR_TRUE);
      nsAutoString oldValue;
      mSubclass->SetStringProperty(NS_ConvertUCS2toUTF8(key), value, oldValue);
    }
  }
  mIn->Close();
  NS_RELEASE(mIn);

  return NS_OK;
}

NS_IMETHODIMP
nsPersistentProperties::SetStringProperty(const nsACString& aKey,
                                          const nsAString& aNewValue,
                                          nsAString& aOldValue)
{
#if 0
  cout << "will add " << aKey.get() << "=" <<
    NS_LossyConvertUCS2ToASCII(aNewValue).get() << endl;
#endif

  const nsAFlatCString&  flatKey = PromiseFlatCString(aKey);
  propertyTableEntry *entry =
      NS_STATIC_CAST(propertyTableEntry*,
                     PL_DHashTableOperate(&mTable, flatKey.get(), PL_DHASH_ADD));

  if (entry->mKey) {
      aOldValue = entry->mValue;
      NS_WARNING(nsPrintfCString(aKey.Length() + 30,
                                 "the property %s already exists\n",
                                 flatKey.get()).get());
  }

  entry->mKey = ArenaStrdup(flatKey, &mArena);
  entry->mValue = ArenaStrdup(PromiseFlatString(aNewValue), &mArena);
  
  return NS_OK;
}

NS_IMETHODIMP
nsPersistentProperties::Save(nsIOutputStream* aOut, const nsACString& aHeader)
{
  return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
nsPersistentProperties::Subclass(nsIPersistentProperties* aSubclass)
{
  if (aSubclass) {
    mSubclass = aSubclass;
  }

  return NS_OK;
}

NS_IMETHODIMP
nsPersistentProperties::GetStringProperty(const nsACString& aKey,
                                          nsAString& aValue)
{
  const nsAFlatCString&  flatKey = PromiseFlatCString(aKey);

  propertyTableEntry *entry =
    NS_STATIC_CAST(propertyTableEntry*,
                   PL_DHashTableOperate(&mTable, flatKey.get(), PL_DHASH_LOOKUP));

  if (PL_DHASH_ENTRY_IS_FREE(entry))
    return NS_ERROR_FAILURE;

  aValue = entry->mValue;
  return NS_OK;
}

PR_STATIC_CALLBACK(PLDHashOperator)
AddElemToArray(PLDHashTable* table, PLDHashEntryHdr *hdr,
               PRUint32 i, void *arg)
{
  nsISupportsArray  *propArray = (nsISupportsArray *) arg;
  propertyTableEntry* entry =
      NS_STATIC_CAST(propertyTableEntry*, hdr);
  
  nsPropertyElement *element =
    new nsPropertyElement(nsDependentCString(entry->mKey),
                          nsDependentString(entry->mValue));
  if (!element)
     return PL_DHASH_STOP;

  NS_ADDREF(element);
  propArray->InsertElementAt(element, i);

  return PL_DHASH_NEXT;
}


NS_IMETHODIMP
nsPersistentProperties::Enumerate(nsISimpleEnumerator** aResult)
{
  nsCOMPtr<nsIBidirectionalEnumerator> iterator;

  nsISupportsArray* propArray;
  nsresult rv = NS_NewISupportsArray(&propArray);
  if (rv != NS_OK)
    return rv;

  // Step through hash entries populating a transient array
  PRUint32 n =
      PL_DHashTableEnumerate(&mTable, AddElemToArray, (void *)propArray);
  if ( n < (PRIntn) mTable.entryCount )
      return NS_ERROR_OUT_OF_MEMORY;

  return NS_NewArrayEnumerator(aResult, propArray); 
}


PRInt32
nsPersistentProperties::Read()
{
  PRUnichar  c;
  PRUint32  nRead;
  nsresult  ret;

  ret = mIn->Read(&c, 0, 1, &nRead);
  if (ret == NS_OK && nRead == 1) {
    return c;
  }

  return -1;
}

#define IS_WHITE_SPACE(c) \
  (((c) == ' ') || ((c) == '\t') || ((c) == '\r') || ((c) == '\n'))

PRInt32
nsPersistentProperties::SkipWhiteSpace(PRInt32 c)
{
  while ((c >= 0) && IS_WHITE_SPACE(c)) {
    c = Read();
  }

  return c;
}

PRInt32
nsPersistentProperties::SkipLine(PRInt32 c)
{
  while ((c >= 0) && (c != '\r') && (c != '\n')) {
    c = Read();
  }
  if (c == '\r') {
    c = Read();
  }
  if (c == '\n') {
    c = Read();
  }

  return c;
}

////////////////////////////////////////////////////////////////////////////////
// XXX Some day we'll unify the nsIPersistentProperties interface with 
// nsIProperties, but until now...

NS_IMETHODIMP 
nsPersistentProperties::Get(const char* prop, const nsIID & uuid, void* *result)
{
  return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP 
nsPersistentProperties::Set(const char* prop, nsISupports* value)
{
  return NS_ERROR_NOT_IMPLEMENTED;
}
NS_IMETHODIMP 
nsPersistentProperties::Undefine(const char* prop)
{
  return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP 
nsPersistentProperties::Has(const char* prop, PRBool *result)
{
  return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP 
nsPersistentProperties::GetKeys(PRUint32 *count, char ***keys)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

////////////////////////////////////////////////////////////////////////////////
// PropertyElement
////////////////////////////////////////////////////////////////////////////////


NS_METHOD
nsPropertyElement::Create(nsISupports *aOuter, REFNSIID aIID, void **aResult)
{
    if (aOuter)
        return NS_ERROR_NO_AGGREGATION;
    nsPropertyElement* propElem = new nsPropertyElement();
    if (propElem == nsnull)
        return NS_ERROR_OUT_OF_MEMORY;
    NS_ADDREF(propElem);
    nsresult rv = propElem->QueryInterface(aIID, aResult);
    NS_RELEASE(propElem);
    return rv;
}

NS_IMPL_ISUPPORTS1(nsPropertyElement, nsIPropertyElement)

NS_IMETHODIMP
nsPropertyElement::GetKey(nsACString& aReturnKey)
{
  aReturnKey = mKey;
  return NS_OK;
}

NS_IMETHODIMP
nsPropertyElement::GetValue(nsAString& aReturnValue)
{
  aReturnValue = mValue;
  return NS_OK;
}

NS_IMETHODIMP
nsPropertyElement::SetKey(const nsACString& aKey)
{
  mKey = aKey;
  return NS_OK;
}

NS_IMETHODIMP
nsPropertyElement::SetValue(const nsAString& aValue)
{
  mValue = aValue;
  return NS_OK;
}

////////////////////////////////////////////////////////////////////////////////

