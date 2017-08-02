#pragma once
#include <string>
namespace ConfigType
{
	//搜集节点资源进程配置定义
	struct _collector_config_info
	{
		//端口号 
		int nPort;
		//上报服务器IP地址
		std::string strReportAddress;
		//上报服务器端口号
		int nReportPort;

		_collector_config_info()
		{
			nPort = 0;
			strReportAddress = "127.0.0.1";
			nReportPort = 0;
		}
	};

	using COLLECTOR_CONFIG_INFO = _collector_config_info;
	using LP_COLLECTOR_CONFIG_INFO = _collector_config_info*;

	//状态管理模块配置定义
	struct _status_manager_config_info
	{
		//端口号
		int nPort;
		//数据库服务器IP地址(MYSQL)
		std::string strDBAddress;
		//数据库服务器端口号
		int nDBPort;
		//数据库实例(MYSQL)
		std::string strDBInstanceName;
		//数据库用户名
		std::string strUserName;
		//数据库密码
		std::string strPassword;

		_status_manager_config_info()
		{
			nPort = 0;
			strDBAddress = "127.0.0.1";
			nDBPort = 3306;
			strDBInstanceName = "";
			strUserName = "";
			strPassword = "";
		}
	};
	using STATUS_MANAGER_CONFIG_INFO = _status_manager_config_info;
	using LP_STATUS_MANAGER_CONFIG_INFO = _status_manager_config_info*;

	//调度模块配置定义
	struct _scheduer_config_info
	{
		//端口号 
		int nPort;
		//上报服务器的IP地址
		std::string strReportAddress;
		//上报服务器的端口号
		int nReportPort;

		_scheduer_config_info()
		{
			nPort = 0;
			strReportAddress = "127.0.0.1";
			nReportPort = 0;
		}
	};
	using SCHEDUER_CONFIG_INFO = _scheduer_config_info;
	using LP_SCHEDUER_CONFIG_INFO = _scheduer_config_info*;

	//接收任务模块配置定义
	struct _receiver_config_info
	{
		//端口
		int nPort;
		//上报服务器的IP地址
		std::string strReportAddress;
		//上报服务器的端口号
		int nReportPort;
		//调度服务器的IP地址
		std::string strScheduerAddress;
		//调度服务器的端口号
		int nScheduerPort;

		_receiver_config_info()
		{
			nPort = 0;
			strReportAddress = "127.0.0.1";
			nReportPort = 0;
			strScheduerAddress = "127.0.0.1";
			nScheduerPort = 0;
		}
	};
	using RECEIVER_CONFIG_INFO = _receiver_config_info;
	using LP_RECEIVER_CONFIG_INFO = _receiver_config_info*;


	//计算节点配置定义
	struct _node_config_info
	{
		//端口号
		int nPort;
		//自身的IP地址
		std::string strIPAddress;
		//上报时间间隔(s)
		int nReportGap;
		//中心服务器的IP地址
		std::string strCenterAddress;
		//中心服务器的端口号
		int nCenterPort;
		//上报服务器的IP地址
		std::string strReportAddress;
		//上报服务器的端口号
		int nReportPort;

		_node_config_info()
		{
			nPort = 0;
			strIPAddress = "127.0.0.1";
			nReportGap = 15;
			strCenterAddress = "127.0.0.1";
			nCenterPort = 0;
			strReportAddress = "127.0.0.1";
			nReportPort = 0;
		}
	};
	using NODE_CONFIG_INFO = _node_config_info;
	using LP_NODE_CONFIG_INFO = _node_config_info*;

}
