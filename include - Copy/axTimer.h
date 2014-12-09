#ifndef _AX_TIMER_
#define _AX_TIMER_

#include <iostream>
#include <thread>
#include <mutex>
#include <string>

#include "axEventManager.h"
#include "axObject.h"
//#include <pthread.h>

class axTimerMsg : public axMsg
{
public:
    
    axTimerMsg(unsigned int t) :
    _time(t)
    {
    }
    
    unsigned int GetTime() const
    {
        return _time;
    }
    
    virtual axMsg* GetCopy()
    {
        return new axTimerMsg(*this);
    }
    
private:
    unsigned int _time;
    
};


class axTimer : public axObject
{
public:
    axTimer(axEventFunction fct, int ms);
    axTimer();
    
    static void timer_thread(axTimer& timer, int interval_ms, int length_ms);
    
    void StartTimer(const int& interval_ms, const int& length_ms);
    
private:
    std::thread _timerThread;
    
    void InitTimer(int ms);
};

#endif //_AX_TIMER_
