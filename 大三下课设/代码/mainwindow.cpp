#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <qdir.h>
#include<cstdio>
#include<Windows.h>
#include <QDebug>
#include <QFileDialog>
#include <QString>
#include <QTextEdit>
#include <QCheckBox>
#include <cstring>
#include<iostream>
#include<detours.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

QString s;
WCHAR EXE[MAX_PATH + 1];
WCHAR DirPath[MAX_PATH + 1];
char DLLPath[MAX_PATH + 1];

void MainWindow::on_pushButton_clicked()
{

    if(ui->checkBox_ALL->isChecked())
    {
        wcscpy_s(DirPath, MAX_PATH, L"C:\\Users\\12432\\Desktop\\qt\\qt\\all\\Dll\\Debug");
        strcpy_s(DLLPath, MAX_PATH, "C:\\Users\\12432\\Desktop\\qt\\qt\\all\\Dll\\Debug\\Dll.dll");
    }
    else if (ui->checkBox_messagebox->isChecked())
    {
        wcscpy_s(DirPath, MAX_PATH, L"C:\\Users\\12432\\Desktop\\qt\\qt\\all\\MessageBox_\\Dll3\\Debug");
        strcpy_s(DLLPath, MAX_PATH, "C:\\Users\\12432\\Desktop\\qt\\qt\\all\\MessageBox_\\Dll3\\Debug\\Dll3.dll");
    }
    else if (ui->checkBox_file->isChecked())
    {
        wcscpy_s(DirPath, MAX_PATH, L"C:\\Users\\12432\\Desktop\\qt\\qt\\all\\File_\\Dll1\\Debug");
        strcpy_s(DLLPath, MAX_PATH, "C:\\Users\\12432\\Desktop\\qt\\qt\\all\\File_\\Dll1\\Debug\\Dll1.dll");
    }
    else if (ui->checkBox_heap->isChecked())
    {
        wcscpy_s(DirPath, MAX_PATH, L"C:\\Users\\12432\\Desktop\\qt\\qt\\all\\Heap_\\Dll2\\Debug");
        strcpy_s(DLLPath, MAX_PATH, "C:\\Users\\12432\\Desktop\\qt\\qt\\all\\Heap_\\Dll2\\Debug\\Dll2.dll");

    }
    else if (ui->checkBox_reg->isChecked())
    {
        wcscpy_s(DirPath, MAX_PATH, L"C:\\Users\\12432\\Desktop\\qt\\qt\\all\\Reg_\\Dll4\\Debug");
        strcpy_s(DLLPath, MAX_PATH, "C:\\Users\\12432\\Desktop\\qt\\qt\\all\\Reg_\\Dll4\\Debug\\Dll4.dll");
    }


        STARTUPINFO si;
        PROCESS_INFORMATION pi;
        ZeroMemory(&si, sizeof(STARTUPINFO));
        ZeroMemory(&pi, sizeof(PROCESS_INFORMATION));
        si.cb = sizeof(STARTUPINFO);

        if (DetourCreateProcessWithDllEx(EXE, NULL, NULL, NULL, TRUE, CREATE_DEFAULT_ERROR_MODE | CREATE_SUSPENDED, NULL, DirPath, &si, &pi, DLLPath, NULL))
        {
            ResumeThread(pi.hThread);
            WaitForSingleObject(pi.hProcess, INFINITE);
        }
        else {
            char error[100];
            sprintf_s(error, "%d", GetLastError());
        }

}

void MainWindow::on_toolButton_clicked()
{
    s = QFileDialog::getOpenFileName(
                        this, "选择要测试的程序",
                        "/",
                        "(*.exe);;");
    wcscpy_s(reinterpret_cast<wchar_t*>(EXE),MAX_PATH + 1,reinterpret_cast<const wchar_t*>(s.utf16()));
    ui->text_path->setText(s);
}
