/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*-
 *
 * The contents of this file are subject to the Mozilla Public
 * License Version 1.1 (the "License"); you may not use this file
 * except in compliance with the License. You may obtain a copy of
 * the License at http://www.mozilla.org/MPL/
 * 
 * Software distributed under the License is distributed on an "AS
 * IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or
 * implied. See the License for the specific language governing
 * rights and limitations under the License.
 * 
 * The Original Code is Mozilla Communicator.
 * 
 * The Initial Developer of the Original Code is Netscape Communications
 * Corp.  Portions created by Netscape are Copyright (C) 1999 Netscape 
 * Communications Corp.  All Rights Reserved.
 * 
 * Contributor(s): 
 *   Mike Pinkerton
 */


//
// Part of the reason these routines are all in once place is so that as new
// data flavors are added that are known to be one-byte or two-byte strings, or even
// raw binary data, then we just have to go to one place to change how the data
// moves into/out of the primitives and native line endings.
//
// If you add new flavors that have special consideration (binary data or one-byte
// char* strings), please update all the helper classes in this file.
//
// For now, this is the assumption that we are making:
//  - text/plain is always a char*
//  - anything else is a PRUnichar*
//


#include "nsPrimitiveHelpers.h"
#include "nsCOMPtr.h"
#include "nsXPCOM.h"
#include "nsISupportsPrimitives.h"
#include "nsITransferable.h"
#include "nsIComponentManager.h"
#include "nsLinebreakConverter.h"
#include "nsReadableUtils.h"

#include "nsIServiceManager.h"
#include "nsICharsetConverterManager.h"
// unicode conversion
#  include "nsIPlatformCharset.h"
#include "nsISaveAsCharset.h"


//
// CreatePrimitiveForData
//
// Given some data and the flavor it corresponds to, creates the appropriate
// nsISupports* wrapper for passing across IDL boundaries. Right now, everything
// creates a two-byte |nsISupportsString|, except for "text/plain" and native
// platform HTML (CF_HTML on win32)
//
void
nsPrimitiveHelpers :: CreatePrimitiveForData ( const char* aFlavor, void* aDataBuff, 
                                                 PRUint32 aDataLen, nsISupports** aPrimitive )
{
  if ( !aPrimitive )
    return;

  if ( strcmp(aFlavor,kTextMime) == 0 || strcmp(aFlavor,kNativeHTMLMime) == 0 ) {
    nsCOMPtr<nsISupportsCString> primitive;
    nsComponentManager::CreateInstance(NS_SUPPORTS_CSTRING_CONTRACTID, nsnull, 
                                       NS_GET_IID(nsISupportsCString), getter_AddRefs(primitive));
    if ( primitive ) {
      const char * start = (const char*)aDataBuff;
      primitive->SetData(Substring(start, start + aDataLen));
      nsCOMPtr<nsISupports> genericPrimitive ( do_QueryInterface(primitive) );
      *aPrimitive = genericPrimitive;
      NS_ADDREF(*aPrimitive);
    }
  }
  else {
    nsCOMPtr<nsISupportsString> primitive;
    nsresult rv = nsComponentManager::CreateInstance(NS_SUPPORTS_STRING_CONTRACTID, nsnull, 
                                                      NS_GET_IID(nsISupportsString), getter_AddRefs(primitive));
    if (NS_SUCCEEDED(rv) && primitive ) {
      // recall that length takes length as characters, not bytes
      const PRUnichar* start = (const PRUnichar*)aDataBuff;
      primitive->SetData(Substring(start, start + (aDataLen / 2)));
      nsCOMPtr<nsISupports> genericPrimitive ( do_QueryInterface(primitive) );
      *aPrimitive = genericPrimitive;
      NS_ADDREF(*aPrimitive);
    }  
  }

} // CreatePrimitiveForData


