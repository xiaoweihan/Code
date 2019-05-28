/*******************************************************************************
Copyright(C):
FileName:DumpFileSwitch.h
Description:DumpÎÄ¼þ¿ª¹ØÆ÷
Author:xiaowei.han
Date:2016/6/29 12:43
Others:
Histroy:
*******************************************************************************/
#ifndef DUMP_FILE_SWITCH_H
#define DUMP_FILE_SWITCH_H
#ifdef _WIN32
#include <Windows.h>
#endif
class CDumpFileSwitch
{
public:
    CDumpFileSwitch(LPCTSTR pDumpFileName);
    ~CDumpFileSwitch(void);
};



#endif
