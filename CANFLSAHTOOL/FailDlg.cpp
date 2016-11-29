/*----------------------------------------------------------------------------
| File        : FailDlg.cpp
| Project     : CANFlashtool 
|
| Description : Dialog box is displayed when reprogram failed.
|-----------------------------------------------------------------------------
| $Author: liaoshaoyan      $Revision:  
| $Data    2014.04.09   
|-----------------------------------------------------------------------------
| Copyright (c) 2014 by liaoshaoyan.  All rights reserved.
|---------------------------------------------------------------------------*/
// FailDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "CANFLSAHTOOL.h"
#include "FailDlg.h"
#include "afxdialogex.h"

#include "CAN_protocol.h"
extern CString strBootReason;

//CRichEditCtrlEx m_EditResult;
extern int Flash_Result;
// CFailDlg 对话框

IMPLEMENT_DYNAMIC(CFailDlg, CDialogEx)

CFailDlg::CFailDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CFailDlg::IDD, pParent)
{

}

CFailDlg::~CFailDlg()
{
}

void CFailDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RICHEDIT21, m_EditResult);
}


BEGIN_MESSAGE_MAP(CFailDlg, CDialogEx)
END_MESSAGE_MAP()


// CFailDlg 消息处理程序


BOOL CFailDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CRect rect1;
	CPaintDC dc1(GetDlgItem(IDC_RICHEDIT21));
	m_EditResult.GetClientRect(rect1);
	//dc1.FillSolidRect(rect1,RGB(125,255,200));
	displayResult(Flash_Result);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
//****************************************
//函数名：displayCal(CString buffer)
//功能  ：刷写过程中显示提示信息
//参数  ：CString buffer:将要输出的字符串
//返回值：void
//****************************************
void  CFailDlg::displayResult(int result)
{	
	//m_CalInfor.UpdateData(TRUE);
	CHARFORMAT cf1;
	CString str;
	CString res;
	res.Format(_T("%x"),result);
	//result = CAN_RESULT_NO_DEVICE;
	switch(result)
	{
	case CHECKSUM_ERROR:
		str += " Checksum error";
		break;
	case ECU_INBOOT:
		str = "\r\n\r\n";
		str += "ECU in boot. \r\n";
		str +=strBootReason;
		break;
	case CAN_RESULT_TIMEOUT:
	case XL_ERR_CMD_TIMEOUT:
		str = "\r\n\r\n";
		str += "timeout. \r\n";
		str +="check the wire.\r\n";
		str +="check the Power Supply.";
		break;
	case CAN_RESULT_BUSOFF_ERROR:
	//case CAN_RESULT_NO_DEVICE:
	//	str = "\r\n\r\n";
	//	str += " BUS OFF.\r\n check the CAN card";
	//	break;
	case XL_ERR_PORT_IS_OFFLINE:
		str = "\r\n\r\n";
		str += " PORT is OFFLINE.\r\n check the CAN card";
		break;
	case CAN_RESULT_PASSIVE_ERROR:
		str = "\r\n\r\n";
		str += " BUS PASSIVE.\r\n check the CAN card";
		break;
	case CAN_RESULT_ACTIVE_ERROR:
		str = "\r\n\r\n";
		str += " BUS ACTIVE.\r\n check the CAN card";
		break;
	case CAN_RESULT_FRAME_ERROR:
		str = "\r\n\r\n";
		str += " Error frame detected.\r\n check the CAN card";
		break;
	case CAN2USB_RESULT_NOT_OPEN:
		str = "\r\n\r\n";
		str += " CAN card is not open.\r\n check the CAN card";
		break;
	case CAN_RESULT_ADD_ERROR:
		str = "\r\n\r\n";
		str += " Additional error causes.\r\n check the CAN card";
		break;
	case XL_ERR_QUEUE_IS_FULL:
		str = "\r\n\r\n";
		str += " QUEUE is FULL ACTIVE.\r\n check the CAN card";
		break;
	case XL_ERR_WRONG_PARAMETER:
	case CAN_RESULT_PARAM_ERROR:
		str = "\r\n\r\n";
		str += " PARAMETER ERROR.\r\n check the CAN card";
		break;
	case CAN_RESULT_SYSTEM_ERROR:
		str = "\r\n\r\n";
		str += " SYSTEM ERROR.\r\n check the CAN card";
		break;
	case CAN_RESULT_REV_PARAM_ERROR:
		str = "\r\n\r\n";
		str += " REV PARAM ERROR.\r\n check the CAN card";
		break;
	case FINDDLL_ERROR:
		str = "\r\n\r\n";
		str += " Auto find seed&key dll ERROR.\r\n check the dll file";
		break;

	case Nega_1:
		str += "\r\nCheckSum error.\r\n";
		break;
	case Nega_11:
		str.Format(_T("Negative Response :%x\r\n"),result);
		str += "The server does not support";
		break;
	case Nega_12:
		str.Format(_T("Negative Response :%x\r\n"),result);
		str += "Send if the sub-function parameter in the request message is not supported";
		break;
	case Nega_13:
		str.Format(_T("Negative Response :%x\r\n"),result);
		str += "The length of the message is wrong";
		break;
	case Nega_22:
		str.Format(_T("Negative Response :%x\r\n"),result);
		str += "The code shall be returned if the criteria for the request are not met";
		break;
	case Nega_24:
		str.Format(_T("Negative Response :%x\r\n"),result);
		str += "The 'sendKey'sub-parameter is received without first receiving a 'requestSeed' request";
		break;
	case Nega_31:
		str.Format(_T("Negative Response :%d"),result);
		str += "Request Out of Range";
		break;
	case Nega_33:
		str.Format(_T("Negative Response :%x\r\n"),result);
		str += "Security Access Denied";
		break;
	case Nega_35:
		str.Format(_T("Negative Response :%x\r\n"),result);
		str += "Send if the value of the KEY is not valid for the server";
		break;
	case Nega_36:
		str.Format(_T("Negative Response :%x\r\n"),result);
		str += "Send if too many attempts with invalid KEY values are requested";
		break;
	case Nega_37:
		str.Format(_T("Negative Response :%x\r\n"),result);
		str += "Send if the delay timer is active and a request is transmitted";
		break;
	case Nega_73:
		str.Format(_T("Negative Response :%x\r\n"),result);
		str += "wrong Block Sequence Counter";
		break;
	case Nega_78:
		str.Format(_T("Negative Response :%x\r\n"),result);
		str += "Request Correctly Received - ResponsePending";
		break;
	case CAN_STOP:
		str += "\r\n Communication is STOP\r\n";
		break;

	default :
		 str = "  ERROR.\r\n check the CAN card";
		//str.Format(_T("命令发送失败，错误码：%d。"),result);
		break;
	}
	str+="\r\n";
	//str += res;
	
	m_EditResult.GetDefaultCharFormat(cf1);
	cf1.crTextColor=RGB(255,0,0);
	m_EditResult.AddMessgaeTextQur(str,cf1.szFaceName,cf1.yHeight/14,(bool)(cf1.dwEffects & CFE_BOLD),0,0,cf1.crTextColor);

	
}