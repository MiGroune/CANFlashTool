
// CANFLSAHTOOLDlg.h : 头文件
//

#pragma once


#include "ProjectEdit.h"
#include "afxwin.h"
#include "RichEditCtrlEx.h"
#include "FailDlg.h"
#include "USB2CAN_interface.h"  
#include "xlCANFunctions.h"
#include "AN_Flash.h"
#include "Querydlg.h"
#include "SuccessFlash.h"

#include "DongleDll.h"

#define ADDMESS 0
#define FAIL    1
#define SUCCESS 2
typedef struct 
{
   HWND hWnd;
   char cWinBuf[256];
}WINLIST;
struct CTOOLINFO : public TOOLINFO {
 
    int nWidth; //Width of box, or 0 for default
    COLORREF clrTextColor; //text color
    COLORREF clrBackColor; //background color
};
typedef struct 
{
   char cCloseList[100];
}CLOSELIST;
// CCANFLSAHTOOLDlg 对话框

#define MENUPOSITION			6
typedef struct COMBOPATH
{
   CString szCutBuf[10];
   CString szPtpBuf[10];
   CString szCalBuf[10];
   CString szBootBuf[10];
   CString szMfgBuf[10];
   CString szTswBuf[10];
   BYTE nCutNum;
   BYTE nCalNum;
   BYTE nPtpNum;
   BYTE nBootNum;
   BYTE nMfgNum;
   BYTE nTswNum;
}ComboPath;
#define WM_MESSAGE_CANIN       (WM_USER+101)
#define WM_MESSAGE_CANOUT      (WM_USER+102)

class CCANFLSAHTOOLDlg : public CDialogEx
{
   // 构造
public:
   CCANFLSAHTOOLDlg(CWnd* pParent = NULL);	// 标准构造函数

   // 对话框数据
   enum { IDD = IDD_CANFLSAHTOOL_DIALOG };

protected:
   virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
   virtual void OnCancel();
BOOL CCANFLSAHTOOLDlg::OnDeviceChange(UINT nEventType, DWORD dwData);
   // 实现
protected:
   HICON m_hIcon;
BOOL CCANFLSAHTOOLDlg::SetTipText( UINT id, NMHDR * pTTTStruct, LRESULT * pResult );
   // 生成的消息映射函数
   virtual BOOL OnInitDialog();
   afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
   afx_msg void OnPaint();
   afx_msg HCURSOR OnQueryDragIcon();
   DECLARE_MESSAGE_MAP()
   LRESULT   CCANFLSAHTOOLDlg::OnCANINMessage(WPARAM wParam, LPARAM lParam);
 
LRESULT   CCANFLSAHTOOLDlg::OnCANOUTMessage(WPARAM wParam, LPARAM lParam);
public:
	  BOOL	deviceQuitFlag;
	  //CAN2USB_DevInfo m_CanDevice[64];
	  int m_NumDevices;
	  int DevState;
	  char  OpenDevPath[256];


	  class CProjectEdit *m_pProjectEdit;
      class CAN_Flash *m_pCANFlash;
      class Querydlg *querydlg;   
      class CActionSetting *m_pCheckSet;  
      class CBuildCut *m_pbuildcut;
      class CFailDlg m_faildlg;
      class SuccessFlash m_successdlg;
      //class COperateFile *m_pOpFile;
      class cdisplay *m_pDisplay;
      bool m_bCurrentPro;//用来判断project显示对话框是否是新工程还是编辑当前
      BOOL m_bdeviceQuitFlag;

      CComboBox m_bEcuFile;
      //CString m_szMfgPath;
      //CString m_szBootPath;
      //CString m_szCutPath;
      //CString m_szCalPath;
      //CString m_szPtpPath;
      CString m_szFltPath;
      CString m_szECUType;
      CString m_szConfigPath;
      CString m_szCommandPath;
      CString m_szSaveFltfPath;
      CString m_szBuildPath;
      CStdioFile  csFile; 
      CStdioFile  csFilelog;
      CString m_szFilelog;
		CStdioFile  csFilecut; 

