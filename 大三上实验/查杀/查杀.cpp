#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include <stdio.h>
#include<string.h>
#include<Windows.h>
#include<string>
using namespace std;

#define SIZE 256  //�ַ���
#define MAXSIZE 1024  //����BM�㷨���������Χ
bool Direct_CMP(const char* FileName);  //����ƫ��ֵ��ֱ�ӱȶ�
void GetAttribution(const char* FileName);  //��������ļ�����ϸ������Ϣ
void generateBadChar(char* b, int m, int* badchar);//(ģʽ���ַ�b��ģʽ������m��ģʽ���Ĺ�ϣ��)
void generateGS(const char* pattern, int suffix[], bool prefix[]);//���ַ�����
int moveByGS(int j, int m, int* suffix, bool* prefix);//�����j�ǻ��ַ���Ӧ��ģʽ���е��ַ��±�
int str_bm(char* a, int n, char* b, int m);//a��ʾ��������n; b��ʾģʽ��,��m

unsigned const char signature[] ={ 0x54,0x68,0x69,0x73,0x20,0x70,0x72,0x6F,0x67,0x72,0x61,0x6D,0x20,0x63,0x61,0x6E,0x6E,0x6F,0x74,0x20,0x62,0x65,0x20,0x72,0x75,0x6E,0x20,0x69,0x6E,0x20,0x44,0x4F,0x53,0x20,0x6D,0x6F,0x64,0x65 };
//��������ݶ�����뷴���ѡȡ "This program cannot be run in DOS mode" �ܼ�39���ֽ�
long length = 38;  //��¼��������ֽڴ�С

//�����������ɫ
BOOL SetConsoleColor(WORD wa) {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hConsole == INVALID_HANDLE_VALUE)
		return FALSE;

	return SetConsoleTextAttribute(hConsole, wa);
}

int main()
{
	char Virus[100]; //���鲡�����ļ�·��
	bool flag = false;

	while (1)
	{
		system("cls");
		memset(Virus, 0, sizeof(Virus));
		printf("�����뱻�鲡���ĵ�·��:");
		scanf("%s", Virus);
		int op;
		flag = false;
		cout << "\n\n\n\n\n\n\n";
		printf("\t\t\t\t*-------------------------------------*\n");
		printf("\t\t\t\t*\tѡ����Ҫʹ�õ��㷨:           *\n");
		printf("\t\t\t\t*\t1: ƫ����ֱ�ӱȶ�             *\n");
		printf("\t\t\t\t*\t2: BM�㷨                     *\n");
		printf("\t\t\t\t*\t0: �˳�                       *\n");
		printf("\t\t\t\t*-------------------------------------*\n");
		cout << "\t\t\t\t����ѡ��";
		scanf("%d", &op);
		if (op == 1) {
			flag = Direct_CMP(Virus);
			if (flag)//��Ҫ���������ϸ��Ϣ�������������ļ���С������ʱ���
			{
				GetAttribution(Virus);  //��ӡ����ϸ��Ϣ
			}
			if (!flag) printf("û�з��ֲ���!\n");
			cout << "\t\t\t\t"; system("pause");
		}
		else if (op == 2) {
			printf("����������ȶԵ���ʼ�ֽ����ͽ�ֹ�ֽ�����\n");
			long start, end;
			scanf("%ld %ld", &start, &end);
			char TXT[MAXSIZE] = { 0 };
			printf("\n*************ʹ��BMƥ���㷨���в�ɱ*************\n");
			FILE* fp = NULL;
			fp = fopen(Virus, "rb"); //��2���ƴ�ָ���ļ�
			if (fp == NULL)
			{
				printf("�ļ�·������\n");
				continue;
			}
			fseek(fp, start, SEEK_SET); //���ļ�ָ��ָ��ƫ�Ƶ�ַ
			fread(TXT, end - start + 1, 1, fp);
			flag=str_bm(TXT,end-start+1,(char*)signature,length);
			if (flag)//��Ҫ���������ϸ��Ϣ�������������ļ���С������ʱ���
			{
				GetAttribution(Virus);  //��ӡ����ϸ��Ϣ
			}
			if (!flag) printf("û�з��ֲ���!\n");
			cout << "\t\t\t\t"; system("pause");
		}
		else if (op == 0) {
			break;
		}
		else {
			printf("�����Ŵ���!\n");
			continue;
		}
	}

	return 0;
}

