#include "StdAfx.h"
#include "OperateFile.h"
 #include "Global.h"
char Bank_flag[20];
char Bank_flag2[20];
char buf[100];

COperateFile::COperateFile(void)
{
}


COperateFile::~COperateFile(void)
{
}
int COperateFile::GetTransData(CString &FilePath ,unsigned char Type)
{
	int ss=0;
 
	U32 sizeAdd=0;
	int value1=0;
	int value2=0;
		int value3=0;
	int value4=0;
	int readnum=0;
		 S0_Flag=S7_Flag=0;
	unsigned int Slinelength;
	unsigned int  KKSUM_Index;
	CString str;
	int result;
	tempCheckSum=0;
	result = OpenFile(FilePath);
	if(result == 0)return -1;

	 
	KKSUM_Index = config->ECUprg.ChecksumAddr[0];//KKSUM 地址

 	if(config->ECUprg.b_cal_app == MIXED)//Calibration
	{
  		memset(&Calibration,config->ECUprg.Fill_Byte,sizeof(TransDataType));//
		memset(Bank_flag,0,sizeof(Bank_flag));
		memset(Bank_flag2,0,sizeof(Bank_flag2));
		Calibration.FileDataSize=0;
		while(1)
		{
			if( !ReadRowDataToSRecord() )
			{
				break;
			}
			if( S_RecordType >0 && S_RecordType < 4 )
			{
				for(int j=0;j<config->banknum;j+=2)
				{
					if( S_Address>=config->ECUprg.Bankaddr[j]  && S_Address<=config->ECUprg.Bankaddr[j+1])
					{			
						value1=j;
						if(value1 != value2)
						{
							sizeAdd=0;
							for(int n=0;n<config->banknum;n++)
							{
								if(Bank_flag[n] == 1)sizeAdd+=config->ECUprg.Bankaddr[n+1]-config->ECUprg.Bankaddr[n]+1;
							}
							value2=value1;
						}
						for(int i=0;i<S_RealDataLength;i++)
						{
							/*****/
							if(config->ChecksumNum >0)//如果有checksum
							{
								for(int m=0;m<config->WordsumNum[0];m+=2)
								{
									if(S_Address >= config->ECUprg.Wordsumaddr[0][m]&& S_Address <= config->ECUprg.Wordsumaddr[0][m+1])						
									{
										if( i%2 == 0 )
											tempCheckSum +=(((unsigned int)S_Data[i])<<8);
										else
											tempCheckSum += S_Data[i];
										break;	
									}
								}
							}
							/*****/
							if(S_Address == config->ECUprg.ChecksumAddr[0])KKSUM_Index=(unsigned int)Calibration.FileDataSize;
									
							Calibration.totalTransData.FileData[(S_Address++) - config->ECUprg.Bankaddr[j]+sizeAdd]  = S_Data[i];        //当文件内的地址不连续 但是 不缺少时 使用S_Address - MT221AddressStart + i 当连续的地址时使用 Calibration.FileDataSize + i						 
							
							Calibration.FileDataSize++;																		 							 
						}						 
						Bank_flag[j]=1;
					}
				}
			}			
		}
		/*****/
		if(config->ChecksumNum >0)
		{
 			S_KKSUM = tempCheckSum;
			config->FileChecksum = (Calibration.totalTransData.FileData[KKSUM_Index]<<8) | Calibration.totalTransData.FileData[KKSUM_Index+1];
 			config->Checksum=S_KKSUM;
		}
  		if( S0_Flag==1)strcpy_s(Calibration.S0_Data,300,S0_Data);
		else strcpy_s(Calibration.S0_Data,300,"S00600004844521B\r\n");
 
		if( S7_Flag==1){strcpy_s(Calibration.S7_Data,300,S7_Data);Calibration.S7_Flag=1;}
		else Calibration.S7_Flag=0;
 
   	}
	else//seperated
	{
		memset(Bank_flag,0,sizeof(Bank_flag));
		if( Type == CUTFILE)
		{
			memset(&Calibration,config->ECUprg.Fill_Byte,sizeof(TransDataType));//
		 Calibration.FileDataSize=0;
			while(1)
			{
				if( !ReadRowDataToSRecord() )
				{
					break;
				}
				Slinelength=S_RealDataLength;
				//config->AddressStart 0xa0020000
				if( S_RecordType >0 && S_RecordType < 4 )
				{
					if( S_Address < config->ECUprg.Caladdr[0]  && S_Address+S_RealDataLength>= config->ECUprg.Caladdr[0])
					{
						int j;
						for(j=0;j<S_RealDataLength;j++)
						{
							if(S_Address== config->ECUprg.Caladdr[0])break;
							S_Address++;
						}
							Slinelength=S_RealDataLength-j;				
					}
					if( S_Address < config->ECUprg.Appaddr[0]  && S_Address+S_RealDataLength>= config->ECUprg.Appaddr[0])
					{
						int j;
						for(j=0;j<S_RealDataLength;j++)
						{
							if(S_Address== config->ECUprg.Appaddr[0])break;
							S_Address++;
						}
							Slinelength=S_RealDataLength-j;				
					}
					if( S_Address>=config->ECUprg.Caladdr[0]  && S_Address<=config->ECUprg.Caladdr[1])
					{

						for(int j=0;j<config->banknum;j+=2)
						{
							if( S_Address>=config->ECUprg.Bankaddr[j]  && S_Address<=config->ECUprg.Bankaddr[j+1])
							{			
								value1=j;
								if(value1 != value2)
								{
									sizeAdd=0;
									for(int n=0;n<config->banknum;n++)
									{
										if(Bank_flag[n] == 1)sizeAdd+=config->ECUprg.Bankaddr[n+1]-config->ECUprg.Bankaddr[n]+1;
									}
									value2=value1;
								}
								for(int i=0;i<Slinelength;i++)
								{
									Calibration.totalTransData.FileData[(S_Address++) - config->ECUprg.Bankaddr[j]+sizeAdd]  = S_Data[i];        //当文件内的地址不连续 但是 不缺少时 使用S_Address - MT221AddressStart + i 当连续的地址时使用 Calibration.FileDataSize + i						 
									Calibration.FileDataSize++;																		 							 
								}						 
								Bank_flag[j]=1;
							}
						}
					}									 
					else if( S_Address>=config->ECUprg.Appaddr[0] && S_Address<=config->ECUprg.Appaddr[1] )
					{	 
						//0x80040000
						//for(int i=0;i<S_RealDataLength;i++)
						//{
						//	Routine.totalTransData.FileData[(S_Address++) - config->ECUprg.Appaddr[0]] = S_Data[i];      //当文件内的地址不连续 但是 不缺少时 使用S_Address - MT221AddressStart + i 当连续的地址时使用 Calibration.FileDataSize + i
						//}
						for(int j=0;j<config->banknum;j+=2)
						{
							if( S_Address>=config->ECUprg.Bankaddr[j]  && S_Address<=config->ECUprg.Bankaddr[j+1])
							{			
								value3=j;
								if(value3 != value4)
								{
									sizeAdd=0;
									for(int n=0;n<config->banknum;n++)
									{
										if(Bank_flag2[n] == 1)sizeAdd+=config->ECUprg.Bankaddr[n+1]-config->ECUprg.Bankaddr[n]+1;
									}
									value4=value3;
								}
								for(int i=0;i<S_RealDataLength;i++)
								{
 									Routine.totalTransData.FileData[(S_Address++) - config->ECUprg.Bankaddr[j]+sizeAdd]  = S_Data[i];        //当文件内的地址不连续 但是 不缺少时 使用S_Address - MT221AddressStart + i 当连续的地址时使用 Calibration.FileDataSize + i						 							
 								}						 
								Bank_flag2[j]=1;
							}
						}
					}
				}			
			}
			Calibration.FileDataSize = config->Calsize;
			Routine.FileDataSize = config->Appsize;//large mt80 S19文件长度小于c0000,缺失的部分填充0.不能按文件的实际长度计算。
		}
 		else if(Type == CALFILE)//cal Calibration
		{
			ReadDataFromFileTobuf(config->ECUprg.Caladdr,0);
		 
			Calibration.FileDataSize = config->Calsize;
		} 
		else if(Type == PTPFILE)//app  Routine
		{
			ReadDataFromFileTobuf(config->ECUprg.Appaddr,1);
			strcpy_s(Routine.S0_Data,300,S0_Data);
			strcpy_s(Routine.S7_Data,300,S7_Data);
			Routine.FileDataSize = config->Appsize;//large mt80 S19文件长度小于c0000,缺失的部分填充0.不能按文件的实际长度计算。		
		}
		else if(Type == BOOTFILE)
		{
			ReadDataFromFileTobuf_BOOT(config->ECUprg.Bootblock,0);
		 
			Calibration.FileDataSize = config->ECUprg.Bootblock[1]-config->ECUprg.Bootblock[0]+1;
		}
		else if(Type == MFGFILE)
		{
			ReadDataFromFileTobuf_BOOT(config->ECUprg.Mfgblock,0);
		 
			Calibration.FileDataSize = config->ECUprg.Mfgblock[1]-config->ECUprg.Mfgblock[0]+1;
		}
		else if(Type == TSWFILE)
		{
			ReadDataFromFileTobuf_BOOT(config->ECUprg.Tswblock,0);
		 
			Calibration.FileDataSize = config->ECUprg.Tswblock[1]-config->ECUprg.Tswblock[0]+1;
		}
	
	}
	CloseFile();
	return 1;
}
int COperateFile::GetModuleTransDataFromCStringArray(CString &FilePath ,int type,int ctrl)
{
	u32 add[2];
	int buftype;

	if(type == PTPFILE)
	{
		add[0]=config->ECUprg.Appaddr[ctrl];
		add[1]=config->ECUprg.Appaddr[ctrl+1];
		buftype=1;
	}
	else if(type == CALFILE)
	{
		add[0]=config->ECUprg.Caladdr[ctrl];
		add[1]=config->ECUprg.Caladdr[ctrl+1];
		buftype=0;
	}
	else if(type == SEPCALFILE)
	{
		add[0]=config->ECUprg.SpecCaladdr[ctrl];
		add[1]=config->ECUprg.SpecCaladdr[ctrl+1];
		buftype=0;
	}
	 ReadDataFromCStringArrayTobuf(add,buftype);

	 return 1;
}
int COperateFile::GetModuleTransData(CString &FilePath ,int type,int ctrl)
{
	u32 add[2];
	int buftype;
	int	result = OpenFile(FilePath);
	if(result == 0)return -1;

	if(type == PTPFILE)
	{
		add[0]=config->ECUprg.Appaddr[ctrl];
		add[1]=config->ECUprg.Appaddr[ctrl+1];
		buftype=1;
	}
	else if(type == CALFILE)
	{
		add[0]=config->ECUprg.Caladdr[ctrl];
		add[1]=config->ECUprg.Caladdr[ctrl+1];
		buftype=0;
	}
	else if(type == SEPCALFILE)
	{
		add[0]=config->ECUprg.SpecCaladdr[ctrl];
		add[1]=config->ECUprg.SpecCaladdr[ctrl+1];
		buftype=0;
	}
	 ReadDataFromFileTobuf(add,buftype);
	 CloseFile();

	 return 1;
}
int COperateFile::GetFileBankNum(CString &FilePath ,int type)
{
	u32 add[2];
 	int	result = OpenFile(FilePath);
	if(result == 0)return -1;
	
 	if(type == PTPFILE || type == CUTFILE)
	{
		config->ECUprg.AppCurNum=0;
		memset(config->ECUprg.AppaddrFlag,0,10);
		for(int i=0;i<config->ECUprg.AppAddrNum;i+=2)
		{
			add[0]=config->ECUprg.Appaddr[i];
			add[1]=config->ECUprg.Appaddr[i+1];
			result = CheckCurrentBank(add);
			if(result == 1)
			{
				config->ECUprg.AppaddrFlag[i]=1;
				config->ECUprg.AppCurNum++;
			}
		}
 	}
	if(type == CALFILE || type == CUTFILE )
	{
		config->ECUprg.CalCurNum=0;
		memset(config->ECUprg.CaladdrFlag,0,10);
		for(int i=0;i<config->ECUprg.CalAddrNum;i+=2)
		{
			add[0]=config->ECUprg.Caladdr[i];
			add[1]=config->ECUprg.Caladdr[i+1];
			result = CheckCurrentBank(add);
			if(result == 1)
			{
				config->ECUprg.CaladdrFlag[i]=1;
				config->ECUprg.CalCurNum++;
			}
		}
		config->ECUprg.SepCalCurNum=0;
		memset(config->ECUprg.SepCaladdrFlag,0,10);
		for(int i=0;i<config->ECUprg.SpecCalAddrNum;i+=2)
		{
			add[0]=config->ECUprg.SpecCaladdr[i];
			add[1]=config->ECUprg.SpecCaladdr[i+1];
			result = CheckCurrentBank(add);
			if(result == 1)
			{
				config->ECUprg.SepCaladdrFlag[i]=1;
				config->ECUprg.SepCalCurNum++;
			}
		}
	}
	if(type == BOOTFILE)
	{
		config->ECUprg.BootCurNum=0;
 		add[0]=config->ECUprg.Bootblock[0];
		add[1]=config->ECUprg.Bootblock[1];
		result = CheckCurrentBank(add);
		if(result == 1)
		{
 			config->ECUprg.BootCurNum++;
		}
  	}
	if(type == MFGFILE)
	{
		config->ECUprg.MfgCurNum=0;
 		add[0]=config->ECUprg.Mfgblock[0];
		add[1]=config->ECUprg.Mfgblock[1];
		result = CheckCurrentBank(add);
		if(result == 1)
		{
 			config->ECUprg.MfgCurNum++;
		}
 	}
	if(type == TSWFILE)
	{
		config->ECUprg.TswCalCurNum=0;
		add[0]=config->ECUprg.Tswblock[0];
		add[1]=config->ECUprg.Tswblock[1];
		result = CheckCurrentBank(add);
		if(result == 1)
		{
 			config->ECUprg.TswCalCurNum++;
		}
 	}
 	 CloseFile();
	 return 1;
}
int COperateFile::GetFileBankNum_ptpcal(CString &FilePath, CString &FilePath2,int type)
{
	u32 add[2];
 	int	result = OpenFile(FilePath);
	if(result == 0)return -1;
	
 	if(type == PTPFILE || type == CUTFILE)
	{
		config->ECUprg.AppCurNum=0;
		memset(config->ECUprg.AppaddrFlag,0,10);
		for(int i=0;i<config->ECUprg.AppAddrNum;i+=2)
		{
			add[0]=config->ECUprg.Appaddr[i];
			add[1]=config->ECUprg.Appaddr[i+1];
			result = CheckCurrentBank(add);
			if(result == 1)
			{
				config->ECUprg.AppaddrFlag[i]=1;
				config->ECUprg.AppCurNum++;
			}
			else
			{
				CloseFile();
				int	result = OpenFile(FilePath2);
				if(result == 0)return -1;
				result = CheckCurrentBank(add);
				if(result == 1)
				{
					config->ECUprg.AppaddrFlag[i]=1;
					config->ECUprg.AppCurNum++;
				}
			}
		}
 	}
	if(type == CALFILE || type == CUTFILE )
	{
		config->ECUprg.CalCurNum=0;
		memset(config->ECUprg.CaladdrFlag,0,10);
		for(int i=0;i<config->ECUprg.CalAddrNum;i+=2)
		{
			add[0]=config->ECUprg.Caladdr[i];
			add[1]=config->ECUprg.Caladdr[i+1];
			result = CheckCurrentBank(add);
			if(result == 1)
			{
				config->ECUprg.CaladdrFlag[i]=1;
				config->ECUprg.CalCurNum++;
			}
			else
			{
				CloseFile();
				int	result = OpenFile(FilePath2);
				if(result == 0)return -1;
				result = CheckCurrentBank(add);
				if(result == 1)
				{
					config->ECUprg.CaladdrFlag[i]=1;
					config->ECUprg.CalCurNum++;
				}
			}
		}
		config->ECUprg.SepCalCurNum=0;
		memset(config->ECUprg.SepCaladdrFlag,0,10);
		for(int i=0;i<config->ECUprg.SpecCalAddrNum;i+=2)
		{
			add[0]=config->ECUprg.SpecCaladdr[i];
			add[1]=config->ECUprg.SpecCaladdr[i+1];
			result = CheckCurrentBank(add);
			if(result == 1)
			{
				config->ECUprg.SepCaladdrFlag[i]=1;
				config->ECUprg.SepCalCurNum++;
			}
			else
			{
				CloseFile();
				int	result = OpenFile(FilePath2);
				if(result == 0)return -1;
				result = CheckCurrentBank(add);
				if(result == 1)
				{
					config->ECUprg.SepCaladdrFlag[i]=1;
					config->ECUprg.SepCalCurNum++;
				}
			}
		}
	}
	 
 	 CloseFile();
	 return 1;
}

