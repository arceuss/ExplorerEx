#include "runtask.h"

ULONG __stdcall CRunnableTask::AddRef(void)
{
    return 0;
}

ULONG __stdcall CRunnableTask::Release(void)
{
    return 0;
}

HRESULT __stdcall CRunnableTask::QueryInterface(REFIID riid, LPVOID* ppvObj)
{
    return E_NOTIMPL;
}

HRESULT __stdcall CRunnableTask::Run(void)
{
    return E_NOTIMPL;
}

HRESULT __stdcall CRunnableTask::Kill(BOOL bWait)
{
    return E_NOTIMPL;
}

HRESULT __stdcall CRunnableTask::Suspend(void)
{
    return E_NOTIMPL;
}

HRESULT __stdcall CRunnableTask::Resume(void)
{
    return E_NOTIMPL;
}

ULONG __stdcall CRunnableTask::IsRunning(void)
{
    return 0;
}

CRunnableTask::CRunnableTask(DWORD dwFlags)
{
}

CRunnableTask::~CRunnableTask()
{
}
