#pragma once
#include "ecufileoperate.h"
class COperateFile :
	public CEcuFileOperate
{
public:
	
	int GetTransData(CString &FilePath ,unsigned char Type);
	int COperateFile::ReadDataFromFileTobuf(u32 add[2],int ctrl);
	int COperateFile::ReadDataFromFileTobuf_BOOT(u32 add[2],int ctrl);
	COperateFile(void);
	unsigned short tempCheckSum;
	~COperateFile(void);
	int COperateFile::GetModuleTransData(CString &FilePath ,int type,int ctrl);
	int COperateFile::CheckCurrentBank(u32 add[2]);
	int COperateFile::GetFileBankNum(CString &FilePath ,int type);
int COperateFile::GetFileBankNum_ptpcal(CString &FilePath, CString &FilePath2,int type);
	int COperateFile::ReadDataFromFileTobuf_buildcut(CString &FilePath,u32 add[2],int ctrl);
int COperateFile::ReadDataFromCStringArrayTobuf(u32 add[2],int ctrl);
int COperateFile::GetModuleTransDataFromCStringArray(CString &FilePath ,int type,int ctrl);
int COperateFile::ReadDataFromCStringArrayTobuf_Checksum(u32 add[2],int ctrl,int checksumCtrl,int isMerge,unsigned int *returnsum);

};

