//+-------------------------------------------------------------------------
//
//  TaskMan - NT TaskManager
//  Copyright (C) Microsoft
//
//  File:       shundoc.cpp
//
//  History:    Oct-11-24   aubymori  Created
//              Jan-22-25   kfh83     Modified
//
//--------------------------------------------------------------------------
#include "shundoc.h"
#include "Windows.h"

#include "docobj.h"

#include "synchapi.h"

#include "shlobj.h"

#include "Shlwapi.h"

#include "msi.h"
#include "port32.h"

#include "path.h"



//
// Classes
//


class CDarwinAd
{
public:
    LPITEMIDLIST    _pidl;
    LPTSTR          _pszDescriptor;
    LPTSTR          _pszLocalPath;
    INSTALLSTATE    _state;

    CDarwinAd(LPITEMIDLIST pidl, LPTSTR psz)
    {
        // I take ownership of this pidl
        _pidl = pidl;
        Str_SetPtrW(&_pszDescriptor, psz);
    }

    void CheckInstalled()
    {
        TCHAR szProduct[38];
        TCHAR szFeature[38];
        TCHAR szComponent[38];

        if (MsiDecomposeDescriptorW(_pszDescriptor, szProduct, szFeature, szComponent, NULL) == ERROR_SUCCESS)
        {
            _state = MsiQueryFeatureState(szProduct, szFeature);
        }
        else
        {
            _state = (INSTALLSTATE)-2;
        }

        // Note: Cannot use ParseDarwinID since that bumps the usage count
        // for the app and we're not running the app, just looking at it.
        // Also because ParseDarwinID tries to install the app (eek!)
        //
        // Must ignore INSTALLSTATE_SOURCE because MsiGetComponentPath will
        // try to install the app even though we're just querying...
        TCHAR szCommand[MAX_PATH];
        DWORD cch = ARRAYSIZE(szCommand);

        if (_state == 3 &&
            MsiGetComponentPath(szProduct, szComponent, szCommand, &cch) == _state)
        {
            PathUnquoteSpaces(szCommand);
            Str_SetPtrW(&_pszLocalPath, szCommand);
        }
        else
        {
            Str_SetPtrW(&_pszLocalPath, NULL);
        }
    }

    BOOL IsAd()
    {
        return _state == 1;
    }

    ~CDarwinAd()
    {
        ILFree(_pidl);
        Str_SetPtrW(&_pszDescriptor, NULL);
        Str_SetPtrW(&_pszLocalPath, NULL);
    }
};

//
// Function definitions
// 

HRESULT(STDMETHODCALLTYPE* IUnknown_Exec)(IUnknown* punk, const GUID* pguidCmdGroup, DWORD nCmdID, DWORD nCmdexecopt, VARIANTARG* pvarargIn, VARIANTARG* pvarargOut) = nullptr;
HRESULT(STDMETHODCALLTYPE* IUnknown_GetClassID)(IUnknown* punk, CLSID* pclsid) = nullptr;
HRESULT(STDMETHODCALLTYPE* IUnknown_OnFocusChangeIS)(IUnknown* punk, IUnknown* punkSrc, BOOL fSetFocus) = nullptr;
HRESULT(STDMETHODCALLTYPE* IUnknown_QueryStatus)(IUnknown* punk, const GUID* pguidCmdGroup, ULONG cCmds, OLECMD rgCmds[], OLECMDTEXT* pcmdtext) = nullptr;
HRESULT(STDMETHODCALLTYPE* IUnknown_UIActivateIO)(IUnknown* punk, BOOL fActivate, LPMSG lpMsg) = nullptr;
HRESULT(STDMETHODCALLTYPE* IUnknown_TranslateAcceleratorIO)(IUnknown* punk, LPMSG lpMsg) = nullptr;


STDAPI IUnknown_DragEnter(IUnknown* punk, IDataObject* pdtobj, DWORD grfKeyState, POINTL pt, DWORD* pdwEffect)
{
    HRESULT hr = E_FAIL;
    if (punk)
    {
        IDropTarget* pdt;
        hr = punk->QueryInterface(IID_PPV_ARGS(&pdt));
        if (SUCCEEDED(hr))
        {
            hr = pdt->DragEnter(pdtobj, grfKeyState, pt, pdwEffect);
            pdt->Release();
        }
    }

    if (FAILED(hr))
        *pdwEffect = DROPEFFECT_NONE;

    return hr;
}

STDAPI IUnknown_DragLeave(IUnknown* punk)
{
    HRESULT hr = E_FAIL;
    if (punk)
    {
        IDropTarget* pdt;
        hr = punk->QueryInterface(IID_PPV_ARGS(&pdt));
        if (SUCCEEDED(hr))
        {
            hr = pdt->DragLeave();
            pdt->Release();
        }
    }
    return hr;
}

STDAPI IUnknown_DragOver(IUnknown* punk, DWORD grfKeyState, POINTL pt, DWORD* pdwEffect)
{
    HRESULT hr = E_FAIL;
    if (punk)
    {
        IDropTarget* pdt;
        hr = punk->QueryInterface(IID_PPV_ARGS(&pdt));
        if (SUCCEEDED(hr))
        {
            hr = pdt->DragOver(grfKeyState, pt, pdwEffect);
            pdt->Release();
        }
    }

    if (FAILED(hr))
        *pdwEffect = DROPEFFECT_NONE;

    return hr;
}


