/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */
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

#include "nsImgManager.h"
#include "nsImages.h"
#include "nsIDocument.h"
#include "nsIContent.h"
#include "nsCOMPtr.h"
#include "nsXPIDLString.h"
#include "nsIURI.h"
#include "nsNetCID.h"
#include "nsIServiceManager.h"
#include "nsIScriptGlobalObject.h"
#include "nsIDOMWindow.h"
#include "nsIDocShellTreeItem.h"
#include "nsINodeInfo.h"

static NS_DEFINE_CID(kIOServiceCID, NS_IOSERVICE_CID);


////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// nsImgManager Implementation

NS_IMPL_ISUPPORTS2(nsImgManager, 
                   nsIImgManager, 
                   nsIContentPolicy);

nsImgManager::nsImgManager()
{
}

nsImgManager::~nsImgManager(void)
{
}

nsresult nsImgManager::Init()
{
    IMAGE_RegisterPrefCallbacks();
    nsresult rv;
    mIOService = do_GetService(NS_IOSERVICE_CONTRACTID, &rv);
    return rv;
}


NS_IMETHODIMP nsImgManager::Block(nsIURI* imageURI)
{
    ::IMAGE_Block(imageURI);
    return NS_OK;
}


// nsIContentPolicy Implementation
NS_IMETHODIMP nsImgManager::ShouldLoad(PRInt32 aContentType, 
                                       nsIURI *aContentLoc,
                                       nsISupports *aContext,
                                       nsIDOMWindow *aWindow,
                                       PRBool *_retval)
{
    *_retval = PR_TRUE;
    nsresult rv = NS_OK;

    // we can't do anything w/ out these.
    if (!aContentLoc || !aContext) return rv;

    switch (aContentType) {
        case nsIContentPolicy::IMAGE:   
        {
            // First, let be sure we are processing an HTTP or HTTPS images.
            // We should not waste time with chrome url...
		    // we only want to check http, https, ftp
    		PRBool isFtp;
    		rv = aContentLoc->SchemeIs("ftp", &isFtp);
			NS_ENSURE_SUCCESS(rv,rv);
		    PRBool needToCheck = isFtp;
    		if (!needToCheck) {
      			rv = aContentLoc->SchemeIs("http", &needToCheck);
      			NS_ENSURE_SUCCESS(rv,rv);
			    if (!needToCheck) {
        			rv = aContentLoc->SchemeIs("https", &needToCheck);
    				NS_ENSURE_SUCCESS(rv,rv);
      			}
    		}

		    // for chrome:// and resources and others, no need to check.
		    if (!needToCheck)
       			return NS_OK;


// bug 51631 and 203629 disables this
#if(0)
            PRBool httpType;
            rv = aContentLoc->SchemeIs("http", &httpType);
            if (NS_FAILED(rv) || !httpType) {
                // check HTTPS as well
                rv = aContentLoc->SchemeIs("https", &httpType);
                if (NS_FAILED(rv) || !httpType) return rv;
            }
#endif

            nsCOMPtr<nsIURI> baseURI;
            nsCOMPtr<nsIDocument> doc;
            nsCOMPtr<nsINodeInfo> nodeinfo; // needed for 211634 below
            nsCOMPtr<nsIContent> content(do_QueryInterface(aContext));
            NS_ASSERTION(content, "no content available");
            if (content) {
            	// XXXbz GetOwnerDocument (bug 211634).
                rv = content->GetDocument(*getter_AddRefs(doc));
// bug 211634 plus backbugs modified for Classilla
#if(0)
                if (NS_FAILED(rv) || !doc) return rv;
#else
      if (NS_FAILED(rv) || !doc) {
        rv = content->GetNodeInfo(*getter_AddRefs(nodeinfo)); // Clecko needs the *
        if (NS_FAILED(rv) || !nodeinfo) return rv;
 
        doc = nodeinfo->GetDocument();
        // XXX what should this code do if there is really no document?
        if (!doc) return NS_OK;
      }
#endif
// end bug
                rv = doc->GetBaseURL(*getter_AddRefs(baseURI));
                if (NS_FAILED(rv) || !baseURI) return rv;

// bug 51631 and 203629
#if(0)
                // Let check if we are running a mail window, doesn't matter if mail images are allowed
                if (IMAGE_BlockedInMail()) {
                    nsCOMPtr<nsIDocShell> docshell;
                    rv = GetRootDocShell(aWindow, getter_AddRefs(docshell));
                    if (docshell) {
                        PRUint32 appType;
                        rv = docshell->GetAppType(&appType);
                        if (NS_SUCCEEDED(rv) && appType == nsIDocShell::APP_TYPE_MAIL) {
                            //we are dealing with an mail or newsgroup window, let's block the image
                            *_retval = PR_FALSE;
                            return NS_OK;
                        }
                    }
                }
#else
				nsCOMPtr<nsIDocShell> docshell;
       			rv = GetRootDocShell(aWindow, getter_AddRefs(docshell));
       			if (docshell) {
         			PRUint32 appType;
         			rv = docshell->GetAppType(&appType);
         			if (NS_SUCCEEDED(rv) && appType == nsIDocShell::APP_TYPE_MAIL) {
           				// never allow ftp for mail messages, 
           				// because we don't want to send the users email address
           				// as the anonymous password
           				//if (mBlockInMailNewsPref || isFtp) {
           				if (isFtp) { // if (mBlockInMailNewsPref || isFtp) {
                            //we are dealing with an mail or newsgroup window, let's block the image
                            *_retval = PR_FALSE;
                            return NS_OK;
                        }
                    }
                }
#endif

                nsCAutoString baseHost;
                rv = baseURI->GetAsciiHost(baseHost);
                if (NS_FAILED(rv)) return rv;

                nsCAutoString host;
                rv = aContentLoc->GetAsciiHost(host);
                if (NS_FAILED(rv)) return rv;

                return ::IMAGE_CheckForPermission(host.get(), baseHost.get(),
                                                  _retval);
            }
        }
        break;
    }
    return NS_OK;
}

NS_IMETHODIMP nsImgManager::ShouldProcess(PRInt32 aContentType,
                                          nsIURI *aDocumentLoc,
                                          nsISupports *aContext,
                                          nsIDOMWindow *aWindow,
                                          PRBool *_retval) {
  return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP nsImgManager::GetRootDocShell(nsIDOMWindow *aWindow, nsIDocShell **result)
{
  nsresult rv;

  nsCOMPtr<nsIScriptGlobalObject> globalObj(do_QueryInterface(aWindow));
  if (!globalObj)
    return NS_ERROR_FAILURE;

  nsCOMPtr<nsIDocShell> docshell;
  rv = globalObj->GetDocShell(getter_AddRefs(docshell));
  if (NS_FAILED(rv))
    return rv;

  nsCOMPtr<nsIDocShellTreeItem> docshellTreeItem(do_QueryInterface(docshell, &rv));
  if (NS_FAILED(rv))
    return rv;

  nsCOMPtr<nsIDocShellTreeItem> rootItem;
  rv = docshellTreeItem->GetRootTreeItem(getter_AddRefs(rootItem));
  if (NS_FAILED(rv))
    return rv;

  return CallQueryInterface(rootItem, result);
}
