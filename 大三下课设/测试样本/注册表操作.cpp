#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <windows.h>
#include <wchar.h>
#include <locale.h>

int mian() {
    HANDLE hHeap;
    hHeap = HeapCreate(HEAP_GENERATE_EXCEPTIONS, 1024, 4096);
    HANDLE hHeap1 = HeapCreate(HEAP_GENERATE_EXCEPTIONS, 1024, 4096);
    if (NULL == hHeap) {
        printf("HeapCreate ß∞‹\n");
        return 1;
    }
    LPVOID p1 = HeapAlloc(hHeap, HEAP_ZERO_MEMORY, 100);
    LPVOID p2 = HeapAlloc(hHeap1, HEAP_ZERO_MEMORY, 100);
    LPVOID p3 = HeapAlloc(hHeap, HEAP_ZERO_MEMORY, 100);
    LPVOID p4 = HeapAlloc(hHeap1, HEAP_ZERO_MEMORY, 100);
    LPVOID p5 = NULL;
    BOOL ret = HeapFree(hHeap1, 0, p4);
    ret = HeapFree(hHeap, 0, p5);
    ret = HeapFree(hHeap1, 0, p4);
    BOOL ret1 = HeapDestroy(hHeap);
    BOOL ret2 = HeapDestroy(hHeap1);
    return 0;
}