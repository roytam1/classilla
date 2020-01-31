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
 * The Original Code is mozilla.org code.
 *
 * The Initial Developer of the Original Code is Netscape
 * Communications Corporation.  Portions created by Netscape are
 * Copyright (C) 1998 Netscape Communications Corporation. All
 * Rights Reserved.
 *
 * Contributor(s):
 *  Brian Ryner <bryner@netscape.com>
 *  Terry Hayes <thayes@netscape.com>
*/
#include "nsNSSComponent.h" // for PIPNSS string bundle calls.
#include "nsNSSCallbacks.h"
#include "nsNSSCertificate.h"
#include "nsISSLStatus.h"
#include "nsNSSIOLayer.h" // for nsNSSSocketInfo
#include "nsIWebProgressListener.h"
#include "nsIStringBundle.h"
#include "nsXPIDLString.h"
#include "nsCOMPtr.h"
#include "nsIServiceManager.h"
#include "nsReadableUtils.h"
#include "nsIPrompt.h"
#include "nsProxiedService.h"
#include "nsIInterfaceRequestor.h"
#include "nsIInterfaceRequestorUtils.h"
#include "nsCRT.h"
#include "nsNSSShutDown.h"

#include "ssl.h"
#include "cert.h"

// bug 642395 modified for Classilla issue 158
#include "secerr.h"
//#include "nsNSSCleaner.h"
//NSSCleanupAutoPtrClass(CERTCertificate, CERT_DestroyCertificate)

static NS_DEFINE_CID(kNSSComponentCID, NS_NSSCOMPONENT_CID);

/* Implementation of nsISSLStatus */
class nsSSLStatus
  : public nsISSLStatus
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSISSLSTATUS

  nsSSLStatus();
  virtual ~nsSSLStatus();

  /* public for initilization in this file */
  nsCOMPtr<nsIX509Cert> mServerCert;
  PRUint32 mKeyLength;
  PRUint32 mSecretKeyLength;
  PRUint32 mProtocolVersion;
  nsXPIDLCString mCipherName;
};

NS_IMETHODIMP
nsSSLStatus::GetServerCert(nsIX509Cert** _result)
{
  NS_ASSERTION(_result, "non-NULL destination required");

  *_result = mServerCert;
  NS_IF_ADDREF(*_result);

  return NS_OK;
}

NS_IMETHODIMP
nsSSLStatus::GetKeyLength(PRUint32* _result)
{
  NS_ASSERTION(_result, "non-NULL destination required");

  *_result = mKeyLength;

  return NS_OK;
}

NS_IMETHODIMP
nsSSLStatus::GetSecretKeyLength(PRUint32* _result)
{
  NS_ASSERTION(_result, "non-NULL destination required");

  *_result = mSecretKeyLength;

  return NS_OK;
}

NS_IMETHODIMP
nsSSLStatus::GetProtocolVersion(PRUint32* _result)
{
  NS_ASSERTION(_result, "non-NULL destination required");

  *_result = mProtocolVersion;

  return NS_OK;
}

NS_IMETHODIMP
nsSSLStatus::GetCipherName(char** _result)
{
  NS_ASSERTION(_result, "non-NULL destination required");

  *_result = PL_strdup(mCipherName.get());

  return NS_OK;
}

nsSSLStatus::nsSSLStatus()
: mKeyLength(0), mSecretKeyLength(0), mProtocolVersion(0)
{
}

NS_IMPL_THREADSAFE_ISUPPORTS1(nsSSLStatus, nsISSLStatus);

nsSSLStatus::~nsSSLStatus()
{
}


