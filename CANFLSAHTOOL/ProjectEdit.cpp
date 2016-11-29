// ProjectEdit.cpp : 实现文件
//

#include "stdafx.h"
#include "CANFLSAHTOOL.h"
#include "ProjectEdit.h"
#include "afxdialogex.h"
#include "Global.h"

// CProjectEdit 对话框

IMPLEMENT_DYNAMIC(CProjectEdit, CDialogEx)

CProjectEdit::CProjectEdit(CWnd* pParent /*=NULL*/)
	: CDialogEx(CProjectEdit::IDD, pParent)
	, m_szParamValue(_T(""))
{
	m_pConfig = NULL;

}

CProjectEdit::~CProjectEdit()
{
}

void CProjectEdit::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_KEYVALUE, m_szKeyValue);
	DDX_Text(pDX, IDC_EDIT_PARAM, m_szParamValue);
}


BEGIN_MESSAGE_MAP(CProjectEdit, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_PRG, &CProjectEdit::OnBnClickedButtonPrg)
	ON_BN_CLICKED(IDC_CHECK_KEYNEED, &CProjectEdit::OnBnClickedCheckKeyneed)
	ON_BN_CLICKED(IDC_CHECK_KEYMANUAL, &CProjectEdit::OnBnClickedCheckKeymanual)
	ON_BN_CLICKED(IDOK, &CProjectEdit::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_KEYPATH, &CProjectEdit::OnBnClickedButtonKeypath)
	ON_BN_CLICKED(IDC_CHECK_RUNBEFORE, &CProjectEdit::OnBnClickedCheckRunbefore)
	ON_BN_CLICKED(IDC_CHECK_AFTER, &CProjectEdit::OnBnClickedCheckAfter)
	ON_BN_CLICKED(IDC_BUTTON_BEFORE, &CProjectEdit::OnBnClickedButtonBefore)
	ON_BN_CLICKED(IDC_BUTTON_AFTER, &CProjectEdit::OnBnClickedButtonAfter)
	ON_BN_CLICKED(IDCANCEL, &CProjectEdit::OnBnClickedCancel)
 
	ON_BN_CLICKED(IDC_CHECK_DEFAULT, &CProjectEdit::OnBnClickedCheckDefault)
	ON_BN_CLICKED(IDC_CHECK_KEYPARAM, &CProjectEdit::OnBnClickedCheckKeyparam)
	ON_BN_CLICKED(IDC_CHECK_KEYAUTO, &CProjectEdit::OnBnClickedCheckKeyauto)
	ON_BN_CLICKED(IDC_CHECK_CHECKSUM, &CProjectEdit::OnBnClickedCheckChecksum)
	ON_BN_CLICKED(IDC_CHECK_ENABLESTOP, &CProjectEdit::OnBnClickedCheckEnablestop)
	ON_BN_CLICKED(IDC_CHECK_BYPASS, &CProjectEdit::OnBnClickedCheckBypass)
END_MESSAGE_MAP()


// CProjectEdit 消息处理程序


