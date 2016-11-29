#pragma once
#include "ConfigFile.h"
#include "xlCANFunctions.h"
#include "RichEditCtrlEx.h"
#include "CANCARD2Func.h"
#include "OperateFile.h"
#include "USB2CAN_interface.h"
#include "ES581Function.h"
//#include "ICAN.h"
#define CAN_CHANNEL_0 0 
#define CAN_CHANNEL_1 1 
#define FILETYPE_NUM 6 

extern HANDLE hCanDev;
 typedef struct _ACTIONITEMWALUE
{
	CString		 Label;
	CString      ActionType; 
	CString	     Value;
	CString	     Address;
	CString      Size;
	CString      Comment;
	CString      CurrentValue;
	
}*pActionItemValue,ActionItemValue;
 #define MAXSIZE   10
 typedef struct _ACTIONITEMINFOR
{
	 ActionItemValue ActionItem[MAXSIZE];
	 int ItemNum;
}*pActionItemInfor,ActionItemInfor;

extern class CES581Function *m_ES581CAN;
extern int g_EcuFile;       //具体类型ECU，需要处理的文件种类
extern int g_CANDirverType;
extern class ConfigFile *config;
extern class CCANFunctions *m_CAN;
extern class CProgressCtrl m_Process;
extern class CRichEditCtrlEx m_Infor;
extern class CCANCARD2Func *m_CANcard;
extern char g_StopFlag;
extern char g_usb2canFlag;
extern ActionItemInfor ItemInfor;
extern unsigned char cut_data[8000];
extern  char start_clear;
extern class COperateFile *pOpFile;
extern int g_nKeyFlag;
extern int g_nKeyAutoFlag;
extern unsigned int g_nKeyValue;
extern unsigned int g_nParamValue;
extern unsigned int g_nChecksumFlag;

extern int g_bSetParamFlag;
extern CString g_szKeyPath;
extern class CRichEditCtrlEx m_Editdisplay;
extern BOOL Display_flag;
typedef struct _CAN_interactMsg
{
	char	    data[200];  
	float		opTime;
	UINT		serialNo;
	char		MsgState;
	char		Iswrite;
	_CAN_interactMsg	*next;
}CAN_interactMsg;//链锁
extern CAN_interactMsg	interactMsgHead,*interactMsgTail;//头 尾
extern HANDLE	interactMsgEvent;
extern HANDLE	interactMsgMutex;
extern HANDLE	displayMsgMutex;
extern void putInteractMsg(CAN_interactMsg *msg);
extern CAN_interactMsg *getInteractMsg();
#define LIST_ITEM_MAX	1000000
extern CAN_interactMsg	* displayArray[LIST_ITEM_MAX];
extern int				  listItemNum ;
#define MSGOUT	0
#define MSGIN   1
#define MSGINFO 2
#define MSGERR  3

extern TransDataType      Calibration;
extern TransDataType      Routine;
extern int gserialNo;
extern int displaystreamnum;
extern CAN_interactMsg	* streamArray[LIST_ITEM_MAX];
extern CString m_szMfgPath;
extern CString m_szTswPath;
extern CString m_szBootPath;
extern CString m_szCutPath;
extern CString m_szCalPath;
extern CString m_szPtpPath;
extern CString g_strdll[50];
extern  CStringArray g_strArray;
extern  CString m_szSerialNum;  
extern BOOL g_bBypassFlag;