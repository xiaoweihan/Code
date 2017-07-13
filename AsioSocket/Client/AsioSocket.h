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

//基于practor模式的套接字
class CAsioSocket
{
public:
	CAsioSocket() = default;
	~CAsioSocket() = default;


};


#endif