HRESULT(STDMETHODCALLTYPE* SHPropagateMessage)(HWND hwndParent, UINT uMsg, WPARAM wParam, LPARAM lParam, int iFlags) = nullptr;
HRESULT(STDMETHODCALLTYPE* SHGetUserDisplayName)(LPWSTR pszDisplayName, PULONG uLen) = nullptr;
HRESULT(STDMETHODCALLTYPE* SHGetUserPicturePath)(LPCWSTR pszUsername, DWORD dwFlags, LPWSTR pszPath, DWORD cchPathMax) = nullptr;
HRESULT(STDMETHODCALLTYPE* SHSetWindowBits)(HWND hwnd, int iWhich, DWORD dwBits, DWORD dwValue) = nullptr;
HRESULT(STDMETHODCALLTYPE* SHRunIndirectRegClientCommand)(HWND hwnd, LPCWSTR pszClient) = nullptr;
HRESULT(STDMETHODCALLTYPE* SHInvokeDefaultCommand)(HWND hwnd, IShellFolder* psf, LPCITEMIDLIST pidlItem) = nullptr;
HRESULT(STDMETHODCALLTYPE* SHSettingsChanged)(WPARAM wParam, LPARAM lParam) = nullptr;
HRESULT(STDMETHODCALLTYPE* SHIsChildOrSelf)(HWND hwndParent, HWND hwnd) = nullptr;
HRESULT(STDMETHODCALLTYPE* SHLoadRegUIStringW)(HKEY     hkey, LPCWSTR  pszValue, LPWSTR   pszOutBuf, UINT     cchOutBuf) = nullptr;
BOOL(WINAPI* SHQueueUserWorkItem)(IN LPTHREAD_START_ROUTINE pfnCallback, IN LPVOID pContext, IN LONG lPriority, IN DWORD_PTR dwTag, OUT DWORD_PTR* pdwId OPTIONAL, IN LPCSTR pszModule OPTIONAL, IN DWORD dwFlags) = nullptr;
BOOL(WINAPI* WinStationSetInformationW)(HANDLE hServer, ULONG LogonId, WINSTATIONINFOCLASS WinStationInformationClass, PVOID  pWinStationInformation, ULONG WinStationInformationLength) = nullptr;
BOOL(WINAPI* WinStationUnRegisterConsoleNotification)(HANDLE hServer, HWND hWnd) = nullptr;
BOOL(STDMETHODCALLTYPE* SHFindComputer)(LPCITEMIDLIST pidlFolder, LPCITEMIDLIST pidlSaveFile) = nullptr;
LRESULT(WINAPI* SHDefWindowProc)(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) = nullptr;
UINT(WINAPI* MsiDecomposeDescriptorW)(LPCWSTR	szDescriptor, LPWSTR szProductCode, LPWSTR szFeatureId, LPWSTR szComponentCode, DWORD* pcchArgsOffset) = nullptr;
HRESULT(STDMETHODCALLTYPE* ExitWindowsDialog)(HWND hwndParent) = nullptr;
UINT(STDMETHODCALLTYPE* SHGetCurColorRes)(void) = nullptr;
INT(STDMETHODCALLTYPE* SHMessageBoxCheckExW)(HWND hwnd, HINSTANCE hinst, LPCWSTR pszTemplateName, DLGPROC pDlgProc, LPVOID pData, int iDefault, LPCWSTR pszRegVal) = nullptr;
INT(STDMETHODCALLTYPE* RunFileDlg)(HWND hwndParent, HICON hIcon, LPCTSTR pszWorkingDir, LPCTSTR pszTitle, LPCTSTR pszPrompt, DWORD dwFlags) = nullptr;
VOID(STDMETHODCALLTYPE* SHUpdateRecycleBinIcon)() = nullptr;
VOID(STDMETHODCALLTYPE* LogoffWindowsDialog)(HWND hwndParent) = nullptr;
VOID(STDMETHODCALLTYPE* DisconnectWindowsDialog)(HWND hwndParent) = nullptr;
//BOOL(STDMETHODCALLTYPE* RegisterShellHook)(HWND hwnd, BOOL fInstall) = nullptr;
DWORD_PTR(WINAPI* SHGetMachineInfo)(UINT gmi) = nullptr;

COLORREF(STDMETHODCALLTYPE* SHFillRectClr)(HDC hdc, LPRECT lprect, COLORREF color) = nullptr;

extern BOOL(STDMETHODCALLTYPE* WinStationRegisterConsoleNotification)(HANDLE  hServer, HWND    hWnd, DWORD   dwFlags) = nullptr;

// SHRegisterDarwinLink takes ownership of the pidl

// for g_hdpaDarwinAds
EXTERN_C CRITICAL_SECTION g_csDarwinAds = { 0 };
HDPA g_hdpaDarwinAds = NULL;

#define ENTERCRITICAL_DARWINADS EnterCriticalSection(&g_csDarwinAds)
#define LEAVECRITICAL_DARWINADS LeaveCriticalSection(&g_csDarwinAds)


int GetDarwinIndex(LPCITEMIDLIST pidlFull, CDarwinAd** ppda)
{
    int iRet = -1;
    if (g_hdpaDarwinAds)
    {
        int chdpa = DPA_GetPtrCount(g_hdpaDarwinAds);
        for (int ihdpa = 0; ihdpa < chdpa; ihdpa++)
        {
            *ppda = (CDarwinAd*)DPA_FastGetPtr(g_hdpaDarwinAds, ihdpa);
            if (*ppda)
            {
                if (ILIsEqual((*ppda)->_pidl, pidlFull))
                {
                    iRet = ihdpa;
                    break;
                }
            }
        }
    }
    return iRet;
}

STDMETHODIMP_(int) s_DarwinAdsDestroyCallback(LPVOID pData1, LPVOID pData2)
{
    CDarwinAd* pda = (CDarwinAd*)pData1;
    if (pda)
        delete pda;
    return TRUE;
}

BOOL SHRegisterDarwinLink(LPITEMIDLIST pidlFull, LPWSTR pszDarwinID, BOOL fUpdate)
{
    BOOL fRetVal = FALSE;

    ENTERCRITICAL_DARWINADS;

    if (pidlFull)
    {
        CDarwinAd* pda = NULL;

        if (GetDarwinIndex(pidlFull, &pda) != -1 && pda)
        {
            // We already know about this link; don't need to add it
            fRetVal = TRUE;
        }
        else
        {
            pda = new CDarwinAd(pidlFull, pszDarwinID);
            if (pda)
            {
                pidlFull = NULL;    // take ownership

                // Do we have a global cache?
                if (g_hdpaDarwinAds == NULL)
                {
                    // No; This is either the first time this is called, or we
                    // failed the last time.
                    g_hdpaDarwinAds = DPA_Create(5);
                }

                if (g_hdpaDarwinAds)
                {
                    // DPA_AppendPtr returns the zero based index it inserted it at.
                    if (DPA_AppendPtr(g_hdpaDarwinAds, (void*)pda) >= 0)
                    {
                        fRetVal = TRUE;
                    }

                }
            }
        }

        if (!fRetVal)
        {
            // if we failed to create a dpa, delete this.
            delete pda;
        }
        else if (fUpdate)
        {
            // update the entry if requested
            pda->CheckInstalled();
        }
        ILFree(pidlFull);

    }
    else if (!pszDarwinID)
    {
        // NULL, NULL means "destroy darwin info, we're shutting down"
        HDPA hdpa = g_hdpaDarwinAds;
        g_hdpaDarwinAds = NULL;
        if (hdpa)
            DPA_DestroyCallback(hdpa, s_DarwinAdsDestroyCallback, NULL);
    }

    LEAVECRITICAL_DARWINADS;

    return fRetVal;
}

