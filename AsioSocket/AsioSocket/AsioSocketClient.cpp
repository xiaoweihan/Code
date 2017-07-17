
/**************************************************************************
Copyright(C):
FileName:AsioSocketClient.cpp
Description:基于Asio机制的Socket
Author:xiaowei.han
Date:2017/07/13
Others:
Histroy:
**************************************************************************/
#include "stdafx.h"
#include "AsioSocketClient.h"
#include <boost/core/ignore_unused.hpp>
#include <boost/bind.hpp>
CAsioSocketClient::CAsioSocketClient():
	m_Work(m_IOService),
	m_AsioSocket(m_IOService)
{

	m_pRecvBuffer = new char[m_nRecvBufferLength];
}

/**************************************************************************
@FunctionName:CAsioSocketClient
@FunctionDestription:根据外部的IOService构造Socket
@InputParam:
@OutPutParam:
@ReturnValue:
**************************************************************************/
CAsioSocketClient::CAsioSocketClient(boost::asio::io_service& IOService):
	m_AsioSocket(IOService),
	m_Work(m_IOService)
{
	m_pRecvBuffer = new char[m_nRecvBufferLength];
}

CAsioSocketClient::~CAsioSocketClient()
{
	if (nullptr != m_pRecvBuffer)
	{
		delete[] m_pRecvBuffer;
		m_pRecvBuffer = nullptr;
	}
}

void CAsioSocketClient::StartWork()
{
	boost::system::error_code e;
	m_IOService.run(e);
}

void CAsioSocketClient::StopWork()
{
	m_IOService.stop();
}

/*******************************************************************
*函数名称:SyncConnect
*功能描述:同步连接服务器
*输入参数:
*输出参数:
*返回值:
*作者:xiaowei.han
*日期:2017/07/13 22:51:32
*******************************************************************/
bool CAsioSocketClient::SyncConnect(const std::string strAddress, unsigned short usPort)
{
	if (strAddress.empty())
	{
		return false;
	}

	//构建Remot Endpoint
	auto RemoteAddress = boost::asio::ip::address::from_string(strAddress);
	boost::asio::ip::tcp::endpoint RemotePoint(RemoteAddress,usPort);
	boost::system::error_code ec;
	m_AsioSocket.connect(RemotePoint, ec);

	if (ec)
	{
		return false;
	}

	return true;

}

/*******************************************************************
*函数名称:SyncConnect
*功能描述:同步连接服务器
*输入参数:
*输出参数:
*返回值:
*作者:xiaowei.han
*日期:2017/07/13 22:51:32
*******************************************************************/
bool CAsioSocketClient::SyncConnect(const char* szAddress, unsigned short usPort)
{

	return SyncConnect(std::string(szAddress), usPort);

}

/**************************************************************************
@FunctionName:AsyncConnect
@FunctionDestription:异步连接
@InputParam:
@OutPutParam:
@ReturnValue:
**************************************************************************/
void CAsioSocketClient::AsyncConnect(const std::string strAddress, unsigned short usPort)
{
	if (strAddress.empty())
	{
		return;
	}
	//构建Remot Endpoint
	auto RemoteAddress = boost::asio::ip::address::from_string(strAddress);
	boost::asio::ip::tcp::endpoint RemotePoint(RemoteAddress, usPort);

	auto Handler = [this](const boost::system::error_code& e)
	{
		if (m_ConnectNotifier)
		{
			if (e)
			{
				m_ConnectNotifier(false);
			}
			else
			{
				m_ConnectNotifier(true);
			}
		}
	};
	m_AsioSocket.async_connect(RemotePoint,Handler);
}

/**************************************************************************
@FunctionName:AsyncConnect
@FunctionDestription:异步连接
@InputParam:
@OutPutParam:
@ReturnValue:
**************************************************************************/
void CAsioSocketClient::AsyncConnect(const char* szAddress, unsigned short usPort)
{
	AsyncConnect(std::string(szAddress), usPort);
}

void CAsioSocketClient::AsyncSendData(const char* pData, unsigned int nDataLength)
{
	if (nullptr == pData || 0 == nDataLength)
	{
		return;
	}

	auto Handler = [this](const boost::system::error_code& e, size_t nBytesTransferred)
	{
		if (m_SendedNotifier)
		{

			if (!e)
			{
				m_SendedNotifier((unsigned int)nBytesTransferred);
			}

		}
	};

	m_AsioSocket.async_send(boost::asio::buffer(pData, nDataLength), Handler);
}

/**************************************************************************
@FunctionName:SyncSendData
@FunctionDestription:同步发送数据
@InputParam:
@OutPutParam:
@ReturnValue:
**************************************************************************/
bool CAsioSocketClient::SyncSendData(const char* pData, unsigned int nDataLength, unsigned int& nSendLength)
{
	if (nullptr == pData || 0 == nDataLength)
	{
		return false;
	}

	nSendLength = 0;
	try
	{
		auto nSendSize = m_AsioSocket.send(boost::asio::buffer(pData, nDataLength));
		nSendLength = static_cast<unsigned int>(nSendSize);
	}
	catch (const boost::system::system_error& e)
	{
		boost::ignore_unused(e);
		return false;
	}

	return true;

}

void CAsioSocketClient::AsyncReceiveData(char* pData, unsigned int nDataLength)
{

}

/**************************************************************************
@FunctionName:SyncRecvData
@FunctionDestription:同步接收数据
@InputParam:
@OutPutParam:
@ReturnValue:
**************************************************************************/
bool CAsioSocketClient::SyncRecvData(char* pBuffer, unsigned int nBufferLength, unsigned int& nRecvLength)
{
	if (nullptr == pBuffer || 0 == nBufferLength)
	{
		return false;
	}

	auto nAviableReadByte = m_AsioSocket.available();

	if (nBufferLength < nAviableReadByte)
	{
		return false;
	}
	nRecvLength = 0;
	try
	{
		auto nRecvSize = m_AsioSocket.receive(boost::asio::buffer(pBuffer, nBufferLength));

		nRecvLength = static_cast<decltype(nBufferLength)>(nRecvSize);
	}
	catch (const boost::system::system_error& e)
	{
		boost::ignore_unused(e);
		return false;
	}


	return true;
}

boost::asio::ip::tcp::socket& CAsioSocketClient::GetSocket(void)
{
	return m_AsioSocket;
}

