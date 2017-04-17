//基于Asio前摄器模式的定时器
#ifndef ASIO_TIMER_HPP
#define ASIO_TIMER_HPP

#define BOOST_ASIO_DISABLE_STD_CHRONO
#include <boost/asio/steady_timer.hpp>
#include <boost/asio.hpp>
#include <boost/function.hpp>
#include <type_traits>
#include <boost/static_assert.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/bind.hpp>
template <typename T>
class AsioTimer
{
public:
	AsioTimer(T CallBack) :m_pCallBack(CallBack),
	m_Timer(m_IOService)
	{
	}
	AsioTimer() = default;
	~AsioTimer() = default;


public:
	void StartAsioTimer()
	{
		m_Timer.expires_from_now(boost::chrono::seconds(m_nGap));
		m_Timer.async_wait(boost::bind(&AsioTimer::TimeProc, this, boost::asio::placeholders::error));
		m_IOService.run();

	}

	void StopAsioTimer()
	{
		m_Timer.cancel();

		m_IOService.stop();
	}

	void SetTimeGap(int nTimeGap)
	{
		m_nGap = nTimeGap;
	}

	int GetTimeGap(void) const
	{
		return m_nGap;
	}

private:
	void TimeProc(const boost::system::error_code& e)
	{
		if (boost::system::errc::success == e.value())
		{
			if (m_pCallBack)
			{
				m_pCallBack();
			}

			m_Timer.expires_from_now(boost::chrono::seconds(m_nGap));
			m_Timer.async_wait(boost::bind(&AsioTimer::TimeProc,this,boost::asio::placeholders::error));
		}
	}
private:
	//IO_Service
	boost::asio::io_service m_IOService;
	//用于执行定时任务的回调函数
	boost::function<T> m_pCallBack;
	//定时间隔(单位:S)
	int m_nGap = 1;
	//定时器
	boost::asio::steady_timer m_Timer;

};





#endif
