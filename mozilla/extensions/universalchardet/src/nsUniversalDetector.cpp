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
 * The Original Code is Mozilla Communicator client code.
 *
 * The Initial Developer of the Original Code is 
 * Netscape Communications Corporation.
 * Portions created by the Initial Developer are Copyright (C) 1998
 * the Initial Developer. All Rights Reserved.
 *
 * Contributor(s):
 *
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

#include "nscore.h"
#include <stdio.h>

#include "nsUniversalDetector.h"

#include "nsUniversalCharDetDll.h"
//---- for XPCOM
#include "nsIFactory.h"
#include "nsISupports.h"
#include "pratom.h"
#include "prmem.h"
#include "nsCOMPtr.h"

#include "nsMBCSGroupProber.h"
#include "nsSBCSGroupProber.h"
#include "nsEscCharsetProber.h"
#include "nsLatin1Prober.h"

static NS_DEFINE_CID(kUniversalDetectorCID, NS_UNIVERSAL_DETECTOR_CID);
static NS_DEFINE_CID(kUniversalStringDetectorCID, NS_UNIVERSAL_STRING_DETECTOR_CID);

nsUniversalDetector::nsUniversalDetector()
{
  mDone = PR_FALSE;
  mBestGuess = -1;   //illegal value as signal
  mInTag = PR_FALSE;
  mEscCharSetProber = nsnull;

  mStart = PR_TRUE;
  mDetectedCharset = nsnull;
  mGotData = PR_FALSE;
  mInputState = ePureAscii;
  mLastChar = '\0';

  PRUint32 i;
  for (i = 0; i < NUM_OF_CHARSET_PROBERS; i++)
    mCharSetProbers[i] = nsnull;
}

nsUniversalDetector::~nsUniversalDetector() 
{
  for (PRInt32 i = 0; i < NUM_OF_CHARSET_PROBERS; i++)
    if (mCharSetProbers[i])      
      delete mCharSetProbers[i];
  if (mEscCharSetProber)
    delete mEscCharSetProber;
}

void 
nsUniversalDetector::Reset()
{
  mDone = PR_FALSE;
  mBestGuess = -1;   //illegal value as signal
  mInTag = PR_FALSE;

  mStart = PR_TRUE;
  mDetectedCharset = nsnull;
  mGotData = PR_FALSE;
  mInputState = ePureAscii;
  mLastChar = '\0';

  if (mEscCharSetProber)
    mEscCharSetProber->Reset();

  PRUint32 i;
  for (i = 0; i < NUM_OF_CHARSET_PROBERS; i++)
    if (mCharSetProbers[i])
      mCharSetProbers[i]->Reset();
}

//---------------------------------------------------------------------
#define SHORTCUT_THRESHOLD      (float)0.95
#define MINIMUM_THRESHOLD      (float)0.20

