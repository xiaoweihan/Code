#pragma once

#include <map>
#include <string>
class CTabNameMap
{
public:
	CTabNameMap();
	virtual ~CTabNameMap();

public:
	//更具配置文件名称获取Tab名称
	std::string GetTabNameByConfigName(const std::string& strConfigName);

private:
	void InitTabMap(void);
private:
	//配置文件名称与tab页名称一一对应
	std::map<std::string, std::string> m_TabMap;
};

