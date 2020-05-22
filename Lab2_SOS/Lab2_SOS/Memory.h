#pragma once

struct Memory
{
	void* page16;
	size_t page16Count;
	void* page32;
	size_t page32Count;
	void* page64;
	size_t page64Count;
	void* page128;
	size_t page128Count;
	void* page256;
	size_t page256Count;
	void* page512;
	size_t page512Count;
	void* page1024;
	size_t page1024Count;
	void* page2048;
	size_t page2048Count;
	void* pageFull;
	size_t pageFullCount;
};