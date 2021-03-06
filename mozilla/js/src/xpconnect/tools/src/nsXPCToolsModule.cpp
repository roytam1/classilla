/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 4 -*-
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
 * Copyright (C) 1999 Netscape Communications Corporation. All
 * Rights Reserved.
 *
 * Contributor(s): 
 *   John Bandhauer <jband@netscape.com>
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

/* Module code for XPCTools. */

#include "xpctools_private.h"

// Module implementation for the xpctools library

NS_GENERIC_FACTORY_CONSTRUCTOR(nsXPCToolsCompiler)
NS_GENERIC_FACTORY_CONSTRUCTOR(nsXPCToolsProfiler)

// {331148C0-E599-11d3-8F65-0010A4E73D9A}
#define COMPILER_CID \
    { 0x331148c0, 0xe599, 0x11d3, \
        { 0x8f, 0x65, 0x0, 0x10, 0xa4, 0xe7, 0x3d, 0x9a } }

// {7F5D12E0-E97B-11d3-8F69-0010A4E73D9A}
#define PROFILER_CID \
    { 0x7f5d12e0, 0xe97b, 0x11d3, \
        { 0x8f, 0x69, 0x0, 0x10, 0xa4, 0xe7, 0x3d, 0x9a } }

static const nsModuleComponentInfo components[] = {
 {nsnull, COMPILER_CID, XPCTOOLS_COMPILER_CONTRACTID, nsXPCToolsCompilerConstructor},
 {nsnull, PROFILER_CID, XPCTOOLS_PROFILER_CONTRACTID, nsXPCToolsProfilerConstructor}
};

NS_IMPL_NSGETMODULE(xpctools, components)
