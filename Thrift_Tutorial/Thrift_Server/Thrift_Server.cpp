// Thrift_Server.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include <string>
#include "ConcreteService.h"
#include "ConcreteZip.h"
#include <thrift/concurrency/ThreadManager.h>
#include <thrift/concurrency/PlatformThreadFactory.h>
#include <thrift/concurrency/PosixThreadFactory.h>
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TThreadPoolServer.h>
#include <thrift/processor/TMultiplexedProcessor.h>
#include <thrift/server/TSimpleServer.h>
#include <thrift/server/TThreadedServer.h>
#include <thrift/server/TNonblockingServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TBufferTransports.h>
#include <thrift/windows/TWinsockSingleton.h>
#include <boost/smart_ptr.hpp>
#include <boost/thread.hpp>
#include <Windows.h>
#include "Log.h"
using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
using namespace ::apache::thrift::server;
using namespace ::apache::thrift::concurrency;
using boost::shared_ptr;
using namespace  ::ThriftModule;
using std::endl;
using std::cout;
#pragma comment(lib,"libeay32.lib")
#pragma comment(lib,"ssleay32.lib")
#pragma comment(lib,"libevent.lib")
#pragma comment(lib,"libevent_core.lib")
#pragma comment(lib,"libevent_extras.lib")
#ifdef _DEBUG
#pragma comment(lib,"libthriftd.lib")
#pragma comment(lib,"libthriftnbd.lib")
#else
#pragma comment(lib,"libthrift.lib")
#pragma comment(lib,"libthriftnb.lib")
#endif



//简单的服务器模型，串行服务
static void SimpleServerModel(int nPort)
{
	shared_ptr<CConcreteService> handler(new CConcreteService());
	shared_ptr<TProcessor> processor(new QueryServiceProcessor(handler));
	shared_ptr<TServerTransport> serverTransport(new TServerSocket(nPort));
	shared_ptr<TTransportFactory> transportFactory(new TBufferedTransportFactory());
	shared_ptr<TProtocolFactory> protocolFactory(new TBinaryProtocolFactory());

	TSimpleServer Server(processor, serverTransport, transportFactory, protocolFactory);
	Server.serve();
}

//一个连接一个线程模型
static void TThreadedServerModel(int nPort)
{

	shared_ptr<CConcreteService> handler(new CConcreteService());
	shared_ptr<TProcessor> processor(new QueryServiceProcessor(handler));
	shared_ptr<TServerTransport> serverTransport(new TServerSocket(nPort));
	shared_ptr<TTransportFactory> transportFactory(new TBufferedTransportFactory());
	shared_ptr<TProtocolFactory> protocolFactory(new TBinaryProtocolFactory());

	TThreadedServer Server(processor,serverTransport,transportFactory,protocolFactory);

	Server.serve();
}

//线程池模型
static void ThreadServerPoolModel(int nPort)
{
	//获取CPU的可并发数量
	int nCPUNums = boost::thread::hardware_concurrency();

	boost::shared_ptr<ThreadManager> threadManager = ThreadManager::newSimpleThreadManager(nCPUNums);
	threadManager->threadFactory(boost::make_shared<PlatformThreadFactory>());
	threadManager->start();

	shared_ptr<CConcreteService> handler(new CConcreteService());
	shared_ptr<TProcessor> processor(new QueryServiceProcessor(handler));
	shared_ptr<TServerTransport> serverTransport(new TServerSocket(nPort));
	shared_ptr<TTransportFactory> transportFactory(new TBufferedTransportFactory());
	shared_ptr<TProtocolFactory> protocolFactory(new TBinaryProtocolFactory());

	TThreadPoolServer Server(processor,serverTransport,transportFactory,protocolFactory,threadManager);

	Server.serve();
}

//非阻塞模型
static void NoBlockServerModel(int nPort)
{
	shared_ptr<CConcreteService> handler(new CConcreteService());
	shared_ptr<TProcessor> processor(new QueryServiceProcessor(handler));
	shared_ptr<TProtocolFactory> protocolFactory(new TBinaryProtocolFactory());

	//获取CPU的可并发数量
	int nCPUNums = boost::thread::hardware_concurrency();
	boost::shared_ptr<ThreadManager> threadManager = ThreadManager::newSimpleThreadManager(nCPUNums);
	threadManager->threadFactory(boost::make_shared<PlatformThreadFactory>());
	threadManager->start();
#if 0
	shared_ptr<TServerTransport> serverTransport(new TServerSocket(nPort));
	shared_ptr<TTransportFactory> transportFactory(new TBufferedTransportFactory());
	
#endif

	try
	{
		TNonblockingServer server(processor,protocolFactory,nPort,threadManager);
		server.serve();
	}
	catch (TException& e)
	{
		cout<<e.what()<<endl;
	}

}

//多服务模型
static void MultiServiceServerModel(int nPort)
{

	shared_ptr<CConcreteService> handler1(new CConcreteService());
	shared_ptr<TProcessor> processor1(new QueryServiceProcessor(handler1));

	shared_ptr<CConcreteZip> handler2(new CConcreteZip());
	shared_ptr<TProcessor> processor2(new ZipServiceProcessor(handler2));

	shared_ptr<TMultiplexedProcessor> MixProcessor(new TMultiplexedProcessor());

	if (MixProcessor)
	{
		MixProcessor->registerProcessor("QueryService",processor1);
		MixProcessor->registerProcessor("ZipService",processor2);
	}

	shared_ptr<TServerTransport> serverTransport(new TServerSocket(nPort));
	shared_ptr<TTransportFactory> transportFactory(new TBufferedTransportFactory());
	shared_ptr<TProtocolFactory> protocolFactory(new TBinaryProtocolFactory());

	TSimpleServer Server(MixProcessor, serverTransport, transportFactory, protocolFactory);
	Server.serve();

}

std::string GetExeDirecory( void )
{
	std::string strPath;
	char szPath[MAX_PATH] = {0};

	if (0 != GetModuleFileName(NULL,szPath,sizeof(szPath)))
	{
		strPath = szPath;
		int nPos = (int)strPath.rfind('\\');
		if (std::string::npos != nPos)
		{
			strPath = strPath.substr(0,nPos);
		}
	}

	return strPath;
}

int _tmain(int argc, _TCHAR* argv[])
{

	//初始化打印日志
	std::string strLogDir = GetExeDirecory();
	strLogDir += std::string("\\");
	CLog::CreateInstance().SetLogPath(strLogDir.c_str());
	CLog::CreateInstance().SetLogNamePrefix("Thrift_Server");
	TWinsockSingleton::create();
	int nPort = 10067;
	//ThreadServerPoolModel(nPort);
	//SimpleServerModel(nPort);
	NoBlockServerModel(nPort);
	return 0;
}

