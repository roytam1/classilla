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
 *   David Hyatt <hyatt@netscape.com>
 *   Daniel Glazman <glazman@netscape.com>
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
#include "nsCOMPtr.h"
#include "nsCSSRule.h"
#include "nsICSSStyleRule.h"
#include "nsICSSGroupRule.h"
#include "nsCSSDeclaration.h"
#include "nsICSSStyleSheet.h"
#include "nsICSSParser.h"
#include "nsICSSLoader.h"
#include "nsIHTMLContentContainer.h"
#include "nsIURL.h"
#include "nsIPresContext.h"
#include "nsIDocument.h"
#include "nsIDeviceContext.h"
#include "nsIArena.h"
#include "nsIAtom.h"
#include "nsCRT.h"
#include "nsString.h"
#include "nsStyleConsts.h"
#include "nsHTMLAtoms.h"
#include "nsUnitConversion.h"
#include "nsIFontMetrics.h"
#include "nsIDOMCSSStyleSheet.h"
// bug 188803
#include "nsICSSStyleRuleDOMWrapper.h"
//#include "nsIDOMCSSRule.h"
//#include "nsIDOMCSSStyleRule.h"
#include "nsIDOMCSSStyleDeclaration.h"
#include "nsDOMCSSDeclaration.h"
#include "nsINameSpaceManager.h"
#include "nsINameSpace.h"
#include "nsILookAndFeel.h"
#include "nsRuleNode.h"
#include "nsUnicharUtils.h"

#include "nsIStyleSet.h"
#include "nsISizeOfHandler.h"

#include "nsContentUtils.h"

// bug 125246
#if(0)
// #define DEBUG_REFS
 
static NS_DEFINE_IID(kISupportsIID, NS_ISUPPORTS_IID);

static NS_DEFINE_IID(kCSSFontSID, NS_CSS_FONT_SID);
static NS_DEFINE_IID(kCSSColorSID, NS_CSS_COLOR_SID);
static NS_DEFINE_IID(kCSSTextSID, NS_CSS_TEXT_SID);
static NS_DEFINE_IID(kCSSMarginSID, NS_CSS_MARGIN_SID);
static NS_DEFINE_IID(kCSSPositionSID, NS_CSS_POSITION_SID);
static NS_DEFINE_IID(kCSSListSID, NS_CSS_LIST_SID);
static NS_DEFINE_IID(kCSSDisplaySID, NS_CSS_DISPLAY_SID);
static NS_DEFINE_IID(kCSSTableSID, NS_CSS_TABLE_SID);
static NS_DEFINE_IID(kCSSContentSID, NS_CSS_CONTENT_SID);
static NS_DEFINE_IID(kCSSUserInterfaceSID, NS_CSS_USER_INTERFACE_SID);
#ifdef INCLUDE_XUL
static NS_DEFINE_IID(kCSSXULSID, NS_CSS_XUL_SID);
#endif
#ifdef MOZ_SVG
static NS_DEFINE_IID(kCSSSVGSID, NS_CSS_SVG_SID);
#endif
#endif
// end bug

// -- nsCSSSelector -------------------------------

#define NS_IF_COPY(dest,source,type)  \
  if (nsnull != source)  dest = new type(*(source))

#define NS_IF_DELETE(ptr)   \
  if (nsnull != ptr) { delete ptr; ptr = nsnull; }

#define NS_IF_NEGATED_START(bool,str)  \
  if (bool) { str.Append(NS_LITERAL_STRING(":not(")); }

#define NS_IF_NEGATED_END(bool,str)  \
  if (bool) { str.Append(PRUnichar(')')); }

MOZ_DECL_CTOR_COUNTER(nsAtomList)

nsAtomList::nsAtomList(nsIAtom* aAtom)
  : mAtom(aAtom),
    mNext(nsnull)
{
  MOZ_COUNT_CTOR(nsAtomList);
}

nsAtomList::nsAtomList(const nsString& aAtomValue)
  : mAtom(nsnull),
    mNext(nsnull)
{
  MOZ_COUNT_CTOR(nsAtomList);
  mAtom = do_GetAtom(aAtomValue);
}

nsAtomList::nsAtomList(const nsAtomList& aCopy)
  : mAtom(aCopy.mAtom),
    mNext(nsnull)
{
  MOZ_COUNT_CTOR(nsAtomList);
  NS_IF_COPY(mNext, aCopy.mNext, nsAtomList);
}

nsAtomList::~nsAtomList(void)
{
  MOZ_COUNT_DTOR(nsAtomList);
  NS_IF_DELETE(mNext);
}

PRBool nsAtomList::Equals(const nsAtomList* aOther) const
{
  if (this == aOther) {
    return PR_TRUE;
  }
  if (nsnull != aOther) {
    if (mAtom == aOther->mAtom) {
      if (nsnull != mNext) {
        return mNext->Equals(aOther->mNext);
      }
      return PRBool(nsnull == aOther->mNext);
    }
  }
  return PR_FALSE;
}

MOZ_DECL_CTOR_COUNTER(nsAtomStringList)

nsAtomStringList::nsAtomStringList(nsIAtom* aAtom, const PRUnichar* aString)
  : mAtom(aAtom),
    mString(nsnull),
    mNext(nsnull)
{
  MOZ_COUNT_CTOR(nsAtomStringList);
  if (aString)
    mString = nsCRT::strdup(aString);
}

nsAtomStringList::nsAtomStringList(const nsString& aAtomValue,
                                   const PRUnichar* aString)
  : mAtom(nsnull),
    mString(nsnull),
    mNext(nsnull)
{
  MOZ_COUNT_CTOR(nsAtomStringList);
  mAtom = do_GetAtom(aAtomValue);
  if (aString)
    mString = nsCRT::strdup(aString);
}

nsAtomStringList::nsAtomStringList(const nsAtomStringList& aCopy)
  : mAtom(aCopy.mAtom),
    mString(nsnull),
    mNext(nsnull)
{
  MOZ_COUNT_CTOR(nsAtomStringList);
  if (aCopy.mString)
    mString = nsCRT::strdup(aCopy.mString);
  NS_IF_COPY(mNext, aCopy.mNext, nsAtomStringList);
}

nsAtomStringList::~nsAtomStringList(void)
{
  MOZ_COUNT_DTOR(nsAtomStringList);
  if (mString)
    nsCRT::free(mString);
  NS_IF_DELETE(mNext);
}

PRBool nsAtomStringList::Equals(const nsAtomStringList* aOther) const
{
  return (this == aOther) ||
         (aOther &&
          mAtom == aOther->mAtom &&
          !mString == !aOther->mString &&
          !mNext == !aOther->mNext &&
          (!mNext || mNext->Equals(aOther->mNext)) &&
          // Check strings last, since it's the slowest check.
          (!mString || nsDependentString(mString).Equals(
                                        nsDependentString(aOther->mString),
                                        nsCaseInsensitiveStringComparator())));
}

MOZ_DECL_CTOR_COUNTER(nsAttrSelector)

// bug 125246
#if(0)
#ifdef DEBUG_REFS
PRUint32 gAttrSelectorCount=0;
#endif
#endif
// end bug

nsAttrSelector::nsAttrSelector(PRInt32 aNameSpace, const nsString& aAttr)
  : mNameSpace(aNameSpace),
    mAttr(nsnull),
    mFunction(NS_ATTR_FUNC_SET),
    mCaseSensitive(1),
    mValue(),
    mNext(nsnull)
{
  MOZ_COUNT_CTOR(nsAttrSelector);

// bug 125246
#if(0)
#ifdef DEBUG_REFS
  gAttrSelectorCount++;
  printf( "nsAttrSelector Instances (ctor): %ld\n", (long)gAttrSelectorCount);
#endif
#endif
// end bug

  mAttr = NS_NewAtom(aAttr);
}

nsAttrSelector::nsAttrSelector(PRInt32 aNameSpace, const nsString& aAttr, PRUint8 aFunction, 
                               const nsString& aValue, PRBool aCaseSensitive)
  : mNameSpace(aNameSpace),
    mAttr(nsnull),
    mFunction(aFunction),
    mCaseSensitive(aCaseSensitive),
    mValue(aValue),
    mNext(nsnull)
{
  MOZ_COUNT_CTOR(nsAttrSelector);

// bug 125246
#if(0)
#ifdef DEBUG_REFS
  gAttrSelectorCount++;
  printf( "nsAttrSelector Instances (ctor): %ld\n", (long)gAttrSelectorCount);
#endif
#endif
// end bug

  mAttr = NS_NewAtom(aAttr);
}

nsAttrSelector::nsAttrSelector(const nsAttrSelector& aCopy)
  : mNameSpace(aCopy.mNameSpace),
    mAttr(aCopy.mAttr),
    mFunction(aCopy.mFunction),
    mCaseSensitive(aCopy.mCaseSensitive),
    mValue(aCopy.mValue),
    mNext(nsnull)
{
  MOZ_COUNT_CTOR(nsAttrSelector);

// bug 125246
#if(0)
#ifdef DEBUG_REFS
  gAttrSelectorCount++;
  printf( "nsAttrSelector Instances (cp-ctor): %ld\n", (long)gAttrSelectorCount);
#endif
#endif
// end bug

  NS_IF_ADDREF(mAttr);
  NS_IF_COPY(mNext, aCopy.mNext, nsAttrSelector);
}

nsAttrSelector::~nsAttrSelector(void)
{
  MOZ_COUNT_DTOR(nsAttrSelector);

// bug 125246
#if(0)
#ifdef DEBUG_REFS
  gAttrSelectorCount--;
  printf( "nsAttrSelector Instances (dtor): %ld\n", (long)gAttrSelectorCount);
#endif
#endif

  NS_IF_RELEASE(mAttr);
  NS_IF_DELETE(mNext);
}

PRBool nsAttrSelector::Equals(const nsAttrSelector* aOther) const
{
  if (this == aOther) {
    return PR_TRUE;
  }
  if (nsnull != aOther) {
    if ((mNameSpace == aOther->mNameSpace) &&
        (mAttr == aOther->mAttr) && 
        (mFunction == aOther->mFunction) && 
        (mCaseSensitive == aOther->mCaseSensitive) &&
        mValue.Equals(aOther->mValue)) {
      if (nsnull != mNext) {
        return mNext->Equals(aOther->mNext);
      }
      return PRBool(nsnull == aOther->mNext);
    }
  }
  return PR_FALSE;
}

#ifdef DEBUG
/******************************************************************************
* SizeOf method:
*
*  Self (reported as nsAttrSelector's size): 
*    1) sizeof(*this) + the size of mAttr atom (if it exists and is unique)
*
*  Contained / Aggregated data (not reported as nsAttrSelector's size):
*    none
*
*  Children / siblings / parents:
*    1) Recurses to the mMext instance which is reported as a seperate instance
*    
******************************************************************************/
void nsAttrSelector::SizeOf(nsISizeOfHandler *aSizeOfHandler, PRUint32 &aSize)
{
  NS_ASSERTION(aSizeOfHandler != nsnull, "SizeOf handler cannot be null");

  // first get the unique items collection
  UNIQUE_STYLE_ITEMS(uniqueItems);
  if(! uniqueItems->AddItem((void*)this)){
    return;
  }

  PRUint32 localSize=0;

  // create a tag for this instance
  nsCOMPtr<nsIAtom> tag = do_GetAtom("nsAttrSelector");
  // get the size of an empty instance and add to the sizeof handler
  aSize = sizeof(*this);

  // add in the mAttr atom
  if (mAttr && uniqueItems->AddItem(mAttr)){
    mAttr->SizeOf(aSizeOfHandler, &localSize);
    aSize += localSize;
  }
  aSizeOfHandler->AddSize(tag,aSize);

  // recurse to the next one...
  if(mNext){
    mNext->SizeOf(aSizeOfHandler, localSize);
  }
}
#endif

MOZ_DECL_CTOR_COUNTER(nsCSSSelector)

// bug 125246
#if(0)
#ifdef DEBUG_REFS
PRUint32 gSelectorCount=0;
#endif
#endif

nsCSSSelector::nsCSSSelector(void)
  : mNameSpace(kNameSpaceID_Unknown), mTag(nsnull), 
    mIDList(nsnull), 
    mClassList(nsnull), 
    mPseudoClassList(nsnull),
    mAttrList(nsnull), 
    mOperator(0),
    mNegations(nsnull),
    mNext(nsnull)
{
  MOZ_COUNT_CTOR(nsCSSSelector);

// bug 125246
#if(0)
#ifdef DEBUG_REFS
  gSelectorCount++;
  printf( "nsCSSSelector Instances (ctor): %ld\n", (long)gSelectorCount);
#endif
#endif
// end bug
}

nsCSSSelector::nsCSSSelector(const nsCSSSelector& aCopy) 
  : mNameSpace(aCopy.mNameSpace), mTag(aCopy.mTag), 
    mIDList(nsnull), 
    mClassList(nsnull), 
    mPseudoClassList(nsnull),
    mAttrList(nsnull), 
    mOperator(aCopy.mOperator),
    mNegations(nsnull),
    mNext(nsnull)
{
  MOZ_COUNT_CTOR(nsCSSSelector);
  NS_IF_COPY(mIDList, aCopy.mIDList, nsAtomList);
  NS_IF_COPY(mClassList, aCopy.mClassList, nsAtomList);
  NS_IF_COPY(mPseudoClassList, aCopy.mPseudoClassList, nsAtomStringList);
  NS_IF_COPY(mAttrList, aCopy.mAttrList, nsAttrSelector);
  NS_IF_COPY(mNegations, aCopy.mNegations, nsCSSSelector);

// bug 125246
#if(0)  
#ifdef DEBUG_REFS
  gSelectorCount++;
  printf( "nsCSSSelector Instances (cp-ctor): %ld\n", (long)gSelectorCount);
#endif
#endif
// end bug
}

nsCSSSelector::~nsCSSSelector(void)  
{
  MOZ_COUNT_DTOR(nsCSSSelector);
  Reset();

// bug 125246
#if(0)
#ifdef DEBUG_REFS
  gSelectorCount--;
  printf( "nsCSSSelector Instances (dtor): %ld\n", (long)gSelectorCount);
#endif
#endif
// end bug
}

nsCSSSelector& nsCSSSelector::operator=(const nsCSSSelector& aCopy)
{
  NS_IF_DELETE(mIDList);
  NS_IF_DELETE(mClassList);
  NS_IF_DELETE(mPseudoClassList);
  NS_IF_DELETE(mAttrList);
  NS_IF_DELETE(mNegations);
  
  mNameSpace    = aCopy.mNameSpace;
  mTag          = aCopy.mTag;
  NS_IF_COPY(mIDList, aCopy.mIDList, nsAtomList);
  NS_IF_COPY(mClassList, aCopy.mClassList, nsAtomList);
  NS_IF_COPY(mPseudoClassList, aCopy.mPseudoClassList, nsAtomStringList);
  NS_IF_COPY(mAttrList, aCopy.mAttrList, nsAttrSelector);
  mOperator     = aCopy.mOperator;
  NS_IF_COPY(mNegations, aCopy.mNegations, nsCSSSelector);

  return *this;
}

