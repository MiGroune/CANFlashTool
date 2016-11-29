/*----------------------------------------------------------------------------
| File        : ConfigFile.cpp
| Project     : CANFlashtool 
|
| Description : deal with the configure file(*.prg)
|-----------------------------------------------------------------------------
| $Author: liaoshaoyan      $Revision:  
| $Data    2014.04.09   
|-----------------------------------------------------------------------------*/
#include "StdAfx.h"
#include "ConfigFile.h"


ConfigFile::ConfigFile()
{
	banknum = 0;
}
int ConfigFile::HextoAdd(char *buf,U32 *addr)
{
	while(*buf != 0)
	{
		if(*buf>='A'&&*buf<='F')
		{
			*addr = *addr*16+(*buf - 0X37);
		}
		else if(*buf>='a'&&*buf<='f')
		{	
			*addr = *addr*16+(*buf - 'a')+10;
		}
		else 
		{
			*addr = *addr*16+(*buf - 0X30);
		}
		buf++;
	}
	return 1;
} 
void ConfigFile::CharToHex(unsigned char *buf)
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

//*********************************************************
//函数名：GetValue()
//功  能：找到数值
//参  数：CString str
//**********************************************************
unsigned int ConfigFile::GetValue(CString str,int type)
{
	int offset,len,m;
	unsigned int data=0;

	offset = str.Find(_T(' '));
	CString strLeft = str.Mid(offset);
	strLeft.TrimLeft();
	CStringA strReadA(strLeft);
	strcpy_s((char *)RowBuffer,500,strReadA);
	len = strlen((const char *)RowBuffer);
	CharToHex(RowBuffer);

	if(type == 0)
	{
		for(m=0;m<len;m++)data = data*10 + RowBuffer[m];
	}
	else 
	{
		if(RowBuffer[0]== 0 && RowBuffer[1] == 'x')m=2;
		else if(RowBuffer[0]== 0 && RowBuffer[1] == 'X')m=2;
		else m=0;

		for(;m<len;m++)data = data*16 + RowBuffer[m];
	}
	return data;
}
//*********************************************************
//函数名：GetAddr()
//功  能：找到数据地址
//参  数：CString str,U32 *addr
//**********************************************************
int ConfigFile::GetAddr(CString str,U32 *addr)
{
	int offset,len;
 	 int num=0;
	CString str1;
	for(int i=0;i<20;i++)
	{
		offset = str.Find(_T(' '));
		if(offset != -1)
		{
			*addr=0;
			CString strLeft = str.Mid(offset);
			strLeft.TrimLeft();
			offset = strLeft.Find(_T(':'));	
			if(offset != -1)
			{
				str1=strLeft.Left(offset);
				CStringA strReadA(str1);
				strcpy_s((char *)RowBuffer,500,strReadA);
				len = strlen((const char *)RowBuffer);
				CharToHex(RowBuffer);
				for(int m=0;m<len;m++)*addr = *addr *16 + RowBuffer[m];
				addr++;
				*addr=0;
				str1=strLeft.Mid(offset+1);
				CStringA strRead(str1);
				strcpy_s((char *)RowBuffer,500,strRead);
				len = strlen((const char *)RowBuffer);
				offset = str1.Find(_T(' '));	
				if(offset != -1){len = offset;}
				CharToHex(RowBuffer);
				for(int m=0;m<len;m++)*addr  = *addr *16 + RowBuffer[m];
				num++;
			}
			else
			{
				str1 = strLeft;
				CStringA strReadA(str1);
				strcpy_s((char *)RowBuffer,500,strReadA);
				len = strlen((const char *)RowBuffer);
				CharToHex(RowBuffer);
				for(int m=0;m<len;m++)addr[0]  = addr[0] *16 + RowBuffer[m];
				num++;
			}
			addr++;
			str = str1;
		}
	}
	num*=2;
	return num;
}
 
