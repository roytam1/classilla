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
#ifndef nsPrintData_h___
#define nsPrintData_h___

#include "nsPrintObject.h"

// Interfaces
#include "nsIDeviceContext.h"
#include "nsIDocument.h"
#include "nsIDOMWindow.h"
#include "nsIDOMWindowInternal.h"
#include "nsIObserver.h"
#include "nsIPrintProgress.h"
#include "nsIPrintProgressParams.h"
#include "nsIPrintOptions.h"
#include "nsIPrintSettings.h"
#include "nsIWebProgressListener.h"
#include "nsIPrintSession.h"

// Other Includes
#include "nsPrintPreviewListener.h"
#include "nsIDocShellTreeNode.h"

// Classes
class nsIPageSequenceFrame;
class nsPagePrintTimer;

// Special Interfaces
#include "nsIDocumentViewer.h"
#include "nsIDocumentViewerPrint.h"

//---------------------------------------------------
//-- Object for Caching the Presentation
//---------------------------------------------------
class CachedPresentationObj
{
public:
  CachedPresentationObj(nsIPresShell* aShell, nsIPresContext* aPC,
                        nsIViewManager* aVM, nsIWidget* aW):
    mWindow(aW), mViewManager(aVM), mPresShell(aShell), mPresContext(aPC)
  {
  }

  // The order here is important because the order of destruction is the
  // reverse of the order listed here, and the view manager must outlive
  // the pres shell.
  nsCOMPtr<nsIWidget>      mWindow;
  nsCOMPtr<nsIViewManager> mViewManager;
  nsCOMPtr<nsIPresShell>   mPresShell;
  nsCOMPtr<nsIPresContext> mPresContext;
};

//------------------------------------------------------------------------
// nsPrintData Class
//
// mPreparingForPrint - indicates that we have started Printing but 
//   have not gone to the timer to start printing the pages. It gets turned 
//   off right before we go to the timer.
//
// mDocWasToBeDestroyed - Gets set when "someone" tries to unload the document
//   while we were prparing to Print. This typically happens if a user starts 
//   to print while a page is still loading. If they start printing and pause 
//   at the print dialog and then the page comes in, we then abort printing 
//   because the document is no longer stable.
// 
//------------------------------------------------------------------------
class nsPrintData {
public:

  typedef enum {eIsPrinting, eIsPrintPreview } ePrintDataType;

  // This enum tells indicates what the default should be for the title
  // if the title from the document is null
  enum eDocTitleDefault {
    eDocTitleDefNone,
    eDocTitleDefBlank,
    eDocTitleDefURLDoc
  };


  nsPrintData(ePrintDataType aType);
  ~nsPrintData(); // non-virtual

  // Listener Helper Methods
  void OnEndPrinting();
  void OnStartPrinting();
  static void DoOnProgressChange(nsVoidArray& aListeners,
                                 PRInt32      aProgess,
                                 PRInt32      aMaxProgress,
                                 PRBool       aDoStartStop = PR_FALSE,
                                 PRInt32      aFlag = 0);


  //
  // The following three methods are used for printing...
  //
  nsresult DocumentReadyForPrinting();
  nsresult GetSelectionDocument(nsIDeviceContextSpec * aDevSpec,
                                nsIDocument ** aNewDoc);

  nsresult SetupToPrintContent(nsIWebShell*          aParent,
                               nsIDeviceContext*     aDContext,
                               nsIDOMWindowInternal* aCurrentFocusedDOMWin);
  nsresult EnablePOsForPrinting();
  nsPrintObject* FindXMostPO();
  void FindXMostFrameSize(nsIPresContext* aPresContext,
                          nsIRenderingContext* aRC, nsIFrame* aFrame,
                          nscoord aX, nscoord aY, PRInt32& aMaxWidth);
  void FindXMostFrameInList(nsIPresContext* aPresContext,
                            nsIRenderingContext* aRC, nsIAtom* aList,
                            nsIFrame* aFrame, nscoord aX, nscoord aY,
                            PRInt32& aMaxWidth);

