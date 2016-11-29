/*----------------------------------------------------------------------------
| File        : ES581Function.cpp
| Project     : CANFlashtool 
|
| Description : Shows the basic CAN functionality for the ES581 Driver Library
|-----------------------------------------------------------------------------
| $Author: liaoshaoyan      $Revision:  
| $Data    2014.04.09   
|-----------------------------------------------------------------------------*/
 
// ES581Function.cpp : 实现文件
//

#include "stdafx.h"
#include "ES581Function.h"


// CES581Function


CES581Function::CES581Function()
{

}

CES581Function::~CES581Function()
{
}

bool CES581Function::Load_icsneo40DLL(void)	
 {
	if((hDLL = LoadLibrary(_T("icsneo40.dll"))) == NULL)
			return false;

	icsneoFindNeoDevices =    (FINDNEODEVICES) GetProcAddress(hDLL,              "icsneoFindNeoDevices");
	icsneoOpenNeoDevice =     (OPENNEODEVICE) GetProcAddress(hDLL,               "icsneoOpenNeoDevice");
	icsneoClosePort =         (CLOSEPORT) GetProcAddress(hDLL,                   "icsneoClosePort");	
	icsneoFreeObject =        (FREEOBJECT) GetProcAddress(hDLL,                  "icsneoFreeObject");
	icsneoGetDLLVersion =  	(GETDLLVERSION) GetProcAddress(hDLL,                  "icsneoGetDLLVersion");
	icsneoValidateHObject = (VALIDATEHOBJECT) GetProcAddress(hDLL,"icsneoValidateHObject");

	icsneoGetConfiguration =  (GETCONFIG) GetProcAddress(hDLL,                   "icsneoGetConfiguration");
	icsneoSendConfiguration = (SENDCONFIG) GetProcAddress(hDLL,                  "icsneoSendConfiguration");
	icsneoGetFireSettings =   (GETFIRESETTINGS) GetProcAddress(hDLL,             "icsneoGetFireSettings");
	icsneoSetFireSettings =   (SETFIRESETTINGS) GetProcAddress(hDLL,             "icsneoSetFireSettings");
	icsneoGetDeviceParameters = (GETDEVICEPARMS) GetProcAddress(hDLL,            "icsneoGetDeviceParameters");
	icsneoSetDeviceParameters = (SETDEVICEPARMS) GetProcAddress(hDLL,            "icsneoSetDeviceParameters");
		
	icsneoGetLastAPIError =   (GETLASTAPIERROR) GetProcAddress(hDLL,             "icsneoGetLastAPIError");
	icsneoScriptLoad =        (SCRIPTLOAD) GetProcAddress(hDLL,                  "icsneoScriptLoad");
	icsneoScriptStart =       (SCRIPTSTART) GetProcAddress(hDLL,                 "icsneoScriptStart");
	icsneoScriptStop =        (SCRIPTSTOP) GetProcAddress(hDLL,                  "icsneoScriptStop");
	icsneoScriptClear =       (SCRIPTCLEAR) GetProcAddress(hDLL,                 "icsneoScriptClear");
	icsneoScriptStartFBlock = (SCRIPTSTARTFBLOCK) GetProcAddress(hDLL,           "icsneoScriptStartFBlock");
	icsneoScriptStopFBlock =  (SCRIPTSTOPFBLOCK) GetProcAddress(hDLL,            "icsneoScriptStopFBlock");
	icsneoScriptGetFBlockStatus = (SCRIPTGETFBLOCKSTATUS) GetProcAddress(hDLL,   "icsneoScriptGetFBlockStatus");
	icsneoScriptReadAppSignal = (SCRIPTREADAPPSIGNAL) GetProcAddress(hDLL,       "icsneoScriptReadAppSignal");
	icsneoScriptWriteAppSignal = (SCRIPTWRITEAPPSIGNAL) GetProcAddress(hDLL,     "icsneoScriptWriteAppSignal");
	icsneoScriptReadTxMessage = (SCRIPTREADTXMESSAGE) GetProcAddress(hDLL,       "icsneoScriptReadTxMessage");
	icsneoScriptWriteTxMessage = (SCRIPTWRITETXMESSAGE) GetProcAddress(hDLL,     "icsneoScriptWriteTxMessage");
	icsneoScriptReadRxMessage = (SCRIPTREADRXMESSAGE) GetProcAddress(hDLL,       "icsneoScriptReadRxMessage");
	icsneoScriptWriteRxMessage = (SCRIPTWRITERXMESSAGE)GetProcAddress(hDLL,      "icsneoScriptWriteRxMessage");
	//	icsneoScriptReadISO15765_2_TxMessage = (SCRIPTREADISO15765TXMESSAGE) GetProcAddress(hDLL, "icsneoScriptReadISO15765_2_TxMessage");
	//	icsneoScriptWriteISO15765_2_TxMessage = (SCRIPTWRITEISO15765TXMESSAGE) GetProcAddress(hDLL, "icsneoScriptWriteISO15765_2_TxMessage");
	icsneoScriptGetScriptStatus = (SCRIPTGETSCRIPTSTATUS) GetProcAddress(hDLL, "icsneoScriptGetScriptStatus");
	icsneoTxMessages = (TXMESSAGES)GetProcAddress (hDLL,						"icsneoTxMessages");
	icsneoGetMessages = (GETMESSAGES) GetProcAddress (hDLL, "icsneoGetMessages");
	icsneoGetTimeStampForMsg = (GETTSFORMSG)GetProcAddress(hDLL,"icsneoGetTimeStampForMsg");
	icsneoWaitForRxMessagesWithTimeOut = (WAITFORRXMSGS) GetProcAddress(hDLL,"icsneoWaitForRxMessagesWithTimeOut");
	icsneoGetLastAPIError = (GETLASTAPIERROR) GetProcAddress(hDLL,"icsneoGetLastAPIError");
	icsneoGetErrorMessages = (GETERRMSGS) GetProcAddress(hDLL,"icsneoGetErrorMessages");
	icsneoGetErrorInfo = (GETERRORINFO) GetProcAddress(hDLL,"icsneoGetErrorInfo");
	icsneoSetBitRate = (SETBITRATE) GetProcAddress (hDLL, "icsneoSetBitRate");
	icsneoGetVCAN3Settings = (GETVCAN3SETTINGS) GetProcAddress(hDLL,"icsneoGetVCAN3Settings");
	icsneoSetVCAN3Settings =(SETVCAN3SETTINGS) GetProcAddress(hDLL,"icsneoSetVCAN3Settings");
	icsneoSetISO15765RxParameters=(SETISO15765RXPARMS) GetProcAddress(hDLL,"icsneoSetISO15765RxParameters");

	if(icsneoFindNeoDevices  == NULL ||
	icsneoOpenNeoDevice  == NULL || 
	icsneoClosePort  == NULL || 
	icsneoFreeObject  == NULL || 
	icsneoGetDLLVersion  == NULL || 
	icsneoValidateHObject  == NULL || 
	icsneoGetConfiguration  == NULL || 
	icsneoSendConfiguration  == NULL || 
	icsneoGetFireSettings  == NULL || 
	icsneoSetFireSettings  == NULL || 
	icsneoGetLastAPIError  == NULL || 
	icsneoScriptLoad  == NULL || 
	icsneoScriptStart  == NULL || 
	icsneoScriptStop  == NULL || 
	icsneoScriptClear  == NULL || 
	icsneoScriptStartFBlock  == NULL || 
	icsneoScriptStopFBlock  == NULL || 
	icsneoScriptGetFBlockStatus  == NULL || 
	icsneoScriptReadAppSignal  == NULL || 
	icsneoScriptWriteAppSignal  == NULL || 
	icsneoScriptReadTxMessage  == NULL || 
	icsneoScriptWriteTxMessage  == NULL || 
	icsneoScriptReadRxMessage  == NULL || 
	icsneoScriptWriteRxMessage  == NULL || 
	icsneoScriptGetScriptStatus  == NULL || 
	icsneoTxMessages  == NULL || 
	icsneoGetMessages  == NULL || 
	icsneoGetTimeStampForMsg  == NULL || 
	icsneoWaitForRxMessagesWithTimeOut  == NULL || 
	icsneoGetLastAPIError  == NULL || 
	icsneoGetErrorMessages  == NULL || 
	icsneoGetErrorInfo  == NULL || 
	icsneoSetBitRate  == NULL || 
	icsneoGetVCAN3Settings  == NULL || 
	icsneoSetVCAN3Settings  == NULL  
	)
	{
		FreeLibrary(hDLL);
		return false;
	}

   return true;
 }
