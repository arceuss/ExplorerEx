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
IUnknown* g_uempUaSingleton;
DECLARE_INTERFACE_(IUserAssist, IUnknown)
{
	// *** IUnknown methods ***
	STDMETHOD(QueryInterface) (THIS_ REFIID riid, void** ppv) PURE;
	STDMETHOD_(ULONG, AddRef) (THIS)  PURE;
	STDMETHOD_(ULONG, Release) (THIS) PURE;

	// *** IUserAssist methods ***
	STDMETHOD(FireEvent)(THIS_ const GUID * pguidGrp, int eCmd, DWORD dwFlags, WPARAM wParam, LPARAM lParam) PURE;
	STDMETHOD(QueryEvent)(THIS_ const GUID * pguidGrp, int eCmd, WPARAM wParam, LPARAM lParam, LPUEMINFO pui) PURE;
	STDMETHOD(SetEvent)(THIS_ const GUID * pguidGrp, int eCmd, WPARAM wParam, LPARAM lParam, LPUEMINFO pui) PURE;
};

MIDL_INTERFACE("49b36d57-5fd2-45a7-981b-06028d577a47")
IUserAssist10: public IUnknown
{
public:
	STDMETHOD(FireEvent)(REFIID, PVOID, LPWSTR, int) PURE;
	STDMETHOD(QueryEntry)(REFIID, LPWSTR, LPUEMINFO) PURE;
	//...more of whatever
};


class CUserAssist : public IUserAssist
{
public:
	//*** IUnknown
	virtual STDMETHODIMP QueryInterface(REFIID riid, LPVOID* ppv);
	virtual STDMETHODIMP_(ULONG) AddRef(void);
	virtual STDMETHODIMP_(ULONG) Release(void);

	//*** IUserAssist
	virtual STDMETHODIMP FireEvent(const GUID* pguidGrp, int eCmd, DWORD dwFlags, WPARAM wParam, LPARAM lParam);
	virtual STDMETHODIMP QueryEvent(const GUID* pguidGrp, int eCmd, WPARAM wParam, LPARAM lParam, LPUEMINFO pui);
	virtual STDMETHODIMP SetEvent(const GUID* pguidGrp, int eCmd, WPARAM wParam, LPARAM lParam, LPUEMINFO pui);

protected:
	CUserAssist();
	HRESULT Initialize();
	virtual ~CUserAssist();
	friend HRESULT CUserAssist_CI2(IUnknown* pUnkOuter, IUnknown** ppunk, LPCOBJECTINFO poi);
	friend void CUserAssist_CleanUp(DWORD dwReason, void* lpvReserved);

	friend HRESULT UEMRegisterNotify(UEMCallback pfnUEMCB, void* param);

	HRESULT _InitLock();
	HRESULT _Lock();
	HRESULT _Unlock();

	void FireNotify(const GUID* pguidGrp, int eCmd)
	{
		// Assume that we have the lock
		if (_pfnNotifyCB)
			_pfnNotifyCB(_param, pguidGrp, eCmd);
	}

	HRESULT RegisterNotify(UEMCallback pfnUEMCB, void* param)
	{
		HRESULT hr;
		int cTries = 0;
		do
		{
			cTries++;
			hr = _Lock();
			if (SUCCEEDED(hr))
			{
				_pfnNotifyCB = pfnUEMCB;
				_param = param;
				_Unlock();
			}
			else
			{
				::Sleep(100); // wait some for the lock to get freed up
			}
		} while (FAILED(hr) && cTries < 20);
		return hr;
	}

private:
	LONG    _cRef;

	HANDLE  _hLock;

	UEMCallback _pfnNotifyCB;
	void* _param;
	IUserAssist10* i10;

};


CUserAssist::CUserAssist()
{

}

HRESULT CUserAssist::Initialize()
{
	HRESULT hr = CoCreateInstance(CLSID_UserAssist, NULL, CLSCTX_INPROC_SERVER || CLSCTX_INPROC_HANDLER || CLSCTX_NO_CODE_DOWNLOAD, IID_IUserAssist10, (PVOID*)&i10);
	return hr;
}

CUserAssist::~CUserAssist()
{
	if (i10)
		i10->Release();
}

HRESULT CUserAssist::_InitLock()
{
	return E_NOTIMPL;
}

HRESULT CUserAssist::_Lock()
{
	return E_NOTIMPL;
}

HRESULT CUserAssist::_Unlock()
{
	return E_NOTIMPL;
}

HRESULT CUserAssist_CI2(IUnknown* pUnkOuter, IUnknown** ppunk, LPCOBJECTINFO poi)
{
	CUserAssist* p = new CUserAssist();

	if (p && FAILED(p->Initialize())) 
	{
		wprintf(L"it failed\n");
		delete p;
		p = NULL;
	}

	if (p) {
		*ppunk = SAFECAST(p, IUserAssist*);
		return S_OK;
	}

	*ppunk = NULL;
	return E_OUTOFMEMORY;
}

void CUserAssist_CleanUp(DWORD dwReason, void* lpvReserved)
{
}

