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
#include "axObject.h"
#include "axEventManager.h"

axID axObject::_global_id_count = 0;

axID axObject::IncrementGlobalIdCount()
{
    return ++_global_id_count;
}

axObject::axObject() :
_id(IncrementGlobalIdCount())
{
}

void axObject::AddConnection(const axEventId& evtId, axEventFunction fct) const
{
    axEventManager::GetInstance()->AddConnection(_id, evtId, fct);
}

void axObject::PushEvent(const axEventId& evtId, axMsg* msg)
{
    axEventManager::GetInstance()->PushEvent(_id, evtId, msg);
}

void axObject::ChangeId(const axID& id)
{
	_id = id;
}

void axObject::AddEventFunction(const std::string& name, axEventFunction fct)
{
    _evtMap.insert(std::pair<std::string, axEventFunction>(name, fct));
}

axEventFunction axObject::GetEventFunction(const std::string& name)
{
    std::map<std::string, axEventFunction>::iterator it = _evtMap.find(name);
    
    if(it != _evtMap.end())
    {
        return it->second;
    }
    
    return nullptr;
}
