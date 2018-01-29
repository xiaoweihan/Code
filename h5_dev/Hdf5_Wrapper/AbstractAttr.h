/*********************************************************
Copyright(C):
FileName:AbstractAttr.h
Descripton:brief
Author:xiaowei.han
Data:2018/01/29
Others:
History:
Version:1.0
*********************************************************/
#pragma once
#include <H5Cpp.h>
class CAbstractAttr
{
public:
	CAbstractAttr();
	virtual ~CAbstractAttr();


public:
	//∂¡ Ù–‘
	virtual void WriteSelf(H5::DataSet& ElementDataSet) = 0;

	//–¥ Ù–‘
	virtual void WriteSelf(H5::Group& GroupNode) = 0;
};

