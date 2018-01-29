// Hdf5_Wrapper.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "Hdf5_Wrapper.h"
#include "H5_Wrapper.h"
#include "Log.h"
#include "Authenticate.h"
#include "ErrorCode.h"

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

	if (!CAuthentication::CreateInstance().AuthCall())
	{
		return ERROR_SIGNATURE_EXPIRED;
	}


	//参数合法性判断
	if (nullptr == szFilePath || nullptr == pData)
	{
		ERROR_LOG("the param is invalid.");
		return ERROR_PARAM_INVALID;
	}
	CH5Wrapper Wrapper;
	
	Wrapper.SetData(pData);
	Wrapper.SetFilePath(szFilePath);
	return Wrapper.WriteHdf5File();
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

	if (!CAuthentication::CreateInstance().AuthCall())
	{
		return ERROR_SIGNATURE_EXPIRED;
	}

	//参数合法性判断
	if (nullptr == szFilePath || nullptr == pData)
	{
		ERROR_LOG("the param is invalid.");
		return ERROR_PARAM_INVALID;
	}
	CH5Wrapper Wrapper;
	Wrapper.SetFilePath(szFilePath);
	int nResult = Wrapper.ReadHdf5File();

	if (0 == nResult)
	{
		*pData = Wrapper.GetData();
	}

	return nResult;
}

/*********************************************************
FunctionName:RecursiveFreeData
FunctionDesc:
InputParam:
OutputParam:
Return:
Author:xiaowei.han
*********************************************************/
static void RecursiveFreeData(Hdf5_Wrapper::LP_HDF5_DATA pData)
{
	if (nullptr == pData)
	{
		return;
	}
	using namespace Hdf5_Wrapper;
	//释放attr数据
	for (auto& AttrElement : pData->AttributeArray)
	{
		if (nullptr != AttrElement.pData)
		{
			delete[] AttrElement.pData;
			AttrElement.pData = nullptr;
		}
	}
	//没有子节点了
	if (pData->SubDataArray.empty())
	{
		//释放DataSet的数据
		if (nullptr != pData->pData)
		{
			delete[]pData->pData;
			pData->pData = nullptr;
		}
	}
	else
	{
		//还有子节点
		for (auto& ElementData : pData->SubDataArray)
		{
			RecursiveFreeData(ElementData);
			//释放自己
			if (nullptr != ElementData)
			{
				delete ElementData;
				ElementData = nullptr;
			}
		}
		pData->SubDataArray.clear();
	}
}

/*********************************************************
FunctionName:RecyleData
FunctionDesc:回收内存
InputParam:
OutputParam:
Return:0:成功 非0:错误码
Author:xiaowei.han
*********************************************************/
int Hdf5_Wrapper::RecyleData(LP_HDF5_DATA* pData)
{
	if (nullptr != pData)
	{
		RecursiveFreeData(*pData);

		if (nullptr != *pData)
		{
			delete *pData;
			*pData = nullptr;
		}
	}
	return ERROR_NO_ERROR;
}

