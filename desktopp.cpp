#include "desktopp.h"

HANDLE SHCreateDesktop(IDeskTray* pdtray)
{
	static HANDLE(*fSHCreateDesktop)(IDeskTray * pdtray) = (decltype(fSHCreateDesktop))GetProcAddress(LoadLibrary(L"shell32.dll"), (LPSTR)200);
	return fSHCreateDesktop(pdtray);
}

BOOL CreateFromDesktop(PNEWFOLDERINFO pfi)
{
    return 0;
}

BOOL SHCreateFromDesktop(PNEWFOLDERINFO pfi)
{
    return 0;
}

BOOL SHDesktopMessageLoop(HANDLE hDesktop)
{
	static BOOL(*fSHDesktopMessageLoop)(HANDLE hDesktop) = (decltype(fSHDesktopMessageLoop))GetProcAddress(LoadLibrary(L"shell32.dll"), (LPSTR)201);
	return fSHDesktopMessageLoop(hDesktop);
}

BOOL SHExplorerParseCmdLine(PNEWFOLDERINFO pfi)
{
    return 0;
}

LRESULT DDEHandleMsgs(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    return LRESULT();
}

void DDEHandleTimeout(HWND hwnd)
{
}