HRESULT CUserAssist_CreateInstance(IUnknown* pUnkOuter, IUnknown** ppunk, LPCOBJECTINFO poi)
{
	HRESULT hr = E_FAIL;

	if (g_uempUaSingleton == 0) {
		IUnknown* pua;

		hr = CUserAssist_CI2(pUnkOuter, &pua, poi);
		if (pua)
		{
			ENTERCRITICAL;
			if (g_uempUaSingleton == 0)
			{
				// Now the global owns the ref.
				g_uempUaSingleton = pua;    // xfer refcnt
				pua = NULL;
			}
			LEAVECRITICAL;
			if (pua)
			{
				// somebody beat us.
				// free up the 2nd one we just created, and use new one
				//TraceMsg(DM_UEMTRACE, "sl.cua_ci: undo race");
				pua->Release();
			}

			// Now, the caller gets it's own ref.
			g_uempUaSingleton->AddRef();
			//TraceMsg(DM_UEMTRACE, "sl.cua_ci: create pua=0x%x g_uempUaSingleton=%x", pua, g_uempUaSingleton);
		}
	}
	else {
		g_uempUaSingleton->AddRef();
	}

	//TraceMsg(DM_UEMTRACE, "sl.cua_ci: ret g_uempUaSingleton=0x%x", g_uempUaSingleton);
	*ppunk = g_uempUaSingleton;
	return *ppunk ? S_OK : hr;
}

IUserAssist* g_uempUa;      // 0:uninit, -1:failed, o.w.:cached obj

//***   GetUserAssist -- get (and create) cached UAssist object
//
IUserAssist* GetUserAssist()
{
	IUserAssist* pua = NULL;

	if (g_uempUa == 0)
	{
		// re: CLSCTX_NO_CODE_DOWNLOAD
		// an ('impossible') failed CCI of UserAssist is horrendously slow.
		// e.g. click on the start menu, wait 10 seconds before it pops up.
		// we'd rather fail than hose perf like this, plus this class should
		// never be remote.
		// FEATURE: there must be a better way to tell if CLSCTX_NO_CODE_DOWNLOAD
		// is supported, i've sent mail to 'com' to find out...

		CUserAssist_CreateInstance(0, (IUnknown**)&pua, 0);
		//hr = THR(CoCreateInstance(CLSID_UserAssist, NULL, dwFlags, IID_IUserAssist, (void**)&pua));

		//ENTERCRITICAL;
		if (g_uempUa == 0) {
			g_uempUa = pua;     // xfer refcnt (if any)
			if (!pua) {
				// mark it failed so we won't try any more
				g_uempUa = (IUserAssist*)-1;
			}
			pua = NULL;
		}
		//LEAVECRITICAL;
		if (pua)
			pua->Release();
		//TraceMsg(DM_UASSIST, "sl.gua: pua=0x%x g_uempUa=%x", pua, g_uempUa);
	}

	return (g_uempUa == (IUserAssist*)-1) ? 0 : g_uempUa;
}

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
	IUserAssist* pua;

	pua = GetUserAssist();
	if (pua) {
		hr = pua->FireEvent(pguidGrp, eCmd, dwFlags, wParam, lParam);
	}
	return hr;
}
IUserAssist10* i10 = NULL;

HRESULT UEMQueryEvent(const GUID* pguidGrp, int eCmd, WPARAM wParam, LPARAM lParam, LPUEMINFO pui)
{
	printf("UEMQueryEvent\n");
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
			HRESULT hr = CoCreateInstance(CLSID_UserAssist, NULL, CLSCTX_INPROC_SERVER || CLSCTX_INPROC_HANDLER || CLSCTX_NO_CODE_DOWNLOAD, IID_IUserAssist10, (PVOID*)&i10);

		i10->QueryEntry(UAIID_SHORTCUTS, psz, pui);
		//wprintf(L"%s; launchcount: %d\n",psz, pui->cHit);
		CoTaskMemFree(psz);
		return S_OK;
	}

	return hr;
}

HRESULT UEMSetEvent(const GUID* pguidGrp, int eCmd, WPARAM wParam, LPARAM lParam, LPUEMINFO pui)
{
	printf("UEMSetEvent\n");

	HRESULT hr = E_FAIL;
	IUserAssist* pua;

	pua = GetUserAssist();
	if (pua) {
		hr = pua->SetEvent(pguidGrp, eCmd, wParam, lParam, pui);
	}
	return hr;
}

HRESULT UEMRegisterNotify(UEMCallback pfnUEMCB, void* param)
{
	printf("UEMRegisterNotify\n");
	HRESULT hr = E_UNEXPECTED;
	if (g_uempUaSingleton)
	{
		CUserAssist* pua = reinterpret_cast<CUserAssist*>(g_uempUaSingleton);
		hr = pua->RegisterNotify(pfnUEMCB, param);
	}
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

HRESULT __stdcall CUserAssist::QueryInterface(REFIID riid, LPVOID* ppv)
{
	return E_NOTIMPL;
}

ULONG __stdcall CUserAssist::AddRef(void)
{
	return 0;
}

ULONG __stdcall CUserAssist::Release(void)
{
	return 0;
}

HRESULT __stdcall CUserAssist::FireEvent(const GUID* pguidGrp, int eCmd, DWORD dwFlags, WPARAM wParam, LPARAM lParam)
{

	return E_NOTIMPL;
}

HRESULT __stdcall CUserAssist::QueryEvent(const GUID* pguidGrp, int eCmd, WPARAM wParam, LPARAM lParam, LPUEMINFO pui)
{
	// UAIID_SHORTCUTS
	// 49
	// IShellFolder
	// PIDL
	// ueminfo
	
	/*
	if (i10)
	{
		i10->QueryEntry(UAIID_SHORTCUTS,  )
	}
	*/
	return E_NOTIMPL;
}

HRESULT __stdcall CUserAssist::SetEvent(const GUID* pguidGrp, int eCmd, WPARAM wParam, LPARAM lParam, LPUEMINFO pui)
{
	return E_NOTIMPL;
}

