
#include "StdAfx.h"
#include "EcuFileOperate.h"

CEcuFileOperate::CEcuFileOperate()
{
	EcuType = 0;                                            
	memset(FileRowBuf,0,300);                               

	//S-Record Format
	S_RecordType = 0;                                       
	S_RowRecordLength = 0;                                  
	S_Address = 0;                                          
	memset(S_Data,0,300);                                   
	S_CheckSum = 0; 
	S_RealDataLength = 0;
	
	//S_KKSUM S_KKPGMID S_KKSECURITY   ��ʱ��ʹ��
	S_KKSUM = 0;                                            
	S_KKPGMID = 0;                                          
	S_KKSECURITY = 0;
}
                                                                       
BOOL CEcuFileOperate::OpenFile(CString &filePath)
{
	return csFile.Open(filePath,CFile::modeRead);                 
}
bool CEcuFileOperate::ReadRowDataToSRecordFromCString(CString strline)
{
	int tempOffset = 1;
	CString strReadW;
	
	strReadW = strline;
	CStringA strReadA(strReadW);
	strcpy_s((char *)FileRowBuf,600,strReadA);
	if(FileRowBuf[0]==0)return false;
	//csFile.ReadString((LPTSTR)FileRowBuf,600);
	CharToHex(FileRowBuf);
	S_RecordType = FileRowBuf[tempOffset];
	 
	if(S_RecordType == 0 )
	{
		strReadW+=_T("\r\n");
		CStringA strReadA(strReadW);
		strcpy_s(S0_Data,300,strReadA);
		S0_Flag=1;
	}
	else if( S_RecordType > 0 && S_RecordType < 4 )//����s1��s3������ʶ��ת��
	{
		tempOffset++;
		S_RowRecordLength = FileRowBuf[tempOffset]*16 + FileRowBuf[tempOffset+1];
		tempOffset+=2;
		S_Address = 0;                                              
		for(int i=0;i<(S_RecordType+1)*2;i++)//�������ͺ����ַ���ȵĹ�ϵ
		{
			S_Address <<= 4;
			S_Address += FileRowBuf[tempOffset];
			tempOffset++;
		}

		S_RealDataLength = S_RowRecordLength - S_RecordType - 2 ;
		for(unsigned short i = 0;i < S_RealDataLength;i++)
		{
			S_Data[i] = FileRowBuf[tempOffset];
			tempOffset++;
			S_Data[i] <<= 4;
			S_Data[i] += FileRowBuf[tempOffset];
			tempOffset++;
			//Calibration.totalTransData.FileData[Calibration.FileDataSize + i] = S_Data[i]; 
		}
		//Calibration.FileDataSize += S_RealDataLength;
		S_CheckSum = FileRowBuf[tempOffset++];
		S_CheckSum <<= 4;
		S_CheckSum += FileRowBuf[tempOffset++];


#ifdef   _DEBUG 
		if ( tempOffset != S_RowRecordLength*2 + 4)
		{
			AfxMessageBox(_T("tempOffset is error"));
			return false;
		}
#endif

		unsigned char testCheckSum = 0;
		unsigned char tempAddress[4];
		memcpy(tempAddress,&S_Address,4);
		for(int i = 0 ; i < S_RealDataLength;i++)
		{
			testCheckSum += S_Data[i];
		}
		
		for(int j = 0; j < S_RecordType + 1 ; j++)
		{
			testCheckSum += tempAddress[j];
		}
		
		testCheckSum += S_RowRecordLength;

		unsigned char tempCheckSum = ~testCheckSum;
		if( tempCheckSum != S_CheckSum )
		{
		//	AfxMessageBox(_T("CheckSum is error") );
			return false;
		}
	}
	else if( S_RecordType >= 7&& S_RecordType <= 9 )
	{
		strReadW+=_T("\r\n");
		CStringA strReadA(strReadW);
		strcpy_s(S7_Data,300,strReadA);
		S7_Flag=1;
	}
                                                                                 //ֻ��S1 S2 S3 ���д���
	return true;
	 
}