PRBool nsCSSSelector::Equals(const nsCSSSelector* aOther) const
{
  if (this == aOther) {
    return PR_TRUE;
  }
  if (nsnull != aOther) {
    if ((aOther->mNameSpace == mNameSpace) && 
        (aOther->mTag == mTag) && 
        (aOther->mOperator == mOperator)) {
      if (nsnull != mIDList) {
        if (PR_FALSE == mIDList->Equals(aOther->mIDList)) {
          return PR_FALSE;
        }
      }
      else {
        if (nsnull != aOther->mIDList) {
          return PR_FALSE;
        }
      }
      if (nsnull != mClassList) {
        if (PR_FALSE == mClassList->Equals(aOther->mClassList)) {
          return PR_FALSE;
        }
      }
      else {
        if (nsnull != aOther->mClassList) {
          return PR_FALSE;
        }
      }
      if (nsnull != mPseudoClassList) {
        if (PR_FALSE == mPseudoClassList->Equals(aOther->mPseudoClassList)) {
          return PR_FALSE;
        }
      }
      else {
        if (nsnull != aOther->mPseudoClassList) {
          return PR_FALSE;
        }
      }
      if (nsnull != mAttrList) {
        if (PR_FALSE == mAttrList->Equals(aOther->mAttrList)) {
          return PR_FALSE;
        }
      }
      else {
        if (nsnull != aOther->mAttrList) {
          return PR_FALSE;
        }
      }
      if (nsnull != mNegations) {
        if (PR_FALSE == mNegations->Equals(aOther->mNegations)) {
          return PR_FALSE;
        }
      }
      return PR_TRUE;
    }
  }
  return PR_FALSE;
}


void nsCSSSelector::Reset(void)
{
  mNameSpace = kNameSpaceID_Unknown;
  mTag = nsnull;
  NS_IF_DELETE(mIDList);
  NS_IF_DELETE(mClassList);
  NS_IF_DELETE(mPseudoClassList);
  NS_IF_DELETE(mAttrList);
  NS_IF_DELETE(mNegations);
  mOperator = PRUnichar(0);
}

void nsCSSSelector::SetNameSpace(PRInt32 aNameSpace)
{
  mNameSpace = aNameSpace;
}

void nsCSSSelector::SetTag(const nsString& aTag)
{
  if (aTag.IsEmpty())
    mTag = nsnull;
  else
    mTag = do_GetAtom(aTag);
}

void nsCSSSelector::AddID(const nsString& aID)
{
  if (!aID.IsEmpty()) {
    nsAtomList** list = &mIDList;
    while (nsnull != *list) {
      list = &((*list)->mNext);
    }
    *list = new nsAtomList(aID);
  }
}

void nsCSSSelector::AddClass(const nsString& aClass)
{
  if (!aClass.IsEmpty()) {
    nsAtomList** list = &mClassList;
    while (nsnull != *list) {
      list = &((*list)->mNext);
    }
    *list = new nsAtomList(aClass);
  }
}

void nsCSSSelector::AddPseudoClass(const nsString& aPseudoClass,
                                   const PRUnichar* aString)
{
  if (!aPseudoClass.IsEmpty()) {
    nsAtomStringList** list = &mPseudoClassList;
    while (nsnull != *list) {
      list = &((*list)->mNext);
    }
    *list = new nsAtomStringList(aPseudoClass, aString);
  }
}

void nsCSSSelector::AddPseudoClass(nsIAtom* aPseudoClass,
                                   const PRUnichar* aString)
{
  if (nsnull != aPseudoClass) {
    nsAtomStringList** list = &mPseudoClassList;
    while (nsnull != *list) {
      list = &((*list)->mNext);
    }
    *list = new nsAtomStringList(aPseudoClass, aString);
  }
}

void nsCSSSelector::AddAttribute(PRInt32 aNameSpace, const nsString& aAttr)
{
  if (!aAttr.IsEmpty()) {
    nsAttrSelector** list = &mAttrList;
    while (nsnull != *list) {
      list = &((*list)->mNext);
    }
    *list = new nsAttrSelector(aNameSpace, aAttr);
  }
}

void nsCSSSelector::AddAttribute(PRInt32 aNameSpace, const nsString& aAttr, PRUint8 aFunc, 
                                 const nsString& aValue, PRBool aCaseSensitive)
{
  if (!aAttr.IsEmpty()) {
    nsAttrSelector** list = &mAttrList;
    while (nsnull != *list) {
      list = &((*list)->mNext);
    }
    *list = new nsAttrSelector(aNameSpace, aAttr, aFunc, aValue, aCaseSensitive);
  }
}

void nsCSSSelector::SetOperator(PRUnichar aOperator)
{
  mOperator = aOperator;
}

PRInt32 nsCSSSelector::CalcWeight(void) const
{
  PRInt32 weight = 0;

  if (nsnull != mTag) {
    weight += 0x000001;
  }
  nsAtomList* list = mIDList;
  while (nsnull != list) {
    weight += 0x010000;
    list = list->mNext;
  }
  list = mClassList;
  while (nsnull != list) {
    weight += 0x000100;
    list = list->mNext;
  }
  nsAtomStringList *plist = mPseudoClassList;
  while (nsnull != plist) {
    weight += 0x000100;
    plist = plist->mNext;
  }
  nsAttrSelector* attr = mAttrList;
  while (nsnull != attr) {
    weight += 0x000100;
    attr = attr->mNext;
  }
  if (nsnull != mNegations) {
    weight += mNegations->CalcWeight();
  }
  return weight;
}

#ifdef DEBUG
/******************************************************************************
* SizeOf method:
*
*  Self (reported as nsCSSSelector's size): 
*    1) sizeof(*this) + the size of the mTag 
*       + the size of the mIDList unique items 
*       + the size of the mClassList and mPseudoClassList unique items
*
*  Contained / Aggregated data (not reported as nsCSSSelector's size):
*    1) AttributeList is called out to seperately if it exists
*
*  Children / siblings / parents:
*    1) Recurses to mNext which is counted as it's own instance
*    
******************************************************************************/
void nsCSSSelector::SizeOf(nsISizeOfHandler *aSizeOfHandler, PRUint32 &aSize)
{
  NS_ASSERTION(aSizeOfHandler != nsnull, "SizeOf handler cannot be null");

  // first get the unique items collection
  UNIQUE_STYLE_ITEMS(uniqueItems);
  if(! uniqueItems->AddItem((void*)this)){
    return;
  }

  PRUint32 localSize=0;

  // create a tag for this instance
  nsCOMPtr<nsIAtom> tag = do_GetAtom("nsCSSSelector");
  // get the size of an empty instance and add to the sizeof handler
  aSize = sizeof(*this);
  
  // now get the member-atoms and add them in
  if(mTag && uniqueItems->AddItem(mTag)){
    localSize = 0;
    mTag->SizeOf(aSizeOfHandler, &localSize);
    aSize += localSize;
  }


  // XXX ????



  // a couple of simple atom lists
  if(mIDList && uniqueItems->AddItem(mIDList)){
    aSize += sizeof(*mIDList);
    nsAtomList *pNext = nsnull;
    pNext = mIDList;    
    while(pNext){
      if(pNext->mAtom && uniqueItems->AddItem(pNext->mAtom)){
        localSize = 0;
        pNext->mAtom->SizeOf(aSizeOfHandler, &localSize);
        aSize += localSize;
      }
      pNext = pNext->mNext;
    }
  }
  if(mClassList && uniqueItems->AddItem(mClassList)){
    aSize += sizeof(*mClassList);
    nsAtomList *pNext = nsnull;
    pNext = mClassList;    
    while(pNext){
      if(pNext->mAtom && uniqueItems->AddItem(pNext->mAtom)){
        localSize = 0;
        pNext->mAtom->SizeOf(aSizeOfHandler, &localSize);
        aSize += localSize;
      }
      pNext = pNext->mNext;
    }
  }
  if(mPseudoClassList && uniqueItems->AddItem(mPseudoClassList)){
    nsAtomStringList *pNext = nsnull;
    pNext = mPseudoClassList;    
    while(pNext){
      if(pNext->mAtom && uniqueItems->AddItem(pNext->mAtom)){
        localSize = 0;
        pNext->mAtom->SizeOf(aSizeOfHandler, &localSize);
        aSize += localSize;
      }
      pNext = pNext->mNext;
    }
  }
  // done with undelegated sizes 
  aSizeOfHandler->AddSize(tag, aSize);

  // the AttributeList gets its own delegation-call
  if(mAttrList){
    localSize = 0;
    mAttrList->SizeOf(aSizeOfHandler, localSize);
  }

  // don't forget the negated selectors
  if(mNegations) {
    localSize = 0;
    mNegations->SizeOf(aSizeOfHandler, localSize);
  }
  
  // finally chain to the next...
  if(mNext){
    localSize = 0;
    mNext->SizeOf(aSizeOfHandler, localSize);
  }
}
#endif

// pseudo-elements are stored in the selectors' chain using fictional elements;
// these fictional elements have mTag starting with a colon
static PRBool IsPseudoElement(nsIAtom* aAtom)
{
  if (aAtom) {
    const PRUnichar *str;
    aAtom->GetUnicode(&str);
    return str && (*str == ':');
  }

  return PR_FALSE;
}

void nsCSSSelector::AppendNegationToString(nsAString& aString)
{
  aString.Append(NS_LITERAL_STRING(":not("));
}

//
// Builds the textual representation of a selector. Called by DOM 2 CSS 
// StyleRule:selectorText
//
// bug 98765
#if(0)
nsresult nsCSSSelector::ToString( nsAString& aString, nsICSSStyleSheet* aSheet, PRBool aIsPseudoElem,
                                  PRInt8 aNegatedIndex) const
#else
void
nsCSSSelector::ToString(nsAString& aString, nsICSSStyleSheet* aSheet) const
{
  ToStringInternal(aString, aSheet, IsPseudoElement(mTag), 0);
}

void nsCSSSelector::ToStringInternal(nsAString& aString,
                                     nsICSSStyleSheet* aSheet,
                                     PRBool aIsPseudoElem,
                                     PRIntn aNegatedIndex) const
#endif
// end bug
{
  const PRUnichar* temp;
  PRBool aIsNegated = PRBool(0 < aNegatedIndex);

  // selectors are linked from right-to-left, so the next selector in the linked list
  // actually precedes this one in the resulting string
  if (mNext) {
    //mNext->ToString(aString, aSheet, IsPseudoElement(mTag), PR_FALSE);
    mNext->ToStringInternal(aString, aSheet, IsPseudoElement(mTag), 0); // bug 98765
    if (!aIsNegated && !IsPseudoElement(mTag)) {
      // don't add a leading whitespace if we have a pseudo-element
      // or a negated simple selector
      aString.Append(PRUnichar(' '));
    }
  }
  if (1 < aNegatedIndex) {
    // the first mNegations does not contain a negated type element selector
    // or a negated universal selector
    NS_IF_NEGATED_START(aIsNegated, aString)
  }

  // append the namespace prefix
  if (mNameSpace > 0) {
    nsCOMPtr<nsINameSpace> sheetNS;
    aSheet->GetNameSpace(*getter_AddRefs(sheetNS));
    nsCOMPtr<nsIAtom> prefixAtom;
    // will return null if namespace was the default
    sheetNS->FindNameSpacePrefix(mNameSpace, *getter_AddRefs(prefixAtom));
    if (prefixAtom) {
      const PRUnichar* prefix;
      prefixAtom->GetUnicode(&prefix);
      aString.Append(prefix);
      aString.Append(PRUnichar('|'));
    }
  }
  // smells like a universal selector
  if (!mTag && !mIDList && !mClassList) {
    if (1 != aNegatedIndex) {
      aString.Append(PRUnichar('*'));
    }
    if (1 < aNegatedIndex) {
      NS_IF_NEGATED_END(aIsNegated, aString)
    }
  } else {
    // Append the tag name, if there is one
    if (mTag) {
      mTag->GetUnicode(&temp);
      aString.Append(temp);
      NS_IF_NEGATED_END(aIsNegated, aString)
    }
    // Append the id, if there is one
    if (mIDList) {
      nsAtomList* list = mIDList;
      while (list != nsnull) {
        list->mAtom->GetUnicode(&temp);
        NS_IF_NEGATED_START(aIsNegated, aString)
        aString.Append(PRUnichar('#'));
        aString.Append(temp);
        NS_IF_NEGATED_END(aIsNegated, aString)
        list = list->mNext;
      }
    }
    // Append each class in the linked list
    if (mClassList) {
      nsAtomList* list = mClassList;
      while (list != nsnull) {
        list->mAtom->GetUnicode(&temp);
        NS_IF_NEGATED_START(aIsNegated, aString)
        aString.Append(PRUnichar('.'));
        aString.Append(temp);
        NS_IF_NEGATED_END(aIsNegated, aString)
        list = list->mNext;
      }
    }
  }

  // Append each attribute selector in the linked list
  if (mAttrList) {
    nsAttrSelector* list = mAttrList;
    while (list != nsnull) {
      NS_IF_NEGATED_START(aIsNegated, aString)
      aString.Append(PRUnichar('['));
      // Append the namespace prefix
      if (list->mNameSpace > 0) {
        nsCOMPtr<nsINameSpace> sheetNS;
        aSheet->GetNameSpace(*getter_AddRefs(sheetNS));
        nsCOMPtr<nsIAtom> prefixAtom;
        // will return null if namespace was the default
        sheetNS->FindNameSpacePrefix(list->mNameSpace, *getter_AddRefs(prefixAtom));
        if (prefixAtom) { 
          const PRUnichar* prefix;
          prefixAtom->GetUnicode(&prefix);
          aString.Append(prefix);
          aString.Append(PRUnichar('|'));
        }
      }
      // Append the attribute name
      list->mAttr->GetUnicode(&temp);
      aString.Append(temp);
      // Append the function
      if (list->mFunction == NS_ATTR_FUNC_EQUALS) {
        aString.Append(PRUnichar('='));
      } else if (list->mFunction == NS_ATTR_FUNC_INCLUDES) {
        aString.Append(PRUnichar('~'));
        aString.Append(PRUnichar('='));
      } else if (list->mFunction == NS_ATTR_FUNC_DASHMATCH) {
        aString.Append(PRUnichar('|'));
        aString.Append(PRUnichar('='));
      } else if (list->mFunction == NS_ATTR_FUNC_BEGINSMATCH) {
        aString.Append(PRUnichar('^'));
        aString.Append(PRUnichar('='));
      } else if (list->mFunction == NS_ATTR_FUNC_ENDSMATCH) {
        aString.Append(PRUnichar('$'));
        aString.Append(PRUnichar('='));
      } else if (list->mFunction == NS_ATTR_FUNC_CONTAINSMATCH) {
        aString.Append(PRUnichar('*'));
        aString.Append(PRUnichar('='));
      }
      // Append the value
      aString.Append(list->mValue);
      aString.Append(PRUnichar(']'));
      NS_IF_NEGATED_END(aIsNegated, aString)
      list = list->mNext;
    }
  }

  // Append each pseudo-class in the linked list
  if (mPseudoClassList) {
    nsAtomStringList* list = mPseudoClassList;
    while (list != nsnull) {
      list->mAtom->GetUnicode(&temp);
      NS_IF_NEGATED_START(aIsNegated, aString)
      aString.Append(temp);
      if (nsnull != list->mString) {
        aString.Append(PRUnichar('('));
        aString.Append(list->mString);
        aString.Append(PRUnichar(')'));
      }
      NS_IF_NEGATED_END(aIsNegated, aString)
      list = list->mNext;
    }
  }

  if (mNegations) {
    // chain all the negated selectors
    //mNegations->ToString(aString, aSheet, PR_FALSE, aNegatedIndex + 1);
    mNegations->ToStringInternal(aString, aSheet, PR_FALSE, aNegatedIndex + 1); // bug 98765
  }

  // Append the operator only if the selector is not negated and is not
  // a pseudo-element
  if (!aIsNegated && mOperator && !aIsPseudoElem) {
    aString.Append(PRUnichar(' '));
    aString.Append(mOperator);
  }
  //return NS_OK; // bug 98765
}

// bug 98765
// -- nsCSSSelectorList -------------------------------

MOZ_DECL_CTOR_COUNTER(nsCSSSelectorList)

nsCSSSelectorList::nsCSSSelectorList(void)
  : mSelectors(nsnull),
    mNext(nsnull)
{
  MOZ_COUNT_CTOR(nsCSSSelectorList);
}

