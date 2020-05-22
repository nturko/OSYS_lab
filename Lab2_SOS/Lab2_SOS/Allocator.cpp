#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <math.h>
#include "Test.h"
#include "Allocator.h"
#include "Memory.h"
#include "Page.h"
#include <ctime>
#include <tchar.h>

using namespace std;

Allocator::Allocator(size_t pageCount)
{
	this->pageCount = pageCount;

	if (this->pageCount < 9)
	{
		this->pageCount = 9;
	}

	size_t memorySize = (pageSize + sizeof(Page)) * this->pageCount + sizeof(Memory);

	memory = new char[memorySize];
	struct Memory* mHeader = (Memory*)memory;

	mHeader->page16 = NULL;
	mHeader->page16Count = NULL;
	mHeader->page32 = NULL;
	mHeader->page32Count = NULL;
	mHeader->page64 = NULL;
	mHeader->page64Count = NULL;
	mHeader->page128 = NULL;
	mHeader->page128Count = NULL;
	mHeader->page256 = NULL;
	mHeader->page256Count = NULL;
	mHeader->page512 = NULL;
	mHeader->page512Count = NULL;
	mHeader->page1024 = NULL;
	mHeader->page1024Count = NULL;
	mHeader->page2048 = NULL;
	mHeader->page2048Count = NULL;
	mHeader->pageFull = (void*)((size_t)memory + sizeof(Memory));
	mHeader->pageFullCount = this->pageCount;

	struct Page* pHeader = (Page*)(mHeader->pageFull);

	for (size_t i = 0; i < this->pageCount; i++)
	{
		pHeader->status = 0;
		pHeader->freeBlock = (void*)((size_t)pHeader + sizeof(Page));
		pHeader->freeBlocksCount = 1;
		pHeader->blockSize = pageSize;

		if (i < (this->pageCount - 1))
		{
			pHeader->nextPage = (void*)((size_t)pHeader + sizeof(Page) + pageSize);
		}
		else
		{
			pHeader->nextPage = NULL;
		}
		pHeader = (Page*)pHeader->nextPage;
	}
}

Allocator::~Allocator(void)
{
	delete[] memory;
}


