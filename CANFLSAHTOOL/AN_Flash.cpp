/*----------------------------------------------------------------------------
| File        : AN_Flash.cpp
| Project     : CANFlashtool 
|
| Description :  Reprogram process  
|-----------------------------------------------------------------------------
| $Author: liaoshaoyan      $Revision:  
| $Data    2014.04.09   
|-----------------------------------------------------------------------------
| Copyright (c) 2014 by liaoshaoyan.  All rights reserved.
|---------------------------------------------------------------------------*/
#include "StdAfx.h"
#include "AN_Flash.h"
#include "resource.h"
#include <stdio.h>
#include "Global.h"
#include "xlCANFunctions.h"
#include "RichEditCtrlEx.h"
 #define STEPLINESIZE 16
 #define LOCK 1
 #define UNLOCK 0
 #define SEEKA 0
 #define SEEKB 1
CString strQueryInfor;
CString strBootReason;
bool BootFlag=0;
extern  HWND hWnd;
extern	SYSTEMTIME pcStartTime;
U32 flashsize;

U8 Stepbuf_First[20][16]={
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0X00,0X00},
	{0x01,0x3e,0x00,0x00,0x00,0x00,0x7E,0x11,0xFD,0x0A,0XFF,0x0A,0X00,0X00,0X00,0X00},//3e
	{0x02,0x10,0x03,0x01,0x00,0x00,0x50,0x03,0xFD,0x0A,0XFF,0x0A,0X00,0X00,0X00,0X00},
	{0x03,0x85,0x02,0x00,0x00,0x00,0xC5,0x04,0xFD,0x04,0XFF,0x0A,0X00,0X00,0X00,0X00},
	{0x04,0x28,0x03,0x01,0x00,0x00,0x68,0x05,0xFD,0x05,0XFF,0x0A,0X00,0X00,0X00,0X00},
	{0x05,0x10,0x02,0x00,0x00,0x00,0x50,0x06,0xFD,0x0A,0XFF,0x0A,0X00,0X00,0X00,0X00},
	{0x06,0x22,0xF1,0x92,0x00,0x00,0x62,0x07,0xFD,0x0A,0XFF,0x07,0X00,0X00,0X00,0X00},//F192
	{0x07,0x22,0xF1,0xb0,0x00,0x00,0x62,0x08,0xFD,0x0A,0XFF,0x08,0X00,0X00,0X00,0X00},
	{0x08,0x22,0xF1,0x94,0x00,0x00,0x62,0x0F,0xFD,0x0A,0XFF,0x0F,0X00,0X00,0X00,0X00},
	{0x09,0x22,0xF1,0x95,0x00,0x00,0x62,0x10,0xFD,0x0A,0XFF,0x10,0x00,0x00,0X00,0X00},
	{0x0A,0xFB,0x01,0x04,0x01,0x00,0x00,0x0B,0x00,0x0D,0x00,0x00,0x00,0x00,0X00,0X00},
	{0x0B,0xFC,0x00,0x00,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0X00,0X00},
	{0x0C,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0X00,0X00},
	{0x0D,0xee,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0X00,0X00},

	{0x0E,0x22,0xF1,0xb2,0x00,0x00,0x62,0x10,0xFD,0x0A,0XFF,0x10,0x00,0x00,0X00,0X00},
	{0x0F,0x22,0x01,0xA0,0x00,0x00,0x62,0x09,0xFD,0x0A,0XFF,0x0E,0x00,0x00,0X00,0X00},
	{0x10,0x22,0xF1,0x8c,0x00,0x00,0x62,0x12,0xFD,0x0A,0XFF,0x12,0x00,0x00,0X00,0X00},
	{0x11,0x09,0x06,0x00,0x00,0x00,0x49,0x02,0xFD,0x02,0XFF,0x02,0x00,0x00,0X00,0X00},
	{0x12,0x22,0xF1,0xB1,0x00,0x00,0x62,0x0C,0xFD,0x0A,0XFF,0x0C,0x00,0x00,0X00,0X00},

};
U8 Stepbuf_First_Choose[20][16]={
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0X00,0X00},
	{0x01,0x3e,0x00,0x00,0x00,0x00,0x7E,0x02,0xFD,0x0A,0XFF,0x0A,0X00,0X00,0X00,0X00},//3e
	{0x02,0x10,0x03,0x01,0x00,0x00,0x50,0x03,0xFD,0x0A,0XFF,0x0A,0X00,0X00,0X00,0X00},
	{0x03,0x85,0x02,0x00,0x00,0x00,0xC5,0x04,0xFD,0x04,0XFF,0x0A,0X00,0X00,0X00,0X00},
	{0x04,0x28,0x03,0x01,0x00,0x00,0x68,0x05,0xFD,0x05,0XFF,0x0A,0X00,0X00,0X00,0X00},
	{0x05,0x10,0x02,0x00,0x00,0x00,0x50,0x10,0xFD,0x0A,0XFF,0x10,0X00,0X00,0X00,0X00},
	{0x06,0x22,0xF1,0x92,0x00,0x00,0x62,0x07,0xFD,0x0A,0XFF,0X07,0X00,0X00,0X00,0X00},
	{0x07,0x22,0xF1,0xb0,0x00,0x00,0x62,0x08,0xFD,0x0A,0XFF,0x08,0X00,0X00,0X00,0X00},
	{0x08,0x22,0xF1,0x94,0x00,0x00,0x62,0x0F,0xFD,0x0A,0XFF,0x0F,0X00,0X00,0X00,0X00},
	{0x09,0x22,0xF1,0x95,0x00,0x00,0x62,0x10,0xFD,0x0A,0XFF,0x10,0x00,0x00,0X00,0X00},
	{0x0A,0xFB,0x01,0x04,0x01,0x00,0x00,0x0B,0x00,0x0D,0x00,0x00,0x00,0x00,0X00,0X00},
	{0x0B,0xFC,0x00,0x00,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0X00,0X00},
	{0x0C,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0X00,0X00},
	{0x0D,0xee,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0X00,0X00},

	{0x0E,0x22,0xF1,0xb2,0x00,0x00,0x62,0x10,0xFD,0x0A,0XFF,0x10,0x00,0x00,0X00,0X00},
	{0x0F,0x22,0x01,0xA0,0x00,0x00,0x62,0x09,0xFD,0x0A,0XFF,0x0E,0x00,0x00,0X00,0X00},
	{0x10,0x22,0xF1,0x8c,0x00,0x00,0x62,0x12,0xFD,0x0A,0XFF,0x12,0x00,0x00,0X00,0X00},
	{0x11,0x09,0x06,0x00,0x00,0x00,0x49,0x02,0xFD,0x02,0XFF,0x02,0x00,0x00,0X00,0X00},
	{0x12,0x22,0xF1,0xB1,0x00,0x00,0x62,0x0C,0xFD,0x0A,0XFF,0x0C,0x00,0x00,0X00,0X00},
};
U8 Stepbuf_FlashNew[43][16]={
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0X00,0X00},
{0x01,0x10,0x03,0x00,0x00,0x00,0x50,0x25,0xFD,0x13,0XFF,0x13,0X00,0X00,0X00,0X00},
{0x02,0x27,0x00,0x00,0x00,0x00,0x67,0x03,0x11,0x27,0XFF,0x13,0X35,0x1A,0X37,0x18},//11
{0x03,0x31,0xF0,0x00,0x00,0x00,0x71,0x04,0xFD,0x13,0XFF,0x13,0X00,0X00,0X00,0X00},
{0x04,0x2e,0x00,0x00,0x00,0x00,0x6e,0x05,0xFD,0X05,0XFF,0X05,0X00,0X00,0X00,0X00},// 
{0x05,0x04,0x00,0x00,0x00,0x00,0x00,0x0A,0x00,0X06,0X00,0X00,0X00,0X00,0X00,0X00},//SID=04
{0x06,0x31,0xFF,0x00,0xBD,0x00,0x71,0x0A,0xFD,0x13,0XFF,0X07,0X00,0X00,0X00,0X00},
{0x07,0x01,0x00,0x00,0x00,0x00,0x00,0x08,0x00,0X09,0X00,0X00,0X00,0X00,0X00,0X00},
{0x08,0x31,0xFF,0x00,0xBC,0x00,0x71,0x0A,0xFD,0x13,0XFF,0X0A,0X00,0X00,0X00,0X00},
{0x09,0x31,0xFF,0x00,0xBB,0x00,0x71,0x0A,0xFD,0x13,0XFF,0x13,0X00,0X00,0X00,0X00},
{0x0A,0x02,0x00,0x00,0x00,0x00,0x00,0x0B,0x00,0x0C,0x00,0x16,0x00,0x29,0x00,0x21},////SID=02
{0x0B,0x31,0xFF,0x00,0x01,0x00,0x71,0x0E,0xFD,0x13,0XFF,0x13,0X00,0X00,0X00,0X00},//01 APP
{0x0C,0x31,0xFF,0x00,0x05,0x00,0x71,0x0E,0xFD,0x13,0XFF,0x13,0X00,0X00,0X00,0X00},//05 CAL
{0x0D,0x31,0xFF,0x00,0x06,0x00,0x71,0x1c,0xFD,0x13,0XFF,0x13,0X00,0X00,0X00,0X00},//06 CAL