int COperateFile::ReadDataFromFileTobuf_buildcut(CString &FilePath,u32 add[2],int ctrl)
{
		int ss=0;
	 S0_Flag=S7_Flag=0;
	U32 sizeAdd=0;
	int value1=0;
	int value2=0;
	int readnum=0;
	unsigned int Slinelength;
 	int	result = OpenFile(FilePath);
	if(result == 0)return -1;
	if(ctrl == 0)
	{
		memset(&Calibration,config->ECUprg.Fill_Byte,sizeof(TransDataType));//
		//memset(&Routine,config->ECUprg.Fill_Byte,sizeof(TransDataType));

	}
	memset(Bank_flag,0,sizeof(Bank_flag));
	Calibration.FileDataSize=0;
	//Routine.FileDataSize=0;	 
	while(1)
	{
		if( !ReadRowDataToSRecord() )
		{
			break;
		}
		Slinelength =S_RealDataLength;
		//config->AddressStart 0xa0020000
		if( S_RecordType >0 && S_RecordType < 4 )
		{
			if( S_Address < add[0]  && S_Address+S_RealDataLength>= add[0])
			{
				int j;
				for(j=0;j<S_RealDataLength;j++)
				{
					if(S_Address== add[0])break;
					S_Address++;
				}
					Slinelength=S_RealDataLength-j;				
			}
			 

			if( S_Address >= add[0]  && S_Address<= add[1])
			{
				//for(int j=0;j<config->banknum;j+=2)
				{
					//if( S_Address>=config->ECUprg.Bankaddr[j]  && S_Address<=config->ECUprg.Bankaddr[j+1])
					{			
						//value1=j;
						//if(value1 != value2)
						//{
						//	sizeAdd=0;
						//	for(int n=0;n<config->banknum;n++)
						//	{
						//		if(Bank_flag[n] == 1)sizeAdd+=config->ECUprg.Bankaddr[n+1]-config->ECUprg.Bankaddr[n]+1;
						//	}
						//	value2=value1;
						//}
						for(int i=0;i<Slinelength;i++)
						{
 							//if(ctrl == 0) 
							{
								Calibration.totalTransData.FileData[(S_Address++) - add[0] +sizeAdd]  = S_Data[i];        //当文件内的地址不连续 但是 不缺少时 使用S_Address - MT221AddressStart + i 当连续的地址时使用 Calibration.FileDataSize + i						 
							
								Calibration.FileDataSize++;																		 							 

							}
							//else
							//{
							//	Routine.totalTransData.FileData[(S_Address++) - add[0] +sizeAdd]  = S_Data[i];        //当文件内的地址不连续 但是 不缺少时 使用S_Address - MT221AddressStart + i 当连续的地址时使用 Calibration.FileDataSize + i						 
							//
							//	Routine.FileDataSize++;																		 							 

							//}
						}						 
 					}
				}
			}	
			

		}			
	}
		if( S0_Flag==1)strcpy_s(Calibration.S0_Data,300,S0_Data);
		else strcpy_s(Calibration.S0_Data,300,"S00600004844521B");
 
		if( S7_Flag==1){strcpy_s(Calibration.S7_Data,300,S7_Data);Calibration.S7_Flag=1;}
		else Calibration.S7_Flag=0;
 

 	 CloseFile();
	return 1;
}
int COperateFile::ReadDataFromFileTobuf(u32 add[2],int ctrl)
{
		int ss=0;
	 S0_Flag=S7_Flag=0;
	U32 sizeAdd=0;
	int value1=0;
	int value2=0;
	int readnum=0;
	unsigned int Slinelength;
	if(ctrl == 0)memset(&Calibration,config->ECUprg.Fill_Byte,sizeof(TransDataType));//
	else memset(&Routine,config->ECUprg.Fill_Byte,sizeof(TransDataType));//
	memset(Bank_flag,0,sizeof(Bank_flag));
	Calibration.FileDataSize=0;
	Routine.FileDataSize=0;	 
	while(1)
	{
		if( !ReadRowDataToSRecord() )
		{
			break;
		}
		Slinelength =S_RealDataLength;
		//config->AddressStart 0xa0020000
		if( S_RecordType >0 && S_RecordType < 4 )
		{
			if( S_Address < add[0]  && S_Address+S_RealDataLength>= add[0])
			{
				int j;
				for(j=0;j<S_RealDataLength;j++)
				{
					if(S_Address== add[0])break;
					S_Address++;
				}
					Slinelength=S_RealDataLength-j;				
			}
			 

			if( S_Address >= add[0]  && S_Address<= add[1])
			{
				for(int j=0;j<config->banknum;j+=2)
				{
					if( S_Address>=config->ECUprg.Bankaddr[j]  && S_Address<=config->ECUprg.Bankaddr[j+1])
					{			
						value1=j;
						if(value1 != value2)
						{
							sizeAdd=0;
							for(int n=0;n<config->banknum;n++)
							{
								if(Bank_flag[n] == 1)sizeAdd+=config->ECUprg.Bankaddr[n+1]-config->ECUprg.Bankaddr[n]+1;
							}
							value2=value1;
						}
						for(int i=0;i<Slinelength;i++)
						{
 							if(ctrl == 0) 
							{
								Calibration.totalTransData.FileData[(S_Address++) - config->ECUprg.Bankaddr[j]+sizeAdd]  = S_Data[i];        //当文件内的地址不连续 但是 不缺少时 使用S_Address - MT221AddressStart + i 当连续的地址时使用 Calibration.FileDataSize + i						 
							
								Calibration.FileDataSize++;																		 							 

							}
							else
							{
							Routine.totalTransData.FileData[(S_Address++) - config->ECUprg.Bankaddr[j]+sizeAdd]  = S_Data[i];        //当文件内的地址不连续 但是 不缺少时 使用S_Address - MT221AddressStart + i 当连续的地址时使用 Calibration.FileDataSize + i						 
							
							Routine.FileDataSize++;																		 							 

							}
						}						 
						Bank_flag[j]=1;
					}
				}
			}	
			

		}			
	}
	if(ctrl == 0) 
	{
		if( S0_Flag==1)strcpy_s(Calibration.S0_Data,300,S0_Data);
		else strcpy_s(Calibration.S0_Data,300,"S00600004844521B");
 
		if( S7_Flag==1){strcpy_s(Calibration.S7_Data,300,S7_Data);Calibration.S7_Flag=1;}
		else Calibration.S7_Flag=0;
 
	}
	else
	{
		if( S0_Flag==1)strcpy_s(Routine.S0_Data,300,S0_Data);
		else strcpy_s(Routine.S0_Data,300,"S00600004844521B");
		if( S7_Flag==1){strcpy_s(Routine.S7_Data,300,S7_Data);Routine.S0_Flag=1;}
		else Routine.S0_Flag=0;
	}
	return 1;
}
int COperateFile::ReadDataFromCStringArrayTobuf(u32 add[2],int ctrl)
{
		int ss=0;
	 S0_Flag=S7_Flag=0;
	U32 sizeAdd=0;
	int value1=0;
	int value2=0;
	int readnum=0;
	unsigned int Slinelength;
	if(ctrl == 0)memset(&Calibration,config->ECUprg.Fill_Byte,sizeof(TransDataType));//
	else memset(&Routine,config->ECUprg.Fill_Byte,sizeof(TransDataType));//
	memset(Bank_flag,0,sizeof(Bank_flag));
	Calibration.FileDataSize=0;
	Routine.FileDataSize=0;	 
	CString str;
	int   size=g_strArray.GetSize(); 
	for(int k=0;k<size;k++)
	{
		str=g_strArray.GetAt(k);
		if( !ReadRowDataToSRecordFromCString(str) )
		{
			break;
		}
		Slinelength =S_RealDataLength;
		//config->AddressStart 0xa0020000
		if( S_RecordType >0 && S_RecordType < 4 )
		{
			if( S_Address < add[0]  && S_Address+S_RealDataLength>= add[0])
			{
				int j;
				for(j=0;j<S_RealDataLength;j++)
				{
					if(S_Address== add[0])break;
					S_Address++;
				}
					Slinelength=S_RealDataLength-j;				
			}

			if( S_Address >= add[0]  && S_Address<= add[1])
			{
				for(int j=0;j<config->banknum;j+=2)
				{
					if( S_Address>=config->ECUprg.Bankaddr[j]  && S_Address<=config->ECUprg.Bankaddr[j+1])
					{			
						value1=j;
						if(value1 != value2)
						{
							sizeAdd=0;
							for(int n=0;n<config->banknum;n++)
							{
								if(Bank_flag[n] == 1)sizeAdd+=config->ECUprg.Bankaddr[n+1]-config->ECUprg.Bankaddr[n]+1;
							}
							value2=value1;
						}
						for(int i=0;i<Slinelength;i++)
						{
 							if(ctrl == 0) 
							{
								Calibration.totalTransData.FileData[(S_Address++) - config->ECUprg.Bankaddr[j]+sizeAdd]  = S_Data[i];        //当文件内的地址不连续 但是 不缺少时 使用S_Address - MT221AddressStart + i 当连续的地址时使用 Calibration.FileDataSize + i						 
							
								Calibration.FileDataSize++;																		 							 

							}
							else
							{
							Routine.totalTransData.FileData[(S_Address++) - config->ECUprg.Bankaddr[j]+sizeAdd]  = S_Data[i];        //当文件内的地址不连续 但是 不缺少时 使用S_Address - MT221AddressStart + i 当连续的地址时使用 Calibration.FileDataSize + i						 
							
							Routine.FileDataSize++;																		 							 

							}
						}						 
						Bank_flag[j]=1;
					}
				}
			}			

		}			
	}	
	return 1;
}
int COperateFile::ReadDataFromCStringArrayTobuf_Checksum(u32 add[2],int ctrl,int checksumCtrl,int isMerge,unsigned int *returnsum)
{
	int ss=0;
	S0_Flag=S7_Flag=0;
	U32 sizeAdd=0;
	int value1=0;
	int value2=0;
	int readnum=0;
	tempCheckSum=0;
	unsigned int Slinelength;
	if(ctrl == 0)memset(&Calibration,config->ECUprg.Fill_Byte,sizeof(TransDataType));//
	else memset(&Routine,config->ECUprg.Fill_Byte,sizeof(TransDataType));//
	memset(Bank_flag,0,sizeof(Bank_flag));
	Calibration.FileDataSize=0;
	Routine.FileDataSize=0;	 
	CString str;
	unsigned int  KKSUM_Index;
	bool kksum_flag=0;
		KKSUM_Index = config->ECUprg.ChecksumAddr[checksumCtrl];//KKSUM 地址

	int   size=g_strArray.GetSize(); 
	for(int k=0;k<size;k++)
	{
		str=g_strArray.GetAt(k);
		if( !ReadRowDataToSRecordFromCString(str) )
		{
			break;
		}
		Slinelength =S_RealDataLength;
		//config->AddressStart 0xa0020000
		if( S_RecordType >0 && S_RecordType < 4 )
		{
			if( S_Address < add[0]  && S_Address+S_RealDataLength>= add[0])
			{
				int j;
				for(j=0;j<S_RealDataLength;j++)
				{
					if(S_Address== add[0])break;
					S_Address++;
				}
					Slinelength=S_RealDataLength-j;				
			}

			if( S_Address >= add[0]  && S_Address<= add[1])
			{
				for(int j=0;j<config->banknum;j+=2)
				{
					if( S_Address>=config->ECUprg.Bankaddr[j]  && S_Address<=config->ECUprg.Bankaddr[j+1])
					{			
						value1=j;
						if(value1 != value2)
						{
							sizeAdd=0;
							for(int n=0;n<config->banknum;n++)
							{
								if(Bank_flag[n] == 1)sizeAdd+=config->ECUprg.Bankaddr[n+1]-config->ECUprg.Bankaddr[n]+1;
							}
							value2=value1;
						}
						for(int i=0;i<Slinelength;i++)
						{
							if(config->ChecksumNum >0)//如果有checksum
							{
 								for(int m=0;m<config->WordsumNum[checksumCtrl];m+=2)
								{
									if(S_Address >= config->ECUprg.Wordsumaddr[checksumCtrl][m]&& S_Address <= config->ECUprg.Wordsumaddr[checksumCtrl][m+1])						
									{
										if((S_Address == config->ECUprg.ChecksumAddr[checksumCtrl])||(S_Address == (config->ECUprg.ChecksumAddr[checksumCtrl]+1)))break;
										if(config->ECUprg.ChecksumOrder[checksumCtrl] == 1)//big endian
										{
											if( i%2 == 0 )
												tempCheckSum +=(((unsigned int)S_Data[i])<<8);
											else
												tempCheckSum += S_Data[i];
											break;	
										}
										else
										{
											if( i%2 == 0 )
												tempCheckSum +=S_Data[i];
											else
												tempCheckSum += (((unsigned int)S_Data[i])<<8);
											break;	
										}
									}
									
								}
							}
							/*****/
									
						if(S_Address == config->ECUprg.ChecksumAddr[checksumCtrl])
						{
							KKSUM_Index=(unsigned int)Routine.FileDataSize;
							kksum_flag=1;
						}
						 

 							if(ctrl == 0) 
							{
								Calibration.totalTransData.FileData[(S_Address++) - config->ECUprg.Bankaddr[j]+sizeAdd]  = S_Data[i];        //当文件内的地址不连续 但是 不缺少时 使用S_Address - MT221AddressStart + i 当连续的地址时使用 Calibration.FileDataSize + i						 
							
								Calibration.FileDataSize++;																		 							 

							}
							else
							{
								Routine.totalTransData.FileData[(S_Address++) - config->ECUprg.Bankaddr[j]+sizeAdd]  = S_Data[i];        //当文件内的地址不连续 但是 不缺少时 使用S_Address - MT221AddressStart + i 当连续的地址时使用 Calibration.FileDataSize + i						 
							
								Routine.FileDataSize++;																		 							 

							}
						}						 
						Bank_flag[j]=1;
					}
				}
			}			

		}			
	}
	*returnsum=tempCheckSum;
		if((config->ChecksumNum >0)&&(kksum_flag==1))
		{
			if(config->ECUprg.ChecksumOrder[checksumCtrl] == 1)
				config->FileChecksum = (Routine.totalTransData.FileData[KKSUM_Index]<<8) | Routine.totalTransData.FileData[KKSUM_Index+1];
			else
				config->FileChecksum = (Routine.totalTransData.FileData[KKSUM_Index+1]<<8) | Routine.totalTransData.FileData[KKSUM_Index];
			//config->Checksum=S_KKSUM;

		}

	return 1;
}

