#ifndef ABSTRACT_PROCESS_MONITOR_H
#define ABSTRACT_PROCESS_MONITOR_H
#include <string>
#include <boost/smart_ptr.hpp>
#include <boost/thread.hpp>
class CAbstractProcessMonitor
{

public:
	void SetMonitorProcessName(const std::string& strMonitorProcessName) { m_strMonitorProcessName = strMonitorProcessName; }
	std::string GetMonitorProcessName(void) const { return m_strMonitorProcessName; }
	void StartMonitor(void);
	void StopMonitor(void);

	virtual void MonitorProcess(void) = 0;
protected:
	CAbstractProcessMonitor(void) {}
	virtual ~CAbstractProcessMonitor(void) {}
protected:
	//监控进程的名称
	std::string m_strMonitorProcessName;
	//监控线程
	boost::shared_ptr<boost::thread> m_MonitorThread;
};





#endif