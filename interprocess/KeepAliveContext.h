#ifndef KEEP_ALIVE_CONTEXT_H
#define KEEP_ALIVE_CONTEXT_H
#include <string>
class CAbstractKeepAliveStrategy;
class CKeepAliveContext
{
public:
	CKeepAliveContext(void) :m_pKeepAliveStrategy(nullptr) {}
	CKeepAliveContext(CAbstractKeepAliveStrategy* pKeepAliveStrategy) :m_pKeepAliveStrategy(pKeepAliveStrategy) {}
	virtual ~CKeepAliveContext(void) {}

	void SetKeepAliveStrategy(CAbstractKeepAliveStrategy* pKeepAliveStrategy)
	{
		m_pKeepAliveStrategy = pKeepAliveStrategy;
	}
	CAbstractKeepAliveStrategy* GetKeepAliveStrategy(void) const
	{
		return m_pKeepAliveStrategy;
	}

	void StartKeepAlive(int nKeepAliveGap);
	void StopKeepAlive(void);
private:
	CAbstractKeepAliveStrategy* m_pKeepAliveStrategy;
};

#endif