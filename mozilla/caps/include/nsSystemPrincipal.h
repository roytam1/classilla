/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */
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
 * Portions created by the Initial Developer are Copyright (C) 1999-2000
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

/* The privileged system principal. */

#ifndef _NS_SYSTEM_PRINCIPAL_H_
#define _NS_SYSTEM_PRINCIPAL_H_

#include "nsBasePrincipal.h"

#define NS_SYSTEMPRINCIPAL_CLASSNAME "systemprincipal"
#define NS_SYSTEMPRINCIPAL_CID \
{ 0x4a6212db, 0xaccb, 0x11d3, \
{ 0xb7, 0x65, 0x0, 0x60, 0xb0, 0xb6, 0xce, 0xcb }}
#define NS_SYSTEMPRINCIPAL_CONTRACTID "@mozilla.org/systemprincipal;1"


class nsSystemPrincipal : public nsBasePrincipal {
public:
    
    NS_DECL_ISUPPORTS
    NS_DECL_NSISERIALIZABLE
    
    NS_IMETHOD ToString(char **result);

    NS_IMETHOD ToUserVisibleString(char **result);

    NS_IMETHOD Equals(nsIPrincipal *other, PRBool *result);

    NS_IMETHOD HashValue(PRUint32 *result);

    NS_IMETHOD CanEnableCapability(const char *capability, PRInt16 *result);

    NS_IMETHOD SetCanEnableCapability(const char *capability, 
                                      PRInt16 canEnable);

    NS_IMETHOD IsCapabilityEnabled(const char *capability, void * annotation, 
                                   PRBool *result);

    NS_IMETHOD EnableCapability(const char *capability, void * *annotation);

    NS_IMETHOD RevertCapability(const char *capability, void * *annotation);

    NS_IMETHOD DisableCapability(const char *capability, void * *annotation);

    NS_IMETHOD GetPreferences(char** aPrefName, char** aID, 
                              char** aGrantedList, char** aDeniedList);
    
    NS_IMETHOD Init();

    nsSystemPrincipal();

    virtual ~nsSystemPrincipal(void);
};

#endif // _NS_SYSTEM_PRINCIPAL_H_
