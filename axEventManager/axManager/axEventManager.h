//
//  axEventManager.h
//  axManager
//
//  Created by Alexandre Arsenault on 2014-12-05.
//  Copyright (c) 2014 Alexandre Arsenault. All rights reserved.
//

#ifndef __axManager__axEventManager__
#define __axManager__axEventManager__


#include <deque>
#include <map>
#include <thread>
#include <mutex>
#include <vector>
#include "axEvent.h"
#include "axObject.h"

extern std::mutex manager_mutex;


//typedef std::multimap<axID, axEventFunction>::iterator axEventMapIterator;

typedef std::multimap<axEventId, axEventFunction> axEventMultimap;
typedef std::pair<const axEventId, axEventFunction> axEventMultimapPair;
typedef axEventMultimap::iterator axEventMultimapIterator;

typedef std::map<axID, axEventMultimap> axEventMap;
typedef std::pair<axID, axEventMultimap> axEventMapPair;
typedef axEventMap::iterator axEventMapIterator;

typedef std::deque<axBindedEvent> axEventQueue;

class axEventManager
{
public:
    static axEventManager* GetInstance();
    
    void AddConnection(const axID& id,
                       const axEventId& evtId,
                       axEventFunction fct);
    
    void PushEvent(const axID& id,
                   const axEventId& evtId,
                   axMsg* msg);
    
    void CallNext();
    
private:
    axEventManager();
    void AddFunction(axBindedEvent fct);
    
    static axEventManager* _instance;
    
    axEventQueue _evtQueue;
    axEventMap _event_fct_map;
};

#endif /* defined(__axManager__axEventManager__) */
