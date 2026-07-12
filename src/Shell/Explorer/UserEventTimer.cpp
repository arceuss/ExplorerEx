#include "pch.h"
#include "cabinet.h"    // g_hinstCabinet
#include "shundoc.h"    // IUserEventTimer, IUnknown_SetSite
#include "UserEventTimer.h"

#include <new>

// Ported from Vista shell32 CUserEventTimer (d:\longhorn\shell\shell32\uevttmr.cpp).

namespace
{
    const wchar_t c_szUserEventWindowClass[] = L"ExplorerExUserEventWindow";
    const UINT_PTR IDT_USEREVENT_TICK = 1000;   // 0x3E8
    const UINT     DEFAULT_TICK_INTERVAL = 5000;
}

CUserEventTimer::CUserEventTimer()
    : _cRef(1)
    , _hwnd(nullptr)
    , _uIntervalTimerID(0)
    , _dwLastTick(0)
    , _uTickInterval(DEFAULT_TICK_INTERVAL)
{
}

CUserEventTimer::~CUserEventTimer()
{
    if (_uIntervalTimerID != 0 && _hwnd)
    {
        KillTimer(_hwnd, _uIntervalTimerID);
        _uIntervalTimerID = 0;
    }

    for (TIMERDETAILS* ptd : _timers)
    {
        _FreeTimerDetails(ptd);
    }
    _timers.clear();

    if (_hwnd)
    {
        DestroyWindow(_hwnd);
        _hwnd = nullptr;
    }
}

HRESULT CUserEventTimer::CreateInstance(REFIID riid, void** ppv)
{
    if (ppv)
        *ppv = nullptr;

    CUserEventTimer* p = new (std::nothrow) CUserEventTimer();
    if (!p)
        return E_OUTOFMEMORY;

    HRESULT hr = p->Init();
    if (SUCCEEDED(hr))
        hr = p->QueryInterface(riid, ppv);

    p->Release();
    return hr;
}

HRESULT CUserEventTimer::Init()
{
    if (!_hwnd)
    {
        WNDCLASSEXW wc = {};
        wc.cbSize = sizeof(wc);
        wc.style = CS_GLOBALCLASS;
        wc.lpfnWndProc = s_WndProc;
        wc.hInstance = g_hinstCabinet;
        wc.hCursor = LoadCursorW(nullptr, IDC_ARROW);
        wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
        wc.cbWndExtra = sizeof(void*);
        wc.lpszClassName = c_szUserEventWindowClass;
        RegisterClassExW(&wc); // ignore ERROR_CLASS_ALREADY_EXISTS

        _hwnd = CreateWindowExW(WS_EX_TOOLWINDOW, c_szUserEventWindowClass, nullptr, WS_POPUP,
                                0, 0, 0, 0, HWND_MESSAGE, nullptr, g_hinstCabinet, this);
    }

    if (!_hwnd)
        return E_FAIL;

    _uTickInterval = DEFAULT_TICK_INTERVAL;
    return S_OK;
}

LRESULT CALLBACK CUserEventTimer::s_WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    CUserEventTimer* self = reinterpret_cast<CUserEventTimer*>(GetWindowLongPtrW(hWnd, 0));
    if (uMsg == WM_CREATE)
    {
        self = reinterpret_cast<CUserEventTimer*>(reinterpret_cast<CREATESTRUCT*>(lParam)->lpCreateParams);
        self->_hwnd = hWnd;
        SetWindowLongPtrW(hWnd, 0, reinterpret_cast<LONG_PTR>(self));
    }

    if (self)
        return self->v_WndProc(hWnd, uMsg, wParam, lParam);

    return DefWindowProcW(hWnd, uMsg, wParam, lParam);
}

LRESULT CUserEventTimer::v_WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    if (uMsg != WM_TIMER)
        return DefWindowProcW(hWnd, uMsg, wParam, lParam);

    ASSERT(wParam == IDT_USEREVENT_TICK);
    _OnTimer();
    return 0;
}

STDMETHODIMP CUserEventTimer::QueryInterface(REFIID riid, void** ppv)
{
    if (!ppv)
        return E_POINTER;

    if (riid == IID_IUnknown || riid == __uuidof(IUserEventTimer))
    {
        *ppv = static_cast<IUserEventTimer*>(this);
        AddRef();
        return S_OK;
    }

    *ppv = nullptr;
    return E_NOINTERFACE;
}