STDAPI_(void) SHAdjustLOGFONT(IN OUT LOGFONT* plf)
{
    if (plf->lfCharSet == SHIFTJIS_CHARSET ||
        plf->lfCharSet == HANGEUL_CHARSET ||
        plf->lfCharSet == GB2312_CHARSET ||
        plf->lfCharSet == CHINESEBIG5_CHARSET)
    {
        if (plf->lfWeight > FW_NORMAL)
            plf->lfWeight = FW_NORMAL;
    }
}

// moved to runonce.cpp


STDAPI_(DWORD) SHProcessMessagesUntilEventEx(HWND hwnd, HANDLE hEvent, DWORD dwTimeout, DWORD dwWakeMask)
{
    DWORD dwEndTime = GetTickCount() + dwTimeout;
    LONG lWait = (LONG)dwTimeout;
    DWORD dwReturn;

    if (!hEvent && (dwTimeout == INFINITE))
    {
        return -1;
    }

    for (;;)
    {
        DWORD dwCount = hEvent ? 1 : 0;
        dwReturn = MsgWaitForMultipleObjects(dwCount, &hEvent, FALSE, lWait, dwWakeMask);

        // were we signalled or did we time out?
        if (dwReturn != (WAIT_OBJECT_0 + dwCount))
        {
            break;
        }

        // we woke up because of messages.
        MSG msg;
        while (PeekMessage(&msg, hwnd, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            if (msg.message == WM_SETCURSOR)
            {
                SetCursor(LoadCursor(NULL, IDC_WAIT));
            }
            else
            {
                DispatchMessage(&msg);
            }
        }

        // calculate new timeout value
        if (dwTimeout != INFINITE)
        {
            lWait = (LONG)dwEndTime - GetTickCount();
        }
    }

    return dwReturn;
}

// 
// Determine if the images represented by the two icons are the same
// (NOTE: this does not compare ICON masks, but this should never be a distinguishing factor)
//
STDAPI_(BOOL) SHAreIconsEqual(HICON hIcon1, HICON hIcon2)
{
    BOOL bRet = FALSE;

    ICONINFO ii1;
    if (hIcon1 && hIcon2 && GetIconInfo(hIcon1, &ii1))
    {
        ICONINFO ii2;
        if (GetIconInfo(hIcon2, &ii2))
        {
            BITMAP bm1 = { 0 };
            if (GetObject(ii1.hbmColor, sizeof(bm1), &bm1))
            {
                BITMAP bm2 = { 0 };
                if (GetObject(ii2.hbmColor, sizeof(bm2), &bm2))
                {
                    if ((bm1.bmWidth == bm2.bmWidth) && (bm1.bmHeight == bm2.bmHeight))
                    {
                        BITMAPINFO bmi = { 0 };
                        bmi.bmiHeader.biSize = sizeof(bmi.bmiHeader);
                        bmi.bmiHeader.biWidth = bm1.bmWidth;
                        bmi.bmiHeader.biHeight = bm1.bmHeight;
                        bmi.bmiHeader.biPlanes = 1;
                        bmi.bmiHeader.biBitCount = 32;
                        bmi.bmiHeader.biCompression = BI_RGB;

                        HDC hdc = GetDC(NULL);
                        if (hdc)
                        {
                            ULONG* pulIcon1 = new ULONG[bm1.bmWidth * bm1.bmHeight];
                            if (pulIcon1)
                            {
                                if (GetDIBits(hdc, ii1.hbmColor, 0, bm1.bmHeight, (LPVOID)pulIcon1, &bmi, DIB_RGB_COLORS))
                                {
                                    ULONG* pulIcon2 = new ULONG[bm1.bmWidth * bm1.bmHeight];
                                    if (pulIcon2)
                                    {
                                        if (GetDIBits(hdc, ii2.hbmColor, 0, bm1.bmHeight, (LPVOID)pulIcon2, &bmi, DIB_RGB_COLORS))
                                        {
                                            bRet = (0 == memcmp(pulIcon1, pulIcon2, bm1.bmWidth * bm1.bmHeight * sizeof(ULONG)));
                                        }
                                        delete[] pulIcon2;
                                    }
                                }
                                delete[] pulIcon1;
                            }
                            ReleaseDC(NULL, hdc);
                        }
                    }
                }
            }
            DeleteObject(ii2.hbmColor);
            DeleteObject(ii2.hbmMask);
        }
        DeleteObject(ii1.hbmColor);
        DeleteObject(ii1.hbmMask);
    }

    return bRet;
}

STDAPI_(BOOL) SetWindowZorder(HWND hwnd, HWND hwndInsertAfter)
{
    return SetWindowPos(hwnd, hwndInsertAfter, 0, 0, 0, 0,
        SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE | SWP_NOOWNERZORDER);
}

BOOL CALLBACK _FixZorderEnumProc(HWND hwnd, LPARAM lParam)
{
    HWND hwndTest = (HWND)lParam;
    HWND hwndOwner = hwnd;

    while (hwndOwner = GetWindow(hwndOwner, GW_OWNER))
    {
        if (hwndOwner == hwndTest)
        {
            SetWindowZorder(hwnd, HWND_NOTOPMOST);

            break;
        }
    }

    return TRUE;
}

STDAPI SHCoInitialize(void)
{
    HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
    if (FAILED(hr))
    {
        hr = CoInitializeEx(NULL, COINIT_MULTITHREADED | COINIT_DISABLE_OLE1DDE);
    }
    return hr;
}

STDAPI_(BOOL) SHForceWindowZorder(HWND hwnd, HWND hwndInsertAfter)
{
    BOOL fRet = SetWindowZorder(hwnd, hwndInsertAfter);

    if (fRet && hwndInsertAfter == HWND_TOPMOST)
    {
        if (!(GetWindowLong(hwnd, GWL_EXSTYLE) & WS_EX_TOPMOST))
        {
            //
            // user didn't actually move the hwnd to topmost
            //
            // According to GerardoB, this can happen if the window has
            // an owned window that somehow has become topmost while the 
            // owner remains non-topmost, i.e., the two have become
            // separated in the z-order.  In this state, when the owner
            // window tries to make itself topmost, the call will
            // silently fail.
            //
            // TERRIBLE HORRIBLE NO GOOD VERY BAD HACK
            //
            // Hacky fix is to enumerate the toplevel windows, check to see
            // if any are topmost and owned by hwnd, and if so, make them
            // non-topmost.  Then, retry the SetWindowPos call.
            //

            // Fix up the z-order
            EnumWindows(_FixZorderEnumProc, (LPARAM)hwnd);

            // Retry the set.  (This should make all owned windows topmost as well.)
            SetWindowZorder(hwnd, HWND_TOPMOST);
        }
    }

    return fRet;
}

//BOOL(WINAPI* EndTask)(HWND hWnd, BOOL fShutDown, BOOL fForce) = nullptr;

#define FILETIMEtoInt64(ft) _FILETIMEtoInt64(&(ft))

inline void SetFILETIMEfromInt64(FILETIME* pft, unsigned __int64 i64)
{
    pft->dwLowDateTime = (DWORD)i64;
    pft->dwHighDateTime = (DWORD)(i64 >> 32);
}

inline void IncrementFILETIME(FILETIME* pft, unsigned __int64 iAdjust)
{
    SetFILETIMEfromInt64(pft, _FILETIMEtoInt64(pft) + iAdjust);
}

inline void DecrementFILETIME(FILETIME* pft, unsigned __int64 iAdjust)
{
    SetFILETIMEfromInt64(pft, _FILETIMEtoInt64(pft) - iAdjust);
}

// shits not exported bro i swear


BOOL ConvertHexStringToIntW(WCHAR* pszHexNum, int* piNum)
{
    int   n = 0L;
    WCHAR* psz = pszHexNum;

    for (n = 0; ; psz = CharNextW(psz))
    {
        if ((*psz >= '0') && (*psz <= '9'))
            n = 0x10 * n + *psz - '0';
        else
        {
            WCHAR ch = *psz;
            int n2;

            if (ch >= 'a')
                ch -= 'a' - 'A';

            n2 = ch - 'A' + 0xA;
            if (n2 >= 0xA && n2 <= 0xF)
                n = 0x10 * n + n2;
            else
                break;
        }
    }

    /*
     * Update results
     */
    *piNum = n;

    return (psz != pszHexNum);
}

VOID MuSecurity(VOID)
{
    //
    // Do nothing on the console
    //

    if (SHGetMachineInfo(GMI_TSCLIENT))
    {
        WinStationSetInformationW(SERVERNAME_CURRENT,
            LOGONID_CURRENT,
            WinStationNtSecurity,
            NULL, 0);
    }
}

BOOL CALLBACK Mirror_EnumUILanguagesProc(LPTSTR lpUILanguageString, LONG_PTR lParam)
{
    int langID = 0;

    ConvertHexStringToIntW(lpUILanguageString, &langID);

    if ((LANGID)langID == ((LPMUIINSTALLLANG)lParam)->LangID)
    {
        ((LPMUIINSTALLLANG)lParam)->bInstalled = TRUE;
        return FALSE;
    }
    return TRUE;
}

BOOL Mirror_IsWindowMirroredRTL(HWND hWnd)
{
    return (GetWindowLongA(hWnd, GWL_EXSTYLE) & WS_EX_LAYOUTRTL);
}

BOOL Mirror_IsUILanguageInstalled(LANGID langId)
{
    MUIINSTALLLANG MUILangInstalled = { 0 };
    MUILangInstalled.LangID = langId;

    EnumUILanguagesW(Mirror_EnumUILanguagesProc, 0, (LONG_PTR)&MUILangInstalled);

    return MUILangInstalled.bInstalled;
}

BOOL IsBiDiLocalizedSystemEx(LANGID* pLangID)
{
    int           iLCID = 0L;
    static BOOL   bRet = (BOOL)(DWORD)-1;
    static LANGID langID = MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL);

    if (bRet != (BOOL)(DWORD)-1)
    {
        if (bRet && pLangID)
        {
            *pLangID = langID;
        }
        return bRet;
    }

    bRet = FALSE;
    /*
     * Need to use NT5 detection method (Multiligual UI ID)
     */
    langID = GetUserDefaultUILanguage();

    if (langID)
    {
        WCHAR wchLCIDFontSignature[16];
        iLCID = MAKELCID(langID, SORT_DEFAULT);

        /*
         * Let's verify this is a RTL (BiDi) locale. Since reg value is a hex string, let's
         * convert to decimal value and call GetLocaleInfo afterwards.
         * LOCALE_FONTSIGNATURE always gives back 16 WCHARs.
         */

        if (GetLocaleInfoW(iLCID,
            LOCALE_FONTSIGNATURE,
            (WCHAR*)&wchLCIDFontSignature[0],
            (sizeof(wchLCIDFontSignature) / sizeof(WCHAR))))
        {

            /* Let's verify the bits we have a BiDi UI locale */
            if ((wchLCIDFontSignature[7] & (WCHAR)0x0800) && Mirror_IsUILanguageInstalled(langID))
            {
                bRet = TRUE;
            }
        }
    }

    if (bRet && pLangID)
    {
        *pLangID = langID;
    }
    return bRet;
}