{0x0E,0x34,0x00,0x00,0x00,0x00,0x74,0x0F,0xFD,0x13,0XFF,0x13,0X00,0X00,0X00,0X00},
{0x0F,0x36,0x00,0x00,0x00,0x00,0x76,0x10,0xFD,0x13,0XFF,0x13,0X00,0X00,0X00,0X00},
{0x10,0x37,0x00,0x00,0x00,0x00,0x77,0x11,0xFD,0x13,0XFF,0x13,0X00,0X00,0X00,0X00},
{0x11,0x31,0xFF,0x01,0x00,0x00,0x71,0x12,0xFD,0x13,0XFF,0x13,0X02,0X1A,0X00,0X00},
{0x12,0x06,0x00,0x00,0x00,0x00,0x00,0x0B,0x00,0X0C,0X00,0X0D,0X00,0X15,0X00,0X00},

{0x13,0xFB,0x01,0x03,0x00,0x00,0x00,0x14,0x00,0x1A,0x00,0x00,0x00,0x00,0X00,0X00},
{0x14,0xFC,0x01,0x00,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0X00,0X00},//gai 1
{0x15,0x11,0x01,0x00,0x00,0x00,0x51,0x26,0xFD,0x13,0XFF,0x13,0X00,0X00,0X00,0X00},//1B

{0x16,0x31,0xFF,0x00,0x04,0x00,0x71,0x0E,0xFD,0x13,0XFF,0x13,0X00,0X00,0X00,0X00},
{0x17,0x31,0xFF,0x00,0x03,0x00,0x71,0x0E,0xFD,0x13,0XFF,0x13,0X00,0X00,0X00,0X00},
{0x18,0xFB,0x02,0x05,0x00,0x00,0x00,0x19,0x00,0x1A,0x00,0x00,0x00,0x00,0X00,0X00},
{0x19,0xFC,0x07,0x00,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0X00,0X00},
{0x1A,0xEE,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0X00,0X00},
{0x1B,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0X00,0X00},	
 
{0x1C,0x34,0x01,0x00,0x00,0x00,0x74,0x1D,0xFD,0x13,0XFF,0x13,0X00,0X00,0X00,0X00},//specail cal
{0x1D,0x36,0x01,0x00,0x00,0x00,0x76,0x1E,0xFD,0x13,0XFF,0x13,0X00,0X00,0X00,0X00},
{0x1E,0x37,0x00,0x00,0x00,0x00,0x77,0x1F,0xFD,0x13,0XFF,0x13,0X00,0X00,0X00,0X00},
{0x1F,0x31,0xFF,0x01,0x00,0x00,0x71,0x20,0xFD,0x13,0XFF,0x13,0X02,0X1A,0X00,0X00},
{0x20,0x07,0x00,0x00,0x00,0x00,0x00,0x0D,0x00,0X15,0X00,0X00,0X00,0X00,0X00,0X00},
{0x21,0x31,0xFF,0x00,0x02,0x00,0x71,0x0E,0xFD,0x13,0XFF,0x13,0X00,0X00,0X00,0X00},//02 APP+CAL

{0x22,0x22,0xF1,0x90,0x00,0x00,0x62,0x1B,0xFD,0x13,0XFF,0x24,0X00,0X00,0X00,0X00},
{0x23,0x10,0x02,0x00,0x00,0x00,0x50,0x02,0xFD,0x13,0XFF,0x13,0X00,0X00,0X00,0X00},
{0x24,0x22,0xF1,0xB2,0x00,0x00,0x62,0x1B,0xFD,0x13,0XFF,0x1B,0X00,0X00,0X00,0X00},
{0x25,0x28,0x03,0x01,0x00,0x00,0x68,0x23,0xFD,0x13,0XFF,0x13,0X00,0X00,0X00,0X00},
{0x26,0x22,0xf1,0x95,0x00,0x00,0x62,0x22,0xFD,0x0B,0XFF,0x22,0x00,0x00,0X00,0X00},
{0x27,0x10,0x03,0x00,0x00,0x00,0x50,0x28,0xFD,0x13,0XFF,0x13,0X00,0X00,0X00,0X00},
{0x28,0x27,0x00,0x00,0x00,0x00,0x67,0x03,0xFD,0x13,0XFF,0x13,0X35,0x1A,0X37,0x18},//11
{0x29,0x05,0x00,0x00,0x00,0x00,0x00,0x17,0x00,0x2A,0x00,0x16,0x00,0x17,0x00,0x21},//SID=02
{0x2A,0x31,0xFF,0x00,0x07,0x00,0x71,0x0E,0xFD,0x13,0XFF,0x13,0X00,0X00,0X00,0X00},
};
 
U8 Stepbuf_ECUINFO[28][16]={
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0X00,0X00},
{0x01,0x22,0xF1,0x92,0x00,0x00,0x62,0x02,0xFD,0x0B,0XFF,0x0A,0X00,0X00,0X00,0X00},
{0x02,0x22,0xF1,0xb0,0x00,0x00,0x62,0x03,0xFD,0x0B,0XFF,0x03,0X00,0X00,0X00,0X00},
{0x03,0x22,0xF1,0x94,0x00,0x00,0x62,0x04,0xFD,0x0B,0XFF,0x04,0X00,0X00,0X00,0X00},
{0x04,0x22,0xF1,0x95,0x00,0x00,0x62,0x07,0xFD,0x0B,0XFF,0x07,0x00,0x00,0X00,0X00},
{0x05,0xEE,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0X00,0X00},
{0x06,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0X00,0X00},	
{0x07,0x22,0x01,0xA0,0x00,0x00,0x62,0x08,0xFD,0x0B,0XFF,0x08,0x00,0x00,0X00,0X00},
{0x08,0x22,0xF1,0x90,0x00,0x00,0x62,0x09,0xFD,0x0B,0XFF,0x09,0x00,0x00,0X00,0X00},
{0x09,0x22,0xF1,0x8C,0x00,0x00,0x62,0x06,0xFD,0x0B,0XFF,0x06,0x00,0x00,0X00,0X00},

{0x0A,0x19,0xF1,0xB2,0x00,0x00,0x59,0x02,0xFD,0x0B,0XFF,0x02,0x01,0x05,0X00,0X00},
{0x0B,0xFB,0x01,0x02,0x01,0x00,0x00,0x0C,0x00,0x05,0x00,0x00,0x00,0x00,0X00,0X00},
{0x0C,0xFC,0x00,0x00,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0X00,0X00},
};
 
