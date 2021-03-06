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
 * The Original Code is mozilla.org code.
 *
 * The Initial Developer of the Original Code is
 * Netscape Communications Corporation.
 * Portions created by the Initial Developer are Copyright (C) 2003
 * the Initial Developer. All Rights Reserved.
 *
 * Contributor(s):
 *   Christopher A. Aillon <christopher@aillon.com>
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

#include "nscore.h"
#include "nsScriptSecurityManager.h"
#include "nsString.h"
#include "nsReadableUtils.h"
#include "plstr.h"
#include "nsCRT.h"
#include "nsIURI.h"
#include "nsNetUtil.h"
#include "nsJSPrincipals.h"
#include "nsVoidArray.h"
#include "nsHashtable.h"
#include "nsIObjectInputStream.h"
#include "nsIObjectOutputStream.h"

#include "nsPrincipal.h"


// Static member variables
PRInt32 nsPrincipal::sCapabilitiesOrdinal = 0;
const char nsPrincipal::sInvalid[] = "Invalid";


nsPrincipal::nsPrincipal()
  : mCapabilities(7)
{
}


NS_IMPL_QUERY_INTERFACE2_CI(nsPrincipal,
                            nsIPrincipal,
                            nsISerializable)
NS_IMPL_CI_INTERFACE_GETTER2(nsPrincipal,
                             nsIPrincipal,
                             nsISerializable)

NS_IMETHODIMP_(nsrefcnt)
nsPrincipal::AddRef()
{
  NS_PRECONDITION(PRInt32(mJSPrincipals.refcount) >= 0, "illegal refcnt");
  // XXXcaa does this need to be threadsafe?  See bug 143559.
  nsrefcnt count = PR_AtomicIncrement((PRInt32 *)&mJSPrincipals.refcount);
  NS_LOG_ADDREF(this, count, "nsPrincipal", sizeof(*this));
  return count;
}

NS_IMETHODIMP_(nsrefcnt)
nsPrincipal::Release()
{
  NS_PRECONDITION(0 != mJSPrincipals.refcount, "dup release");
  nsrefcnt count = PR_AtomicDecrement((PRInt32 *)&mJSPrincipals.refcount);
  NS_LOG_RELEASE(this, count, "nsPrincipal");
  if (count == 0) {
    NS_DELETEXPCOM(this);
  }

  return count;
}

nsPrincipal::nsPrincipal(nsIURI *aURI)
  : mSecurityPolicy(nsnull),
    mCodebase(aURI)
{
}


PR_STATIC_CALLBACK(PRBool)
deleteElement(void* aElement, void *aData)
{
  nsHashtable *ht = (nsHashtable *) aElement;
  delete ht;
  return PR_TRUE;
}

nsPrincipal::~nsPrincipal(void)
{
  mAnnotations.EnumerateForwards(deleteElement, nsnull);
  delete mCert;
}

NS_IMETHODIMP
nsPrincipal::GetJsPrincipals(JSPrincipals **jsprin)
{
  if (!mJSPrincipals.nsIPrincipalPtr) {
    // Don't addref here, since we are referencing each other.
    mJSPrincipals.nsIPrincipalPtr = this;
  }

  *jsprin = &mJSPrincipals;

  // JSPRINCIPALS_HOLD does not use its first argument.
  // Just use a dummy cx to save the codesize.
  JSPRINCIPALS_HOLD(cx, *jsprin);

  return NS_OK;
}

NS_IMETHODIMP
nsPrincipal::GetOrigin(char **aOrigin)
{
  nsIURI* uri = mDomain ? mDomain : mCodebase;
  NS_ASSERTION(uri, "No Domain or Codebase");

  nsCAutoString hostPort;
  nsresult rv = uri->GetHostPort(hostPort);
  if (NS_SUCCEEDED(rv)) {
    nsCAutoString scheme;
    rv = uri->GetScheme(scheme);
    NS_ENSURE_SUCCESS(rv, rv);
    *aOrigin = ToNewCString(scheme + NS_LITERAL_CSTRING("://") + hostPort);
  }
  else {
    // Some URIs (e.g., nsSimpleURI) don't support host. Just
    // get the full spec.
    nsCAutoString spec;
    rv = uri->GetSpec(spec);
    NS_ENSURE_SUCCESS(rv, rv);
    *aOrigin = ToNewCString(spec);
  }

  return *aOrigin ? NS_OK : NS_ERROR_OUT_OF_MEMORY;
}