/**********************************************************
Read_Prg(CString m_Path)
读取配置文件：*.prg
参数：m_Path：路径
获取配置信息。配置信息存在结构体CAN_ECUPrg ECUprg中。
banknum用来表示刷写区域的区域数c
***********************************************************/
int ConfigFile::Read_Prg(CString m_Path)
{	
	CString strReadW;
 	U32 len = 0;
	int i,j,n;
 	int flag=0;
	int offset=0;

	ChecksumNum=0;
	banknum=0;
	flag=0;


	if(!csFile.Open(m_Path,CFile::modeRead))return -1;
	memset(&ECUprg,0x00,sizeof(ECUprg));
	for(int i=0;i<3;i++)ECUprg.funcType[i]=1;
	for(int i=3;i<6;i++)ECUprg.funcType[i]=0;
	 
	while(1)
	{
		if(flag == 0)
		{
			if(csFile.ReadString(strReadW)== NULL)break;
		}
		flag=0;
		//ECU TYPE
		if(strReadW.Find(_T("ECM_Type"))!=-1)
		{
			offset = strReadW.Find(_T(' '));
			ECU_Type=strReadW.Mid(offset);
			ECU_Type.TrimLeft();
		}
		//BB
		if(strReadW.Find(_T("Utility_Type_BB"))!=-1)
		{
			strReadW.MakeUpper();
			if(strReadW.Find(_T("FALSE")) != -1)
			{
				ECUprg.Utility_BB_Flag = FALSE;
			} 
			else ECUprg.Utility_BB_Flag= TRUE;//gai
		}
		//DELAY
		if(strReadW.Find(_T("Delay_Time_seconds"))!=-1)
		{
			ECUprg.Delay_Time_seconds =GetValue(strReadW,0);
		}
		//size 0x
		if(strReadW.Find(_T("CAN_Refalsh_DataSection_Size"))!=-1)
		{
			ECUprg.DataSection_Size =GetValue(strReadW,1);		 
		}
		//dongle
		if(strReadW.Find(_T("Write_Dongle"))!=-1)
		{
			strReadW.MakeUpper();
			if(strReadW.Find(_T("FALSE")) != -1)
			{
				ECUprg.g_bDongleFlag = FALSE;
			} 
			else ECUprg.g_bDongleFlag= TRUE;//gai
		}
		if(strReadW.Find(_T("Cal_App_Allocation"))!=-1)
		{
			strReadW.MakeUpper();
			if(strReadW.Find(_T("MIXED"))!=-1)
			{
				ECUprg.b_cal_app=MIXED;
			}
			else
				ECUprg.b_cal_app=SEPARATED;
		}
		if(strReadW.Find(_T("Dongle_Time_Milliseconds"))!=-1)
		{
			ECUprg.Dongle_Time_seconds =GetValue(strReadW,0);
		}
		if(strReadW.Find(_T("Boot_Allocation"))!=-1)
		{
			strReadW.MakeUpper();
			if(strReadW.Find(_T("MIXED"))!=-1)
			{
				ECUprg.b_boot=MIXED;
			}
			else
				ECUprg.b_boot=SEPARATED;
		}
		if(strReadW.Find(_T("Check_Type "))!=-1)
		{
			strReadW.MakeUpper();
			if(strReadW.Find(_T("MIXED"))!=-1)
			{
				ECUprg.b_boot=MIXED;
			}
			else
				ECUprg.b_boot=SEPARATED;
		}
		if(strReadW.Find(_T("[Start_Address]"))!=-1)
		{
			j = 0;		
			while(csFile.ReadString(strReadW))
			{					
				if((strReadW.Find(_T("Application"))!=-1))
				{
					ECUprg.Start_app=GetValue(strReadW,1);	
				}
				else if(strReadW.Find(_T("Calibration"))!=-1)
				{
					ECUprg.Start_cal=GetValue(strReadW,1);			
				}
				else 
				{
					if(strReadW.Find(_T("["))!=-1)
					flag=1;
					break;
				}
			}		
		}
		if(strReadW.Find(_T("[Bank_Definition]"))!=-1)
		{
			n=0;
			while(csFile.ReadString(strReadW))
			{				
				if((strReadW.Find(_T("Bank"))!=-1))
				{
					GetAddr(strReadW,&ECUprg.Bankaddr[n]);
					n+=2;
				}
				else 
				{
					banknum=n;
					flag=1;
					break;
				}

			}
		}

		if(strReadW.Find(_T("[Calibration_Module]"))!=-1)
		{	j = 0;		
			while(csFile.ReadString(strReadW))
			{					
				if((strReadW.Find(_T("Application"))!=-1))
				{
					ECUprg.AppAddrNum=GetAddr(strReadW,ECUprg.Appaddr);
				}
				else if(strReadW.Find(_T("Special_Part"))!=-1)
				{
					//j++;
					//if(j>2){flag=1;break;}
					ECUprg.SpecCalAddrNum =GetAddr(strReadW,ECUprg.SpecCaladdr);			
				}
				else if(strReadW.Find(_T("Calibration"))!=-1)
				{
 					ECUprg.CalAddrNum =GetAddr(strReadW,ECUprg.Caladdr);			
				}
				else 
				{
					if(strReadW.Find(_T("["))!=-1)
					flag=1;
					break;
				}
			}			 		 
		}	
		if(strReadW.Find(_T("[Remap_Address]"))!=-1)
		{	j = 0;		
			while(csFile.ReadString(strReadW))
			{					
				if((strReadW.Find(_T("Application_Part "))!=-1))
				{
					ECUprg.AppRemaNum=GetAddr(strReadW,ECUprg.AppRemapaddr);
				}
				else if(strReadW.Find(_T("Calibration_Part "))!=-1)
				{
					j++;
					if(j>1){flag=1;break;}
					ECUprg.CalRemaNum =GetAddr(strReadW,ECUprg.CalRemapaddr);			
				}
				else 
				{
					if(strReadW.Find(_T("["))!=-1)
					flag=1;
					break;
				}
			}			 		 
		}	
		if(strReadW.Find(_T("[Bootblock_Definition]"))!=-1)
		{			
			if(csFile.ReadString(strReadW))
			{	
				if((strReadW.Find(_T("Address"))!=-1))
				{
					GetAddr(strReadW,ECUprg.Bootblock);				
				}
			}			 		 
		}
		if(strReadW.Find(_T("[Mfgblock_Definition]"))!=-1)
		{			
			if(csFile.ReadString(strReadW))
			{	
				if((strReadW.Find(_T("Address"))!=-1))
				{
					GetAddr(strReadW,ECUprg.Mfgblock);				
				}
			}			 		 
		}
		if(strReadW.Find(_T("[Tswblock_Definition]"))!=-1)
		{			
			if(csFile.ReadString(strReadW))
			{	
				if((strReadW.Find(_T("Address"))!=-1))
				{
					GetAddr(strReadW,ECUprg.Tswblock);				
				}
			}			 		 
		}
		if(strReadW.Find(_T("[Fill_Byte]"))!=-1)
		{
			if(csFile.ReadString(strReadW))
			{
				ECUprg.Fill_Byte =GetValue(strReadW,1);				
			}
		}
		if(strReadW.Find(_T("[Checksum_Definition]"))!=-1)
		{
			if(csFile.ReadString(strReadW))
			{
				if(strReadW.Find(_T("Checksum_Definition_Number"))!=-1)
				{
					ChecksumNum =GetValue(strReadW,1);
				}
				for(i=0;i<ChecksumNum;i++)
				{
					while(csFile.ReadString(strReadW))
					{
						if(strReadW.Find(_T("Definition"))!=-1)
						{
							offset = strReadW.Find(_T(' '));
							ChecksumName[i]=strReadW.Mid(offset);
							ChecksumName[i].TrimLeft();
							 
						}
						else if(strReadW.Find(_T("Address"))!=-1)
						{
							GetAddr(strReadW,&ECUprg.ChecksumAddr[i]);	
						}	
						else if(strReadW.Find(_T("Size"))!=-1)
						{
							ECUprg.ChecksumSize[i] =GetValue(strReadW,1);
						}
						else if(strReadW.Find(_T("Byte_Ordering"))!=-1)
						{
							offset = strReadW.Find(_T(' '));
							ChecksumOrder[i]=strReadW.Mid(offset);
							ChecksumOrder[i].TrimLeft();
							ChecksumOrder[i].MakeUpper();
							if(ChecksumOrder[i].Find(_T("BIG"))!= -1)
								ECUprg.ChecksumOrder[i]=1;
							else ECUprg.ChecksumOrder[i]=0;

						}
						else if(strReadW.Find(_T("SUM"))!=-1)
						{
							WordsumNum[i]=GetAddr(strReadW,ECUprg.Wordsumaddr[i]);	
							break;
						}					
						else
						{
							flag=1;
							break;
						}
					}
				}
			}
		}
		if(strReadW.Find(_T("[KKPGMID_Definition]"))!=-1)
		{
			while(csFile.ReadString(strReadW))
			{
				if(strReadW.Find(_T("Address"))!=-1)
				{
					GetAddr(strReadW,&ECUprg.KkpgmIdAddr);	
				}	
				else if(strReadW.Find(_T("Size"))!=-1)
				{
					ECUprg.KkpgmIdSize =GetValue(strReadW,1);
				}
				else if(strReadW.Find(_T("Productive_PrgmId"))!=-1)
				{
					ECUprg.KkpmIdValue[0]=GetValue(strReadW,1);
				}
				else if(strReadW.Find(_T("Developing_PrgmId"))!=-1)
				{
					ECUprg.KkpmIdValue[1]=GetValue(strReadW,1);
					break;
				}			
				else
				{
					flag=1;
					break;
				}
			}
		}
		if(strReadW.Find(_T("[FlashTool_Fuction]"))!=-1)
		{
			ECUprg.funcFlag=1;
			while(csFile.ReadString(strReadW))
			{
				if(strReadW.Find(_T("Flash_PTP_CAL"))!=-1)
				{
					 strReadW.MakeUpper();
					if(strReadW.Find(_T("FALSE")) != -1)
					{
						ECUprg.funcType[2] = 0;
					} 
					 
				}	
				else if(strReadW.Find(_T("Flash_CUT"))!=-1)
				{
					 strReadW.MakeUpper();
					if(strReadW.Find(_T("FALSE")) != -1)
					{
						ECUprg.funcType[0] = 0;
					} 
				}
				else if(strReadW.Find(_T("Flash_CAL"))!=-1)
				{
					 strReadW.MakeUpper();
					if(strReadW.Find(_T("FALSE")) != -1)
					{
						ECUprg.funcType[1] = 0;
					} 
				}
				else if(strReadW.Find(_T("Flash_BOOL"))!=-1)
				{
					 strReadW.MakeUpper();
					if(strReadW.Find(_T("TRUE")) != -1)
					{
						ECUprg.funcType[3] = 1;
					} 
				}
				else if(strReadW.Find(_T("Flash_MFG"))!=-1)
				{
					 strReadW.MakeUpper();
					if(strReadW.Find(_T("TRUE")) != -1)
					{
						ECUprg.funcType[4] = 1;
					} 
				}
				else if(strReadW.Find(_T("Flash_TSW"))!=-1)
				{
					 strReadW.MakeUpper();
					if(strReadW.Find(_T("TRUE")) != -1)
					{
						ECUprg.funcType[5] = 1;
					} 
				}				
				else if(strReadW.Find(_T("Flash_BuildCut"))!=-1)
				{
					 strReadW.MakeUpper();
					if(strReadW.Find(_T("TRUE")) != -1)
					{
						ECUprg.Flash_Buildcut_Flag =1;
					} 
				}		
				else
				{
					flag=1;
					break;
				}
			}
		}

	}
	csFile.Close();
	for(int i=0;i<ChecksumNum;i++)
	{
		m_checksum[i].ChecksumName=ChecksumName[i];
		m_checksum[i].ChecksumAddr=ECUprg.ChecksumAddr[i];
		 
	}
	Appsize=Calsize=0;
	for(int i=0;i<banknum;i+=2)//刷写的长度 
	{
		if(ECUprg.Bankaddr[i] >= ECUprg.Caladdr[0] && ECUprg.Bankaddr[i] <= ECUprg.Caladdr[1])
			Calsize += ECUprg.Bankaddr[i+1] - ECUprg.Bankaddr[i] + 1;
 	}
	for(int i=0;i<banknum;i+=2)
	{
		if(ECUprg.Bankaddr[i] >= ECUprg.Appaddr[0] && ECUprg.Bankaddr[i] <= ECUprg.Appaddr[1])
			Appsize += ECUprg.Bankaddr[i+1] - ECUprg.Bankaddr[i] + 1;				
 	}
	Cutsize = Appsize+Calsize;

	return 1;
}