// aight we good

BOOL IsBiDiLocalizedSystem(void)
{
    return IsBiDiLocalizedSystemEx(NULL);
}

BOOL GetExplorerUserSetting(HKEY hkeyRoot, LPCTSTR pszSubKey, LPCTSTR pszValue)
{
    TCHAR szPath[MAX_PATH];
    TCHAR szPathExplorer[MAX_PATH];
    DWORD cbSize = ARRAYSIZE(szPath);
    DWORD dwType;

    PathCombine(szPathExplorer, L"Software\\Microsoft\\Windows\\CurrentVersion\\Explorer", pszSubKey);
    if (ERROR_SUCCESS == SHGetValue(hkeyRoot, szPathExplorer, pszValue,
        &dwType, szPath, &cbSize))
    {
        // Zero in the DWORD case or NULL in the string case
        // indicates that this item is not available.
        if (dwType == REG_DWORD)
            return *((DWORD*)szPath) != 0;
        else
            return (TCHAR)szPath[0] != 0;
    }

    return -1;
}

STDAPI_(BOOL) IsRestrictedOrUserSetting(HKEY hkeyRoot, RESTRICTIONS rest, LPCTSTR pszSubKey, LPCTSTR pszValue, UINT flags)
{
    // See if the system policy restriction trumps

    DWORD dwRest = SHRestricted(rest);

    if (dwRest == 1)
        return TRUE;

    if (dwRest == 2)
        return FALSE;

    //
    //  Restriction not in place or defers to user setting.
    //
    BOOL fValidKey = GetExplorerUserSetting(hkeyRoot, pszSubKey, pszValue);

    switch (fValidKey)
    {
    case 0:     // Key is present and zero
        if (flags & ROUS_KEYRESTRICTS)
            return FALSE;       // restriction not present
        else
            return TRUE;        // ROUS_KEYALLOWS, value is 0 -> restricted

    case 1:     // Key is present and nonzero

        if (flags & ROUS_KEYRESTRICTS)
            return TRUE;        // restriction present -> restricted
        else
            return FALSE;       // ROUS_KEYALLOWS, value is 1 -> not restricted

    default:
        // Fall through

    case -1:    // Key is not present
        return (flags & ROUS_DEFAULTRESTRICT);
    }

    /*NOTREACHED*/
}

