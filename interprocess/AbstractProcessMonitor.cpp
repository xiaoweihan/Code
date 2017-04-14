#include "stdafx.h"
#include "AbstractProcessMonitor.h"
#include <boost/core/ignore_unused.hpp>
void CAbstractProcessMonitor::StartMonitor(void)
{
	if (m_strMonitorProcessName.empty())
	{
		return;
	}

	if (m_MonitorThread)
	{
		return;
	}

	auto pThreadFunc = [this]()->void
	{
		MonitorProcess();	
	};

	m_MonitorThread = boost::make_shared<boost::thread>(pThreadFunc);
}

void CAbstractProcessMonitor::StopMonitor(void)
{
	if (!m_MonitorThread)
	{
		return;
	}

	m_MonitorThread->interrupt();
	m_MonitorThread->join();

}
