#pragma once
#include "string.h"
#include <string>
 
// OpenPort "OpenType" Argument Constants
#define	NEOVI_COMMTYPE_RS232  0
#define	NEOVI_COMMTYPE_USB_BULK	1
#define	NEOVI_COMMTYPE_USB_ISO_DONT_USE	2
#define	NEOVI_COMMTYPE_TCPIP 3

//hardware constants
#define NEODEVICE_BLUE  1
#define NEODEVICE_DW_VCAN  4
#define NEODEVICE_FIRE  8
#define NEODEVICE_VCAN3  16
#define NEODEVICE_ANY  65535


//device ID's
const int NETID_DEVICE  = 0;
const int NETID_HSCAN = 1;
const int NETID_MSCAN = 2;
const int NETID_SWCAN = 3;
const int NETID_LSFTCAN = 4;
const int NETID_FORDSCP = 5;
const int NETID_J1708 = 6;
const int NETID_AUX = 7;
const int NETID_JVPW = 8;
const int NETID_ISO = 9;
const int NETID_LIN = 9;
const int NETID_ISOPIC= 10;
const int NETID_MAIN51 = 11;
const int NETID_SCI = 13;
const int NETID_ISO2 = 14;
const int NETID_FIRE_HSCAN1 = 41;
const int NETID_FIRE_HSCAN2 = 42;
const int NETID_FIRE_MSCAN1 = 43;
const int NETID_FIRE_MSCAN2 = 44;
const int NETID_FIRE_HSCAN3 = 44;
const int NETID_FIRE_SWCAN = 45;
const int NETID_FIRE_LSFT = 46;
const int NETID_FIRE_LIN1 = 47;
const int NETID_FIRE_LIN2 = 48;
const int NETID_FIRE_LIN3 = 49;
const int NETID_FIRE_LIN4 = 50;

const long SPY_STATUS_GLOBAL_ERR = 0x01;
const long SPY_STATUS_TX_MSG = 0x02;
const long SPY_STATUS_XTD_FRAME = 0x04;
const long SPY_STATUS_REMOTE_FRAME = 0x08;

const long SPY_STATUS_CRC_ERROR = 0x10;
const long SPY_STATUS_CAN_ERROR_PASSIVE = 0x20;
const long SPY_STATUS_INCOMPLETE_FRAME = 0x40;
const long SPY_STATUS_LOST_ARBITRATION = 0x80;

const long SPY_STATUS_UNDEFINED_ERROR = 0x100;
const long SPY_STATUS_CAN_BUS_OFF = 0x200;
const long SPY_STATUS_CAN_ERROR_WARNING = 0x400;
const long SPY_STATUS_BUS_SHORTED_PLUS = 0x800;

const long SPY_STATUS_BUS_SHORTED_GND = 0x1000;
const long SPY_STATUS_CHECKSUM_ERROR = 0x2000;
const long SPY_STATUS_BAD_MESSAGE_BIT_TIME_ERROR = 0x4000;
const long SPY_STATUS_IFR_DATA = 0x8000;

const long SPY_STATUS_COMM_IN_OVERFLOW = 0x10000;
const long SPY_STATUS_COMM_OUT_OVERFLOW = 0x20000;
const long SPY_STATUS_COMM_MISC_ERROR = 0x40000;
const long SPY_STATUS_BREAK = 0x80000;

const long SPY_STATUS_AVSI_REC_OVERFLOW = 0x100000;
const long SPY_STATUS_TEST_TRIGGER = 0x200000;
const long SPY_STATUS_AUDIO_COMMENT = 0x400000;
const long SPY_STATUS_GPS_DATA = 0x800000;

const long SPY_STATUS_ANALOG_DIGITAL_INPUT = 0x1000000;
const long SPY_STATUS_TEXT_COMMENT = 0x2000000;
const long SPY_STATUS_NETWORK_MESSAGE_TYPE = 0x4000000;
const long SPY_STATUS_VSI_TX_UNDERRUN = 0x8000000;

const long SPY_STATUS_VSI_IFR_CRC_BIT = 0x10000000;
const long SPY_STATUS_INIT_MESSAGE = 0x20000000;
const long SPY_STATUS_HIGH_SPEED = 0x40000000;