char* PR_CALLBACK
PK11PasswordPrompt(PK11SlotInfo* slot, PRBool retry, void* arg) {
  nsNSSShutDownPreventionLock locker;
  nsresult rv = NS_OK;
  PRUnichar *password = nsnull;
  PRBool value = PR_FALSE;
  nsIInterfaceRequestor *ir = NS_STATIC_CAST(nsIInterfaceRequestor*, arg);
  nsCOMPtr<nsIPrompt> proxyPrompt;

  // If no context is provided, no prompt is possible.
  if (!ir)
    return nsnull;

  /* TODO: Retry should generate a different dialog message */
/*
  if (retry)
    return nsnull;
*/

  // The interface requestor object may not be safe, so
  // proxy the call to get the nsIPrompt.

  nsCOMPtr<nsIProxyObjectManager> proxyman(do_GetService(NS_XPCOMPROXY_CONTRACTID));
  if (!proxyman) return nsnull;

  nsCOMPtr<nsIInterfaceRequestor> proxiedCallbacks;
  proxyman->GetProxyForObject(NS_UI_THREAD_EVENTQ,
                              NS_GET_IID(nsIInterfaceRequestor),
                              ir,
                              PROXY_SYNC,
                              getter_AddRefs(proxiedCallbacks));

  // Get the desired interface
  nsCOMPtr<nsIPrompt> prompt(do_GetInterface(proxiedCallbacks));
  if (!prompt) {
    NS_ASSERTION(PR_FALSE, "callbacks does not implement nsIPrompt");
    return nsnull;
  }

  // Finally, get a proxy for the nsIPrompt
  proxyman->GetProxyForObject(NS_UI_THREAD_EVENTQ,
	                      NS_GET_IID(nsIPrompt),
                              prompt,
                              PROXY_SYNC,
                              getter_AddRefs(proxyPrompt));


  nsXPIDLString promptString;
  nsCOMPtr<nsINSSComponent> nssComponent(do_GetService(kNSSComponentCID, &rv));

  if (NS_FAILED(rv))
    return nsnull; 

  nsCOMPtr<nsISupportsCString> is_infostring(do_GetInterface(proxiedCallbacks));
  nsCAutoString infostring;
  if (is_infostring)
    is_infostring->GetData(infostring);

  if (!infostring.IsEmpty()) {
    const PRUnichar* formatStrings[2] = { 
        ToNewUnicode(NS_ConvertUTF8toUCS2(PK11_GetTokenName(slot))),
        ToNewUnicode(infostring) };
    rv = nssComponent->PIPBundleFormatStringFromName(NS_LITERAL_STRING("CertPassPrompt2").get(),
                                        formatStrings, 2,
                                        getter_Copies(promptString));
    nsMemory::Free(NS_CONST_CAST(PRUnichar*, formatStrings[0]));
    nsMemory::Free(NS_CONST_CAST(PRUnichar*, formatStrings[1]));
  }
  else {
    const PRUnichar* formatStrings[1] = { ToNewUnicode(NS_ConvertUTF8toUCS2(PK11_GetTokenName(slot))) };
    rv = nssComponent->PIPBundleFormatStringFromName(NS_LITERAL_STRING("CertPassPrompt").get(),
                                        formatStrings, 1,
                                        getter_Copies(promptString));
    nsMemory::Free(NS_CONST_CAST(PRUnichar*, formatStrings[0]));
  }

  if (NS_FAILED(rv))
    return nsnull;

  {
    nsPSMUITracker tracker;
    if (tracker.isUIForbidden()) {
      rv = NS_ERROR_NOT_AVAILABLE;
    }
    else {
      rv = proxyPrompt->PromptPassword(nsnull, promptString.get(),
                                       &password, nsnull, nsnull, &value);
    }
  }
  
  if (NS_SUCCEEDED(rv) && value) {
    char* str = ToNewCString(nsDependentString(password));
    Recycle(password);
    return str;
  }

  return nsnull;
}