BOOL CProjectEdit::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	if(pParent->m_bCurrentPro == 0)//new
	{
		m_szConfigPath.Empty();
		GetDlgItem(IDC_CHECK_KEYMANUAL)->EnableWindow(false);
		GetDlgItem(IDC_EDIT_KEYVALUE)->EnableWindow(false);
		GetDlgItem(IDC_EDIT_KEYPATH)->EnableWindow(false);
		GetDlgItem(IDC_BUTTON_KEYPATH)->EnableWindow(false);		 
		GetDlgItem(IDC_EDIT_BEFORE)->EnableWindow(false);
		GetDlgItem(IDC_BUTTON_BEFORE)->EnableWindow(false);
 		GetDlgItem(IDC_EDIT_AFTER)->EnableWindow(false);
		GetDlgItem(IDC_BUTTON_AFTER)->EnableWindow(false);
		GetDlgItem(IDC_EDIT_PARAM)->EnableWindow(false);
		
		m_bChecksumFlag=0;
		m_bEnableStopFlag=0;
	 	m_bBeforeFlag=0;
		m_bAfterFlag=0;
		m_bDefaultFlag=1;
		m_nKeyFlag=0;
		m_bBypassFlag=0;
		m_bSetParamFlag=0;
		m_szKeyPath.Empty();
		g_nKeyValue = 0;
		g_nParamValue=1;
  	}
	else
	{
		m_szConfigPath = pParent->m_szConfigPath;
		GetDlgItem(IDC_EDIT_PRG)->SetWindowTextW(m_szConfigPath);
		m_szBeforePath = pParent->m_szBeforePath;
		m_szAfterPath = pParent->m_szAfterPath;
		GetDlgItem(IDC_EDIT_BEFORE)->SetWindowTextW(m_szBeforePath);
		GetDlgItem(IDC_EDIT_AFTER)->SetWindowTextW(m_szAfterPath);
		m_bAfterFlag= pParent->m_bAfterFlag;
		m_bBeforeFlag= pParent->m_bBeforeFlag;
		m_bDefaultFlag=pParent->m_bDefaultFlag;
		m_bBypassFlag = pParent->m_bBypassFlag;
		m_bSetParamFlag=g_bSetParamFlag;
		m_nKeyFlag=g_nKeyFlag;
		m_szKeyPath=g_szKeyPath;
		m_bChecksumFlag=pParent->m_bChecksumFlag;
		m_bEnableStopFlag=pParent->m_bEnableStopFlag;
  	}

	DisplaySecurity();
 	// TODO:  在此添加额外的初始化

	if(m_bBeforeFlag== 1)
	{
		((CButton *)GetDlgItem(IDC_CHECK_RUNBEFORE))->SetCheck(1);
		GetDlgItem(IDC_EDIT_BEFORE)->EnableWindow(true);
		GetDlgItem(IDC_BUTTON_BEFORE)->EnableWindow(true);

	}
	else
	{
		((CButton *)GetDlgItem(IDC_CHECK_RUNBEFORE))->SetCheck(0);
			GetDlgItem(IDC_EDIT_BEFORE)->EnableWindow(false);
		GetDlgItem(IDC_BUTTON_BEFORE)->EnableWindow(false);

	}
	if(m_bAfterFlag == 1)
	{
		((CButton *)GetDlgItem(IDC_CHECK_AFTER))->SetCheck(1);
				GetDlgItem(IDC_EDIT_AFTER)->EnableWindow(true);
		GetDlgItem(IDC_BUTTON_AFTER)->EnableWindow(true);

	}
	else
	{
		((CButton *)GetDlgItem(IDC_CHECK_AFTER))->SetCheck(0);
			GetDlgItem(IDC_EDIT_AFTER)->EnableWindow(false);
		GetDlgItem(IDC_BUTTON_AFTER)->EnableWindow(false);

	}
 	if(m_bDefaultFlag == 1)
	{
		((CButton *)GetDlgItem(IDC_CHECK_DEFAULT))->SetCheck(1);
 	}
	else
	{
		((CButton *)GetDlgItem(IDC_CHECK_DEFAULT))->SetCheck(0);
		//g_nParamValue=1;
 	}
 	if(m_bBypassFlag == 1)
	{
		((CButton *)GetDlgItem(IDC_CHECK_BYPASS))->SetCheck(1);
 	}
	else
	{
		((CButton *)GetDlgItem(IDC_CHECK_BYPASS))->SetCheck(0);
		//g_nParamValue=1;
 	} 	if(m_bChecksumFlag == 1)
	{
		((CButton *)GetDlgItem(IDC_CHECK_CHECKSUM))->SetCheck(1);
 	}
	else
	{
		((CButton *)GetDlgItem(IDC_CHECK_CHECKSUM))->SetCheck(0);
		//g_nParamValue=1;
 	}
 	if(m_bEnableStopFlag == 1)
	{
		((CButton *)GetDlgItem(IDC_CHECK_ENABLESTOP))->SetCheck(1);
 	}
	else
	{
		((CButton *)GetDlgItem(IDC_CHECK_ENABLESTOP))->SetCheck(0);
		//g_nParamValue=1;
 	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
 
void CProjectEdit::DisplaySecurity()
{
	if(m_nKeyFlag == 1)
	{
		((CButton *)GetDlgItem(IDC_CHECK_KEYMANUAL))->SetCheck(1);
		((CButton *)GetDlgItem(IDC_CHECK_KEYNEED))->SetCheck(1);
		GetDlgItem(IDC_CHECK_KEYMANUAL)->EnableWindow(true);
		GetDlgItem(IDC_EDIT_KEYVALUE)->EnableWindow(true);
		GetDlgItem(IDC_EDIT_KEYPATH)->EnableWindow(false);
		GetDlgItem(IDC_BUTTON_KEYPATH)->EnableWindow(false);
	}
	else if(m_nKeyFlag == 2)
	{
		((CButton *)GetDlgItem(IDC_CHECK_KEYNEED))->SetCheck(1);
		((CButton *)GetDlgItem(IDC_CHECK_KEYMANUAL))->SetCheck(0);
		GetDlgItem(IDC_EDIT_KEYVALUE)->EnableWindow(false);
		GetDlgItem(IDC_EDIT_KEYPATH)->EnableWindow(true);
		GetDlgItem(IDC_BUTTON_KEYPATH)->EnableWindow(true);
	}
	else
	{
		//m_szConfigPath.Empty();
		GetDlgItem(IDC_CHECK_KEYMANUAL)->EnableWindow(false);
		GetDlgItem(IDC_EDIT_KEYVALUE)->EnableWindow(false);
		GetDlgItem(IDC_EDIT_KEYPATH)->EnableWindow(false);
		GetDlgItem(IDC_BUTTON_KEYPATH)->EnableWindow(false);
 

	}
	GetDlgItem(IDC_EDIT_KEYPATH)->SetWindowTextW(m_szKeyPath);
	CString str;
	str.Format(_T("%X"),g_nKeyValue);
	GetDlgItem(IDC_EDIT_KEYVALUE)->SetWindowTextW(str);
 	str.Format(_T("%02X"),g_nParamValue);
	GetDlgItem(IDC_EDIT_PARAM)->SetWindowTextW(str);
	if(m_bSetParamFlag == 1)
	{
		((CButton *)GetDlgItem(IDC_CHECK_KEYPARAM))->SetCheck(1);
		 GetDlgItem(IDC_EDIT_PARAM)->EnableWindow(true);

 	}
	else
	{
		GetDlgItem(IDC_EDIT_PARAM)->EnableWindow(false);
		 ((CButton *)GetDlgItem(IDC_CHECK_KEYPARAM))->SetCheck(0);

 	}
	if(g_nKeyAutoFlag == 1)
	{
		((CButton *)GetDlgItem(IDC_CHECK_KEYAUTO))->SetCheck(1);
 
 	}
	else
	{
		((CButton *)GetDlgItem(IDC_CHECK_KEYAUTO))->SetCheck(0);
 
 	}
}
void CProjectEdit::OnBnClickedButtonPrg()
{
	// TODO: 在此添加控件通知处理程序代码
	 
	 	 
	LPCTSTR Types ;
	Types = 
			_T("PRG File (*.PRG)|*.PRG|")
			_T("All Files (*.*)|*.*||");
	CFileDialog dlg(TRUE,_T("bin"),NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,Types );
		dlg.m_ofn.lpstrTitle =_T("Open File");

	if(dlg.DoModal()==IDCANCEL)return;
		
	m_szConfigPath = dlg.GetPathName();
	GetDlgItem(IDC_EDIT_PRG)->SetWindowTextW(m_szConfigPath);
 
   
}

void CProjectEdit::OnBnClickedCheckKeyneed()
{
	// TODO: 在此添加控件通知处理程序代码
	if( ((CButton *)(GetDlgItem(IDC_CHECK_KEYNEED)))->GetCheck() )
	{
		GetDlgItem(IDC_CHECK_KEYMANUAL)->EnableWindow(true);
		GetDlgItem(IDC_EDIT_KEYVALUE)->EnableWindow(true);
		GetDlgItem(IDC_EDIT_KEYPATH)->EnableWindow(false);
		GetDlgItem(IDC_BUTTON_KEYPATH)->EnableWindow(false);
		((CButton *)GetDlgItem(IDC_CHECK_KEYMANUAL))->SetCheck(1);
		((CButton *)(GetDlgItem(IDC_CHECK_KEYAUTO)))->SetCheck(0);
		g_nKeyAutoFlag=0;
		m_nKeyFlag=1;
 	}
	else
	{
		GetDlgItem(IDC_EDIT_KEYPATH)->EnableWindow(false);
		GetDlgItem(IDC_BUTTON_KEYPATH)->EnableWindow(false);
		GetDlgItem(IDC_EDIT_KEYVALUE)->EnableWindow(false);
		GetDlgItem(IDC_CHECK_KEYMANUAL)->EnableWindow(false);
		m_nKeyFlag=0;
 	}
}


void CProjectEdit::OnBnClickedCheckKeymanual()
{
	// TODO: 在此添加控件通知处理程序代码

	if( ((CButton *)(GetDlgItem(IDC_CHECK_KEYMANUAL)))->GetCheck() )
	{
		GetDlgItem(IDC_CHECK_KEYMANUAL)->EnableWindow(true);
		GetDlgItem(IDC_EDIT_KEYVALUE)->EnableWindow(true);
		GetDlgItem(IDC_EDIT_KEYPATH)->EnableWindow(false);
		GetDlgItem(IDC_BUTTON_KEYPATH)->EnableWindow(false);
		m_nKeyFlag=1;
 	}
	else
	{
		GetDlgItem(IDC_EDIT_KEYPATH)->EnableWindow(true);
		GetDlgItem(IDC_BUTTON_KEYPATH)->EnableWindow(true);
		GetDlgItem(IDC_EDIT_KEYVALUE)->EnableWindow(false);
		m_nKeyFlag=2;
 	}
}


void CProjectEdit::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
  	int result=0;		
	result = config->Read_Prg(m_szConfigPath);
	if(result == -1)
	{
		m_szConfigPath="";
		MessageBox(_T("config file open ERROR."),_T("ERROR"),MB_ICONERROR|MB_OK);
	}
	else CDialogEx::OnOK();
}


