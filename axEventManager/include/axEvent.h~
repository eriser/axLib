#ifndef _AX_EVENT_
#define _AX_EVENT_

#include <functional>
#include <memory>

#include "axMsg.h"

typedef void axEvent;
typedef shared_ptr<axMsg> axMsg_Ptr;
typedef std::function<axEvent(axMsg_Ptr)> axEventFct;

class axBindedEvent
{
public:
	axBindedEvent(axEventFct& fct, axMsg_Ptr msg):
	_fct(fct), _msg(msg){}

	void operator() (void) { _fct(_msg); }

private:
	axEventFct _fct;
	axMsg_Ptr _msg;
};

#define axEVENT(TYPE, FUNC) axEventFct Get ## FUNC() \
{ return [=](axMsg_Ptr x){ return this->FUNC(static_cast<TYPE*>(x.get())); }; }

#endif // _AX_EVENT_