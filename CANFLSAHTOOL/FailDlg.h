#pragma once

#include "RichEditCtrlEx.h"

// CFailDlg �Ի���

class CFailDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CFailDlg)

public:
	CFailDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CFailDlg();

// �Ի�������
	enum { IDD = IDD_FAILDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:

	virtual BOOL OnInitDialog();
	void  CFailDlg::displayResult(int result);
	CRichEditCtrlEx m_EditResult;
};
