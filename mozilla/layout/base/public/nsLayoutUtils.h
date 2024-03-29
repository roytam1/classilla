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
 * The Original Code is Mozilla.org code.
 *
 * The Initial Developer of the Original Code is
 * Boris Zbarsky <bzbarsky@mit.edu>.
 * Portions created by the Initial Developer are Copyright (C) 2002
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

#ifndef nsLayoutUtils_h__
#define nsLayoutUtils_h__

class nsIFrame;
class nsIPresContext;

/**
 * nsLayoutUtils is a namespace class used for various helper
 * functions that are useful in multiple places in layout.  The goal
 * is not to define multiple copies of the same static helper.
 */
class nsLayoutUtils
{
public:
  /**
   * GetBeforeFrame returns the :before frame of the given frame, if
   * one exists.  This is typically O(1).  The frame passed in must be
   * the first-in-flow.   
   *
   * @param aFrame the frame whose :before is wanted
   * @param aPresContext the prescontext
   * @return the :before frame or nsnull if there isn't one
   */
  static nsIFrame* GetBeforeFrame(nsIFrame* aFrame, nsIPresContext* aPresContext);

  /**
   * GetAfterFrame returns the :after frame of the given frame, if one
   * exists.  This will walk the in-flow chain to the last-in-flow if
   * needed.  This function is typically O(N) in the number of child
   * frames, following in-flows, etc.
   *
   * @param aFrame the frame whose :after is wanted
   * @param aPresContext the prescontext
   * @return the :after frame or nsnull if there isn't one
   */
  static nsIFrame* GetAfterFrame(nsIFrame* aFrame, nsIPresContext* aPresContext);

  /** ---------------------------------------------------
   *  Giving a child frame it searches "up" the tree until it
   *  finds a "Page" frame.
   */
  static nsIFrame* GetPageFrame(nsIFrame* aFrame);
  
  // bug 209694
  // Combine aNewBreakType with aOrigBreakType, but limit the break types
  // to NS_STYLE_CLEAR_LEFT, RIGHT, LEFT_AND_RIGHT.
  static PRUint8 CombineBreakType(unsigned char aOrigBreakType, unsigned char aNewBreakType);
};

#endif // nsLayoutUtils_h__
