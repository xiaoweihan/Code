#pragma once

#include "BaseDialog.h"
#include "afxcmn.h"
// CDlgStatusManagerConfig 对话框

class CDlgStatusManagerConfig : public CBaseDialog
{
	DECLARE_DYNAMIC(CDlgStatusManagerConfig)

public:
	CDlgStatusManagerConfig(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgStatusManagerConfig();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_STATUS_MANAGER_CONFIG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL CreateSelf(CWnd* pWnd) override;
	virtual void SaveConfig(void) override;
private:
	CIPAddressCtrl m_DBServerAddress;
public:
	virtual BOOL OnInitDialog();
private:
	void DisplayConfigInfo(void);
};
