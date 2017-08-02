#pragma once
#include <string>
#include "ConfigType.h"
class CConfigManager
{

public:
	static CConfigManager& CreateInstance(void);
private:
	CConfigManager();
	~CConfigManager();

public:
	//加载搜集节点资源配置
	bool ReadCollectorConfigInfo(const std::string& strConfigPath, ConfigType::LP_COLLECTOR_CONFIG_INFO pConfigInfo);
	//保存搜集节点资源配置
	bool WriteCollectorConfigInfo(const std::string& strConfigPath, const ConfigType::LP_COLLECTOR_CONFIG_INFO pConfigInfo);

	//加载节点资源配置
	bool ReadNodeConfigInfo(const std::string& strConfigPath, ConfigType::LP_NODE_CONFIG_INFO pConfigInfo);
	//保存节点资源配置
	bool WriteNodeConfigInfo(const std::string& strConfigPath, const ConfigType::LP_NODE_CONFIG_INFO pConfigInfo);

	//加载状态管理资源配置
	bool ReadStatusManagerConfigInfo(const std::string& strConfigPath, ConfigType::LP_STATUS_MANAGER_CONFIG_INFO pConfigInfo);
	//保存状态管理资源配置
	bool WriteStatusManagerConfigInfo(const std::string& strConfigPath, const ConfigType::LP_STATUS_MANAGER_CONFIG_INFO pConfigInfo);

	//加载调度资源配置
	bool ReadScheduerConfigInfo(const std::string& strConfigPath, ConfigType::LP_SCHEDUER_CONFIG_INFO pConfigInfo);
	//保存调度资源配置
	bool WriteScheduerConfigInfo(const std::string& strConfigPath, const ConfigType::LP_SCHEDUER_CONFIG_INFO pConfigInfo);

	//加载接收任务资源配置
	bool ReadReceiverConfigInfo(const std::string& strConfigPath, ConfigType::LP_RECEIVER_CONFIG_INFO pConfigInfo);
	//保存接收任务资源配置
	bool WriteReceiverConfigInfo(const std::string& strConfigPath, const ConfigType::LP_RECEIVER_CONFIG_INFO pConfigInfo);
private:
	static CConfigManager s_Obj;
};

