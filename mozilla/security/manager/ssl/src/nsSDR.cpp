/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*-
 *
 * The contents of this file are subject to the Netscape Public
 * License Version 1.1 (the "License"); you may not use this file
 * except in compliance with the License. You may obtain a copy of
 * the License at http://www.mozilla.org/NPL/
 *
 * Software distributed under the License is distributed on an "AS
 * IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or
 * implied. See the License for the specific language governing
 * rights and limitations under the License.
 *
 * The Original Code is mozilla.org code.
 *
 * The Initial Developer of the Original Code is Netscape
 * Communications Corporation.  Portions created by Netscape are
 * Copyright (C) 2001 Netscape Communications Corporation. All
 * Rights Reserved.
 *
 * Contributor(s): 
 *  Terry Hayes <thayes@netscape.com>
 */

#include "stdlib.h"
#include "plstr.h"
#include "plbase64.h"

#include "nsMemory.h"
#include "nsString.h"
#include "nsCOMPtr.h"
#include "nsISupports.h"
#include "nsIInterfaceRequestor.h"
#include "nsIInterfaceRequestorUtils.h"
#include "nsIServiceManager.h"
#include "nsIWindowWatcher.h"
#include "nsIPrompt.h"
#include "nsProxiedService.h"
#include "nsITokenPasswordDialogs.h"

#include "nsISecretDecoderRing.h"
#include "nsSDR.h"
#include "nsNSSComponent.h"
#include "nsNSSShutDown.h"
#include "nsNSSHelper.h"

#include "pk11func.h"
#include "pk11sdr.h" // For PK11SDR_Encrypt, PK11SDR_Decrypt

#include "nsNSSCleaner.h"
NSSCleanupAutoPtrClass(PK11SlotInfo, PK11_FreeSlot)

// Standard ISupports implementation
// NOTE: Should these be the thread-safe versions?
NS_IMPL_ISUPPORTS2(nsSecretDecoderRing, nsISecretDecoderRing, nsISecretDecoderRingConfig)

// nsSecretDecoderRing constructor
nsSecretDecoderRing::nsSecretDecoderRing()
{
  // initialize superclass
}

// nsSecretDecoderRing destructor
nsSecretDecoderRing::~nsSecretDecoderRing()
{
}

/* [noscript] long encrypt (in buffer data, in long dataLen, out buffer result); */
NS_IMETHODIMP nsSecretDecoderRing::
Encrypt(unsigned char * data, PRInt32 dataLen, unsigned char * *result, PRInt32 *_retval)
{
  nsNSSShutDownPreventionLock locker;
  nsresult rv = NS_OK;
  PK11SlotInfo *slot = 0;
  PK11SlotInfoCleaner tmpSlotCleaner(slot);
  SECItem keyid;
  SECItem request;
  SECItem reply;
  SECStatus s;
  nsCOMPtr<nsIInterfaceRequestor> ctx = new PipUIContext("Encrypt a password");

  slot = PK11_GetInternalKeySlot();
  if (!slot) { rv = NS_ERROR_NOT_AVAILABLE; goto loser; }

  /* Make sure token is initialized. */
  rv = setPassword(slot, ctx);
  if (NS_FAILED(rv))
    goto loser;

  s = PK11_Authenticate(slot, PR_TRUE, ctx);
  if (s != SECSuccess) { rv = NS_ERROR_FAILURE; goto loser; }

  /* Use default key id */
  keyid.data = 0;
  keyid.len = 0;
  request.data = data;
  request.len = dataLen;
  reply.data = 0;
  reply.len = 0;
  s= PK11SDR_Encrypt(&keyid, &request, &reply, ctx);
  if (s != SECSuccess) { rv = NS_ERROR_FAILURE; goto loser; }

  *result = reply.data;
  *_retval = reply.len;

loser:
  return rv;
}

/* [noscript] long decrypt (in buffer data, in long dataLen, out buffer result); */
NS_IMETHODIMP nsSecretDecoderRing::
Decrypt(unsigned char * data, PRInt32 dataLen, unsigned char * *result, PRInt32 *_retval)
{
  nsNSSShutDownPreventionLock locker;
  nsresult rv = NS_OK;
  PK11SlotInfo *slot = 0;
  PK11SlotInfoCleaner tmpSlotCleaner(slot);
  SECStatus s;
  SECItem request;
  SECItem reply;
  nsCOMPtr<nsIInterfaceRequestor> ctx = new PipUIContext("Decrypt a remembered password");

  *result = 0;
  *_retval = 0;

  /* Find token with SDR key */
  slot = PK11_GetInternalKeySlot();
  if (!slot) { rv = NS_ERROR_NOT_AVAILABLE; goto loser; }

  /* Force authentication */
  if (PK11_Authenticate(slot, PR_TRUE, ctx) != SECSuccess)
  {
    rv = NS_ERROR_NOT_AVAILABLE;
    goto loser;
  }

  request.data = data;
  request.len = dataLen;
  reply.data = 0;
  reply.len = 0;
  s = PK11SDR_Decrypt(&request, &reply, ctx);
  if (s != SECSuccess) { rv = NS_ERROR_FAILURE; goto loser; }

  *result = reply.data;
  *_retval = reply.len;

loser:
  return rv;
}

