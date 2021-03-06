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
/* 
 * DO NOT EDIT THIS DOCUMENT MANUALLY !!!
 * THIS FILE IS AUTOMATICALLY GENERATED BY THE TOOLS UNDER
 *    mozilla/intl/chardet/tools/
 * Please contact ftang@netscape.com or mozilla-i18n@mozilla.org
 * if you have any question. Thanks
 */
#include "nsVerifier.h"
static PRUint32 EUCJP_cls [ 256 / 8 ] = {
PCK4BITS(4,4,4,4,4,4,4,4),  // 00 - 07 
PCK4BITS(4,4,4,4,4,4,5,5),  // 08 - 0f 
PCK4BITS(4,4,4,4,4,4,4,4),  // 10 - 17 
PCK4BITS(4,4,4,5,4,4,4,4),  // 18 - 1f 
PCK4BITS(4,4,4,4,4,4,4,4),  // 20 - 27 
PCK4BITS(4,4,4,4,4,4,4,4),  // 28 - 2f 
PCK4BITS(4,4,4,4,4,4,4,4),  // 30 - 37 
PCK4BITS(4,4,4,4,4,4,4,4),  // 38 - 3f 
PCK4BITS(4,4,4,4,4,4,4,4),  // 40 - 47 
PCK4BITS(4,4,4,4,4,4,4,4),  // 48 - 4f 
PCK4BITS(4,4,4,4,4,4,4,4),  // 50 - 57 
PCK4BITS(4,4,4,4,4,4,4,4),  // 58 - 5f 
PCK4BITS(4,4,4,4,4,4,4,4),  // 60 - 67 
PCK4BITS(4,4,4,4,4,4,4,4),  // 68 - 6f 
PCK4BITS(4,4,4,4,4,4,4,4),  // 70 - 77 
PCK4BITS(4,4,4,4,4,4,4,4),  // 78 - 7f 
PCK4BITS(5,5,5,5,5,5,5,5),  // 80 - 87 
PCK4BITS(5,5,5,5,5,5,1,3),  // 88 - 8f 
PCK4BITS(5,5,5,5,5,5,5,5),  // 90 - 97 
PCK4BITS(5,5,5,5,5,5,5,5),  // 98 - 9f 
PCK4BITS(5,2,2,2,2,2,2,2),  // a0 - a7 
PCK4BITS(2,2,2,2,2,2,2,2),  // a8 - af 
PCK4BITS(2,2,2,2,2,2,2,2),  // b0 - b7 
PCK4BITS(2,2,2,2,2,2,2,2),  // b8 - bf 
PCK4BITS(2,2,2,2,2,2,2,2),  // c0 - c7 
PCK4BITS(2,2,2,2,2,2,2,2),  // c8 - cf 
PCK4BITS(2,2,2,2,2,2,2,2),  // d0 - d7 
PCK4BITS(2,2,2,2,2,2,2,2),  // d8 - df 
PCK4BITS(0,0,0,0,0,0,0,0),  // e0 - e7 
PCK4BITS(0,0,0,0,0,0,0,0),  // e8 - ef 
PCK4BITS(0,0,0,0,0,0,0,0),  // f0 - f7 
PCK4BITS(0,0,0,0,0,0,0,5)   // f8 - ff 
};


static PRUint32 EUCJP_st [ 5] = {
PCK4BITS(     3,     4,     3,     5,eStart,eError,eError,eError),//00-07 
PCK4BITS(eError,eError,eError,eError,eItsMe,eItsMe,eItsMe,eItsMe),//08-0f 
PCK4BITS(eItsMe,eItsMe,eStart,eError,eStart,eError,eError,eError),//10-17 
PCK4BITS(eError,eError,eStart,eError,eError,eError,     3,eError),//18-1f 
PCK4BITS(     3,eError,eError,eError,eStart,eStart,eStart,eStart) //20-27 
};


static nsVerifier nsEUCJPVerifier = {
     "EUC-JP",
    {
       eIdxSft4bits, 
       eSftMsk4bits, 
       eBitSft4bits, 
       eUnitMsk4bits, 
       EUCJP_cls 
    },
    6,
    {
       eIdxSft4bits, 
       eSftMsk4bits, 
       eBitSft4bits, 
       eUnitMsk4bits, 
       EUCJP_st 
    }
};
