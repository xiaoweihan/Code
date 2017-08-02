#pragma once


// CDlgNodeConfig 对话框
#include "BaseDialog.h"
#include "afxcmn.h"
class CDlgNodeConfig : public CBaseDialog
{
	DECLARE_DYNAMIC(CDlgNodeConfig)

public:
	CDlgNodeConfig(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgNodeConfig();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_NODE_CONFIG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL CreateSelf(CWnd* pWnd) override;
	virtual void SaveConfig(void) override;
private:
	CIPAddressCtrl m_SelfAddress;
	CIPAddressCtrl m_ReportServerAddress;
	CIPAddressCtrl m_CenterServerAddress;
public:
	virtual BOOL OnInitDialog();

private:
	void DisplayConfigInfo(void);
};
