#ifndef INTER_PROCESS_STYLE_PROCESS_MONITOR_H
#define INTER_PROCESS_STYLE_PROCESS_MONITOR_H
#include <string>
#include "AbstractProcessMonitor.h"
class CInterProcessStyleProcessMonitor : public CAbstractProcessMonitor
{
public:
	CInterProcessStyleProcessMonitor(const std::string& strSharedMemoryName, const std::string& strNamedMutexName,int nGap);
	~CInterProcessStyleProcessMonitor(void);



public:
	void MonitorProcess(void) override;
private:
	//用于进程通信的共享内存名称
	std::string m_strSharedMemoryName;
	//用于进程同步的命名互斥体名称
	std::string m_strNamedMutexName;
	//监测周期
	int m_nMonitorGap;
};

#endif