	  CString m_szlogDirectory;
      CString m_szBeforePath;
      CString m_szAfterPath;
      BOOL m_bBeforeFlag;
      BOOL m_bAfterFlag;
      BOOL m_bMergeFlag;
      BOOL m_bNVFlag;
      BOOL m_bECUFlag;
      BOOL m_bDefaultFlag;
	  BOOL m_bChecksumFlag;
	  BOOL m_bEnableStopFlag;
	  BOOL m_bOnFlashFlag;
	  BOOL m_bBypassFlag;
	  //bool m_bUsbChangeFlag;
	 // BOOL m_bSetParamFlag;
	  BOOL m_bChecksumCorrectFlag;

      int m_nflashtime;
      DWORD m_dwSN;
      CWnd *m_pWnd; 
CToolTipCtrl m_ctrlTT;
      CAN_protocol *CANprotocol;
      //CString g_szKeyPath;
      void CCANFLSAHTOOLDlg::AddComboFile(int type,CString str);
	  int CCANFLSAHTOOLDlg::FindFileName1(CString filepath);
      afx_msg void OnFileNewproject();
      afx_msg void OnEditEditcurrentproject();
      afx_msg void OnBnClickedButtonApp();
      afx_msg void OnBnClickedButtonCal();
      afx_msg void OnBnClickedButtonCut();
      afx_msg void OnSelchangeComboFiletype();
      afx_msg void OnSelchangeComboPTP();
      afx_msg void OnSelchangeComboCAL();
      afx_msg void OnSelchangeComboCUT();
      afx_msg void OnSelchangeComboBOOT();
      afx_msg void OnSelchangeComboMFG();
      afx_msg void ChangeComboFile(int type,CString str);
      afx_msg void DisplayFileTypeState();
      afx_msg void OnBnClickedButtonFlash();
      afx_msg void OnBnClickedButtonStop();
	  afx_msg void DisplayFileTypeSpecialState(bool flag);

      afx_msg void AutoCheckCANcard(void);
      afx_msg void OnBnClickedRadioCan2usb();
      afx_msg void OnBnClickedRadioCancard2();
      afx_msg void OnBnClickedRadioCancardxl();
      afx_msg int  openUSB();
      afx_msg int closeUSB();
      afx_msg void showStatus(CString statusStr,BYTE addFlag);
      afx_msg void OnFileLoadproject();
      afx_msg int OpenFlt(CString m_Path);
      afx_msg void DisplayProject(int ctrl);
      afx_msg int OperateFile(int type);
      afx_msg void WriteFileHeader(void);
      afx_msg void OnTimer(UINT_PTR nIDEvent);
      afx_msg void DisplayActionItem();
      afx_msg int readCommandFile(void);
      afx_msg void CombinationCUT(unsigned char buf[],unsigned int size,unsigned int add);
      afx_msg BYTE getHexByte(CString& str,int *index);
      afx_msg int DisplayActionItemCurrentValue();
      afx_msg int MergeActionItemCurrentValue();
      afx_msg BYTE getSize(CString& str,U32 *index);
      afx_msg void ButtonStatus(int state,int ctrl);
      afx_msg bool CheckFlashItems();
      afx_msg void Dongle();
      afx_msg void OnSize(UINT nType, int cx, int cy);
      static DWORD WINAPI CEcuFlashToolDlgDisplayAllThread(LPVOID lpParameter);	 
      static DWORD WINAPI CEcuFlashToolDlgFlashProcThread(LPVOID lpParameter);
      static DWORD WINAPI CEcuFlashToolDlgCommandThread(LPVOID lpParameter);
      static DWORD WINAPI CCANFLSAHTOOLDlgBuildCutThreadThread(LPVOID lpParameter);
      static DWORD WINAPI CCANFLSAHTOOLDlgBuildCutPreThreadThread(LPVOID lpParameter);
      static DWORD WINAPI CEcuFlashToolDlgTestThread(LPVOID lpParameter);
      LRESULT   CCANFLSAHTOOLDlg::OnGetDefID(WPARAM   wp,   LPARAM   lp);
private:
      HANDLE FlashProcThread ;
      HANDLE CommandThread ;
      HANDLE DongleThread ;
      HANDLE BuildCutThread ;
      HANDLE BuildCutPreThread;
      HANDLE testThread ;
      HANDLE DisplayAllThread ;
      DongleUtility m_DongleUtility;

public:
      afx_msg void OnProjectSaveproject();
      afx_msg void OnEditSpecialactioneditor();
      afx_msg void OnToolsSavedatastream();
      afx_msg void OnToolsCommandfile();
      afx_msg void OnProjectExit();
      afx_msg void OnHelpAbout();
      CComboBox m_ComboPtp;
      CComboBox m_ComboCal;
      CComboBox m_ComboCut;
      CComboBox m_bECUType;
      CComboBox m_ComboBoot;
      CComboBox m_ComboMfg;

