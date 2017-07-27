// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "stdafx.h"
#include <string>
#include <boost/filesystem.hpp>
#include "Log.h"
#include "Utility.h"
#pragma comment(lib,"libmysql.lib")
#pragma comment(lib,"ws2_32.lib")
//初始化
static void Init(void);
//反初始化
static void UnInit(void);

/**************************************************************************
@FunctionName:UnInit
@FunctionDestription:反初始化
@InputParam:
@OutPutParam:
@ReturnValue:
**************************************************************************/
void UnInit(void)
{

}

/**************************************************************************
@FunctionName:Init
@FunctionDestription:初始化
@InputParam:
@OutPutParam:
@ReturnValue:
**************************************************************************/
void Init(void)
{
	using namespace std;
	string strLogDir = Utility::GetExeDirecory() + string(R"(\mysql_operator_log)");
	//建立日志文件夹
	boost::filesystem::create_directories(strLogDir);

	CLog::CreateInstance().SetLogPath(strLogDir.c_str());
	CLog::CreateInstance().SetLogNamePrefix("mysql_operator");

#ifdef _DEBUG
	CLog::CreateInstance().SetLogLevel(LOG_DEBUG);
#else
	CLog::CreateInstance().SetLogLevel(LOG_ERROR);
#endif


}

BOOL APIENTRY DllMain(HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		Init();
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
		UnInit();
		break;
	default:
		break;
	}
	return TRUE;
}

