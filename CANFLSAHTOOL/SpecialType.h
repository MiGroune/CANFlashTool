#ifndef TYPE
#define TYPE
#pragma once
//#include "USB2CAN_interface.h"

typedef  unsigned char       U8;
typedef  unsigned short     U16;
typedef  unsigned long       U32;
typedef  unsigned long       u32;
typedef  unsigned long       u32;
typedef unsigned long long int U64;
#endif

#define   CUT    0
#define  CAL     1
#define  PTP_CAL 2
#define   BOOT    3
#define   MFG    4
#define   TSW    5
#define  PTP     6//gai 3
#define  MT80P                     "11111MT80P"
#define  MT80C                     "22222MT80C"
#define  MT80U                     "aaaaaMT80S"
#define  MT22P                     "33333MT22P"
#define  MT22C                     "44444MT22C"
#define  MT221P                    "5555MT221P"
#define  MT221C                    "6666MT221C"
#define  MT2211P                   "777MT2211P"
#define  MT2211C                   "888MT2211C"
#define  MT20U2P                   "999MT20U2P"
#define  MT20U2C                   "AAAMT20U2C"
#define  MT20UP                    "BBBBMT20UP"
#define  MT20UC                    "CCCCMT20UC"

#define  READLOG_COMMAND           "999readlog"
#define  READ_TIME_VERSION_COMMAND "17readtime"
#define  SYNTIME_COMMAND           "161616time"
#define  READBRUSHNUM_COMMAND      "151readcnt"

#define  MT221                      0
#define  MT2211                     1
#define  MT621                      2
#define  MT80                       3
#define  MT92                       4

#define  MT20U                      9
#define  MT221_768                  6
#define  MT05                       7
#define  MT20U2                     8



#define  UTILITYFILE                0                  //文件类型
#define  CUTFILE                    1
#define  CALFILE                    2
#define  PTPFILE                    3
#define  BOOTFILE                   4
#define  MFGFILE                    5
#define  SEPCALFILE                 6
#define  TSWFILE                    7
#define  ROUTINE                    0                  //文件内容的分类
#define  CALIBRATION                1
#define  UTILITY                    2

#define  LARGES19                   0
#define  SMALLS19                   1
#define  MT80CUT                    2
#define  MT80ERRSEL                 3

#define  FIVEMSIZE                  5242880

typedef struct _LimitTime
{
  unsigned short  LimitYear;                                            //2  字节   限制年份
  unsigned char   LimitMonth;                                           //1  字节   限制月份
  unsigned char   LimitDay;                                             //1  字节   限制日期
}LimitTimeType,*pLlimitTimeType;

typedef struct _TRANSFERFILEHEADER
{
  unsigned char   EcuFileTypeIdentifier[10];                             //10 字节  文件类型标识
  unsigned char   EcuFileSize[10];                                       //10 字节  文件大小  将文件大小转换成了字符表现形式
  unsigned char   Basemodel[4];                                          //4  字节　basemodel 码
  unsigned char   EcuFlag;                                               //1  字节  ecu类型标识
  unsigned char   LimitSelect;                                           //1  字节  限制标识 为1代表次数限制 为2代表时间限制  为3代表两种限制都进行了添加          //---8,30进行了重大改变
  unsigned short  LimitWriteNum;                                         //2  字节  允许刷写次数
  LimitTimeType   LimitTimeEnd;                                          //4  字节  限制的截止时间
  LimitTimeType   LimitTimeStart;                                        //4  字节  限制的起始时间
  unsigned short  EcuMemType;                                            //2  字节  1代表512    2代表768
  unsigned char   Unused[26];                                            //28 字节  unused 留作占位 不能舍弃
}TransferFileHeaderType,*pTransferFileHeaderType;

typedef struct _TRANSDATA                                                //Usb将要传输的数据流 包括64字节的头部和真正的data
{
	struct _TOTALTRANSDATA
	{
		TransferFileHeaderType TranseFileHeader;
		unsigned char          FileData[FIVEMSIZE];
	}totalTransData;
	unsigned long long     FileDataSize;                                 //将要传输的文件的大小
	char  S0_Data[300];  
	char  S7_Data[300]; 
	bool  S0_Flag;
	bool  S7_Flag;
}TransDataType,*pTransDataType;

typedef struct  _VersionBit{
	unsigned int  HardwareSerialNum:12;                   
	unsigned int  HardwareVersion:4;                                    //---12,12 硬件版本        0:老版     1：新版  
	unsigned int  FirmwareVersion:4;                                    //---12,12 下位机软件版本   0：老按键  1：新按键  2：...
	unsigned int  ButtonVersion :4;                                     //---12,12 按键版本        0：老按键  1：新按键  2：...
	unsigned int  Month :4;                                             //---12,12 月份 
	unsigned int  Year  :4;                                             //---12,12 年份 201x
}VersionBit;         
//extern unsigned char SoftVersion;                                             //软件版本
typedef struct _TimeVersion{
    WORD wYear;
    WORD wMonth;
    WORD wDayOfWeek;
    WORD wDay;
    WORD wHour;
    WORD wMinute;
    WORD wSecond;
    WORD wMilliseconds;
	VersionBit version;
	unsigned long SoftwareVersion;
} TimeVersion;

typedef struct _PackFileHeader
{
	unsigned short EcuTypeFlag;
	unsigned short fileNum;
	WCHAR fileName1[100];
	WCHAR fileName2[100];
	WCHAR fileName3[100];
}PackFileHeader,*pPackFileHeader;

#define  DEEPYELLOW       RGB(249, 252, 153)
#define  GROUPHEADER      RGB(201, 201, 151)
#define  SLOWYELLOW       RGB(255, 255, 204)
#define  BORDERREAD       RGB(255,0,0)
#define  TITLETEXT        _T("EcuFlashCustom--http://www.hardcan.com")


////////////////////////////
#define CAN_ERROR_RESP_SERVICE_ID		-1
#define CAN_ERROR_RESP_ARG_ABSENT		-2
#define CAN_ERROR_SERVICE_PROCESS		0x01000000

#define CAN_TIMING_PARAM_Read_limits		0x00
#define CAN_TIMING_PARAM_Set_to_default		0x01
#define CAN_TIMING_PARAM_Read_current		0x02
#define CAN_TIMING_PARAM_Set_values			0x03

#define CAN_SecurityAccess_Mode_requestSeed	0x01
#define CAN_SecurityAccess_Mode_sendKey		0x02



/////////////////////////////
 
/////////////////////////////
typedef struct _CAN_timingParam
{
	U16	P2min;
	U16	P2max;
	U16	P3min;
	U16	P3max;
	U16	P4min;
}CAN_timingParam;



typedef struct _CAN_ECUIdentification
{
	U8	VIN[17];    //109 NA
	U8	BaseModelNumber[4];				// P/N 4 Hex = N
	U8	SWNumber[8];   // System Supplier ECU Software Number
	U8	SWName[10]; 
	U8  TesterSerialNumber[10];  // 4 ASCII = N
	U8  ProgrammingReflashDate[4];
	U8  EndModelNumber[4];
	U8  CrankshaftCounter;
	U8  BootReason;
	U8  SerialNum[20];
	U8  CALID[16];
	U8  dllkeyword[10];
}CAN_ECUIdentification;

#define RENUM 1