      int m_nDongleType;
      CRect m_rect;//用于保存对话框大小变化前的大小
      void  ChangeSize(UINT nID,int x,int y);
      HBITMAP m_hBitmapBack;
      HBITMAP m_hBitmap;
      CStatic m_StaticFrame;
      afx_msg void OnToolsDisplaydatastream();
      afx_msg   LRESULT   OnKickIdle(WPARAM   wParam,   LPARAM   lParam); 
      int timeCount;
      afx_msg void OnBnClickedButtonMfg();	
      afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
      afx_msg void OnBnClickedCheckClearnv();
      afx_msg void OnBnClickedCheckReadecu();
      afx_msg void OnBnClickedButtonBoot();

      void CCANFLSAHTOOLDlg::HideStatus(int state,int ctrl);
      afx_msg void OnToolsBuildcutfile();
      int CCANFLSAHTOOLDlg::SetActionItemCurrentValue(int count,int type);
      int CCANFLSAHTOOLDlg::Recurse(LPCTSTR pstr,LPCTSTR pfilestr);
      void CCANFLSAHTOOLDlg::Saveproject(CString path);
      afx_msg LRESULT onAddCANMsgToList(WPARAM flag, LPARAM lparam);
      void CCANFLSAHTOOLDlg::setFlashState(bool flag);
      int CCANFLSAHTOOLDlg::Writedatastream(CString path,char type,int result);
	int CCANFLSAHTOOLDlg::CollectUSBInfo();
      HANDLE	WriteMsgMutex;
      bool m_bflag;
      CString m_szExePath;
      int choose_flag;
      BOOL isCanFlag;
      ComboPath m_ComboPath;
      CString filebuf;
      CString writestr;
      CString strdate;
      unsigned int m_nPassNum;
      unsigned int m_nFailNmu;
      afx_msg void OnSetFocus(CWnd* pOldWnd);
	  int CCANFLSAHTOOLDlg::OpenPrg();
	  afx_msg void OnBnClickedRadioIcan();
	  afx_msg void OnMenuDeal(UINT nID);
	  char m_nMenuNum;
	  int m_nIcanStatus;
	  int m_nES581Status;
	  int CCANFLSAHTOOLDlg::ReadProject(CString str);
	  void CCANFLSAHTOOLDlg::AddPrgToMenu(CString path);

	  afx_msg void OnProjectSaveprojectas();
	  void CCANFLSAHTOOLDlg::ReadRecentProjectLog();
	  CString m_szProLogName;
	  CString m_szProLog[5];
	  void CCANFLSAHTOOLDlg::DisplayPrgToMenu(CString path);
	  virtual BOOL PreTranslateMessage(MSG* pMsg);
	  int CCANFLSAHTOOLDlg::CheckSpecialCalAddress(void);
	  afx_msg void OnBnClickedRadioEs581();
	  //	char  OpenDevPath[256];
	  afx_msg void OnBnClickedButtonBuild();
	  void CCANFLSAHTOOLDlg::CombinationCUTtoCString(unsigned char buf[],unsigned int size,unsigned int add);
	  CComboBox m_bPRGPath;
	  int CCANFLSAHTOOLDlg::ComputeChecksum();
	  void CCANFLSAHTOOLDlg::BuildcutAndComputeCheckSum();
	  bool buildreturn;
	  afx_msg void OnBnClickedButtonTsw();
	  CComboBox m_ComboTsw;
	  afx_msg void OnSelchangeComboTsw();
};
