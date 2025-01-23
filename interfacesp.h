#pragma once
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
    STDMETHOD(RegisterHotKey)(THIS_ IShellFolder* psf, LPCITEMIDLIST pidlParent, LPCITEMIDLIST pidl) PURE;
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

DECLARE_INTERFACE_(IDeskTray, IUnknown)
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

MIDL_INTERFACE("6F51C646-0EFE-4370-882A-C1F61CB27C3B")
IShellMenu2 : IShellMenu
{
    // Retrieves an interface on a submenu.
    STDMETHOD(GetSubMenu)(THIS_ UINT idCmd, REFIID riid, void** ppvObj) PURE;
    STDMETHOD(SetToolbar)(THIS_ HWND hwnd, DWORD dwFlags) PURE;
    STDMETHOD(SetMinWidth)(THIS_ int cxMenu) PURE;
    STDMETHOD(SetNoBorder)(THIS_ BOOL fNoBorder) PURE;
    STDMETHOD(SetTheme)(THIS_ LPCWSTR pszTheme) PURE;
};

MIDL_INTERFACE("EC35E37A-6579-4F3C-93CD-6E62C4EF7636")
IStartMenuPin : IUnknown
{
    #define SMPIN_POS(i) (LPCITEMIDLIST)MAKEINTRESOURCE((i)+1))
    #define SMPINNABLE_EXEONLY          0x00000001) // allow only EXEs to be pinned
    #define SMPINNABLE_REJECTSLOWMEDIA  0x00000002) // reject slow media

    STDMETHOD(EnumObjects)(THIS_ IEnumIDList * *ppenumIDList) PURE;
    //
    //  Pin:        pidlFrom = NULL, pidlTo = pidl
    //  Unpin:      pidlFrom = pidl, pidlTo = NULL
    //  Update:     pidlFrom = old,  pidlTo = new
    //  Move:       pidlFrom = pidl, pidlTo = SMPINPOS(iPos)
    STDMETHOD(Modify)(THIS_ LPCITEMIDLIST pidlFrom, LPCITEMIDLIST pidlTo) PURE;
    STDMETHOD(GetChangeCount)(THIS_ ULONG* pulOut) PURE;

    //
    //  pdto = data object to test
    //  dwFlags is an SMPINNABLE_* flag
    //  *ppidl receives pidl being pinned
    //
    STDMETHOD(IsPinnable)(THIS_ IDataObject* pdto, DWORD dwFlags, LPITEMIDLIST* ppidl) PURE; // S_FALSE if not

    //
    //  Find the pidl on the pin list and resolve the shortcut that
    //  tracks it.
    //
    //  Returns S_OK if the pidl changed and was resolved.
    //  Returns S_FALSE if the pidl did not change.
    //  Returns an error if the Resolve failed.
    //
    STDMETHOD(Resolve)(THIS_ HWND hwnd, DWORD dwFlags, LPCITEMIDLIST pidl, LPITEMIDLIST* ppidlResolved) PURE;
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

MIDL_INTERFACE("FADB55B4-D382-4FC4-81D7-ABB325C7F12A")
IFadeTask : IUnknown
{
    STDMETHOD(FadeRect)(THIS_ LPCRECT prc) PURE;
};

MIDL_INTERFACE("E9EAD8E6-2A25-410E-9B58-A9fBEF1DD1A2")
IUserEventTimerCallback: IUnknown
{
    STDMETHOD(UserEventTimerProc)(THIS_ ULONG uUserEventTimerID, UINT uTimerElapse) PURE;
};

MIDL_INTERFACE("0F504B94-6E42-42E6-99E0-E20FAFE52AB4")
IUserEventTimer: IUnknown
{
    STDMETHOD(SetUserEventTimer)(THIS_ HWND hWnd, UINT uCallbackMessage, UINT uTimerElapse, IUserEventTimerCallback * pUserEventTimerCallback, ULONG * puUserEventTimerID) PURE;
    STDMETHOD(KillUserEventTimer)(THIS_ HWND hWnd, ULONG uUserEventTimerID) PURE;

    STDMETHOD(GetUserEventTimerElapsed)(THIS_ HWND hWnd, ULONG uUserEventTimerID, UINT* puTimerElapsed) PURE;
    STDMETHOD(InitTimerTickInterval)(THIS_ UINT uTimerTickIntervalMs) PURE;
};

MIDL_INTERFACE("D782CCBA-AFB0-43F1-94DB-FDA3779EACCB")
INotificationCB : IUnknown
{
    STDMETHOD(Notify)(THIS_ ULONG, NOTIFYITEM*) PURE;
};

MIDL_INTERFACE("FB852B2C-6BAD-4605-9551-F15F87830935")
ITrayNotify : IUnknown
{
    STDMETHOD(RegisterCallback)(INotificationCB * callback) PURE;
    STDMETHOD(SetPreference)(const NOTIFYITEM* notify_item) PURE;
    STDMETHOD(EnableAutoTray)(BOOL enabled) PURE;
};

//this is the Windows 8+ variant of ITrayNotify, probably not needed for now but might need it for later
MIDL_INTERFACE("D133CE13-3537-48BA-93A7-AFCD5D2053B4")
ITrayNotifyWin8 : IUnknown
{
    STDMETHOD(RegisterCallback)(INotificationCB * callback, ULONG*) PURE;
    STDMETHOD(UnregisterCallback)(ULONG*) PURE;
    STDMETHOD(SetPreference)(NOTIFYITEM const*) PURE;
    STDMETHOD(EnableAutoTray)(BOOL) PURE;
    STDMETHOD(DoAction)(BOOL) PURE;
};