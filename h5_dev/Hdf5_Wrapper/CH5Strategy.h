/*********************************************************
Copyright(C):
FileName:CH5Strategy.h
Descripton:C模式操作h5算法
Author:xiaowei.han
Data:2018/01/29
Others:
History:
Version:1.0
*********************************************************/
#pragma once
#include "AbstractH5Strategy.h"
class CCH5Strategy :
	public CAbstractH5Strategy
{
public:
	CCH5Strategy();
	~CCH5Strategy();
public:
	//读取H5
	int ReadH5() override;
	//写入h5
	int WriteH5(InnerType::LP_HDF5_WRITE_DATA pData) override;
};

