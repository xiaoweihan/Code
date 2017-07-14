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
	CAsioSocketClient Client;

	Client.SetConnectCallBack(Display);
	Client.SetSendedCallBack(SendResult);
	Client.AsyncConnect("172.24.32.240", 10024);



	Client.StartWork();
	
    return 0;
}

