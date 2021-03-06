/* -*- Mode: Java; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
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
 * Modified for Classilla 9.0.4 by Cameron Kaiser
 *
 * The Initial Developer of the Original Code is 
 * Netscape Communications Corporation.
 * Portions created by the Initial Developer are Copyright (C) 1998
 * the Initial Developer. All Rights Reserved.
 *
 * Contributor(s):
 *   Blake Ross <blakeross@telocity.com>
 *   Peter Annema <disttsc@bart.nl>
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

function nsBrowserStatusHandler()
{
  this.init();
}

nsBrowserStatusHandler.prototype =
{
// bug 104778
/* 
  userTyped :
  {
    _value : false,
    browser : null,

    get value() {
      if (this.browser != getBrowser().mCurrentBrowser)
        this._value = false;
      
      return this._value;
    },

    set value(aValue) {
      if (this._value != aValue) {
        this._value = aValue;
        this.browser = aValue ? getBrowser().mCurrentBrowser : null;
      }

      return aValue;
    }
  },
*/
  // Stored Status, Link and Loading values
  status : "",
  defaultStatus : "",
  jsStatus : "",
  jsDefaultStatus : "",
  overLink : "",
  pref : null, // added for prefwatcher.

  hideAboutBlank : true,

  QueryInterface : function(aIID)
  {
    if (aIID.equals(Components.interfaces.nsIWebProgressListener) ||
        aIID.equals(Components.interfaces.nsISupportsWeakReference) ||
        aIID.equals(Components.interfaces.nsIXULBrowserWindow) ||
        aIID.equals(Components.interfaces.nsISupports))
      return this;
    throw Components.results.NS_NOINTERFACE;
  },

  init : function()
  {
    this.urlBar          = document.getElementById("urlbar");
    this.throbberElement = document.getElementById("navigator-throbber");
    this.statusMeter     = document.getElementById("statusbar-icon");
    this.stopButton      = document.getElementById("stop-button");
    this.stopMenu        = document.getElementById("menuitem-stop");
    this.stopContext     = document.getElementById("context-stop");
    this.statusTextField = document.getElementById("statusbar-display");
    this.isImage         = document.getElementById("isImage");
    this.securityButton  = document.getElementById("security-button");

    // Initialize the security button's state and tooltip text
    const nsIWebProgressListener = Components.interfaces.nsIWebProgressListener;
    this.onSecurityChange(null, null, nsIWebProgressListener.STATE_IS_INSECURE);
    
    // Get a reference to the preferences service (mostly for the fixup renderer state)
    var prefService = Components.classes["@mozilla.org/preferences-service;1"]
                                .getService(Components.interfaces.nsIPrefService);
    pref = prefService.getBranch(null);
  },

  destroy : function()
  {
    // XXXjag to avoid leaks :-/, see bug 60729
    this.urlBar          = null;
    this.throbberElement = null;
    this.statusMeter     = null;
    this.stopButton      = null;
    this.stopMenu        = null;
    this.stopContext     = null;
    this.statusTextField = null;
    this.isImage         = null;
    this.securityButton  = null;
    // this.userTyped       = null; // bug 104778
  },

  setJSStatus : function(status)
  {
    this.jsStatus = status;
    this.updateStatusField();
  },

  setJSDefaultStatus : function(status)
  {
    this.jsDefaultStatus = status;
    this.updateStatusField();
  },

  setDefaultStatus : function(status)
  {
    this.defaultStatus = status;
    this.updateStatusField();
  },

  setOverLink : function(link)
  {
    this.overLink = link;
    // clear out 'Done' (or other message) on first hover
    if (this.defaultStatus)
      this.defaultStatus = "";
    this.updateStatusField();
    if (link)
      this.statusTextField.setAttribute('crop', 'center');
    else
      this.statusTextField.setAttribute('crop', 'end');
  },

  updateStatusField : function()
  {
    var text = this.overLink || this.status || this.jsStatus || this.jsDefaultStatus || this.defaultStatus;

    // check the current value so we don't trigger an attribute change
    // and cause needless (slow!) UI updates
    if (this.statusTextField.label != text)
      this.statusTextField.label = text;
  },

  mimeTypeIsTextBased : function(contentType)
  {
    return /^text\/|\+xml$/.test(contentType) ||
           contentType == "application/x-javascript" ||
           contentType == "application/xml" ||
           contentType == "mozilla.application/cached-xul";
  },

  onLinkIconAvailable : function(aHref) {
  	var browser = getBrowser(); // bug 104778
    if (gProxyFavIcon && pref.getBoolPref("browser.chrome.site_icons"))
    {
    	if (browser.userTypedValue === null) // pull up to 1.8
      gProxyFavIcon.setAttribute("src", aHref);

      // update any bookmarks with new icon reference
      if (!gBookmarksService)
        gBookmarksService = Components.classes["@mozilla.org/browser/bookmarks-service;1"]
                                      .getService(Components.interfaces.nsIBookmarksService);
      //gBookmarksService.updateBookmarkIcon(this.urlBar.value, aHref);
      gBookmarksService.updateBookmarkIcon(browser.currentURI.spec, aHref); // bug 104778
    }
  },

  onProgressChange : function (aWebProgress, aRequest,
                               aCurSelfProgress, aMaxSelfProgress,
                               aCurTotalProgress, aMaxTotalProgress)
  {
    if (aMaxTotalProgress > 0) {
      // This is highly optimized.  Don't touch this code unless
      // you are intimately familiar with the cost of setting
      // attrs on XUL elements. -- hyatt
      var percentage = (aCurTotalProgress * 100) / aMaxTotalProgress;
      this.statusMeter.value = percentage;
    } 
  },

  onStateChange : function(aWebProgress, aRequest, aStateFlags, aStatus)
  {  
    const nsIWebProgressListener = Components.interfaces.nsIWebProgressListener;
    const nsIChannel = Components.interfaces.nsIChannel;
    var ctype;
    if (aStateFlags & nsIWebProgressListener.STATE_START) {
      // This (thanks to the filter) is a network start or the first
      // stray request (the first request outside of the document load),
      // initialize the throbber and his friends.

      // Call start document load listeners (only if this is a network load)
      if (aStateFlags & nsIWebProgressListener.STATE_IS_NETWORK &&
          aRequest && aWebProgress.DOMWindow == content)
        this.startDocumentLoad(aRequest);

      // Turn the throbber on.
      this.throbberElement.setAttribute("busy", "true");

      // XXX: These need to be based on window activity...
      this.stopButton.disabled = false;
      this.stopMenu.removeAttribute('disabled');
      this.stopContext.removeAttribute('disabled');
    }
    else if (aStateFlags & nsIWebProgressListener.STATE_STOP) {
      if (aStateFlags & nsIWebProgressListener.STATE_IS_NETWORK) {
        if (aRequest) {
          if (aWebProgress.DOMWindow == content)
            this.endDocumentLoad(aRequest, aStatus);
        }
      }

      // This (thanks to the filter) is a network stop or the last
      // request stop outside of loading the document, stop throbbers
      // and progress bars and such
      if (aRequest) {
        var msg = "";
        // Get the channel if the request is a channel
        var channel;
        try {
          channel = aRequest.QueryInterface(nsIChannel);
        }
        catch(e) { };

        if (channel) {
          var location = channel.URI.spec;
          if (location != "about:blank") {
            const kErrorBindingAborted = 0x804B0002; // 2152398850;
            const kErrorNetTimeout = 0x804B000E; // 2152398862;
            switch (aStatus) {
              case kErrorBindingAborted:
                msg = gNavigatorBundle.getString("nv_stopped");
                break;
              case kErrorNetTimeout:
                msg = gNavigatorBundle.getString("nv_timeout");
                break;
            }
          }
        }
        // If msg is false then we did not have an error (channel may have
        // been null, in the case of a stray image load).
        if (!msg) {
          msg = gNavigatorBundle.getString("nv_done");
          // figure out if Classilla rendered with fixups, as a reminder to the user.
          // we don't care if this fails, but we don't want failure to wreck the browser.
          // DISABLED IN 9.1
          //try {
          //	var isfixup = pref.getBoolPref("classilla.layout.fixup") ? "nv_done_didfix" :
          //				"nv_done_didntfix";
          //	msg += " " + gNavigatorBundle.getString(isfixup);
          //} catch(e) {
          //	// damn
          //}
        }
        this.status = "";
        this.setDefaultStatus(msg);
        
        // Disable menu entries for images, enable otherwise
        if (content.document && this.mimeTypeIsTextBased(content.document.contentType))
          this.isImage.removeAttribute('disabled');
        else
          this.isImage.setAttribute('disabled', 'true');
      }

      // Turn the progress meter and throbber off.
      this.statusMeter.value = 0;  // be sure to clear the progress bar
      this.throbberElement.removeAttribute("busy");

      // XXX: These need to be based on window activity...
      // XXXjag: <command id="cmd_stop"/> ?
      this.stopButton.disabled = true;
      this.stopMenu.setAttribute('disabled', 'true');
      this.stopContext.setAttribute('disabled', 'true');
    }
  },

  onLocationChange : function(aWebProgress, aRequest, aLocation)
  {
    this.setOverLink("");

    var locationURI = null;
    var location = "";    

    if (aLocation) {
      try {
        if (!gURIFixup)
          gURIFixup = Components.classes["@mozilla.org/docshell/urifixup;1"]
                                .getService(Components.interfaces.nsIURIFixup);
        // If the url has "wyciwyg://" as the protocol, strip it off.
        // Nobody wants to see it on the urlbar for dynamically generated
        // pages.
        locationURI = gURIFixup.createExposableURI(aLocation);
        location = locationURI.spec;
        
      }
      catch(ex) {
        location = aLocation.spec;
      }
    }

    if (this.hideAboutBlank) {
      this.hideAboutBlank = false;
      if (location == "about:blank")
        location = "";
    }

    // Disable menu entries for images, enable otherwise
    if (content.document && this.mimeTypeIsTextBased(content.document.contentType))
      this.isImage.removeAttribute('disabled');
    else
      this.isImage.setAttribute('disabled', 'true');

    // We should probably not do this if the value has changed since the user
    // searched
    // Update urlbar only if a new page was loaded on the primary content area
    // Do not update urlbar if there was a subframe navigation

    var browser = getBrowser().selectedBrowser; // bug 104778
    if (aWebProgress.DOMWindow == content) {
    // bug 104778
    /*
      if (!this.userTyped.value) {
        this.urlBar.value = location;
        // the above causes userTyped.value to become true, reset it
        this.userTyped.value = false;
      }

      SetPageProxyState("valid", aLocation);
    */
      
      // pull up from 1.7
      // The document loaded correctly, clear the value if we should
      if (browser.userTypedClear > 0)
        browser.userTypedValue = null;
        
      var userTypedValue = browser.userTypedValue;
        
      if (userTypedValue === null) {
    	this.urlBar.value = location;
    	if (this.urlBar.value != location) { // weird! but see bug 249322
    	  this.urlBar.value = ""; // hack for bug 249322
    	  this.urlBar.value = location;
    	} // end bug
    	SetPageProxyState("valid", aLocation);
    	// Setting the urlBar value in some cases causes userTypedValue to
    	// become set because of oninput, so reset it to its old value
    	browser.userTypedValue = null;
      } else {
    	this.urlBar.value = userTypedValue;
    	SetPageProxyState("invalid", null);
      }
    // end bug
    }
    UpdateBackForwardButtons();

    var blank = (location == "about:blank") || (location == "");
    //var browser = getBrowser().mCurrentBrowser; // bug 104778

    //clear popupDomain accordingly so that icon will go away when visiting
    //an unblocked site after a blocked site. note: if a popup is blocked 
    //the icon will stay as long as we are in the same domain.    

    if (blank ||
        !("popupDomain" in browser)) {
      browser.popupDomain = null;
    }
    else {
      var hostPort = "";
      try {
        hostPort = locationURI.hostPort;
      }
      catch(ex) { }
      if (hostPort != browser.popupDomain)
        browser.popupDomain = null;
    }

    var popupIcon = document.getElementById("popupIcon");
    popupIcon.hidden = !browser.popupDomain;
  },

  onStatusChange : function(aWebProgress, aRequest, aStatus, aMessage)
  {
    this.status = aMessage;
    this.updateStatusField();
  },

  onSecurityChange : function(aWebProgress, aRequest, aState)
  {
    const wpl = Components.interfaces.nsIWebProgressListener;

    switch (aState) {
      case wpl.STATE_IS_SECURE | wpl.STATE_SECURE_HIGH:
        this.securityButton.setAttribute("level", "high");
        break;
      case wpl.STATE_IS_SECURE | wpl.STATE_SECURE_LOW:
        this.securityButton.setAttribute("level", "low");
        break;
      case wpl.STATE_IS_BROKEN:
        this.securityButton.setAttribute("level", "broken");
        break;
      case wpl.STATE_IS_INSECURE:
      default:
        this.securityButton.removeAttribute("level");
        break;
    }

    var securityUI = getBrowser().securityUI;
    if (securityUI)
      this.securityButton.setAttribute("tooltiptext", securityUI.tooltipText);
    else
      this.securityButton.removeAttribute("tooltiptext");
  },

  startDocumentLoad : function(aRequest)
  {
    // Reset so we can see if the user typed after^Wbetween (bug 104778) the document load
    // starting and the location changing.
    //this.userTyped.value = false;
    //getBrowser().userTypedValue = null;
    getBrowser().userTypedClear++; // pull up from 1.8.1

    const nsIChannel = Components.interfaces.nsIChannel;
    var urlStr = aRequest.QueryInterface(nsIChannel).URI.spec;
    var observerService = Components.classes["@mozilla.org/observer-service;1"]
                                    .getService(Components.interfaces.nsIObserverService);
    try {
      observerService.notifyObservers(_content, "StartDocumentLoad", urlStr);
    } catch (e) {
    }
  },

  endDocumentLoad : function(aRequest, aStatus)
  {
    // pull up from 1.8.1
    // The document is done loading, it's okay to clear
    // the value again.
    if (getBrowser().userTypedClear > 0)
      getBrowser().userTypedClear--;
      
    const nsIChannel = Components.interfaces.nsIChannel;
    var urlStr = aRequest.QueryInterface(nsIChannel).originalURI.spec;

    if (Components.isSuccessCode(aStatus))
      dump("Document "+urlStr+" loaded successfully\n"); // per QA request
    else {
      // per QA request
      var e = new Components.Exception("", aStatus);
      var name = e.name;
      dump("Error loading URL "+urlStr+" : "+
           Number(aStatus).toString(16));
      if (name)
           dump(" ("+name+")");
      dump('\n'); 
    }

    var observerService = Components.classes["@mozilla.org/observer-service;1"]
                                    .getService(Components.interfaces.nsIObserverService);

    var notification = Components.isSuccessCode(aStatus) ? "EndDocumentLoad" : "FailDocumentLoad";
    try {
      observerService.notifyObservers(_content, notification, urlStr);
    } catch (e) {
    }
  }
}

