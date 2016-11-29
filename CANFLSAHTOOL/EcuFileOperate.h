//CEcuFileOperate��

#ifndef SPECIALTYPE
 #define SPECIALTYPE
 #include "SpecialType.h"
#endif

#pragma once

class CEcuFileOperate           
{
public:
	unsigned char  EcuType;                                            //������̳�ʱ�ɹ��캯����ʼ��( 0:MT80   1:MT22U   2:MT22.1   3:MT22.1.1 )
	unsigned char  FileRowBuf[600];                                    //��¼�ļ���һ������ ()

	//S-Record Format
	unsigned char  S_RecordType;                                       //��¼S-Record������( 0:S0  1:S1  2:S2  3:S3  4:S4  5:S5  6:S6  7:S7  8:S8  9:S9 )
	unsigned int   S_RowRecordLength;                                  //��¼һ�к�������ݳ��� (���� address data checksum)
	unsigned int   S_Address;                                          //��¼��һ�����ݵ���ʼMemory�ĵ�ַ
	unsigned char  S_Data[300];                                        //��¼һ���еĽ�Ҫ������д��memory��Data
	unsigned char  S_CheckSum;                                         //��¼һ�е�checksum
	unsigned char  S_RealDataLength;
	
	//S_KKSUM S_KKPGMID S_KKSECURITY   ��ʱ��ʹ��
	unsigned short S_KKSUM;                                            //KKSUM ��FlashTool��Special Action Items�е�һ�� ��һ��ͨ��Ҫ�ɳ����Լ�����������㷽���ڷ���  CalculateKKSUM();
	unsigned char  S_KKPGMID;                                          //KKPGMID��FlashTool��Special Action Items�е�һ�� ��ʱδʹ��
	unsigned char  S_KKSECURITY;                                       //KKSECURITY��FlashTool��Special Action Items�е�һ�� ��ʱδʹ��
	CString S7line;
	CString S0line;
	char  S0_Data[300];  
	char  S7_Data[300];  
		bool  S0_Flag;
	bool  S7_Flag;
	//�ļ���������
	CStdioFile               csFile;                                   //�ļ��������� 

	//����ECU�ļ�������Utility�ļ���Calibration�ļ�
	//TransDataType      Utility;
	//TransDataType      Calibration;
public:
	CEcuFileOperate();
	BOOL OpenFile(CString &filePath);
	bool ReadRowDataToSRecord();                                                                 //���ض������ֽ���  �� FileRowBuf �������
	void CloseFile();
	void CharToHex(unsigned char * buf);
	void CalculateCRC8(TransDataType &transStream);
	void EncryptData(TransDataType &transStream,unsigned char FileType);                         //��Ҫ����ĵ����ݽ��м򵥵ļ��� ���ܸ����ļ������� Utility (0) ��Calibration (1) �ļ�����������
	bool CEcuFileOperate::ReadRowDataToSRecordFromCString(CString strline);

};
