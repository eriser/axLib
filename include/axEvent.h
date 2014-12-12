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
#ifndef __AX_EVENT__
#define __AX_EVENT__

/// @defgroup Core
/// @{

#include <functional>

#define axEvtFunction(TYPE) std::function<void(TYPE)>

#define axEVENT(TYPE, FUNC) std::function<void(TYPE)> Get ## FUNC() \
				{ return [=](const TYPE& x){return this->FUNC(x); }; }

#define axEvent virtual void

// New event manager.
//------------------------------------------------------------------------------

#include "axMsg.h"
typedef unsigned int axEventId;

typedef std::function<void(axMsg*)> axEventFunction;

#define axEVENT_ACCESSOR(TYPE, FUNC) axEventFunction Get ## FUNC() \
{ return [=](axMsg* x){ return this->FUNC(*static_cast<TYPE*>(x)); }; }

class axBindedEvent
{
public:
    axBindedEvent(axEventFunction& fct, axMsg* msg):
    _fct(fct), _msg(msg){}
    
    void operator() (void) { _fct(_msg); delete _msg; }
    
private:
    axEventFunction _fct;
    axMsg* _msg;
};
//------------------------------------------------------------------------------

/// @}
#endif //__AX_EVENT__
