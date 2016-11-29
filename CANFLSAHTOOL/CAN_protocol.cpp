/*----------------------------------------------------------------------------
| File        : CAN_protocol.cpp
| Project     : CANFlashtool 
|
| Description :  Processing  CAN message commands via different CAN card driver
|-----------------------------------------------------------------------------
| $Author: liaoshaoyan      $Revision:  
| $Data    2014.04.09   
|-----------------------------------------------------------------------------
| Copyright (c) 2014 by liaoshaoyan.  All rights reserved.
|---------------------------------------------------------------------------*/
#pragma once
#include "stdafx.h"
#include <string.h>
#include <stdio.h>  
#include "Global.h"
#include "CAN_protocol.h"
#include "AN_Flash.h"
#include "ICAN.h"
XLstatus gstatus;
XLstatus bstatus;
int resultx;
class CAN_Flash *flash;
CAN_msg msg_Arr[600];
XLevent xlEvent_Arr[600];
ICAN_EVENT IEvent_Arr[200];
ICAN_EVENT gCanObj[200];
icsSpyMessage gMsg_ES581[20000];
U32 CANNo=1;
//WORD Recordnum=0;
unsigned int xlChannel=0;
extern int Flash_Result; 
unsigned __int64 xltime;
extern	SYSTEMTIME pcStartTime;
int m_DevType=1;
int m_DevIndex=0;
//int m_nCanIndex=0;

/*0x10 01 02 03*/
CAN_protocol::CAN_protocol()
{
	//???????????
}
 int CAN_protocol::RecordCommand(char mode,unsigned char *data,long id,int result,char Iswrite)
{
	 
	SYSTEMTIME pcTime;
	float ptime;
	CString strPCtime;
	GetLocalTime(&pcTime);
	CAN_interactMsg *interactmsg=new CAN_interactMsg;
		
	if(g_CANDirverType == 0 || g_CANDirverType == 3)//|| g_CANDirverType == 4
	{
		float ms=(pcTime.wMilliseconds-pcStartTime.wMilliseconds);//(float)
		//ms = ms/1000;
		ptime = (float)((pcTime.wHour-pcStartTime.wHour)*3600+(pcTime.wMinute-pcStartTime.wMinute)*60+(pcTime.wSecond-pcStartTime.wSecond))*1000+ms;
		if(pcStartTime.wHour > pcTime.wHour)
		ptime += 24*3600*1000;
	}
	else if(g_CANDirverType == 1)ptime  = (float)xltime/1000;//
	else if(g_CANDirverType == 2)
	{
		ptime  = (float)xltime/1000000;///1000
	}
	else if(g_CANDirverType == 4)
	{
		ptime  = (float)dTime*1000;///1000
	}
	interactmsg->opTime=ptime;
 	//Recordnum++;
	//interactmsg->serialNo=Recordnum;
	interactmsg->MsgState=mode;
	interactmsg->Iswrite = Iswrite;
 
 	if(result != CAN2USB_RESULT_TIMEOUT)
	{
		sprintf_s(interactmsg->data,"%4X  %02X   ",id,8);		 
		for(int i=0;i<8;i++)//msg->len
		{
			sprintf_s(&interactmsg->data[11+3*i],200,"%02X ",data[i]);			
		}
		sprintf_s(&interactmsg->data[35],200,"\r\n");
		interactmsg->data[37]=0;
	}
	displayArray[listItemNum]=interactmsg;
 	putInteractMsg(interactmsg);
 
	listItemNum++;
 	return 1;
}
 int CAN_protocol::CAN_messageProc(CAN_msg *msg,unsigned char *buf)
{
	int dwReNum;
	int i;
	param_struct 	paramRec;
	unsigned int msgsrx=1;
	XLevent xlEvent;
	XLevent xlEvent_R;
	int rec_num;
	icsSpyMessage stMessagesTx;
	int result;
	if(g_StopFlag == 1){
			result = CAN_STOP;
			g_StopFlag = 0;
			return result;
		}
	switch(g_CANDirverType)
	{
		case 0:
			for(i=0;i<RENUM;i++)
			{
				result	=CAN_messageInteract(CANNo,msg,buf);
				if(result != CAN2USB_RESULT_TIMEOUT)break;

			} 
			  
			for(i=0;i<20 && result == 0x78;i++)	//延时
			{ 
				flash->display (_T("Busy..."),0);
				result = CAN_ReadDataUSB2CAN(CANNo,msg);
				if(result != CAN_RESULT_OK)break;
 				//if(resultx == 0x78)break;
 				if(msg->data[1] == 0x7F)
				{
					result = msg->data[3];
 				}
				else result=0;
				//Sleep(1000);
			}
			break;
		case 1:
			for(i=0;i<RENUM;i++)
			{
				result	=CAN_messageInteractCancard2(xlChannel,msg,buf);
				if(result != CAN2USB_RESULT_TIMEOUT)break;//
				//Sleep(1000);
 			} 			
			for(i=0;i<20 && result == 0x78;i++)	//延时
			{ 
				flash->display(_T("Busy..."),0);//1字体
				result = CAN_ReadDataCancard2(xlChannel,&paramRec);
				if(result != CAN_RESULT_OK)break;
				if(paramRec.RCV_data[1] == 0x7F)
				{
					result = paramRec.RCV_data[3];//0X78
 				}
				else result=0;
				memcpy(msg->data,paramRec.RCV_data,8);
			}
			break;
		case 2:
			xlEvent.tag                 = XL_TRANSMIT_MSG;
			xlEvent.tagData.msg.id      = msg->id;
			xlEvent.tagData.msg.dlc     = msg->len;
			xlEvent.tagData.msg.flags   = 0;
			
			memcpy(xlEvent.tagData.msg.data,msg->data,8);
			for(i=0;i<RENUM;i++)
			{
				result	=CAN_messageInteractCancardxl(xlChannel,&xlEvent,buf);
				if(result != CAN2USB_RESULT_TIMEOUT)break;
				//Sleep(1000);
			} 
			memcpy(msg->data,xlEvent.tagData.msg.data,8);
			for(i=0,msgsrx=1;i<20 && result == 0x78;i++)	//延时
			{ 
				flash->display (_T("Busy..."),0);
				result = CAN_ReadDataCancardxl(xlChannel,&xlEvent_R);
				if(result != CAN_RESULT_OK)break;
 				if(xlEvent_R.tagData.msg.data[1] == 0x7F)
				{
					result = xlEvent_R.tagData.msg.data[3];//78
 				}
				else result=0;
				memcpy(msg->data,xlEvent_R.tagData.msg.data,8);
			}	
 				
			break;
		case 3:
 
   			for(i=0;i<RENUM;i++)
			{
				result	=CAN_messageInteractICAN(xlChannel,msg,buf);
				if(result != CAN2USB_RESULT_TIMEOUT)break;
 			} 
			for(i=0,msgsrx=1;i<20 && result == 0x78;i++)	//延时
			{ 
				flash->display (_T("Busy..."),0);
				dwReNum=1;
				//Sleep(2000);
				result = CAN_ReadDataICAN(xlChannel,&dwReNum);
  				if(result != CAN_RESULT_OK)break;
				if(gCanObj[0].Data[1] == 0x7F)
				{
					result = gCanObj[0].Data[3];//78
 				}
				else result=0;
				if(dwReNum == 2)
				{
					if(gCanObj[1].Data[1] != 0x7f)
					{
						result = 0;//78
 						break;
 					}
					else
					{
						result = gCanObj[1].Data[3];
						memcpy(&gCanObj[0].Data,&gCanObj[1].Data,8);
 					}
					 
				}
				memcpy(msg->data,gCanObj[0].Data,8);
			}	
			
			break;
		case 4:
			//stMessagesTx.                = XL_TRANSMIT_MSG;
			stMessagesTx.ArbIDOrHeader      = msg->id;
			stMessagesTx.NumberBytesData     = msg->len;
			stMessagesTx.StatusBitField = 0;
			stMessagesTx.StatusBitField2 = 0;
			
			memcpy(stMessagesTx.Data,msg->data,8);
			for(i=0;i<RENUM;i++)
			{
			    result = CAN_messageInteractES581(&stMessagesTx,buf);
				if(result != CAN2USB_RESULT_TIMEOUT)break;
				
 			} 
			memcpy(msg->data,stMessagesTx.Data,8);
			for(i=0,msgsrx=1;i<20 && result == 0x78;i++)	//延时
			{ 
				flash->display (_T("Busy..."),0);
				dwReNum=1;
				//Sleep(2000);
				rec_num=1;
				result = CAN_ReadDataES581(&stMessagesTx,&rec_num);
  				if(result != CAN_RESULT_OK)break;
				if(msg_Arr[0].data[1] == 0x7F)
				{
					result = msg_Arr[0].data[3];//78
 				}
				else result=0;
				if(rec_num == 2)
				{
					if(msg_Arr[1].data[1] != 0x7f)
					{
						result = 0;//78
					 
						//RecordCommand(CAN_RECEIVE,gCanObj[1].Data,gCanObj[1].ID,1,writetype);
						break;
 					}
					else
					{
						result = msg_Arr[1].data[3];
						memcpy(&msg_Arr[0].data,&msg_Arr[1].data,8);
						
					}
					 
				}
				memcpy(msg->data,&msg_Arr[0].data,8);
			}	
			
			break;
		default:
			break;
	}
			 
		if(g_StopFlag == 1){
			result = CAN_STOP;
			//g_StopFlag = 0;
			return result;
		}
 	return result;

}

/*0x3e 文档没有啊*/
int CAN_protocol::CAN_TesterPresent(U32 CANNo)
{
	int	result;
	CAN_msg	msg;
	unsigned char buf[20];

	if(g_CANDirverType == 2)xlChannel = 0;
	if(g_CANDirverType == 1)
	{
		if(Flash_Result < 0)
		{
		if(xlChannel==0)xlChannel=1;
				else xlChannel=0;
		}
	}
	if(g_CANDirverType == 3)xlChannel = 0;
	memset(msg.data,0xff,8);
	msg.id	=0x7E0;
	msg.data[0]	=0x02;
	msg.data[1]	=0x3e;  // PIC
	msg.data[2]	=0x00;  // PIC
	msg.len = 8;
	msg.ch  = 1;
	msg.format = CAN_ID_FORMAT_STANDARD;
	msg.type   = CAN_FRAME_TYPE_DATA;
 	result=CAN_messageProc(&msg,buf);
	if(result != 0)
	{
		if(g_CANDirverType == 2)
		{
			if(result == 1)m_CAN->CANGoOffBus();
			if(m_CAN->appChannel>1)
			{
				xlChannel=1;//9-4
  			}
				m_CAN->CANInit();
				m_CAN->CANGoOnBus(500000);
				m_CAN->CANSetFilter(0x700,0x7ff);
				result=CAN_messageProc(&msg,buf);

		}
		if(g_CANDirverType == 1)
		{
			CANPC_reinitialize();
			INIPC_close_board();
			m_CANcard->CANInitialize();
			m_CANcard->InitializeFIFO();
			CANPC_start_chip();
			if(xlChannel==0)xlChannel=1;
				else xlChannel=0;
			result=CAN_messageProc(&msg,buf);
		}
		if(g_CANDirverType == 3)
		{
		/*	if(xlChannel==0)xlChannel=1;
				else xlChannel=0;
			InitICANDrv(xlChannel);
			result=CAN_messageProc(&msg,buf);*/
		}
	}
 	return result;
}
int CAN_protocol::CAN_ReadVehicleInfor(U32 CANNo,U32 CANId,U32 type,unsigned char *buf)
{
	int	result;
	CAN_msg	msg;
 
	memset(msg.data,0xff,8);
	msg.id	= CANId;
	msg.data[0]	= 0x02;
	msg.data[1]	= 0x09;  // PIC
	msg.data[2]	= (U8)type ;
 	msg.len = 8;
	msg.ch  = (U8)CANNo;
	msg.format = CAN_ID_FORMAT_STANDARD;
	msg.type   = CAN_FRAME_TYPE_DATA;

	result=CAN_messageProc(&msg,buf);
	memcpy(buf,msg.data ,8);
 	return result;
}
//
int CAN_protocol::CAN_ReadData(U32 CANNo,U32 CANId,U32 type,unsigned char *buf)
{
	int	result;
	CAN_msg	msg;
 
	memset(msg.data,0xff,8);
	msg.id	= CANId;
	msg.data[0]	= 0x03;
	msg.data[1]	= 0x22;  // PIC
	msg.data[2]	= (type >> 8)&0xff;
	msg.data[3]	= type & 0xff;
	msg.len = 8;
	msg.ch  = (U8)CANNo;
	msg.format = CAN_ID_FORMAT_STANDARD;
	msg.type   = CAN_FRAME_TYPE_DATA;

	result=CAN_messageProc(&msg,buf);
 	return result;
}
int CAN_protocol::CAN_ReadDTC(U32 CANNo,U8 ctrl,unsigned char *buf)
{
	int	result;
	CAN_msg	msg;
	//char buf[200];

 	memset(msg.data,0xff,8);
	msg.id	=0x7e0;
	msg.data[0]	=0x03;
	msg.data[1]	=0x19;  // PIC
	msg.data[2]	=0x02;  // PIC
	msg.data[3]	=ctrl;
	msg.len = 8;
	msg.ch  = 1;
	msg.format = CAN_ID_FORMAT_STANDARD;
	msg.type   = CAN_FRAME_TYPE_DATA;

	result=CAN_messageProc(&msg,buf);
 	return result;
}

