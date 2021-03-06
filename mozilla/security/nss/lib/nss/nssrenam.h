/*
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
 * The Original Code is the Netscape security libraries.
 * 
 * The Initial Developer of the Original Code is Netscape
 * Communications Corporation.  Portions created by Netscape are 
 * Copyright (C) 2001 Netscape Communications Corporation.  All
 * Rights Reserved.
 * 
 * Contributor(s):
 * 
 * Alternatively, the contents of this file may be used under the
 * terms of the GNU General Public License Version 2 or later (the
 * "GPL"), in which case the provisions of the GPL are applicable 
 * instead of those above.  If you wish to allow use of your 
 * version of this file only under the terms of the GPL and not to
 * allow others to use your version of this file under the MPL,
 * indicate your decision by deleting the provisions above and
 * replace them with the notice and other provisions required by
 * the GPL.  If you do not delete the provisions above, a recipient
 * may use your version of this file under either the MPL or the
 * GPL.
 */

#ifndef __nssrenam_h_
#define __nssrenam_h_

#define CERT_NewTempCertificate __CERT_NewTempCertificate
#define PK11_CreateContextByRawKey __PK11_CreateContextByRawKey
#define PK11_GetKeyData __PK11_GetKeyData
#define nss_InitLock __nss_InitLock
#define CERT_ClosePermCertDB __CERT_ClosePermCertDB
#define CERT_DecodeDERCertificate __CERT_DecodeDERCertificate
#define CERT_TraversePermCertsForNickname __CERT_TraversePermCertsForNickname
#define CERT_TraversePermCertsForSubject __CERT_TraversePermCertsForSubject
#define PBE_CreateContext __PBE_CreateContext
#define PBE_DestroyContext __PBE_DestroyContext
#define PBE_GenerateBits __PBE_GenerateBits

#endif /* __nssrenam_h_ */