int CES581Function::CmdOpenFirstDevice() 
{
	int iResult; 
	
	int iNumberOfDevices;
			 
	//if(m_bPortOpen == true) return 0;

	iNumberOfDevices = 1;
	iResult = icsneoFindNeoDevices(65535, &ndNeoToOpen, &iNumberOfDevices);
	if(iResult == false)
	{
		//MessageBox("Problem Finding Device");
		return -1;
	}

	if(iNumberOfDevices<1)
	{
	//MessageBox::Show("No Devices Found");
		return -1;
	}
				
	m_hObject = NULL;
	iResult = icsneoOpenNeoDevice(&ndNeoToOpen, &m_hObject, NULL,1,0);
	if(iResult==false)
	{
	//MessageBox::Show("Problem Opening Port");
		return -1;
	}
	//MessageBox::Show("Port Opened OK!");
	iOpenDeviceType = ndNeoToOpen.DeviceType;
	CString str;
	switch(iOpenDeviceType)
	{
	case 1:
		str.Format(_T("neoVI Blue SN :%d"),ndNeoToOpen.SerialNumber);
 	break;
	case 4:
 		str.Format(_T("ValueCAN 2 SN :%d"),ndNeoToOpen.SerialNumber);
	break;
	case 8:
 		str.Format(_T("neoVI FIRE SN :%d"),ndNeoToOpen.SerialNumber);
	break;
	case 16:
 		str.Format(_T("ValueCAN 3 SN :%d"),ndNeoToOpen.SerialNumber);
	break;
	default:
 		str.Format(_T("neoVI Blue SN :%d"),ndNeoToOpen.SerialNumber);
	}
	m_bPortOpen = true;
	return 0;
}

