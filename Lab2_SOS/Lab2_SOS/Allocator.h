#pragma once

#ifndef _WIN32_WINNT         
#define _WIN32_WINNT 0x0600    

#include "Memory.h";
#include "Page.h"

class Allocator
{
public:
	Allocator(size_t pageCount);

	~Allocator(void);

	//Allocate memory
	void* mem_alloc(size_t size);

	//Free block memory
	void mem_free(void* ptr);

	//Reallocate memory
	void* mem_realloc(void* ptr, size_t size);

	//Return the size of pages
	size_t getPageSize();

	//Return the number of pages
	size_t getPageCount();

private:
	static const size_t pageSize = 4096;
	void* memory;
	size_t pageCount;
};

#endif

