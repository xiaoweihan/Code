/**************************************************************************
Copyright(C):Envision. Co., Ltd.
FileName:MysqlConnector.h
Description:操作Mysql类
Author:xiaowei.han
Date:2017/05/27
Others:
Histroy:
**************************************************************************/
#include "stdafx.h"
#include "MysqlConnector.h"
#include <boost/thread/lock_guard.hpp>
#include "Log.h"

CMysqlConnector& CMysqlConnector::CreateInstance(void)
{
	return s_obj;
}


void CMysqlConnector::SetConnectorElement(const CONNECTOR_ELEMENT& Element)
{
	m_ConnectorElement = Element;
}


bool CMysqlConnector::ConnectDB(void)
{

	m_pConnectPtr = mysql_init(nullptr);

	if (nullptr == m_pConnectPtr)
	{
		ERROR_LOG("connect [%s] failed,username [%s] password [%s] dbinstance[%s] dbport[%d].", m_ConnectorElement.strIPAddress.c_str(),
			      m_ConnectorElement.strUserName.c_str(),
			      m_ConnectorElement.strPassword.c_str(),
			      m_ConnectorElement.strMysqlInstance.c_str(),
			      m_ConnectorElement.nPort);
		return false;
	}

	my_bool EnableReconnectFlag = 1;
	//启用断开重连功能
	if (mysql_options(m_pConnectPtr, MYSQL_OPT_RECONNECT, &EnableReconnectFlag))
	{
		ERROR_LOG("set auto connect failed,error code [%d].", mysql_errno(m_pConnectPtr));
		mysql_close(m_pConnectPtr);
		m_pConnectPtr = nullptr;
		return false;
	}

	if (nullptr == mysql_real_connect(m_pConnectPtr, m_ConnectorElement.strIPAddress.c_str(), m_ConnectorElement.strUserName.c_str(),
		m_ConnectorElement.strPassword.c_str(), m_ConnectorElement.strMysqlInstance.c_str(),
		m_ConnectorElement.nPort, nullptr, 0))
	{
		ERROR_LOG("connect [%s] failed,username [%s] password [%s] dbinstance[%s] dbport[%d] the error code[%d].", m_ConnectorElement.strIPAddress.c_str(),
			m_ConnectorElement.strUserName.c_str(),
			m_ConnectorElement.strPassword.c_str(),
			m_ConnectorElement.strMysqlInstance.c_str(),
			m_ConnectorElement.nPort,
			mysql_errno(m_pConnectPtr));
		return false;
	}

	//设置自动commit功能
	if (mysql_autocommit(m_pConnectPtr, 1))
	{
		ERROR_LOG("set auto commit failed,error code [%d].",mysql_errno(m_pConnectPtr));
		mysql_close(m_pConnectPtr);
		m_pConnectPtr = nullptr;
		return false;
	}

	return true;
}


bool CMysqlConnector::DisconnectDB(void)
{

	//断开数据库链接
	if (nullptr != m_pConnectPtr)
	{
		mysql_close(m_pConnectPtr);
		m_pConnectPtr = nullptr;
	}


	return true;
}


void CMysqlConnector::PingDB(void)
{
	if (nullptr != m_pConnectPtr)
	{
		if (mysql_ping(m_pConnectPtr))
		{
			ERROR_LOG("mysql_ping failed.");
		}
	}
}


bool CMysqlConnector::RunNoQuerySQL(const std::string& strSQL)
{


	if (strSQL.empty())
	{
		ERROR_LOG("the SQL is empty.");
		return false;
	}

	if (nullptr == m_pConnectPtr)
	{
		ERROR_LOG("the db is not connect.");
		return false;
	}



	using namespace boost;
	lock_guard<mutex> Lock(m_Lock);
	if (mysql_query(m_pConnectPtr, strSQL.c_str()))
	{
		ERROR_LOG("run [%s] failed,the error code[%d].", strSQL.c_str(), mysql_errno(m_pConnectPtr));
		return false;
	}

	return true;
}

bool CMysqlConnector::RunQuerySQL(const std::string& strSQL, std::vector<std::string>& ResultArray)
{
	if (strSQL.empty() || nullptr == m_pConnectPtr)
	{
		ERROR_LOG("the SQL is empty.");
		return false;
	}
	using namespace boost;
	lock_guard<mutex> Lock(m_Lock);

	//开始查询
	if (mysql_query(m_pConnectPtr, strSQL.c_str()))
	{
		ERROR_LOG("run [%s] failed,the error code[%d].", strSQL.c_str(), mysql_errno(m_pConnectPtr));
		return false;
	}

	MYSQL_RES* pResultSet = mysql_store_result(m_pConnectPtr);
	if (nullptr == pResultSet)
	{
		ERROR_LOG("run [%s] failed,the error code[%d].", strSQL.c_str(), mysql_errno(m_pConnectPtr));
		return false;
	}

	ResultArray.clear();

	//获取字段的个数
	int nColumnNum = mysql_num_fields(pResultSet);

	MYSQL_ROW RowElement;

	while ((RowElement = mysql_fetch_row(pResultSet)) != nullptr)
	{
		for (int i = 0; i < nColumnNum; ++i)
		{
			if (nullptr != RowElement[i])
			{
				ResultArray.push_back(RowElement[i]);
			}
			else
			{
				ResultArray.push_back("");
			}
			
		}
	}

	//释放内存
	mysql_free_result(pResultSet);
	return true;
}

CMysqlConnector::CMysqlConnector() :
	m_pConnectPtr(nullptr)
{
	
	//进行初始化
	mysql_library_init(0, nullptr, nullptr);
}


CMysqlConnector::~CMysqlConnector()
{
	//进行反初始化
	mysql_library_end();
}

CMysqlConnector CMysqlConnector::s_obj;
