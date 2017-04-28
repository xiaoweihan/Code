/**********************************************************
*版权所有:xiaowei.han
*文件名称:ConditionNotifier.hpp
*功能描述:基于条件变量的线程通知类
*作者:xiaowei.han
*日期:2017/04/27
**********************************************************/
#ifndef CONDITION_NOTIFIER_HPP
#define CONDITION_NOTIFIER_HPP
#include <boost/thread/condition_variable.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/lock_types.hpp>
class ConditionNotifier
{

public:
	ConditionNotifier() = default;
	~ConditionNotifier() = default;

public:
	void Wait(void)
	{
		auto Pred = [this]()->bool
		{
			return !m_bWait;
		};
		
		boost::unique_lock<boost::mutex> Lock(m_Mutex);
		m_CV.wait(Lock, Pred);
	}

	void Notify(void)
	{
		{
		  boost::unique_lock<boost::mutex> Lock(m_Mutex);
		  m_bWait = false;
		}
		m_CV.notify_one();
	}
private:
	boost::mutex m_Mutex;
	//用于线程通知的条件变量
	boost::condition_variable m_CV;
	//是否需要等待标志
	bool m_bWait = true;

};

#endif