/*********************************************************
Copyright(C):
FileName:AbstractH5Strategy.h
Descripton:抽象策略模式
Author:xiaowei.han
Data:2018/01/29
Others:
History:
Version:1.0
*********************************************************/
#pragma once
#include "Type.h"
class CAbstractH5Strategy
{
public:
	CAbstractH5Strategy();
	virtual ~CAbstractH5Strategy();


public:
	//读取H5
	virtual int ReadH5() = 0;
	//写入h5
	virtual int WriteH5(InnerType::LP_HDF5_WRITE_DATA pData) = 0;
};

