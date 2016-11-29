/*----------------------------------------------------------------------------
| File        : RichEditCtrlEx.cpp
| Project     : CANFlashtool 
|
| Description : RichEditCtrl Edited string input 
|-----------------------------------------------------------------------------
| $Author: liaoshaoyan      $Revision:  
| $Data    2014.04.09   
|-----------------------------------------------------------------------------
| Copyright (c) 2014 by liaoshaoyan.  All rights reserved.
|---------------------------------------------------------------------------*/
#include "StdAfx.h"
#include "RichEditCtrlEx.h"


CRichEditCtrlEx::CRichEditCtrlEx(void)
{
}


CRichEditCtrlEx::~CRichEditCtrlEx(void)
{
}
//***********************************************************
//��������AddMessgaeTextErr(CString strText,CString crFontName,UINT crFontSize,bool bIsBold,bool bIsItalic,bool bIsUnderline,COLORREF crColor)
//����  �������ַ����ĸ�ʽ�����塢��С����ϸ����б���»��ߡ���ɫ
//����  ��CString strText   ����Ҫ������ַ���
//        CString crFontName: ��������
//        UINT crFontSize   : �����С
//        bool bIsBold      ���Ƿ�Ӵ�
//        bool bIsItalic    ���Ƿ���б
//        bool bIsUnderline ���Ƿ��»���
//        COLORREF crColor  : ������ɫ
//����ֵ��void
//********************************************************
void CRichEditCtrlEx::AddMessgaeTextErr(CString strText,CString crFontName,UINT crFontSize,bool bIsBold,bool bIsItalic,bool bIsUnderline,COLORREF crColor)
{
	
	SendMessage(EM_SETSEL ,-2, -1); //�����ͣ�����
	CHARFORMAT cf;

	ZeroMemory(&cf, sizeof(CHARFORMAT));
	cf.cbSize = sizeof(CHARFORMAT);
	cf.dwMask = CFM_BOLD | CFM_COLOR | CFM_FACE |CFM_ITALIC | CFM_SIZE | CFM_UNDERLINE ;
	
	if (bIsBold)
		cf.dwEffects|=CFE_BOLD;
	if (bIsItalic)
		cf.dwEffects |=CFE_ITALIC;
	if (bIsUnderline)
		cf.dwEffects |=CFE_UNDERLINE;
	cf.yHeight =crFontSize*20; 
	_stprintf_s(cf.szFaceName,32,crFontName);
	cf.crTextColor = RGB(255,0,0);
	//cf.crTextColor = crColor;
	SetWordCharFormat(cf);

	//��������Ϊ400
	//long n_startindex,n_endindex;
	PARAFORMAT2 pf2;
	ZeroMemory(&pf2, sizeof(PARAFORMAT2));
	pf2.cbSize = sizeof(PARAFORMAT2);
	pf2.dwMask = PFM_STARTINDENT;
	pf2.dxStartIndent = 400;
	SetParaFormat(pf2);

	SetSel(-1, -1);
	
	//ReplaceSel(strText+_T("\r\n"));
	ReplaceSel(strText);

	PostMessage(WM_VSCROLL,SB_BOTTOM /*SB_BOTTOM*/, 0);//�������Զ����������һ��
	
}
void CRichEditCtrlEx::AddMessgaeTextColor(CString strText,CString crFontName,UINT crFontSize,bool bIsBold,bool bIsItalic,bool bIsUnderline,COLORREF crColor)
{
	
	SendMessage(EM_SETSEL ,-2, -1); //�����ͣ�����
	CHARFORMAT cf;

	ZeroMemory(&cf, sizeof(CHARFORMAT));
	cf.cbSize = sizeof(CHARFORMAT);
	cf.dwMask = CFM_BOLD | CFM_COLOR | CFM_FACE |CFM_ITALIC | CFM_SIZE | CFM_UNDERLINE ;
	
	if (bIsBold)
		cf.dwEffects|=CFE_BOLD;
	if (bIsItalic)
		cf.dwEffects |=CFE_ITALIC;
	if (bIsUnderline)
		cf.dwEffects |=CFE_UNDERLINE;
	cf.yHeight =crFontSize*20; 
	swprintf(cf.szFaceName,32,crFontName);
	//cf.crTextColor = RGB(0,0,255);
	cf.crTextColor = crColor;
	SetWordCharFormat(cf);

	//��������Ϊ400
	////long n_startindex,n_endindex;

	PARAFORMAT2 pf2;
	ZeroMemory(&pf2, sizeof(PARAFORMAT2));
	pf2.cbSize = sizeof(PARAFORMAT2);
	pf2.dwMask = PFM_STARTINDENT;
	pf2.bLineSpacingRule=0;
	pf2.dxStartIndent = 400;
	SetParaFormat(pf2);
	//long n_index;
	//SetSel(-1, -1);
	//
	//m_Infor.GetSel(n_startindex,n_endindex);
	//GetSel(n_index,n_index);
	ReplaceSel(strText);//+_T("\r\n")
	

	PostMessage(WM_VSCROLL, SB_BOTTOM, 0);//�������Զ����������һ��

}
//***********************************************************
//��������AddMessgaeText(CString strText,CString crFontName,UINT crFontSize,bool bIsBold,bool bIsItalic,bool bIsUnderline,COLORREF crColor)
//����  �������ַ����ĸ�ʽ�����塢��С����ϸ����б���»��ߡ���ɫ
//����  ��CString strText   ����Ҫ������ַ���
//        CString crFontName: ��������
//        UINT crFontSize   : �����С
//        bool bIsBold      ���Ƿ�Ӵ�
//        bool bIsItalic    ���Ƿ���б
//        bool bIsUnderline ���Ƿ��»���
//        COLORREF crColor  : ������ɫ
//����ֵ��void
//********************************************************
void CRichEditCtrlEx::AddMessgaeText(CString strText,CString crFontName,UINT crFontSize,bool bIsBold,bool bIsItalic,bool bIsUnderline,COLORREF crColor)
{
	
	SendMessage(EM_SETSEL ,-2, -1); //�����ͣ�����
	CHARFORMAT cf;

	ZeroMemory(&cf, sizeof(CHARFORMAT));
	cf.cbSize = sizeof(CHARFORMAT);
	cf.dwMask = CFM_BOLD | CFM_COLOR | CFM_FACE |CFM_ITALIC | CFM_SIZE | CFM_UNDERLINE ;
	
	if (bIsBold)
		cf.dwEffects|=CFE_BOLD;
	if (bIsItalic)
		cf.dwEffects |=CFE_ITALIC;
	if (bIsUnderline)
		cf.dwEffects |=CFE_UNDERLINE;
	cf.yHeight =crFontSize*20; 
	swprintf(cf.szFaceName,32,crFontName);
	cf.crTextColor = RGB(0,0,255);
	//cf.crTextColor = crColor;
	SetWordCharFormat(cf);

	//��������Ϊ400
	////long n_startindex,n_endindex;

	PARAFORMAT2 pf2;
	ZeroMemory(&pf2, sizeof(PARAFORMAT2));
	pf2.cbSize = sizeof(PARAFORMAT2);
	pf2.dwMask = PFM_STARTINDENT;
	pf2.bLineSpacingRule=0;
	pf2.dxStartIndent = 400;
	SetParaFormat(pf2);
	//long n_index;
	//SetSel(-1, -1);
	//
	//m_Infor.GetSel(n_startindex,n_endindex);
	//GetSel(n_index,n_index);
	ReplaceSel(strText);//+_T("\r\n")
	

	PostMessage(WM_VSCROLL, SB_BOTTOM, 0);//�������Զ����������һ��

}
void CRichEditCtrlEx::MessgaeText(CString strText,CString crFontName,UINT crFontSize,bool bIsBold,bool bIsItalic,bool bIsUnderline,COLORREF crColor)
{
	//	SetFocus();
	 SetWindowText(_T(""));
	// GetFocus();

	SendMessage(EM_SETSEL ,0, -1); //�����ͣ�����

	CHARFORMAT cf;

	ZeroMemory(&cf, sizeof(CHARFORMAT));
	cf.cbSize = sizeof(CHARFORMAT);
	cf.dwMask = CFM_BOLD | CFM_COLOR | CFM_FACE |CFM_ITALIC | CFM_SIZE | CFM_UNDERLINE ;
	
	if (bIsBold)
		cf.dwEffects|=CFE_BOLD;
	if (bIsItalic)
		cf.dwEffects |=CFE_ITALIC;
	if (bIsUnderline)
		cf.dwEffects |=CFE_UNDERLINE;
	cf.yHeight =crFontSize*20; 
	swprintf(cf.szFaceName,32,crFontName);
	 
	cf.crTextColor = crColor;
	SetWordCharFormat(cf);

	//��������Ϊ400
	////long n_startindex,n_endindex;
	PARAFORMAT2 pf2;
	ZeroMemory(&pf2, sizeof(PARAFORMAT2));
	pf2.cbSize = sizeof(PARAFORMAT2);
	pf2.dwMask = PFM_STARTINDENT;
	pf2.dxStartIndent = 400;
	SetParaFormat(pf2);

 	SetSel(0, -1);
	
 	
	ReplaceSel(strText);//+_T("\r\n")


	PostMessage(WM_VSCROLL, SB_BOTTOM, 0);//�������Զ����������һ��

}
/************************************************************************/
/* ��ʼ���ؼ���һЩ����                                        */
/************************************************************************/
//void CRichEditCtrlEx::PreSubclassWindow()
//{
//	
//	SetTargetDevice(NULL,0);	//�����Զ�����	
//	/*SendMessage(EM_SETTEXTMODE, TM_PLAINTEXT|TM_MULTILEVELUNDO|TM_SINGLECODEPAGE, 0);*/
//	SendMessage(EM_SETLANGOPTIONS,0,0);		//�ر�RichEditĬ�Ͽ�����IMF_AUTOFONT��IMF_DUALFONT
//	SendMessage(EM_AUTOURLDETECT, TRUE, 0);	//�����Զ���ַ���
//	SetEventMask(GetEventMask() | ENM_LINK) ; 
//	SetOptions(ECOOP_OR, ECO_AUTOVSCROLL);
//	LimitText(0xffffffff);
//	CRichEditCtrl::PreSubclassWindow();
//}

