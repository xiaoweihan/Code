/*********************************************************
Copyright(C):
FileName:H5_Wrapper.h
Descripton:HDF5 封装
Author:xiaowei.han
Data:2017/11/28
Others:
History:
Version:1.0
*********************************************************/
#ifndef __H5_WRAPPER_H__
#define __H5_WRAPPER_H__
#include "Hdf5_Wrapper.h"
class CH5Wrapper
{
public:
	CH5Wrapper() = default;
	~CH5Wrapper() = default;
public:
	//写Hdf5文件
	int WriteHdf5File(void);
	//读取hdf5文件
	int ReadHdf5File(void);
	//设置要写的数据
	void SetData(Hdf5_Wrapper::LP_HDF5_DATA pData);
	//设置文件路径
	void SetFilePath(const std::string& strFilePath);
private:
	//待写的数据
	Hdf5_Wrapper::LP_HDF5_DATA m_pData = nullptr;
	//HDF5文件所在的路径
	std::string m_strFilePath;
};

#endif
