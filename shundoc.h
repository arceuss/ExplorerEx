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


//
// Structs
// 
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

#define NT_SUCCESS(Status) (((NTSTATUS)(Status)) >= 0)
#define ATOMICRELEASE(p) IUnknown_AtomicRelease((void **)&p)
#define ResultFromShort(i)  ResultFromScode(MAKE_SCODE(SEVERITY_SUCCESS, 0, (USHORT)(i)))
#define SAFECAST(_obj, _type) (((_type)(_obj)==(_obj)?0:0), (_type)(_obj))
#define BOOLIFY(expr)           (!!(expr))
#define IntToPtr_(T, i) ((T)IntToPtr(i))

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

#define CWM_TASKBARWAKEUP               (WM_USER + 26) // Used to restore tray thread to normal priority in extremely stressed machines
#define DTM_RAISE                       (WM_USER + 83)
#define DTRF_RAISE      0

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

#define ABE_MAX         4

//
// Function definitions
//

extern HRESULT(STDMETHODCALLTYPE* IUnknown_Exec)(IUnknown* punk, const GUID* pguidCmdGroup, DWORD nCmdID, DWORD nCmdexecopt, VARIANTARG* pvarargIn, VARIANTARG* pvarargOut);
extern HRESULT(STDMETHODCALLTYPE* IUnknown_GetClassID)(IUnknown* punk, CLSID* pclsid);
extern HRESULT(STDMETHODCALLTYPE* IUnknown_OnFocusChangeIS)(IUnknown* punk, IUnknown* punkSrc, BOOL fSetFocus);
STDAPI IUnknown_DragEnter(IUnknown* punk, IDataObject* pdtobj, DWORD grfKeyState, POINTL pt, DWORD* pdwEffect);
STDAPI IUnknown_DragOver(IUnknown* punk, DWORD grfKeyState, POINTL pt, DWORD* pdwEffect);
STDAPI IUnknown_DragLeave(IUnknown* punk);

extern HRESULT(STDMETHODCALLTYPE* SHPropagateMessage)(HWND hwndParent, UINT uMsg, WPARAM wParam, LPARAM lParam, int iFlags);
extern HRESULT(STDMETHODCALLTYPE* SHGetUserDisplayName)(LPWSTR pszDisplayName, PULONG uLen);
extern HRESULT(STDMETHODCALLTYPE* SHGetUserPicturePath)(LPCWSTR pszUsername, DWORD dwFlags, LPWSTR pszPath, DWORD cchPathMax);
extern HRESULT(STDMETHODCALLTYPE* SHSetWindowBits)(HWND hwnd, int iWhich, DWORD dwBits, DWORD dwValue);
extern HRESULT(STDMETHODCALLTYPE* SHRunIndirectRegClientCommand)(HWND hwnd, LPCWSTR pszClient);
extern UINT(STDMETHODCALLTYPE* SHGetCurColorRes)(void);
COLORREF(STDMETHODCALLTYPE* SHFillRectClr)(HDC hdc, LPRECT lprect, COLORREF color);
STDAPI_(void) SHAdjustLOGFONT(IN OUT LOGFONT* plf);
STDAPI_(BOOL) SHAreIconsEqual(HICON hIcon1, HICON hIcon2);

inline unsigned __int64 _FILETIMEtoInt64(const FILETIME* pft);
inline void SetFILETIMEfromInt64(FILETIME* pft, unsigned __int64 i64);
inline void IncrementFILETIME(FILETIME* pft, unsigned __int64 iAdjust);
inline void DecrementFILETIME(FILETIME* pft, unsigned __int64 iAdjust);

typedef HANDLE LPSHChangeNotificationLock;
typedef BOOL* BOOL_PTR;

//
// Function loader
//
bool SHUndocInit(void);

//
// COM Interfaces
// 
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

MIDL_INTERFACE("D782CCBA-AFB0-43F1-94DB-FDA3779EACCB") INotificationCB : public IUnknown
{
public:
    BEGIN_INTERFACE
    virtual HRESULT STDMETHODCALLTYPE Notify(ULONG, NOTIFYITEM*) = 0;
    END_INTERFACE
};

MIDL_INTERFACE("FB852B2C-6BAD-4605-9551-F15F87830935") ITrayNotify : public IUnknown
{
public:
    BEGIN_INTERFACE
    virtual HRESULT STDMETHODCALLTYPE RegisterCallback(INotificationCB * callback) = 0;
    virtual HRESULT STDMETHODCALLTYPE SetPreference(const NOTIFYITEM* notify_item) = 0;
    virtual HRESULT STDMETHODCALLTYPE EnableAutoTray(BOOL enabled) = 0;
    END_INTERFACE
};

//this is the Windows 8+ variant of ITrayNotify, probably not needed for now but might need it for later
MIDL_INTERFACE("D133CE13-3537-48BA-93A7-AFCD5D2053B4") ITrayNotifyWin8 : public IUnknown
{
public:
    BEGIN_INTERFACE
    virtual HRESULT STDMETHODCALLTYPE RegisterCallback(INotificationCB * callback, ULONG*) = 0;
    virtual HRESULT STDMETHODCALLTYPE UnregisterCallback(ULONG*) = 0;
    virtual HRESULT STDMETHODCALLTYPE SetPreference(NOTIFYITEM const*) = 0;
    virtual HRESULT STDMETHODCALLTYPE EnableAutoTray(BOOL) = 0;
    virtual HRESULT STDMETHODCALLTYPE DoAction(BOOL) = 0;
    END_INTERFACE
};

const CLSID CLSID_TrayNotify = { 0x25DEAD04, 0x1EAC, 0x4911,{ 0x9E, 0x3A, 0xAD, 0x0A, 0x4A, 0xB5, 0x60, 0xFD } };
