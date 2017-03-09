// Thrift_Client.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include "QueryService.h"
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TSimpleServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TBufferTransports.h>
#include <thrift/transport/TSocket.h>
#include <thrift/windows/TWinsockSingleton.h>
#include <string>
#include <boost/thread.hpp>
#include <Windows.h>
#include "Log.h"
using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
using namespace ::apache::thrift::server;

using boost::shared_ptr;

using namespace  ::ThriftModule;
using std::endl;
using std::cout;
#pragma comment(lib,"libeay32.lib")
#pragma comment(lib,"ssleay32.lib")
#ifdef _DEBUG
#pragma comment(lib,"libthriftd.lib")
#pragma comment(lib,"libthriftnbd.lib")
#else
#pragma comment(lib,"libthrift.lib")
#pragma comment(lib,"libthriftnb.lib")
#endif

static std::string GetExeDirecory(void);

static void BlockTest(int nPort);
static void NoBlockTest(int nPort);
int _tmain(int argc, _TCHAR* argv[])
{

	int nPort = 10067;
	//初始化打印日志
	std::string strLogDir = GetExeDirecory();
	strLogDir += std::string("\\");
	CLog::CreateInstance().SetLogPath(strLogDir.c_str());
	CLog::CreateInstance().SetLogNamePrefix("Thrift_Client");

	TWinsockSingleton::create();

	NoBlockTest(nPort);

	return 0;
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

void BlockTest( int nPort )
{
	boost::shared_ptr<TTransport> socket(new TSocket("127.0.0.1", 10067));
	boost::shared_ptr<TTransport> transport(new TBufferedTransport(socket));
	boost::shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));
	QueryServiceClient Client(protocol);

	try
	{
		transport->open();

		while (true)
		{
			std::string strName;
			Client.QueryNameByID(strName,12);
			cout<<"the Query Name is "<<strName<<endl;
			NECESSARY_LOG("the query name is [%s].",strName.c_str());
			boost::this_thread::sleep(boost::posix_time::seconds(1));
		}

		transport->close();
	}
	catch (TException& tx)
	{
		cout<<"catch a execption,the exception is "<<tx.what()<<endl;
	}
}

void NoBlockTest( int nPort )
{
	boost::shared_ptr<TTransport> socket(new TSocket("127.0.0.1", 10067));
	boost::shared_ptr<TTransport> transport(new TFramedTransport(socket));
	boost::shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));
	QueryServiceClient Client(protocol);

	try
	{
		transport->open();

		while (true)
		{
			std::string strName;
			Client.QueryNameByID(strName,12);
			cout<<"the Query Name is "<<strName<<endl;
			NECESSARY_LOG("the query name is [%s].",strName.c_str());
			boost::this_thread::sleep(boost::posix_time::seconds(1));
		}

		transport->close();
	}
	catch (TException& tx)
	{
		cout<<"catch a execption,the exception is "<<tx.what()<<endl;
	}
}

