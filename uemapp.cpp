#include "uemapp.h"

BOOL UEMIsLoaded()
{
    return 0;
}

HRESULT UEMFireEvent(const GUID* pguidGrp, int eCmd, DWORD dwFlags, WPARAM wParam, LPARAM lParam)
{
    return E_NOTIMPL;
}

HRESULT UEMQueryEvent(const GUID* pguidGrp, int eCmd, WPARAM wParam, LPARAM lParam, LPUEMINFO pui)
{
    return E_NOTIMPL;
}

HRESULT UEMSetEvent(const GUID* pguidGrp, int eCmd, WPARAM wParam, LPARAM lParam, LPUEMINFO pui)
{
    return E_NOTIMPL;
}

HRESULT UEMRegisterNotify(UEMCallback pfnUEMCB, void* param)
{
    return E_NOTIMPL;
}

void UEMEvalMsg(const GUID* pguidGrp, int uemCmd, WPARAM wParam, LPARAM lParam)
{
}

BOOL UEMGetInfo(const GUID* pguidGrp, int eCmd, WPARAM wParam, LPARAM lParam, LPUEMINFO pui)
{
    return 0;
}
