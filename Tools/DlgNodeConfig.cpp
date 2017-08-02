// DlgNodeConfig.cpp : 实现文件
//

#include "stdafx.h"
#include "EHPCSettingTool.h"
#include "DlgNodeConfig.h"
#include <string>
#include <WS2tcpip.h>
#include <boost/format.hpp>
#include "Utility.h"
#include "ConfigManager.h"
// CDlgNodeConfig 对话框

IMPLEMENT_DYNAMIC(CDlgNodeConfig, CBaseDialog)

CDlgNodeConfig::CDlgNodeConfig(CWnd* pParent /*=NULL*/)
	: CBaseDialog(IDD_DLG_NODE_CONFIG, pParent)
{

}

CDlgNodeConfig::~CDlgNodeConfig()
{
}

void CDlgNodeConfig::DoDataExchange(CDataExchange* pDX)
{
	CBaseDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SELF_ADDRESS, m_SelfAddress);
	DDX_Control(pDX, IDC_SERVER_ADDRESS, m_ReportServerAddress);
	DDX_Control(pDX, IDC_CENTER_SERVER_ADDRESS, m_CenterServerAddress);
}


BOOL CDlgNodeConfig::CreateSelf(CWnd* pWnd)
{
	return Create(CDlgNodeConfig::IDD, pWnd);
}

void CDlgNodeConfig::SaveConfig(void)
{
	using namespace std;
	using namespace ConfigType;
	
	if (NULL == m_SelfAddress.GetSafeHwnd() || NULL == m_ReportServerAddress.GetSafeHwnd() || NULL == m_CenterServerAddress.GetSafeHwnd())
	{
		return;
	}

	NODE_CONFIG_INFO ConfigInfo;

	//获取端口号
	ConfigInfo.nPort = GetDlgItemInt(IDC_PORT_NUM);
	BYTE IP1 = 0;
	BYTE IP2 = 0;
	BYTE IP3 = 0;
	BYTE IP4 = 0;
	//获取IP地址
	m_SelfAddress.GetAddress(IP1, IP2, IP3, IP4);
	string strIPAddressFormat("%1%.%2%.%3%.%4%");
	boost::format FT(strIPAddressFormat);
	FT % (int)IP1 % (int)IP2 % (int)IP3 % (int)IP4;
	ConfigInfo.strIPAddress = FT.str();

	ConfigInfo.nReportPort = GetDlgItemInt(IDC_SERVER_PORT);
	//获取IP地址
	m_ReportServerAddress.GetAddress(IP1, IP2, IP3, IP4);
	FT.clear();
	FT % (int)IP1 % (int)IP2 % (int)IP3 % (int)IP4;
	ConfigInfo.strReportAddress = FT.str();


	//获取IP地址
	ConfigInfo.nCenterPort = GetDlgItemInt(IDC_CENTER_SERVER_PORT);
	m_CenterServerAddress.GetAddress(IP1, IP2, IP3, IP4);
	FT.clear();
	FT % (int)IP1 % (int)IP2 % (int)IP3 % (int)IP4;
	ConfigInfo.strCenterAddress = FT.str();

	ConfigInfo.nReportGap = GetDlgItemInt(IDC_GAP);
	
	//获取路径
	string strConfigPath = Utility::GetExeDirecory() + string(R"(\)") + m_strConfigFileName;

	//生成json文件
	CConfigManager::CreateInstance().WriteNodeConfigInfo(strConfigPath, &ConfigInfo);
}

BEGIN_MESSAGE_MAP(CDlgNodeConfig, CBaseDialog)
END_MESSAGE_MAP()


// CDlgNodeConfig 消息处理程序


BOOL CDlgNodeConfig::OnInitDialog()
{
	CBaseDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	DisplayConfigInfo();
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

void CDlgNodeConfig::DisplayConfigInfo(void)
{
	using namespace std;
	ConfigType::NODE_CONFIG_INFO ConfigInfo;

	//获取路径
	string strConfigPath = Utility::GetExeDirecory() + string(R"(\)") + m_strConfigFileName;

	//加载成功
	if (CConfigManager::CreateInstance().ReadNodeConfigInfo(strConfigPath, &ConfigInfo))
	{
		if (m_SelfAddress.GetSafeHwnd())
		{
			unsigned long nAddress = 0;
			inet_pton(AF_INET, ConfigInfo.strIPAddress.c_str(), &nAddress);

			nAddress = htonl(nAddress);
			//对IP地址进行转换
			m_SelfAddress.SetAddress(nAddress);
		}
		//自身端口号
		SetDlgItemInt(IDC_PORT_NUM, ConfigInfo.nPort);
		if (m_ReportServerAddress.GetSafeHwnd())
		{
			unsigned long nAddress = 0;
			inet_pton(AF_INET, ConfigInfo.strReportAddress.c_str(), &nAddress);

			nAddress = htonl(nAddress);
			//对IP地址进行转换
			m_ReportServerAddress.SetAddress(nAddress);
		}
		SetDlgItemInt(IDC_SERVER_PORT, ConfigInfo.nReportPort);

		if (m_CenterServerAddress.GetSafeHwnd())
		{
			unsigned long nAddress = 0;
			inet_pton(AF_INET, ConfigInfo.strCenterAddress.c_str(), &nAddress);

			nAddress = htonl(nAddress);
			//对IP地址进行转换
			m_CenterServerAddress.SetAddress(nAddress);
		}
		SetDlgItemInt(IDC_CENTER_SERVER_PORT, ConfigInfo.nCenterPort);

		SetDlgItemInt(IDC_GAP, ConfigInfo.nReportGap);

	}
	else
	{
		if (m_SelfAddress.GetSafeHwnd())
		{
			unsigned long nAddress = 0;
			inet_pton(AF_INET, Utility::GetSelfNetAddress().c_str(), &nAddress);

			nAddress = htonl(nAddress);
			//对IP地址进行转换
			m_SelfAddress.SetAddress(nAddress);
		}

		//自身端口号
		SetDlgItemInt(IDC_PORT_NUM, ConfigInfo.nPort);
		if (m_ReportServerAddress.GetSafeHwnd())
		{
			unsigned long nAddress = 0;
			inet_pton(AF_INET, ConfigInfo.strReportAddress.c_str(), &nAddress);

			nAddress = htonl(nAddress);
			//对IP地址进行转换
			m_ReportServerAddress.SetAddress(nAddress);
		}
		SetDlgItemInt(IDC_SERVER_PORT, ConfigInfo.nReportPort);

		if (m_CenterServerAddress.GetSafeHwnd())
		{
			unsigned long nAddress = 0;
			inet_pton(AF_INET, ConfigInfo.strCenterAddress.c_str(), &nAddress);

			nAddress = htonl(nAddress);
			//对IP地址进行转换
			m_CenterServerAddress.SetAddress(nAddress);
		}
		SetDlgItemInt(IDC_CENTER_SERVER_PORT, ConfigInfo.nCenterPort);

		SetDlgItemInt(IDC_GAP, ConfigInfo.nReportGap);
	}
}