nsCSSSelectorList::~nsCSSSelectorList()
{
  MOZ_COUNT_DTOR(nsCSSSelectorList);
  nsCSSSelector*  sel = mSelectors;
  while (sel) {
    nsCSSSelector* dead = sel;
    sel = sel->mNext;
    delete dead;
  }
  if (mNext) {
    delete mNext;
  }
}

void nsCSSSelectorList::AddSelector(const nsCSSSelector& aSelector)
{ // prepend to list
  nsCSSSelector* newSel = new nsCSSSelector(aSelector);
  if (newSel) {
    newSel->mNext = mSelectors;
    mSelectors = newSel;
  }
}

void
nsCSSSelectorList::ToString(nsAString& aResult, nsICSSStyleSheet* aSheet)
{
  nsCSSSelectorList *p = this;
  for (;;) {
    p->mSelectors->ToString(aResult, aSheet);
    p = p->mNext;
    if (!p)
      break;
    aResult.Append(NS_LITERAL_STRING(", "));
  }
}

nsCSSSelectorList*
nsCSSSelectorList::Clone()
{
  nsCSSSelectorList *list = nsnull;
  nsCSSSelectorList **list_cur = &list;
  for (nsCSSSelectorList *l = this; l; l = l->mNext) {
    nsCSSSelectorList *lcopy = new nsCSSSelectorList();
    if (!lcopy) {
      delete list;
      return nsnull;
    }
    *list_cur = lcopy;
    list_cur = &lcopy->mNext;

    nsCSSSelector **sel_cur = &lcopy->mSelectors;
    for (nsCSSSelector *s = l->mSelectors; s; s = s->mNext) {
      nsCSSSelector *scopy = new nsCSSSelector(*s);
      if (!scopy) {
        delete list;
        return nsnull;
      }
      *sel_cur = scopy;
      sel_cur = &scopy->mNext;
    }
  }
  return list;
}
// end bug

// -- CSSImportantRule -------------------------------

// bug 125246
#if(0)
// New map helpers shared by both important and regular rules.
static nsresult MapFontForDeclaration(nsCSSDeclaration* aDecl, nsRuleDataFont& aFont);
static nsresult MapDisplayForDeclaration(nsCSSDeclaration* aDecl, const nsStyleStructID& aID, nsRuleDataDisplay& aDisplay);
static nsresult MapColorForDeclaration(nsCSSDeclaration* aDecl, const nsStyleStructID& aID, nsRuleDataColor& aColor);
static nsresult MapMarginForDeclaration(nsCSSDeclaration* aDecl, const nsStyleStructID& aID, nsRuleDataMargin& aMargin); 
static nsresult MapListForDeclaration(nsCSSDeclaration* aDecl, nsRuleDataList& aList);
static nsresult MapPositionForDeclaration(nsCSSDeclaration* aDecl, nsRuleDataPosition& aPosition);
static nsresult MapTableForDeclaration(nsCSSDeclaration* aDecl, const nsStyleStructID& aID, nsRuleDataTable& aTable);
static nsresult MapContentForDeclaration(nsCSSDeclaration* aDecl, const nsStyleStructID& aID, nsRuleDataContent& aContent);
static nsresult MapTextForDeclaration(nsCSSDeclaration* aDecl, const nsStyleStructID& aID, nsRuleDataText& aContent);
static nsresult MapUIForDeclaration(nsCSSDeclaration* aDecl, const nsStyleStructID& aID, nsRuleDataUserInterface& aContent);

#ifdef INCLUDE_XUL
static nsresult MapXULForDeclaration(nsCSSDeclaration* aDecl, nsRuleDataXUL& aXUL);
#endif

#ifdef MOZ_SVG
static nsresult MapSVGForDeclaration(nsCSSDeclaration* aDecl, nsRuleDataSVG& aSVG);
#endif
#endif
// end bug

class CSSStyleRuleImpl;

class CSSImportantRule : public nsIStyleRule {
public:
  CSSImportantRule(nsICSSStyleSheet* aSheet, nsCSSDeclaration* aDeclaration);

  NS_DECL_ISUPPORTS

  NS_IMETHOD GetStyleSheet(nsIStyleSheet*& aSheet) const;

  // The new mapping function.
  NS_IMETHOD MapRuleInfoInto(nsRuleData* aRuleData);

#ifdef DEBUG
  NS_IMETHOD List(FILE* out = stdout, PRInt32 aIndent = 0) const;

  virtual void SizeOf(nsISizeOfHandler *aSizeOfHandler, PRUint32 &aSize);
#endif

protected:
  virtual ~CSSImportantRule(void);

  nsCSSDeclaration*  mDeclaration;
  nsICSSStyleSheet*   mSheet;

friend class CSSStyleRuleImpl;
};

CSSImportantRule::CSSImportantRule(nsICSSStyleSheet* aSheet, nsCSSDeclaration* aDeclaration)
  : mDeclaration(aDeclaration),
    mSheet(aSheet)
{
}

CSSImportantRule::~CSSImportantRule(void)
{
  mDeclaration = nsnull;
}

NS_IMPL_ISUPPORTS1(CSSImportantRule, nsIStyleRule)

NS_IMETHODIMP
CSSImportantRule::GetStyleSheet(nsIStyleSheet*& aSheet) const
{
  NS_IF_ADDREF(mSheet);
  aSheet = mSheet;
  return NS_OK;
}

NS_IMETHODIMP
CSSImportantRule::MapRuleInfoInto(nsRuleData* aRuleData)
{
// bug 125246
#if(0)
  if (!aRuleData)
    return NS_OK;

  if (aRuleData->mFontData)
    return MapFontForDeclaration(mDeclaration, *aRuleData->mFontData);
  else if (aRuleData->mDisplayData)
    return MapDisplayForDeclaration(mDeclaration, aRuleData->mSID, *aRuleData->mDisplayData);
  else if (aRuleData->mColorData)
    return MapColorForDeclaration(mDeclaration, aRuleData->mSID, *aRuleData->mColorData);
  else if (aRuleData->mMarginData)
    return MapMarginForDeclaration(mDeclaration, aRuleData->mSID, *aRuleData->mMarginData);
  else if (aRuleData->mListData)
    return MapListForDeclaration(mDeclaration, *aRuleData->mListData);
  else if (aRuleData->mPositionData)
    return MapPositionForDeclaration(mDeclaration, *aRuleData->mPositionData);
  else if (aRuleData->mTableData)
    return MapTableForDeclaration(mDeclaration, aRuleData->mSID, *aRuleData->mTableData);
  else if (aRuleData->mContentData)
    return MapContentForDeclaration(mDeclaration, aRuleData->mSID, *aRuleData->mContentData);
  else if (aRuleData->mTextData)
    return MapTextForDeclaration(mDeclaration, aRuleData->mSID, *aRuleData->mTextData);
  else if (aRuleData->mUIData)
    return MapUIForDeclaration(mDeclaration, aRuleData->mSID, *aRuleData->mUIData);
#ifdef INCLUDE_XUL
  else if (aRuleData->mXULData)
    return MapXULForDeclaration(mDeclaration, *aRuleData->mXULData);
#endif
#ifdef MOZ_SVG
  else if (aRuleData->mSVGData)
    return MapSVGForDeclaration(mDeclaration, *aRuleData->mSVGData);
#endif

  return NS_OK;
#else
  return mDeclaration->MapImportantRuleInfoInto(aRuleData);
#endif
// end bug
}

#ifdef DEBUG
NS_IMETHODIMP
CSSImportantRule::List(FILE* out, PRInt32 aIndent) const
{
  // Indent
  for (PRInt32 index = aIndent; --index >= 0; ) fputs("  ", out);

  fputs("! Important rule ", out);
  if (nsnull != mDeclaration) {
    mDeclaration->List(out);
  }
  else {
    fputs("{ null declaration }", out);
  }
  fputs("\n", out);

  return NS_OK;
}

/******************************************************************************
* SizeOf method:
*
*  Self (reported as CSSImportantRule's size): 
*    1) sizeof(*this) 
*
*  Contained / Aggregated data (not reported as CSSImportantRule's size):
*    1) mDeclaration is sized seperately
*    2) mSheet is sized seperately
*
*  Children / siblings / parents:
*    none
*    
******************************************************************************/
void CSSImportantRule::SizeOf(nsISizeOfHandler *aSizeOfHandler, PRUint32 &aSize)
{
  NS_ASSERTION(aSizeOfHandler != nsnull, "SizeOf handler cannot be null");

  // first get the unique items collection
  UNIQUE_STYLE_ITEMS(uniqueItems);
  if(! uniqueItems->AddItem((void*)this)){
    return;
  }

  PRUint32 localSize=0;

  // create a tag for this instance
  nsCOMPtr<nsIAtom> tag = do_GetAtom("CSSImportantRule");
  // get the size of an empty instance and add to the sizeof handler
  aSize = sizeof(CSSImportantRule);
  aSizeOfHandler->AddSize(tag,aSize);

  // now dump the mDeclaration and mSheet
  if(mDeclaration){
    mDeclaration->SizeOf(aSizeOfHandler, localSize);
  }
  if(mSheet){
    mSheet->SizeOf(aSizeOfHandler, localSize);
  }
}
#endif

// -- nsDOMStyleRuleDeclaration -------------------------------
// -- nsDOMCSSDeclarationImpl ---------------------------------

class DOMCSSStyleRuleImpl; // bug 188803
class DOMCSSDeclarationImpl : public nsDOMCSSDeclaration
{
public:
  DOMCSSDeclarationImpl(nsICSSStyleRule *aRule);
  //~DOMCSSDeclarationImpl(void);
  virtual ~DOMCSSDeclarationImpl(void); // bug 188803

// bug 188803
//  NS_IMETHOD RemoveProperty(const nsAString& aPropertyName, 
//                            nsAString& aReturn);

  virtual nsresult GetParentRule(nsIDOMCSSRule **aParent); // bug 188803
  virtual void DropReference(void);
  virtual nsresult GetCSSDeclaration(nsCSSDeclaration **aDecl,
                                     PRBool aAllocate);
//  virtual nsresult SetCSSDeclaration(nsCSSDeclaration *aDecl); // bug 171830
// bug 188803
#if(0)
  virtual nsresult GetCSSParsingEnvironment(nsICSSStyleRule* aRule,
                                            nsICSSStyleSheet** aSheet,
                                            nsIDocument** aDocument,
                                            nsIURI** aURI,
                                            nsICSSLoader** aCSSLoader,
                                            nsICSSParser** aCSSParser);
  virtual nsresult ParsePropertyValue(const nsAString& aPropName,
                                      const nsAString& aPropValue);
  virtual nsresult ParseDeclaration(const nsAString& aDecl,
                                    PRBool aParseOnlyOneDecl,
                                    PRBool aClearOldDecl);
#else
  virtual nsresult GetCSSParsingEnvironment(nsIURI** aURI,
                                            nsICSSLoader** aCSSLoader,
                                            nsICSSParser** aCSSParser);
#endif
// end bug
  // virtual nsresult GetParent(nsISupports **aParent); // bug 188803
  virtual nsresult DeclarationChanged(); // bug 188803

// bug 188803
  // Override |AddRef| and |Release| for being a member of
  // |DOMCSSStyleRuleImpl|.
  NS_IMETHOD_(nsrefcnt) AddRef(void);
  NS_IMETHOD_(nsrefcnt) Release(void);

  friend class DOMCSSStyleRuleImpl;
// end bug
  
protected:
  nsICSSStyleRule *mRule;
// inserted by bug 188803
  // This reference is not reference-counted. The rule object tells us
  // when it's about to go away.

  inline DOMCSSStyleRuleImpl* DomRule();

private:
  // NOT TO BE IMPLEMENTED
  // This object cannot be allocated on its own.  It must be a member of
  // DOMCSSStyleRuleImpl.
  void* operator new(size_t size) CPP_THROW_NEW;
};

class DOMCSSStyleRuleImpl : public nsICSSStyleRuleDOMWrapper
{
public:
  DOMCSSStyleRuleImpl(nsICSSStyleRule *aRule);
  virtual ~DOMCSSStyleRuleImpl();

  NS_DECL_ISUPPORTS
  NS_DECL_NSIDOMCSSRULE
  NS_DECL_NSIDOMCSSSTYLERULE

  // nsICSSStyleRuleDOMWrapper
  NS_IMETHOD GetCSSStyleRule(nsICSSStyleRule **aResult);

  DOMCSSDeclarationImpl* DOMDeclaration() { return &mDOMDeclaration; }

  friend class DOMCSSDeclarationImpl;

protected:
  DOMCSSDeclarationImpl mDOMDeclaration;

  nsICSSStyleRule* Rule() {
    return mDOMDeclaration.mRule;
  }
// end bug
};

MOZ_DECL_CTOR_COUNTER(DOMCSSDeclarationImpl)

DOMCSSDeclarationImpl::DOMCSSDeclarationImpl(nsICSSStyleRule *aRule)
: mRule(aRule) // bug 188803
{
  MOZ_COUNT_CTOR(DOMCSSDeclarationImpl);

  // This reference is not reference-counted. The rule
  // object tells us when its about to go away.
  // no it doesn't. bug 188803 // mRule = aRule;
}

DOMCSSDeclarationImpl::~DOMCSSDeclarationImpl(void)
{
  NS_ASSERTION(!mRule, "DropReference not called.");
  MOZ_COUNT_DTOR(DOMCSSDeclarationImpl);
}

// bug 188803
#if(0)
NS_IMETHODIMP
DOMCSSDeclarationImpl::RemoveProperty(const nsAString& aPropertyName, 
                                      nsAString& aReturn)
{
  aReturn.Truncate();

  nsCSSDeclaration* decl;
  nsresult rv = GetCSSDeclaration(&decl, PR_TRUE);

  if (NS_SUCCEEDED(rv) && decl) {
    nsCOMPtr<nsICSSStyleSheet> cssSheet;
    nsCOMPtr<nsIDocument> owningDoc;
    if (mRule) {
      nsCOMPtr<nsIStyleSheet> sheet;
      mRule->GetStyleSheet(*getter_AddRefs(sheet));
      cssSheet = do_QueryInterface(sheet);
      if (sheet) {
        sheet->GetOwningDocument(*getter_AddRefs(owningDoc));
      }
    }
    if (owningDoc) {
      owningDoc->BeginUpdate();
    }
    nsCSSProperty prop = nsCSSProps::LookupProperty(aPropertyName);
// bug 125246
#if(0)
    nsCSSValue val;

    rv = decl->RemoveProperty(prop, val);
#else
    decl->GetValue(prop, aReturn);

    rv = decl->RemoveProperty(prop);
#endif
// end bug

    if (NS_SUCCEEDED(rv)) {
      // We pass in eCSSProperty_UNKNOWN here so that we don't get the
      // property name in the return string.
      //val.ToString(aReturn, eCSSProperty_UNKNOWN); // ... but not after bug 125246
      if (cssSheet) {
        cssSheet->SetModified(PR_TRUE);
      }
      if (owningDoc) {
        owningDoc->StyleRuleChanged(cssSheet, mRule, nsCSSProps::kHintTable[prop]);
      }
    } else {
      // If we tried to remove an invalid property or a property that wasn't 
      //  set we simply return success and an empty string
      rv = NS_OK;
    }
    if (owningDoc) {
      owningDoc->EndUpdate();
    }
  }

  return rv;
}
#else

inline DOMCSSStyleRuleImpl* DOMCSSDeclarationImpl::DomRule()
{
  return NS_REINTERPRET_CAST(DOMCSSStyleRuleImpl*,
           NS_REINTERPRET_CAST(char*, this) -
           offsetof(DOMCSSStyleRuleImpl, mDOMDeclaration));
}

NS_IMPL_ADDREF_USING_AGGREGATOR(DOMCSSDeclarationImpl, DomRule())
NS_IMPL_RELEASE_USING_AGGREGATOR(DOMCSSDeclarationImpl, DomRule())
#endif
// end bug 188803

