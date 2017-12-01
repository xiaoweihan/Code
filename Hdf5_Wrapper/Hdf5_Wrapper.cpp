// Hdf5_Wrapper.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "Hdf5_Wrapper.h"
#include "H5_Wrapper.h"
#include "Log.h"
#include "Authenticate.h"
static CH5Wrapper s_Wrapper;

/*********************************************************
FunctionName:Data2Hdf5
FunctionDesc:
InputParam:
OutputParam:
Return:
Author:xiaowei.han
*********************************************************/
int Hdf5_Wrapper::Data2Hdf5(const char* szFilePath, const LP_HDF5_DATA pData)
{
	if (nullptr == szFilePath || nullptr == pData)
	{
		ERROR_LOG("the param is invalid.");
		return -1;
	}

	CAuthentication::CreateInstance().AuthCall();
	s_Wrapper.SetData(pData);
	s_Wrapper.SetFilePath(szFilePath);
	return s_Wrapper.WriteHdf5File();
}

/*********************************************************
FunctionName:Hdf52Data
FunctionDesc:
InputParam:
OutputParam:
Return:
Author:xiaowei.han
*********************************************************/
int Hdf5_Wrapper::Hdf52Data(const char* szFilePath, LP_HDF5_DATA pData)
{
	if (nullptr == szFilePath || nullptr == pData)
	{
		ERROR_LOG("the param is invalid.");
		return -1;
	}
	CAuthentication::CreateInstance().AuthCall();
	s_Wrapper.SetFilePath(szFilePath);
	return s_Wrapper.ReadHdf5File();
}
