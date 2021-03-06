/*
 * The contents of this file are subject to the Mozilla Public License
 * Version 1.1 (the "License"); you may not use this file except in
 * compliance with the License. You may obtain a copy of the License at
 * http://www.mozilla.org/MPL/
 *
 * Software distributed under the License is distributed on an "AS IS"
 * basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See the
 * License for the specific language governing rights and limitations
 * under the License.
 *
 * The Original Code is the Mozilla OS/2 libraries.
 *
 * The Initial Developer of the Original Code is John Fairhurst,
 * <john_fairhurst@iname.com>.  Portions created by John Fairhurst are
 * Copyright (C) 1999 John Fairhurst. All Rights Reserved.
 *
 * Contributor(s):
 *   Henry Sobotka <sobotka@axess.com>
 *   IBM Corp.
 */

#include <unidef.h>
#include "nsDateTimeFormatOS2.h"

NS_IMPL_THREADSAFE_ISUPPORTS1(nsDateTimeFormatOS2,nsIDateTimeFormat)

#define NSDATETIME_FORMAT_BUFFER_LEN  80

#ifndef LOCI_iTime
#define LOCI_iTime   ((LocaleItem)73)
#endif

nsresult nsDateTimeFormatOS2::FormatTime(nsILocale* locale, 
                               const nsDateFormatSelector  dateFormatSelector, 
                               const nsTimeFormatSelector  timeFormatSelector, 
                               const time_t                timetTime,
                               nsString                   &stringOut)
{
  return FormatTMTime(locale, dateFormatSelector, timeFormatSelector, localtime( &timetTime ), stringOut);
}

// performs a locale sensitive date formatting operation on the struct tm parameter
nsresult nsDateTimeFormatOS2::FormatTMTime(nsILocale* locale, 
                               const nsDateFormatSelector  dateFormatSelector, 
                               const nsTimeFormatSelector  timeFormatSelector, 
                               const struct tm*            tmTime, 
                               nsString                   &stringOut)
{

  nsresult rc = NS_ERROR_FAILURE;
  UniChar uFmtD[NSDATETIME_FORMAT_BUFFER_LEN] = { 0 };
  UniChar uFmtT[NSDATETIME_FORMAT_BUFFER_LEN] = { 0 };
  UniChar *pString = NULL;
  LocaleObject locObj = NULL;
  int res = UniCreateLocaleObject(UNI_UCS_STRING_POINTER, (UniChar *)L"", &locObj);
  if (res != ULS_SUCCESS)
    return rc;  

  PRBool f24Hour = PR_FALSE;

  UniQueryLocaleItem(locObj, LOCI_iTime, &pString);

  if (pString[0] == '1') {
    f24Hour = PR_TRUE;
  } /* endif */

  // set date format
  switch (dateFormatSelector) {
    case kDateFormatNone:
      UniStrcat( uFmtD, (UniChar*)L"");
      break; 
    case kDateFormatLong:
    case kDateFormatShort:
      UniStrcat( uFmtD, (UniChar*)L"%x");
      break; 
    case kDateFormatYearMonth:
      UniQueryLocaleItem( locObj, DATESEP, &pString);
      UniStrcat( uFmtD, (UniChar*)L"%y");
      UniStrcat( uFmtD, pString);
      UniStrcat( uFmtD, (UniChar*)L"%m");
      UniFreeMem(pString);
      break; 
    case kDateFormatWeekday:
      UniStrcat( uFmtD, (UniChar*)L"%a");
      break;
    default: 
      UniStrcat( uFmtD, (UniChar*)L"");
  }

  // set time format
  switch (timeFormatSelector) {
    case kTimeFormatNone: 
      UniStrcat( uFmtT, (UniChar*)L"");
      break;
   case kTimeFormatSeconds:
      UniQueryLocaleItem( locObj, TIMESEP, &pString);
      if (f24Hour)
        UniStrcat( uFmtT, (UniChar*)L"%H");
      else
        UniStrcat( uFmtT, (UniChar*)L"%I");
      UniStrcat( uFmtT, pString);
      UniStrcat( uFmtT, (UniChar*)L"%M");
      UniStrcat( uFmtT, pString);
      UniStrcat( uFmtT, (UniChar*)L"%S");
      if (!f24Hour)
        UniStrcat( uFmtT, (UniChar*)L" %p");
      UniFreeMem(pString);
      break;
    case kTimeFormatNoSeconds:
      UniQueryLocaleItem( locObj, TIMESEP, &pString);
      if (f24Hour)
        UniStrcat( uFmtT, (UniChar*)L"%H");
      else
        UniStrcat( uFmtT, (UniChar*)L"%I");
      UniStrcat( uFmtT, pString);
      UniStrcat( uFmtT, (UniChar*)L"%M");
      if (!f24Hour)
        UniStrcat( uFmtT, (UniChar*)L" %p");
      UniFreeMem(pString);
      break;
    case kTimeFormatSecondsForce24Hour:
      UniQueryLocaleItem( locObj, TIMESEP, &pString);
      UniStrcat( uFmtT, (UniChar*)L"%H");
      UniStrcat( uFmtT, pString);
      UniStrcat( uFmtT, (UniChar*)L"%M");
      UniStrcat( uFmtT, pString);
      UniStrcat( uFmtT, (UniChar*)L"%S");
      UniFreeMem(pString);
      break;
    case kTimeFormatNoSecondsForce24Hour:
      UniQueryLocaleItem( locObj, TIMESEP, &pString);
      UniStrcat( uFmtT, (UniChar*)L"%H");
      UniStrcat( uFmtT, pString);
      UniStrcat( uFmtT, (UniChar*)L"%M");
      UniFreeMem(pString);
      break;  
    default: 
      UniStrcat( uFmtT, (UniChar*)L"");
  }

  PRUnichar buffer[NSDATETIME_FORMAT_BUFFER_LEN] = {0};
  if ((dateFormatSelector != kDateFormatNone) && (timeFormatSelector != kTimeFormatNone)) {
    UniStrcat( uFmtD, (UniChar*)L" ");
  } /* endif */
  UniStrcat( uFmtD, uFmtT);
  int length = UniStrftime(locObj, NS_REINTERPRET_CAST(UniChar *, buffer),
                           NSDATETIME_FORMAT_BUFFER_LEN, uFmtD, tmTime);
  UniFreeLocaleObject(locObj);

  if ( length != 0) {
    stringOut.Assign(buffer, length);
    rc = NS_OK;
  }
  
  return rc;
}