  PRBool   PrintDocContent(nsPrintObject* aPO, nsresult& aStatus);
  nsresult DoPrint(nsPrintObject * aPO, PRBool aDoSyncPrinting,
                   PRBool& aDonePrinting);
  void SetPrintAsIs(nsPrintObject* aPO, PRBool aAsIs = PR_TRUE);

  enum ePrintFlags {eSetPrintFlag = 1U, eSetHiddenFlag = 2U };
  void SetPrintPO(nsPrintObject* aPO, PRBool aPrint, PRBool aIsHidden = PR_FALSE, PRUint32 aFlags = eSetPrintFlag);


  nsresult ShowDocList(nsPrintObject* aPO, PRBool aShow);
  void InstallNewPresentation();
  void ReturnToGalleyPresentation();
  void TurnScriptingOn(PRBool aDoTurnOn);
  PRBool CheckDocumentForPPCaching();
  void InstallPrintPreviewListener();

  // nsIDocumentViewerPrint Printing Methods
  PRBool   PrintPage(nsIPresContext* aPresContext,
                     nsIPrintSettings* aPrintSettings,
                     nsPrintObject* aPOect, PRBool& aInRange);
  PRBool   DonePrintingPages(nsPrintObject* aPO);

  //---------------------------------------------------------------------
  void BuildDocTree(nsIDocShellTreeNode * aParentNode,
                    nsVoidArray *         aDocList,
                    nsPrintObject *         aPO);
  nsresult ReflowDocList(nsPrintObject * aPO, PRBool aSetPixelScale,
                         PRBool aDoCalcShrink);
  void SetClipRect(nsPrintObject*  aPO,
                   const nsRect& aClipRect,
                   nscoord       aOffsetX,
                   nscoord       aOffsetY,
                   PRBool        aDoingSetClip);

  nsresult ReflowPrintObject(nsPrintObject * aPO, PRBool aDoCalcShrink);
  nsresult CalcPageFrameLocation(nsIPresShell * aPresShell,
                                  nsPrintObject*   aPO);
  nsPrintObject * FindPrintObjectByWS(nsPrintObject* aPO, nsIWebShell * aWebShell);
  void MapContentForPO(nsPrintObject*   aRootObject,
                       nsIPresShell*  aPresShell,
                       nsIContent*    aContent);
  void MapContentToWebShells(nsPrintObject* aRootPO, nsPrintObject* aPO);
  nsresult MapSubDocFrameLocations(nsPrintObject* aPO);

  void CalcNumPrintableDocsAndPages(PRInt32& aNumDocs, PRInt32& aNumPages);
  void DoProgressForAsIsFrames();
  void DoProgressForSeparateFrames();
  void ShowPrintProgress(PRBool aIsForPrinting, PRBool& aDoNotify);
  void CleanUpBeforeReflow(nsresult aResult);
  nsresult FinishPrintPreview();
  void CloseProgressDialog(nsIWebProgressListener* aWebProgressListener);

  void SetDocAndURLIntoProgress(nsPrintObject* aPO,
                                nsIPrintProgressParams* aParams);
  void ElipseLongString(PRUnichar *& aStr, const PRUint32 aLen, PRBool aDoFront);
  void CleanupDocTitleArray(PRUnichar**& aArray, PRInt32& aCount);
  void CheckForHiddenFrameSetFrames();

  PRBool   IsThereARangeSelection(nsIDOMWindowInternal * aDOMWin);

  //---------------------------------------------------------------------

#ifdef NS_PRINT_PREVIEW
  PRBool            mIsDoingPrintPreview; // per DocumentViewer
  nsIWidget*        mParentWidget; // purposely won't be ref counted
  nsPrintData*      mPrtPreview;
  nsPrintData*      mOldPrtPreview;
#endif

  nsIPresContext* mPresContext; // XXX 
  nsIDeviceContext* mDeviceContext; // XXX 

  // Timer Methods
  nsresult StartPagePrintTimer(nsIPresContext * aPresContext,
                               nsIPrintSettings* aPrintSettings,
                               nsPrintObject*     aPO,
                               PRUint32         aDelay);

  //---------------------------------------------------------------------
  // Static Methods
  //---------------------------------------------------------------------
  static nsresult FindFrameSetWithIID(nsIContent * aParentContent, const nsIID& aIID);
  static PRBool IsWindowsInOurSubTree(nsIDOMWindowInternal * aDOMWindow, nsISupports* aContainer);
  static PRBool IsWebShellAFrameSet(nsIWebShell * aParent);

