/*********************************************************
Copyright(C):
FileName:H5Contex.h
Descripton:用于解析h5文件的执行上下文
Author:xiaowei.han
Data:2018/01/29
Others:
History:
Version:1.0
*********************************************************/
#ifndef H5_CONTEX_H
#define H5_CONTEX_H
#include "Type.h"
class CAbstractH5Strategy;
class CH5Contex
{
public:
	CH5Contex() = default;
	~CH5Contex() = default;

public:
	//设置执行策略
	void SetH5Strategy(CAbstractH5Strategy* pHandler);

	//读取H5
	int ReadH5();

	//写入H5
	int WriteH5(InnerType::LP_HDF5_WRITE_DATA pData);

private:
	CAbstractH5Strategy* m_pStrategyHandler = nullptr;

};
#endif