/* 10*/
int CAN_protocol::CAN_DiagnosticSessionControl(U32 CANNo,U32 CANId,U8 mode)
{
	int	result;
	CAN_msg	msg;
	unsigned char buf[20];
	
	memset(msg.data,0xff,8);
	msg.id	= CANId;
	msg.data[0] = 0x02; 
	msg.data[1] = 0x10;  // PIC
	msg.data[2] = mode;  
	msg.len = 8;
	msg.ch  = (U8)CANNo;
	msg.format = CAN_ID_FORMAT_STANDARD;
	msg.type   = CAN_FRAME_TYPE_DATA;
 
	result=CAN_messageProc(&msg,buf);

		return result;

}
/* 10 clear nv*/
int CAN_protocol::CAN_ClearNv(U32 CANNo)
{
	int	result;
	CAN_msg	msg;
	unsigned char buf[20];
	
	memset(msg.data,0xff,8);
	msg.id	= 0x7e0;
	msg.data[0] = 0x04; 
	msg.data[1] = 0x31;  // PIC
	msg.data[2] = 01;  
	msg.data[3] = 0xaa;  // PIC
	msg.data[4] = 0;  
	msg.len = 8;
	msg.ch  = (U8)CANNo;
	msg.format = CAN_ID_FORMAT_STANDARD;
	msg.type   = CAN_FRAME_TYPE_DATA;
 
	result=CAN_messageProc(&msg,buf);

		return result;

}

/* 0x11*/
int CAN_protocol::CAN_ECUReset(U32 CANNo,U32 CANId)
{
	int	result;
	CAN_msg	msg;
	unsigned char buf[20];

	memset(msg.data,0xff,8);
	msg.id	= CANId;
	msg.data[0]	= 0x02;
	msg.data[1]	= 0x11;  // PIC
	msg.data[2]	= 0x03;
	msg.len = 8;
	msg.ch  = (U8)CANNo;
	msg.format = CAN_ID_FORMAT_STANDARD;
	msg.type   = CAN_FRAME_TYPE_DATA;
	
	result=CAN_messageProc(&msg,buf);
	return result;
}
 /*0x27 mode */
int CAN_protocol::CAN_SecurityAccess(U32 CANNo,U8 mode,U32 *seedOrKey,U8 *keylen)
{
	int	result;
	CAN_msg	msg;
	unsigned char buf[20];

	memset(msg.data,0xff,8);
	if(mode % 2 != 0)	  //01
	{
		msg.id	= 0x7E0;
		msg.data[0]	= 0x02;
		msg.data[1]	= 0x27;  // PIC
		msg.data[2]	= mode;
		msg.len = 8;
		msg.ch  = (U8)CANNo;
		msg.format = CAN_ID_FORMAT_STANDARD;
		msg.type   = CAN_FRAME_TYPE_DATA;
	
		result=CAN_messageProc(&msg,buf);
		if(result != 0)return result;

		if(msg.data[0] == 0x04)
		{
 			*seedOrKey=((buf[1]&0XFF)<<8 )| (buf[2]&0XFF);
			*keylen=2;
 
		}
		else
		{
			*seedOrKey=((buf[1]&0XFF)<<24 )|((buf[2]&0XFF)<<16 )|((buf[3]&0XFF)<<8 )| (buf[4]&0XFF);
			*keylen=4;
			 
		}
	}
	else
	{
		msg.id	= 0x7E0;
		msg.data[0]	= *keylen+2;
		msg.data[1]	= 0x27;  // PIC
		msg.data[2]	= mode;
		if(*keylen == 2)
		{
		msg.data[3]= (*seedOrKey >> 8)&0xff;
		msg.data[4]=(*seedOrKey & 0xFF);
		}
		else
		{
			msg.data[3]=(*seedOrKey >> 24)&0xff;
			msg.data[4]=(*seedOrKey >> 16)&0xff;
			msg.data[5]=(*seedOrKey >> 8)&0xff;
			msg.data[6]=(*seedOrKey & 0xFF);
		}
		msg.len = 8;
		msg.ch  = (U8)CANNo;
		msg.format = CAN_ID_FORMAT_STANDARD;
		msg.type   = CAN_FRAME_TYPE_DATA;
 
		result=CAN_messageProc(&msg,buf);
	}
 
	return result;
}
/*0x28
controltype--enableRxAndTx:00 enableRxAndDisableTx；01 disableRxAndEnableTx：02 disableRxAndTx：03
  mode-- 001b application,010b networkManagementm,100b diagnostic    */
int CAN_protocol::CAN_CommunicationControl(U32 CANNo,U32 CANId,U8 controltype,U8 mode)
{
	int	result;
	CAN_msg	msg;
	unsigned char buf[20];

	memset(msg.data,0xff,8);
	msg.id	= CANId;
	msg.data[0]	= 0x03;
	msg.data[1]	= 0x28;
	msg.data[2]	= controltype;  // PIC
	msg.data[3]	= mode;   
	msg.len = 8;
	msg.ch  = (U8)CANNo;
	msg.format = CAN_ID_FORMAT_STANDARD;
	msg.type   = CAN_FRAME_TYPE_DATA;

	result=CAN_messageProc(&msg,buf);
	 
	return result;
}

/*0x85 on:01;off:02*/
int CAN_protocol::CAN_ControlDTCSetting(U32 CANNo,U32 CANId,U8 type)
{
	int	result;
	CAN_msg	msg;
	unsigned char buf[20];

	memset(msg.data,0xff,8);
	msg.id	= CANId;
	msg.data[0]	= 0x02;
	msg.data[1]	= 0x85;
	msg.data[2]	= type;  // PIC
	msg.len = 8;
	msg.ch  = (U8)CANNo;
	msg.format = CAN_ID_FORMAT_STANDARD;
	msg.type   = CAN_FRAME_TYPE_DATA;

	result=CAN_messageProc(&msg,buf);
	 
	return result;
}

/*0x22  f192:base-model number*/
int CAN_protocol::CAN_ReadDataByIdentifier(U32 CANNo,U32 CANId,u32 type,CAN_ECUIdentification *Data)
{
	int	result;
	CAN_msg	msg;
	unsigned char buf[30];
	memset(buf,0x0,30);
	memset(msg.data,0xff,8);
	msg.id	= CANId;
	msg.data[0]	= 0x03;
	msg.data[1]	= 0x22;  // PIC
	msg.data[2]	= (type >> 8)&0xff;
	msg.data[3]	= type & 0xff;
	msg.len = 8;
	msg.ch  = (U8)CANNo;
	msg.format = CAN_ID_FORMAT_STANDARD;
	msg.type   = CAN_FRAME_TYPE_DATA;

	result=CAN_messageProc(&msg,buf);
	//if(result != CAN2USB_RESULT_OK)
	//	return -(CAN_ERROR_SERVICE_PROCESS | (-result) | 0x02 <<8);
	unsigned int temp;
	temp = ((buf[0]&0xff)<<8) | buf[1]&0xff;
	int i;
	i=2;
	if(temp == type)i=2;
	else i=0;

	switch(type)
	{
		case 0xF190:
 			memcpy(Data->VIN,&buf[i],17); //Vehicle Identification Number
			break;
		case 0xF192:
			memcpy(Data->BaseModelNumber,&buf[i],4);
			break;
		case 0xF194:
			memcpy(Data->SWNumber,&buf[i],8);
			break;
		case 0xF195:
			memcpy(Data->SWName,&buf[i],10);
			break;
		case 0xF198:
			memcpy(Data->TesterSerialNumber,&buf[i],10);
			break;
		case 0xF199:
			memcpy(Data->ProgrammingReflashDate,&buf[i],4);
			break;
		case 0xF1B0:
			memcpy(Data->EndModelNumber,&buf[i],4);
			break;
		case 0x01A0:
			memcpy(&Data->CrankshaftCounter,&buf[i],1);
			break;
		case 0xF1B2:
			memcpy(&Data->BootReason,&buf[i],1);
			break;
		case 0xF18c:
			memcpy(&Data->SerialNum,&buf[i],20);
			break;
		case 0xF2B1:
			memcpy(&Data->CALID,&buf[i],16);
			break;
		case 0xF1F0:
			memcpy(&Data->dllkeyword,&buf[i],3);
			break;

		default:break;
	}
	return result;
}
/*0x2e */
int CAN_protocol::CAN_WriteByDataIdentifier(U32 CANNo,U16 type,CAN_ECUIdentification *Data)
{
	int	result;
	CAN_msg	msg;
	unsigned char buf[30];
	U16 len;

	memset(msg.data,0xff,8);
	switch(type)
	{
		case 0xF190:
			memcpy(&buf[3],Data->VIN,17); //Vehicle Identification Number
			memcpy(&msg.data[5],Data->ProgrammingReflashDate,3);
			len = 17;
			break;
		case 0xF198:
			memcpy(&buf[3],Data->TesterSerialNumber,10);
			memcpy(&msg.data[5],Data->ProgrammingReflashDate,3);
			len=10;
			break;
		case 0xF199:
			memcpy(&buf[4],Data->ProgrammingReflashDate,4);
			len=4;
			break;
		default:break;
	}
	if(len < 7)
	{
		msg.id	=0x7E0;
		msg.data[0]	= 0x07;
		msg.data[1]	= 0x2E;  // PIC
		msg.data[2]	= (type >> 8)& 0xff;  
		msg.data[3]	= type& 0xff; 
		memcpy(&msg.data[4],Data->ProgrammingReflashDate,4);
		msg.len = 8;
		msg.ch  = (U8)CANNo;
		msg.format = CAN_ID_FORMAT_STANDARD;
		msg.type   = CAN_FRAME_TYPE_DATA;

		result=CAN_messageProc(&msg,buf);
	}
	else
	{
		len+=3;
		msg.id	=0x7E0;
		msg.data[0]	= 0x10 | ((len >> 8)&0xff);
		msg.data[1]	= len&0xff;  // PIC
		msg.data[2]	= 0x2E;  // PIC
		msg.data[3]	= (type >> 8)& 0xff;  
		msg.data[4]	= type& 0xff; 
		
		msg.len = 8;
		msg.ch  = (U8)CANNo;
		msg.format = CAN_ID_FORMAT_STANDARD;
		msg.type   = CAN_FRAME_TYPE_DATA;

		result=CAN_messageProc(&msg,buf);

	}

	return result;
}
int CAN_protocol::CAN_Writeid(U32 CANNo,U16 type,char *buffer,char len)
{
	int	result;
	CAN_msg	msg;
	unsigned char buf[30];

	memset(msg.data,0xff,8);
	if(len > 7)
	{
 		msg.id	=0x7E0;
		msg.data[0]	= 0x10 | ((len >> 8)&0xff);
		msg.data[1]	= len;  // PIC
		msg.data[2]	= 0x2E;  // PIC
		msg.data[3]	= (type >> 8)& 0xff;  
		msg.data[4]	= type& 0xff; 
		memcpy(&msg.data[5],buffer,3);
		memcpy(&buf[5],&buffer[3],7);
		msg.len = 8;
		msg.ch  = (U8)CANNo;
		msg.format = CAN_ID_FORMAT_STANDARD;
		msg.type   = CAN_FRAME_TYPE_DATA;

		result=CAN_messageProc(&msg,buf);

	}

	return result;
}
/*0x14*/
int CAN_protocol::CAN_ClearDiagnosticInformation(U32 CANNo)
{
	int	result;
	CAN_msg	msg;
	unsigned char buf[20];

	memset(msg.data,0xff,8);
	msg.id	=0x7E0;
	msg.data[0]	=0x04;
	msg.data[1]	=0x14;  // PIC
	msg.data[2]	=0xff;
	msg.data[3]	=0xff;
	msg.data[4]	=0xff;
 	msg.len = 8;
	msg.ch  = (U8)CANNo;
	msg.format = CAN_ID_FORMAT_STANDARD;
	msg.type   = CAN_FRAME_TYPE_DATA;
 
	result=CAN_messageProc(&msg,buf);
	return result;
}
/*  0x31  */
int CAN_protocol::CAN_RoutineControl(U32 CANNo,U16 opCode,U8 ctrl,unsigned char *buf)
{
	int	result;
	CAN_msg	msg;
	//char buf[20];
	int i=0;

	msg.id	=0x7E0;
	memset(msg.data,0xff,8);
	msg.data[1]	=0x31;  // PIC
	msg.data[2]	=0x01;  
	msg.data[3]	=(opCode>>8)&0xff;  
	msg.data[4]	=opCode&0xff;  

	if(opCode == 0xff00)
	{
		msg.data[0]	=0x05;
		msg.data[5]	=ctrl;  //Erase application + cal flash
		msg.len = 8;
	}
	else
	{
		msg.data[0]	=0x04;
		msg.len = 8;
	}
	msg.ch  = (U8)CANNo;
	msg.format = CAN_ID_FORMAT_STANDARD;
	msg.type   = CAN_FRAME_TYPE_DATA;

	result=CAN_messageProc(&msg,buf);
	memcpy(buf,msg.data ,8);
	return result;
}
/*0x34
dataFormatIdentifier：压缩  加密  0x00
addressAndLengthFormatIdentifier:addr and memory size
*/
int CAN_protocol::CAN_RequestDownload(U32 CANNo,U8 type,U8 len,U32 *Addr,U32 *Size)
{
	int	result;
	CAN_msg	msg;
	unsigned char buf[20];
	int i=0;

	memset(msg.data,0xff,8);
	msg.id	=0x7E0;
	msg.data[0]	= 0x10;
	msg.data[1]	= 0x0B;
	msg.data[2]	= 0x34;  // PIC
	msg.data[3]	= type;  
	msg.data[4]	= len; //addr and memory size
	msg.data[5]	= (*Addr >> 24)&0xff;  
	msg.data[6]	= (*Addr >> 16)&0xff; 
	msg.data[7]	= (*Addr >> 8)&0xff;
	msg.len = 8;
	msg.ch  = (U8)CANNo;
	msg.format = CAN_ID_FORMAT_STANDARD;
	msg.type   = CAN_FRAME_TYPE_DATA;
	buf[2] = (*Addr >> 24)&0xff; 	 //注意，不能用memcyp()函数
	buf[3] = (*Addr >> 16)&0xff; 
	buf[4] = (*Addr >> 8)&0xff;
	buf[5] = *Addr &0xff;
	buf[6] = (*Size >> 24)&0xff; 
	buf[7] = (*Size >> 16)&0xff;
	buf[8] = (*Size >> 8)&0xff;
	buf[9] = *Size&0xff;

	result=CAN_messageProc(&msg,buf);

	return result;
}
/* 0x36 uncomplete*/
int CAN_protocol::CAN_TransferData(U32 CANNo,U8 ctrl,U32 len)
{
	int	result;
	CAN_msg	msg;
 	U16 lenth;
	
	lenth=(U16)len+2;
	memset(msg.data,0xff,8);
	msg.id	=0x7E0;
	msg.data[0]	=(0x10 | (lenth>>8));	//
	msg.data[1]	=lenth&0xff;	//
	msg.data[2]	=0x36;  // PIC
	msg.data[3]	=ctrl;  // PIC
	memcpy(&msg.data[4],&cut_data[1],4);
	msg.len = 8;
	msg.ch  = 1;
	msg.format = CAN_ID_FORMAT_STANDARD;
	msg.type   = CAN_FRAME_TYPE_DATA;

	result=CAN_messageProc(&msg,cut_data);

	return result;
}


