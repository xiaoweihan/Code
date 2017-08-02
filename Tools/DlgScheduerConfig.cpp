// DlgScheduerConfig.cpp : 实现文件
//

#include "stdafx.h"
#include "EHPCSettingTool.h"
#include "DlgScheduerConfig.h"
#include <string>
#include <WS2tcpip.h>
#include <boost/format.hpp>
#include "Utility.h"
#include "ConfigManager.h"

// CDlgScheduerConfig 对话框

IMPLEMENT_DYNAMIC(CDlgScheduerConfig, CBaseDialog)

CDlgScheduerConfig::CDlgScheduerConfig(CWnd* pParent /*=NULL*/)
	: CBaseDialog(IDD_DLG_SCHEDUER_CONFIG, pParent)
{

}

CDlgScheduerConfig::~CDlgScheduerConfig()
{
}

void CDlgScheduerConfig::DoDataExchange(CDataExchange* pDX)
{
	CBaseDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SERVER_ADDRESS, m_IPAddress);
}


BOOL CDlgScheduerConfig::CreateSelf(CWnd* pWnd)
{
	return Create(CDlgScheduerConfig::IDD, pWnd);
}

void CDlgScheduerConfig::SaveConfig(void)
{
	using namespace std;
	using namespace ConfigType;
	if (NULL == m_IPAddress.GetSafeHwnd())
	{
		return;
	}

	SCHEDUER_CONFIG_INFO ConfigInfo;

	BYTE IP1 = 0;
	BYTE IP2 = 0;
	BYTE IP3 = 0;
	BYTE IP4 = 0;
	//获取IP地址
	m_IPAddress.GetAddress(IP1, IP2, IP3, IP4);
	string strIPAddressFormat("%1%.%2%.%3%.%4%");
	boost::format FT(strIPAddressFormat);
	FT % (int)IP1 % (int)IP2 % (int)IP3 % (int)IP4;
	ConfigInfo.strReportAddress = FT.str();

	//获取端口号
	ConfigInfo.nPort = GetDlgItemInt(IDC_PORT_NUM);
	ConfigInfo.nReportPort = GetDlgItemInt(IDC_SERVER_PORT);

	//获取路径
	string strConfigPath = Utility::GetExeDirecory() + string(R"(\)") + m_strConfigFileName;

	//生成json文件
	CConfigManager::CreateInstance().WriteScheduerConfigInfo(strConfigPath, &ConfigInfo);
}

void CDlgScheduerConfig::DisplayConfigInfo(void)
{
	using namespace std;
	ConfigType::SCHEDUER_CONFIG_INFO ConfigInfo;

	//获取路径
	string strConfigPath = Utility::GetExeDirecory() + string(R"(\)") + m_strConfigFileName;

	//加载成功
	if (CConfigManager::CreateInstance().ReadScheduerConfigInfo(strConfigPath, &ConfigInfo))
	{
		if (m_IPAddress.GetSafeHwnd())
		{
			unsigned long nAddress = 0;
			inet_pton(AF_INET, ConfigInfo.strReportAddress.c_str(), &nAddress);

			nAddress = htonl(nAddress);
			//对IP地址进行转换
			m_IPAddress.SetAddress(nAddress);
		}

		SetDlgItemInt(IDC_PORT_NUM, ConfigInfo.nPort);
		SetDlgItemInt(IDC_SERVER_PORT, ConfigInfo.nReportPort);
	}
	else
	{
		if (m_IPAddress.GetSafeHwnd())
		{
			unsigned long nAddress = 0;
			inet_pton(AF_INET, "127.0.0.1", &nAddress);
			//对IP地址进行转换
			nAddress = htonl(nAddress);
			m_IPAddress.SetAddress(nAddress);
		}
		SetDlgItemInt(IDC_PORT_NUM, 0);
		SetDlgItemInt(IDC_SERVER_PORT, 0);
	}
}

BEGIN_MESSAGE_MAP(CDlgScheduerConfig, CBaseDialog)
END_MESSAGE_MAP()


// CDlgScheduerConfig 消息处理程序


BOOL CDlgScheduerConfig::OnInitDialog()
{
	CBaseDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	DisplayConfigInfo();
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}
