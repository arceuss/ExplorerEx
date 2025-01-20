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