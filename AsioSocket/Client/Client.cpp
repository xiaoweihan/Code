// Client.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ip/address.hpp>
#include <iostream>
int main()
{

	using namespace std;
	using namespace boost::asio;

	boost::asio::ip::address NetAddress = boost::asio::ip::address::from_string("224.1.1.223");


	if (NetAddress.is_loopback())
	{
		cout << "It is a loopback address." << endl;
	}

	if (NetAddress.is_multicast())
	{
		cout << "It is a multicase address." << endl;
	}


    return 0;
}