int CES581Function::cmdSetBitRate(int baud)
{
		int iBitRateToUse;
        int iNetworkID;
        int iResult; 


		// Has the uset open neoVI yet?
		if (m_bPortOpen==false)
		{
			//MessageBox::Show("neoVI not opened");
			return -1;   // do not read messages if we haven't opened neoVI yet
		}

		//Get the network name index to set the baud rate of 
        switch(0)
		{
			case 0:
                iNetworkID = NETID_HSCAN;
				break;
			case 1:
                iNetworkID = NETID_MSCAN;
				break;
			case 2:
                iNetworkID = NETID_SWCAN;
				break;
			case 3:
                iNetworkID = NETID_LSFTCAN;
				break;
			case 4:
                iNetworkID = NETID_FIRE_HSCAN2;
				break;
			case 5:
                iNetworkID = NETID_FIRE_HSCAN3;
				break;
			default:
                 return -1;
		}

		iBitRateToUse = baud;

		//Set the bit rate
        iResult = icsneoSetBitRate(m_hObject, iBitRateToUse, iNetworkID);
		//cmdSetRxParameters();
        if (iResult == 0)  
		{
			//MessageBox::Show("Problem setting bit rate");
			return -1;
		}
		else
		{
			//MessageBox::Show("Bit rate Set");
			return 0;
		}
    
		 }

