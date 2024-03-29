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
#ifndef nsPagePrintTimer_h___
#define nsPagePrintTimer_h___

// Timer Includes
#include "nsITimer.h"
#include "nsITimelineService.h"

// Interfaces
#include "nsIDocumentViewerPrint.h"
#include "nsIPresContext.h"
#include "nsIPrintSettings.h"

// Other Includes
#include "nsPrintObject.h"
#include "nsRect.h"

class nsPrintEngine;

//---------------------------------------------------
//-- Page Timer Class
//---------------------------------------------------
class nsPagePrintTimer : public nsITimerCallback
{
public:

  NS_DECL_ISUPPORTS

  nsPagePrintTimer();
  virtual ~nsPagePrintTimer();

  // nsITimerCallback
  NS_DECL_NSITIMERCALLBACK

  // Other Methods
  nsresult StartTimer(PRBool aUseDelay = PR_TRUE);

  void Init(nsPrintEngine*          aPrintEngine,
            nsIDocumentViewerPrint* aDocViewerPrint,
            nsIPresContext*         aPresContext,
            nsIPrintSettings*       aPrintSettings,
            nsPrintObject*          aPO,
            PRUint32                aDelay);

  nsresult Start(nsPrintEngine*          aPrintEngine,
                 nsIDocumentViewerPrint* aDocViewerPrint,
                 nsIPresContext*     aPresContext,
                 nsIPrintSettings*   aPrintSettings,
                 nsPrintObject*      aPO,
                 PRUint32            aDelay);

  void Stop();
private:
  nsPrintEngine*             mPrintEngine;
  nsCOMPtr<nsIDocumentViewerPrint> mDocViewerPrint;
  nsIPresContext*            mPresContext;
  nsCOMPtr<nsIPrintSettings> mPrintSettings;
  nsCOMPtr<nsITimer>         mTimer;
  PRUint32                   mDelay;
  nsPrintObject *            mPrintObj;
};


extern nsresult NS_NewPagePrintTimer(nsPagePrintTimer **aResult);

#endif /* nsPagePrintTimer_h___ */
