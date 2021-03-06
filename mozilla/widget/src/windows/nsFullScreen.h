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
#ifndef fullscreen___h___
#define fullscreen___h___

#include "nsIFullScreen.h"
#include "nsISupportsArray.h"
#include "nsISimpleEnumerator.h"
#include "nsString.h"
#include <windows.h>

#define NS_FULLSCREEN_SERVICE_CONTRACTID "@mozilla.org/browser/fullscreen;1"

// {F6DC257B-04F5-4d4c-B94F-88BCB52A9A46}
#define NS_FULLSCREEN_SERVICE_CID \
{ 0xf6dc257b, 0x4f5, 0x4d4c, { 0xb9, 0x4f, 0x88, 0xbc, 0xb5, 0x2a, 0x9a, 0x46 } }

/* Header file */
class nsFullScreen : public nsIFullScreen
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSIFULLSCREEN

  nsFullScreen();
  virtual ~nsFullScreen();
  /* additional members */

protected:
  void HideAllOSChrome(PRBool aVisbility);

protected:
  nsCOMPtr<nsISupportsArray> mOSChromeItems;
};

/* Header file */
class nsOSChromeItem : public nsIOSChromeItem
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSIOSCHROMEITEM

  nsOSChromeItem(char* aName);
  virtual ~nsOSChromeItem();

protected:
  nsCString mName;
  PRBool mIsHidden;
  RECT mItemRect;
};

#endif // fullscreen___h___