STDMETHODIMP_(ULONG) CUserEventTimer::AddRef()
{
    return InterlockedIncrement(&_cRef);
}

STDMETHODIMP_(ULONG) CUserEventTimer::Release()
{
    ULONG cRef = InterlockedDecrement(&_cRef);
    if (cRef == 0)
        delete this;
    return cRef;
}

STDMETHODIMP CUserEventTimer::InitTimerTickInterval(UINT uTimerTickIntervalMs)
{
    // The tick interval may only be changed while no timers are active.
    if (!_timers.empty())
        return E_FAIL;

    _uTickInterval = uTimerTickIntervalMs ? uTimerTickIntervalMs : DEFAULT_TICK_INTERVAL;
    return S_OK;
}

int CUserEventTimer::_GetTimerDetailsIndex(HWND hWnd, UINT uTimerID)
{
    if (uTimerID == 0)
        return -1;

    for (int i = static_cast<int>(_timers.size()) - 1; i >= 0; --i)
    {
        TIMERDETAILS* p = _timers[i];
        if (p && p->hWnd == hWnd && p->uTimerID == uTimerID)
            return i;
    }
    return -1;
}

STDMETHODIMP CUserEventTimer::SetUserEventTimer(HWND hWnd, UINT uCallbackMessage, UINT uTimerElapse,
                                                IUserEventTimerCallback* pCallback, ULONG* puTimerID)
{
    if (!_hwnd)
        return E_FAIL;

    if (hWnd == nullptr && pCallback == nullptr)
        return E_INVALIDARG;

    if (puTimerID == nullptr || uTimerElapse == 0)
        return E_INVALIDARG;

    if (hWnd != nullptr)
    {
        int nIndex = _GetTimerDetailsIndex(hWnd, *puTimerID);
        if (nIndex >= 0)
            return _ResetUserEventTimer(hWnd, uTimerElapse, nIndex);
    }

    return _SetUserEventTimer(hWnd, uCallbackMessage, uTimerElapse, pCallback, puTimerID);
}

HRESULT CUserEventTimer::_SetUserEventTimer(HWND hWnd, UINT uCallbackMessage, UINT uTimerElapse,
                                            IUserEventTimerCallback* pCallback, ULONG* puTimerID)
{
    ASSERT(_uTickInterval != 0);

    TIMERDETAILS* ptd = new (std::nothrow) TIMERDETAILS();
    if (!ptd)
        return E_OUTOFMEMORY;

    ZeroMemory(ptd, sizeof(*ptd));
    ptd->hWnd = hWnd;
    if (hWnd != nullptr)
    {
        ptd->uCallbackMessage = uCallbackMessage;
        ptd->uTimerID = *puTimerID;
    }
    else
    {
        ptd->pCallback = pCallback;
    }

    // Allocate a free timer id if the caller did not supply one.
    if (ptd->uTimerID == 0)
    {
        UINT id = 4096;
        for (; id <= 0xDDDD1000; ++id)
        {
            if (_GetTimerDetailsIndex(hWnd, id) < 0)
                break;
        }
        if (id <= 0xDDDD1000)
            ptd->uTimerID = id;
    }

    HRESULT hr = E_OUTOFMEMORY;
    if (ptd->uTimerID != 0)
    {
        ptd->uTimerElapse = uTimerElapse;
        ptd->uTicksRemaining = uTimerElapse / _uTickInterval;
        ptd->fJustSet = TRUE;

        bool fInserted = false;
        try
        {
            _timers.push_back(ptd);
            fInserted = true;
            hr = S_OK;
        }
        catch (...)
        {
            hr = E_OUTOFMEMORY;
        }

        if (SUCCEEDED(hr))
        {
            *puTimerID = ptd->uTimerID;

            if (_uIntervalTimerID == 0)
                _uIntervalTimerID = SetTimer(_hwnd, IDT_USEREVENT_TICK, _uTickInterval, nullptr);

            if (_uIntervalTimerID == 0)
            {
                // Could not start the interval timer: back out the insert.
                if (fInserted)
                    _timers.pop_back();
            }
        }
    }

    if (SUCCEEDED(hr) && _uIntervalTimerID != 0)
    {
        // Callback-mode timers site themselves on us and hold a reference.
        if (hWnd == nullptr && ptd->pCallback)
        {
            IUnknown_SetSite(ptd->pCallback, static_cast<IUserEventTimer*>(this));
            ptd->pCallback->AddRef();
        }

        if (_dwLastTick == 0 && _timers.size() == 1)
            _dwLastTick = GetTickCount();

        return S_OK;
    }

    *puTimerID = 0;
    delete ptd;
    return E_FAIL;
}