NS_IMETHODIMP
nsPrincipal::GetSecurityPolicy(void** aSecurityPolicy)
{
  *aSecurityPolicy = mSecurityPolicy;
  return NS_OK;
}

NS_IMETHODIMP
nsPrincipal::SetSecurityPolicy(void* aSecurityPolicy)
{
  mSecurityPolicy = aSecurityPolicy;
  return NS_OK;
}

NS_IMETHODIMP
nsPrincipal::Equals(nsIPrincipal *aOther, PRBool *aResult)
{
  *aResult = PR_FALSE;

  if (!aOther) {
    NS_WARNING("Need a principal to compare this to!");
    return NS_OK;
  }

  if (this != aOther) {
    if (mCert) {
      PRBool otherHasCert;
      aOther->GetHasCertificate(&otherHasCert);
      if (!otherHasCert) {
        return NS_OK;
      }

      nsXPIDLCString otherCertID;
      aOther->GetCertificateID(getter_Copies(otherCertID));
      if (!otherCertID.Equals(mCert->certificateID)) {
        return NS_OK;
      }
    }

    // Codebases are equal if they have the same origin.
    nsIURI *origin = mDomain ? mDomain : mCodebase;
    nsCOMPtr<nsIURI> otherOrigin;
    aOther->GetDomain(getter_AddRefs(otherOrigin));
    if (!otherOrigin) {
      aOther->GetURI(getter_AddRefs(otherOrigin));
    }

    return nsScriptSecurityManager::SecurityCompareURIs(origin,
                                                        otherOrigin,
                                                        aResult);
  }

  *aResult = PR_TRUE;
  return NS_OK;
}


NS_IMETHODIMP
nsPrincipal::CanEnableCapability(const char *capability, PRInt16 *result)
{
  // If this principal is marked invalid, can't enable any capabilities
  nsCStringKey invalidKey(sInvalid);
  if (mCapabilities.Exists(&invalidKey)) {
    *result = nsIPrincipal::ENABLE_DENIED;

    return NS_OK;
  }

  const char *start = capability;
  *result = nsIPrincipal::ENABLE_GRANTED;
  for(;;) {
    const char *space = PL_strchr(start, ' ');
    PRInt32 len = space ? space - start : strlen(start);
    nsCAutoString capString(start, len);
    nsCStringKey key(capString);
    PRInt16 value = (PRInt16)NS_PTR_TO_INT32(mCapabilities.Get(&key));
    if (value == 0) {
      value = nsIPrincipal::ENABLE_UNKNOWN;
    }

    if (value < *result) {
      *result = value;
    }

    if (!space) {
      break;
    }

    start = space + 1;
  }

  return NS_OK;
}

NS_IMETHODIMP
nsPrincipal::SetCanEnableCapability(const char *capability,
                                    PRInt16 canEnable)
{
  // If this principal is marked invalid, can't enable any capabilities

  nsCStringKey invalidKey(sInvalid);
  if (mCapabilities.Exists(&invalidKey)) {
    return NS_OK;
  }

  if (PL_strcmp(capability, sInvalid) == 0) {
    mCapabilities.Reset();
  }

  const char *start = capability;
  for(;;) {
    const char *space = PL_strchr(start, ' ');
    int len = space ? space - start : strlen(start);
    nsCAutoString capString(start, len);
    nsCStringKey key(capString);
    mCapabilities.Put(&key, NS_INT32_TO_PTR(canEnable));
    if (!space) {
      break;
    }

    start = space + 1;
  }

  return NS_OK;
}

NS_IMETHODIMP
nsPrincipal::IsCapabilityEnabled(const char *capability, void *annotation,
                                 PRBool *result)
{
  *result = PR_FALSE;
  nsHashtable *ht = (nsHashtable *) annotation;
  if (!ht) {
    return NS_OK;
  }
  const char *start = capability;
  for(;;) {
    const char *space = PL_strchr(start, ' ');
    int len = space ? space - start : strlen(start);
    nsCAutoString capString(start, len);
    nsCStringKey key(capString);
    *result = (ht->Get(&key) == (void *) AnnotationEnabled);
    if (!*result) {
      // If any single capability is not enabled, then return false.
      return NS_OK;
    }

    if (!space) {
      return NS_OK;
    }

    start = space + 1;
  }

  return NS_OK;
}

