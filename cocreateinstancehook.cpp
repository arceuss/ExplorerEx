#include "cocreateinstancehook.h"
#include <stdio.h>
#include <iostream>
#include <Dbghelp.h>
#include "dbg.h"

DWORD WINAPI BeepThread(LPVOID)
{
	Beep(1700, 200);
	return 0;
}

HRESULT CoCreateInstanceHook(REFCLSID rclsid, LPUNKNOWN pUnkOuter, DWORD dwClsContext, REFIID riid, LPVOID* ppv)
{
	HRESULT hr = CoCreateInstance(rclsid, pUnkOuter, dwClsContext, riid, ppv);
#ifndef RELEASE
	if (FAILED(hr))
	{
		CreateThread(0, 0, BeepThread, 0, 0, 0);

		wchar_t* clsidstring = 0;
		if (FAILED(StringFromCLSID(rclsid, &clsidstring))) return hr;

		wchar_t* iidstring = 0;
		if (FAILED(StringFromCLSID(riid, &iidstring)))
		{
			if (clsidstring) CoTaskMemFree(clsidstring);
			return hr;
		}

		wprintf(L"COCREATEINSTANCE FAILED! clsid %s, riid %s\n", clsidstring, iidstring);
		if (clsidstring) CoTaskMemFree(clsidstring);
		if (iidstring) CoTaskMemFree(iidstring);
		dbg::printstacktrace();
	}
#endif
	return hr;
}