void 
DOMCSSDeclarationImpl::DropReference(void)
{
  mRule = nsnull;
}

nsresult
DOMCSSDeclarationImpl::GetCSSDeclaration(nsCSSDeclaration **aDecl,
                                         PRBool aAllocate)
{
  if (mRule) {
    *aDecl = mRule->GetDeclaration();
  }
  else {
    *aDecl = nsnull;
  }

  return NS_OK;
}

// bug 171830
#if(0)
nsresult
DOMCSSDeclarationImpl::SetCSSDeclaration(nsCSSDeclaration *aDecl)
{
  if (mRule) {
    mRule->SetDeclaration(aDecl);
  }

  return NS_OK;
}
#endif
// end bug

/*
 * This is a utility function.  It will only fail if it can't get a
 * parser.  This means it can return NS_OK without all of aSheet,
 *  aDocument, aURI, aCSSLoader being initialized
 * after bug 188803:
 This means it can return NS_OK without aURI or aCSSLoader being initialized.
 */
 // bug 188803
nsresult
DOMCSSDeclarationImpl::GetCSSParsingEnvironment(//nsICSSStyleRule* aRule,
                                                //nsICSSStyleSheet** aSheet,
                                                //nsIDocument** aDocument,
                                                nsIURI** aURI,
                                                nsICSSLoader** aCSSLoader,
                                                nsICSSParser** aCSSParser)
{
  // null out the out params since some of them may not get initialized below
  //*aSheet = nsnull; // bug 188803
  //*aDocument = nsnull; // bug 188803
  *aURI = nsnull;
  *aCSSLoader = nsnull;
  *aCSSParser = nsnull;
  nsresult result;
  nsCOMPtr<nsIStyleSheet> sheet;
  // mRule from aRule bug 188803
  if (mRule) { //(aRule) {
    //aRule->
    mRule->GetStyleSheet(*getter_AddRefs(sheet));
    if (sheet) {
      // bug 188803
      //CallQueryInterface(sheet, aSheet);
      //sheet->GetOwningDocument(*aDocument);
      sheet->GetURL(*aURI);
      // bug 188803
      nsCOMPtr<nsIDocument> document;
      sheet->GetOwningDocument(*getter_AddRefs(document));
      nsCOMPtr<nsIHTMLContentContainer> htmlContainer =
        do_QueryInterface(document);
      if (htmlContainer) {
        htmlContainer->GetCSSLoader(*aCSSLoader);
        NS_ASSERTION(*aCSSLoader, "Document with no CSS loader!");
      }
      // end bug
    }
  }
// bug 188803
#if(0)
  nsCOMPtr<nsIHTMLContentContainer> htmlContainer(do_QueryInterface(*aDocument));
  if (htmlContainer) {
    htmlContainer->GetCSSLoader(*aCSSLoader);
  }
  NS_ASSERTION(*aCSSLoader || !*aDocument, "Document with no CSS loader!");
#endif
// end bug
  if (*aCSSLoader) {
    result = (*aCSSLoader)->GetParserFor(nsnull, aCSSParser);
  } else {
    result = NS_NewCSSParser(aCSSParser);
  }

  return result;
}

// bug 188803
// PATCH IS FIRST
#if(1)

// this got done up again by bug 188803 part 2. sorry about that. patch first
// again.
#if(1)
nsresult 
DOMCSSDeclarationImpl::GetParentRule(nsIDOMCSSRule **aParent)
{
  NS_ENSURE_ARG_POINTER(aParent);
 
  if (!mRule) {
    *aParent = nsnull;
    return NS_OK;
  }

  return mRule->GetDOMRule(aParent);
}
#else
nsresult 
DOMCSSDeclarationImpl::GetParent(nsISupports **aParent)
{
  NS_ENSURE_ARG_POINTER(aParent);

  if (mRule) {
    return CallQueryInterface(mRule, aParent);
  }

  *aParent = nsnull;

  return NS_OK;
}
#endif
// end patch 2. now back to part 1.

nsresult
DOMCSSDeclarationImpl::DeclarationChanged()
{
  NS_PRECONDITION(mRule,
         "can only be called when |GetCSSDeclaration| returned a declaration");

  nsCOMPtr<nsIDocument> owningDoc;
  nsCOMPtr<nsIStyleSheet> sheet;
  mRule->GetStyleSheet(*getter_AddRefs(sheet));
  if (sheet) {
    sheet->GetOwningDocument(*getter_AddRefs(owningDoc));
  }
  if (owningDoc) {
    // XXXldb Do we need to bother with this?  We're now doing it in
    // more places than we used to, but it probably doesn't matter...
    owningDoc->BeginUpdate();
  }

  nsCOMPtr<nsICSSStyleRule> oldRule = mRule;
  mRule = oldRule->DeclarationChanged(PR_TRUE).get();
  nsrefcnt cnt = mRule->Release();
  // bug 188803 patch 2
  //NS_ASSERTION(cnt != 0, "container didn't take ownership");
  if (cnt == 0) {
    NS_NOTREACHED("container didn't take ownership");
    mRule = nsnull;
    return NS_ERROR_UNEXPECTED;
  }
  // end bug

  if (owningDoc) {
    owningDoc->StyleRuleChanged(sheet, oldRule, mRule);
    owningDoc->EndUpdate();
  }
  return NS_OK;
}

// this came from patch 2.

DOMCSSStyleRuleImpl::DOMCSSStyleRuleImpl(nsICSSStyleRule* aRule)
  : mDOMDeclaration(aRule)
{
}

DOMCSSStyleRuleImpl::~DOMCSSStyleRuleImpl()
{
}

NS_INTERFACE_MAP_BEGIN(DOMCSSStyleRuleImpl)
  NS_INTERFACE_MAP_ENTRY(nsICSSStyleRuleDOMWrapper)
  NS_INTERFACE_MAP_ENTRY(nsIDOMCSSStyleRule)
  NS_INTERFACE_MAP_ENTRY(nsIDOMCSSRule)
  NS_INTERFACE_MAP_ENTRY(nsISupports)
  NS_INTERFACE_MAP_ENTRY_CONTENT_CLASSINFO(CSSStyleRule)
NS_INTERFACE_MAP_END

NS_IMPL_ADDREF(DOMCSSStyleRuleImpl)
NS_IMPL_RELEASE(DOMCSSStyleRuleImpl)

NS_IMETHODIMP    
DOMCSSStyleRuleImpl::GetType(PRUint16* aType)
{
  *aType = nsIDOMCSSRule::STYLE_RULE;
  
  return NS_OK;
}

NS_IMETHODIMP    
DOMCSSStyleRuleImpl::GetCssText(nsAString& aCssText)
{
  if (!Rule()) {
    aCssText.Truncate();
    return NS_OK;
  }
  return Rule()->GetCssText(aCssText);
}

NS_IMETHODIMP    
DOMCSSStyleRuleImpl::SetCssText(const nsAString& aCssText)
{
  if (!Rule()) {
    return NS_OK;
  }
  return Rule()->SetCssText(aCssText);
}

NS_IMETHODIMP    
DOMCSSStyleRuleImpl::GetParentStyleSheet(nsIDOMCSSStyleSheet** aSheet)
{
  if (!Rule()) {
    *aSheet = nsnull;
    return NS_OK;
  }
  nsCOMPtr<nsICSSStyleSheet> sheet;
  Rule()->GetParentStyleSheet(getter_AddRefs(sheet));
  if (!sheet) {
    *aSheet = nsnull;
    return NS_OK;
  }
  return CallQueryInterface(sheet, aSheet);
}

NS_IMETHODIMP    
DOMCSSStyleRuleImpl::GetParentRule(nsIDOMCSSRule** aParentRule)
{
  if (!Rule()) {
    *aParentRule = nsnull;
    return NS_OK;
  }
  nsCOMPtr<nsICSSGroupRule> rule;
  Rule()->GetParentRule(getter_AddRefs(rule));
  if (!rule) {
    *aParentRule = nsnull;
    return NS_OK;
  }
  return rule->GetDOMRule(aParentRule);
}

NS_IMETHODIMP    
DOMCSSStyleRuleImpl::GetSelectorText(nsAString& aSelectorText)
{
  if (!Rule()) {
    aSelectorText.Truncate();
    return NS_OK;
  }
  return Rule()->GetSelectorText(aSelectorText);
}

NS_IMETHODIMP    
DOMCSSStyleRuleImpl::SetSelectorText(const nsAString& aSelectorText)
{
  if (!Rule()) {
    return NS_OK;
  }
  return Rule()->SetSelectorText(aSelectorText);
}

NS_IMETHODIMP    
DOMCSSStyleRuleImpl::GetStyle(nsIDOMCSSStyleDeclaration** aStyle)
{
  *aStyle = &mDOMDeclaration;
  NS_ADDREF(*aStyle);
  return NS_OK;
}

NS_IMETHODIMP
DOMCSSStyleRuleImpl::GetCSSStyleRule(nsICSSStyleRule **aResult)
{
  *aResult = Rule();
  NS_IF_ADDREF(*aResult);
  return NS_OK;
}


#else 

nsresult
DOMCSSDeclarationImpl::ParsePropertyValue(const nsAString& aPropName,
                                          const nsAString& aPropValue)
{
  nsCSSDeclaration* decl;
  nsresult result = GetCSSDeclaration(&decl, PR_TRUE);
  if (!decl) {
    return result;
  }
  nsCOMPtr<nsICSSLoader> cssLoader;
  nsCOMPtr<nsICSSParser> cssParser;
  nsCOMPtr<nsIURI> baseURI;
  nsCOMPtr<nsICSSStyleSheet> cssSheet;
  nsCOMPtr<nsIDocument> owningDoc;
  result = GetCSSParsingEnvironment(mRule,
                                    getter_AddRefs(cssSheet),
                                    getter_AddRefs(owningDoc),
                                    getter_AddRefs(baseURI),
                                    getter_AddRefs(cssLoader),
                                    getter_AddRefs(cssParser));

  if (NS_FAILED(result)) {
    return result;
  }
  
  nsChangeHint hint;
  if (owningDoc) {
    owningDoc->BeginUpdate();
  }
  result = cssParser->ParseProperty(aPropName, aPropValue, baseURI, decl, &hint);
  if (NS_SUCCEEDED(result)) {
    if (cssSheet) {
      cssSheet->SetModified(PR_TRUE);
    }
    if (owningDoc) {
      owningDoc->StyleRuleChanged(cssSheet, mRule, hint);
      owningDoc->EndUpdate();
    }
  }
  if (cssLoader) {
    cssLoader->RecycleParser(cssParser);
  }

  return result;
}

nsresult 
DOMCSSDeclarationImpl::ParseDeclaration(const nsAString& aDecl,
                                        PRBool aParseOnlyOneDecl,
                                        PRBool aClearOldDecl)
{
  nsCSSDeclaration* decl;
  nsresult result = GetCSSDeclaration(&decl, PR_TRUE);

  if (decl) {
    nsCOMPtr<nsICSSLoader> cssLoader;
    nsCOMPtr<nsICSSParser> cssParser;
    nsCOMPtr<nsIURI> baseURI;
    nsCOMPtr<nsICSSStyleSheet> cssSheet;
    nsCOMPtr<nsIDocument> owningDoc;

    result = GetCSSParsingEnvironment(mRule,
                                      getter_AddRefs(cssSheet),
                                      getter_AddRefs(owningDoc),
                                      getter_AddRefs(baseURI),
                                      getter_AddRefs(cssLoader),
                                      getter_AddRefs(cssParser));

    if (NS_SUCCEEDED(result)) {
// bug 125246 plus backbugs
#if(0)
      nsCSSDeclaration* declClone = decl->Clone();
      NS_ENSURE_TRUE(declClone, NS_ERROR_OUT_OF_MEMORY);

      if (aClearOldDecl) {
        // This should be done with decl->Clear() once such a method exists.
        nsAutoString propName;
        PRUint32 count, i;

        count = decl->Count();

        for (i = 0; i < count; i++) {
          decl->GetNthProperty(0, propName);

          nsCSSProperty prop = nsCSSProps::LookupProperty(propName);
          nsCSSValue val;
 
          decl->RemoveProperty(prop, val);
        }
      }
#endif
      nsChangeHint hint;
      result = cssParser->ParseAndAppendDeclaration(aDecl, baseURI, decl,
                                                    aParseOnlyOneDecl, &hint,
                                                    aClearOldDecl); // bug 125246
// end bug

// bug 171830
#if(0)
      if (result == NS_CSS_PARSER_DROP_DECLARATION) {
        SetCSSDeclaration(declClone);
        result = NS_OK;
      } else
#endif
// end bug
 
      if (NS_SUCCEEDED(result)) {
        if (cssSheet) {
          cssSheet->SetModified(PR_TRUE);
        }
        if (owningDoc) {
          owningDoc->StyleRuleChanged(cssSheet, mRule, hint);
        }
      }
      if (cssLoader) {
        cssLoader->RecycleParser(cssParser);
      }
    }
  }

  return result;
}



nsresult 
DOMCSSDeclarationImpl::GetParent(nsISupports **aParent)
{
  NS_ENSURE_ARG_POINTER(aParent);

  if (mRule) {
    return CallQueryInterface(mRule, aParent);
  }

  *aParent = nsnull;

  return NS_OK;
}

#endif
// end bug 188803 BIG PATCH!


// -- nsCSSStyleRule -------------------------------

class CSSStyleRuleImpl : public nsCSSRule,
                         public nsICSSStyleRule // bug 188803 , 
                         //public nsIDOMCSSStyleRule
{
public:
  //CSSStyleRuleImpl(const nsCSSSelector& aSelector);
  CSSStyleRuleImpl(nsCSSSelectorList* aSelector, // bug 98765
  	nsCSSDeclaration *aDeclaration); // bug 188803
private: // bug 188803
  // for |Clone|
  CSSStyleRuleImpl(const CSSStyleRuleImpl& aCopy); 
  // for |DeclarationChanged| (bug 188803)
  CSSStyleRuleImpl(CSSStyleRuleImpl& aCopy,
  			nsCSSDeclaration *aDeclaration); 
public: // bug 188803

  NS_DECL_ISUPPORTS_INHERITED

// bug 98765
#if(0)
  virtual nsCSSSelector* FirstSelector(void);
  virtual void AddSelector(const nsCSSSelector& aSelector);
  virtual void DeleteSelector(nsCSSSelector* aSelector);
  virtual void SetSourceSelectorText(const nsString& aSelectorText);
  virtual void GetSourceSelectorText(nsString& aSelectorText) const;
#else
  virtual nsCSSSelectorList* Selector(void);
#endif
// bug 98765

  virtual PRUint32 GetLineNumber(void) const;
  virtual void SetLineNumber(PRUint32 aLineNumber);

  virtual nsCSSDeclaration* GetDeclaration(void) const;
  //virtual void SetDeclaration(nsCSSDeclaration* aDeclaration); // bug 188803

  // bug 98765
  //virtual PRInt32 GetWeight(void) const;
  //virtual void SetWeight(PRInt32 aWeight);

  virtual already_AddRefed<nsIStyleRule> GetImportantRule(void);

  // hook for inspector
  virtual nsresult GetValue(nsCSSProperty aProperty, nsCSSValue& aValue);

  NS_IMETHOD GetStyleSheet(nsIStyleSheet*& aSheet) const;
  NS_IMETHOD SetStyleSheet(nsICSSStyleSheet* aSheet);
  
  NS_IMETHOD SetParentRule(nsICSSGroupRule* aRule);
  
  // bug 188803
  virtual nsresult GetCssText(nsAString& aCssText);
  virtual nsresult SetCssText(const nsAString& aCssText);
  virtual nsresult GetParentStyleSheet(nsICSSStyleSheet** aSheet);
  virtual nsresult GetParentRule(nsICSSGroupRule** aParentRule);
  virtual nsresult GetSelectorText(nsAString& aSelectorText);
  virtual nsresult SetSelectorText(const nsAString& aSelectorText);

  NS_IMETHOD GetType(PRInt32& aType) const;
  NS_IMETHOD Clone(nsICSSRule*& aClone) const;
  
  // bug 188803
  NS_IMETHOD GetDOMRule(nsIDOMCSSRule** aDOMRule);
  
  // bug 188803
  virtual already_AddRefed<nsICSSStyleRule>
    DeclarationChanged(PRBool aHandleContainer);

  // The new mapping function.
  NS_IMETHOD MapRuleInfoInto(nsRuleData* aRuleData);

#ifdef DEBUG
  NS_IMETHOD List(FILE* out = stdout, PRInt32 aIndent = 0) const;

  virtual void SizeOf(nsISizeOfHandler *aSizeOfHandler, PRUint32 &aSize);
#endif

// bug 188803
#if(0)
  // nsIDOMCSSRule interface
  NS_DECL_NSIDOMCSSRULE

  // nsIDOMCSSStyleRule interface
  NS_DECL_NSIDOMCSSSTYLERULE
#endif
// end bug

private: 
  // These are not supported and are not implemented! 
  CSSStyleRuleImpl& operator=(const CSSStyleRuleImpl& aCopy); 

protected:
  virtual ~CSSStyleRuleImpl(void);

protected:
  //nsCSSSelector           mSelector;
  //nsCSSDeclaration*      mDeclaration;
  //PRInt32                 mWeight;
  nsCSSSelectorList*      mSelector; // null for style attribute
  nsCSSDeclaration*       mDeclaration;

  CSSImportantRule*       mImportantRule;
  // bug 188803
  DOMCSSStyleRuleImpl*    mDOMRule; 
  //DOMCSSDeclarationImpl*  mDOMDeclaration;                          
  PRUint32                mLineNumber;
};

