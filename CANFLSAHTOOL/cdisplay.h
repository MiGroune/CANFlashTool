#pragma once
#include "afxcmn.h"


// cdisplay 对话框

class cdisplay : public CDialogEx
{
	DECLARE_DYNAMIC(cdisplay)

public:
	cdisplay(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~cdisplay();

// 对话框数据
	enum { IDD = IDD_DISPLAY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual void OnCancel();
	DECLARE_MESSAGE_MAP()
public:
 
	virtual BOOL OnInitDialog();
		HANDLE DisplayThread ;
		static DWORD WINAPI CDisplayThread(LPVOID lpParameter);	
		int flag;
		 afx_msg LRESULT onbreak(WPARAM flag, LPARAM lparam);
};