// performs a locale sensitive date formatting operation on the PRTime parameter
nsresult nsDateTimeFormatOS2::FormatPRTime(nsILocale* locale, 
                                           const nsDateFormatSelector  dateFormatSelector, 
                                           const nsTimeFormatSelector timeFormatSelector, 
                                           const PRTime  prTime, 
                                           nsString& stringOut)
{
  PRExplodedTime explodedTime;
  PR_ExplodeTime(prTime, PR_LocalTimeParameters, &explodedTime);

  return FormatPRExplodedTime(locale, dateFormatSelector, timeFormatSelector, &explodedTime, stringOut);
}

// performs a locale sensitive date formatting operation on the PRExplodedTime parameter
nsresult nsDateTimeFormatOS2::FormatPRExplodedTime(nsILocale* locale, 
                                                   const nsDateFormatSelector  dateFormatSelector, 
                                                   const nsTimeFormatSelector timeFormatSelector, 
                                                   const PRExplodedTime*  explodedTime, 
                                                   nsString& stringOut)
{
  struct tm  tmTime;
  /* be safe and set all members of struct tm to zero
   *
   * there are other fields in the tm struct that we aren't setting
   * (tm_isdst, tm_gmtoff, tm_zone, should we set these?) and since
   * tmTime is on the stack, it may be filled with garbage, but
   * the garbage may vary.  (this may explain why some saw bug #10412, and
   * others did not.
   *
   * when tmTime is passed to strftime() with garbage bad things may happen. 
   * see bug #10412
   */
  memset( &tmTime, 0, sizeof(tmTime) );

  tmTime.tm_yday = explodedTime->tm_yday;
  tmTime.tm_wday = explodedTime->tm_wday;
  tmTime.tm_year = explodedTime->tm_year;
  tmTime.tm_year -= 1900;
  tmTime.tm_mon = explodedTime->tm_month;
  tmTime.tm_mday = explodedTime->tm_mday;
  tmTime.tm_hour = explodedTime->tm_hour;
  tmTime.tm_min = explodedTime->tm_min;
  tmTime.tm_sec = explodedTime->tm_sec;

  return FormatTMTime(locale, dateFormatSelector, timeFormatSelector, &tmTime, stringOut);
}

