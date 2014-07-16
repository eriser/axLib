#ifndef _AX_MANAGER_
#define _AX_MANAGER_

#include <iostream>               
#include <thread>        
#include <deque>         
#include <mutex>
#include <map>

#include "axObject.h"

typedef pair<axID, axEventFct> axFctEventMapPair;
typedef std::multimap<axID, axEventFct>::iterator axEventMapIterator;

extern std::mutex manager_mutex;

// Singleton.
class axEventManager
{
public:
	static axEventManager* Instance();

	void AddConnection(const axID& id, axEventFct fct);
	void PushEvent(const axID& id, axMsg* msg);
	void CallNext();

private:
	axEventManager();
	void AddFunction(axBindedEvent fct);

	static axEventManager* _instance;

	std::deque<axBindedEvent> _fcts;
	std::multimap<axID, axEventFct> _event_fct_map;
};

#endif //_AX_MANAGER_