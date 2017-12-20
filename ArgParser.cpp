/*********************************************************
Copyright(C):
FileName:ArgParser.cpp
Descripton:命令行描述，解析，存储工具类
Author:xiaowei.han
Data:2017/12/19
Others:
History:
Version:1.0
*********************************************************/
#include "stdafx.h"
#include "ArgParser.h"
#include <sstream>
#include <algorithm>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>

/*********************************************************
FunctionName:AddCommandInfo
FunctionDesc:添加命令行提示
InputParam:
OutputParam:
Return:
Author:xiaowei.han
*********************************************************/
void CArgParser::AddCommandInfo(const LP_COMMAND_INFO pCommandInfo)
{
	if (nullptr == pCommandInfo)
	{
		return;
	}
	m_commandInfo.emplace_back(pCommandInfo->strCommand, pCommandInfo->eumType, pCommandInfo->strCommandPrompt,pCommandInfo->eumSource);
}

/*********************************************************
FunctionName:ParseCommandInfo
FunctionDesc:
InputParam:
OutputParam:
Return:
Author:xiaowei.han
*********************************************************/
void CArgParser::ParseCommandInfo(int nArgc, char* argv[])
{
	//参数合法性判断
	if (nArgc <= 0 || nullptr == argv || m_commandInfo.empty())
	{
		return;
	}
	using namespace std;
	using namespace boost::program_options;
	//来源是命令行的参数信息
	vector<COMMAND_INFO> CommandSourceArray;
	//来源是命令行位置信息的参数信息
	vector<COMMAND_INFO> PositionCommandSourceArray;
	//来源是配置文件的参数信息
	vector<COMMAND_INFO> ConfigSourceArray;
	//进行分类
	SortParamInfo(CommandSourceArray, ConfigSourceArray, PositionCommandSourceArray);

	if (!CommandSourceArray.empty())
	{
		//定义描述器
		options_description Opt("commands usage.");
		for (auto& V : CommandSourceArray)
		{
			switch (V.eumType)
			{
			case COMMAND_NO_TYPE:
				Opt.add_options()(V.strCommand.c_str(),V.strCommandPrompt.c_str());
				break;
			case COMMAND_INT_TYPE:
				Opt.add_options()(V.strCommand.c_str(), value<int>(), V.strCommandPrompt.c_str());
				break;
			case COMMAND_FLOAT_TYPE:
				Opt.add_options()(V.strCommand.c_str(), value<double>(), V.strCommandPrompt.c_str());
				break;
			case COMMAND_STR_TYPE:
				Opt.add_options()(V.strCommand.c_str(), value<string>(), V.strCommandPrompt.c_str());
				break;
			default:
				break;
			}
		}

		if (!PositionCommandSourceArray.empty())
		{
			positional_options_description PositionOpt;

			for (int i = 0; i < (int)PositionCommandSourceArray.size(); ++i)
			{
				//提前注册
				switch (PositionCommandSourceArray[i].eumType)
				{
				case COMMAND_INT_TYPE:
					Opt.add_options()(PositionCommandSourceArray[i].strCommand.c_str(), value<int>(), PositionCommandSourceArray[i].strCommandPrompt.c_str());
					break;
				case COMMAND_FLOAT_TYPE:
					Opt.add_options()(PositionCommandSourceArray[i].strCommand.c_str(), value<double>(), PositionCommandSourceArray[i].strCommandPrompt.c_str());
					break;
				case COMMAND_STR_TYPE:
					Opt.add_options()(PositionCommandSourceArray[i].strCommand.c_str(), value<string>(), PositionCommandSourceArray[i].strCommandPrompt.c_str());
					break;
				default:
					break;
				}

				//如果是最后一个
				if (i == (int)PositionCommandSourceArray.size() - 1)
				{
					PositionOpt.add(PositionCommandSourceArray[i].strCommand.c_str(), -1);
				}
				else
				{
					PositionOpt.add(PositionCommandSourceArray[i].strCommand.c_str(), 1);
				}
			}
			//开始解析
			auto Result = command_line_parser(nArgc, argv).options(Opt).positional(PositionOpt).run();
			//保存结果
			store(Result, m_VarInfo);
		}
		//直接解析
		else
		{
			auto Result = parse_command_line(nArgc, argv, Opt);
			//保存结果
			store(Result, m_VarInfo);
		}
	}

	if (!ConfigSourceArray.empty())
	{

		if (boost::filesystem::exists(m_strConfigPath))
		{
			//定义描述器
			options_description Opt("config");
			for (auto& V : ConfigSourceArray)
			{
				switch (V.eumType)
				{
				case COMMAND_INT_TYPE:
					Opt.add_options()(V.strCommand.c_str(), value<int>(), V.strCommandPrompt.c_str());
					break;
				case COMMAND_FLOAT_TYPE:
					Opt.add_options()(V.strCommand.c_str(), value<double>(), V.strCommandPrompt.c_str());
					break;
				case COMMAND_STR_TYPE:
					Opt.add_options()(V.strCommand.c_str(), value<string>(), V.strCommandPrompt.c_str());
					break;
				default:
					break;
				}
			}

			auto Result = parse_config_file<char>(m_strConfigPath.c_str(), Opt, true);

			store(Result, m_VarInfo);
		}
	}

	//通知
	notify(m_VarInfo);
}

