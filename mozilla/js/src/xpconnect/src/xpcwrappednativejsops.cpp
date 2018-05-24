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
 *   John Bandhauer <jband@netscape.com> (original author)
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

/* JavaScript JSClasses and JSOps for our Wrapped Native JS Objects. */

#include "xpcprivate.h"

// bug 289074
// Pull in nsIScriptSecurityManager.h temporarily until we can do
// IsSystemCallingContent() w/o a direct caps dependency.
#include "nsIScriptSecurityManager.h"

/***************************************************************************/

// All of the exceptions thrown into JS from this file go through here.
// That makes this a nice place to set a breakpoint.

static JSBool Throw(uintN errNum, JSContext* cx)
{
    XPCThrower::Throw(errNum, cx);
    return JS_FALSE;
}

// Handy macro used in many callback stub below.

#define THROW_AND_RETURN_IF_BAD_WRAPPER(cx, wrapper)                         \
    PR_BEGIN_MACRO                                                           \
    if(!wrapper)                                                             \
        return Throw(NS_ERROR_XPC_BAD_OP_ON_WN_PROTO, cx);                   \
    if(!wrapper->IsValid())                                                  \
        return Throw(NS_ERROR_XPC_HAS_BEEN_SHUTDOWN, cx);                    \
    PR_END_MACRO

// We rely on the engine only giving us jsval ids that are actually the
// self-same jsvals that are in the atom table (that is, if the id represents
// a string). So, we assert by converting the jsval to an id and then back
// to a jsval and comparing pointers. If the engine ever breaks this promise
// then we will scream.
#ifdef DEBUG
#define CHECK_IDVAL(cx, idval)                                               \
    PR_BEGIN_MACRO                                                           \
    if(JSVAL_IS_STRING(idval))                                               \
    {                                                                        \
        jsid d_id;                                                           \
        jsval d_val;                                                         \
        NS_ASSERTION(JS_ValueToId(cx, idval, &d_id), "JS_ValueToId failed!");\
        NS_ASSERTION(JS_IdToValue(cx, d_id, &d_val), "JS_IdToValue failed!");\
        NS_ASSERTION(d_val == idval, "id differs from id in atom table!");   \
    }                                                                        \
    PR_END_MACRO
#else
#define CHECK_IDVAL(cx, idval) ((void)0)
#endif

/***************************************************************************/

static JSBool
ToStringGuts(XPCCallContext& ccx)
{
    char* sz;
    XPCWrappedNative* wrapper = ccx.GetWrapper();

    if(wrapper)
        sz = wrapper->ToString(ccx, ccx.GetTearOff());
    else
        sz = JS_smprintf("[xpconnect wrapped native prototype]");

    if(!sz)
    {
        JS_ReportOutOfMemory(ccx);
        return JS_FALSE;
    }

    JSString* str = JS_NewString(ccx, sz, strlen(sz));
    if(!str)
    {
        JS_smprintf_free(sz);
        // JS_ReportOutOfMemory already reported by failed JS_NewString
        return JS_FALSE;
    }

    ccx.SetRetVal(STRING_TO_JSVAL(str));
    return JS_TRUE;
}

/***************************************************************************/

JS_STATIC_DLL_CALLBACK(JSBool)
XPC_WN_Shared_ToString(JSContext *cx, JSObject *obj,
                       uintN argc, jsval *argv, jsval *vp)
{
    XPCCallContext ccx(JS_CALLER, cx, obj);
    ccx.SetArgsAndResultPtr(argc, argv, vp);
    return ToStringGuts(ccx);
}

JS_STATIC_DLL_CALLBACK(JSBool)
XPC_WN_Shared_ToSource(JSContext *cx, JSObject *obj,
                       uintN argc, jsval *argv, jsval *vp)
{
    static const char empty[] = "{}";
    *vp = STRING_TO_JSVAL(JS_NewStringCopyN(cx, empty, sizeof(empty)-1));
    return JS_TRUE;
}

/***************************************************************************/

// A "double wrapped object" is a user JSObject that has been wrapped as a
// wrappedJS in order to be used by native code and then re-wrapped by a
// wrappedNative wrapper to be used by JS code. One might think of it as:
//    wrappedNative(wrappedJS(underlying_JSObject))
// This is done (as opposed to just unwrapping the wrapped JS and automatically
// returning the underlying JSObject) so that JS callers will see what looks
// Like any other xpcom object - and be limited to use its interfaces.
//
// See the comment preceeding nsIXPCWrappedJSObjectGetter in nsIXPConnect.idl.

static JSObject*
GetDoubleWrappedJSObject(XPCCallContext& ccx, XPCWrappedNative* wrapper)
{
    JSObject* obj = nsnull;
    nsCOMPtr<nsIXPConnectWrappedJS>
        underware = do_QueryInterface(wrapper->GetIdentityObject());
    if(underware)
    {
        JSObject* mainObj = nsnull;
        if(NS_SUCCEEDED(underware->GetJSObject(&mainObj)) && mainObj)
        {
            jsid id = ccx.GetRuntime()->
                    GetStringID(XPCJSRuntime::IDX_WRAPPED_JSOBJECT);

            jsval val;
            if(OBJ_GET_PROPERTY(ccx, mainObj, id,
                                &val) && !JSVAL_IS_PRIMITIVE(val))
            {
                obj = JSVAL_TO_OBJECT(val);
            }
        }
    }
    return obj;
}

// This is the getter native function we use to handle 'wrappedJSObject' for
// double wrapped JSObjects.

JS_STATIC_DLL_CALLBACK(JSBool)
XPC_WN_DoubleWrappedGetter(JSContext *cx, JSObject *obj,
                           uintN argc, jsval *argv, jsval *vp)
{
    XPCCallContext ccx(JS_CALLER, cx, obj);
    XPCWrappedNative* wrapper = ccx.GetWrapper();
    THROW_AND_RETURN_IF_BAD_WRAPPER(cx, wrapper);

    NS_ASSERTION(JS_TypeOfValue(cx, argv[-2]) == JSTYPE_FUNCTION, "bad function");

    JSObject* realObject = GetDoubleWrappedJSObject(ccx, wrapper);
    if(!realObject)
    {
        // This is pretty unexpected at this point. The object originally
        // responded to this get property call and now gives no object.
        // XXX Should this throw something at the caller?
        *vp = JSVAL_NULL;
        return JS_TRUE;
    }

    // It is a double wrapped object. Figure out if the caller
    // is allowed to see it.

    nsIXPCSecurityManager* sm;
    XPCContext* xpcc = ccx.GetXPCContext();

    sm = xpcc->GetAppropriateSecurityManager(
                    nsIXPCSecurityManager::HOOK_GET_PROPERTY);
    if(sm)
    {
        AutoMarkingNativeInterfacePtr iface(ccx);
        iface = XPCNativeInterface::
                    GetNewOrUsed(ccx, &NS_GET_IID(nsIXPCWrappedJSObjectGetter));

        if(iface)
        {
            jsval idval = ccx.GetRuntime()->
                        GetStringJSVal(XPCJSRuntime::IDX_WRAPPED_JSOBJECT);

            ccx.SetCallInfo(iface, iface->GetMemberAt(1), JS_FALSE);
            if(NS_FAILED(sm->
                    CanAccess(nsIXPCSecurityManager::ACCESS_GET_PROPERTY,
                              &ccx, ccx,
                              ccx.GetFlattenedJSObject(),
                              wrapper->GetIdentityObject(),
                              wrapper->GetClassInfo(), idval,
                              wrapper->GetSecurityInfoAddr())))
            {
                // The SecurityManager should have set an exception.
                return JS_FALSE;
            }
        }
    }
    *vp = OBJECT_TO_JSVAL(realObject);
    return JS_TRUE;
}

/***************************************************************************/

// This is our shared function to define properties on our JSObjects.

/*
 * NOTE:
 * We *never* set the tearoff names (e.g. nsIFoo) as JS_ENUMERATE.
 * We *never* set toString or toSource as JS_ENUMERATE.
 */

#define JSPROP_TRUST_ME 0x100   // XXXbe secret handshake

