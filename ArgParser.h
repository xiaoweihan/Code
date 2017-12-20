/*********************************************************
Copyright(C):
FileName:ArgParser.h
Descripton:命令行描述，解析，存储工具类
Author:xiaowei.han
Data:2017/12/19
Others:
History:
Version:1.0
*********************************************************/
#ifndef __ARG_PARSER_H__
#define __ARG_PARSER_H__
#include <map>
#include <vector>
#include <boost/program_options.hpp>

//参数类型
enum PARAM_TYPE
{
	//非类型
	COMMAND_NO_TYPE = 0,
	//整型
	COMMAND_INT_TYPE,
	//浮点类型
	COMMAND_FLOAT_TYPE,
	//字符串类型
	COMMAND_STR_TYPE
};

//参数来源
enum PARAM_SOURCE
{
	//来自命令行输入
	COMMAND_SOURCE = 0,
	//来自配置文件
	CONFIG_FILE_SOURCE,
	//来自命令行位置输入
	POSITION_COMMAND_SOURCE

};

struct _command_info
{
	//命令行(长名和短名以,分割)
	std::string strCommand;
	//参数类型
	PARAM_TYPE eumType;
	//命令行提示信息
	std::string strCommandPrompt;
	//参数来源
	PARAM_SOURCE eumSource;

	_command_info() = default;

	_command_info(const std::string& strCommand, PARAM_TYPE eumType, const std::string& strCommandPrompt, PARAM_SOURCE eumSource)
	{
		this->strCommand = strCommand;
		this->eumType = eumType;
		this->strCommandPrompt = strCommandPrompt;
		this->eumSource = eumSource;
	}

};
using COMMAND_INFO = _command_info;
using LP_COMMAND_INFO = _command_info*;


class CArgParser
{
public:
	CArgParser() = default;
	~CArgParser() = default;

public:
	//配置命令行信息
	void AddCommandInfo(const LP_COMMAND_INFO pCommandInfo);

	//解析命令行
	void ParseCommandInfo(int nArgc,char* argv[]);

	//获取解析后的命令值
	bool GetCommandValue(const std::string& strCommand, void* pValue);

	//设置配置文件路径(如果参数来源是配置文件中来的则必须配置此路径)
	void SetConfigPath(const std::string& strConfigPath);

	//显示帮助信息
	std::string DisplayHelpInfo(void);
private:
	//对参数信息进行分类
	void SortParamInfo(std::vector<COMMAND_INFO>& CommandSourceArray, std::vector<COMMAND_INFO>& ConfigSourceArray, std::vector<COMMAND_INFO>& PositionCommandSourceArray);
private:
	//保存的参数信息
	std::vector<COMMAND_INFO> m_commandInfo;
	//配置文件的路径
	std::string m_strConfigPath;
	//命令行的存储器
	boost::program_options::variables_map m_VarInfo;
};



#endif
