#pragma once

#include "RichEditCtrlEx.h"

// SuccessFlash 对话框

class SuccessFlash : public CDialogEx
{
	DECLARE_DYNAMIC(SuccessFlash)

public:
	SuccessFlash(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~SuccessFlash();

// 对话框数据
	enum { IDD = IDD_SUCCESSDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CRichEditCtrlEx m_EditResult;
};