/*0x37
opcode=0x00,recieve=0xf2+0xd4=454,allDTC=0X95,size=0x96*3=450
the ECU send max size is 250,which need to send twice service command
mode 0x81 0x86 0x87*/
int CAN_protocol::CAN_RequestTransferExit(U32 CANNo)
{
	int	result;
	CAN_msg	msg;
	unsigned char buf[20];
 
	memset(msg.data,0xff,8);
	msg.id	=0x7E0;
	msg.data[0]	=0x01;
	msg.data[1]	=0x37;  // PIC
	msg.len = 8;
	msg.ch  = (U8)CANNo;
	msg.format = CAN_ID_FORMAT_STANDARD;
	msg.type   = CAN_FRAME_TYPE_DATA;

	result=CAN_messageProc(&msg,buf);

	return result;
}

int CAN_protocol::CAN_ReadDataUSB2CAN(unsigned int channel,pCAN_msg msg)
{
	
	int result;
	//pCAN_Rxmsg pRxmsg;
	LARGE_INTEGER lgPre,lgCur,lgFrq;
	QueryPerformanceFrequency(&lgFrq);
	double dqFreq;
	dqFreq = (double)lgFrq.QuadPart;
	QueryPerformanceCounter(&lgPre);
	int n=config->ECUprg.Delay_Time_seconds;//
	if(n<=0)n=2;
	if(msg->data[1] == 0x3e)n=1;
			
	while(1)
	{
			if(g_usb2canFlag == 0){
			resultx = CAN_RESULT_SYSTEM_ERROR;
			return resultx;
		}	
		if(g_StopFlag == 1){
			result = CAN_STOP;
			//g_StopFlag = 0;
			break;
		}
		result = CAN2USB_readCAN(msg,50);//500ms
		if(msg->id == 0x7e8 && result == CAN_RESULT_OK && msg->ch == channel)break; 

		QueryPerformanceCounter(&lgCur);
		if((lgCur.QuadPart-lgPre.QuadPart)/dqFreq > n)break;//1  3s

	}	
	if(result == CAN_RESULT_OK)
	{
		if(msg->id == 0x7e8 && msg->ch == channel)
		{   
			RecordCommand(CAN_RECEIVE,&msg->data[0],msg->id,1,writetype);
		}
		else result = CAN_RESULT_TIMEOUT;
	}

	return result;

}

int CAN_protocol::CAN_messageInteract(U32 CANNo,pCAN_msg msg,unsigned char *PIDData)
{
	char serviceId;
	CAN_msg msg_recv,msg_send;
	U8 framenumth;
	U16 mulframe;
	int addframe;
	int	result=0;
	int	i,k,m;
	WORD j;
	WORD sendnumi=0;
	int errornum=0;
	int recelen,len;
	WORD framenum;
 	if(g_StopFlag == 1){
			resultx = CAN_STOP;
			//g_StopFlag = 0;
			return resultx;
		}	
	if(g_usb2canFlag == 0){
			resultx = CAN_RESULT_SYSTEM_ERROR;
			return resultx;
		}	
	if(msg->data[2]==0x36 ){ writetype=1;}
		else { writetype=0;}
	result=0;
	if(start_clear==1) 
	{
		for(i=0;i<3000 && result ==0;i++)  //先清空缓存
			result = CAN2USB_readCAN(&msg_recv,1);
		//start_clear=0;
	}
	for(i=0;i<3;i++)
	{
			if(g_usb2canFlag == 0){
			resultx = CAN_RESULT_SYSTEM_ERROR;
			return resultx;
		}
		result = CAN2USB_writeCAN(msg,20);	
		if(result == CAN2USB_RESULT_OK)break;
	}
	if(result != CAN2USB_RESULT_OK)return result; 
	RecordCommand(CAN_SEND,msg->data,msg->id,1,writetype);
	memcpy(&msg_recv,msg,sizeof(CAN_msg));
	result = CAN_ReadDataUSB2CAN(CANNo,&msg_recv);
	if(result != CAN2USB_RESULT_OK)return result; 

	mulframe=msg_recv.data[0]&0XF0;
	if(mulframe==0x10)		//first frame 10   	receive	 多帧接受
	{
		recelen=(msg_recv.data[0] & 0X0F)*256 + msg_recv.data[1];
		if((recelen-6)%7==0)framenum=(recelen-6)/7;
		else framenum=(recelen-6)/7+1;
		serviceId=msg_recv.data[2]; //SID

		for(i=0,j=3;j<8;i++,j++)
			PIDData[i]=msg_recv.data[j];

		memset(&msg_send.data,0xff,8);
		for(i=0;i<10;i++)
		{
			msg_send.id	=0x7E0;
			msg_send.data[0]	=0x30;  // PIC
			msg_send.data[1]	=0x00;  // Model 1
			msg_send.data[2]	=0x00;  // Model 1
			msg_send.len = 8;
			msg_send.ch  = (U8)CANNo;
			msg_send.format = CAN_ID_FORMAT_STANDARD;
			msg_send.type   = CAN_FRAME_TYPE_DATA;
	
			result = CAN2USB_writeCAN(&msg_send,100); 
			if(result != CAN2USB_RESULT_OK)
			{
				errornum=1;
			RecordCommand(0,msg_send.data,msg_send.id,1,0);
			continue;
			}
			addframe=0;
			for(j=0;j<framenum && errornum==0;j++)
			{
				result = CAN_ReadDataUSB2CAN(CANNo,&msg_recv);
				//result =CAN2USB_readCAN(&msg_recv,300);
				//RecordCommand(1,msg_recv.data,msg_recv.id,1);
				if(result == CAN2USB_RESULT_OK)
				{
					framenumth=msg_recv.data[0]&0x0F;
					for(m=1,k=7*(framenumth+(addframe*16)-1)+5;m<8;m++,k++)
					{
					   PIDData[k] = msg_recv.data[m];
					}
					if(framenumth == 0x0F)
					   	addframe++;
				}
				else 
				{
					errornum=1;
					break;
				}
			}
			if(errornum==0)break;
		}	
		if(errornum==1)
			{
				 result= CAN2USB_RESULT_TIMEOUT;
				return result;
		}
	}
	else if(mulframe==0x30)	   //1k 146 message	 发送多帧
	{
		recelen=(msg->data[0]&0X0F)*256+msg->data[1];
		framenum=(recelen-6)/7;
 		PIDData+=5;				  //去掉第一帧的5个字节
		m=0x21;
		//char btype;
 
		for(j=0;j<framenum;j++)
		{
			msg_Arr[j].data[0]=m;
			memcpy(&msg_Arr[j].data[1],PIDData,7);	
			msg_Arr[j].id	=0x7E0;
			msg_Arr[j].len = 8;
			msg_Arr[j].ch  = (U8)CANNo;
			msg_Arr[j].format = CAN_ID_FORMAT_STANDARD;
			msg_Arr[j].type   = CAN_FRAME_TYPE_DATA;
				
			PIDData+=7;
			m++;
			if(m==0x30)m=0x20;		
			RecordCommand(0,msg_Arr[j].data,msg_Arr[j].id,1,writetype);	
		}
		len= (recelen-6)%7;
		if(len!=0)
		{
			memset(&msg_Arr[j].data,0xff,8);
			msg_Arr[j].id	=0x7E0;
			msg_Arr[j].data[0] = m;  // PIC
			memcpy(&msg_Arr[j].data[1],PIDData,len);
			msg_Arr[j].len = 8;
			msg_Arr[j].ch  = (U8)CANNo;
			msg_Arr[j].format = CAN_ID_FORMAT_STANDARD;
			msg_Arr[j].type   = CAN_FRAME_TYPE_DATA;
			framenum++;
			RecordCommand(0,msg_Arr[j].data,msg_Arr[j].id,1,writetype);
		}
		result = CAN2USB_writeCAN_highspeed(msg_Arr,framenum,&sendnumi,10*1000);
 		if(result != CAN2USB_RESULT_OK)
			return result;

		result = CAN_ReadDataUSB2CAN(CANNo,&msg_recv);
		if(result != CAN2USB_RESULT_OK)
			return result; 
		
		serviceId=msg_recv.data[1];
		recelen=msg_recv.data[0]&0X0F;
		for(j=2,k=0;j<recelen+1;j++,k++)				  // SID PID1
			PIDData[k] = msg_recv.data[j]; 
	}
	else 
	{
		serviceId=msg_recv.data[1];
		recelen=msg_recv.data[0]&0X0F;
		for(j=2,k=0;j<recelen+1;j++,k++)				  // SID PID1
			PIDData[k] = msg_recv.data[j]; 
	}
	if(serviceId == 0x7F)
	{
		//memcpy(msg,&msg_recv,sizeof(CAN_msg));
		if(recelen < 3)
	 		 return CAN_ERROR_RESP_ARG_ABSENT;
		//if(msg_recv.data[2] == msg->data[1])result = PIDData[1];
		//else result = PIDData[0]; //	msg_recv.data[3]
		result = PIDData[1];
	}
	memcpy(msg,&msg_recv,sizeof(CAN_msg));
 
	return result;
}
	
