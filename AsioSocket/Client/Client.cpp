// Client.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include <cstring>
#include <boost/thread.hpp>
#include "AsioSocket.h"

void Display(bool bResult)
{
	using namespace std;
	if (bResult)
	{
		cout << "Succeed!" << endl;
	}
	else
	{
		cout << "Failed!" << endl;
	}
}

void SendResult(unsigned int nDataLength)
{

	using namespace std;

	cout << "send succeed,the length = " << nDataLength << endl;

}
int main()
{

	char szData[] = "hello,Asio,It is very good.";
	using namespace std;
	using namespace boost;
	CAsioSocketClient Client;

	Client.SetConnectCallBack(Display);
	Client.SetSendedCallBack(SendResult);
	Client.AsyncConnect("172.24.32.240", 10024);



	auto ThreadHandler = [&]()
	{
		Client.StartWork();
	};
	//开启启动线程
	boost::thread T(ThreadHandler);


	cout << "请根据以下提示进行操作!" << endl;

	cout << "send:发送数据" << endl;
	cout << "quit:退出程序" << endl;

	while (true)
	{
		string strOperation;

		cin >> strOperation;

		if (strOperation == "send")
		{
			string strContent;
			cout << "请输入需要发送的内容:" << endl;
			cin >> strContent;

			Client.AsyncSendData(strContent.c_str(), strContent.length());
		}
		else if (strOperation == "quit")
		{
			Client.StopWork();
			T.join();
			break;
		}
		else
		{

		}

	}
    return 0;
}

