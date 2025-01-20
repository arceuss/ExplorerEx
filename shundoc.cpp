//+-------------------------------------------------------------------------
//
//  TaskMan - NT TaskManager
//  Copyright (C) Microsoft
//
//  File:       shundoc.cpp
//
//  History:    Oct-11-24   aubymori  Created
//
//--------------------------------------------------------------------------
#include "shundoc.h"
#include "Windows.h"

//
// Function definitions
// 

HRESULT(STDMETHODCALLTYPE* IUnknown_Exec)(IUnknown* punk, const GUID* pguidCmdGroup, DWORD nCmdID, DWORD nCmdexecopt, VARIANTARG* pvarargIn, VARIANTARG* pvarargOut) = nullptr;
HRESULT(STDMETHODCALLTYPE* IUnknown_GetClassID)(IUnknown* punk, CLSID* pclsid) = nullptr;


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

bool SHUndocInit(void)
{

	LOAD_MODULE(shlwapi);
	LOAD_ORDINAL(shlwapi, IUnknown_Exec, 164);

	LOAD_MODULE(shcore);
	LOAD_ORDINAL(shcore, IUnknown_GetClassID, 142);

	return true;
}