static JSBool
DefinePropertyIfFound(XPCCallContext& ccx,
                      JSObject *obj, jsval idval,
                      XPCNativeSet* set,
                      XPCNativeInterface* iface,
                      XPCNativeMember* member,
                      XPCWrappedNativeScope* scope,
                      JSBool reflectToStringAndToSource,
                      XPCWrappedNative* wrapperToReflectInterfaceNames,
                      XPCWrappedNative* wrapperToReflectDoubleWrap,
                      XPCNativeScriptableInfo* scriptableInfo,
                      uintN propFlags,
                      JSBool* resolved)
{
    XPCJSRuntime* rt = ccx.GetRuntime();
    JSBool found;
    const char* name;
    jsid id;
    
    propFlags |= JSPROP_TRUST_ME; // bug 289074

    if(set)
    {
        if(iface)
            found = JS_TRUE;
        else
            found = set->FindMember(idval, &member, &iface);
    }
    else
        found = (nsnull != (member = iface->FindMember(idval)));

    if(!found)
    {
        HANDLE_POSSIBLE_NAME_CASE_ERROR(ccx, set, iface, idval);

        if(reflectToStringAndToSource)
        {
            JSNative call;

            if(idval == rt->GetStringJSVal(XPCJSRuntime::IDX_TO_STRING))
            {
                call = XPC_WN_Shared_ToString;
                name = rt->GetStringName(XPCJSRuntime::IDX_TO_STRING);
                id   = rt->GetStringID(XPCJSRuntime::IDX_TO_STRING);
            }
            else if(idval == rt->GetStringJSVal(XPCJSRuntime::IDX_TO_SOURCE))
            {
                call = XPC_WN_Shared_ToSource;
                name = rt->GetStringName(XPCJSRuntime::IDX_TO_SOURCE);
                id   = rt->GetStringID(XPCJSRuntime::IDX_TO_SOURCE);
            }

            else
                call = nsnull;

            if(call)
            {
                JSFunction* fun = JS_NewFunction(ccx, call, 0, 0, obj, name);
                if(!fun)
                {
                    JS_ReportOutOfMemory(ccx);
                    return JS_FALSE;
                }

                AutoResolveName arn(ccx, idval);
                if(resolved)
                    *resolved = JS_TRUE;
                return OBJ_DEFINE_PROPERTY(ccx, obj, id,
                                           OBJECT_TO_JSVAL(JS_GetFunctionObject(fun)),
                                           nsnull, nsnull,
                                           propFlags & ~JSPROP_ENUMERATE,
                                           nsnull);
            }
        }
        // This *might* be a tearoff name that is not yet part of our
        // set. Let's lookup the name and see if it is the name of an
        // interface. Then we'll see if the object actually *does* this
        // interface and add a tearoff as necessary.

        if(wrapperToReflectInterfaceNames)
        {
            AutoMarkingNativeInterfacePtr iface2(ccx);
            XPCWrappedNativeTearOff* to;
            JSObject* jso;

            if(JSVAL_IS_STRING(idval) &&
               nsnull != (name = JS_GetStringBytes(JSVAL_TO_STRING(idval))) &&
               (iface2 = XPCNativeInterface::GetNewOrUsed(ccx, name), iface2) &&
               nsnull != (to = wrapperToReflectInterfaceNames->
                                    FindTearOff(ccx, iface2, JS_TRUE)) &&
               nsnull != (jso = to->GetJSObject()))

            {
                AutoResolveName arn(ccx, idval);
                if(resolved)
                    *resolved = JS_TRUE;
                return JS_ValueToId(ccx, idval, &id) &&
                       OBJ_DEFINE_PROPERTY(ccx, obj, id, OBJECT_TO_JSVAL(jso),
                                           nsnull, nsnull,
                                           propFlags & ~JSPROP_ENUMERATE,
                                           nsnull);
            }
        }

        // This *might* be a double wrapped JSObject
        if(wrapperToReflectDoubleWrap &&
           idval == rt->GetStringJSVal(XPCJSRuntime::IDX_WRAPPED_JSOBJECT) &&
           GetDoubleWrappedJSObject(ccx, wrapperToReflectDoubleWrap))
        {
            // We build and add a getter function.
            // A security check is done on a per-get basis.

            JSFunction* fun;

            id = rt->GetStringID(XPCJSRuntime::IDX_WRAPPED_JSOBJECT);
            name = rt->GetStringName(XPCJSRuntime::IDX_WRAPPED_JSOBJECT);

            fun = JS_NewFunction(ccx, XPC_WN_DoubleWrappedGetter,
                                 0, JSFUN_GETTER, obj, name);

            if(!fun)
                return JS_FALSE;

            JSObject* funobj = JS_GetFunctionObject(fun);
            if(!funobj)
                return JS_FALSE;

            propFlags |= JSPROP_GETTER;
            propFlags &= ~JSPROP_ENUMERATE;

            AutoResolveName arn(ccx, idval);
            if(resolved)
                *resolved = JS_TRUE;
            return OBJ_DEFINE_PROPERTY(ccx, obj, id, JSVAL_VOID,
                                       (JSPropertyOp) funobj, nsnull,
                                       propFlags, nsnull);
        }

#ifdef XPC_IDISPATCH_SUPPORT
        // Check to see if there's an IDispatch tearoff     
        if(wrapperToReflectInterfaceNames &&
            XPCIDispatchExtension::DefineProperty(ccx, obj, 
                idval, wrapperToReflectInterfaceNames, propFlags, resolved))
            return JS_TRUE;
#endif
        
        if(resolved)
            *resolved = JS_FALSE;
        return JS_TRUE;
    }

    if(!member)
    {
        if(wrapperToReflectInterfaceNames)
        {
            XPCWrappedNativeTearOff* to =
              wrapperToReflectInterfaceNames->FindTearOff(ccx, iface, JS_TRUE);

            if(!to)
                return JS_FALSE;
            JSObject* jso = to->GetJSObject();
            if(!jso)
                return JS_FALSE;

            AutoResolveName arn(ccx, idval);
            if(resolved)
                *resolved = JS_TRUE;
            return JS_ValueToId(ccx, idval, &id) &&
                   OBJ_DEFINE_PROPERTY(ccx, obj, id, OBJECT_TO_JSVAL(jso),
                                       nsnull, nsnull,
                                       propFlags & ~JSPROP_ENUMERATE,
                                       nsnull);
        }
        if(resolved)
            *resolved = JS_FALSE;
        return JS_TRUE;
    }

    if(member->IsConstant())
    {
        jsval val;
        AutoResolveName arn(ccx, idval);
        if(resolved)
            *resolved = JS_TRUE;
        return member->GetValue(ccx, iface, &val) &&
               JS_ValueToId(ccx, idval, &id) &&
               OBJ_DEFINE_PROPERTY(ccx, obj, id, val, nsnull, nsnull,
                                   propFlags, nsnull);
    }

    if(scriptableInfo &&
       scriptableInfo->GetFlags().DontEnumQueryInterface() &&
       idval == rt->GetStringJSVal(XPCJSRuntime::IDX_QUERY_INTERFACE))
        propFlags &= ~JSPROP_ENUMERATE;

    JSObject* funobj;
    
    {
        // scoped gc protection of funval
        jsval funval;

        if(!member->GetValue(ccx, iface, &funval))
            return JS_FALSE;
    
        AUTO_MARK_JSVAL(ccx, funval);

        funobj = JS_CloneFunctionObject(ccx, JSVAL_TO_OBJECT(funval), obj);
        if(!funobj)
            return JS_FALSE;
    }

    // protect funobj until it is actually attached
    AUTO_MARK_JSVAL(ccx, OBJECT_TO_JSVAL(funobj));

#ifdef off_DEBUG_jband
    {
        static int cloneCount = 0;
        if(!(++cloneCount%10))
            printf("<><><> %d cloned functions created\n", cloneCount);
    }
#endif

    if(member->IsMethod())
    {
        AutoResolveName arn(ccx, idval);
        if(resolved)
            *resolved = JS_TRUE;
        return JS_ValueToId(ccx, idval, &id) &&
               OBJ_DEFINE_PROPERTY(ccx, obj, id, OBJECT_TO_JSVAL(funobj),
                                   nsnull, nsnull, propFlags, nsnull);
    }

    // else...

    NS_ASSERTION(member->IsAttribute(), "way broken!");

    propFlags |= JSPROP_GETTER | JSPROP_SHARED;
    if(member->IsWritableAttribute())
    {
        propFlags |= JSPROP_SETTER;
        propFlags &= ~JSPROP_READONLY;
    }

    AutoResolveName arn(ccx, idval);
    if(resolved)
        *resolved = JS_TRUE;
    return JS_ValueToId(ccx, idval, &id) &&
           OBJ_DEFINE_PROPERTY(ccx, obj, id, JSVAL_VOID,
                               (JSPropertyOp) funobj,
                               (JSPropertyOp) funobj,
                               propFlags, nsnull);
}

/***************************************************************************/
/***************************************************************************/

JS_STATIC_DLL_CALLBACK(JSBool)
XPC_WN_OnlyIWrite_PropertyStub(JSContext *cx, JSObject *obj, jsval idval, jsval *vp)
{
    CHECK_IDVAL(cx, idval);

    XPCCallContext ccx(JS_CALLER, cx, obj, nsnull, idval);
    XPCWrappedNative* wrapper = ccx.GetWrapper();
    THROW_AND_RETURN_IF_BAD_WRAPPER(cx, wrapper);

    // Allow only XPConnect to add the property
    if(ccx.GetResolveName() == idval)
        return JS_TRUE;

    return Throw(NS_ERROR_XPC_CANT_MODIFY_PROP_ON_WN, cx);
}

JS_STATIC_DLL_CALLBACK(JSBool)
XPC_WN_CannotModifyPropertyStub(JSContext *cx, JSObject *obj, jsval idval, jsval *vp)
{
    CHECK_IDVAL(cx, idval);
    return Throw(NS_ERROR_XPC_CANT_MODIFY_PROP_ON_WN, cx);
}

JS_STATIC_DLL_CALLBACK(JSBool)
XPC_WN_Shared_Convert(JSContext *cx, JSObject *obj, JSType type, jsval *vp)
{
    if(type == JSTYPE_OBJECT)
    {
        *vp = OBJECT_TO_JSVAL(obj);
        return JS_TRUE;
    }

    XPCCallContext ccx(JS_CALLER, cx, obj);
    XPCWrappedNative* wrapper = ccx.GetWrapper();
    THROW_AND_RETURN_IF_BAD_WRAPPER(cx, wrapper);

    switch (type)
    {
        case JSTYPE_FUNCTION:
            {
                if(!ccx.GetTearOff())
                {
                    XPCNativeScriptableInfo* si = wrapper->GetScriptableInfo();
                    if(si && (si->GetFlags().WantCall() ||
                              si->GetFlags().WantConstruct()))
                    {
                        *vp = OBJECT_TO_JSVAL(obj);
                        return JS_TRUE;
                    }
                }
            }
            return Throw(NS_ERROR_XPC_CANT_CONVERT_WN_TO_FUN, cx);
        case JSTYPE_NUMBER:
            *vp = JS_GetNaNValue(cx);
            return JS_TRUE;
        case JSTYPE_BOOLEAN:
            *vp = JSVAL_TRUE;
            return JS_TRUE;
        case JSTYPE_VOID:
        case JSTYPE_STRING:
        {
            ccx.SetName(ccx.GetRuntime()->GetStringJSVal(XPCJSRuntime::IDX_TO_STRING));
            ccx.SetArgsAndResultPtr(0, nsnull, vp);

            XPCNativeMember* member = ccx.GetMember();
            if(member && member->IsMethod())
            {
                if(!XPCWrappedNative::CallMethod(ccx))
                    return JS_FALSE;

                if(JSVAL_IS_PRIMITIVE(*vp))
                    return JS_TRUE;
            }

            // else...
            return ToStringGuts(ccx);
        }
        default:
            NS_ERROR("bad type in conversion");
            return JS_FALSE;
    }
    NS_NOTREACHED("huh?");
    return JS_FALSE;
}