  static void GetPresShellAndRootContent(nsIWebShell * aWebShell,
                                  nsIPresShell** aPresShell,
                                  nsIContent** aContent);

  static nsPrintObject* FindPrintObjectByDOMWin(nsPrintObject* aParentObject,
                                         nsIDOMWindowInternal * aDOMWin);

  // get the DOMWindow for a given WebShell
  static nsIDOMWindowInternal * GetDOMWinForWebShell(nsIWebShell* aWebShell);

  static void GetWebShellTitleAndURL(nsIWebShell* aWebShell,
                                     PRUnichar** aTitle, PRUnichar** aURLStr);

  static void GetDisplayTitleAndURL(nsPrintObject*      aPO, 
                                    nsIPrintSettings* aPrintSettings, 
                                    const PRUnichar*  aBrandName,
                                    PRUnichar**       aTitle, 
                                    PRUnichar**       aURLStr,
                                    eDocTitleDefault  aDefType = eDocTitleDefNone);
  static void ShowPrintErrorDialog(nsresult printerror,
                                   PRBool aIsPrinting = PR_TRUE);


  nsIDocumentViewerPrint*  mDocViewerPrint; // [WEAK] it owns me!
  nsIDocumentViewer*       mDocViewer; // [WEAK] it owns me!

  nsISupports* mContainer; // [WEAK] it owns me!
  nsCOMPtr<nsIDocument>    mDocument;

  ePrintDataType               mType;            // the type of data this is (Printing or Print Preview)
  nsCOMPtr<nsIDeviceContext>   mPrintDC;
  nsIView                     *mPrintView;
  FILE                        *mDebugFilePtr;    // a file where information can go to when printing

  nsPrintObject *                mPrintObject;
  nsPrintObject *                mSelectedPO;

  nsVoidArray                      mPrintProgressListeners;
  nsCOMPtr<nsIWebProgressListener> mPrintProgressListener;
  nsCOMPtr<nsIPrintProgress>       mPrintProgress;
  nsCOMPtr<nsIPrintProgressParams> mPrintProgressParams;
  PRBool                           mShowProgressDialog;    // means we should try to show it
  PRPackedBool                     mProgressDialogIsShown; // means it is already being shown

  nsCOMPtr<nsIDOMWindowInternal> mCurrentFocusWin; // cache a pointer to the currently focused window

  nsVoidArray*                mPrintDocList;
  nsCOMPtr<nsIDeviceContext>  mPrintDocDC;
  nsCOMPtr<nsIDOMWindow>      mPrintDocDW;
  PRPackedBool                mIsIFrameSelected;
  PRPackedBool                mIsParentAFrameSet;
  PRPackedBool                mPrintingAsIsSubDoc;
  PRPackedBool                mOnStartSent;
  PRPackedBool                mIsAborted;           // tells us the document is being aborted
  PRPackedBool                mPreparingForPrint;   // see comments above
  PRPackedBool                mDocWasToBeDestroyed; // see comments above
  PRBool                      mShrinkToFit;
  PRInt16                     mPrintFrameType;
  PRInt32                     mNumPrintableDocs;
  PRInt32                     mNumDocsPrinted;
  PRInt32                     mNumPrintablePages;
  PRInt32                     mNumPagesPrinted;
  float                       mShrinkRatio;
  float                       mOrigDCScale;
  float                       mOrigTextZoom;
  float                       mOrigZoom;

  nsCOMPtr<nsIPrintSession>   mPrintSession;
  nsCOMPtr<nsIPrintSettings>  mPrintSettings;
  nsCOMPtr<nsIPrintOptions>   mPrintOptions;
  nsPrintPreviewListener*     mPPEventListeners;

  PRUnichar*            mBrandName; //  needed as a substitute name for a document

  nsPagePrintTimer* mPagePrintTimer;
  nsIPageSequenceFrame* mPageSeqFrame;

private:
  nsPrintData() {}
  nsPrintData& operator=(const nsPrintData& aOther); // not implemented

};

#endif /* nsPrintData_h___ */

