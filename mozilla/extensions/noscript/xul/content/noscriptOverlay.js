/***** BEGIN LICENSE BLOCK *****

NoScript - a Firefox extension for whitelist driven safe JavaScript execution
Copyright (C) 2004-2005 Giorgio Maone - g.maone@informaction.com

Modified for Classilla (C)2009 Cameron Kaiser

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

function NoScriptOverlay() {
  this.ns=new NoScript();
}

NoScriptOverlay.prototype={
  _myLogIt: function(msg, error) {

  return;
  var consoleService = Components.classes["@mozilla.org/consoleservice;1"].
                       getService(Components.interfaces.nsIConsoleService);
  msg = "NS says: "+msg;
  //if (error) { // apparently not supported in 1.3
  //  consoleService.logStringError(msg);
  //} else {
    consoleService.logStringMessage(msg);
  //}
},

  _strings: null,
  get strings() {
    return this._strings?this._strings:this._strings=document.getElementById("noscript-strings");  
  }
,
  _stringsFB: null,
  get stringsFB() {
    return this._stringsFB?this._stringsFB:this._stringsFB=document.getElementById("noscript-stringsFB");  
  }
,
  _stringFrom: function(bundle,key,parms) {
    try {
      return parms?bundle.getFormattedString(key,parms):bundle.getString(key);
    } catch(ex) {
      return null;
    }
  }
,
  getString: function(key,parms) {
    var s=this._stringFrom(this.strings,key,parms);
    return s?s:this._stringFrom(this.stringsFB,key,parms);
  }
,
  getSites: function() {
    function populate(tagName) {
      var elems=doc.getElementsByTagName(tagName);
      for(var j=elems.length; j-->0;) {
        try {
	  var p;
          p = ns.getSite(new XPCNativeWrapper(elems[j],"src").src);
          if (p)
		sites[sites.length] = p;
        } catch(ex) {
		//this._myLogIt("bugged populate getsites "+tagName);
        }
      }
    }
    var ns=this.ns;
    var doc;
    var durl;
    try {
      doc=this.srcDocument;
      durl=doc.URL;
    } catch(ex) {
      // OMG WORST KLUDGE EVAR
      // this depends on chrome documents, XUL windows, etc. not having srcDocs or URLs
      this._myLogIt("must be a chrome URL");
      return ['chrome:'];
    }
    try {
      if (durl.substr(0,6)=="about:" || durl.substr(0,7)=="chrome:") {
        // and this catches the ones that work
        this._myLogIt(durl+" is a chrome URL");
        return [durl];
      }
      var sites=[ns.getSite(doc.URL)];
      populate('frame');
      populate('iframe');
      return ns.sortedSiteSet(sites);
    } catch(ex) {
      this._myLogIt("bugged toplevel "+sites+" "+durl+" getSites: "+ex);
      return [];
    }
  }
,
  get prompter() {
    return Components.classes["@mozilla.org/embedcomp/prompt-service;1"
          ].getService(Components.interfaces.nsIPromptService);
  }
,
  uninstallAlert: function() {
    this.prompter.alert(window,this.getString("uninstall.alert.title"),
          this.getString("uninstall.alert.text",
            [this.getString("allowed."+(this.ns.jsEnabled?"glb":"no") ) ]
            ));
  }
,
  prepareMenu: function(ev) {
    if(this.ns.uninstalling) {
      ev.preventDefault();
      return;
    }
    var ns=this.ns;
    var miGlobal=document.getElementById("noscript-sm-allow-global");
    var global=ns.jsEnabled;
    miGlobal.setAttribute("label",this.getString((global?"forbid":"allow")+"Global"));
    miGlobal.setAttribute("oncommand","noscriptOverlay.menuAllow("+(!global)+")");
    miGlobal.setAttribute("tooltiptext",document.getElementById("noscript-status").getAttribute("tooltiptext"));
    miGlobal.setAttribute("image",this.getIcon(global?"no":"glb"));
    var sep=document.getElementById("noscript-sm-sep");
    
    var parent=miGlobal.parentNode;
    var node=miGlobal.nextSibling;
    var remNode;
    while(node && (node!=sep)) {
       remNode=node;
       node=node.nextSibling;
       parent.removeChild(remNode);
    }
    
    var sites=this.getSites();
    var site,enabled,lev;
    for(var j=sites.length; j-->0;) {
      site=sites[j];
      enabled=ns.isJSEnabled(site);
      node=document.createElement("menuitem");
      node.setAttribute("label",this.getString((enabled?"forbidLocal":"allowLocal"),[site]));
      node.setAttribute("statustext",site);
      node.setAttribute("oncommand","noscriptOverlay.menuAllow("+(!enabled)+",this)");
      node.setAttribute("class","menuitem-iconic");
      node.setAttribute("tooltiptext",this.getString("allowed."+(enabled?"yes":"no")));
      node.setAttribute("image",this.getIcon(enabled?"no":"yes"));
      parent.insertBefore(node,sep);
    }
  }
,
  get srcWindow() {
    //var w=document.commandDispatcher.focusedWindow;
    return new XPCNativeWrapper(window._content, 'document','getSelection()');
  }
,
  get srcDocument() {
    return new XPCNativeWrapper(this.srcWindow.document, 'getElementsByTagName()','URL');
  }
,
  menuAllow: function(enabled,menuItem) {
    if(menuItem) { // local 
      var site=menuItem.getAttribute("statustext");
      if(site) {
       this.ns.setJSEnabled(site,enabled);
      }
    } else { // global
      if(enabled) {
        enabled=this.prompter.confirm(window,this.getString("global.warning.title"),
          this.getString("global.warning.text"));
      }
      this.ns.jsEnabled=enabled;
    }
    this.syncUI();
  }
,
  _iconURL: null,
  getIcon: function(lev) {
    if(!this._iconURL)
       //this._iconURL=document.getElementById("noscript-status").src;
       this._iconURL=document.getElementById("noscript-status").getAttribute("src");
    return this._iconURL.replace(/\b(yes|no|glb|part)(\d+\.)/,lev+"$2")
  }
,
  syncUI: function(ev) {
    var ns=this.ns;
    
    //alert("sync UI 2");
    if(ev.eventPhase==Event.AT_TARGET && ev.type=="focus") {
      if((!this.ns.uninstalling) && this.cleanup()) {
        window.setTimeout(function() { noscriptOverlay.uninstallAlert(); },10);
      }
    }
    
    if(this.ns.uninstalling) {
      var popup=document.getElementById("noscript-status-popup");
      if(popup) {
        popup.parentNode.setAttribute("onclick","noscriptOverlay.uninstallAlert()");
        popup.parentNode.removeChild(popup);
      }
    }
    
    var global=ns.jsEnabled;
    var lev;
    var levstr = '';
    var scount=0;
    var upcount=0;
    var sites = new Array();
    if(global) {
      lev="glb";
    } else {
      sites=this.getSites();
      scount=sites.length;
      upcount=scount;
      this._myLogIt("sites: "+sites.join(' '));
      while(scount-->0 && !ns.isJSEnabled(sites[scount])) {
		this._myLogIt("comping "+sites[scount]);
	  }
      lev=scount>-1?"yes":"no";
      if (scount == -1)
        levstr=" (0/"+upcount+")";
      else
        levstr=" ("+(upcount-scount)+"/"+upcount+")";
      // eventually add support for partial? the image is there for later.
    }
    var widget=document.getElementById("noscript-status");
    if (widget) {
      widget.setAttribute("tooltiptext",this.getString("allowed."+lev)+levstr);
      widget.setAttribute("src",this.getIcon(lev));
    }
    //this.numberCalled++;
  }
,
  numberCalled: 0,
  chromeBase: "chrome://noscript/content/",
  chromeName: "noscript"
,
  openOptionsDialog: function() {
    window.openDialog(this.chromeBase+this.chromeName+"Options.xul",this.chromeName+"Options",
      "chrome,dialog,centerscreen,alwaysRaised");
  }
,
  openAboutDialog: function() {
    window.openDialog(this.chromeBase+"about.xul",this.chromeName+"About",
      "chrome,dialog,centerscreen");
  }
,
  cleanup: function() {
    return this.ns.cleanupIfUninstalling();
  }
}

noscriptOverlay=new NoScriptOverlay();

_noScript_syncUI=function(ev) { 
  //alert("sync UI 1");
  noscriptOverlay.syncUI(ev); 
};

function _noScript_openPopup(ev) {
      window.openDialog("chrome://noscript/content/noscriptOptions.xul", "",
                        "chrome,resizable=yes,modal=yes", 
                        pref.getBoolPref("dom.disable_open_during_load"),
                        "",
                        false);
}

window.addEventListener("load",_noScript_syncUI,false);
window.addEventListener("DOMFrameContentLoaded",_noScript_syncUI,false);
window.addEventListener("DOMContentLoaded",_noScript_syncUI,false);
window.addEventListener("focus",_noScript_syncUI,false);
window.addEventListener("command",_noScript_syncUI,false);
window.addEventListener("click",_noScript_syncUI,false);
window.addEventListener("unload",function() { noscriptOverlay.cleanup(); },false);
