#include "cowsite.h"

STDMETHODIMP_(HRESULT __stdcall) CObjectWithSite::SetSite(IUnknown* punkSite)
{
    return E_NOTIMPL;
}

STDMETHODIMP_(HRESULT __stdcall) CObjectWithSite::GetSite(REFIID riid, void** ppvSite)
{
    return E_NOTIMPL;
}

STDMETHODIMP_(HRESULT __stdcall) CSafeServiceSite::QueryInterface(REFIID riid, void** ppv)
{
    return E_NOTIMPL;
}

STDMETHODIMP_(ULONG __stdcall) CSafeServiceSite::AddRef()
{
    return 0;
}

STDMETHODIMP_(ULONG __stdcall) CSafeServiceSite::Release()
{
    return 0;
}

STDMETHODIMP_(HRESULT __stdcall) CSafeServiceSite::QueryService(REFGUID guidService, REFIID riid, void** ppvObj)
{
    return E_NOTIMPL;
}

HRESULT CSafeServiceSite::SetProviderWeakRef(IServiceProvider* psp)
{
    return E_NOTIMPL;
}
