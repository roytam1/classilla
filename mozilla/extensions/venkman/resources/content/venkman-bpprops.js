/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*-
 *
 * The contents of this file are subject to the Mozilla Public License
 * Version 1.1 (the "License"); you may not use this file except in
 * compliance with the License. You may obtain a copy of the License at
 * http://www.mozilla.org/MPL/ 
 * 
 * Software distributed under the License is distributed on an "AS IS" basis,
 * WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
 * for the specific language governing rights and limitations under the
 * License. 
 *
 * The Original Code is The JavaScript Debugger
 * 
 * The Initial Developer of the Original Code is
 * Netscape Communications Corporation
 * Portions created by Netscape are
 * Copyright (C) 1998 Netscape Communications Corporation.
 *
 * Alternatively, the contents of this file may be used under the
 * terms of the GNU Public License (the "GPL"), in which case the
 * provisions of the GPL are applicable instead of those above.
 * If you wish to allow use of your version of this file only
 * under the terms of the GPL and not to allow others to use your
 * version of this file under the MPL, indicate your decision by
 * deleting the provisions above and replace them with the notice
 * and other provisions required by the GPL.  If you do not delete
 * the provisions above, a recipient may use your version of this
 * file under either the MPL or the GPL.
 *
 * Contributor(s):
 *  Robert Ginda, <rginda@netscape.com>, original author
 *
 */

var dialog = new Object();
var breakpoint;

function onLoad()
{
    function cacheElement(id)
    {
        dialog[id] = document.getElementById(id);
    };
    
    breakpoint = window.arguments[0];
    cacheElement("url-textbox");
    cacheElement("line-textbox");
    cacheElement("function-textbox");
    cacheElement("pc-textbox");
    cacheElement("condition-checkbox");
    cacheElement("trigger-textbox");
    cacheElement("enabled-checkbox");
    cacheElement("onetime-checkbox");
    cacheElement("condition-textbox");
    cacheElement("result-radio");
    cacheElement("exception-checkbox");
    cacheElement("log-checkbox");

    cacheElement("apply-button");
    cacheElement("revert-button");
    cacheElement("close-button");
    
    populateFromBreakpoint();
}

function onUnload()
{
    delete breakpoint.propsWindow;
}

function onBreakpointTriggered()
{
    dialog["trigger-textbox"].value = breakpoint.triggerCount;
}

function onConditionKeyPress(event)
{
    if (event.keyCode == 9)
    {
        opener.dd(dialog["condition-textbox"].selectionStart);
        event.preventDefault();
    }
}

function populateFromBreakpoint()
{
    dialog["url-textbox"].value = breakpoint.url;
    dialog["line-textbox"].value = breakpoint.lineNumber;

    dialog["condition-checkbox"].checked = breakpoint.conditionEnabled;
    if ("scriptWrapper" in breakpoint)
    {
        window.title = opener.MSG_BPPROPS_TITLE;
        dialog["enabled-checkbox"].setAttribute("label",
                                                opener.MSG_BPPROPS_ENABLED);
        dialog["function-textbox"].value = breakpoint.scriptWrapper.functionName;
        dialog["pc-textbox"].value = breakpoint.pc;
        dialog["onetime-checkbox"].checked = breakpoint.oneTime;
        dialog["trigger-textbox"].value = breakpoint.triggerCount;
    }
    else
    {
        window.title = opener.MSG_FBPPROPS_TITLE;
        dialog["enabled-checkbox"].setAttribute("label",
                                                opener.MSG_FBPPROPS_ENABLED);
        dialog["function-textbox"].value = opener.MSG_VAL_NA;
        dialog["pc-textbox"].value = opener.MSG_VAL_NONE;
        dialog["onetime-checkbox"].disabled = true;
        dialog["trigger-textbox"].disabled = true;
    }

    dialog["enabled-checkbox"].checked = breakpoint.enabled;
    dialog["condition-textbox"].value = breakpoint.condition;
    dialog["result-radio"].selectedItem =
        dialog["result-radio"].childNodes[breakpoint.resultAction];
    dialog["exception-checkbox"].checked = breakpoint.passExceptions;
    dialog["log-checkbox"].checked = breakpoint.logResult;
    updateConditionGroup();
    makeClean();
}

function copyToBreakpoint()
{
    breakpoint.conditionEnabled = dialog["condition-checkbox"].checked;
    if ("scriptWrapper" in breakpoint)
    {
        breakpoint.oneTime = dialog["onetime-checkbox"].checked;
        breakpoint.triggerCount = dialog["trigger-textbox"].value;
    }

    breakpoint.enabled = dialog["enabled-checkbox"].checked;
    for (var i = 0; i < 4; ++i)
    {
        if (dialog["result-radio"].childNodes[i] ==
            dialog["result-radio"].selectedItem)
        {
            dd ("result radio is " + i);
            breakpoint.resultAction = i;
            break;
        }
        else
            dd ("not at index " + i);
    }
    breakpoint.condition = dialog["condition-textbox"].value;
    breakpoint.passExceptions = dialog["exception-checkbox"].checked;
    breakpoint.logResult = dialog["log-checkbox"].checked;
    makeClean();
}

function updateConditionGroup()
{
    function ableNode (node)
    {
        node.disabled = !state;
        if (node.childNodes.length > 0)
        {
            for (var i = 0; i < node.childNodes.length; ++i)
                ableNode(node.childNodes[i]);
        }
    };

    var state = dialog["condition-checkbox"].checked;

    var kid = dialog["condition-checkbox"].parentNode.nextSibling;
    
    while (kid)
    {
        ableNode(kid);
        kid = kid.nextSibling;
    }
}

function makeDirty()
{
    dialog["apply-button"].disabled  = false;
    dialog["revert-button"].disabled = false;
    dialog["close-button"].disabled  = true;
}

function makeClean()
{
    dialog["apply-button"].disabled  = true;
    dialog["revert-button"].disabled = true;
    dialog["close-button"].disabled  = false;
}

    
    
