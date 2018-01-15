/*********************************************************
Copyright(C):
FileName:SharedMemory.cpp
Descripton:实现跨平台的IPC
Author:xiaowei.han
Data:2018/01/12
Others:
History:
Version:1.0
*********************************************************/
#include "stdafx.h"
#include "SharedMemory.h"
#include <algorithm>
#include <boost/interprocess/mapped_region.hpp>
#include "ErrorCode.h"

CSharedMemory::CSharedMemory(const std::string& strID, unsigned int nCapacity, bool bCreateFlag /*= true*/):
	m_strID(strID),
	m_nCapacity(nCapacity),
	m_bCreateFlag(bCreateFlag)	
{
	if (m_bCreateFlag)
	{
		using namespace boost::interprocess;
		shared_memory_object::remove(m_strID.c_str());
	}

}

CSharedMemory::~CSharedMemory()
{
	if (m_bCreateFlag)
	{
		using namespace boost::interprocess;
		shared_memory_object::remove(m_strID.c_str());
	}
}

/*********************************************************
FunctionName:CreateSharedMemory
FunctionDesc:创建共享内存
InputParam:
OutputParam:
Return:0:成功 非0:失败
Author:xiaowei.han
*********************************************************/
int CSharedMemory::CreateSharedMemory(void)
{
	using namespace boost::interprocess;
	//首先清空共享内存的ID
	if (m_strID.empty() || m_nCapacity <= 0)
	{
		return ERROR_INVALID_PARAM;
	}

	if (m_pSharedMemObj)
	{
		m_pSharedMemObj.reset();
	}

	if (m_bCreateFlag)
	{
		m_pSharedMemObj = boost::make_shared<shared_memory_object>(create_only, m_strID.c_str(), read_write);

		//设置共享内存空间大小
		if (m_pSharedMemObj)
		{
			m_pSharedMemObj->truncate(m_nCapacity);
			mapped_region shared_memory_mapper(*m_pSharedMemObj, read_write, 0, m_nCapacity);
			memset(shared_memory_mapper.get_address(),0,m_nCapacity);
		}
	}
	else
	{
		m_pSharedMemObj = boost::make_shared<shared_memory_object>(open_only, m_strID.c_str(), read_write);
	}
	if (m_pSharedMemObj)
	{
		return ERROR_NO_ERROR;
	}

	return ERROR_ALLOCATE_FAILED;
}

/*********************************************************
FunctionName:WriteDataToSharedMemory
FunctionDesc:向共享内存写入数据
InputParam:
OutputParam:
Return:
Author:xiaowei.han
*********************************************************/
int CSharedMemory::WriteDataToSharedMemory(char* pData, unsigned int nDataLength)
{
	if (nullptr != pData && nDataLength > 0)
	{
		//判断有多余的空间保证写入
		if (nDataLength <= (m_nCapacity - m_nUseBytes))
		{
			using namespace boost::interprocess;
			mapped_region shared_memory_mapper(*m_pSharedMemObj, read_write, m_nUseBytes, m_nCapacity - m_nUseBytes);
			//准备写入数据
			memcpy(shared_memory_mapper.get_address(), pData, nDataLength);	
			m_nUseBytes += nDataLength;
		}
		else
		{
			return ERROR_NOT_ENOUGH_SPACE;
		}
	}

	return ERROR_NO_ERROR;
}

/*********************************************************
FunctionName:ReadDataFromSharedMemory
FunctionDesc:从共享内存读取数据
InputParam:
OutputParam:
Return:
Author:xiaowei.han
*********************************************************/
int CSharedMemory::ReadDataFromSharedMemory(char* pData, unsigned int nDataLength)
{
	if (nullptr == pData || nDataLength <= 0)
	{
		return ERROR_INVALID_PARAM;
	}
	
	using namespace boost::interprocess;
	mapped_region shared_memory_mapper(*m_pSharedMemObj, read_only);
	unsigned int nShareMemoryLen = static_cast<unsigned int>(shared_memory_mapper.get_size());
	unsigned int nReadLength = std::min<unsigned int>(nDataLength, nShareMemoryLen);
	//准备读取数据
	memcpy(pData, shared_memory_mapper.get_address(), nReadLength);
	return ERROR_NO_ERROR;
}
