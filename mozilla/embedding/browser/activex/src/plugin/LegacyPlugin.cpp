/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* ***** BEGIN LICENSE BLOCK *****
 * Version: NPL 1.1/GPL 2.0/LGPL 2.1
 *
 * The contents of this file are subject to the Netscape Public License
 * Version 1.1 (the "License"); you may not use this file except in
 * compliance with the License. You may obtain a copy of the License at
 * http://www.mozilla.org/NPL/
 *
 * Software distributed under the License is distributed on an "AS IS" basis,
 * WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
 * for the specific language governing rights and limitations under the
 * License.
 *
 * The Original Code is mozilla.org code.
 *
 * The Initial Developer of the Original Code is 
 * Netscape Communications Corporation.
 * Portions created by the Initial Developer are Copyright (C) 1998
 * the Initial Developer. All Rights Reserved.
 *
 * Contributor(s):
 *
 *   Adam Lock <adamlock@netscape.com>
 *   Paul Oswald <paul.oswald@isinet.com>
 *
 * Alternatively, the contents of this file may be used under the terms of
 * either the GNU General Public License Version 2 or later (the "GPL"), or
 * the GNU Lesser General Public License Version 2.1 or later (the "LGPL"),
 * in which case the provisions of the GPL or the LGPL are applicable instead
 * of those above. If you wish to allow use of your version of this file only
 * under the terms of either the GPL or the LGPL, and not to allow others to
 * use your version of this file under the terms of the NPL, indicate your
 * decision by deleting the provisions above and replace them with the notice
 * and other provisions required by the GPL or the LGPL. If you do not delete
 * the provisions above, a recipient may use your version of this file under
 * the terms of any one of the NPL, the GPL or the LGPL.
 *
 * ***** END LICENSE BLOCK ***** */
#include "stdafx.h"

#include "jni.h"
#include "npapi.h"

#include "nsISupports.h"

#ifdef MOZ_ACTIVEX_PLUGIN_XPCONNECT
#include "XPConnect.h"
#endif

#ifdef MOZ_ACTIVEX_PLUGIN_LIVECONNECT
#include "LiveConnect.h"
#endif

#include "LegacyPlugin.h"

#ifdef XPC_IDISPATCH_SUPPORT
#include "nsCOMPtr.h"
#include "nsString.h"
#include "nsIDocument.h"
#include "nsIDOMElement.h"
#include "nsIDOMDocument.h"
#include "nsIURI.h"
#endif

///////////////////////////////////////////////////////////////////////////////
// These are constants to control certain default behaviours

#ifndef MOZ_ACTIVEX_PLUGIN_XPCONNECT
// Flag determines if controls should be created if they are not marked
// safe for scripting.
const BOOL kHostSafeControlsOnly = FALSE;
// Flag determines if controls should be downloaded and installed if there is a
// codebase specified
const BOOL kDownloadControlsIfMissing = FALSE;
#endif

// Flag determines whether the plugin will complain to the user if a page
// contains a control it cannot load
const BOOL kDisplayErrorMessages = FALSE;

///////////////////////////////////////////////////////////////////////////////
// CInstallControlProgress

