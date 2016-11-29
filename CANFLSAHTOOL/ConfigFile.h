#pragma once
 #include "SpecialType.h"

#define MIXED 0
#define SEPARATED 1
#define CHECKSUM_NUM 3

typedef struct
{
	U8	Flash_Buildcut_Flag;			   //ˢдBB nr
	U8	Utility_BB_Flag;			   //ˢдBB nr
	U32	Delay_Time_seconds;			   //��ʱ�ĳ���
	U32	Dongle_Time_seconds;
	U32 DataSection_Size;              //CANһ�����ݰ���С
	BOOL b_cal_app;
	BOOL b_boot;
	BOOL b_check;
	U32 Start_cal;
	U32 Start_app;
	U32 Bankaddr[40];                  //���ˢд�����
	U32 Appaddr[10];                    //�����ַ
	U32 Caladdr[10];                    //�궨��ַ �ļ�����ģ���ʼ�ͽ�����ַ����ַ��Χ
	U32 SpecCaladdr[10]; 
	U32  Bootblock[2];                 //boot�����ַ
	U32  Mfgblock[2];                 //mfg�����ַ
	U32  Tswblock[2];                 //tsw�����ַ
	U8  Fill_Byte;                     //�����ֽ�
 	char ChecksumName[CHECKSUM_NUM][10];
	U32 ChecksumAddr[CHECKSUM_NUM];
	U32 ChecksumSize[CHECKSUM_NUM];
	U8 ChecksumOrder[CHECKSUM_NUM];
	U32 Wordsumaddr[CHECKSUM_NUM][20]; 
	U32 KkpgmIdAddr;
	U32 KkpgmIdSize;
	U32 KkpmIdValue[2]; 
	U32 SecAdd;
	int FileType;
	BOOL g_bDongleFlag;
	bool funcFlag;
	char funcType[6];//FILETYPE_NUM
	U8 AppAddrNum;
	U8 CalAddrNum;
	U8 SpecCalAddrNum;
	U32 AppRemapaddr[10];  
	U32 CalRemapaddr[10]; 
	U8 AppRemaNum;
	U8 CalRemaNum;
	U8 AppaddrFlag[10];
	U8 CaladdrFlag[10];
	U8 SepCaladdrFlag[10];
	U8 AppCurNum;
	U8 CalCurNum;
	U8 SepCalCurNum;
	U8 BootCurNum;
	U8 MfgCurNum;
	U8 TswCalCurNum;
}CAN_ECUPrg;
typedef struct
{
	CString ChecksumName;
	U16 Checksum;
	U32 ChecksumAddr;
	U16 FileChecksum;
}Check_Sum;

class ConfigFile
{
public:
	ConfigFile();
	int ConfigFile::ReadPRG(CString m_Path);
	int Read_Prg(CString m_Path);
 	void ConfigFile::CharToHex(unsigned char *buf);
 	unsigned int ConfigFile::GetValue(CString str,int type);
	int ConfigFile::GetAddr(CString str,U32 *addr);
	int ConfigFile::HextoAdd(char *buf,U32 *addr);
 public:
	CString	ECU_Type;              //ECU���� 
	CString	path;  
	U8  WordsumNum[CHECKSUM_NUM];	
	int banknum;
	int ChecksumNum;
	CString ChecksumName[CHECKSUM_NUM];
	CString ChecksumOrder[CHECKSUM_NUM];
	char flag;
	Check_Sum m_checksum[CHECKSUM_NUM];

	CStdioFile  csFile;  

	CAN_ECUPrg  ECUprg;
	U32 AddressStart;
	U32 AddressEnd; 
	U32 Calsize;
	U32 Appsize;
	U32 Cutsize;
	U32 Checksum;
	U16 FileChecksum;
	U32 Kkpgmindvalue;
	U32 SecValue;
	U8 RowBuffer[500];
};

