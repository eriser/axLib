#ifndef _AX_TIMER_
#define _AX_TIMER_

#include <iostream>
#include <thread>
#include <mutex>

#include "axEventManager.h"
#include "axObject.h"

class  axTimerMsg : public axMsg
{
public:
	static axMsg* CreateTimerMsg(string c, unsigned int t)
	{
		return (axMsg*) new axTimerMsg(c, t);
	}

	axTimerMsg(string c, unsigned int t) : 
		axMsg(c), _time(t){}

	unsigned int GetTime() const
	{
		return _time;
	}
	
	virtual axMsg* CopyChildFromParentPointer()
	{
		return CreateTimerMsg(GetStringMsg(), GetTime());
	}

private:
	unsigned int _time;
};


class axTimer : public axObject
{
public:
	axTimer(axEventFct fct, int ms);
	axTimer(int ms);

	static void timer_thread(axEventManager& evtManager,
		                     axTimer& timer,
							 axID id,
							 int ms);
	
private:
	std::thread _timerThread;
	unsigned int _ms_since_timer_start;

	void InitTimer(int ms);
};

#endif //_AX_TIMER_
