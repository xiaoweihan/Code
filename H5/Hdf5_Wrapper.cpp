// Hdf5_Wrapper.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "Hdf5_Wrapper.h"
#include "H5_Wrapper.h"
#include "Log.h"
#include "Authenticate.h"
#include "ErrorCode.h"
static CH5Wrapper s_Wrapper;

/*********************************************************
FunctionName:Data2Hdf5
FunctionDesc:
InputParam:
OutputParam:
Return:0:成功 非0:错误码
Author:xiaowei.han
*********************************************************/
int Hdf5_Wrapper::Data2Hdf5(const char* szFilePath, const LP_HDF5_DATA pData)
{
	//参数合法性判断
	if (nullptr == szFilePath || nullptr == pData)
	{
		ERROR_LOG("the param is invalid.");
		return ERROR_PARAM_INVALID;
	}

	CAuthentication::CreateInstance().AuthCall();
	s_Wrapper.SetData(pData);
	s_Wrapper.SetFilePath(szFilePath);
	return s_Wrapper.WriteHdf5File();
}

/*********************************************************
FunctionName:Hdf52Data
FunctionDesc:读取HDF5数据到内存
InputParam:
OutputParam:
Return:0:成功 非0:错误码
Author:xiaowei.han
*********************************************************/
int Hdf5_Wrapper::Hdf52Data(const char* szFilePath, LP_HDF5_DATA* pData)
{
	//参数合法性判断
	if (nullptr == szFilePath || nullptr == pData)
	{
		ERROR_LOG("the param is invalid.");
		return ERROR_PARAM_INVALID;
	}
	CAuthentication::CreateInstance().AuthCall();
	s_Wrapper.SetFilePath(szFilePath);
	int nResult = s_Wrapper.ReadHdf5File();

	if (0 == nResult)
	{
		*pData = s_Wrapper.GetData();
	}

	return nResult;
}

/*********************************************************
FunctionName:RecyleData
FunctionDesc:回收内存
InputParam:
OutputParam:
Return:0:成功 非0:错误码
Author:xiaowei.han
*********************************************************/
int Hdf5_Wrapper::RecyleData(void)
{
	s_Wrapper.FreeData();
	return ERROR_NO_ERROR;
}
