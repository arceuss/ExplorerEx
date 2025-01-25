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


static bool bFirstTime = true;

HRESULT CoCreateInstanceHook(REFCLSID rclsid, LPUNKNOWN pUnkOuter, DWORD dwClsContext, REFIID riid, LPVOID* ppv)
{
	HRESULT res = CoCreateInstance(rclsid, pUnkOuter, dwClsContext, riid, ppv);
	if (res != ERROR_SUCCESS)
	{
		CreateThread(0, 0, BeepThread, 0, 0, 0);

		wchar_t* clsidstring = 0;
		if (FAILED(StringFromCLSID(rclsid, &clsidstring))) return res;

		wchar_t* iidstring = 0;
		if (FAILED(StringFromCLSID(riid, &iidstring))) return res;

		wprintf(L"COCREATEINSTANCE FAILED! clsid %s, riid %s\n", clsidstring, iidstring);
		
		dbg::printstacktrace();
	}
	return res;
}
