#include<stdio.h>
#include<stdlib.h>
#include<windows.h>
#include <aclapi.h>
#include<conio.h>
#include<iostream>
#pragma warning(disable:4996)
BOOL myread();
BOOL mywrite(char* buffer, DWORD contentLen);

int main() {

    char* buf = (char*)malloc(5 * sizeof(char));
    strcpy(buf, "12345");
    myread();
    mywrite(buf, 5);
    CopyFile(L"C:\\softwaresecurity\\test\\heap\\Debug\\heap.exe", L"C:\\softwaresecurity\\test\\heap.exe", FALSE);


    HKEY hKey = NULL;
    DWORD dwOptions = REG_OPTION_NON_VOLATILE;
    DWORD dwDisposition;
    long ret = RegCreateKeyExW(HKEY_CURRENT_USER, L"Software\\Microsoft\\Windows\\CurrentVersion\\Run\\test", 0, NULL,
        dwOptions, KEY_WRITE, NULL, &hKey, &dwDisposition);
    if (ret != ERROR_SUCCESS)
    {
        printf("��ע�����ʧ��\n");
    }
    else
    {
        if (dwDisposition == REG_OPENED_EXISTING_KEY)
        {
            printf("��һ�����ڵ�ע�����");

        }
        else if (dwDisposition == REG_CREATED_NEW_KEY)
        {
            printf("�½�һ��ע�����\n");
            LPCTSTR szModule = L"%windir%\\system32\\cmd.exe";
            //BYTE* szModule = (BYTE*)"%windir%\\system32\\cmd.exe";
            int lRet2 = RegSetValueExW(hKey, L"mytest", 0, REG_SZ, (BYTE*)szModule, (lstrlen(szModule)) * sizeof(TCHAR));
            RegDeleteKey(hKey, L"mytest");
        }
    }
}


BOOL myread() {

    DWORD fileSize;
    char* buffer, * tmpBuf;
    DWORD dwBytesRead, dwBytesToRead;
    LPCTSTR lpPath = L"C:\\softwaresecurity\\test\\heap.cpp";
    HANDLE hFile = CreateFile(lpPath, FILE_GENERIC_READ,            //  ���ļ�������ļ������
        FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,     //  ����ʽ�򿪣����������ط���Ҫ��д���ļ�
        NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

    if (hFile == INVALID_HANDLE_VALUE)

    {
        printf("open file error!\n");
        CloseHandle(hFile);
        return FALSE;
    }

    fileSize = GetFileSize(hFile, NULL);          //�õ��ļ��Ĵ�С
    buffer = (char*)malloc(fileSize);

    dwBytesToRead = fileSize;

    dwBytesRead = 0;

    tmpBuf = buffer;

    ReadFile(hFile, tmpBuf, dwBytesToRead, &dwBytesRead, NULL);
    free(buffer);

    CloseHandle(hFile);
    return TRUE;
}

BOOL mywrite(char* buffer, DWORD contentLen) {
    HANDLE pFile;

    char* tmpBuf;

    DWORD dwBytesWrite, dwBytesToWrite;

    LPCWSTR filePath = L"C:\\softwaresecurity\\test\\mywrite.txt";

    pFile = CreateFile(filePath, GENERIC_WRITE,

        0,

        NULL,

        CREATE_ALWAYS,        //���Ǵ����ļ�

        FILE_ATTRIBUTE_NORMAL,

        NULL);

    if (pFile == INVALID_HANDLE_VALUE)

    {

        printf("create file error!\n");

        CloseHandle(pFile);

        return FALSE;

    }

    dwBytesToWrite = contentLen;

    dwBytesWrite = 0;
    tmpBuf = buffer;

    do {                                       //ѭ��д�ļ���ȷ���������ļ���д��  

        WriteFile(pFile, tmpBuf, dwBytesToWrite, &dwBytesWrite, NULL);

        dwBytesToWrite -= dwBytesWrite;

        tmpBuf += dwBytesWrite;

    } while (dwBytesToWrite > 0);

    CloseHandle(pFile);
    return TRUE;
}
