#include "stdafx.h"
#define MYSQL_INTERFACE_EXPORT  (1)
#include "mysql_interface.h"
#include "MysqlConnector.h"
MYSQL_DLL_INTERFACE bool OpenDB(const std::string& strAddress, int nPort, const std::string& strUserName, const std::string& strPassword, const std::string& strDBInstance)
{
	CONNECTOR_ELEMENT Element;
	Element.nPort = nPort;
	Element.strIPAddress = strAddress;
	Element.strMysqlInstance = strDBInstance;
	Element.strPassword = strPassword;
	Element.strUserName = strUserName;
	CMysqlConnector::CreateInstance().SetConnectorElement(Element);

	return CMysqlConnector::CreateInstance().ConnectDB();
}

MYSQL_DLL_INTERFACE bool CloseDB(void)
{
	return CMysqlConnector::CreateInstance().DisconnectDB();
}

MYSQL_DLL_INTERFACE void PingDB(void)
{
	return CMysqlConnector::CreateInstance().PingDB();
}

MYSQL_DLL_INTERFACE bool RunNoQuerySQL(const std::string& strSQL)
{
	return CMysqlConnector::CreateInstance().RunNoQuerySQL(strSQL);
}

MYSQL_DLL_INTERFACE bool RunQuerySQL(const std::string& strSQL, std::vector<std::string>& ResultArray)
{
	return CMysqlConnector::CreateInstance().RunQuerySQL(strSQL,ResultArray);
}