JS_STATIC_DLL_CALLBACK(JSBool)
XPC_WN_Shared_Enumerate(JSContext *cx, JSObject *obj)
{
    XPCCallContext ccx(JS_CALLER, cx, obj);
    XPCWrappedNative* wrapper = ccx.GetWrapper();
    THROW_AND_RETURN_IF_BAD_WRAPPER(cx, wrapper);

    // Since we aren't going to enumerate tearoff names and the prototype
    // handles non-mutated members, we can do this potential short-circuit.
    if(!wrapper->HasMutatedSet())
        return JS_TRUE;

    // Since we might be using this in the helper case, we check to
    // see if this is all avoidable.

    if(wrapper->GetScriptableInfo() &&
       wrapper->GetScriptableInfo()->GetFlags().DontEnumStaticProps())
        return JS_TRUE;

    XPCNativeSet* set = wrapper->GetSet();
    XPCNativeSet* protoSet = wrapper->HasProto() ?
                                wrapper->GetProto()->GetSet() : nsnull;

    PRUint16 interface_count = set->GetInterfaceCount();
    XPCNativeInterface** interfaceArray = set->GetInterfaceArray();
    for(PRUint16 i = 0; i < interface_count; i++)
    {
        XPCNativeInterface* iface = interfaceArray[i];
#ifdef XPC_IDISPATCH_SUPPORT
        if(iface->GetIID()->Equals(NSID_IDISPATCH))
        {
            XPCIDispatchExtension::Enumerate(ccx, obj, wrapper);
            continue;
        }
#endif
        PRUint16 member_count = iface->GetMemberCount();
        for(PRUint16 k = 0; k < member_count; k++)
        {
            XPCNativeMember* member = iface->GetMemberAt(k);
            jsval name = member->GetName();

            // Skip if this member is going to come from the proto.
            PRUint16 index;
            if(protoSet &&
               protoSet->FindMember(name, nsnull, &index) && index == i)
                continue;
            if(!xpc_ForcePropertyResolve(cx, obj, name))
                return JS_FALSE;
        }
    }
    return JS_TRUE;
}

/***************************************************************************/

JS_STATIC_DLL_CALLBACK(void)
XPC_WN_NoHelper_Finalize(JSContext *cx, JSObject *obj)
{
    XPCWrappedNative* p = (XPCWrappedNative*) JS_GetPrivate(cx, obj);
    if(!p)
        return;
    p->FlatJSObjectFinalized(cx, obj);
}

static void
MarkScopeJSObjects(JSContext *cx, XPCWrappedNativeScope* scope, void *arg)
{
    NS_ASSERTION(scope, "bad scope");

    JSObject* obj;

    obj = scope->GetGlobalJSObject();
    NS_ASSERTION(scope, "bad scope JSObject");
    JS_MarkGCThing(cx, obj, "XPCWrappedNativeScope::mGlobalJSObject", arg);

    obj = scope->GetPrototypeJSObject();
    if(obj)
    {
        JS_MarkGCThing(cx, obj, "XPCWrappedNativeScope::mPrototypeJSObject", arg);
    }
}

void
xpc_MarkForValidWrapper(JSContext *cx, XPCWrappedNative* wrapper, void *arg)
{
    // NOTE: It might be nice to also do the wrapper->Mark() call here too.
    // That call marks the wrapper's and wrapper's proto's interface sets.
    // We currently do that in the GC callback code. The reason we don't do that
    // here is because the bits used in that marking do unpleasant things to the
    // member counts in the interface and interface set objects. Those counts
    // are used in the DealWithDyingGCThings calls that are part of this JS GC
    // marking phase. By doing these calls later during our GC callback we 
    // avoid that problem. Arguably this could be changed. But it ain't broke.

    // However, we do need to call the wrapper's MarkBeforeJSFinalize so that
    // it can be sure that its (potentially shared) JSClass gets marked. The
    // danger is that a live wrapper might not be in a wrapper map and thus
    // won't be fully marked in the GC callback. This can happen if there is
    // a security exception during wrapper creation or if during wrapper
    // creation it is determined that the wrapper is not needed. In those cases
    // the wrapper can never actually be used from JS code - so resources like
    // the interface set will never be accessed. But the JS engine will still
    // need to use the JSClass. So, some marking is required for protection.

    wrapper->MarkBeforeJSFinalize(cx);
     
    if(wrapper->HasProto())
    {
        JSObject* obj = wrapper->GetProto()->GetJSProtoObject();
        NS_ASSERTION(obj, "bad proto");
        JS_MarkGCThing(cx, obj, "XPCWrappedNativeProto::mJSProtoObject", arg);
    }
    MarkScopeJSObjects(cx, wrapper->GetScope(), arg);
}

JS_STATIC_DLL_CALLBACK(uint32)
XPC_WN_Shared_Mark(JSContext *cx, JSObject *obj, void *arg)
{
    XPCWrappedNative* wrapper =
        XPCWrappedNative::GetWrappedNativeOfJSObject(cx, obj);

    if(wrapper && wrapper->IsValid())
        xpc_MarkForValidWrapper(cx, wrapper, arg);
    return 1;
}

JS_STATIC_DLL_CALLBACK(JSBool)
XPC_WN_NoHelper_Resolve(JSContext *cx, JSObject *obj, jsval idval)
{
    CHECK_IDVAL(cx, idval);

    XPCCallContext ccx(JS_CALLER, cx, obj, nsnull, idval);
    XPCWrappedNative* wrapper = ccx.GetWrapper();
    THROW_AND_RETURN_IF_BAD_WRAPPER(cx, wrapper);

    XPCNativeSet* set = ccx.GetSet();
    if(!set)
        return JS_TRUE;

    // Don't resolve properties that are on our prototype.
    if(ccx.GetInterface() && !ccx.GetStaticMemberIsLocal())
        return JS_TRUE;

    return DefinePropertyIfFound(ccx, obj, idval,
                                 set, nsnull, nsnull, wrapper->GetScope(),
                                 JS_TRUE, wrapper, wrapper, nsnull,
                                 JSPROP_ENUMERATE |
                                 JSPROP_READONLY |
                                 JSPROP_PERMANENT, nsnull);
}

JSClass XPC_WN_NoHelper_JSClass = {
    "XPCWrappedNative_NoHelper",    // name;
    JSCLASS_HAS_PRIVATE |
    JSCLASS_PRIVATE_IS_NSISUPPORTS, // flags;

    /* Mandatory non-null function pointer members. */
    XPC_WN_OnlyIWrite_PropertyStub, // addProperty;
    XPC_WN_CannotModifyPropertyStub,// delProperty;
    JS_PropertyStub,                // getProperty;
    XPC_WN_OnlyIWrite_PropertyStub, // setProperty;

    XPC_WN_Shared_Enumerate,        // enumerate;
    XPC_WN_NoHelper_Resolve,        // resolve;
    XPC_WN_Shared_Convert,          // convert;
    XPC_WN_NoHelper_Finalize,       // finalize;

    /* Optionally non-null members start here. */
    nsnull,                         // getObjectOps;
    nsnull,                         // checkAccess;
    nsnull,                         // call;
    nsnull,                         // construct;
    nsnull,                         // xdrObject;
    nsnull,                         // hasInstance;
    XPC_WN_Shared_Mark,             // mark;
    nsnull                          // spare;
};


/***************************************************************************/

JS_STATIC_DLL_CALLBACK(JSBool)
XPC_WN_MaybeResolvingPropertyStub(JSContext *cx, JSObject *obj, jsval idval, jsval *vp)
{
    CHECK_IDVAL(cx, idval);
    XPCCallContext ccx(JS_CALLER, cx, obj);
    XPCWrappedNative* wrapper = ccx.GetWrapper();
    THROW_AND_RETURN_IF_BAD_WRAPPER(cx, wrapper);

    if(ccx.GetResolvingWrapper() == wrapper)
        return JS_TRUE;
    return Throw(NS_ERROR_XPC_CANT_MODIFY_PROP_ON_WN, cx);
}

// macro fun!
#define PRE_HELPER_STUB                                                      \
    XPCWrappedNative* wrapper =                                              \
        XPCWrappedNative::GetWrappedNativeOfJSObject(cx, obj);               \
    THROW_AND_RETURN_IF_BAD_WRAPPER(cx, wrapper);                            \
    PRBool retval = JS_TRUE;                                                 \
    nsresult rv = wrapper->GetScriptableCallback()->

#define POST_HELPER_STUB                                                     \
    if(NS_FAILED(rv))                                                        \
        return Throw(rv, cx);                                                \
    return retval;

JS_STATIC_DLL_CALLBACK(JSBool)
XPC_WN_Helper_AddProperty(JSContext *cx, JSObject *obj, jsval idval, jsval *vp)
{
    PRE_HELPER_STUB
    AddProperty(wrapper, cx, obj, idval, vp, &retval);
    POST_HELPER_STUB
}

JS_STATIC_DLL_CALLBACK(JSBool)
XPC_WN_Helper_DelProperty(JSContext *cx, JSObject *obj, jsval idval, jsval *vp)
{
    PRE_HELPER_STUB
    DelProperty(wrapper, cx, obj, idval, vp, &retval);
    POST_HELPER_STUB
}

static nsIScriptSecurityManager *sSecMan;
static nsIPrincipal *sSystemPrincipal;

// bug 289074
static PRBool
EnsureSecMan()
{
    if(!sSecMan)
    {
        CallGetService(NS_SCRIPTSECURITYMANAGER_CONTRACTID, &sSecMan);

        // No security manager reachable, not much point in pretending
        // we care about security then.
        if(!sSecMan)
            return PR_FALSE;

        sSecMan->GetSystemPrincipal(&sSystemPrincipal);

        if(!sSystemPrincipal)
        {
            // No system principal. Shouldn't happen, but if it does
            // we've lost already.
            NS_RELEASE(sSecMan);

            return PR_FALSE;
        }
    }
    return PR_TRUE;
}

