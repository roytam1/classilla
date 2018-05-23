/***** BEGIN LICENSE BLOCK *****
	Glue for Script-B-Gone to NoScript by Cameron Kaiser (C)2010

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

var ns_pref = Components.classes["@mozilla.org/preferences-service;1"].getService(
    Components.interfaces.nsIPrefService).getBranch("");
var sbg_prefs = Components.classes["@mozilla.org/preferences-service;1"].getService(
	Components.interfaces.nsIPrefService).getBranch("classilla.sbg.");
var sbg_sites = new Array();
var sbg_fsites = null;
var sbg_docdoc = null;
var sbg_ns = null;
var sbg_window = null;
      
function _noScript_openOptions(ev) {
	  document.getElementById('dothis').selectedIndex = 3;
      window.openDialog("chrome://noscript/content/noscriptOptions.xul", "",
                        "chrome,resizable=yes,modal=yes", 
                        ns_pref.getBoolPref("dom.disable_open_during_load"),
                        "",
                        false);
}

function sbg_setup() {
	sbg_sites = window.arguments[0].sites;
	sbg_docdoc = window.arguments[0].docdoc;
	sbg_ns = window.arguments[0].ns;
	sbg_window = window.arguments[0].browser;
	
	// figure out the domain.
	sbg_fsites = domainamain(sbg_sites, sbg_docdoc);
	sbg_sites.sort();
	sbg_fsites.sort();
	
	// write in our results
	document.getElementById('ldall').value = sbg_fsites.join("\n");
	document.getElementById('lalld').value = sbg_sites.join("\n");
	document.getElementById('lthis').value = " (" + sbg_docdoc + ")";
	document.getElementById('dothis').selectedIndex = 0;
}

function sbg_done() {
	var i;
	
	if((i = document.getElementById('dothis').selectedIndex) == 3)
		return;
	var working_set = (i == 0) ? sbg_fsites : (i == 1) ? [ sbg_docdoc ] : sbg_sites;
	if (working_set == null || working_set.length == null || working_set.length < 1)
		return;
	var j = working_set.length;
	while (j--) {
		var e = working_set[j];
		e = e.replace("://", ".");
		sbg_prefs.setCharPref(e, "y");
	}
	
	// regenerate
	sbg_ns.sbgToNoScript();
	// and reload
	if (sbg_window && sbg_window.location && sbg_window.location.reload)
		sbg_window.location.reload();
	else
		alert("Problem finding content. Reload this page for changes to take effect.");
}

// figure out the root domain for this page, and see what script request
// match it.
function domainamain(a, s) {
	if (/:\/\/localhost$/.test(s) ||
			/:\/\/localhost\.localdomain$/.test(s) ||
			!/\./.test(s)) { // not an FQDN
		return [ s ];
	}
	var p = s.indexOf(".")+1;
	if(p>=s.length) return [ s ]; // how did that get here?
	var w = s.indexOf("://")+3;
	if (w < 3) w = 0;
	s = s.substr(w);
	p -= w;
	w = 0;
	
	var j = a.length;
	var r = new Array();	
	
	// determine domain if a TLD
	if (/\.(aero|asia|biz|cat|com|coop|edu|gov|info|int|jobs|mil|mobi|museum|name|net|org|pro|tel|travel|xxx)$/i.test(s)) {
		//alert(w+ " "+s.substr(p));
		while(s.indexOf(".", p) != -1) {
			//alert(w+ " "+s.substr(p));
			w = p;
			p = s.indexOf(".", p)+1; // don't do this in the loop!
			if (p >= s.length) return [ s ];
		}
	} else {
	// try some heuristics
		if (/^www\./i.test(s) && s.substr(4).indexOf(".") != -1 && s.length > 9) {
			// avoid ac.uk, etc.
			w = 4;
		}
	}
	var domain = s.substr(w);
	
	while(j--) {
		var e = a[j];
		var i = e.indexOf("://")+3;
		if (i<e.length) {
			var ss = e.substr(i);
			var k = ss.indexOf("."+domain);
			//alert(ss+ " " + domain + " " +ss.indexOf("."+domain)+ " " +(ss.length - domain.length -1));
			if (ss == domain || (k != -1 && k == (ss.length - domain.length - 1)))
				r.push(e);
		}
	}
	return r;
}