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

#include "nsUnicodeRange.h"

// This table depends on unicode range definitions. 
// Each item's index must correspond unicode range value
// eg. x-cyrillic = LangGroupTable[kRangeCyrillic]
const char *gUnicodeRangeToLangGroupTable[] = 
{
  "x-cyrillic",
  "el",
  "tr",
  "he",
  "ar",
  "x-baltic",
  "th",
  "ko",
  "ja",
  "zh-CN",
  "zh-TW",
};

/**********************************************************************
 * Unicode subranges as defined in unicode 3.0
 * x-western, x-central-euro, tr, x-baltic  -> latin 
 *  0000 - 036f 
 *  1e00 - 1eff
 *  2000 - 206f  (general punctuation)
 *  20a0 - 20cf  (currency symbols)
 *  2100 - 214f  (letterlike symbols)
 *  2150 - 218f  (Number Forms)
 * el         -> greek
 *  0370 - 03ff
 *  1f00 - 1fff
 * x-cyrillic -> cyrillic
 *  0400 - 04ff
 * he         -> hebrew
 *  0590 - 05ff
 * ar         -> arabic
 *  0600 - 06ff
 *  fb50 - fdff (arabic presentation forms)
 *  fe70 - feff (arabic presentation forms b)
 * th - thai
 *  0e00 - 0e7f
 * ko        -> korean
 *  ac00 - d7af  (hangul Syllables)
 *  1100 - 11ff    (jamo)
 *  3130 - 318f (hangul compatibility jamo)
 * ja
 *  3040 - 309f (hiragana)
 *  30a0 - 30ff (katakana)
 * zh-CN
 * zh-TW
 *
 * CJK
 *  3100 - 312f (bopomofo)
 *  31a0 - 31bf (bopomofo extended)
 *  3000 - 303f (CJK Symbols and Punctuation) 
 *  2e80 - 2eff (CJK radicals supplement)
 *  2f00 - 2fdf (Kangxi Radicals)
 *  2ff0 - 2fff (Ideographic Description Characters)
 *  3190 - 319f (kanbun)
 *  3200 - 32ff (Enclosed CJK letters and Months)
 *  3300 - 33ff (CJK compatibility)
 *  3400 - 4dbf (CJK Unified Ideographs Extension A)
 *  4e00 - 9faf (CJK Unified Ideographs)
 *  f900 - fa5f (CJK Compatibility Ideographs)
 *  fe30 - fe4f (CJK compatibility Forms)
 *  ff00 - ffef (halfwidth and fullwidth forms)
 *
 * Armenian
 *  0530 - 058f 
 * Sriac 
 *  0700 - 074f
 * Thaana
 *  0780 - 07bf
 * Devanagari
 *  0900 - 097f
 * Bengali
 *  0980 - 09ff
 * Gurmukhi
 *  0a00 - 0a7f
 * Gujarati
 *  0a80 - 0aff
 * Oriya
 *  0b00 - 0b7f
 * Tamil
 *  0b80 - 0bff
 * Telugu
 *  0c00 - 0c7f
 * Kannada
 *  0c80 - 0cff
 * Malayalam
 *  0d00 - 0d7f
 * Sinhala
 *  0d80 - 0def
 * Lao
 *  0e80 - 0eff
 * Tibetan
 *  0f00 - 0fbf
 * Myanmar
 *  1000 - 109f
 * Georgian
 *  10a0 - 10ff
 * Ethiopic
 *  1200 - 137f
 * Cherokee
 *  13a0 - 13ff
 * Canadian Aboriginal Syllabics
 *  1400 - 167f
 * Ogham
 *  1680 - 169f
 * Runic 
 *  16a0 - 16ff
 * Khmer
 *  1780 - 17ff
 * Mongolian
 *  1800 - 18af
 * Misc - superscripts and subscripts
 *  2070 - 209f
 * Misc - Combining Diacritical Marks for Symbols
 *  20d0 - 20ff
 * Misc - Arrows
 *  2190 - 21ff
 * Misc - Mathematical Operators
 *  2200 - 22ff
 * Misc - Miscellaneous Technical
 *  2300 - 23ff
 * Misc - Control picture
 *  2400 - 243f
 * Misc - Optical character recognition
 *  2440 - 2450
 * Misc - Enclose Alphanumerics
 *  2460 - 24ff
 * Misc - Box Drawing 
 *  2500 - 257f
 * Misc - Block Elements
 *  2580 - 259f
 * Misc - Geometric Shapes
 *  25a0 - 25ff
 * Misc - Miscellaneous Symbols
 *  2600 - 267f
 * Misc - Dingbats
 *  2700 - 27bf
 * Misc - Braille Patterns
 *  2800 - 28ff
 * Yi Syllables
 *  a000 - a48f
 * Yi radicals
 *  a490 - a4cf
 * Alphabetic Presentation Forms
 *  fb00 - fb4f
 * Misc - Combining half Marks
 *  fe20 - fe2f
 * Misc - small form variants
 *  fe50 - fe6f
 * Misc - Specials
 *  fff0 - ffff
 *********************************************************************/