void
XPC_WN_JSOps_Shutdown()
{
    NS_IF_RELEASE(sSecMan);
}

class XPCWrappedNativeOrProto
{
public:
#define XPC_PROTO_TAG     ((jsword)0x1)
#define XPC_PROTO_WORD(p) ((jsword)(p))

    static JSBool
    IsTaggedProto(XPCWrappedNativeProto* p)
        {return XPC_PROTO_WORD(p) & XPC_PROTO_TAG;}

    static XPCWrappedNativeProto*
    TagProto(XPCWrappedNativeProto* s)
        {NS_ASSERTION(!IsTaggedProto(s), "bad pointer!");
         return (XPCWrappedNativeProto*)(XPC_PROTO_WORD(s) | XPC_PROTO_TAG);}

    static XPCWrappedNativeProto*
    UntagProto(XPCWrappedNativeProto* s)
        {return (XPCWrappedNativeProto*)(XPC_PROTO_WORD(s) & ~XPC_PROTO_TAG);}

    JSBool
    IsWrapper() const {return !IsTaggedProto(mMaybeProto);}

    XPCWrappedNativeOrProto(XPCWrappedNative *wrapper)
    {
        NS_ASSERTION(wrapper, "oops, null wrapper");
        mMaybeWrapper = wrapper;
    }

    XPCWrappedNativeOrProto(XPCWrappedNativeProto *proto)
    {
        NS_ASSERTION(proto, "oops, null proto");
        mMaybeProto = TagProto(proto);
    }

    operator void*()
        {return UntagProto(mMaybeProto);}

    XPCWrappedNative* GetWrapper()
        {NS_ASSERTION(IsWrapper(), "d'oh!"); return mMaybeWrapper;}

    XPCWrappedNativeProto* GetProto()
        {NS_ASSERTION(!IsWrapper(), "D'OH!"); return UntagProto(mMaybeProto);}

    XPCWrappedNativeScope* GetScope()
        {return IsWrapper()
                ? mMaybeWrapper->GetScope()
                : UntagProto(mMaybeProto)->GetScope();}

    JSObject* GetFlatJSObject()
        {return IsWrapper()
                ? mMaybeWrapper->GetFlatJSObject()
                : UntagProto(mMaybeProto)->GetJSProtoObject();}

    XPCNativeScriptableInfo* GetScriptableInfo()
        {return IsWrapper()
                ? mMaybeWrapper->GetScriptableInfo()
                : UntagProto(mMaybeProto)->GetScriptableInfo();}

    static XPCWrappedNativeOrProto
    GetWrappedNativeOfJSObject(JSContext *cx, JSObject *obj,
                               JSObject *funobj = nsnull)
    {
        if(JS_InstanceOf(cx, obj, &XPC_WN_ModsAllowed_Proto_JSClass, nsnull))
            return (XPCWrappedNativeProto*) JS_GetPrivate(cx, obj);

        XPCWrappedNative* wrapper =
            XPCWrappedNative::GetWrappedNativeOfJSObject(cx, obj, funobj);

        // Inline and specialize THROW_AND_RETURN_IF_BAD_WRAPPER(cx, wrapper);
        if(!wrapper)
            Throw(NS_ERROR_XPC_BAD_OP_ON_WN_PROTO, cx);
        else if(!wrapper->IsValid())
        {
            Throw(NS_ERROR_XPC_HAS_BEEN_SHUTDOWN, cx);
            wrapper = nsnull;
        }
        return wrapper;
    }

private:
    union
    {
        XPCWrappedNative*       mMaybeWrapper;
        XPCWrappedNativeProto*  mMaybeProto;
    };
};

// XXXbe, XXXjst this checks if our chrome code is calling into
// content code, but perhaps it should check for any trust-domain
// boundary crossing.
static PRBool
IsSystemCallingContent(JSContext *cx, XPCWrappedNativeOrProto wrapper_or_proto)
{
    // Provided script is active on cx, a scope chain that's the same
    // as the global object in the wrappers scope means we're not in a
    // cross-trust- domain call (i.e. our system code is not calling
    // content etc).
    if(cx->fp &&
       (cx->fp->scopeChain == wrapper_or_proto.GetScope()->GetGlobalJSObject()))
        return PR_FALSE;

    if(!EnsureSecMan())
        return PR_FALSE;

    PRBool isCallerChrome = PR_FALSE;
    if(NS_SUCCEEDED(sSecMan->SubjectPrincipalIsSystem(&isCallerChrome)) &&
       isCallerChrome)
    {
        nsCOMPtr<nsIPrincipal> objectPrincipal;

        sSecMan->GetObjectPrincipal(cx, wrapper_or_proto.GetFlatJSObject(),
                                    getter_AddRefs(objectPrincipal));

        return objectPrincipal != sSystemPrincipal;
    }

    return PR_FALSE;
}

static JSBool
GetOrSetUnshadowedMemberValue(JSContext *cx,
                              XPCWrappedNativeOrProto wrapper_or_proto,
                              jsval idval, uintN argc, jsval *argv, jsval *vp,
                              JSBool *foundMember)
{
    // Ok, we know chrome is getting a content-defined property on a
    // wrapped native prototype.  We're done -- no way can we let this
    // madness go further.
    if(!wrapper_or_proto.IsWrapper())
        return Throw(NS_ERROR_XPC_BAD_OP_ON_WN_PROTO, cx);

    XPCWrappedNative* wrapper = wrapper_or_proto.GetWrapper();
    JSObject *obj = wrapper->GetFlatJSObject();

    // this will do verification and the method lookup for us
    XPCCallContext ccx(JS_CALLER, cx, obj, nsnull, idval);

    // did we find a method/attribute by that name?
    XPCNativeMember* member = ccx.GetMember();
    if(member)
    {
        *foundMember = JS_TRUE;

        // it would a be a big surprise if there is a member without
        // an interface :)
        XPCNativeInterface* iface = ccx.GetInterface();
        if(!iface)
            return Throw(NS_ERROR_XPC_BAD_CONVERT_JS, cx);

        // get (and perhaps lazily create) the member's cloneable
        // value (which is a function, unless we're dealing with a
        // constant).
        jsval val;
        if(!member->GetValue(ccx, iface, &val))
            return Throw(NS_ERROR_XPC_BAD_CONVERT_JS, cx);

        // For constants, we simply return the value
        if(member->IsConstant())
        {
            *vp = val;
            return JS_TRUE;
        }

        // Make sure val doesn't get collected during any of the JS
        // engine calls in this scope from here on.
        AUTO_MARK_JSVAL(ccx, val);

        // XXXbe why did jband add this?  It's not strictly necessary in core
        // JS native objects, e.g. d = new Date; y = d.getFullYear() does not
        // clone Date.prototype.getFullYear solely in order to make
        // d.getFullYear.__parent__ == d.
        // See the log comment for 1.26.22.10 of this file.
        // The concern here is that while jband cloned in Resolve, we are
        // cloning on every Get that crosses from chrome to content.

        // clone a function we can use for this object
        JSObject* funobj =
            JS_CloneFunctionObject(cx, JSVAL_TO_OBJECT(val), obj);
        if(!funobj)
            return Throw(NS_ERROR_XPC_BAD_CONVERT_JS, cx);

        // Extra scope just to be able to use AUTO_MARK_JSVAL once
        // more for funval below. W/o the extra scope we get name
        // collisions from the macro.
        {
            jsval funval = OBJECT_TO_JSVAL(funobj);

            // protect funobj until it is actually attached
            AUTO_MARK_JSVAL(ccx, funval);

            // Call the getter function if the member is an attribute
            if(member->IsAttribute())
            {
                jsid id;
                if(!JS_ValueToId(cx, idval, &id))
                    return JS_FALSE;

                NS_ASSERTION(argc == 0 || member->IsWritableAttribute(),
                             "Huh, setter called for readonly attribute!");

                // Do the actual call to the getter or setter.
                return js_InternalGetOrSet(cx, obj, id, funval,
                                           (argc == 0) ? JSACC_READ : JSACC_WRITE,
                                           argc, argv, vp);
            }

            // return the function
            *vp = OBJECT_TO_JSVAL(funobj);
        }
    } else
        *foundMember = JS_FALSE;
        
    return JS_TRUE;
}
// end bug

JS_STATIC_DLL_CALLBACK(JSBool)
XPC_WN_Helper_GetProperty(JSContext *cx, JSObject *obj, jsval idval, jsval *vp)
{
// bug 289074
#if(0)
    PRE_HELPER_STUB
    GetProperty(wrapper, cx, obj, idval, vp, &retval);
#else
    // We can't use PRE_HELPER_STUB here since we need code between
    // it and what it expects to follow.
    XPCWrappedNative* wrapper =
        XPCWrappedNative::GetWrappedNativeOfJSObject(cx, obj);
    THROW_AND_RETURN_IF_BAD_WRAPPER(cx, wrapper);

    if(IsSystemCallingContent(cx, wrapper))
    {
        JSBool foundMember;
        if(!GetOrSetUnshadowedMemberValue(cx, wrapper, idval, 0, nsnull, vp,
                                          &foundMember))
            return JS_FALSE;

        if(foundMember)
            return JS_TRUE;
    }

    PRBool retval = JS_TRUE;
    nsresult rv = wrapper->GetScriptableCallback()->
        GetProperty(wrapper, cx, obj, idval, vp, &retval);
#endif
// end bug
    POST_HELPER_STUB
}

