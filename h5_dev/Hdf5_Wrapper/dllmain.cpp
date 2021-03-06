// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include <boost/filesystem.hpp>
#include "Utility.h"
#include "Log.h"
#include "Authenticate.h"

#ifdef _DEBUG
#pragma comment(lib,"libhdf5_D.lib")
#pragma comment(lib,"libhdf5_cpp_D.lib")
#pragma comment(lib,"libszip_D.lib")
#pragma comment(lib,"libzlib_D.lib")
#pragma comment(lib,"libhdf5_hl_D.lib")
#pragma comment(lib,"libhdf5_tools_D.lib")
#else
#pragma comment(lib,"libhdf5.lib")
#pragma comment(lib,"libhdf5_cpp.lib")
#pragma comment(lib,"libszip.lib")
#pragma comment(lib,"libzlib.lib")
#pragma comment(lib,"libhdf5_hl.lib")
#pragma comment(lib,"libhdf5_tools.lib")
#endif

static void Init(void);

static void UnInit(void);

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		DisableThreadLibraryCalls(hModule);
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

void Init(void)
{
	using namespace std;
	using namespace boost::filesystem;
	//初始化打印日志
	string strLogDir = Utility::GetExeDirecory() + std::string("\\H5WrapperLog\\");
	create_directories(strLogDir);
	CLog::CreateInstance().SetLogPath(strLogDir.c_str());
	CLog::CreateInstance().SetLogNamePrefix("H5Wrapper");
#ifdef _DEBUG
	CLog::CreateInstance().SetLogLevel(LOG_DEBUG);
#else
	CLog::CreateInstance().SetLogLevel(LOG_ERROR);
#endif

	CAuthentication::CreateInstance().LoadCallTimes();
	CAuthentication::CreateInstance().On();
}

void UnInit(void)
{
	CAuthentication::CreateInstance().SaveCallTimes();
}