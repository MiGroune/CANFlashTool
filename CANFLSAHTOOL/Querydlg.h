#pragma once

#include "CANFLSAHTOOLDlg.h"

// Querydlg �Ի���

class Querydlg : public CDialogEx
{
	DECLARE_DYNAMIC(Querydlg)

public:
	Querydlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~Querydlg();

// �Ի�������
	enum { IDD = IDD_QUERYDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	class CCANFLSAHTOOLDlg *pParent;
	afx_msg void displayCal(CString buffer);
	afx_msg void displayquery();
 };