// bug 289074
JS_STATIC_DLL_CALLBACK(JSBool)
XPC_WN_Safe_PropertyStub(JSContext *cx, JSObject *obj, jsval idval, jsval *vp)
{
    XPCWrappedNativeOrProto wrapper_or_proto =
        XPCWrappedNativeOrProto::GetWrappedNativeOfJSObject(cx, obj);

    if(wrapper_or_proto && IsSystemCallingContent(cx, wrapper_or_proto))
    {
        JSBool foundMember;
        if(!GetOrSetUnshadowedMemberValue(cx, wrapper_or_proto, idval,
                                          0, nsnull, vp, &foundMember))
            return JS_FALSE;

        if(foundMember)
            return JS_TRUE;
   }
    return JS_PropertyStub(cx, obj, idval, vp);
}

// We must wrap untrusted (user-defined) getters and setters defined
// on wrapped native instances with a thunk that checks whether the
// caller is crossing a trust domain, and bypasses the user-defined
// getter or setter if so.

const char id_str[]             = "id";
const char unsafe_gsobj_str[]   = "unsafe_gsobj";

JS_STATIC_DLL_CALLBACK(JSBool)
XPC_WN_Safe_GetterSetterThunkNative(JSContext *cx, JSObject *obj, uintN argc,
                                    jsval *argv, jsval *rval)
{
    JSObject *safe_gsobj = JSVAL_TO_OBJECT(argv[-2]);

    jsval idval;
    if(!JS_GetProperty(cx, safe_gsobj, id_str, &idval))
        return JS_FALSE;

    jsid id;
    if(!JS_ValueToId(cx, idval, &id))
        return JS_FALSE;
    XPCWrappedNativeOrProto wrapper_or_proto =
        XPCWrappedNativeOrProto::GetWrappedNativeOfJSObject(cx, obj,
                                                            safe_gsobj);

    if(wrapper_or_proto && IsSystemCallingContent(cx, wrapper_or_proto))
    {
        JSBool foundMember;
        if(!GetOrSetUnshadowedMemberValue(cx, wrapper_or_proto,
                                          ID_TO_VALUE(id), argc, argv, rval,
                                          &foundMember))
            return JS_FALSE;

        if(foundMember)
            return JS_TRUE;

        XPCWrappedNative* wrapper = wrapper_or_proto.GetWrapper();
        XPCNativeScriptableInfo* si = wrapper->GetScriptableInfo();
        if(si)
        {
            if(argc == 0 && si->GetFlags().WantGetProperty())
            {
                // wrapper wants GetProperty() calls, call it, to prevent
                // user defined getters from shadowing things from chrome
                // code.
                JSBool ok = PR_TRUE;
                nsresult rv =
                    si->GetCallback()->GetProperty(wrapper, cx,
                                                   wrapper->GetFlatJSObject(),
                                                   id, rval, &ok);

                if(NS_FAILED(rv))
                    return Throw(rv, cx);

                return ok;
            }

            if(argc == 1 && si->GetFlags().WantSetProperty())
            {
                // wrapper wants SetProperty() calls, call it, to prevent
                // user defined setters from shadowing things from chrome
                // code.
                JSBool ok = PR_TRUE;
                jsval v = argv[0];

                nsresult rv =
                    si->GetCallback()->SetProperty(wrapper, cx,
                                                   wrapper->GetFlatJSObject(),
                                                   id, &v, &ok);

                if(NS_FAILED(rv))
                    return Throw(rv, cx);

                return ok;
            }
        }

        // Fall through
    }

    jsval unsafe_gsobj_val;
    if(!JS_GetProperty(cx, safe_gsobj, unsafe_gsobj_str, &unsafe_gsobj_val))
        return JS_FALSE;

    return js_InternalGetOrSet(cx, obj, id, unsafe_gsobj_val,
                               (argc == 1) ? JSACC_WRITE : JSACC_READ,
                               argc, argv, rval);
}

#include "jsfun.h"      // XXXbe should have a JS_GetFunctionNative API

static JSPropertyOp
NewSafeGetterOrSetterThunk(JSContext *cx, JSObject *obj, jsid id,
                           JSPropertyOp gsop, uintN nargs)
{
    JSObject *unsafe_gsobj = (JSObject *) gsop;

    // If called from native code, no need for a thunk: gsop is trusted.
    if(!cx->fp)
        return gsop;

    // Similarly, check whether the caller is a chrome script we must trust.
    if(!EnsureSecMan())
        return NULL;
    PRBool isCallerChrome = PR_FALSE;
    if(NS_SUCCEEDED(sSecMan->SubjectPrincipalIsSystem(&isCallerChrome)) &&
       isCallerChrome)
        return gsop;

    // Don't double-thunk if we can help it.
    if(JS_ObjectIsFunction(cx, unsafe_gsobj))
    {
        JSFunction *unsafe_gsfun =
            (JSFunction *) JS_GetPrivate(cx, unsafe_gsobj);
        if(unsafe_gsfun->u.n.native == XPC_WN_Safe_GetterSetterThunkNative) // not u.native; see jsfun.h (for Classilla)
            return gsop;
    }

    // Make a thunk to check cross-trust-domain access before calling gsop.
    JSFunction *safe_gsfun = JS_NewFunction(cx,
                                            XPC_WN_Safe_GetterSetterThunkNative,
                                            nargs, 0, obj, NULL);
    if(!safe_gsfun)
        return NULL;

    JSObject *safe_gsobj = JS_GetFunctionObject(safe_gsfun);

    // The only clean way to get id and unsafe_gsobj through is via permanent,
    // readonly properties on safe_gsobj.
    if(!JS_DefineProperty(cx, safe_gsobj, id_str, ID_TO_VALUE(id), NULL, NULL,
                          JSPROP_READONLY | JSPROP_PERMANENT))
        return NULL;

    if(!JS_DefineProperty(cx, safe_gsobj, unsafe_gsobj_str,
                          OBJECT_TO_JSVAL(unsafe_gsobj), NULL, NULL,
                          JSPROP_READONLY | JSPROP_PERMANENT))
        return NULL;

    return (JSPropertyOp) safe_gsobj;
}

JS_STATIC_DLL_CALLBACK(JSBool)
XPC_WN_JSOp_Safe_DefineProperty(JSContext *cx, JSObject *obj,
                                jsid id, jsval value,
                                JSPropertyOp getter, JSPropertyOp setter,
                                uintN attrs, JSProperty **propp)
{
    if(!(attrs & JSPROP_TRUST_ME))
    {
        if(attrs & JSPROP_GETTER)
        {
            getter = NewSafeGetterOrSetterThunk(cx, obj, id, getter, 0);
            if(!getter)
                return JS_FALSE;
        }
        if(attrs & JSPROP_SETTER)
        {
            setter = NewSafeGetterOrSetterThunk(cx, obj, id, setter, 1);
            if(!setter)
                return JS_FALSE;
        }
    }

    return js_ObjectOps.defineProperty(cx, obj, id, value, getter, setter,
                                       attrs, propp);
}

JS_STATIC_DLL_CALLBACK(JSBool)
XPC_WN_JSOp_Safe_GetProperty(JSContext *cx, JSObject *obj, jsid id, jsval *vp)
{
    XPCWrappedNativeOrProto wrapper_or_proto =
        XPCWrappedNativeOrProto::GetWrappedNativeOfJSObject(cx, obj);

    if(IsSystemCallingContent(cx, wrapper_or_proto))
    {
        JSBool foundMember;
        if(!GetOrSetUnshadowedMemberValue(cx, wrapper_or_proto,
                                          ID_TO_VALUE(id), 0, nsnull, vp,
                                          &foundMember))
            return JS_FALSE;

        if(foundMember)
            return JS_TRUE;
    }

    return js_ObjectOps.getProperty(cx, obj, id, vp);
}
// end bug

JS_STATIC_DLL_CALLBACK(JSBool)
XPC_WN_Helper_SetProperty(JSContext *cx, JSObject *obj, jsval idval, jsval *vp)
{
    PRE_HELPER_STUB
    SetProperty(wrapper, cx, obj, idval, vp, &retval);
    POST_HELPER_STUB
}

JS_STATIC_DLL_CALLBACK(JSBool)
XPC_WN_Helper_Convert(JSContext *cx, JSObject *obj, JSType type, jsval *vp)
{
    PRE_HELPER_STUB
    Convert(wrapper, cx, obj, type, vp, &retval);
    POST_HELPER_STUB
}

JS_STATIC_DLL_CALLBACK(JSBool)
XPC_WN_Helper_CheckAccess(JSContext *cx, JSObject *obj, jsval idval,
                          JSAccessMode mode, jsval *vp)
{
    CHECK_IDVAL(cx, idval);
    PRE_HELPER_STUB
    CheckAccess(wrapper, cx, obj, idval, mode, vp, &retval);
    POST_HELPER_STUB
}

JS_STATIC_DLL_CALLBACK(JSBool)
XPC_WN_Helper_Call(JSContext *cx, JSObject *obj, uintN argc, jsval *argv,
                   jsval *rval)
{
    // this is a hack to get the obj of the actual object not the object
    // that JS thinks is the 'this' (which it passes as 'obj').
    if(!(obj = (JSObject*)argv[-2]))
        return JS_FALSE;

    PRE_HELPER_STUB
    Call(wrapper, cx, obj, argc, argv, rval, &retval);
    POST_HELPER_STUB
}

JS_STATIC_DLL_CALLBACK(JSBool)
XPC_WN_Helper_Construct(JSContext *cx, JSObject *obj, uintN argc, jsval *argv,
                        jsval *rval)
{
    // this is a hack to get the obj of the actual object not the object
    // that JS thinks is the 'this' (which it passes as 'obj').
    if(!(obj = (JSObject*)argv[-2]))
        return JS_FALSE;

    PRE_HELPER_STUB
    Construct(wrapper, cx, obj, argc, argv, rval, &retval);
    POST_HELPER_STUB
}

JS_STATIC_DLL_CALLBACK(JSBool)
XPC_WN_Helper_HasInstance(JSContext *cx, JSObject *obj, jsval v, JSBool *bp)
{
    PRE_HELPER_STUB
    HasInstance(wrapper, cx, obj, v, bp, &retval);
    POST_HELPER_STUB
}