bool CEcuFileOperate::ReadRowDataToSRecord()
{
	int tempOffset = 1;
	CString strReadW;
	if(csFile.ReadString(strReadW) == NULL)return false;
	CStringA strReadA(strReadW);
	strcpy_s((char *)FileRowBuf,600,strReadA);
	if(FileRowBuf[0]==0)return false;
	//csFile.ReadString((LPTSTR)FileRowBuf,600);
	CharToHex(FileRowBuf);
	S_RecordType = FileRowBuf[tempOffset];
	 
	if(S_RecordType == 0 )
	{
		strReadW+=_T("\r\n");
		CStringA strReadA(strReadW);
		strcpy_s(S0_Data,300,strReadA);
		S0_Flag=1;
	}
	else if( S_RecordType > 0 && S_RecordType < 4 )//����s1��s3������ʶ��ת��
	{
		tempOffset++;
		S_RowRecordLength = FileRowBuf[tempOffset]*16 + FileRowBuf[tempOffset+1];
		tempOffset+=2;
		S_Address = 0;                                              
		for(int i=0;i<(S_RecordType+1)*2;i++)//�������ͺ����ַ���ȵĹ�ϵ
		{
			S_Address <<= 4;
			S_Address += FileRowBuf[tempOffset];
			tempOffset++;
		}

		S_RealDataLength = S_RowRecordLength - S_RecordType - 2 ;
		for(unsigned short i = 0;i < S_RealDataLength;i++)
		{
			S_Data[i] = FileRowBuf[tempOffset];
			tempOffset++;
			S_Data[i] <<= 4;
			S_Data[i] += FileRowBuf[tempOffset];
			tempOffset++;
			//Calibration.totalTransData.FileData[Calibration.FileDataSize + i] = S_Data[i]; 
		}
		//Calibration.FileDataSize += S_RealDataLength;
		S_CheckSum = FileRowBuf[tempOffset++];
		S_CheckSum <<= 4;
		S_CheckSum += FileRowBuf[tempOffset++];


#ifdef   _DEBUG 
		if ( tempOffset != S_RowRecordLength*2 + 4)
		{
			AfxMessageBox(_T("tempOffset is error"));
			return false;
		}
#endif

		unsigned char testCheckSum = 0;
		unsigned char tempAddress[4];
		memcpy(tempAddress,&S_Address,4);
		for(int i = 0 ; i < S_RealDataLength;i++)
		{
			testCheckSum += S_Data[i];
		}
		
		for(int j = 0; j < S_RecordType + 1 ; j++)
		{
			testCheckSum += tempAddress[j];
		}
		
		testCheckSum += S_RowRecordLength;

		unsigned char tempCheckSum = ~testCheckSum;
		if( tempCheckSum != S_CheckSum )
		{
		//	AfxMessageBox(_T("CheckSum is error") );
			return false;
		}
	}
	else if( S_RecordType >= 7&& S_RecordType <= 9 )
	{
		strReadW+=_T("\r\n");
		CStringA strReadA(strReadW);
		strcpy_s(S7_Data,300,strReadA);
		S7_Flag=1;
	}
                                                                                 //ֻ��S1 S2 S3 ���д���
	return true;
	 
}
void CEcuFileOperate::CharToHex(unsigned char *buf)
{
	unsigned int bufSize = strlen((const char *)buf);
	for(unsigned short i = 0;i < bufSize ;i++)
	{
		if( buf[i]>='0' && buf[i]<='9')
			buf[i] = buf[i] - '0';
		else if( buf[i]>='a' && buf[i]<='f' )
			buf[i] = buf[i] - 'a'+10;
		else if( buf[i]>='A' && buf[i]<='F' )
			buf[i] = buf[i] - 'A'+10;
	}
}

void CEcuFileOperate::CloseFile()
{
	csFile.Close();
}
/////////////////////////////////////////////////////////////////CRC8У�鷽��????????
void CEcuFileOperate::CalculateCRC8(TransDataType &transStream)
{
	unsigned char tempTransData = 0;
	unsigned char crc = 0;
	for(unsigned int i=0; i<transStream.FileDataSize ;i++)
	{
		tempTransData = transStream.totalTransData.FileData[i];
		for (int j = 0; j < 8; ++j)
		{
			int n = (tempTransData^crc) & 1;		                                                           //�ѵ�һ��8λ������crc�Ĵ�������򣬲�������λ�Ƿ�Ϊ1��
			crc >>= 1;				                                                                           //�ѼĴ���������������һλ��
			tempTransData >>= 1;				                                                               //��������ͬʱ����һλ��
			if (n) crc ^= 0x8c;					                                                               //������λ�Ƿ�Ϊ1�����Ϊ1��crc�Ĵ��������ʽ��������
		}
	}
	transStream.totalTransData.FileData[transStream.FileDataSize] = crc;
	transStream.FileDataSize++;
}
//////////////////////////////////////////////////////////////////////////////

//���ݵļ��ܹ��̣������ݼ��ܳ�UTILITY��CALIBRATION��������

void CEcuFileOperate::EncryptData(TransDataType &transStream,unsigned char FileType)
{
	unsigned int EncryptKeyUtilityOrRoutine = 3;
	unsigned int EncryptKeyCalibration = 4;
	if( FileType == UTILITY || FileType == ROUTINE )                                                                                         //Utility OR MT80 Routine
	{
		for(int i = 0 ; i < transStream.FileDataSize; i++)
		{
			transStream.totalTransData.FileData[i]^=EncryptKeyUtilityOrRoutine++;
		}
	}
	else if ( FileType == CALIBRATION )                                                                                  //Calibration
	{
		for(int i = 0 ; i < transStream.FileDataSize ; i++)
		{ 
			transStream.totalTransData.FileData[i]^=EncryptKeyCalibration++;
		}
	}
}