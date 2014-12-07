/// @defgroup Core 
/// @{

#ifndef __AX_EVENT__
#define __AX_EVENT__

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

#define axEVENT_ACCESOR(TYPE, FUNC) axEventFunction Get ## FUNC() \
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

#endif //__AX_EVENT__

/// @}