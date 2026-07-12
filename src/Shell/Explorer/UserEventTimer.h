#pragma once

#include "shundoc.h" // IUserEventTimer / IUserEventTimerCallback
#include <vector>

//
// Local implementation of the classic-shell notification-area idle timer.
//
// The auto-tray ("hide inactive icons") demotion engine drives itself off an
// IUserEventTimer obtained from CLSID_UserEventTimer. Windows 7+ redesigned the
// notification area and the system shell32's CUserEventTimer no longer counts
// down / fires the demote callback (it instantiates and accepts SetUserEventTimer
// but its elapsed counter never advances), so on modern Windows the feature is
// inert. This class reimplements the Vista shell32 CUserEventTimer
// (d:\longhorn\shell\shell32\uevttmr.cpp) so demotion works without the OS object.
//
class CUserEventTimer : public IUserEventTimer
{
public:
    static HRESULT CreateInstance(REFIID riid, void** ppv);

    // IUnknown
    STDMETHODIMP QueryInterface(REFIID riid, void** ppv) override;
    STDMETHODIMP_(ULONG) AddRef() override;
    STDMETHODIMP_(ULONG) Release() override;

    // IUserEventTimer
    STDMETHODIMP SetUserEventTimer(HWND hWnd, UINT uCallbackMessage, UINT uTimerElapse,
                                   IUserEventTimerCallback* pCallback, ULONG* puTimerID) override;
    STDMETHODIMP KillUserEventTimer(HWND hWnd, ULONG uTimerID) override;
    STDMETHODIMP GetUserEventTimerElapsed(HWND hWnd, ULONG uTimerID, UINT* puTimerElapsed) override;
    STDMETHODIMP InitTimerTickInterval(UINT uTimerTickIntervalMs) override;

private:
    CUserEventTimer();
    ~CUserEventTimer();

    HRESULT Init();

    struct TIMERDETAILS
    {
        HWND                     hWnd;
        UINT                     uCallbackMessage;
        UINT                     uTimerID;
        UINT                     uTimerElapse;
        UINT                     uTicksRemaining;
        IUserEventTimerCallback* pCallback;
        BOOL                     fJustSet;
    };

    static LRESULT CALLBACK s_WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    LRESULT v_WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

    void    _OnTimer();
    int     _GetTimerDetailsIndex(HWND hWnd, UINT uTimerID);
    HRESULT _SetUserEventTimer(HWND hWnd, UINT uCallbackMessage, UINT uTimerElapse,
                               IUserEventTimerCallback* pCallback, ULONG* puTimerID);
    HRESULT _ResetUserEventTimer(HWND hWnd, UINT uTimerElapse, int nIndex);
    void    _KillIntervalTimer();
    static void _FreeTimerDetails(TIMERDETAILS* ptd);

    LONG                        _cRef;
    HWND                        _hwnd;
    UINT_PTR                    _uIntervalTimerID;
    DWORD                       _dwLastTick;
    UINT                        _uTickInterval;
    std::vector<TIMERDETAILS*>  _timers;
};
