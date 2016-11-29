/*----------------------------------------------------------------------------
| File        : DongleDll.cpp
| Project     : CANFlashtool 
|
| Description : Shows the basic functionality for the dongle Library
|----------------------------------------------------------------------------*/
#include "stdafx.h"
#include "DongleDll.h"

DongleUtility::DongleUtility()
{
   // load dll here.
   m_hDongleDLL = LoadLibrary(_T("DongleDll.dll"));

   // get all functions
   if(m_hDongleDLL != NULL)
   {
      m_fDelphiDongleCheck = DELPHI_DONGLE_CHECK_T(GetProcAddress(m_hDongleDLL, "DelphiDongleCheck"));  

      m_fGetDelphiDongleSN =
         DELPHI_GET_DONGLE_SN_T(GetProcAddress(m_hDongleDLL, "GetDelphiDongleSN"));

      m_fEncryptInputLen =
         DELPHI_ENCRYPT_INPUT_LEN_T(GetProcAddress(m_hDongleDLL, "EncryptInputLen"));

      m_fEncryptDatastream =
         DELPHI_ENCRYPT_DATASTREAM_T(GetProcAddress(m_hDongleDLL, "EncryptDatastream"));
   }
}

DongleUtility::~DongleUtility()
{
   // free dll here
   if(m_hDongleDLL != NULL)
   {
      FreeLibrary(m_hDongleDLL);
   }
}

bool DongleUtility::CheckDllIntegrity()
{
   if(m_hDongleDLL == NULL
         || m_fDelphiDongleCheck == NULL
         || m_fEncryptDatastream == NULL
         || m_fEncryptInputLen == NULL
         || m_fGetDelphiDongleSN == NULL)
   {
      return false;
   }

   return true;
}

bool DongleUtility::DelphiDongleCheck(DONGLE_VERSION in_version)
{
   return m_fDelphiDongleCheck(in_version);
}

DWORD DongleUtility::GetDelphiDongleSN()
{
   return m_fGetDelphiDongleSN();
}

DWORD DongleUtility::EncryptInputLen(CString str)
{
   return m_fEncryptInputLen(str);
}

DWORD DongleUtility::EncryptDatastream(CString str, BYTE *pOutput)
{
   return m_fEncryptDatastream(str, pOutput);
}