// DDE shit

TCHAR const c_szCreateGroup[] = TEXT("CreateGroup");
TCHAR const c_szShowGroup[] = TEXT("ShowGroup");
TCHAR const c_szAddItem[] = TEXT("AddItem");
TCHAR const c_szExitProgman[] = TEXT("ExitProgman");
TCHAR const c_szDeleteGroup[] = TEXT("DeleteGroup");
TCHAR const c_szDeleteItem[] = TEXT("DeleteItem");
TCHAR const c_szReplaceItem[] = TEXT("ReplaceItem");
TCHAR const c_szReload[] = TEXT("Reload");
TCHAR const c_szFindFolder[] = TEXT("FindFolder");
TCHAR const c_szOpenFindFile[] = TEXT("OpenFindFile");
TCHAR const c_szTrioDataFax[] = TEXT("DDEClient");
TCHAR const c_szTalkToPlus[] = TEXT("ddeClass");
TCHAR const c_szStartUp[] = TEXT("StartUp");
TCHAR const c_szCCMail[] = TEXT("ccInsDDE");
TCHAR const c_szBodyWorks[] = TEXT("BWWFrame");
TCHAR const c_szMediaRecorder[] = TEXT("DDEClientWndClass");
TCHAR const c_szDiscis[] = TEXT("BACKSCAPE");
TCHAR const c_szMediaRecOld[] = TEXT("MediaRecorder");
TCHAR const c_szMediaRecNew[] = TEXT("Media Recorder");
TCHAR const c_szDialog[] = TEXT("#32770");
TCHAR const c_szJourneyMan[] = TEXT("Sender");
TCHAR const c_szCADDE[] = TEXT("CA_DDECLASS");
TCHAR const c_szFaxServe[] = TEXT("Install");
TCHAR const c_szMakePMG[] = TEXT("Make Program Manager Group");
TCHAR const c_szViewFolder[] = TEXT("ViewFolder");
TCHAR const c_szExploreFolder[] = TEXT("ExploreFolder");
TCHAR const c_szRUCabinet[] = TEXT("ConfirmCabinetID");
CHAR const c_szNULLA[] = "";
TCHAR const c_szGetIcon[] = TEXT("GetIcon");
TCHAR const c_szGetDescription[] = TEXT("GetDescription");
TCHAR const c_szGetWorkingDir[] = TEXT("GetWorkingDir");
TCHAR const c_szShellFile[] = TEXT("ShellFile");

BOOL DDE_CreateGroup(LPTSTR, UINT*, PDDECONV);
BOOL DDE_ShowGroup(LPTSTR, UINT*, PDDECONV);
BOOL DDE_AddItem(LPTSTR, UINT*, PDDECONV);
BOOL DDE_ExitProgman(LPTSTR, UINT*, PDDECONV);
BOOL DDE_DeleteGroup(LPTSTR, UINT*, PDDECONV);
BOOL DDE_DeleteItem(LPTSTR, UINT*, PDDECONV);
#define DDE_ReplaceItem DDE_DeleteItem
BOOL DDE_Reload(LPTSTR, UINT*, PDDECONV);
BOOL DDE_ViewFolder(LPTSTR, UINT*, PDDECONV);
BOOL DDE_ExploreFolder(LPTSTR, UINT*, PDDECONV);
BOOL DDE_FindFolder(LPTSTR, UINT*, PDDECONV);
BOOL DDE_OpenFindFile(LPTSTR, UINT*, PDDECONV);
BOOL DDE_ConfirmID(LPTSTR lpszBuf, UINT* lpwCmd, PDDECONV pddec);
BOOL DDE_ShellFile(LPTSTR lpszBuf, UINT* lpwCmd, PDDECONV pddec);
BOOL DDE_Beep(LPTSTR, UINT*, PDDECONV);

DDECOMMANDINFO const c_sDDECommands[] =
{
    { c_szCreateGroup  , DDE_CreateGroup   },
    { c_szShowGroup    , DDE_ShowGroup     },
    { c_szAddItem      , DDE_AddItem       },
    { c_szExitProgman  , DDE_ExitProgman   },
    { c_szDeleteGroup  , DDE_DeleteGroup   },
    { c_szDeleteItem   , DDE_DeleteItem    },
    { c_szReplaceItem  , DDE_ReplaceItem   },
    { c_szReload       , DDE_Reload        },
    { c_szViewFolder   , DDE_ViewFolder    },
    { c_szExploreFolder, DDE_ExploreFolder },
    { c_szFindFolder,    DDE_FindFolder    },
    { c_szOpenFindFile,  DDE_OpenFindFile  },
    { c_szRUCabinet,     DDE_ConfirmID},
    { c_szShellFile,     DDE_ShellFile},
#ifdef DEBUG
    { c_szBeep         , DDE_Beep          },
#endif
    { 0, 0 },
};

