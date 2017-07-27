/**************************************************************************
Copyright(C):Envision. Co., Ltd.
FileName:MysqlConnector.h
Description:操作Mysql类
Author:xiaowei.han
Date:2017/05/27
Others:
Histroy:
**************************************************************************/
#pragma once
#include <string>
#include <boost/thread/mutex.hpp>
#include <mysql.h>
#include <string>
#include <vector>
typedef struct _connector_element
{
	//用户名
	std::string strUserName;
	//密码
	std::string strPassword;
	//IP地址
	std::string strIPAddress;
	//端口号
	int nPort;
	//数据库实例名称
	std::string strMysqlInstance;

	_connector_element()
	{
		nPort = 3306;
	}

}CONNECTOR_ELEMENT,* LP_CONNECTOR_ELEMENT;

class CMysqlConnector
{

public:
	static CMysqlConnector& CreateInstance(void);

	//设置链接因子
	void SetConnectorElement(const CONNECTOR_ELEMENT& Element);

	//链接数据库
	bool ConnectDB(void);

	//断开数据库链接
	bool DisconnectDB(void);

	//实现断开重连功能
	void PingDB(void);

	//执行非查询的SQL语句
	bool RunNoQuerySQL(const std::string& strSQL);

	//执行查询的SQL语句
	bool RunQuerySQL(const std::string& strSQL,std::vector<std::string>& ResultArray);
protected:
	CMysqlConnector();
	virtual ~CMysqlConnector();
private:
	//链接数据库的因子
	CONNECTOR_ELEMENT m_ConnectorElement;

	//链接mysql的对象
	MYSQL* m_pConnectPtr;

	//操作数据库的锁
	boost::mutex m_Lock;

	//单实例对象
	static CMysqlConnector s_obj;
};