void CProjectEdit::OnBnClickedButtonKeypath()
{
	// TODO: 在此添加控件通知处理程序代码
		LPCTSTR Types ;
	Types = 
			_T("PRG File (*.DLL)|*.dll|")
			_T("All Files (*.*)|*.*||");
	CFileDialog dlg(TRUE,_T("bin"),NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,Types );
		dlg.m_ofn.lpstrTitle =_T("Open File");

	if(dlg.DoModal()==IDCANCEL)return;
		
	m_szKeyPath = dlg.GetPathName();
	GetDlgItem(IDC_EDIT_KEYPATH)->SetWindowTextW(m_szKeyPath);

}


void CProjectEdit::OnBnClickedCheckRunbefore()
{
	// TODO: 在此添加控件通知处理程序代码
	if( ((CButton *)(GetDlgItem(IDC_CHECK_RUNBEFORE)))->GetCheck() )
	{
		m_bBeforeFlag  = 1;
		GetDlgItem(IDC_EDIT_BEFORE)->EnableWindow(true);
		GetDlgItem(IDC_BUTTON_BEFORE)->EnableWindow(true);
	}
	else
	{
		m_bBeforeFlag  = 0;
		GetDlgItem(IDC_EDIT_BEFORE)->EnableWindow(false);
		GetDlgItem(IDC_BUTTON_BEFORE)->EnableWindow(false);

	}
		 

}


