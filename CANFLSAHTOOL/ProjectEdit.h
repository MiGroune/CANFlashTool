#pragma once
#include "ConfigFile.h"
#include "CANFLSAHTOOLDlg.h"


// CProjectEdit 对话框

class CProjectEdit : public CDialogEx
{
	DECLARE_DYNAMIC(CProjectEdit)

public:
	CProjectEdit(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CProjectEdit();

// 对话框数据
	enum { IDD = IDD_EDITPROJECT_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonPrg();
	afx_msg void OnBnClickedCheckKeyneed();
	afx_msg void OnBnClickedCheckKeymanual();
	void CProjectEdit::DisplaySecurity();

	CString m_szConfigPath;
	CString m_szBeforePath;
	CString m_szAfterPath;
	BOOL m_bBeforeFlag;
	BOOL m_bAfterFlag;
	BOOL m_bDefaultFlag;
	BOOL m_bSetParamFlag;
	BOOL m_bChecksumFlag;
	BOOL m_bEnableStopFlag;
	BOOL m_bBypassFlag;
	ConfigFile *m_pConfig;
	class CCANFLSAHTOOLDlg *pParent;
	CString m_szKeyValue;
	int m_nKeyFlag;
	
	CString m_szKeyPath;

	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButtonKeypath();
	afx_msg void OnBnClickedCheckRunbefore();
	afx_msg void OnBnClickedCheckAfter();
	afx_msg void OnBnClickedButtonBefore();
	afx_msg void OnBnClickedButtonAfter();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedCheckNv();
	afx_msg void OnBnClickedCheckDongle();
	afx_msg void OnBnClickedCheckEcuainfo();
 
	afx_msg void OnBnClickedCheckDefault();
	afx_msg void OnBnClickedCheckKeyparam();
	CString m_szParamValue;
	afx_msg void OnBnClickedCheckKeyauto();
	afx_msg void OnBnClickedCheckChecksum();
	afx_msg void OnBnClickedCheckEnablestop();
	afx_msg void OnBnClickedCheckBypass();
};
