/*----------------------------------------------------------------------------
| File        : SuccessFlash.cpp
| Project     : CANFlashtool 
|
| Description : Dialog box is displayed when reprogram successfully.
|-----------------------------------------------------------------------------
| $Author: liaoshaoyan      $Revision:  
| $Data    2014.04.09   
|-----------------------------------------------------------------------------
| Copyright (c) 2014 by liaoshaoyan.  All rights reserved.
|---------------------------------------------------------------------------*/
//  : ʵ���ļ�
//

#include "stdafx.h"
#include "CANFLSAHTOOL.h"
#include "SuccessFlash.h"
#include "afxdialogex.h"
extern bool BootFlag;
extern CString strBootReason;
extern CString strQueryInfor;
// SuccessFlash �Ի���

IMPLEMENT_DYNAMIC(SuccessFlash, CDialogEx)

SuccessFlash::SuccessFlash(CWnd* pParent /*=NULL*/)
	: CDialogEx(SuccessFlash::IDD, pParent)
{

}

SuccessFlash::~SuccessFlash()
{
}

void SuccessFlash::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RICHEDIT_SUCCESS, m_EditResult);
}


BEGIN_MESSAGE_MAP(SuccessFlash, CDialogEx)
END_MESSAGE_MAP()


// SuccessFlash ��Ϣ�������


BOOL SuccessFlash::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
 	CHARFORMAT cf1;
	CString str;
 	//str =("NOTICE:\r\nFirst,turn off the car keys,then click OK and wait 15 seconds");
 	str =("NOTICE:\r\nKey off and fully power down for 10 seconds\n");
	str+=strQueryInfor;
 	CRect rect1;
	CPaintDC dc1(GetDlgItem(IDC_RICHEDIT_SUCCESS));
	m_EditResult.GetClientRect(rect1);
	//dc1.FillSolidRect(rect1,RGB(125,255,200));
 
	m_EditResult.GetDefaultCharFormat(cf1);
	//cf1.crTextColor=RGB(255,0,0);
	//m_EditResult.AddMessgaeTextQur(str,cf1.szFaceName,cf1.yHeight/12,(bool)(cf1.dwEffects & CFE_BOLD),(bool)(cf1.dwEffects & CFE_ITALIC),(bool)(cf1.dwEffects & CFE_UNDERLINE),cf1.crTextColor);
	m_EditResult.AddMessgaeText(str,cf1.szFaceName,cf1.yHeight/12,(bool)(cf1.dwEffects & CFE_BOLD), 0,0,cf1.crTextColor);
 m_EditResult.UpdateData(TRUE);
	m_EditResult.GetDefaultCharFormat(cf1);

 	if(BootFlag == 1)
	{
 	 
 	cf1.crTextColor=RGB(255,0,0);
	m_EditResult.AddMessgaeTextErr(strBootReason,cf1.szFaceName,cf1.yHeight/12,(bool)(cf1.dwEffects & CFE_BOLD),0,0,cf1.crTextColor);
	}
	else
	{
	m_EditResult.AddMessgaeText(strBootReason,cf1.szFaceName,cf1.yHeight/12,0,0,0,cf1.crTextColor);

	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}
