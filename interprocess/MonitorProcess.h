#ifndef MONITOR_PROCESS_H
#define MONITOR_PROCESS_H
#include <vector>
#include <boost/smart_ptr.hpp>
class CAbstractProcessMonitor;
class CMonitorProcess
{

public:
	//µ¥ÊµÀý
	static CMonitorProcess& CreateInstance(void);

	//×¢²á¼à¿Ø½ø³Ì
	void RegisterProcessMonitor(boost::shared_ptr<CAbstractProcessMonitor> pMonitor);

	//¿ªÊ¼¼à¿Ø
	void StartMonitorProcess(void);

	//Í£Ö¹¼à¿Ø
	void StopMonitorProcess(void);

protected:
	CMonitorProcess(void) { }
	~CMonitorProcess(void) {}

private:
	static CMonitorProcess s_MonitorProcessObj;

	std::vector<boost::shared_ptr<CAbstractProcessMonitor>> m_MonitorArray;
};


#endif