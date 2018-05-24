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
 *
 * The Initial Developer of the Original Code is 
 * Netscape Communications Corporation.
 * Portions created by the Initial Developer are Copyright (C) 1998
 * the Initial Developer. All Rights Reserved.
 *
 * Contributor(s):
 *   Blake Ross <blakeross@telocity.com>
 *   Peter Annema <disttsc@bart.nl>
 *   Samir Gehani <sgehani@netscape.com>
 *
 * Modified for Classilla by Cameron Kaiser
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

/* startDocumentLoad, etc., and everything else in nsBrowserStatusHandler.js
   is, well, in that file. -- Cameron */
   
const nsIWebNavigation = Components.interfaces.nsIWebNavigation;
var gPrintSettingsAreGlobal = true;
var gSavePrintSettings = true;
var gChromeState = null; // chrome state before we went into print preview
var gOldCloseHandler = null; // close handler before we went into print preview
var gInPrintPreviewMode = false;
var gWebProgress        = null;

var Cc = Components.classes;
var Ci = Components.interfaces;
var prefService = Cc["@mozilla.org/preferences-service;1"].getService(Ci.nsIPrefService);

/*
 *
 * Byblos stelae controller
 * Classilla issue 170
 *
 */
 
var observerService = Cc["@mozilla.org/observer-service;1"].getService(Ci.nsIObserverService);
    
// The tracer is responsible for aggregating events from the HTTP traced channel
// and consuming data, handing it to the designated stele for translation.
//
// These functions are designed to be absolutely bulletproof because if they don't
// work, the browser don't work.

function ByblosTracer() { }

ByblosTracer.prototype = {
	originalListener : null, // observer sets this
	receivedData : null,
	buffer : null,
	stele : null, // observer sets this
	console : null, // observer sets this
	
	onStartRequest : function(request, context) {
		//this.console.logStringMessage("tracer onStartRequest");
		this.receivedData = [];
		this.buffer = [];
		
		this.originalListener.onStartRequest(request, context);
	},
	onDataAvailable : function(request, context, stream, offset, count) {
		var Cc = Components.classes;
		var Ci = Components.interfaces;

		//this.console.logStringMessage("tracer onDataAvailable");

       	var binaryInputStream = Cc["@mozilla.org/binaryinputstream;1"]
                                .createInstance(Ci.nsIBinaryInputStream);
        binaryInputStream.setInputStream(stream);

        // Copy received data as they come.
        var data = binaryInputStream.readBytes(count);
        this.receivedData.push(data);

		// Defer! DEFER! But don't delete! Or something. (This is here for debugging)
		//this.originalListener.onDataAvailable(request, context, stream, offset, count);
	},
	onStopRequest : function(request, context, status) {
		var i, j;
		var did_rewrite = true;
		var Cc = Components.classes;
		var Ci = Components.interfaces;

		//this.console.logStringMessage("tracer onStopRequest");
		
		// Hand the coalesced data to the stele.
		var fump = this.receivedData.join('');
		var lump = fump;
		try {
			var respobj = this.stele.parseHTML(fump);
			lump = (respobj.response == "ok") ? respobj.body : fump;
		} catch(e) {
			this.console.logStringMessage("Byblos failure: stele failed parsing the page: "+e);
			did_rewrite = false;
		}
		
		// Break up the lump into 4K gobs to simulate a network read using a storage stream.
		var storageStream = Cc["@mozilla.org/storagestream;1"]
							.createInstance(Ci.nsIStorageStream);
		storageStream.init(4096, lump.length, null);
		var binaryOutputStream = Cc["@mozilla.org/binaryoutputstream;1"]
								 .createInstance(Ci.nsIBinaryOutputStream);

		binaryOutputStream.setOutputStream(storageStream.getOutputStream(0));
		binaryOutputStream.writeBytes(lump, lump.length);
		j = lump.length;
		for (i = 0; i < lump.length; i += 4096) {
			var c = (j < 4096) ? j : 4096;
			//this.console.logStringMessage("simulating onDataAvailable "+i+" "+j+" "+c);
			this.originalListener.onDataAvailable(request, context, storageStream.newInputStream(i), 0, c);
			j -= 4096;
			if (j < 1) break;
		}		
		
		// Finally, push the stop request.
		if (did_rewrite)
			this.console.logStringMessage("Byblos success: successfully rewrote page");
		this.originalListener.onStopRequest(request, context, status);
	},
	QueryInterface : function(aIID) {
		var Cc = Components.classes;
		var Ci = Components.interfaces;

		if (aIID.equals(Ci.nsIStreamListener) || aIID.equals(Ci.nsISupports))
			return this;
		throw Components.results.NS_NOINTERFACE;
	}
};

