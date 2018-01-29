/*********************************************************
Copyright(C):
FileName:CppH5Strategy.cpp
Descripton:C++模式操作h5算法
Author:xiaowei.han
Data:2018/01/29
Others:
History:
Version:1.0
*********************************************************/
#include "stdafx.h"
#include "CppH5Strategy.h"
#include "ErrorCode.h"

CCppH5Strategy::CCppH5Strategy()
{
}


CCppH5Strategy::~CCppH5Strategy()
{
}

/*********************************************************
FunctionName:ReadH5
FunctionDesc:
InputParam:
OutputParam:
Return:
Author:xiaowei.han
*********************************************************/
int CCppH5Strategy::ReadH5()
{
	return ERROR_NO_ERROR;
}

/*********************************************************
FunctionName:WriteH5
FunctionDesc:
InputParam:
OutputParam:
Return:
Author:xiaowei.han
*********************************************************/
int CCppH5Strategy::WriteH5(InnerType::LP_HDF5_WRITE_DATA pData)
{
	if (nullptr == pData)
	{
		return ERROR_PARAM_INVALID;
	}

	return ERROR_NO_ERROR;
}
