#pragma once

struct Page
{
	char status;
	size_t blockSize;
	size_t freeBlocksCount;
	void* freeBlock;
	void* nextPage;
};