// bug 125246
#if(0)
#ifdef DEBUG_REFS
PRUint32 gStyleRuleCount=0;
#endif
#endif
// end bug

CSSStyleRuleImpl::CSSStyleRuleImpl(nsCSSSelectorList* aSelector, // bug 98765 // const nsCSSSelector& aSelector)
                                   nsCSSDeclaration* aDeclaration) // bug 188803
  : nsCSSRule(),
    mSelector(aSelector),
    mDeclaration(aDeclaration), // bug 188803 // nsnull), 
    //mWeight(0), 
    mImportantRule(nsnull),
    mDOMRule(nsnull) // bug 188803 //mDOMDeclaration(nsnull)
{
// bug 125246
#if(0)
#ifdef DEBUG_REFS
  gStyleRuleCount++;
  printf( "CSSStyleRuleImpl Instances (ctor): %ld\n", (long)gStyleRuleCount);
#endif
#endif
// end bug

// bug 188803
   if (mDeclaration) mDeclaration->AddRef();
}

// for |Clone|
CSSStyleRuleImpl::CSSStyleRuleImpl(const CSSStyleRuleImpl& aCopy)
  : nsCSSRule(aCopy),
    //mSelector(aCopy.mSelector),
    mSelector(aCopy.mSelector ? aCopy.mSelector->Clone() : nsnull),
    mDeclaration(aCopy.mDeclaration ? aCopy.mDeclaration->Clone() : nsnull), // bug 188803 //nsnull),
    //mWeight(aCopy.mWeight),
    mImportantRule(nsnull),
    mDOMRule(nsnull) // bug 188803 // mDOMDeclaration(nsnull)
{
// bug 125246
#if(0)
#ifdef DEBUG_REFS
  gStyleRuleCount++;
  printf( "CSSStyleRuleImpl Instances (cp-ctor): %ld\n", (long)gStyleRuleCount);
#endif
#endif
// end bug

// bug 98765
#if(0)
  nsCSSSelector* copySel = aCopy.mSelector.mNext;
  nsCSSSelector* ourSel = &mSelector;

  while (copySel && ourSel) {
    ourSel->mNext = new nsCSSSelector(*copySel);
    ourSel = ourSel->mNext;
    copySel = copySel->mNext;
  }
#endif

// bug 188803
#if(0)
  if (aCopy.mDeclaration) {
    mDeclaration = aCopy.mDeclaration->Clone();
    if (nsnull != mDeclaration) {
      mDeclaration->AddRef();
    }
  }
#else
  if (mDeclaration) mDeclaration->AddRef();
#endif
// end bug

  // rest is constructed lazily on existing data
}

// bug 188803
// for |DeclarationChanged|
CSSStyleRuleImpl::CSSStyleRuleImpl(CSSStyleRuleImpl& aCopy,
                                   nsCSSDeclaration* aDeclaration)
  : nsCSSRule(aCopy),
    mSelector(aCopy.mSelector),
    mDeclaration(aDeclaration),
    mImportantRule(nsnull),
    mDOMRule(aCopy.mDOMRule) // bug 188803 // mDOMDeclaration(aCopy.mDOMDeclaration)
{
  // The DOM declaration is replacing |aCopy| with |this|, so transfer
  // the reverse pointer as well.
  // bug 188803
  // The DOM rule is replacing |aCopy| with |this|, so transfer
  // the reverse pointer as well (and transfer ownership).
  aCopy.mDOMRule = nsnull;
  //aCopy.mDOMDeclaration = nsnull;

  // Transfer ownership of selector and declaration:
  NS_ASSERTION(aDeclaration == aCopy.mDeclaration, "declaration mismatch");
  aCopy.mSelector = nsnull;
  aCopy.mDeclaration = nsnull;
}
// end bug

CSSStyleRuleImpl::~CSSStyleRuleImpl(void)
{
// bug 125246
#if(0)
#ifdef DEBUG_REFS
  gStyleRuleCount--;
  printf( "CSSStyleRuleImpl Instances (dtor): %ld\n", (long)gStyleRuleCount);
#endif
#endif
// end bug

// bug 98765
#if(0)
  nsCSSSelector*  next = mSelector.mNext;

  while (nsnull != next) {
    nsCSSSelector*  selector = next;
    next = selector->mNext;
    delete selector;
  }
#else
  if (mSelector) {
    delete mSelector;
    mSelector = nsnull;
  }
#endif
// end bug

  if (nsnull != mDeclaration) {
    mDeclaration->Release();
    mDeclaration = nsnull;
  }
  if (nsnull != mImportantRule) {
    mImportantRule->mSheet = nsnull;
    NS_RELEASE(mImportantRule);
    mImportantRule = nsnull;
  }
// bug 188803
#if(0)
  if (nsnull != mDOMDeclaration) {
    mDOMDeclaration->DropReference();
  }
#else
  if (mDOMRule) {
    mDOMRule->DOMDeclaration()->DropReference();
    NS_RELEASE(mDOMRule);
  }
#endif
}

// QueryInterface implementation for CSSStyleRuleImpl
NS_INTERFACE_MAP_BEGIN(CSSStyleRuleImpl)
  NS_INTERFACE_MAP_ENTRY(nsICSSStyleRule)
  NS_INTERFACE_MAP_ENTRY(nsICSSRule)
  NS_INTERFACE_MAP_ENTRY(nsIStyleRule)
// bug 188803
#if(0)
  NS_INTERFACE_MAP_ENTRY(nsIDOMCSSRule)
  NS_INTERFACE_MAP_ENTRY(nsIDOMCSSStyleRule)
#endif
  NS_INTERFACE_MAP_ENTRY_AMBIGUOUS(nsISupports, nsICSSStyleRule)
// bug 188803
#if(0)
  NS_INTERFACE_MAP_ENTRY_CONTENT_CLASSINFO(CSSStyleRule)
#endif
// end bug
NS_INTERFACE_MAP_END

NS_IMPL_ADDREF_INHERITED(CSSStyleRuleImpl, nsCSSRule);
NS_IMPL_RELEASE_INHERITED(CSSStyleRuleImpl, nsCSSRule);

// bug 98765
#if(0)
nsCSSSelector* CSSStyleRuleImpl::FirstSelector(void)
{
  return &mSelector;
}

void CSSStyleRuleImpl::AddSelector(const nsCSSSelector& aSelector)
{
  nsCSSSelector*  selector = new nsCSSSelector(aSelector);
  nsCSSSelector*  last = &mSelector;

  while (nsnull != last->mNext) {
    last = last->mNext;
  }
  last->mNext = selector;
}


void CSSStyleRuleImpl::DeleteSelector(nsCSSSelector* aSelector)
{
  if (nsnull != aSelector) {
    if (&mSelector == aSelector) {  // handle first selector
      if (nsnull != mSelector.mNext) {
        nsCSSSelector* nextOne = mSelector.mNext;
        mSelector = *nextOne; // assign values
        mSelector.mNext = nextOne->mNext;
        delete nextOne;
      }
      else {
        mSelector.Reset();
      }
    }
    else {
      nsCSSSelector*  selector = &mSelector;

      while (nsnull != selector->mNext) {
        if (aSelector == selector->mNext) {
          selector->mNext = aSelector->mNext;
          delete aSelector;
          return;
        }
        selector = selector->mNext;
      }
    }
  }
}

void CSSStyleRuleImpl::SetSourceSelectorText(const nsString& aSelectorText)
{
    /* no need for set, since get recreates the string */
}
void CSSStyleRuleImpl::GetSourceSelectorText(nsString& aSelectorText) const
{
  mSelector.ToString( aSelectorText, mSheet, IsPseudoElement(mSelector.mTag),
                      0 );
}
#else
nsCSSSelectorList* CSSStyleRuleImpl::Selector(void)
{
  return mSelector;
}
#endif
// end bug

PRUint32 CSSStyleRuleImpl::GetLineNumber(void) const
{
  return mLineNumber;
}

void CSSStyleRuleImpl::SetLineNumber(PRUint32 aLineNumber)
{
  mLineNumber = aLineNumber;
}

nsCSSDeclaration* CSSStyleRuleImpl::GetDeclaration(void) const
{
  return mDeclaration; // bug 188803
  //nsCSSDeclaration* result = mDeclaration;
  //return result;
}

// bug 188803
#if(0)
void CSSStyleRuleImpl::SetDeclaration(nsCSSDeclaration* aDeclaration)
{
  if (mDeclaration != aDeclaration) {
    NS_IF_RELEASE(mImportantRule); 
    if (nsnull != mDeclaration) {
      mDeclaration->Release();
    }
    mDeclaration = aDeclaration;
    mDeclaration->AddRef();
  }
}
#endif
// end bug

// bug 98765
#if(0)
PRInt32 CSSStyleRuleImpl::GetWeight(void) const
{
  return mWeight;
}

void CSSStyleRuleImpl::SetWeight(PRInt32 aWeight)
{
  mWeight = aWeight;
}
#endif
// end bug

already_AddRefed<nsIStyleRule> CSSStyleRuleImpl::GetImportantRule(void)
{
  if (!mImportantRule && mDeclaration) {
  // bug 125246
  //  nsCSSDeclaration*  important = mDeclaration->GetImportantValues();
  //  if (important) {
  //    mImportantRule = new CSSImportantRule(mSheet, important);
  if (mDeclaration->HasImportantData()) {
      mImportantRule = new CSSImportantRule(mSheet, mDeclaration);
  // end bug
      NS_ADDREF(mImportantRule);
    }
  }
  NS_IF_ADDREF(mImportantRule);
  return mImportantRule;
}

nsresult
CSSStyleRuleImpl::GetValue(nsCSSProperty aProperty, nsCSSValue& aValue)
{
  //return mDeclaration->GetValue(aProperty, aValue);
  return mDeclaration->GetValueOrImportantValue(aProperty, aValue); // bug 125246
}

NS_IMETHODIMP
CSSStyleRuleImpl::GetStyleSheet(nsIStyleSheet*& aSheet) const
{
  return nsCSSRule::GetStyleSheet(aSheet);
}

NS_IMETHODIMP
CSSStyleRuleImpl::SetStyleSheet(nsICSSStyleSheet* aSheet)
{
  nsCSSRule::SetStyleSheet(aSheet);
  if (nsnull != mImportantRule) { // we're responsible for this guy too
    mImportantRule->mSheet = aSheet;
  }
  return NS_OK;
}

NS_IMETHODIMP
CSSStyleRuleImpl::SetParentRule(nsICSSGroupRule* aRule)
{
  return nsCSSRule::SetParentRule(aRule);
}

NS_IMETHODIMP
CSSStyleRuleImpl::GetType(PRInt32& aType) const
{
  aType = nsICSSRule::STYLE_RULE;
  return NS_OK;
}

// bug 188830
#if(0)
NS_IMETHODIMP
CSSStyleRuleImpl::Clone(nsICSSRule*& aClone) const
{
  CSSStyleRuleImpl* clone = new CSSStyleRuleImpl(*this);
  if (clone) {
    return CallQueryInterface(clone, &aClone);
  }
  aClone = nsnull;
  return NS_ERROR_OUT_OF_MEMORY;
}

#else

NS_IMETHODIMP
CSSStyleRuleImpl::Clone(nsICSSRule*& aClone) const
{
  CSSStyleRuleImpl* clone = new CSSStyleRuleImpl(*this);
  if (!clone) {
    aClone = nsnull;
    return NS_ERROR_OUT_OF_MEMORY;
  }
  return CallQueryInterface(clone, &aClone);
}

NS_IMETHODIMP
CSSStyleRuleImpl::GetDOMRule(nsIDOMCSSRule** aDOMRule)
{
  if (!mDOMRule) {
    mDOMRule = new DOMCSSStyleRuleImpl(this);
    if (!mDOMRule) {
      *aDOMRule = nsnull;
      return NS_ERROR_OUT_OF_MEMORY;
    }
    NS_ADDREF(mDOMRule);
  }
  *aDOMRule = mDOMRule;
  NS_ADDREF(*aDOMRule);
  return NS_OK;
}

/* virtual */ already_AddRefed<nsICSSStyleRule>
CSSStyleRuleImpl::DeclarationChanged(PRBool aHandleContainer)
{
  CSSStyleRuleImpl* clone = new CSSStyleRuleImpl(*this, mDeclaration);
  if (!clone) {
    return nsnull;
  }

  NS_ADDREF(clone); // for return

  if (aHandleContainer && mSheet) { // mSheet from bug 2
    if (mParentRule) {
      mSheet->ReplaceRuleInGroup(mParentRule, this, clone);
    } else {
      mSheet->ReplaceStyleRule(this, clone);
    }
  }

  return clone;
}

#endif
// end bug

// bug 125246
// BIG PATCH!
// PATCH IS FIRST!
#if(1)
NS_IMETHODIMP
CSSStyleRuleImpl::MapRuleInfoInto(nsRuleData* aRuleData)
{
  return mDeclaration->MapRuleInfoInto(aRuleData);
}
#else
NS_IMETHODIMP
CSSStyleRuleImpl::MapRuleInfoInto(nsRuleData* aRuleData)
{
  if (!aRuleData)
    return NS_OK;

  if (aRuleData->mFontData)
    return MapFontForDeclaration(mDeclaration, *aRuleData->mFontData);
  else if (aRuleData->mDisplayData)
    return MapDisplayForDeclaration(mDeclaration, aRuleData->mSID, *aRuleData->mDisplayData);
  else if (aRuleData->mColorData)
    return MapColorForDeclaration(mDeclaration, aRuleData->mSID, *aRuleData->mColorData);
  else if (aRuleData->mMarginData)
    return MapMarginForDeclaration(mDeclaration, aRuleData->mSID, *aRuleData->mMarginData);
  else if (aRuleData->mListData)
    return MapListForDeclaration(mDeclaration, *aRuleData->mListData);
  else if (aRuleData->mPositionData)
    return MapPositionForDeclaration(mDeclaration, *aRuleData->mPositionData);
  else if (aRuleData->mTableData)
    return MapTableForDeclaration(mDeclaration, aRuleData->mSID, *aRuleData->mTableData);
  else if (aRuleData->mContentData)
    return MapContentForDeclaration(mDeclaration, aRuleData->mSID, *aRuleData->mContentData);
  else if (aRuleData->mTextData)
    return MapTextForDeclaration(mDeclaration, aRuleData->mSID, *aRuleData->mTextData);
  else if (aRuleData->mUIData)
    return MapUIForDeclaration(mDeclaration, aRuleData->mSID, *aRuleData->mUIData);
#ifdef INCLUDE_XUL
  else if (aRuleData->mXULData)
    return MapXULForDeclaration(mDeclaration, *aRuleData->mXULData);
#endif
#ifdef MOZ_SVG
  else if (aRuleData->mSVGData)
    return MapSVGForDeclaration(mDeclaration, *aRuleData->mSVGData);
#endif

  return NS_OK;
}

