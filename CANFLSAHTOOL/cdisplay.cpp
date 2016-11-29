// cdisplay.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "CANFLSAHTOOL.h"
#include "cdisplay.h"
#include "afxdialogex.h"
#include "Global.h"

// cdisplay �Ի���

IMPLEMENT_DYNAMIC(cdisplay, CDialogEx)

cdisplay::cdisplay(CWnd* pParent /*=NULL*/)
	: CDialogEx(cdisplay::IDD, pParent)
{

}

cdisplay::~cdisplay()
{
}

void cdisplay::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RICHEDIT_DISPLAY, m_Editdisplay);
	
}


BEGIN_MESSAGE_MAP(cdisplay, CDialogEx)
	ON_MESSAGE( WM_USER+1,onbreak)
END_MESSAGE_MAP()


// cdisplay ��Ϣ�������


BOOL cdisplay::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	CRect rect1;
	CPaintDC dc1(GetDlgItem(IDC_RICHEDIT_DISPLAY));
	m_Editdisplay.GetClientRect(rect1);
 
	 DisplayThread = ::CreateThread(NULL,
									NULL,
									(LPTHREAD_START_ROUTINE)CDisplayThread,
									(LPVOID)this,
									NULL,
									NULL);
	CloseHandle(DisplayThread);
 
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}
/*displaystreamnum��ÿ��ˢд����ʾǰ��0����Ϊ��ǰ��ʾ��message����������ʾ��������200000����ʾҳ�����*/
DWORD WINAPI cdisplay::CDisplayThread(LPVOID lpParameter)
{
	cdisplay	*dlg=(cdisplay*)lpParameter;
	static unsigned int lognum=0;
	CString str;
	while(Display_flag!=0)
	{
  		WaitForSingleObject(displayMsgMutex,INFINITE);//
		str.Empty();
		for(;displaystreamnum<gserialNo;displaystreamnum++)
		{
			if(Display_flag==0)break;
			if(streamArray[displaystreamnum]->MsgState == MSGIN)str+="<=   ";
			else str+="=>   ";
			str+=streamArray[displaystreamnum]->data;
  			lognum++;
		}
		ReleaseMutex(displayMsgMutex);
		if(Display_flag==0)break;
		if(str!="")
		{
			if(lognum>200000){m_Editdisplay.SetWindowText(_T("")); lognum=0;}
 			CHARFORMAT cf;
			m_Editdisplay.GetDefaultCharFormat(cf);
			m_Editdisplay.AddMessgaeText(str,cf.szFaceName,cf.yHeight/15,(bool)(cf.dwEffects & CFE_BOLD),
			0,0,cf.crTextColor);
		}			
	}
	::SendMessage(dlg->m_hWnd,WM_USER+1,0,1);	 
	return 0;

}
void cdisplay::OnCancel() //����OnCancel()
{	 
	Display_flag=0;
}
LRESULT cdisplay::onbreak(WPARAM flag, LPARAM lparam)
{
	CDialog::OnCancel();
	return 1;
}