int result=0;
CAN_Flash::CAN_Flash(void)
{
 	 flashsize = 0;
	 CANNo = 1;
	basemodelnum = 0;
	endmodelnum = 0;
	past=1;
 
}
//****************************************
//函数名：GetPrebuftoDisplay(char *buf)
//功能 ： 将命令执行前显示信息放入队列里
//参数  ：buf：刷写信息
//返回值：void
//****************************************
void CAN_Flash::GetPrebuftoDisplay(char *buf)// 
{
	CAN_interactMsg *interactmsg=new CAN_interactMsg;
	CString str;
	str = buf;
	display(str,0);
	interactmsg->MsgState=MSGINFO;
	int len=strlen(buf);
 	memcpy(interactmsg->data,buf,len);
	memcpy(&interactmsg->data[len],"\r\n",3);
 	interactmsg->Iswrite =0;
	displayArray[listItemNum]=interactmsg;
	
	listItemNum++;
}
int gii;
//****************************************
//函数名：GetbuftoDisplay(int Respons,int type)
//功能 ： 将命令执行后显示信息放入队列里
//参数  ：Respons：结果,type：决定是否红色字体，还是蓝色字体
//返回值：void
//****************************************
void CAN_Flash::GetbuftoDisplay(int Respons,int type)//后
{  
 
	CString str;
	CAN_interactMsg *interactmsg=new CAN_interactMsg;
	interactmsg->Iswrite =0;
 	if(Respons== CAN_RESULT_OK)
	{
 		interactmsg->MsgState=MSGINFO;
		sprintf_s(interactmsg->data,"%s","OK\r\n");
		str=interactmsg->data;
		display(str,0);
	}
	else if(Respons == CAN_STOP)
	{
 		interactmsg->MsgState=MSGINFO;
		sprintf_s(interactmsg->data,"%s","CAN Communication is STOP\r\n");
		str=interactmsg->data;
		display(str,1); 
	}
	else
	{
		interactmsg->MsgState=MSGERR;
 
		if(Respons == CAN2USB_RESULT_TIMEOUT) sprintf_s(interactmsg->data,"%s","No Response\r\n");
		else sprintf_s(interactmsg->data,"%s","Negative Response\r\n"); 

		str=interactmsg->data;
  
		if(type == 0)display(str,0);
		else display(str,1);
 
 	}
	
	displayArray[listItemNum]=interactmsg;
 	listItemNum++;
}
//****************************************
//函数名：ResponsProcessing(int Respons,U8 *p,int step)
//功能 ： 根据结果得到下一步执行的位置
//参数  ：Respons：结果,*p：当前指令的位置，step：指令的序列号
//返回值：int
//****************************************
int CAN_Flash::ResponsProcessing(int Respons,U8 *p,int step)
{
	int stepNext=0;
	int i;
	CString str;
	 //Respons=0;
	GetbuftoDisplay(Respons,step);
	 if(Respons == CAN_STOP||Respons == CAN_RESULT_SYSTEM_ERROR)
	{
 		stepNext = 256;
		return stepNext;	
	}

	if(Respons==CAN2USB_RESULT_TIMEOUT)			//延时 oxfd	   SPS_TIMEOUT=-1
	{
		for(i=0;i<5;i++)
		{
			if(SPS_Inst.gotofield[i][0]==0xFD)
			{
				stepNext = SPS_Inst.gotofield[i][1];
				return stepNext;	
			} 
		}
	
	}
	else if(Respons==0)				   //有返回值	positive
	{
		for(i=0;i<5;i++)
		{
			if(SPS_Inst.gotofield[i][0]==0x40+SPS_Inst.opcode)	 //posiive response 
			{
				stepNext = SPS_Inst.gotofield[i][1];
				return stepNext;	
			} 
		}
	
	}
	else
	{
		 if(Respons==0x40+SPS_Inst.opcode)		//????	Respons是负数			 //negative response
		 {
		 	 for(i=0;i<5;i++)									  //跳过第一个	  0x40+SPS_Inst.opcode
			 {
				if(SPS_Inst.gotofield[i][0]==0x40+SPS_Inst.opcode)
				  break;
			 }
			 for(;i<5;i++)
			 {
				if(SPS_Inst.gotofield[i][0]==0x40+SPS_Inst.opcode)
				{
					stepNext = SPS_Inst.gotofield[i][1];
					return stepNext;
				}
			 }
			
		 }
		 else
		 {
		 	 for(i=0;i<5;i++)									  
			 {
				if(SPS_Inst.gotofield[i][0]==Respons)	  //use response code
				{
					stepNext = SPS_Inst.gotofield[i][1];
					return stepNext;	
				} 
			 }
			
		 }
	}
  	for(i=0;i<5;i++)					   //oxff
	{
		if(SPS_Inst.gotofield[i][0]==0xFF)
		{
			stepNext =SPS_Inst.gotofield[i][1];
			return stepNext;	
		} 
	}
	return stepNext;
}

int CAN_Flash::ReadfileStep(U8 *p,int step)
{
	if(step==0||step>255)
	return step; 
	
	memcpy(&SPS_Inst,p+STEPLINESIZE*step,STEPLINESIZE);
	return 1;
}