void nsUniversalDetector::HandleData(const char* aBuf, PRUint32 aLen)
{
  if(mDone) 
    return;

  if (aLen > 0)
    mGotData = PR_TRUE;

  //If the data starts with BOM, we know it is UTF
  if (mStart)
  {
    mStart = PR_FALSE;
    if (aLen > 3)
      switch (aBuf[0])
        {
        case 0xEF:
          if ((0xBB == aBuf[1]) && (0xBF == aBuf[2]))
            // EF BB BF  UTF-8 encoded BOM
            mDetectedCharset = "UTF-8";
        break;
        case 0xFE:
          if ((0xFF == aBuf[1]) && (0x00 == aBuf[2]) && (0x00 == aBuf[3]))
            // FE FF 00 00  UCS-4, unusual octet order BOM (3412)
            mDetectedCharset = "X-ISO-10646-UCS-4-3412";
          else if (0xFF == aBuf[1])
            // FE FF  UTF-16, big endian BOM
            mDetectedCharset = "UTF-16BE";
        break;
        case 0x00:
          if ((0x00 == aBuf[1]) && (0xFE == aBuf[2]) && (0xFF == aBuf[3]))
            // 00 00 FE FF  UTF-32, big-endian BOM
            mDetectedCharset = "UTF-32BE";
          else if ((0x00 == aBuf[1]) && (0xFF == aBuf[2]) && (0xFE == aBuf[3]))
            // 00 00 FF FE  UCS-4, unusual octet order BOM (2143)
            mDetectedCharset = "X-ISO-10646-UCS-4-2143";
        break;
        case 0xFF:
          if ((0xFE == aBuf[1]) && (0x00 == aBuf[2]) && (0x00 == aBuf[3]))
            // FF FE 00 00  UTF-32, little-endian BOM
            mDetectedCharset = "UTF-32LE";
          else if (0xFE == aBuf[1])
            // FF FE  UTF-16, little endian BOM
            mDetectedCharset = "UTF-16LE";
        break;
      }  // switch

      if (mDetectedCharset)
      {
        mDone = PR_TRUE;
        return;
      }
  }
  
  PRUint32 i;
  for (i = 0; i < aLen; i++)
  {
    //other than 0xa0, if every othe character is ascii, the page is ascii
    if (aBuf[i] & 0x80 && aBuf[i] != (char)0xA0)  //Since many Ascii only page contains NBSP 
    {
      //we got a non-ascii byte (high-byte)
      if (mInputState != eHighbyte)
      {
        //adjust state
        mInputState = eHighbyte;

        //kill mEscCharSetProber if it is active
        if (mEscCharSetProber) {
          delete mEscCharSetProber;
          mEscCharSetProber = nsnull;
        }

        //start multibyte and singlebyte charset prober
        if (nsnull == mCharSetProbers[0])
          mCharSetProbers[0] = new nsMBCSGroupProber;
        if (nsnull == mCharSetProbers[1])
          mCharSetProbers[1] = new nsSBCSGroupProber;
        if (nsnull == mCharSetProbers[2])
          mCharSetProbers[2] = new nsLatin1Prober; 
      }
    }
    else
    {
      //ok, just pure ascii so far
      if ( ePureAscii == mInputState &&
        (aBuf[i] == '\033' || (aBuf[i] == '{' && mLastChar == '~')) )
      {
        //found escape character or HZ "~{"
        mInputState = eEscAscii;
      }
      mLastChar = aBuf[i];
    }
  }

  nsProbingState st;
  switch (mInputState)
  {
  case eEscAscii:
    if (nsnull == mEscCharSetProber)
      mEscCharSetProber = new nsEscCharSetProber;
    st = mEscCharSetProber->HandleData(aBuf, aLen);
    if (st == eFoundIt)
    {
      mDone = PR_TRUE;
      mDetectedCharset = mEscCharSetProber->GetCharSetName();
    }
    break;
  case eHighbyte:
    for (i = 0; i < NUM_OF_CHARSET_PROBERS; i++)
    {
      st = mCharSetProbers[i]->HandleData(aBuf, aLen);
      if (st == eFoundIt) 
      {
        mDone = PR_TRUE;
        mDetectedCharset = mCharSetProbers[i]->GetCharSetName();
        return;
      } 
    }
    break;

  default:  //pure ascii
    ;//do nothing here
  }
  return ;  
}


//---------------------------------------------------------------------
void nsUniversalDetector::DataEnd()
{
  if (!mGotData)
  {
    // we haven't got any data yet, return immediately 
    // caller program sometimes call DataEnd before anything has been sent to detector
    return;
  }

  if (mDetectedCharset)
  {
    mDone = PR_TRUE;
    Report(mDetectedCharset);
    return;
  }
  
  switch (mInputState)
  {
  case eHighbyte:
    {
      float proberConfidence;
      float maxProberConfidence = (float)0.0;
      PRInt32 maxProber = 0;

      for (PRInt32 i = 0; i < NUM_OF_CHARSET_PROBERS; i++)
      {
        proberConfidence = mCharSetProbers[i]->GetConfidence();
#ifdef DEBUG_chardet
        mCharSetProbers[i]->DumpStatus();
#endif

        if (proberConfidence > maxProberConfidence)
        {
          maxProberConfidence = proberConfidence;
          maxProber = i;
        }
      }
      //do not report anything because we are not confident of it, that's in fact a negative answer
      if (maxProberConfidence > MINIMUM_THRESHOLD)
        Report(mCharSetProbers[maxProber]->GetCharSetName());
    }
    break;
  case eEscAscii:
    break;
  default:
    ;
  }
  return;
}

