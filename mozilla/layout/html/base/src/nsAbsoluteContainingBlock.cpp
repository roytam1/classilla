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
#include "nsCOMPtr.h"
#include "nsAbsoluteContainingBlock.h"
#include "nsContainerFrame.h"
#include "nsReflowPath.h"
#include "nsIViewManager.h"
#include "nsLayoutAtoms.h"
#include "nsIPresShell.h"
#include "nsHTMLParts.h"
#include "nsIPresContext.h"
#include "nsIFrameManager.h"

#ifdef DEBUG
#include "nsBlockFrame.h"
#endif


nsresult
nsAbsoluteContainingBlock::FirstChild(const nsIFrame* aDelegatingFrame,
                                      nsIAtom*        aListName,
                                      nsIFrame**      aFirstChild) const
{
  NS_PRECONDITION(GetChildListName() == aListName, "unexpected child list name");
  *aFirstChild = mAbsoluteFrames.FirstChild();
  return NS_OK;
}

nsresult
nsAbsoluteContainingBlock::SetInitialChildList(nsIFrame*       aDelegatingFrame,
                                               nsIPresContext* aPresContext,
                                               nsIAtom*        aListName,
                                               nsIFrame*       aChildList)
{
  NS_PRECONDITION(GetChildListName() == aListName, "unexpected child list name");
#ifdef NS_DEBUG
  nsFrame::VerifyDirtyBitSet(aChildList);
#endif
  mAbsoluteFrames.SetFrames(aChildList);
  return NS_OK;
}

nsresult
nsAbsoluteContainingBlock::AppendFrames(nsIFrame*       aDelegatingFrame,
                                        nsIPresContext* aPresContext,
                                        nsIPresShell&   aPresShell,
                                        nsIAtom*        aListName,
                                        nsIFrame*       aFrameList)
{
  nsresult  rv = NS_OK;

  // Append the frames to our list of absolutely positioned frames
#ifdef NS_DEBUG
  nsFrame::VerifyDirtyBitSet(aFrameList);
#endif
  mAbsoluteFrames.AppendFrames(nsnull, aFrameList);

  // Generate a reflow command to reflow the dirty frames
  nsHTMLReflowCommand* reflowCmd;
  rv = NS_NewHTMLReflowCommand(&reflowCmd, aDelegatingFrame, eReflowType_ReflowDirty);
  if (NS_SUCCEEDED(rv)) {
    reflowCmd->SetChildListName(GetChildListName());
    aPresShell.AppendReflowCommand(reflowCmd);
  }

  return rv;
}

nsresult
nsAbsoluteContainingBlock::InsertFrames(nsIFrame*       aDelegatingFrame,
                                        nsIPresContext* aPresContext,
                                        nsIPresShell&   aPresShell,
                                        nsIAtom*        aListName,
                                        nsIFrame*       aPrevFrame,
                                        nsIFrame*       aFrameList)
{
  nsresult  rv = NS_OK;

  // Insert the new frames
#ifdef NS_DEBUG
  nsFrame::VerifyDirtyBitSet(aFrameList);
#endif
  mAbsoluteFrames.InsertFrames(nsnull, aPrevFrame, aFrameList);

  // Generate a reflow command to reflow the dirty frames
  nsHTMLReflowCommand* reflowCmd;
  rv = NS_NewHTMLReflowCommand(&reflowCmd, aDelegatingFrame, eReflowType_ReflowDirty);
  if (NS_SUCCEEDED(rv)) {
    reflowCmd->SetChildListName(GetChildListName());
    aPresShell.AppendReflowCommand(reflowCmd);
  }

  return rv;
}

nsresult
nsAbsoluteContainingBlock::RemoveFrame(nsIFrame*       aDelegatingFrame,
                                       nsIPresContext* aPresContext,
                                       nsIPresShell&   aPresShell,
                                       nsIAtom*        aListName,
                                       nsIFrame*       aOldFrame)
{
  PRBool result = mAbsoluteFrames.DestroyFrame(aPresContext, aOldFrame);
  NS_ASSERTION(result, "didn't find frame to delete");
  // Because positioned frames aren't part of a flow, there's no additional
  // work to do, e.g. reflowing sibling frames. And because positioned frames
  // have a view, we don't need to repaint
  return result ? NS_OK : NS_ERROR_FAILURE;
}

