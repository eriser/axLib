#include "axTimer.h"

axTimer::axTimer(axEventFct fct, int ms)
{
	AddConnection(fct);
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
							   std::ref(*axEventManager::Instance()),
							   std::ref(*this),
							   GetID(), ms);
}

void axTimer::timer_thread(axEventManager& evtManager,
						   axTimer& timer,
						   axID id,
						   int ms)
{
	while (1)
	{
		
		axMsg* msg = axTimerMsg::CreateTimerMsg("Timer", 
					 timer._ms_since_timer_start);
		
		timer._ms_since_timer_start += ms;
		evtManager.PushEvent(id, msg);
		std::this_thread::sleep_for(std::chrono::milliseconds(ms));
	}
}