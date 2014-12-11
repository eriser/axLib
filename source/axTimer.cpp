#include "axTimer.h"

axTimer::axTimer(axEventFunction fct, int ms)
{
//    AddConnection(0, fct);
//    InitTimer(ms);
}

axTimer::axTimer():
_isRunning(false)
{
    //InitTimer(ms);
}

bool axTimer::IsRunning() const
{
    return _isRunning;
}

void axTimer::StartTimer(const int& interval_ms, const int& length_ms)
{
    timer_mutex.lock();
    
    _isRunning = true;
    
    timer_mutex.unlock();
    
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

void axTimer::StartTimer(const int& interval_ms)
{
    timer_mutex.lock();
    
    _isRunning = true;
    
    timer_mutex.unlock();
    
    // Return true if the thread object identifies an active thread
    // of execution, false otherwise.
    if(_timerThread.joinable() == false)
    {
        _timerThread = std::thread(timer_thread_no_end,
                                   std::ref(*this),
                                   interval_ms);
        _timerThread.detach();
    }
}

void axTimer::StopTimer()
{
    timer_mutex.lock();
    _isRunning = false;
    timer_mutex.unlock();
}

void axTimer::timer_thread_no_end(axTimer& timer, int interval_ms)
{
    int count_ms = 0;
    
    while (timer.IsRunning())
    {
        timer.PushEvent(0, new axTimerMsg(count_ms));
        count_ms += interval_ms;
        std::this_thread::sleep_for(std::chrono::milliseconds(interval_ms));
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
