/**************************************************************************
Copyright(C):
FileName:InterProcessStyleKeepAlive.h
Description:使用进程间通信方式保持心跳
Author:xiaowei.han
Date:2017/03/31
Others:
Histroy:
**************************************************************************/
#include "stdafx.h"
#include "InterProcessStyleKeepAlive.h"
#include <boost/interprocess/sync/named_mutex.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/core/ignore_unused.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/date_time/posix_time/ptime.hpp>
#include <boost/date_time.hpp>
#include <iostream>
//共享内存
static boost::interprocess::shared_memory_object s_SharedMemory;
//命名互斥体
static boost::shared_ptr<boost::interprocess::named_mutex> s_NamedMutex;
CInterProcessStyleKeepAlive::CInterProcessStyleKeepAlive(const std::string& strSharedMemoryName, const std::string& strNamedMutexName):
m_strNamedMutexName(strNamedMutexName),
m_strSharedMemoryName(strSharedMemoryName)
{
	UnInitSharedMemory();
	UnInitNamedMutex();
	InitNamedMutex();
	InitSharedMemory();
}

CInterProcessStyleKeepAlive::~CInterProcessStyleKeepAlive(void)
{
	UnInitSharedMemory();
	UnInitNamedMutex();
}

void CInterProcessStyleKeepAlive::KeepAlive(void)
{

	try
	{
		//映射共享内存
		boost::interprocess::mapped_region region(s_SharedMemory, boost::interprocess::read_write);
		//获取当前时间
		time_t currentTime = time(nullptr);

		boost::posix_time::ptime T = boost::posix_time::from_time_t(currentTime);

		std::cout << boost::posix_time::to_iso_extended_string(T) << std::endl;

		if (s_NamedMutex)
		{
			s_NamedMutex->lock();
			//拷贝值进去
			memcpy(region.get_address(), &currentTime, sizeof(currentTime));
			s_NamedMutex->unlock();
		}
	}
	catch (boost::interprocess::interprocess_exception& e)
	{
		boost::ignore_unused(e);
	}

}

/**************************************************************************
@FunctionName:UnInitSharedMemory
@FunctionDestription:反初始化共享内存
@InputParam:
@OutPutParam:
@ReturnValue:
**************************************************************************/
void CInterProcessStyleKeepAlive::UnInitSharedMemory(void)
{

	try
	{
		if (!m_strSharedMemoryName.empty())
		{
			boost::interprocess::shared_memory_object::remove(m_strSharedMemoryName.c_str());
		}
		
	}
	catch (boost::interprocess::interprocess_exception& e)
	{
		boost::ignore_unused(e);
	}
}

/**************************************************************************
@FunctionName:InitSharedMemory
@FunctionDestription:初始化共享内存
@InputParam:
@OutPutParam:
@ReturnValue:
**************************************************************************/
void CInterProcessStyleKeepAlive::InitSharedMemory(void)
{
	//参数合法性判断
	if (m_strSharedMemoryName.empty())
	{
		return;
	}

	try
	{
		//打开一个共享内存
		boost::interprocess::shared_memory_object shm_obj(boost::interprocess::create_only,
			m_strSharedMemoryName.c_str(), boost::interprocess::read_write);

		s_SharedMemory = boost::move(shm_obj);

		//设置共享内存的容量设置为8个字节
		s_SharedMemory.truncate(sizeof(time_t));

	}
	catch (boost::interprocess::interprocess_exception& e)
	{
		boost::ignore_unused(e);
	}
}

/**************************************************************************
@FunctionName:InitNamedMutex
@FunctionDestription:初始化命名互斥体
@InputParam:
@OutPutParam:
@ReturnValue:
**************************************************************************/
void CInterProcessStyleKeepAlive::InitNamedMutex(void)
{
	try
	{
		if (!s_NamedMutex)
		{
			s_NamedMutex = boost::make_shared<boost::interprocess::named_mutex>(boost::interprocess::create_only, m_strNamedMutexName.c_str());
		}
	}
	catch (boost::interprocess::interprocess_exception& e)
	{
		boost::ignore_unused(e);
	}
	
}

/**************************************************************************
@FunctionName:UnInitNamedMutex
@FunctionDestription:反初始化命名互斥体
@InputParam:
@OutPutParam:
@ReturnValue:
**************************************************************************/
void CInterProcessStyleKeepAlive::UnInitNamedMutex(void)
{
	try
	{
		if (m_strNamedMutexName.empty())
		{
			boost::interprocess::named_mutex::remove(m_strNamedMutexName.c_str());
		}
		
	}
	catch (boost::interprocess::interprocess_exception& e)
	{
		boost::ignore_unused(e);
	}
	
}


