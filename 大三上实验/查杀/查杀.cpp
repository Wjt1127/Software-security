#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include <stdio.h>
#include<string.h>
#include<Windows.h>
#include<string>
using namespace std;

#define SIZE 256  //字符数
#define MAXSIZE 1024  //定义BM算法的区域最大范围
bool Direct_CMP(const char* FileName);  //给定偏移值的直接比对
void GetAttribution(const char* FileName);  //输出病毒文件的详细属性信息
void generateBadChar(char* b, int m, int* badchar);//(模式串字符b，模式串长度m，模式串的哈希表)
void generateGS(const char* pattern, int suffix[], bool prefix[]);//好字符规则
int moveByGS(int j, int m, int* suffix, bool* prefix);//传入的j是坏字符对应的模式串中的字符下标
int str_bm(char* a, int n, char* b, int m);//a表示主串，长n; b表示模式串,长m

unsigned const char signature[] ={ 0x54,0x68,0x69,0x73,0x20,0x70,0x72,0x6F,0x67,0x72,0x61,0x6D,0x20,0x63,0x61,0x6E,0x6E,0x6F,0x74,0x20,0x62,0x65,0x20,0x72,0x75,0x6E,0x20,0x69,0x6E,0x20,0x44,0x4F,0x53,0x20,0x6D,0x6F,0x64,0x65 };
//特征码根据恶意代码反汇编选取 "This program cannot be run in DOS mode" 总计39个字节
long length = 38;  //记录特征码的字节大小

//设置输出的颜色
BOOL SetConsoleColor(WORD wa) {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hConsole == INVALID_HANDLE_VALUE)
		return FALSE;

	return SetConsoleTextAttribute(hConsole, wa);
}

int main()
{
	char Virus[100]; //被查病毒的文件路径
	bool flag = false;

	while (1)
	{
		system("cls");
		memset(Virus, 0, sizeof(Virus));
		printf("请输入被查病毒的的路径:");
		scanf("%s", Virus);
		int op;
		flag = false;
		cout << "\n\n\n\n\n\n\n";
		printf("\t\t\t\t*-------------------------------------*\n");
		printf("\t\t\t\t*\t选择你要使用的算法:           *\n");
		printf("\t\t\t\t*\t1: 偏移量直接比对             *\n");
		printf("\t\t\t\t*\t2: BM算法                     *\n");
		printf("\t\t\t\t*\t0: 退出                       *\n");
		printf("\t\t\t\t*-------------------------------------*\n");
		cout << "\t\t\t\t输入选择：";
		scanf("%d", &op);
		if (op == 1) {
			flag = Direct_CMP(Virus);
			if (flag)//需要输出病毒详细信息，病毒命名，文件大小，创建时间等
			{
				GetAttribution(Virus);  //打印出详细信息
			}
			if (!flag) printf("没有发现病毒!\n");
			cout << "\t\t\t\t"; system("pause");
		}
		else if (op == 2) {
			printf("请输入你想比对的起始字节数和截止字节数：\n");
			long start, end;
			scanf("%ld %ld", &start, &end);
			char TXT[MAXSIZE] = { 0 };
			printf("\n*************使用BM匹配算法进行查杀*************\n");
			FILE* fp = NULL;
			fp = fopen(Virus, "rb"); //以2进制打开指定文件
			if (fp == NULL)
			{
				printf("文件路径出错！\n");
				continue;
			}
			fseek(fp, start, SEEK_SET); //把文件指针指向偏移地址
			fread(TXT, end - start + 1, 1, fp);
			flag=str_bm(TXT,end-start+1,(char*)signature,length);
			if (flag)//需要输出病毒详细信息，病毒命名，文件大小，创建时间等
			{
				GetAttribution(Virus);  //打印出详细信息
			}
			if (!flag) printf("没有发现病毒!\n");
			cout << "\t\t\t\t"; system("pause");
		}
		else if (op == 0) {
			break;
		}
		else {
			printf("输入编号错误!\n");
			continue;
		}
	}

	return 0;
}

