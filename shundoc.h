//+-------------------------------------------------------------------------
//
//  ExplorerEx - Windows XP Explorer
//  Copyright (C) Microsoft
//
//  File:       shundoc.h
//
//  History:    Oct-11-24   aubymori  Created
//  History:    Jan-20-25   kfh83     Modify for Explorer
//  History:    Jan-20-25   olive6841 Add COM Interaces
//
//--------------------------------------------------------------------------
#pragma once
#include <Windows.h>
#include <shobjidl_core.h>

const DWORD dwExStyleRTLMirrorWnd = WS_EX_LAYOUTRTL;

//
// Structs
// 

typedef struct {
    NMHDR  hdr;
    CHAR   szCmd[MAX_PATH * 2];
    DWORD  dwHotKey;
} NMVIEWFOLDERA, FAR* LPNMVIEWFOLDERA;
typedef struct {
    NMHDR  hdr;
    WCHAR  szCmd[MAX_PATH * 2];
    DWORD  dwHotKey;
} NMVIEWFOLDERW, FAR* LPNMVIEWFOLDERW;

typedef struct tagMUIINSTALLLANG {
    LANGID LangID;
    BOOL   bInstalled;
} MUIINSTALLLANG, * LPMUIINSTALLLANG;

typedef struct
{
    CLSID clsid;
    DWORD dwFlags;
} LOADINPROCDATA, * PLOADINPROCDATA;

typedef struct
{
    DWORD   cbSize;     // SIZEOF
    DWORD   dwMask;     // INOUT requested/given (UEIM_*)
    int     cHit;       // profile count
    DWORD   dwAttrs;    // attributes (UEIA_*)
    FILETIME ftExecute; // Last execute filetime
} UEMINFO, * LPUEMINFO;

typedef struct _SHShortcutInvokeAsIDList {
    USHORT  cb;
    DWORD   dwItem1;                    // SHCNEE_EXTENDED_EVENT requires this
    DWORD   dwPid;                      // PID of target application
    WCHAR   szShortcutName[MAX_PATH];   // Path to shortcut
    WCHAR   szTargetName[MAX_PATH];     // Path to target application
    USHORT  cbZero;
} SHShortcutInvokeAsIDList, * LPSHShortcutInvokeAsIDList;

typedef struct _tagHardErrorData
{
    DWORD   dwSize;             // Size of this structure
    DWORD   dwError;            // Hard Error
    DWORD   dwFlags;            // Hard Error flags
    UINT    uOffsetTitleW;      // Offset to UNICODE Title
    UINT    uOffsetTextW;       // Offset to UNICODE Text
} HARDERRORDATA, * PHARDERRORDATA;

typedef struct _AppBarData3264
{
    DWORD cbSize;
    DWORD dwWnd;
    UINT uCallbackMessage;
    UINT uEdge;
    RECT rc;
    DWORDLONG lParam; // message specific
} APPBARDATA3264, * PAPPBARDATA3264;

typedef struct _TRAYAPPBARDATA
{
    APPBARDATA3264 abd;
    DWORD dwMessage;
    DWORD hSharedABD;
    DWORD dwProcId;
} TRAYAPPBARDATA, * PTRAYAPPBARDATA;

typedef struct tagNOTIFYITEM
{
    LPWSTR      pszExeName;
    LPWSTR      pszIconText;
    HICON       hIcon;
    HWND        hWnd;
    DWORD       dwUserPref;
    UINT        uID;
    GUID        guidItem;
} NOTIFYITEM, * LPNOTIFYITEM;

typedef struct _NOTIFYICONDATA32A {
    DWORD cbSize;
    DWORD dwWnd;                        // NB!
    UINT uID;
    UINT uFlags;
    UINT uCallbackMessage;
    DWORD dwIcon;                       // NB!
#if (_WIN32_IE < 0x0500)
    CHAR   szTip[64];
#else
    CHAR   szTip[128];
#endif
#if (_WIN32_IE >= 0x0500)
    DWORD dwState;
    DWORD dwStateMask;
    CHAR   szInfo[256];
    union {
        UINT  uTimeout;
        UINT  uVersion;
    } DUMMYUNIONNAME;
    CHAR   szInfoTitle[64];
    DWORD dwInfoFlags;
#endif
#if (_WIN32_IE >= 0x600)
    GUID guidItem;
#endif
} NOTIFYICONDATA32A, * PNOTIFYICONDATA32A;
typedef struct _NOTIFYICONDATA32W {
    DWORD cbSize;
    DWORD dwWnd;                        // NB!
    UINT uID;
    UINT uFlags;
    UINT uCallbackMessage;
    DWORD dwIcon;                       // NB!
#if (_WIN32_IE < 0x0500)
    WCHAR  szTip[64];
#else
    WCHAR  szTip[128];
#endif
#if (_WIN32_IE >= 0x0500)
    DWORD dwState;
    DWORD dwStateMask;
    WCHAR  szInfo[256];
    union {
        UINT  uTimeout;
        UINT  uVersion;
    } DUMMYUNIONNAME;
    WCHAR  szInfoTitle[64];
    DWORD dwInfoFlags;
#endif
#if (_WIN32_IE >= 0x600)
    GUID guidItem;
#endif
} NOTIFYICONDATA32W, * PNOTIFYICONDATA32W;