NS_IMETHODIMP
nsPrincipal::EnableCapability(const char *capability, void **annotation)
{
  return SetCapability(capability, annotation, AnnotationEnabled);
}

NS_IMETHODIMP
nsPrincipal::DisableCapability(const char *capability, void **annotation)
{
  return SetCapability(capability, annotation, AnnotationDisabled);
}

NS_IMETHODIMP
nsPrincipal::RevertCapability(const char *capability, void **annotation)
{
  if (*annotation) {
    nsHashtable *ht = (nsHashtable *) *annotation;
    const char *start = capability;
    for(;;) {
      const char *space = PL_strchr(start, ' ');
      int len = space ? space - start : strlen(start);
      nsCAutoString capString(start, len);
      nsCStringKey key(capString);
      ht->Remove(&key);
      if (!space) {
        return NS_OK;
      }

      start = space + 1;
    }
  }
  return NS_OK;
}

nsresult
nsPrincipal::SetCapability(const char *capability, void **annotation,
                           AnnotationValue value)
{
  if (*annotation == nsnull) {
    *annotation = new nsHashtable(5);
    if (!*annotation) {
       return NS_ERROR_OUT_OF_MEMORY;
     }

    // This object owns its annotations. Save them so we can release
    // them when we destroy this object.
    mAnnotations.AppendElement(*annotation);
  }

  const char *start = capability;
  for(;;) {
    const char *space = PL_strchr(start, ' ');
    int len = space ? space - start : strlen(start);
    nsCAutoString capString(start, len);
    nsCStringKey key(capString);
    nsHashtable *ht = (nsHashtable *) *annotation;
    ht->Put(&key, (void *) value);
    if (!space) {
      break;
    }

    start = space + 1;
  }

  return NS_OK;
}

NS_IMETHODIMP
nsPrincipal::GetHasCertificate(PRBool* aResult)
{
  *aResult = (mCert != nsnull);

  return NS_OK;
}

NS_IMETHODIMP
nsPrincipal::GetURI(nsIURI** aURI)
{
  NS_IF_ADDREF(*aURI = mCodebase);

  return NS_OK;
}

NS_IMETHODIMP
nsPrincipal::SetURI(nsIURI* aURI)
{
  mCodebase = aURI;
  mDomain = nsnull;
  // Codebase has changed, forget cached security policy
  mSecurityPolicy = nsnull;

  return NS_OK;
}

NS_IMETHODIMP
nsPrincipal::SetCertificateID(const char* aID)
{
  if (!aID) {
    mCert = nsnull;
    return NS_OK;
  }

  if (!mCert) {
    mCert = new Certificate(aID, "");
    if (!mCert) {
      return NS_ERROR_OUT_OF_MEMORY;
    }
  }

  return NS_OK;
}

NS_IMETHODIMP
nsPrincipal::GetCertificateID(char** aID)
{
  NS_ENSURE_STATE(mCert);

  *aID = ToNewCString(mCert->certificateID);
  if (!*aID) {
    return NS_ERROR_OUT_OF_MEMORY;
  }

  return NS_OK;
}

NS_IMETHODIMP
nsPrincipal::GetCommonName(char** aName)
{
  NS_ENSURE_STATE(mCert);

  *aName = ToNewCString(mCert->commonName);
  if (!*aName) {
    return NS_ERROR_OUT_OF_MEMORY;
  }

  return NS_OK;
}

NS_IMETHODIMP
nsPrincipal::SetCommonName(const char* aName)
{
  if (!mCert) {
    NS_ERROR("You must first initialize the certificate with an ID");
    return NS_ERROR_FAILURE;
  }

  mCert->commonName = aName;

  return NS_OK;
}


NS_IMETHODIMP
nsPrincipal::GetHashValue(PRUint32* aValue)
{
  NS_PRECONDITION(mCert || mCodebase, "Need a cert or codebase");

  // If there is a certificate, it takes precendence over the codebase.
  if (mCert) {
    *aValue = nsCRT::HashCode(mCert->certificateID.get(), nsnull);
  }
  else {
    nsCAutoString str;
    mCodebase->GetSpec(str);
    *aValue = nsCRT::HashCode(str.get(), nsnull);
  }

  return NS_OK;
}

NS_IMETHODIMP
nsPrincipal::GetDomain(nsIURI** aDomain)
{
  NS_IF_ADDREF(*aDomain = mDomain);

  return NS_OK;
}