// The request observer is responsible for watching for URIs it can intercept and
// determining if a stele exists for translation.

var httpRequestObserver = { // watches for URIs it can intercept
	observe : function(subject, topic, data) {
		if (topic == "http-on-examine-response" && subject) {
			var Cc = null;
			var Ci = null;
			try {
				Cc = Components.classes;
				Ci = Components.interfaces;
			} catch(e) { }
			if (!Cc) return; // wtf?!

			var consoleService = Cc["@mozilla.org/consoleservice;1"]
                               	.getService(Ci.nsIConsoleService);
            if (!consoleService)
            	consoleService = { logStringMessage : function(w) { } };
                               	
            // The object we get from nsHttpHandler is a raw nsISupports, so now we need to
            // QI it. We start with an nsIChannel to get the URI and MIME type.
			subject.QueryInterface(Ci.nsIChannel);
			if (!subject.URI)
				return; // wtf?
			var uri = subject.URI;
			var host = subject.URI.asciiHost;
			            
            // Next, QI to nsIHttpChannel to get the request information.
			subject.QueryInterface(Ci.nsIHttpChannel);
            if (!subject.getResponseHeader)
            	return; // wtf?

            // Examine the response headers. If the content type is not text/html, ignore it also.
            var mime = "???";
            try {
            	mime = subject.getResponseHeader("Content-Type");
            } catch(e) { /* Hmm. */ }
			// Remove the character set, if there is one.
			mime = mime.replace(/; charset=.+$/i, "");
			
			//consoleService.logStringMessage("HTTP observer fired "+subject+" "+mime+" "+host);

            if (mime != "text/html" && mime != "application/xhtml+xml")
            	return;
			
			// Is this a site that has a stele available for translation? Let's try loading one.
			var str = "";
			var srcUrl = "resource://programdir/Byblos/"+host+".js";
			try {
				var ioService=Cc["@mozilla.org/network/io-service;1"]
    							.getService(Ci.nsIIOService);
  				var scriptableStream=Cc["@mozilla.org/scriptableinputstream;1"]
    							.getService(Ci.nsIScriptableInputStream);
  				var channel=ioService.newChannel(srcUrl,null,null);
  				var input=channel.open();
  				scriptableStream.init(input);
  				str=scriptableStream.read(input.available());
  				scriptableStream.close();
  				input.close();
  			} catch(e) { }

			// Couldn't load from the Byblos directory, so quietly fail.
			if (!str)
				return;
				
			// Try to interpret the JS.
			var stele = null;
			try {
				var stelef = null;
				eval("var stelef = "+str+";");
				stele = stelef();
			} catch(e) { consoleService.logStringMessage("Byblos failure parsing stele for "+host+": "+e); }
			if (!stele)
				return;
			
			consoleService.logStringMessage("Byblos info: got stele for "+host);
			// See if the stele wants this URL.
			if (!stele.wantURI(uri))
				// Guess not.
				return;

			// Yes, start the Byblos tracer. This is delicate -- if the QI fails on setNewListener,
			// we must undo this immediately.
			var tracer = new ByblosTracer();
			tracer.console = consoleService;
			tracer.stele = stele;
			// Finally QI ourselves to nsITraceableChannel. Do NOT catch failure here; it should propagate.
			subject.QueryInterface(Ci.nsITraceableChannel);
			
			try {
				// See if we can QI the tracer to the desired object. XHR will fail this, so
				// we need to fail in a recoverable way (unlike the original nsITraceableChannel).
				tracer.originalListener = subject.getCurrentListener();
			} catch(e) { }
			if (tracer.originalListener == null)
				// This will not work. Abort now.
				return;
			// We do NOT want to catch this error; it should be propagated if this fails.
			subject.setNewListener(tracer);
		}
	},
	QueryInterface : function (aIID) {
		var Cc = Components.classes;
		var Ci = Components.interfaces;

        if (aIID.equals(Ci.nsIObserver) || aIID.equals(Ci.nsISupports))
        	return this;
        throw Components.results.NS_NOINTERFACE;
    }
};

// Enable the observer.
observerService.addObserver(httpRequestObserver, "http-on-examine-response", false);

/*
 *
 * Site Control
 * This controls aspects of how the request is made, such as dynamic User-Agent control, etc.
 * Classilla issue 169
 *
 */

