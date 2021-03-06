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
 * The Original Code is Mozilla Communicator client code.
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

//
// Eric Vaughan
// Netscape Communications
//
// See documentation in associated header file
//

#include "nsFrameNavigator.h"
#include "nsCOMPtr.h"
#include "nsIContent.h"
#include "nsIBox.h"

void
nsFrameNavigator::GetTag(nsIBox* aBox, nsCOMPtr<nsIAtom>& aAtom)
{
   nsIFrame* frame = nsnull;
   aBox->GetFrame(&frame);

   nsCOMPtr<nsIContent> content;
   frame->GetContent(getter_AddRefs(content));
   if (content) {
     content->GetTag(*getter_AddRefs(aAtom));
     return; 
   }

   return;
}

nsIBox*
nsFrameNavigator::GetChildBeforeAfter(nsIPresContext* aPresContext, nsIBox* start, PRBool before)
{
   nsIBox* parent = nsnull;
   start->GetParentBox(&parent);
   PRInt32 index = IndexOf(aPresContext, parent,start);
   PRInt32 count = CountFrames(aPresContext, parent);

   if (index == -1) 
     return nsnull;

   if (before) {
     if (index == 0) {
         return nsnull;
     }

     return GetChildAt(aPresContext, parent, index-1);
   }


   if (index == count-1)
       return nsnull;

   return GetChildAt(aPresContext, parent, index+1);
}

PRInt32
nsFrameNavigator::IndexOf(nsIPresContext* aPresContext, nsIBox* parent, nsIBox* child)
{
  PRInt32 count = 0;

  nsIBox* box = nsnull;
  parent->GetChildBox(&box); 
  while (nsnull != box) 
  {    
    if (box == child)
       return count;

    nsresult rv = box->GetNextBox(&box);
    NS_ASSERTION(rv == NS_OK,"failed to get next child");
    count++;
  }

  return -1;
}

PRInt32
nsFrameNavigator::CountFrames(nsIPresContext* aPresContext, nsIBox* aBox)
{
  PRInt32 count = 0;

  nsIBox* box;
  aBox->GetChildBox(&box); 
  while (nsnull != box) 
  {    
    nsresult rv = box->GetNextBox(&box);
    NS_ASSERTION(rv == NS_OK,"failed to get next child");
    count++;
  }

  return count;
}

nsIBox*
nsFrameNavigator::GetChildAt(nsIPresContext* aPresContext, nsIBox* parent, PRInt32 index)
{
  PRInt32 count = 0;

  nsIBox* box;
  parent->GetChildBox(&box); 
  while (nsnull != box) 
  {    
    if (count == index)
       return box;

    nsresult rv = box->GetNextBox(&box);
    NS_ASSERTION(rv == NS_OK,"failed to get next child");
    count++;
  }

  return nsnull;
}

