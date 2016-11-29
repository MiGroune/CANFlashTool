//CEcuFileOperate类

#ifndef SPECIALTYPE
 #define SPECIALTYPE
 #include "SpecialType.h"
#endif

#pragma once

class CEcuFileOperate           
{
public:
	unsigned char  EcuType;                                            //给子类继承时由构造函数初始化( 0:MT80   1:MT22U   2:MT22.1   3:MT22.1.1 )
	unsigned char  FileRowBuf[600];                                    //记录文件的一行数据 ()

	//S-Record Format
	unsigned char  S_RecordType;                                       //记录S-Record的类型( 0:S0  1:S1  2:S2  3:S3  4:S4  5:S5  6:S6  7:S7  8:S8  9:S9 )
	unsigned int   S_RowRecordLength;                                  //记录一行后面的数据长度 (包括 address data checksum)
	unsigned int   S_Address;                                          //记录这一行数据的起始Memory的地址
	unsigned char  S_Data[300];                                        //记录一行中的将要真正烧写到memory的Data
	unsigned char  S_CheckSum;                                         //记录一行的checksum
	unsigned char  S_RealDataLength;
	
	//S_KKSUM S_KKPGMID S_KKSECURITY   暂时不使用
	unsigned short S_KKSUM;                                            //KKSUM 是FlashTool中Special Action Items中的一项 这一项通常要由程序自己计算出，计算方法在方法  CalculateKKSUM();
	unsigned char  S_KKPGMID;                                          //KKPGMID是FlashTool中Special Action Items中的一项 暂时未使用
	unsigned char  S_KKSECURITY;                                       //KKSECURITY是FlashTool中Special Action Items中的一项 暂时未使用
	CString S7line;
	CString S0line;
	char  S0_Data[300];  
	char  S7_Data[300];  
		bool  S0_Flag;
	bool  S7_Flag;
	//文件操作对象
	CStdioFile               csFile;                                   //文件操作对象 

	//所有ECU文件都具有Utility文件和Calibration文件
	//TransDataType      Utility;
	//TransDataType      Calibration;
public:
	CEcuFileOperate();
	BOOL OpenFile(CString &filePath);
	bool ReadRowDataToSRecord();                                                                 //返回读出的字节数  对 FileRowBuf 进行填充
	void CloseFile();
	void CharToHex(unsigned char * buf);
	void CalculateCRC8(TransDataType &transStream);
	void EncryptData(TransDataType &transStream,unsigned char FileType);                         //对要传输的的数据进行简单的加密 加密根据文件类型来 Utility (0) 和Calibration (1) 的加密稍有区别
	bool CEcuFileOperate::ReadRowDataToSRecordFromCString(CString strline);

};
