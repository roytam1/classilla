/* -*- Mode: C; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*-
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
 *	Get interfaces by ID, and compare to the interface returned by name
 *
 */

StartTest( "Get interfaces by ID" );
SetupTest();
AddTestData();
StopTest();

function SetupTest() {
}

function AddTestData() {
	// positive test cases

	for ( p in Components.interfaces ) {
		AddTestCase(
			"Components.interfaces[" +p +"].equals( Components.ID(" +
			Components.interfaces[p].id +"))",
			true,
			Components.interfaces[p].equals(
				Components.ID(Components.interfaces[p].id))
		);
		AddTestCase(
			"Components.ID(" +Components.interfaces[p].id 
			+").equals( Components.interfaces["+p+"])",
			true,
			Components.interfaces[p].equals(
				Components.ID(Components.interfaces[p].id))
		);
	}
if (0) {
	// negative test cases
	AddTestCase(
		"Components.interfaces[0].equals( Components.ID("+
			Components.interfaces[1].id +") )",
		false,
		Components.interfaces[0].equals( 
			Components.ID(Components.interfaces[1].id) )
	);
}
}