nsresult
nsAbsoluteContainingBlock::ReplaceFrame(nsIFrame*       aDelegatingFrame,
                                        nsIPresContext* aPresContext,
                                        nsIPresShell&   aPresShell,
                                        nsIAtom*        aListName,
                                        nsIFrame*       aOldFrame,
                                        nsIFrame*       aNewFrame)
{
  PRBool result = mAbsoluteFrames.ReplaceFrame(aPresContext, aDelegatingFrame,
                                               aOldFrame, aNewFrame, PR_TRUE);
  NS_ASSERTION(result, "Problems replacing a frame");
  return result ? NS_OK : NS_ERROR_FAILURE;
}

// bug 197581
#if(0)
// Destructor function for the collapse offset frame property
static void
DestroyRectFunc(nsIPresContext* aPresContext,
                nsIFrame*       aFrame,
                nsIAtom*        aPropertyName,
                void*           aPropertyValue)
{
  delete (nsRect*)aPropertyValue;
}

static nsRect*
GetOverflowAreaProperty(nsIPresContext* aPresContext,
                        nsIFrame*       aFrame,
                        PRBool          aCreateIfNecessary = PR_FALSE)
{
  nsCOMPtr<nsIPresShell>     presShell;
  aPresContext->GetShell(getter_AddRefs(presShell));

  if (presShell) {
    nsCOMPtr<nsIFrameManager>  frameManager;
    presShell->GetFrameManager(getter_AddRefs(frameManager));
  
    if (frameManager) {
      void* value;
  
      frameManager->GetFrameProperty(aFrame, nsLayoutAtoms::overflowAreaProperty,
                                     0, &value);
      if (value) {
        return (nsRect*)value;  // the property already exists

      } else if (aCreateIfNecessary) {
        // The property isn't set yet, so allocate a new rect, set the property,
        // and return the newly allocated rect
        nsRect*  overflow = new nsRect(0, 0, 0, 0);

        frameManager->SetFrameProperty(aFrame, nsLayoutAtoms::overflowAreaProperty,
                                       overflow, DestroyRectFunc);
        return overflow;
      }
    }
  }

  return nsnull;
}
#endif
// end bug

nsresult
nsAbsoluteContainingBlock::Reflow(nsIFrame*                aDelegatingFrame,
                                  nsIPresContext*          aPresContext,
                                  const nsHTMLReflowState& aReflowState,
                                  nscoord                  aContainingBlockWidth,
                                  nscoord                  aContainingBlockHeight,
                                  nsRect*                  aChildBounds)
{
  // Initialize OUT parameter
  if (aChildBounds)
    aChildBounds->SetRect(0, 0, 0, 0);

  // Make a copy of the reflow state.  If the reason is
  // eReflowReason_Incremental (which should mean either that the target
  // is the frame for which this is the absolute container or that the
  // container changed size due to incremental reflow of its children),
  // then change it to eReflowReason_Resize.
  // XXXldb If the target is this frame, shouldn't we be setting it
  // appropriately (which might mean to StyleChanged)?
  nsHTMLReflowState reflowState(aReflowState);
  if (eReflowReason_Incremental == reflowState.reason) {
    reflowState.reason = eReflowReason_Resize;
  }

  nsIFrame* kidFrame;
  for (kidFrame = mAbsoluteFrames.FirstChild(); nsnull != kidFrame; kidFrame->GetNextSibling(&kidFrame)) {
    nsReflowReason  reason = reflowState.reason;

    nsFrameState kidState;
    kidFrame->GetFrameState(&kidState);
    if (NS_FRAME_FIRST_REFLOW & kidState) {
      // The frame has never had a reflow, so change the reason to eReflowReason_Initial
      reason = eReflowReason_Initial;

    } else if (NS_FRAME_IS_DIRTY & kidState) {
      // The frame is dirty so give it the correct reflow reason
      reason = eReflowReason_Dirty;
    }

    // Reflow the frame
    nsReflowStatus  kidStatus;
    ReflowAbsoluteFrame(aDelegatingFrame, aPresContext, reflowState, aContainingBlockWidth,
                        aContainingBlockHeight, kidFrame, reason, kidStatus);

    if (aChildBounds) {
      // Add in the child's bounds
      nsRect  kidBounds;
      kidFrame->GetRect(kidBounds);
      aChildBounds->UnionRect(*aChildBounds, kidBounds);

      // If the frame has visible overflow, then take it into account, too.
      nsFrameState  kidFrameState;
      kidFrame->GetFrameState(&kidFrameState);
      if (kidFrameState & NS_FRAME_OUTSIDE_CHILDREN) {
        // Get the property
        // :: -> kF-> bug 197581
        nsRect* overflowArea = kidFrame->GetOverflowAreaProperty(aPresContext); // , kidFrame);

        if (overflowArea) {
          // The overflow area is in the child's coordinate space, so translate
          // it into the parent's coordinate space
          nsRect  rect(*overflowArea);

          rect.MoveBy(kidBounds.x, kidBounds.y);
          aChildBounds->UnionRect(*aChildBounds, rect);
        }
      }
    }
  }
  return NS_OK;
}