typedef NOTIFYICONDATA32W NOTIFYICONDATA32;
typedef PNOTIFYICONDATA32W PNOTIFYICONDATA32;


typedef struct _TRAYNOTIFYDATAA {
    DWORD dwSignature;
    DWORD dwMessage;
    NOTIFYICONDATA32 nid;
} TRAYNOTIFYDATAA, * PTRAYNOTIFYDATAA;
typedef struct _TRAYNOTIFYDATAW {
    DWORD dwSignature;
    DWORD dwMessage;
    NOTIFYICONDATA32 nid;
} TRAYNOTIFYDATAW, * PTRAYNOTIFYDATAW;

typedef TRAYNOTIFYDATAW TRAYNOTIFYDATA;
typedef PTRAYNOTIFYDATAW PTRAYNOTIFYDATA;

typedef struct tagSHCNF_INSTRUMENT {
    USHORT uOffset;
    USHORT uAlign;
    DWORD dwEventType;
    DWORD dwEventStructure;
    SYSTEMTIME st;
    union tagEvents {
        struct tagSTRING {
            TCHAR sz[32];
        } string;
        struct tagHOTKEY {
            WPARAM wParam;
        } hotkey;
        struct tagWNDPROC {
            HWND hwnd;
            UINT uMsg;
            WPARAM wParam;
            LPARAM lParam;
        } wndproc;
        struct tagCOMMAND {
            HWND hwnd;
            UINT idCmd;
        } command;
        struct tagDROP {
            HWND hwnd;
            UINT idCmd;
            //          TCHAR sz[32]; // convert pDataObject into something we can log
        } drop;
    } e;
    USHORT uTerm;
} SHCNF_INSTRUMENT_INFO, * LPSHCNF_INSTRUMENT_INFO;

typedef struct _tagSHELLREMINDER
{
    DWORD  cbSize;
    LPWSTR pszName;
    LPWSTR pszTitle;
    LPWSTR pszText;
    LPWSTR pszTooltip;
    LPWSTR pszIconResource;
    LPWSTR pszShellExecute;
    GUID* pclsid;
    DWORD  dwShowTime;
    DWORD  dwRetryInterval;
    DWORD  dwRetryCount;
    DWORD  dwTypeFlags;
} SHELLREMINDER;

//
// Macros
//


// iepriv macros