int CAN_protocol::CAN_ReadDataCancard2(unsigned int channel,param_struct *paramRec)
{
	LARGE_INTEGER lgPre,lgCur,lgFrq;
	QueryPerformanceFrequency(&lgFrq);
	double dqFreq;
	dqFreq = (double)lgFrq.QuadPart;
	QueryPerformanceCounter(&lgPre);
	int n=config->ECUprg.Delay_Time_seconds;
	if(paramRec->RCV_data[1] == 0x3e)n=1;

	while(1)
	{
		if(g_StopFlag == 1){
			resultx = CAN_STOP;
			//g_StopFlag = 0;
			break;
		}	
		resultx =  CANPC_read_ac( paramRec );
		if(resultx == CANPC_RA_DATAFRAME)
		{
			if(paramRec->Can == channel+1)
			{
				xltime= paramRec->Time ;
				RecordCommand(CAN_RECEIVE,paramRec->RCV_data,paramRec->Ident ,1,writetype);
				return CAN_RESULT_OK;
			}
		}
		QueryPerformanceCounter(&lgCur);
		if((lgCur.QuadPart-lgPre.QuadPart)/dqFreq >n)break;
	}
	switch(resultx)
	{
	case CANPC_RA_NO_DATA:
		resultx = CAN_RESULT_TIMEOUT;
		break;
	case CANPC_RA_DATAFRAME:
		resultx = CAN_RESULT_TIMEOUT;
		break;
	case CANPC_RA_TX_DATAFRAME:
		resultx = CAN_RESULT_TIMEOUT;
		break;
	case CANPC_RA_CHG_BUS_STATE:
		resultx = CAN_RESULT_TIMEOUT;//CAN_RESULT_NO_DEVICE
		break;
	case CANPC_RA_ERRORFRAME:
		resultx = CAN_RESULT_REV_PARAM_ERROR;
		break;
	case CANPC_RA_ERR_ADD:
		resultx = CAN_RESULT_ADD_ERROR;
		break;
	default:
		resultx = CAN_RESULT_TIMEOUT;
		break;
	}				 
	return resultx;

}
int CAN_protocol::CAN_SendDataCancard2(unsigned int channel,pCAN_msg msg)
{
// int result;
	param_struct 	paramRec;
 	
	for(int i=0;i<10;i++)
	{
		if(channel==0)resultx = CANPC_send_data(msg->id, msg->format, msg->len, msg->data);//id std len 
		else resultx = CANPC_send_data2(msg->id, msg->format, msg->len, msg->data);//id std len 
		if(resultx == CANPC_OK)break; 
	}
	for(int i=0;i<10;i++)
	{
		for(int i=0;i<100;i++)
		{
			resultx =  CANPC_read_ac( &paramRec );
			if(resultx!=CANPC_RA_NO_DATA)break;
		}
		if(resultx == CANPC_RA_TX_DATAFRAME || resultx == CANPC_RA_CHG_BUS_STATE 
			|| resultx == CANPC_RA_ERRORFRAME || resultx == CANPC_RA_ERR_ADD)break;
		Sleep(1);
	}

	switch(resultx)
	{
	case CANPC_RA_NO_DATA:
		resultx = CAN_RESULT_TIMEOUT;
		break;
	case CANPC_RA_TX_DATAFRAME:
		xltime= paramRec.Time ;
		RecordCommand(CAN_SEND,paramRec.RCV_data,paramRec.Ident,1,writetype);
		resultx = CAN_RESULT_OK;
		break;
	case CANPC_RA_CHG_BUS_STATE:
		resultx =CAN_RESULT_TIMEOUT ;//CAN_RESULT_NO_DEVICE
		break;
	case CANPC_RA_ERRORFRAME:
		resultx = CAN_RESULT_NO_DEVICE;//
		break;
	case CANPC_RA_ERR_ADD:
		resultx = CAN_RESULT_ADD_ERROR;
		break;
	default:
		
		resultx = CAN_RESULT_TIMEOUT;
		break;
	}				 
	return resultx;
}
int CAN_protocol::CAN_messageInteractCancard2(unsigned int channel,pCAN_msg msg,unsigned char *PIDData)
{
	char serviceId;
	U8 framenumth;
	U16 mulframe;
	int addframe; 
	int	i,k,m;
	WORD j;
	int recelen,recelenh,recelenl,len;
	WORD framenum;
	int result;
	param_struct 	paramRec;
	CAN_msg msg_send;
	if(g_StopFlag == 1){
			resultx = CAN_STOP;
			//g_StopFlag = 0;
			return resultx;
		}	
		if(msg->data[2]==0x36 ){writetype=1;}
		else {writetype=0;}

	//for(int i=0;i<100;i++)
	//	{
	//		resultx =  CANPC_read_ac( &paramRec );
	//		if(resultx==CANPC_RA_NO_DATA)break;//(resultx!=CANPC_RA_NO_DATA)
	//	}
	
	result = CAN_SendDataCancard2(channel,msg);
	if(result != CAN_RESULT_OK)return result;
	memcpy(paramRec.RCV_data,msg->data ,8); 
	result = CAN_ReadDataCancard2(channel,&paramRec);
	if(result != CAN_RESULT_OK)return result;

	recelenh=paramRec.RCV_data[0];
	mulframe=recelenh&0XF0;
	if(mulframe==0x10)		//first frame 10   	receive	 多帧接受
	{
		recelenl=paramRec.RCV_data[1];	 //message len
		recelen=(recelenh&0X0F)*256+recelenl;
		if((recelen-6)%7==0)framenum=(recelen-6)/7;
		else framenum=(recelen-6)/7+1;
		serviceId=paramRec.RCV_data[2]; //SID

		for(i=0,j=3;j<8;i++,j++)
			PIDData[i]=paramRec.RCV_data[j];//第一帧数据

		memset(&msg_send.data,0xff,8);
		msg_send.id	= 0x7E0;
		msg_send.data[0] = 0x30;  // PIC
		msg_send.data[1] = 0x00;  // Model 1
		msg_send.data[2] = 0x00;  // Model 1
		msg_send.len = 8;
		msg_send.format = CAN_ID_FORMAT_STANDARD;
	
		result = CAN_SendDataCancard2(channel,&msg_send);
		if(result != CAN_RESULT_OK)return result;
		addframe=0;
		for(j=0;j<framenum;j++)
		{
 			result = CAN_ReadDataCancard2(channel,&paramRec);
			if(result != CAN_RESULT_OK)return result;
			else
			{
				framenumth=paramRec.RCV_data[0]&0x0F;
				for(m=1,k=7*(framenumth+(addframe*16)-1)+5;m<8;m++,k++)
				{
					PIDData[k] = paramRec.RCV_data[m];//后续帧数据
				}
				if(framenumth==0x0F)
					addframe++;
			}
		}
	}
	else if(mulframe==0x30)	   //1k 146 message	 发送多帧
	{
		recelenl=msg->data[1];	 //message len
		recelenh=msg->data[0];
		recelen=(recelenh&0X0F)*256+recelenl;
		framenum=(recelen-6)/7;
		PIDData+=5;				  //去掉第一帧的5个字节
		m=0x21;
		
		for(j=0;j<framenum;j++)
		{
			msg_Arr[j].data[0] = m;
			memcpy(&msg_Arr[j].data[1],PIDData,7);	
			msg_Arr[j].id	=0x7E0;
			msg_Arr[j].len = 8;
 			msg_Arr[j].format = CAN_ID_FORMAT_STANDARD;
 						 		
			result = CAN_SendDataCancard2(channel,&msg_Arr[j]);
			if(result != CANPC_OK)return result;
			PIDData+=7;
			m++;	
			if(m==0x30)m=0x20;
 		}
		len= (recelen-6)%7;
		if(len!=0)
		{
			memset(&msg_Arr[j].data,0xff,8);

			msg_Arr[j].id = 0x7E0;
			msg_Arr[j].data[0] = m;  // PIC
			memcpy(&msg_Arr[j].data[1],PIDData,len);
			msg_Arr[j].len = 8;
			msg_Arr[j].format = CAN_ID_FORMAT_STANDARD;
 						 
			result = CAN_SendDataCancard2(channel,&msg_Arr[j]);
			if(result != CANPC_OK)return result;
		}
		
 		result = CAN_ReadDataCancard2(channel,&paramRec);
		if(result != CANPC_OK)return result;

		serviceId=paramRec.RCV_data [1];
		recelen=paramRec.RCV_data [0]&0X0F;
		for(j=2,k=0;j<recelen+1;j++,k++)				  // SID PID1
			PIDData[k] = paramRec.RCV_data [j]; 
	}
	else 
	{
		serviceId=paramRec.RCV_data [1];
		recelen=paramRec.RCV_data [0]&0X0F;
		for(j=2,k=0;j<recelen+1;j++,k++)				  // SID PID1
			PIDData[k] = paramRec.RCV_data [j]; 
	}
	if(serviceId == 0x7F)
	{
		//memcpy(msg->data ,paramRec.RCV_data,8);
		if(recelen < 3)
	 		 return CAN_ERROR_RESP_ARG_ABSENT;
		//if(paramRec.RCV_data[2] == msg->data[1])return PIDData[1];
		//else return PIDData[0]; //	msg_recv.data[3]
		//if(paramRec.RCV_data[2] == msg->data[1])result = PIDData[1];
		//else result = PIDData[0]; //	msg_recv.data[3]
		result = PIDData[1];
	}
	memcpy(msg->data ,paramRec.RCV_data,8);

	return result;
	 
}
int CAN_protocol::CAN_ReadDataICAN(unsigned int channel,int *readNum)
{
	int dwReNum;
	int result;
	int index=0;
	//int time;
	LARGE_INTEGER lgPre,lgCur,lgFrq;
	QueryPerformanceFrequency(&lgFrq);
	double dqFreq;
	dqFreq = (double)lgFrq.QuadPart;
	QueryPerformanceCounter(&lgPre);

	//time = config->ECUprg.Delay_Time_seconds *200;//10s
	//if(config->ECUprg.Delay_Time_seconds <= 0)time=5*200;//5s
	//if(IEvent_Arr[0].Data[1] == 0x3e)time = 1 *200;//1s
	int n=config->ECUprg.Delay_Time_seconds;
	if(IEvent_Arr[0].Data[1] == 0x3e)n=1;
	if(config->ECUprg.Delay_Time_seconds <= 0)n=2;
 
 	while(1)
	{
		if(g_StopFlag == 1){
			resultx = CAN_STOP;
			//g_StopFlag = 0;
			return resultx;
		}	
 		dwReNum = ICANReceive( m_DevType, m_DevIndex,channel,&gCanObj[index],50,50);//50ms
		index += dwReNum;
		if(index >= *readNum)break;
		Sleep(3);

		QueryPerformanceCounter(&lgCur);
		if((lgCur.QuadPart-lgPre.QuadPart)/dqFreq >n)break;
	}
	 
	*readNum=index;
	if(dwReNum == 0){
		result = CAN_RESULT_TIMEOUT;
 	}
	else if(dwReNum == -1){result = CAN_RESULT_NO_DEVICE; }
	else if(dwReNum > 0 )
		{
			result = CAN_RESULT_OK;
			for(int j=0;j<*readNum;j++)
			{
 				RecordCommand(CAN_RECEIVE,gCanObj[j].Data,gCanObj[j].ID,1,writetype);
			}
			
	}
	else {result = CAN_RESULT_NO_DEVICE; }
	return result;

}
int gnn;
int CAN_protocol::CAN_SendDataICAN(unsigned int channel,int sendNum)
{
	int dwReNum;
	int result;
	int index=0;
	for(int i=0;i<50;i++)
	{
		dwReNum=ICANTransmit(m_DevType,m_DevIndex,channel,&IEvent_Arr[index],sendNum);//单双通道，ICAN索引,CAN通道
 		index += dwReNum;
		if(index == sendNum)break;
		Sleep(5);//5
   	}
	if(index == sendNum){ 
			result = CAN_RESULT_OK;
			for(int j=0;j<sendNum;j++)
			{	
				//xltime= IEvent_Arr[j].TimeStamp;
				RecordCommand(CAN_SEND,IEvent_Arr[j].Data,IEvent_Arr[j].ID,1,writetype);
			}
	}
	else result = CAN_RESULT_NO_DEVICE;
	return result;

}