#define NUM_OF_SUBTABLES      7
#define SUBTABLE_SIZE         16

static PRUint8 gUnicodeSubrangeTable[NUM_OF_SUBTABLES][SUBTABLE_SIZE] = 
{ 
  { // table for X---
    kRangeTableBase+1,  //u0xxx
    kRangeTableBase+2,  //u1xxx
    kRangeTableBase+3,  //u2xxx
    kRangeSetCJK,       //u3xxx
    kRangeSetCJK,       //u4xxx
    kRangeSetCJK,       //u5xxx
    kRangeSetCJK,       //u6xxx
    kRangeSetCJK,       //u7xxx
    kRangeSetCJK,       //u8xxx
    kRangeSetCJK,       //u9xxx
    kRangeTableBase+4,  //uaxxx
    kRangeKorean,       //ubxxx
    kRangeKorean,       //ucxxx
    kRangeTableBase+5,  //udxxx  
    kRangePrivate,      //uexxx
    kRangeTableBase+6   //ufxxx
  },
  { //table for 0X--
    kRangeSetLatin,          //u00xx
    kRangeSetLatin,          //u01xx
    kRangeSetLatin,          //u02xx
    kRangeGreek,             //u03xx     XXX 0300-036f is in fact kRangeCombiningDiacriticalMarks
    kRangeCyrillic,          //u04xx
    kRangeHebrew,            //u05xx     XXX 0530-058f is in fact kRangeArmenian
    kRangeArabic,            //u06xx
    kRangeSriacThaana,       //u07xx
    kRangeUnassigned,        //u08xx
    kRangeDevanagariBengali, //u09xx
    kRangeGurmukhiGujarati,  //u0axx
    kRangeOriyaTamil,        //u0bxx
    kRangeTeluguKannada,     //u0cxx
    kRangeMalayalamSinhala,  //u0dxx
    kRangeThaiLao,           //u0exx
    kRangeTibetan,           //u0fxx
  },
  { //table for 1x--
    kRangeMyanmarGeorgian,   //u10xx
    kRangeKorean,            //u11xx
    kRangeEthiopic,          //u12xx
    kRangeEthiopicCherokee,  //u13xx
    kRangeAboriginal,        //u14xx
    kRangeAboriginal,        //u15xx
    kRangeAboriginalOghamRunic, //u16xx
    kRangeKhmer,             //u17xx
    kRangeMongolian,         //u18xx
    kRangeUnassigned,        //u19xx
    kRangeUnassigned,        //u1axx
    kRangeUnassigned,        //u1bxx
    kRangeUnassigned,        //u1cxx
    kRangeUnassigned,        //u1dxx
    kRangeSetLatin,          //u1exx
    kRangeGreek,             //u1fxx
  },
  { //table for 2x--
    kRangeSetLatin,          //u20xx
    kRangeSetLatin,          //u21xx
    kRangeMathOperators,     //u22xx
    kRangeMiscTechical,      //u23xx
    kRangeControlOpticalEnclose, //u24xx
    kRangeBoxBlockGeometrics, //u25xx
    kRangeMiscSymbols,       //u26xx
    kRangeDingbats,          //u27xx
    kRangeBraillePattern,    //u28xx
    kRangeUnassigned,        //u29xx
    kRangeUnassigned,        //u2axx
    kRangeUnassigned,        //u2bxx
    kRangeUnassigned,        //u2cxx
    kRangeUnassigned,        //u2dxx
    kRangeSetCJK,            //u2exx
    kRangeSetCJK,            //u2fxx                      
  },
  {  //table for ax--
    kRangeYi,                //ua0xx
    kRangeYi,                //ua1xx
    kRangeYi,                //ua2xx
    kRangeYi,                //ua3xx
    kRangeYi,                //ua4xx
    kRangeUnassigned,        //ua5xx
    kRangeUnassigned,        //ua6xx
    kRangeUnassigned,        //ua7xx
    kRangeUnassigned,        //ua8xx
    kRangeUnassigned,        //ua9xx
    kRangeUnassigned,        //uaaxx
    kRangeUnassigned,        //uabxx
    kRangeKorean,            //uacxx
    kRangeKorean,            //uadxx
    kRangeKorean,            //uaexx
    kRangeKorean,            //uafxx
  },
  {  //table for dx--
    kRangeKorean,            //ud0xx
    kRangeKorean,            //ud1xx
    kRangeKorean,            //ud2xx
    kRangeKorean,            //ud3xx
    kRangeKorean,            //ud4xx
    kRangeKorean,            //ud5xx
    kRangeKorean,            //ud6xx
    kRangeKorean,            //ud7xx
    kRangeSurrogate,         //ud8xx
    kRangeSurrogate,         //ud9xx
    kRangeSurrogate,         //udaxx
    kRangeSurrogate,         //udbxx
    kRangeSurrogate,         //udcxx
    kRangeSurrogate,         //uddxx
    kRangeSurrogate,         //udexx
    kRangeSurrogate,         //udfxx
  },
  { // table for fx--
    kRangePrivate,           //uf0xx 
    kRangePrivate,           //uf1xx 
    kRangePrivate,           //uf2xx 
    kRangePrivate,           //uf3xx 
    kRangePrivate,           //uf4xx 
    kRangePrivate,           //uf5xx 
    kRangePrivate,           //uf6xx 
    kRangePrivate,           //uf7xx 
    kRangePrivate,           //uf8xx 
    kRangeSetCJK,            //uf9xx                      
    kRangeSetCJK,            //ufaxx                      
    kRangeArabic,            //ufbxx, includes alphabic presentation form
    kRangeArabic,            //ufcxx
    kRangeArabic,            //ufdxx
    kRangeArabic,            //ufexx, includes Combining half marks, 
                            //               CJK compatibility forms, 
                            //               CJK compatibility forms, 
                            //               small form variants
    
    kRangeSetCJK,            //uffxx, halfwidth and fullwidth forms, includes Special
  }
};

// A two level index is almost enough for locating a range, with the 
// exception of u03xx and u05xx. Since we don't really care about range for
// Armenian and combining diacritical marks in our font application, they are 
// not discriminated further. But future adoption of this module for other use 
// should be aware of this limitation. The implementation can be extended if 
// there is such a need.
PRUint32 FindCharUnicodeRange(PRUnichar ch)
{
  PRUint32 range;

  //search the first table
  range = gUnicodeSubrangeTable[0][ch >> 12];
  
  if (range < kRangeTableBase)
    // we try to get a specific range 
    return range;

  // otherwise, we have one more table to look at
  range = gUnicodeSubrangeTable[range - kRangeTableBase][(ch & 0x0f00) >> 8];

  return range;
}
