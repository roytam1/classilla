/* -*- Mode: C; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 4 -*-
 *
 * The contents of this file are subject to the Netscape Public
 * License Version 1.1 (the "License"); you may not use this file
 * except in compliance with the License. You may obtain a copy of
 * the License at http://www.mozilla.org/NPL/
 *
 * Software distributed under the License is distributed on an "AS
 * IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or
 * implied. See the License for the specific language governing
 * rights and limitations under the License.
 *
 * The Original Code is Mozilla Communicator client code, released
 * March 31, 1998.
 *
 * The Initial Developer of the Original Code is Netscape
 * Communications Corporation.  Portions created by Netscape are
 * Copyright (C) 1998 Netscape Communications Corporation. All
 * Rights Reserved.
 *
 * Contributor(s): 
 *
 * Alternatively, the contents of this file may be used under the
 * terms of the GNU Public License (the "GPL"), in which case the
 * provisions of the GPL are applicable instead of those above.
 * If you wish to allow use of your version of this file only
 * under the terms of the GPL and not to allow others to use your
 * version of this file under the NPL, indicate your decision by
 * deleting the provisions above and replace them with the notice
 * and other provisions required by the GPL.  If you do not delete
 * the provisions above, a recipient may use your version of this
 * file under either the NPL or the GPL.
 */

/**
 * Test evaluation.  Corresponds to the xpcTestCallJS in 
 * xpctest_calljs.cpp
 *
 * In order to see what exceptions may be thrown, 
 * Enumerate Components.results
 */

StartTest( "Evaluate JavaScript Expressions" );

SetupTest();
AddTestData();
AddTestComment();
StopTest();

function SetupTest() {
	CONTRACTID = "@mozilla.org/js/xpc/test/CallJS;1";
	CLASS = Components.classes[CONTRACTID].createInstance();
	IFACE = Components.interfaces.nsIXPCTestCallJS;
	ERROR = Components.results;

	caller = CLASS.QueryInterface(IFACE);

	JSO = new Function();
	JSO.prototype.Evaluate = new Function( "s", "this.result = eval(s); return this.result" );
	JSO.prototype.gotException = false;
	JSO.prototype.exception = {};

	// keep track of the errors we've tried to use.

	ERRORS_WE_GOT = {};

	for ( var p in Components.results ) {
		ERRORS_WE_GOT[p] = false;
	}
}

function AddTestData() {
	for ( p in Components.results ) {
		TestEvaluate(
			"throw new Components.Exception('error message', "+Components.results[p]+")",
			undefined,
			true,
			p);
	}
}

function TestEvaluate (evalArg, eResult, eGotException, eExceptionName ) {
	var jso = new JSO();
	caller.SetJSObject(jso);
	
	try {
		caller.Evaluate(evalArg);
	} catch (e) {
		jso.gotException = true;
		jso.exception = e;
	} finally {
		AddTestCase(
			"caller.Evaluate(" +evalArg+ "); jso.result is ",
			eResult,
			jso.result);

		AddTestCase(
			"jso.gotException ",
			eGotException,
			jso.gotException );

		AddTestCase(
			"jso.exception.name",
			eExceptionName,
			jso.exception.name );

		AddTestCase(
			"jso.exception.result",
			Components.results[eExceptionName],
			jso.exception.result );

		if ( jso.gotException ) {
			ERRORS_WE_GOT[ jso.exception.name ] = true;
		}
	}
}

function AddTestComment() {
	var s = "This test exercised the exceptions defined in "+
		"Components.results. The following errors were not exercised:\n";

	for ( var p in ERRORS_WE_GOT ) {
		if ( ERRORS_WE_GOT[p] == false ) {
			s += p +"\n";
		}
	}

	AddComment(s);

}

function JSException ( message, name, data ) {
	this.message = message;
	this.name = name;
	this.code = Components.results[name];
	this.location = 0;
	this.data = (data) ? data : null;
	this.initialize = new Function();
	this.toString = new Function ( "return this.message" );
}

function TryIt(s) {
	try {
		eval(s);
	} catch (e) {
		Enumerate(e);
	}
}
