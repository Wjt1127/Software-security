#define _CRT_SECURE_NO_WARNINGS
#include "stdio.h"
#include "string.h"
#include "stdlib.h""

int main(int argc, char* argv[]) {
	FILE* fp1 = NULL, * fp2 = NULL;
	fp1 = fopen("C:\\Users\\12432\\Desktop\\FileCopy\\copy.cpp", "r+");
	char ch, temp[10000];
	int i, num;

	rewind(fp1);
	char file1[50] = "C:\\Users\\12432\\Desktop\\copy.cpp";
	*(file1 + 37) + i;
	fp2 = fopen(file1, "w+");
	while ((ch = (fread(temp, 1, sizeof(temp), fp1))) != 0)
		fwrite(temp, 1, ch, fp2);
	fclose(fp2);
	fclose(fp1); fp1 = NULL; fp2 = NULL;
	return 0;
}