void* Allocator::mem_alloc(size_t size)
{
	struct Memory* mHeader = (Memory*)memory;
	struct Page* pHeader = NULL;

	if (size <= (pageSize / 2))
	{
		if (size > (pageSize / 4))
		{
			if (mHeader->page2048 != NULL)
			{
				pHeader = (Page*)mHeader->page2048;
				while ((pHeader->freeBlock == NULL) && (pHeader->nextPage != NULL))
				{
					pHeader = (Page*)pHeader->nextPage;
				}
				if (pHeader->freeBlock != NULL)
				{
					size_t* nextBlockPtr = (size_t*)pHeader->freeBlock;
					pHeader->freeBlock = (void*)*nextBlockPtr;
					pHeader->freeBlocksCount = pHeader->freeBlocksCount - 1;
					return (void*)nextBlockPtr;
				}
				else
				{
					if (mHeader->pageFull == NULL)
					{
						return mem_alloc(4096);
					}
					else
					{
						pHeader = (Page*)mHeader->pageFull;
						pHeader->status = 1;
						mHeader->pageFull = pHeader->nextPage;
						pHeader->nextPage = mHeader->page2048;
						pHeader->blockSize = 2048;
						pHeader->freeBlocksCount = pageSize / 2048;
						size_t* blockPtr = (size_t*)((size_t)pHeader + sizeof(Page) + pageSize - pHeader->blockSize);
						size_t nextBlock = NULL;
						while ((size_t)blockPtr >= ((size_t)pHeader + sizeof(Page)))
						{
							*blockPtr = nextBlock;
							nextBlock = (size_t)blockPtr;
							blockPtr = (size_t*)((size_t)blockPtr - pHeader->blockSize);
						}
						pHeader->freeBlock = (void*)((size_t)pHeader + sizeof(Page));
						mHeader->pageFullCount = mHeader->pageFullCount - 1;
						mHeader->page2048 = (void*)pHeader;
						mHeader->page2048Count = mHeader->page2048Count + 1;
						return mem_alloc(size);
					}
				}
			}
			else
			{
				if (mHeader->pageFullCount == 0)
				{
					return NULL;
				}
				pHeader = (Page*)mHeader->pageFull;
				pHeader->status = 1;
				mHeader->pageFull = pHeader->nextPage;
				pHeader->nextPage = mHeader->page2048;
				pHeader->blockSize = 2048;
				pHeader->freeBlocksCount = pageSize / 2048;
				size_t* blockPtr = (size_t*)((size_t)pHeader + sizeof(Page) + pageSize - pHeader->blockSize);
				size_t nextBlock = NULL;
				while ((size_t)blockPtr >= ((size_t)pHeader + sizeof(Page)))
				{
					*blockPtr = nextBlock;
					nextBlock = (size_t)blockPtr;
					blockPtr = (size_t*)((size_t)blockPtr - pHeader->blockSize);
				}
				pHeader->freeBlock = (void*)((size_t)pHeader + sizeof(Page));
				mHeader->pageFullCount = mHeader->pageFullCount - 1;
				mHeader->page2048 = (void*)pHeader;
				mHeader->page2048Count = mHeader->page2048Count + 1;
				return mem_alloc(size);
			}
		}
		else
		{
			if (size > (pageSize / 8))
			{
				if (mHeader->page1024 != NULL)
				{
					pHeader = (Page*)mHeader->page1024;
					while ((pHeader->freeBlock == NULL) && (pHeader->nextPage != NULL))
					{
						pHeader = (Page*)pHeader->nextPage;
					}
					if (pHeader->freeBlock != NULL)
					{
						size_t* nextBlockPtr = (size_t*)pHeader->freeBlock;
						pHeader->freeBlock = (void*)*nextBlockPtr;
						pHeader->freeBlocksCount = pHeader->freeBlocksCount - 1;
						return (void*)nextBlockPtr;
					}
					else
					{
						if (mHeader->pageFull == NULL)
						{
							return mem_alloc(2048);
						}
						else
						{
							pHeader = (Page*)mHeader->pageFull;
							pHeader->status = 1;
							mHeader->pageFull = pHeader->nextPage;
							pHeader->nextPage = mHeader->page1024;
							pHeader->blockSize = 1024;
							pHeader->freeBlocksCount = pageSize / 1024;
							size_t* blockPtr = (size_t*)((size_t)pHeader + sizeof(Page) + pageSize - pHeader->blockSize);
							size_t nextBlock = NULL;
							while ((size_t)blockPtr >= ((size_t)pHeader + sizeof(Page)))
							{
								*blockPtr = nextBlock;
								nextBlock = (size_t)blockPtr;
								blockPtr = (size_t*)((size_t)blockPtr - pHeader->blockSize);
							}
							pHeader->freeBlock = (void*)((size_t)pHeader + sizeof(Page));
							mHeader->pageFullCount = mHeader->pageFullCount - 1;
							mHeader->page1024 = (void*)pHeader;
							mHeader->page1024Count = mHeader->page1024Count + 1;
							return mem_alloc(size);
						}
					}
				}
				else
				{
					pHeader = (Page*)mHeader->pageFull;
					pHeader->status = 1;
					mHeader->pageFull = pHeader->nextPage;
					pHeader->nextPage = mHeader->page1024;
					pHeader->blockSize = 1024;
					pHeader->freeBlocksCount = pageSize / 1024;
					size_t* blockPtr = (size_t*)((size_t)pHeader + sizeof(Page) + pageSize - pHeader->blockSize);
					size_t nextBlock = NULL;
					while ((size_t)blockPtr >= ((size_t)pHeader + sizeof(Page)))
					{
						*blockPtr = nextBlock;
						nextBlock = (size_t)blockPtr;
						blockPtr = (size_t*)((size_t)blockPtr - pHeader->blockSize);
					}
					pHeader->freeBlock = (void*)((size_t)pHeader + sizeof(Page));
					mHeader->pageFullCount = mHeader->pageFullCount - 1;
					mHeader->page1024 = (void*)pHeader;
					mHeader->page1024Count = mHeader->page1024Count + 1;
					return mem_alloc(size);
				}
			}
			else
			{
				if (size > (pageSize / 16))
				{
					if (mHeader->page512 != NULL)
					{
						pHeader = (Page*)mHeader->page512;
						while ((pHeader->freeBlock == NULL) && (pHeader->nextPage != NULL))
						{
							pHeader = (Page*)pHeader->nextPage;
						}
						if (pHeader->freeBlock != NULL)
						{
							size_t* nextBlockPtr = (size_t*)pHeader->freeBlock;
							pHeader->freeBlock = (void*)*nextBlockPtr;
							pHeader->freeBlocksCount = pHeader->freeBlocksCount - 1;
							return (void*)nextBlockPtr;
						}
						else
						{
							if (mHeader->pageFull == NULL)
							{
								return mem_alloc(1024);
							}
							else
							{
								pHeader = (Page*)mHeader->pageFull;
								pHeader->status = 1;
								mHeader->pageFull = pHeader->nextPage;
								pHeader->nextPage = mHeader->page512;
								pHeader->blockSize = 512;
								pHeader->freeBlocksCount = pageSize / 512;
								size_t* blockPtr = (size_t*)((size_t)pHeader + sizeof(Page) + pageSize - pHeader->blockSize);
								size_t nextBlock = NULL;
								while ((size_t)blockPtr >= ((size_t)pHeader + sizeof(Page)))
								{
									*blockPtr = nextBlock;
									nextBlock = (size_t)blockPtr;
									blockPtr = (size_t*)((size_t)blockPtr - pHeader->blockSize);
								}
								pHeader->freeBlock = (void*)((size_t)pHeader + sizeof(Page));
								mHeader->pageFullCount = mHeader->pageFullCount - 1;
								mHeader->page512 = (void*)pHeader;
								mHeader->page512Count = mHeader->page512Count + 1;
								return mem_alloc(size);
							}
						}
					}
					else
					{
						pHeader = (Page*)mHeader->pageFull;
						pHeader->status = 1;
						mHeader->pageFull = pHeader->nextPage;
						pHeader->nextPage = mHeader->page512;
						pHeader->blockSize = 512;
						pHeader->freeBlocksCount = pageSize / 512;
						size_t* blockPtr = (size_t*)((size_t)pHeader + sizeof(Page) + pageSize - pHeader->blockSize);
						size_t nextBlock = NULL;
						while ((size_t)blockPtr >= ((size_t)pHeader + sizeof(Page)))
						{
							*blockPtr = nextBlock;
							nextBlock = (size_t)blockPtr;
							blockPtr = (size_t*)((size_t)blockPtr - pHeader->blockSize);
						}
						pHeader->freeBlock = (void*)((size_t)pHeader + sizeof(Page));
						mHeader->pageFullCount = mHeader->pageFullCount - 1;
						mHeader->page512 = (void*)pHeader;
						mHeader->page512Count = mHeader->page512Count + 1;
						return mem_alloc(size);
					}
				}
				else
				{
					if (size > (pageSize / 32))
					{
						if (mHeader->page256 != NULL)
						{
							pHeader = (Page*)mHeader->page256;
							while ((pHeader->freeBlock == NULL) && (pHeader->nextPage != NULL))
							{
								pHeader = (Page*)pHeader->nextPage;
							}
							if (pHeader->freeBlock != NULL)
							{
								size_t* nextBlockPtr = (size_t*)pHeader->freeBlock;
								pHeader->freeBlock = (void*)*nextBlockPtr;
								pHeader->freeBlocksCount = pHeader->freeBlocksCount - 1;
								return (void*)nextBlockPtr;
							}
							else
							{
								if (mHeader->pageFull == NULL)
								{
									return mem_alloc(512);
								}
								else
								{
									pHeader = (Page*)mHeader->pageFull;
									pHeader->status = 1;
									mHeader->pageFull = pHeader->nextPage;
									pHeader->nextPage = mHeader->page256;
									pHeader->blockSize = 256;
									pHeader->freeBlocksCount = pageSize / 256;
									size_t* blockPtr = (size_t*)((size_t)pHeader + sizeof(Page) + pageSize - pHeader->blockSize);
									size_t nextBlock = NULL;
									while ((size_t)blockPtr >= ((size_t)pHeader + sizeof(Page)))
									{
										*blockPtr = nextBlock;
										nextBlock = (size_t)blockPtr;
										blockPtr = (size_t*)((size_t)blockPtr - pHeader->blockSize);
									}
									pHeader->freeBlock = (void*)((size_t)pHeader + sizeof(Page));
									mHeader->pageFullCount = mHeader->pageFullCount - 1;
									mHeader->page256 = (void*)pHeader;
									mHeader->page256Count = mHeader->page256Count + 1;
									return mem_alloc(size);
								}
							}
						}
						else
						{
							pHeader = (Page*)mHeader->pageFull;
							pHeader->status = 1;
							mHeader->pageFull = pHeader->nextPage;
							pHeader->nextPage = mHeader->page256;
							pHeader->blockSize = 256;
							pHeader->freeBlocksCount = pageSize / 256;
							size_t* blockPtr = (size_t*)((size_t)pHeader + sizeof(Page) + pageSize - pHeader->blockSize);
							size_t nextBlock = NULL;
							while ((size_t)blockPtr >= ((size_t)pHeader + sizeof(Page)))
							{
								*blockPtr = nextBlock;
								nextBlock = (size_t)blockPtr;
								blockPtr = (size_t*)((size_t)blockPtr - pHeader->blockSize);
							}
							pHeader->freeBlock = (void*)((size_t)pHeader + sizeof(Page));
							mHeader->pageFullCount = mHeader->pageFullCount - 1;
							mHeader->page256 = (void*)pHeader;
							mHeader->page256Count = mHeader->page256Count + 1;
							return mem_alloc(size);
						}
					}
					else
					{
						if (size > (pageSize / 64))
						{
							if (mHeader->page128 != NULL)
							{
								pHeader = (Page*)mHeader->page128;
								while ((pHeader->freeBlock == NULL) && (pHeader->nextPage != NULL))
								{
									pHeader = (Page*)pHeader->nextPage;
								}
								if (pHeader->freeBlock != NULL)
								{
									size_t* nextBlockPtr = (size_t*)pHeader->freeBlock;
									pHeader->freeBlock = (void*)*nextBlockPtr;
									pHeader->freeBlocksCount = pHeader->freeBlocksCount - 1;
									return (void*)nextBlockPtr;
								}
								else
								{
									if (mHeader->pageFull == NULL)
									{
										return mem_alloc(256);
									}
									else
									{
										pHeader = (Page*)mHeader->pageFull;
										pHeader->status = 1;
										mHeader->pageFull = pHeader->nextPage;
										pHeader->nextPage = mHeader->page128;
										pHeader->blockSize = 128;
										pHeader->freeBlocksCount = pageSize / 128;
										size_t* blockPtr = (size_t*)((size_t)pHeader + sizeof(Page) + pageSize - pHeader->blockSize);
										size_t nextBlock = NULL;
										while ((size_t)blockPtr >= ((size_t)pHeader + sizeof(Page)))
										{
											*blockPtr = nextBlock;
											nextBlock = (size_t)blockPtr;
											blockPtr = (size_t*)((size_t)blockPtr - pHeader->blockSize);
										}
										pHeader->freeBlock = (void*)((size_t)pHeader + sizeof(Page));
										mHeader->pageFullCount = mHeader->pageFullCount - 1;
										mHeader->page128 = (void*)pHeader;
										mHeader->page128Count = mHeader->page128Count + 1;
										return mem_alloc(size);
									}
								}
							}
							else
							{
								pHeader = (Page*)mHeader->pageFull;
								pHeader->status = 1;
								mHeader->pageFull = pHeader->nextPage;
								pHeader->nextPage = mHeader->page128;
								pHeader->blockSize = 128;
								pHeader->freeBlocksCount = pageSize / 128;
								size_t* blockPtr = (size_t*)((size_t)pHeader + sizeof(Page) + pageSize - pHeader->blockSize);
								size_t nextBlock = NULL;
								while ((size_t)blockPtr >= ((size_t)pHeader + sizeof(Page)))
								{
									*blockPtr = nextBlock;
									nextBlock = (size_t)blockPtr;
									blockPtr = (size_t*)((size_t)blockPtr - pHeader->blockSize);
								}
								pHeader->freeBlock = (void*)((size_t)pHeader + sizeof(Page));
								mHeader->pageFullCount = mHeader->pageFullCount - 1;
								mHeader->page128 = (void*)pHeader;
								mHeader->page128Count = mHeader->page128Count + 1;
								return mem_alloc(size);
							}
						}
						else
						{
							if (size > (pageSize / 128))
							{
								if (mHeader->page64 != NULL)
								{
									pHeader = (Page*)mHeader->page64;
									while ((pHeader->freeBlock == NULL) && (pHeader->nextPage != NULL))
									{
										pHeader = (Page*)pHeader->nextPage;
									}
									if (pHeader->freeBlock != NULL)
									{
										size_t* nextBlockPtr = (size_t*)pHeader->freeBlock;
										pHeader->freeBlock = (void*)*nextBlockPtr;
										pHeader->freeBlocksCount = pHeader->freeBlocksCount - 1;
										return (void*)nextBlockPtr;
									}
									else
									{
										if (mHeader->pageFull == NULL)
										{
											return mem_alloc(128);
										}
										else
										{
											pHeader = (Page*)mHeader->pageFull;
											pHeader->status = 1;
											mHeader->pageFull = pHeader->nextPage;
											pHeader->nextPage = mHeader->page64;
											pHeader->blockSize = 64;
											pHeader->freeBlocksCount = pageSize / 64;
											size_t* blockPtr = (size_t*)((size_t)pHeader + sizeof(Page) + pageSize - pHeader->blockSize);
											size_t nextBlock = NULL;
											while ((size_t)blockPtr >= ((size_t)pHeader + sizeof(Page)))
											{
												*blockPtr = nextBlock;
												nextBlock = (size_t)blockPtr;
												blockPtr = (size_t*)((size_t)blockPtr - pHeader->blockSize);
											}
											pHeader->freeBlock = (void*)((size_t)pHeader + sizeof(Page));
											mHeader->pageFullCount = mHeader->pageFullCount - 1;
											mHeader->page64 = (void*)pHeader;
											mHeader->page64Count = mHeader->page64Count + 1;
											return mem_alloc(size);
										}
									}
								}
								else
								{
									pHeader = (Page*)mHeader->pageFull;
									pHeader->status = 1;
									mHeader->pageFull = pHeader->nextPage;
									pHeader->nextPage = mHeader->page64;
									pHeader->blockSize = 64;
									pHeader->freeBlocksCount = pageSize / 64;
									size_t* blockPtr = (size_t*)((size_t)pHeader + sizeof(Page) + pageSize - pHeader->blockSize);
									size_t nextBlock = NULL;
									while ((size_t)blockPtr >= ((size_t)pHeader + sizeof(Page)))
									{
										*blockPtr = nextBlock;
										nextBlock = (size_t)blockPtr;
										blockPtr = (size_t*)((size_t)blockPtr - pHeader->blockSize);
									}
									pHeader->freeBlock = (void*)((size_t)pHeader + sizeof(Page));
									mHeader->pageFullCount = mHeader->pageFullCount - 1;
									mHeader->page64 = (void*)pHeader;
									mHeader->page64Count = mHeader->page64Count + 1;
									return mem_alloc(size);
								}
							}
							else
							{
								if (size > (pageSize / 256))
								{
									if (mHeader->page32 != NULL)
									{
										pHeader = (Page*)mHeader->page32;
										while ((pHeader->freeBlock == NULL) && (pHeader->nextPage != NULL))
										{
											pHeader = (Page*)pHeader->nextPage;
										}
										if (pHeader->freeBlock != NULL)
										{
											size_t* nextBlockPtr = (size_t*)pHeader->freeBlock;
											pHeader->freeBlock = (void*)*nextBlockPtr;
											pHeader->freeBlocksCount = pHeader->freeBlocksCount - 1;
											return (void*)nextBlockPtr;
										}
										else
										{
											if (mHeader->pageFull == NULL)
											{
												return mem_alloc(64);
											}
											else
											{
												pHeader = (Page*)mHeader->pageFull;
												pHeader->status = 1;
												mHeader->pageFull = pHeader->nextPage;
												pHeader->nextPage = mHeader->page32;
												pHeader->blockSize = 32;
												pHeader->freeBlocksCount = pageSize / 32;
												size_t* blockPtr = (size_t*)((size_t)pHeader + sizeof(Page) + pageSize - pHeader->blockSize);
												size_t nextBlock = NULL;
												while ((size_t)blockPtr >= ((size_t)pHeader + sizeof(Page)))
												{
													*blockPtr = nextBlock;
													nextBlock = (size_t)blockPtr;
													blockPtr = (size_t*)((size_t)blockPtr - pHeader->blockSize);
												}
												pHeader->freeBlock = (void*)((size_t)pHeader + sizeof(Page));
												mHeader->pageFullCount = mHeader->pageFullCount - 1;
												mHeader->page32 = (void*)pHeader;
												mHeader->page32Count = mHeader->page32Count + 1;
												return mem_alloc(size);
											}
										}
									}
									else
									{
										pHeader = (Page*)mHeader->pageFull;
										pHeader->status = 1;
										mHeader->pageFull = pHeader->nextPage;
										pHeader->nextPage = mHeader->page32;
										pHeader->blockSize = 32;
										pHeader->freeBlocksCount = pageSize / 32;
										size_t* blockPtr = (size_t*)((size_t)pHeader + sizeof(Page) + pageSize - pHeader->blockSize);
										size_t nextBlock = NULL;
										while ((size_t)blockPtr >= ((size_t)pHeader + sizeof(Page)))
										{
											*blockPtr = nextBlock;
											nextBlock = (size_t)blockPtr;
											blockPtr = (size_t*)((size_t)blockPtr - pHeader->blockSize);
										}
										pHeader->freeBlock = (void*)((size_t)pHeader + sizeof(Page));
										mHeader->pageFullCount = mHeader->pageFullCount - 1;
										mHeader->page32 = (void*)pHeader;
										mHeader->page32Count = mHeader->page32Count + 1;
										return mem_alloc(size);
									}
								}
								else
								{
									if (mHeader->page16 != NULL)
									{
										pHeader = (Page*)mHeader->page16;
										while ((pHeader->freeBlock == NULL) && (pHeader->nextPage != NULL))
										{
											pHeader = (Page*)pHeader->nextPage;
										}
										if (pHeader->freeBlock != NULL)
										{
											size_t* nextBlockPtr = (size_t*)pHeader->freeBlock;
											pHeader->freeBlock = (void*)*nextBlockPtr;
											pHeader->freeBlocksCount = pHeader->freeBlocksCount - 1;
											return (void*)nextBlockPtr;
										}
										else
										{
											if (mHeader->pageFull == NULL)
											{
												return mem_alloc(32);
											}
											else
											{
												pHeader = (Page*)mHeader->pageFull;
												pHeader->status = 1;
												mHeader->pageFull = pHeader->nextPage;
												pHeader->nextPage = mHeader->page16;
												pHeader->blockSize = 16;
												pHeader->freeBlocksCount = pageSize / 16;
												size_t* blockPtr = (size_t*)((size_t)pHeader + sizeof(Page) + pageSize - pHeader->blockSize);
												size_t nextBlock = NULL;
												while ((size_t)blockPtr >= ((size_t)pHeader + sizeof(Page)))
												{
													*blockPtr = nextBlock;
													nextBlock = (size_t)blockPtr;
													blockPtr = (size_t*)((size_t)blockPtr - pHeader->blockSize);
												}
												pHeader->freeBlock = (void*)((size_t)pHeader + sizeof(Page));
												mHeader->pageFullCount = mHeader->pageFullCount - 1;
												mHeader->page16 = (void*)pHeader;
												mHeader->page16Count = mHeader->page16Count + 1;
												return mem_alloc(size);
											}
										}
									}
									else
									{
										pHeader = (Page*)mHeader->pageFull;
										pHeader->status = 1;
										mHeader->pageFull = pHeader->nextPage;
										pHeader->nextPage = mHeader->page16;
										pHeader->blockSize = 16;
										pHeader->freeBlocksCount = pageSize / 16;
										size_t* blockPtr = (size_t*)((size_t)pHeader + sizeof(Page) + pageSize - pHeader->blockSize);
										size_t nextBlock = NULL;
										while ((size_t)blockPtr >= ((size_t)pHeader + sizeof(Page)))
										{
											*blockPtr = nextBlock;
											nextBlock = (size_t)blockPtr;
											blockPtr = (size_t*)((size_t)blockPtr - pHeader->blockSize);
										}
										pHeader->freeBlock = (void*)((size_t)pHeader + sizeof(Page));
										mHeader->pageFullCount = mHeader->pageFullCount - 1;
										mHeader->page16 = (void*)pHeader;
										mHeader->page16Count = mHeader->page16Count + 1;
										return mem_alloc(size);
									}
								}
							}
						}
					}
				}
			}
		}
	}
	else
	{
		size_t neededPages = size / pageSize;
		if ((size % pageSize) != 0)
		{
			neededPages++;
		}
		if (mHeader->pageFullCount < neededPages)
		{
			return NULL;
		}
		else
		{
			pHeader = (Page*)mHeader->pageFull;
			struct Page* nextPageHeader = pHeader;
			for (size_t i = 0; i < neededPages; i++)
			{
				nextPageHeader->status = 2;
				nextPageHeader->freeBlock = NULL;
				nextPageHeader->freeBlocksCount = neededPages;
				nextPageHeader = (Page*)nextPageHeader->nextPage;
			}
			mHeader->pageFull = (void*)nextPageHeader;
			mHeader->pageFullCount = mHeader->pageFullCount - neededPages;
			return (void*)((size_t)pHeader + sizeof(Page));
		}
	}
}