int CAN_protocol::CAN_messageInteractICAN(unsigned int channel,pCAN_msg msg,unsigned char *PIDData)
{
	int ggret;
	int gsn;
	char serviceId;
	int index;
	U8 framenumth;
	U16 mulframe;
	int addframe; 
	int	i,k,m;
	WORD j;
	int recelen,recelenh,recelenl;
	WORD framenum;
	int result;
 	int dwReNum;

	if(g_StopFlag == 1){
			resultx = CAN_STOP;
			//g_StopFlag = 0;
			return resultx;
		}	

	IEvent_Arr[0].ExternFlag=CAN_ID_FORMAT_STANDARD;
	IEvent_Arr[0].RemoteFlag=CAN_FRAME_TYPE_DATA;
	IEvent_Arr[0].DataLen=8;
	IEvent_Arr[0].ID=msg->id;
	memcpy(IEvent_Arr[0].Data,msg->data,8);
	
 //	for(int i=0;i<10;i++)
	//{
	//	dwReNum = ICANReceive( m_DevType, m_DevIndex,channel,gCanObj,50,50);
	//	if(dwReNum<=0)break;
	//}
	if(msg->data[2]==0x36 ){
		//Sleep(1);
		writetype=1;
		for(int m=0;m<2;m++)
		{
 			int bRel = ICANClearBuffer(m_DevType,m_DevIndex,channel);
			if(bRel == 1)break;
			else if(bRel < 1)AfxMessageBox(_T("ICAN Clear buffer fail"));
			Sleep(5);
		}
		Sleep(3);
	}
	else {writetype=0;}
	
	result = CAN_SendDataICAN(channel,1);
	if(result != CAN_RESULT_OK)return result;
  	dwReNum=1;
	
	result = CAN_ReadDataICAN(channel,&dwReNum);
	if(result != 0)return result;
	if(dwReNum == 1)index=0;
	else index=1;
 	recelenh=gCanObj[index].Data[0];
	mulframe=recelenh&0XF0;
	if(mulframe==0x10)		//first frame 10   	receive	 多帧接受
	{
		recelenl=gCanObj[index].Data[1];	 //message len
		recelen=(recelenh&0X0F)*256+recelenl;
		if((recelen-6)%7==0)framenum=(recelen-6)/7;
		else framenum=(recelen-6)/7+1;
		serviceId=gCanObj[index].Data[2];; //SID

		for(i=0,j=3;j<8;i++,j++)
			PIDData[i]=gCanObj[index].Data[j];//第一帧数据

		memset(IEvent_Arr[0].Data,0xff,8);
		IEvent_Arr[0].ID	= 0x7E0;
		IEvent_Arr[0].Data[0] = 0x30;  // PIC
		IEvent_Arr[0].Data[1] = 0x00;  // Model 1
		IEvent_Arr[0].Data[2] = 0x00;  // Model 1
		IEvent_Arr[0].DataLen=8;
		IEvent_Arr[0].ExternFlag = CAN_ID_FORMAT_STANDARD;
		IEvent_Arr[0].RemoteFlag=CAN_FRAME_TYPE_DATA;
		 
		result = CAN_SendDataICAN(channel,1);
		if(result != CAN_RESULT_OK)return result;
 		addframe=0;
 		dwReNum=framenum;
			result = CAN_ReadDataICAN(channel,&dwReNum);
			if(result != CAN_RESULT_OK)return result;
			else
			{
				for(j=0;j<framenum;j++)
				{
				framenumth=gCanObj[j].Data[0]&0x0F;
				for(m=1,k=7*(framenumth+(addframe*16)-1)+5;m<8;m++,k++)
				{
					PIDData[k] = gCanObj[j].Data[m];//后续帧数据
				}
				if(framenumth==0x0F)
					addframe++;
			}
		}
	}
	else if(mulframe==0x30)	   //1k 146 message	 发送多帧
	{
		recelenl=msg->data[1];	 //message len
		recelenh=msg->data[0];
 		recelen=(recelenh&0X0F)*256+recelenl;
		framenum=(recelen-6)/7;
		if((recelen-6)%7 != 0)framenum+=1;
		ggret =framenum;
		PIDData+=5;				  //去掉第一帧的5个字节
		m=0x21;
		j=0;
		int IcanSendNum=framenum/48;
		int IcanSendNumL=framenum%48;
		if(IcanSendNumL == 0)IcanSendNum-=1;
		if(IcanSendNum != 0)
		{
			for(gsn=0;gsn<IcanSendNum;gsn++)
			{
  				for(int n=0;n<48;n++)
				{
					IEvent_Arr[n].Data[0] = m;
					memcpy(&IEvent_Arr[n].Data[1],PIDData,7);	
					IEvent_Arr[n].ID	=0x7E0;
					IEvent_Arr[n].DataLen = 8;
 					IEvent_Arr[n].ExternFlag = CAN_ID_FORMAT_STANDARD;
					IEvent_Arr[n].RemoteFlag=CAN_FRAME_TYPE_DATA;
					PIDData+=7;
					m++;	
					if(m==0x30)m=0x20; 
   				}
				result = CAN_SendDataICAN(channel,48);
				if(result != CAN_RESULT_OK)return result;
				Sleep(9);//9
				 
 			}
		}
 		if(IcanSendNumL == 0)ggret=48;
		else ggret=IcanSendNumL;
		gsn=m;
		int n=0;
			for(int k=0;k<ggret;k++)
			{
				if(k == IcanSendNumL-1){memset(IEvent_Arr[n].Data,0xff,8);}
				IEvent_Arr[n].Data[0] = m;
				memcpy(&IEvent_Arr[n].Data[1],PIDData,7);	
				IEvent_Arr[n].ID	=0x7E0;
				IEvent_Arr[n].DataLen = 8;
 				IEvent_Arr[n].ExternFlag = CAN_ID_FORMAT_STANDARD;
				IEvent_Arr[n].RemoteFlag=CAN_FRAME_TYPE_DATA;
				PIDData+=7;
				m++;	
				if(m==0x30)m=0x20;
				result = CAN_SendDataICAN(channel,1);
				if(result != CAN_RESULT_OK)return result;
  			}
			//result = CAN_SendDataICAN(channel,ggret);
			//if(result != CAN_RESULT_OK)return result;
		dwReNum=1;
	result = CAN_ReadDataICAN(channel,&dwReNum);
	if(result != CAN_RESULT_OK){ return result;}//gai
 	
	if(dwReNum == 2)
	{
 		index=1;
	}
	else index=0;
	//Sleep(7);
		serviceId=gCanObj[index].Data[1];
		recelen=gCanObj[index].Data[0]&0X0F;
		for(j=2,k=0;j<recelen+1;j++,k++)				  // SID PID1
			PIDData[k] = gCanObj[index].Data[j]; 
	}
	else 
	{
 		if(dwReNum == 2)
			index=1;
		else index=0;
		serviceId=gCanObj[index].Data[1];
		recelen=gCanObj[index].Data[0]&0X0F;
		for(j=2,k=0;j<recelen+1;j++,k++)				  // SID PID1
			PIDData[k] = gCanObj[index].Data[j]; 
	}
	if(serviceId == 0x7F)
	{
		if(recelen < 3)
	 		 return CAN_ERROR_RESP_ARG_ABSENT;
 		result = PIDData[1];
		 
	}
	memcpy(msg->data,gCanObj[index].Data,8);

	return result;
	 
}	
int CAN_protocol::CAN_ReadDataCancardxl(unsigned int channel,XLevent *xlEvent)
{
	unsigned int msgsrx;
	LARGE_INTEGER lgPre,lgCur,lgFrq;
	QueryPerformanceFrequency(&lgFrq);
	double dqFreq;
	dqFreq = (double)lgFrq.QuadPart;
	QueryPerformanceCounter(&lgPre);
	int n=config->ECUprg.Delay_Time_seconds;
	if(xlEvent->tagData.msg.data[1] == 0x3e)n=1;
	if(config->ECUprg.Delay_Time_seconds <= 0)n=2;
	while(1)
	{
		if(g_StopFlag == 1){
			gstatus = CAN_STOP;
			//g_StopFlag = 0;
			break;
		}		
		msgsrx=1;
		gstatus = xlReceive(m_CAN->m_xlPortHandle, &msgsrx,xlEvent);
		if(gstatus != XL_ERR_QUEUE_IS_EMPTY)
		{
			if(xlEvent->tag == XL_RECEIVE_MSG && m_CAN->m_xlChannelMask[0])
			{xltime= xlEvent->timeStamp ;
				RecordCommand(CAN_RECEIVE,xlEvent->tagData.msg.data,xlEvent->tagData.msg.id,1,writetype);
				return CAN_RESULT_OK;
			}
		}		
		QueryPerformanceCounter(&lgCur);
		if((lgCur.QuadPart-lgPre.QuadPart)/dqFreq >n)break;
	}
 	if(xlEvent->tag == XL_RECEIVE_MSG)
	{
		switch(xlEvent->tagData.msg.flags)
		{
		case XL_CAN_MSG_FLAG_ERROR_FRAME:
			gstatus = CAN_RESULT_FRAME_ERROR;
			break;
		case XL_CAN_MSG_FLAG_TX_COMPLETED:
			gstatus = CAN_RESULT_TIMEOUT;
			break;
		default : gstatus = CAN_RESULT_TIMEOUT;
			break;
		}
	}
	else if(xlEvent->tag == XL_CHIP_STATE)
	{
		switch(xlEvent->tagData.chipState.busStatus)
		{
		case XL_CHIPSTAT_BUSOFF:
			gstatus = CAN_RESULT_BUSOFF_ERROR;
			break;
		case XL_CHIPSTAT_ERROR_PASSIVE:
			gstatus = CAN_RESULT_PASSIVE_ERROR;
			break;
		case XL_CHIPSTAT_ERROR_ACTIVE:
			gstatus = CAN_RESULT_ACTIVE_ERROR;
			break;
		default :
			gstatus = CAN_RESULT_BUSOFF_ERROR;
			break;
		}
	}
	else if(xlEvent->tag == XL_TRANSCEIVER)
	{
		gstatus = CAN_RESULT_BUSOFF_ERROR;
	}
	else
	{
		gstatus = CAN_RESULT_TIMEOUT;
	}

	return gstatus;

}

int CAN_protocol::CAN_SendDataCancardxl(unsigned int channel,XLevent *xlEvent)
{
	XLevent xlEventRec;
	unsigned int msgsrx;

	gstatus= m_CAN->CANSend(*xlEvent,channel);
	if(gstatus != XL_SUCCESS)
		return gstatus; 

	for(int i=0;i<2000;i++)
	{
		for(int i=0;i<2000;i++)
		{
			msgsrx=1;
			gstatus = xlReceive(m_CAN->m_xlPortHandle, &msgsrx, &xlEventRec);
			if(gstatus != XL_ERR_QUEUE_IS_EMPTY)break;
		}
		if(gstatus != XL_ERR_QUEUE_IS_EMPTY )
		{
			if(xlEventRec.tag == XL_RECEIVE_MSG 
			|| xlEventRec.tag == XL_CHIP_STATE || xlEventRec.tag == XL_TRANSCEIVER)break;
		}
		//Sleep(1);
	}

	if(xlEventRec.tag == XL_RECEIVE_MSG)
	{
		switch(xlEventRec.tagData.msg.flags)
		{
		case XL_CAN_MSG_FLAG_ERROR_FRAME:
			gstatus = CAN_RESULT_FRAME_ERROR;
			break;
		case XL_CAN_MSG_FLAG_TX_COMPLETED:
			//if(xlEventRec.tagData.msg.id == 0x7e8)
			xltime= xlEventRec.timeStamp ;
				RecordCommand(CAN_SEND,xlEventRec.tagData.msg.data,xlEventRec.tagData.msg.id,1,writetype);
			//else gstatus = CAN_RESULT_TIMEOUT;
			break;
		default : gstatus = CAN_RESULT_TIMEOUT;
			break;
		}
	}
	else if(xlEventRec.tag == XL_CHIP_STATE)
	{
		switch(xlEventRec.tagData.chipState.busStatus)
		{
		case XL_CHIPSTAT_BUSOFF:
			gstatus = CAN_RESULT_BUSOFF_ERROR;
			break;
		case XL_CHIPSTAT_ERROR_PASSIVE:
			gstatus = CAN_RESULT_PASSIVE_ERROR;
			break;
		case XL_CHIPSTAT_ERROR_ACTIVE:
			gstatus = CAN_RESULT_ACTIVE_ERROR;
			break;
		default :
			gstatus = CAN_RESULT_BUSOFF_ERROR;
			break;
		}
	}
	else if(xlEventRec.tag == XL_TRANSCEIVER)
	{
		gstatus = CAN_RESULT_BUSOFF_ERROR;
	}
	else
	{
		gstatus = CAN_RESULT_TIMEOUT;
	}
	return gstatus;

}


XLstatus CAN_protocol::CAN_messageInteractCancardxl(unsigned int channel,XLevent *xlEvent,unsigned char *PIDData)
{
	char serviceId;
	U8 framenumth;
	U16 mulframe;
	int addframe; 
	int	i,k,m;
	WORD j;
	int errornum=0;
	int recelen,recelenh,recelenl,len;
	WORD framenum;
 	 XLevent xlEventRec;
	 XLevent xlEventSend;
	unsigned int msgsrx = 1;
	if(g_StopFlag == 1){
			resultx = CAN_STOP;
			//g_StopFlag = 0;
			return resultx;
		}	

	if(xlEvent->tagData.msg.data[2]==0x36 ){ writetype=1;}
		else { writetype=0;}

	gstatus=0;
	while (!gstatus) {
		 msgsrx = 1;
		gstatus = xlReceive(m_CAN->m_xlPortHandle, &msgsrx, &xlEventRec);
		if ( gstatus == XL_ERR_QUEUE_IS_EMPTY )break;
	}

	gstatus = CAN_SendDataCancardxl(channel,xlEvent);
	if(gstatus != XL_SUCCESS)
		return gstatus; 
	memcpy(xlEventRec.tagData.msg.data,xlEvent->tagData.msg.data,8);
	gstatus = CAN_ReadDataCancardxl(channel,&xlEventRec);
	if(gstatus != XL_SUCCESS)
		return gstatus; 

	recelenh=xlEventRec.tagData.msg.data[0];
	mulframe=recelenh&0XF0;
	if(mulframe==0x10)		//first frame 10   	receive	 多帧接受
	{
		recelenl=xlEventRec.tagData.msg.data[1];	 //message len
		recelen=(recelenh&0X0F)*256+recelenl;
		if((recelen-6)%7==0)framenum=(recelen-6)/7;
		else framenum=(recelen-6)/7+1;
		serviceId=xlEventRec.tagData.msg.data[2]; //SID

		for(i=0,j=3;j<8;i++,j++)
			PIDData[i]=xlEventRec.tagData.msg.data[j];

		memset(&xlEventSend.tagData.msg.data,0xff,8);
		//for(i=0;i<10;i++)
		{
			xlEventSend.tag = XL_TRANSMIT_MSG;
			xlEventSend.tagData.msg.id	=0x7E0;
			xlEventSend.tagData.msg.data[0]	=0x30;  // PIC
			xlEventSend.tagData.msg.data[1]	=0x00;  // Model 1
			xlEventSend.tagData.msg.data[2]	=0x00;  // Model 1
			xlEventSend.tagData.msg.dlc = 8;
			xlEventSend.tagData.msg.flags = 0;

			gstatus = CAN_SendDataCancardxl(channel,&xlEventSend);
			if(gstatus != XL_SUCCESS)return gstatus;
		 	
			addframe=0;
			for(j=0;j<framenum;j++)
			{
				gstatus = CAN_ReadDataCancardxl(channel,&xlEventRec);
				if(gstatus != XL_SUCCESS)return gstatus; 
				framenumth=xlEventRec.tagData.msg.data[0]&0x0F;
				for(m=1,k=7*(framenumth+(addframe*16)-1)+5;m<8;m++,k++)
				{
					PIDData[k] = xlEventRec.tagData.msg.data[m];
				}
				if(framenumth==0x0F)
					addframe++;
			}
		}	
	}
	else if(mulframe==0x30)	   //1k 146 message	 发送多帧
	{
		recelenl=xlEvent->tagData.msg.data[1];	 //message len
		recelenh=xlEvent->tagData.msg.data[0];
		recelen=(recelenh&0X0F)*256+recelenl;
		framenum=(recelen-6)/7;
		PIDData+=5;				  //去掉第一帧的5个字节
		m=0x21;
	 
		for(j=0;j<framenum;j++)
		{
			xlEvent_Arr[j].tag=XL_TRANSMIT_MSG;
			xlEvent_Arr[j].tagData.msg.id	=0x7E0;
			xlEvent_Arr[j].tagData.msg.data[0]=m;
			memcpy(&xlEvent_Arr[j].tagData.msg.data[1],PIDData,7);
			xlEvent_Arr[j].tagData.msg.dlc = 8;
			xlEvent_Arr[j].tagData.msg.flags  = 0;
		 
			PIDData+=7;
			m++;
			if(m==0x30)m=0x20;
			gstatus = CAN_SendDataCancardxl(channel,&xlEvent_Arr[j]);
			if(gstatus != XL_SUCCESS)return gstatus;
			//Sleep(1);
		}
		len= (recelen-6)%7;
 
		if(len!=0)
		{
			memset(&xlEvent_Arr[j].tagData.msg.data,0xff,8);
			xlEvent_Arr[j].tagData.msg.id	=0x7E0;
			xlEvent_Arr[j].tag=XL_TRANSMIT_MSG;
			xlEvent_Arr[j].tagData.msg.data[0] = m;  // PIC
			memcpy(&xlEvent_Arr[j].tagData.msg.data[1],PIDData,len);
			xlEvent_Arr[j].tagData.msg.dlc = 8;
			xlEvent_Arr[j].tagData.msg.flags  = 0;

			gstatus = CAN_SendDataCancardxl(channel,&xlEvent_Arr[j]);
			if(gstatus != XL_SUCCESS)return gstatus;
		}
		gstatus = CAN_ReadDataCancardxl(channel,&xlEventRec);
		if(gstatus != XL_SUCCESS)
			return gstatus; 
		serviceId=xlEventRec.tagData.msg.data[1];
		recelen=xlEventRec.tagData.msg.data[0]&0X0F;
		for(j=2,k=0;j<recelen+1;j++,k++)				  // SID PID1
			PIDData[k] = xlEventRec.tagData.msg.data[j]; 
	}
	else 
	{
		serviceId=xlEventRec.tagData.msg.data[1];
		recelen=recelenh&0X0F;
		for(j=2,k=0;j<recelen+1;j++,k++)				  // SID PID1
			PIDData[k] = xlEventRec.tagData.msg.data[j]; 
	}
	if(serviceId == 0x7F)
	{
		if(recelen < 3)
	 		 return CAN_ERROR_RESP_ARG_ABSENT;
		gstatus =  PIDData[1];
		//if(xlEventRec.tagData.msg.data[2] == xlEvent->tagData.msg.data[1])gstatus =  PIDData[1];
		//else gstatus =  PIDData[0]; //	msg_recv.data[3]

	}
	memcpy(xlEvent->tagData.msg.data,xlEventRec.tagData.msg.data,8);
	
	 return gstatus;
	 
}	