//***********************************************************
//��������AddMessgaeTextQur(CString strText,CString crFontName,UINT crFontSize,bool bIsBold,bool bIsItalic,bool bIsUnderline,COLORREF crColor)
//����  �������ַ����ĸ�ʽ�����塢��С����ϸ����б���»��ߡ���ɫ
//����  ��CString strText   ����Ҫ������ַ���
//        CString crFontName: ��������
//        UINT crFontSize   : �����С
//        bool bIsBold      ���Ƿ�Ӵ�
//        bool bIsItalic    ���Ƿ���б
//        bool bIsUnderline ���Ƿ��»���
//        COLORREF crColor  : ������ɫ
//����ֵ��void
//********************************************************
void CRichEditCtrlEx::AddMessgaeTextQur(CString strText,CString crFontName,UINT crFontSize,bool bIsBold,bool bIsItalic,bool bIsUnderline,COLORREF crColor)
{
	
	SendMessage(EM_SETSEL ,-2, -1); //�����ͣ�����
	CHARFORMAT cf;

	ZeroMemory(&cf, sizeof(CHARFORMAT));
	cf.cbSize = sizeof(CHARFORMAT);
	cf.dwMask = CFM_BOLD | CFM_COLOR | CFM_FACE |CFM_ITALIC | CFM_SIZE | CFM_UNDERLINE ;
	
	if (bIsBold)
		cf.dwEffects|=CFE_BOLD;
	if (bIsItalic)
		cf.dwEffects |=CFE_ITALIC;
	if (bIsUnderline)
		cf.dwEffects |=CFE_UNDERLINE;
	cf.yHeight =crFontSize*20; 
	swprintf(cf.szFaceName,32,crFontName);
	//cf.crTextColor = RGB(255,0,0);
	cf.crTextColor = crColor;
	SetWordCharFormat(cf);

	//��������Ϊ0
	////long n_startindex,n_endindex;
	PARAFORMAT2 pf2;
	ZeroMemory(&pf2, sizeof(PARAFORMAT2));
	pf2.cbSize = sizeof(PARAFORMAT2);
	pf2.dwMask = PFM_STARTINDENT;
	pf2.dxStartIndent = 0;
	SetParaFormat(pf2);

	SetSel(-1, -1);
	
	//m_Infor.GetSel(n_startindex,n_endindex);
	
	ReplaceSel(strText+_T("\r\n"));


	PostMessage(WM_VSCROLL, SB_BOTTOM, 0);//�������Զ����������һ��

}