// Configuration Array constants
// high speed CAN neoVI / valuecan baud rate constants
const long NEO_CFG_MPIC_HS_CAN_CNF1 = 512 + 10;
const long NEO_CFG_MPIC_HS_CAN_CNF2 = 512 + 9;
const long NEO_CFG_MPIC_HS_CAN_CNF3 = 512 + 8;
const long NEO_CFG_MPIC_HS_CAN_MODE = 512 + 54;
// med speed CAN
const long NEO_CFG_MPIC_MS_CAN_CNF1 = 512 + 22;
const long NEO_CFG_MPIC_MS_CAN_CNF2 = 512 + 21;
const long NEO_CFG_MPIC_MS_CAN_CNF3 = 512 + 20;
//SW CAN
const long NEO_CFG_MPIC_SW_CAN_CNF1 = 512 + 34;
const long NEO_CFG_MPIC_SW_CAN_CNF2 = 512 + 33;
const long NEO_CFG_MPIC_SW_CAN_CNF3 = 512 + 32;
//LSFT CAN
const long NEO_CFG_MPIC_LSFT_CAN_CNF1 = 512 + 46;
const long NEO_CFG_MPIC_LSFT_CAN_CNF2 = 512 + 45;
const long NEO_CFG_MPIC_LSFT_CAN_CNF3 = 512 + 44;

// The second status bitfield
const long SPY_STATUS2_HAS_VALUE = 1;
const long SPY_STATUS2_VALUE_IS_BOOLEAN = 2;
const long SPY_STATUS2_HIGH_VOLTAGE = 4;
const long SPY_STATUS2_LONG_MESSAGE = 8;

const int SPY_PROTOCOL_CUSTOM = 0;
const int SPY_PROTOCOL_CAN = 1;
const int SPY_PROTOCOL_GMLAN = 2;
const int SPY_PROTOCOL_J1850VPW = 3;
const int SPY_PROTOCOL_J1850PWM = 4;
const int SPY_PROTOCOL_ISO9141 = 5;
const int SPY_PROTOCOL_Keyword2000 = 6;
const int SPY_PROTOCOL_GM_ALDL_UART = 7;
const int SPY_PROTOCOL_CHRYSLER_CCD = 8;
const int SPY_PROTOCOL_CHRYSLER_SCI = 9;
const int SPY_PROTOCOL_FORD_UBP = 10;
const int SPY_PROTOCOL_BEAN = 11;
const int SPY_PROTOCOL_LIN = 12;
const int SPY_PROTOCOL_J1708 = 13;
const int SPY_PROTOCOL_CHRYSLER_JVPW = 14;
const int SPY_PROTOCOL_J1939 = 15;


// these are used in status2 for Vehicle Spy 3
const long icsspystatusChangeLength =0x10;
const long icsspystatusChangeBitH1 = 0x20;
const long icsspystatusChangeBitH2 = 0x40;
const long icsspystatusChangeBitH3 = 0x80 ;
const long icsspystatusChangeBitB1 = 0x100;
const long icsspystatusChangeBitB2 = 0x200 ;
const long icsspystatusChangeBitB3 = 0x400 ;
const long icsspystatusChangeBitB4 = 0x800 ;
const long icsspystatusChangeBitB5 = 0x1000;
const long icsspystatusChangeBitB6 = 0x2000;
const long icsspystatusChangeBitB7 = 0x4000;
const long icsspystatusChangeBitB8 = 32768  ;
const long icsspystatusChangedGlobal = 65536 ;



//Structure for neoVI device types

typedef struct 
{
	int DeviceType;
	int Handle;
	int NumberOfClients;
	int SerialNumber;
	int MaxAllowedClients;
} NeoDevice;

typedef struct
{
    unsigned char Mode;
    unsigned char SetBaudrate;
    unsigned char Baudrate;
    unsigned char NetworkType;
    unsigned char TqSeg1;
    unsigned char TqSeg2;
    unsigned char TqProp;
    unsigned char TqSync;
    unsigned short BRP;
    unsigned short auto_baud;
} CAN_SETTINGS;

typedef struct
{
    unsigned short time_500us;
    unsigned short k;
    unsigned short l;
}ISO9141_KEYWORD2000__INIT_STEP;