int CES581Function::cmdTransmit(icsSpyMessage *pMessagesTx,int sendnum) 
{
    int lResult; 
    icsSpyMessageJ1850 stJMsg;
    long lNetworkID=1;
    long lNumberBytes;
	lResult = icsneoTxMessages(m_hObject, pMessagesTx, lNetworkID, sendnum); 
 	return lResult;
}
int CES581Function::cmdReceive(icsSpyMessage *pMsg,int *NumberOfMessages,int *NumberOfErrors)
{
	long lResult,iResult;
	int iNumberOfErrors;
    long lCount;
	int lNumberOfMessages;
    double dTime;
    long lByteCount;
 	//static icsSpyMessage stMessages[20000];
	icsSpyMessageJ1850 icsJ1850Msg;
 
     // Has the uset open neoVI yet?
		for(int i=0;i<10;i++)
		{
			//if(iResult == 0)continue;
			lResult = icsneoGetMessages(m_hObject, pMsg, &iNumberOfErrors,NumberOfErrors); //Call get message function
 			if(lResult>0&&iNumberOfErrors>0)break;
			iResult = icsneoWaitForRxMessagesWithTimeOut(m_hObject,100);
			
			
		}
		*NumberOfMessages = iNumberOfErrors;
		return lResult;
  //       for(lCount=1;lCount<=lNumberOfMessages;lCount++)   //Loop to acquire all of the messages in the buffer
		//{
  //          lByteCount = stMessages[lCount - 1].NumberBytesData;
		//	lResult = icsneoGetTimeStampForMsg(m_hObject, &stMessages[lCount - 1], &dTime);
		//	sListString = "Time : " + Convert::ToString(dTime) + " ";  //Format time and add to output string
		//	if((stMessages[lCount - 1].StatusBitField & 2)!=0)   //See if message is RX or TX
		//	{
  //              sListString = sListString + "Tx Message ";
		//	}
  //          else
		//	{
  //              sListString = sListString + "Rx Message ";
		//	}

		//	sNetName="";
		//	switch(stMessages[lCount - 1].NetworkID)
		//	{
		//		case NETID_HSCAN:
		//			sNetName = "HSCAN";
		//			break;
		//		case NETID_MSCAN:
		//			sNetName = "MSCAN";
		//			break;
		//		case NETID_SWCAN:
		//			sNetName = "SWCAN";
		//			break;
		//		case NETID_LSFTCAN:
		//			sNetName = "LSFTCAN";
		//			break;
		//		case NETID_FORDSCP:
		//			sNetName = "FORD SCP";
		//			break;
		//		case NETID_J1708:
		//			sNetName = "J1708";
		//			break;
		//		case NETID_AUX:
		//			sNetName = "AUX";
		//			break;
		//		case NETID_JVPW:
		//			sNetName = "J1850 VPW";
		//			break;
		//		case NETID_ISO:
		//			sNetName = "ISO/UART";
		//			break;
		//		default:
		//			sNetName = "Unknown";
		//	}

		//	sListString = sListString + sNetName;

		//	if(stMessages[lCount-1].Protocol == SPY_PROTOCOL_CAN)
		//	{
	 //           //sListString = sListString + stMessages[lCount - 1].NetworkID;
		//		sListString = sListString + " ArbID-";  //Create output string with ArbID and Data Bytes
		//		sListString = sListString + Convert::ToString(stMessages[lCount - 1].ArbIDOrHeader,16) + " Data-";
		//		if(lByteCount>=1) sListString = sListString + Convert::ToString(stMessages[lCount - 1].Data[0],16) + " ";
		//		if(lByteCount>=2) sListString = sListString + Convert::ToString(stMessages[lCount - 1].Data[1],16) + " ";
		//		if(lByteCount>=3) sListString = sListString + Convert::ToString(stMessages[lCount - 1].Data[2],16) + " ";
		//		if(lByteCount>=4) sListString = sListString + Convert::ToString(stMessages[lCount - 1].Data[3],16) + " ";
		//		if(lByteCount>=5) sListString = sListString + Convert::ToString(stMessages[lCount - 1].Data[4],16) + " ";
		//		if(lByteCount>=6) sListString = sListString + Convert::ToString(stMessages[lCount - 1].Data[5],16) + " ";
		//		if(lByteCount>=7) sListString = sListString + Convert::ToString(stMessages[lCount - 1].Data[6],16) + " ";
		//		if(lByteCount>=8) sListString = sListString + Convert::ToString(stMessages[lCount - 1].Data[7],16) + " ";
		//	}
		//	else
		//	{
		//		memcpy((void*)&icsJ1850Msg,(void*)&stMessages[lCount-1],sizeof(icsSpyMessageJ1850));

		//		//sListString = sListString + icsJ1850Msg.NetworkID;
		//		sListString = sListString + " ArbID-";  //Create output string with ArbID and Data Bytes
		//		sListString = sListString + Convert::ToString(icsJ1850Msg.Header[0],16) + " ";
		//		sListString = sListString + Convert::ToString(icsJ1850Msg.Header[1],16) + " ";
		//		sListString = sListString + Convert::ToString(icsJ1850Msg.Header[2],16) + " Data-";
		//		if(lByteCount>=1) sListString = sListString + Convert::ToString(icsJ1850Msg.Data[0],16) + " ";
		//		if(lByteCount>=2) sListString = sListString + Convert::ToString(icsJ1850Msg.Data[1],16) + " ";
		//		if(lByteCount>=3) sListString = sListString + Convert::ToString(icsJ1850Msg.Data[2],16) + " ";
		//		if(lByteCount>=4) sListString = sListString + Convert::ToString(icsJ1850Msg.Data[3],16) + " ";
		//		if(lByteCount>=5) sListString = sListString + Convert::ToString(icsJ1850Msg.Data[4],16) + " ";
		//		if(lByteCount>=6) sListString = sListString + Convert::ToString(icsJ1850Msg.Data[5],16) + " ";
		//		if(lByteCount>=7) sListString = sListString + Convert::ToString(icsJ1850Msg.Data[6],16) + " ";
		//		if(lByteCount>=8) sListString = sListString + Convert::ToString(icsJ1850Msg.Data[7],16) + " ";
		//	}
  //          lstMessage->Items->Add(sListString);  //Display data bytes
		//}

		 }

