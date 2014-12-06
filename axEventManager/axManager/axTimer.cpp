#include "axTimer.h"

axTimer::axTimer(axEventFunction fct, int ms)
{
    AddConnection(0, fct);
    InitTimer(ms);
}

axTimer::axTimer(int ms)
{
    InitTimer(ms);
}

void axTimer::InitTimer(int ms)
{
    _ms_since_timer_start = 0;
    
    _timerThread = std::thread(timer_thread,
                               std::ref(*axEventManager::GetInstance()),
                               std::ref(*this),
                               ms);
}

void axTimer::timer_thread(axEventManager& evtManager,
                           axTimer& timer,
                           int ms)
{
    while (1)
    {
        axMsg* msg = new axTimerMsg(timer._ms_since_timer_start);
        timer._ms_since_timer_start += ms;
        timer.PushEvent(0, msg);
        std::this_thread::sleep_for(std::chrono::milliseconds(ms));
    }
}