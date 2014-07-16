#include "axObject.h"
// #include "axEventManager.h"

axID axObject::_global_id_count = 0;

axObject::axObject() :
_id(IncrementGlobalIdCount())
{
}

axID axObject::IncrementGlobalIdCount()
{
	return ++_global_id_count;
}

// void axObject::AddConnection(axEventFct fct) const
// {
// 	axEventManager::Instance()->AddConnection(_id, fct);
// }