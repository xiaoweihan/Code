/*********************************************************
Copyright(C):
FileName:IntAttr.h
Descripton:整型属性的处理
Author:xiaowei.han
Data:2018/01/29
Others:
History:
Version:1.0
*********************************************************/
#pragma once
#include "AbstractAttr.h"
class CIntAttr :
	public CAbstractAttr
{
public:
	CIntAttr();
	~CIntAttr();

public:
	//读属性
	void WriteSelf(H5::DataSet& ElementDataSet) override;

	//写属性
	void WriteSelf(H5::Group& GroupNode) override;
};

