/**************************************************************************
Copyright(C):
FileName:AsioSocketClient.h
Description:基于Asio机制的Socket
Author:xiaowei.han
Date:2017/07/13
Others:
Histroy:
**************************************************************************/
#ifndef ASIO_SOCKET_CLIENT_H
#define ASIO_SOCKET_CLIENT_H
//不使用STD中的CHRONO
#define BOOST_ASIO_DISABLE_STD_CHRONO
#include <boost/asio.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/address.hpp>
#include <boost/function.hpp>
#include <boost/noncopyable.hpp>
#include <string>
//连接完成的回调
using pAsyncConnectCallBack = void(*)(bool bResult);
//发送完成的回调
using pAsyncSendedCallBack = void(*)(unsigned int nDataLength);
//接收完成的回调
using pAsyncReceivedCallBack = void(*)(char* pData,unsigned int nDataLength);
//基于practor模式的套接字
class CAsioSocketClient : boost::noncopyable
{
public:
	CAsioSocketClient();
	explicit CAsioSocketClient(boost::asio::io_service& IOService);
	~CAsioSocketClient();
public:
	void StartWork();
	void StopWork();
	//获取内部套接字
	boost::asio::ip::tcp::socket& GetSocket(void);
	//同步连接
	bool SyncConnect(const std::string strAddress, unsigned short usPort);
	bool SyncConnect(const char* szAddress, unsigned short usPort);

	//异步连接
	void AsyncConnect(const std::string strAddress, unsigned short usPort);
	void AsyncConnect(const char* szAddress, unsigned short usPort);

	//异步发送数据
	void AsyncSendData(const char* pData, unsigned int nDataLength);

	//同步发送数据
	bool SyncSendData(const char* pData, unsigned int nDataLength,unsigned int& nSendLength);

	//异步接收数据
	void AsyncReceiveData(char* pData, unsigned int nDataLength);

	//同步接收数据
	bool SyncRecvData(char* pBuffer, unsigned int nBufferLength,unsigned int& nRecvLength);

	//设置回调
	void SetConnectCallBack(pAsyncConnectCallBack pFunction)
	{
		m_ConnectNotifier = pFunction;
	}

	void SetSendedCallBack(pAsyncSendedCallBack pFunction)
	{
		m_SendedNotifier = pFunction;
	}

	void SetReceivedCallBack(pAsyncReceivedCallBack pFunction)
	{
		m_ReceivedNotifier = pFunction;
	}
	//设置缓冲区长度
	void SetBufferLength(unsigned int nLength)
	{
		m_nRecvBufferLength = nLength;
	}

private:
	//IOService
	boost::asio::io_service m_IOService;
	//work 用于循环不退出
	boost::asio::io_service::work m_Work;
	//套接字
	boost::asio::ip::tcp::socket m_AsioSocket;
	//用于连接完成的回调
	boost::function<void(bool)> m_ConnectNotifier;
	//用于发送完成的回调
	boost::function<void(unsigned int nDataLength)> m_SendedNotifier;
	//用于接收完成的回调
	boost::function<void(char* pData, unsigned int nDataLength)> m_ReceivedNotifier;
	//接收缓冲区的大小
	unsigned int m_nRecvBufferLength = 1024;
	//接收缓冲区
	char* m_pRecvBuffer = nullptr;
};


#endif