LPTSTR SkipWhite(LPTSTR lpsz)
{
    /* prevent sign extension in case of DBCS */
    while (*lpsz && (TUCHAR)*lpsz <= TEXT(' '))
        lpsz++;

    return(lpsz);
}


LPTSTR GetCommandName(LPTSTR lpCmd, const DDECOMMANDINFO* lpsCommands, UINT* lpW)
{
    TCHAR chT;
    UINT iCmd = 0;
    LPTSTR lpT;

    /* Eat any white space. */
    lpT = lpCmd = SkipWhite(lpCmd);

    /* Find the end of the token. */
    while (IsCharAlpha(*lpCmd))
        lpCmd = CharNext(lpCmd);

    /* Temporarily NULL terminate it. */
    chT = *lpCmd;
    *lpCmd = 0;

    /* Look up the token in a list of commands. */
    *lpW = (UINT)-1;
    while (lpsCommands->pszCommand)
    {
        if (!lstrcmpi(lpsCommands->pszCommand, lpT))
        {
            *lpW = iCmd;
            break;
        }
        iCmd++;
        ++lpsCommands;
    }

    *lpCmd = chT;

    return(lpCmd);
}

LPTSTR GetOneParameter(LPCTSTR lpCmdStart, LPTSTR lpCmd,
    UINT* lpW, BOOL fIncludeQuotes)
{
    LPTSTR     lpT;

    switch (*lpCmd)
    {
    case TEXT(','):
        *lpW = (UINT)(lpCmd - lpCmdStart);  // compute offset
        *lpCmd++ = 0;                /* comma: becomes a NULL string */
        break;

    case TEXT('"'):
        if (fIncludeQuotes)
        {

            // quoted string... don't trim off "
            *lpW = (UINT)(lpCmd - lpCmdStart);  // compute offset
            ++lpCmd;
            while (*lpCmd && *lpCmd != TEXT('"'))
                lpCmd = CharNext(lpCmd);
            if (!*lpCmd)
                return(NULL);
            lpT = lpCmd;
            ++lpCmd;

            goto skiptocomma;
        }
        else
        {
            // quoted string... trim off "
            ++lpCmd;
            *lpW = (UINT)(lpCmd - lpCmdStart);  // compute offset
            while (*lpCmd && *lpCmd != TEXT('"'))
                lpCmd = CharNext(lpCmd);
            if (!*lpCmd)
                return(NULL);
            *lpCmd++ = 0;
            lpCmd = SkipWhite(lpCmd);

            // If there's a comma next then skip over it, else just go on as
            // normal.
            if (*lpCmd == TEXT(','))
                lpCmd++;
        }
        break;

    case TEXT(')'):
        return(lpCmd);                /* we ought not to hit this */

    case TEXT('('):
    case TEXT('['):
    case TEXT(']'):
        return(NULL);                 /* these are illegal */

    default:
        lpT = lpCmd;
        *lpW = (UINT)(lpCmd - lpCmdStart);  // compute offset
    skiptocomma:
        while (*lpCmd && *lpCmd != TEXT(',') && *lpCmd != TEXT(')'))
        {
            /* Check for illegal characters. */
            if (*lpCmd == TEXT(']') || *lpCmd == TEXT('[') || *lpCmd == TEXT('('))
                return(NULL);

            /* Remove trailing whitespace */
            /* prevent sign extension */
            if ((TUCHAR)*lpCmd > TEXT(' '))
                lpT = lpCmd;

            lpCmd = CharNext(lpCmd);
        }

        /* Eat any trailing comma. */
        if (*lpCmd == TEXT(','))
            lpCmd++;

        /* NULL terminator after last nonblank character -- may write over
         * terminating ')' but the caller checks for that because this is
         * a hack.
         */

#ifdef UNICODE
        lpT[1] = 0;
#else
        lpT[IsDBCSLeadByte(*lpT) ? 2 : 1] = 0;
#endif
        break;
    }

    // Return next unused character.
    return(lpCmd);
}

UINT* GetDDECommands(LPTSTR lpCmd, const DDECOMMANDINFO* lpsCommands, BOOL fLFN)
{
    UINT cParm, cCmd = 0;
    UINT* lpW;
    UINT* lpRet;
    LPCTSTR lpCmdStart = lpCmd;
    BOOL fIncludeQuotes = FALSE;

    lpRet = lpW = (UINT*)GlobalAlloc(GPTR, 512L);
    if (!lpRet)
        return 0;

    while (*lpCmd)
    {
        /* Skip leading whitespace. */
        lpCmd = SkipWhite(lpCmd);

        /* Are we at a NULL? */
        if (!*lpCmd)
        {
            /* Did we find any commands yet? */
            if (cCmd)
                goto GDEExit;
            else
                goto GDEErrExit;
        }

        /* Each command should be inside square brackets. */
        if (*lpCmd != TEXT('['))
            goto GDEErrExit;
        lpCmd++;

        /* Get the command name. */
        lpCmd = GetCommandName(lpCmd, lpsCommands, lpW);
        if (*lpW == (UINT)-1)
            goto GDEErrExit;

        // We need to leave quotes in for the first param of an AddItem.
        if (fLFN && *lpW == 2)
        {
            fIncludeQuotes = TRUE;
        }

        lpW++;

        /* Start with zero parms. */
        cParm = 0;
        lpCmd = SkipWhite(lpCmd);

        /* Check for opening '(' */
        if (*lpCmd == TEXT('('))
        {
            lpCmd++;

            /* Skip white space and then find some parameters (may be none). */
            lpCmd = SkipWhite(lpCmd);

            while (*lpCmd != TEXT(')'))
            {
                if (!*lpCmd)
                    goto GDEErrExit;

                // Only the first param of the AddItem command needs to
                // handle quotes from LFN guys.
                if (fIncludeQuotes && (cParm != 0))
                    fIncludeQuotes = FALSE;

                /* Get the parameter. */
                if (!(lpCmd = GetOneParameter(lpCmdStart, lpCmd, lpW + (++cParm), fIncludeQuotes)))
                    goto GDEErrExit;

                /* HACK: Did GOP replace a ')' with a NULL? */
                if (!*lpCmd)
                    break;

                /* Find the next one or ')' */
                lpCmd = SkipWhite(lpCmd);
            }

            // Skip closing bracket.
            lpCmd++;

            /* Skip the terminating stuff. */
            lpCmd = SkipWhite(lpCmd);
        }

        /* Set the count of parameters and then skip the parameters. */
        *lpW++ = cParm;
        lpW += cParm;

        /* We found one more command. */
        cCmd++;

        /* Commands must be in square brackets. */
        if (*lpCmd != TEXT(']'))
            goto GDEErrExit;
        lpCmd++;
    }

GDEExit:
    /* Terminate the command list with -1. */
    *lpW = (UINT)-1;

    return lpRet;

GDEErrExit:
    GlobalFree(lpW);
    return(0);
}