int CAN_Flash::CheckAscii(unsigned char *buf,int size)
{
	for(int i=0;i<size;i++)
	{
		if(buf[i]>128)return -1;
	}
	return 0;
}
//U32 g1,g2,g3;
//****************************************
//函数名：CAN_ReadStep(int type)
//功能 ： 刷写的流程
//参数  ：type：刷写类型。
//返回值：int
//****************************************
int CAN_Flash::CAN_ReadStep(int type)
{
	//??????????
	U8 ctrl=1;
 	U32 addr;
	u32 percent=0;
	U32 downloadlen,offset;
	u32  messagelen = 0;
	char buffer[500];
	CString str;
	U8 keylen=0;
	unsigned char bdata[100];
	CAN_ECUIdentification data;
	U32	SendId;
	U8 *p; 
	int i;
	U32 actionid;
	int AppFlash=0;
	int step =1;//gai 1
	int displaytrynum=0;
	unsigned int checksum,rechecksum,gsum,Allbanksum;
	CANNo=1;
	int currentMNum=0;
	int NextFlag=0;
	if(type == 0)p =  Stepbuf_First[0];  //开始刷写之前读取ecu信息
	else if(type == 1)p =  Stepbuf_FlashNew[0];//刷写流程
	else if(type == 2)p =  Stepbuf_ECUINFO[0];
	else if(type == 3)p =  Stepbuf_First_Choose[0];
	for(int i=0;i<10;i++)loopcounter[i]=0xff;
	BootFlag=0;
/*1 表示是否刷写bb, 2 表示刷写类型*/
	while(step<255&&step>0)
	{
		 ReadfileStep(p,step);
		 switch(SPS_Inst.opcode)
		{
		case 0x01:
			//str = "bb...";
			if(config->ECUprg.Utility_BB_Flag == TRUE)
				step=SPS_Inst.gotofield[1][1];
			else step=SPS_Inst.gotofield[0][1];
			break;
		case 0x02://具体刷哪种类型
			//str = "cal...";
			if(config->ECUprg.b_cal_app == MIXED )
				{step=SPS_Inst.gotofield[4][1];break;}
			 
			if(g_EcuFile == PTP_CAL || g_EcuFile == CUT)
			{step=SPS_Inst.gotofield[0][1];AppFlash =1;}
			else if(g_EcuFile == CAL) step=SPS_Inst.gotofield[1][1];
			else if(g_EcuFile == BOOT) step=SPS_Inst.gotofield[2][1];
			else step=SPS_Inst.gotofield[3][1];
			//else if(g_EcuFile == MFG) step=SPS_Inst.gotofield[3][1];
			//else if(g_EcuFile == TSW) step=SPS_Inst.gotofield[4][1];
 			break;
		case 0x03:
			if(config->ECUprg.b_cal_app == MIXED || g_EcuFile == BOOT || g_EcuFile == MFG|| g_EcuFile == TSW)
				step=SPS_Inst.gotofield[1][1];
			else
			{
				if(NextFlag == 1)
				{
					if(g_EcuFile == CAL)step=SPS_Inst.gotofield[1][1];
					else 
					{
						if(AppFlash == 1)step=SPS_Inst.gotofield[1][1];
						else step=SPS_Inst.gotofield[0][1];
						AppFlash = 1;
					}
 				}
				else step=SPS_Inst.gotofield[0][1];
			}
			break;
		case 0x04://判断是否是mixed 顺序刷bb
  			if(config->ECUprg.b_cal_app == MIXED)
				step=SPS_Inst.gotofield[1][1];
			else  
			{
				step=SPS_Inst.gotofield[0][1];
 			}
			break;
		case 0x05://具体刷哪种类型 扩展
			//str = "cal...";
			if(g_EcuFile == MFG) step=SPS_Inst.gotofield[0][1];
			else if(g_EcuFile == TSW) step=SPS_Inst.gotofield[1][1];
 			break;
		case 0x06://判断是否是mixed 顺序刷bb
			if(config->ECUprg.b_cal_app == MIXED || g_EcuFile == BOOT || g_EcuFile == MFG|| g_EcuFile == TSW)
				step=SPS_Inst.gotofield[3][1];
			else
			{
				if(AppFlash == 1)
				{
					if(NextFlag == 1)
					{
						step=SPS_Inst.gotofield[1][1];//cal
						AppFlash=0;
					}
					else step=SPS_Inst.gotofield[0][1];//app
				}
				else 
				{
					if(NextFlag == 1)
					{
						step=SPS_Inst.gotofield[3][1];
						if(config->ECUprg.SepCalCurNum>0)
						{
							step=SPS_Inst.gotofield[2][1];//sepcal
						}
					}
					else step=SPS_Inst.gotofield[1][1];
				}
 
			}
			break;
	case 0x07://判断是否是mixed 顺序刷bb
			if(NextFlag == 1)
			{
				step=SPS_Inst.gotofield[1][1];
						 
			}
			else step=SPS_Inst.gotofield[0][1];
		 
			break;
		case 0x09:
  			sprintf_s(buffer,"%s","Request Checksum...\0");
 			GetPrebuftoDisplay(buffer);
			result = CAN_ReadVehicleInfor(CANNo,0x7e0,SPS_Inst.actionfield[0],bdata);
			
			if(result == CAN_RESULT_OK)
				{
					int len=bdata[0]-3;//0 1 2
					if(len==4)
					actionid = ((bdata[4]&0xff) << 24) | ((bdata[5]&0xff) << 16) 
									| ((bdata[6]&0xff) << 8)| bdata[7]&0xff;
					else actionid = ((bdata[4]&0xff) << 8) | (bdata[5]&0xff);
					sprintf_s(buffer,"CutCheckSum: 0x%x",actionid);
				}					
				else
				{
					sprintf_s(buffer,"%s","CutCheckSum: Unkwon",actionid);
				}
				//strQueryInfor+="\r\n";
 				strQueryInfor += buffer;
				strQueryInfor+="\r\n";
				step=ResponsProcessing(result,p,0xff);
 			 break;

 		case 0xfb:  				//其他指令返回错误，一般跳到这一步
			if(loopcounter[SPS_Inst.actionfield[0]]==0xff)//这个loopcounter被初始化为0xff，
			{
				loopcounter[SPS_Inst.actionfield[0]]=SPS_Inst.actionfield[1]; //得到utility设置的尝试次数
			}
				
			loopcounter[SPS_Inst.actionfield[0]]--;	  //尝试次数每次减一
				
			if(loopcounter[SPS_Inst.actionfield[0]]>0)				
				step = SPS_Inst.gotofield[0][1];
			else
				step = SPS_Inst.gotofield[1][1];		//一般跳到0xee指令上
			if(SPS_Inst.actionfield[2] == 1)strQueryInfor="";
			break;
 		case 0xfc:  					 //delays	 ????			
			if(SPS_Inst.actionfield[3]==0) //in seconds
			{
				displaytrynum++;
				char dbuf[100];
				sprintf_s(dbuf,"Fail to communicate,try again %d...\r\n",displaytrynum);
 				GetPrebuftoDisplay(dbuf);
				for(;SPS_Inst.actionfield[0]>0;SPS_Inst.actionfield[0]--)
				Sleep(1000);		  //1s
 					
			}
			else							//in minutes
			{
				for(;SPS_Inst.actionfield[0]>0;SPS_Inst.actionfield[0]--)
				Sleep (1000);	   //60s
			}
                
			step = SPS_Inst.gotofield[0][1];
			break;
		case 0x19:
			unsigned short bDtc;
			i=1;			
			sprintf_s(buffer,"%s","ReadDTC...");
 			GetPrebuftoDisplay(buffer);
			memset(buffer ,0,500);
			result= CAN_ReadDTC(1,0x01,(unsigned char *)buffer);
			if(result == CAN_RESULT_OK)
			{	
  				for(i=1;i>100; )
				{
					bDtc	=(buffer[i] << 8) | (buffer[i+1]);
   					if(bDtc==0)break;
				}
 			}
			 
			if(bDtc==0x601)result=1;
			 step=ResponsProcessing(result,p,0xff);
		
			break;
		case 0x2E:
			if(config->ECUprg.g_bDongleFlag == FALSE)result=0;
			else
			{
				U32 bnum;
				bnum = config->ECUprg.Dongle_Time_seconds;
				if(bnum == 0)Sleep(1000);
				else Sleep(bnum);
  				sprintf_s(buffer,"%s","WriteByDataIdentifier...");
 				GetPrebuftoDisplay(buffer);
 				buffer[0]=(m_dwSN >> 24)&0xff;
				buffer[1]=(m_dwSN >> 16)&0xff;
				buffer[2]=(m_dwSN >> 8)&0xff;
				buffer[3]=m_dwSN&0xff;
 				buffer[4]=0x45;
 				buffer[5] = (char )(pcStartTime.wYear%100);
				buffer[6] = (char )pcStartTime.wMonth;
				buffer[7] = (char )pcStartTime.wDay;
				buffer[8] = (char )pcStartTime.wHour;
				buffer[9] = (char )pcStartTime.wMinute;
				result = CAN_Writeid(CANNo,0xde00,buffer,0x0d);
			}
			step=ResponsProcessing(result,p,0x0);
 			 break;
 		case 0x3E:
  			sprintf_s(buffer,"%s","Start Communication...\0");
 			GetPrebuftoDisplay(buffer);
			result = CAN_TesterPresent(CANNo);
			step=ResponsProcessing(result,p,0xff);
 			 break;
		case 0x10:
  			sprintf_s(buffer,"%s","Enable ECUExtendedDiagnosticSession...\0");
  			GetPrebuftoDisplay(buffer);
			if(SPS_Inst.actionfield[1] == 0x01)SendId=0x7df;
			else SendId=0x7e0;
 			result = CAN_DiagnosticSessionControl(CANNo,SendId,SPS_Inst.actionfield[0]);//0x7df
			if((g_bBypassFlag == 1)&&(SPS_Inst.actionfield[0] == 0x02))result=0;
			step=ResponsProcessing(result,p,0xff);
			Sleep(500);
			break;
		case 0x85:
  			sprintf_s(buffer,"%s","Control DTC Setting...\0");
 			GetPrebuftoDisplay(buffer);
			result = CAN_ControlDTCSetting(CANNo,0x7df,SPS_Inst.actionfield[0]);
			step=ResponsProcessing(result,p,0xff);
			break;
		case 0x28:
  			sprintf_s(buffer,"%s","Communication Control...");
 			GetPrebuftoDisplay(buffer);
			result = CAN_CommunicationControl(CANNo,0x7df,SPS_Inst.actionfield[0],SPS_Inst.actionfield[1]);
			step=ResponsProcessing(result,p,0xff);
			Sleep(500);
			start_clear=0;
			break;
		case 0x22:
   			sprintf_s(buffer,"%s","ReadDataByIdentifier...");
 			GetPrebuftoDisplay(buffer);
			actionid = (SPS_Inst.actionfield[0]<<8) | (SPS_Inst.actionfield[1]);
			
			for(int i=0;i<3;i++)
			{
				result = CAN_ReadDataByIdentifier(CANNo,0x7e0,actionid,&data);
				if(result != CAN2USB_RESULT_TIMEOUT)break;
				if(result == CAN_STOP)break;
			}
			step=ResponsProcessing(result,p,0x0);
			if(actionid == 0xf192)
			{
				if(result == CAN_RESULT_OK)
				{
					basemodelnum = (data.BaseModelNumber[0] << 24) | (data.BaseModelNumber[1] << 16) 
									| (data.BaseModelNumber[2] << 8)| data.BaseModelNumber[3];
					sprintf_s(buffer,"BaseModelNumber: %u",basemodelnum);
				}					
				else
				{
					for(int i=0;i<3;i++)
					{
						result = CAN_ReadDataByIdentifier(CANNo,0x7e0,0xf193,&data);
						if(result != CAN2USB_RESULT_TIMEOUT)break;
						if(result == CAN_STOP)break;
					}
					if(result == CAN_RESULT_OK)
					{
						basemodelnum = (data.BaseModelNumber[0] << 24) | (data.BaseModelNumber[1] << 16) 
										| (data.BaseModelNumber[2] << 8)| data.BaseModelNumber[3];
						sprintf_s(buffer,"BaseModelNumber: %u",basemodelnum);
					}
					else
 						sprintf_s(buffer,"%s","BaseModelNumber: Unkwon",basemodelnum);
				}
				//strQueryInfor+="\r\n";
 				strQueryInfor += buffer;
				strQueryInfor+="\r\n";
			}
			if(actionid == 0xf1b0)
			{
				if(result == CAN_RESULT_OK)
				{
					endmodelnum = (data.EndModelNumber[0] << 24) | (data.EndModelNumber[1] << 16) 
								| (data.EndModelNumber[2] << 8)| data.EndModelNumber[3];
		 			sprintf_s(buffer,"EndModelNumber:  %u",endmodelnum);
				}
				else
				{
					sprintf_s(buffer,"EndModelNumber:  Unkwon",basemodelnum);
				}
 				strQueryInfor += buffer;
				strQueryInfor+="\r\n";
			}
			if(actionid == 0xf2b1)
			{
				if(result == CAN_RESULT_OK)
				{
					int bcheck = CheckAscii(data.CALID,sizeof(data.CALID));
					if(bcheck == 0)
						sprintf_s(buffer,"MainSoftwareCalID:   %.16s    ",data.CALID);
					else sprintf_s(buffer,"MainSoftwareCalID:   Unkwon    ",data.CALID);
 				}
				else
				{
					sprintf_s(buffer,"MainSoftwareCalID:  Unkwon");
				}
 				strQueryInfor += buffer;
				strQueryInfor+="\r\n";
			}
			if(actionid == 0xf194)
			{
				if(result == CAN_RESULT_OK)
				{
					int bcheck = CheckAscii(data.SWNumber,sizeof(data.SWNumber));
					if(bcheck == 0)
 					sprintf_s(buffer,"SoftwareNumber:   %.8s    ",data.SWNumber);
					else sprintf_s(buffer,"SoftwareNumber:   Unkwon    ",data.SWNumber);
				}
				else
				{
 					sprintf_s(buffer,"SoftwareNumber:   Unkwon    ",data.SWNumber);
				}
 				strQueryInfor += buffer;
				strQueryInfor+="\r\n";
			}
			if(actionid == 0xf195)
			{
				if(result == CAN_RESULT_OK)
				{
					int bcheck = CheckAscii(data.SWName,sizeof(data.SWName));
					if(bcheck == 0)
						sprintf_s(buffer,"SoftwareVersionNumber:   %.10s    ",data.SWName);
					else sprintf_s(buffer,"SoftwareVersionNumber:   Unkwon    ",data.SWName);
				}
				else
				{
					sprintf_s(buffer,"SoftwareVersionNumber:   Unkwon    ",data.SWName);
				}
				strBootReason =buffer;
  				//strQueryInfor += buffer;
				strBootReason+="\r\n";
 			//	strQueryInfor += buffer;
				//strQueryInfor+="\r\n";
 			}
			if(actionid == 0xf190)
			{
				if(result == CAN_RESULT_OK)
				{
					int bcheck = CheckAscii(data.VIN,sizeof(data.VIN));
					if(bcheck == 0)
						sprintf_s(buffer,"VIN:   %.17s    ",data.VIN);
					else sprintf_s(buffer,"VIN:   Unkwon    ",data.VIN);
					
				}
				else
				{
					sprintf_s(buffer,"VIN:   Unkwon    ",data.VIN);
					BootFlag=1;
				}
				strBootReason +=buffer;
  				//strQueryInfor += buffer;
				strBootReason+="\r\n";
			}
			if(actionid == 0x01A0)
			{
				if(result == CAN_RESULT_OK)
				{
					sprintf_s(buffer,"MEC(ManufacturingEnableCounter):   %d    ",data.CrankshaftCounter);
				}
				else
				{
					sprintf_s(buffer,"MEC(ManufacturingEnableCounter):   Unkwon    ",data.CrankshaftCounter);
				}
 				strQueryInfor += buffer;
				strQueryInfor+="\r\n";
			}
			if(actionid == 0xF1B2)
			{
				if(result == CAN_RESULT_OK)
				{
					if(data.BootReason !=0)//值为零
					{
						BootFlag=1;
						sprintf_s(buffer,"BootReason:   %x   (ECU in boot)",data.BootReason);
					}
					else 
					{
						sprintf_s(buffer,"BootReason:   %x   ",data.BootReason);
						BootFlag=0;
					}
					strBootReason=buffer;
  				}
				else
				{
					sprintf_s(buffer,"BootReason:   Unkwon   ",data.BootReason);
					strBootReason=buffer;
					BootFlag=0;
				}
				
 				
			}
			if(actionid == 0xf18c)
			{
				if(result == CAN_RESULT_OK)
				{
					int bcheck = CheckAscii(data.SerialNum,sizeof(data.SerialNum));
					if(bcheck == 0)
					{
						sprintf_s(buffer,"%s",(char *)data.SerialNum);
 						m_szSerialNum= buffer;
						sprintf_s(buffer,"ECU Serial Number:   %.20s    ",(char *)data.SerialNum);
						//m_szSerialNum= (char *)data.SerialNum;
					}
					else
					{
						m_szSerialNum=_T("Unkwon");
						sprintf_s(buffer,"ECU Serial Number:   Unkwon    ",data.SerialNum);
					}
				}
				else
				{
					m_szSerialNum=_T("Unkwon");
					sprintf_s(buffer,"ECU Serial Number:   Unkwon    ",data.SerialNum);
				}
 				strQueryInfor += buffer;
				strQueryInfor+="\r\n";
			}

			break;
		case 0x27:
			AppFlash = 0;
			currentMNum=0;
			CalFlashNum = 0;
			AppFlashNum = 0;
			Allbanksum=0;
			SepCalFlashNum = 0;
  			sprintf_s(buffer,"%s","Security Access..." );
 			GetPrebuftoDisplay(buffer);
			result = CAN_DiagnosticSecurityAccess();
			step=ResponsProcessing(result,p,0xff);
			break;
		case 0x31:			
			actionid = (SPS_Inst.actionfield[0]<<8) | (SPS_Inst.actionfield[1]);
			for(int i=0;i<3;i++)//0 2
			{
				if(actionid == 0xff00)//kernel
				{
					//str = "RoutineControl...";
					if(SPS_Inst.actionfield[2] == 0xBD){ sprintf_s(buffer,"%s","Boot is locked..."); Sleep(15);}
					else if(SPS_Inst.actionfield[2] == 0xBC)sprintf_s(buffer,"%s","lock Boot..."); 
					else if(SPS_Inst.actionfield[2] == 0xBB)sprintf_s(buffer,"%s","Unlock Boot..."); 
					else  {sprintf_s(buffer,"%s","EraseMemory...");Sleep(35);} 
				}
				else if(actionid == 0xf000) sprintf_s(buffer,"%s","RoutineControl..."); //eraser
				else sprintf_s(buffer,"%s","CheckProgrammingDependencies..."); //check
 		 
 				GetPrebuftoDisplay(buffer);
 				result = CAN_RoutineControl(CANNo,(U16)actionid,SPS_Inst.actionfield[2],bdata);
				if(result == 0)
				{
					if((actionid == 0xff01) && (g_nChecksumFlag == 1))
					{
						if(bdata[0]== 0x05)rechecksum = ((bdata[3]&0xff)<<8) | (bdata[4]&0xff);
						else rechecksum = ((bdata[5]&0xff)<<8) | (bdata[6]&0xff);
						if((rechecksum != (checksum&0xffff)) &&( rechecksum != (Allbanksum&0xffff)))
						{
							 result = 2;
						}
						//sprintf_s(buffer,"%02X %02X %02X %02X %02X %02X %02X",bdata[0],bdata[1],bdata[2],bdata[3],bdata[4],bdata[5],bdata[6]); 		 
 					//	GetPrebuftoDisplay(buffer);
						//sprintf_s(buffer,"\n ECU checksum:%04X",rechecksum&0xffff); 		 
 					//	GetPrebuftoDisplay(buffer);
						//sprintf_s(buffer,"\n Calculate checksum (section,total):%04X:%04X...",checksum&0xffff,Allbanksum&0xffff); 		 
 					//	GetPrebuftoDisplay(buffer);
					}

						
				}

				if( result!=CAN2USB_RESULT_TIMEOUT)break;
			}
			step=ResponsProcessing(result,p,0);
			
			break;
		case 0x11:
  			sprintf_s(buffer,"%s","ECUReset...");
 			GetPrebuftoDisplay(buffer);
			if(SPS_Inst.actionfield[1] == 0x01)SendId=0x7df;
			else SendId=0x7e0;
 			result = CAN_ECUReset(CANNo,0x7df);//0x7df
			step=ResponsProcessing(result,p,0xff);
			Sleep(2000);
			start_clear=1;
			break;
		case 0x14: 
  			sprintf_s(buffer,"%s","ClearDiagnosticInformation...");
 			GetPrebuftoDisplay(buffer);
 			result = CAN_ClearDiagnosticInformation(CANNo); 
			step=ResponsProcessing(result,p,0xff);
			break;
		case 0x37: 
		//	::SendMessage(hWnd,WM_USER+1,3,0);	 
  			sprintf_s(buffer,"%s","Request Transfer Exit...");
 			GetPrebuftoDisplay(buffer);
 			result = CAN_RequestTransferExit(CANNo); 
			step=ResponsProcessing(result,p,0xff);
			break;
		case 0x34: 
			 
  			if(AppFlash == 0)	//
			{
 				if(g_EcuFile == BOOT)
				{ 
					flashsize = config->ECUprg.Bootblock[1]-config->ECUprg.Bootblock[0]+1;
					addr = config->ECUprg.Bootblock[0];
				}
				else if(g_EcuFile == MFG)
				{ 
					flashsize =  config->ECUprg.Mfgblock[1]- config->ECUprg.Mfgblock[0]+1;
					addr = config->ECUprg.Mfgblock[0];
				}
				else if(g_EcuFile == TSW)
				{ 
					flashsize =  config->ECUprg.Tswblock[1]- config->ECUprg.Tswblock[0]+1;
					addr = config->ECUprg.Tswblock[0];
				}
				else if(config->ECUprg.b_cal_app == MIXED)
				{
					flashsize = config->Calsize;
					addr = config->ECUprg.Caladdr[0];
				}
				else 
				{ 
 					if(SPS_Inst.actionfield[0] == 0)
					{
						for(int i=currentMNum;i< config->ECUprg.CalAddrNum;i+=2 )
						{
							if(config->ECUprg.CaladdrFlag[i] == 1) 
							{
								currentMNum = i;
								break;
							}

						}
						if(config->ECUprg.CalRemaNum>0){
							addr = RemapAddress(CALFILE,currentMNum);
 						}
						else addr =config->ECUprg.Caladdr[currentMNum];
						if(g_EcuFile == CUT)
							pOpFile->GetModuleTransData(m_szCutPath,CALFILE,currentMNum);
						else if(g_EcuFile == PTP_CAL)
 							pOpFile->GetModuleTransDataFromCStringArray(m_szCalPath,CALFILE,currentMNum);
						else
 							pOpFile->GetModuleTransData(m_szCalPath,CALFILE,currentMNum);
 						flashsize = config->ECUprg.Caladdr[currentMNum+1]-config->ECUprg.Caladdr[currentMNum]+1;
						gsum=0;
						for(int i=0;i<flashsize;i++)
						{
							gsum+=Calibration.totalTransData.FileData[i];
						}
						CString str;
						str.Format(_T("%x"),gsum);

					}
					else
					{
							for(int i=currentMNum;i< config->ECUprg.SpecCalAddrNum;i+=2 )
						{
							if(config->ECUprg.SepCaladdrFlag[i] == 1) 
							{
								currentMNum = i;
								break;
							}

						}
						if(config->ECUprg.CalRemaNum>0){
							addr = RemapAddress(CALFILE,currentMNum);
 						}
						else addr =config->ECUprg.SpecCaladdr[currentMNum];
						if(g_EcuFile == CUT)
							pOpFile->GetModuleTransData(m_szCutPath,SEPCALFILE,currentMNum);
						else if(g_EcuFile == PTP_CAL)
 							pOpFile->GetModuleTransDataFromCStringArray(m_szCalPath,SEPCALFILE,currentMNum);
						else
 							pOpFile->GetModuleTransData(m_szCalPath,SEPCALFILE,currentMNum);
 						flashsize = config->ECUprg.SpecCaladdr[currentMNum+1]-config->ECUprg.SpecCaladdr[currentMNum]+1;
					}
				}
				
			}
			else//app
			{ 
 				for(int i=currentMNum;i< config->ECUprg.AppAddrNum;i+=2 )
				{
					if(config->ECUprg.AppaddrFlag[i] == 1) 
					{
						currentMNum = i;
						break;
					}

				}
				if(config->ECUprg.AppRemaNum>0){
					addr = RemapAddress(PTPFILE,currentMNum);
 				}
				else addr =config->ECUprg.Appaddr[currentMNum];
				if(g_EcuFile == CUT)	
					pOpFile->GetModuleTransData(m_szCutPath,PTPFILE,currentMNum);
				else  
 					pOpFile->GetModuleTransDataFromCStringArray(m_szPtpPath,PTPFILE,currentMNum);
 				flashsize = config->ECUprg.Appaddr[currentMNum+1]-config->ECUprg.Appaddr[currentMNum]+1;
				for(int i=0;i<flashsize;i++)
				{
					gsum+=Routine.totalTransData.FileData[i];
				}
				CString str;
				str.Format(_T("%x"),gsum);
				//MessageBox(str);
 			}
 			messagelen = config->ECUprg.DataSection_Size;  
 
 			sprintf_s(buffer, "%s","Request Download...");
 			GetPrebuftoDisplay(buffer);
			result=1;
 			Sleep(100);
			result = CAN_RequestDownload(CANNo,0x00,0x44,&addr,&flashsize);
			
 			step=ResponsProcessing(result,p,0xff);
			break;
		case 0x36:
			checksum=0;
 			downloadlen = flashsize;
			offset=0;
			result=0;
			ctrl=1;
			if(g_EcuFile ==  PTP_CAL)
			{
				if(AppFlash == 0)sprintf_s(buffer,"%s","Transfer CAL Data..."); 
				else sprintf_s(buffer,"%s","Transfer APP Data..."); 
			}
			else 
  			sprintf_s(buffer,"%s","Transfer Data...");
 			GetPrebuftoDisplay(buffer);
 			for(;downloadlen >messagelen&& result==0;downloadlen-=messagelen)//   
			{	
   				CAN_Readfile(offset,&cut_data[1],messagelen,AppFlash);
	//				if(AppFlash == 0)
 // 		memcpy(&cut_data[1],&Calibration.totalTransData.FileData[offset],messagelen);
	//else 
	//	memcpy(&cut_data[1],&Routine.totalTransData.FileData[offset],messagelen);
				for(int i=0;i<messagelen;i++)checksum+=cut_data[i+1];//
 				//result=1;
 				result=CAN_TransferData(CANNo,ctrl,messagelen);
  				offset+=messagelen;
				if(ctrl == 0xff)ctrl = 0;
				else ctrl++;
				if(ctrl%5 == 0)
				{
					percent = ((offset*100)/flashsize);
 					m_Process.SetPos(percent);
				}
 			}
			if(downloadlen>0&&result == 0 )	// 	
			{
				CAN_Readfile(offset,&cut_data[1],downloadlen,AppFlash);
				for(int i=0;i<downloadlen;i++)checksum+=cut_data[i+1];

				result=CAN_TransferData(CANNo,ctrl,downloadlen);
				offset += downloadlen;
 			}
			m_Process.SetPos(100);
			if(result ==0)
			{
				Allbanksum+=checksum;
			//	if(g_EcuFile == PTP_CAL || g_EcuFile == CAL || g_EcuFile == CUT)
				{
					currentMNum+=2;
					if(AppFlash == 0)	
					{
						if(SPS_Inst.actionfield[0] == 0)
						{
							CalFlashNum++;
							if(CalFlashNum >= config->ECUprg.CalCurNum){NextFlag=1;currentMNum=0;}
							else {NextFlag=0;  }
						}
						else
						{
							SepCalFlashNum++;
							if(SepCalFlashNum >= config->ECUprg.SepCalCurNum){NextFlag=1;currentMNum=0;}
							else {NextFlag=0;  }
						}
					}
					else 
					{
						AppFlashNum++; 
						if(AppFlashNum >= config->ECUprg.AppCurNum){NextFlag=1;currentMNum=0;}
						else {NextFlag=0;}
					}
				}
			}
			step=ResponsProcessing(result,p,0xff);
			break;				
		case 0xEE:
 			step = 0;
			break;
		case 0xFF:
			//if(BootFlag == 1)result=-16;
			//else
				result=0;
			step = 0;
			
 			break;
		default:
			step=256;
			result=-1;
			break;
		}

	}
	return result;
}
//****************************************
//函数名：RemapAddress(int type,int ctrl)
//功能 ： 映射地址。
//参数  ：type：刷写文件类型。ctrl：序列值，地址段的序号。 
//返回值：u32 返回地址信息
//****************************************
u32 CAN_Flash::RemapAddress(int type,int ctrl)//pOpfile
{
 	u32 addr;
	int flag=0;
	if(type == CALFILE)
	{
		for(int i=0;i<config->ECUprg.CalRemaNum;i+=2)
		{
			if(config->ECUprg.Caladdr[ctrl]==config->ECUprg.CalRemapaddr[i])
			{
				addr = config->ECUprg.CalRemapaddr[i+1];
				flag = 1;
 				break;
			}
		}
		if(flag == 0)addr = config->ECUprg.Caladdr[ctrl];
	}
	else
	{
		for(int i=0;i<config->ECUprg.AppRemaNum;i+=2)
		{
			if(config->ECUprg.Appaddr[ctrl]==config->ECUprg.AppRemapaddr[i])
			{
				addr = config->ECUprg.AppRemapaddr[i+1];
				flag = 1;
 				break;
			}
		}
		if(flag == 0)addr = config->ECUprg.Appaddr[ctrl];
	}
	return addr;
}
//****************************************
//函数名：CAN_Readfile(long position,char* buffer,U32 lenth,int flag)
//功能 ： 将标定数据加载到缓存
//参数  ：position：偏移位置。buffer：缓存。lenth：读取的字节数。flag：判断是app还cal数据。
//返回值：int
//****************************************
int CAN_Flash::CAN_Readfile(long position,unsigned char* buffer,U32 lenth,int flag)//pOpfile
{
	if(flag == 0)
  		memcpy(buffer,&Calibration.totalTransData.FileData[position],lenth);
	else 
		memcpy(buffer,&Routine.totalTransData.FileData[position],lenth);
 		return 1;	 
}
//****************************************
//函数名：display(CString buffer)
//功能  ：刷写过程中显示提示信息
//参数  ：CString buffer:将要输出的字符串
//返回值：void
//****************************************

