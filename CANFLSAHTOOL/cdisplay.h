#pragma once
#include "afxcmn.h"


// cdisplay �Ի���

class cdisplay : public CDialogEx
{
	DECLARE_DYNAMIC(cdisplay)

public:
	cdisplay(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~cdisplay();

// �Ի�������
	enum { IDD = IDD_DISPLAY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual void OnCancel();
	DECLARE_MESSAGE_MAP()
public:
 
	virtual BOOL OnInitDialog();
		HANDLE DisplayThread ;
		static DWORD WINAPI CDisplayThread(LPVOID lpParameter);	
		int flag;
		 afx_msg LRESULT onbreak(WPARAM flag, LPARAM lparam);
};
