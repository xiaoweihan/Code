/**************************************************************************
Copyright(C):Envision. Co., Ltd.
FileName:AsioSocketServer.cpp
Description:基于Asio的服务端模型
Author:xiaowei.han
Date:2017/07/17
Others:
Histroy:
**************************************************************************/
#include "stdafx.h"
#include "AsioSocketServer.h"
#include <iostream>
CAsioSocketServer::CAsioSocketServer(unsigned short usPort):
m_Work(m_IOService),
m_Acceptor(m_IOService,boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(),usPort),true)
{

}

CAsioSocketServer::~CAsioSocketServer()
{
}

boost::shared_ptr<CAsioSocketClient> CAsioSocketServer::SyncAccept(void)
{

	using namespace boost::asio::ip;
	boost::system::error_code ec;
	boost::shared_ptr<CAsioSocketClient> pNewSocket(new CAsioSocketClient(m_IOService));

	//申请内存失败
	if (!pNewSocket)
	{
		return nullptr;
	}

	m_Acceptor.accept(pNewSocket->GetSocket(), ec);

	//接收失败
	if (ec)
	{
		return nullptr;
	}

	return pNewSocket;
}

void CAsioSocketServer::SetAcceptCallBack(pAsyncAcceptCallBack pCallBack)
{
	m_AcceptNotifier = pCallBack;
}

void CAsioSocketServer::AsyncAccept(void)
{
	using namespace boost::asio::ip;
	boost::system::error_code ec;
	boost::shared_ptr<CAsioSocketClient> pNewSocket(new CAsioSocketClient(m_IOService));
	
	//申请内存失败
	if (!pNewSocket)
	{
		return;
	}
	
	auto AcceptFun = [pNewSocket,this](const boost::system::error_code& ec)
	{
		if (m_AcceptNotifier)
		{
			if (ec)
			{
				m_AcceptNotifier(pNewSocket.get(), false);
			}
			else
			{
				m_AcceptNotifier(pNewSocket.get(), true);
			}
		}

		AsyncAccept();
	};

	m_Acceptor.async_accept(pNewSocket->GetSocket(), AcceptFun);

}

void CAsioSocketServer::StartWork(void)
{
	boost::system::error_code e;
	m_IOService.run(e);
}

void CAsioSocketServer::StopWork(void)
{
	m_IOService.stop();
}
