#pragma once
#include "afxcmn.h"
class CRichEditCtrlEx :public CRichEditCtrl	
{
public:
	CRichEditCtrlEx(void);
	~CRichEditCtrlEx(void);

	void AddMessgaeTextErr(CString strText,CString crFontName,UINT crFontSize,bool bIsBold,bool bIsItalic,bool bIsUnderline,COLORREF crColor);
	void AddMessgaeText(CString strText,CString crFontName,UINT crFontSize,bool bIsBold,bool bIsItalic,bool bIsUnderline,COLORREF crColor);
	void AddMessgaeTextQur(CString strText,CString crFontName,UINT crFontSize,bool bIsBold,bool bIsItalic,bool bIsUnderline,COLORREF crColor);
	void MessgaeText(CString strText,CString crFontName,UINT crFontSize,bool bIsBold,bool bIsItalic,bool bIsUnderline,COLORREF crColor);
	void AddMessgaeTextColor(CString strText,CString crFontName,UINT crFontSize,bool bIsBold,bool bIsItalic,bool bIsUnderline,COLORREF crColor);
};