/* string encryptString (in string text); */
NS_IMETHODIMP nsSecretDecoderRing::
EncryptString(const char *text, char **_retval)
{
    nsNSSShutDownPreventionLock locker;
    nsresult rv = NS_OK;
    unsigned char *encrypted = 0;
    PRInt32 eLen;

    if (text == nsnull || _retval == nsnull) {
        rv = NS_ERROR_INVALID_POINTER;
        goto loser;
    }

    rv = Encrypt((unsigned char *)text, PL_strlen(text), &encrypted, &eLen);
    if (rv != NS_OK) { goto loser; }

    rv = encode(encrypted, eLen, _retval);

loser:
    if (encrypted) nsMemory::Free(encrypted);

    return rv;
}

/* string decryptString (in string crypt); */
NS_IMETHODIMP nsSecretDecoderRing::
DecryptString(const char *crypt, char **_retval)
{
    nsNSSShutDownPreventionLock locker;
    nsresult rv = NS_OK;
    char *r = 0;
    unsigned char *decoded = 0;
    PRInt32 decodedLen;
    unsigned char *decrypted = 0;
    PRInt32 decryptedLen;

    if (crypt == nsnull || _retval == nsnull) {
      rv = NS_ERROR_INVALID_POINTER;
      goto loser;
    }

    rv = decode(crypt, &decoded, &decodedLen);
    if (rv != NS_OK) goto loser;

    rv = Decrypt(decoded, decodedLen, &decrypted, &decryptedLen);
    if (rv != NS_OK) goto loser;

    // Convert to NUL-terminated string
    r = (char *)nsMemory::Alloc(decryptedLen+1);
    if (!r) { rv = NS_ERROR_OUT_OF_MEMORY; goto loser; }

    memcpy(r, decrypted, decryptedLen);
    r[decryptedLen] = 0;

    *_retval = r;
    r = 0;

loser:
    if (r) nsMemory::Free(r);
    if (decrypted) nsMemory::Free(decrypted);
    if (decoded) nsMemory::Free(decoded);
 
    return rv;
}

/* void changePassword(); */
NS_IMETHODIMP nsSecretDecoderRing::
ChangePassword()
{
  nsNSSShutDownPreventionLock locker;
  nsresult rv;
  PK11SlotInfo *slot;

  slot = PK11_GetInternalKeySlot();
  if (!slot) return NS_ERROR_NOT_AVAILABLE;

  /* Convert UTF8 token name to UCS2 */
  NS_ConvertUTF8toUCS2 tokenName(PK11_GetTokenName(slot));

  PK11_FreeSlot(slot);

  /* Get the set password dialog handler imlementation */
  nsCOMPtr<nsITokenPasswordDialogs> dialogs;

  rv = getNSSDialogs(getter_AddRefs(dialogs),
           NS_GET_IID(nsITokenPasswordDialogs),
           NS_TOKENPASSWORDSDIALOG_CONTRACTID);
  if (NS_FAILED(rv)) return rv;

  nsCOMPtr<nsIInterfaceRequestor> ctx = new PipUIContext("Remember an updated password");
  PRBool canceled;

  {
    nsPSMUITracker tracker;
    if (tracker.isUIForbidden()) {
      rv = NS_ERROR_NOT_AVAILABLE;
    }
    else {
      rv = dialogs->SetPassword(ctx, tokenName.get(), &canceled);
    }
  }

  /* canceled is ignored */


  return rv;
}

static NS_DEFINE_CID(kNSSComponentCID, NS_NSSCOMPONENT_CID);

NS_IMETHODIMP nsSecretDecoderRing::
Logout()
{
  nsresult rv;
  nsCOMPtr<nsINSSComponent> nssComponent(do_GetService(kNSSComponentCID, &rv));
  if (NS_FAILED(rv))
    return rv;

  {
    nsNSSShutDownPreventionLock locker;
    PK11_LogoutAll();
  }

  return NS_OK;
}

NS_IMETHODIMP nsSecretDecoderRing::
LogoutAndTeardown()
{
  nsresult rv;
  nsCOMPtr<nsINSSComponent> nssComponent(do_GetService(kNSSComponentCID, &rv));
  if (NS_FAILED(rv))
    return rv;

  {
    nsNSSShutDownPreventionLock locker;
    PK11_LogoutAll();
  }

  return nssComponent->LogoutAuthenticatedPK11();
}

/* void setWindow(in nsISupports w); */
nsresult nsSecretDecoderRing::
SetWindow(nsISupports *w)
{
  return NS_OK;
}

// Support routines

nsresult nsSecretDecoderRing::
encode(const unsigned char *data, PRInt32 dataLen, char **_retval)
{
    nsresult rv = NS_OK;

    *_retval = PL_Base64Encode((const char *)data, dataLen, NULL);
    if (!*_retval) { rv = NS_ERROR_OUT_OF_MEMORY; goto loser; }

loser:
    return rv;
}

nsresult nsSecretDecoderRing::
decode(const char *data, unsigned char **result, PRInt32 * _retval)
{
    nsresult rv = NS_OK;
    PRUint32 len = PL_strlen(data);
    int adjust = 0;

    /* Compute length adjustment */
    if (data[len-1] == '=') {
      adjust++;
      if (data[len-2] == '=') adjust++;
    }

    *result = (unsigned char *)PL_Base64Decode(data, len, NULL);
    if (!*result) { rv = NS_ERROR_ILLEGAL_VALUE; goto loser; }

    *_retval = (len*3)/4 - adjust;

loser:
    return rv;
}
