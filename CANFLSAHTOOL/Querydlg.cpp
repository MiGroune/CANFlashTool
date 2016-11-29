// Querydlg.cpp : ʵ���ļ�
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
// Querydlg �Ի���

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


// Querydlg ��Ϣ�������


BOOL Querydlg::OnInitDialog()
{

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
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
	// �쳣: OCX ����ҳӦ���� FALSE
}
void Querydlg::OnPaint()
{
	//CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CDialogEx::OnPaint()
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
//��������displayquery()
//����  ��ˢд��������ʾ��ʾ��Ϣ
//����  ����
//����ֵ��void
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
//��������displayCal(CString buffer)
//����  ��ˢд��������ʾ��ʾ��Ϣ
//����  ��CString buffer:��Ҫ������ַ���
//����ֵ��void
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

