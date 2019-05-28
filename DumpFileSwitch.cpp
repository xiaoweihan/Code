/*******************************************************************************
Copyright(C):
FileName:DumpFileSwitch.cpp
Description:生成dump文件
Author:xiaowei.han
Date:2016/6/29 12:43
Others:
Histroy:
*******************************************************************************/
#include "stdafx.h"
#include "DumpFileSwitch.h"
#include <DbgHelp.h>
#include <string>
#pragma warning(disable:4996)
#pragma comment(lib,"Dbghelp.lib")

static TCHAR s_DumpFileName[MAX_PATH] = { 0 };

static LONG WINAPI CustomExceptionFilter(EXCEPTION_POINTERS* pException)
{
	if (nullptr == pException)
	{
		return EXCEPTION_CONTINUE_EXECUTION;
	}

	HANDLE hFile = CreateFile(s_DumpFileName,GENERIC_WRITE,0,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
	if (INVALID_HANDLE_VALUE == hFile)
	{
		return EXCEPTION_CONTINUE_EXECUTION;
	}
	MINIDUMP_EXCEPTION_INFORMATION einfo = { 0 };
	einfo.ThreadId = ::GetCurrentThreadId();
	einfo.ExceptionPointers = pException;
	einfo.ClientPointers = FALSE;
	MiniDumpWriteDump(GetCurrentProcess(),GetCurrentProcessId(),hFile,MiniDumpNormal,&einfo,NULL,NULL);
	CloseHandle(hFile);
	return EXCEPTION_EXECUTE_HANDLER;
}


/*****************************************************************************
* @函数名称 : CDumpFileSwitch
* @功能描述 : 构造函数
* @author : xiaowei.han
* @date : 2016/6/29 13:33
* @version : ver 1.0
* @inparam : 
* @outparam : 
*****************************************************************************/
CDumpFileSwitch::CDumpFileSwitch(LPCTSTR pDumpFileName)
{

	memset(s_DumpFileName, 0, sizeof(s_DumpFileName));

	_tcsncpy(s_DumpFileName, pDumpFileName, _tcslen(pDumpFileName));

	SetUnhandledExceptionFilter(CustomExceptionFilter);
}

/*****************************************************************************
* @函数名称 : CDumpFileSwitch
* @功能描述 : 析构函数
* @author : xiaowei.han
* @date : 2016/6/29 13:33
* @version : ver 1.0
* @inparam : 
* @outparam : 
*****************************************************************************/
CDumpFileSwitch::~CDumpFileSwitch( void )
{

}

