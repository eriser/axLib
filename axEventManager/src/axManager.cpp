//#include "axManager.h"
//
//std::mutex manager_mutex;
//axEventManager* axEventManager::_instance = nullptr;
//
//axEventManager::axEventManager()
//{
//
//}
//
//axEventManager* axEventManager::Instance()
//{
//	if (_instance == nullptr)
//	{
//		_instance = new axEventManager();
//	}
//	
//	return _instance;
//}
//
//void axEventManager::AddFunction(axBindedEvent fct)
//{
//	manager_mutex.lock();
//	_fcts.push_back(fct);
//	manager_mutex.unlock();
//}
//
//void axEventManager::AddConnection(const axID& id, axEventFct fct)
//{
//	manager_mutex.lock();
//	_event_fct_map.insert(axFctEventMapPair(id, fct));
//	manager_mutex.unlock();
//}
//
//void axEventManager::PushEvent(const axID& id, axMsg* msg)
//{
//	// Pair of the first and last element of this id.
//	auto range(_event_fct_map.equal_range(id));
//
//	// Add every connected functions from this id to the event queue.
//	for (axEventMapIterator it = range.first; it != range.second; ++it)
//	{
//		// Create a Copy of child params.
//		axMsg_Ptr msg_ptr(msg->CopyChildFromParentPointer());
//		
//		// Add binded function to event queue.
//		AddFunction(axBindedEvent(it->second, msg_ptr));
//	}
//
//	 delete msg;
//}
//
//void axEventManager::CallNext()
//{
//	manager_mutex.lock();
//	if (_fcts.size() != 0)
//	{
//		// Call function.
//		_fcts[0]();
//
//		// Remove fct from queue.
//		_fcts.pop_front();
//	}
//	manager_mutex.unlock();
//}