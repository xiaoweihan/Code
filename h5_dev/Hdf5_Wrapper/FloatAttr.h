/*********************************************************
Copyright(C):
FileName:FloatAttr.h
Descripton:字符串属性的处理
Author:xiaowei.han
Data:2018/01/29
Others:
History:
Version:1.0
*********************************************************/
#pragma once
#include "AbstractAttr.h"
class CFloatAttr :
	public CAbstractAttr
{
public:
	CFloatAttr();
	~CFloatAttr();

public:
	//读属性
	void WriteSelf(H5::DataSet& ElementDataSet) override;

	//写属性
	void WriteSelf(H5::Group& GroupNode) override;
};

