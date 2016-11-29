#ifndef DONGLEDLL_H
#define DONGLEDLL_H

typedef enum
{
	ENTERPRISE_VERSION,
	EXPRESS_VERSION,
	PROFESSIONAL_VERSION,
	ADV_PROFESSIONAL_VERSION,
	CUSTOM_BUILT_VERSION

}DONGLE_VERSION;

// Function pointer types
typedef bool(*DELPHI_DONGLE_CHECK_T)(DONGLE_VERSION);
typedef DWORD(*DELPHI_GET_DONGLE_SN_T)();
typedef DWORD(*DELPHI_ENCRYPT_INPUT_LEN_T)(CString str);
typedef DWORD(*DELPHI_ENCRYPT_DATASTREAM_T)(CString str, BYTE *pOutput);

// Check if the dongle is pluged in
//bool _declspec(dllexport) DelphiDongleCheck(DONGLE_VERSION version);
//
//// Get Dongle's Serial Number, this is a GUID guaranteed by the dongle manufactor
//DWORD _declspec(dllexport) GetDelphiDongleSN();
//
//// Use this function to make sure how much data you need to allocate.
//DWORD _declspec(dllexport) EncryptInputLen(CString str);
//
//// The datastream encrypt function
//// Call EncryptInputLen() first to allocate pOutput
//// Then call this function, the return value is the final output's length
//DWORD _declspec(dllexport) EncryptDatastream(CString str, BYTE *pOutput);

class DongleUtility
{
public:
   DongleUtility();
   ~DongleUtility();
   bool DelphiDongleCheck(DONGLE_VERSION in_version);
   DWORD GetDelphiDongleSN();
   DWORD EncryptInputLen(CString str);
   DWORD EncryptDatastream(CString str, BYTE *pOutput);
   bool CheckDllIntegrity();

private:
   HMODULE m_hDongleDLL;
   DELPHI_DONGLE_CHECK_T m_fDelphiDongleCheck;
   DELPHI_GET_DONGLE_SN_T m_fGetDelphiDongleSN;
   DELPHI_ENCRYPT_INPUT_LEN_T m_fEncryptInputLen;
   DELPHI_ENCRYPT_DATASTREAM_T m_fEncryptDatastream;
};

#endif
