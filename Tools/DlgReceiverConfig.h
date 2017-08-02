#pragma once

#include "BaseDialog.h"
// CDlgReceiverConfig 对话框

class CDlgReceiverConfig : public CBaseDialog
{
	DECLARE_DYNAMIC(CDlgReceiverConfig)

public:
	CDlgReceiverConfig(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgReceiverConfig();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_RECEIVER_CONFIG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL CreateSelf(CWnd* pWnd) override;
	virtual void SaveConfig(void) override;
private:
	CIPAddressCtrl m_ReportAddress;
	CIPAddressCtrl m_ScheduerAddress;
public:
	virtual BOOL OnInitDialog();

private:
	void DisplayConfigInfo(void);
};
