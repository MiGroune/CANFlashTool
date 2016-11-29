
#ifndef USB2CAN_INTERFACE_20100129
#define USB2CAN_INTERFACE_20100129

#ifdef CAN2USB_API_EXPORTS
#define CAN2USB_API __declspec(dllexport)
#else
#define CAN2USB_API __declspec(dllimport)
#endif

//EXTERN_C const GUID FAR GUID_CLASS_DATA_RECORD;

//CAN Frame type definition
#define CAN_FRAME_TYPE_DATA		0
#define CAN_FRAME_TYPE_REMOTE	1

//CAN Identifier format definition
#define CAN_ID_FORMAT_STANDARD	0
#define CAN_ID_FORMAT_EXTENDED	1

/* CAN message object structure                                              */
typedef struct  
{
  DWORD id;                 /* 29 bit identifier                               */
  BYTE	data[8];            /* Data field                                      */
  BYTE	len;                /* Length of data field in bytes                   */
  BYTE  ch;                 /* Object channel: 1  OR 2                         */
  BYTE  format;             /* 0 - STANDARD, 1- EXTENDED IDENTIFIER            */
  BYTE  type;               /* 0 - DATA FRAME, 1 - REMOTE FRAME                */
} CAN_msg,*pCAN_msg;

typedef struct _CAN2USB_interfaceInfo
{
	DWORD	version;
	char	releaseDate[20];
	char	author[20];
	char	manufacturer[50];
} CAN2USB_interfaceInfo;

//return code:
#define CAN2USB_RESULT_OK			0
#define CAN2USB_RESULT_NO_DEVICE	-1
#define CAN2USB_RESULT_SMALL_BUFFER	-2
#define CAN2USB_RESULT_SYSTEM_ERROR	-3
#define CAN2USB_RESULT_NOT_OPEN		-4
#define CAN2USB_RESULT_TIMEOUT		-5
#define CAN2USB_RESULT_REV_PARAM_ERROR	-6
#define CAN2USB_RESULT_DEV_DETACHED		-7 
#define CAN2USB_RESULT_PARAM_ERROR		-8


CAN2USB_API int  CAN2USB_registerNotification(HANDLE winHandle);

CAN2USB_API int CAN2USB_getInterfaceInfo(CAN2USB_interfaceInfo *interfaceInfo);



CAN2USB_API	int CAN2USB_openCAN();
CAN2USB_API	int CAN2USB_closeCAN();

CAN2USB_API	int CAN2USB_resetCAN(DWORD timeout); //millisecond
CAN2USB_API	int CAN2USB_setBaudRate(DWORD can1BaudRate,DWORD can2BaudRate,DWORD timeout); //millisecond

CAN2USB_API	int	CAN2USB_writeCAN(pCAN_msg pMsg,DWORD timeout); //millisecond
CAN2USB_API	int	CAN2USB_writeCAN_highspeed(CAN_msg msgArr[],WORD msgNum,WORD *sendMsgNum,DWORD timeout); //millisecond

CAN2USB_API	int	CAN2USB_readCAN(pCAN_msg pMsg,DWORD timeout);


#endif