//---------------------------------------------------------------------
nsUniversalXPCOMDetector:: nsUniversalXPCOMDetector()
	     : nsUniversalDetector()
{
    mObserver = nsnull;
}
//---------------------------------------------------------------------
nsUniversalXPCOMDetector::~nsUniversalXPCOMDetector() 
{
    NS_IF_RELEASE(mObserver);
}
//---------------------------------------------------------------------

NS_IMPL_ISUPPORTS1(nsUniversalXPCOMDetector, nsICharsetDetector)

//---------------------------------------------------------------------
NS_IMETHODIMP nsUniversalXPCOMDetector::Init(
  nsICharsetDetectionObserver* aObserver)
{
  NS_ASSERTION(mObserver == nsnull , "Init twice");
  if(nsnull == aObserver)
     return NS_ERROR_ILLEGAL_VALUE;

  NS_IF_ADDREF(aObserver);
  mObserver = aObserver;
  return NS_OK;
}
//----------------------------------------------------------
NS_IMETHODIMP nsUniversalXPCOMDetector::DoIt(
  const char* aBuf, PRUint32 aLen, PRBool* oDontFeedMe)
{
  NS_ASSERTION(mObserver != nsnull , "have not init yet");

  if((nsnull == aBuf) || (nsnull == oDontFeedMe))
     return NS_ERROR_ILLEGAL_VALUE;

  this->HandleData(aBuf, aLen);
	
  if (mDone)
  {

    if (mDetectedCharset)
    {
	    Report(mDetectedCharset);
    }

	 *oDontFeedMe = PR_TRUE;
  }
  *oDontFeedMe = PR_FALSE;
  return NS_OK;
}
//----------------------------------------------------------
NS_IMETHODIMP nsUniversalXPCOMDetector::Done()
{
  NS_ASSERTION(mObserver != nsnull , "have not init yet");
  this->DataEnd();
  return NS_OK;
}
//----------------------------------------------------------
void nsUniversalXPCOMDetector::Report(const char* aCharset)
{
  NS_ASSERTION(mObserver != nsnull , "have not init yet");
#ifdef DEBUG_chardet
  printf("Universal Charset Detector report charset %s . \r\n", aCharset);
  for (PRInt32 i = 0; i < NUM_OF_CHARSET_PROBERS; i++)
    mCharSetProbers[i]->DumpStatus();
#endif
  mObserver->Notify(aCharset, eBestAnswer);
}


//---------------------------------------------------------------------
nsUniversalXPCOMStringDetector:: nsUniversalXPCOMStringDetector()
	     : nsUniversalDetector()
{
}
//---------------------------------------------------------------------
nsUniversalXPCOMStringDetector::~nsUniversalXPCOMStringDetector() 
{
}
//---------------------------------------------------------------------
NS_IMPL_ISUPPORTS1(nsUniversalXPCOMStringDetector, nsIStringCharsetDetector)
//---------------------------------------------------------------------
void nsUniversalXPCOMStringDetector::Report(const char *aCharset) 
{
   mResult = aCharset;
#ifdef DEBUG_chardet
       printf("New Charset Prober report charset %s . \r\n", aCharset);
#endif
}
//---------------------------------------------------------------------
NS_IMETHODIMP nsUniversalXPCOMStringDetector::DoIt(const char* aBuf, PRUint32 aLen, 
                     const char** oCharset, nsDetectionConfident &oConf)
{
   mResult = nsnull;
   this->Reset();
   this->HandleData(aBuf, aLen); 
   this->DataEnd();
   if (mResult)
   {
       *oCharset=mResult;
       oConf = eBestAnswer;
   }
   return NS_OK;
}
       

