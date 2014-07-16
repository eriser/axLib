#include "axC++.h"

bool axFlag_exist(const axFlag& FLAG, axFlag flags)
{
	return((flags & FLAG) != 0);
}