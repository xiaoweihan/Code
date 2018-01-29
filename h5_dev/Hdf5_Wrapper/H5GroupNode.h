/*********************************************************
Copyright(C):
FileName:H5GroupNode.h
Descripton:组节点类
Author:xiaowei.han
Data:2018/01/29
Others:
History:
Version:1.0
*********************************************************/
#ifndef H5_GROUP_NODE_H
#define H5_GROUP_NODE_H
#include "H5AbstractNode.h"
class CH5GroupNode : public CH5AbstractNode
{
public:
	CH5GroupNode() = default;
	~CH5GroupNode() = default;
public:

	//添加子节点
	void AppendChild(CH5AbstractNode* pChildNode) override;

	//移除子节点
	void RemoveChild(CH5AbstractNode* pChildNode) override;

	//获取节点个数
	unsigned int GetChildNum(void) override;

	//创建自身
	int CreateSelfToFile(H5::H5File& H5Root) override;
};
#endif