NS_IMETHODIMP
nsPrincipal::SetDomain(nsIURI* aDomain)
{
  mDomain = aDomain;
  // Domain has changed, forget cached security policy
  mSecurityPolicy = nsnull;

  return NS_OK;
}

nsresult
nsPrincipal::InitFromPersistent(const char* aPrefName,
                                const char* aToken,
                                const char* aGrantedList,
                                const char* aDeniedList,
                                PRBool aIsCert,
                                PRBool aTrusted)
{
  if (aIsCert) {
    SetCertificateID(aToken);
  }
  else {
    nsCOMPtr<nsIURI> uri;
    nsresult rv = NS_NewURI(getter_AddRefs(uri), aToken, nsnull);
    if (NS_FAILED(rv)) {
      NS_ERROR("Malformed URI in capability.principal preference.");
      return rv;
    }

    nsCAutoString token;
    rv = uri->GetSpec(token);
    if (NS_FAILED(rv)) {
      return rv;
    }

    rv = mJSPrincipals.Init(PL_strdup(token.get()));
    if (NS_FAILED(rv)) {
      return rv;
    }

    mTrusted = aTrusted;
  }

  //-- Empty the capability table
  mCapabilities.Reset();

  //-- Save the preference name
  mPrefName = aPrefName;

  const char* ordinalBegin = PL_strpbrk(aPrefName, "1234567890");
  if (ordinalBegin) {
    PRIntn n = atoi(ordinalBegin);
    if (sCapabilitiesOrdinal <= n) {
      sCapabilitiesOrdinal = n + 1;
    }
  }

  //-- Store the capabilities
  nsresult rv = NS_OK;
  if (aGrantedList) {
    rv = SetCanEnableCapability(aGrantedList, nsIPrincipal::ENABLE_GRANTED);
  }

  if (NS_SUCCEEDED(rv) && aDeniedList) {
    rv = SetCanEnableCapability(aDeniedList, nsIPrincipal::ENABLE_DENIED);
  }

  return rv;
}

struct CapabilityList
{
  nsCString* granted;
  nsCString* denied;
};

PR_STATIC_CALLBACK(PRBool)
AppendCapability(nsHashKey *aKey, void *aData, void *capListPtr)
{
  CapabilityList* capList = (CapabilityList*)capListPtr;
  PRInt16 value = (PRInt16)NS_PTR_TO_INT32(aData);
  nsCStringKey* key = (nsCStringKey *)aKey;
  if (value == nsIPrincipal::ENABLE_GRANTED) {
    capList->granted->Append(key->GetString(), key->GetStringLength());
    capList->granted->Append(' ');
  }
  else if (value == nsIPrincipal::ENABLE_DENIED) {
    capList->denied->Append(key->GetString(), key->GetStringLength());
    capList->denied->Append(' ');
  }

  return PR_TRUE;
}

NS_IMETHODIMP
nsPrincipal::GetPreferences(char** aPrefName, char** aID,
                            char** aGrantedList, char** aDeniedList)
{
  if (mPrefName.IsEmpty()) {
    if (mCert) {
      mPrefName.Assign("capability.principal.certificate.p");
    }
    else {
      mPrefName.Assign("capability.principal.codebase.p");
    }

    mPrefName.AppendInt(sCapabilitiesOrdinal++);
    mPrefName.Append(".id");
  }

  *aPrefName = nsnull;
  *aID = nsnull;
  *aGrantedList = nsnull;
  *aDeniedList = nsnull;

  //-- Preference name
  *aPrefName = ToNewCString(mPrefName);
  if (!aPrefName) {
    return NS_ERROR_OUT_OF_MEMORY;
  }

  //-- ID
  nsresult rv;
  if (mCert) {
    rv = GetCertificateID(aID);
  }
  else {
    rv = GetOrigin(aID);
  }

  if (NS_FAILED(rv)) {
    nsMemory::Free(aPrefName);
    return rv;
  }

  //-- Capabilities
  nsCAutoString grantedListStr, deniedListStr;
  CapabilityList capList = CapabilityList();
  capList.granted = &grantedListStr;
  capList.denied = &deniedListStr;
  mCapabilities.Enumerate(AppendCapability, (void*)&capList);

  if (!grantedListStr.IsEmpty()) {
    grantedListStr.Truncate(grantedListStr.Length() - 1);
    *aGrantedList = ToNewCString(grantedListStr);
    if (!*aGrantedList) {
      nsMemory::Free(aPrefName);
      nsMemory::Free(aID);
      return NS_ERROR_OUT_OF_MEMORY;
    }
  }

  if (!deniedListStr.IsEmpty()) {
    deniedListStr.Truncate(deniedListStr.Length() - 1);
    *aDeniedList = ToNewCString(deniedListStr);
    if (!*aDeniedList) {
      nsMemory::Free(aPrefName);
      nsMemory::Free(aID);
      if (*aGrantedList) {
        nsMemory::Free(aGrantedList);
      }
      return NS_ERROR_OUT_OF_MEMORY;
    }
  }

  return NS_OK;
}

