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
 * Copyright (C) 2000 Netscape Communications Corporation.  All
 * Rights Reserved.
 * 
 * Contributor(s):
 *  Ian McGreer <mcgreer@netscape.com>
 *  Javier Delgadillo <javi@netscape.com>
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
 *
 */

#ifndef _NSNSSCERTHELPER_H_
#define _NSNSSCERTHELPER_H_

#include "nsNSSCertHeader.h"

class nsINSSComponent;
class nsIASN1PrintableItem;

nsresult
GetIntValue(SECItem *versionItem, 
            unsigned long *version);

nsresult
ProcessVersion(SECItem         *versionItem,
               nsINSSComponent *nssComponent,
               nsIASN1PrintableItem **retItem);

nsresult 
ProcessSerialNumberDER(SECItem         *serialItem, 
                       nsINSSComponent *nssComponent,
                       nsIASN1PrintableItem **retItem);

nsresult
GetDefaultOIDFormat(SECItem *oid,
                    nsString &outString);

nsresult
GetOIDText(SECItem *oid, nsINSSComponent *nssComponent, nsString &text);

nsresult
ProcessRawBytes(SECItem *data, nsString &text);

nsresult
ProcessNSCertTypeExtensions(SECItem  *extData, 
                            nsString &text,
                            nsINSSComponent *nssComponent);

nsresult
ProcessKeyUsageExtension(SECItem *extData, nsString &text,
                         nsINSSComponent *nssComponent);

nsresult
ProcessExtensionData(SECOidTag oidTag, SECItem *extData, 
                     nsString &text, nsINSSComponent *nssComponent);

nsresult
ProcessSingleExtension(CERTCertExtension *extension,
                       nsINSSComponent *nssComponent,
                       nsIASN1PrintableItem **retExtension);

PRUint32
getCertType(CERTCertificate *cert);

#endif
