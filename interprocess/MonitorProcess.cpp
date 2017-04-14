#include "stdafx.h"
#include "MonitorProcess.h"
#include <boost/foreach.hpp>
#include "AbstractProcessMonitor.h"
CMonitorProcess& CMonitorProcess::CreateInstance(void)
{
	return s_MonitorProcessObj;
}

void CMonitorProcess::RegisterProcessMonitor(boost::shared_ptr<CAbstractProcessMonitor> pMonitor)
{
	if (pMonitor)
	{
		m_MonitorArray.push_back(pMonitor);
	}
}

void CMonitorProcess::StartMonitorProcess(void)
{
	BOOST_FOREACH(auto& Element, m_MonitorArray)
	{
		if (Element)
		{
			Element->StartMonitor();
		}
	}
}

void CMonitorProcess::StopMonitorProcess(void)
{
	BOOST_FOREACH(auto& Element, m_MonitorArray)
	{
		if (Element)
		{
			Element->StopMonitor();
		}
	}
}

CMonitorProcess CMonitorProcess::s_MonitorProcessObj;