class CInstallControlProgress :
    public CComObjectRootEx<CComSingleThreadModel>,
    public IBindStatusCallback,
    public IWindowForBindingUI
{
public:
    CInstallControlProgress()
    {
    }

    BOOL mBindingInProgress;
    HRESULT mResult;
    NPP mNPP;

protected:
    virtual ~CInstallControlProgress()
    {
    }
public:
BEGIN_COM_MAP(CInstallControlProgress)
    COM_INTERFACE_ENTRY(IBindStatusCallback)
    COM_INTERFACE_ENTRY(IWindowForBindingUI)
END_COM_MAP()

// IBindStatusCallback
    HRESULT STDMETHODCALLTYPE OnStartBinding(DWORD dwReserved, 
                                            IBinding __RPC_FAR *pib)
    {
        return S_OK;
    }

    HRESULT STDMETHODCALLTYPE GetPriority(LONG __RPC_FAR *pnPriority)
    {
        return S_OK;
    }
        
    HRESULT STDMETHODCALLTYPE OnLowResource(DWORD reserved)
    {
        return S_OK;
    }
        
    HRESULT STDMETHODCALLTYPE OnProgress(ULONG ulProgress, 
                                         ULONG ulProgressMax, 
                                         ULONG ulStatusCode, 
                                         LPCWSTR szStatusText)
    {
        switch (ulStatusCode)
        {
        case BINDSTATUS_BEGINDOWNLOADDATA:
        case BINDSTATUS_DOWNLOADINGDATA:
        case BINDSTATUS_ENDDOWNLOADDATA:
            {
                char szMsg[100];
                _snprintf(szMsg, sizeof(szMsg) - 1, "Downloading control (%lu of %lu)", ulProgress, ulProgressMax);
                szMsg[sizeof(szMsg) - 1] = '\0';
                NPN_Status(mNPP, szMsg);
            }
            break;
        case BINDSTATUS_FINDINGRESOURCE:
        case BINDSTATUS_CONNECTING:
        case BINDSTATUS_REDIRECTING:
        case BINDSTATUS_BEGINDOWNLOADCOMPONENTS:
        case BINDSTATUS_INSTALLINGCOMPONENTS:
        case BINDSTATUS_ENDDOWNLOADCOMPONENTS:
        case BINDSTATUS_USINGCACHEDCOPY:
        case BINDSTATUS_SENDINGREQUEST:
        case BINDSTATUS_CLASSIDAVAILABLE:
        case BINDSTATUS_MIMETYPEAVAILABLE:
        case BINDSTATUS_CACHEFILENAMEAVAILABLE:
        case BINDSTATUS_BEGINSYNCOPERATION:
        case BINDSTATUS_ENDSYNCOPERATION:
        case BINDSTATUS_BEGINUPLOADDATA:
        case BINDSTATUS_UPLOADINGDATA:
        case BINDSTATUS_ENDUPLOADDATA:
        case BINDSTATUS_PROTOCOLCLASSID:
        case BINDSTATUS_ENCODING:
        case BINDSTATUS_CLASSINSTALLLOCATION:
        case BINDSTATUS_DECODING:
        case BINDSTATUS_LOADINGMIMEHANDLER:
        default:
            /* do nothing */
            break;
        }
        return S_OK;
    }

    HRESULT STDMETHODCALLTYPE OnStopBinding(HRESULT hresult, LPCWSTR szError)
    {
        mBindingInProgress = FALSE;
        mResult = hresult;
        NPN_Status(mNPP, "");
        return S_OK;
    }
        
    HRESULT STDMETHODCALLTYPE GetBindInfo(DWORD __RPC_FAR *pgrfBINDF, 
                                                        BINDINFO __RPC_FAR *pbindInfo)
    {
        *pgrfBINDF = BINDF_ASYNCHRONOUS | BINDF_ASYNCSTORAGE |
                    BINDF_GETNEWESTVERSION | BINDF_NOWRITECACHE;
        pbindInfo->cbSize = sizeof(BINDINFO);
        pbindInfo->szExtraInfo = NULL;
        memset(&pbindInfo->stgmedData, 0, sizeof(STGMEDIUM));
        pbindInfo->grfBindInfoF = 0;
        pbindInfo->dwBindVerb = 0;
        pbindInfo->szCustomVerb = NULL;
        return S_OK;
    }
        
    HRESULT STDMETHODCALLTYPE OnDataAvailable(DWORD grfBSCF, 
                                              DWORD dwSize, 
                                              FORMATETC __RPC_FAR *pformatetc, 
                                              STGMEDIUM __RPC_FAR *pstgmed)
    {
        return E_NOTIMPL;
    }
      
    HRESULT STDMETHODCALLTYPE OnObjectAvailable(REFIID riid, 
                                                IUnknown __RPC_FAR *punk)
    {
        return S_OK;
    }

// IWindowForBindingUI
    virtual HRESULT STDMETHODCALLTYPE GetWindow(
        /* [in] */ REFGUID rguidReason,
        /* [out] */ HWND *phwnd)
    {
        HWND hwnd = NULL;
        NPN_GetValue(mNPP, NPNVnetscapeWindow, &hwnd);
        *phwnd = hwnd;
        return S_OK;
    }
};

///////////////////////////////////////////////////////////////////////////////


// NPP_Initialize
//
//    Initialize the plugin library. Your DLL global initialization
//    should be here
//
NPError NPP_Initialize(void)
{
    NG_TRACE_METHOD(NPP_Initialize);
    _Module.Lock();
    return NPERR_NO_ERROR;
}


// NPP_Shutdown
//
//    shutdown the plugin library. Revert initializition
//
void NPP_Shutdown(void)
{
    NG_TRACE_METHOD(NPP_Shutdown);
#ifdef MOZ_ACTIVEX_PLUGIN_LIVECONNECT
    liveconnect_Shutdown();
#endif
    _Module.Unlock();
}


// NPP_GetJavaClass
//
//    Return the Java class representing this plugin
//
jref NPP_GetJavaClass(void)
{
    NG_TRACE_METHOD(NPP_GetJavaClass);
#ifdef MOZ_ACTIVEX_PLUGIN_LIVECONNECT
    return liveconnect_GetJavaClass();
#endif
    return NULL;
}

#define MIME_OLEOBJECT1   "application/x-oleobject"
#define MIME_OLEOBJECT2   "application/oleobject"
#define MIME_ACTIVESCRIPT "text/x-activescript"

enum MozAxPluginErrors
{
    MozAxErrorActiveScriptNotSupported,
    MozAxErrorControlIsNotSafeForScripting,
    MozAxErrorCouldNotCreateControl,
};

