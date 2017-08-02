#include "stdafx.h"
#include "TabNameMap.h"
#include <boost/assign.hpp>

CTabNameMap::CTabNameMap()
{
	InitTabMap();
}


CTabNameMap::~CTabNameMap()
{
}

std::string CTabNameMap::GetTabNameByConfigName(const std::string& strConfigName)
{
	using namespace std;
	auto Iter = m_TabMap.find(strConfigName);

	if (Iter != m_TabMap.end())
	{
		return Iter->second;
	}

	return string("");
}

void CTabNameMap::InitTabMap(void)
{
	using namespace boost::assign;

	insert(m_TabMap)("CollectorConfig.json","资源收集模块")("NodeServiceConfig.json","节点模块")("TaskReceiverConfig.json","任务接收模块")("TaskScheduerConfig.json","任务调度模块")("TaskStatusManagerConfig.json","任务状态管理模块");

}
