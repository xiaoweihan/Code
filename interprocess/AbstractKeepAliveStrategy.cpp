#include "stdafx.h"
#include "AbstractKeepAliveStrategy.h"
#include <boost/core/ignore_unused.hpp>
//默认的保活时间间隔
const int DEFAULT_KEEP_ALIVE_GAP = 5;


CAbstractKeepAliveStrategy::CAbstractKeepAliveStrategy():
m_nKeepAliveGap(DEFAULT_KEEP_ALIVE_GAP)
{

}

CAbstractKeepAliveStrategy::~CAbstractKeepAliveStrategy()
{

}

void CAbstractKeepAliveStrategy::StartKeepAlive(void)
{
	//是否开启线程
	if (m_KeepAliveThread)
	{
		return;
	}

	//lamda表达式
	auto pThreadFunc = [this]()->void
	{
		try
		{
			while (true)
			{
				//允许线程中断
				boost::this_thread::interruption_point();

				KeepAlive();

				boost::this_thread::sleep(boost::posix_time::seconds(m_nKeepAliveGap));
			}
		}
		catch (boost::thread_interrupted& e)
		{
			boost::ignore_unused(e);
		}
	};
	m_KeepAliveThread = boost::make_shared<boost::thread>(pThreadFunc);
}

void CAbstractKeepAliveStrategy::StopKeepAlive(void)
{
	if (m_KeepAliveThread)
	{
		m_KeepAliveThread->interrupt();
		m_KeepAliveThread->join();
	}
}
