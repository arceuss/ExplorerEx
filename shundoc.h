//+-------------------------------------------------------------------------
//
//  TaskMan - NT TaskManager
//  Copyright (C) Microsoft
//
//  File:       shundoc.h
//
//  History:    Oct-11-24   aubymori  Created
//
//--------------------------------------------------------------------------
#pragma once
#include <Windows.h>

//
// Macros
//
#define NT_SUCCESS(Status) (((NTSTATUS)(Status)) >= 0)

#define CMF_ICM3                0x00020000      // QueryContextMenu can assume IContextMenu3 semantics (i.e.,
                                                // will receive WM_INITMENUPOPUP, WM_MEASUREITEM, WM_DRAWITEM,
                                                // and WM_MENUCHAR, via HandleMenuMsg2)


//
// Function definitions
//

extern HRESULT(STDMETHODCALLTYPE* IUnknown_Exec)(IUnknown* punk, const GUID* pguidCmdGroup, DWORD nCmdID, DWORD nCmdexecopt, VARIANTARG* pvarargIn, VARIANTARG* pvarargOut);
extern HRESULT(STDMETHODCALLTYPE* IUnknown_GetClassID)(IUnknown* punk, CLSID* pclsid);

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

