#pragma once

#include "RichEditCtrlEx.h"

// CFailDlg 对话框

class CFailDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CFailDlg)

public:
	CFailDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CFailDlg();

// 对话框数据
	enum { IDD = IDD_FAILDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:

	virtual BOOL OnInitDialog();
	void  CFailDlg::displayResult(int result);
	CRichEditCtrlEx m_EditResult;
};
