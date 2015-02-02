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
#include "axEventManager.h"
#include "axApp.h"
#include <iostream>

std::mutex manager_mutex;

axEventManager* axEventManager::_instance = nullptr;

axEventManager::axEventManager()
{
    
}

axEventManager* axEventManager::GetInstance()
{
    if (_instance == nullptr)
    {
        _instance = new axEventManager();
    }
    
    return _instance;
}

void axEventManager::AddFunction(axBindedEvent fct)
{
    //manager_mutex.lock();
    
    _evtQueue.push_back(fct);
    
    //manager_mutex.unlock();
}

int axEventManager::GetEventQueueSize() const
{
    return (int)_evtQueue.size();
}

void axEventManager::AddConnection(const axID& id,
                                   const axEventId& evtId,
                                   axEventFunction fct)
{
    manager_mutex.lock();
    
    auto it = _event_fct_map.find(id);
        
    if(it != _event_fct_map.end())
    {
        it->second.insert(axEventMultimapPair(evtId, fct));
    }
    else
    {
        axEventMultimap evt_map;
        evt_map.insert(axEventMultimapPair(evtId, fct));
        _event_fct_map.insert(axEventMapPair(id, evt_map));
    }

    manager_mutex.unlock();
}

void axEventManager::PushEvent(const axID& id,
                               const axEventId& evtId,
                               axMsg* msg)
{
    manager_mutex.lock();
    
    auto it = _event_fct_map.find(id);
    
    if(it != _event_fct_map.end())
    {
        // Pair of the first and last element of this id.
        auto range(it->second.equal_range(evtId));
        
        axCore* core = axApp::GetInstance()->GetCore();
        
        // Add every connected functions from this id to the event queue.
        for (axEventMultimapIterator i = range.first; i != range.second; ++i)
        {
            // Create a Copy of child params.
            axMsg* msg_copy = msg->GetCopy();

            // Add binded function to event queue.bn
            AddFunction(axBindedEvent(i->second, msg_copy));
            
            core->PushEventOnSystemQueue();
        }
    }
    
    manager_mutex.unlock();
    
    delete msg;
}

void axEventManager::CallNext()
{
    manager_mutex.lock();
    
    if (_evtQueue.size() != 0)
    {
        manager_mutex.unlock();
        // Call function.
        _evtQueue[0]();
        
        manager_mutex.lock();
        // Remove fct from queue.
        _evtQueue.pop_front();
    }
    
    manager_mutex.unlock();
}
