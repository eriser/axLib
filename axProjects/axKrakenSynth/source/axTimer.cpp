/*******************************************************************************
 * Copyright (c) 2013 Alexandre Arsenault.
 *
 * This file is part of axLibrary.
 *
 * axLibrary is free or commercial software: you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 or any later version of the
 * License or use a commercial axLibrary License.
 *
 * axLibrary is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with axLibrary. If not, see <http://www.gnu.org/licenses/>.
 *
 * To release a closed-source product which uses axLibrary, commercial
 * licenses are available, email alx.arsenault@gmail.com for more information.
 ******************************************************************************/
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
