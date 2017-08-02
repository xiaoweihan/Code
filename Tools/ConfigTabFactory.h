#pragma once
#include <string>
#include "DlgCollectorConfig.h"
#include "DlgNodeConfig.h"
#include "DlgReceiverConfig.h"
#include "DlgScheduerConfig.h"
#include "DlgStatusManagerConfig.h"

class CBaseDialog;

CBaseDialog* CreateConfigTabWnd(const std::string& strConfigFileName);

CBaseDialog* CreateConfigTabWnd(const std::string& strConfigFileName)
{
	CBaseDialog* pTabWnd = nullptr;

	if (strConfigFileName == "CollectorConfig.json")
	{
		pTabWnd = new CDlgCollectorConfig;
	}
	else if (strConfigFileName == "NodeServiceConfig.json")
	{
		pTabWnd = new CDlgNodeConfig;
	}
	else if (strConfigFileName == "TaskReceiverConfig.json")
	{
		pTabWnd = new CDlgReceiverConfig;
	}
	else if (strConfigFileName == "TaskScheduerConfig.json")
	{
		pTabWnd = new CDlgScheduerConfig;
	}
	else if (strConfigFileName == "TaskStatusManagerConfig.json")
	{
		pTabWnd = new CDlgStatusManagerConfig;
	}
	else
	{

	}



	return pTabWnd;
}