/*
函数名：void GetAttribution(const char* FileName)
输入参数：病毒文件的路径
返回值：无
功能：输出病毒文件的详细属性信息
*/
void GetAttribution(const char* FileName) {
	WIN32_FIND_DATAA ffd;//声明一个FIND_DATAA 结构题
	HANDLE hFind = FindFirstFileA(FileName, &ffd);  //使用该函数获得该文件的相关属性
	SetConsoleColor(FOREGROUND_RED);
	printf("在为 %s 的路径文件中发现了复制感染类型的病毒攻击\n",FileName);
	printf("该病毒文件的创建时间:");
	SYSTEMTIME st;
	FileTimeToSystemTime(&(ffd.ftCreationTime), &st);
	printf("%d-%d-%d  %02d:%02d:%02d\n", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
	unsigned long long size = (ffd.nFileSizeHigh << 32) + ffd.nFileSizeLow;
	printf("该病毒文件的大小为:");
	cout << size << "字节\n";
	SetConsoleColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

}


/*
函数名：Direct_CMP
输入参数：FileName 是被查文件的路径
返回值：True or False
功能：判断是否在该文件中识别出特征码
*/
bool Direct_CMP(const char* FileName)
{
	printf("\n*************使用偏移位置直接比较方法查杀*************\n");
	FILE* fp = NULL;
	int result = 0;
	char rvir[255] = { 0 };
	bool Flag = false;
	fp = fopen(FileName, "rb"); //以2进制打开指定文件
	if (fp == NULL)
	{
		printf("文件路径出错！\n");
		return Flag;
	}
	printf("请输入偏移位置:");
	long offset;
	scanf("%ld", &offset);

	fseek(fp, offset, SEEK_SET); //把文件指针指向偏移地址
	fread(rvir, length, 1, fp); //读取length 长度的代码到  rvir数组中保存
	result = memcmp(signature, rvir, length); //与我们提取的代码比较，返回值放到result中
	if (result == 0)
	{
		Flag = true;
	}
	fclose(fp);
	return Flag;
}

void generateBadChar(char* b, int m, int* badchar)//(模式串字符b，模式串长度m，模式串的哈希表)
{
	int i, ascii;
	for (i = 0; i < SIZE; ++i)
	{
		badchar[i] = -1;//哈希表初始化为-1
	}
	for (i = 0; i < m; ++i)
	{
		ascii = int(b[i]);  //计算字符的ASCII值
		badchar[ascii] = i;//重复字符被覆盖，记录的是最后出现的该字符的位置
	}
}

void generateGS(char* b, int m, int* suffix, bool* prefix)//预处理模式串，填充suffix，prefix
{
	int i, j, k;
	for (i = 0; i < m; ++i)//两个数组初始化
	{
		suffix[i] = -1;
		prefix[i] = false;
	}
	for (i = 0; i < m - 1; ++i)//b[0,i]
	{
		j = i;
		k = 0;//公共后缀子串长度(模式串尾部取k个出来，分别比较)
		while (j >= 0 && b[j] == b[m - 1 - k])//与b[0,m-1]求公共后缀子串
		{
			--j;
			++k;
			suffix[k] = j + 1;
			//相同后缀子串长度为k时，该子串在b[0,i]中的起始下标
			// (如果有多个相同长度的子串，被赋值覆盖，存较大的)
		}
		if (j == -1)//查找到模式串的头部了
			prefix[k] = true;//如果公共后缀子串也是模式串的前缀子串
	}
}

int moveByGS(int j, int m, int* suffix, bool* prefix)//传入的j是坏字符对应的模式串中的字符下标
{
	int k = m - 1 - j;//好后缀长度
	if (suffix[k] != -1)//case1，找到跟好后缀一样的模式子串（多个的话，存的靠后的那个（子串起始下标））
		return j - suffix[k] + 1;
	for (int r = j + 2; r < m; ++r)//case2
	{
		if (prefix[m - r] == true)//m-r是好后缀的子串的长度，如果这个好后缀的子串是模式串的前缀子串
			return r;//在上面没有找到相同的好后缀下，移动r位，对齐前缀到好后缀
	}
	return m;//case3,都没有匹配的，移动m位（模式串长度）
}

int str_bm(char* a, int n, char* b, int m)//a表示主串，长n; b表示模式串,长m
{
	int* badchar = new int[SIZE];//记录模式串中每个字符最后出现的位置
	generateBadChar(b, m, badchar);     //构建坏字符哈希表
	int* suffix = new int[m];
	bool* prefix = new bool[m];
	generateGS(b, m, suffix, prefix);   //预处理模式串，填充suffix，prefix
	int i = 0, j, moveLen1, moveLen2;//j表示主串与模式串匹配的第一个字符
	while (i < n - m + 1)
	{
		for (j = m - 1; j >= 0; --j)  //模式串从后往前匹配
		{
			if (a[i + j] != b[j])
				break;  //坏字符对应模式串中的下标是j
		}
		if (j < 0)   //匹配成功
		{
			delete[] badchar;
			delete[] suffix;
			delete[] prefix;
			return true;   //返回true
		}
		//这里等同于将模式串往后滑动 j-badchar[int(a[i+j])] 位
		moveLen1 = j - badchar[int(a[i + j])];//按照坏字符规则移动距离
		moveLen2 = 0;
		if (j < m - 1)//如果有好后缀的话
		{
			moveLen2 = moveByGS(j, m, suffix, prefix);//按照好后缀规则移动距离
		}
		i = i + max(moveLen1, moveLen2);//取大的移动
	}
	delete[] badchar;
	delete[] suffix;
	delete[] prefix;
	return false;   //匹配失败
}