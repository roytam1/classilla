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
 * The Original Code is mozilla.org code.
 *
 * The Initial Developer of the Original Code is 
 * Netscape Communications Corporation.
 * Portions created by the Initial Developer are Copyright (C) 1998
 * the Initial Developer. All Rights Reserved.
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

// The following constants define unicode subranges
// values below kRangeNum must be continuous so that we can map to 
// lang group directly.
// all ranges we care about should be defined under 32, that allows 
// us to store range using bits of a PRUint32

// frequently used range definitions
const PRUint8   kRangeCyrillic =    0;
const PRUint8   kRangeGreek    =    1;
const PRUint8   kRangeTurkish  =    2;
const PRUint8   kRangeHebrew   =    3;
const PRUint8   kRangeArabic   =    4;
const PRUint8   kRangeBaltic   =    5;
const PRUint8   kRangeThaiLao  =    6;
const PRUint8   kRangeKorean   =    7;
const PRUint8   kRangeJapanese =    8;
const PRUint8   kRangeSChinese =    9;
const PRUint8   kRangeTChinese =   10;

const PRUint8   kRangeSpecificItemNum =11;  

//range/rangeSet grow to this place 8-29

const PRUint8   kRangeSetStart  =  30;    // range set definition starts from here
const PRUint8   kRangeSetLatin  =  30;
const PRUint8   kRangeSetCJK    =  31;
const PRUint8   kRangeSetEnd    =  31;   // range set definition ends here

// less frequently used range definition
const PRUint8   kRangeSurrogate            = 32;
const PRUint8   kRangePrivate              = 33;
const PRUint8   kRangeMisc                 = 34;
const PRUint8   kRangeUnassigned           = 35;
const PRUint8   kRangeSriacThaana          = 36;
const PRUint8   kRangeDevanagariBengali    = 37;
const PRUint8   kRangeGurmukhiGujarati     = 38;
const PRUint8   kRangeOriyaTamil           = 39;
const PRUint8   kRangeTeluguKannada        = 40;
const PRUint8   kRangeMalayalamSinhala     = 41;
const PRUint8   kRangeTibetan              = 42;
const PRUint8   kRangeMyanmarGeorgian      = 43;
const PRUint8   kRangeEthiopic             = 44;
const PRUint8   kRangeEthiopicCherokee     = 45;
const PRUint8   kRangeAboriginal           = 46;
const PRUint8   kRangeAboriginalOghamRunic = 47;
const PRUint8   kRangeKhmer                = 48;
const PRUint8   kRangeMongolian            = 49;
const PRUint8   kRangeMathOperators        = 50;
const PRUint8   kRangeMiscTechical         = 51;
const PRUint8   kRangeControlOpticalEnclose = 52;
const PRUint8   kRangeBoxBlockGeometrics   = 53;
const PRUint8   kRangeMiscSymbols          = 54;
const PRUint8   kRangeDingbats             = 55;
const PRUint8   kRangeBraillePattern       = 56;
const PRUint8   kRangeYi                   = 57;
const PRUint8   kRangeCombiningDiacriticalMarks = 58;
const PRUint8   kRangeArmenian                = 59;

const PRUint8   kRangeTableBase   = 128;    //values over 127 are reserved for internal use only



extern PRUint32 FindCharUnicodeRange(PRUnichar ch);
extern const char* gUnicodeRangeToLangGroupTable[];

inline const char* LangGroupFromUnicodeRange(PRUint8 unicodeRange)
{
  if (kRangeSpecificItemNum > unicodeRange)  
    return gUnicodeRangeToLangGroupTable[unicodeRange];
  return nsnull;
}
