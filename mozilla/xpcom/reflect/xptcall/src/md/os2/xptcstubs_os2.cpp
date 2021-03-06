/* -*- Mode: C; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 4 -*-
 *
 * The contents of this file are subject to the Netscape Public License
 * Version 1.0 (the "NPL"); you may not use this file except in
 * compliance with the NPL.  You may obtain a copy of the NPL at
 * http://www.mozilla.org/NPL/
 *
 * Software distributed under the NPL is distributed on an "AS IS" basis,
 * WITHOUT WARRANTY OF ANY KIND, either express or implied. See the NPL
 * for the specific language governing rights and limitations under the
 * NPL.
 *
 * The Initial Developer of this code under the NPL is Netscape
 * Communications Corporation.  Portions created by Netscape are
 * Copyright (C) 1999 Netscape Communications Corporation.  All Rights
 * Reserved.
 *
 * Contributor: John Fairhurst <john_fairhurst@iname.com>
 *              Henry Sobotka <sobotka@axess.com> added VAC++ support
 *              and fixed emx asm to work with gcc 2.95.2 (Jan. 2000)
 */

/* Implement shared vtbl methods */

#include "xptcprivate.h"

#if !defined (__EMX__) && !defined(__IBMCPP__)
#error "This code is only for OS/2"
#endif

// Procedure in xptcall_vacpp.asm
#ifdef XP_OS2_VACPP
extern nsresult SetEntryFromIndex(int stubidx);
#endif

#ifdef XP_OS2_VACPP
nsresult
PrepareAndDispatch( nsXPTCStubBase *self, PRUint32 methodIndex,
                    PRUint32 *args)
#else
static nsresult
PrepareAndDispatch( nsXPTCStubBase *self, PRUint32 methodIndex,
                    PRUint32 *args)
#endif
{
#define PARAM_BUFFER_COUNT     16

    nsXPTCMiniVariant paramBuffer[PARAM_BUFFER_COUNT];
    nsXPTCMiniVariant* dispatchParams = NULL;
    nsIInterfaceInfo* iface_info = NULL;
    const nsXPTMethodInfo* info;
    PRUint8 paramCount;
    PRUint8 i;
    nsresult result = NS_ERROR_FAILURE;

    // If anything fails before stackBytesToPop can be set then
    // the failure is completely catastrophic!

    NS_ASSERTION(self,"no self");

    self->GetInterfaceInfo(&iface_info);
    NS_ASSERTION(iface_info,"no interface info");

    iface_info->GetMethodInfo(PRUint16(methodIndex), &info);
    NS_ASSERTION(info,"no interface info");

    paramCount = info->GetParamCount();

#ifdef XP_OS2_VACPP
    /* If paramCount is > 0, write out the EDX pointer to the
       space on the stack args[0]. args[-4] is the space on
       the stack where it was pushed */
    if (paramCount) {
        args[0] = args[-4];

        /* If this is the second parameter, or if the first parameter is an
           8 byte long long, write out the ECX pointer to the space on the
           stack args[1]. args[-3] is the space on the stack where it was
           pushed */
        nsXPTType type = info->GetParam(0).GetType();
        if( paramCount > 1 ||
            type == nsXPTType::T_I64 || type == nsXPTType::T_U64 )
        {
            args[1] = args[-3];
        }            
    }
#endif

    // setup variant array pointer
    if(paramCount > PARAM_BUFFER_COUNT)
        dispatchParams = new nsXPTCMiniVariant[paramCount];
    else
        dispatchParams = paramBuffer;
    NS_ASSERTION(dispatchParams,"no place for params");

    PRUint32* ap = args;
    for(i = 0; i < paramCount; i++, ap++)
    {
        const nsXPTParamInfo& param = info->GetParam(i);
        const nsXPTType& type = param.GetType();
        nsXPTCMiniVariant* dp = &dispatchParams[i];

        if(param.IsOut() || !type.IsArithmetic())
        {
            dp->val.p = (void*) *ap;
            continue;
        }
        // else
        switch(type)
        {
        case nsXPTType::T_I8     : dp->val.i8  = *((PRInt8*)  ap);       break;
        case nsXPTType::T_I16    : dp->val.i16 = *((PRInt16*) ap);       break;
        case nsXPTType::T_I32    : dp->val.i32 = *((PRInt32*) ap);       break;
        case nsXPTType::T_I64    : dp->val.i64 = *((PRInt64*) ap); ap++; break;
        case nsXPTType::T_U8     : dp->val.u8  = *((PRUint8*) ap);       break;
        case nsXPTType::T_U16    : dp->val.u16 = *((PRUint16*)ap);       break;
        case nsXPTType::T_U32    : dp->val.u32 = *((PRUint32*)ap);       break;
        case nsXPTType::T_U64    : dp->val.u64 = *((PRUint64*)ap); ap++; break;
        case nsXPTType::T_FLOAT  : dp->val.f   = *((float*)   ap);       break;
        case nsXPTType::T_DOUBLE : dp->val.d   = *((double*)  ap); ap++; break;
        case nsXPTType::T_BOOL   : dp->val.b   = *((PRBool*)  ap);       break;
        case nsXPTType::T_CHAR   : dp->val.c   = *((char*)    ap);       break;
        case nsXPTType::T_WCHAR  : dp->val.wc  = *((wchar_t*) ap);       break;
        default:
            NS_ASSERTION(0, "bad type");
            break;
        }
    }

    result = self->CallMethod((PRUint16)methodIndex, info, dispatchParams);

    NS_RELEASE(iface_info);

    if(dispatchParams != paramBuffer)
        delete [] dispatchParams;

    return result;
}

#ifdef XP_OS2_VACPP

#define STUB_ENTRY(n)

#else

#define STUB_ENTRY(n) \
nsresult nsXPTCStubBase::Stub##n() \
{ \
  register nsresult (*method) (nsXPTCStubBase *, PRUint32, PRUint32 *) = PrepareAndDispatch; \
  int temp0, temp1; \
  register nsresult result; \
  __asm__ __volatile__( \
    "leal   0x0c(%%ebp), %%ecx\n\t"    /* args */ \
    "pushl  %%ecx\n\t" \
    "pushl  $"#n"\n\t"                 /* method index */ \
    "movl   0x08(%%ebp), %%ecx\n\t"    /* this */ \
    "pushl  %%ecx\n\t" \
    "call   *%%edx\n\t"                /* PrepareAndDispatch */ \
    "addl   $12, %%esp" \
    : "=a" (result),    /* %0 */ \
      "=&c" (temp0),    /* %1 */ \
      "=d" (temp1)      /* %2 */ \
    : "2" (method)      /* %2 */ \
    : "memory" ); \
    return result; \
}
#endif

#define SENTINEL_ENTRY(n) \
nsresult nsXPTCStubBase::Sentinel##n() \
{ \
    NS_ASSERTION(0,"nsXPCWrappedJS::Sentinel called"); \
    return NS_ERROR_NOT_IMPLEMENTED; \
}

#include "xptcstubsdef.inc"