JS_STATIC_DLL_CALLBACK(void)
XPC_WN_Helper_Finalize(JSContext *cx, JSObject *obj)
{
    XPCWrappedNative* wrapper = (XPCWrappedNative*) JS_GetPrivate(cx, obj);
    if(!wrapper)
        return;
    wrapper->GetScriptableCallback()->Finalize(wrapper, cx, obj);
    wrapper->FlatJSObjectFinalized(cx, obj);
}

JS_STATIC_DLL_CALLBACK(uint32)
XPC_WN_Helper_Mark(JSContext *cx, JSObject *obj, void *arg)
{
    PRUint32 ignored;
    XPCWrappedNative* wrapper =
        XPCWrappedNative::GetWrappedNativeOfJSObject(cx, obj);
    if(wrapper && wrapper->IsValid())
    {
        wrapper->GetScriptableCallback()->Mark(wrapper, cx, obj, arg, &ignored);
        xpc_MarkForValidWrapper(cx, wrapper, arg);
    }
    return (uint32) ignored;
}

JS_STATIC_DLL_CALLBACK(JSBool)
XPC_WN_Helper_NewResolve(JSContext *cx, JSObject *obj, jsval idval, uintN flags,
                         JSObject **objp)
{
    CHECK_IDVAL(cx, idval);

    XPCCallContext ccx(JS_CALLER, cx, obj);
    XPCWrappedNative* wrapper = ccx.GetWrapper();
    THROW_AND_RETURN_IF_BAD_WRAPPER(cx, wrapper);

    jsval old = ccx.SetResolveName(idval);

    nsresult rv = NS_OK;
    JSBool retval = JS_TRUE;
    JSObject* obj2FromScriptable = nsnull;

    XPCNativeScriptableInfo* si = wrapper->GetScriptableInfo();
    if(si && si->GetFlags().WantNewResolve())
    {
        XPCWrappedNative* oldResolvingWrapper;
        JSBool allowPropMods = si->GetFlags().AllowPropModsDuringResolve();

        if(allowPropMods)
            oldResolvingWrapper = ccx.SetResolvingWrapper(wrapper);

        rv = si->GetCallback()->NewResolve(wrapper, cx, obj, idval, flags,
                                             &obj2FromScriptable, &retval);

        if(allowPropMods)
            (void)ccx.SetResolvingWrapper(oldResolvingWrapper);
    }

    old = ccx.SetResolveName(old);
    NS_ASSERTION(old == idval, "bad nest");

    if(NS_SUCCEEDED(rv))
    {
        if(obj2FromScriptable)
        {
            *objp = obj2FromScriptable;
        }
        else if(wrapper->HasMutatedSet())
        {
            // We are here if scriptable did not resolve this property and
            // it *might* be in the instance set but not the proto set.

            XPCNativeSet* set = wrapper->GetSet();
            XPCNativeSet* protoSet = wrapper->HasProto() ?
                                        wrapper->GetProto()->GetSet() : nsnull;
            XPCNativeMember* member;
            XPCNativeInterface* iface;
            JSBool IsLocal;

            if(set->FindMember(idval, &member, &iface, protoSet, &IsLocal) &&
               IsLocal)
            {
                XPCWrappedNative* oldResolvingWrapper;

                XPCNativeScriptableFlags siFlags(0);
                if(si)
                    siFlags = si->GetFlags();

                uintN enumFlag =
                    siFlags.DontEnumStaticProps() ? 0 : JSPROP_ENUMERATE;

                XPCWrappedNative* wrapperForInterfaceNames =
                    siFlags.DontReflectInterfaceNames() ? nsnull : wrapper;

                JSBool resolved;
                oldResolvingWrapper = ccx.SetResolvingWrapper(wrapper);
                retval = DefinePropertyIfFound(ccx, obj, idval,
                                               set, iface, member,
                                               wrapper->GetScope(),
                                               JS_FALSE,
                                               wrapperForInterfaceNames,
                                               nsnull, si,
                                               enumFlag, &resolved);
                (void)ccx.SetResolvingWrapper(oldResolvingWrapper);
                if(retval && resolved)
                    *objp = obj;
            }
        }
    }
    else
    {
        Throw(rv, cx);
    }

    return retval;
}

/***************************************************************************/

extern "C" JS_IMPORT_DATA(JSObjectOps) js_ObjectOps;

static JSObjectOps XPC_WN_WithCall_JSOps;
static JSObjectOps XPC_WN_NoCall_JSOps;
static JSObjectOps XPC_WN_ModsAllowed_JSOps; // bug 289074

/*
    Here are the enumerator cases:

    set jsclass enumerate to stub (unless noted otherwise)

    if( helper wants new enumerate )
        if( DONT_ENUM_STATICS )
            forward to scriptable enumerate
        else
            if( set not mutated )
                forward to scriptable enumerate
            else
                call shared enumerate
                forward to scriptable enumerate
    else if( helper wants old enumerate )
        use this JSOp
        if( DONT_ENUM_STATICS )
            call scriptable enumerate
            call stub
        else
            if( set not mutated )
                call scriptable enumerate
                call stub
            else
                call shared enumerate
                call scriptable enumerate
                call stub

    else //... if( helper wants NO enumerate )
        if( DONT_ENUM_STATICS )
            use enumerate stub - don't use this JSOp thing at all
        else
            do shared enumerate - don't use this JSOp thing at all
*/

JS_STATIC_DLL_CALLBACK(JSBool)
XPC_WN_JSOp_Enumerate(JSContext *cx, JSObject *obj, JSIterateOp enum_op,
                      jsval *statep, jsid *idp)
{
    XPCCallContext ccx(JS_CALLER, cx, obj);
    XPCWrappedNative* wrapper = ccx.GetWrapper();
    THROW_AND_RETURN_IF_BAD_WRAPPER(cx, wrapper);

    XPCNativeScriptableInfo* si = wrapper->GetScriptableInfo();
    if(!si)
        return Throw(NS_ERROR_XPC_BAD_OP_ON_WN_PROTO, cx);

    PRBool retval = JS_TRUE;
    nsresult rv;

    if(si->GetFlags().WantNewEnumerate())
    {
        if(enum_op == JSENUMERATE_INIT &&
           !si->GetFlags().DontEnumStaticProps() &&
           wrapper->HasMutatedSet() &&
           !XPC_WN_Shared_Enumerate(cx, obj))
        {
            *statep = JSVAL_NULL;
            return JS_FALSE;
        }

        // XXX Might we really need to wrap this call and *also* call
        // js_ObjectOps.enumerate ???

        rv = si->GetCallback()->
            NewEnumerate(wrapper, cx, obj, enum_op, statep, idp, &retval);
        
        if(enum_op == JSENUMERATE_INIT && (NS_FAILED(rv) || !retval))
            *statep = JSVAL_NULL;
        
        if(NS_FAILED(rv))
            return Throw(rv, cx);
        return retval;
    }

    if(si->GetFlags().WantEnumerate())
    {
        if(enum_op == JSENUMERATE_INIT)
        {
            if(!si->GetFlags().DontEnumStaticProps() &&
               wrapper->HasMutatedSet() &&
               !XPC_WN_Shared_Enumerate(cx, obj))
            {
                *statep = JSVAL_NULL;
                return JS_FALSE;
            }
            rv = si->GetCallback()->
                Enumerate(wrapper, cx, obj, &retval);

            if(NS_FAILED(rv) || !retval)
                *statep = JSVAL_NULL;

            if(NS_FAILED(rv))
                return Throw(rv, cx);
            if(!retval)
                return JS_FALSE;
            // Then fall through and call js_ObjectOps.enumerate...
        }
    }

    // else call js_ObjectOps.enumerate...

    return js_ObjectOps.enumerate(cx, obj, enum_op, statep, idp);
}

JSObjectOps * JS_DLL_CALLBACK
XPC_WN_GetObjectOpsNoCall(JSContext *cx, JSClass *clazz)
{
    return &XPC_WN_NoCall_JSOps;
}

JSObjectOps * JS_DLL_CALLBACK
XPC_WN_GetObjectOpsWithCall(JSContext *cx, JSClass *clazz)
{
    return &XPC_WN_WithCall_JSOps;
}

JSBool xpc_InitWrappedNativeJSOps()
{
    if(!XPC_WN_NoCall_JSOps.newObjectMap)
    {
        memcpy(&XPC_WN_NoCall_JSOps, &js_ObjectOps, sizeof(JSObjectOps));
        XPC_WN_NoCall_JSOps.enumerate = XPC_WN_JSOp_Enumerate;
        // bug 289074
        XPC_WN_NoCall_JSOps.defineProperty = XPC_WN_JSOp_Safe_DefineProperty;
        XPC_WN_NoCall_JSOps.getProperty = XPC_WN_JSOp_Safe_GetProperty;
        // end bug

        memcpy(&XPC_WN_WithCall_JSOps, &js_ObjectOps, sizeof(JSObjectOps));
        XPC_WN_WithCall_JSOps.enumerate = XPC_WN_JSOp_Enumerate;
        // bug 289074
        XPC_WN_WithCall_JSOps.defineProperty = XPC_WN_JSOp_Safe_DefineProperty;
        XPC_WN_WithCall_JSOps.getProperty = XPC_WN_JSOp_Safe_GetProperty;

        memcpy(&XPC_WN_ModsAllowed_JSOps, &js_ObjectOps, sizeof(JSObjectOps));
        XPC_WN_ModsAllowed_JSOps.defineProperty = XPC_WN_JSOp_Safe_DefineProperty;
        XPC_WN_ModsAllowed_JSOps.getProperty = XPC_WN_JSOp_Safe_GetProperty;
        // end bug

        XPC_WN_NoCall_JSOps.call = nsnull;
        XPC_WN_NoCall_JSOps.construct = nsnull;
    }
    return JS_TRUE;
}

/***************************************************************************/

