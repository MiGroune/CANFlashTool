#pragma once


#ifndef CAN_FLASH
#define CAN_FLASH
#include "SpecialType.h"
#include "CAN_protocol.h"
#include "EcuFileOperate.h"
 
#endif

typedef DWORD (*FUNCE)(DWORD);
	typedef DWORD (*FUNCF)(DWORD , BYTE* );

struct SPS_Instruction
{
  U8 step;
  U8  opcode;
  U8  actionfield[4];
  U8  gotofield[5][2];
};
class CAN_Flash:public CAN_protocol
{
public:
	CAN_Flash();
 	void display(CString buffer,int result);
	void GetbuftoDisplay(int Respons,int type);
 	int CAN_Readfile(long position,unsigned char* buffer,U32 lenth,int flag);
	void CAN_Flash::GetPrebuftoDisplay(char *buf);
	int CAN_Flash::CAN_Diagnostic(void);
 	//U32 flashsize;
	U32 CANNo;
	
	U32 basemodelnum;
	U32 endmodelnum;
	int m_nEcuFile;
	DWORD m_dwSN;
 
	int CAN_Flash::LoadPath(CString path,unsigned int *seed);
	HMODULE hDLL;
	BYTE SA[4];
	U32 SeedOrKey;
	int CAN_Flash::CAN_ReadStep(int type);
	int CAN_Flash::ResponsProcessing(int Respons,U8 *p,int step);
	int CAN_Flash::ReadfileStep(U8 *p,int step);
	struct SPS_Instruction 	SPS_Inst;
	struct SPS_Instruction 	*pSPS_Inst;
	U8 loopcounter[10];
	U32 past;
	u32 CAN_Flash::RemapAddress(int type,int ctrl);
	U8 CalFlashNum;
	U8 SepCalFlashNum;
	U8 AppFlashNum;
	int CAN_Flash::CAN_DiagnosticSecurityAccess(void);
int CAN_Flash::CheckAscii(unsigned char *buf,int size);
int CAN_Flash::getkeydll();
unsigned char CAN_Flash::GetValue(CString str,int type);
int CAN_Flash::seadkeycommand(CString strpath,CString strdllname,CString strindex);
int CAN_Flash::dealwithdllpathname(CString strpath);
 int CAN_Flash::checkdllpathname(CString strpath);
 };