PR_STATIC_CALLBACK(nsresult)
ReadAnnotationEntry(nsIObjectInputStream* aStream, nsHashKey** aKey,
                    void** aData)
{
  nsresult rv;
  nsCStringKey* key = new nsCStringKey(aStream, &rv);
  if (NS_FAILED(rv)) {
    return rv;
  }

  PRUint32 value;
  rv = aStream->Read32(&value);
  if (NS_FAILED(rv)) {
    delete key;
    return rv;
  }

  *aKey = key;
  *aData = (void*) value;
  return NS_OK;
}

PR_STATIC_CALLBACK(void)
FreeAnnotationEntry(nsIObjectInputStream* aStream, nsHashKey* aKey,
                    void* aData)
{
  delete aKey;
}

NS_IMETHODIMP
nsPrincipal::Read(nsIObjectInputStream* aStream)
{
  PRUint32 annotationCount;
  nsresult rv = aStream->Read32(&annotationCount);
  if (NS_FAILED(rv)) {
    return rv;
  }

  for (PRInt32 i = 0, n = PRInt32(annotationCount); i < n; i++) {
    nsHashtable *ht = new nsHashtable(aStream,
                                      ReadAnnotationEntry,
                                      FreeAnnotationEntry,
                                      &rv);
    if (!ht) {
      return NS_ERROR_OUT_OF_MEMORY;
    }

    if (NS_FAILED(rv)) {
      delete ht;
      return rv;
    }

    if (!mAnnotations.InsertElementAt(NS_REINTERPRET_CAST(void*, ht), i)) {
      delete ht;
      return NS_ERROR_OUT_OF_MEMORY;
    }
  }

  PRBool hasCapabilities;
  rv = aStream->ReadBoolean(&hasCapabilities);
  if (NS_SUCCEEDED(rv) && hasCapabilities) {
    mCapabilities = nsHashtable(aStream,
                                ReadAnnotationEntry,
                                FreeAnnotationEntry,
                                &rv);
  }

  if (NS_FAILED(rv)) {
    return rv;
  }

  rv = NS_ReadOptionalCString(aStream, mPrefName);
  if (NS_FAILED(rv)) {
    return rv;
  }

  return NS_OK;
}

PR_STATIC_CALLBACK(nsresult)
WriteScalarValue(nsIObjectOutputStream* aStream, void* aData)
{
  PRUint32 value = NS_PTR_TO_INT32(aData);

  return aStream->Write32(value);
}

NS_IMETHODIMP
nsPrincipal::Write(nsIObjectOutputStream* aStream)
{
  PRUint32 annotationCount = PRUint32(mAnnotations.Count());
  nsresult rv = aStream->Write32(annotationCount);
  if (NS_FAILED(rv)) {
    return rv;
  }

  for (PRInt32 i = 0, n = PRInt32(annotationCount); i < n; i++) {
    nsHashtable *ht = NS_REINTERPRET_CAST(nsHashtable *, mAnnotations[i]);
    rv = ht->Write(aStream, WriteScalarValue);
    if (NS_FAILED(rv)) {
      return rv;
    }
  }

  PRBool hasCapabilities = (mCapabilities.Count() > 0);
  rv = aStream->WriteBoolean(hasCapabilities);
  if (NS_SUCCEEDED(rv) && hasCapabilities) {
    rv = mCapabilities.Write(aStream, WriteScalarValue);
  }

  if (NS_FAILED(rv)) {
    return rv;
  }

  rv = NS_WriteOptionalStringZ(aStream, mPrefName.get());
  if (NS_FAILED(rv)) {
    return rv;
  }

  return NS_OK;
}
