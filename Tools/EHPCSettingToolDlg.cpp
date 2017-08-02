
// EHPCSettingToolDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "EHPCSettingTool.h"
#include "EHPCSettingToolDlg.h"
#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>
#include "Utility.h"
#include "ConfigTabFactory.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CBCGPDialog
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif
	virtual BOOL OnInitDialog();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CBCGPDialog(IDD_ABOUTBOX)
{
}

BOOL CAboutDlg::OnInitDialog()
{
	CBCGPDialog::OnInitDialog();

	EnableVisualManagerStyle(TRUE, TRUE);

	return TRUE;
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CBCGPDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CBCGPDialog)
END_MESSAGE_MAP()


// CEHPCSettingToolDlg 对话框



CEHPCSettingToolDlg::CEHPCSettingToolDlg(CWnd* pParent /*=NULL*/)
	: CBCGPDialog(IDD_EHPCSETTINGTOOL_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	//支持Layout
	//EnableLayout(TRUE);
}

void CEHPCSettingToolDlg::DoDataExchange(CDataExchange* pDX)
{
	CBCGPDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CEHPCSettingToolDlg, CBCGPDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BTN_SAVE, &CEHPCSettingToolDlg::OnBnClickedBtnSave)
	ON_BN_CLICKED(IDC_BTN_QUIT, &CEHPCSettingToolDlg::OnBnClickedBtnQuit)
END_MESSAGE_MAP()


// CEHPCSettingToolDlg 消息处理程序

BOOL CEHPCSettingToolDlg::OnInitDialog()
{
	using namespace std;
	CBCGPDialog::OnInitDialog();

	EnableVisualManagerStyle(TRUE,TRUE);

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	//监测配置文件
	vector<string> ConfigFileArray;
	DetectConfigFile(ConfigFileArray);

	if (!ConfigFileArray.empty())
	{
		InitDisplay();
		for (auto& Element : ConfigFileArray)
		{
			auto strTabName = m_TabMap.GetTabNameByConfigName(Element);
			if (!strTabName.empty())
			{
				auto pTabWnd = CreateConfigTabWnd(Element);
				if (nullptr != pTabWnd)
				{
					m_TabPtrArray.push_back(pTabWnd);
					pTabWnd->SetConfigFileName(Element);
					pTabWnd->CreateSelf(&m_TabWnd);
					m_TabWnd.AddTab(pTabWnd, CString(strTabName.c_str()));
				}
			}
		}
	}
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CEHPCSettingToolDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CBCGPDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CEHPCSettingToolDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CBCGPDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CEHPCSettingToolDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CEHPCSettingToolDlg::InitDisplay(void)
{

	if (NULL == m_TabWnd.GetSafeHwnd())
	{
		CWnd* pWnd = GetDlgItem(IDC_STATIC_TAB_POS);
		if (nullptr != pWnd && NULL != pWnd->GetSafeHwnd())
		{
			pWnd->ShowWindow(SW_HIDE);
			CRect rc;
			pWnd->GetWindowRect(&rc);
			ScreenToClient(&rc);
			//创建TabWnd
			m_TabWnd.Create(CBCGPTabWnd::STYLE_3D_ROUNDED, rc, this, CBaseTabWnd::s_nTabBaseID++, CBCGPTabWnd::LOCATION_TOP);
		
			m_TabWnd.EnableAutoColor();
		}
	
#if 0
		CBCGPStaticLayout* pLayout = (CBCGPStaticLayout*)GetLayout();
		if (pLayout != nullptr)
		{
			pLayout->AddAnchor(m_TabWnd.GetSafeHwnd(), CBCGPStaticLayout::e_MoveTypeNone, CBCGPStaticLayout::e_SizeTypeHorz);
		}
#endif
	}

	if (m_TabWnd.GetSafeHwnd())
	{
		m_TabWnd.RecalcLayout();
	}
}



void CEHPCSettingToolDlg::OnSize(UINT nType, int cx, int cy)
{
	CBCGPDialog::OnSize(nType, cx, cy);

}

//检索配置文件是否存在
void CEHPCSettingToolDlg::DetectConfigFile(std::vector<std::string>& ConfigFileArray)
{
	using namespace std;
	using namespace boost::filesystem;
	ConfigFileArray.clear();
	//获取当前目录
	string strDir = Utility::GetExeDirecory();
	if (strDir.empty())
	{
		return;
	}

	directory_iterator End;
	//遍历当前目录
	for (directory_iterator Iter(strDir); Iter != End; ++Iter)
	{

		path TempPath = *Iter;
		string strExtensionName = TempPath.extension().string();
		if (boost::iends_with(strExtensionName, "json"))
		{
			//获取配置文件名称
			ConfigFileArray.push_back(TempPath.filename().string());
		}
	}

}


void CEHPCSettingToolDlg::OnBnClickedBtnSave()
{
	// TODO: 在此添加控件通知处理程序代码

	for (auto Iter = m_TabPtrArray.begin(); Iter != m_TabPtrArray.end(); ++Iter)
	{
		Iter->SaveConfig();
	}
}


void CEHPCSettingToolDlg::OnBnClickedBtnQuit()
{
	// TODO: 在此添加控件通知处理程序代码
	PostMessage(WM_CLOSE, 0, 0);
}


BOOL CEHPCSettingToolDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (WM_KEYDOWN == pMsg->message)
	{
		if (VK_RETURN == pMsg->wParam || VK_ESCAPE == pMsg->wParam)
		{
			return TRUE;
		}
	}

	return CBCGPDialog::PreTranslateMessage(pMsg);
}
