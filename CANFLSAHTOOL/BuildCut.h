#pragma once
#include "afxcmn.h"


// CBuildCut �Ի���

class CBuildCut : public CDialogEx
{
	DECLARE_DYNAMIC(CBuildCut)

public:
	CBuildCut(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CBuildCut();

// �Ի�������
	enum { IDD = IDD_DIALOG_BUILD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:

	virtual BOOL OnInitDialog();
	CString calstr;
	CString appstr;
	int flag;
	int mixflag;
};