var httpModifyObserver = { // watches for requests
	observe : function(subject, topic, data) {
		if (topic == "http-on-modify-request" && subject) {
			var Cc = null;
			var Ci = null;
			try {
				Cc = Components.classes;
				Ci = Components.interfaces;
			} catch(e) { }
			if (!Cc) return; // wtf?!
			
			// Get our prefs branch handle.
			var pb = Cc["@mozilla.org/preferences-service;1"].getService(Ci.nsIPrefService).getBranch("classilla.sitecontrol.");

			var consoleService = Cc["@mozilla.org/consoleservice;1"]
                               	.getService(Ci.nsIConsoleService);
            if (!consoleService)
            	consoleService = { logStringMessage : function(w) { } };
            	
			// QI to a channel and get the URI.
			try {
				subject.QueryInterface(Ci.nsIChannel);
			} catch(e) {
				// We're dead.
				consoleService.logStringMessage("Site Control observer failure on "+subject+": "+e);
			}
			
			if (!subject.URI)
				return; // abort now
			var spec = subject.URI.spec;
			var host = subject.URI.host.toLowerCase();
			var althost = "Classilla/CFM";
			
			// Check for hardcoded URLs and for URLs the user has specified. Let the user override.
			var q = '';
			try {
				var q = pb.getCharPref(host);
			} catch(e) { } // if this fails, it's no big deal.
			if (q && q.length) {
				if (q.length > 1)
					althost = q;
			} else if (!/^https?:\/\/(www\.|mail\.|accounts\.)?google\.com($|\/)/.test(spec))
				return;
			
			// Now QI to an nsIHttpChannel so we can modify the request.
			try {
				subject.QueryInterface(Ci.nsIHttpChannel);
			} catch(e) {
				// We're so dead.
				consoleService.logStringMessage("Site Control observer failure on "+subject+": "+e);
			}
			
			if (!subject.setRequestHeader)
				return; // abort now
				
			// Forge the user-agent and continue.
			subject.setRequestHeader("User-Agent", althost, false);
			//consoleService.logStringMessage("Silent user-agent change for "+spec);
		}
	},
	QueryInterface : function (aIID) {
        if (aIID.equals(Ci.nsIObserver) || aIID.equals(Ci.nsISupports))
        	return this;
        throw Components.results.NS_NOINTERFACE;
    }	
};

// Enable the observer.
observerService.addObserver(httpModifyObserver, "http-on-modify-request", false);

/*
 *
 * Back to the browser, which is already in progress.
 *
 */

function getWebNavigation()
{
  try {
    return getBrowser().webNavigation;
  } catch (e) {
    return null;
  }
}

function BrowserReloadWithFlags(reloadFlags)
{
  /* First, we'll try to use the session history object to reload so 
   * that framesets are handled properly. If we're in a special 
   * window (such as view-source) that has no session history, fall 
   * back on using the web navigation's reload method.
   */

  var webNav = getWebNavigation();
  try {
    var sh = webNav.sessionHistory;
    if (sh)
      webNav = sh.QueryInterface(Components.interfaces.nsIWebNavigation);
  } catch (e) {
  }

  try {
    webNav.reload(reloadFlags);
  } catch (e) {
  }
}

function toggleAffectedChrome(aHide)
{
  // chrome to toggle includes:
  //   (*) menubar
  //   (*) navigation bar
  //   (*) personal toolbar
  //   (*) tab browser ``strip''
  //   (*) sidebar

  if (!gChromeState)
    gChromeState = new Object;
  var navToolbox = document.getElementById("navigator-toolbox");
  navToolbox.hidden = aHide;
  var theTabbrowser = document.getElementById("content"); 

  // sidebar states map as follows:
  //   was-hidden    => hide/show nothing
  //   was-collapsed => hide/show only the splitter
  //   was-shown     => hide/show the splitter and the box
  if (aHide)
  {
    // going into print preview mode
    if (sidebar_is_collapsed())
    {
      gChromeState.sidebar = "was-collapsed";
    }
    else if (sidebar_is_hidden())
      gChromeState.sidebar = "was-hidden";
    else 
    {
      gChromeState.sidebar = "was-visible";
    }
    document.getElementById("sidebar-box").hidden = true;
    document.getElementById("sidebar-splitter").hidden = true;
    //deal with tab browser
    gChromeState.hadTabStrip = theTabbrowser.getStripVisibility();
    theTabbrowser.setStripVisibilityTo(false);
  }
  else
  {
    // restoring normal mode (i.e., leaving print preview mode)
    //restore tab browser
    theTabbrowser.setStripVisibilityTo(gChromeState.hadTabStrip);
    if (gChromeState.sidebar == "was-collapsed" ||
        gChromeState.sidebar == "was-visible")
      document.getElementById("sidebar-splitter").hidden = false;
    if (gChromeState.sidebar == "was-visible")
      document.getElementById("sidebar-box").hidden = false;
  }

  // if we are unhiding and sidebar used to be there rebuild it
  if (!aHide && gChromeState.sidebar == "was-visible")
    SidebarRebuild();
}

