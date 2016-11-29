#pragma once

#include "CANFLSAHTOOLDlg.h"

// Querydlg 对话框

class Querydlg : public CDialogEx
{
	DECLARE_DYNAMIC(Querydlg)

public:
	Querydlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~Querydlg();

// 对话框数据
	enum { IDD = IDD_QUERYDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	class CCANFLSAHTOOLDlg *pParent;
	afx_msg void displayCal(CString buffer);
	afx_msg void displayquery();
 };