static nsresult 
MapFontForDeclaration(nsCSSDeclaration* aDecl, nsRuleDataFont& aFont)
{
  if (!aDecl)
    return NS_OK; // The rule must have a declaration.

  nsCSSFont* ourFont = (nsCSSFont*)aDecl->GetData(kCSSFontSID);
  if (!ourFont)
    return NS_OK; // We don't have any rules for fonts.

  if (eCSSUnit_Null == aFont.mFamily.GetUnit() && eCSSUnit_Null != ourFont->mFamily.GetUnit()) {
    aFont.mFamily = ourFont->mFamily;
    aFont.mFamilyFromHTML = PR_FALSE;
  }

  if (eCSSUnit_Null == aFont.mStyle.GetUnit() && eCSSUnit_Null != ourFont->mStyle.GetUnit())
    aFont.mStyle = ourFont->mStyle;

  if (eCSSUnit_Null == aFont.mVariant.GetUnit() && eCSSUnit_Null != ourFont->mVariant.GetUnit())
    aFont.mVariant = ourFont->mVariant;

  if (eCSSUnit_Null == aFont.mWeight.GetUnit() && eCSSUnit_Null != ourFont->mWeight.GetUnit())
    aFont.mWeight = ourFont->mWeight;

  if (eCSSUnit_Null == aFont.mSize.GetUnit() && eCSSUnit_Null != ourFont->mSize.GetUnit())
    aFont.mSize = ourFont->mSize;

  if (eCSSUnit_Null == aFont.mSizeAdjust.GetUnit() && eCSSUnit_Null != ourFont->mSizeAdjust.GetUnit())
    aFont.mSizeAdjust = ourFont->mSizeAdjust;

  return NS_OK;
}

#ifdef INCLUDE_XUL
static nsresult 
MapXULForDeclaration(nsCSSDeclaration* aDecl, nsRuleDataXUL& aXUL)
{
  if (!aDecl)
    return NS_OK; // The rule must have a declaration.

  nsCSSXUL* ourXUL = (nsCSSXUL*)aDecl->GetData(kCSSXULSID);
  if (!ourXUL)
    return NS_OK; // We don't have any rules for XUL.

  // box-align: enum, inherit
  if (aXUL.mBoxAlign.GetUnit() == eCSSUnit_Null && ourXUL->mBoxAlign.GetUnit() != eCSSUnit_Null)
    aXUL.mBoxAlign = ourXUL->mBoxAlign;

  // box-direction: enum, inherit
  if (aXUL.mBoxDirection.GetUnit() == eCSSUnit_Null && ourXUL->mBoxDirection.GetUnit() != eCSSUnit_Null)
    aXUL.mBoxDirection = ourXUL->mBoxDirection;

  // box-flex: enum, inherit
  if (aXUL.mBoxFlex.GetUnit() == eCSSUnit_Null && ourXUL->mBoxFlex.GetUnit() != eCSSUnit_Null)
    aXUL.mBoxFlex = ourXUL->mBoxFlex;

  // box-orient: enum, inherit
  if (aXUL.mBoxOrient.GetUnit() == eCSSUnit_Null && ourXUL->mBoxOrient.GetUnit() != eCSSUnit_Null)
    aXUL.mBoxOrient = ourXUL->mBoxOrient;

  // box-pack: enum, inherit
  if (aXUL.mBoxPack.GetUnit() == eCSSUnit_Null && ourXUL->mBoxPack.GetUnit() != eCSSUnit_Null)
    aXUL.mBoxPack = ourXUL->mBoxPack;

  // box-ordinal-group: number
  if (aXUL.mBoxOrdinal.GetUnit() == eCSSUnit_Null && ourXUL->mBoxOrdinal.GetUnit() != eCSSUnit_Null)
    aXUL.mBoxOrdinal = ourXUL->mBoxOrdinal;

  return NS_OK;
}
#endif

#ifdef MOZ_SVG
static nsresult 
MapSVGForDeclaration(nsCSSDeclaration* aDecl, nsRuleDataSVG& aSVG)
{
  if (!aDecl)
    return NS_OK; // The rule must have a declaration.

  nsCSSSVG* ourSVG = (nsCSSSVG*)aDecl->GetData(kCSSSVGSID);
  if (!ourSVG)
    return NS_OK; // We don't have any rules for SVG.

  // fill:
  if (aSVG.mFill.GetUnit() == eCSSUnit_Null && ourSVG->mFill.GetUnit() != eCSSUnit_Null)
    aSVG.mFill = ourSVG->mFill;
  // fill-opacity:
  if (aSVG.mFillOpacity.GetUnit() == eCSSUnit_Null && ourSVG->mFillOpacity.GetUnit() != eCSSUnit_Null)
    aSVG.mFillOpacity = ourSVG->mFillOpacity;
  // fill-rule:
  if (aSVG.mFillRule.GetUnit() == eCSSUnit_Null && ourSVG->mFillRule.GetUnit() != eCSSUnit_Null)
    aSVG.mFillRule = ourSVG->mFillRule;
  // stroke:
  if (aSVG.mStroke.GetUnit() == eCSSUnit_Null && ourSVG->mStroke.GetUnit() != eCSSUnit_Null)
    aSVG.mStroke = ourSVG->mStroke;
  // stroke-dasharray:
  if (aSVG.mStrokeDasharray.GetUnit() == eCSSUnit_Null && ourSVG->mStrokeDasharray.GetUnit() != eCSSUnit_Null)
    aSVG.mStrokeDasharray = ourSVG->mStrokeDasharray;
  // stroke-dashoffset:
  if (aSVG.mStrokeDashoffset.GetUnit() == eCSSUnit_Null && ourSVG->mStrokeDashoffset.GetUnit() != eCSSUnit_Null)
    aSVG.mStrokeDashoffset = ourSVG->mStrokeDashoffset;
  // stroke-linecap: enum, inherit
  if (aSVG.mStrokeLinecap.GetUnit() == eCSSUnit_Null && ourSVG->mStrokeLinecap.GetUnit() != eCSSUnit_Null)
    aSVG.mStrokeLinecap = ourSVG->mStrokeLinecap;
  // stroke-linejoin
  if (aSVG.mStrokeLinejoin.GetUnit() == eCSSUnit_Null && ourSVG->mStrokeLinejoin.GetUnit() != eCSSUnit_Null)
    aSVG.mStrokeLinejoin = ourSVG->mStrokeLinejoin;
  // stroke-miterlimit:
  if (aSVG.mStrokeMiterlimit.GetUnit() == eCSSUnit_Null && ourSVG->mStrokeMiterlimit.GetUnit() != eCSSUnit_Null)
    aSVG.mStrokeMiterlimit = ourSVG->mStrokeMiterlimit;
  // stroke-opacity:
  if (aSVG.mStrokeOpacity.GetUnit() == eCSSUnit_Null && ourSVG->mStrokeOpacity.GetUnit() != eCSSUnit_Null)
    aSVG.mStrokeOpacity = ourSVG->mStrokeOpacity;
  // stroke-width:
  if (aSVG.mStrokeWidth.GetUnit() == eCSSUnit_Null && ourSVG->mStrokeWidth.GetUnit() != eCSSUnit_Null)
    aSVG.mStrokeWidth = ourSVG->mStrokeWidth;
  
  return NS_OK;
}
#endif


static nsresult 
MapPositionForDeclaration(nsCSSDeclaration* aDecl, nsRuleDataPosition& aPosition)
{
  if (!aDecl)
    return NS_OK; // The rule must have a declaration.

  nsCSSPosition* ourPosition = (nsCSSPosition*)aDecl->GetData(kCSSPositionSID);
  if (!ourPosition)
    return NS_OK; // We don't have any rules for position.

  // box offsets: length, percent, auto, inherit
  if (ourPosition->mOffset) {
    if (aPosition.mOffset->mLeft.GetUnit() == eCSSUnit_Null && ourPosition->mOffset->mLeft.GetUnit() != eCSSUnit_Null)
      aPosition.mOffset->mLeft = ourPosition->mOffset->mLeft;

    if (aPosition.mOffset->mRight.GetUnit() == eCSSUnit_Null && ourPosition->mOffset->mRight.GetUnit() != eCSSUnit_Null)
      aPosition.mOffset->mRight = ourPosition->mOffset->mRight;

    if (aPosition.mOffset->mTop.GetUnit() == eCSSUnit_Null && ourPosition->mOffset->mTop.GetUnit() != eCSSUnit_Null)
      aPosition.mOffset->mTop = ourPosition->mOffset->mTop;

    if (aPosition.mOffset->mBottom.GetUnit() == eCSSUnit_Null && ourPosition->mOffset->mBottom.GetUnit() != eCSSUnit_Null)
      aPosition.mOffset->mBottom = ourPosition->mOffset->mBottom;
  }

  // width/min-width/max-width
  if (aPosition.mWidth.GetUnit() == eCSSUnit_Null && ourPosition->mWidth.GetUnit() != eCSSUnit_Null)
    aPosition.mWidth = ourPosition->mWidth;
  if (aPosition.mMinWidth.GetUnit() == eCSSUnit_Null && ourPosition->mMinWidth.GetUnit() != eCSSUnit_Null)
    aPosition.mMinWidth = ourPosition->mMinWidth;
  if (aPosition.mMaxWidth.GetUnit() == eCSSUnit_Null && ourPosition->mMaxWidth.GetUnit() != eCSSUnit_Null)
    aPosition.mMaxWidth = ourPosition->mMaxWidth;

  // height/min-height/max-height
  if (aPosition.mHeight.GetUnit() == eCSSUnit_Null && ourPosition->mHeight.GetUnit() != eCSSUnit_Null)
    aPosition.mHeight = ourPosition->mHeight;
  if (aPosition.mMinHeight.GetUnit() == eCSSUnit_Null && ourPosition->mMinHeight.GetUnit() != eCSSUnit_Null)
    aPosition.mMinHeight = ourPosition->mMinHeight;
  if (aPosition.mMaxHeight.GetUnit() == eCSSUnit_Null && ourPosition->mMaxHeight.GetUnit() != eCSSUnit_Null)
    aPosition.mMaxHeight = ourPosition->mMaxHeight;

  // box-sizing: enum, inherit
  if (aPosition.mBoxSizing.GetUnit() == eCSSUnit_Null && ourPosition->mBoxSizing.GetUnit() != eCSSUnit_Null)
    aPosition.mBoxSizing = ourPosition->mBoxSizing;

  // z-index
  if (aPosition.mZIndex.GetUnit() == eCSSUnit_Null && ourPosition->mZIndex.GetUnit() != eCSSUnit_Null)
    aPosition.mZIndex = ourPosition->mZIndex;

  return NS_OK;
}

static nsresult 
MapListForDeclaration(nsCSSDeclaration* aDecl, nsRuleDataList& aList)
{
  if (!aDecl)
    return NS_OK; // The rule must have a declaration.

  nsCSSList* ourList = (nsCSSList*)aDecl->GetData(kCSSListSID);
  if (!ourList)
    return NS_OK; // We don't have any rules for lists.

  // list-style-type: enum, none, inherit
  if (aList.mType.GetUnit() == eCSSUnit_Null && ourList->mType.GetUnit() != eCSSUnit_Null)
    aList.mType = ourList->mType;

  // list-style-image: url, none, inherit
  if (aList.mImage.GetUnit() == eCSSUnit_Null && ourList->mImage.GetUnit() != eCSSUnit_Null)
    aList.mImage = ourList->mImage;
      
  // list-style-position: enum, inherit
  if (aList.mPosition.GetUnit() == eCSSUnit_Null && ourList->mPosition.GetUnit() != eCSSUnit_Null)
    aList.mPosition = ourList->mPosition;

  // image region: length, auto or inherit
  if (ourList->mImageRegion) {
    if (aList.mImageRegion->mLeft.GetUnit() == eCSSUnit_Null && 
        ourList->mImageRegion->mLeft.GetUnit() != eCSSUnit_Null)
      aList.mImageRegion->mLeft = ourList->mImageRegion->mLeft;

    if (aList.mImageRegion->mRight.GetUnit() == eCSSUnit_Null && 
        ourList->mImageRegion->mRight.GetUnit() != eCSSUnit_Null)
      aList.mImageRegion->mRight = ourList->mImageRegion->mRight;

    if (aList.mImageRegion->mTop.GetUnit() == eCSSUnit_Null && 
        ourList->mImageRegion->mTop.GetUnit() != eCSSUnit_Null)
      aList.mImageRegion->mTop = ourList->mImageRegion->mTop;

    if (aList.mImageRegion->mBottom.GetUnit() == eCSSUnit_Null && 
        ourList->mImageRegion->mBottom.GetUnit() != eCSSUnit_Null)
      aList.mImageRegion->mBottom = ourList->mImageRegion->mBottom;
  }

  return NS_OK;
}
    
