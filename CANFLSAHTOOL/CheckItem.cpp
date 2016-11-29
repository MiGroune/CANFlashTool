// CheckItem.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "CANFLSAHTOOL.h"
#include "CheckItem.h"
#include "afxdialogex.h"


// CCheckItem �Ի���

IMPLEMENT_DYNAMIC(CCheckItem, CDialogEx)

CCheckItem::CCheckItem(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCheckItem::IDD, pParent)
	, m_Item_Type(_T(""))
{

	m_Item_Address = _T("");
	m_Item_Value = _T("");
	m_Item_Label = _T("");
	m_Item_Size = _T("");
	m_Item_Comment = _T("");
}

CCheckItem::~CCheckItem()
{
}

void CCheckItem::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//  DDX_Control(pDX, IDC_COMBO_VALUE, m_Item_Value);
	//  DDX_Control(pDX, IDC_EDIT_ADDRESS, m_Item_Address);
	DDX_Text(pDX, IDC_EDIT_ADDRESS, m_Item_Address);
	DDX_CBString(pDX, IDC_COMBO_VALUE, m_Item_Value);
	//  DDX_Control(pDX, IDC_COMBO1, m_Item_Type);
	DDX_Text(pDX, IDC_EDIT_LABEL, m_Item_Label);
	DDX_Text(pDX, IDC_EDIT_SIZE, m_Item_Size);
	//  DDX_Control(pDX, IDC_EDIT_COMMENT, m_Item_Comment);
	DDX_CBString(pDX, IDC_COMBO_TYPE, m_Item_Type);
	DDX_Text(pDX, IDC_EDIT_COMMENT, m_Item_Comment);
}


BEGIN_MESSAGE_MAP(CCheckItem, CDialogEx)
END_MESSAGE_MAP()


// CCheckItem ��Ϣ�������


BOOL CCheckItem::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	CComboBox *combo=(CComboBox*) GetDlgItem(IDC_COMBO_TYPE);
	combo->AddString(_T("Merge"));
	combo->AddString(_T("Display"));
 	combo->SetItemData(0,1);
	combo->SetItemData(1,2);

	int i=combo->FindString( -1,m_Item_Type);
	if(i>=0)combo->SelectString(i,m_Item_Type);  

	CComboBox *combo2=(CComboBox*) GetDlgItem(IDC_COMBO_VALUE);
	
	if(m_Item_Label == "KKPGMID")
	{ 
		//combo2->AddString(_T("%NULL"));
		combo2->AddString(_T("AA"));
		combo2->AddString(_T("08"));
 		combo2->SetItemData(0,1);
		combo2->SetItemData(1,2);
		//combo2->SetItemData(2,3);
		combo2->SetCurSel(0);
	}
	if(m_Item_Label == "SECURITY")
	{
		combo2->AddString(_T("%NULL"));
		combo2->AddString(_T("7C"));
		combo2->AddString(_T("FE"));
 		combo2->SetItemData(0,1);
		combo2->SetItemData(1,2);
		combo2->SetItemData(2,3);
		combo2->SetCurSel(0);
	}
	else
	{
		combo2->AddString(_T("%NULL"));
 		combo2->SetItemData(0,1);
		combo2->SetCurSel(0);
	}
	 
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}