int CAN_protocol::CAN_ReadDataES581(icsSpyMessage *pMessagesTx,int *readNum)
{
	int lResult;
    long lCount;
	int index=0;
	int rec_num,error_num;
 	LARGE_INTEGER lgPre,lgCur,lgFrq;
	QueryPerformanceFrequency(&lgFrq);
	double dqFreq;
	dqFreq = (double)lgFrq.QuadPart;
	QueryPerformanceCounter(&lgPre);
	int n=config->ECUprg.Delay_Time_seconds;
	if(pMessagesTx->Data[1] == 0x3e)n=1;
	if(config->ECUprg.Delay_Time_seconds <= 0)n=2;
	 
	while(1)
	{
		if(g_StopFlag == 1){
			gstatus = CAN_STOP;
			//g_StopFlag = 0;
			break;
		}	
 		lResult = m_ES581CAN->cmdReceive(gMsg_ES581,&rec_num,&error_num);
 		if(rec_num > 0)
		{
	//RecordCommand(CAN_RECEIVE,gMsg_ES581[0].Data,0x222,1,writetype);
			for(lCount=0;lCount<rec_num;lCount++)
			{
					//RecordCommand(CAN_RECEIVE,gMsg_ES581[lCount].Data,0x333,1,writetype);
				if((gMsg_ES581[lCount].StatusBitField & 2)!=0)
				{
					m_ES581CAN->cmdGetTimeStamp(&dTime,&gMsg_ES581[lCount]);
					RecordCommand(CAN_SEND,gMsg_ES581[lCount].Data,gMsg_ES581[lCount].ArbIDOrHeader,1,writetype);
 				}
  				else if((gMsg_ES581[lCount].StatusBitField & 2)==0 &&gMsg_ES581[lCount].ArbIDOrHeader == 0x7e8)
				{
					
					msg_Arr[index].id =gMsg_ES581[lCount].ArbIDOrHeader;
					memcpy(msg_Arr[index].data,gMsg_ES581[lCount].Data,8);
 					m_ES581CAN->cmdGetTimeStamp(&dTime,&gMsg_ES581[lCount]);
					RecordCommand(CAN_RECEIVE,msg_Arr[index].data,msg_Arr[index].id,1,writetype);
					index++;
				}
 			}

 		}
		if(index >= *readNum)break; 
 		QueryPerformanceCounter(&lgCur);
		if((lgCur.QuadPart-lgPre.QuadPart)/dqFreq >n)break;
		//Sleep(2);
	}
	*readNum=index;
 	 if(index <= 0)lResult = CAN_RESULT_TIMEOUT;
	 else lResult = CAN_RESULT_OK;
	return lResult;

}

