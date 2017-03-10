#include "StdAfx.h"
#include "ConcreteZip.h"
#include "Global.h"
using std::cout;
using std::endl;

CConcreteZip::CConcreteZip(void)
{
}


CConcreteZip::~CConcreteZip(void)
{
}

void CConcreteZip::Zip()
{
	boost::mutex::scoped_lock IoLock(g_IOMutex);

	cout<<"the client call Zip function!"<<endl;

}
