#include "axEventManager.h"
#include "axCocoaInterfaceMac.h"
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
    manager_mutex.lock();
    _evtQueue.push_back(fct);
    manager_mutex.unlock();
}

int axEventManager::GetEventQueueSize() const
{
    return _evtQueue.size();
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
    auto it = _event_fct_map.find(id);
    
    if(it != _event_fct_map.end())
    {
        // Pair of the first and last element of this id.
        auto range(it->second.equal_range(evtId));
        
        // Add every connected functions from this id to the event queue.
        for (axEventMultimapIterator i = range.first; i != range.second; ++i)
        {
            // Create a Copy of child params.
            axMsg* msg_copy = msg->GetCopy();

            // Add binded function to event queue.bn
            AddFunction(axBindedEvent(i->second, msg_copy));
            AddEventToDispatchQueue();
        }
    }
    
    delete msg;
}

void axEventManager::CallNext()
{
    manager_mutex.lock();
    
    if (_evtQueue.size() != 0)
    {
        // Call function.
        _evtQueue[0]();
        
        // Remove fct from queue.
        _evtQueue.pop_front();
    }
    
    manager_mutex.unlock();
}
