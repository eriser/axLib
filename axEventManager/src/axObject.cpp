#include "axObject.h"
#include "axEventManager.h"

axID axObject::_global_id_count = 0;

void axObject::AddConnection(axEventFct fct) const
{
	axEventManager::Instance()->AddConnection(_id, fct);
}