#define SMC_INITMENU            0x00000001  // The callback is called to init a menuband
#define SMC_CREATE              0x00000002
#define SMC_EXITMENU            0x00000003  // The callback is called when menu is collapsing
#define SMC_EXEC                0x00000004  // The callback is called to execute an item
#define SMC_GETINFO             0x00000005  // The callback is called to return DWORD values
#define SMC_GETSFINFO           0x00000006  // The callback is called to return DWORD values
#define SMC_GETOBJECT           0x00000007  // The callback is called to get some object
#define SMC_GETSFOBJECT         0x00000008  // The callback is called to get some object
#define SMC_SFEXEC              0x00000009  // The callback is called to execute an shell folder item
#define SMC_SFSELECTITEM        0x0000000A  // The callback is called when an item is selected
#define SMC_SELECTITEM          0x0000000B  // The callback is called when an item is selected
#define SMC_GETSFINFOTIP        0x0000000C  // The callback is called to get some object
#define SMC_GETINFOTIP          0x0000000D  // The callback is called to get some object
#define SMC_INSERTINDEX         0x0000000E  // New item insert index
#define SMC_POPUP               0x0000000F  // InitMenu/InitMenuPopup (sort of)
#define SMC_REFRESH             0x00000010  // Menus have completely refreshed. Reset your state.
#define SMC_DEMOTE              0x00000011  // Demote an item
#define SMC_PROMOTE             0x00000012  // Promote an item, wParam = SMINV_* flag
#define SMC_BEGINENUM           0x00000013  // tell callback that we are beginning to ENUM the indicated parent
#define SMC_ENDENUM             0x00000014  // tell callback that we are ending the ENUM of the indicated paren
#define SMC_MAPACCELERATOR      0x00000015  // Called when processing an accelerator.
#define SMC_DEFAULTICON         0x00000016  // Returns Default icon location in wParam, index in lParam
#define SMC_NEWITEM             0x00000017  // Notifies item is not in the order stream.
#define SMC_GETMINPROMOTED      0x00000018  // Returns the minimum number of promoted items
#define SMC_CHEVRONEXPAND       0x00000019  // Notifies of a expansion via the chevron
#define SMC_DISPLAYCHEVRONTIP   0x0000002A  // S_OK display, S_FALSE not.
#define SMC_DESTROY             0x0000002B  // Called when a pane is being destroyed.
#define SMC_SETOBJECT           0x0000002C  // Called to save the passed object
#define SMC_SETSFOBJECT         0x0000002D  // Called to save the passed object
#define SMC_SHCHANGENOTIFY      0x0000002E  // Called when a Change notify is received. lParam points to SMCSHCHANGENOTIFYSTRUCT
#define SMC_CHEVRONGETTIP       0x0000002F  // Called to get the chevron tip text. wParam = Tip title, Lparam = TipText Both MAX_PATH
#define SMC_SFDDRESTRICTED      0x00000030  // Called requesting if it's ok to drop. wParam = IDropTarget.
#define SMC_GETIMAGELISTS       0x00000031  // Called to get the small & large icon image lists, otherwise it will default to shell image list
#define SMC_CUSTOMDRAW          0x00000032  // Requires SMINIT_CUSTOMDRAW
#define SMC_BEGINDRAG           0x00000033  // Called to get preferred drop effect. wParam = &pdwEffect
#define SMC_MOUSEFILTER         0x00000034  // Called to allow host to filter mouse messages. wParam=bRemove, lParam=pmsg
#define SMC_DUMPONUPDATE        0x00000035  // S_OK if host wants old trash-everything-on-update behavior (recent docs)

#define SMC_FILTERPIDL          0x10000000  // The callback is called to see if an item is visible
#define SMC_CALLBACKMASK        0xF0000000  // Mask of comutationally intense messages

//

#define UEIM_HIT        0x01
#define UEIM_FILETIME   0x02

#define SHCNFI_MAIN_WNDPROC               6
#define SHCNFI_EVENT_WNDPROC              3   // e.wndproc


#define WINMMDEVICECHANGEMSGSTRING L"winmm_devicechange"

#define ENABLE_BALLOONTIP_MESSAGE L"Enable Balloon Tip"

#define NT_SUCCESS(Status) (((NTSTATUS)(Status)) >= 0)

#define ATOMICRELEASE(p) IUnknown_AtomicRelease((void **)&p)

#define ATOMICRELEASET(p, type) { if(p) { type* punkT=p; p=NULL; punkT->Release();} }

#define ResultFromShort(i)  ResultFromScode(MAKE_SCODE(SEVERITY_SUCCESS, 0, (USHORT)(i)))

#define SAFECAST(_obj, _type) (((_type)(_obj)==(_obj)?0:0), (_type)(_obj))

#define BOOLIFY(expr)           (!!(expr))

#define IntToPtr_(T, i) ((T)IntToPtr(i))

#define INSTRUMENT_STATECHANGE(t)

#define SERVERNAME_CURRENT  ((HANDLE)NULL)

#define SHCoUninitialize(hr) if (SUCCEEDED(hr)) CoUninitialize()

#define IS_BIDI_LOCALIZED_SYSTEM()       IsBiDiLocalizedSystem()



#define IS_WINDOW_RTL_MIRRORED(hwnd)     (g_bMirroredOS && Mirror_IsWindowMirroredRTL(hwnd))

#ifdef UNICODE
typedef NMVIEWFOLDERW NMVIEWFOLDER;
typedef LPNMVIEWFOLDERW LPNMVIEWFOLDER;
#else
typedef NMVIEWFOLDERA NMVIEWFOLDER;
typedef LPNMVIEWFOLDERA LPNMVIEWFOLDER;
#endif // UNICODE