STDAPI_(LPNMVIEWFOLDER) DDECreatePostNotify(LPNMVIEWFOLDER pnm)
{
    LPNMVIEWFOLDER pnmPost = NULL;
    TCHAR szCmd[MAX_PATH * 2];
    StrCpyN(szCmd, pnm->szCmd, SIZECHARS(szCmd));
    UINT* pwCmd = GetDDECommands(szCmd, c_sDDECommands, FALSE);

    // -1 means there aren't any commands we understand.  Oh, well
    if (pwCmd && (-1 != *pwCmd))
    {
        LPCTSTR pszCommand = c_sDDECommands[*pwCmd].pszCommand;

        //
        //  these are the only commands handled by a PostNotify
        if (pszCommand == TEXT("ViewFolder")
            || pszCommand == TEXT("ExploreFolder")
            || pszCommand == TEXT("ShellFile"))
        {
            pnmPost = (LPNMVIEWFOLDER)LocalAlloc(LPTR, sizeof(NMVIEWFOLDER));

            if (pnmPost)
                memcpy(pnmPost, pnm, sizeof(NMVIEWFOLDER));
        }

        GlobalFree(pwCmd);
    }

    return pnmPost;
}

// Helper function to convert passed in command parameters into the
// appropriate Id list
LPITEMIDLIST _GetPIDLFromDDEArgs(UINT nArg, LPTSTR lpszBuf, UINT* lpwCmd, PSHDDEERR psde, LPCITEMIDLIST* ppidlGlobal)
{
    LPTSTR lpsz;
    LPITEMIDLIST pidl = NULL;

    // Switch from 0-based to 1-based 
    ++nArg;
    if (*lpwCmd < nArg)
    {
        return NULL;
    }

    // Skip to the right argument
    lpwCmd += nArg;
    lpsz = &lpszBuf[*lpwCmd];

    // REVIEW: all associations will go through here.  this
    // is probably not what we want for normal cmd line type operations

    // A colon at the begining of the path means that this is either
    // a pointer to a pidl (win95 classic) or a handle:pid (all other
    // platforms including win95+IE4).  Otherwise, it's a regular path.

    if (lpsz[0] == TEXT(':'))
    {
        HANDLE hMem;
        DWORD  dwProcId;
        LPTSTR pszNextColon;

        // Convert the string into a pidl.

        hMem = LongToHandle(StrToLong((LPTSTR)(lpsz + 1)));
        pszNextColon = StrChr(lpsz + 1, TEXT(':'));
        if (pszNextColon)
        {
            LPITEMIDLIST pidlShared;

            dwProcId = (DWORD)StrToLong(pszNextColon + 1);
            pidlShared = (LPITEMIDLIST)SHLockShared(hMem, dwProcId);
            if (pidlShared && !IsBadReadPtr(pidlShared, 1))
            {
                pidl = ILClone(pidlShared);
                SHUnlockShared(pidlShared);
            }
            else
            {
            }
            SHFreeShared(hMem, dwProcId);
        }
        else if (hMem && !IsBadReadPtr(hMem, sizeof(WORD)))
        {
            // this is likely to be browser only mode on win95 with the old pidl arguments which is
            // going to be in shared memory.... (must be cloned into local memory)...
            pidl = ILClone((LPITEMIDLIST)hMem);

            // this will get freed if we succeed.
            *ppidlGlobal = (LPITEMIDLIST)hMem;
        }

        return pidl;
    }
    else
    {
        TCHAR tszQual[MAX_PATH];

        // We must copy to a temp buffer because the PathQualify may
        // result in a string longer than our input buffer and faulting
        // seems like a bad way of handling that situation.
        lstrcpyn(tszQual, lpsz, ARRAYSIZE(tszQual));
        lpsz = tszQual;

        /* Fuck you
        // Is this a URL?
        if (!PathIsURL(lpsz))
        {
            // No; qualify it
            PathQualifyDef(lpsz, NULL, PQD_NOSTRIPDOTS);
        }
        */ 
        pidl = ILCreateFromPath(lpsz);

        if (pidl == NULL && psde)
        {
            psde->idMsg = IDS_CANTFINDDIR;
            lstrcpyn(psde->szParam, lpsz, ARRAYSIZE(psde->szParam));
        }
        return pidl;
    }
}

LPITEMIDLIST GetPIDLFromDDEArgs(LPTSTR lpszBuf, UINT* lpwCmd, PSHDDEERR psde, LPCITEMIDLIST* ppidlGlobal)
{
    LPITEMIDLIST pidl = _GetPIDLFromDDEArgs(1, lpszBuf, lpwCmd, psde, ppidlGlobal);
    if (!pidl)
    {
        pidl = _GetPIDLFromDDEArgs(0, lpszBuf, lpwCmd, psde, ppidlGlobal);
    }

    return pidl;
}

