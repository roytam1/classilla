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
 *      John Gaunt (jgaunt@netscape.com) (original author)
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

#ifndef _nsHTMLWin32ObjectAccessible_H_
#define _nsHTMLWin32ObjectAccessible_H_

#include "nsIAccessibleWin32Object.h"
#include "nsAccessible.h"

struct IAccessible;

/**
  * This class is used only internally, we never! send out an IAccessible linked
  *   back to this object. This class is used to represent a plugin object when
  *   referenced as a child or sibling of another nsAccessible node. We need only
  *   a limited portion of the nsIAccessible interface implemented here. The
  *   in depth accessible information will be returned by the actual IAccessible
  *   object returned by us in Accessible::NewAccessible() that gets the IAccessible
  *   from the windows system from the window handle.
  */
class nsHTMLWin32ObjectAccessible : public nsAccessible,
                                    public nsIAccessibleWin32Object
{
public:

  nsHTMLWin32ObjectAccessible(nsIDOMNode* aNode, nsIWeakReference* aShell, PRInt32 aHwnd);
  virtual ~nsHTMLWin32ObjectAccessible() {}

  NS_DECL_ISUPPORTS_INHERITED

  // ---- nsIAccessibleWin32Object ----
  NS_IMETHOD GetHwnd(PRInt32 *aHwnd);

protected:
  // ---- Data Members ----
  /** 
    * A handle to the native plugin window (hopefully), given to 
    *   Accessible::NewAccessible() so the actual IAccessible can be retrieved.
    */
  PRInt32       mHwnd;
};

#endif  