void  CAN_Flash::display(CString buffer,int result)
{	
	m_Infor.UpdateData(TRUE);
	CHARFORMAT cf;
	m_Infor.GetDefaultCharFormat(cf);//(bool)(cf.dwEffects & CFE_BOLD)
	if(result == 0)
		m_Infor.AddMessgaeText(buffer,cf.szFaceName,cf.yHeight/15,(bool)(cf.dwEffects & CFE_BOLD),
			0,0,cf.crTextColor);
	else
		m_Infor.AddMessgaeTextErr(buffer,cf.szFaceName,cf.yHeight/15,(bool)(cf.dwEffects & CFE_BOLD),
			0,0,cf.crTextColor);

 }
int CAN_Flash::CAN_DiagnosticSecurityAccess(void)
{
	int result;
	U8 keylen;
	if(g_nKeyAutoFlag == 0)
	{
		if(g_nKeyFlag == 2)
		{
			/*LoadPath(g_szKeyPath,(unsigned int *)&SeedOrKey);	*/	
			result = checkdllpathname(g_szKeyPath);
			if(result == 0)
				result = dealwithdllpathname(g_szKeyPath);
			else
			{
				if(g_szKeyPath.Find(_T("JAC"))!=-1)
				{		 
 					int result;	
 					U8 buf[100];
					memset(buf,0xff,100);
					result= CAN_ReadData(1,0x7e0,0xF18C,buf);				
					if(result ==0)
					{			 
						for(int i=0;i<4;i++)SA[i]= buf[i+18];//buf[i+18]
 					}		 
				}
				Sleep(500);		
				result=CAN_SecurityAccess(CANNo,g_nParamValue,&SeedOrKey,&keylen);
				if(result == 0)
				{
					result = LoadPath(g_szKeyPath,(unsigned int *)&SeedOrKey);
					result = CAN_SecurityAccess(CANNo,g_nParamValue+1,&SeedOrKey,&keylen);	 
				}
			}

		}
		else
		{
			result = CAN_SecurityAccess(CANNo,g_nParamValue,&SeedOrKey,&keylen);//0x01		
			if( result == CAN_RESULT_OK )
			{
				past =SeedOrKey;
 				if(g_nKeyFlag == 1)
				{					  
					SeedOrKey =g_nKeyValue;
					if(g_nKeyValue <= 0xffff)keylen=2;
					else keylen=4;
					result = CAN_SecurityAccess(CANNo,g_nParamValue+1,&SeedOrKey,&keylen);	 

				}
				else
				{ 
					if(SeedOrKey == 0x1234)
					{
	 					SeedOrKey  = 0x5678; //5678
 					}
					else if(SeedOrKey ==0xffff  )
					{
						SeedOrKey =0xffff;
 					}
					else if(SeedOrKey ==0x0000  )
					{
						SeedOrKey =0;
 					}
					else if(SeedOrKey ==0xAA55  )
					{
						SeedOrKey =0x55AA;
 					}
					else if(SeedOrKey ==0xffffffff  )
					{
						SeedOrKey =0xffffffff;
 					}
				result = CAN_SecurityAccess(CANNo,g_nParamValue+1,&SeedOrKey,&keylen);	 
				if(past ==0x00 )
					result=0;
				}
			}
		}
	}
	else
	{	
		int flag=0;
		result = CAN_SecurityAccess(CANNo,g_nParamValue,&SeedOrKey,&keylen);//0x01
		if( result == CAN_RESULT_OK )
		{
			if(SeedOrKey == 0x1234)
			{
	 			SeedOrKey  = 0x5678; //5678
 			}
			else if(SeedOrKey ==0xffff  )
			{
				SeedOrKey =0xffff;
 			}
			else if(SeedOrKey ==0x0000  )
			{
				SeedOrKey =0;
 			}
			else if(SeedOrKey ==0xAA55  )
			{
				SeedOrKey =0x55AA;
 			}
			else if(SeedOrKey ==0xffffffff  )
			{
				SeedOrKey =0xffffffff;
 			}			
			else 
			{
				flag=1;
			}
			if(flag == 0)
			{
 				result = CAN_SecurityAccess(CANNo,g_nParamValue+1,&SeedOrKey,&keylen);	 
				if(SeedOrKey ==0x00 )
					result=0;
			}
			else result = getkeydll();
		}
		//else
		//{
		//	if(result == 0x37)return result;
		//	result = getkeydll();

		//}
	}
			return result;
}
//****************************************
//函数名：CAN_Diagnostic(void)
//功能 ：诊断命令及读取ECU信息
//参数  ：void
//返回值：int
//****************************************
int CAN_Flash::CAN_Diagnostic(void)
{
	int result;
	CString str;
	char buffer[100];
 	sprintf_s(buffer, "Clear NV RAM\r\n");
 	GetPrebuftoDisplay(buffer);
  
	sprintf_s(buffer, "Info:DiagnosticSessionControl...");
	GetPrebuftoDisplay(buffer);
	result = CAN_DiagnosticSessionControl(CANNo,0x7e0,0x03);
	GetbuftoDisplay(result,1);
	//if(result != 0)return result;
	sprintf_s(buffer, "Info:SecurityAccess...");
	GetPrebuftoDisplay(buffer);
	for(int i=0;i<3;i++)
	{
		result =CAN_DiagnosticSecurityAccess();
		if(result==0)break;
		else if(result == 0x37)
		{
			Sleep(5000);
			result = CAN_DiagnosticSessionControl(CANNo,0x7e0,0x03);
		}
	}
	GetbuftoDisplay(result,1);
	if(result != 0)return result;
	sprintf_s(buffer, "Info:Clear NV RAM...");
 	GetPrebuftoDisplay(buffer);
	for(int i=0;i<3;i++)
	{
		result = CAN_ClearNv(CANNo);
		if(result==0)break;
	}
	GetbuftoDisplay(result,1);
	if(result != 0)return result;

	return result;
}  
//****************************************
//函数名：LoadPath(CString path,unsigned int *seed)
//功能 ： 加载seek&key动态链接库
//参数  ：CString path：路径。unsigned int *seed：种子码
//返回值：int
//****************************************
 int CAN_Flash::LoadPath(CString path,unsigned int *seed)
{	
	hDLL = LoadLibrary(path);
	if(hDLL != NULL)  
	{  
		if(path.Find(_T("JAC"))!=-1)
		{
			FUNCF JACGetKey = FUNCF(GetProcAddress(hDLL, "GetLevelOneSecurityAccessKey"));
			if(JACGetKey != NULL)  
			{  
				unsigned int key=0;
				key=JACGetKey(*seed,SA);
				*seed=key;
				CString str;
				str.Format(_T("%x"),key);
				//MessageBox(str);
				FreeLibrary(hDLL);  
			}
			else return -2;

		}
		else
		{
 			FUNCE BAICGetKey = FUNCE(GetProcAddress(hDLL, "GetLevelOneSecurityAccessKey"));
			if(BAICGetKey != NULL)  
			{ 
				unsigned int key=0;
				key=BAICGetKey(*seed);
				*seed = key;
				CString str;
				str.Format(_T("%x"),key);
			//	MessageBox(str);
				FreeLibrary(hDLL);  
			}
		}
		
	}  
	else  
	{  
		// DLL not found in folder.
		// MessageBox(_T("Can't find dll."), NULL,MB_ICONERROR|MB_OK);
		//exit(1);
		return -1;
	}	
	return 1;
}
 