void PR_CALLBACK HandshakeCallback(PRFileDesc* fd, void* client_data) {
  nsNSSShutDownPreventionLock locker;
  PRInt32 sslStatus;
  char* signer = nsnull;
  char* cipherName = nsnull;
  PRInt32 keyLength;
  nsresult rv;
  PRInt32 encryptBits;

  if (SECSuccess != SSL_SecurityStatus(fd, &sslStatus, &cipherName, &keyLength,
                                       &encryptBits, &signer, nsnull)) {
    return;
  }

  PRInt32 secStatus;
  if (sslStatus == SSL_SECURITY_STATUS_OFF)
    secStatus = nsIWebProgressListener::STATE_IS_BROKEN;
  else if (encryptBits >= 90)
    secStatus = (nsIWebProgressListener::STATE_IS_SECURE |
                 nsIWebProgressListener::STATE_SECURE_HIGH);
  else
    secStatus = (nsIWebProgressListener::STATE_IS_SECURE |
                 nsIWebProgressListener::STATE_SECURE_LOW);

  CERTCertificate *peerCert = SSL_PeerCertificate(fd);
  char* caName = CERT_GetOrgName(&peerCert->issuer);
  CERT_DestroyCertificate(peerCert);
  if (!caName) {
    caName = signer;
  }

  // If the CA name is RSA Data Security, then change the name to the real
  // name of the company i.e. VeriSign, Inc.
  if (nsCRT::strcmp((const char*)caName, "RSA Data Security, Inc.") == 0) {
    // In this case, caName != signer since the logic implies signer
    // would be at minimal "O=RSA Data Security, Inc" because caName
    // is what comes after to O=.  So we're OK just freeing this memory
    // without checking to see if it's equal to signer;
    NS_ASSERTION(caName != signer, "caName was equal to caName when it shouldn't be");
    PR_Free(caName);
    caName = PL_strdup("Verisign, Inc.");
  }

  nsXPIDLString shortDesc;
  const PRUnichar* formatStrings[1] = { ToNewUnicode(NS_ConvertUTF8toUCS2(caName)) };
  nsCOMPtr<nsINSSComponent> nssComponent(do_GetService(kNSSComponentCID, &rv));
  if (NS_SUCCEEDED(rv)) {
    rv = nssComponent->PIPBundleFormatStringFromName(NS_LITERAL_STRING("SignedBy").get(),
                                                   formatStrings, 1,
                                                   getter_Copies(shortDesc));

    nsMemory::Free(NS_CONST_CAST(PRUnichar*, formatStrings[0]));

    nsNSSSocketInfo* infoObject = (nsNSSSocketInfo*) fd->higher->secret;
    infoObject->SetSecurityState(secStatus);
    infoObject->SetShortSecurityDescription((const PRUnichar*)shortDesc);

    /* Set the SSL Status information */
    nsCOMPtr<nsSSLStatus> status = new nsSSLStatus();

    CERTCertificate *serverCert = SSL_PeerCertificate(fd);
    if (serverCert) {
      status->mServerCert = new nsNSSCertificate(serverCert);
      CERT_DestroyCertificate(serverCert);
    }

    status->mKeyLength = keyLength;
    status->mSecretKeyLength = encryptBits;
    status->mCipherName.Adopt(cipherName);

    SSLChannelInfo channelInfo;
    if (SSL_GetChannelInfo(fd, &channelInfo, sizeof(channelInfo)) == SECSuccess) {
      // Get the protocol version
      // 0=ssl3, 1=tls1, 2=tls1.1, 3=tls1.2
      status->mProtocolVersion = channelInfo.protocolVersion & 0xFF;
    }

    infoObject->SetSSLStatus(status);
  }

  if (caName != signer) {
    PR_Free(caName);
  }
  PR_Free(signer);
}

// Classilla issue 158: the infamous Comodo Iran certificate block
struct nsSerialBinaryBlacklistEntry
{
  unsigned int len;
  const char *binary_serial;
};

// bug 642395
static struct nsSerialBinaryBlacklistEntry myUTNBlacklistEntries[] = {
  { 17, "\x00\x92\x39\xd5\x34\x8f\x40\xd1\x69\x5a\x74\x54\x70\xe1\xf2\x3f\x43" },
  { 17, "\x00\xd8\xf3\x5f\x4e\xb7\x87\x2b\x2d\xab\x06\x92\xe3\x15\x38\x2f\xb0" },
  { 16, "\x72\x03\x21\x05\xc5\x0c\x08\x57\x3d\x8e\xa5\x30\x4e\xfe\xe8\xb0" },
  { 17, "\x00\xb0\xb7\x13\x3e\xd0\x96\xf9\xb5\x6f\xae\x91\xc8\x74\xbd\x3a\xc0" },
  { 16, "\x39\x2a\x43\x4f\x0e\x07\xdf\x1f\x8a\xa3\x05\xde\x34\xe0\xc2\x29" },
  { 16, "\x3e\x75\xce\xd4\x6b\x69\x30\x21\x21\x88\x30\xae\x86\xa8\x2a\x71" },
  { 17, "\x00\xe9\x02\x8b\x95\x78\xe4\x15\xdc\x1a\x71\x0a\x2b\x88\x15\x44\x47" },
  { 17, "\x00\xd7\x55\x8f\xda\xf5\xf1\x10\x5b\xb2\x13\x28\x2b\x70\x77\x29\xa3" },
  { 16, "\x04\x7e\xcb\xe9\xfc\xa5\x5f\x7b\xd0\x9e\xae\x36\xe1\x0c\xae\x1e" },
  { 17, "\x00\xf5\xc8\x6a\xf3\x61\x62\xf1\x3a\x64\xf5\x4f\x6d\xc9\x58\x7c\x06" },
  { 0, 0 } // end marker
};
// end issue

