/**************************************************************************
Copyright(C):
FileName:AsioSocket.h
Description:基于Asio机制的Socket
Author:xiaowei.han
Date:2017/07/13
Others:
Histroy:
**************************************************************************/
#ifndef ASIO_SOCKET_H
#define ASIO_SOCKET_H
//不使用STD中的CHRONO
#define BOOST_ASIO_DISABLE_STD_CHRONO
#include <boost/asio.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/address.hpp>
#include <string>
//基于practor模式的套接字
class CAsioSocketClient
{
public:
	CAsioSocketClient();
	~CAsioSocketClient() = default;

public:
	//同步连接
	bool AsyncConnect(const std::string strAddress, unsigned short usPort);
	bool AsyncConnect(const char* szAddress, unsigned short usPort);
private:
	//IOService
	boost::asio::io_service m_IOService;
	//work 用于循环不退出
	boost::asio::io_service::work m_Work;
	//套接字
	boost::asio::ip::tcp::socket m_AsioSocket;
};


#endif