void Allocator::mem_free(void* ptr)
{
	if (ptr == NULL)
	{
		return;
	}
	struct Memory* mHeader = (Memory*)memory;
	struct Page* pHeader = (Page*)((size_t)memory + sizeof(Memory));
	while (((size_t)ptr < (size_t)pHeader) || ((size_t)ptr > ((size_t)pHeader + sizeof(Page) + pageSize)))
	{
		pHeader = (Page*)((size_t)pHeader + sizeof(Page) + pageSize);
	}
	if (pHeader->status == 2)
	{
		struct Page* pagePtr = pHeader;
		size_t pagePtrCount = pHeader->freeBlocksCount;
		for (size_t i = 0; i < pagePtrCount; i++)
		{
			pagePtr->status = 0;
			pagePtr->freeBlocksCount = 1;
			pagePtr->freeBlock = (void*)((size_t)pagePtr + sizeof(Page));
			if (i < (pagePtrCount - 1))
			{
				pagePtr = (Page*)pagePtr->nextPage;
			}
		}
		pagePtr->nextPage = mHeader->pageFull;
		mHeader->pageFull = (void*)pHeader;
		mHeader->pageFullCount = mHeader->pageFullCount + pagePtrCount;
	}
	else
	{
		if (pHeader->status == 1)
		{
			size_t* nextBlockPtr = (size_t*)ptr;
			*nextBlockPtr = (size_t)pHeader->freeBlock;
			pHeader->freeBlock = ptr;
			pHeader->freeBlocksCount = pHeader->freeBlocksCount + 1;
			if (pHeader->freeBlocksCount == (pageSize / pHeader->blockSize))
			{
				if (pHeader->blockSize == 2048)
				{
					struct Page* pagePtr = (Page*)mHeader->page2048;
					if ((size_t)pHeader == (size_t)pagePtr)
					{
						mHeader->page2048 = pHeader->nextPage;
					}
					else
					{
						while ((size_t)pHeader != (size_t)(pagePtr->nextPage))
						{
							pagePtr = (Page*)pHeader->nextPage;
						}
						pagePtr->nextPage = pHeader->nextPage;
					}
					mHeader->page2048Count = mHeader->page2048Count - 1;
				}
				else
				{
					if (pHeader->blockSize == 1024)
					{
						struct Page* pagePtr = (Page*)mHeader->page1024;
						if ((size_t)pHeader == (size_t)pagePtr)
						{
							mHeader->page1024 = pHeader->nextPage;
							
						}
						else
						{
							while ((size_t)pHeader != (size_t)(pagePtr->nextPage))
							{
								pagePtr = (Page*)pHeader->nextPage;
							}
							pagePtr->nextPage = pHeader->nextPage;
						}
						mHeader->page1024Count = mHeader->page1024Count - 1;
					}
					else
					{
						if (pHeader->blockSize == 512)
						{
							struct Page* pagePtr = (Page*)mHeader->page512;
							if ((size_t)pHeader == (size_t)pagePtr)
							{
								mHeader->page512 = pHeader->nextPage;
							}
							else
							{
								while ((size_t)pHeader != (size_t)(pagePtr->nextPage))
								{
									pagePtr = (Page*)pHeader->nextPage;
								}
								pagePtr->nextPage = pHeader->nextPage;
							}
							mHeader->page512Count = mHeader->page512Count - 1;
						}
						else
						{
							if (pHeader->blockSize == 256)
							{
								struct Page* pagePtr = (Page*)mHeader->page256;
								if ((size_t)pHeader == (size_t)pagePtr)
								{
									mHeader->page256 = pHeader->nextPage;
								}
								else
								{
									while ((size_t)pHeader != (size_t)(pagePtr->nextPage))
									{
										pagePtr = (Page*)pHeader->nextPage;
									}
									pagePtr->nextPage = pHeader->nextPage;
								}
								mHeader->page256Count = mHeader->page256Count - 1;
							}
							else
							{
								if (pHeader->blockSize == 128)
								{
									struct Page* pagePtr = (Page*)mHeader->page128;
									if ((size_t)pHeader == (size_t)pagePtr)
									{
										mHeader->page128 = pHeader->nextPage;
										
									}
									else
									{
										while ((size_t)pHeader != (size_t)(pagePtr->nextPage))
										{
											pagePtr = (Page*)pHeader->nextPage;
										}
										pagePtr->nextPage = pHeader->nextPage;
									}
									mHeader->page128Count = mHeader->page128Count - 1;
									
								}
								else
								{
									if (pHeader->blockSize == 64)
									{
										struct Page* pagePtr = (Page*)mHeader->page64;
										if ((size_t)pHeader == (size_t)pagePtr)
										{
											mHeader->page64 = pHeader->nextPage;
										}
										else
										{
											while ((size_t)pHeader != (size_t)(pagePtr->nextPage))
											{
												pagePtr = (Page*)pHeader->nextPage;
											}
											pagePtr->nextPage = pHeader->nextPage;
										}
										mHeader->page64Count = mHeader->page64Count - 1;
									}
									else
									{
										if (pHeader->blockSize == 32)
										{
											struct Page* pagePtr = (Page*)mHeader->page32;
											if ((size_t)pHeader == (size_t)pagePtr)
											{
												mHeader->page32 = pHeader->nextPage;
												
											}
											else
											{
												while ((size_t)pHeader != (size_t)(pagePtr->nextPage))
												{
													pagePtr = (Page*)pHeader->nextPage;
												}
												pagePtr->nextPage = pHeader->nextPage;
											}
											mHeader->page32Count = mHeader->page32Count - 1;
										}
										else
										{
											struct Page* pagePtr = (Page*)mHeader->page16;
											if ((size_t)pHeader == (size_t)pagePtr)
											{
												mHeader->page16 = pHeader->nextPage;
												
											}
											else
											{
												while ((size_t)pHeader != (size_t)(pagePtr->nextPage))
												{
													pagePtr = (Page*)pHeader->nextPage;
												}
												pagePtr->nextPage = pHeader->nextPage;
											}
											mHeader->page16Count = mHeader->page16Count - 1;
										}
									}
								}
							}
						}
					}
				}
				pHeader->freeBlock = (void*)((size_t)pHeader + sizeof(Page));
				pHeader->status = 0;
				pHeader->blockSize = pageSize;
				pHeader->freeBlocksCount = 1;
				pHeader->nextPage = mHeader->pageFull;
				mHeader->pageFull = (void*)pHeader;
				mHeader->pageFullCount = mHeader->pageFullCount + 1;
			}
		}
	}
}


