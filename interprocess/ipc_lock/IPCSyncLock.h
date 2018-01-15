/*********************************************************
Copyright(C):
FileName:IPCSyncLock.h
Descripton:进程同步锁
Author:xiaowei.han
Data:2018/01/15
Others:
History:
Version:1.0
*********************************************************/
#ifndef IPC_SYNC_LOCK_H
#define IPC_SYNC_LOCK_H
#include <string>
#include <boost/interprocess/sync/named_mutex.hpp>
#include <boost/noncopyable.hpp>
#include <boost/smart_ptr.hpp>
class CIPCSyncLock : boost::noncopyable
{
public:
	CIPCSyncLock(const std::string& strID,bool bCreateFlag = true);
	~CIPCSyncLock();

	//创建同步锁
	int CreateSyncLock();

	//开锁
	void Lock();

	//解锁
	void UnLock();
private:
	//同步对象的ID
	std::string m_strID;
	//是否创建标志
	bool m_bCreateFlag;
	//同步锁
	boost::shared_ptr<boost::interprocess::named_mutex> m_pLock;
};


#endif
