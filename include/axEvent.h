/// @defgroup Core 
/// @{

#ifndef __AX_EVENT__
#define __AX_EVENT__

#include <functional>

#define axEvtFunction(TYPE) std::function<void(TYPE)>

#define axEVENT(TYPE, FUNC) std::function<void(TYPE)> Get ## FUNC() \
				{ return [=](const TYPE& x){return this->FUNC(x); }; }

#define axEvent virtual void

#endif //__AX_EVENT__

/// @}