function showPrintPreviewToolbar()
{
  toggleAffectedChrome(true);
  const kXULNS = 
    "http://www.mozilla.org/keymaster/gatekeeper/there.is.only.xul";

  var printPreviewTB = document.createElementNS(kXULNS, "toolbar");
  printPreviewTB.setAttribute("printpreview", true);
  printPreviewTB.setAttribute("id", "print-preview-toolbar");

  var navToolbox = document.getElementById("navigator-toolbox");
  navToolbox.parentNode.insertBefore(printPreviewTB, navToolbox);
}

function BrowserExitPrintPreview()
{
  gInPrintPreviewMode = false;

  var browser = getBrowser();
  browser.setAttribute("handleCtrlPageUpDown", "true");

  // remove the print preview toolbar
  var navToolbox = document.getElementById("navigator-toolbox");
  var printPreviewTB = document.getElementById("print-preview-toolbar");
  navToolbox.parentNode.removeChild(printPreviewTB);

  // restore chrome to original state
  toggleAffectedChrome(false);

  // restore old onclose handler if we found one before previewing
  var mainWin = document.getElementById("main-window");
  mainWin.setAttribute("onclose", gOldCloseHandler);

  // exit print preview galley mode in content area
  var ifreq = _content.QueryInterface(
    Components.interfaces.nsIInterfaceRequestor);
  var webBrowserPrint = ifreq.getInterface(
    Components.interfaces.nsIWebBrowserPrint);     
  webBrowserPrint.exitPrintPreview(); 
  _content.focus();
}

// This observer is called once the progress dialog has been "opened"
var gPrintPreviewObs = {
    observe: function(aSubject, aTopic, aData)
    {
      setTimeout(FinishPrintPreview, 0);
    },

    QueryInterface : function(iid)
    {
     if (iid.equals(Components.interfaces.nsIObserver) || iid.equals(Components.interfaces.nsISupportsWeakReference))
      return this;
     
     throw Components.results.NS_NOINTERFACE;
    }
};

function BrowserPrintPreview()
{
  var ifreq;
  var webBrowserPrint;  
  try {
    ifreq = _content.QueryInterface(Components.interfaces.nsIInterfaceRequestor);
    webBrowserPrint = ifreq.getInterface(Components.interfaces.nsIWebBrowserPrint);     
    gPrintSettings = GetPrintSettings();

  } catch (e) {
    // Pressing cancel is expressed as an NS_ERROR_ABORT return value,
    // causing an exception to be thrown which we catch here.
    // Unfortunately this will also consume helpful failures, so add a
    // dump(e); // if you need to debug
  }

  // Here we get the PrintingPromptService tso we can display the PP Progress from script
  // For the browser implemented via XUL with the PP toolbar we cannot let it be
  // automatically opened from the print engine because the XUL scrollbars in the PP window
  // will layout before the content window and a crash will occur.
  //
  // Doing it all from script, means it lays out before hand and we can let printing do it's own thing
  gWebProgress = new Object();

  var printPreviewParams    = new Object();
  var notifyOnOpen          = new Object();
  var printingPromptService = Components.classes["@mozilla.org/embedcomp/printingprompt-service;1"]
                                  .getService(Components.interfaces.nsIPrintingPromptService);
  if (printingPromptService) {
    // just in case we are already printing, 
    // an error code could be returned if the Prgress Dialog is already displayed
    try {
      printingPromptService.showProgress(this, webBrowserPrint, gPrintSettings, gPrintPreviewObs, false, gWebProgress, 
                                         printPreviewParams, notifyOnOpen);
      if (printPreviewParams.value) {
        var webNav = getWebNavigation();
        printPreviewParams.value.docTitle = webNav.document.title;
        printPreviewParams.value.docURL   = webNav.currentURI.spec;
      }

      // this tells us whether we should continue on with PP or 
      // wait for the callback via the observer
      if (!notifyOnOpen.value.valueOf() || gWebProgress.value == null) {
        FinishPrintPreview();
      }
    } catch (e) {
      FinishPrintPreview();
    }
  }
}