SECStatus PR_CALLBACK AuthCertificateCallback(void* client_data, PRFileDesc* fd,
                                              PRBool checksig, PRBool isServer) {
  nsNSSShutDownPreventionLock locker;

// Classilla issue 158 (bug 642395 modified for Classilla)
  CERTCertificate *serverCert = SSL_PeerCertificate(fd);
  // We do NOT have CertificateCleaner the way this code thinks we do.
  /* CERTCertificateCleaner serverCertCleaner(serverCert);
  if (serverCert &&
  	  serverCert->serialNumber.data &&
      !strcmp(serverCert->issuerName, 
        "CN=UTN-USERFirst-Hardware,OU=http://www.usertrust.com,O=The USERTRUST Network,L=Salt Lake City,ST=UT,C=US")) { */
  // This is somewhat less efficient, but I think checking Comodo more thoroughly is worth it ... :-/
  if (serverCert && &serverCert->issuer && serverCert->serialNumber.data) {
  char *issuerName = CERT_GetCommonName(&serverCert->issuer);
  
  // Classilla issue 166 (bug 682927 modified for Classilla)
  // Since DigiNotar is no more, blacklist everything.
  if (issuerName && strstr(issuerName, "DigiNotar")) {
        PR_SetError(SEC_ERROR_REVOKED_CERTIFICATE, 0);
        return SECFailure;
  }
  	
  // back to 158 ...
  if (issuerName && !strcmp(issuerName, "UTN-USERFirst-Hardware")) {

    unsigned char *server_cert_comparison_start = (unsigned char*)serverCert->serialNumber.data;
    unsigned int server_cert_comparison_len = serverCert->serialNumber.len;

    while (server_cert_comparison_len) {
      if (*server_cert_comparison_start != 0)
        break;

      ++server_cert_comparison_start;
      --server_cert_comparison_len;
    }

    nsSerialBinaryBlacklistEntry *walk = myUTNBlacklistEntries;
    for ( ; walk && walk->len; ++walk) {

      unsigned char *locked_cert_comparison_start = (unsigned char*)walk->binary_serial;
      unsigned int locked_cert_comparison_len = walk->len;
      
      while (locked_cert_comparison_len) {
        if (*locked_cert_comparison_start != 0)
          break;
        
        ++locked_cert_comparison_start;
        --locked_cert_comparison_len;
      }

      if (server_cert_comparison_len == locked_cert_comparison_len &&
          !memcmp(server_cert_comparison_start, locked_cert_comparison_start, locked_cert_comparison_len)) {
        PR_SetError(SEC_ERROR_REVOKED_CERTIFICATE, 0);
        return SECFailure;
      }
    }
  }
  }
// end bug and end issue

  // first the default action
  SECStatus rv = SSL_AuthCertificate(CERT_GetDefaultCertDB(), fd, checksig, isServer);

  // We want to remember the CA certs in the temp db, so that the application can find the
  // complete chain at any time it might need it.
  // But we keep only those CA certs in the temp db, that we didn't already know.
  
  if (SECSuccess == rv) {
    //CERTCertificate *serverCert = SSL_PeerCertificate(fd); // bug 642395 and issue 158
    
    if (serverCert) {
      CERTCertList *certList = CERT_GetCertChainFromCert(serverCert, PR_Now(), certUsageSSLCA);

      nsCOMPtr<nsINSSComponent> nssComponent;
      
      for (CERTCertListNode *node = CERT_LIST_HEAD(certList);
           !CERT_LIST_END(node, certList);
           node = CERT_LIST_NEXT(node)) {

        if (node->cert->slot) {
          // This cert was found on a token, no need to remember it in the temp db.
          continue;
        }

        if (node->cert->isperm) {
          // We don't need to remember certs already stored in perm db.
          continue;
        }
        
        if (node->cert == serverCert) {
          // We don't want to remember the server cert, 
          // the code that cares for displaying page info does this already.
          continue;
        }
        
        // We have found a signer cert that we want to remember.

        if (!nssComponent) {
          // delay getting the service until we really need it
          nsresult rv;
          nssComponent = do_GetService(kNSSComponentCID, &rv);
        }
        
        if (nssComponent) {
          nssComponent->RememberCert(node->cert);
        }
      }

      CERT_DestroyCertList(certList);
      CERT_DestroyCertificate(serverCert);
    }
  }

  return rv;
}
