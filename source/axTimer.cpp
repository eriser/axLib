#include "axTimer.h"

axTimer::axTimer(axEventFunction fct, int ms)
{
//    AddConnection(0, fct);
//    InitTimer(ms);
}

axTimer::axTimer()
{
    //InitTimer(ms);
}

void axTimer::StartTimer(const int& interval_ms, const int& length_ms)
{
    // Return true if the thread object identifies an active thread
    // of execution, false otherwise.
    if(_timerThread.joinable() == false)
    {
        _timerThread = std::thread(timer_thread,
                                   std::ref(*this),
                                   interval_ms,
                                   length_ms);
        _timerThread.detach();
    }
}

void axTimer::timer_thread(axTimer& timer, int interval_ms, int length_ms)
{
    int count_ms = 0;
    
    while (count_ms < length_ms)
    {
        timer.PushEvent(0, new axTimerMsg(count_ms));
        count_ms += interval_ms;
        std::this_thread::sleep_for(std::chrono::milliseconds(interval_ms));
    }
}
