/* -*- Mode: C; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
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

#include "nsConverterInputStream.h"
#include "nsICharsetConverterManager.h"
#include "nsIServiceManager.h"

#define CONVERTER_BUFFER_SIZE 8192

NS_IMPL_ISUPPORTS1(nsConverterInputStream, nsIConverterInputStream)
    
static NS_DEFINE_CID(kCharsetConverterManagerCID, NS_ICHARSETCONVERTERMANAGER_CID);

NS_IMETHODIMP
nsConverterInputStream::Init(nsIInputStream* aStream,
                             const PRUnichar *aCharset,
                             PRInt32 aBufferSize,
                             PRBool aRecoverFromErrors)
{
    nsresult rv;

    if (aBufferSize <=0) aBufferSize=CONVERTER_BUFFER_SIZE;
    
    // get the decoder
    nsCOMPtr<nsICharsetConverterManager> ccm =
        do_GetService(kCharsetConverterManagerCID, &rv);
    if (NS_FAILED(rv)) return nsnull;

    nsAutoString charset;
    if (aCharset)
        charset.Assign(aCharset);
    else
        charset.Assign(NS_LITERAL_STRING("ISO-8859-1"));
    
    rv = ccm->GetUnicodeDecoder(&charset, getter_AddRefs(mConverter));
    if (NS_FAILED(rv)) return rv;
 
    // set up our buffers
    rv = NS_NewByteBuffer(getter_AddRefs(mByteData), nsnull, aBufferSize);
    if (NS_FAILED(rv)) return rv;

    rv = NS_NewUnicharBuffer(getter_AddRefs(mUnicharData), nsnull, aBufferSize);
    if (NS_FAILED(rv)) return rv;

    mInput = aStream;
    mRecoverFromErrors = aRecoverFromErrors;
    
    return NS_OK;
}

NS_IMETHODIMP
nsConverterInputStream::Close()
{
    mInput = nsnull;
    mConverter = nsnull;
    mByteData = nsnull;
    mUnicharData = nsnull;
    return NS_OK;
}

NS_IMETHODIMP
nsConverterInputStream::Read(PRUnichar* aBuf,
                             PRUint32 aOffset,
                             PRUint32 aCount,
                             PRUint32 *aReadCount)
{
  NS_ASSERTION(mUnicharDataLength >= mUnicharDataOffset, "unsigned madness");
  PRUint32 rv = mUnicharDataLength - mUnicharDataOffset;
  if (0 == rv) {
    // Fill the unichar buffer
    rv = Fill(&mLastErrorCode);
    if (rv == 0) {
      *aReadCount = 0;
      return mLastErrorCode;
    }
  }
  if (rv > aCount) {
    rv = aCount;
  }
  memcpy(aBuf + aOffset, mUnicharData->GetBuffer() + mUnicharDataOffset,
         rv * sizeof(PRUnichar));
  mUnicharDataOffset += rv;
  *aReadCount = rv;
  return NS_OK;
}

PRUint32
nsConverterInputStream::Fill(nsresult * aErrorCode)
{
  if (nsnull == mInput) {
    // We already closed the stream!
    *aErrorCode = NS_BASE_STREAM_CLOSED;
    return 0;
  }

  if (NS_FAILED(mLastErrorCode)) {
    // We failed to completely convert last time, and error-recovery
    // is disabled.  We will fare no better this time, so...
    *aErrorCode = mLastErrorCode;
    return 0;
  }
  
  // We assume a many to one conversion and are using equal sizes for
  // the two buffers.  However if an error happens at the very start
  // of a byte buffer we may end up in a situation where n bytes lead
  // to n+1 unicode chars.  Thus we need to keep track of the leftover
  // bytes as we convert.
  
  PRInt32 nb = mByteData->Fill(aErrorCode, mInput, mLeftOverBytes);
#if defined(DEBUG_bzbarsky) && 0
  for (unsigned int foo = 0; foo < mByteData->GetLength(); ++foo) {
    fprintf(stderr, "%c", mByteData->GetBuffer()[foo]);
  }
  fprintf(stderr, "\n");
#endif
  if (nb <= 0 && mLeftOverBytes == 0) {
    // No more data 
    *aErrorCode = NS_OK;
    return 0;
  }

  NS_ASSERTION(PRUint32(nb) + mLeftOverBytes == mByteData->GetLength(),
               "mByteData is lying to us somewhere");
  
  // Now convert as much of the byte buffer to unicode as possible
  mUnicharDataOffset = 0;
  mUnicharDataLength = 0;
  PRUint32 srcConsumed = 0;
  do {
    PRInt32 srcLen = mByteData->GetLength() - srcConsumed;
    PRInt32 dstLen = mUnicharData->GetBufferSize() - mUnicharDataLength;
    *aErrorCode = mConverter->Convert(mByteData->GetBuffer()+srcConsumed,
                                      &srcLen,
                                      mUnicharData->GetBuffer()+mUnicharDataLength,
                                      &dstLen);
    mUnicharDataLength += dstLen;
    // XXX if srcLen is negative, we want to drop the _first_ byte in
    // the erroneous byte sequence and try again.  This is not quite
    // possible right now -- see bug 160784
    srcConsumed += srcLen;
    if (NS_FAILED(*aErrorCode) && mRecoverFromErrors) {
      NS_ASSERTION(0 < mUnicharData->GetBufferSize() - mUnicharDataLength,
                   "Decoder returned an error but filled the output buffer! "
                   "Should not happen.");
      mUnicharData->GetBuffer()[mUnicharDataLength++] = (PRUnichar)0xFFFD;
      ++srcConsumed;
      // XXX this is needed to make sure we don't underrun our buffer;
      // bug 160784 again
      srcConsumed = PR_MAX(srcConsumed, 0);
      mConverter->Reset();
    }
    NS_ASSERTION(srcConsumed <= mByteData->GetLength(),
                 "Whoa.  The converter should have returned NS_OK_UDEC_MOREINPUT before this point!");
  } while (mRecoverFromErrors &&
           NS_FAILED(*aErrorCode));

  mLeftOverBytes = mByteData->GetLength() - srcConsumed;

  return mUnicharDataLength;
}