#define IS_VALID_WRITE_PTR(ptr, type) \
   (IsBadWritePtr((PVOID)(ptr), sizeof(type)) ? \
    (TraceMsgA(TF_ERROR, "invalid %hs write pointer - %#08lx", (LPCSTR)#type" *", (ptr)), FALSE) : \
    TRUE)

#define INSTRUMENT_WNDPROC(t,h,u,w,l)                           \
{                                                               \
    SHCNF_INSTRUMENT_INFO s;                                    \
    s.dwEventType=(t);                                          \
    s.dwEventStructure=SHCNFI_EVENT_WNDPROC;                    \
    s.e.wndproc.hwnd=(h);                                       \
    s.e.wndproc.uMsg=(u);                                       \
    s.e.wndproc.wParam=(w);                                     \
    s.e.wndproc.lParam=(l);                                     \
    SHChangeNotify(SHCNE_INSTRUMENT,SHCNF_INSTRUMENT,&s,NULL);  \
}

#define CMF_ICM3                0x00020000      // QueryContextMenu can assume IContextMenu3 semantics (i.e.,
                                                // will receive WM_INITMENUPOPUP, WM_MEASUREITEM, WM_DRAWITEM,
                                                // and WM_MENUCHAR, via HandleMenuMsg2)

#define SHGUPP_FLAG_BASEPATH            0x00000001
#define SHGUPP_FLAG_DEFAULTPICSPATH     0x00000002
#define SHGUPP_FLAG_CREATE              0x80000000
#define SHGUPP_FLAG_VALID_MASK          0x80000003
#define SHGUPP_FLAG_INVALID_MASK        ~SHGUPP_FLAG_VALID_MASK

#define SPM_POST        0x0000
#define SPM_SEND        0x0001
#define SPM_ONELEVEL    0x0002  // default: send to all descendants including grandkids, etc.

#define SHCNEE_USERINFOCHANGED     11L
#define SHCNEE_SHORTCUTINVOKE       9L  // an app has been launched via a shortcut

#define CWM_TASKBARWAKEUP               (WM_USER + 26) // Used to restore tray thread to normal priority in extremely stressed machines
#define DTM_RAISE                       (WM_USER + 83)
#define DTRF_RAISE      0
#define DTM_SAVESTATE               (WM_USER + 77)
#define DTM_UPDATENOW               (WM_USER + 93)

#define WMTRAY_PROGCHANGE           (WM_USER + 200)     // 200=0xc8
#define WMTRAY_RECCHANGE            (WM_USER + 201)
#define WMTRAY_FASTCHANGE           (WM_USER + 202)
// was  WMTRAY_DESKTOPCHANGE        (WM_USER + 204)

#define WMTRAY_COMMONPROGCHANGE     (WM_USER + 205)
#define WMTRAY_COMMONFASTCHANGE     (WM_USER + 206)

#define WMTRAY_FAVORITESCHANGE      (WM_USER + 207)

#define WMTRAY_REGISTERHOTKEY       (WM_USER + 230)
#define WMTRAY_UNREGISTERHOTKEY     (WM_USER + 231)
#define WMTRAY_SETHOTKEYENABLE      (WM_USER + 232)
#define WMTRAY_SCREGISTERHOTKEY     (WM_USER + 233)
#define WMTRAY_SCUNREGISTERHOTKEY   (WM_USER + 234)
#define WMTRAY_QUERY_MENU           (WM_USER + 235)
#define WMTRAY_QUERY_VIEW           (WM_USER + 236)     // 236=0xec
#define WMTRAY_TOGGLEQL             (WM_USER + 237)

//
//  GMI_TSCLIENT tells you whether you are running as a Terminal Server
//  client and should disable your animations.
//
#define GMI_TSCLIENT            0x0003  // Returns nonzero if TS client

#define ABE_MAX         4

#define TBSTYLE_EX_FIXEDDROPDOWN            0x00000040 // Only used in the taskbar
#define TBSTYLE_EX_TRANSPARENTDEADAREA      0x00000100
#define TBSTYLE_EX_TOOLTIPSEXCLUDETOOLBAR   0x00000200

#define SMINIT_CUSTOMDRAW           0x00000080   // Send SMC_CUSTOMDRAW
#define SMINIT_USEMESSAGEFILTER     0x00000020
#define SMSET_USEPAGER              0x00000080    //Enable pagers in static menus
#define SMSET_NOPREFIX              0x00000100    //Enable ampersand in static menus
#define SMINV_POSITION       0x00000004

#define WS_EX_LAYOUTRTL         0x00400000L // Right to left mirroring

// StopWatch node types used in memory log to identify the type of node
#define EMPTY_NODE  0x0
#define START_NODE  0x1
#define LAP_NODE    0x2
#define STOP_NODE   0x3
#define OUT_OF_NODES 0x4

// Tray CopyData Messages
#define TCDM_APPBAR     0x00000000
#define TCDM_NOTIFY     0x00000001
#define TCDM_LOADINPROC 0x00000002



#define DCX_USESTYLE        0x00010000L
#define DCX_NEEDFONT        0x00020000L /* ;Internal */
#define DCX_NODELETERGN     0x00040000L /* ;Internal */
#define DCX_NOCLIPCHILDREN  0x00080000L /* ;Internal */
#define DCX_NORECOMPUTE     0x00100000L /* ;Internal */
#define DCX_VALIDATE        0x00200000L /* ;Internal */

#define TTF_EXCLUDETOOLAREA     0x4000


//
// Enums
// 
typedef enum
{
    RR_ALLOW = 1,
    RR_DISALLOW,
    RR_NOCHANGE,
} RESTRICTION_RESULT;

// IRestrict::IsRestricted() dwRestrictAction parameter values for
// the RID_RDeskBars pguidID.
typedef enum
{
    RA_DRAG = 1,
    RA_DROP,
    RA_ADD,
    RA_CLOSE,
    RA_MOVE,
} RESTRICT_ACTIONS;

enum INSTALLSTATE
{
    installingNone,
    installingDone,
    installingBoth,
    installingDocObject,
    installingHandler
};


//
// Function definitions
//

extern HRESULT(STDMETHODCALLTYPE* IUnknown_Exec)(IUnknown* punk, const GUID* pguidCmdGroup, DWORD nCmdID, DWORD nCmdexecopt, VARIANTARG* pvarargIn, VARIANTARG* pvarargOut);
extern HRESULT(STDMETHODCALLTYPE* IUnknown_GetClassID)(IUnknown* punk, CLSID* pclsid);
extern HRESULT(STDMETHODCALLTYPE* IUnknown_OnFocusChangeIS)(IUnknown* punk, IUnknown* punkSrc, BOOL fSetFocus);
extern HRESULT(STDMETHODCALLTYPE* IUnknown_QueryStatus)(IUnknown* punk, const GUID* pguidCmdGroup, ULONG cCmds, OLECMD rgCmds[], OLECMDTEXT* pcmdtext);
extern HRESULT(STDMETHODCALLTYPE* IUnknown_UIActivateIO)(IUnknown* punk, BOOL fActivate, LPMSG lpMsg);
STDAPI IUnknown_DragEnter(IUnknown* punk, IDataObject* pdtobj, DWORD grfKeyState, POINTL pt, DWORD* pdwEffect);
STDAPI IUnknown_DragOver(IUnknown* punk, DWORD grfKeyState, POINTL pt, DWORD* pdwEffect);
STDAPI IUnknown_DragLeave(IUnknown* punk);

extern HRESULT(STDMETHODCALLTYPE* SHPropagateMessage)(HWND hwndParent, UINT uMsg, WPARAM wParam, LPARAM lParam, int iFlags);
extern HRESULT(STDMETHODCALLTYPE* SHGetUserDisplayName)(LPWSTR pszDisplayName, PULONG uLen);
extern HRESULT(STDMETHODCALLTYPE* SHGetUserPicturePath)(LPCWSTR pszUsername, DWORD dwFlags, LPWSTR pszPath, DWORD cchPathMax);
extern HRESULT(STDMETHODCALLTYPE* SHSetWindowBits)(HWND hwnd, int iWhich, DWORD dwBits, DWORD dwValue);
extern HRESULT(STDMETHODCALLTYPE* SHRunIndirectRegClientCommand)(HWND hwnd, LPCWSTR pszClient);
extern HRESULT(STDMETHODCALLTYPE* SHInvokeDefaultCommand)(HWND hwnd, IShellFolder* psf, LPCITEMIDLIST pidlItem);
extern HRESULT(STDMETHODCALLTYPE* SHSettingsChanged)(WPARAM wParam, LPARAM lParam);
extern HRESULT(STDMETHODCALLTYPE* SHIsChildOrSelf)(HWND hwndParent, HWND hwnd);
extern BOOL(WINAPI* SHQueueUserWorkItem)(IN LPTHREAD_START_ROUTINE pfnCallback, IN LPVOID pContext, IN LONG lPriority, IN DWORD_PTR dwTag, OUT DWORD_PTR* pdwId OPTIONAL, IN LPCSTR pszModule OPTIONAL, IN DWORD dwFlags);
HRESULT(STDMETHODCALLTYPE* ExitWindowsDialog)(HWND hwndParent);
extern INT(STDMETHODCALLTYPE* SHMessageBoxCheckExW)(HWND hwnd, HINSTANCE hinst, LPCWSTR pszTemplateName, DLGPROC pDlgProc, LPVOID pData, int iDefault, LPCWSTR pszRegVal);
extern INT(STDMETHODCALLTYPE* RunFileDlg)(HWND hwndParent, HICON hIcon, LPCTSTR pszWorkingDir, LPCTSTR pszTitle, LPCTSTR pszPrompt, DWORD dwFlags);
extern UINT(STDMETHODCALLTYPE* SHGetCurColorRes)(void);
extern VOID(STDMETHODCALLTYPE* SHUpdateRecycleBinIcon)();
COLORREF(STDMETHODCALLTYPE* SHFillRectClr)(HDC hdc, LPRECT lprect, COLORREF color);
STDAPI_(void) SHAdjustLOGFONT(IN OUT LOGFONT* plf);
STDAPI_(BOOL) SHAreIconsEqual(HICON hIcon1, HICON hIcon2);
STDAPI_(BOOL) SHForceWindowZorder(HWND hwnd, HWND hwndInsertAfter);
STDAPI SHCoInitialize(void);
BOOL SHRegisterDarwinLink(LPITEMIDLIST pidlFull, LPWSTR pszDarwinID, BOOL fUpdate);
BOOL(STDMETHODCALLTYPE* RegisterShellHook)(HWND hwnd, BOOL fInstall);

BOOL(STDMETHODCALLTYPE* WinStationRegisterConsoleNotification)(HANDLE  hServer, HWND    hWnd, DWORD   dwFlags);

#define GMI_DOCKSTATE           0x0000
// Return values for SHGetMachineInfo(GMI_DOCKSTATE)
#define GMID_NOTDOCKABLE         0  // Cannot be docked
#define GMID_UNDOCKED            1  // Is undocked
#define GMID_DOCKED              2  // Is docked
extern DWORD_PTR(WINAPI* SHGetMachineInfo)(UINT gmi);

BOOL(WINAPI* EndTask)(HWND hWnd, BOOL fShutDown, BOOL fForce);

BOOL IsBiDiLocalizedSystem(void);
BOOL Mirror_IsWindowMirroredRTL(HWND hWnd);

inline unsigned __int64 _FILETIMEtoInt64(const FILETIME* pft);
inline void SetFILETIMEfromInt64(FILETIME* pft, unsigned __int64 i64);
inline void IncrementFILETIME(FILETIME* pft, unsigned __int64 iAdjust);
inline void DecrementFILETIME(FILETIME* pft, unsigned __int64 iAdjust);

typedef HANDLE LPSHChangeNotificationLock;
typedef INT_PTR BOOL_PTR;





//
// Function loader
//
bool SHUndocInit(void);

//
// COM Interfaces
// 

MIDL_INTERFACE("D12F26B1-D90A-11d0-830D-00AA005B4383")
IRestrict : IUnknown
{
    // *** IUnknown methods ***
    STDMETHOD(QueryInterface) (THIS_ REFIID riid, void** ppv) PURE;
    STDMETHOD_(ULONG,AddRef) (THIS)  PURE;
    STDMETHOD_(ULONG,Release) (THIS) PURE;

    // *** IRestrict Methods ***
    STDMETHOD(IsRestricted) (THIS_ const GUID* pguidID, DWORD dwRestrictAction, VARIANT* pvarArgs, OUT DWORD* pdwRestrictionResult) PURE;
};

MIDL_INTERFACE("EA5F2D61-E008-11CF-99CB-00C04FD64497")
IWinEventHandler : IUnknown
{
    // *** IUnknown methods ***
    STDMETHOD(QueryInterface) (THIS_ REFIID riid, void** ppv) PURE;
    STDMETHOD_(ULONG, AddRef) (THIS)  PURE;
    STDMETHOD_(ULONG, Release) (THIS) PURE;

    // *** IWinEventHandler Methods ***
    STDMETHOD(OnWinEvent) (THIS_ HWND hwnd, UINT dwMsg, WPARAM wParam, LPARAM lParam, LRESULT* plres) PURE;
    STDMETHOD(IsWindowOwner) (THIS_ HWND hwnd) PURE;
};

MIDL_INTERFACE("5EA35BC9-19B1-11D1-9828-00C04FD91972")
IShellHotKey : IUnknown
{
    // *** IUnknown methods ***
    STDMETHOD(QueryInterface) (THIS_ REFIID riid, void** ppv) PURE;
    STDMETHOD_(ULONG, AddRef) (THIS)  PURE;
    STDMETHOD_(ULONG, Release) (THIS) PURE;

    // *** IShellHotKey methods ***
    STDMETHOD(RegisterHotKey)(THIS_ IShellFolder * psf, LPCITEMIDLIST pidlParent, LPCITEMIDLIST pidl) PURE;
};

MIDL_INTERFACE("4622AD10-FF23-11D0-8D34-00A0C90F2719")
ITrayPriv : IOleWindow
{
    // *** IUnknown methods ***
    STDMETHOD(QueryInterface) (THIS_ REFIID riid, void** ppv) PURE;
    STDMETHOD_(ULONG,AddRef) (THIS)  PURE;
    STDMETHOD_(ULONG,Release) (THIS) PURE;

    // *** IOleWindow methods ***
    STDMETHOD(GetWindow) (THIS_ HWND* lphwnd) PURE;
    STDMETHOD(ContextSensitiveHelp) (THIS_ BOOL fEnterMode) PURE;

    // *** ITrayPriv methods ***
    STDMETHOD(ExecItem)(THIS_ IShellFolder* psf, LPCITEMIDLIST pidl) PURE;
    STDMETHOD(GetFindCM)(THIS_ HMENU hmenu, UINT idFirst, UINT idLast, IContextMenu** ppcmFind) PURE;
    STDMETHOD(GetStaticStartMenu)(THIS_ HMENU* phmenu) PURE;
};

MIDL_INTERFACE("9E83C057-6823-4F1F-BfA3-7461D40A8173")
ITrayPriv2 : ITrayPriv
{
    // *** IUnknown methods ***
    STDMETHOD(QueryInterface) (THIS_ REFIID riid, void** ppv) PURE;
    STDMETHOD_(ULONG,AddRef) (THIS)  PURE;
    STDMETHOD_(ULONG,Release) (THIS) PURE;

    // *** IOleWindow methods ***
    STDMETHOD(GetWindow) (THIS_ HWND* lphwnd) PURE;
    STDMETHOD(ContextSensitiveHelp) (THIS_ BOOL fEnterMode) PURE;

    // *** ITrayPriv methods ***
    STDMETHOD(ExecItem)(THIS_ IShellFolder* psf, LPCITEMIDLIST pidl) PURE;
    STDMETHOD(GetFindCM)(THIS_ HMENU hmenu, UINT idFirst, UINT idLast, IContextMenu** ppcmFind) PURE;
    STDMETHOD(GetStaticStartMenu)(THIS_ HMENU* phmenu) PURE;

    // *** ITrayPriv2 methods ***
    STDMETHOD(ModifySMInfo)(THIS_ IN LPSMDATA psmd, IN OUT SMINFO* psminfo) PURE;
};

interface IDeskTray : public IUnknown
{
    // *** IUnknown methods ***
    STDMETHOD(QueryInterface) (THIS_ REFIID riid, LPVOID * ppvObj) PURE;
    STDMETHOD_(ULONG, AddRef) (THIS)  PURE;
    STDMETHOD_(ULONG, Release) (THIS) PURE;

    // *** IDeskTray methods ***
    STDMETHOD_(UINT, AppBarGetState)(THIS) PURE;
    STDMETHOD(GetTrayWindow)(THIS_ HWND * phwndTray) PURE;
    STDMETHOD(SetDesktopWindow)(THIS_ HWND hwndDesktop) PURE;

    // WARNING!  BEFORE CALLING THE SetVar METHOD YOU MUST DETECT
    // THE EXPLORER VERSION BECAUSE IE 4.00 WILL CRASH IF YOU TRY
    // TO CALL IT

    STDMETHOD(SetVar)(THIS_ int var, DWORD value) PURE;
};

MIDL_INTERFACE("6f51c646-0efe-4370-882a-c1f61cb27c3b")
IShellMenu2 : IShellMenu
{
    // Retrieves an interface on a submenu.
    HRESULT GetSubMenu(UINT idCmd, REFIID riid, void** ppvObj);
    HRESULT SetToolbar(HWND hwnd, DWORD dwFlags);
    HRESULT SetMinWidth(int cxMenu);
    HRESULT SetNoBorder(BOOL fNoBorder);
    HRESULT SetTheme(LPCWSTR pszTheme);
};

MIDL_INTERFACE("ec35e37a-6579-4f3c-93cd-6e62c4ef7636")
IStartMenuPin : IUnknown
{

    #define SMPIN_POS(i) (LPCITEMIDLIST)MAKEINTRESOURCE((i)+1))
    #define SMPINNABLE_EXEONLY          0x00000001) // allow only EXEs to be pinned
    #define SMPINNABLE_REJECTSLOWMEDIA  0x00000002) // reject slow media

    HRESULT EnumObjects(IEnumIDList * *ppenumIDList);
    //
    //  Pin:        pidlFrom = NULL, pidlTo = pidl
    //  Unpin:      pidlFrom = pidl, pidlTo = NULL
    //  Update:     pidlFrom = old,  pidlTo = new
    //  Move:       pidlFrom = pidl, pidlTo = SMPINPOS(iPos)
    HRESULT Modify(LPCITEMIDLIST pidlFrom, LPCITEMIDLIST pidlTo);
    HRESULT GetChangeCount(ULONG* pulOut);

    //
    //  pdto = data object to test
    //  dwFlags is an SMPINNABLE_* flag
    //  *ppidl receives pidl being pinned
    //
    HRESULT IsPinnable(IDataObject* pdto, DWORD dwFlags, LPITEMIDLIST* ppidl); // S_FALSE if not

    //
    //  Find the pidl on the pin list and resolve the shortcut that
    //  tracks it.
    //
    //  Returns S_OK if the pidl changed and was resolved.
    //  Returns S_FALSE if the pidl did not change.
    //  Returns an error if the Resolve failed.
    //
    HRESULT Resolve(HWND hwnd, DWORD dwFlags, LPCITEMIDLIST pidl, LPITEMIDLIST* ppidlResolved);
};

MIDL_INTERFACE("5836FB00-8187-11CF-A12B-00AA004AE837")
IShellService : IUnknown
{
    // *** IUnknown methods ***
    STDMETHOD(QueryInterface) (THIS_ REFIID riid, void** ppv) PURE;
    STDMETHOD_(ULONG, AddRef) (THIS)  PURE;
    STDMETHOD_(ULONG, Release) (THIS) PURE;

    // *** IShellService specific methods ***
    STDMETHOD(SetOwner)(THIS_ struct IUnknown* punkOwner) PURE;
};

MIDL_INTERFACE("fadb55b4-d382-4fc4-81d7-abb325c7f12a")
IFadeTask : IUnknown
{
    HRESULT FadeRect(LPCRECT prc);
};

MIDL_INTERFACE("e9ead8e6-2a25-410e-9b58-a9fbef1dd1a2")
IUserEventTimerCallback: IUnknown
{
    HRESULT UserEventTimerProc(ULONG uUserEventTimerID, UINT uTimerElapse);
};

MIDL_INTERFACE("0F504B94-6E42-42E6-99E0-E20FAFE52AB4")
IUserEventTimer: IUnknown
{
    STDMETHOD(SetUserEventTimer)(HWND hWnd, UINT uCallbackMessage, UINT uTimerElapse, IUserEventTimerCallback * pUserEventTimerCallback, ULONG * puUserEventTimerID );
    STDMETHOD(KillUserEventTimer)(HWND hWnd, ULONG uUserEventTimerID);

    STDMETHOD(GetUserEventTimerElapsed)(HWND hWnd, ULONG uUserEventTimerID, UINT* puTimerElapsed);
    STDMETHOD(InitTimerTickInterval)(UINT uTimerTickIntervalMs);
};

MIDL_INTERFACE("D782CCBA-AFB0-43F1-94DB-FDA3779EACCB") INotificationCB : IUnknown
{
    STDMETHOD(Notify)(ULONG, NOTIFYITEM*) = 0;
};

MIDL_INTERFACE("FB852B2C-6BAD-4605-9551-F15F87830935") ITrayNotify : IUnknown
{
    STDMETHOD(RegisterCallback)(INotificationCB * callback) = 0;
    STDMETHOD(SetPreference)(const NOTIFYITEM* notify_item) = 0;
    STDMETHOD(EnableAutoTray)(BOOL enabled) = 0;
};

//this is the Windows 8+ variant of ITrayNotify, probably not needed for now but might need it for later
MIDL_INTERFACE("D133CE13-3537-48BA-93A7-AFCD5D2053B4") ITrayNotifyWin8 : IUnknown
{
    STDMETHOD(RegisterCallback)(INotificationCB * callback, ULONG*) = 0;
    STDMETHOD(UnregisterCallback)(ULONG*) = 0;
    STDMETHOD(SetPreference)(NOTIFYITEM const*) = 0;
    STDMETHOD(EnableAutoTray)(BOOL) = 0;
    STDMETHOD(DoAction)(BOOL) = 0;
};

const CLSID CLSID_TrayNotify = { 0x25DEAD04, 0x1EAC, 0x4911,{ 0x9E, 0x3A, 0xAD, 0x0A, 0x4A, 0xB5, 0x60, 0xFD } };
DEFINE_GUID(IID_IShellService, 0x5836FB00L, 0x8187, 0x11CF, 0xA1, 0x2B, 0x00, 0xAA, 0x00, 0x4A, 0xE8, 0x37);
