/*********************************************************
Copyright(C):
FileName:SharedMemory.h
Descripton:实现跨平台的IPC
Author:xiaowei.han
Data:2018/01/12
Others:
History:
Version:1.0
*********************************************************/
#ifndef __SHARED_MEMORY_H__
#define __SHARED_MEMORY_H__
#include <string>
#include <boost/smart_ptr.hpp>
#include <boost/interprocess/shared_memory_object.hpp>
class CSharedMemory
{

public:
	CSharedMemory(const std::string& strID,unsigned int nCapacity,bool bCreateFlag = true);
	~CSharedMemory();

	//创建一片共享内存区域
	int CreateSharedMemory(void);

	//向共享内存写数据
	int WriteDataToSharedMemory(char* pData, unsigned int nDataLength);

	//从共享内存读取数据
	int ReadDataFromSharedMemory(char* pData, unsigned int nDataLength);

private:
	//共享内存的标识符
	std::string m_strID;
	//共享内存空间的容量
	unsigned int m_nCapacity;
	//已经占用的内存容量
	unsigned int m_nUseBytes = 0;
	//是否创建的标志
	bool m_bCreateFlag;
	//共享内存
	boost::shared_ptr<boost::interprocess::shared_memory_object> m_pSharedMemObj;
};




#endif
