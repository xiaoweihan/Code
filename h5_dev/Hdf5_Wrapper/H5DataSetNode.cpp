/*********************************************************
Copyright(C):
FileName:H5DataSetNode.cpp
Descripton:组合模式叶子节点的类
Author:xiaowei.han
Data:2018/01/29
Others:
History:
Version:1.0
*********************************************************/
#include "stdafx.h"
#include "H5DataSetNode.h"
#include "ErrorCode.h"

/*********************************************************
FunctionName:GetChildNum
FunctionDesc:
InputParam:
OutputParam:
Return:
Author:xiaowei.han
*********************************************************/
unsigned int CH5DataSetNode::GetChildNum(void)
{
	return 0;
}

/*********************************************************
FunctionName:CreateSelfToFile
FunctionDesc:
InputParam:
OutputParam:
Return:
Author:xiaowei.han
*********************************************************/
int CH5DataSetNode::CreateSelfToFile(H5::H5File& H5Root)
{
	using namespace std;
	string strAbsoluteName = GetAbsoluteName();
	return ERROR_NO_ERROR;
}
