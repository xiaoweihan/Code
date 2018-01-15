/*********************************************************
Copyright(C):
FileName:IPCSyncLock.cpp
Descripton:进程同步锁
Author:xiaowei.han
Data:2018/01/15
Others:
History:
Version:1.0
*********************************************************/
#include "stdafx.h"
#include "IPCSyncLock.h"
#include <boost/interprocess/sync/scoped_lock.hpp>
#include "ErrorCode.h"

CIPCSyncLock::CIPCSyncLock(const std::string& strID,bool bCreateFlag):
	m_strID(strID),
	m_bCreateFlag(bCreateFlag)
{
	if (m_bCreateFlag)
	{
		using namespace boost::interprocess;
		named_mutex::remove(m_strID.c_str());
	}
}

CIPCSyncLock::~CIPCSyncLock()
{
	if (m_bCreateFlag)
	{
		using namespace boost::interprocess;
		named_mutex::remove(m_strID.c_str());
	}
}

/*********************************************************
FunctionName:Lock
FunctionDesc:
InputParam:
OutputParam:
Return:
Author:xiaowei.han
*********************************************************/
void CIPCSyncLock::Lock()
{
	if (m_pLock)
	{
		using namespace boost::interprocess;

		try
		{
			m_pLock->lock();
		}
		catch (interprocess_exception&)
		{

		}	
	}
}

/*********************************************************
FunctionName:UnLock
FunctionDesc:
InputParam:
OutputParam:
Return:
Author:xiaowei.han
*********************************************************/
void CIPCSyncLock::UnLock()
{
	if (m_pLock)
	{
		using namespace boost::interprocess;

		try
		{
			m_pLock->unlock();
		}
		catch (interprocess_exception&)
		{

		}
	}
}

/*********************************************************
FunctionName:CreateSyncLock
FunctionDesc:
InputParam:
OutputParam:
Return:
Author:xiaowei.han
*********************************************************/
int CIPCSyncLock::CreateSyncLock()
{
	using namespace boost::interprocess;
	
	//ID 合法性判断
	if (m_strID.empty())
	{
		return ERROR_INVALID_PARAM;
	}
	if (m_pLock)
	{
		m_pLock.reset();
	}

	if (m_bCreateFlag)
	{
		m_pLock = boost::make_shared<boost::interprocess::named_mutex>(create_only, m_strID.c_str());
	}
	else
	{
		m_pLock = boost::make_shared<boost::interprocess::named_mutex>(open_only, m_strID.c_str());
	}
	
	if (m_pLock)
	{
		return ERROR_NO_ERROR;
	}
	return ERROR_ALLOCATE_FAILED;
}

