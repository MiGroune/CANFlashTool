/*----------------------------------------------------------------------------
| File        : ActionSetting.cpp
| Project     : CANFlashtool 
|
| Description :  edit special action items
|-----------------------------------------------------------------------------
| $Author: liaoshaoyan      $Revision:  
| $Data    2014.04.09   
|-----------------------------------------------------------------------------
| Copyright (c) 2014 by liaoshaoyan.  All rights reserved.
|---------------------------------------------------------------------------*/
// ActionSetting.cpp : 实现文件
//

#include "stdafx.h"
#include "CANFLSAHTOOL.h"
#include "ActionSetting.h"
#include "afxdialogex.h"
#include "Global.h"

// CActionSetting 对话框

IMPLEMENT_DYNAMIC(CActionSetting, CDialogEx)

CActionSetting::CActionSetting(CWnd* pParent /*=NULL*/)
	: CDialogEx(CActionSetting::IDD, pParent)
{
	CheckItem = NULL;
}

CActionSetting::~CActionSetting()
{
}

void CActionSetting::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_SUBADD, m_ItemAdd);
	DDX_Control(pDX, IDC_BUTTON_SUBDELETE, m_ItemDelete);
	DDX_Control(pDX, IDC_BUTTON_SUBEDIT, m_ItemEdit);
	DDX_Control(pDX, IDC_LIST_SubItem, m_subItemList);
}


BEGIN_MESSAGE_MAP(CActionSetting, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_SUBADD, &CActionSetting::OnBnClickedButtonSubadd)
	ON_BN_CLICKED(IDC_BUTTON_SUBEDIT, &CActionSetting::OnBnClickedButtonSubedit)
	ON_BN_CLICKED(IDC_BUTTON_SUBDELETE, &CActionSetting::OnBnClickedButtonSubdelete)
END_MESSAGE_MAP()


// CActionSetting 消息处理程序


BOOL CActionSetting::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_subItemList.SetExtendedStyle(LVS_EX_GRIDLINES);
	m_subItemList.InsertColumn(0,_T("Label"),LVCFMT_CENTER,80);
	m_subItemList.InsertColumn(1,_T("Aaction Type"),LVCFMT_CENTER,100);
	m_subItemList.InsertColumn(2,_T("Value"),LVCFMT_CENTER,100);
	m_subItemList.InsertColumn(3,_T("Address"),LVCFMT_CENTER,80);
	m_subItemList.InsertColumn(4,_T("Size"),LVCFMT_CENTER,50);
	m_subItemList.InsertColumn(5,_T("Comment"),LVCFMT_CENTER,80);
 
	 ListView_SetExtendedListViewStyleEx(m_subItemList.m_hWnd, LVS_EX_FULLROWSELECT | LVS_REPORT, 
																	LVS_EX_FULLROWSELECT | LVS_REPORT);
	 CString st=_T("0");
	 for(int i=0;i<ItemInfor.ItemNum;i++)
	 {
		m_subItemList.InsertItem(i,st);
		m_subItemList.SetItemText(i,0,ItemInfor.ActionItem[i].Label);
		m_subItemList.SetItemText(i,1,ItemInfor.ActionItem[i].ActionType);
		m_subItemList.SetItemText(i,2,ItemInfor.ActionItem[i].Value);
		m_subItemList.SetItemText(i,3,ItemInfor.ActionItem[i].Address);
		m_subItemList.SetItemText(i,4,ItemInfor.ActionItem[i].Size);
	 }

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CActionSetting::OnBnClickedButtonSubadd()
{
	// TODO: 在此添加控件通知处理程序代码
	if(CheckItem == NULL)CheckItem=new CCheckItem();
	CheckItem->m_Item_Label   = "";
	CheckItem->m_Item_Type    = "";
	CheckItem->m_Item_Value   = "";
	CheckItem->m_Item_Address = "";
	CheckItem->m_Item_Size    = "";
	int nResponse = CheckItem->DoModal();
	if(nResponse == IDCANCEL)
	{
		return;
	}
	CString st=_T("0");
	 
	int i=ItemInfor.ItemNum++;
	ItemInfor.ActionItem[i].Label=CheckItem->m_Item_Label;
	ItemInfor.ActionItem[i].Address=CheckItem->m_Item_Address;
	ItemInfor.ActionItem[i].ActionType=CheckItem->m_Item_Type;
	ItemInfor.ActionItem[i].Size=CheckItem->m_Item_Size;
	ItemInfor.ActionItem[i].Value=CheckItem->m_Item_Value;
	ItemInfor.ActionItem[i].Comment=CheckItem->m_Item_Comment;
	m_subItemList.InsertItem(i,st);
	m_subItemList.SetItemText(i,0,ItemInfor.ActionItem[i].Label);
	m_subItemList.SetItemText(i,1,ItemInfor.ActionItem[i].ActionType);
	m_subItemList.SetItemText(i,2,ItemInfor.ActionItem[i].Value);
	m_subItemList.SetItemText(i,3,ItemInfor.ActionItem[i].Address);
	m_subItemList.SetItemText(i,4,ItemInfor.ActionItem[i].Size);
	m_subItemList.SetItemText(i,5,ItemInfor.ActionItem[i].Comment);
	
	UpdateData(false);

}