typedef struct
{
    unsigned int Baudrate;
    unsigned short spbrg;
    unsigned short brgh;
    ISO9141_KEYWORD2000__INIT_STEP init_steps[16]; //See the ISO9141_KW2000__INIT_STEP structure
    unsigned char init_step_count;
    unsigned short p2_500us;
    unsigned short p3_500us;
    unsigned short p4_500us;
    unsigned short chksum_enabled;
} ISO9141_KW2000SETTINGS;


typedef struct
{
    unsigned char Mode;
    unsigned char SetBaudrate;
    unsigned char Baudrate;
    unsigned char NetworkType;
    unsigned char TqSeg1;
    unsigned char TqSeg2;
    unsigned char TqProp;
    unsigned char TqSync;
    unsigned short BRP;
    unsigned short high_speed_auto_switch;
    unsigned short auto_baud;
} SWCAN_SETTINGS;

typedef struct _LIN_SETTINGS
{
    unsigned int Baudrate;
    unsigned short spbrg;
    unsigned short brgh;
    unsigned char MasterResistor;
    unsigned char Mode;
} LIN_SETTINGS;

typedef struct 
{ 
    CAN_SETTINGS can1;   
    CAN_SETTINGS can2;
    CAN_SETTINGS can3;
    CAN_SETTINGS can4;
    SWCAN_SETTINGS swcan;    
    CAN_SETTINGS lsftcan;
    LIN_SETTINGS lin1;      
    LIN_SETTINGS lin2;
    LIN_SETTINGS lin3;
    LIN_SETTINGS lin4;
    unsigned short cgi_enable;
    unsigned short cgi_baud;
    unsigned short cgi_tx_ifs_bit_times;
    unsigned short cgi_rx_ifs_bit_times;
    unsigned short cgi_chksum_enable;
    unsigned short network_enables;
    unsigned short network_enabled_on_boot;
    unsigned short pwm_man_timeout;
    unsigned short pwr_man_enable;
    unsigned short misc_io_initial_ddr;
    unsigned short misc_io_initial_latch;
    unsigned short misc_io_analog_enable;
    unsigned short misc_io_report_period;
    unsigned short misc_io_on_report_events;
    unsigned short ain_sample_period;
    unsigned short ain_threshold;
    unsigned short iso15765_separation_time_offset; 
    unsigned short iso9141_kwp_enable;
    ISO9141_KW2000SETTINGS iso9141_kwp_settings; 
    unsigned short perf_en;
    unsigned short iso_parity; 
    unsigned short iso_msg_termination; 
}SFireSettings; 




typedef struct 
{
    CAN_SETTINGS can1;
    CAN_SETTINGS can2;
    unsigned short  network_enables;
    unsigned short network_enabled_on_boot;
    short iso15765_separation_time_offset;
    unsigned short perf_en;
    unsigned short misc_io_initial_ddr;
    unsigned short misc_io_initial_latch;
    unsigned short misc_io_report_period;
    unsigned short misc_io_on_report_events;
} SVCAN3Settings;



typedef struct // matching C structure
    {
	unsigned long StatusValue;  // 4
	unsigned long StatusMask;  // 4
	unsigned long Status2Value;  // 4
	unsigned long Status2Mask;  // 4
	unsigned long Header;	// 4
	unsigned long HeaderMask;  // 4
	unsigned long MiscData; // 4
	unsigned long MiscDataMask;  // 4 
	unsigned long ByteDataMSB;	// 4
	unsigned long ByteDataLSB;	// 4
	unsigned long ByteDataMaskMSB;  // 4
	unsigned long ByteDataMaskLSB;  // 4
	unsigned long HeaderLength; // 4 
	unsigned long ByteDataLength; // 4
	unsigned long NetworkID;	// 4
	unsigned short FrameMaster;	// 2
	unsigned char bUseArbIdRangeFilter;
	unsigned char bStuff2;
	unsigned long ExpectedLength;
	unsigned long NodeID;
	}  spyFilterLong;

typedef struct // matching C structure
    {
    unsigned long StatusBitField;	// 4
	unsigned long StatusBitField2;	// 4
    unsigned long TimeHardware;		// 4
	unsigned long TimeHardware2;	// 4
    unsigned long TimeSystem;		// 4
	unsigned long TimeSystem2;		// 4
	unsigned char TimeStampHardwareID;		// 1
	unsigned char TimeStampSystemID;
	unsigned char NetworkID;		// 1
	unsigned char NodeID;
	unsigned char Protocol;
	unsigned char MessagePieceID;	// 1
	unsigned char ColorID;			// 1
    unsigned char NumberBytesHeader;// 1
    unsigned char NumberBytesData;  // 1
	short DescriptionID;			// 2
    long ArbIDOrHeader;				// 4
    unsigned char Data[8];			
	unsigned char AckBytes[8];			
    float Value;
	unsigned char MiscData;
    }  icsSpyMessage;

