#pragma once

#include "Allocator.h"

class Test
{

public:
	Test(Allocator* allocator);

	~Test(void);
	
	//Run the test
	void test(void** ptrs, size_t* ptrsCount);

private:
	Allocator* allocator;
};
