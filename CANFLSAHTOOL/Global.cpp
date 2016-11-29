 
#include "StdAfx.h"
#include "Global.h"
HANDLE hCanDev;
class ConfigFile *config= new ConfigFile();
int g_EcuFile;       //具体类型ECU，需要处理的文件种类
int g_bSetParamFlag;
class CES581Function *m_ES581CAN = NULL;
class CCANFunctions *m_CAN = new CCANFunctions();
int g_CANDirverType;
class CProgressCtrl m_Process;
class CRichEditCtrlEx m_Infor;
class CCANCARD2Func *m_CANcard;
char g_StopFlag;
ActionItemInfor ItemInfor;
unsigned char cut_data[8000];
char start_clear=0;
class COperateFile *pOpFile;
int g_nKeyFlag;
int g_nKeyAutoFlag;
unsigned int g_nKeyValue;
unsigned int g_nParamValue;
unsigned int g_nChecksumFlag;
CString g_szKeyPath;
BOOL Display_flag;
class CRichEditCtrlEx m_Editdisplay;
void putInteractMsg(CAN_interactMsg *msg);
CAN_interactMsg *getInteractMsg();
char g_usb2canFlag;	
CAN_interactMsg	interactMsgHead,*interactMsgTail;//头 尾
HANDLE	interactMsgEvent;
HANDLE	interactMsgMutex;
HANDLE	displayMsgMutex;
CAN_interactMsg	* displayArray[LIST_ITEM_MAX];
int				  listItemNum=0;
TransDataType      Calibration;
TransDataType      Routine;
int gserialNo;
int displaystreamnum=0;
CAN_interactMsg	* streamArray[LIST_ITEM_MAX];
CString m_szTswPath;
CString m_szMfgPath;
CString m_szBootPath;
CString m_szCutPath;
CString m_szCalPath;
CString m_szPtpPath;
CString g_strdll[50];
CStringArray g_strArray;
CString m_szSerialNum;  
BOOL g_bBypassFlag;