// static
XPCNativeScriptableInfo*
XPCNativeScriptableInfo::Construct(XPCCallContext& ccx,
                                   const XPCNativeScriptableCreateInfo* sci)
{
    NS_ASSERTION(sci->GetCallback(), "bad param");
    NS_ASSERTION(sci, "bad param");

    XPCNativeScriptableInfo* newObj =
        new XPCNativeScriptableInfo(sci->GetCallback());
    if(!newObj)
        return nsnull;

    char* name = nsnull;
    if(NS_FAILED(sci->GetCallback()->GetClassName(&name)) || !name)
    {
        delete newObj;
        return nsnull;
    }

    JSBool success;

    XPCJSRuntime* rt = ccx.GetRuntime();
    XPCNativeScriptableSharedMap* map = rt->GetNativeScriptableSharedMap();
    {   // scoped lock
        XPCAutoLock lock(rt->GetMapLock());
        success = map->GetNewOrUsed(sci->GetFlags(), name, newObj);
    }

    if(!success)
    {
        delete newObj;
        return nsnull;
    }

    return newObj;
}

void
XPCNativeScriptableShared::PopulateJSClass()
{
    NS_ASSERTION(mJSClass.name, "bad state!");

    mJSClass.flags = JSCLASS_HAS_PRIVATE |
                     JSCLASS_PRIVATE_IS_NSISUPPORTS |
                     JSCLASS_NEW_RESOLVE;

    if(mFlags.WantAddProperty())
        mJSClass.addProperty = XPC_WN_Helper_AddProperty;
    else if(mFlags.UseJSStubForAddProperty())
        mJSClass.addProperty = JS_PropertyStub;
    else if(mFlags.AllowPropModsDuringResolve())
        mJSClass.addProperty = XPC_WN_MaybeResolvingPropertyStub;
    else
        mJSClass.addProperty = XPC_WN_CannotModifyPropertyStub;

    if(mFlags.WantDelProperty())
        mJSClass.delProperty = XPC_WN_Helper_DelProperty;
    else if(mFlags.UseJSStubForDelProperty())
        mJSClass.delProperty = JS_PropertyStub;
    else if(mFlags.AllowPropModsDuringResolve())
        mJSClass.delProperty = XPC_WN_MaybeResolvingPropertyStub;
    else
        mJSClass.delProperty = XPC_WN_CannotModifyPropertyStub;

    if(mFlags.WantGetProperty())
        mJSClass.getProperty = XPC_WN_Helper_GetProperty;
    else
        mJSClass.getProperty = XPC_WN_Safe_PropertyStub; // JS_PropertyStub; // bug 289074

    if(mFlags.WantSetProperty())
        mJSClass.setProperty = XPC_WN_Helper_SetProperty;
    else if(mFlags.UseJSStubForSetProperty())
        mJSClass.setProperty = JS_PropertyStub;
    else if(mFlags.AllowPropModsDuringResolve())
        mJSClass.setProperty = XPC_WN_MaybeResolvingPropertyStub;
    else
        mJSClass.setProperty = XPC_WN_CannotModifyPropertyStub;

    // We figure out most of the enumerate strategy at call time.

    if(mFlags.WantNewEnumerate() || mFlags.WantEnumerate() ||
       mFlags.DontEnumStaticProps())
        mJSClass.enumerate = JS_EnumerateStub;
    else
        mJSClass.enumerate = XPC_WN_Shared_Enumerate;

    // We have to figure out resolve strategy at call time
    mJSClass.resolve = (JSResolveOp) XPC_WN_Helper_NewResolve;

    if(mFlags.WantConvert())
        mJSClass.convert = XPC_WN_Helper_Convert;
    else
        mJSClass.convert = XPC_WN_Shared_Convert;

    if(mFlags.WantFinalize())
        mJSClass.finalize = XPC_WN_Helper_Finalize;
    else
        mJSClass.finalize = XPC_WN_NoHelper_Finalize;

    // We let the rest default to nsnull unless the helper wants them...
    if(mFlags.WantCheckAccess())
        mJSClass.checkAccess = XPC_WN_Helper_CheckAccess;

    // Note that we *must* set
    //   mJSClass.getObjectOps = XPC_WN_GetObjectOpsNoCall
    // or
    //   mJSClass.getObjectOps = XPC_WN_GetObjectOpsWithCall
    // (even for the cases were it does not do much) because with these
    // dynamically generated JSClasses, the code in
    // XPCWrappedNative::GetWrappedNativeOfJSObject() needs to look for
    // that this callback pointer in order to identify that a given
    // JSObject represents a wrapper.

    if(mFlags.WantCall() || mFlags.WantConstruct())
    {
        mJSClass.getObjectOps = XPC_WN_GetObjectOpsWithCall;
        if(mFlags.WantCall())
            mJSClass.call = XPC_WN_Helper_Call;
        if(mFlags.WantConstruct())
            mJSClass.construct = XPC_WN_Helper_Construct;
    }
    else
    {
        mJSClass.getObjectOps = XPC_WN_GetObjectOpsNoCall;
    }

    if(mFlags.WantHasInstance())
        mJSClass.hasInstance = XPC_WN_Helper_HasInstance;

    if(mFlags.WantMark())
        mJSClass.mark = XPC_WN_Helper_Mark;
    else
        mJSClass.mark = XPC_WN_Shared_Mark;
}

/***************************************************************************/
/***************************************************************************/

JSBool JS_DLL_CALLBACK
XPC_WN_CallMethod(JSContext *cx, JSObject *obj,
                  uintN argc, jsval *argv, jsval *vp)
{
    NS_ASSERTION(JS_TypeOfValue(cx, argv[-2]) == JSTYPE_FUNCTION, "bad function");
    JSObject* funobj = JSVAL_TO_OBJECT(argv[-2]);
    XPCCallContext ccx(JS_CALLER, cx, obj, funobj, 0, argc, argv, vp);
    XPCWrappedNative* wrapper = ccx.GetWrapper();
    THROW_AND_RETURN_IF_BAD_WRAPPER(cx, wrapper);

    XPCNativeInterface* iface;
    XPCNativeMember*    member;

    if(!XPCNativeMember::GetCallInfo(ccx, funobj, &iface, &member))
        return Throw(NS_ERROR_XPC_CANT_GET_METHOD_INFO, cx);
    ccx.SetCallInfo(iface, member, JS_FALSE);
    return XPCWrappedNative::CallMethod(ccx);
}

JSBool JS_DLL_CALLBACK
XPC_WN_GetterSetter(JSContext *cx, JSObject *obj,
                    uintN argc, jsval *argv, jsval *vp)
{
    NS_ASSERTION(JS_TypeOfValue(cx, argv[-2]) == JSTYPE_FUNCTION, "bad function");
    JSObject* funobj = JSVAL_TO_OBJECT(argv[-2]);

    XPCCallContext ccx(JS_CALLER, cx, obj, funobj);
    XPCWrappedNative* wrapper = ccx.GetWrapper();
    THROW_AND_RETURN_IF_BAD_WRAPPER(cx, wrapper);

    XPCNativeInterface* iface;
    XPCNativeMember*    member;

    if(!XPCNativeMember::GetCallInfo(ccx, funobj, &iface, &member))
        return Throw(NS_ERROR_XPC_CANT_GET_METHOD_INFO, cx);

    ccx.SetArgsAndResultPtr(argc, argv, vp);
    if(argc)
    {
        ccx.SetCallInfo(iface, member, JS_TRUE);
        JSBool retval = XPCWrappedNative::SetAttribute(ccx);
        if(retval && vp)
            *vp = argv[0];
        return retval;
    }
    // else...

    ccx.SetCallInfo(iface, member, JS_FALSE);
    return XPCWrappedNative::GetAttribute(ccx);
}

/***************************************************************************/

JS_STATIC_DLL_CALLBACK(JSBool)
XPC_WN_Shared_Proto_Enumerate(JSContext *cx, JSObject *obj)
{
    NS_ASSERTION(
        JS_InstanceOf(cx, obj, &XPC_WN_ModsAllowed_Proto_JSClass, nsnull) ||
        JS_InstanceOf(cx, obj, &XPC_WN_NoMods_Proto_JSClass, nsnull),
                 "bad proto");
    XPCWrappedNativeProto* self = (XPCWrappedNativeProto*) JS_GetPrivate(cx, obj);
    if(!self)
        return JS_FALSE;

    if(self->GetScriptableInfo() &&
       self->GetScriptableInfo()->GetFlags().DontEnumStaticProps())
        return JS_TRUE;

    XPCNativeSet* set = self->GetSet();
    if(!set)
        return JS_FALSE;

    XPCCallContext ccx(JS_CALLER, cx);
    if(!ccx.IsValid())
        return JS_FALSE;

    PRUint16 interface_count = set->GetInterfaceCount();
    XPCNativeInterface** interfaceArray = set->GetInterfaceArray();
    for(PRUint16 i = 0; i < interface_count; i++)
    {
        XPCNativeInterface* iface = interfaceArray[i];
        PRUint16 member_count = iface->GetMemberCount();

        for(PRUint16 k = 0; k < member_count; k++)
        {
            if(!xpc_ForcePropertyResolve(cx, obj, iface->GetMemberAt(k)->GetName()))
                return JS_FALSE;
        }
    }

    return JS_TRUE;
}

JS_STATIC_DLL_CALLBACK(JSBool)
XPC_WN_Shared_Proto_Convert(JSContext *cx, JSObject *obj, JSType type, jsval *vp)
{
    // XXX ?
    return JS_TRUE;
}

JS_STATIC_DLL_CALLBACK(void)
XPC_WN_Shared_Proto_Finalize(JSContext *cx, JSObject *obj)
{
    // This can be null if xpc shutdown has already happened
    XPCWrappedNativeProto* p = (XPCWrappedNativeProto*) JS_GetPrivate(cx, obj);
    if(p)
        p->JSProtoObjectFinalized(cx, obj);
}

JS_STATIC_DLL_CALLBACK(uint32)
XPC_WN_Shared_Proto_Mark(JSContext *cx, JSObject *obj, void *arg)
{
    // This can be null if xpc shutdown has already happened
    XPCWrappedNativeProto* p = (XPCWrappedNativeProto*) JS_GetPrivate(cx, obj);
    if(p)
        MarkScopeJSObjects(cx, p->GetScope(), arg);
    return 1;
}