static void
ShowError(MozAxPluginErrors errorCode, const CLSID &clsid)
{
    if (!kDisplayErrorMessages)
        return;

    const TCHAR *szMsg = NULL;
    const unsigned long kBufSize = 256;
    TCHAR szBuffer[kBufSize];

    // TODO errors are hardcoded for now
    switch (errorCode)
    {
    case MozAxErrorActiveScriptNotSupported:
        szMsg = _T("ActiveScript not supported yet!");
        break;
    case MozAxErrorControlIsNotSafeForScripting:
        {
            USES_CONVERSION;
            LPOLESTR szClsid;
            StringFromCLSID(clsid, &szClsid);
            _sntprintf(szBuffer, kBufSize - 1,
                _T("Could not create the control %s because it is not marked safe for scripting."), OLE2T(szClsid));
            CoTaskMemFree(szClsid);
            szMsg = szBuffer;
        }
        break;
    case MozAxErrorCouldNotCreateControl:
        {
            USES_CONVERSION;
            LPOLESTR szClsid;
            StringFromCLSID(clsid, &szClsid);
            _sntprintf(szBuffer, kBufSize - 1,
                _T("Could not create the control %s. Check that it has been installed on your computer "
                   "and that this page correctly references it."), OLE2T(szClsid));
            CoTaskMemFree(szClsid);
            szMsg = szBuffer;
        }
        break;
    }
    szBuffer[kBufSize - 1] = TCHAR('\0');
    if (szMsg)
        MessageBox(NULL, szMsg, _T("ActiveX Error"), MB_OK | MB_ICONWARNING);
}

static NPError
NewScript(const char *pluginType,
          PluginInstanceData *pData,
          uint16 mode,
          int16 argc,
          char *argn[],
          char *argv[])
{
    CActiveScriptSiteInstance *pScriptSite = NULL;
    CActiveScriptSiteInstance::CreateInstance(&pScriptSite);
    // TODO support ActiveScript
    ShowError(MozAxErrorActiveScriptNotSupported, CLSID_NULL);
    return NPERR_GENERIC_ERROR;
}

static BOOL
WillHandleCLSID(const CLSID &clsid)
{
#if defined(MOZ_ACTIVEX_PLUGIN_XPCONNECT) && defined(XPC_IDISPATCH_SUPPORT)
    // Ensure the control is safe for scripting
    nsCOMPtr<nsIDispatchSupport> dispSupport = do_GetService(NS_IDISPATCH_SUPPORT_CONTRACTID);
    if (!dispSupport)
        return FALSE;
    nsCID cid;
    memcpy(&cid, &clsid, sizeof(nsCID));
    PRBool isSafe = PR_FALSE;
    PRBool classExists = PR_FALSE;
    dispSupport->IsClassSafeToHost(cid, &classExists, &isSafe);
    if (classExists && !isSafe)
        return FALSE;
    return TRUE;
#else
    if (::IsEqualCLSID(clsid, CLSID_NULL))
    {
        return FALSE;
    }

    // Check the Internet Explorer list of vulnerable controls
    CRegKey keyExplorer;
    if (keyExplorer.Open(HKEY_LOCAL_MACHINE,
        _T("SOFTWARE\\Microsoft\\Internet Explorer\\ActiveX Compatibility"), KEY_READ) == ERROR_SUCCESS)
    {
        LPOLESTR szCLSID = NULL;
        ::StringFromCLSID(clsid, &szCLSID);
        if (szCLSID)
        {
            CRegKey keyCLSID;
            USES_CONVERSION;
            if (keyCLSID.Open(keyExplorer, W2T(szCLSID), KEY_READ) == ERROR_SUCCESS)
            {
                DWORD dwType = REG_DWORD;
                DWORD dwFlags = 0;
                DWORD dwBufSize = sizeof(dwFlags);
                if (::RegQueryValueEx(keyCLSID, _T("Compatibility Flags"),
                    NULL, &dwType, (LPBYTE) &dwFlags, &dwBufSize) == ERROR_SUCCESS)
                {
                    // Flags for this reg key
                    const DWORD kKillBit = 0x00000400;
                    if (dwFlags & kKillBit)
                    {
                        ::CoTaskMemFree(szCLSID);
                        return FALSE;
                    }
                }
            }
            ::CoTaskMemFree(szCLSID);
        }
    }

    // Check if the CLSID belongs to a list that the plugin does not support
    CRegKey keyDeny;
    if (keyDeny.Open(HKEY_LOCAL_MACHINE, kControlsToDenyKey, KEY_READ) == ERROR_SUCCESS)
    {
        // Enumerate CLSIDs looking for this one
        int i = 0;
        do {
            USES_CONVERSION;
            TCHAR szCLSID[64];
            const DWORD nLength = sizeof(szCLSID) / sizeof(szCLSID[0]);
            if (::RegEnumKey(keyDeny, i++, szCLSID, nLength) != ERROR_SUCCESS)
            {
                break;
            }
            szCLSID[nLength - 1] = TCHAR('\0');
            CLSID clsidToCompare = GUID_NULL;
            if (SUCCEEDED(::CLSIDFromString(T2OLE(szCLSID), &clsidToCompare)) &&
                ::IsEqualCLSID(clsid, clsidToCompare))
            {
                return FALSE;
            }
        } while (1);
        keyDeny.Close();
    }

    // Check if the CLSID belongs to a list that the plugin only supports
    CRegKey keyAllow;
    if (keyAllow.Open(HKEY_LOCAL_MACHINE, kControlsToAllowKey, KEY_READ) == ERROR_SUCCESS)
    {
        // Enumerate CLSIDs looking for this one
        int i = 0;
        do {
            USES_CONVERSION;
            TCHAR szCLSID[64];
            const DWORD nLength = sizeof(szCLSID) / sizeof(szCLSID[0]);
            if (::RegEnumKey(keyAllow, i, szCLSID, nLength) != ERROR_SUCCESS)
            {
                // An empty list means all controls are allowed.
                return (i == 0) ? TRUE : FALSE;
            }
            ++i;
            szCLSID[nLength - 1] = TCHAR('\0');
            CLSID clsidToCompare = GUID_NULL;
            if (SUCCEEDED(::CLSIDFromString(T2OLE(szCLSID), &clsidToCompare)) &&
                ::IsEqualCLSID(clsid, clsidToCompare))
            {
                return TRUE;
            }
        } while (1);
    }

    return TRUE;
#endif
}

