// Lab2_SOS.cpp : This file contains the 'main' function. Program execution begins and ends there.
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <math.h>
#include "Allocator.h"
#include "Test.h"
#include <tchar.h>
#include <time.h>
#include <conio.h>
#include <windows.h>

using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	const size_t pageCount = 128;
	Allocator alloc(pageCount);
	Test Test(&alloc);

	//pointers to the integer quantities 
	void* ptrs[1000];
	size_t ptrsCount = 0;
	int a = -1;

	cout << "Lab 2_OS. @nturko\n\nPress:";
	while (a != 0)
	{
		cout << "\n1 - To test allocator\n";
		cout << "2 - To allocate the memory\n";
		if (ptrsCount != 0)
		{
			cout << "3 - To free memory\n";
			cout << "4 - To reallocate memory\n";
		}
		cout << "0 - Quit\n";
		cout << "> ";
		cin >> a;
		if (a == 1) {
			Test.test(ptrs, &ptrsCount);
		}
		if (a == 2) {
			size_t size;
			cout << "\nInput the size of the memory\n";
			cout << "> ";
			cin >> size;
			clock_t start = clock();
			void* ptr = alloc.mem_alloc(size);
			clock_t end = clock();
			cout << "\ntime: " << end - start;
			if (ptr != NULL)
			{
				ptrs[ptrsCount++] = ptr;
			}
			cout << "\npointer = ";
			cout << ptr;
			cout << "\n";
		}
		if (a == 3) {
			cout << "\nChoose the block of memory to free:\n";
			for (size_t i = 0; i < ptrsCount; i++)
			{
				cout << i;
				cout << " - ";
				cout << ptrs[i];
				cout << "\n";
			}
			cout << "> ";
			size_t n;
			cin >> n;
			if (n < ptrsCount)
			{
				alloc.mem_free(ptrs[n]);
				for (size_t i = n + 1; i < ptrsCount; i++)
				{
					ptrs[i - 1] = ptrs[i];
				}
				ptrsCount--;
			}
		}
		if (a == 4) {
			cout << "\nChoose the memory block to reallocate:\n";
			for (size_t i = 0; i < ptrsCount; i++)
			{
				cout << i;
				cout << " - ";
				cout << ptrs[i];
				cout << "\n";
			}
			cout << "> ";
			size_t n;
			cin >> n;
			if (n < ptrsCount)
			{
				size_t size;
				cout << "\nInput the size of the memory to reallocate.\n";
				cout << "> ";
				cin >> size;
				void* ptr = alloc.mem_realloc(ptrs[n], size);
				if (ptr != NULL)
				{
					ptrs[n] = ptr;
				}
				cout << "\npointer = ";
				cout << ptr;
				cout << "\n";
			}
		}
		cout << "_______________________________________________________\n\n";
		cin.get();
	}
	return 0;
}