/**************************************************************************
Copyright(C):Envision. Co., Ltd.
FileName:AsioSocketServer.h
Description:基于Asio的服务端模型
Author:xiaowei.han
Date:2017/07/17
Others:
Histroy:
**************************************************************************/
#ifndef ASIO_SOCKET_SERVER_h
#define ASIO_SOCKET_SERVER_h
//不使用STD中的CHRONO
#define BOOST_ASIO_DISABLE_STD_CHRONO
#include <boost/asio.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/address.hpp>
#include <boost/function.hpp>
#include <boost/noncopyable.hpp>
#include <boost/smart_ptr.hpp>
#include "AsioSocketClient.h"

//定义异步接收连接的回调
using pAsyncAcceptCallBack = void(*)(CAsioSocketClient* pClient, bool bResult);

class CAsioSocketServer : boost::noncopyable
{

public:
	CAsioSocketServer(unsigned short usPort);
	~CAsioSocketServer();


public:

	//开始工作
	void StartWork(void);

	//停止工作
	void StopWork(void);

	//设置链接通知
	void SetAcceptCallBack(pAsyncAcceptCallBack pCallBack);
	//同步接收连接
	boost::shared_ptr<CAsioSocketClient> SyncAccept(void);
	//异步接收连接
	void AsyncAccept(void);
private:
	//IOService
	boost::asio::io_service m_IOService;
	//work 用于循环不退出
	boost::asio::io_service::work m_Work;
	//套接字
	boost::asio::ip::tcp::acceptor m_Acceptor;
	//回调接口
	boost::function<void(CAsioSocketClient* pClient, bool bResult)> m_AcceptNotifier;
};
#endif

