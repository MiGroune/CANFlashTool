#pragma once


// CCheckItem �Ի���

class CCheckItem : public CDialogEx
{
	DECLARE_DYNAMIC(CCheckItem)

public:
	CCheckItem(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CCheckItem();

// �Ի�������
	enum { IDD = IDD_CHECKITEM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
//	CComboBox m_Item_Value;
//	CEdit m_Item_Address;
	CString m_Item_Address;
	CString m_Item_Value;
//	CComboBox m_Item_Type;
	CString m_Item_Label;
	CString m_Item_Size;
//	CEdit m_Item_Comment;
	CString m_Item_Type;
	virtual BOOL OnInitDialog();
	CString m_Item_Comment;
};
