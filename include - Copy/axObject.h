/// @defgroup Core 
/// @{

#ifndef _AX_OBJECT_
#define _AX_OBJECT_

#include "axEvent.h"

//typedef unsigned long axID;
//
//class axObject
//{
//public:
//	axObject();
//
//	// void AddConnection(axEventFct fct) const;
//
//	inline axID GetId() const
//	{
//		return _id;
//	}
//
//	void SetIdForReparenting(const axID& id);
//
//private:
//	axID _id;
//	
//	static axID _global_id_count;
//
//	axID IncrementGlobalIdCount();
//};

#include "axEvent.h"

typedef unsigned int axID;

class axObject
{
public:
    axObject() :_id(IncrementGlobalIdCount())
    {
    }
    
    void AddConnection(const axEventId& evtId, axEventFunction fct) const;
    
    void PushEvent(const axEventId& evtId,
                   axMsg* msg);
    
    void SetIdForReparenting(const axID& id);
    
    inline axID GetId() const
    {
        return _id;
    }
    
private:
    axID _id;
    
    static axID _global_id_count;
    
    static axID IncrementGlobalIdCount()
    {
        return ++_global_id_count;
    }
};

#endif // _AX_OBJECT_ 

/// @}