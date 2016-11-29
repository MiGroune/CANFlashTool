// Querydlg.cpp : 实现文件
//

#include "stdafx.h"
#include "CANFLSAHTOOL.h"
#include "Querydlg.h"
#include "afxdialogex.h"

#include "RichEditCtrlEx.h"
 
extern	CString strQueryInfor;
extern CWnd *m_pWnd;
extern CString gItem;
CRichEditCtrlEx m_CalInfor;
CRichEditCtrlEx m_information;
CRichEditCtrlEx m_calinfor;
extern bool BootFlag;
extern CString Curstr;
extern CString strBootReason;
int i=0;
// Querydlg 对话框

IMPLEMENT_DYNAMIC(Querydlg, CDialogEx)

Querydlg::Querydlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(Querydlg::IDD, pParent)
{

}

Querydlg::~Querydlg()
{
}

void Querydlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RICHEDIT_CALINFOR, m_CalInfor);
	DDX_Control(pDX, IDC_RICHEDIT_QUERY, m_information);
	DDX_Control(pDX, IDC_RICHEDIT24, m_calinfor);
}


BEGIN_MESSAGE_MAP(Querydlg, CDialogEx)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// Querydlg 消息处理程序


BOOL Querydlg::OnInitDialog()
{

	// TODO:  在此添加额外的初始化
	CDialogEx::OnInitDialog();
	i=0;
	CRect rect1;
	CPaintDC dc1(GetDlgItem(IDC_RICHEDIT_QUERY));
	m_information.GetClientRect(rect1);
	displayquery();
 	CRect rect2;
	CPaintDC dc2(GetDlgItem(IDC_RICHEDIT_CALINFOR));
	m_CalInfor.GetClientRect(rect2);
	dc2.FillSolidRect(rect1,RGB(125,255,200));
	displayCal(strQueryInfor);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
void Querydlg::OnPaint()
{
	//CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialogEx::OnPaint()
	if(IsIconic())
	{
		CPaintDC dc(this); // device context for painting
	}
	else
	{
	
		CDialogEx::OnPaint();
	}
}
//****************************************
//函数名：displayquery()
//功能  ：刷写过程中显示提示信息
//参数  ：无
//返回值：void
//****************************************
void Querydlg::displayquery()
{
	CHARFORMAT cf;
	CString str;
	m_information.GetDefaultCharFormat(cf);
	//str = "\r\n";
	str += "  Please stop the engine.\r\n";
	str += "  Keep the power on.\r\n";
	str += "  Begin to flash press YES.\r\n";
	str += "  Otherwise,press NO.";
	cf.crTextColor=RGB(128,128,0);
 	m_information.AddMessgaeTextColor(str,cf.szFaceName,cf.yHeight/12,(bool)(cf.dwEffects & CFE_BOLD),0,0,cf.crTextColor);
	int i=0;
	
}
//****************************************
//函数名：displayCal(CString buffer)
//功能  ：刷写过程中显示提示信息
//参数  ：CString buffer:将要输出的字符串
//返回值：void
//****************************************
void  Querydlg::displayCal(CString buffer)
{	
	//m_CalInfor.UpdateData(TRUE);
	CHARFORMAT cf;
	CString str;
	m_CalInfor.UpdateData(TRUE);
	m_CalInfor.GetDefaultCharFormat(cf);

	m_CalInfor.AddMessgaeText(buffer,cf.szFaceName,cf.yHeight/12,(bool)(cf.dwEffects & CFE_BOLD),0,0,cf.crTextColor);
 //	str = "Calibration Information:";
	//m_calinfor.GetDefaultCharFormat(cf1);
	//m_calinfor.AddMessgaeTextQur(str,cf1.szFaceName,cf1.yHeight/12,(bool)(cf1.dwEffects & CFE_BOLD),(bool)(cf1.dwEffects & CFE_ITALIC),(bool)(cf1.dwEffects & CFE_UNDERLINE),cf1.crTextColor);
		m_CalInfor.UpdateData(TRUE);
	m_CalInfor.GetDefaultCharFormat(cf);
	

 	if(BootFlag == 1)
	{
 	 
 		cf.crTextColor=RGB(255,0,0);
		m_CalInfor.AddMessgaeTextErr(strBootReason,cf.szFaceName,cf.yHeight/12,(bool)(cf.dwEffects & CFE_BOLD),0,0,cf.crTextColor);
	}
	else
	{
		m_CalInfor.AddMessgaeText(strBootReason,cf.szFaceName,cf.yHeight/12,(bool)(cf.dwEffects & CFE_BOLD),0,0,cf.crTextColor);

	}
		if(Curstr.Find(_T("incorrect")) != -1)
	{
 		cf.crTextColor=RGB(255,0,0);
		//m_CalInfor.AddMessgaeTextErr(Curstr,cf.szFaceName,cf.yHeight/12,(bool)(cf.dwEffects & CFE_BOLD),0,0,cf.crTextColor);
		str=_T("\r\nChange the checksum by build cut first.\nDo you want to continue without building cut?");
		m_CalInfor.AddMessgaeTextErr(str,cf.szFaceName,cf.yHeight/12,(bool)(cf.dwEffects & CFE_BOLD),0,0,cf.crTextColor);
	}
	else
	{
		//m_CalInfor.AddMessgaeText(Curstr,cf.szFaceName,cf.yHeight/12,(bool)(cf.dwEffects & CFE_BOLD),0,0,cf.crTextColor);

	}
	//PARAFORMAT2  cf3;
 //   cf3.cbSize=sizeof(cf);  
 //   cf3.dwMask=PFM_LINESPACING; 
 //   cf3.dyLineSpacing=40;
 //   m_CalInfor.SetParaFormat(cf3);

}

