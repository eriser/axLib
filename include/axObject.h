/// @defgroup Core 
/// @{

#ifndef _AX_OBJECT_
#define _AX_OBJECT_

#include "axEvent.h"

typedef unsigned long axID;

class axObject
{
public:
	axObject();

	// void AddConnection(axEventFct fct) const;

	inline axID GetId() const
	{
		return _id;
	}

private:
	axID _id;
	
	static axID _global_id_count;

	axID IncrementGlobalIdCount();
};

#endif // _AX_OBJECT_ 

/// @}