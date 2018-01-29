/*********************************************************
Copyright(C):
FileName:H5GroupNode.cpp
Descripton:组节点类
Author:xiaowei.han
Data:2018/01/29
Others:
History:
Version:1.0
*********************************************************/
#include "stdafx.h"
#include "H5GroupNode.h"
#include <algorithm>
#include "ErrorCode.h"

/*********************************************************
FunctionName:AppendChild
FunctionDesc:
InputParam:
OutputParam:
Return:
Author:xiaowei.han
*********************************************************/
void CH5GroupNode::AppendChild(CH5AbstractNode* pChildNode)
{
	if (nullptr == pChildNode)
	{
		return;
	}

	auto Iter = std::find(m_ChildList.begin(), m_ChildList.end(), pChildNode);

	if (Iter == m_ChildList.end())
	{
		m_ChildList.push_back(pChildNode);
	}

}

/*********************************************************
FunctionName:RemoveChild
FunctionDesc:
InputParam:
OutputParam:
Return:
Author:xiaowei.han
*********************************************************/
void CH5GroupNode::RemoveChild(CH5AbstractNode* pChildNode)
{
	if (nullptr == pChildNode)
	{
		return;
	}

	auto Iter = std::find(m_ChildList.begin(), m_ChildList.end(), pChildNode);

	if (Iter != m_ChildList.end())
	{
		m_ChildList.erase(Iter);
	}

}

/*********************************************************
FunctionName:GetChildNum
FunctionDesc:
InputParam:
OutputParam:
Return:
Author:xiaowei.han
*********************************************************/
unsigned int CH5GroupNode::GetChildNum(void)
{

	unsigned int nResult = 0;
	//获取其下所有子节点的个数
	for (auto& ChildNode : m_ChildList)
	{
		if (nullptr != ChildNode)
		{
			nResult += ChildNode->GetChildNum();
		}	
	}
	return nResult;
}

/*********************************************************
FunctionName:CreateSelfToFile
FunctionDesc:
InputParam:
OutputParam:
Return:
Author:xiaowei.han
*********************************************************/
int CH5GroupNode::CreateSelfToFile(H5::H5File& H5Root)
{

	using namespace std;

	string strAbsoluteName = GetAbsoluteName();

	return ERROR_NO_ERROR;
}