typedef struct // matching C structure
    {
    unsigned long StatusBitField;	// 4
	unsigned long StatusBitField2;	// 4
    unsigned long TimeHardware;		// 4
	unsigned long TimeHardware2;	// 4
    unsigned long TimeSystem;		// 4
	unsigned long TimeSystem2;		// 4
	unsigned char TimeStampHardwareID;		// 1
	unsigned char TimeStampSystemID;	// 1
	unsigned char NetworkID;		// 1
	unsigned char NodeID;			// 1
	unsigned char Protocol;			// 1
	unsigned char MessagePieceID;	// 1
	unsigned char ColorID;			// 1
    unsigned char NumberBytesHeader;// 1
    unsigned char NumberBytesData;  // 1
	short DescriptionID;			// 2
    unsigned char Header[4];		// 4
    unsigned char Data[8];
	unsigned char AckBytes[8];			
    float Value;
	unsigned char MiscData;
    }  icsSpyMessageJ1850;

// Win32 DLL prototypes
//Basic Functions
typedef int  (__stdcall *FINDNEODEVICES)(unsigned long DeviceTypes, NeoDevice *pNeoDevice, int *pNumDevices);
typedef int  (__stdcall *OPENNEODEVICE)(NeoDevice *pNeoDevice, int *hObject, unsigned char *bNetworkIDs, int bConfigRead, int bSyncToPC);
typedef int  (__stdcall *CLOSEPORT)(int hObject, int *pNumberOfErrors); 
typedef void (__stdcall *FREEOBJECT)(int hObject);

//Message Functions
typedef int  (__stdcall *GETMESSAGES)(int hObject, icsSpyMessage *pMsg, int * pNumberOfMessages, int * pNumberOfErrors); 
typedef int  (__stdcall *TXMESSAGES)(int hObject, icsSpyMessage *pMsg, int lNetworkID, int lNumMessages); 
typedef int  (__stdcall *WAITFORRXMSGS)(int hObject, unsigned int iTimeOut);
typedef int  (__stdcall *ENABLERXQUEUE)(int hObject, int iEnable);
typedef int  (__stdcall *GETTSFORMSG)(int hObject, icsSpyMessage *pMsg, double *pTimeStamp);
typedef void (__stdcall *GETISO15765STATUS)(int hObject, int lNetwork, int lClearTxStatus, 
											int lClearRxStatus, int *lTxStatus, int *lRxStatus);

typedef void (__stdcall *SETISO15765RXPARMS)(int hObject, int lNetwork, int lEnable, 
			                                spyFilterLong *pFF_CFMsgFilter, icsSpyMessage *pTxMsg, 
											int lCFTimeOutMs, int lFlowCBlockSize,
											int lUsesExtendedAddressing, int lUseHardwareIfPresent);

//Device Functions
typedef int (__stdcall *GETCONFIG)(int hObject, unsigned char * pData, int * lNumBytes);
typedef int (__stdcall *SENDCONFIG)(int hObject, unsigned char * pData, int lNumBytes); 
typedef int (__stdcall *GETFIRESETTINGS)(int hObject, SFireSettings *pSettings, int iNumBytes);
typedef int (__stdcall *SETFIRESETTINGS)(int hObject, SFireSettings *pSettings, int iNumBytes, int bSaveToEEPROM);
typedef int (__stdcall *GETVCAN3SETTINGS)(int hObject, SVCAN3Settings *pSettings, int iNumBytes);
typedef int (__stdcall *SETVCAN3SETTINGS)(int hObject, SVCAN3Settings *pSettings, int iNumBytes, int bSaveToEEPROM);
typedef int (__stdcall *SETBITRATE)(int hObject, int BitRate, int NetworkID);
typedef int (__stdcall *GETDEVICEPARMS)(int hObject, char *pParameter, char *pValues, short ValuesLength);
typedef int (__stdcall *SETDEVICEPARMS)(int hObject, char *pParmValue, int *pErrorIndex, int bSaveToEEPROM);

