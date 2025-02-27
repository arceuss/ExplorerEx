#include "uemapp.h"
#include "shundoc.h"
#include <shlwapi.h>
#include <stdio.h>

#undef  INTERFACE
#define INTERFACE   IUserAssist

typedef struct tagOBJECTINFO
{
	void* cf;
	CLSID const* pclsid;
	HRESULT(*pfnCreateInstance)(IUnknown* pUnkOuter, IUnknown** ppunk, const struct tagOBJECTINFO*);

	// for automatic registration, type library searching, etc
	int nObjectType;        // OI_ flag
	LPTSTR pszName;
	LPTSTR pszFriendlyName;
	IID const* piid;
	IID const* piidEvents;
	long lVersion;
	DWORD dwOleMiscFlags;
	int nidToolbarBitmap;
} OBJECTINFO;
typedef OBJECTINFO const* LPCOBJECTINFO;



#define UEIM_HIT        0x01
#define UEIM_FILETIME   0x02

MIDL_INTERFACE("49b36d57-5fd2-45a7-981b-06028d577a47")
IUserAssist10: public IUnknown
{
public:
	STDMETHOD(FireEvent)(REFIID, PVOID, LPWSTR, int) PURE;
	STDMETHOD(QueryEntry)(REFIID, LPWSTR, LPUEMINFO) PURE;
	//...more of whatever
};

IUserAssist10* i10 = NULL;

BOOL UEMIsLoaded()
{
	printf("UEMIsLoaded\n");
	BOOL fRet;

	fRet = GetModuleHandle(TEXT("ole32.dll")) &&
		GetModuleHandle(TEXT("browseui.dll"));

	return fRet;
}

HRESULT UEMFireEvent(const GUID* pguidGrp, int eCmd, DWORD dwFlags, WPARAM wParam, LPARAM lParam)
{
	printf("UEMFireEvent\n");
	HRESULT hr = E_FAIL;
	/*
	IUserAssist* pua;

	pua = GetUserAssist();
	if (pua) {
		hr = pua->FireEvent(pguidGrp, eCmd, dwFlags, wParam, lParam);
	}*/
	IShellFolder* ish = (IShellFolder*)wParam;
	if (!IsBadReadPtr(ish, sizeof(IShellFolder)))
	{
		LPITEMIDLIST pidl = (LPITEMIDLIST)lParam;
		STRRET psn;
		ish->GetDisplayNameOf(pidl, SHGDN_FORPARSING, &psn);
		LPWSTR psz;
		StrRetToStrW(&psn, pidl, &psz);

		if (!i10)
			HRESULT hr = CoCreateInstance(CLSID_UserAssist, NULL, CLSCTX_INPROC_SERVER | CLSCTX_INPROC_HANDLER | CLSCTX_NO_CODE_DOWNLOAD, IID_IUserAssist10, (PVOID*)&i10);

		i10->FireEvent(UAIID_SHORTCUTS,(PVOID)2, psz, GetTickCount64());
		//wprintf(L"%s;\n",psz);
		CoTaskMemFree(psz);
		hr = S_OK;
	}

	return hr;
}

HRESULT UEMQueryEvent(const GUID* pguidGrp, int eCmd, WPARAM wParam, LPARAM lParam, LPUEMINFO pui)
{
	//printf("UEMQueryEvent\n");
	HRESULT hr = E_FAIL;

	/* ass
	pua = GetUserAssist();
	if (pua) {
		hr = pua->QueryEvent(pguidGrp, eCmd, wParam, lParam, pui);
	}
	*/
	IShellFolder* ish = (IShellFolder*)wParam;
	if (!IsBadReadPtr(ish, sizeof(IShellFolder)))
	{
		LPITEMIDLIST pidl = (LPITEMIDLIST)lParam;
		STRRET psn;
		ish->GetDisplayNameOf(pidl, SHGDN_FORPARSING, &psn);
		LPWSTR psz;
		StrRetToStrW(&psn, pidl, &psz);

		if (!i10)
			HRESULT hr = CoCreateInstance(CLSID_UserAssist, NULL, CLSCTX_INPROC_SERVER | CLSCTX_INPROC_HANDLER | CLSCTX_NO_CODE_DOWNLOAD, IID_IUserAssist10, (PVOID*)&i10);

		i10->QueryEntry(UAIID_SHORTCUTS, psz, pui);
		//wprintf(L"%s; launchcount: %d\n",psz, pui->cHit);
		CoTaskMemFree(psz);
		hr = S_OK;
	}

	return hr;
}

// these are useless (doesnt get called)

HRESULT UEMSetEvent(const GUID* pguidGrp, int eCmd, WPARAM wParam, LPARAM lParam, LPUEMINFO pui)
{
	printf("UEMSetEvent\n");
	HRESULT hr = E_FAIL;
	return hr;
}

HRESULT UEMRegisterNotify(UEMCallback pfnUEMCB, void* param)
{
	printf("UEMRegisterNotify\n");
	HRESULT hr = E_UNEXPECTED;
	return hr;
}

void UEMEvalMsg(const GUID* pguidGrp, int eCmd, WPARAM wParam, LPARAM lParam)
{
	printf("UEMEvalMsg\n");
	HRESULT hr;

	hr = UEMFireEvent(pguidGrp, eCmd, UEMF_XEVENT, wParam, lParam);
	return;
}

BOOL UEMGetInfo(const GUID* pguidGrp, int eCmd, WPARAM wParam, LPARAM lParam, LPUEMINFO pui)
{
	printf("UEMGetInfo\n");
	HRESULT hr;

	hr = UEMQueryEvent(pguidGrp, eCmd, wParam, lParam, pui);
	return SUCCEEDED(hr);
}