static nsresult
MapMarginForDeclaration(nsCSSDeclaration* aDeclaration, const nsStyleStructID& aSID, nsRuleDataMargin& aMargin)
{
  nsCSSMargin*  ourMargin = (nsCSSMargin*)aDeclaration->GetData(kCSSMarginSID);
  if (!ourMargin)
    return NS_OK;

  // Margins
  if (aSID == eStyleStruct_Margin && ourMargin->mMargin) {
    if (eCSSUnit_Null == aMargin.mMargin->mLeft.GetUnit() && eCSSUnit_Null != ourMargin->mMargin->mLeft.GetUnit())
      aMargin.mMargin->mLeft = ourMargin->mMargin->mLeft;

    if (eCSSUnit_Null == aMargin.mMargin->mTop.GetUnit() && eCSSUnit_Null != ourMargin->mMargin->mTop.GetUnit())
      aMargin.mMargin->mTop = ourMargin->mMargin->mTop;

    if (eCSSUnit_Null == aMargin.mMargin->mRight.GetUnit() && eCSSUnit_Null != ourMargin->mMargin->mRight.GetUnit())
      aMargin.mMargin->mRight = ourMargin->mMargin->mRight;

    if (eCSSUnit_Null == aMargin.mMargin->mBottom.GetUnit() && eCSSUnit_Null != ourMargin->mMargin->mBottom.GetUnit())
      aMargin.mMargin->mBottom = ourMargin->mMargin->mBottom;
  }

  // Padding
  if (aSID == eStyleStruct_Padding && ourMargin->mPadding) {
    if (eCSSUnit_Null == aMargin.mPadding->mLeft.GetUnit() && eCSSUnit_Null != ourMargin->mPadding->mLeft.GetUnit())
      aMargin.mPadding->mLeft = ourMargin->mPadding->mLeft;

    if (eCSSUnit_Null == aMargin.mPadding->mTop.GetUnit() && eCSSUnit_Null != ourMargin->mPadding->mTop.GetUnit())
      aMargin.mPadding->mTop = ourMargin->mPadding->mTop;

    if (eCSSUnit_Null == aMargin.mPadding->mRight.GetUnit() && eCSSUnit_Null != ourMargin->mPadding->mRight.GetUnit())
      aMargin.mPadding->mRight = ourMargin->mPadding->mRight;

    if (eCSSUnit_Null == aMargin.mPadding->mBottom.GetUnit() && eCSSUnit_Null != ourMargin->mPadding->mBottom.GetUnit())
      aMargin.mPadding->mBottom = ourMargin->mPadding->mBottom;
  }

  // Borders
  if (aSID == eStyleStruct_Border) {
    // border-size
    if (ourMargin->mBorderWidth) {
      if (eCSSUnit_Null == aMargin.mBorderWidth->mLeft.GetUnit() && eCSSUnit_Null != ourMargin->mBorderWidth->mLeft.GetUnit())
        aMargin.mBorderWidth->mLeft = ourMargin->mBorderWidth->mLeft;

      if (eCSSUnit_Null == aMargin.mBorderWidth->mTop.GetUnit() && eCSSUnit_Null != ourMargin->mBorderWidth->mTop.GetUnit())
        aMargin.mBorderWidth->mTop = ourMargin->mBorderWidth->mTop;

      if (eCSSUnit_Null == aMargin.mBorderWidth->mRight.GetUnit() && eCSSUnit_Null != ourMargin->mBorderWidth->mRight.GetUnit())
        aMargin.mBorderWidth->mRight = ourMargin->mBorderWidth->mRight;

      if (eCSSUnit_Null == aMargin.mBorderWidth->mBottom.GetUnit() && eCSSUnit_Null != ourMargin->mBorderWidth->mBottom.GetUnit())
        aMargin.mBorderWidth->mBottom = ourMargin->mBorderWidth->mBottom;
    }

    // border-style
    if (ourMargin->mBorderStyle) {
      if (eCSSUnit_Null == aMargin.mBorderStyle->mLeft.GetUnit() && eCSSUnit_Null != ourMargin->mBorderStyle->mLeft.GetUnit())
        aMargin.mBorderStyle->mLeft = ourMargin->mBorderStyle->mLeft;

      if (eCSSUnit_Null == aMargin.mBorderStyle->mTop.GetUnit() && eCSSUnit_Null != ourMargin->mBorderStyle->mTop.GetUnit())
        aMargin.mBorderStyle->mTop = ourMargin->mBorderStyle->mTop;

      if (eCSSUnit_Null == aMargin.mBorderStyle->mRight.GetUnit() && eCSSUnit_Null != ourMargin->mBorderStyle->mRight.GetUnit())
        aMargin.mBorderStyle->mRight = ourMargin->mBorderStyle->mRight;

      if (eCSSUnit_Null == aMargin.mBorderStyle->mBottom.GetUnit() && eCSSUnit_Null != ourMargin->mBorderStyle->mBottom.GetUnit())
        aMargin.mBorderStyle->mBottom = ourMargin->mBorderStyle->mBottom;
    }

    // border-color
    if (ourMargin->mBorderColor) {
      if (eCSSUnit_Null == aMargin.mBorderColor->mLeft.GetUnit() && eCSSUnit_Null != ourMargin->mBorderColor->mLeft.GetUnit())
        aMargin.mBorderColor->mLeft = ourMargin->mBorderColor->mLeft;

      if (eCSSUnit_Null == aMargin.mBorderColor->mTop.GetUnit() && eCSSUnit_Null != ourMargin->mBorderColor->mTop.GetUnit())
        aMargin.mBorderColor->mTop = ourMargin->mBorderColor->mTop;

      if (eCSSUnit_Null == aMargin.mBorderColor->mRight.GetUnit() && eCSSUnit_Null != ourMargin->mBorderColor->mRight.GetUnit())
        aMargin.mBorderColor->mRight = ourMargin->mBorderColor->mRight;

      if (eCSSUnit_Null == aMargin.mBorderColor->mBottom.GetUnit() && eCSSUnit_Null != ourMargin->mBorderColor->mBottom.GetUnit())
        aMargin.mBorderColor->mBottom = ourMargin->mBorderColor->mBottom;
    }

    // border-colors
    if (ourMargin->mBorderColors) {
      for (PRInt32 i = 0; i < 4; i++)
        if (!aMargin.mBorderColors[i] && ourMargin->mBorderColors[i])
          aMargin.mBorderColors[i] = ourMargin->mBorderColors[i];
    }

    // -moz-border-radius
    if (ourMargin->mBorderRadius) {
      if (eCSSUnit_Null == aMargin.mBorderRadius->mLeft.GetUnit() && eCSSUnit_Null != ourMargin->mBorderRadius->mLeft.GetUnit())
        aMargin.mBorderRadius->mLeft = ourMargin->mBorderRadius->mLeft;

      if (eCSSUnit_Null == aMargin.mBorderRadius->mTop.GetUnit() && eCSSUnit_Null != ourMargin->mBorderRadius->mTop.GetUnit())
        aMargin.mBorderRadius->mTop = ourMargin->mBorderRadius->mTop;

      if (eCSSUnit_Null == aMargin.mBorderRadius->mRight.GetUnit() && eCSSUnit_Null != ourMargin->mBorderRadius->mRight.GetUnit())
        aMargin.mBorderRadius->mRight = ourMargin->mBorderRadius->mRight;

      if (eCSSUnit_Null == aMargin.mBorderRadius->mBottom.GetUnit() && eCSSUnit_Null != ourMargin->mBorderRadius->mBottom.GetUnit())
        aMargin.mBorderRadius->mBottom = ourMargin->mBorderRadius->mBottom;
    }

    // float-edge
    if (eCSSUnit_Null == aMargin.mFloatEdge.GetUnit() && eCSSUnit_Null != ourMargin->mFloatEdge.GetUnit())
      aMargin.mFloatEdge = ourMargin->mFloatEdge;
  }

  // Outline
  if (aSID == eStyleStruct_Outline) {
    // -moz-outline-radius
    if (ourMargin->mOutlineRadius) {
      if (eCSSUnit_Null == aMargin.mOutlineRadius->mLeft.GetUnit() && eCSSUnit_Null != ourMargin->mOutlineRadius->mLeft.GetUnit())
        aMargin.mOutlineRadius->mLeft = ourMargin->mOutlineRadius->mLeft;

      if (eCSSUnit_Null == aMargin.mOutlineRadius->mTop.GetUnit() && eCSSUnit_Null != ourMargin->mOutlineRadius->mTop.GetUnit())
        aMargin.mOutlineRadius->mTop = ourMargin->mOutlineRadius->mTop;

      if (eCSSUnit_Null == aMargin.mOutlineRadius->mRight.GetUnit() && eCSSUnit_Null != ourMargin->mOutlineRadius->mRight.GetUnit())
        aMargin.mOutlineRadius->mRight = ourMargin->mOutlineRadius->mRight;

      if (eCSSUnit_Null == aMargin.mOutlineRadius->mBottom.GetUnit() && eCSSUnit_Null != ourMargin->mOutlineRadius->mBottom.GetUnit())
        aMargin.mOutlineRadius->mBottom = ourMargin->mOutlineRadius->mBottom;
    }

    // outline-width
    if (eCSSUnit_Null == aMargin.mOutlineWidth.GetUnit() && eCSSUnit_Null != ourMargin->mOutlineWidth.GetUnit())
      aMargin.mOutlineWidth = ourMargin->mOutlineWidth;

    // outline-color
    if (eCSSUnit_Null == aMargin.mOutlineColor.GetUnit() && eCSSUnit_Null != ourMargin->mOutlineColor.GetUnit())
      aMargin.mOutlineColor = ourMargin->mOutlineColor;

    // outline-style
    if (eCSSUnit_Null == aMargin.mOutlineStyle.GetUnit() && eCSSUnit_Null != ourMargin->mOutlineStyle.GetUnit())
      aMargin.mOutlineStyle = ourMargin->mOutlineStyle;
  }

  return NS_OK;
}

static nsresult
MapColorForDeclaration(nsCSSDeclaration* aDecl, const nsStyleStructID& aID, nsRuleDataColor& aColor)
{
  if (!aDecl)
    return NS_OK;

  nsCSSColor* ourColor = (nsCSSColor*)aDecl->GetData(kCSSColorSID);
  if (!ourColor)
    return NS_OK; // No rules for color or background.

  if (aID == eStyleStruct_Color) {
    // color: color, string, inherit
    if (aColor.mColor.GetUnit() == eCSSUnit_Null && ourColor->mColor.GetUnit() != eCSSUnit_Null)
      aColor.mColor = ourColor->mColor;
  }
  else if (aID == eStyleStruct_Background) {
    // background-color: color, string, enum (flags), inherit
    if (aColor.mBackColor.GetUnit() == eCSSUnit_Null && ourColor->mBackColor.GetUnit() != eCSSUnit_Null)
      aColor.mBackColor = ourColor->mBackColor;

    // background-image: url, none, inherit
    if (aColor.mBackImage.GetUnit() == eCSSUnit_Null && ourColor->mBackImage.GetUnit() != eCSSUnit_Null)
      aColor.mBackImage = ourColor->mBackImage;

    // background-repeat: enum, inherit
    if (aColor.mBackRepeat.GetUnit() == eCSSUnit_Null && ourColor->mBackRepeat.GetUnit() != eCSSUnit_Null)
      aColor.mBackRepeat = ourColor->mBackRepeat;

    // background-attachment: enum, inherit
    if (aColor.mBackAttachment.GetUnit() == eCSSUnit_Null && ourColor->mBackAttachment.GetUnit() != eCSSUnit_Null)
      aColor.mBackAttachment = ourColor->mBackAttachment;
      
    // background-position: enum, length, percent (flags), inherit
    if (aColor.mBackPositionX.GetUnit() == eCSSUnit_Null && ourColor->mBackPositionX.GetUnit() != eCSSUnit_Null)
      aColor.mBackPositionX = ourColor->mBackPositionX;
    if (aColor.mBackPositionY.GetUnit() == eCSSUnit_Null && ourColor->mBackPositionY.GetUnit() != eCSSUnit_Null)
      aColor.mBackPositionY = ourColor->mBackPositionY;

    // background-clip: enum, inherit
    if (aColor.mBackClip.GetUnit() == eCSSUnit_Null && ourColor->mBackClip.GetUnit() != eCSSUnit_Null)
      aColor.mBackClip = ourColor->mBackClip;

    // background-inline-policy: enum, inherit
    if (aColor.mBackInlinePolicy.GetUnit() == eCSSUnit_Null && ourColor->mBackInlinePolicy.GetUnit() != eCSSUnit_Null)
      aColor.mBackInlinePolicy = ourColor->mBackInlinePolicy;

    // background-origin: enum, inherit
    if (aColor.mBackOrigin.GetUnit() == eCSSUnit_Null && ourColor->mBackOrigin.GetUnit() != eCSSUnit_Null)
      aColor.mBackOrigin = ourColor->mBackOrigin;
  }

  return NS_OK;
}

static nsresult 
MapTableForDeclaration(nsCSSDeclaration* aDecl, const nsStyleStructID& aID, nsRuleDataTable& aTable)
{
  if (!aDecl)
    return NS_OK; // The rule must have a declaration.

  nsCSSTable* ourTable = (nsCSSTable*)aDecl->GetData(kCSSTableSID);
  if (!ourTable)
    return NS_OK; // We don't have any rules for tables.

  if (aID == eStyleStruct_TableBorder) {
    // border-collapse: enum, inherit
    if (aTable.mBorderCollapse.GetUnit() == eCSSUnit_Null && ourTable->mBorderCollapse.GetUnit() != eCSSUnit_Null)
      aTable.mBorderCollapse = ourTable->mBorderCollapse;

    // border-spacing-x: length, inherit
    if (aTable.mBorderSpacingX.GetUnit() == eCSSUnit_Null && ourTable->mBorderSpacingX.GetUnit() != eCSSUnit_Null)
      aTable.mBorderSpacingX = ourTable->mBorderSpacingX;

    // border-spacing-y: length, inherit
    if (aTable.mBorderSpacingY.GetUnit() == eCSSUnit_Null && ourTable->mBorderSpacingY.GetUnit() != eCSSUnit_Null)
      aTable.mBorderSpacingY = ourTable->mBorderSpacingY;

    // caption-side: enum, inherit
    if (aTable.mCaptionSide.GetUnit() == eCSSUnit_Null && ourTable->mCaptionSide.GetUnit() != eCSSUnit_Null)
      aTable.mCaptionSide = ourTable->mCaptionSide;

    // empty-cells: enum, inherit
    if (aTable.mEmptyCells.GetUnit() == eCSSUnit_Null && ourTable->mEmptyCells.GetUnit() != eCSSUnit_Null)
      aTable.mEmptyCells = ourTable->mEmptyCells;
  }
  else if (aID == eStyleStruct_Table) {
    // table-layout: auto, enum, inherit
    if (aTable.mLayout.GetUnit() == eCSSUnit_Null && ourTable->mLayout.GetUnit() != eCSSUnit_Null)
      aTable.mLayout = ourTable->mLayout;
  }

  return NS_OK;
}

static nsresult 
MapContentForDeclaration(nsCSSDeclaration* aDecl, const nsStyleStructID& aID, nsRuleDataContent& aContent)
{
  if (!aDecl)
    return NS_OK; // The rule must have a declaration.

  nsCSSContent* ourContent = (nsCSSContent*)aDecl->GetData(kCSSContentSID);
  if (!ourContent)
    return NS_OK; // We don't have any rules for content.

  if (aID == eStyleStruct_Content) {
    if (!aContent.mContent && ourContent->mContent)
      aContent.mContent = ourContent->mContent;

    if (!aContent.mCounterIncrement && ourContent->mCounterIncrement)
      aContent.mCounterIncrement = ourContent->mCounterIncrement;

    if (!aContent.mCounterReset && ourContent->mCounterReset)
      aContent.mCounterReset = ourContent->mCounterReset;
    
    if (aContent.mMarkerOffset.GetUnit() == eCSSUnit_Null && ourContent->mMarkerOffset.GetUnit() != eCSSUnit_Null)
      aContent.mMarkerOffset = ourContent->mMarkerOffset;
  }
  else if (aID == eStyleStruct_Quotes) {
    if (!aContent.mQuotes && ourContent->mQuotes)
      aContent.mQuotes = ourContent->mQuotes;
  }

  return NS_OK;
}

static nsresult
MapTextForDeclaration(nsCSSDeclaration* aDecl, const nsStyleStructID& aID, nsRuleDataText& aText)
{
  if (!aDecl)
    return NS_OK; // The rule must have a declaration.

  nsCSSText* ourText = (nsCSSText*)aDecl->GetData(kCSSTextSID);
  if (!ourText)
    return NS_OK; // We don't have any rules for text.

  if (aID == eStyleStruct_Text) {
    if (aText.mLetterSpacing.GetUnit() == eCSSUnit_Null && ourText->mLetterSpacing.GetUnit() != eCSSUnit_Null)
      aText.mLetterSpacing = ourText->mLetterSpacing;

    if (aText.mLineHeight.GetUnit() == eCSSUnit_Null && ourText->mLineHeight.GetUnit() != eCSSUnit_Null)
      aText.mLineHeight = ourText->mLineHeight;

    if (aText.mTextIndent.GetUnit() == eCSSUnit_Null && ourText->mTextIndent.GetUnit() != eCSSUnit_Null)
      aText.mTextIndent = ourText->mTextIndent;

    if (aText.mTextTransform.GetUnit() == eCSSUnit_Null && ourText->mTextTransform.GetUnit() != eCSSUnit_Null)
      aText.mTextTransform = ourText->mTextTransform;

    if (aText.mTextAlign.GetUnit() == eCSSUnit_Null && ourText->mTextAlign.GetUnit() != eCSSUnit_Null)
      aText.mTextAlign = ourText->mTextAlign;

    if (aText.mWhiteSpace.GetUnit() == eCSSUnit_Null && ourText->mWhiteSpace.GetUnit() != eCSSUnit_Null)
      aText.mWhiteSpace = ourText->mWhiteSpace;

    if (aText.mWordSpacing.GetUnit() == eCSSUnit_Null && ourText->mWordSpacing.GetUnit() != eCSSUnit_Null)
      aText.mWordSpacing = ourText->mWordSpacing;
  }
  else if (aID == eStyleStruct_TextReset) {
    if (aText.mVerticalAlign.GetUnit() == eCSSUnit_Null && ourText->mVerticalAlign.GetUnit() != eCSSUnit_Null)
      aText.mVerticalAlign = ourText->mVerticalAlign;

    if (aText.mDecoration.GetUnit() == eCSSUnit_Null && ourText->mDecoration.GetUnit() != eCSSUnit_Null)
      aText.mDecoration = ourText->mDecoration;

    if (aText.mUnicodeBidi.GetUnit() == eCSSUnit_Null && ourText->mUnicodeBidi.GetUnit() != eCSSUnit_Null)
      aText.mUnicodeBidi = ourText->mUnicodeBidi;
  }

  return NS_OK;

}

