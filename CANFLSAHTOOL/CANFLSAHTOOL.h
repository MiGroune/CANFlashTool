
// CANFLSAHTOOL.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CCANFLSAHTOOLApp:
// �йش����ʵ�֣������ CANFLSAHTOOL.cpp
//

class CCANFLSAHTOOLApp : public CWinApp
{
public:
	CCANFLSAHTOOLApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CCANFLSAHTOOLApp theApp;