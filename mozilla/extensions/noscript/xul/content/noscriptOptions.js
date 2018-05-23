/***** BEGIN LICENSE BLOCK *****

NoScript - a Firefox extension for whitelist driven safe JavaScript execution
Copyright (C) 2004-2005 Giorgio Maone - g.maone@informaction.com

Script-B-Gone glue and modifications for Classilla (C) 2010 Cameron Kaiser

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

g_ns=new NoScript();
g_urlList=null;
g_jsglobal=null;
g_urlText=null;
g_addButton=null;
g_removeButton=null;
var gsbg_prefs = Components.classes["@mozilla.org/preferences-service;1"].getService(
	Components.interfaces.nsIPrefService).getBranch("classilla.sbg.");
function nso_init() {
  if(g_ns.uninstalling) { // this should never happen! 
    window.close();
    return;
  }
  g_urlText=document.getElementById("urlText");
  g_urlList=document.getElementById("urlList");
  g_jsglobal=document.getElementById("jsglobal");
  g_addButton=document.getElementById("addButton");
  g_removeButton=document.getElementById("removeButton");
  g_urlList.removeItemAt(0);

  var olen = new Object();
  var children = gsbg_prefs.getChildList("", olen);
  if (children && children.length) {
  	children.sort(function(a,b) { // ignore the protocol portion, group together
  		var c = a.substr(a.indexOf(".")+1);
  		var d = b.substr(b.indexOf(".")+1);
  		var e = c.localeCompare(d);
  		return ((e) ? e : a.localeCompare(b));
  	});
  	children.reverse();
  	var i = children.length;
  	while(i--) {
 		if (children[i] && children[i].length &&
 				(j = children[i].indexOf(".")) != -1 &&
 				j != children[i].length) {
 			var protocol = children[i].substr(0, j);
 			var host = children[i].substr(j+1);
			var site = protocol+ "://" + host;
			g_urlList.appendItem(site,site);
		}
	}
  }
  g_jsglobal.setAttribute("checked",g_ns.jsEnabled);
  nso_urlListChanged();
}

function nso_urlListChanged() {
  g_removeButton.setAttribute("disabled",g_urlList.selectedCount==0);
}

function nso_urlChanged() {
  var url=g_urlText.value;
  if(url.match(/\s/)) url=g_urlText.value=url.replace(/\s/g,'');
  var addEnabled=url.length>0 && (url=g_ns.getSite(url))!=null;
  if(addEnabled) {
    for(var j=g_urlList.getRowCount(); j-->0;) {
      if(g_urlList.getItemAtIndex(j).value==url) {
        addEnabled=false;
        break;
      }
    }
  }
  g_addButton.setAttribute("disabled",!addEnabled);
}

function nso_allow() {
  var site=g_ns.getSite(g_urlText.value);
  g_urlList.appendItem(site,site);
  nso_urlListChanged();
}

function nso_remove() {
  const selectedItems=g_urlList.selectedItems;
  for(var j=selectedItems.length; j-->0;) {
    var nam=selectedItems[j].value;
    g_urlList.removeItemAt(g_urlList.getIndexOfItem(selectedItems[j]));
  }
}

function nso_save() {
/* This loop must succeed, because if it crashes, it will destroy our shadow list! */
  if (!g_urlList || !g_urlList.getRowCount) {
  	alert("Object missing failure, abandoning changes");
  	return;
  }
  g_ns.jsEnabled=g_jsglobal.getAttribute("checked")=="true";
  gsbg_prefs.deleteBranch("");
  for(var j=g_urlList.getRowCount(); j-->0;) {
    var e = g_urlList.getItemAtIndex(j).value;
    if (e && e.length && e.length > 0) {
    	e = e.replace("://", ".");
    	gsbg_prefs.setCharPref(e, "y");
    }
  }
  g_ns.sbgToNoScript();
}