void
nsAbsoluteContainingBlock::CalculateChildBounds(nsIPresContext* aPresContext,
                                                nsRect&         aChildBounds)
{
  // Initialize the OUT parameters
  aChildBounds.SetRect(0, 0, 0, 0);

  for (nsIFrame* f = mAbsoluteFrames.FirstChild(); f; f->GetNextSibling(&f)) {
    // Add in the child's bounds
    nsRect  bounds;
    f->GetRect(bounds);
    aChildBounds.UnionRect(aChildBounds, bounds);
  
    // If the frame has visible overflow, then take it into account, too.
    nsFrameState  frameState;
    f->GetFrameState(&frameState);
    if (frameState & NS_FRAME_OUTSIDE_CHILDREN) {
      // Get the property
      // :: -> f-> bug 197581
      nsRect* overflowArea = f->GetOverflowAreaProperty(aPresContext); // , f);
  
      if (overflowArea) {
        // The overflow area is in the child's coordinate space, so translate
        // it into the parent's coordinate space
        nsRect  rect(*overflowArea);
  
        rect.MoveBy(bounds.x, bounds.y);
        aChildBounds.UnionRect(aChildBounds, rect);
      }
    }
  }
}

nsresult
nsAbsoluteContainingBlock::IncrementalReflow(nsIFrame*                aDelegatingFrame,
                                             nsIPresContext*          aPresContext,
                                             const nsHTMLReflowState& aReflowState,
                                             nscoord                  aContainingBlockWidth,
                                             nscoord                  aContainingBlockHeight,
                                             PRBool&                  aWasHandled)
{
  // Initialize the OUT parameters
  // Assume we handled the incremental reflow
  // until we find something that requires a full block reflow
  aWasHandled = PR_TRUE;

  // See if the reflow command is targeted at us.
  nsReflowPath *path = aReflowState.path;
  nsHTMLReflowCommand *command = path->mReflowCommand;

  if (command) {
    // It's targeted at us. See if it's for the positioned child frames
    nsCOMPtr<nsIAtom> listName;
    command->GetChildListName(*getter_AddRefs(listName));

    if (GetChildListName() == listName) {
      nsReflowType  type;

      // Get the type of reflow command
      command->GetType(type);

      // The only type of reflow command we expect is that we have dirty
      // child frames to reflow
      NS_ASSERTION(type == eReflowType_ReflowDirty, "unexpected reflow type");

      // Walk the positioned frames and reflow the dirty frames
      for (nsIFrame* f = mAbsoluteFrames.FirstChild(); f; f->GetNextSibling(&f)) {
        nsFrameState  frameState;

        f->GetFrameState(&frameState);
        if (frameState & NS_FRAME_IS_DIRTY) {
          nsReflowStatus  status;
          nsReflowReason  reason;

          reason = (frameState & NS_FRAME_FIRST_REFLOW)
            ? eReflowReason_Initial
            : eReflowReason_Dirty;

          ReflowAbsoluteFrame(aDelegatingFrame, aPresContext, aReflowState,
                              aContainingBlockWidth, aContainingBlockHeight, f,
                              reason, status);
        }
      }
    } else {
      // A reflow command is targeted directly at this block.
      // The block will have to do a proper reflow.
      // But keep going to make sure that we process any dirty absolute frames, below.
      aWasHandled = PR_FALSE;
    }
  }

  nsReflowPath::iterator iter = path->FirstChild();
  nsReflowPath::iterator end = path->EndChildren();

  if (iter != end && mAbsoluteFrames.NotEmpty()) {
    for ( ; iter != end; ++iter) {
      // See if it's one of our absolutely positioned child frames
      if (mAbsoluteFrames.ContainsFrame(*iter)) {
        // Remove the next frame from the reflow path
        nsReflowStatus kidStatus;
        ReflowAbsoluteFrame(aDelegatingFrame, aPresContext, aReflowState,
                            aContainingBlockWidth, aContainingBlockHeight, *iter,
                            aReflowState.reason, kidStatus);

        // We don't need to invalidate anything because the frame
        // should invalidate any area within its frame that needs
        // repainting, and because it has a view if it changes size
        // the view manager will damage the dirty area

        // Prune the path so we don't flow the block frame _again_
        // when returning to the caller.
        aReflowState.path->Remove(iter);
      }
      else {
        // At least one of the frames along the reflow path wasn't
        // absolutely positioned, so we'll need to deal with it in
        // normal block reflow.
        aWasHandled = PR_FALSE;
      }
    }
  }

  return NS_OK;
}

