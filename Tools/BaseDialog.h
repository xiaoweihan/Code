/*******************************************************************************
Copyright(C):Envision. Co., Ltd.
FileName:BaseDialog.h
Description:Dialog的基类,所有对话框都从此派生
Author:xiaowei.han
Date:2016-9-11
Others:
Histroy:
*******************************************************************************/
#ifndef BASE_DIALOG_H
#define BASE_DIALOG_H
#include <string>
class CBaseDialog : public CBCGPDialog
{
	DECLARE_DYNAMIC(CBaseDialog)
public:
	CBaseDialog(UINT nIDTemplate, CWnd* pParentWnd = NULL,BOOL bEnableLayout = FALSE);
	~CBaseDialog(void);

	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnSize(UINT nType, int cx, int cy);

public:
	virtual BOOL CreateSelf(CWnd* pWnd) = 0;

	virtual void SaveConfig(void) = 0;

	void SetConfigFileName(const std::string& strConfigFileName)
	{
		m_strConfigFileName = strConfigFileName;
	}

	std::string GetConfigFileName(void)
	{
		return m_strConfigFileName;
	}

protected:
	std::string m_strConfigFileName;
};

#endif
