/*----------------------------------------------------------------------------
| File:
|   ICAN.h
| Project:
|   ICAN device driver interface for WinNT/Win2000/WinXP/Win7
|
| Description:
|   Driver Interface Prototypes - customer version
|
|-----------------------------------------------------------------------------
| $Author: nealcao $    $Locker: $   $Revision: 42 $
|-----------------------------------------------------------------------------
| Copyright (c) 2013 by ICAN professional studio.  All rights reserved.
 ----------------------------------------------------------------------------*/

#pragma once
#ifndef __ICAN__H__
#define __ICAN__H__

//

#ifdef __cplusplus
extern "C" {
#endif

 
//接口卡类型定义
#define DEV_USBCAN			3	//单通道USB-CAN适配器
#define DEV_USBCAN2			4	//双通道USB-CAN适配器

//函数调用返回状态值
#define	STATUS_OK			1	//操作成功
#define STATUS_ERR1			0	//操作失败
#define STATUS_ERR2			-1	//设备未打开


//定义CAN信息帧的数据类型。
typedef  struct  _ICAN_EVENT{
	UINT	ID;
	UINT	TimeStamp;	 			//时间标识
	BYTE	TimeFlag;					//是否使用时间标识
	BYTE	SendType;					//发送标志。保留，未用
	BYTE	RemoteFlag;				//是否是远程帧
	BYTE	ExternFlag;				//是否是扩展帧
	BYTE	DataLen;
	BYTE	Data[8];
	BYTE	Reserved[3];			//保留
}ICAN_EVENT,*PICAN_EVENT;

//定义初始化CAN的数据类型
typedef struct _INIT_DATA{
	DWORD	AccCode;
	DWORD	AccMask;
	DWORD	InitFlag;
	UCHAR	Filter;   //0,1接收所有帧。2标准帧滤波，3是扩展帧滤波。
	UCHAR	Timing0;
	UCHAR	Timing1;
	UCHAR	Mode;     //模式，0表示正常模式，1表示只听模式,2自测模式
}ICAN_INIT_DATA,*PICAN_INIT_DATA;

typedef DWORD (__stdcall* ICANOPENDRIVER)(DWORD DevType,DWORD DevIndex,DWORD Reserved);
ICANOPENDRIVER ICANOpenDriver;
typedef DWORD (__stdcall* ICANCLOSEDRIVER)(DWORD DevType,DWORD DevIndex);
ICANCLOSEDRIVER ICANCloseDriver;
typedef DWORD (__stdcall* ICANINITDRV)(DWORD DevType, DWORD DevIndex, DWORD CANIndex, PICAN_INIT_DATA pInitData);
ICANINITDRV ICANInitDrv;
typedef DWORD (__stdcall* ICANGETRECEIVENUM)(DWORD DevType,DWORD DevIndex,DWORD CANIndex);
ICANGETRECEIVENUM ICANGetReceiveNum;
typedef DWORD (__stdcall* ICANCLEARBUFFER)(DWORD DevType,DWORD DevIndex,DWORD CANIndex);
ICANCLEARBUFFER ICANClearBuffer;
typedef DWORD (__stdcall* ICANSTARTCAN)(DWORD DevType,DWORD DevIndex,DWORD CANIndex);
ICANSTARTCAN ICANStartCAN;
typedef DWORD (__stdcall* ICANRESETCAN)(DWORD DevType,DWORD DevIndex,DWORD CANIndex);
ICANRESETCAN ICANResetCAN;
typedef DWORD (__stdcall* ICANRECEIVE)(DWORD DevType,DWORD DevIndex,DWORD CANIndex,PICAN_EVENT pReceive,ULONG Len,INT WaitTime);
ICANRECEIVE ICANReceive;
typedef DWORD (__stdcall* ICANTRANSMIT)(DWORD DeviceType,DWORD DeviceInd,DWORD CANInd,PICAN_EVENT pSend,DWORD Length);
ICANTRANSMIT ICANTransmit;

/*------------其他函数及数据结构描述---------------------------------*/
//定义常规参数类型
typedef struct _DATA_REGULAR{
	BYTE Mode;				//工作模式
	BYTE Filter;			//滤波方式
	DWORD AccCode;			//接收滤波验收码
	DWORD AccMask;			//接收滤波屏蔽码
	BYTE kBaudRate;			//波特率索引号，0-SelfDefine,1-5Kbps(未用),2-18依次为：10kbps,20kbps,40kbps,50kbps,80kbps,100kbps,125kbps,200kbps,250kbps,400kbps,500kbps,666kbps,800kbps,1000kbps,33.33kbps,66.66kbps,83.33kbps
	BYTE Timing0;
	BYTE Timing1;
	BYTE CANRX_EN;			//保留，未用
	BYTE UARTBAUD;			//保留，未用
}ICAN_DATA_REGULAR,*PICAN_DATA_REGULAR;	

//定义波特率设置参数类型
typedef struct _BAUD_TYPE{
	DWORD Baud;				//存储波特率实际值
	BYTE SJW;				//同步跳转宽度，取值1-4
	BYTE BRP;				//预分频值，取值1-64
	BYTE SAM;				//采样点，取值0=采样一次，1=采样三次
	BYTE PHSEG2_SEL;		//相位缓冲段2选择位，取值0=由相位缓冲段1时间决定,1=可编程
	BYTE PRSEG;				//传播时间段，取值1-8
	BYTE PHSEG1;			//相位缓冲段1，取值1-8
	BYTE PHSEG2;			//相位缓冲段2，取值1-8
}ICAN_BAUD_TYPE,*PICAN_BAUD_TYPE;

//定义Reference参数类型
typedef struct _PARAM_STRUCT{
	ICAN_DATA_REGULAR	NormalData;
	BYTE				Reserved;
	ICAN_BAUD_TYPE		BaudType;
}ICAN_PARAM_STRUCT,*PICAN_PARAM_STRUCT; 

typedef DWORD (__stdcall* ICANGETREFERENCE2)(DWORD DevType,DWORD DevIndex,DWORD CANIndex,DWORD Reserved,PICAN_PARAM_STRUCT pRefStruct);
ICANGETREFERENCE2 ICANGetReference2;
typedef DWORD (__stdcall* ICANSETREFERENCE2)(DWORD DevType,DWORD DevIndex,DWORD CANIndex,DWORD RefType,BYTE *pData);
ICANSETREFERENCE2 ICANSetReference2;


#ifdef __cplusplus
}
#endif   // _cplusplus

 #endif//#define __ICAN__H__