/*
��������void GetAttribution(const char* FileName)
��������������ļ���·��
����ֵ����
���ܣ���������ļ�����ϸ������Ϣ
*/
void GetAttribution(const char* FileName) {
	WIN32_FIND_DATAA ffd;//����һ��FIND_DATAA �ṹ��
	HANDLE hFind = FindFirstFileA(FileName, &ffd);  //ʹ�øú�����ø��ļ����������
	SetConsoleColor(FOREGROUND_RED);
	printf("��Ϊ %s ��·���ļ��з����˸��Ƹ�Ⱦ���͵Ĳ�������\n",FileName);
	printf("�ò����ļ��Ĵ���ʱ��:");
	SYSTEMTIME st;
	FileTimeToSystemTime(&(ffd.ftCreationTime), &st);
	printf("%d-%d-%d  %02d:%02d:%02d\n", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
	unsigned long long size = (ffd.nFileSizeHigh << 32) + ffd.nFileSizeLow;
	printf("�ò����ļ��Ĵ�СΪ:");
	cout << size << "�ֽ�\n";
	SetConsoleColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

}


/*
��������Direct_CMP
���������FileName �Ǳ����ļ���·��
����ֵ��True or False
���ܣ��ж��Ƿ��ڸ��ļ���ʶ���������
*/
bool Direct_CMP(const char* FileName)
{
	printf("\n*************ʹ��ƫ��λ��ֱ�ӱȽϷ�����ɱ*************\n");
	FILE* fp = NULL;
	int result = 0;
	char rvir[255] = { 0 };
	bool Flag = false;
	fp = fopen(FileName, "rb"); //��2���ƴ�ָ���ļ�
	if (fp == NULL)
	{
		printf("�ļ�·������\n");
		return Flag;
	}
	printf("������ƫ��λ��:");
	long offset;
	scanf("%ld", &offset);

	fseek(fp, offset, SEEK_SET); //���ļ�ָ��ָ��ƫ�Ƶ�ַ
	fread(rvir, length, 1, fp); //��ȡlength ���ȵĴ��뵽  rvir�����б���
	result = memcmp(signature, rvir, length); //��������ȡ�Ĵ���Ƚϣ�����ֵ�ŵ�result��
	if (result == 0)
	{
		Flag = true;
	}
	fclose(fp);
	return Flag;
}

void generateBadChar(char* b, int m, int* badchar)//(ģʽ���ַ�b��ģʽ������m��ģʽ���Ĺ�ϣ��)
{
	int i, ascii;
	for (i = 0; i < SIZE; ++i)
	{
		badchar[i] = -1;//��ϣ���ʼ��Ϊ-1
	}
	for (i = 0; i < m; ++i)
	{
		ascii = int(b[i]);  //�����ַ���ASCIIֵ
		badchar[ascii] = i;//�ظ��ַ������ǣ���¼���������ֵĸ��ַ���λ��
	}
}

void generateGS(char* b, int m, int* suffix, bool* prefix)//Ԥ����ģʽ�������suffix��prefix
{
	int i, j, k;
	for (i = 0; i < m; ++i)//���������ʼ��
	{
		suffix[i] = -1;
		prefix[i] = false;
	}
	for (i = 0; i < m - 1; ++i)//b[0,i]
	{
		j = i;
		k = 0;//������׺�Ӵ�����(ģʽ��β��ȡk���������ֱ�Ƚ�)
		while (j >= 0 && b[j] == b[m - 1 - k])//��b[0,m-1]�󹫹���׺�Ӵ�
		{
			--j;
			++k;
			suffix[k] = j + 1;
			//��ͬ��׺�Ӵ�����Ϊkʱ�����Ӵ���b[0,i]�е���ʼ�±�
			// (����ж����ͬ���ȵ��Ӵ�������ֵ���ǣ���ϴ��)
		}
		if (j == -1)//���ҵ�ģʽ����ͷ����
			prefix[k] = true;//���������׺�Ӵ�Ҳ��ģʽ����ǰ׺�Ӵ�
	}
}

int moveByGS(int j, int m, int* suffix, bool* prefix)//�����j�ǻ��ַ���Ӧ��ģʽ���е��ַ��±�
{
	int k = m - 1 - j;//�ú�׺����
	if (suffix[k] != -1)//case1���ҵ����ú�׺һ����ģʽ�Ӵ�������Ļ�����Ŀ�����Ǹ����Ӵ���ʼ�±꣩��
		return j - suffix[k] + 1;
	for (int r = j + 2; r < m; ++r)//case2
	{
		if (prefix[m - r] == true)//m-r�Ǻú�׺���Ӵ��ĳ��ȣ��������ú�׺���Ӵ���ģʽ����ǰ׺�Ӵ�
			return r;//������û���ҵ���ͬ�ĺú�׺�£��ƶ�rλ������ǰ׺���ú�׺
	}
	return m;//case3,��û��ƥ��ģ��ƶ�mλ��ģʽ�����ȣ�
}

int str_bm(char* a, int n, char* b, int m)//a��ʾ��������n; b��ʾģʽ��,��m
{
	int* badchar = new int[SIZE];//��¼ģʽ����ÿ���ַ������ֵ�λ��
	generateBadChar(b, m, badchar);     //�������ַ���ϣ��
	int* suffix = new int[m];
	bool* prefix = new bool[m];
	generateGS(b, m, suffix, prefix);   //Ԥ����ģʽ�������suffix��prefix
	int i = 0, j, moveLen1, moveLen2;//j��ʾ������ģʽ��ƥ��ĵ�һ���ַ�
	while (i < n - m + 1)
	{
		for (j = m - 1; j >= 0; --j)  //ģʽ���Ӻ���ǰƥ��
		{
			if (a[i + j] != b[j])
				break;  //���ַ���Ӧģʽ���е��±���j
		}
		if (j < 0)   //ƥ��ɹ�
		{
			delete[] badchar;
			delete[] suffix;
			delete[] prefix;
			return true;   //����true
		}
		//�����ͬ�ڽ�ģʽ�����󻬶� j-badchar[int(a[i+j])] λ
		moveLen1 = j - badchar[int(a[i + j])];//���ջ��ַ������ƶ�����
		moveLen2 = 0;
		if (j < m - 1)//����кú�׺�Ļ�
		{
			moveLen2 = moveByGS(j, m, suffix, prefix);//���պú�׺�����ƶ�����
		}
		i = i + max(moveLen1, moveLen2);//ȡ����ƶ�
	}
	delete[] badchar;
	delete[] suffix;
	delete[] prefix;
	return false;   //ƥ��ʧ��
}