/**************************************************************************
Copyright(C):Envision. Co., Ltd.
FileName:Queue.hpp
Description:同步队列
Author:xiaowei.han
Date:2017/04/10
Others:
Histroy:
**************************************************************************/
#ifndef QUEUE_H
#define QUEUE_H
#include <boost/thread/condition_variable.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/atomic.hpp>
#include <boost/move/move.hpp>
#include <list>
//定义的模板队列
template <typename T,int nMaxElement = 100000>
class SyncQueue
{
public:
	SyncQueue(void)
	{
		m_nMaxElement = nMaxElement;
		m_bNeedExit = false;
	}
	~SyncQueue(void)
	{

	}

	/**************************************************************************
	@FunctionName:Put
	@FunctionDestription:向队列放入元素
	@InputParam:Element:要放入到队列的元素
	@OutPutParam:None
	@ReturnValue:None
	**************************************************************************/
	void Put(const T& Element)
	{
		boost::unique_lock<boost::mutex> Lock(m_syncMutex);

		//判断是否为空
		m_PutCondition.wait(Lock, [this]()->bool 
		{
			return (!(static_cast<int>(m_ElementArray.size()) == m_nMaxElement) || m_bNeedExit);
		});

		//如果需要退出
		if (m_bNeedExit)
		{
			return;
		}

		//放入元素
		m_ElementArray.push_back(boost::move(Element));

		//通知条件变量
		m_GetCondition.notify_all();

	}

	/**************************************************************************
	@FunctionName:Get
	@FunctionDestription:从队列取元素
	@InputParam:None
	@OutPutParam:Element:从队列取的元素
	@ReturnValue:None
	**************************************************************************/
	void Get(T& Element)
	{
		boost::unique_lock<boost::mutex> Lock(m_syncMutex);

		//判断是否为空
		m_GetCondition.wait(Lock, [this]()->bool
		{
			return (!(m_ElementArray.empty()) || m_bNeedExit);
		});

		//如果需要退出
		if (m_bNeedExit)
		{
			return;
		}

		//拿元素
		Element = m_ElementArray.front();

		m_ElementArray.pop_front();

		//通知条件变量
		m_PutCondition.notify_all();
	}

	/**************************************************************************
	@FunctionName:IsQueueEmpty
	@FunctionDestription:判断队列是否为空
	@InputParam:None
	@OutPutParam:None
	@ReturnValue:true:empty false:not empty.
	**************************************************************************/
	bool IsQueueEmpty(void)
	{
		bool bResult = true;
		{
			boost::lock_guard<boost::mutex> Lock(m_syncMutex);
			bResult = m_ElementArray.empty();
		}
		return bResult;

	}

	/**************************************************************************
	@FunctionName:GetQueueSize
	@FunctionDestription:获取队列元素个数
	@InputParam:None
	@OutPutParam:None
	@ReturnValue:队列元素个数
	**************************************************************************/
	int GetQueueSize(void)
	{
		int nSize = 0;
		{
			boost::lock_guard<boost::mutex> Lock(m_syncMutex);
			nSize = static_cast<int>(m_ElementArray.size());
		}
		return nSize;
	}

	/**************************************************************************
	@FunctionName:StopQueue
	@FunctionDestription:停止等待，为了防止程序退出，队列无法退出
	@InputParam:None
	@OutPutParam:None
	@ReturnValue:None
	**************************************************************************/
	void StopQueue(void)
	{
		{
			boost::lock_guard<boost::mutex> Lock(m_syncMutex);
			m_bNeedExit = true;
		}

		m_PutCondition.notify_all();
		m_GetCondition.notify_all();

	}

private:
	//队列中元素个数
	int m_nMaxElement;
	//添加元素的条件变量
	boost::condition_variable m_PutCondition;
	//获取原色的条件变量
	boost::condition_variable m_GetCondition;
	//用于同步的互斥体
	boost::mutex m_syncMutex;
	//是否结束的标志
	bool m_bNeedExit;
	//存放元素的容器
	std::list<T> m_ElementArray;
};





#endif