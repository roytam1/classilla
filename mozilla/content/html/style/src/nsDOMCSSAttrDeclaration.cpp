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

#include "nsDOMCSSAttrDeclaration.h"
#include "nsCSSDeclaration.h"
#include "nsIDocument.h"
#include "nsHTMLAtoms.h"
#include "nsIHTMLContent.h"
#include "nsIDOMMutationEvent.h"
#include "nsHTMLValue.h"
#include "nsICSSStyleRule.h"
#include "nsINodeInfo.h"
#include "nsICSSLoader.h"
#include "nsICSSParser.h"
#include "nsIURI.h"
#include "nsINameSpaceManager.h"
#include "nsIHTMLContentContainer.h"
#include "nsStyleConsts.h"

MOZ_DECL_CTOR_COUNTER(nsDOMCSSAttributeDeclaration)

nsDOMCSSAttributeDeclaration::nsDOMCSSAttributeDeclaration(nsIHTMLContent *aContent)
{
  MOZ_COUNT_CTOR(nsDOMCSSAttributeDeclaration);

  // This reference is not reference-counted. The content
  // object tells us when its about to go away.
  mContent = aContent;
}

nsDOMCSSAttributeDeclaration::~nsDOMCSSAttributeDeclaration()
{
  MOZ_COUNT_DTOR(nsDOMCSSAttributeDeclaration);
}


// bug 188803
#if(0)
NS_IMETHODIMP
nsDOMCSSAttributeDeclaration::RemoveProperty(const nsAString& aPropertyName,
                                             nsAString& aReturn)
{
  aReturn.Truncate(); // bug 125246
  nsCSSDeclaration* decl;

// bug 171830
#if(0)
  nsresult rv = GetCSSDeclaration(&decl, PR_TRUE); 

  if (NS_SUCCEEDED(rv) && decl && mContent) {
    nsCOMPtr<nsIDocument> doc;
    mContent->GetDocument(*getter_AddRefs(doc));

    if (doc) {
      doc->BeginUpdate();

      doc->AttributeWillChange(mContent, kNameSpaceID_None,
                               nsHTMLAtoms::style);
    }

    nsChangeHint hint = NS_STYLE_HINT_NONE;
#else
  nsresult rv = GetCSSDeclaration(&decl, PR_FALSE);
  if (NS_SUCCEEDED(rv) && decl) {
#endif
// end bug

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
// bug 171830
#if(0)
      // We pass in eCSSProperty_UNKNOWN here so that we don't get the
      // property name in the return string.
      val.ToString(aReturn, eCSSProperty_UNKNOWN);
      hint = nsCSSProps::kHintTable[prop];
    } else {
      // If we tried to remove an invalid property or a property that wasn't
      //  set we simply return success and an empty string
      rv = NS_OK;
    }

    if (doc) {
      doc->AttributeChanged(mContent, kNameSpaceID_None, nsHTMLAtoms::style,
                            nsIDOMMutationEvent::MODIFICATION, 
                            hint);
      doc->EndUpdate();
#else
      rv = SetCSSDeclaration(decl, PR_TRUE);
#endif
// end bug
// backbugs
    } else {
      // RemoveProperty will throw in all sorts of situations -- eg if
      // the property is a shorthand one.  Do not propagate its return
      // value to callers.
      rv = NS_OK;
    }
// end backbugs
  }

  return rv;
}
#endif
// end bug 188803


void
nsDOMCSSAttributeDeclaration::DropReference()
{
  mContent = nsnull;
}

// bug 188803
// PATCH IS FIRST!
#if(1)
nsresult
nsDOMCSSAttributeDeclaration::DeclarationChanged()
{
  NS_ASSERTION(mContent, "Must have content node to set the decl!");
  nsHTMLValue val;
  nsresult rv = mContent->GetHTMLAttribute(nsHTMLAtoms::style, val);
  NS_ASSERTION(rv == NS_CONTENT_ATTR_HAS_VALUE &&
               eHTMLUnit_ISupports == val.GetUnit(),
               "content must have rule");
  nsCOMPtr<nsICSSStyleRule> oldRule =
    do_QueryInterface(nsCOMPtr<nsISupports>(val.GetISupportsValue()));

  nsCOMPtr<nsICSSStyleRule> newRule = oldRule->DeclarationChanged(PR_FALSE);
  if (!newRule) {
    return NS_ERROR_OUT_OF_MEMORY;
  }
    
  return mContent->SetHTMLAttribute(nsHTMLAtoms::style,
                                    nsHTMLValue(newRule),
                                    PR_TRUE);
}