void CProjectEdit::OnBnClickedCheckAfter()
{
	// TODO: 在此添加控件通知处理程序代码
 		if( ((CButton *)(GetDlgItem(IDC_CHECK_AFTER)))->GetCheck() )
		{
			m_bAfterFlag = 1;
			GetDlgItem(IDC_EDIT_AFTER)->EnableWindow(true);
		GetDlgItem(IDC_BUTTON_AFTER)->EnableWindow(true);
		}
		else
		{
			m_bAfterFlag = 0;
			GetDlgItem(IDC_EDIT_AFTER)->EnableWindow(false);
		GetDlgItem(IDC_BUTTON_AFTER)->EnableWindow(false);


		}
}


void CProjectEdit::OnBnClickedButtonBefore()
{
	// TODO: 在此添加控件通知处理程序代码
			LPCTSTR Types ;
	Types = 
			_T("PRG File (*.cms)|*.cms|")
			_T("All Files (*.*)|*.*||");
	CFileDialog dlg(TRUE,_T("txt"),NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,Types );
		dlg.m_ofn.lpstrTitle =_T("Open File");

	if(dlg.DoModal()==IDCANCEL)return;
		
	m_szBeforePath = dlg.GetPathName();
	GetDlgItem(IDC_EDIT_BEFORE)->SetWindowTextW(m_szBeforePath);
}


