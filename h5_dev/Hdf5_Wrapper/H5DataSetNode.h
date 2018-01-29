/*********************************************************
Copyright(C):
FileName:H5DataSetNode.h
Descripton:组合模式叶子节点的类
Author:xiaowei.han
Data:2018/01/29
Others:
History:
Version:1.0
*********************************************************/
#ifndef H5_DATA_SET_NODE_H
#define H5_DATA_SET_NODE_H
#include "H5AbstractNode.h"
class CH5DataSetNode :
	public CH5AbstractNode
{
public:
	CH5DataSetNode() = default;
	~CH5DataSetNode() =default;

public:
	//获取节点个数
	unsigned int GetChildNum(void) override;

	//创建自身
	int CreateSelfToFile(H5::H5File& H5Root) override;
};

#endif