#else

// bug 171830
nsresult
nsDOMCSSAttributeDeclaration::SetCSSDeclaration(nsCSSDeclaration* aDecl,
                                                PRBool aNotify)
                                                // aDeclOwnedByRule we don't have.
{
  NS_ASSERTION(mContent, "Must have content node to set the decl!");
    
  nsCOMPtr<nsICSSStyleRule> cssRule;
  nsresult rv = NS_NewCSSStyleRule(getter_AddRefs(cssRule), nsnull); // bug 98765 // nsCSSSelector());
  //NS_ENSURE_SUCCESS(rv, rv);
  if (NS_FAILED(rv)) {
		aDecl->RuleAbort(); // modified from bug 98765
		// we don't have an aDeclOwnedByRule. maybe we get this later?
		return rv;
  }
    
  cssRule->SetDeclaration(aDecl);
  // cssRule->SetWeight(PR_INT32_MAX); // bug 98765
  return mContent->SetHTMLAttribute(nsHTMLAtoms::style,
                                    nsHTMLValue(cssRule),
                                    aNotify);
}
// end bug

#endif
// end bug 188803


nsresult
nsDOMCSSAttributeDeclaration::GetCSSDeclaration(nsCSSDeclaration **aDecl,
                                                PRBool aAllocate)
{
  nsresult result = NS_OK;

  *aDecl = nsnull;
  if (mContent) {
    nsHTMLValue val;
// bug 171830
    //mContent->GetHTMLAttribute(nsHTMLAtoms::style, val);
    //if (eHTMLUnit_ISupports == val.GetUnit()) {
    result = mContent->GetHTMLAttribute(nsHTMLAtoms::style, val);
    if (result == NS_CONTENT_ATTR_HAS_VALUE &&
        eHTMLUnit_ISupports == val.GetUnit()) {
// end bug    
      nsCOMPtr<nsISupports> rule = val.GetISupportsValue();
      nsCOMPtr<nsICSSStyleRule> cssRule = do_QueryInterface(rule, &result);
      if (cssRule) {
        *aDecl = cssRule->GetDeclaration();
      }
    }
    else if (aAllocate) {
// bug 125246
// I combined 171830 and 125246 into one big superpatch here because otherwise
// it was just too confusing.
#if(0)
      result = NS_NewCSSDeclaration(aDecl);
// bug 171830
#if(0)
      if (NS_OK == result) {
        nsCOMPtr<nsICSSStyleRule> cssRule;
        result = NS_NewCSSStyleRule(getter_AddRefs(cssRule), nsCSSSelector());
        if (NS_OK == result) {
          cssRule->SetDeclaration(*aDecl);
          cssRule->SetWeight(0x7fffffff);
          result = mContent->SetHTMLAttribute(nsHTMLAtoms::style,
                                              nsHTMLValue(cssRule),
                                              PR_FALSE);
        }
        else {
#else
      if (NS_SUCCEEDED(result)) {
        result = SetCSSDeclaration(*aDecl, PR_FALSE);
        if (NS_FAILED(result)) {
#endif
// end bug
          (*aDecl)->RuleAbort();
          *aDecl = nsnull;
        }
#else
      nsCSSDeclaration *decl = new nsCSSDeclaration();
      if (!decl)
        return NS_ERROR_OUT_OF_MEMORY;
      if (!decl->InitializeEmpty()) {
        decl->RuleAbort();
        return NS_ERROR_OUT_OF_MEMORY;
      }
// bug 188803
      //result = SetCSSDeclaration(*aDecl, PR_FALSE); // modified for 1.3.1 , PR_FALSE);
      nsCOMPtr<nsICSSStyleRule> cssRule;
      result = NS_NewCSSStyleRule(getter_AddRefs(cssRule), nsnull, decl);
      if (NS_FAILED(result)) {
        decl->RuleAbort();
        return result;
      }
        
      result = mContent->SetHTMLAttribute(nsHTMLAtoms::style,
                                          nsHTMLValue(cssRule),
                                          PR_FALSE);
// end bug
      if (NS_SUCCEEDED(result)) {
        *aDecl = decl;
      }
#endif
// end bug 125246
    }
  }

  return result;
}

// bug 171830
#if(0)
nsresult
nsDOMCSSAttributeDeclaration::SetCSSDeclaration(nsCSSDeclaration *aDecl)
{
  nsresult result = NS_OK;

  if (mContent) {
    nsHTMLValue val;
    mContent->GetHTMLAttribute(nsHTMLAtoms::style, val);
    if (eHTMLUnit_ISupports == val.GetUnit()) {
      nsCOMPtr<nsISupports> rule = val.GetISupportsValue();
      nsCOMPtr<nsICSSStyleRule> cssRule = do_QueryInterface(rule, &result);
      if (cssRule) {
        cssRule->SetDeclaration(aDecl);
      }
    }
  }

  return result;
}
#endif
// end bug
/*
 * This is a utility function.  It will only fail if it can't get a
 * parser.  This means it can return NS_OK without aURI or aCSSLoader
 * being initialized
 */
nsresult
nsDOMCSSAttributeDeclaration::GetCSSParsingEnvironment(//nsIContent* aContent, // bug 188803
                                                       nsIURI** aBaseURI,
                                                       nsICSSLoader** aCSSLoader,
                                                       nsICSSParser** aCSSParser)
{
// aContent to mContent bug 188803.
  NS_ASSERTION(mContent, // aContent, 
  	"Something is severely broken -- there should be an nsIContent here!");
  // null out the out params since some of them may not get initialized below
  *aBaseURI = nsnull;
  *aCSSLoader = nsnull;
  *aCSSParser = nsnull;
  
  nsCOMPtr<nsINodeInfo> nodeInfo;
  //nsresult result = aContent->GetNodeInfo(*getter_AddRefs(nodeInfo));
  nsresult result = mContent->GetNodeInfo(*getter_AddRefs(nodeInfo));
  if (NS_FAILED(result)) {
    return result;
  }
  
// bug 211634
#if(0)
  nsCOMPtr<nsIDocument> doc;
  result = nodeInfo->GetDocument(*getter_AddRefs(doc));
  if (NS_FAILED(result)) {
    return result;
  }
#else
  // XXXbz GetOwnerDocument
  nsIDocument* doc = nodeInfo->GetDocument();
#endif
  
  if (doc) {
    doc->GetBaseURL(*aBaseURI);
    nsCOMPtr<nsIHTMLContentContainer> htmlContainer(do_QueryInterface(doc));
    if (htmlContainer) {
      htmlContainer->GetCSSLoader(*aCSSLoader);
    }
    NS_ASSERTION(*aCSSLoader, "Document with no CSS loader!");
  }
  if (*aCSSLoader) {
    result = (*aCSSLoader)->GetParserFor(nsnull, aCSSParser);
  } else {
    result = NS_NewCSSParser(aCSSParser);
  }
  if (NS_FAILED(result)) {
    return result;
  }
  
  // look up our namespace.  If we're XHTML, we need to be case-sensitive
  // Otherwise, we should not be
  (*aCSSParser)->SetCaseSensitive(nodeInfo->NamespaceEquals(kNameSpaceID_XHTML));

  return NS_OK;
}

// bug 188803
#if(0)
nsresult
nsDOMCSSAttributeDeclaration::ParsePropertyValue(const nsAString& aPropName,
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
// bug 171830
#if(0)
  nsCOMPtr<nsIDocument> doc;

  result = mContent->GetDocument(*getter_AddRefs(doc));
  if (NS_FAILED(result)) {
    return result;
  }
#endif
// end bug

  result = GetCSSParsingEnvironment(mContent,
                                    getter_AddRefs(baseURI),
                                    getter_AddRefs(cssLoader),
                                    getter_AddRefs(cssParser));
  if (NS_FAILED(result)) {
    return result;
  }

// bug 171830
#if(0)
  nsChangeHint hint = NS_STYLE_HINT_NONE;
  if (doc) {
    doc->BeginUpdate();
    doc->AttributeWillChange(mContent, kNameSpaceID_None, nsHTMLAtoms::style);
  }
  
  result = cssParser->ParseProperty(aPropName, aPropValue, baseURI, decl, &hint);
  if (doc) {
    doc->AttributeChanged(mContent, kNameSpaceID_None, nsHTMLAtoms::style,
                          nsIDOMMutationEvent::MODIFICATION, hint);
    doc->EndUpdate();
#else
  nsChangeHint uselessHint = NS_STYLE_HINT_NONE;
  result = cssParser->ParseProperty(aPropName, aPropValue, baseURI, decl,
                                    &uselessHint);
  if (NS_SUCCEEDED(result)) {
    result = SetCSSDeclaration(decl, PR_TRUE);
#endif
// end bug
  }

  if (cssLoader) {
    cssLoader->RecycleParser(cssParser);
  }

  return result; // bug 171830 // NS_OK;
}

nsresult
nsDOMCSSAttributeDeclaration::ParseDeclaration(const nsAString& aDecl,
                                               PRBool aParseOnlyOneDecl,
                                               PRBool aClearOldDecl)
{
  nsCSSDeclaration* decl;
  nsresult result = GetCSSDeclaration(&decl, PR_TRUE);

// bug 171830
#if(0)
  if (decl) {
    nsCOMPtr<nsICSSLoader> cssLoader;
    nsCOMPtr<nsICSSParser> cssParser;
    nsCOMPtr<nsIURI> baseURI;
    nsCOMPtr<nsIDocument> doc;

    result = mContent->GetDocument(*getter_AddRefs(doc));
    if (NS_FAILED(result)) {
      return result;
    }
    result = GetCSSParsingEnvironment(mContent,
                                      getter_AddRefs(baseURI),
                                      getter_AddRefs(cssLoader),
                                      getter_AddRefs(cssParser));

    if (NS_SUCCEEDED(result)) {
      nsChangeHint hint = NS_STYLE_HINT_NONE;
      if (doc) {
        doc->BeginUpdate();

        doc->AttributeWillChange(mContent, kNameSpaceID_None,
                                 nsHTMLAtoms::style);
      }
      nsCSSDeclaration* declClone = decl->Clone();

      if (aClearOldDecl) {
        hint = decl->GetStyleImpact();
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
  
      nsChangeHint newHint = NS_STYLE_HINT_NONE;
      result = cssParser->ParseAndAppendDeclaration(aDecl, baseURI, decl,
                                                    aParseOnlyOneDecl, &newHint);
      NS_UpdateHint(hint, newHint);
      
      if (result == NS_CSS_PARSER_DROP_DECLARATION) {
        SetCSSDeclaration(declClone);
        result = NS_OK;
      }
      if (doc) {
        if (NS_SUCCEEDED(result) && result != NS_CSS_PARSER_DROP_DECLARATION) {
          doc->AttributeChanged(mContent, kNameSpaceID_None,
                                nsHTMLAtoms::style,
                                nsIDOMMutationEvent::MODIFICATION, hint);
        }
        doc->EndUpdate();
      }
      if (cssLoader) {
        cssLoader->RecycleParser(cssParser);
      }
    }
  }
#else
  if (!decl) {
    return result;
  }
  nsCOMPtr<nsICSSLoader> cssLoader;
  nsCOMPtr<nsICSSParser> cssParser;
  nsCOMPtr<nsIURI> baseURI;

  result = GetCSSParsingEnvironment(mContent,
                                    getter_AddRefs(baseURI),
                                    getter_AddRefs(cssLoader),
                                    getter_AddRefs(cssParser));
  if (NS_FAILED(result)) {
    return result;
  }
// bug 125246
#if(0)
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
// end bug
  
  nsChangeHint uselessHint = NS_STYLE_HINT_NONE;
  result = cssParser->ParseAndAppendDeclaration(aDecl, baseURI, decl,
                                                aParseOnlyOneDecl,
                                                &uselessHint,
                                                aClearOldDecl); // bug 125246
  
  if (NS_SUCCEEDED(result)) {
    result = SetCSSDeclaration(decl, PR_TRUE);
  }

  if (cssLoader) {
    cssLoader->RecycleParser(cssParser);
  }
#endif
// end bug

  return result;
}

#endif
// end bug 188803

// bug 188803
#if(0)
nsresult
nsDOMCSSAttributeDeclaration::GetParent(nsISupports **aParent)
{
  NS_ENSURE_ARG_POINTER(aParent);

  *aParent = mContent;
  NS_IF_ADDREF(*aParent);

  return NS_OK;
}

#else

nsresult
nsDOMCSSAttributeDeclaration::GetParentRule(nsIDOMCSSRule **aParent)
{
  NS_ENSURE_ARG_POINTER(aParent);

  *aParent = nsnull;
  return NS_OK;
}
#endif
// end bug