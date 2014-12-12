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
#ifndef __axManager__axEventManager__
#define __axManager__axEventManager__

/// @defgroup Core
/// @{

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
    
    int GetEventQueueSize() const;
    
private:
    axEventManager();
    void AddFunction(axBindedEvent fct);
    
    static axEventManager* _instance;
    
    axEventQueue _evtQueue;
    axEventMap _event_fct_map;
};

/// @}
#endif /* defined(__axManager__axEventManager__) */
