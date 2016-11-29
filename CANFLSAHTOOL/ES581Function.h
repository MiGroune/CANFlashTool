#pragma once

#include "icsnVC40.h"
#define ES581_SUCCESS                   1
#define ES581_ERROR                     0
class CES581Function
{
FINDNEODEVICES icsneoFindNeoDevices;
OPENNEODEVICE icsneoOpenNeoDevice;
CLOSEPORT icsneoClosePort;
FREEOBJECT icsneoFreeObject;
GETDLLVERSION icsneoGetDLLVersion;

//Message Functions
GETMESSAGES icsneoGetMessages;
TXMESSAGES icsneoTxMessages;
WAITFORRXMSGS icsneoWaitForRxMessagesWithTimeOut;
GETTSFORMSG icsneoGetTimeStampForMsg;
ENABLERXQUEUE icsneoEnableNetworkRXQueue;
GETISO15765STATUS icsneoGetISO15765Status;
SETISO15765RXPARMS icsneoSetISO15765RxParameters;

//Device Functions
GETCONFIG icsneoGetConfiguration;
SENDCONFIG icsneoSendConfiguration;
GETFIRESETTINGS icsneoGetFireSettings;
SETFIRESETTINGS icsneoSetFireSettings;
GETVCAN3SETTINGS icsneoGetVCAN3Settings;
SETVCAN3SETTINGS icsneoSetVCAN3Settings;
SETBITRATE icsneoSetBitRate;
GETDEVICEPARMS icsneoGetDeviceParameters;
SETDEVICEPARMS icsneoSetDeviceParameters;

//Error Functions
GETLASTAPIERROR icsneoGetLastAPIError;
GETERRMSGS icsneoGetErrorMessages;
GETERRORINFO icsneoGetErrorInfo;

//General Utility Functions
VALIDATEHOBJECT icsneoValidateHObject;
GETSERIALNUMBER icsneoGetSerialNumber;
STARTSOCKSERVER icsneoStartSockServer;
STOPSOCKSERVER icsneoStopSockServer;

//CoreMini Functions
SCRIPTSTART icsneoScriptStart;
SCRIPTSTOP icsneoScriptStop;
SCRIPTLOAD icsneoScriptLoad;
SCRIPTCLEAR icsneoScriptClear;
SCRIPTSTARTFBLOCK icsneoScriptStartFBlock;
SCRIPTGETFBLOCKSTATUS icsneoScriptGetFBlockStatus;
SCRIPTSTOPFBLOCK icsneoScriptStopFBlock;
SCRIPTGETSCRIPTSTATUS icsneoScriptGetScriptStatus;
SCRIPTREADAPPSIGNAL icsneoScriptReadAppSignal;
SCRIPTWRITEAPPSIGNAL icsneoScriptWriteAppSignal;
SCRIPTREADRXMESSAGE icsneoScriptReadRxMessage;
SCRIPTREADTXMESSAGE icsneoScriptReadTxMessage;
SCRIPTWRITERXMESSAGE icsneoScriptWriteRxMessage;
SCRIPTWRITETXMESSAGE icsneoScriptWriteTxMessage;

public:
	CES581Function();           // 动态创建所使用的受保护的构造函数
	virtual ~CES581Function();
	bool CES581Function :: Load_icsneo40DLL(void);	 
	int m_hObject; //handle for device
    bool m_bPortOpen; //tells the port status of the device
    int iOpenDeviceType; //Storage for the device type that is open
	byte NetworkIDConvert[15];
	NeoDevice ndNeoToOpen;
	HINSTANCE hDLL;
	int CES581Function::CmdOpenFirstDevice();
	int CES581Function::cmdSetBitRate(int baud);
	int CES581Function::cmdTransmit(icsSpyMessage *pMessagesTx,int sendnum);
	int CES581Function::cmdReceive(icsSpyMessage *pMsg,int *NumberOfMessages,int *NumberOfErrors);
	int CES581Function::cmdGetTimeStamp(double *dtime,icsSpyMessage *msg);
	int CES581Function::cmdCloseDevice();
	void CES581Function::cmdSetRxParameters();
 };