//
// CreateDataFromPrimitive
//
// Given a nsISupports* primitive and the flavor it represents, creates a new data
// buffer with the data in it. This data will be null terminated, but the length
// parameter does not reflect that.
//
void
nsPrimitiveHelpers :: CreateDataFromPrimitive ( const char* aFlavor, nsISupports* aPrimitive, 
                                                   void** aDataBuff, PRUint32 aDataLen )
{
  if ( !aDataBuff )
    return;

  if ( strcmp(aFlavor,kTextMime) == 0 ) {
    nsCOMPtr<nsISupportsCString> plainText ( do_QueryInterface(aPrimitive) );
    if ( plainText ) {
      nsCAutoString data;
      plainText->GetData ( data );
      *aDataBuff = ToNewCString(data);
    }
  }
  else {
    nsCOMPtr<nsISupportsString> doubleByteText ( do_QueryInterface(aPrimitive) );
    if ( doubleByteText ) {
      nsAutoString data;
      doubleByteText->GetData ( data );
      *aDataBuff = ToNewUnicode(data);
    }
  }

}


//
// ConvertUnicodeToPlatformPlainText
//
// Given a unicode buffer (flavor text/unicode), this converts it to plain text using
// the appropriate platform charset encoding. |inUnicodeLen| is the length of the input
// string, not the # of bytes in the buffer. The |outPlainTextData| is null terminated, 
// but its length parameter, |outPlainTextLen|, does not reflect that.
//
nsresult
nsPrimitiveHelpers :: ConvertUnicodeToPlatformPlainText ( PRUnichar* inUnicode, PRInt32 inUnicodeLen, 
                                                            char** outPlainTextData, PRInt32* outPlainTextLen )
{
  if ( !outPlainTextData || !outPlainTextLen )
    return NS_ERROR_INVALID_ARG;

  // Get the appropriate unicode encoder. We're guaranteed that this won't change
  // through the life of the app so we can cache it.
  nsresult rv;
  nsCOMPtr<nsIUnicodeEncoder> encoder;

  // get the charset
  nsAutoString platformCharset;
  nsCOMPtr <nsIPlatformCharset> platformCharsetService = do_GetService(NS_PLATFORMCHARSET_CONTRACTID, &rv);
  if (NS_SUCCEEDED(rv))
    rv = platformCharsetService->GetCharset(kPlatformCharsetSel_PlainTextInClipboard, platformCharset);
  if (NS_FAILED(rv))
    platformCharset.Assign(NS_LITERAL_STRING("ISO-8859-1"));
  

  // use transliterate to convert things like smart quotes to normal quotes for plain text
  nsCAutoString cPlatformCharset;
  cPlatformCharset.AssignWithConversion(platformCharset);

  nsCOMPtr<nsISaveAsCharset> converter = do_CreateInstance("@mozilla.org/intl/saveascharset;1");
  rv = converter->Init(cPlatformCharset.get(),
                  nsISaveAsCharset::attr_EntityAfterCharsetConv +
                  nsISaveAsCharset::attr_FallbackQuestionMark,
                  nsIEntityConverter::transliterate);
  NS_ENSURE_SUCCESS(rv, rv);

  rv = converter->Convert(inUnicode, outPlainTextData);
  *outPlainTextLen = *outPlainTextData ? strlen(*outPlainTextData) : 0;

  NS_ASSERTION ( NS_SUCCEEDED(rv), "Error converting unicode to plain text" );
  
  return rv;
} // ConvertUnicodeToPlatformPlainText


