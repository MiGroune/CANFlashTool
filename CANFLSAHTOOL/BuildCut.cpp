/*----------------------------------------------------------------------------
| File        : buildCut.cpp
| Project     : CANFlashtool 
|
| Description :  buildcut dialog
|-----------------------------------------------------------------------------
| $Author: liaoshaoyan      $Revision:  
| $Data    2014.04.09   
|-----------------------------------------------------------------------------
| Copyright (c) 2014 by liaoshaoyan.  All rights reserved.
|---------------------------------------------------------------------------*/
// BuildCut.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "CANFLSAHTOOL.h"
#include "BuildCut.h"
#include "afxdialogex.h"
#include "RichEditCtrlEx.h"
#include "SpecialType.h"
	CRichEditCtrlEx m_BuildInfor;
// CBuildCut �Ի���

IMPLEMENT_DYNAMIC(CBuildCut, CDialogEx)

CBuildCut::CBuildCut(CWnd* pParent /*=NULL*/)
	: CDialogEx(CBuildCut::IDD, pParent)
{

}

CBuildCut::~CBuildCut()
{
}

void CBuildCut::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RICHEDIT_BUILD, m_BuildInfor);
}


BEGIN_MESSAGE_MAP(CBuildCut, CDialogEx)
END_MESSAGE_MAP()


// CBuildCut ��Ϣ�������


BOOL CBuildCut::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	CString str;

	//str = "\r\n";
	 
	str += "NOTE:\r\n";
	
	if(flag == CUT)
	{
		str +="1.CUT/PTP File: ";
		str +=calstr;
		str+="\r\n";
	}
	else if(flag == PTP_CAL)
	{
		str +="1.APP File: ";
		str +=appstr;
		str+="\r\n";
		str +="  CAL File: ";
		str +=calstr;
		str+="\r\n";
	}
	if(mixflag == 0)
	{
	str +="2.Set the submenu 'Special Action Editor'.\r\n";
	str +="3.Build cut file\r\n";
	}else
	{
		str +="2.Build cut file\r\n";
	}
	str +="\r\nDo you want to build cut and save cut file press OK.\r\n";
	str +="Otherwise,press CANCEL.";
	str +="\r\nRe-select the cut file you build to flash.\r\n";
	//str+="  If you want to flash ,you have to choose the cut file to flash.";

	//����cut�ļ��˵����á�

	CRect rect1;
	CPaintDC dc1(GetDlgItem(IDC_RICHEDIT_BUILD));
	m_BuildInfor.GetClientRect(rect1);
	CHARFORMAT cf;
	m_BuildInfor.UpdateData(TRUE);
	m_BuildInfor.GetDefaultCharFormat(cf);
	cf.crTextColor=RGB(0,0,0);
	m_BuildInfor.AddMessgaeText(str,cf.szFaceName,cf.yHeight/13,0,0,0,cf.crTextColor);
	int i=0;


	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}