int CAN_protocol::CAN_SendDataES581(icsSpyMessage *pMessagesTx,int sendNum)
{
	int lResult;
    long lCount;
	int rec_num,error_num;

	lResult = m_ES581CAN->cmdTransmit(pMessagesTx,sendNum);
	if(lResult != ES581_SUCCESS)
		return CAN_RESULT_TIMEOUT; 
	//RecordCommand(CAN_SEND,pMessagesTx->Data,pMessagesTx->ArbIDOrHeader,1,writetype);
 //	lResult = m_ES581CAN->cmdReceive(gMsg_ES581,&rec_num,&error_num);
	//if(rec_num > 0)
	//{
 //		for(lCount=0;lCount<rec_num;lCount++)
	//	{
 //			if(gMsg_ES581[lCount].ArbIDOrHeader == 0x7e0)
	//		{
	//			break;
 //			}
 //		}
 //	}
	if(lResult = ES581_ERROR)return CAN_RESULT_TIMEOUT;
	else return CAN_RESULT_OK;

}
int CAN_protocol::CAN_messageInteractES581(icsSpyMessage *pMessagesTx,unsigned char *PIDData)
{
	long lResult; 
	char serviceId;
	U8 framenumth;
	U16 mulframe;
	int addframe; 
	int	i,k,m;
	WORD j;
	int errornum=0;
	int recelen,recelenh,recelenl,len;
	WORD framenum;
	int rec_num;
	int index;
	icsSpyMessage icsMsg_send;

	if(g_StopFlag == 1){
			resultx = CAN_STOP;
			//g_StopFlag = 0;
			return resultx;
		}	

	if(pMessagesTx->Data[2]==0x36 ){ writetype=1;}
		else { writetype=0;}

 	lResult = CAN_SendDataES581(pMessagesTx,1);
	if(lResult != CAN_RESULT_OK)
		return lResult; 
	rec_num = 1;
	lResult = CAN_ReadDataES581(pMessagesTx,&rec_num);
	if(lResult != CAN_RESULT_OK)
		return lResult; 
	if(rec_num == 1)index=0;
	else index=1;
	recelenh= msg_Arr[index].data[0];
	mulframe=recelenh&0XF0;
	if(mulframe==0x10)		//first frame 10   	receive	 多帧接受
	{
		recelenl=msg_Arr[index].data[1];	 //message len
		recelen=(recelenh&0X0F)*256+recelenl;
		if((recelen-6)%7==0)framenum=(recelen-6)/7;
		else framenum=(recelen-6)/7+1;
		serviceId=msg_Arr[index].data[2]; //SID

		for(i=0,j=3;j<8;i++,j++)
			PIDData[i]=msg_Arr[index].data[j];

		memset(&icsMsg_send.Data,0xff,8);
		//for(i=0;i<10;i++)
		{
			icsMsg_send.ArbIDOrHeader	=0x7E0;
			icsMsg_send.Data[0]	=0x30;  // PIC
			icsMsg_send.Data[1]	=0x00;  // Model 1
			icsMsg_send.Data[2]	=0x00;  // Model 1
			icsMsg_send.NumberBytesData = 8;
			icsMsg_send.StatusBitField = 0;
			icsMsg_send.StatusBitField2 = 0;
			lResult = CAN_SendDataES581(&icsMsg_send,1);
			if(lResult != CAN_RESULT_OK)
				return lResult; 
		 	rec_num = framenum;
			addframe=0;
			//for(j=0;j<framenum;j++)
			{
				rec_num = framenum;
				lResult = CAN_ReadDataES581(pMessagesTx,&rec_num);
				if(lResult != CAN_RESULT_OK)return lResult; 
				else
				{
					for(j=0;j<framenum;j++)
					{
						framenumth=msg_Arr[j].data[0]&0x0F;
						for(m=1,k=7*(framenumth+(addframe*16)-1)+5;m<8;m++,k++)
						{
							PIDData[k] = msg_Arr[j].data[m];//后续帧数据
						}
						if(framenumth==0x0F)
							addframe++;
					}
				}
			}
		}	
	}
	else if(mulframe==0x30)	   //1k 146 message	 发送多帧
	{
		recelenl=pMessagesTx->Data[1];	 //message len
		recelenh=pMessagesTx->Data[0];
		recelen=(recelenh&0X0F)*256+recelenl;
		framenum=(recelen-6)/7;
		PIDData+=5;				  //去掉第一帧的5个字节
		m=0x21;
	 
		for(j=0;j<framenum;j++)
		{
			icsMsg_send.ArbIDOrHeader	=0x7E0;
			icsMsg_send.Data[0]	=m;  // PIC
 			memcpy(&icsMsg_send.Data[1],PIDData,7);
			icsMsg_send.NumberBytesData = 8;
			icsMsg_send.StatusBitField = 0;
			icsMsg_send.StatusBitField2 = 0;
			lResult = CAN_SendDataES581(&icsMsg_send,1);
			if(lResult != CAN_RESULT_OK)
				return lResult; 
 			/*gMsg_ES581[j].ArbIDOrHeader=0x7e0;
			gMsg_ES581[j].Data[0]==m; 
			memcpy(&gMsg_ES581[j].Data[1],PIDData,7);
			gMsg_ES581[j].NumberBytesData = 8;
			gMsg_ES581[j].StatusBitField = 0;
			gMsg_ES581[j].StatusBitField2 = 0;*/

			PIDData+=7;
			m++;
			if(m==0x30)m=0x20;
		}
			//		lResult = CAN_SendDataES581(gMsg_ES581,framenum);
			//if(lResult != CAN_RESULT_OK)
			//	return lResult; 

		len= (recelen-6)%7;
 
		if(len!=0)
		{
			memset(icsMsg_send.Data,0xff,8);
			icsMsg_send.ArbIDOrHeader	=0x7E0;
			icsMsg_send.Data[0]	=m;  // PIC
 			memcpy(&icsMsg_send.Data[1],PIDData,len);
			icsMsg_send.NumberBytesData = 8;
			icsMsg_send.StatusBitField = 0;
			icsMsg_send.StatusBitField2 = 0;
			lResult = CAN_SendDataES581(&icsMsg_send,1);
			if(lResult != CAN_RESULT_OK)
				return lResult; 
		}
		rec_num=1;
		lResult = CAN_ReadDataES581(gMsg_ES581,&rec_num);
		if(lResult != CAN_RESULT_OK)
			return lResult; 
		if(rec_num == 1)index=0;
		else index=1;
		serviceId=msg_Arr[index].data[1];
		recelen=msg_Arr[index].data[0]&0X0F;
		for(j=2,k=0;j<recelen+1;j++,k++)				  // SID PID1
			PIDData[k] = msg_Arr[index].data[j]; 
	}
	else 
	{
		if(rec_num == 1)index=0;
		else index=1;
		serviceId=msg_Arr[index].data[1];
		recelen=recelenh&0X0F;
		for(j=2,k=0;j<recelen+1;j++,k++)				  // SID PID1
			PIDData[k] = msg_Arr[index].data[j]; 
		
	}
	if(serviceId == 0x7F)
	{
		if(recelen < 3)
	 		 return CAN_ERROR_RESP_ARG_ABSENT;
		lResult =  PIDData[1];
		//if(xlEventRec.tagData.msg.data[2] == xlEvent->tagData.msg.data[1])gstatus =  PIDData[1];
		//else gstatus =  PIDData[0]; //	msg_recv.data[3]

	}
	

	memcpy(pMessagesTx->Data,msg_Arr[index].data,8);
	 return lResult;
	 
}	
int CAN_protocol::CAN_DisplayCommand(CAN_msg *canMsg,int result)
{
	CString s;
	CString str;
	s.Empty();
	for(int i=0;i<8;i++)//msg->len
	{
		str.Format(_T("%02x "),canMsg->data[i]);
		s+=str;
	}
	s+="\r\n";
	//if(result == 0)flash->display(s,0);
	//else flash->display(s,1);
	return 1;
}
int CAN_protocol::CAN_CommandMessageProc(CAN_msg *msg,int *mulnum)
{
	int i;
	param_struct 	paramRec;
	unsigned int msgsrx=1;
	XLevent xlEvent_R;
	int rec_num;
	icsSpyMessage stMessagesTx;
	int result;
	for(i=0;i<RENUM;i++)
	{
		resultx	=CAN_CommandmessageInteract(msg,mulnum);
		if(resultx != CAN2USB_RESULT_TIMEOUT)break;
	} 
	if(resultx == 0x78)
	{	
		CAN_DisplayCommand(msg,1);
		switch(g_CANDirverType)
		{
			case 0:
				for(i=0;i<10 && resultx == 0x78;i++)	//延时
				{ 
					flash->display (_T("Busy..."),0);
					resultx = CAN_ReadDataUSB2CAN(CANNo,msg);					
					if(resultx != CAN_RESULT_OK)break;				
 					if(msg->data[1] == 0x7F)
					{
						resultx = msg->data[3];						
 					}
					else resultx=0;		
					CAN_DisplayCommand(msg,resultx);
				}
				break;
			case 1:
				for(i=0;i<10 && resultx == 0x78;i++)	//延时
				{ 
					flash->display (_T("Busy..."),0);
					resultx = CAN_ReadDataCancard2(xlChannel,&paramRec);
					memcpy(msg->data,paramRec.RCV_data,8);
					if(resultx != CAN_RESULT_OK)break;
					if(paramRec.RCV_data[1] == 0x7F)
					{
						resultx = paramRec.RCV_data[3];//0X78
 					}
					else resultx=0;
					CAN_DisplayCommand(msg,resultx);
				}
				break;
			case 2:
				for(i=0,msgsrx=1;i<20 && resultx == 0x78;i++)	//延时
				{ 
					flash->display (_T("Busy..."),0);
					resultx = CAN_ReadDataCancardxl(xlChannel,&xlEvent_R);
					memcpy(msg->data,xlEvent_R.tagData.msg.data,8);
					if(resultx != CAN_RESULT_OK)break;
 					if(xlEvent_R.tagData.msg.data[1] == 0x7F)
					{
						resultx = xlEvent_R.tagData.msg.data[3];//78

 					}
					else resultx=0;
					CAN_DisplayCommand(msg,resultx);
				}	
				break;
			case 3:
   			 int dwReNum;
			for(i=0,msgsrx=1;i<20 && resultx == 0x78;i++)	//延时
			{ dwReNum=1;
 				flash->display (_T("Busy..."),0); 
				resultx = CAN_ReadDataICAN(xlChannel,&dwReNum);
  				if(resultx != CAN_RESULT_OK)break;
				if(gCanObj[0].Data[1] == 0x7F)
				{
					resultx = gCanObj[0].Data[3];//78
 				}
				else resultx=0;
				if(dwReNum == 2)
				{
					if(gCanObj[1].Data[1] != 0x7f)
					{
						resultx = 0;//78
						//RecordCommand(CAN_RECEIVE,gCanObj[1].Data,gCanObj[1].ID,1,writetype);
						break;
 					}
					else
					{
						resultx = gCanObj[1].Data[3];
						memcpy(&gCanObj[0].Data,&gCanObj[1].Data,8);

					}
					 
				}
				CAN_DisplayCommand(msg,resultx);
			}	
			memcpy(msg->data,gCanObj[0].Data,8);
			CAN_DisplayCommand(msg,resultx);
			break;
			case 4:
 			for(i=0,msgsrx=1;i<20 && resultx == 0x78;i++)	//延时
			{ 
				flash->display (_T("Busy..."),0);
				dwReNum=1;
				//Sleep(2000);
				rec_num=1;
				resultx = CAN_ReadDataES581(&stMessagesTx,&rec_num);
  				if(resultx != CAN_RESULT_OK)break;
				if(msg_Arr[0].data[1] == 0x7F)
				{
					resultx = msg_Arr[0].data[3];//78
 				}
				else resultx=0;
				if(rec_num == 2)
				{
					if(msg_Arr[1].data[1] != 0x7f)
					{
						resultx = 0;//78
 						break;
 					}
					else
					{
						resultx = msg_Arr[1].data[3];
						memcpy(&msg_Arr[0].data,&msg_Arr[1].data,8);
					}
					 
				}
			}	
			memcpy(msg->data,stMessagesTx.Data,8);
			break;
			 
			default:
				break;
		}
		*mulnum=1;
	}
	
	return resultx;
}
int CAN_protocol::CAN_CommandmessageInteract(pCAN_msg msg,int *mulnum)
{
	char serviceId;
	CAN_msg msg_recv,msg_send;
	//CAN_Rxmsg Rxmsg;
	int	result=0;
	int	i,j,errornum;
	param_struct 	paramRec;
	 XLevent xlEventRec,xlEventSend;
	unsigned int msgsrx = 1;
	int recelen,recelenh,recelenl;
	WORD framenum;
	U8 framenumth;
	U16 mulframe;
	int addframe;
	int index;
	*mulnum=1;
	switch(g_CANDirverType)
	{
	case 0:
		result=0;
		for(i=0;i<100 && result ==0;i++)  //先清空缓存
			result = CAN2USB_readCAN(&msg_recv,1);
 
		result = CAN2USB_writeCAN(msg,20);	
		if(result != CAN2USB_RESULT_OK)return result; 
		RecordCommand(CAN_SEND,msg->data,msg->id,1,0);

		result = CAN_ReadDataUSB2CAN(xlChannel+1,&msg_recv);
		if(result != CAN2USB_RESULT_OK)return result; 
		mulframe=msg_recv.data[0]&0XF0;
		 
	if(mulframe==0x10)		//first frame 10   	receive	 多帧接受
	{
		recelen=(msg_recv.data[0] & 0X0F)*256 + msg_recv.data[1];
		if((recelen-6)%7==0)framenum=(recelen-6)/7;
		else framenum=(recelen-6)/7+1;
		serviceId=msg_recv.data[2]; //SID
		*mulnum=framenum+2;
		memset(&msg_send.data,0xff,8);
		errornum=0;

		memcpy(&msg_Arr[0].data,msg_recv.data,8);
		for(i=0;i<10;i++)
		{
			msg_send.id	=0x7E0;
			msg_send.data[0]	=0x30;  // PIC
			msg_send.data[1]	=0x00;  // Model 1
			msg_send.data[2]	=0x00;  // Model 1
			msg_send.len = 8;
			msg_send.ch  = (U8)CANNo;
			msg_send.format = CAN_ID_FORMAT_STANDARD;
			msg_send.type   = CAN_FRAME_TYPE_DATA;
	
			result = CAN2USB_writeCAN(&msg_send,30); 
			if(result != CAN2USB_RESULT_OK)
				errornum=1;
			RecordCommand(0,msg_send.data,msg_send.id,1,0);
			memcpy(&msg_Arr[1].data,msg_send.data,8);
			addframe=0;
			
			for(j=0;j<framenum && errornum==0;j++)
			{
				/*result =CAN2USB_readCAN(hCanDev,CAN_CHANNEL_0,&msg_recv,300);
				RecordCommand(1,msg_recv.data,msg_recv.id,1,0);*/
				result =CAN_ReadDataUSB2CAN(xlChannel+1,&msg_recv);
				if(result == CAN2USB_RESULT_OK)
				{
					framenumth=msg_recv.data[0]&0x0F;
					/*for(m=1,k=7*(framenumth+(addframe*16)-1)+5;m<8;m++,k++)
					{
					   PIDData[k] = msg_recv.data[m];
					}*/
					memcpy(&msg_Arr[j+2].data,msg_recv.data,8);
					if(framenumth == 0x0F)
					   	addframe++;
				}
				else 
				{
					errornum=1;
					break;
				}
			}
			if(errornum==0)break;
		}	
		if(errornum==1) return result;
	}
	else
	{
		serviceId=msg_recv.data[1];
		recelen=msg_recv.data[0]&0X0F;

	}
	if(serviceId == 0x7F)
	{
		//memcpy(msg,&msg_recv,sizeof(CAN_msg));
		if(recelen < 3)
	 		 return CAN_ERROR_RESP_ARG_ABSENT;
		return result=msg_recv.data[3];
 	}
		memcpy(msg->data ,msg_recv.data,8);
		break;
	case 1:
		result = CAN_SendDataCancard2(xlChannel,msg);
		if(result != CAN_RESULT_OK)return result;

		result = CAN_ReadDataCancard2(xlChannel,&paramRec);
		if(result != CAN_RESULT_OK)return result;
			recelenh=paramRec.RCV_data[0];
	mulframe=recelenh&0XF0;
	if(mulframe==0x10)		//first frame 10   	receive	 多帧接受
	{
		memcpy(&msg_Arr[0].data,paramRec.RCV_data,8);
		recelenl=paramRec.RCV_data[1];	 //message len
		recelen=(recelenh&0X0F)*256+recelenl;
		if((recelen-6)%7==0)framenum=(recelen-6)/7;
		else framenum=(recelen-6)/7+1;
		serviceId=paramRec.RCV_data[2]; //SID
		*mulnum=framenum+2;
		//for(i=0,j=3;j<8;i++,j++)
		//	PIDData[i]=paramRec.RCV_data[j];//第一帧数据

		memset(&msg_send.data,0xff,8);
		msg_send.id	= 0x7E0;
		msg_send.data[0] = 0x30;  // PIC
		msg_send.data[1] = 0x00;  // Model 1
		msg_send.data[2] = 0x00;  // Model 1
		msg_send.len = 8;
		msg_send.format = CAN_ID_FORMAT_STANDARD;
	
		result = CAN_SendDataCancard2(xlChannel,&msg_send);
		if(result != CAN_RESULT_OK)return result;
		addframe=0;
		memcpy(&msg_Arr[1].data,msg_send.data,8);
		for(j=0;j<framenum;j++)
		{
 			result = CAN_ReadDataCancard2(xlChannel,&paramRec);
			if(result != CAN_RESULT_OK)return result;
			else
			{
				framenumth=paramRec.RCV_data[0]&0x0F;
				//for(m=1,k=7*(framenumth+(addframe*16)-1)+5;m<8;m++,k++)
				//{
				//	PIDData[k] = paramRec.RCV_data[m];//后续帧数据
				//}
				memcpy(&msg_Arr[j+2].data,paramRec.RCV_data,8);
				if(framenumth==0x0F)
					addframe++;
			}
		}
	}
	else
	{
		serviceId=paramRec.RCV_data [1];
	}
	memcpy(msg->data ,paramRec.RCV_data,8);
	if(serviceId == 0x7F)
	{
		//memcpy(msg->data ,paramRec.RCV_data,8);
		if(recelen < 3)
	 		 return CAN_ERROR_RESP_ARG_ABSENT;
		 return paramRec.RCV_data[3];
 		
		
	}
		
		break;
	case 2:
		XLevent xlEvent;
		xlEvent.tag                 = XL_TRANSMIT_MSG;
		xlEvent.tagData.msg.id      = msg->id;
		xlEvent.tagData.msg.dlc     = msg->len;
		xlEvent.tagData.msg.flags   = 0;
		memcpy(xlEvent.tagData.msg.data,msg->data,8);
		result=0;
		while (!result) {
				result = 1;
			result = xlReceive(m_CAN->m_xlPortHandle, &msgsrx, &xlEventRec);
			if ( result == XL_ERR_QUEUE_IS_EMPTY )break;
		}

		result = CAN_SendDataCancardxl(xlChannel,&xlEvent);
		if(result != XL_SUCCESS)
			return result; 

		result = CAN_ReadDataCancardxl(xlChannel,&xlEventRec);
		if(gstatus != XL_SUCCESS)
			return result; 
		recelenh=xlEventRec.tagData.msg.data[0];
		mulframe=recelenh&0XF0;
		if(mulframe==0x10)		//first frame 10   	receive	 多帧接受
		{
			memcpy(&msg_Arr[0].data,xlEventRec.tagData.msg.data,8);
			recelenl=xlEventRec.tagData.msg.data[1];	 //message len
			recelen=(recelenh&0X0F)*256+recelenl;
			if((recelen-6)%7==0)framenum=(recelen-6)/7;
			else framenum=(recelen-6)/7+1;
			serviceId=xlEventRec.tagData.msg.data[2]; //SID

			*mulnum=framenum+2;
			//for(i=0,j=3;j<8;i++,j++)
			//	PIDData[i]=xlEventRec.tagData.msg.data[j];

			memset(&xlEventSend.tagData.msg.data,0xff,8);
			//for(i=0;i<10;i++)
			{
				xlEventSend.tag = XL_TRANSMIT_MSG;
				xlEventSend.tagData.msg.id	=0x7E0;
				xlEventSend.tagData.msg.data[0]	=0x30;  // PIC
				xlEventSend.tagData.msg.data[1]	=0x00;  // Model 1
				xlEventSend.tagData.msg.data[2]	=0x00;  // Model 1
				xlEventSend.tagData.msg.dlc = 8;
				xlEventSend.tagData.msg.flags = 0;

				gstatus = CAN_SendDataCancardxl(xlChannel,&xlEventSend);
				if(gstatus != XL_SUCCESS)return gstatus;
				memcpy(&msg_Arr[1].data,xlEventSend.tagData.msg.data,8);
				addframe=0;
				for(j=0;j<framenum;j++)
				{
					gstatus = CAN_ReadDataCancardxl(xlChannel,&xlEventRec);
					if(gstatus != XL_SUCCESS)return gstatus; 
					framenumth=xlEventRec.tagData.msg.data[0]&0x0F;
					//for(m=1,k=7*(framenumth+(addframe*16)-1)+5;m<8;m++,k++)
					//{
					//	PIDData[k] = xlEventRec.tagData.msg.data[m];
					//}
					memcpy(&msg_Arr[j+2].data,xlEventRec.tagData.msg.data,8);
					if(framenumth==0x0F)
						addframe++;
				}
			}	
		}
		else
		{
			serviceId=xlEventRec.tagData.msg.data[1];
		}
		memcpy(msg->data ,xlEventRec.tagData.msg.data,8);
		if(serviceId == 0x7F)
	{
		 
		if(recelen < 3)
	 		 return CAN_ERROR_RESP_ARG_ABSENT;
		 return xlEventRec.tagData.msg.data[3];
 		
		
	}
		
		break;
	case 3:
 			U8 framenumth;
 			int addframe; 
 			int recelen,recelenh,recelenl;
			WORD framenum;
			int result;
 			int dwReNum;

 			IEvent_Arr[0].ExternFlag=CAN_ID_FORMAT_STANDARD;
			IEvent_Arr[0].RemoteFlag=CAN_FRAME_TYPE_DATA;
			IEvent_Arr[0].DataLen=8;
			IEvent_Arr[0].ID=msg->id;
			memcpy(IEvent_Arr[0].Data,msg->data,8);

 		//	for(int i=0;i<10;i++)
			//{
			//	dwReNum = ICANReceive( m_DevType, m_DevIndex,xlChannel,gCanObj,50,50);
			//	if(dwReNum<=0)break;
			//}
			result = CAN_SendDataICAN(xlChannel,1);
			if(result != CAN_RESULT_OK)return result;
  			dwReNum=1;
	
			result = CAN_ReadDataICAN(xlChannel,&dwReNum);
			if(result != 0)return result;
			if(dwReNum == 1)index=0;
			else index=1;
 			recelenh=gCanObj[index].Data[0];
			mulframe=recelenh&0XF0;
			if(mulframe==0x10)		//first frame 10   	receive	 多帧接受
			{
				memcpy(&msg_Arr[0].data,gCanObj[index].Data,8);
				recelenl=gCanObj[index].Data[1];	 //message len
				recelen=(recelenh&0X0F)*256+recelenl;
				if((recelen-6)%7==0)framenum=(recelen-6)/7;
				else framenum=(recelen-6)/7+1;
				serviceId=gCanObj[index].Data[2];; //SID
				*mulnum=framenum+2;
				//for(i=0,j=0;j<8;i++,j++)
				//	PIDData[i]=gCanObj[0].Data[j];//第一帧数据

				memset(IEvent_Arr[0].Data,0xff,8);
				IEvent_Arr[0].ID	= 0x7e0;
				IEvent_Arr[0].Data[0] = 0x30;  // PIC
				IEvent_Arr[0].Data[1] = 0x00;  // Model 1
				IEvent_Arr[0].Data[2] = 0x00;  // Model 1
				IEvent_Arr[0].DataLen=8;
				IEvent_Arr[0].ExternFlag = CAN_ID_FORMAT_STANDARD;
				IEvent_Arr[0].RemoteFlag=CAN_FRAME_TYPE_DATA;
		 
				result = CAN_SendDataICAN(xlChannel,1);
				if(result != CAN_RESULT_OK)return result;
				memcpy(&msg_Arr[1].data,IEvent_Arr[index].Data,8);
				//RecordCommand(CAN_SEND,IEvent_Arr[0].Data,IEvent_Arr[0].ID,1,writetype);
				addframe=0;
				//for(j=0;j<framenum;j++)
				//{
				dwReNum=framenum;
				result = CAN_ReadDataICAN(xlChannel,&dwReNum);
				if(result != CAN_RESULT_OK)return result;
				else
				{
					for(j=0;j<framenum;j++)
					{
					//RecordCommand(CAN_RECEIVE,gCanObj[j].Data,gCanObj[j].ID,1,writetype);
					framenumth=gCanObj[j].Data[0]&0x0F;
					//for(m=1,k=7*(framenumth+(addframe*16)-1)+8;m<8;m++,k++)
					//{
					//	PIDData[k] = gCanObj[j].Data[m];//后续帧数据
					//}
					memcpy(&msg_Arr[j+2].data,gCanObj[j].Data,8);
					if(framenumth==0x0F)
						addframe++;
				}
				}
			}
			else serviceId=gCanObj[index].Data[1]; //SID
			 memcpy(msg->data ,gCanObj[index].Data,8);
				if(serviceId == 0x7F)
				{
		 
					if(recelen < 3)
	 					 return CAN_ERROR_RESP_ARG_ABSENT;
					return gCanObj[index].Data[3];
 				}
		 
		 break;
		case 4:
			long lResult; 
			 
			int rec_num;
			icsSpyMessage icsMsg_send,icsMsg_rec;
			icsSpyMessage stMessagesTx;

			stMessagesTx.ArbIDOrHeader      = msg->id;
			stMessagesTx.NumberBytesData     = msg->len;
			stMessagesTx.StatusBitField = 0;
			stMessagesTx.StatusBitField2 = 0;
			
			memcpy(stMessagesTx.Data,msg->data,8);

 			lResult = CAN_SendDataES581(&stMessagesTx,1);
			if(lResult != CAN_RESULT_OK)
				return lResult; 
			rec_num = 1;
			lResult = CAN_ReadDataES581(&stMessagesTx,&rec_num);
			if(lResult != CAN_RESULT_OK)
				return lResult; 
			if(rec_num == 1)index=0;
			else index=1;
			recelenh= msg_Arr[index].data[0];
			memcpy(msg->data ,msg_Arr[index].data,8);
			mulframe=recelenh&0XF0;
			if(mulframe==0x10)		//first frame 10   	receive	 多帧接受
			{
				recelenl=msg_Arr[index].data[1];	 //message len
				recelen=(recelenh&0X0F)*256+recelenl;
				if((recelen-6)%7==0)framenum=(recelen-6)/7;
				else framenum=(recelen-6)/7+1;
				serviceId=msg_Arr[index].data[2]; //SID
				*mulnum=framenum+1;
				memcpy(icsMsg_rec.Data,msg_Arr[index].data,8);
				//for(i=0,j=3;j<8;i++,j++)
				//	PIDData[i]=msg_Arr[0].data[j];

				memset(&icsMsg_send.Data,0xff,8);
 				{
					icsMsg_send.ArbIDOrHeader	=0x7E0;
					icsMsg_send.Data[0]	=0x30;  // PIC
					icsMsg_send.Data[1]	=0x00;  // Model 1
					icsMsg_send.Data[2]	=0x00;  // Model 1
					icsMsg_send.NumberBytesData = 8;
					icsMsg_send.StatusBitField = 0;
					icsMsg_send.StatusBitField2 = 0;
					lResult = CAN_SendDataES581(&icsMsg_send,1);
					if(lResult != CAN_RESULT_OK)
						return lResult; 
		 			rec_num = framenum;
					addframe=0;
					//for(j=0;j<framenum;j++)
					{
						rec_num = framenum;
						lResult = CAN_ReadDataES581(&stMessagesTx,&rec_num);
						if(lResult != CAN_RESULT_OK)return lResult; 
						else
						{
							
							for(j=framenum-1;j>=0;j--)
							 memcpy(&msg_Arr[j+1].data,&msg_Arr[j].data,8);
							memcpy(&msg_Arr[0].data,icsMsg_rec.Data ,8);
 						}
					}
				}	
				//memcpy(msg->data ,msg_Arr[2].data,8);
			}
			else serviceId=msg->data[1];
			if(serviceId == 0x7F)
				{
		 
					if(recelen < 3)
	 					 return CAN_ERROR_RESP_ARG_ABSENT;
					return msg->data[3];
		
		
				}
 		break;
	
		default:break;
	}
 		//recelen=recelenh&0X0F;
  
	//serviceId=msg->data[1];
	
	return result;
}
int CAN_protocol::InitICANDrv(unsigned int type)
{
	ICAN_INIT_DATA InitInfo[1];
	/*设置为500K波特率*/
	InitInfo->Timing0=0x00;
	InitInfo->Timing1=0x1C;
	/*接收标准的帧*/
	InitInfo->Filter=1;
	/*打开过滤器*/
	InitInfo->AccCode=0xfd000000;
	InitInfo->AccMask=0x00000000;
	InitInfo->Mode=0;
	
    //初始化通道0
 	if(ICANInitDrv(m_DevType,m_DevIndex,type,InitInfo)!=1)	//init can-0
	{
  		return -2;
	}
	Sleep(100);	 
	ICANResetCAN(m_DevType,m_DevIndex,type);
	int bRel = ICANClearBuffer(m_DevType,m_DevIndex,type);//清空缓存
    //启动通道0
	 
	if(ICANStartCAN(m_DevType,m_DevIndex, type)!=1)	//start can-0
	{
		AfxMessageBox(_T("Start-CAN failed!"));
		return -4;
	}
	return 1;
}