//
// ConvertPlatformPlainTextToUnicode
//
// Given a char buffer (flavor text/plaikn), this converts it to unicode using
// the appropriate platform charset encoding. |outUnicode| is null terminated, 
// but its length parameter, |outUnicodeLen|, does not reflect that. |outUnicodeLen| is
// the length of the string in characters, not bytes.
//
nsresult
nsPrimitiveHelpers :: ConvertPlatformPlainTextToUnicode ( const char* inText, PRInt32 inTextLen, 
                                                            PRUnichar** outUnicode, PRInt32* outUnicodeLen )
{
  if ( !outUnicode || !outUnicodeLen )
    return NS_ERROR_INVALID_ARG;

  // Get the appropriate unicode decoder. We're guaranteed that this won't change
  // through the life of the app so we can cache it.
  nsresult rv;
  static nsCOMPtr<nsIUnicodeDecoder> decoder;
  static PRBool hasConverter = PR_FALSE;
  if ( !hasConverter ) {
    // get the charset
    nsAutoString platformCharset;
    nsCOMPtr <nsIPlatformCharset> platformCharsetService = do_GetService(NS_PLATFORMCHARSET_CONTRACTID, &rv);
    if (NS_SUCCEEDED(rv))
      rv = platformCharsetService->GetCharset(kPlatformCharsetSel_PlainTextInClipboard, platformCharset);
    if (NS_FAILED(rv))
      platformCharset.Assign(NS_LITERAL_STRING("ISO-8859-1"));
      
    // get the decoder
    nsCOMPtr<nsICharsetConverterManager> ccm = 
             do_GetService(NS_CHARSETCONVERTERMANAGER_CONTRACTID, &rv);  
    rv = ccm->GetUnicodeDecoder(&platformCharset, getter_AddRefs(decoder));

    hasConverter = PR_TRUE;
  }
  
  // Estimate out length and allocate the buffer based on a worst-case estimate, then do
  // the conversion. 
  decoder->GetMaxLength(inText, inTextLen, outUnicodeLen);   // |outUnicodeLen| is number of chars
  if ( *outUnicodeLen ) {
    *outUnicode = NS_REINTERPRET_CAST(PRUnichar*, nsMemory::Alloc((*outUnicodeLen + 1) * sizeof(PRUnichar)));
    if ( *outUnicode ) {
      rv = decoder->Convert(inText, &inTextLen, *outUnicode, outUnicodeLen);
      (*outUnicode)[*outUnicodeLen] = '\0';                   // null terminate. Convert() doesn't do it for us
    }
  } // if valid length

  NS_ASSERTION ( NS_SUCCEEDED(rv), "Error converting plain text to unicode" );

  return rv;
} // ConvertPlatformPlainTextToUnicode


#ifdef XP_MAC
#pragma mark -
#endif


//
// ConvertPlatformToDOMLinebreaks
//
// Given some data, convert from the platform linebreaks into the LF expected by the
// DOM. This will attempt to convert the data in place, but the buffer may still need to
// be reallocated regardless (disposing the old buffer is taken care of internally, see
// the note below).
//
// NOTE: this assumes that it can use nsMemory to dispose of the old buffer.
//
nsresult
nsLinebreakHelpers :: ConvertPlatformToDOMLinebreaks ( const char* inFlavor, void** ioData, 
                                                          PRInt32* ioLengthInBytes )
{
  NS_ASSERTION ( ioData && *ioData && ioLengthInBytes, "Bad Params");
  if ( !(ioData && *ioData && ioLengthInBytes) )
    return NS_ERROR_INVALID_ARG;
    
  nsresult retVal = NS_OK;
  
  if ( strcmp(inFlavor, "text/plain") == 0 ) {
    char* buffAsChars = NS_REINTERPRET_CAST(char*, *ioData);
    char* oldBuffer = buffAsChars;
    retVal = nsLinebreakConverter::ConvertLineBreaksInSitu ( &buffAsChars, nsLinebreakConverter::eLinebreakAny, 
                                                              nsLinebreakConverter::eLinebreakContent, 
                                                              *ioLengthInBytes, ioLengthInBytes );
    if ( NS_SUCCEEDED(retVal) ) {
      if ( buffAsChars != oldBuffer )             // check if buffer was reallocated
        nsMemory::Free ( oldBuffer );
      *ioData = buffAsChars;
    }
  }
  else if ( strcmp(inFlavor, "image/jpeg") == 0 ) {
    // I'd assume we don't want to do anything for binary data....
  }
  else {       
    PRUnichar* buffAsUnichar = NS_REINTERPRET_CAST(PRUnichar*, *ioData);
    PRUnichar* oldBuffer = buffAsUnichar;
    PRInt32 newLengthInChars;
    retVal = nsLinebreakConverter::ConvertUnicharLineBreaksInSitu ( &buffAsUnichar, nsLinebreakConverter::eLinebreakAny, 
                                                                     nsLinebreakConverter::eLinebreakContent, 
                                                                     *ioLengthInBytes / sizeof(PRUnichar), &newLengthInChars );
    if ( NS_SUCCEEDED(retVal) ) {
      if ( buffAsUnichar != oldBuffer )           // check if buffer was reallocated
        nsMemory::Free ( oldBuffer );
      *ioData = buffAsUnichar;
      *ioLengthInBytes = newLengthInChars * sizeof(PRUnichar);
    }
  }
  
  return retVal;

} // ConvertPlatformToDOMLinebreaks