void CProjectEdit::OnBnClickedButtonAfter()
{
	// TODO: 在此添加控件通知处理程序代码
				LPCTSTR Types ;
	Types = 
			_T("PRG File (*.cms)|*.cms|")
			_T("All Files (*.*)|*.*||");
	CFileDialog dlg(TRUE,_T("txt"),NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,Types );
		dlg.m_ofn.lpstrTitle =_T("Open File");

	if(dlg.DoModal()==IDCANCEL)return;
		
	m_szAfterPath = dlg.GetPathName();
	GetDlgItem(IDC_EDIT_AFTER)->SetWindowTextW(m_szAfterPath);
}


void CProjectEdit::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}
 
 


 

void CProjectEdit::OnBnClickedCheckDefault()
{
	// TODO: 在此添加控件通知处理程序代码
		if( ((CButton *)(GetDlgItem(IDC_CHECK_DEFAULT)))->GetCheck() )
		{
			m_bDefaultFlag = 1;
		 
		}
		else
		{
			m_bDefaultFlag = 0;
		}
}


void CProjectEdit::OnBnClickedCheckKeyparam()
{
	// TODO: ÔÚ´ËÌí¼Ó¿Ø¼þÍ¨Öª´¦Àí³ÌÐò´úÂë
		if( ((CButton *)(GetDlgItem(IDC_CHECK_KEYPARAM)))->GetCheck() )
		{
			m_bSetParamFlag = 1;
		    GetDlgItem(IDC_EDIT_PARAM)->EnableWindow(true);
		}
		else
		{
			m_bSetParamFlag = 0;
			GetDlgItem(IDC_EDIT_PARAM)->EnableWindow(false);
			g_nParamValue=1;
		}
}


void CProjectEdit::OnBnClickedCheckKeyauto()
{
	// TODO: ÔÚ´ËÌí¼Ó¿Ø¼þÍ¨Öª´¦Àí³ÌÐò´úÂë
	if( ((CButton *)(GetDlgItem(IDC_CHECK_KEYAUTO)))->GetCheck() )
		{
			g_nKeyAutoFlag=1;
			GetDlgItem(IDC_EDIT_KEYPATH)->EnableWindow(false);
			GetDlgItem(IDC_BUTTON_KEYPATH)->EnableWindow(false);
			GetDlgItem(IDC_EDIT_KEYVALUE)->EnableWindow(false);
			GetDlgItem(IDC_CHECK_KEYMANUAL)->EnableWindow(false);
			((CButton *)(GetDlgItem(IDC_CHECK_KEYPARAM)))->SetCheck(0);
			((CButton *)(GetDlgItem(IDC_CHECK_KEYMANUAL)))->SetCheck(0);
			((CButton *)(GetDlgItem(IDC_CHECK_KEYNEED)))->SetCheck(0);
			m_nKeyFlag=0;
			m_bSetParamFlag = 0;
			GetDlgItem(IDC_EDIT_PARAM)->EnableWindow(false);
			g_nParamValue=1;
 		}
		else
		{
			g_nKeyAutoFlag=0;
 		}
}


void CProjectEdit::OnBnClickedCheckChecksum()
{
	// TODO: ÔÚ´ËÌí¼Ó¿Ø¼þÍ¨Öª´¦Àí³ÌÐò´úÂë
	if( ((CButton *)(GetDlgItem(IDC_CHECK_CHECKSUM)))->GetCheck() )
	{
		m_bChecksumFlag = 1;
		 
	}
	else
	{
		m_bChecksumFlag = 0;
	}
}


void CProjectEdit::OnBnClickedCheckEnablestop()
{
	// TODO: ÔÚ´ËÌí¼Ó¿Ø¼þÍ¨Öª´¦Àí³ÌÐò´úÂë

	if( ((CButton *)(GetDlgItem(IDC_CHECK_ENABLESTOP)))->GetCheck() )
	{
		m_bEnableStopFlag = 1;
		 
	}
	else
	{
		m_bEnableStopFlag = 0;
	}
}


void CProjectEdit::OnBnClickedCheckBypass()
{
	// TODO: ÔÚ´ËÌí¼Ó¿Ø¼þÍ¨Öª´¦Àí³ÌÐò´úÂë
	if( ((CButton *)(GetDlgItem(IDC_CHECK_BYPASS)))->GetCheck() )
		{
			m_bBypassFlag = 1;
		 
		}
		else
		{
			m_bBypassFlag = 0;
		}
}