static NPError
CreateControl(const CLSID &clsid, PluginInstanceData *pData, PropertyList &pl, LPCOLESTR szCodebase)
{
    // Make sure we got a CLSID we can handle
    if (!WillHandleCLSID(clsid))
    {
        return NPERR_GENERIC_ERROR;
    }

    pData->clsid = clsid;

    // Set flags to specify if it is allowed to host safe or unsafe controls
    // and download them.
    PRBool hostSafeControlsOnly;
    PRBool downloadControlsIfMissing;
#if defined(MOZ_ACTIVEX_PLUGIN_XPCONNECT) && defined(XPC_IDISPATCH_SUPPORT)
    PRUint32 hostingFlags = MozAxPlugin::PrefGetHostingFlags();
    if (hostingFlags & nsIActiveXSecurityPolicy::HOSTING_FLAGS_HOST_SAFE_OBJECTS &&
        !(hostingFlags & nsIActiveXSecurityPolicy::HOSTING_FLAGS_HOST_ALL_OBJECTS))
    {
        hostSafeControlsOnly = TRUE;
    }
    else if (hostingFlags & nsIActiveXSecurityPolicy::HOSTING_FLAGS_HOST_ALL_OBJECTS)
    {
        hostSafeControlsOnly = FALSE;
    }
    else
    {
        // Plugin can host neither safe nor unsafe controls, so just return
        // without creating anything.
        return NPERR_GENERIC_ERROR;
    }
    if (hostingFlags & nsIActiveXSecurityPolicy::HOSTING_FLAGS_DOWNLOAD_CONTROLS)
    {
        downloadControlsIfMissing = PR_TRUE;
    }
    else
    {
        downloadControlsIfMissing = PR_FALSE;
    }
    // Ensure we can obtain the nsIDispatchSupport service
    nsCOMPtr<nsIDispatchSupport> dispSupport = do_GetService(NS_IDISPATCH_SUPPORT_CONTRACTID);
    if (!dispSupport)
    {
        return NPERR_GENERIC_ERROR;
    }
    // Now test if the CLSID is safe for scripting
    PRBool classIsMarkedSafeForScripting = PR_FALSE;
    if (hostSafeControlsOnly)
    {
        PRBool classExists = PR_FALSE;
        PRBool isClassSafeForScripting = PR_FALSE;
        nsCID cid;
        memcpy(&cid, &clsid, sizeof(cid));
        if (NS_SUCCEEDED(dispSupport->IsClassMarkedSafeForScripting(cid, &classExists, &isClassSafeForScripting)) &&
            classExists && isClassSafeForScripting)
        {
            classIsMarkedSafeForScripting = PR_TRUE;
        }
    }
#else
    hostSafeControlsOnly = kHostSafeControlsOnly;
    downloadControlsIfMissing = kDownloadControlsIfMissing;
#endif

    // Create the control site
    CControlSiteInstance *pSite = NULL;
    CControlSiteInstance::CreateInstance(&pSite);
    if (pSite == NULL)
    {
        return NPERR_GENERIC_ERROR;
    }

    pSite->m_bSupportWindowlessActivation = FALSE;
#if defined(MOZ_ACTIVEX_PLUGIN_XPCONNECT) && defined(XPC_IDISPATCH_SUPPORT)
    // We handle our own security further down
    pSite->SetSecurityPolicy(NULL);
    pSite->m_bSafeForScriptingObjectsOnly = FALSE;
#else
    pSite->m_bSafeForScriptingObjectsOnly = hostSafeControlsOnly;
#endif

    pSite->AddRef();

#ifdef MOZ_ACTIVEX_PLUGIN_XPCONNECT
    // Set up the service provider and container so the control can get hold
    // of the IE DOM and other interfaces
    CComPtr<IServiceProvider> sp;
    MozAxPlugin::GetServiceProvider(pData, &sp);
    if (sp)
        pSite->SetServiceProvider(sp);
    CComQIPtr<IOleContainer> container  = sp;
    if (container)
        pSite->SetContainer(container);
#endif

    // TODO check the object is installed and at least as recent as
    //      that specified in szCodebase

    // Create the object
    HRESULT hr;
    if (!downloadControlsIfMissing || !szCodebase)
    {
        hr = pSite->Create(clsid, pl);
    }
    else if (szCodebase)
    {
        CComObject<CInstallControlProgress> *pProgress = NULL;
        CComPtr<IBindCtx> spBindCtx;
        CComPtr<IBindStatusCallback> spOldBSC;
        CComObject<CInstallControlProgress>::CreateInstance(&pProgress);
        pProgress->AddRef();
        CreateBindCtx(0, &spBindCtx);
        RegisterBindStatusCallback(spBindCtx, dynamic_cast<IBindStatusCallback *>(pProgress), &spOldBSC, 0);

        hr = pSite->Create(clsid, pl, szCodebase, spBindCtx);
        if (hr == MK_S_ASYNCHRONOUS)
        {
            pProgress->mNPP = pData->pPluginInstance;
            pProgress->mBindingInProgress = TRUE;
            pProgress->mResult = E_FAIL;

            // Spin around waiting for binding to complete
            HANDLE hFakeEvent = ::CreateEvent(NULL, TRUE, FALSE, NULL);
            while (pProgress->mBindingInProgress)
            {
                MSG msg;
                // Process pending messages
                while (::PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
                {
                    if (!::GetMessage(&msg, NULL, 0, 0))
                    {
                        pProgress->mBindingInProgress = FALSE;
                        break;
                    }
                    ::TranslateMessage(&msg);
                    ::DispatchMessage(&msg);
                }
                if (!pProgress->mBindingInProgress)
                    break;
                // Sleep for a bit or the next msg to appear
                ::MsgWaitForMultipleObjects(1, &hFakeEvent, FALSE, 500, QS_ALLEVENTS);
            }
            ::CloseHandle(hFakeEvent);

            hr = pProgress->mResult;
            if (SUCCEEDED(hr))
            {
                hr = pSite->Create(clsid, pl);
            }
        }
        if (pProgress)
        {
            RevokeBindStatusCallback(spBindCtx, dynamic_cast<IBindStatusCallback *>(pProgress));
            pProgress->Release();
        }
    }
    if (FAILED(hr))
    {
        ShowError(MozAxErrorCouldNotCreateControl, clsid);
    }
#if defined(MOZ_ACTIVEX_PLUGIN_XPCONNECT) && defined(XPC_IDISPATCH_SUPPORT)
    if (SUCCEEDED(hr) && hostSafeControlsOnly && !classIsMarkedSafeForScripting)
    {
        CComPtr<IUnknown> cpUnk;
        pSite->GetControlUnknown(&cpUnk);
        nsIID iidDispatch;
        memcpy(&iidDispatch, &__uuidof(IDispatch), sizeof(nsIID));
        PRBool isObjectSafe = PR_FALSE;
        if (!cpUnk ||
            NS_FAILED(dispSupport->IsObjectSafeForScripting(
                reinterpret_cast<void *>(cpUnk.p), iidDispatch, &isObjectSafe)) ||
            !isObjectSafe)
        {
            pSite->Detach();
            hr = E_FAIL;
            ShowError(MozAxErrorControlIsNotSafeForScripting, clsid);
            // DROP THROUGH
        }
    }
#endif

    // Clean up if the control could not be created
    if (FAILED(hr))
    {
        pSite->Release();
        return NPERR_GENERIC_ERROR;
    }
    
#if defined(MOZ_ACTIVEX_PLUGIN_XPCONNECT) && defined(XPC_IDISPATCH_SUPPORT)
    // Hook up the event sink
    nsEventSinkInstance *pSink = NULL;
    nsEventSinkInstance::CreateInstance(&pSink);
    if (pSink)
    {
        pSink->AddRef();
        pSink->mPlugin = pData;
        CComPtr<IUnknown> control;
        pSite->GetControlUnknown(&control);
        pSink->SubscribeToEvents(control);
    }
    pData->pControlEventSink = pSink;
#endif
    pData->nType = itControl;
    pData->pControlSite = pSite;

    return NPERR_NO_ERROR;
}

static NPError
NewControl(const char *pluginType,
           PluginInstanceData *pData,
           uint16 mode,
           int16 argc,
           char *argn[],
           char *argv[])
{
    // Read the parameters
    CLSID clsid = CLSID_NULL;
    CComBSTR codebase;
    PropertyList pl;

    if (strcmp(pluginType, MIME_OLEOBJECT1) != 0 &&
        strcmp(pluginType, MIME_OLEOBJECT2) != 0)
    {
        clsid = MozAxPlugin::GetCLSIDForType(pluginType);
    }

    for (int16 i = 0; i < argc; i++)
    {
        if (stricmp(argn[i], "CLSID") == 0 ||
            stricmp(argn[i], "CLASSID") == 0)
        {
            // Accept CLSIDs specified in various ways
            // e.g:
            //   "CLSID:C16DF970-D1BA-11d2-A252-000000000000"
            //   "C16DF970-D1BA-11d2-A252-000000000000"
            //   "{C16DF970-D1BA-11d2-A252-000000000000}"
            //
            // The first example is the proper way

            const kCLSIDLen = 256;
            char szCLSID[kCLSIDLen];
            if (strlen(argv[i]) < sizeof(szCLSID))
            {
                if (strnicmp(argv[i], "CLSID:", 6) == 0)
                {
                    _snprintf(szCLSID, kCLSIDLen - 1, "{%s}", argv[i]+6);
                }
                else if(argv[i][0] != '{')
                {
                    _snprintf(szCLSID, kCLSIDLen - 1, "{%s}", argv[i]);
                }
                else
                {
                    strncpy(szCLSID, argv[i], kCLSIDLen - 1);
                }
                szCLSID[kCLSIDLen - 1] = '\0';
                USES_CONVERSION;
                CLSIDFromString(A2OLE(szCLSID), &clsid);
            }
        }
        else if (stricmp(argn[i], "CODEBASE") == 0)
        {
            codebase = argv[i];

#ifdef XPC_IDISPATCH_SUPPORT
            // resolve relative URLs on CODEBASE
            if (argv[i])
            {
                nsCOMPtr<nsIDOMElement> element;
                NPN_GetValue(pData->pPluginInstance, NPNVDOMElement, 
                             NS_STATIC_CAST(nsIDOMElement **, getter_AddRefs(element)));
                if (element)
                {
                    nsCOMPtr<nsIDOMNode> tagAsNode (do_QueryInterface(element));
                    if (tagAsNode)
                    {
                        nsCOMPtr<nsIDOMDocument> DOMdocument;
                        tagAsNode->GetOwnerDocument(getter_AddRefs(DOMdocument));
                        // XXX nsIDocument is not frozen!!!
                        nsCOMPtr<nsIDocument> doc(do_QueryInterface(DOMdocument));
                        if (doc)
                        {
                            nsCOMPtr<nsIURI> baseURI;
                            doc->GetBaseURL(*getter_AddRefs(baseURI));
                            if (baseURI)
                            {
                                nsCAutoString newURL;
                                if (NS_SUCCEEDED(baseURI->Resolve(nsDependentCString(argv[i]), newURL)))
                                {
                                    codebase = newURL.get();
                                }
                            }
                        }
                    }
                }
            }
#endif

        }
        else 
        {
            CComBSTR paramName;
            if (strnicmp(argn[i], "PARAM_", 6) == 0)
            {
                paramName = argn[i] + 6;
            }
            else if (stricmp(argn[i], "PARAM") == 0)
            {
                // The next argn and argv values after this symbol
                // will be <param> tag names and values
                continue;
            }
            else
            {
                paramName = argn[i];
            }

            // Empty parameters are ignored
            if (!paramName.m_str || paramName.Length() == 0)
            {
                continue;
            }

            USES_CONVERSION;
            CComBSTR paramValue(A2W(argv[i]));

            // Check for existing params with the same name
            BOOL bFound = FALSE;
            for (unsigned long i = 0; i < pl.GetSize(); i++)
            {
                if (wcscmp(pl.GetNameOf(i), (BSTR) paramName) == 0)
                {
                    bFound = TRUE;
                    break;
                }
            }
            // If the parameter already exists, don't add it to the
            // list again.
            if (bFound)
            {
                continue;
            }

            CComVariant vsValue(paramValue);
            CComVariant vIValue; // Value converted to int
            CComVariant vRValue; // Value converted to real
            CComVariant vBValue; // Value converted to bool
            CComVariant &vValue = vsValue;

            // See if the variant can be converted to a number or boolean
            if (VariantChangeType(&vIValue, &vsValue, 0, VT_I4) == S_OK)
            {
                vValue = vIValue;
            }
            else if (VariantChangeType(&vRValue, &vsValue, 0, VT_R8) == S_OK)
            {
                vValue = vRValue;
            }
            else if (VariantChangeType(&vBValue, &vsValue, 0, VT_BOOL) == S_OK)
            {
                vValue = vBValue;
            }

            // Add named parameter to list
            pl.AddNamedProperty(paramName, vValue);
        }
    }

    return CreateControl(clsid, pData, pl, codebase.m_str);
}


// NPP_New
//
//    create a new plugin instance 
//    handle any instance specific code initialization here
//
NPError NP_LOADDS NPP_New(NPMIMEType pluginType,
                NPP instance,
                uint16 mode,
                int16 argc,
                char* argn[],
                char* argv[],
                NPSavedData* saved)
{
    NG_TRACE_METHOD(NPP_New);

    // trap duff args
    if (instance == NULL)
    {
        return NPERR_INVALID_INSTANCE_ERROR;
    }

    PluginInstanceData *pData = new PluginInstanceData;
    if (pData == NULL)
    {
        return NPERR_GENERIC_ERROR;
    }
    pData->pPluginInstance = instance;
    pData->szUrl = NULL;
    pData->szContentType = (pluginType) ? strdup(pluginType) : NULL;
#ifdef MOZ_ACTIVEX_PLUGIN_XPCONNECT
    pData->pScriptingPeer = NULL;
#endif

    // Create a plugin according to the mime type
#ifdef MOZ_ACTIVEX_PLUGIN_XPCONNECT
    MozAxPlugin::AddRef();
#endif

    NPError rv = NPERR_GENERIC_ERROR;
    if (strcmp(pluginType, MIME_ACTIVESCRIPT) == 0)
    {
        rv = NewScript(pluginType, pData, mode, argc, argn, argv);
    }
    else /* if (strcmp(pluginType, MIME_OLEOBJECT1) == 0 ||
             strcmp(pluginType, MIME_OLEOBJECT2) == 0) */
    {
        rv = NewControl(pluginType, pData, mode, argc, argn, argv);
    }

    // Test if plugin creation has succeeded and cleanup if it hasn't
    if (rv != NPERR_NO_ERROR)
    {
        if (pData->szContentType)
            free(pData->szContentType);
        if (pData->szUrl)
            free(pData->szUrl);
        delete pData;
#ifdef MOZ_ACTIVEX_PLUGIN_XPCONNECT
        MozAxPlugin::Release();
#endif
        return rv;
    }

    instance->pdata = pData;

    return NPERR_NO_ERROR;
}


// NPP_Destroy
//
//    Deletes a plug-in instance and releases all of its resources.
//
NPError NP_LOADDS
NPP_Destroy(NPP instance, NPSavedData** save)
{
    NG_TRACE_METHOD(NPP_Destroy);

    PluginInstanceData *pData = (PluginInstanceData *) instance->pdata;
    if (pData == NULL)
    {
        return NPERR_INVALID_INSTANCE_ERROR;
    }

    if (pData->nType == itControl)
    {
        // Destroy the site
        CControlSiteInstance *pSite = pData->pControlSite;
        if (pSite)
        {
            pSite->Detach();
            pSite->Release();
        }
#if defined(MOZ_ACTIVEX_PLUGIN_XPCONNECT) && defined(XPC_IDISPATCH_SUPPORT)
        if (pData->pControlEventSink)
        {
            pData->pControlEventSink->UnsubscribeFromEvents();
            pData->pControlEventSink->Release();
        }
#endif
#ifdef MOZ_ACTIVEX_PLUGIN_XPCONNECT
        if (pData->pScriptingPeer)
        {
            pData->pScriptingPeer->Release();
        }
#endif
    }
    else if (pData->nType == itScript)
    {
        // TODO
    }

    if (pData->szUrl)
        free(pData->szUrl);
    if (pData->szContentType)
        free(pData->szContentType);
    delete pData;
#ifdef MOZ_ACTIVEX_PLUGIN_XPCONNECT
    MozAxPlugin::Release();
#endif

    instance->pdata = 0;

    return NPERR_NO_ERROR;

}


// NPP_SetWindow
//
//    Associates a platform specific window handle with a plug-in instance.
//        Called multiple times while, e.g., scrolling.  Can be called for three
//        reasons:
//
//            1.  A new window has been created
//            2.  A window has been moved or resized
//            3.  A window has been destroyed
//
//    There is also the degenerate case;  that it was called spuriously, and
//  the window handle and or coords may have or have not changed, or
//  the window handle and or coords may be ZERO.  State information
//  must be maintained by the plug-in to correctly handle the degenerate
//  case.
//
NPError NP_LOADDS
NPP_SetWindow(NPP instance, NPWindow* window)
{
    NG_TRACE_METHOD(NPP_SetWindow);

    // Reject silly parameters
    if (!window)
    {
        return NPERR_GENERIC_ERROR;
    }

    PluginInstanceData *pData = (PluginInstanceData *) instance->pdata;
    if (pData == NULL)
    {
        return  NPERR_INVALID_INSTANCE_ERROR;
    }

    if (pData->nType == itControl)
    {
        CControlSiteInstance *pSite = pData->pControlSite;
        if (pSite == NULL)
        {
            return NPERR_GENERIC_ERROR;
        }

        HWND hwndParent = (HWND) window->window;
        if (hwndParent)
        {
            RECT rcPos;
            GetClientRect(hwndParent, &rcPos);

            if (pSite->GetParentWindow() == NULL)
            {
                pSite->Attach(hwndParent, rcPos, NULL);
            }
            else
            {
                pSite->SetPosition(rcPos);
            }
        }
    }

    return NPERR_NO_ERROR;
}


// NPP_NewStream
//
//    Notifies the plugin of a new data stream.
//  The data type of the stream (a MIME name) is provided.
//  The stream object indicates whether it is seekable.
//  The plugin specifies how it wants to handle the stream.
//
//  In this case, I set the streamtype to be NPAsFile.  This tells the Navigator
//  that the plugin doesn't handle streaming and can only deal with the object as
//  a complete disk file.  It will still call the write functions but it will also
//  pass the filename of the cached file in a later NPE_StreamAsFile call when it
//  is done transfering the file.
//
//  If a plugin handles the data in a streaming manner, it should set streamtype to
//  NPNormal  (e.g. *streamtype = NPNormal)...the NPE_StreamAsFile function will
//  never be called in this case
//
NPError NP_LOADDS
NPP_NewStream(NPP instance,
              NPMIMEType type,
              NPStream *stream, 
              NPBool seekable,
              uint16 *stype)
{
    NG_TRACE_METHOD(NPP_NewStream);

    if(!instance)
    {
        return NPERR_INVALID_INSTANCE_ERROR;
    }

    // save the plugin instance object in the stream instance
    stream->pdata = instance->pdata;
    *stype = NP_ASFILE;
    return NPERR_NO_ERROR;
}


// NPP_StreamAsFile
//
//    The stream is done transferring and here is a pointer to the file in the cache
//    This function is only called if the streamtype was set to NPAsFile.
//
void NP_LOADDS
NPP_StreamAsFile(NPP instance, NPStream *stream, const char* fname)
{
    NG_TRACE_METHOD(NPP_StreamAsFile);

    if(fname == NULL || fname[0] == NULL)
    {
        return;
    }
}


//
//        These next 2 functions are really only directly relevant 
//        in a plug-in which handles the data in a streaming manner.  
//        For a NPAsFile stream, they are still called but can safely 
//        be ignored.
//
//        In a streaming plugin, all data handling would take place here...
//
////\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//.
//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\.

int32 STREAMBUFSIZE = 0X0FFFFFFF;   // we are reading from a file in NPAsFile mode
                                    // so we can take any size stream in our write
                                    // call (since we ignore it)
                                

// NPP_WriteReady
//
//    The number of bytes that a plug-in is willing to accept in a subsequent
//    NPO_Write call.
//
int32 NP_LOADDS
NPP_WriteReady(NPP instance, NPStream *stream)
{
    return STREAMBUFSIZE;  
}


// NPP_Write
//
//    Provides len bytes of data.
//
int32 NP_LOADDS
NPP_Write(NPP instance, NPStream *stream, int32 offset, int32 len, void *buffer)
{   
    return len;
}


// NPP_DestroyStream
//
//    Closes a stream object.  
//    reason indicates why the stream was closed.  Possible reasons are
//    that it was complete, because there was some error, or because 
//    the user aborted it.
//
NPError NP_LOADDS
NPP_DestroyStream(NPP instance, NPStream *stream, NPError reason)
{
    // because I am handling the stream as a file, I don't do anything here...
    // If I was streaming, I would know that I was done and do anything appropriate
    // to the end of the stream...   
    return NPERR_NO_ERROR;
}


// NPP_Print
//
//    Printing the plugin (to be continued...)
//
void NP_LOADDS
NPP_Print(NPP instance, NPPrint* printInfo)
{
    if(printInfo == NULL)   // trap invalid parm
    {
        return;
    }

//    if (instance != NULL) {
//        CPluginWindow* pluginData = (CPluginWindow*) instance->pdata;
//        pluginData->Print(printInfo);
//    }
}

/*******************************************************************************
// NPP_URLNotify:
// Notifies the instance of the completion of a URL request. 
// 
// NPP_URLNotify is called when Netscape completes a NPN_GetURLNotify or
// NPN_PostURLNotify request, to inform the plug-in that the request,
// identified by url, has completed for the reason specified by reason. The most
// common reason code is NPRES_DONE, indicating simply that the request
// completed normally. Other possible reason codes are NPRES_USER_BREAK,
// indicating that the request was halted due to a user action (for example,
// clicking the "Stop" button), and NPRES_NETWORK_ERR, indicating that the
// request could not be completed (for example, because the URL could not be
// found). The complete list of reason codes is found in npapi.h. 
// 
// The parameter notifyData is the same plug-in-private value passed as an
// argument to the corresponding NPN_GetURLNotify or NPN_PostURLNotify
// call, and can be used by your plug-in to uniquely identify the request. 
 ******************************************************************************/

void
NPP_URLNotify(NPP instance, const char* url, NPReason reason, void* notifyData)
{
    PluginInstanceData *pData = (PluginInstanceData *) instance->pdata;
    if (pData)
    {
        if (pData->szUrl)
            free(pData->szUrl);
        pData->szUrl = strdup(url);
    }
}

NPError	NP_LOADDS
NPP_GetValue(NPP instance, NPPVariable variable, void *value)
{
    NPError rv = NPERR_GENERIC_ERROR;
#ifdef MOZ_ACTIVEX_PLUGIN_XPCONNECT
    rv = MozAxPlugin::GetValue(instance, variable, value);
#endif
    return rv;
}

NPError	NP_LOADDS
NPP_SetValue(NPP instance, NPNVariable variable, void *value)
{
    return NPERR_GENERIC_ERROR;
}
