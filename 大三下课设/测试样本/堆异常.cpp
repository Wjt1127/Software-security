#include <stdio.h>
#include <windows.h>


int main(void)
{
	const int nHeapSize = 1024;
	HANDLE pNewHeap = (PBYTE) ::HeapCreate(HEAP_CREATE_ENABLE_EXECUTE | HEAP_GENERATE_EXCEPTIONS, 1024, 2048);

	HeapDestroy(pNewHeap);
	HeapDestroy(pNewHeap);
	system("pause");
	return 0;
}