function FinishPrintPreview()
{
  gInPrintPreviewMode = true;

  var browser = getBrowser();
  try {
    var ifreq = _content.QueryInterface(Components.interfaces.nsIInterfaceRequestor);
    var webBrowserPrint = ifreq.getInterface(Components.interfaces.nsIWebBrowserPrint);     
    if (webBrowserPrint) {
      gPrintSettings = GetPrintSettings();
      webBrowserPrint.printPreview(gPrintSettings, null, gWebProgress.value);
    }

    browser.setAttribute("handleCtrlPageUpDown", "false");

    var mainWin = document.getElementById("main-window");

    // save previous close handler to restoreon exiting print preview mode
    if (mainWin.hasAttribute("onclose"))
      gOldCloseHandler = mainWin.getAttribute("onclose");
    else
      gOldCloseHandler = null;
    mainWin.setAttribute("onclose", "BrowserExitPrintPreview(); return false;");
 
    // show the toolbar after we go into print preview mode so
    // that we can initialize the toolbar with total num pages
    showPrintPreviewToolbar();

    _content.focus();
  } catch (e) {
    // Pressing cancel is expressed as an NS_ERROR_ABORT return value,
    // causing an exception to be thrown which we catch here.
    // Unfortunately this will also consume helpful failures, so add a
    // dump(e); // if you need to debug
  }
}


function BrowserSetDefaultCharacterSet(aCharset)
{
  // no longer needed; set when setting Force; see bug 79608
}

function BrowserSetForcedCharacterSet(aCharset)
{
  var docCharset = getBrowser().docShell.QueryInterface(
                            Components.interfaces.nsIDocCharset);
  docCharset.charset = aCharset;
  BrowserReloadWithFlags(nsIWebNavigation.LOAD_FLAGS_CHARSET_CHANGE);
}

function BrowserSetForcedDetector(doReload)
{
  getBrowser().documentCharsetInfo.forcedDetector = true;
  if (doReload)
    BrowserReloadWithFlags(nsIWebNavigation.LOAD_FLAGS_CHARSET_CHANGE);
}

var gFindInstData;
function getFindInstData()
{
  if (!gFindInstData) {
    gFindInstData = new nsFindInstData();
    gFindInstData.browser = getBrowser();
    // defaults for rootSearchWindow and currentSearchWindow are fine here
  }
  return gFindInstData;
}

function BrowserFind()
{
  findInPage(getFindInstData());
}

function BrowserFindAgain(reverse)
{
  findAgainInPage(getFindInstData(), reverse);
}

function BrowserCanFindAgain()
{
  return canFindAgainInPage();
}

function getMarkupDocumentViewer()
{
  return getBrowser().markupDocumentViewer;
}

/**
 * Content area tooltip.
 * XXX - this must move into XBL binding/equiv! Do not want to pollute
 *       navigator.js with functionality that can be encapsulated into
 *       browser widget. TEMPORARY!
 *
 * NOTE: Any changes to this routine need to be mirrored in ChromeListener::FindTitleText()
 *       (located in mozilla/embedding/browser/webBrowser/nsDocShellTreeOwner.cpp)
 *       which performs the same function, but for embedded clients that
 *       don't use a XUL/JS layer. It is important that the logic of
 *       these two routines be kept more or less in sync.
 *       (pinkerton)
 **/
function FillInHTMLTooltip(tipElement)
{
  var retVal = false;
  if (tipElement.namespaceURI == "http://www.mozilla.org/keymaster/gatekeeper/there.is.only.xul")
    return retVal;

  const XLinkNS = "http://www.w3.org/1999/xlink";


  var titleText = null;
  var XLinkTitleText = null;
  
  while (!titleText && !XLinkTitleText && tipElement) {
    if (tipElement.nodeType == Node.ELEMENT_NODE) {
      titleText = tipElement.getAttribute("title");
      XLinkTitleText = tipElement.getAttributeNS(XLinkNS, "title");
    }
    tipElement = tipElement.parentNode;
  }

  var texts = [titleText, XLinkTitleText];
  var tipNode = document.getElementById("aHTMLTooltip");

  for (var i = 0; i < texts.length; ++i) {
    var t = texts[i];
    if (t && t.search(/\S/) >= 0) {
      tipNode.setAttribute("label", t);
      retVal = true;
    }
  }

  return retVal;
}