void
nsAbsoluteContainingBlock::DestroyFrames(nsIFrame*       aDelegatingFrame,
                                         nsIPresContext* aPresContext)
{
  mAbsoluteFrames.DestroyFrames(aPresContext);
}

// XXX Optimize the case where it's a resize reflow and the absolutely
// positioned child has the exact same size and position and skip the
// reflow...

// When bug 154892 is checked in, make sure that when 
// GetChildListName() == nsLayoutAtoms::fixedList, the height is unconstrained.
// since we don't allow replicated frames to split.

nsresult
nsAbsoluteContainingBlock::ReflowAbsoluteFrame(nsIFrame*                aDelegatingFrame,
                                               nsIPresContext*          aPresContext,
                                               const nsHTMLReflowState& aReflowState,
                                               nscoord                  aContainingBlockWidth,
                                               nscoord                  aContainingBlockHeight,
                                               nsIFrame*                aKidFrame,
                                               nsReflowReason           aReason,
                                               nsReflowStatus&          aStatus)
{
#ifdef DEBUG
  if (nsBlockFrame::gNoisyReflow) {
    nsFrame::IndentBy(stdout,nsBlockFrame::gNoiseIndent);
    printf("abs pos ");
    if (nsnull != aKidFrame) {
      nsIFrameDebug*  frameDebug;
      if (NS_SUCCEEDED(CallQueryInterface(aKidFrame, &frameDebug))) {
        nsAutoString name;
        frameDebug->GetFrameName(name);
        printf("%s ", NS_LossyConvertUCS2toASCII(name).get());
      }
    }
    printf("r=%d",aReflowState.reason);

    if (aReflowState.reason == eReflowReason_Incremental) {
      nsHTMLReflowCommand *command = aReflowState.path->mReflowCommand;

      if (command) {
        // We're the target.
        nsReflowType type;
        command->GetType(type);
        printf("(%d)", type);      
      }
    }
    char width[16];
    char height[16];
    PrettyUC(aReflowState.availableWidth, width);
    PrettyUC(aReflowState.availableHeight, height);
    printf(" a=%s,%s ", width, height);
    PrettyUC(aReflowState.mComputedWidth, width);
    PrettyUC(aReflowState.mComputedHeight, height);
    printf("c=%s,%s \n", width, height);
  }
  if (nsBlockFrame::gNoisy) {
    nsBlockFrame::gNoiseIndent++;
  }
#endif // DEBUG


  nsresult  rv;
  nsMargin  border;
  // Get the border values
  if (!aReflowState.mStyleBorder->GetBorder(border)) {
    NS_NOTYETIMPLEMENTED("percentage border");
  }
  
// It's not clear if this actually fixed anything, so I'm temporarily backing it out.
//#define BUG201897

#ifdef BUG201897
// backbugs from bug 201897 modified for 1.3.1
  nscoord availWidth = aReflowState.mComputedWidth;
  enum { NOT_SHRINK_TO_FIT, SHRINK_TO_FIT_AVAILWIDTH, SHRINK_TO_FIT_MEW };
  PRUint32 situation = NOT_SHRINK_TO_FIT;
  while (1) {
    nsHTMLReflowMetrics kidDesiredSize(nsnull);
    if (situation == NOT_SHRINK_TO_FIT) {
      // CSS2.1 10.3.7 width:auto and at least one of left/right is auto...
      //const nsStylePosition* stylePosition = aKidFrame->GetStylePosition();
      const nsStylePosition* stylePosition;
      aKidFrame->GetStyleData(eStyleStruct_Position, (const nsStyleStruct *&)stylePosition); // modified for 1.3
      if (stylePosition &&
      		eStyleUnit_Auto == stylePosition->mWidth.GetUnit() &&
          (eStyleUnit_Auto == stylePosition->mOffset.GetLeftUnit() ||
           eStyleUnit_Auto == stylePosition->mOffset.GetRightUnit())) {
        situation = SHRINK_TO_FIT_AVAILWIDTH;
        if (aContainingBlockWidth != -1) {
          availWidth = aContainingBlockWidth;
        } else {
          availWidth = aReflowState.mComputedWidth;
        }
        kidDesiredSize.mComputeMEW = PR_TRUE;
      }
    }
// end bug + backbugs
#endif

  nsFrameState        kidFrameState;
  nsSize              availSize(aReflowState.mComputedWidth, NS_UNCONSTRAINEDSIZE);
#ifndef BUG201897
  nsHTMLReflowMetrics kidDesiredSize(nsnull); // moved up above by bug 201897
#endif
  nsHTMLReflowState   kidReflowState(aPresContext, aReflowState, aKidFrame,
                                     availSize, aContainingBlockWidth,
                                     aContainingBlockHeight,
                                     aReason);

#ifdef BUG201897
    if (situation == SHRINK_TO_FIT_MEW) {
      situation = NOT_SHRINK_TO_FIT; // This is the last reflow
      kidReflowState.mComputedWidth = PR_MIN(availWidth, kidReflowState.mComputedMaxWidth);
      if (kidReflowState.mComputedWidth < kidReflowState.mComputedMinWidth) {
        kidReflowState.mComputedWidth = kidReflowState.mComputedMinWidth;
      }
    } else if (situation == SHRINK_TO_FIT_AVAILWIDTH) {
      PRInt32 maxWidth = availWidth -
        (kidReflowState.mComputedMargin.left + kidReflowState.mComputedBorderPadding.left +
         kidReflowState.mComputedBorderPadding.right + kidReflowState.mComputedMargin.right);
      if (NS_AUTOOFFSET != kidReflowState.mComputedOffsets.right) {
        maxWidth -= kidReflowState.mComputedOffsets.right;
      }
      if (NS_AUTOOFFSET != kidReflowState.mComputedOffsets.left) {
        maxWidth -= kidReflowState.mComputedOffsets.left;
      }
      // The following also takes care of maxWidth<0
      if (kidReflowState.mComputedMaxWidth > maxWidth) {
        kidReflowState.mComputedMaxWidth = PR_MAX(maxWidth, kidReflowState.mComputedMinWidth);
      }
    }
#endif

  // Send the WillReflow() notification and position the frame
  aKidFrame->WillReflow(aPresContext);

  // XXXldb We can simplify this if we come up with a better way to
  // position views.
  nscoord x;
  if (NS_AUTOOFFSET == kidReflowState.mComputedOffsets.left) {
    // Just use the current x-offset
    nsPoint origin;
    aKidFrame->GetOrigin(origin);
    x = origin.x;
  } else {
    x = border.left + kidReflowState.mComputedOffsets.left + kidReflowState.mComputedMargin.left;
  }
  aKidFrame->MoveTo(aPresContext, 
                    x, border.top + kidReflowState.mComputedOffsets.top + kidReflowState.mComputedMargin.top);

  // Position its view, but don't bother it doing it now if we haven't
  // yet determined the left offset
  if (NS_AUTOOFFSET != kidReflowState.mComputedOffsets.left) {
    nsContainerFrame::PositionFrameView(aPresContext, aKidFrame);
  }

  // Do the reflow
  rv = aKidFrame->Reflow(aPresContext, kidDesiredSize, kidReflowState, aStatus);
  
#ifdef BUG201897
// backbugs from bug 201897 + pull up to 1.8.1
    if (situation == SHRINK_TO_FIT_AVAILWIDTH) {
      // ...continued CSS2.1 10.3.7 width:auto and at least one of left/right is auto
      availWidth -= kidReflowState.mComputedMargin.left + kidReflowState.mComputedMargin.right;

      if (NS_AUTOOFFSET == kidReflowState.mComputedOffsets.right) {
        NS_ASSERTION(NS_AUTOOFFSET != kidReflowState.mComputedOffsets.left,
                     "Can't solve for both left and right");
        availWidth -= kidReflowState.mComputedOffsets.left;
      } else {
        NS_ASSERTION(NS_AUTOOFFSET == kidReflowState.mComputedOffsets.left,
                     "Expected to solve for left");
        availWidth -= kidReflowState.mComputedOffsets.right;
      }
      if (availWidth < 0) {
        availWidth = 0;
      }

      // Shrink-to-fit: min(max(preferred minimum width, available width), preferred width).
      // XXX this is not completely correct - see bug 201897 comment 56/58 and bug 268499.
      if (kidDesiredSize.mMaxElementWidth > availWidth) {
        aKidFrame->DidReflow(aPresContext, &kidReflowState, NS_FRAME_REFLOW_FINISHED);
        availWidth = PR_MAX(0, kidDesiredSize.mMaxElementWidth -
                               kidReflowState.mComputedBorderPadding.left -
                               kidReflowState.mComputedBorderPadding.right);
        situation = SHRINK_TO_FIT_MEW;
        aReason = eReflowReason_Resize;
        continue; // Do a second reflow constrained to MEW.
      }
    }
#endif

  // If we're solving for 'left' or 'top', then compute it now that we know the
  // width/height
  if ((NS_AUTOOFFSET == kidReflowState.mComputedOffsets.left) ||
      (NS_AUTOOFFSET == kidReflowState.mComputedOffsets.top)) {
    if (-1 == aContainingBlockWidth) {
      // Get the containing block width/height
      kidReflowState.ComputeContainingBlockRectangle(aPresContext,
                                                     &aReflowState,
                                                     aContainingBlockWidth,
                                                     aContainingBlockHeight);
    }

    if (NS_AUTOOFFSET == kidReflowState.mComputedOffsets.left) {
      kidReflowState.mComputedOffsets.left = aContainingBlockWidth -
                                             kidReflowState.mComputedOffsets.right -
                                             kidReflowState.mComputedMargin.right -
                                             kidDesiredSize.width -
                                             kidReflowState.mComputedMargin.left;
    }
    if (NS_AUTOOFFSET == kidReflowState.mComputedOffsets.top) {
      kidReflowState.mComputedOffsets.top = aContainingBlockHeight -
                                            kidReflowState.mComputedOffsets.bottom -
                                            kidReflowState.mComputedMargin.bottom -
                                            kidDesiredSize.height -
                                            kidReflowState.mComputedMargin.top;
    }
  }
    
  // Position the child relative to our padding edge
  nsRect  rect(border.left + kidReflowState.mComputedOffsets.left + kidReflowState.mComputedMargin.left,
               border.top + kidReflowState.mComputedOffsets.top + kidReflowState.mComputedMargin.top,
               kidDesiredSize.width, kidDesiredSize.height);
#if(0)               
// bug 6976
  if (rect.x < 0) {
    //const nsStyleDisplay *disp = aKidFrame->GetStyleDisplay();
    const nsStyleDisplay *disp;
    aKidFrame->GetStyleData(eStyleStruct_Display, (const nsStyleStruct*&)disp);
    //if (disp->mOverflowX != NS_STYLE_OVERFLOW_HIDDEN) { CSS 3
    if (disp->mOverflow != NS_STYLE_OVERFLOW_HIDDEN) { 
    // see nsStyleStruct.h for this
      // avoids a floater overflowing into the left of the window,
      // because this frame has scrolled area horizontally.
      rect.x = 0;
    }
  }
// end bug
#endif

  aKidFrame->SetRect(aPresContext, rect);

  // Size and position the view and set its opacity, visibility, content
  // transparency, and clip
  nsIView*  kidView;
  aKidFrame->GetView(aPresContext, &kidView);
  nsContainerFrame::SyncFrameViewAfterReflow(aPresContext, aKidFrame, kidView,
                                             &kidDesiredSize.mOverflowArea);
  aKidFrame->DidReflow(aPresContext, &kidReflowState, NS_FRAME_REFLOW_FINISHED);

  // If the frame has visible overflow, then store it as a property on the
  // frame. This allows us to be able to recover it without having to reflow
  // the frame
  aKidFrame->GetFrameState(&kidFrameState);
  if (kidFrameState & NS_FRAME_OUTSIDE_CHILDREN) {
    // Get the property (creating a rect struct if necessary)
    // :: -> aKF-> bug 197581
    nsRect* overflowArea = aKidFrame->GetOverflowAreaProperty(aPresContext, // aKidFrame,
    	 PR_TRUE);

    NS_ASSERTION(overflowArea, "should have created rect");
    if (overflowArea) {
      *overflowArea = kidDesiredSize.mOverflowArea;
    }
  }
#ifdef DEBUG
  if (nsBlockFrame::gNoisy) {
    nsBlockFrame::gNoiseIndent--;
  }
  if (nsBlockFrame::gNoisyReflow) {
    nsFrame::IndentBy(stdout,nsBlockFrame::gNoiseIndent);
    printf("abs pos ");
    if (nsnull != aKidFrame) {
      nsIFrameDebug*  frameDebug;
      if (NS_SUCCEEDED(CallQueryInterface(aKidFrame, &frameDebug))) {
        nsAutoString name;
        frameDebug->GetFrameName(name);
        printf("%s ", NS_LossyConvertUCS2toASCII(name).get());
      }
    }
    printf("%p rect=%d,%d,%d,%d", aKidFrame, rect.x, rect.y, rect.width, rect.height);
    printf("\n");
  }
#endif

#ifdef BUG201897
	break;
} // this ends the backbug while(1)
#endif
  return rv;
}

#ifdef DEBUG
 void nsAbsoluteContainingBlock::PrettyUC(nscoord aSize,
                        char*   aBuf)
{
  if (NS_UNCONSTRAINEDSIZE == aSize) {
    strcpy(aBuf, "UC");
  }
  else {
    if(0xdeadbeef == aSize)
    {
      strcpy(aBuf, "deadbeef");
    }
    else {
      sprintf(aBuf, "%d", aSize);
    }
  }
}
#endif
