/***** BEGIN LICENSE BLOCK *****

NoScript - a Firefox extension for whitelist driven safe JavaScript execution
Copyright (C) 2004-2005 Giorgio Maone - g.maone@informaction.com

Modified for Classilla and Script-B-Gone (C)2009, 2010 Cameron Kaiser

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

***** END LICENSE BLOCK *****/

function NoScript() {

  if(this.cleanupIfUninstalling()) { // AS IF.
    return;
  }
  
  // make sure our policy is asserted
  const POLICY_NAME=this.POLICY_NAME;
  var prefArray;
  var prefString=null;
  try {
    prefArray=this.splitList(this.prefs.getCharPref("policynames"));
    var pcount=prefArray.length;
    while(pcount-->0 && prefArray[pcount]!=POLICY_NAME);
    if(pcount==-1) {
      prefArray[prefArray.length]=POLICY_NAME;
      prefString=prefArray.join(",");
    }
  } catch(ex) {
    prefString=POLICY_NAME;
  }
  if(prefString) {
    this.prefs.setCharPref("policynames",prefString);
    this.prefs.setCharPref(POLICY_NAME+".javascript.enabled","allAccess");
  }
  
  // now build our string off classilla.sbg preferences
  this.sbgToNoScript();
  
  // done. return ...
  this.sitesString;
}

