#ifndef ABSTRACT_KEEP_ALIVE_STRATEGY_H
#define ABSTRACT_KEEP_ALIVE_STRATEGY_H
#include <boost/thread.hpp>
#include <boost/smart_ptr.hpp>
class CAbstractKeepAliveStrategy
{
public:
	CAbstractKeepAliveStrategy();
	virtual ~CAbstractKeepAliveStrategy();


public:
	//设置心跳间隔
	void SetKeepAliveGap(int nGap) { m_nKeepAliveGap = nGap; }
	
	int GetKeepAliveGap(void) const { return m_nKeepAliveGap; }

	//开始保活
	void StartKeepAlive(void);
	
	//停止保活
	void StopKeepAlive(void);

	//实现KeepAlive的虚函数
	virtual void KeepAlive(void) = 0;
private:
	//发送心跳的时间(单位:s)
	int m_nKeepAliveGap;
	//线程对象
	boost::shared_ptr<boost::thread> m_KeepAliveThread;
};





#endif