int CAN_protocol::InitICAN( )
 {
	int result;

 	if(g_CANDirverType==3)
	{
 		if(ICANCloseDriver(m_DevType,m_DevIndex)!=1)
		{
			AfxMessageBox(_T("Close failed！"));
			return -1;
		
		}
 	}
	xlChannel=0;
 	m_DevIndex=0;	
	m_DevType= DEV_USBCAN2;//m_nDevType == 1 ? DEV_USBCAN2 : DEV_USBCAN;//双通道 单通道
	DWORD Reserved=0;
	//打开设备
 	if(ICANOpenDriver(m_DevType,m_DevIndex,Reserved)!=1)
	{
 		return -1;
	}
	 
	 result =InitICANDrv(xlChannel);
	//ICAN_INIT_DATA InitInfo[1];
	///*设置为500K波特率*/
	//InitInfo->Timing0=0x00;
	//InitInfo->Timing1=0x1C;
	///*接收标准的帧*/
	//InitInfo->Filter=1;
	///*打开过滤器*/
	//InitInfo->AccCode=0xfd000000;
	//InitInfo->AccMask=0x00000000;
	//InitInfo->Mode=0;
	//xlChannel=0;
 //   //初始化通道0
	//if(ICANInitDrv(m_DevType,m_DevIndex,xlChannel,InitInfo)!=1)	//init can-0
	//{
 // 		return -2;
	//}
	//Sleep(100);	 

 //   //启动通道0
	//if(ICANStartCAN(m_DevType,m_DevIndex, xlChannel)!=1)	//start can-0
	//{
	//	AfxMessageBox(_T("Start-CAN failed!"));
	//	return -4;
	//}

    //初始化通道1
	//if(m_nDevType == 1)
	//{
	//	if(ICANInitDrv(m_DevType,m_DevIndex, 1,InitInfo)!=1)	//can-1
	//	{
	//		AfxMessageBox(_T("Init-CAN failed!"));
	//		return -5;
	//	}
	//}
	//Sleep(100);
 //   //启动通道1
	//if(ICANStartCAN(m_DevType,m_DevIndex, 1)!=1)	//start can-1
	//{
	//	AfxMessageBox(_T("Start-CAN failed!"));
	//	return -6;
	//}
	return result;
 }
 int CAN_protocol::LoadIcan()
{
	// TODO:  
	HMODULE usbCanDll = LoadLibraryW(L"ican.dll");

	if(usbCanDll != NULL){

		ICANOpenDriver = (ICANOPENDRIVER)GetProcAddress(usbCanDll,"ICANOpenDriver");
		if(ICANOpenDriver == NULL){
			//GetDlgItem(IDC_STATIC_STATUS)->SetWindowText(_T("<ERROR>Load ICAN error"));
			//AfxMessageBox(_T("Load ICAN error"));
			return -1;
		}

		ICANInitDrv = (ICANINITDRV)GetProcAddress(usbCanDll,"ICANInitDrv");
		if(ICANInitDrv == NULL){
			//AfxMessageBox(_T("Load ICAN error"));
			return -1;
		}

		ICANSetReference2 = (ICANSETREFERENCE2)GetProcAddress(usbCanDll,"ICANSetReference2");
		if(ICANSetReference2 == NULL){
			//AfxMessageBox(_T("Load ICAN error"));
			return -1;
		}

		ICANTransmit = (ICANTRANSMIT)GetProcAddress(usbCanDll,"ICANTransmit");
		if(ICANTransmit == NULL){
			//AfxMessageBox(_T("Load ICAN error"));
			return -1;
		}

		ICANClearBuffer = (ICANCLEARBUFFER)GetProcAddress(usbCanDll,"ICANClearBuffer");
		if(ICANClearBuffer == NULL){
			//AfxMessageBox(_T("Load ICAN error"));
			return -1;
		}

		ICANStartCAN = (ICANSTARTCAN)GetProcAddress(usbCanDll,"ICANStartCAN");
		if(ICANStartCAN == NULL){
			//AfxMessageBox(_T("Load ICAN error"));
			return -1;
		}

		ICANResetCAN = (ICANRESETCAN)GetProcAddress(usbCanDll,"ICANResetCAN");
		if(ICANResetCAN == NULL){
			//AfxMessageBox(_T("Load ICAN error"));
			return -1;
		}

		ICANCloseDriver = (ICANCLOSEDRIVER)GetProcAddress(usbCanDll,"ICANCloseDriver");
		if(ICANCloseDriver == NULL){
			//AfxMessageBox(_T("Load ICAN error"));
			return -1;
		}

		ICANGetReceiveNum = (ICANGETRECEIVENUM)GetProcAddress(usbCanDll,"ICANGetReceiveNum");
		if(ICANGetReceiveNum == NULL){
			//AfxMessageBox(_T("Load ICAN error"));
		}

		ICANReceive = (ICANRECEIVE)GetProcAddress(usbCanDll,"ICANReceive");
		if(ICANReceive == NULL){
			//AfxMessageBox(_T("Load ICAN error"));
			return -1;
		}

	}else{
			//AfxMessageBox(_T("Load ICAN error"));
			return -1;
	}
	return 0;

}
