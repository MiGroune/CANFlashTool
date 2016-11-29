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

 
//�ӿڿ����Ͷ���
#define DEV_USBCAN			3	//��ͨ��USB-CAN������
#define DEV_USBCAN2			4	//˫ͨ��USB-CAN������

//�������÷���״ֵ̬
#define	STATUS_OK			1	//�����ɹ�
#define STATUS_ERR1			0	//����ʧ��
#define STATUS_ERR2			-1	//�豸δ��


//����CAN��Ϣ֡���������͡�
typedef  struct  _ICAN_EVENT{
	UINT	ID;
	UINT	TimeStamp;	 			//ʱ���ʶ
	BYTE	TimeFlag;					//�Ƿ�ʹ��ʱ���ʶ
	BYTE	SendType;					//���ͱ�־��������δ��
	BYTE	RemoteFlag;				//�Ƿ���Զ��֡
	BYTE	ExternFlag;				//�Ƿ�����չ֡
	BYTE	DataLen;
	BYTE	Data[8];
	BYTE	Reserved[3];			//����
}ICAN_EVENT,*PICAN_EVENT;

//�����ʼ��CAN����������
typedef struct _INIT_DATA{
	DWORD	AccCode;
	DWORD	AccMask;
	DWORD	InitFlag;
	UCHAR	Filter;   //0,1��������֡��2��׼֡�˲���3����չ֡�˲���
	UCHAR	Timing0;
	UCHAR	Timing1;
	UCHAR	Mode;     //ģʽ��0��ʾ����ģʽ��1��ʾֻ��ģʽ,2�Բ�ģʽ
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

/*------------�������������ݽṹ����---------------------------------*/
//���峣���������
typedef struct _DATA_REGULAR{
	BYTE Mode;				//����ģʽ
	BYTE Filter;			//�˲���ʽ
	DWORD AccCode;			//�����˲�������
	DWORD AccMask;			//�����˲�������
	BYTE kBaudRate;			//�����������ţ�0-SelfDefine,1-5Kbps(δ��),2-18����Ϊ��10kbps,20kbps,40kbps,50kbps,80kbps,100kbps,125kbps,200kbps,250kbps,400kbps,500kbps,666kbps,800kbps,1000kbps,33.33kbps,66.66kbps,83.33kbps
	BYTE Timing0;
	BYTE Timing1;
	BYTE CANRX_EN;			//������δ��
	BYTE UARTBAUD;			//������δ��
}ICAN_DATA_REGULAR,*PICAN_DATA_REGULAR;	

//���岨�������ò�������
typedef struct _BAUD_TYPE{
	DWORD Baud;				//�洢������ʵ��ֵ
	BYTE SJW;				//ͬ����ת��ȣ�ȡֵ1-4
	BYTE BRP;				//Ԥ��Ƶֵ��ȡֵ1-64
	BYTE SAM;				//�����㣬ȡֵ0=����һ�Σ�1=��������
	BYTE PHSEG2_SEL;		//��λ�����2ѡ��λ��ȡֵ0=����λ�����1ʱ�����,1=�ɱ��
	BYTE PRSEG;				//����ʱ��Σ�ȡֵ1-8
	BYTE PHSEG1;			//��λ�����1��ȡֵ1-8
	BYTE PHSEG2;			//��λ�����2��ȡֵ1-8
}ICAN_BAUD_TYPE,*PICAN_BAUD_TYPE;

//����Reference��������
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