int CAN_Flash::checkdllpathname(CString strpath)
{
	CString str;
	 
	int len=0;
	int result;
	strpath.MakeUpper();
	int num = strpath.Find (_T("KEY_")); 
	if(num >0)
	{
		str=strpath.Mid(num+4);
		while(1)
		{
			num =str.Find (_T("_")); 
			if(num ==2)
			{
				str=str.Mid(num+1);
		 
				len++;
			}
			else break;
		}
	}
	else result =-1;
	 
	if(len >= 3)return 0;
	else return -1;
}
int CAN_Flash::dealwithdllpathname(CString strpath)
{
	CString str,strdllpath,strindex;
	strpath.MakeUpper();
	int num = strpath.Find (_T("KEY_")); 
	//strdllpath, str,  strindex
	if(num >0)
	{
		strdllpath=strpath.Left(num);
		str = strpath.Mid(num);
		strindex=strpath.Mid(num,6);
		result = seadkeycommand(strdllpath, str,  strindex);
	}
	else result = FINDDLL_ERROR;
	return result;
}
int CAN_Flash::seadkeycommand(CString strpath,CString strdllname,CString strindex)
{
	int num1;
	CString strCustomerName;
	unsigned char level;
	unsigned char seedkeynum;
	CString strdll;
	CString str1;
	int num = strdllname.Find(strindex);
	CString temp=strdllname.Mid(7);//Key_11_01_24_BAIC
	level=GetValue(temp,1);
	
	num = temp.Find(_T('_'));
	temp=temp.Mid(num+1);//   24_BAIC
	seedkeynum=GetValue(temp,1);
	
	num = temp.Find(_T('_'));////  BAIC_f
	temp=temp.Mid(num+1);
	str1=temp;

	num1 = temp.Find(_T('_')); 
	strCustomerName=temp.Left(num1);
	strdll=strpath + strdllname; 
		 
	if(strdllname.Find(_T("JAC"))!=-1)
	{		 
 		int result;	
 		U8 buf[100];
		memset(buf,0xff,100);
		result= CAN_ReadData(1,0x7e0,0xF18C,buf);				
		if(result ==0)
		{			 
			for(int i=0;i<4;i++)SA[i]= buf[i+18];//buf[i+18]
 		}		 
	}
	Sleep(1500);
	U8 keylen;
	result = CAN_SecurityAccess(CANNo,level,&SeedOrKey,&keylen);//0x01
//SeedOrKey=0x1122;
//result=CAN_RESULT_OK;
	if( result == CAN_RESULT_OK )
	{	
		result = LoadPath(strdll,(unsigned int *)&SeedOrKey);	
		if(result == -1)result=0x35;
		else
		{
			keylen = seedkeynum&0x0f;
			result = CAN_SecurityAccess(CANNo,level+1,&SeedOrKey,&keylen);	 
		}
	}
	return result;
}
int CAN_Flash::getkeydll()
{
	CAN_ECUIdentification data;
	unsigned char dllindex;
	CString strindex;

   TCHAR szPathOrig[300];
   GetModuleFileName(NULL, szPathOrig, 300);
   (_tcsrchr(szPathOrig,_T('\\')))[1]=0;
 
 
    CString strdllpath;
   strdllpath=szPathOrig;
	   strdllpath +=_T("Seed&Key DLL\\");
	for(int i=0;i<3;i++)
	{
		result = CAN_ReadDataByIdentifier(CANNo,0x7e0,0xF1F0,&data);
		if(result != CAN2USB_RESULT_TIMEOUT)break;
	}
//data.dllkeyword[0]=0X01;
//data.dllkeyword[1]=0X02;
//data.dllkeyword[2]=0Xff;
//result = CAN_RESULT_OK;
	if(result == CAN_RESULT_OK)
	{
		if(data.dllkeyword[0] == LOCK)
		{
			if(data.dllkeyword[1] < 0X80)dllindex=data.dllkeyword[1];
			else dllindex=data.dllkeyword[2];
			strindex.Format(_T("KEY_%02x"),dllindex);
			CString str;
			str.Empty();

			int i=0;
			unsigned char value;
			while(g_strdll[i]!=_T("\0"))
			{
				g_strdll[i].MakeUpper();

				int num = g_strdll[i].Find(_T("KEY_"));
				CString temp=g_strdll[i].Mid(4);//Key_11_01_24_BAIC
				value=GetValue(temp,1);
				if(value==dllindex)
				{
					str=g_strdll[i];
					
 					result = seadkeycommand(strdllpath, str,  strindex);
					return result;
 					
				}
				i++;
				str.Empty();
			}
			if(str ==_T("\0"))return FINDDLL_ERROR;
			else return result;
 		}
		else 
		{
			return 0;
		}
 	
	}
	else
	{
		return result;			 
	}
	 return result;	
}
unsigned char CAN_Flash::GetValue(CString str,int type)
{
	int offset,len,m;
	unsigned int data=0;
	unsigned char RowBuffer[100];
	offset = str.Find(_T('_'));
	CString strLeft = str.Left(offset);
	 
	CStringA strReadA(strLeft);
	strcpy_s((char *)RowBuffer,500,strReadA);
	len = strlen((const char *)RowBuffer);
	config->CharToHex(RowBuffer);

	if(type == 0)
	{
		for(m=0;m<len;m++)data = data*10 + RowBuffer[m];
	}
	else 
	{
		if(RowBuffer[0]== 0 && RowBuffer[1] == 'x')m=2;
		else if(RowBuffer[0]== 0 && RowBuffer[1] == 'X')m=2;
		else m=0;

		for(;m<len;m++)data = data*16 + RowBuffer[m];
	}
	return data;
}