/*****************************************************/

JS_STATIC_DLL_CALLBACK(JSBool)
XPC_WN_ModsAllowed_Proto_Resolve(JSContext *cx, JSObject *obj, jsval idval)
{
    CHECK_IDVAL(cx, idval);

    NS_ASSERTION(
        JS_InstanceOf(cx, obj, &XPC_WN_ModsAllowed_Proto_JSClass, nsnull),
                 "bad proto");

    XPCWrappedNativeProto* self = (XPCWrappedNativeProto*) JS_GetPrivate(cx, obj);
    if(!self)
        return JS_FALSE;

    XPCCallContext ccx(JS_CALLER, cx);
    if(!ccx.IsValid())
        return JS_FALSE;

    XPCNativeScriptableInfo* si = self->GetScriptableInfo();
    uintN enumFlag = (si && si->GetFlags().DontEnumStaticProps()) ?
                                                0 : JSPROP_ENUMERATE;

    return DefinePropertyIfFound(ccx, obj, idval,
                                 self->GetSet(), nsnull, nsnull,
                                 self->GetScope(),
                                 JS_TRUE, nsnull, nsnull, si,
                                 enumFlag, nsnull);
}

// bug 289074
JS_STATIC_DLL_CALLBACK(JSObjectOps *)
XPC_WN_ModsAllowed_Proto_GetObjectOps(JSContext *cx, JSClass *clasp)
{
    return &XPC_WN_ModsAllowed_JSOps;
} // end bug

JSClass XPC_WN_ModsAllowed_Proto_JSClass = {
    "XPC_WN_ModsAllowed_Proto_JSClass", // name;
    JSCLASS_HAS_PRIVATE,                // flags;

    /* Mandatory non-null function pointer members. */
    JS_PropertyStub,                // addProperty;
    JS_PropertyStub,                // delProperty;
    //JS_PropertyStub,                // getProperty;
    XPC_WN_Safe_PropertyStub,       // getProperty; // bug 289074
    JS_PropertyStub,                // setProperty;
    XPC_WN_Shared_Proto_Enumerate,         // enumerate;
    XPC_WN_ModsAllowed_Proto_Resolve,      // resolve;
    XPC_WN_Shared_Proto_Convert,           // convert;
    XPC_WN_Shared_Proto_Finalize,          // finalize;

    /* Optionally non-null members start here. */
    //nsnull,                         // getObjectOps;
    XPC_WN_ModsAllowed_Proto_GetObjectOps, // getObjectOps; // bug 289074
    nsnull,                         // checkAccess;
    nsnull,                         // call;
    nsnull,                         // construct;
    nsnull,                         // xdrObject;
    nsnull,                         // hasInstance;
    XPC_WN_Shared_Proto_Mark,       // mark;
    nsnull                          // spare;
};

/***************************************************************************/

JS_STATIC_DLL_CALLBACK(JSBool)
XPC_WN_OnlyIWrite_Proto_PropertyStub(JSContext *cx, JSObject *obj, jsval idval, jsval *vp)
{
    CHECK_IDVAL(cx, idval);

    NS_ASSERTION(
        JS_InstanceOf(cx, obj, &XPC_WN_NoMods_Proto_JSClass, nsnull),
                 "bad proto");

    XPCWrappedNativeProto* self = (XPCWrappedNativeProto*) JS_GetPrivate(cx, obj);
    if(!self)
        return JS_FALSE;

    XPCCallContext ccx(JS_CALLER, cx);
    if(!ccx.IsValid())
        return JS_FALSE;

    // Allow XPConnect to add the property only
    if(ccx.GetResolveName() == idval)
        return JS_TRUE;

    return Throw(NS_ERROR_XPC_BAD_OP_ON_WN_PROTO, cx);
}

JS_STATIC_DLL_CALLBACK(JSBool)
XPC_WN_NoMods_Proto_Resolve(JSContext *cx, JSObject *obj, jsval idval)
{
    CHECK_IDVAL(cx, idval);

    NS_ASSERTION(
        JS_InstanceOf(cx, obj, &XPC_WN_NoMods_Proto_JSClass, nsnull),
                 "bad proto");

    XPCWrappedNativeProto* self = (XPCWrappedNativeProto*) JS_GetPrivate(cx, obj);
    if(!self)
        return JS_FALSE;

    XPCCallContext ccx(JS_CALLER, cx);
    if(!ccx.IsValid())
        return JS_FALSE;

    XPCNativeScriptableInfo* si = self->GetScriptableInfo();
    uintN enumFlag = (si && si->GetFlags().DontEnumStaticProps()) ?
                                                0 : JSPROP_ENUMERATE;

    return DefinePropertyIfFound(ccx, obj, idval,
                                 self->GetSet(), nsnull, nsnull,
                                 self->GetScope(),
                                 JS_TRUE, nsnull, nsnull, si,
                                 JSPROP_READONLY |
                                 JSPROP_PERMANENT |
                                 enumFlag, nsnull);
}

JSClass XPC_WN_NoMods_Proto_JSClass = {
    "XPC_WN_NoMods_Proto_JSClass",      // name;
    JSCLASS_HAS_PRIVATE,                // flags;

    /* Mandatory non-null function pointer members. */
    XPC_WN_OnlyIWrite_Proto_PropertyStub,  // addProperty;
    XPC_WN_CannotModifyPropertyStub,       // delProperty;
    /*
       It's ok to use JS_PropertyStub here since this class (which is
       used for wrapped native's prototypes) doesn't allow any changes
       and thus no properties on the prototype can be overridden.
     */
    JS_PropertyStub,                       // getProperty;
    XPC_WN_OnlyIWrite_Proto_PropertyStub,  // setProperty;
    XPC_WN_Shared_Proto_Enumerate,         // enumerate;
    XPC_WN_NoMods_Proto_Resolve,           // resolve;
    XPC_WN_Shared_Proto_Convert,           // convert;
    XPC_WN_Shared_Proto_Finalize,          // finalize;

    /* Optionally non-null members start here. */
    nsnull,                         // getObjectOps;
    nsnull,                         // checkAccess;
    nsnull,                         // call;
    nsnull,                         // construct;
    nsnull,                         // xdrObject;
    nsnull,                         // hasInstance;
    XPC_WN_Shared_Proto_Mark,       // mark;
    nsnull                          // spare;
};

/***************************************************************************/

JS_STATIC_DLL_CALLBACK(JSBool)
XPC_WN_TearOff_Enumerate(JSContext *cx, JSObject *obj)
{
    XPCCallContext ccx(JS_CALLER, cx, obj);
    XPCWrappedNative* wrapper = ccx.GetWrapper();
    THROW_AND_RETURN_IF_BAD_WRAPPER(cx, wrapper);

    XPCWrappedNativeTearOff* to = ccx.GetTearOff();
    XPCNativeInterface* iface;

    if(!to || nsnull == (iface = to->GetInterface()))
        return Throw(NS_ERROR_XPC_BAD_OP_ON_WN_PROTO, cx);

    PRUint16 member_count = iface->GetMemberCount();
    for(PRUint16 k = 0; k < member_count; k++)
    {
        if(!xpc_ForcePropertyResolve(cx, obj, iface->GetMemberAt(k)->GetName()))
            return JS_FALSE;
    }

    return JS_TRUE;
}

JS_STATIC_DLL_CALLBACK(JSBool)
XPC_WN_TearOff_Resolve(JSContext *cx, JSObject *obj, jsval idval)
{
    CHECK_IDVAL(cx, idval);

    XPCCallContext ccx(JS_CALLER, cx, obj);
    XPCWrappedNative* wrapper = ccx.GetWrapper();
    THROW_AND_RETURN_IF_BAD_WRAPPER(cx, wrapper);

    XPCWrappedNativeTearOff* to = ccx.GetTearOff();
    XPCNativeInterface* iface;

    if(!to || nsnull == (iface = to->GetInterface()))
        return Throw(NS_ERROR_XPC_BAD_OP_ON_WN_PROTO, cx);

    return DefinePropertyIfFound(ccx, obj, idval, nsnull, iface, nsnull,
                                 wrapper->GetScope(),
                                 JS_TRUE, nsnull, nsnull, nsnull,
                                 JSPROP_READONLY |
                                 JSPROP_PERMANENT |
                                 JSPROP_ENUMERATE, nsnull);
}

JS_STATIC_DLL_CALLBACK(void)
XPC_WN_TearOff_Finalize(JSContext *cx, JSObject *obj)
{
    XPCWrappedNativeTearOff* p = (XPCWrappedNativeTearOff*)
        JS_GetPrivate(cx, obj);
    if(!p)
        return;
    p->JSObjectFinalized();
}

JSClass XPC_WN_Tearoff_JSClass = {
    "WrappedNative_TearOff",            // name;
    JSCLASS_HAS_PRIVATE,                // flags;

    /* Mandatory non-null function pointer members. */
    XPC_WN_OnlyIWrite_PropertyStub,     // addProperty;
    XPC_WN_CannotModifyPropertyStub,    // delProperty;
    //JS_PropertyStub,                    // getProperty;
    XPC_WN_Safe_PropertyStub,           // getProperty; // bug 289074
    XPC_WN_OnlyIWrite_PropertyStub,     // setProperty;
    XPC_WN_TearOff_Enumerate,           // enumerate;
    XPC_WN_TearOff_Resolve,             // resolve;
    XPC_WN_Shared_Convert,              // convert;
    XPC_WN_TearOff_Finalize,            // finalize;

    /* Optionally non-null members start here. */
    nsnull,                         // getObjectOps;
    nsnull,                         // checkAccess;
    nsnull,                         // call;
    nsnull,                         // construct;
    nsnull,                         // xdrObject;
    nsnull,                         // hasInstance;
    nsnull,                         // mark;
    nsnull                          // spare;
};