STDAPI_(BOOL) DDEHandleViewFolderNotify(IShellBrowser* psb, HWND hwnd, LPNMVIEWFOLDER pnm)
{
    BOOL fRet = FALSE;
    UINT* pwCmd = GetDDECommands(pnm->szCmd, c_sDDECommands, FALSE);

    // -1 means there aren't any commands we understand.  Oh, well
    if (pwCmd && (-1 != *pwCmd))
    {
        UINT* pwCmdSave = pwCmd;
        UINT c = *pwCmd++;

        LPCTSTR pszCommand = c_sDDECommands[c].pszCommand;

        if (pszCommand == c_szViewFolder ||
            pszCommand == c_szExploreFolder)
        {
            //fRet = DoDDE_ViewFolder(psb, hwnd, pnm->szCmd, pwCmd,
                //pszCommand == c_szExploreFolder, pnm->dwHotKey, pnm->hMonitor);
        }
        else if (pszCommand == c_szShellFile)
        {
            fRet = DDE_ShellFile(pnm->szCmd, pwCmd, 0);
        }

        GlobalFree(pwCmdSave);
    }

    return fRet;
}

STDAPI_(TCHAR) SHFindMnemonic(LPCTSTR psz)
{
    ASSERT(psz);
    TCHAR tchDefault = *psz;                // Default is first character
    LPCTSTR pszAmp;

    while ((pszAmp = StrChr(psz, TEXT('&'))) != NULL)
    {
        switch (pszAmp[1])
        {
        case TEXT('&'):         // Skip over &&
            psz = pszAmp + 2;
            continue;

        case TEXT('\0'):        // Ignore trailing ampersand
            return tchDefault;

        default:
            return pszAmp[1];
        }
    }
    return tchDefault;
}

STDAPI SHBindToIDListParent(LPCITEMIDLIST pidl, REFIID riid, void** ppv, LPCITEMIDLIST* ppidlLast)
{
    return SHBindToFolderIDListParent(NULL, pidl, riid, ppv, ppidlLast);
}

__inline CHAR CharUpperCharA(CHAR c)
{
    return (CHAR)(DWORD_PTR)CharUpperA((LPSTR)(DWORD_PTR)(c));
}


//
// Function loader
//
#define MODULE_VARNAME(NAME) hMod_ ## NAME

#define LOAD_MODULE(NAME)                                        \
HMODULE MODULE_VARNAME(NAME) = LoadLibraryW(L#NAME ".dll");      \
if (!MODULE_VARNAME(NAME))                                       \
    return false;

#define LOAD_FUNCTION(MODULE, FUNCTION)                                      \
*(FARPROC *)&FUNCTION = GetProcAddress(MODULE_VARNAME(MODULE), #FUNCTION);   \
if (!FUNCTION)                                                               \
	return false;

#define LOAD_ORDINAL(MODULE, FUNCNAME, ORDINAL)                                   \
*(FARPROC *)&FUNCNAME = GetProcAddress(MODULE_VARNAME(MODULE), (LPCSTR)ORDINAL);  \
if (!FUNCNAME)                                                                    \
	return false;

STDAPI_(BOOL) SHWinHelp(HWND hwndMain, LPCTSTR lpszHelp, UINT usCommand, ULONG_PTR ulData)
{
    // Try to show help
    if (!WinHelp(hwndMain, lpszHelp, usCommand, ulData))
    {
        // Problem.
        return FALSE;
    }
    return TRUE;
}

bool SHUndocInit(void)
{

	LOAD_MODULE(shlwapi);
	LOAD_ORDINAL(shlwapi, IUnknown_Exec, 164);
    LOAD_ORDINAL(shlwapi, SHSetWindowBits, 165);
	LOAD_ORDINAL(shlwapi, IUnknown_OnFocusChangeIS, 509);
	LOAD_ORDINAL(shlwapi, SHPropagateMessage, 178);
	LOAD_ORDINAL(shlwapi, SHGetCurColorRes, 193);
	LOAD_ORDINAL(shlwapi, SHFillRectClr, 197);
    LOAD_ORDINAL(shlwapi, SHRunIndirectRegClientCommand, 467);
    LOAD_ORDINAL(shlwapi, SHGetMachineInfo, 413);
    LOAD_ORDINAL(shlwapi, IUnknown_QueryStatus, 163);
    LOAD_ORDINAL(shlwapi, SHInvokeDefaultCommand, 279);
    LOAD_ORDINAL(shlwapi, IUnknown_UIActivateIO, 479);
    LOAD_ORDINAL(shlwapi, SHMessageBoxCheckExW, 292);
    LOAD_ORDINAL(shlwapi, SHDefWindowProc, 240);
    LOAD_ORDINAL(shlwapi, IUnknown_TranslateAcceleratorIO, 478);
    LOAD_FUNCTION(shlwapi, SHIsChildOrSelf);

	LOAD_MODULE(shell32);
	LOAD_ORDINAL(shell32, SHGetUserDisplayName, 241);
    LOAD_ORDINAL(shell32, RegisterShellHook, 181);
    LOAD_ORDINAL(shell32, SHSettingsChanged, 244);
    LOAD_ORDINAL(shell32, ExitWindowsDialog, 60);
    LOAD_ORDINAL(shell32, RunFileDlg, 61);
    LOAD_ORDINAL(shell32, LogoffWindowsDialog, 54);
    LOAD_ORDINAL(shell32, DisconnectWindowsDialog, 254);
    LOAD_ORDINAL(shell32, SHFindComputer, 91);
    LOAD_FUNCTION(shell32, SHUpdateRecycleBinIcon);

	LOAD_MODULE(shcore);
	LOAD_ORDINAL(shcore, IUnknown_GetClassID, 142);
    LOAD_ORDINAL(shcore, SHQueueUserWorkItem, 162)
    LOAD_ORDINAL(shcore, SHLoadRegUIStringW, 126);

    LOAD_MODULE(user32);
    LOAD_FUNCTION(user32, EndTask);

    LOAD_MODULE(msi);
    LOAD_FUNCTION(msi, MsiDecomposeDescriptorW);

    LOAD_MODULE(winsta);
    LOAD_FUNCTION(winsta, WinStationRegisterConsoleNotification);
    LOAD_FUNCTION(winsta, WinStationSetInformationW);
    LOAD_FUNCTION(winsta, WinStationUnRegisterConsoleNotification);


	return true;
}