#include "StdAfx.h"
#include "ConcreteService.h"
#include <iostream>
#include <boost/thread.hpp>
#include "Log.h"
#include "Global.h"
using std::cout;
using std::endl;

CConcreteService::CConcreteService(void)
{
}


CConcreteService::~CConcreteService(void)
{
}

void CConcreteService::QueryNameByID( std::string& _return, const int32_t nID )
{

	NECESSARY_LOG("the query ID is [%d].",nID);
	_return = "xiaowei.han";

	boost::mutex::scoped_lock IoLock(g_IOMutex);

	cout<<"I Receive the Query Request,the nID = "<<nID<<"the threadID = "<<boost::this_thread::get_id()<<endl;

	
}
