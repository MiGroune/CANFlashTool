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
//函数名：AddMessgaeTextErr(CString strText,CString crFontName,UINT crFontSize,bool bIsBold,bool bIsItalic,bool bIsUnderline,COLORREF crColor)
//功能  ：设置字符串的格式：字体、大小、粗细、倾斜、下划线、颜色
//参数  ：CString strText   ：将要输出的字符串
//        CString crFontName: 字体名称
//        UINT crFontSize   : 字体大小
//        bool bIsBold      ：是否加粗
//        bool bIsItalic    ：是否倾斜
//        bool bIsUnderline ：是否下划线
//        COLORREF crColor  : 字体颜色
//返回值：void
//********************************************************
void CRichEditCtrlEx::AddMessgaeTextErr(CString strText,CString crFontName,UINT crFontSize,bool bIsBold,bool bIsItalic,bool bIsUnderline,COLORREF crColor)
{
	
	SendMessage(EM_SETSEL ,-2, -1); //将光标停到最后
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

	//设置缩进为400
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

	PostMessage(WM_VSCROLL,SB_BOTTOM /*SB_BOTTOM*/, 0);//滚动条自动滚动到最后一行
	
}
void CRichEditCtrlEx::AddMessgaeTextColor(CString strText,CString crFontName,UINT crFontSize,bool bIsBold,bool bIsItalic,bool bIsUnderline,COLORREF crColor)
{
	
	SendMessage(EM_SETSEL ,-2, -1); //将光标停到最后
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

	//设置缩进为400
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
	

	PostMessage(WM_VSCROLL, SB_BOTTOM, 0);//滚动条自动滚动到最后一行

}
//***********************************************************
//函数名：AddMessgaeText(CString strText,CString crFontName,UINT crFontSize,bool bIsBold,bool bIsItalic,bool bIsUnderline,COLORREF crColor)
//功能  ：设置字符串的格式：字体、大小、粗细、倾斜、下划线、颜色
//参数  ：CString strText   ：将要输出的字符串
//        CString crFontName: 字体名称
//        UINT crFontSize   : 字体大小
//        bool bIsBold      ：是否加粗
//        bool bIsItalic    ：是否倾斜
//        bool bIsUnderline ：是否下划线
//        COLORREF crColor  : 字体颜色
//返回值：void
//********************************************************
void CRichEditCtrlEx::AddMessgaeText(CString strText,CString crFontName,UINT crFontSize,bool bIsBold,bool bIsItalic,bool bIsUnderline,COLORREF crColor)
{
	
	SendMessage(EM_SETSEL ,-2, -1); //将光标停到最后
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

	//设置缩进为400
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
	

	PostMessage(WM_VSCROLL, SB_BOTTOM, 0);//滚动条自动滚动到最后一行

}
void CRichEditCtrlEx::MessgaeText(CString strText,CString crFontName,UINT crFontSize,bool bIsBold,bool bIsItalic,bool bIsUnderline,COLORREF crColor)
{
	//	SetFocus();
	 SetWindowText(_T(""));
	// GetFocus();

	SendMessage(EM_SETSEL ,0, -1); //将光标停到最后

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

	//设置缩进为400
	////long n_startindex,n_endindex;
	PARAFORMAT2 pf2;
	ZeroMemory(&pf2, sizeof(PARAFORMAT2));
	pf2.cbSize = sizeof(PARAFORMAT2);
	pf2.dwMask = PFM_STARTINDENT;
	pf2.dxStartIndent = 400;
	SetParaFormat(pf2);

 	SetSel(0, -1);
	
 	
	ReplaceSel(strText);//+_T("\r\n")


	PostMessage(WM_VSCROLL, SB_BOTTOM, 0);//滚动条自动滚动到最后一行

}
/************************************************************************/
/* 初始化控件的一些设置                                        */
/************************************************************************/
//void CRichEditCtrlEx::PreSubclassWindow()
//{
//	
//	SetTargetDevice(NULL,0);	//设置自动换行	
//	/*SendMessage(EM_SETTEXTMODE, TM_PLAINTEXT|TM_MULTILEVELUNDO|TM_SINGLECODEPAGE, 0);*/
//	SendMessage(EM_SETLANGOPTIONS,0,0);		//关闭RichEdit默认开启的IMF_AUTOFONT和IMF_DUALFONT
//	SendMessage(EM_AUTOURLDETECT, TRUE, 0);	//开启自动网址检测
//	SetEventMask(GetEventMask() | ENM_LINK) ; 
//	SetOptions(ECOOP_OR, ECO_AUTOVSCROLL);
//	LimitText(0xffffffff);
//	CRichEditCtrl::PreSubclassWindow();
//}

//***********************************************************
//函数名：AddMessgaeTextQur(CString strText,CString crFontName,UINT crFontSize,bool bIsBold,bool bIsItalic,bool bIsUnderline,COLORREF crColor)
//功能  ：设置字符串的格式：字体、大小、粗细、倾斜、下划线、颜色
//参数  ：CString strText   ：将要输出的字符串
//        CString crFontName: 字体名称
//        UINT crFontSize   : 字体大小
//        bool bIsBold      ：是否加粗
//        bool bIsItalic    ：是否倾斜
//        bool bIsUnderline ：是否下划线
//        COLORREF crColor  : 字体颜色
//返回值：void
//********************************************************
void CRichEditCtrlEx::AddMessgaeTextQur(CString strText,CString crFontName,UINT crFontSize,bool bIsBold,bool bIsItalic,bool bIsUnderline,COLORREF crColor)
{
	
	SendMessage(EM_SETSEL ,-2, -1); //将光标停到最后
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

	//设置缩进为0
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


	PostMessage(WM_VSCROLL, SB_BOTTOM, 0);//滚动条自动滚动到最后一行

}