void CActionSetting::OnBnClickedButtonSubedit()
{
	// TODO: 在此添加控件通知处理程序代码
	int nItem;
	CString str;
	POSITION pos = m_subItemList.GetFirstSelectedItemPosition();
	if(pos)
	{
		nItem = m_subItemList.GetNextSelectedItem(pos);
	
		if(CheckItem == NULL)CheckItem=new CCheckItem();
		CheckItem->m_Item_Label   = m_subItemList.GetItemText(nItem,0);
		CheckItem->m_Item_Type    = m_subItemList.GetItemText(nItem,1);
		CheckItem->m_Item_Value   = m_subItemList.GetItemText(nItem,2);	
		CheckItem->m_Item_Address = m_subItemList.GetItemText(nItem,3);
		CheckItem->m_Item_Size    = m_subItemList.GetItemText(nItem,4);

		int nResponse = CheckItem->DoModal();
		if(nResponse == IDCANCEL)
		{
			return;
		}
		CString st=_T("0");
			int i=nItem;
		ItemInfor.ActionItem[i].Label=CheckItem->m_Item_Label;
		ItemInfor.ActionItem[i].Address=CheckItem->m_Item_Address;
		ItemInfor.ActionItem[i].ActionType=CheckItem->m_Item_Type;
		ItemInfor.ActionItem[i].Size=CheckItem->m_Item_Size;
		ItemInfor.ActionItem[i].Value=CheckItem->m_Item_Value;
		ItemInfor.ActionItem[i].Comment=CheckItem->m_Item_Comment;
	 
		m_subItemList.SetItemText(i,0,ItemInfor.ActionItem[i].Label);
		m_subItemList.SetItemText(i,1,ItemInfor.ActionItem[i].ActionType);
		m_subItemList.SetItemText(i,2,ItemInfor.ActionItem[i].Value);
		m_subItemList.SetItemText(i,3,ItemInfor.ActionItem[i].Address);
		m_subItemList.SetItemText(i,4,ItemInfor.ActionItem[i].Size);
		m_subItemList.SetItemText(i,5,ItemInfor.ActionItem[i].Comment); 
	}

}


void CActionSetting::OnBnClickedButtonSubdelete()
{
	// TODO: 在此添加控件通知处理程序代码
	int nItem;
	POSITION pos = m_subItemList.GetFirstSelectedItemPosition();
	while(pos)
	{
		nItem = m_subItemList.GetNextSelectedItem(pos);
		m_subItemList.DeleteItem(nItem);

		pos = m_subItemList.GetFirstSelectedItemPosition();
	}
	
	for(int i=nItem;i<ItemInfor.ItemNum;i++)
	{
		ItemInfor.ActionItem[i].Label      =ItemInfor.ActionItem[i+1].Label;
		ItemInfor.ActionItem[i].Address    =ItemInfor.ActionItem[i+1].Address;
		ItemInfor.ActionItem[i].ActionType =ItemInfor.ActionItem[i+1].ActionType;
		ItemInfor.ActionItem[i].Size       =ItemInfor.ActionItem[i+1].Size;
		ItemInfor.ActionItem[i].Value      =ItemInfor.ActionItem[i+1].Value;
		ItemInfor.ActionItem[i].Comment    =ItemInfor.ActionItem[i+1].Comment;
	}
	ItemInfor.ItemNum--;
}