//Error Functions
typedef int (__stdcall *GETLASTAPIERROR)(int hObject, unsigned long *pErrorNumber);
typedef int (__stdcall *GETERRMSGS)(int hObject, int * pErrorMsgs, int * pNumberOfErrors);
typedef int (__stdcall *GETERRORINFO)(int lErrorNumber,  char *szErrorDescriptionShort, 
										 char *szErrorDescriptionLong, int * lMaxLengthShort,
                                        int * lMaxLengthLong,int * lErrorSeverity,int * lRestartNeeded);

//General Utility Functions
typedef int (__stdcall *VALIDATEHOBJECT)(int hObject);
typedef int (__stdcall *GETDLLVERSION)(void);
typedef int (__stdcall *GETSERIALNUMBER)(int hObject, unsigned int *iSerialNumber);
typedef int (__stdcall *STARTSOCKSERVER)(int hObject, int iPort);
typedef int (__stdcall *STOPSOCKSERVER)(int hObject);

//CoreMini Script functions
typedef int  (__stdcall *SCRIPTSTART)(int hObject, int iLocation);  
typedef int  (__stdcall *SCRIPTSTOP)(int hObject);  
typedef int  (__stdcall *SCRIPTLOAD)(int hObject, const unsigned char * bin, unsigned long len_bytes, int iLocation);
typedef int  (__stdcall *SCRIPTCLEAR)(int hObject, int iLocation);
typedef int  (__stdcall *SCRIPTSTARTFBLOCK)(int hObject,unsigned int fb_index);
typedef int  (__stdcall *SCRIPTGETFBLOCKSTATUS)(int hObject, unsigned int fb_index, int *piRunStatus);
typedef int  (__stdcall *SCRIPTSTOPFBLOCK)(int hObject,unsigned int fb_index);
typedef int  (__stdcall *SCRIPTGETSCRIPTSTATUS)(int hObject, int *piStatus);
typedef int  (__stdcall *SCRIPTREADAPPSIGNAL)(int hObject, unsigned int iIndex, double *dValue);
typedef int  (__stdcall *SCRIPTWRITEAPPSIGNAL)(int hObject, unsigned int iIndex, double dValue);
typedef int  (__stdcall *SCRIPTREADRXMESSAGE)(int hObject, unsigned int iIndex,
											  icsSpyMessage *pRxMessageMask, icsSpyMessage *pRxMessageValue);
typedef int  (__stdcall *SCRIPTREADTXMESSAGE)(int hObject, unsigned int iIndex, icsSpyMessage *pTxMessage);
typedef int  (__stdcall *SCRIPTWRITERXMESSAGE)(int hObject, unsigned int iIndex,
											   const icsSpyMessage *pRxMessageMask, const icsSpyMessage *pRxMessageValue);
typedef int  (__stdcall *SCRIPTWRITETXMESSAGE)(int hObject, unsigned int iIndex, const icsSpyMessage *pTxMessage);
//typedef int  (__stdcall *SCRIPTREADISO15765TXMESSAGE)(int hObject, unsigned int iIndex, stCM_ISO157652_TxMessage *pTxMessage);
//typedef int  (__stdcall *SCRIPTWRITEISO15765TXMESSAGE)(int hObject, unsigned int iIndex, const stCM_ISO157652_TxMessage *pTxMessage);


//Deprecated (but still suppored in the DLL)
typedef int  (__stdcall *OPENPORTEX)(int lPortSerialNumber, int lPortType, int lDriverType, 
					                 int lIPAddressMSB, int lIPAddressLSBOrBaudRate,int bConfigRead, 
				                     unsigned char * bNetworkID, int * hObject);
typedef int  (__stdcall *OPENPORT)(int lPortNumber, int lPortType, int lDriverType, 
					               unsigned char *bNetworkID, unsigned char *bSCPIDs,  int * hObject);
typedef int (__stdcall *ENABLENETWORKCOM)(int hObject, int Enable);
typedef int (__stdcall *FINDCOMDEVICES)(int lDriverType,  int lGetSerialNumbers, int lStopAtFirst, int lUSBCommOnly,
							            int *p_lDeviceTypes, int *p_lComPorts, int *p_lSerialNumbers, int *lNumDevices); 