void* Allocator::mem_realloc(void* ptr, size_t size)
{
	if (ptr == NULL)
	{
		return mem_alloc(size);
	}

	struct Memory* mHeader = (Memory*)memory;
	struct Page* pHeader = (Page*)((size_t)memory + sizeof(Memory));

	while (((size_t)ptr < (size_t)pHeader) || ((size_t)ptr > ((size_t)pHeader + sizeof(Page) + pageSize)))
	{
		pHeader = (Page*)((size_t)pHeader + sizeof(Page) + pageSize);
	}

	if (((pHeader->blockSize / 2) < size) && (pHeader->blockSize >= size))
	{
		return ptr;
	}

	void* resultPtr = mem_alloc(size);

	if (resultPtr == NULL)
	{
		return NULL;
	}

	char* ptr1 = (char*)ptr;
	char* ptr2 = (char*)resultPtr;

	if ((pHeader->status == 1) || ((pHeader->status == 2) && (pHeader->freeBlocksCount == 1)) || ((pHeader->status == 2) && (size <= pageSize)))
	{
		size_t copySize;
		if (size < pHeader->blockSize)
		{
			copySize = size;
		}
		else
		{
			copySize = pHeader->blockSize;
		}
		for (size_t i = 0; i < copySize; i++)
		{
			*ptr2 = *ptr1;
			ptr1++;
			ptr2++;
		}
	}
	else
	{
		struct Page* copyPHeader = (Page*)((size_t)resultPtr - sizeof(Page));
		size_t copyPagesCount;
		if (pHeader->freeBlocksCount < copyPHeader->freeBlocksCount)
		{
			copyPagesCount = pHeader->freeBlocksCount;
		}
		else
		{
			copyPagesCount = copyPHeader->freeBlocksCount;
		}
		for (size_t i = 0; i < copyPagesCount; i++)
		{
			for (size_t j = 0; j < pageSize; j++)
			{
				*ptr2 = *ptr1;
				ptr1++;
				ptr2++;
			}
			ptr1 = (char*)((size_t)pHeader->nextPage + sizeof(Page));
			ptr2 = (char*)((size_t)copyPHeader->nextPage + sizeof(Page));
		}
	}
	mem_free(ptr);
	return resultPtr;
}

size_t Allocator::getPageSize()
{
	return pageSize;
}

size_t Allocator::getPageCount()
{
	return pageCount;
}