#ifndef CAN_PROTOCOL_H 
#define CAN_PROTOCOL_H
 
#include "SpecialType.h"
#include "xlCANFunctions.h"
#include "CANCARD2Func.h"
#include "USB2CAN_interface.h"  
#include "ES581Function.h" 
#define CAN_RESULT_OK			0
#define CAN_RESULT_NO_DEVICE	-1
#define CAN_RESULT_SMALL_BUFFER	-2
#define CAN_RESULT_SYSTEM_ERROR	-3
#define CAN_RESULT_NOT_OPEN		-4
#define CAN_RESULT_TIMEOUT		-5//-5
#define CAN_RESULT_REV_PARAM_ERROR	-6
#define CAN_RESULT_DEV_DETACHED		-7 
#define CAN_RESULT_PARAM_ERROR		-8
#define CAN_RESULT_ADD_ERROR		-9
#define CAN_RESULT_BUSOFF_ERROR		-10
#define CAN_RESULT_PASSIVE_ERROR	-11
#define CAN_RESULT_ACTIVE_ERROR	    -12
#define CAN_RESULT_FRAME_ERROR	    -13
#define CAN_STOP	    -15
#define ECU_INBOOT	    -16
#define FINDDLL_ERROR	    -17
 #define CAN_DELAY		10//10
#define Nega_1       0x1
#define CHECKSUM_ERROR	    2

#define Nega_11	     0x11
#define Nega_12	     0x12
#define Nega_13	     0x13
#define Nega_22	     0x22
#define Nega_24	     0x24
#define Nega_31	     0x31
#define Nega_33	     0x33
#define Nega_35	     0x35
#define Nega_36	     0x36
#define Nega_37      0x37
#define Nega_73	     0x73
#define Nega_78	     0x78


#define CAN_SEND		0
#define CAN_RECEIVE		1
#define CANTOUSB		0
#define CANCARD2		1
#define CANCARDXL		2
#define ICAN			3
#define ES581			4
typedef struct _NTIME {
 
    WORD wHour;
    WORD wMinute;
    WORD wSecond;
    WORD wMilliseconds;
}ntime;
class CAN_protocol
{
public :
	CAN_protocol();
	


	int CAN_messageProc(CAN_msg *msg,unsigned char *buf);
	int CAN_DiagnosticSessionControl(U32 CANNo,U32 CANId,U8 mode);
	int CAN_ECUReset(U32 CANNo,U32 CANId);
	int CAN_SecurityAccess(U32 CANNo,U8 mode,U32 *seedOrKey,U8 *keylen);
	int CAN_CommunicationControl(U32 CANNo,U32 CANId,U8 controltype,U8 mode);
	int CAN_ControlDTCSetting(U32 CANNo,U32 CANId,U8 type);
	int CAN_ReadDataByIdentifier(U32 CANNo,U32 CANId,u32 type,CAN_ECUIdentification *Data);
	int CAN_DynamicallyDefineLocalIdentifier(U32 CANNo,U8 opCode,U8 *addr,int len);
	int CAN_WriteByDataIdentifier(U32 CANNo,U16 type,CAN_ECUIdentification *Data);

	int CAN_RoutineControl(U32 CANNo,U16 opCode,U8 ctrl,unsigned char *buf);
	int CAN_TransferData(U32 CANNo,U8 ctrl,U32 len);
	
	int CAN_RequestTransferExit(U32 CANNo);
	int CAN_TesterPresent(U32 CANNo);
	int CAN_RequestDownload(U32 CANNo,U8 type,U8 len,U32 *Addr,U32 *Size);
	int CAN_ClearDiagnosticInformation(U32 CANNo);
	int CAN_Writeid(U32 CANNo,U16 type,char *buffer,char len);
	////////////////////////////
	int CAN_protocol::RecordCommand(char mode,unsigned char *data,long id,int result,char Iswrite);
	
	int CAN_messageInteract(U32 CANNo,pCAN_msg msg,unsigned char *PIDData); 
	XLstatus CAN_messageInteractCancardxl(unsigned int channel,XLevent *xlEvent,unsigned char *PIDData);
	int CAN_messageInteractCancard2(unsigned int channel,pCAN_msg msg,unsigned char *PIDData);
	////////////////////////////////
	 int CAN_protocol::CAN_SendDataCancard2(unsigned int channel,pCAN_msg msg);
	int CAN_protocol::CAN_ReadDataCancard2(unsigned int channel,param_struct *paramRec);
	int CAN_protocol::CAN_ReadDataUSB2CAN(unsigned int channel,pCAN_msg msg);
	int CAN_protocol::CAN_SendDataCancardxl(unsigned int channel,XLevent *xlEvent);
	int CAN_protocol::CAN_ReadDataCancardxl(unsigned int channel,XLevent *xlEvent);

	int CAN_protocol::CAN_CommandMessageProc(CAN_msg *msg,int *mulnum);
	int CAN_protocol::CAN_CommandmessageInteract(pCAN_msg msg,int *mulnum);
	int CAN_protocol::CAN_DisplayCommand(CAN_msg *canMsg,int result);
	int CAN_protocol::CAN_ReadData(U32 CANNo,U32 CANId,U32 type,unsigned char *buf);
 	int CAN_protocol::CAN_ClearNv(U32 CANNo);
	int writetype;
	int CAN_protocol::CAN_ReadVehicleInfor(U32 CANNo,U32 CANId,U32 type,unsigned char *buf);
	int CAN_protocol::CAN_ReadDTC(U32 CANNo,U8 ctrl,unsigned char *buf);
	int CAN_protocol::CAN_messageInteractICAN(unsigned int channel,pCAN_msg msg,unsigned char *PIDData);
	 int CAN_protocol::LoadIcan();
	  int CAN_protocol::InitICAN( );
	  int CAN_protocol::CAN_ReadDataICAN(unsigned int channel,int *readNum);
	int CAN_protocol::CAN_SendDataICAN(unsigned int channel,int sendNum);
	int CAN_protocol::InitICANDrv(unsigned int type);
	int CAN_protocol::CAN_messageInteractES581(icsSpyMessage *pMessagesTx,unsigned char *PIDData);
	int CAN_protocol::CAN_SendDataES581(icsSpyMessage *pMessagesTx,int sendNum);
	    double dTime;
		int CAN_protocol::CAN_ReadDataES581(icsSpyMessage *pMessagesTx,int *readNum);
};
#endif