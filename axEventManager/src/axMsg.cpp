#include "axMsg.h"

axMsg::axMsg(string msg) : 
	   _msg(msg)
{
}

string axMsg::GetStringMsg() const
{
	return _msg;
}