int COperateFile::CheckCurrentBank(u32 add[2])
{
		int ss=0;
	 
	U32 sizeAdd=0;
	int value1=0;
	int value2=0;
	int readnum=0;
 	
	csFile.SeekToBegin(); 
	while(1)
	{
		if( !ReadRowDataToSRecord() )
		{
			break;
		}
		//config->AddressStart 0xa0020000
		if( S_RecordType >0 && S_RecordType < 4 )
		{
			if( S_Address >= add[0]  && S_Address<= add[1])
			{
				return 1;
			}
		}			
	}
	return 0;
}

int COperateFile::ReadDataFromFileTobuf_BOOT(u32 add[2],int ctrl)
{
	int ss=0;
 	U32 sizeAdd=0;
	int value1=0;
	int value2=0;
	int readnum=0;
		 S0_Flag=S7_Flag=0;

	memset(&Calibration,config->ECUprg.Fill_Byte,sizeof(TransDataType));//
 		Calibration.FileDataSize=0; 
	while(1)
	{
		if( !ReadRowDataToSRecord() )
		{
			break;
		}
		//config->AddressStart 0xa0020000
		if( S_RecordType >0 && S_RecordType < 4 )
		{
			if( S_Address >= add[0]  && S_Address<= add[1])
			{
			
				for(int i=0;i<S_RealDataLength;i++)
				{
				
					Calibration.totalTransData.FileData[(S_Address++) - add[0]]  = S_Data[i];        //当文件内的地址不连续 但是 不缺少时 使用S_Address - MT221AddressStart + i 当连续的地址时使用 Calibration.FileDataSize + i						 
							
					Calibration.FileDataSize++;																		 							 
				}
			}									 
		}			
	}
	return 1;
}

