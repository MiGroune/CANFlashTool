#pragma once
#include "afxcmn.h"


// CBuildCut 对话框

class CBuildCut : public CDialogEx
{
	DECLARE_DYNAMIC(CBuildCut)

public:
	CBuildCut(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CBuildCut();

// 对话框数据
	enum { IDD = IDD_DIALOG_BUILD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:

	virtual BOOL OnInitDialog();
	CString calstr;
	CString appstr;
	int flag;
	int mixflag;
};
