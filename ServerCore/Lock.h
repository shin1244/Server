#pragma once
#include "Types.h"
class Lock
{
public:
private:
	Atomic<uint32> lockFlag_;
	uint16 writeCount_ = 0;
};

