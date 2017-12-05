#ifndef __AUTHENTICATE_H__
#define __AUTHENTICATE_H__
#include <boost/atomic.hpp>

class CAuthentication
{

private:
	CAuthentication():m_nTotalCallTimes(100){}
	~CAuthentication() = default;

public:
	static CAuthentication& CreateInstance();

	//进行验证
	void AuthCall(void);

	//读取值
	void SaveCallTimes(void);

	//保存值
	void LoadCallTimes(void);

	void On()
	{
		m_bSwitch = true;
	}
	void Off()
	{
		m_bSwitch = false;
	}
private:
	boost::atomic_int m_nTotalCallTimes;
	bool m_bSwitch = true;
private:
	static CAuthentication s_obj;

};


#endif