static nsresult 
MapDisplayForDeclaration(nsCSSDeclaration* aDecl, const nsStyleStructID& aID, nsRuleDataDisplay& aDisplay)
{
  if (!aDecl)
    return NS_OK; // The rule must have a declaration.

  nsCSSDisplay* ourDisplay = (nsCSSDisplay*)aDecl->GetData(kCSSDisplaySID);
  if (!ourDisplay)
    return NS_OK; // We don't have any rules for display.

  if (aID == eStyleStruct_Display) {
    // appearance: enum, none, inherit
    if (aDisplay.mAppearance.GetUnit() == eCSSUnit_Null && 
        ourDisplay->mAppearance.GetUnit() != eCSSUnit_Null)
      aDisplay.mAppearance = ourDisplay->mAppearance;

    // display: enum, none, inherit
    if (aDisplay.mDisplay.GetUnit() == eCSSUnit_Null && ourDisplay->mDisplay.GetUnit() != eCSSUnit_Null)
      aDisplay.mDisplay = ourDisplay->mDisplay;
    
    // binding: url, none, inherit
    if (aDisplay.mBinding.GetUnit() == eCSSUnit_Null && ourDisplay->mBinding.GetUnit() != eCSSUnit_Null)
      aDisplay.mBinding = ourDisplay->mBinding;

    // position: enum, inherit
    if (aDisplay.mPosition.GetUnit() == eCSSUnit_Null && ourDisplay->mPosition.GetUnit() != eCSSUnit_Null)
      aDisplay.mPosition = ourDisplay->mPosition;

    // clear: enum, none, inherit
    if (aDisplay.mClear.GetUnit() == eCSSUnit_Null && ourDisplay->mClear.GetUnit() != eCSSUnit_Null)
      aDisplay.mClear = ourDisplay->mClear;

    // temp fix for bug 24000
    if (aDisplay.mBreakBefore.GetUnit() == eCSSUnit_Null && ourDisplay->mBreakBefore.GetUnit() != eCSSUnit_Null)
      aDisplay.mBreakBefore = ourDisplay->mBreakBefore;
    if (aDisplay.mBreakAfter.GetUnit() == eCSSUnit_Null && ourDisplay->mBreakAfter.GetUnit() != eCSSUnit_Null)
      aDisplay.mBreakAfter = ourDisplay->mBreakAfter;
    // end temp fix

    // float: enum, none, inherit
    if (aDisplay.mFloat.GetUnit() == eCSSUnit_Null && ourDisplay->mFloat.GetUnit() != eCSSUnit_Null)
      aDisplay.mFloat = ourDisplay->mFloat;

    // overflow: enum, auto, inherit
    if (aDisplay.mOverflow.GetUnit() == eCSSUnit_Null && ourDisplay->mOverflow.GetUnit() != eCSSUnit_Null)
      aDisplay.mOverflow = ourDisplay->mOverflow;
    
    // clip property: length, auto, inherit
    if (ourDisplay->mClip) {
      if (aDisplay.mClip->mLeft.GetUnit() == eCSSUnit_Null && ourDisplay->mClip->mLeft.GetUnit() != eCSSUnit_Null)
        aDisplay.mClip->mLeft = ourDisplay->mClip->mLeft;
      if (aDisplay.mClip->mRight.GetUnit() == eCSSUnit_Null && ourDisplay->mClip->mRight.GetUnit() != eCSSUnit_Null)
        aDisplay.mClip->mRight = ourDisplay->mClip->mRight;
      if (aDisplay.mClip->mTop.GetUnit() == eCSSUnit_Null && ourDisplay->mClip->mTop.GetUnit() != eCSSUnit_Null)
        aDisplay.mClip->mTop = ourDisplay->mClip->mTop;
      if (aDisplay.mClip->mBottom.GetUnit() == eCSSUnit_Null && ourDisplay->mClip->mBottom.GetUnit() != eCSSUnit_Null)
        aDisplay.mClip->mBottom = ourDisplay->mClip->mBottom;
    }
  }
  else if (aID == eStyleStruct_Visibility) {
    // opacity: factor, percent, inherit
    if (aDisplay.mOpacity.GetUnit() == eCSSUnit_Null && ourDisplay->mOpacity.GetUnit() != eCSSUnit_Null)
      aDisplay.mOpacity = ourDisplay->mOpacity;

    // direction: enum, inherit
    if (aDisplay.mDirection.GetUnit() == eCSSUnit_Null && ourDisplay->mDirection.GetUnit() != eCSSUnit_Null)
      aDisplay.mDirection = ourDisplay->mDirection;

    // visibility: enum, inherit
    if (aDisplay.mVisibility.GetUnit() == eCSSUnit_Null && ourDisplay->mVisibility.GetUnit() != eCSSUnit_Null)
      aDisplay.mVisibility = ourDisplay->mVisibility; 
  }

  return NS_OK;
}

static nsresult
MapUIForDeclaration(nsCSSDeclaration* aDecl, const nsStyleStructID& aID, nsRuleDataUserInterface& aUI)
{
  if (!aDecl)
    return NS_OK; // The rule must have a declaration.

  nsCSSUserInterface* ourUI = (nsCSSUserInterface*)aDecl->GetData(kCSSUserInterfaceSID);
  if (!ourUI)
    return NS_OK; // We don't have any rules for UI.

  if (aID == eStyleStruct_UserInterface) {
    if (aUI.mUserFocus.GetUnit() == eCSSUnit_Null && ourUI->mUserFocus.GetUnit() != eCSSUnit_Null)
      aUI.mUserFocus = ourUI->mUserFocus;
    
    if (aUI.mUserInput.GetUnit() == eCSSUnit_Null && ourUI->mUserInput.GetUnit() != eCSSUnit_Null)
      aUI.mUserInput = ourUI->mUserInput;

    if (aUI.mUserModify.GetUnit() == eCSSUnit_Null && ourUI->mUserModify.GetUnit() != eCSSUnit_Null)
      aUI.mUserModify = ourUI->mUserModify;

    if (!aUI.mCursor && ourUI->mCursor)
      aUI.mCursor = ourUI->mCursor;


  }
  else if (aID == eStyleStruct_UIReset) {
    if (aUI.mUserSelect.GetUnit() == eCSSUnit_Null && ourUI->mUserSelect.GetUnit() != eCSSUnit_Null)
      aUI.mUserSelect = ourUI->mUserSelect;
   
    if (!aUI.mKeyEquivalent && ourUI->mKeyEquivalent)
      aUI.mKeyEquivalent = ourUI->mKeyEquivalent;

    if (aUI.mResizer.GetUnit() == eCSSUnit_Null && ourUI->mResizer.GetUnit() != eCSSUnit_Null)
      aUI.mResizer = ourUI->mResizer;

    if (aUI.mForceBrokenImageIcon.GetUnit() == eCSSUnit_Null && ourUI->mForceBrokenImageIcon.GetUnit() == eCSSUnit_Integer)
      aUI.mForceBrokenImageIcon = ourUI->mForceBrokenImageIcon;
  }

  return NS_OK;

}
#endif
// end bug 125246

#ifdef DEBUG
NS_IMETHODIMP
CSSStyleRuleImpl::List(FILE* out, PRInt32 aIndent) const
{
  // Indent
  for (PRInt32 index = aIndent; --index >= 0; ) fputs("  ", out);

  nsAutoString buffer;
//  mSelector.ToString(buffer, mSheet, PR_FALSE, 0);
  if (mSelector)
    mSelector->ToString(buffer, mSheet);
// end bug 98765

//  buffer.Append(NS_LITERAL_STRING(" weight: "));
//  buffer.AppendInt(mWeight, 10);
  buffer.Append(NS_LITERAL_STRING(" "));
  fputs(NS_LossyConvertUCS2toASCII(buffer).get(), out);
  if (nsnull != mDeclaration) {
    mDeclaration->List(out);
  }
  else {
    fputs("{ null declaration }", out);
  }
  fputs("\n", out);

  return NS_OK;
}

/******************************************************************************
* SizeOf method:
*
*  Self (reported as CSSStyleRuleImpl's size): 
*    1) sizeof(*this) 
*       + sizeof the DOMDeclaration if it exists and is unique
*
*  Contained / Aggregated data (not reported as CSSStyleRuleImpl's size):
*    1) mDeclaration if it exists
*    2) mImportantRule if it exists
*
*  Children / siblings / parents:
*    none
*    
******************************************************************************/
void CSSStyleRuleImpl::SizeOf(nsISizeOfHandler *aSizeOfHandler, PRUint32 &aSize)
{
  NS_ASSERTION(aSizeOfHandler != nsnull, "SizeOf handler cannot be null");

  // first get the unique items collection
  UNIQUE_STYLE_ITEMS(uniqueItems);
  if(! uniqueItems->AddItem((void*)this)){
    return;
  }

  PRUint32 localSize=0;

  // create a tag for this instance
  nsCOMPtr<nsIAtom> tag = do_GetAtom("CSSStyleRuleImpl");
  // get the size of an empty instance and add to the sizeof handler
  aSize = sizeof(*this);
  // remove the sizeof the mSelector's class since we count it seperately below
  aSize -= sizeof(mSelector);

  // and add the size of the DOMDeclaration
  // XXX - investigate the size and quantity of these
  if(mDOMDeclaration && uniqueItems->AddItem(mDOMDeclaration)){
    aSize += sizeof(DOMCSSDeclarationImpl);
  }
  aSizeOfHandler->AddSize(tag,aSize);
  
  // now delegate to the Selector, Declaration, and ImportantRule
  mSelector.SizeOf(aSizeOfHandler, localSize);

  if(mDeclaration){
    mDeclaration->SizeOf(aSizeOfHandler, localSize);
  }
  if(mImportantRule){
    mImportantRule->SizeOf(aSizeOfHandler, localSize);
  }
}
#endif

// bug 188803
#if(0)
NS_IMETHODIMP    
CSSStyleRuleImpl::GetType(PRUint16* aType)
{
  *aType = nsIDOMCSSRule::STYLE_RULE;
  
  return NS_OK;
}
#endif
// end bug

// bug 188803
//NS_IMETHODIMP
/* virtual */ nsresult    
CSSStyleRuleImpl::GetCssText(nsAString& aCssText)
{

// bug 98765
#if(0)
  mSelector.ToString( aCssText, mSheet, IsPseudoElement(mSelector.mTag),
                      0 );
  aCssText.Append(PRUnichar(' '));
#else
  if (mSelector) {
    mSelector->ToString(aCssText, mSheet);
    aCssText.Append(PRUnichar(' '));
  }
#endif
// end bug

  aCssText.Append(PRUnichar('{'));
  aCssText.Append(PRUnichar(' '));
  if (mDeclaration)
  {
    nsAutoString   tempString;
    mDeclaration->ToString( tempString );
    aCssText.Append( tempString );
  }
  aCssText.Append(PRUnichar(' '));
  aCssText.Append(PRUnichar('}'));
  return NS_OK;
}
// bug 188803
//NS_IMETHODIMP
/* virtual */ nsresult     
CSSStyleRuleImpl::SetCssText(const nsAString& aCssText)
{
  // XXX TBI - need to re-parse rule & declaration
  return NS_OK;
}

// bug 188803
// PATCH IS FIRST!
#if(1)
/* virtual */ nsresult    
CSSStyleRuleImpl::GetParentStyleSheet(nsICSSStyleSheet** aSheet)
{
  *aSheet = mSheet;
  NS_IF_ADDREF(*aSheet);
  return NS_OK;
}

/* virtual */ nsresult    
CSSStyleRuleImpl::GetParentRule(nsICSSGroupRule** aParentRule)
{
  *aParentRule = mParentRule;
  NS_IF_ADDREF(*aParentRule);
  return NS_OK;
}

/* virtual */ nsresult    
CSSStyleRuleImpl::GetSelectorText(nsAString& aSelectorText)
{
  if (mSelector)
    mSelector->ToString(aSelectorText, mSheet);
  else
    aSelectorText.Truncate();
  return NS_OK;
}

/* virtual */ nsresult    
CSSStyleRuleImpl::SetSelectorText(const nsAString& aSelectorText)
{
  // XXX TBI - get a parser and re-parse the selectors, 
  // XXX then need to re-compute the cascade
  // XXX and dirty sheet
  return NS_OK;
}

#else

NS_IMETHODIMP    
CSSStyleRuleImpl::GetParentStyleSheet(nsIDOMCSSStyleSheet** aSheet)
{
  if (mSheet) {
    return CallQueryInterface(mSheet, aSheet);
  }
  *aSheet = nsnull;
  return NS_OK;
}

NS_IMETHODIMP    
CSSStyleRuleImpl::GetParentRule(nsIDOMCSSRule** aParentRule)
{
  if (mParentRule) {
    return CallQueryInterface(mParentRule, aParentRule);
  }
  *aParentRule = nsnull;
  return NS_OK;
}

NS_IMETHODIMP    
CSSStyleRuleImpl::GetSelectorText(nsAString& aSelectorText)
{
// bug 98765
#if(0)
  mSelector.ToString( aSelectorText, mSheet, IsPseudoElement(mSelector.mTag), 0 );
#else
  if (mSelector)
    mSelector->ToString(aSelectorText, mSheet);
  else
    aSelectorText.Truncate();
#endif
// end bug
  return NS_OK;
}

NS_IMETHODIMP    
CSSStyleRuleImpl::SetSelectorText(const nsAString& aSelectorText)
{
  // XXX TBI - get a parser and re-parse the selectors, 
  // XXX then need to re-compute the cascade
  // XXX and dirty sheet
  return NS_OK;
}

NS_IMETHODIMP    
CSSStyleRuleImpl::GetStyle(nsIDOMCSSStyleDeclaration** aStyle)
{
  if (nsnull == mDOMDeclaration) {
    mDOMDeclaration = new DOMCSSDeclarationImpl(this);
    if (nsnull == mDOMDeclaration) {
      return NS_ERROR_OUT_OF_MEMORY;
    }
    NS_ADDREF(mDOMDeclaration);
  }
  
  *aStyle = mDOMDeclaration;
  NS_ADDREF(mDOMDeclaration);
  
  return NS_OK;
}

#endif
// end bug 188803

NS_EXPORT nsresult
  NS_NewCSSStyleRule(nsICSSStyleRule** aInstancePtrResult, 
  	//const nsCSSSelector& aSelector)
  	nsCSSSelectorList* aSelector,
  	nsCSSDeclaration* aDeclaration) // bug 188803
{
// bug 188803
#if(0)
  if (aInstancePtrResult == nsnull) {
    return NS_ERROR_NULL_POINTER;
  }

  CSSStyleRuleImpl *it = new CSSStyleRuleImpl(aSelector);
#else
  CSSStyleRuleImpl *it = new CSSStyleRuleImpl(aSelector, aDeclaration);
#endif
// end bug

  if (!it) {
    return NS_ERROR_OUT_OF_MEMORY;
  }

  return CallQueryInterface(it, aInstancePtrResult);
}
