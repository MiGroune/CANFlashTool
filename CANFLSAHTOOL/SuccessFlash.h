#pragma once

#include "RichEditCtrlEx.h"

// SuccessFlash �Ի���

class SuccessFlash : public CDialogEx
{
	DECLARE_DYNAMIC(SuccessFlash)

public:
	SuccessFlash(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~SuccessFlash();

// �Ի�������
	enum { IDD = IDD_SUCCESSDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CRichEditCtrlEx m_EditResult;
};
