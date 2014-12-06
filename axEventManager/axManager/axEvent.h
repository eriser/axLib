//
//  axEvent.h
//  axManager
//
//  Created by Alexandre Arsenault on 2014-12-05.
//  Copyright (c) 2014 Alexandre Arsenault. All rights reserved.
//

#ifndef axManager_axEvent_h
#define axManager_axEvent_h

#include <functional>
#include <memory>
#include "axMsg.h"

typedef unsigned int axEventId;

typedef std::function<void(axMsg*)> axEventFunction;

#define axEVENT(TYPE, FUNC) axEventFunction Get ## FUNC() \
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

#endif