HRESULT CUserEventTimer::_ResetUserEventTimer(HWND hWnd, UINT uTimerElapse, int nIndex)
{
    ASSERT(_hwnd && !_timers.empty() && hWnd && nIndex >= 0);

    TIMERDETAILS* p = _timers[nIndex];
    ASSERT(p && p->hWnd == hWnd);

    p->uTimerElapse = uTimerElapse;
    p->uTicksRemaining = uTimerElapse / _uTickInterval;
    p->fJustSet = TRUE;
    return S_OK;
}

STDMETHODIMP CUserEventTimer::GetUserEventTimerElapsed(HWND hWnd, ULONG uTimerID, UINT* puTimerElapsed)
{
    HRESULT hr = E_FAIL;

    if (puTimerElapsed && hWnd && _hwnd)
    {
        int nIndex = _GetTimerDetailsIndex(hWnd, uTimerID);
        if (nIndex < 0)
        {
            *puTimerElapsed = 0;
        }
        else
        {
            TIMERDETAILS* p = _timers[nIndex];
            hr = S_OK;
            *puTimerElapsed = _uTickInterval * (p->uTimerElapse / _uTickInterval - p->uTicksRemaining);
        }
    }

    return hr;
}

STDMETHODIMP CUserEventTimer::KillUserEventTimer(HWND hWnd, ULONG uTimerID)
{
    if (!_hwnd)
        return E_FAIL;

    HRESULT hr;
    int nIndex = _GetTimerDetailsIndex(hWnd, uTimerID);
    if (nIndex < 0)
    {
        hr = E_FAIL;
    }
    else
    {
        _FreeTimerDetails(_timers[nIndex]);
        _timers.erase(_timers.begin() + nIndex);
        _KillIntervalTimer();
        hr = S_OK;
    }

    if (_timers.empty())
        _dwLastTick = 0;

    return hr;
}

void CUserEventTimer::_KillIntervalTimer()
{
    if (_uIntervalTimerID != 0 && _timers.empty())
    {
        KillTimer(_hwnd, _uIntervalTimerID);
        _uIntervalTimerID = 0;
    }
}

void CUserEventTimer::_FreeTimerDetails(TIMERDETAILS* ptd)
{
    if (ptd)
    {
        if (ptd->hWnd == nullptr && ptd->pCallback)
        {
            IUnknown_SetSite(ptd->pCallback, nullptr);
            ptd->pCallback->Release();
        }
        delete ptd;
    }
}

void CUserEventTimer::_OnTimer()
{
    if (!_hwnd)
        return;

    // The user-event timer only advances during periods of user activity: nDelta
    // is how long after the last tick the most recent input arrived. If the user
    // was idle across the interval, nDelta == 0 and no timer counts down, so icons
    // are demoted based on active-usage time (the classic "hide inactive" model).
    int nDelta = -1;
    LASTINPUTINFO lii;
    lii.cbSize = sizeof(lii);
    lii.dwTime = 0;
    if (GetLastInputInfo(&lii))
        nDelta = (lii.dwTime >= _dwLastTick) ? static_cast<int>(lii.dwTime - _dwLastTick) : 0;

    const int nLow = static_cast<int>(static_cast<float>(_uTickInterval) * 0.89999998f);
    const int nHigh = static_cast<int>(static_cast<float>(_uTickInterval) * 1.01f);

    for (int i = static_cast<int>(_timers.size()) - 1; i >= 0; --i)
    {
        TIMERDETAILS* p = _timers[i];
        if (!p)
            continue;

        if (nDelta != 0 &&
            (nDelta == -1 ||
             (p->fJustSet != 0 && nDelta > nLow && nDelta <= nHigh) ||
             (p->fJustSet == 0 && nDelta <= nHigh)) &&
            p->uTicksRemaining-- == 1)
        {
            UINT uElapse = p->uTimerElapse;
            p->uTicksRemaining = p->uTimerElapse / _uTickInterval;

            if (p->hWnd)
                PostMessageW(p->hWnd, p->uCallbackMessage, uElapse, static_cast<LPARAM>(p->uTimerID));
            else if (p->pCallback)
                p->pCallback->UserEventTimerProc(p->uTimerID, uElapse);
        }

        p->fJustSet = 0;
    }

    _dwLastTick = GetTickCount();
}