/*********************************************************
FunctionName:GetCommandValue
FunctionDesc:
InputParam:
OutputParam:
Return:
Author:xiaowei.han
*********************************************************/
bool CArgParser::GetCommandValue(const std::string& strCommand, void* pValue)
{
	if (strCommand.empty() || nullptr == pValue)
	{
		return false;
	}

	//根据key找对应的类型
	auto FindPred = [strCommand](const COMMAND_INFO& Element)->bool
	{
		if (boost::contains(Element.strCommand,strCommand))
		{
			return true;
		}
		return false;
	};

	auto Iter = std::find_if(m_commandInfo.begin(), m_commandInfo.end(), FindPred);

	if (Iter == m_commandInfo.end())
	{
		return false;
	}

	if (!m_VarInfo.count(strCommand))
	{
		return false;
	}

	
	switch (Iter->eumType)
	{
	case COMMAND_NO_TYPE:
		break;
	case COMMAND_INT_TYPE:
	{
		*(int*)pValue = m_VarInfo[strCommand].as<int>();
	}
		break;
	case COMMAND_FLOAT_TYPE:
	{
		*(double*)pValue = m_VarInfo[strCommand].as<double>();
	}
		break;
	case COMMAND_STR_TYPE:
	{
		*(std::string*)pValue = m_VarInfo[strCommand].as<std::string>();
	}
		break;
	default:
		break;
	}
	return true;
}

/*********************************************************
FunctionName:SetConfigPath
FunctionDesc:设置配置文件的路径
InputParam:
OutputParam:
Return:
Author:xiaowei.han
*********************************************************/
void CArgParser::SetConfigPath(const std::string& strConfigPath)
{
	m_strConfigPath = strConfigPath;
}


std::string CArgParser::DisplayHelpInfo(void)
{
	using namespace std;
	using namespace boost::program_options;

	string strResult;

	//来源是命令行的参数信息
	vector<COMMAND_INFO> CommandSourceArray;
	//来源是命令行位置信息的参数信息
	vector<COMMAND_INFO> PositionCommandSourceArray;
	//来源是配置文件的参数信息
	vector<COMMAND_INFO> ConfigSourceArray;
	//进行分类
	SortParamInfo(CommandSourceArray, ConfigSourceArray, PositionCommandSourceArray);

	if (!CommandSourceArray.empty())
	{
		//定义描述器
		options_description HelpOpt("commands usage.");
		for (auto& V : CommandSourceArray)
		{
			switch (V.eumType)
			{
			case COMMAND_NO_TYPE:
				HelpOpt.add_options()(V.strCommand.c_str(), V.strCommandPrompt.c_str());
				break;
			case COMMAND_INT_TYPE:
				HelpOpt.add_options()(V.strCommand.c_str(), value<int>(), V.strCommandPrompt.c_str());
				break;
			case COMMAND_FLOAT_TYPE:
				HelpOpt.add_options()(V.strCommand.c_str(), value<double>(), V.strCommandPrompt.c_str());
				break;
			case COMMAND_STR_TYPE:
				HelpOpt.add_options()(V.strCommand.c_str(), value<string>(), V.strCommandPrompt.c_str());
				break;
			default:
				break;
			}
		}

		stringstream OutPut;

		HelpOpt.print(OutPut);

		strResult = OutPut.str();
	}

	return strResult;
}

/*********************************************************
FunctionName:SortParamInfo
FunctionDesc:参数信息分类
InputParam:
OutputParam:
Return:
Author:xiaowei.han
*********************************************************/
void CArgParser::SortParamInfo(std::vector<COMMAND_INFO>& CommandSourceArray, std::vector<COMMAND_INFO>& ConfigSourceArray, std::vector<COMMAND_INFO>& PositionCommandSourceArray)
{
	CommandSourceArray.clear();
	ConfigSourceArray.clear();
	PositionCommandSourceArray.clear();
	//开始遍历
	for (auto& Element : m_commandInfo)
	{
		switch (Element.eumSource)
		{
			//源是来自命令行的
		case COMMAND_SOURCE:
			CommandSourceArray.emplace_back(Element.strCommand, Element.eumType, Element.strCommandPrompt, Element.eumSource);
			break;
			//源是来自命令行的位置的
		case POSITION_COMMAND_SOURCE:
			PositionCommandSourceArray.emplace_back(Element.strCommand, Element.eumType, Element.strCommandPrompt, Element.eumSource);
			break;
			//源是来自配置文件的
		case CONFIG_FILE_SOURCE:
			ConfigSourceArray.emplace_back(Element.strCommand, Element.eumType, Element.strCommandPrompt, Element.eumSource);
			break;
		default:
			break;
		}
	}
}
