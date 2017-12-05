#include "stdafx.h"
#include "Authenticate.h"
#include <boost/thread.hpp>
#include <windows.h>
#include <string>
#include <boost/lexical_cast.hpp>
#include <boost/filesystem.hpp>
CAuthentication& CAuthentication::CreateInstance()
{
	return s_obj;
}

void CAuthentication::AuthCall(void)
{

	if (!m_bSwitch)
	{
		return;
	}

	if (m_nTotalCallTimes.load() <= 0)
	{
		m_nTotalCallTimes.store(0);
		boost::this_thread::sleep(boost::posix_time::minutes(1));
	}
	else
	{
		--m_nTotalCallTimes;
	}
	
}

void CAuthentication::SaveCallTimes(void)
{
	char szPath[MAX_PATH] = { 0 };
	GetTempPathA(MAX_PATH, szPath);

	using namespace std;

	string strPath = string(szPath) + string("cf.ini");

	int nRemainingTime = m_nTotalCallTimes.load();

	string strRemainingTime = boost::lexical_cast<string>(nRemainingTime);

	WritePrivateProfileStringA("Calltimes", "RemaingTimes", strRemainingTime.c_str(), strPath.c_str());
}

void CAuthentication::LoadCallTimes(void)
{
	char szPath[MAX_PATH] = { 0 };
	GetTempPathA(MAX_PATH, szPath);

	using namespace std;

	string strPath = string(szPath) + string("cf.ini");

	if (boost::filesystem::exists(strPath))
	{
		auto nTimes = GetPrivateProfileIntA("Calltimes", "RemaingTimes", 2, strPath.c_str());

		m_nTotalCallTimes.store(nTimes);
	}


}

CAuthentication CAuthentication::s_obj;

