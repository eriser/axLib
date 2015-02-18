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
#ifndef _AX_OBJECT_
#define _AX_OBJECT_

/// @defgroup Core
/// @{

#include "axEvent.h"
#include <string>
#include <map>
#include <iostream>

typedef unsigned int axID;

class axObject
{
public:
    axObject();
    
    // Add a function to the EventManager.
    void AddConnection(const axEventId& evtId, axEventFunction fct) const;
    
    // Add the function to the EventManager queue.
    void PushEvent(const axEventId& evtId, axMsg* msg);
    
    void ChangeId(const axID& id);
    
    inline axID GetId() const { return _id; }
    
    void AddEventFunction(const std::string& name, axEventFunction fct);
    
    axEventFunction GetEventFunction(const std::string& name);
    
private:
    
    // Unique identifier.
    axID _id;
    
    // Map of lamda functions.
    std::map<std::string, axEventFunction> _evtMap;
    
    static axID _global_id_count;
    static axID IncrementGlobalIdCount();
};

/// @}
#endif // _AX_OBJECT_ 
