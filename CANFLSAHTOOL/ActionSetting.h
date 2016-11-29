#pragma once
 
#include "CheckItem.h"

// CActionSetting �Ի���

class CActionSetting : public CDialogEx
{
	DECLARE_DYNAMIC(CActionSetting)

public:
	CActionSetting(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CActionSetting();

// �Ի�������
	enum { IDD = IDD_ACTIONSETTIONG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CButton m_ItemAdd;
	CButton m_ItemDelete;
	CButton m_ItemEdit;
	CListCtrl m_subItemList;
	afx_msg void OnBnClickedButtonSubadd();
	afx_msg void OnBnClickedButtonSubedit();
	afx_msg void OnBnClickedButtonSubdelete();
		CCheckItem *CheckItem;
 	afx_msg void OnClickListSubitem(NMHDR *pNMHDR, LRESULT *pResult);
	int m_currentline;

};