NoScript.prototype={

  // our permanent whitelist
  _always_add : "sbg: about: about:blank chrome: file://" ,
  
  _myLogIt: function(msg, error) {
  	var consoleService = Components.classes["@mozilla.org/consoleservice;1"].
                       getService(Components.interfaces.nsIConsoleService);
  	msg = "NoScript Log: "+msg;
  	//if (error) { // apparently not supported in 1.3
  	//  consoleService.logStringError(msg);
  	//} else {
    	consoleService.logStringMessage(msg);
  	//}
  }
,
  POLICY_NAME: "maonoscript",

  _iprefs : null,
  get prefs() {
    return ((this._iprefs != null) ? this._iprefs :
    (this._iprefs = Components.classes["@mozilla.org/preferences-service;1"].getService(
      Components.interfaces.nsIPrefService).getBranch("capability.policy.")));
  }
,  

  // our sbg pref branch (cache it)
  _sbg : null,
  get sbgbranch() {
    return ((this._sbg != null) ? this._sbg :
    (this._sbg = Components.classes["@mozilla.org/preferences-service;1"].getService(
      Components.interfaces.nsIPrefService).getBranch("classilla.sbg.")));
  }
,  
  uninstalling: false
,
  splitList: function(s) {
    return s.split(/\s*[, ]\s*/);
  }
,
  sbgToNoScript : function() {
  	// turn sbg keys into a capability list
  	var olen = new Object();
 	var children = this.sbgbranch.getChildList("", olen);
 	if (children == null || children.length == null || children.length < 1) {
 		this.sitesString = this._always_add;
 		return;
 	}
 	var i = children.length;
 	var mystring = this._always_add;
 	var j;
 	
 	while(i--) {
 		if (children[i] && children[i].length &&
 				(j = children[i].indexOf(".")) != -1 &&
 				j != children[i].length) {
 			var protocol = children[i].substr(0, j);
 			var host = children[i].substr(j+1);
 			mystring += " "+ protocol+ "://" + host;
 		}
 	}
 	//this._myLogIt("string = "+mystring);
  	this.sitesString = mystring;
  }
,
  get sitesString() {
    try {
      return this.prefs.getCharPref(this.POLICY_NAME+".sites");
    } catch(ex) {
      return this._always_add;
    }
  }
,
  set sitesString(s) {
    s=s.replace(/(^\s+|\s+$)/g,'');
    if(s.substr(0,5) != "sbg: ")
    	s = this._always_add + " "+s;
    this.prefs.setCharPref(this.POLICY_NAME+".sites",s);
    return s;
  }
,
  get sites() {
    var sstr=this.sitesString;
    return /^\s*$/.test(sstr)?[]:sstr.split(/\s+/);
  }
,
  set sites(ss) {
    this.sitesString=ss.join(' ');
    return ss;
  }
,
  sortedSiteSet: function(ss) {
    ss=ss.sort();
    // sanitize and kill duplicates
    var prevSite=null;
    for(var j=ss.length; j-->0;) {
      var curSite=ss[j];
      if((!curSite) || curSite.toLowerCase().replace(/\s/g,'')==prevSite) { 
        ss.splice(j,1);
      } else {
        ss[j]=prevSite=curSite;
      }
    }
    return ss;
  }
,
  dopedSiteSet: function(ss) {
    // YOU CANNOT GET RID OF THEM HA HA HA PUNY MORTALS ahem.	
    return this.sortedSiteSet(ss.concat("about:", "about:blank", "chrome:"));
  }
,
  get jsEnabled() {
    try {
      return this.prefs.getCharPref("default.javascript.enabled") != "noAccess";
    } catch(ex) {
      return this.uninstalling || (this.jsEnabled=false);
    }
  }
,
  set jsEnabled(enabled) {
    //this._myLogIt("set jsEnabled ="+enabled);
    this.prefs.setCharPref("default.javascript.enabled",enabled?"allAccess":"noAccess");
    return enabled;
  }
,
  _ios: null,
  get ios() {
     return this._ios?this._ios
      :this._ios=Components.classes["@mozilla.org/network/io-service;1"
        ].getService(Components.interfaces.nsIIOService);
  }
,
  getSite: function(url, relative_to) {
    if(url==null || url == "") return null;
    if(/^(about|chrome):/.test(url)) return url;
    var protocol;
    if (relative_to && url.indexOf("://")==-1)
    	url = relative_to + url;
    try {
      protocol=this.ios.extractScheme(url)+"://";
    } catch(ex) {
      url=(protocol="http://")+url;
    }
    try {
      var k;
      k = this.ios.newURI(url,null,null).host;
      return (k == null || k == "") ? null : (protocol+k);
    } catch(ex) {
      return null;
    }
  }
,
  isJSEnabled: function(site) {
    if(!site) return false;
    if (/^about:/.test(site) || /^chrome:/.test(site))
    	return true;
    var w = site.replace("://", ".");
    var i = 0;
    if(w.indexOf("/") != -1) {
    	this._myLogIt("Bad input to isJSEnabled: "+site);
    	return false;
    }
    try { i = this.sbgbranch.getCharPref(w).length; } catch(e) { return false; }
    if (i > 0) {
    	var sites=this.sitesString;
    	if(!(sites && sites.length)) return false;
    	var pos=sites.indexOf(" "+site);
		if (pos == -1) {
			this._myLogIt("FAILURE> "+site+" not in whitelist string, added "+
				"(len "+sites.length+")");
    		var w = this.sitesString.replace(this._always_add, "");
    		w = w.replace(/^ +/, "");
    		this.sitesString = this._always_add + " " + site +
    			(w.length ? (" " + w) : "");
    	}
    	return true;
    }
    return false;
  }
,
  setJSEnabled: function(site,enabled) {
  	return enabled;
  	
  	
  	
    if(!site) return false;
    this._myLogIt("getting set with "+site+" to "+enabled);
    //if(site.substr(0,6) == "about:" || site.substr(0,7) == "chrome:") 
    //  return enabled; // but don't actually do anything
    var sites=this.sitesString.split(/\s+/);
    var scount=sites.length;
    while(scount-->0) {
      if(sites[scount]==site) {
        if(enabled) return true;
        sites.splice(scount,1);
        break;
      }
    }
    if(scount==-1) sites[sites.length]=site;
    this.sites=sites;
    return enabled;
  }
,
  willBeUninstalled: function() {
   if(this.uninstalling) return true;
   try {
     const RDFService = Components.classes["@mozilla.org/rdf/rdf-service;1"]
               .getService(Components.interfaces.nsIRDFService);
     const container = Components.classes["@mozilla.org/rdf/container;1"]
               .getService(Components.interfaces.nsIRDFContainer);
     const extensionDS= Components.classes["@mozilla.org/extensions/manager;1"]
          .getService(Components.interfaces.nsIExtensionManager).datasource;
     var root = RDFService.GetResource("urn:mozilla:extension:root");
     const nameArc = RDFService.GetResource("http://www.mozilla.org/2004/em-rdf#name");
     const toBeUninstalledArc = RDFService.GetResource("http://www.mozilla.org/2004/em-rdf#toBeUninstalled");
     const toBeDisabledArc=RDFService.GetResource("http://www.mozilla.org/2004/em-rdf#toBeDisabled");
     container.Init(extensionDS,root);
    
     var found = false;
     var elements = container.GetElements();
     var element,name,target;
     while (elements.hasMoreElements()) {
      element = elements.getNext().QueryInterface(Components.interfaces.nsIRDFResource);
     
  
      if((target=extensionDS.GetTarget(element, nameArc ,true)) && 
          target.QueryInterface(Components.interfaces.nsIRDFLiteral).Value=="NoScript"
        && 
        (
          (target = extensionDS.GetTarget(element, toBeUninstalledArc,true))
           && target.QueryInterface(Components.interfaces.nsIRDFLiteral).Value == "true"
          ||
          (target = extensionDS.GetTarget(element, toBeDisabledArc,true))
          && target.QueryInterface(Components.interfaces.nsIRDFLiteral).Value == "true"
        )  
      ) {
         return this.uninstalling=true;     
        }         
     }
   } catch(ex) {} // quick and dirty work-around for Mozilla ;)
   return this.uninstalling=false;
  }
,
  cleanupIfUninstalling: function() {
    if(this.willBeUninstalled()) this.cleanup();
    return this.uninstalling;
  }
,
  cleanup: function() {
    try {
      this.prefs.clearUserPref("default.javascript.enabled");
    } catch(ex) {
      dump(ex);
    }
    try {
      const POLICY_NAME=this.POLICY_NAME;
      var prefArray=this.splitList(this.prefs.getCharPref("policynames"));
      var pcount=prefArray.length;
      prefArrayTarget=[];
      for(var pcount=prefArray.length; pcount-->0;) {
        if(prefArray[pcount]!=POLICY_NAME) prefArrayTarget[prefArrayTarget.length]=prefArray[pcount];
      }
      this.prefs.setCharPref("policynames",prefArrayTarget.join(","));
      return Components.classes["@mozilla.org/preferences-service;1"].getService(
        Components.interfaces.nsIPrefService).savePrefFile(null);
    } catch(ex) {
      dump(ex);
    }
  }
}