int CES581Function::cmdGetTimeStamp(double *dtime,icsSpyMessage *msg)
{
	long lResult;
	int iNumberOfErrors;
    long lCount;
	int lNumberOfMessages;
     long lByteCount;
 //	static icsSpyMessage stMessages[20000];
	icsSpyMessageJ1850 icsJ1850Msg;
 
     // Has the uset open neoVI yet?
 
        lResult = icsneoGetTimeStampForMsg(m_hObject,msg, dtime);//Call get message function
        if(lResult==0)//See if Call was successful
		{
             return -1;
		}
 		return 0;
}
int CES581Function::cmdCloseDevice()
{
			 				int iResult;
				int iNumberOfErrors;
				iResult = icsneoClosePort(m_hObject,&iNumberOfErrors);
				if(iResult==0)
				{
					//MessageBox::Show("Problem Closing Port");
					return -1;
				}
				else
				{
					//MessageBox::Show("Port Closed");
					return 0;
				}
				//lblneoInfo->Text = "Port Not Opened";

				iOpenDeviceType = 0;
				m_bPortOpen = false;
		 }
void CES581Function::cmdSetRxParameters()
{
			 	spyFilterLong FF_CFMSGFilter;
	icsSpyMessage FlowCTxMsg;
	// reset the structures to zero
	memset(&FF_CFMSGFilter,0,sizeof(FF_CFMSGFilter));
	memset(&FlowCTxMsg,0,sizeof(FlowCTxMsg));
	// setup the filter
	FF_CFMSGFilter.Header = 0x7E8;
	FF_CFMSGFilter.HeaderMask = 0xFFF;
	FlowCTxMsg.StatusBitField=2;
	FlowCTxMsg.NumberBytesData =8;
	FlowCTxMsg.ArbIDOrHeader = 0x7e0;
	FlowCTxMsg.Data[0] = 0x30; // flow control frame : FlowStatus=CTS
	FlowCTxMsg.Data[1] = 0x0; // Blocksize
	FlowCTxMsg.Data[2] = 0x0; // STMin=0
	// setup rx on HSCAN with 100 ms timeout and 3 block size
	icsneoSetISO15765RxParameters(m_hObject,NETID_HSCAN,true,&FF_CFMSGFilter,&FlowCTxMsg,100,3,0,0);
		 }

