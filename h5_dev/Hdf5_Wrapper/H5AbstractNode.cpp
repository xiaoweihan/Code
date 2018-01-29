/*********************************************************
Copyright(C):
FileName:H5Root.cpp
Descripton:组合模式基类
Author:xiaowei.han
Data:2018/01/26
Others:
History:
Version:1.0
*********************************************************/
#include "stdafx.h"
#include "H5AbstractNode.h"

/*********************************************************
FunctionName:GetAbsoluteName
FunctionDesc:获取绝对路径
InputParam:
OutputParam:
Return:
Author:xiaowei.han
*********************************************************/
std::string CH5AbstractNode::GetAbsoluteName(void)
{
	using namespace std;
	//如果没有父节点
	if (nullptr == m_pParent)
	{
		return "/" + m_strName;
	}

	return m_pParent->GetAbsoluteName() + "/" + m_strName;
}

/*********************************************************
FunctionName:
FunctionDesc:
InputParam:
OutputParam:
Return:
Author:xiaowei.han
*********************************************************/
unsigned int CH5AbstractNode::GetAttrNum()
{

	return static_cast<unsigned int>(m_AttrList.size());
}
