
// EHPCSettingToolDlg.h : 头文件
//

#pragma once
#include <string>
#include <vector>
#include <boost/ptr_container/ptr_vector.hpp>
#include "BaseTabWnd.h"
#include "TabNameMap.h"
#include "BaseDialog.h"
// CEHPCSettingToolDlg 对话框
class CEHPCSettingToolDlg : public CBCGPDialog
{
// 构造
public:
	CEHPCSettingToolDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_EHPCSETTINGTOOL_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()


private:
	//初始化控件
	void InitDisplay(void);
private:
	//TabWnd
	CBaseTabWnd m_TabWnd;
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);


private:
	//监测相关配置文件是否存在
	void DetectConfigFile(std::vector<std::string>& ConfigFileArray);
public:
	afx_msg void OnBnClickedBtnSave();
	afx_msg void OnBnClickedBtnQuit();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

private:
	CTabNameMap m_TabMap;
	boost::ptr_vector<CBaseDialog> m_TabPtrArray;
};
