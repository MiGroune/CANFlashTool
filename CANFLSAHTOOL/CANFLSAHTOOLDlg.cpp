
// CANFLSAHTOOLDlg.cpp : 实现文件
//

#include "stdafx.h"
//#include "ICAN.h"
#include "CANFLSAHTOOL.h"
#include "CANFLSAHTOOLDlg.h"
#include "afxdialogex.h"
#include "OperateFile.h"
#include "Global.h"
#include "AN_Flash.h"
#include "cdisplay.h"
#include "ActionSetting.h"
#include "MemDC.h"
#include <locale.h>
#include "BuildCut.h"
#include "dbt.h"
#include <Setupapi.h> 
#define HARDCAN 1
//#define USBDOG 1
//#include "ES581Function.h"
//#include "ICAN.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
extern CString strBootReason;
CString file[100];
int Flash_Result;
static	BOOL usb_flag=0;
static	BOOL card2_flag=0;
static	BOOL ican_flag=0;
// 用于应用程序“关于”菜单项的 CAboutDlg 对话框
extern CString strQueryInfor;
extern CAN_msg msg_Arr[600];
CString Curstr;
CString strLog;
HWND hWnd;
extern bool BootFlag;
SYSTEMTIME pcStartTime;
static int changenum=0;
class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CCANFLSAHTOOLDlg 对话框

CCANFLSAHTOOLDlg::CCANFLSAHTOOLDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCANFLSAHTOOLDlg::IDD, pParent)
	, m_szECUType(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON1);
	m_pProjectEdit =NULL;
	m_pCANFlash = NULL;
	m_pDisplay= NULL;
	querydlg = NULL;
	m_pCheckSet = NULL;
	m_pbuildcut=NULL;
	m_szCalPath = _T("");
	m_bBeforeFlag=0;
	m_bAfterFlag=0;
	g_bSetParamFlag=0;
	m_bNVFlag=0;
	m_bECUFlag=0;
	Display_flag=0;
	m_bDefaultFlag=1;
	m_bChecksumFlag=0;
	m_bEnableStopFlag=0;
	m_bBypassFlag=0;
	choose_flag=0;
	isCanFlag=0;
	m_nPassNum=0;
	m_nMenuNum=0;
	m_nFailNmu=0;
	g_nParamValue=1;
	m_szFltPath=_T("");
	buildreturn=0;
	//m_bUsbChangeFlag=0;
 }

void CCANFLSAHTOOLDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS_FIASH, m_Process);
	DDX_Control(pDX, IDC_RICHEDIT21_MINFOR, m_Infor);
	DDX_Control(pDX, IDC_COMBO_FILETYPE, m_bEcuFile);
	DDX_Control(pDX, IDC_COMBO_PTP, m_ComboPtp);
	DDX_Control(pDX, IDC_COMBO_CAL, m_ComboCal);
	DDX_Control(pDX, IDC_COMBO_CUT, m_ComboCut);
	DDX_Control(pDX, IDC_COMBO_BOOT, m_ComboBoot);
	DDX_Control(pDX, IDC_COMBO_MFG, m_ComboMfg);
	DDX_Control(pDX, IDC_COMBO_ECMTYPE, m_bECUType);

	DDX_Control(pDX, IDC_COMBO_PRG, m_bPRGPath);
	DDX_Control(pDX, IDC_COMBO_TSW, m_ComboTsw);
}

BEGIN_MESSAGE_MAP(CCANFLSAHTOOLDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY_EX( TTN_NEEDTEXT, 0, SetTipText )
	ON_COMMAND(ID_FILE_NEWPROJECT, &CCANFLSAHTOOLDlg::OnFileNewproject)
	ON_COMMAND(ID_EDIT_EDITCURRENTPROJECT, &CCANFLSAHTOOLDlg::OnEditEditcurrentproject)
	ON_BN_CLICKED(IDC_BUTTON_APP, &CCANFLSAHTOOLDlg::OnBnClickedButtonApp)
	ON_BN_CLICKED(IDC_BUTTON_CAL, &CCANFLSAHTOOLDlg::OnBnClickedButtonCal)
	ON_BN_CLICKED(IDC_BUTTON_CUT, &CCANFLSAHTOOLDlg::OnBnClickedButtonCut)
	ON_CBN_SELCHANGE(IDC_COMBO_FILETYPE, &CCANFLSAHTOOLDlg::OnSelchangeComboFiletype)
	ON_CBN_SELCHANGE(IDC_COMBO_PTP, &CCANFLSAHTOOLDlg::OnSelchangeComboPTP)
	ON_CBN_SELCHANGE(IDC_COMBO_CAL, &CCANFLSAHTOOLDlg::OnSelchangeComboCAL)
	ON_CBN_SELCHANGE(IDC_COMBO_CUT, &CCANFLSAHTOOLDlg::OnSelchangeComboCUT)
	ON_CBN_SELCHANGE(IDC_COMBO_BOOT, &CCANFLSAHTOOLDlg::OnSelchangeComboBOOT)
	ON_CBN_SELCHANGE(IDC_COMBO_MFG, &CCANFLSAHTOOLDlg::OnSelchangeComboMFG)
	ON_BN_CLICKED(IDC_BUTTON_FLASH, &CCANFLSAHTOOLDlg::OnBnClickedButtonFlash)
	ON_BN_CLICKED(IDC_BUTTON_STOP, &CCANFLSAHTOOLDlg::OnBnClickedButtonStop)
	ON_COMMAND(ID_FILE_LOADPROJECT, &CCANFLSAHTOOLDlg::OnFileLoadproject)
	ON_WM_TIMER()
	ON_COMMAND(ID_PROJECT_SAVEPROJECT, &CCANFLSAHTOOLDlg::OnProjectSaveproject)
	ON_COMMAND(ID_EDIT_SPECIALACTIONEDITOR, &CCANFLSAHTOOLDlg::OnEditSpecialactioneditor)
	ON_COMMAND(ID_TOOLS_SAVEDATASTREAM, &CCANFLSAHTOOLDlg::OnToolsSavedatastream)
	ON_COMMAND(ID_TOOLS_COMMANDFILE, &CCANFLSAHTOOLDlg::OnToolsCommandfile)

	ON_BN_CLICKED(IDC_RADIO_CAN2USB, &CCANFLSAHTOOLDlg::OnBnClickedRadioCan2usb)
	ON_BN_CLICKED(IDC_RADIO_CANCARD2, &CCANFLSAHTOOLDlg::OnBnClickedRadioCancard2)
	ON_BN_CLICKED(IDC_RADIO_CANCARDXL, &CCANFLSAHTOOLDlg::OnBnClickedRadioCancardxl)
	//ON_MESSAGE(WM_MESSAGE_CANIN,  OnCANINMessage)
	//ON_MESSAGE(WM_MESSAGE_CANOUT, OnCANOUTMessage)
	ON_COMMAND(ID_PROJECT_EXIT, &CCANFLSAHTOOLDlg::OnProjectExit)
	ON_COMMAND(ID_HELP_ABOUT, &CCANFLSAHTOOLDlg::OnHelpAbout)
	ON_WM_SIZE()
	ON_MESSAGE(DM_GETDEFID,   OnGetDefID)
 	ON_COMMAND(ID_TOOLS_DISPLAYDATASTREAM, &CCANFLSAHTOOLDlg::OnToolsDisplaydatastream)
	 ON_MESSAGE(WM_KICKIDLE,OnKickIdle) 
	 ON_WM_CTLCOLOR()
	 ON_BN_CLICKED(IDC_CHECK_CLEARNV, &CCANFLSAHTOOLDlg::OnBnClickedCheckClearnv)
	 ON_BN_CLICKED(IDC_CHECK_READECU, &CCANFLSAHTOOLDlg::OnBnClickedCheckReadecu)
	 ON_BN_CLICKED(IDC_BUTTON_BOOT, &CCANFLSAHTOOLDlg::OnBnClickedButtonBoot)
	 ON_BN_CLICKED(IDC_BUTTON_MFG, &CCANFLSAHTOOLDlg::OnBnClickedButtonMfg)
	 ON_COMMAND(ID_TOOLS_BUILDCUTFILE, &CCANFLSAHTOOLDlg::OnToolsBuildcutfile)
	 ON_MESSAGE( WM_USER+1,onAddCANMsgToList)//触发
 	 ON_COMMAND_RANGE(ID_MENUSTART,ID_MENUEND,&CCANFLSAHTOOLDlg::OnMenuDeal)
	 ON_BN_CLICKED(IDC_RADIO_ICAN, &CCANFLSAHTOOLDlg::OnBnClickedRadioIcan)
	 ON_COMMAND(ID_PROJECT_SAVEPROJECTAS, &CCANFLSAHTOOLDlg::OnProjectSaveprojectas)
	 ON_BN_CLICKED(IDC_RADIO_ES581, &CCANFLSAHTOOLDlg::OnBnClickedRadioEs581)
	 ON_BN_CLICKED(IDC_BUTTON_BUILD, &CCANFLSAHTOOLDlg::OnBnClickedButtonBuild)
	 ON_BN_CLICKED(IDC_BUTTON_TSW, &CCANFLSAHTOOLDlg::OnBnClickedButtonTsw)
	 ON_CBN_SELCHANGE(IDC_COMBO_TSW, &CCANFLSAHTOOLDlg::OnSelchangeComboTsw)
END_MESSAGE_MAP()
LRESULT   CCANFLSAHTOOLDlg::OnGetDefID(WPARAM   wp,   LPARAM   lp)    
  {  
          return   MAKELONG(0,DC_HASDEFID);    
  }  
//LRESULT   CCANFLSAHTOOLDlg::OnCANINMessage(WPARAM wParam, LPARAM lParam)    
//{  
//	char *pdbcc_name;
//	char *pOpenDevPath;
//
//	pdbcc_name = (char *)wParam;
//	pOpenDevPath = _strlwr(OpenDevPath);
//	CAN2USB_findDevices(m_CanDevice, &m_NumDevices);
//	if(usb_flag==0)
//	{
// 	 
//	 if(strcmp(pdbcc_name,pOpenDevPath) == 0)openUSB();
//	 if(m_NumDevices > 0)showStatus(_T("CAN2USB Device find"),1);
//
//	}
//
//	return 0;  
//}
//LRESULT   CCANFLSAHTOOLDlg::OnCANOUTMessage(WPARAM wParam, LPARAM lParam)    
//{  
//	char *pdbcc_name;
//	char *pOpenDevPath;
//
//	pdbcc_name = (char *)wParam;
//	pOpenDevPath = _strlwr(OpenDevPath);
//	
//		CAN2USB_findDevices(m_CanDevice, &m_NumDevices);
// 	 //openUSB();
//		if(usb_flag==1||m_NumDevices==0)
//		{
//				
//		//if(strcmp(pdbcc_name,pOpenDevPath) == 0)
//		{
//			//isCanFlag=0;
//			g_usb2canFlag=0;
//	  showStatus(_T("CAN2USB Device close"),1);
//	   closeUSB();
//	   strcpy(OpenDevPath,pdbcc_name);
//		} 		
//		}
//	// showStatus(_T("CAN2USB Device close"),1);
//	return 0;  
//}
/*添加菜单项
#define ID_MENUSTART                    32784
#define ID_MENU0                        32785
#define ID_MENU1                        32786
#define ID_MENUEND                      32787
处理当前点击的菜单项的位置，来确定当前项目路径，由此来打开对于的project
*/
void CCANFLSAHTOOLDlg::OnMenuDeal(UINT nID)
{
	// TODO: 在此添加命令处理程序代码
	CMenu *pMenu = AfxGetMainWnd()->GetMenu();
	CMenu *pSubMenu = pMenu->GetSubMenu(0);
	CString PrgName;
	int ctrl=0;
	//根据命令找到子项
	pMenu->GetMenuString(nID,PrgName,MF_BYCOMMAND);

	for(int i=0;i<m_nMenuNum;i++)
	{
		if(PrgName.Find(m_szProLog[i])!=-1)
		{
			ctrl=i;
			break;
		}
	}
	if(ctrl>0)
	{
		for(int i=ctrl;i>0;i--)
		{
			m_szProLog[i]= m_szProLog[i-1];//[1]=[0]
		}
		m_szProLog[0]=PrgName;
 		for(int i=0;i<m_nMenuNum;i++)	
		{
			pSubMenu->DeleteMenu(6,MF_BYPOSITION);
		}
		ctrl=m_nMenuNum;
		m_nMenuNum=0;
		for(int i=ctrl-1;i>=0;i--)	
		{
			DisplayPrgToMenu(m_szProLog[i]);
		}
		
	}
	CString strTitle;
	strTitle = _T("CANFLASHTOOL V2.33[")+PrgName+_T("]");
	this->SetWindowText(strTitle);
 
 	 ReadProject(PrgName);

}
/*从文件$RecentProjectLog.hlh中读取添加菜单项*/
void CCANFLSAHTOOLDlg::ReadRecentProjectLog()
{
	// TODO: 在此添加命令处理程序代码
	CString strReadW;
	int sflag=0;
	int flag=0;
	m_szProLogName=m_szExePath+_T("$RecentProjectLog.hlh");

	if(csFile.Open(m_szProLogName,CFile::modeRead))
	{
		while(1)
		{
			if(csFile.ReadString(strReadW)!= NULL)
			{
				if(strReadW.Find(_T("[Begin]"))!=-1)
				{
					sflag=1;
					break;
				}
			}
			else break;
		}
		int n=0;
		while(sflag==1)
		{
			if(flag == 0)
			{
				if(csFile.ReadString(strReadW)== NULL)break;
			}
			flag=0;
			//ECU TYPE
			if(strReadW.Find(_T("[End]"))!=-1)
			{
				break;
 			}
			else
			{
				m_szProLog[n]=strReadW;
				//AddPrgToMenu(strReadW);
				n++;
			}
			if(n>3)break;
 		}
		int m=n-1;
		for(int i=0;i<n;i++)
		{
			DisplayPrgToMenu(m_szProLog[m--]);
		}
	}
	csFile.Close();
}
// CCANFLSAHTOOLDlg 消息处理程序
void	initInteractMsgQueue()//队列初始化
{
	interactMsgEvent =CreateEvent(NULL,FALSE,FALSE,NULL);//自动改变状态 初始为无信号
	interactMsgMutex =CreateMutex(NULL,FALSE,NULL);
	displayMsgMutex =CreateMutex(NULL,FALSE,NULL);
	
	interactMsgHead.next =NULL;
	interactMsgTail		 =NULL;
}
/*搜索软件运行所在目录下Seed&Key DLL文件夹里所有的*.dll文件*/
int CCANFLSAHTOOLDlg::FindFileName1(CString filepath)
{
    WIN32_FIND_DATA data;
    HANDLE hfind;
    char tempname[100] = {0};
    int i = 0;
	CString str;
	g_strdll[0].Empty();
    hfind = FindFirstFile(filepath,&data);
    while( hfind != INVALID_HANDLE_VALUE ) 
    { 
        memset(tempname,0,sizeof(tempname));
        
		g_strdll[i]=data.cFileName;
        if(  !FindNextFile(hfind,   &data)   ) 
        { 
            hfind = INVALID_HANDLE_VALUE; 
        } 
		//MessageBox( g_strdll[i]);
		i++;
		g_strdll[i].Empty();
		if(i>=50)break;
    }
    CloseHandle(hfind);
    return i;
}
 
BOOL CCANFLSAHTOOLDlg::OnInitDialog()
{
   CDialogEx::OnInitDialog();

   // 将“关于...”菜单项添加到系统菜单中。

   // IDM_ABOUTBOX 必须在系统命令范围内。
   ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
   ASSERT(IDM_ABOUTBOX < 0xF000);

   CMenu* pSysMenu = GetSystemMenu(FALSE);
   if (pSysMenu != NULL)
   {
      BOOL bNameValid;
      CString strAboutMenu;
      bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
      ASSERT(bNameValid);
      if (!strAboutMenu.IsEmpty())
      {
         pSysMenu->AppendMenu(MF_SEPARATOR);
         pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
      }
   } 
   // 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
   //  执行此操作
   SetIcon(m_hIcon, TRUE);			// 设置大图标
   SetIcon(m_hIcon, FALSE);		// 设置小图标
   // TODO: 在此添加额外的初始化代码
   strLog.Empty();
   m_pProjectEdit = new CProjectEdit();
   m_pProjectEdit->pParent = this;

   pOpFile = new COperateFile();

   m_pCANFlash = new CAN_Flash();
   m_pWnd =this;
   hWnd = m_hWnd;
   CANprotocol = new CAN_protocol();
   m_pCheckSet = new CActionSetting();
   m_Process.SetRange(0,100);
   m_Process.SetStep(10);      
   m_Process.SetPos(0); 
   setlocale(LC_CTYPE,("chs"));
   g_nKeyFlag=0;
   m_szConfigPath.Empty ();
   g_szKeyPath.Empty();

   ButtonStatus(false,0);	
   m_ComboPath.nCalNum=0;
   m_ComboPath.nCutNum=0;
   m_ComboPath.nPtpNum=0;
   m_ComboPath.nBootNum=0;
   m_ComboPath.nMfgNum=0;
   CheckDlgButton(IDC_RADIO_CAN2USB,BST_CHECKED); 
   m_nDongleType=0;	
#if USBDOG
	// int result =CollectUSBInfo();
 //  if(result ==0)
	//{
	//	MessageBox(_T("The software is not authorised"), _T("ERROR"),MB_ICONERROR|MB_OK);
	//	CDialog::OnCancel();
	//} 
	//m_dwSN=0;
#else
	 Dongle();
#endif
  
	
   writestr.Empty();
   //if(m_nDongleType != ENTERPRISE_VERSION)
   {
      GetDlgItem(IDC_BUTTON_BOOT)->ShowWindow(false);
      GetDlgItem(IDC_BUTTON_MFG)->ShowWindow(false);
      GetDlgItem(IDC_COMBO_BOOT)->ShowWindow(false);
      GetDlgItem(IDC_COMBO_MFG)->ShowWindow(false);
      GetDlgItem(IDC_STATIC_BOOT)->ShowWindow(false);
      GetDlgItem(IDC_STATIC_MFG)->ShowWindow(false);
      UpdateWindow();
   }

   WriteMsgMutex=CreateMutex(NULL,FALSE,NULL);
   initInteractMsgQueue();
   DisplayAllThread = ::CreateThread(NULL,
         NULL,
         (LPTHREAD_START_ROUTINE)CEcuFlashToolDlgDisplayAllThread,
         (LPVOID)this,
         NULL,
         NULL);
   CloseHandle(DisplayAllThread);
   TCHAR szPathOrig[300];
   GetModuleFileName(NULL, szPathOrig, 300);
   (_tcsrchr(szPathOrig,_T('\\')))[1]=0;
  
   	SYSTEMTIME pcTime;
	GetLocalTime(&pcTime);			 
	CString strtime;
	//strtime.Format(_T("data\\Log%04d%02d%02d\\CANFlashLog.csv"),pcTime.wYear,pcTime.wMonth,pcTime.wDay);
   strtime.Format(_T("Log%04d%02d\\"),pcTime.wYear,pcTime.wMonth);
	m_szExePath=szPathOrig;
   m_szlogDirectory=m_szExePath;
   m_szlogDirectory+=_T("data\\");
    if(GetFileAttributes(m_szlogDirectory)==-1)//this folder or file exist 
	{ 
		CreateDirectory(m_szlogDirectory,NULL); 
 	}
   m_szlogDirectory+=strtime;
   CString strdllpath;
	strdllpath=m_szExePath+_T("Seed&Key DLL\\*.dll");
    FindFileName1(strdllpath);

   if(GetFileAttributes(m_szlogDirectory)==-1)//this folder or file exist 
	{ 
		CreateDirectory(m_szlogDirectory,NULL); 
 	}
    m_szFilelog = m_szlogDirectory;
    m_szFilelog+="CANFlashLog.csv";
   m_nIcanStatus = m_pCANFlash->LoadIcan();
   m_ES581CAN = new CES581Function();                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                             
   m_nES581Status = m_ES581CAN->Load_icsneo40DLL();
   g_nKeyValue=0;
   m_bflag=0;
   g_usb2canFlag=0;
   ReadRecentProjectLog();
   if(m_bEnableStopFlag == 0)GetDlgItem(IDC_BUTTON_STOP)->ShowWindow(false);//EnableWindow(false);
  //CAN2USB_registerNotification((HWND)this->GetSafeHwnd(),
		//										WM_MESSAGE_CANIN,
		//										WM_MESSAGE_CANOUT,
		//										NULL);

     EnableToolTips(TRUE);//enable use it 
   m_ctrlTT.Create(this,TTS_BALLOON | TTS_ALWAYSTIP | TTS_CLOSE | TTS_NOFADE); 
   m_ctrlTT.Activate(TRUE); 
 //  m_ctrlTT.SetTipTextColor(RGB(255,0,192));//font color RGB( 255,0,192)
  
  
	m_ctrlTT.AddTool(GetDlgItem(IDC_COMBO_PRG),TTS_ALWAYSTIP);
	m_ctrlTT.SetTipTextColor( RGB( 255,0,192));
	m_ctrlTT.SetDelayTime(1000);//delay time 
	m_ctrlTT.Activate(true);
	CToolInfo ToolInfo;
	DisplayFileTypeSpecialState(false);

	/* CRect rc;   GetClientRect(&rc);
	m_ctrlTT.MoveWindow(rc,1);*/
   return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}
/*界面按钮的状态*/
void CCANFLSAHTOOLDlg::ButtonStatus(int state,int ctrl)
{
	GetDlgItem(IDC_BUTTON_CAL)->EnableWindow(state);
	GetDlgItem(IDC_BUTTON_CUT)->EnableWindow(state);
	GetDlgItem(IDC_BUTTON_APP)->EnableWindow(state);
	GetDlgItem(IDC_BUTTON_BOOT)->EnableWindow(state);
	GetDlgItem(IDC_BUTTON_MFG)->EnableWindow(state);
 	GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(state);
	GetDlgItem(IDC_BUTTON_FLASH)->EnableWindow(state);
}
/* 高级功能BOOT MFG隐藏。根据dog的级别*/
void CCANFLSAHTOOLDlg::HideStatus(int state,int ctrl)
{
 	GetDlgItem(IDC_BUTTON_BOOT)->EnableWindow(state);
	GetDlgItem(IDC_BUTTON_MFG)->EnableWindow(state);
  }

void CCANFLSAHTOOLDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CCANFLSAHTOOLDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}
/*定时器1：dog的扫描
        2：刷写过程中时间的显示
*/
void CCANFLSAHTOOLDlg::OnTimer(UINT_PTR nIDEvent)
{
   // TODO: 在此添加消息处理程序代码和/或调用默认值
   if(nIDEvent == 1)
   {
      bool flag;	 

      if(m_DongleUtility.CheckDllIntegrity())  
      {
         /*		ggdongle=m_nDongleType;
               if( m_nDongleType == 0)
               {
               flag = DelphiDongleCheck(ENTERPRISE_VERSION);

               ggdongle=flag;
               }
               else
               flag = DelphiDongleCheck(PROFESSIONAL_VERSION);*/
         flag = m_DongleUtility.DelphiDongleCheck((DONGLE_VERSION)m_nDongleType);

         for(int i=0;i<5&&flag==false;i++)
         {
            flag = m_DongleUtility.DelphiDongleCheck((DONGLE_VERSION)m_nDongleType);

         }
         if(flag == false)
         {
            KillTimer(1);
            MessageBox(_T("The software is not authorised"),_T("ERROR"),MB_ICONERROR|MB_OK);//
            HWND vhWnd=NULL;
            vhWnd=::FindWindow(NULL,_T("Open File"));//CFileDialog
            if(vhWnd != NULL)::SendMessage(vhWnd,WM_CLOSE,(WPARAM)(LPCTSTR)"",16); 

            vhWnd=::FindWindow(NULL,_T("Save File"));//CFileDialog
            if(vhWnd != NULL) ::SendMessage(vhWnd,WM_CLOSE,(WPARAM)(LPCTSTR)"",16); 

            vhWnd=::FindWindow(NULL,_T("Project Editor"));//CFileDialog
            if(vhWnd != NULL) ::SendMessage(vhWnd,WM_CLOSE,(WPARAM)(LPCTSTR)"",16); 

            vhWnd=::FindWindow(NULL,_T("Special Action Setting"));//CFileDialog
            if(vhWnd != NULL) ::SendMessage(vhWnd,WM_CLOSE,(WPARAM)(LPCTSTR)"",16); 

            vhWnd=::FindWindow(NULL,_T("Datastream"));//CFileDialog
            if(vhWnd != NULL) ::SendMessage(vhWnd,WM_CLOSE,(WPARAM)(LPCTSTR)"",16); 

            vhWnd=::FindWindow(NULL,_T("Begin to m_pCANFlash"));//CFileDialog
            if(vhWnd != NULL) ::SendMessage(vhWnd,WM_CLOSE,(WPARAM)(LPCTSTR)"",16); 

            vhWnd=::FindWindow(NULL,_T("Edit Special Check Item"));//CFileDialog
            if(vhWnd != NULL) ::SendMessage(vhWnd,WM_CLOSE,(WPARAM)(LPCTSTR)"",16); 

            vhWnd=::FindWindow(NULL,_T("About CAN FlashTool"));//CFileDialog
            if(vhWnd != NULL) ::SendMessage(vhWnd,WM_CLOSE,(WPARAM)(LPCTSTR)"",16); 

            vhWnd=::FindWindow(NULL,_T("ERROR"));//CFileDialog
            if(vhWnd != NULL) ::SendMessage(vhWnd,WM_CLOSE,(WPARAM)(LPCTSTR)"",16); 
            vhWnd=::FindWindow(NULL,_T("SAVE"));//CFileDialog
            if(vhWnd != NULL) 
               ::SendMessage(vhWnd,WM_CLOSE,(WPARAM)(LPCTSTR)"",16); 	
            OnCancel();	

         }  

      }
   }
   if(nIDEvent==2) //检测刷写过程
   {
      m_nflashtime++;
      CString str;
      str.Format(_T("%d"),m_nflashtime);		
      GetDlgItem(IDC_STATIC_TIMEPAST)->SetWindowTextW(str);
   }
   if(nIDEvent==3)  //检测“stop”按钮的触发
   {
      //暂停刷写过程
   }	
   CDialogEx::OnTimer(nIDEvent);
}
//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CCANFLSAHTOOLDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}	
//bool flag1,flag2,flag3,flag4;
/*初始化dog*/
void CCANFLSAHTOOLDlg::Dongle()
{	
   bool flag = false;

   if(m_DongleUtility.CheckDllIntegrity())  
   {  
      /* flag = DelphiDongleCheck(ENTERPRISE_VERSION);
         if(flag == false){
         MessageBox(_T("The software is not authorised"), NULL,MB_ICONERROR|MB_OK);
         CDialog::OnCancel();
         }*/
   //   for(int i =0;i<5&&flag==false;i++)
   //   {
   //      flag = m_DongleUtility.DelphiDongleCheck((DONGLE_VERSION)i);
   //      m_nDongleType=(DONGLE_VERSION)i;
		 //Sleep(2000);
   //      //if(flag == true)break;

   //   }
		int i=0;//ADV_PROFESSIONAL_VERSION; 
		flag = m_DongleUtility.DelphiDongleCheck((DONGLE_VERSION)i);
	/*  flag = m_DongleUtility.DelphiDongleCheck((DONGLE_VERSION)i);
 
      if(flag == false)ADV_PROFESSIONAL_VERSION
      {
		  Sleep(1000);
		  i=2;
		flag = m_DongleUtility.DelphiDongleCheck((DONGLE_VERSION)i);
	   

	  }*/

	  m_nDongleType=i;
 			//flag = m_DongleUtility.DelphiDongleCheck(EXPRESS_VERSION);
	   
 
	 //  flag4 = m_DongleUtility.DelphiDongleCheck((DONGLE_VERSION)i);
	 //  i++;

      if(flag == false)
      {
         MessageBox(_T("The software is not authorised"), _T("ERROR"),MB_ICONERROR|MB_OK);
         CDialog::OnCancel();
      }
      else 
      {
	     //m_nDongleType=ADV_PROFESSIONAL_VERSION;
         m_dwSN = m_DongleUtility.GetDelphiDongleSN();
      }


   }
   else  
   {  
      // DLL broken.
      MessageBox(_T("DLL ERROR."), _T("ERROR"),MB_ICONERROR|MB_OK);
      exit(1);
   }  
}
/*project信息的显示
ctrl：0对于新建的project 1对于edit project或read project
*/
void CCANFLSAHTOOLDlg::DisplayProject(int ctrl)
{

	if(config->ChecksumNum>0)
	{		
		DisplayActionItem();
	}
	else ItemInfor.ItemNum=0;

	GetDlgItem(IDC_COMBO_FILETYPE)->EnableWindow(true);

	m_bECUType.ResetContent();
	m_bECUType.AddString(config->ECU_Type);
	m_bECUType.SetCurSel(0);
	 
	/*buf=m_szConfigPath.GetBuffer(m_szConfigPath.GetLength());
	(_tcsrchr(strA,_T('\\')))[1]=0;*/
	m_bPRGPath.ResetContent();
	m_bPRGPath.AddString(m_szConfigPath);
	m_bPRGPath.SetCurSel(0);

	if(config->ECUprg.b_cal_app == MIXED)
	{
		m_bEcuFile.ResetContent();
		m_bEcuFile.AddString(_T("CUT/PTP"));
		m_bEcuFile.SetCurSel(0);
		g_EcuFile=CUT;
		
 	}
	else
	{
		//if(ctrl == 0)g_EcuFile=CUT;
		m_bEcuFile.ResetContent();
		
		//	if(config->ECUprg.funcFlag ==1)g_EcuFile=config->ECUprg.funcType;
	/*	if(config->ECUprg.b_boot == MIXED)
		{
			m_bEcuFile.AddString(_T("CUT"));
			m_bEcuFile.AddString(_T("CAL"));
			m_bEcuFile.AddString(_T("APP+CAL"));
			m_bEcuFile.SetItemData(0,1);
			m_bEcuFile.SetItemData(1,2);
			m_bEcuFile.SetItemData(2,3);
		}
		else*/
		{
			//if(config->ECUprg.funcFlag ==1)g_EcuFile=config->ECUprg.funcType;
			 
			if(config->ECUprg.funcType[0]==1)m_bEcuFile.AddString(_T("CUT"));
			if(config->ECUprg.funcType[1]==1)m_bEcuFile.AddString(_T("CAL"));
			if(config->ECUprg.funcType[2]==1)m_bEcuFile.AddString(_T("APP+CAL"));
			if(config->ECUprg.funcType[3]==1)m_bEcuFile.AddString(_T("BOOT"));
			if(config->ECUprg.funcType[4]==1)m_bEcuFile.AddString(_T("MFG"));
			if(config->ECUprg.funcType[5]==1)m_bEcuFile.AddString(_T("TSW"));
			//m_bEcuFile.AddString(_T("BOOT"));
			//m_bEcuFile.AddString(_T("MFG"));
			int ctrl=0;
			int flag=0;
			for(int i=0;i<FILETYPE_NUM;i++)
			{
				if(config->ECUprg.funcType[i]==1)
				{
					m_bEcuFile.SetItemData(i,i+1);
					if(g_EcuFile == i)
					{
						flag=1;
						m_bEcuFile.SetCurSel(ctrl);
					}
					ctrl++;
				}
				
			}
			if(flag==0)
			{
				CString str;
				m_bEcuFile.SetCurSel(0);//g_EcuFile
				m_bEcuFile.GetWindowTextW(str);
	
				if(str.Find(_T("CUT"))!=-1)g_EcuFile=0;
				else if(str.Find(_T("CAL"))==0)g_EcuFile=1;
				else if(str.Find(_T("APP+CAL"))!=-1)g_EcuFile=2;
				else if(str.Find(_T("BOOT"))!=-1)g_EcuFile=3;
				else if(str.Find(_T("MFG"))!=-1)g_EcuFile=4;
				else if(str.Find(_T("TSW"))!=-1)g_EcuFile=5;
 				//if(config->ECUprg.funcFlag==1)GetDlgItem(IDC_COMBO_FILETYPE)->EnableWindow(false);

			}
		 
			//m_bEcuFile.SetItemData(3,4);
			//m_bEcuFile.SetItemData(4,5);

		}
	}
 	DisplayFileTypeState();
 	UpdateData(false);
}
/* create project 
m_szFltPath：路径为空。做初始化
*/
void CCANFLSAHTOOLDlg::OnFileNewproject()
{
	// TODO: 在此添加命令处理程序代码
	int		index=0;
	m_bCurrentPro=0;
	int nResponse = m_pProjectEdit->DoModal(); 	
	if(nResponse == IDCANCEL){ return;}
	 m_szFltPath="";//当前的工程路径为空
	m_szConfigPath=m_pProjectEdit->m_szConfigPath; 
	m_szBeforePath=m_pProjectEdit->m_szBeforePath;
	m_szAfterPath=m_pProjectEdit->m_szAfterPath;
	m_bBeforeFlag=m_pProjectEdit->m_bBeforeFlag;
	m_bAfterFlag=m_pProjectEdit->m_bAfterFlag;
	m_bDefaultFlag=m_pProjectEdit->m_bDefaultFlag;
	g_bSetParamFlag=m_pProjectEdit->m_bSetParamFlag;
	m_bBypassFlag = m_pProjectEdit->m_bBypassFlag;
	g_nKeyFlag=m_pProjectEdit->m_nKeyFlag;
	g_szKeyPath=m_pProjectEdit->m_szKeyPath;
	m_bChecksumFlag=m_pProjectEdit->m_bChecksumFlag;
	m_bEnableStopFlag=m_pProjectEdit->m_bEnableStopFlag;
	m_ComboPath.nCalNum=0;
	m_ComboPath.nCutNum=0;
	m_ComboPath.nPtpNum=0;
	m_ComboPath.nBootNum=0;
	m_ComboPath.nMfgNum=0;
	m_ComboPath.nTswNum=0;
	if( g_nKeyFlag == 1)
	{
		index=0;
		g_nKeyValue=0;
 		while(index>=0)
		{
			BYTE  value=getHexByte(m_pProjectEdit->m_szKeyValue,&index);

			if(index>=0)
				g_nKeyValue =(g_nKeyValue<<8) | value;
			 
		}
 	}
	if( g_bSetParamFlag == 1)
	{
		index=0;
		g_nParamValue=0;
 		while(index>=0)
		{
			BYTE  value=getHexByte(m_pProjectEdit->m_szParamValue,&index);

			if(index>=0)
				g_nParamValue =(g_nParamValue<<8) | value;
			 
		}
 	}
	else g_nParamValue=1;
	 
	DisplayProject(0);
}

/*edit project */
void CCANFLSAHTOOLDlg::OnEditEditcurrentproject()
{
	// TODO: 在此添加命令处理程序代码
	int		index=0;
	m_bCurrentPro=1;
 
  	int nResponse = m_pProjectEdit->DoModal(); 	
	if(nResponse == IDCANCEL){ return;}
	m_szConfigPath=m_pProjectEdit->m_szConfigPath;	 
	m_szBeforePath=m_pProjectEdit->m_szBeforePath;
	m_szAfterPath=m_pProjectEdit->m_szAfterPath;
	m_bBeforeFlag=m_pProjectEdit->m_bBeforeFlag;
 	m_bAfterFlag=m_pProjectEdit->m_bAfterFlag;
	m_bDefaultFlag=m_pProjectEdit->m_bDefaultFlag;
	m_bBypassFlag = m_pProjectEdit->m_bBypassFlag;
	g_nKeyFlag=m_pProjectEdit->m_nKeyFlag;
	g_szKeyPath=m_pProjectEdit->m_szKeyPath;
	g_bSetParamFlag=m_pProjectEdit->m_bSetParamFlag;
	m_bChecksumFlag=m_pProjectEdit->m_bChecksumFlag;
	m_bEnableStopFlag=m_pProjectEdit->m_bEnableStopFlag;
	if(m_bEnableStopFlag == 1)GetDlgItem(IDC_BUTTON_STOP)->ShowWindow(true);//EnableWindow(false);
	else GetDlgItem(IDC_BUTTON_STOP)->ShowWindow(false);
	if( g_nKeyFlag == 1)
	{
		index=0;
		g_nKeyValue=0;
 		while(index>=0)
		{
			BYTE  value=getHexByte(m_pProjectEdit->m_szKeyValue,&index);

			if(index>=0)
				g_nKeyValue =(g_nKeyValue<<8) | value;
			 
		}
 	}
	if( g_bSetParamFlag == 1)
	{
		index=0;
		g_nParamValue=0;
 		while(index>=0)
		{
			BYTE  value=getHexByte(m_pProjectEdit->m_szParamValue,&index);

			if(index>=0)
				g_nParamValue =(g_nParamValue<<8) | value;
			 
		}
 	}
	else g_nParamValue=1;
	DisplayProject(1);
}
/*
添加新的文件路径，就最新文件最顶
*/
void CCANFLSAHTOOLDlg::AddComboFile(int type,CString str)
{
	int num;
	switch(type)
	{
	case PTP:
		if(m_ComboPath.nPtpNum < 10) 
			num = m_ComboPath.nPtpNum++;
		 else num= 9;
		m_ComboPtp.ResetContent();
		for(int i=num;i>0;i--)
		{
			m_ComboPath.szPtpBuf[i]= m_ComboPath.szPtpBuf[i-1];
		}
		m_ComboPath.szPtpBuf[0]=str;

		for(int j=0;j<=num;j++)
		{
			m_ComboPtp.AddString(m_ComboPath.szPtpBuf[j]);
			m_ComboPtp.SetItemData(j,j+1);
		}		
		m_ComboPtp.SetCurSel(0);
		break;
	case CAL:
		if(m_ComboPath.nCalNum < 10) 
			num = m_ComboPath.nCalNum++;
		 else num= 9;
		m_ComboCal.ResetContent();
		for(int i=num;i>0;i--)
		{
			m_ComboPath.szCalBuf[i]= m_ComboPath.szCalBuf[i-1];
		}
		m_ComboPath.szCalBuf[0]=str;

		for(int j=0;j<=num;j++)
		{
			m_ComboCal.AddString(m_ComboPath.szCalBuf[j]);
			m_ComboCal.SetItemData(j,j+1);
		}		
		m_ComboCal.SetCurSel(0);
		 
		break;
	case CUT:
		if(m_ComboPath.nCutNum < 10) 
			num = m_ComboPath.nCutNum++;
		 else num= 9;
		m_ComboCut.ResetContent();
		for(int i=num;i>0;i--)
		{
			m_ComboPath.szCutBuf[i]= m_ComboPath.szCutBuf[i-1];
		}
		m_ComboPath.szCutBuf[0]=str;

		for(int j=0;j<=num;j++)
		{
			m_ComboCut.AddString(m_ComboPath.szCutBuf[j]);
			m_ComboCut.SetItemData(j,j+1);
		}		
		m_ComboCut.SetCurSel(0);
 
		break;
	case BOOT:
		if(m_ComboPath.nBootNum < 10) 
			num = m_ComboPath.nBootNum++;
		 else num= 9;
		m_ComboBoot.ResetContent();
		for(int i=num;i>0;i--)
		{
			m_ComboPath.szBootBuf[i]= m_ComboPath.szBootBuf[i-1];
		}
		m_ComboPath.szBootBuf[0]=str;

		for(int j=0;j<=num;j++)
		{
			m_ComboBoot.AddString(m_ComboPath.szBootBuf[j]);
			m_ComboBoot.SetItemData(j,j+1);
		}		
		m_ComboBoot.SetCurSel(0);
 
		break;
	case MFG:
		if(m_ComboPath.nMfgNum < 10) 
			num = m_ComboPath.nMfgNum++;
		 else num= 9;
		m_ComboMfg.ResetContent();
		for(int i=num;i>0;i--)
		{
			m_ComboPath.szMfgBuf[i]= m_ComboPath.szMfgBuf[i-1];
		}
		m_ComboPath.szMfgBuf[0]=str;

		for(int j=0;j<=num;j++)
		{
			m_ComboMfg.AddString(m_ComboPath.szMfgBuf[j]);
			m_ComboMfg.SetItemData(j,j+1);
		}		
		m_ComboMfg.SetCurSel(0);
 
		break;
	case TSW:
		if(m_ComboPath.nTswNum < 10) 
			num = m_ComboPath.nTswNum++;
		 else num= 9;
		m_ComboTsw.ResetContent();
		for(int i=num;i>0;i--)
		{
			m_ComboPath.szTswBuf[i]= m_ComboPath.szTswBuf[i-1];
		}
		m_ComboPath.szTswBuf[0]=str;

		for(int j=0;j<=num;j++)
		{
			m_ComboTsw.AddString(m_ComboPath.szTswBuf[j]);
			m_ComboTsw.SetItemData(j,j+1);
		}		
		m_ComboTsw.SetCurSel(0);
		break;

	default:break;

	}
	 
 }
/*
打开一个app文件
*/
void CCANFLSAHTOOLDlg::OnBnClickedButtonApp()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);
	CWnd* pOldWnd = theApp.GetMainWnd();//theApp.GetMainWnd()->GetWindow  (GW_HWNDPREV),NULL
	//CWnd* handle=GetForegroundWindow();
	LPCTSTR Types;
	Types =
		_T("PTP File (*.S19;*.PTP)|*.S19;*.PTP|")
		_T("All Files (*.*)|*.*||");

	CFileDialog dlg(TRUE,_T("bin"),NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,Types,this,NULL);

	if(dlg.DoModal() == IDOK )
	{ 
		 // this->ShowWindow (SW_HIDE); 
 		 // SetForegroundWindow( );
		  // pOldWnd->ShowWindow (SW_SHOWNOACTIVATE);
//  OnPaint();
		m_szPtpPath = dlg.GetPathName();
		AddComboFile(PTP,m_szPtpPath);	
		GetDlgItem(IDC_BUTTON_BUILD)->EnableWindow(true);
		GetDlgItem(IDC_BUTTON_FLASH)->EnableWindow(false);

 	}
	 //SetForegroundWindow( );
	
	//pOldWnd->SetFocus();
}

/*
打开一个cal文件
*/
void CCANFLSAHTOOLDlg::OnBnClickedButtonCal()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);

	LPCTSTR Types;
	Types =
		_T("PTP File (*.S19;*.PTP)|*.S19;*.PTP|")
		_T("All Files (*.*)|*.*||");
	CFileDialog dlg(TRUE,_T("bin"),NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,Types);
	dlg.m_ofn.lpstrTitle =_T("Open File");
	if(dlg.DoModal() == IDOK )
	{  
 		m_szCalPath = dlg.GetPathName();
		AddComboFile(CAL,m_szCalPath);
		if(g_EcuFile == PTP_CAL)
		{
			GetDlgItem(IDC_BUTTON_BUILD)->EnableWindow(true);
			GetDlgItem(IDC_BUTTON_FLASH)->EnableWindow(false);
		}
 	}
	 
	this->SetFocus();
}

/*
打开一个Cut文件
*/
void CCANFLSAHTOOLDlg::OnBnClickedButtonCut()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);

	LPCTSTR Types;
	Types =
		_T("PTP File (*.CUT;*.PTP;*.S19)|*.CUT;*.PTP;*.S19|")
		_T("All Files (*.*)|*.*||");
	CFileDialog dlg(TRUE,_T("bin"),NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,Types);
	dlg.m_ofn.lpstrTitle =_T("Open File");
	if(dlg.DoModal() == IDOK )
	{  
 		m_szCutPath = dlg.GetPathName();
 		AddComboFile(CUT,m_szCutPath);
	
 	}
	SetFocus();
}
void CCANFLSAHTOOLDlg::DisplayFileTypeSpecialState(bool flag)
{
 		GetDlgItem(IDC_COMBO_BOOT)->EnableWindow(flag);
		GetDlgItem(IDC_BUTTON_BOOT)->EnableWindow(flag);
		GetDlgItem(IDC_COMBO_BOOT)->ShowWindow(flag);
		GetDlgItem(IDC_BUTTON_BOOT)->ShowWindow(flag);
		GetDlgItem(IDC_STATIC_BOOT)->ShowWindow(flag);
		GetDlgItem(IDC_STATIC_BOOT)->EnableWindow(flag);


		GetDlgItem(IDC_COMBO_MFG)->EnableWindow(flag);
		GetDlgItem(IDC_COMBO_MFG)->ShowWindow(flag);
		GetDlgItem(IDC_BUTTON_MFG)->EnableWindow(flag);		 
		GetDlgItem(IDC_BUTTON_MFG)->ShowWindow(flag);
		GetDlgItem(IDC_STATIC_MFG)->ShowWindow(flag);
		GetDlgItem(IDC_STATIC_MFG)->EnableWindow(flag);

		GetDlgItem(IDC_COMBO_TSW)->EnableWindow(flag);
		GetDlgItem(IDC_COMBO_TSW)->ShowWindow(flag);
		GetDlgItem(IDC_BUTTON_TSW)->EnableWindow(flag);		 
		GetDlgItem(IDC_BUTTON_TSW)->ShowWindow(flag);
		GetDlgItem(IDC_STATIC_TSW)->ShowWindow(flag);
		GetDlgItem(IDC_STATIC_TSW)->EnableWindow(flag);


}
/*
根据文件类型不同，界面状态不同
*/
void CCANFLSAHTOOLDlg::DisplayFileTypeState()
{
	GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(true);
	 
	switch(g_EcuFile)
	{
		DisplayFileTypeSpecialState(false);
		case CUT:
			GetDlgItem(IDC_COMBO_CAL)->EnableWindow(false);
			GetDlgItem(IDC_COMBO_PTP)->EnableWindow(false);
			GetDlgItem(IDC_BUTTON_APP)->EnableWindow(false);
			GetDlgItem(IDC_BUTTON_CAL)->EnableWindow(false);

			GetDlgItem(IDC_COMBO_CUT)->ShowWindow(true);
			GetDlgItem(IDC_BUTTON_CUT)->ShowWindow(true);
			GetDlgItem(IDC_COMBO_CUT)->EnableWindow(true);
			GetDlgItem(IDC_BUTTON_CUT)->EnableWindow(true);
			GetDlgItem(IDC_COMBO_CUT)->EnableWindow(true);
			GetDlgItem(IDC_STATIC_CUT)->ShowWindow(true);
			GetDlgItem(IDC_STATIC_CUT)->EnableWindow(true);

 			GetDlgItem(IDC_BUTTON_FLASH)->EnableWindow(true);
			//GetDlgItem(IDC_COMBO_BOOT)->EnableWindow(false);
			//GetDlgItem(IDC_BUTTON_BOOT)->EnableWindow(false);
			//GetDlgItem(IDC_COMBO_MFG)->EnableWindow(false);
			//GetDlgItem(IDC_BUTTON_MFG)->EnableWindow(false);
			GetDlgItem(IDC_BUTTON_BUILD)->EnableWindow(false);
			DisplayFileTypeSpecialState(false);

			break;
		case CAL:
			GetDlgItem(IDC_COMBO_CAL)->EnableWindow(true);
			GetDlgItem(IDC_COMBO_PTP)->EnableWindow(false);
			GetDlgItem(IDC_COMBO_CUT)->EnableWindow(false);
			GetDlgItem(IDC_BUTTON_APP)->EnableWindow(false);
			GetDlgItem(IDC_BUTTON_CUT)->EnableWindow(false);
			GetDlgItem(IDC_BUTTON_CAL)->EnableWindow(true);
			GetDlgItem(IDC_BUTTON_FLASH)->EnableWindow(true);
 			GetDlgItem(IDC_COMBO_BOOT)->EnableWindow(false);
			GetDlgItem(IDC_BUTTON_BOOT)->EnableWindow(false);
			GetDlgItem(IDC_COMBO_MFG)->EnableWindow(false);
			GetDlgItem(IDC_BUTTON_MFG)->EnableWindow(false);
			GetDlgItem(IDC_BUTTON_BUILD)->EnableWindow(false);
			break;
		case PTP_CAL:
			GetDlgItem(IDC_BUTTON_CAL)->EnableWindow(true);
			GetDlgItem(IDC_COMBO_CAL)->EnableWindow(true);
			GetDlgItem(IDC_COMBO_PTP)->EnableWindow(true);
			GetDlgItem(IDC_COMBO_CUT)->EnableWindow(false);
			GetDlgItem(IDC_BUTTON_APP)->EnableWindow(true);
			GetDlgItem(IDC_BUTTON_CUT)->EnableWindow(false);
 			GetDlgItem(IDC_BUTTON_FLASH)->EnableWindow(false);
 			GetDlgItem(IDC_COMBO_BOOT)->EnableWindow(false);
			GetDlgItem(IDC_BUTTON_BOOT)->EnableWindow(false);
			GetDlgItem(IDC_COMBO_MFG)->EnableWindow(false);
			GetDlgItem(IDC_BUTTON_MFG)->EnableWindow(false);
			GetDlgItem(IDC_BUTTON_BUILD)->EnableWindow(true);
			break;
		case BOOT:
			GetDlgItem(IDC_BUTTON_CAL)->EnableWindow(false);
			GetDlgItem(IDC_COMBO_CAL)->EnableWindow(false);
			GetDlgItem(IDC_COMBO_PTP)->EnableWindow(false);

			GetDlgItem(IDC_COMBO_CUT)->ShowWindow(false);
			GetDlgItem(IDC_BUTTON_CUT)->ShowWindow(false);
			GetDlgItem(IDC_STATIC_CUT)->ShowWindow(false);

			GetDlgItem(IDC_BUTTON_APP)->EnableWindow(false);//
 			GetDlgItem(IDC_BUTTON_FLASH)->EnableWindow(true);
			DisplayFileTypeSpecialState(false);

			//IDC_STATIC_MFG IDC_STATIC_CUT
 			GetDlgItem(IDC_COMBO_BOOT)->ShowWindow(true);
			GetDlgItem(IDC_BUTTON_BOOT)->ShowWindow(true);
 			GetDlgItem(IDC_COMBO_BOOT)->EnableWindow(true);
			GetDlgItem(IDC_BUTTON_BOOT)->EnableWindow(true);
			GetDlgItem(IDC_STATIC_BOOT)->ShowWindow(true);
			GetDlgItem(IDC_STATIC_BOOT)->EnableWindow(true);

 			GetDlgItem(IDC_BUTTON_BUILD)->EnableWindow(false);
			break;
		case MFG:
			GetDlgItem(IDC_BUTTON_CAL)->EnableWindow(false);
			GetDlgItem(IDC_COMBO_CAL)->EnableWindow(false);
			GetDlgItem(IDC_COMBO_PTP)->EnableWindow(false);

			GetDlgItem(IDC_COMBO_CUT)->ShowWindow(false);
			GetDlgItem(IDC_BUTTON_CUT)->ShowWindow(false);
			GetDlgItem(IDC_STATIC_CUT)->ShowWindow(false);

			GetDlgItem(IDC_BUTTON_APP)->EnableWindow(false);//
 			GetDlgItem(IDC_BUTTON_FLASH)->EnableWindow(true);
 			GetDlgItem(IDC_BUTTON_BUILD)->EnableWindow(false);
			DisplayFileTypeSpecialState(false);

			//IDC_STATIC_MFG IDC_STATIC_CUT
 			GetDlgItem(IDC_COMBO_MFG)->ShowWindow(true);
			GetDlgItem(IDC_BUTTON_MFG)->ShowWindow(true);
 			GetDlgItem(IDC_COMBO_MFG)->EnableWindow(true);
			GetDlgItem(IDC_BUTTON_MFG)->EnableWindow(true);
			GetDlgItem(IDC_STATIC_MFG)->ShowWindow(true);
			GetDlgItem(IDC_STATIC_MFG)->EnableWindow(true);

			break;
		case TSW:
			GetDlgItem(IDC_BUTTON_CAL)->EnableWindow(false);
			GetDlgItem(IDC_COMBO_CAL)->EnableWindow(false);
			GetDlgItem(IDC_COMBO_PTP)->EnableWindow(false);

			GetDlgItem(IDC_COMBO_CUT)->ShowWindow(false);
			GetDlgItem(IDC_BUTTON_CUT)->ShowWindow(false);
			GetDlgItem(IDC_STATIC_CUT)->ShowWindow(false);
 			GetDlgItem(IDC_BUTTON_BUILD)->EnableWindow(false);
			GetDlgItem(IDC_BUTTON_APP)->EnableWindow(false);//
 			GetDlgItem(IDC_BUTTON_FLASH)->EnableWindow(true);
			DisplayFileTypeSpecialState(false);

			//IDC_STATIC_MFG IDC_STATIC_CUT
 			GetDlgItem(IDC_COMBO_TSW)->ShowWindow(true);
			GetDlgItem(IDC_BUTTON_TSW)->ShowWindow(true);
 			GetDlgItem(IDC_COMBO_TSW)->EnableWindow(true);
			GetDlgItem(IDC_BUTTON_TSW)->EnableWindow(true);
			GetDlgItem(IDC_STATIC_TSW)->ShowWindow(true);
			GetDlgItem(IDC_STATIC_TSW)->EnableWindow(true);
			break;
		default:break;
		}
	 
}

/*
g_EcuFile表示刷写文件的类型
*/
void CCANFLSAHTOOLDlg::OnSelchangeComboFiletype()
{
	// TODO: 在此添加控件通知处理程序代码

	UpdateData(false);
	//g_EcuFile = m_bEcuFile.GetCurSel();
	CString str;
	m_bEcuFile.GetWindowTextW(str);
	if(str.Find(_T("CUT"))!=-1)g_EcuFile=0;
	else if(str.Find(_T("CAL"))==0)g_EcuFile=1;
	else if(str.Find(_T("APP+CAL"))!=-1)g_EcuFile=2;
	else if(str.Find(_T("BOOT"))!=-1)g_EcuFile=3;
	else if(str.Find(_T("MFG"))!=-1)g_EcuFile=4;
	else if(str.Find(_T("TSW"))!=-1)g_EcuFile=5;
 	//MessageBox(str);
 	GetDlgItem(IDC_BUTTON_FLASH)->EnableWindow(true);
	DisplayFileTypeState();
}
 
/*
下拉菜单选择文件。需要把选择的文件置顶处理
*/ 
void CCANFLSAHTOOLDlg::OnSelchangeComboPTP()
{
	// TODO: 在此添加控件通知处理程序代码
	int ctrl;
	UpdateData(false);
	ctrl = m_ComboPtp.GetCurSel();
 	m_szPtpPath = m_ComboPath.szPtpBuf[ctrl];
	for(int i=ctrl;i>0;i--)
	{
		m_ComboPath.szPtpBuf[i]= m_ComboPath.szPtpBuf[i-1];
	}
	m_ComboPath.szPtpBuf[0]=m_szPtpPath;

 	m_ComboPtp.ResetContent();
 	 
	for(int j=0;j<m_ComboPath.nPtpNum;j++)
	{
		m_ComboPtp.AddString(m_ComboPath.szPtpBuf[j]);
		m_ComboPtp.SetItemData(j,j+1);
	}		
	m_ComboPtp.SetCurSel(0);
 		GetDlgItem(IDC_BUTTON_BUILD)->EnableWindow(true);
		GetDlgItem(IDC_BUTTON_FLASH)->EnableWindow(false);

}
/* 
复选框选择文件时，选中的文件置顶 
*/
void CCANFLSAHTOOLDlg::OnSelchangeComboCAL()
{
	// TODO: 在此添加控件通知处理程序代码

	UpdateData(false);
	int ctrl = m_ComboCal.GetCurSel();
	m_szCalPath = m_ComboPath.szCalBuf[ctrl];
 
		for(int i=ctrl;i>0;i--)
		{
			m_ComboPath.szCalBuf[i]= m_ComboPath.szCalBuf[i-1];
		}
		 
		m_ComboPath.szCalBuf[0]=m_szCalPath;
 	m_ComboCal.ResetContent();
 	 
	for(int j=0;j<m_ComboPath.nCalNum;j++)
	{
		m_ComboCal.AddString(m_ComboPath.szCalBuf[j]);
		m_ComboCal.SetItemData(j,j+1);
	}		
	m_ComboCal.SetCurSel(0);
	if(g_EcuFile == PTP_CAL)
	{
		GetDlgItem(IDC_BUTTON_BUILD)->EnableWindow(true);
		GetDlgItem(IDC_BUTTON_FLASH)->EnableWindow(false);
	}
		 
}
/* 
复选框选择文件时，选中的文件置顶 
*/
void CCANFLSAHTOOLDlg::OnSelchangeComboCUT()
{
	// TODO: 在此添加控件通知处理程序代码

	UpdateData(false);
	int ctrl = m_ComboCut.GetCurSel();
	m_szCutPath = m_ComboPath.szCutBuf[ctrl];
	for(int i=ctrl;i>0;i--)
		{
			m_ComboPath.szCutBuf[i]= m_ComboPath.szCutBuf[i-1];
		}
		m_ComboPath.szCutBuf[0]=m_szCutPath;
 	m_ComboCut.ResetContent();
 	 
	for(int j=0;j<m_ComboPath.nCutNum;j++)
	{
		m_ComboCut.AddString(m_ComboPath.szCutBuf[j]);
		m_ComboCut.SetItemData(j,j+1);
	}		
	m_ComboCut.SetCurSel(0);
}
/* 
复选框选择文件时，选中的文件置顶 
*/
void CCANFLSAHTOOLDlg::OnSelchangeComboBOOT()
{
	// TODO: 在此添加控件通知处理程序代码


	UpdateData(false);
	int ctrl = m_ComboBoot.GetCurSel();
	m_szBootPath = m_ComboPath.szBootBuf[ctrl];
	for(int i=ctrl;i>0;i--)
		{
			m_ComboPath.szBootBuf[i]= m_ComboPath.szBootBuf[i-1];
		}
		m_ComboPath.szBootBuf[0]=m_szBootPath;
		m_ComboBoot.ResetContent();
 	 
		for(int j=0;j<m_ComboPath.nBootNum;j++)
	{
		m_ComboBoot.AddString(m_ComboPath.szBootBuf[j]);
		m_ComboBoot.SetItemData(j,j+1);
	}		
	m_ComboBoot.SetCurSel(0);
 
}
/* 
复选框选择文件时，选中的文件置顶 
*/
void CCANFLSAHTOOLDlg::OnSelchangeComboMFG()
{
	// TODO: 在此添加控件通知处理程序代码

	UpdateData(false);
	int ctrl = m_ComboMfg.GetCurSel();
	m_szMfgPath = m_ComboPath.szMfgBuf[ctrl];
	for(int i=ctrl;i>0;i--)
		{
			m_ComboPath.szMfgBuf[i]= m_ComboPath.szMfgBuf[i-1];
		}
		m_ComboPath.szMfgBuf[0]=m_szMfgPath;
		m_ComboMfg.ResetContent();
 	 
		for(int j=0;j<m_ComboPath.nMfgNum;j++)
	{
		m_ComboMfg.AddString(m_ComboPath.szMfgBuf[j]);
		m_ComboMfg.SetItemData(j,j+1);
	}		
	m_ComboMfg.SetCurSel(0);
}
/*
f:数据存取的文件
buf[]:源数据
size:源数据的长度
add:起始地址
addlen:地址的长度
*/
void CCANFLSAHTOOLDlg::CombinationCUT(unsigned char buf[],unsigned int size,unsigned int add)
{CString axx;
	int linenum=size/16;
	CString stradd;
 	unsigned char cksum=0; 
	CString strdata;
	CString strs;
	CString data;
	int reusult;
	int addlen;
	char buffer[100];
	if(add>0xFFFFFF){strs="S315";addlen=3;}
	else if(add>0xFFFF){strs="S214";addlen=2;}
	else if(add>0xFF){strs="S113";addlen=1;}
	else {strs="S014";addlen=0;}
	//strs="S315";addlen=3;
 	for(int i=0;i<linenum;i++)
	{
		CString strline;
		cksum=0x12+addlen; 
		strline.Empty();
		strline =strs.Left(4);
		
		if(add>0xFFFFFF){ 
			stradd.Format(_T("%08X"),add);}
		else if(add>0xFFFF){
			stradd.Format(_T("%06X"),add);		
		}
		else if(add>0xFF){
			stradd.Format(_T("%04X"),add);}
		//stradd.Format(_T("%08X"),add);	
		strline += stradd;

		for(int k=0;k<4;k++)
		{
			cksum += (add>>k*8)&0xff;
		}
		for(int j=0;j<16;j++)
		{
			strdata.Format(_T("%02X"),buf[i*16+j]);
			strline += strdata;
			cksum += buf[i*16+j];
		}		
		cksum = 0xff - cksum&0xff;
		add+=0x10;
		strdata.Format(_T("%02X"),cksum);
		strline+=strdata;
		strline+="\r\n";
		CStringA strlineA(strline);
 	 
	strcpy_s((char *)buffer,100,strlineA);
	csFilecut.Write(buffer,strline.GetLength());
	}
}
/* 
数字ASCII码转换为数值 
*/
BYTE CCANFLSAHTOOLDlg::getSize(CString& str,U32 *index)
{
	U32	value=0;
	int		i,j=0;
	int	len =str.GetLength();
	
	for(i=0;i<len;i++)
	{
		if(isxdigit(str[i]))
		{
			if(str[i]<='9')
				value =value*10 + (str[i]-'0');
		}
	}
	*index =value;
	return 1;
}
/*mt2x系列，有kksum,kkpgmio,security显示当前值*/
int CCANFLSAHTOOLDlg::DisplayActionItemCurrentValue()
{
	CString str;	 
	int i=0;
	int index=0;
 	unsigned char buf[100];
	u32 bAddress;
	u32 bSize;
	m_bMergeFlag=0;

	for(i=0;i<ItemInfor.ItemNum;i++)
	{
 			CStringA strA(ItemInfor.ActionItem[i].Address);
			strcpy_s((char *)buf,100,strA);
			config->HextoAdd((char *)buf,&bAddress);
			index=0;
			for(int j=0;j<config->banknum;j+=2)
			{
				if(bAddress >= config->ECUprg.Bankaddr[j]&& bAddress <= config->ECUprg.Bankaddr[j+1])						
				{
					index += bAddress - config->ECUprg.Bankaddr[j];
					break;
				}
				else if(bAddress >= config->ECUprg.Bankaddr[j]&& bAddress >= config->ECUprg.Bankaddr[j+1])//gai
				{
					index += config->ECUprg.Bankaddr[j+1]-config->ECUprg.Bankaddr[j]+1;

				}
			}
			
			int bNum=0;
			getSize(ItemInfor.ActionItem[i].Size,&bSize);		 
 			memcpy(cut_data,&Calibration.totalTransData.FileData[index],bSize); 
		
 			cut_data[bSize]=0;
			str.Empty();
			ItemInfor.ActionItem[i].CurrentValue.Empty();
			CString strValue;
			for(unsigned int n=0;n<bSize;n++)
			{
				strValue.Format(_T("%02X"),(unsigned char)cut_data[n]);
				ItemInfor.ActionItem[i].CurrentValue += strValue;
 			}
 	}	
		return index;
 
}
//***************************************************
//函数名：CheckFlashItems（）
//功能  ：在进行m_pCANFlash刷写前，判断是否准备完全
//        (Ecu_Type   EcuFile  Configuration Open...）
//参数  : 无
//返回值：bool类型
//***************************************************
bool CCANFLSAHTOOLDlg::CheckFlashItems()
{
 	switch(g_EcuFile)
	{
	case CUT:
		if(m_szCutPath == "")
		{
			MessageBox(_T("Cut File have not been selected!!"),_T("ERROR"));
			return false;
		}
		break;
	case CAL:
		if(m_szCalPath == "")
		{
			MessageBox(_T("Cal File have not been selected!!"),_T("ERROR"));
			return false;
		}
		break;
	case PTP_CAL:
		if(m_szCalPath == "")
		{
			MessageBox(_T("Cal File have not been selected!!"),_T("ERROR"));
			return false;
		}
		if(m_szPtpPath == "")
		{
			MessageBox(_T("Ptp File hava not been selected!!"),_T("ERROR"));
			return false;
		}
		break;
	case BOOT:
		if(m_szBootPath == "")
		{
			MessageBox(_T("Boot File have not been selected!!"),_T("ERROR"));
			return false;
		}
		break;
	case MFG:
		if(m_szMfgPath == "")
		{
			MessageBox(_T("Mfg File have not been selected!!"),_T("ERROR"));
			return false;
		}
		break;
	default:break;
	}
	return true;

}
/*
生成cut文件，分为MIXED（cut），separated（cut，app，cal）。
*/
DWORD WINAPI CCANFLSAHTOOLDlg:: CCANFLSAHTOOLDlgBuildCutThreadThread(LPVOID lpParameter)
{
	CCANFLSAHTOOLDlg	*dlg=(CCANFLSAHTOOLDlg*)lpParameter;
	CString str;
 	int i, length;
  	int result; 
	u32 add[2];
 
	bool res=dlg->csFilecut.Open(dlg->m_szBuildPath,CFile::modeWrite | CFile::modeCreate| CFile::typeBinary);
 
	 if(res == 0)//dlg->m_szBuildPath
	 {
 		CString str_err;
		str_err.Format(_T("errorcode:%d\r\n"),res);
		str="build cut failed:";
		str+=str_err;
		str += dlg->m_szBuildPath;
		str += " was not opened.\r\n";
 		dlg->m_pCANFlash->display(str,1);
		
 		if(g_EcuFile == PTP_CAL)
		{
			dlg->GetDlgItem(IDC_BUTTON_BUILD)->EnableWindow(true);
			dlg->GetDlgItem(IDC_BUTTON_FLASH)->EnableWindow(false);
		}
		else
 		dlg->GetDlgItem(IDC_BUTTON_FLASH)->EnableWindow(true);
		return 1;
	}
	char len;
	unsigned int size;
	unsigned int addr=0;
	int flag=0;
	int index;
	unsigned calSum;
 	g_strArray.RemoveAll();
 
	switch(config->ECUprg.b_cal_app)
	{
		case MIXED:
 				dlg->csFilecut.Write(Calibration.S0_Data,strlen(Calibration.S0_Data));
				for(int i=0;i<config->banknum;i+=2)
				{				
					size=config->ECUprg.Bankaddr[i+1]-config->ECUprg.Bankaddr[i]+1;
					dlg->CombinationCUT(&Calibration.totalTransData.FileData[addr],size,config->ECUprg.Bankaddr[i]);
					addr+=size;
				}
				if(Calibration.S7_Flag==1)dlg->csFilecut.Write(Calibration.S7_Data,strlen(Calibration.S7_Data));
			 break;
			default: //mt80  cal，app一个地址范围			 
 				for(int j=0;j<config->ChecksumNum;j++)//校验值的个数
				{
					config->m_checksum[j].Checksum=0;
					for(int m=0;m<config->WordsumNum[0];m+=2)
					{
						for(int i=0;i<config->banknum;i+=2)
						{
							add[0]=config->ECUprg.Bankaddr[i];
							add[1]=config->ECUprg.Bankaddr[i+1];
							if(config->ECUprg.Wordsumaddr[j][m] >=add[0]&&config->ECUprg.Wordsumaddr[j][m]<=add[1])
							{
								if(g_EcuFile == CUT)	
									pOpFile->ReadDataFromFileTobuf_buildcut(m_szCutPath,add,0);
								else
								{
									pOpFile->ReadDataFromFileTobuf_buildcut(m_szPtpPath,add,0);
									pOpFile->ReadDataFromFileTobuf_buildcut(m_szCalPath,add,1);
								}
								size=config->ECUprg.Bankaddr[i+1]-config->ECUprg.Bankaddr[i]+1;
 
 								dlg->CombinationCUTtoCString(Calibration.totalTransData.FileData,size,config->ECUprg.Bankaddr[i]);
  								pOpFile->ReadDataFromCStringArrayTobuf_Checksum(add,1,j,1,&calSum);
								//index = config->ECUprg.ChecksumAddr[j] -add[0];
 								config->m_checksum[j].Checksum += calSum;
							}
 
						}
					}
					index = config->m_checksum[j].ChecksumAddr-add[0];
					config->Checksum = 0xffff-config->m_checksum[j].Checksum+1;
					config->m_checksum[j].Checksum=config->Checksum;
					config->m_checksum[j].FileChecksum=config->FileChecksum;

				}
				//addr+=size;
				//if(Routine.S7_Flag==1||Calibration.S7_Flag==1)fputs(Routine.S7_Data,f);

					 
			for(int i=0;i<config->banknum;i+=2)
			{	
				add[0]=config->ECUprg.Bankaddr[i];
				add[1]=config->ECUprg.Bankaddr[i+1];
				if(g_EcuFile == CUT)	
					pOpFile->ReadDataFromFileTobuf_buildcut(m_szCutPath,add,0);
				else
				{
					pOpFile->ReadDataFromFileTobuf_buildcut(m_szPtpPath,add,0);
					pOpFile->ReadDataFromFileTobuf_buildcut(m_szCalPath,add,1);
				}
					 

			size=config->ECUprg.Bankaddr[i+1]-config->ECUprg.Bankaddr[i]+1;
			if(flag==0)
			{
				dlg->csFilecut.Write(Calibration.S0_Data,strlen(Calibration.S0_Data));
				flag=1;
			}
			for(int j=0;j<config->ChecksumNum;j++)
			{
				if(config->ECUprg.ChecksumAddr[j] >=add[0]&& config->ECUprg.ChecksumAddr[j]<=add[1])
				{
					index = config->m_checksum[j].ChecksumAddr-add[0];
					if(config->ECUprg.ChecksumOrder[j] == 1)//大端
					{
						Calibration.totalTransData.FileData[index]=(config->m_checksum[j].Checksum>>8)&0xff; 
						Calibration.totalTransData.FileData[index+1]=config->m_checksum[j].Checksum&0xff; 
					}
					else
					{
						Calibration.totalTransData.FileData[index]=config->m_checksum[j].Checksum&0xff; 
						Calibration.totalTransData.FileData[index+1]=(config->m_checksum[j].Checksum>>8)&0xff; 

					}

				}
			}
 			dlg->CombinationCUT(Calibration.totalTransData.FileData,size,config->ECUprg.Bankaddr[i]);
			} 
 		if(Calibration.S7_Flag==1)dlg->csFilecut.Write(Calibration.S7_Data,strlen(Calibration.S7_Data));
					
		break;
		}
		//fclose(f);
		dlg->csFilecut.Close();
 		str = "Building CUT file...OK.\r\n";
		dlg->m_pCANFlash->display(str,0);
 		if(g_EcuFile == PTP_CAL)
		{
			dlg->GetDlgItem(IDC_BUTTON_BUILD)->EnableWindow(true);
			dlg->GetDlgItem(IDC_BUTTON_FLASH)->EnableWindow(false);
		}
		else
		dlg->GetDlgItem(IDC_BUTTON_FLASH)->EnableWindow(true);

	 return 1;
}
/*状态栏的显示*/
void CCANFLSAHTOOLDlg::showStatus(CString statusStr,BYTE addFlag)
{
	CEdit *statusEdit = (CEdit*)GetDlgItem(IDC_STATIC_STATUS);
	statusEdit->SetWindowTextW(statusStr);
 
}
//void MyCAN2USB_callBback(PDEV_BROADCAST_DEVICEINTERFACE pDevInf,WPARAM wParam,LPVOID lpParameter)
//{
//	CCANFLSAHTOOLDlg *pDlg = (CCANFLSAHTOOLDlg *)lpParameter;
//	CAN2USB_findDevices(pDlg->m_CanDevice, &pDlg->m_NumDevices);
//
//	char *pdbcc_name;
//	char *pOpenDevPath;
//
//	//pdbcc_name = _strlwr(pDevInf->dbcc_name);
//	//pOpenDevPath = _strlwr(pDlg->OpenDevPath);
//
//	switch(wParam)
//	{
//	case DBT_DEVICEARRIVAL:
//		//pDlg->InitInstanceCanList();
//		//if(pDlg->DevState == DEV_CLOSED)
//		//{
//		//	if(strcmp(pdbcc_name,pOpenDevPath) == 0)
//		//	{
//		//		//pDlg->openUSB();
//		//		pDlg->SendMessage( WM_MYMESSAGE,1,NULL); 
//		//	}
//		//}
//	break;
//	case DBT_DEVICEREMOVECOMPLETE:
//		//pDlg->InitInstanceCanList();
//		//if(pDlg->DevState == DEV_OPENED)
//		//{
//		//	if(strcmp(pdbcc_name,pOpenDevPath) == 0)
//		//	{
//		//		//pDlg->closeUSB();
//		//		
//		//		pDlg->SendMessage( WM_MYMESSAGE,2,NULL); 
//		//		strcpy(pDlg->OpenDevPath,pDevInf->dbcc_name);
//		//	}			
//		//}
//
//	break;
//	default:
//	break;
//	}
//}
/*打开can卡：CANtoUSB*/
int CCANFLSAHTOOLDlg::openUSB()
{   
//	int CanIdx = 0;
//	DWORD FilterID;
//	DWORD FilterMask;
//	CAN_Idfilter IdfilterList;
//	CAN_Idfilter *pIdfilter;
//	int dwFilterNum = 0;
//	int result = CAN2USB_RESULT_OK;
//	m_bdeviceQuitFlag = FALSE;
//
//	CAN2USB_findDevices(m_CanDevice, &m_NumDevices);
//
//#if 0
//	int result =CAN2USB_openCAN();
//#else
//	hCanDev = NULL;
//	if(m_NumDevices>0)
//	{
//		if((m_CanDevice[0].DevPath[0] !=0x3a)||(m_CanDevice[0].DevPath[5]==0))
//		{
//			result = CAN2USB_RESULT_NO_DEVICE;
//			return result;
//		}
//		 
//		CAN2USB_openCAN(m_CanDevice[0].DevPath,&hCanDev);
//		if(hCanDev == NULL)
//		{
//			result = CAN2USB_RESULT_NO_DEVICE;
//		}
//	}
//	else
//	{
//			result = CAN2USB_RESULT_NO_DEVICE;
//	}
//#endif
//	if(result == CAN2USB_RESULT_OK)
//	{
//		usb_flag=1;	 
//		strcpy(OpenDevPath,m_CanDevice[0].DevPath);
//		Sleep(100);
//		//CAN2USB_resetCAN(5*1000);
//		#ifdef HARDCAN
//	//	CAN2USB_resetCAN(hCanDev,20*1000);
//		CAN2USB_setWorkMode(hCanDev,CAN_CHANNEL_0, 0);
//		 CAN2USB_setBaudRate(hCanDev,CAN_CHANNEL_0,500000,20*1000);
//		CAN2USB_getFilterId(hCanDev,CAN_CHANNEL_0,&IdfilterList,&dwFilterNum);
//		CAN2USB_removeFilterId(hCanDev,CAN_CHANNEL_0,&IdfilterList);
//		CAN_msg msg;
//		msg.id=0x7e0;
//		msg.data[0]=0x02;
//		msg.data[1]=0x10;
//		msg.data[2]=0x02;
//		msg.ch=1;
//		msg.format=0;
//		msg.len=3;
//		msg.type=0;
//		IdfilterList.filterId = 0x7e8;
//		IdfilterList.filterMask = 0xFFFFFFFF;
//		IdfilterList.frmStdOrExt = CAN_ID_FORMAT_STANDARD;//CAN_ID_FORMAT_EXTENDED;
//		IdfilterList.frmRemote = CAN_FRAME_TYPE_DATA ;//CAN_FRAME_TYPE_REMOTE;
//		CAN2USB_getFilterId(hCanDev,CAN_CHANNEL_0,&IdfilterList,&dwFilterNum);
//		CAN2USB_removeFilterId(hCanDev,CAN_CHANNEL_0,&IdfilterList);
//
//		CAN2USB_setFilterId(hCanDev,CAN_CHANNEL_0,&IdfilterList);
//		#endif
//		//IdfilterList.filterId = 0x7df;
//		//IdfilterList.filterMask = 0;
//		//IdfilterList.frmStdOrExt = CAN_ID_FORMAT_STANDARD;//CAN_ID_FORMAT_EXTENDED;
//		//IdfilterList.frmRemote = CAN_FRAME_TYPE_DATA ;//CAN_FRAME_TYPE_REMOTE;
//		//CAN2USB_setFilterId(CanIdx,CAN_CHANNEL_0,&IdfilterList);
//		
//		//showStatus(_T("USB2CAN is open。"),1);
//		return result;
//	}
 
	int result;
	m_bdeviceQuitFlag = FALSE;
	result = CAN2USB_openCAN();
	if(result == CAN2USB_RESULT_OK)
	{
		usb_flag=1;	
		Sleep(100);
		CAN2USB_resetCAN(5*1000);
		 
		showStatus(_T("USB2CAN is open。"),1);
		return result;
	}
	else if(result == CAN2USB_RESULT_NO_DEVICE)
	{isCanFlag=0;
		showStatus(_T("USB2CAN not find！"),1);
		return result;
	}
	else
	{isCanFlag=0;
		CString temp;
		temp.Format(_T("open USB2CAN failed,error code：%d。"),result);

		showStatus(temp,1); 
		return result;
	}
	return result;
}
/*关闭can卡：CANtoUSB*/
int CCANFLSAHTOOLDlg::closeUSB()
{
	m_bdeviceQuitFlag = TRUE;
	//if(hCanDev!=NULL)
	//CAN2USB_closeCAN(hCanDev);
	//hCanDev=NULL;
	CAN2USB_closeCAN();
	usb_flag=0;	 
	memset(OpenDevPath,0,256);
	showStatus(_T("CAN2USB Device close"),1);
	return 0;
}
/*初始化can卡：CANtoUSB*/
void CCANFLSAHTOOLDlg::OnBnClickedRadioCan2usb()
{
	// TODO: 在此添加控件通知处理程序代码
	isCanFlag=0;
	g_CANDirverType=0;
	choose_flag=1;
	if(usb_flag== 0x01)
	{		
		closeUSB();
		usb_flag=0;
	}
	int result_usb = openUSB();
 	if(result_usb==CAN2USB_RESULT_OK)
	{
		GetDlgItem(IDC_STATIC_STATUS)->SetWindowText(_T("CAN2USB"));
		((CButton*)GetDlgItem(IDC_RADIO_CAN2USB))->SetCheck(1);
		isCanFlag=1;
		usb_flag=1;
	}
	else if(result_usb == CAN2USB_RESULT_NO_DEVICE)
	{
		GetDlgItem(IDC_STATIC_STATUS)->SetWindowText(_T("<ERROR>no CAN2USB device found"));
	}
	else
	{
		GetDlgItem(IDC_STATIC_STATUS)->SetWindowText(_T("Error:CAN2USB code：")+result_usb);
	}
	UpdateData();
}

/*初始化can卡：Cancard2*/
void CCANFLSAHTOOLDlg::OnBnClickedRadioCancard2()
{
	// TODO: 在此添加控件通知处理程序代码
	isCanFlag=0;
	choose_flag=1;
	if(m_CANcard == NULL)m_CANcard=new CCANCARD2Func();                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                    
	else 
	{
		if(card2_flag== 1)
		{
			CANPC_reinitialize();
			INIPC_close_board();
			card2_flag=0;
		}
	}
	isCanFlag=0;
	if (m_CANcard->CANInitialize()) 
	{
		GetDlgItem(IDC_STATIC_STATUS)->SetWindowText(_T("<ERROR>no Cancard2 device found."));//
	}
	else
	{		
 		if (m_CANcard->InitializeFIFO()) {GetDlgItem(IDC_STATIC_STATUS)->SetWindowText(_T("Error:Cancard2 FIFO"));}
		if (CANPC_start_chip() != 0)
		{
			//printf("-->Error in CANPC_start_chip \n");
			INIPC_close_board();
		}
		  else
		  {
			  GetDlgItem(IDC_STATIC_STATUS)->SetWindowText(_T("Cancard2"));
				((CButton*)GetDlgItem(IDC_RADIO_CANCARD2))->SetCheck(1);
			g_CANDirverType=1;
			card2_flag=isCanFlag=1;
		}
	}
}
/*初始化can卡：Ican*/
void CCANFLSAHTOOLDlg::OnBnClickedRadioIcan()
{
	// TODO:  
	isCanFlag=0;
	if(m_nIcanStatus != 0)return ;
	int result =m_pCANFlash->InitICAN();
	if(result == 1)
	{
		g_CANDirverType=3;	
		ican_flag=isCanFlag=1;
		choose_flag=1;
		((CButton*)GetDlgItem(IDC_RADIO_ICAN))->SetCheck(1);
		GetDlgItem(IDC_STATIC_STATUS)->SetWindowText(_T("ICAN"));
	}
	else
	{
		GetDlgItem(IDC_STATIC_STATUS)->SetWindowText(_T("<ERROR>no ICAN device found."));
	}


}
/*初始化can卡：Es581*/
void CCANFLSAHTOOLDlg::OnBnClickedRadioEs581()
{
	// TODO: ÔÚ´ËÌí¼Ó¿Ø¼þÍ¨Öª´¦Àí³ÌÐò´úÂë
	static	bool ES581_flag=0;
	isCanFlag=0;
	int result;
	if(m_nES581Status == false)return ;
	 
 	if(ES581_flag== 1)
	{
		m_ES581CAN->cmdCloseDevice();
		ES581_flag=0;
	}
	result = m_ES581CAN->CmdOpenFirstDevice();
	if(result == 0)
	{
		g_CANDirverType=ES581;	
		ES581_flag=1;
		choose_flag=isCanFlag=1;
		GetDlgItem(IDC_STATIC_STATUS)->SetWindowText(_T("ES581"));
		((CButton*)GetDlgItem(IDC_RADIO_ES581))->SetCheck(1);
		if( m_ES581CAN->cmdSetBitRate(500000)!=0)
		{
			//GetDlgItem(IDC_STATIC_STATUS)->SetWindowText(_T("<ERROR>Problem setting ES581 rate."));
		}

	}
	else
	{
		GetDlgItem(IDC_STATIC_STATUS)->SetWindowText(_T("<ERROR>no ES581 device found."));
	}
 }

/*初始化can卡：Cancardxl*/
void CCANFLSAHTOOLDlg::OnBnClickedRadioCancardxl()
{choose_flag=1;
	// TODO: 在此添加控件通知处理程序代码
	XLstatus      xlStatus;
	unsigned long bitrate = 500000;
	static	BOOL xl_flag=0;
	isCanFlag=0;
	if(m_CAN == NULL)m_CAN=new CCANFunctions();
	else 
	{
		if(xl_flag== 1)
		{
			xl_flag=0;
			xlCloseDriver();
			//CCANFunctions();
		}
	}
	isCanFlag=0;
	if (m_CAN->CANInit()) 
	{
		GetDlgItem(IDC_STATIC_STATUS)->SetWindowText(_T("<ERROR>no cancardxl device found"));
	}
	else
	{
		GetDlgItem(IDC_STATIC_STATUS)->SetWindowText(_T("cancardxl"));
		((CButton*)GetDlgItem(IDC_RADIO_CANCARDXL))->SetCheck(1);
		xlStatus = m_CAN->CANGoOnBus(bitrate);
		m_CAN->CANSetFilter(0x7e8,0x7ff);
		if (xlStatus) {GetDlgItem(IDC_STATIC_STATUS)->SetWindowText(_T("Error:cancardxl GoOnBus"));}
		g_CANDirverType=2;
		xl_flag=isCanFlag=1;
	}
}

/*自动搜索CAN卡：Can2usb，Cancard2，Cancardxl，Ican*/
void CCANFLSAHTOOLDlg::AutoCheckCANcard(void)
{
	isCanFlag=0;
	CheckDlgButton(IDC_RADIO_CANCARDXL,BST_UNCHECKED);
	CheckDlgButton(IDC_RADIO_CANCARD2,BST_UNCHECKED);
	CheckDlgButton(IDC_RADIO_CAN2USB,BST_UNCHECKED); 
	CheckDlgButton(IDC_RADIO_ICAN,BST_UNCHECKED); 
	CheckDlgButton(IDC_RADIO_ES581,BST_UNCHECKED); 
	 
	
	//if(isCanFlag != 0)return;
	
	if(isCanFlag == 0) 
	{
		OnBnClickedRadioCancardxl();		
		if(isCanFlag == 1)
		{
			CheckDlgButton(IDC_RADIO_CANCARDXL,BST_CHECKED);
			CheckDlgButton(IDC_RADIO_CANCARD2,BST_UNCHECKED);
			CheckDlgButton(IDC_RADIO_CAN2USB,BST_UNCHECKED); 
			CheckDlgButton(IDC_RADIO_ICAN,BST_UNCHECKED); 
			CheckDlgButton(IDC_RADIO_ES581,BST_UNCHECKED); 
			Flash_Result=0;
		}
	}
	if(isCanFlag == 0) 
	{
		OnBnClickedRadioCancard2();
		if(isCanFlag == 1)
		{
			CheckDlgButton(IDC_RADIO_CANCARDXL,BST_UNCHECKED);
			CheckDlgButton(IDC_RADIO_CANCARD2,BST_CHECKED);
			CheckDlgButton(IDC_RADIO_CAN2USB,BST_UNCHECKED); 
			CheckDlgButton(IDC_RADIO_ICAN,BST_UNCHECKED); 
			CheckDlgButton(IDC_RADIO_ES581,BST_UNCHECKED); 
			Flash_Result=0;
		}
	}
	if(isCanFlag == 0) 
	{
		OnBnClickedRadioEs581();
		if(isCanFlag == 1)
		{
			CheckDlgButton(IDC_RADIO_CANCARDXL,BST_UNCHECKED);
			CheckDlgButton(IDC_RADIO_CANCARD2,BST_UNCHECKED);
			CheckDlgButton(IDC_RADIO_CAN2USB,BST_UNCHECKED);
			CheckDlgButton(IDC_RADIO_ICAN,BST_UNCHECKED);
			CheckDlgButton(IDC_RADIO_ES581,BST_CHECKED); 
			Flash_Result=0;
		}
	}
	if(isCanFlag == 0) 
	{
		OnBnClickedRadioIcan();
		if(isCanFlag == 1)
		{
			CheckDlgButton(IDC_RADIO_CANCARDXL,BST_UNCHECKED);
			CheckDlgButton(IDC_RADIO_CANCARD2,BST_UNCHECKED);
			CheckDlgButton(IDC_RADIO_CAN2USB,BST_UNCHECKED); 
			CheckDlgButton(IDC_RADIO_ICAN,BST_CHECKED); 
			CheckDlgButton(IDC_RADIO_ES581,BST_UNCHECKED); 
			Flash_Result=0;
		}
	}
if(isCanFlag == 0) 
	{	
		OnBnClickedRadioCan2usb();
		if(isCanFlag == 1)
		{
			CheckDlgButton(IDC_RADIO_CANCARDXL,BST_UNCHECKED);
			CheckDlgButton(IDC_RADIO_CANCARD2,BST_UNCHECKED);
			CheckDlgButton(IDC_RADIO_CAN2USB,BST_CHECKED); 
			CheckDlgButton(IDC_RADIO_ICAN,BST_UNCHECKED); 
			CheckDlgButton(IDC_RADIO_ES581,BST_UNCHECKED); 
			Flash_Result=0;
		}
	}
 }
/*
检测文件的正确性，MIXED处理文件。
*/
int CCANFLSAHTOOLDlg::CheckSpecialCalAddress(void)
{
	int result=0;
	CString str;
	int i;
	for(i=0;i<config->ECUprg.SpecCalAddrNum;i+=2)
	{
		if(config->ECUprg.SpecCaladdr[i]>0 && config->ECUprg.SepCaladdrFlag[i]==0)break;
	}
	//str.Format(_T("Can not find the address:%08X:%08X,\nPlease check the cal file,\nContinue flashing?"),config->ECUprg.SpecCaladdr[i],config->ECUprg.SpecCaladdr[i+1]);
 //	int n = MessageBox(str,_T("ERROR"),MB_YESNO);
	//if(n == IDNO)return -1;	
	//else result = 0;
	return result;

}
int CCANFLSAHTOOLDlg::OperateFile(int type)
{
	int result=0;
 	if(g_EcuFile == CAL)
	{
		result = pOpFile->GetFileBankNum(m_szCalPath,CAL);
		if(result == -1)MessageBox(_T("Cannot open the cal file"),_T("ERROR"));
		else
		{
			if(config->ECUprg.CalCurNum == 0)
			{
				MessageBox(_T("Please choose correct cal file"),_T("ERROR"));
				return -1;
			}
			if(config->ECUprg.SpecCalAddrNum >0 && config->ECUprg.SepCalCurNum == 0)
			{
				result =CheckSpecialCalAddress();
  			}
		}
	}
	else if(g_EcuFile == CUT)
	{
		if(config->ECUprg.b_cal_app == MIXED)
		{
			result = pOpFile->GetTransData(m_szCutPath,CUTFILE);
			if(result == -1)MessageBox(_T("Cannot open the cut/ptp file"),_T("ERROR"));
		}
		else
		{
			result = pOpFile->GetFileBankNum(m_szCutPath,CUTFILE);
			if(result == -1)MessageBox(_T("Cannot open the cut/ptp file"),_T("ERROR"));
			else
			{
				if(config->ECUprg.AppCurNum == 0 || config->ECUprg.CalCurNum == 0)
				{
					MessageBox(_T("Please choose correct cut file"),_T("ERROR"));
					return -1;
				}
				if(config->ECUprg.SpecCalAddrNum >0 && config->ECUprg.SepCalCurNum == 0)
				{
					result =CheckSpecialCalAddress();
  				}
			}
		}
	}
	else if(g_EcuFile == PTP_CAL)
	{
		result = pOpFile->GetFileBankNum_ptpcal(m_szPtpPath,m_szCalPath,PTPFILE);
		if(result == -1)MessageBox(_T("Cannot open the app file"),_T("ERROR"));
		else
		{
			if(config->ECUprg.AppCurNum == 0)
			{
				MessageBox(_T("Please choose correct app file"),_T("ERROR"));
				return -1;
			}
			result = pOpFile->GetFileBankNum_ptpcal(m_szCalPath,m_szPtpPath,CALFILE);
			if(result == -1)MessageBox(_T("Cannot open the cal file"),_T("ERROR"));
			else
			{
				if(config->ECUprg.CalCurNum == 0)
				{
					MessageBox(_T("Please choose correct cal file"),_T("ERROR"));
					return -1;
				}
				if(config->ECUprg.SpecCalAddrNum >0 && config->ECUprg.SepCalCurNum == 0)
				{
					result =CheckSpecialCalAddress();
  				}
			}
		}
	}
	else if(g_EcuFile == BOOT)
	{
		result = pOpFile->GetFileBankNum(m_szBootPath,BOOTFILE);
		if(result == -1)MessageBox(_T("Cannot open the boot file"),_T("ERROR"));
		else
		{
			if(config->ECUprg.BootCurNum == 0)
			{
				MessageBox(_T("Please choose correct boot file"),_T("ERROR"));
				return -1;
			}
			result = pOpFile->GetTransData(m_szBootPath,BOOTFILE);
			if(result == -1)MessageBox(_T("Cannot open the boot file"),_T("ERROR"));
  		}
	}
	else if(g_EcuFile == MFG)
	{
		result = pOpFile->GetFileBankNum(m_szMfgPath,MFGFILE);
		if(result == -1)MessageBox(_T("Cannot open the mfg file"),_T("ERROR"));
		else
		{
			if(config->ECUprg.MfgCurNum == 0)
			{
				MessageBox(_T("Please choose correct mfg file"),_T("ERROR"));
				return -1;
			}
			result = pOpFile->GetTransData(m_szMfgPath,MFGFILE);
			if(result == -1)MessageBox(_T("Cannot open the mfg file"),_T("ERROR"));
  		}
	}
	else if(g_EcuFile == TSW)
	{
		result = pOpFile->GetFileBankNum(m_szTswPath,TSWFILE);
		if(result == -1)MessageBox(_T("Cannot open the tsw file"),_T("ERROR"));
		else
		{
			if(config->ECUprg.TswCalCurNum == 0)
			{
				MessageBox(_T("Please choose correct tsw file"),_T("ERROR"));
				return -1;
			}
			result = pOpFile->GetTransData(m_szTswPath,TSWFILE);
			if(result == -1)MessageBox(_T("Cannot open the tsw file"),_T("ERROR"));
  		}
	}
	return result;
}
/*
data stream文件头
*/
void CCANFLSAHTOOLDlg::WriteFileHeader(void)
{
	
	char buffer[100];
	char month[12][20]={"January","February","March","April","May","June","July","August","September","October","November","December"}; 
	char date[100];	
	sprintf_s(date,"%02d:%02d:%02d, %s %02d,%4d.\r\n",
	pcStartTime.wHour,pcStartTime.wMinute,pcStartTime.wSecond,month[pcStartTime.wMonth-1],pcStartTime.wDay,pcStartTime.wYear); 
	strdate.Format(_T("%02d:%02d:%02d|"),pcStartTime.wHour,pcStartTime.wMinute,pcStartTime.wSecond);
	CString strYear;
	strYear.Format(_T("-%02d-%04d,"),pcStartTime.wDay,pcStartTime.wYear);
	strdate+=month[pcStartTime.wMonth-1];
	strdate+=strYear;
	LPTSTR s=new TCHAR[MAX_PATH+1]; 
	DWORD l=MAX_PATH; 
    GetUserName(s,&l); 	
	CStringA bufUserName(s);

	LPTSTR sn=new TCHAR[MAX_PATH+1]; 
	l=MAX_PATH;
	GetComputerName(sn,&l); 
	CStringA bufCompName(sn);
	CString str;
	filebuf+="************************************************************\r\n";
	filebuf+="CANFLASHTOOL 2.33 Enterprise,Rebuild on Oct 29,2015\r\n";
	
	str.Format(_T("Dongle Sn=%d\r\n"), m_dwSN);
	filebuf+=str;
	sprintf_s(buffer,"User:%s@%s\r\n",bufUserName,bufCompName); 
	filebuf+=buffer;
	filebuf+=date;

	CString tempstr;
	int n=m_szConfigPath.ReverseFind(_T('\\'));
	tempstr=m_szConfigPath.Mid(n+1);
	filebuf+="CONFIG:";
	filebuf+=tempstr;
	filebuf+="\r\n";
	if(g_EcuFile == CAL)
	{
		CString tempstr;
		int n=m_szCalPath.ReverseFind(_T('\\'));
		tempstr=m_szCalPath.Mid(n+1);
		filebuf+="CAL:";
		filebuf+=tempstr;
		filebuf+="\r\n";
 	}
	else if(g_EcuFile == CUT)
	{
		CString tempstr;
		int n=m_szCutPath.ReverseFind(_T('\\'));
		tempstr=m_szCutPath.Mid(n+1);
		filebuf+="CUT:";
		filebuf+=tempstr;
		filebuf+="\r\n";
 	}
	else if(g_EcuFile == PTP_CAL)
	{
		CString tempstr;
		int n=m_szPtpPath.ReverseFind(_T('\\'));
		tempstr=m_szPtpPath.Mid(n+1);
		filebuf+="PTP:";
		filebuf+=tempstr;
		filebuf+="\r\n";
		n=m_szCalPath.ReverseFind(_T('\\'));
		tempstr=m_szCalPath.Mid(n+1);
		filebuf+="CAL:";
		filebuf+=tempstr;
		filebuf+="\r\n";
 	}
	else if(g_EcuFile == BOOT)
	{
		CString tempstr;
		int n=m_szBootPath.ReverseFind(_T('\\'));
		tempstr=m_szBootPath.Mid(n+1);
		filebuf+="BOOT:";
		filebuf+=tempstr;
		filebuf+="\r\n";
 	}
	else if(g_EcuFile == MFG)
	{
		CString tempstr;
		int n=m_szMfgPath.ReverseFind(_T('\\'));
		tempstr=m_szMfgPath.Mid(n+1);
		filebuf+="MFG:";
		filebuf+=tempstr;
		filebuf+="\r\n";
 	}
	filebuf+="*************************************************************\r\n";
	filebuf+="   +Message Number\r\n";
	filebuf+="   |        +Time(ms)\r\n";
	filebuf+="   |        |      +Type\r\n";
	filebuf+="   |        |      |      +ID (hex)\r\n";
	filebuf+="   |        |      |      |    +Data Length Code\r\n";
	filebuf+="   |        |      |      |    |     +Data Bytes (hex)\r\n";
	filebuf+="   |        |      |      |    |     |\r\n";
	filebuf+="-----------------------------------------------------------\r\n";
	filebuf+="CAN Card Type:";
	switch(g_CANDirverType)
	{
	case 0: 
		filebuf+=_T("CAN2USB");
		break;
	case 1:
		filebuf+=_T("Cancard2");
		break;
	case 2:
		filebuf+=_T("CancardXL");
		break;
	case 3:
		filebuf+=_T("ICAN");
		break;
	case 4:
		filebuf+=_T("ES581");
		break;
		default:break;
	}
  filebuf+="\r\n";
}
/*************************************************
刷写线程
提示窗口，提示是否进行刷写，如果“是”，进行刷写
如果“取消”，停止刷写
***************************************************/
DWORD WINAPI CCANFLSAHTOOLDlg::CEcuFlashToolDlgFlashProcThread(LPVOID lpParameter)
{
	CCANFLSAHTOOLDlg	*dlg=(CCANFLSAHTOOLDlg*)lpParameter;
	int i=0;
	//SetThreadPriority(GetCurrentThread(),THREAD_PRIORITY_NORMAL/*THREAD_PRIORITY_HIGHEST*/);
	dlg->SetTimer(2,1000,NULL);
 	start_clear=1;
	dlg->setFlashState(false);
	::SendMessage( dlg->m_hWnd,WM_USER+1,3,0);
	dlg->m_bflag=1;
	Flash_Result=dlg->m_pCANFlash->CAN_ReadStep(1);
	if( BootFlag == 1)Flash_Result=-16;
	strQueryInfor.Empty();
	if(Flash_Result == 0)
	{
		if(dlg->m_bECUFlag==1)
		 {
			 Flash_Result =dlg->m_pCANFlash->CAN_ReadStep(2);
 		 }
		 if(dlg->m_bNVFlag == 1)
		{
			 Flash_Result =dlg->m_pCANFlash->CAN_Diagnostic();			 
		}
		dlg->KillTimer(2);
		if(Flash_Result != 0)
		{
			::SendMessage(dlg->m_hWnd,WM_USER+1,FAIL,0);	
 		}
		else
		{
			::SendMessage(dlg->m_hWnd,WM_USER+1,SUCCESS,0);	
			dlg->Writedatastream(_T("log.txt"),1,Flash_Result);
		}
		dlg->m_bflag=0;
	}
	else
	{
		dlg->KillTimer(2);
  		::SendMessage(dlg->m_hWnd,WM_USER+1,FAIL,0);	
 	}
	if(dlg->m_bAfterFlag == 1)
	{
		dlg->csFile.Open(dlg->m_szAfterPath,CFile::modeRead);
		while(dlg->readCommandFile()>0);
		dlg->csFile.Close();
	}
 	dlg->setFlashState(true);
 	
	//::SendMessage(dlg->m_hWnd,WM_USER+1,3,0);
 	dlg->GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(true);
 	dlg->GetDlgItem(IDC_BUTTON_FLASH)->EnableWindow(true);
	if(g_EcuFile == PTP_CAL)
	dlg->GetDlgItem(IDC_BUTTON_BUILD)->EnableWindow(true);	

	m_Process.SetPos(0); 
	dlg->GetDlgItem(IDC_STATIC_TIMEPAST)->SetWindowTextW(_T(""));
	dlg->GetDlgItem(IDC_BUTTON_FLASH)->SetFocus();
 	return 1;
}
/*
刷写过程中，控件禁用
*/
void CCANFLSAHTOOLDlg::setFlashState(bool flag)
{
	GetDlgItem(IDC_RADIO_CAN2USB)->EnableWindow(flag);
	GetDlgItem(IDC_RADIO_CANCARD2)->EnableWindow(flag);
	GetDlgItem(IDC_RADIO_CANCARDXL)->EnableWindow(flag);
	GetDlgItem(IDC_COMBO_FILETYPE)->EnableWindow(flag);
	GetDlgItem(IDC_RADIO_ICAN)->EnableWindow(flag);
	GetDlgItem(IDC_RADIO_ES581)->EnableWindow(flag);
// 	if(config->ECUprg.funcFlag==1)GetDlgItem(IDC_COMBO_FILETYPE)->EnableWindow(false);
}
/*
刷写功能
*/
int iID;
void CCANFLSAHTOOLDlg::OnBnClickedButtonFlash()
{
	// TODO: 在此添加控件通知处理程序代码
	gserialNo=0;
	filebuf.Empty();
	m_szSerialNum.Empty();
	strBootReason.Empty();
	displaystreamnum=0;
	g_nChecksumFlag=m_bChecksumFlag;
	start_clear=1;
	for(int i=0;i<listItemNum;i++)
	{
		delete displayArray[i];
	}
	listItemNum=0;
	//if(choose_flag==1 && isCanFlag == 1)choose_flag=0;
	//else 
	//	AutoCheckCANcard();
	if(choose_flag != 1 || isCanFlag != 1)AutoCheckCANcard();
 	if(usb_flag==1)g_usb2canFlag=1;	
 	if(config->ECUprg.Utility_BB_Flag == TRUE)GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(false);
	g_StopFlag = 0;

	if(isCanFlag != 1)	 
	{		
		MessageBox(_T("No CANCard Available!!"),_T("ERROR"));		
 		return;
	}	
 
	g_bBypassFlag=m_bBypassFlag;
	GetLocalTime(&pcStartTime);

 	GetDlgItem(IDC_STATIC_TIMEPAST)->SetWindowTextW(_T("")); 
 	m_nflashtime=0;
	strQueryInfor.Empty();
	if((CheckFlashItems()) == false)//
	{
 		return;
	}
 	else
	{
		int result;
		//if(config->ECUprg.b_cal_app == MIXED || g_EcuFile == BOOT || g_EcuFile == MFG)
		if(g_EcuFile != PTP_CAL)
		{
			result=OperateFile(0);
			if(result == -1)return;
		}

		WriteFileHeader();
		GetDlgItem(IDC_BUTTON_FLASH)->EnableWindow(false);
		GetDlgItem(IDC_BUTTON_BUILD)->EnableWindow(false);	
		Curstr.Empty();

		if((config->ChecksumNum>0))
		{
			if(config->ECUprg.b_cal_app == MIXED)
			{
			CString str;
			DisplayActionItemCurrentValue();
 			 if(config->Checksum !=config->FileChecksum)
			{
				str.Format(_T("Cut_Sum =%04X,which is incorrect.\nThe Correct:%04X\n"),config->FileChecksum, config->Checksum);
				m_pCANFlash->display(str,1);
			 }
			 else
			{
				str.Format(_T("Cut_Sum =%04X,which is correct.\n"),config->FileChecksum);
				m_pCANFlash->display(str,0);
			 }
			 Curstr=_T("Current File Information:\r\n");
			int j=0;
			for(;j<ItemInfor.ItemNum;j++)
			{
				Curstr+=ItemInfor.ActionItem[j].Label;
				Curstr+=_T("=");
				if(ItemInfor.ActionItem[j].Label == "KKSUM")
				{
					ItemInfor.ActionItem[j].CurrentValue.Format(_T("%04X"),config->FileChecksum);
					Curstr+=ItemInfor.ActionItem[j].CurrentValue;
					if(config->Checksum !=config->FileChecksum)
					{
						m_bChecksumCorrectFlag=true;
						Curstr+="(incorrect,please check it.)";
					}
					else 
					{
						m_bChecksumCorrectFlag=false;
						Curstr+="(correct)";
					}
				}
				else
					Curstr+=ItemInfor.ActionItem[j].CurrentValue;
				Curstr +="\r\n";

			}
			}
			//filebuf+=strQueryInfor;
			else
				{
			 if( (g_EcuFile == CUT)||(g_EcuFile == CAL))
				{
					BuildcutAndComputeCheckSum();
				}
					while(1)
					{
						if(buildreturn==0)break;
						Sleep(1);
					}
					Curstr=_T("Current File Information:\r\n");
					CString str;
					CString str1;
					for(int i=0;i<config->ChecksumNum;i++)
					{
						str1=config->m_checksum[i].ChecksumName;
						if(config->m_checksum[i].Checksum !=config->m_checksum [i].FileChecksum)
						{
							str.Format(_T(" =%04X,which is incorrect.\nThe Correct:%04X\n"),config->m_checksum [i].FileChecksum,config->m_checksum[i].Checksum);
							str1 += str;
							m_pCANFlash->display(str1,1);
						 }
						 else
						{
							str.Format(_T(" =%04X,which is correct.\n"),config->m_checksum[i].Checksum);
							str1 += str;
							m_pCANFlash->display(str,0);
						 }
						Curstr+=str1;
					}
					 
			
					//filebuf+=strQueryInfor;
 				}
		}
			//m_bOnFlashFlag=1;
		
			if(m_bBeforeFlag == 1)
			{
				csFile.Open(m_szBeforePath,CFile::modeRead);
				while(readCommandFile()>0);
				 csFile.Close();
			}
			
			m_pCANFlash->m_dwSN=m_dwSN;
 			::SendMessage( m_hWnd,WM_USER+1,3,0);

			Flash_Result=0;
			if(m_bDefaultFlag == 1)
			{
				Curstr+=_T("Current ECU Information:\r\n");	
				 
				Flash_Result = m_pCANFlash->CAN_ReadStep(0);
				
			}
			else
			{
				Curstr+=_T("Current ECU Information:\r\n");	
				Flash_Result = m_pCANFlash->CAN_ReadStep(3);
			}
		//	 Flash_Result=0;
			if(Flash_Result == 0) 
			{	
				filebuf +=Curstr+strQueryInfor+strBootReason;//
				CString str;
				str = Curstr+strQueryInfor;//Curstr
				strQueryInfor=str;
				querydlg = new Querydlg();
				int nResponse = querydlg->DoModal();
				if(nResponse == IDOK)
				{
					strQueryInfor.Empty();	
  					 FlashProcThread = ::CreateThread(NULL,
												   NULL,
												   (LPTHREAD_START_ROUTINE)CEcuFlashToolDlgFlashProcThread,
												   (LPVOID)this,
												   NULL,
												   NULL);
					 CloseHandle(FlashProcThread); 
				}
				else
				{
					//Writedatastream(_T("log.txt"),1,Flash_Result);
					//m_nFailNmu++;
					GetDlgItem(IDC_BUTTON_FLASH)->EnableWindow(true);
					if(g_EcuFile == PTP_CAL)
					GetDlgItem(IDC_BUTTON_BUILD)->EnableWindow(true);	
 					GetDlgItem(IDC_BUTTON_FLASH)->SetFocus();
				}
			}
			else
			{ 
				::SendMessage(m_hWnd,WM_USER+1,FAIL,0);
				GetDlgItem(IDC_BUTTON_FLASH)->SetFocus();
 				//出错，取消刷写
 			}
 		}
	
 	return ;

 
}

/*
刷写暂停
*/
void CCANFLSAHTOOLDlg::OnBnClickedButtonStop()
{
	// TODO: 在此添加控件通知处理程序代码
 
	g_StopFlag=1;	
}
/*
打开工程文件。
*/
int CCANFLSAHTOOLDlg::OpenFlt(CString m_Path)
{
	CString strReadW;
	U32 len = 0;
 	int flag=0;
	int offset=0;
	int result;
 	flag=0;
 	if(!csFile.Open(m_Path,CFile::modeRead))return -1;
	m_bAfterFlag = 0;
	m_bDefaultFlag=1;
	m_bBeforeFlag=m_bChecksumFlag=m_bBypassFlag=0;
	m_bEnableStopFlag=0;
	m_szBeforePath.Empty();
	m_szAfterPath.Empty();
	g_EcuFile = CUT;
 	while(1)
	{
		if(flag == 0)
		{
			if(csFile.ReadString(strReadW)== NULL)break;
		}
		flag=0;

		if(strReadW.Find(_T("ECM_Definition_Location"))!=-1)
		{
			csFile.ReadString(m_szConfigPath);
		  
		}
		if(strReadW.Find(_T("Special_Check"))!=-1)
		{
			csFile.ReadString(strReadW);
		  
		}
		if(strReadW.Find(_T("Security_Key"))!=-1)
		{
			g_nKeyFlag = 0;
			while(csFile.ReadString(strReadW))
			{					
				if(strReadW.Find(_T("Option"))!=-1)
				{
					if(strReadW.Find(_T("TRUE"))!=-1)g_nKeyFlag = 2;

				}
 				else if(strReadW.Find(_T("Manually_Input"))!=-1)
				{
					if(strReadW.Find(_T("TRUE"))!=-1)g_nKeyFlag = 1;
 
				}
 				else if(strReadW.Find(_T("Manual_Key_Value"))!=-1)
				{
					g_nKeyValue = config->GetValue(strReadW,1);		 
				}
				else if(strReadW.Find(_T("Function_Interface"))!=-1)
				{
					int n=strReadW.Find(_T(" "));
					g_szKeyPath = strReadW.Mid(n);
					g_szKeyPath.TrimLeft();
				}
				else if(strReadW.Find(_T("Manually_SetParam"))!=-1)
				{
					if(strReadW.Find(_T("TRUE"))!=-1)g_bSetParamFlag = 1;
					else g_bSetParamFlag=0;
				}
 				else if(strReadW.Find(_T("Manual_Param_Value"))!=-1)
				{
					g_nParamValue = config->GetValue(strReadW,1);		 
				}
				else if(strReadW.Find(_T("Auto_ChooseDll"))!=-1)
				{
					if(strReadW.Find(_T("FALSE"))!=-1)g_nKeyAutoFlag = 0;
					else g_nKeyAutoFlag =1;
 
				}
				else 
				{
					if(strReadW.Find(_T("["))!=-1)
					flag=1;
					break;
				}
			}		
		}
		if(strReadW.Find(_T("Read_ECU_Information"))!=-1)
		{
		
			while(csFile.ReadString(strReadW))
			{					
				if(strReadW.Find(_T("Default"))!=-1)
				{
					if(strReadW.Find(_T("FALSE"))!=-1)m_bDefaultFlag = 0;
					else m_bDefaultFlag =1;

				}
				else 
				{
					if(strReadW.Find(_T("["))!=-1)
					flag=1;
					break;
				}
			}		
		} 
		if(strReadW.Find(_T("Checksum_Calculation"))!=-1)
		{
		
			while(csFile.ReadString(strReadW))
			{					
				if(strReadW.Find(_T("Default"))!=-1)
				{
					if(strReadW.Find(_T("FALSE"))!=-1)m_bChecksumFlag = 0;
					else m_bChecksumFlag =1;

				}
				else 
				{
					if(strReadW.Find(_T("["))!=-1)
					flag=1;
					break;
				}
			}		
		} 		
		if(strReadW.Find(_T("Enable_Button_Stop"))!=-1)
		{
		
			while(csFile.ReadString(strReadW))
			{					
				if(strReadW.Find(_T("Default"))!=-1)
				{
					if(strReadW.Find(_T("FALSE"))!=-1)m_bEnableStopFlag = 0;
					else m_bEnableStopFlag =1;

				}
				else 
				{
					if(strReadW.Find(_T("["))!=-1)
					flag=1;
					break;
				}
			}		
		} 
		if(strReadW.Find(_T("Bypass_10_02"))!=-1)
		{
		
			while(csFile.ReadString(strReadW))
			{					
				if(strReadW.Find(_T("Default"))!=-1)
				{
					if(strReadW.Find(_T("FALSE"))!=-1)m_bBypassFlag = 0;
					else m_bBypassFlag =1;

				}
				else 
				{
					if(strReadW.Find(_T("["))!=-1)
					flag=1;
					break;
				}
			}		
		} 
		if(strReadW.Find(_T("Verify_Communication"))!=-1)
		{
		
			while(csFile.ReadString(strReadW))
			{					
				if(strReadW.Find(_T("Run_Keyword_Command_Batch_Option_Before_Reprogramming"))!=-1)
				{
					if(strReadW.Find(_T("FALSE"))!=-1)m_bBeforeFlag = 0;
					else m_bBeforeFlag =1;

				}
 				else if(strReadW.Find(_T("Keyword_Command_Batch_File_Patch_Before_Reprogramming"))!=-1)
				{
 					int n = strReadW.Find(_T(" "));
					if(n!=-1) m_szBeforePath = strReadW.Mid(n);
					m_szBeforePath.TrimLeft();
 
				}
 				else if(strReadW.Find(_T("Run_Keyword_Command_Batch_Option_After_Reprogramming"))!=-1)
				{
					if(strReadW.Find(_T("FALSE"))!=-1)m_bAfterFlag =0;
					else m_bAfterFlag = 1;

				}
 				else if(strReadW.Find(_T("Keyword_Command_Batch_File_Patch_After_Reprogramming"))!=-1)
				{
					
					int n = strReadW.Find(_T(" "));
					if(n!=-1) m_szAfterPath = strReadW.Mid(n);
					m_szAfterPath.TrimLeft();
 
				}				
				else 
				{
					if(strReadW.Find(_T("["))!=-1)
					flag=1;
					break;
				}
			}		
		} 
		if(strReadW.Find(_T("Flash_Page:Type"))!=-1)
		{
			csFile.ReadString(strReadW);
			if(strReadW.Find(_T("CUT"))!=-1)g_EcuFile = CUT;
			else if(strReadW.Find(_T("APP_CAL"))!=-1)g_EcuFile = PTP_CAL;
			else  if(strReadW.Find(_T("CAL"))!=-1)g_EcuFile = CAL;
			else  if(strReadW.Find(_T("BOOT"))!=-1)g_EcuFile = BOOT;
			else  if(strReadW.Find(_T("MFG"))!=-1)g_EcuFile = MFG;
			else if(strReadW.Find(_T("TSW"))!=-1)g_EcuFile = TSW;
		}
		if(strReadW.Find(_T("Flash_Page:CAL_FILES"))!=-1)
		{
			csFile.ReadString(strReadW);
			while(strReadW[0] != _T('['))
			{
				m_szCalPath = strReadW;
				csFile.ReadString(strReadW);
 
				m_ComboPath.szCalBuf[m_ComboPath.nCalNum++]=m_szCalPath;			 	 
					 
				}
			flag = 1;
		}
		if(strReadW.Find(_T("Flash_Page:PTP_FILES"))!=-1)
		{
			csFile.ReadString(strReadW);
			
			while(strReadW[0] != _T('['))
			{

				m_szPtpPath = strReadW;
				csFile.ReadString(strReadW);
				m_ComboPath.szPtpBuf[m_ComboPath.nPtpNum++]=m_szPtpPath;	
			}		 
				flag = 1;
		  
		}
		if(strReadW.Find(_T("Flash_Page:BOOT_FILES"))!=-1)
		{
			csFile.ReadString(strReadW);
			
			while(strReadW[0] != _T('['))
			{

				m_szBootPath = strReadW;
				csFile.ReadString(strReadW);				
 				m_ComboPath.szBootBuf[m_ComboPath.nBootNum++]=m_szBootPath;	

			}		 
				flag = 1;		  
		}
		if(strReadW.Find(_T("Flash_Page:MFG_FILES"))!=-1)
		{
			csFile.ReadString(strReadW);
			
			while(strReadW[0] != _T('['))
			{

				m_szMfgPath = strReadW;
				csFile.ReadString(strReadW);				
 				m_ComboPath.szMfgBuf[m_ComboPath.nMfgNum++]=m_szMfgPath;	

			}		 
				flag = 1;		  
		}
		if(strReadW.Find(_T("Flash_Page:TSW_FILES"))!=-1)
		{
			csFile.ReadString(strReadW);
			
			while(strReadW[0] != _T('['))
			{
				m_szTswPath = strReadW;
				csFile.ReadString(strReadW);				
 				m_ComboPath.szTswBuf[m_ComboPath.nTswNum++]=m_szTswPath;	

			}		 
			flag = 1;		  
		}

		if(strReadW.Find(_T("Flash_Page:CUT_FILES"))!=-1)
		{
			csFile.ReadString(strReadW);
			while(strReadW[0] != _T('['))
			{
				m_szCutPath = strReadW;
				csFile.ReadString(strReadW);
				m_ComboPath.szCutBuf[m_ComboPath.nCutNum++]=m_szCutPath;
			}		 
			 flag = 1;
		}
		if(strReadW.Find(_T("Datastream_Option"))!=-1)
		{
			csFile.ReadString(strReadW);
		  
		}	
	}
	if(m_bEnableStopFlag == 1)GetDlgItem(IDC_BUTTON_STOP)->ShowWindow(true);//EnableWindow(false);
	else GetDlgItem(IDC_BUTTON_STOP)->ShowWindow(false);
	if(g_bSetParamFlag == 0)g_nParamValue=1;
	csFile.Close();
	UpdateData(false);
	m_bCurrentPro=1;
	

	int jj=0;
	//if(m_szConfigPath=="")OpenPrg() ;	 
	 

	result = config->Read_Prg(m_szConfigPath);
	if(result == -1)
	{
		result=OpenPrg() ;
		result = config->Read_Prg(m_szConfigPath);
		if(result == -1)
		{
		m_szConfigPath="";
		MessageBox(_T("config file open ERROR."),_T("ERROR"),MB_ICONERROR|MB_OK);
		return -1;
		}
		Saveproject(m_Path);
	}
	return 1; 
}
/*配置文件路径错误时，重新弹出对话框来选择新的配置文件*/
int CCANFLSAHTOOLDlg::OpenPrg() 
{
	int n;
	n=MessageBox(_T("Cannot Open ECM Definition file.\n Do you want to Reload it?"),_T("ERROR"),MB_YESNO);
	 if(n == IDNO)return -1;
	LPCTSTR Types ;
	Types = 
			_T("PRG File (*.PRG)|*.PRG|")
			_T("All Files (*.*)|*.*||");
	CFileDialog dlg(TRUE,_T("bin"),NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,Types );
		dlg.m_ofn.lpstrTitle =_T("Open File");

	if(dlg.DoModal()==IDCANCEL)return -1;
		
	m_szConfigPath = dlg.GetPathName();
	return 1;
 
}
int CCANFLSAHTOOLDlg::Recurse(LPCTSTR pstr,LPCTSTR pfilestr) 
{ 
	CFileFind finder; 
	
		static int jj=0;
	// build a string with wildcards 
	CString strWildcard(pstr); 
	strWildcard += _T( "*.* ");//_T( "\\*.* "); 

	// start working for files 
	BOOL bWorking = finder.FindFile(strWildcard); 

	while (bWorking) 
	{ 
	bWorking = finder.FindNextFile(); 

	// skip . and .. files; otherwise, we 'd 
	// recur infinitely! 

	if (finder.IsDots()) 
	continue; 

	// if it 's a directory, recursively search it 

	if (finder.IsDirectory()) 
	{ 

		CString str = finder.GetFilePath(); 
			 file[jj]=str;
			 jj++;
		str+=_T("\\");
		Recurse(str,pfilestr); 
		} 
		if (!finder.IsDirectory()) 
		{ 

			CString str = finder.GetFilePath(); 
			 file[jj]=str;
			 jj++;
	 
		} 
	} 

	finder.Close(); 
	return jj;
} 
/* 把单个项目路径显示到菜单栏的子菜单中，最多添加4个*/
void CCANFLSAHTOOLDlg::DisplayPrgToMenu(CString path)
{
	CMenu *pMenu = GetMenu();
	CMenu *pSubMenu = pMenu->GetSubMenu(0);

	pSubMenu->InsertMenu(MENUPOSITION,MF_BYPOSITION,ID_MENUSTART+m_nMenuNum,path);
	
		
 	if(m_nMenuNum==4)
	{
		pSubMenu->DeleteMenu(MENUPOSITION+m_nMenuNum,MF_BYPOSITION);
 	}
	else m_nMenuNum++;
}
/* 当添加新的项目路径，把新的项目路径保存到菜单栏的子菜单中，最多添加4个*/
void CCANFLSAHTOOLDlg::AddPrgToMenu(CString path)
{
	CMenu *pMenu = GetMenu();
	CMenu *pSubMenu = pMenu->GetSubMenu(0);

	pSubMenu->InsertMenu(MENUPOSITION,MF_BYPOSITION,ID_MENUSTART+m_nMenuNum,path);
	if(m_nMenuNum> 0)
	{
		for(int i=m_nMenuNum-1;i>=0;i--)
		{
 			m_szProLog[i+1]= m_szProLog[i];//[1]=[0]
		}

	}		
		
 	if(m_nMenuNum==4)
	{
		pSubMenu->DeleteMenu(6+m_nMenuNum,MF_BYPOSITION);
 	}
	else m_nMenuNum++;
	m_szProLog[0]=path;
}
/*  处理project文件*/
int CCANFLSAHTOOLDlg::ReadProject(CString str)
{
	m_ComboPath.nCalNum=0;
	m_ComboPath.nCutNum=0;
	m_ComboPath.nPtpNum=0;
	m_ComboPath.nBootNum=0;
	m_ComboPath.nMfgNum=0;
	m_ComboPath.nTswNum=0;
	m_szCutPath.Empty();
	m_szCalPath.Empty();
	m_szPtpPath.Empty();
	m_szTswPath.Empty();
	m_szMfgPath.Empty();
	m_szBootPath.Empty();	
	m_szFltPath=str;
	int result = OpenFlt(m_szFltPath);
	if(result == -1)MessageBox(_T("Cannot open the project file"),_T("ERROR"));
	else
	{
		m_ComboCut.ResetContent();
		for(int j=0;j<m_ComboPath.nCutNum;j++)
		{
			m_ComboCut.AddString(m_ComboPath.szCutBuf[j]);
			m_ComboCut.SetItemData(j,j+1);
		}		
		m_ComboCut.SetCurSel(0);	
		if(m_ComboPath.nCutNum > 0)m_szCutPath = m_ComboPath.szCutBuf[0];

		m_ComboPtp.ResetContent();
		for(int j=0;j<m_ComboPath.nPtpNum;j++)
		{
			m_ComboPtp.AddString(m_ComboPath.szPtpBuf[j]);
			m_ComboPtp.SetItemData(j,j+1);
		}		
		m_ComboPtp.SetCurSel(0);
		if(m_ComboPath.nPtpNum > 0)m_szPtpPath = m_ComboPath.szPtpBuf[0];

		m_ComboCal.ResetContent();
		for(int j=0;j<m_ComboPath.nCalNum;j++)
		{
			m_ComboCal.AddString(m_ComboPath.szCalBuf[j]);
			m_ComboCal.SetItemData(j,j+1);
		}		
		m_ComboCal.SetCurSel(0);
		if(m_ComboPath.nCalNum > 0)m_szCalPath = m_ComboPath.szCalBuf[0];

		m_ComboBoot.ResetContent();
		for(int j=0;j<m_ComboPath.nBootNum;j++)
		{
			m_ComboBoot.AddString(m_ComboPath.szBootBuf[j]);
			m_ComboBoot.SetItemData(j,j+1);
		}		
		m_ComboBoot.SetCurSel(0);	
		if(m_ComboPath.nBootNum > 0)m_szBootPath = m_ComboPath.szBootBuf[0];

		m_ComboMfg.ResetContent();
		for(int j=0;j<m_ComboPath.nMfgNum;j++)
		{
			m_ComboMfg.AddString(m_ComboPath.szMfgBuf[j]);
			m_ComboMfg.SetItemData(j,j+1);
		}		
		m_ComboMfg.SetCurSel(0);
		if(m_ComboPath.nMfgNum > 0)m_szMfgPath = m_ComboPath.szMfgBuf[0];
		m_ComboTsw.ResetContent();
		for(int j=0;j<m_ComboPath.nTswNum;j++)
		{
			m_ComboTsw.AddString(m_ComboPath.szTswBuf[j]);
			m_ComboTsw.SetItemData(j,j+1);
		}		
		m_ComboTsw.SetCurSel(0);		
		if(m_ComboPath.nTswNum > 0)m_szTswPath = m_ComboPath.szTswBuf[0];
		DisplayProject(1);
			
	}
		return result;
}
/* 打开一个现有的project文件
把project path 添加到menu
如果menu里已经有此project path，就显示，否则就添加。同时置顶
*/
void CCANFLSAHTOOLDlg::OnFileLoadproject()
{
	// TODO: 在此添加命令处理程序代码
	int result;
	UpdateData(true);

	LPCTSTR Types;
	Types =
		_T("PTP File (*.cpj)|*.cpj|");
		//_T("All Files (*.*)|*.*||");
	CFileDialog dlg(TRUE,_T("txt"),NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,Types);
	dlg.m_ofn.lpstrTitle =_T("Open File");
	if(dlg.DoModal() == IDOK )
	{ 

		m_szFltPath.Empty();
		m_szFltPath = dlg.GetPathName();

		 result = ReadProject(m_szFltPath);
		 int ctrl;
		 bool flag=0;
		if(result != -1)
 		{ 
			for(int i=0;i<m_nMenuNum;i++)
			{
				if(m_szProLog[i].Find(m_szFltPath)!=-1)
				{
					ctrl=i;
					flag=1;
					break;
				}
			}
				if(flag==1&&ctrl>0)
				{
						CMenu *pMenu = AfxGetMainWnd()->GetMenu();
						CMenu *pSubMenu = pMenu->GetSubMenu(0);

					for(int i=ctrl;i>0;i--)
					{
						m_szProLog[i]= m_szProLog[i-1];//[1]=[0]
					}
					m_szProLog[0]=m_szFltPath;
 					for(int i=0;i<m_nMenuNum;i++)	
					{
						pSubMenu->DeleteMenu(6,MF_BYPOSITION);
					}
					ctrl=m_nMenuNum;
					m_nMenuNum=0;
					for(int i=ctrl-1;i>=0;i--)	
					{
						DisplayPrgToMenu(m_szProLog[i]);
					}
 				}
				if(flag==0)
					AddPrgToMenu(m_szFltPath);
		}
		 
	}
	UpdateData();
	//dlg.DestroyWindow();
	 SetFocus();
	//SetActiveWindow();
	//SetForegroundWindow();
	//SetWindowPos(this,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
}
/* save project to file*/
void CCANFLSAHTOOLDlg::Saveproject(CString path)
{
	if(!csFile.Open(path,CFile::modeWrite | CFile::modeCreate))
	{
			MessageBox(_T("Open file failed"),_T("ERROR"));
			return;
		}
		char buf[200];
		csFile.WriteString(_T("[Begin]\n[ECM_Definition_Location]\n"));
 		csFile.WriteString(m_szConfigPath);
		 csFile.WriteString(_T("\n"));	
		 csFile.WriteString(_T("[Verify_Communication]\n"));
  	 
	 
		if(m_bBeforeFlag == 0)
		{
			 csFile.WriteString(_T("Run_Keyword_Command_Batch_Option_Before_Reprogramming FALSE\n"));
		 
		}
		else
		{
			csFile.WriteString(_T("Run_Keyword_Command_Batch_Option_Before_Reprogramming TRUE\n"));
 		}
			csFile.WriteString(_T("Keyword_Command_Batch_File_Patch_Before_Reprogramming "));
			csFile.WriteString(m_szBeforePath);
 			 csFile.WriteString(_T("\n"));
	 
 	 
		if(m_bAfterFlag == 0)
		{
 			csFile.WriteString(_T("Run_Keyword_Command_Batch_Option_After_Reprogramming FALSE\n"));
		}
		else
		{
			csFile.WriteString(_T("Run_Keyword_Command_Batch_Option_After_Reprogramming TRUE\n"));
 		}
	

			csFile.WriteString(_T("Keyword_Command_Batch_File_Patch_After_Reprogramming "));

			csFile.WriteString(m_szAfterPath);
 			 csFile.WriteString(_T("\n"));
			csFile.WriteString(_T("[Security_Key]\n"));

 		CString str;
		if(g_nKeyFlag == 0)
		{
			csFile.WriteString(_T("Option FALSE\n"));
			csFile.WriteString(_T("Manually_Input FALSE\n"));
 		}
		else 
		{	
			csFile.WriteString(_T("Option TRUE\n"));

 		}
	 
		if(g_nKeyFlag ==1)
		{
			csFile.WriteString(_T("Manually_Input TRUE\n"));

 		}
		else  if(g_nKeyFlag ==2)
		{
			csFile.WriteString(_T("Manually_Input FALSE\n"));
		}
 		
		CString ss;
		ss.Format(_T("Manual_Key_Value %X\n"),g_nKeyValue);
		csFile.WriteString(ss);
		str=_T("Function_Interface ");
		str += g_szKeyPath;
		str += _T("\n");
		CStringA strw(str);
 		csFile.WriteString(str);
		if(g_bSetParamFlag ==1)
		{
			csFile.WriteString(_T("Manually_SetParam TRUE\n"));

 		}
		else   
		{
			csFile.WriteString(_T("Manually_SetParam FALSE\n"));
		}
 		
		ss.Format(_T("Manual_Param_Value %X\n"),g_nParamValue);
		csFile.WriteString(ss);
		if(g_nKeyAutoFlag==1)csFile.WriteString(_T("Auto_ChooseDll TRUE\n"));
		else csFile.WriteString(_T("Auto_ChooseDll FALSE\n"));
 		csFile.WriteString(_T("[Flash_Page:Type]\n"));

		if(g_EcuFile ==  CUT)csFile.WriteString(_T("Option CUT\n"));
		else if(g_EcuFile ==  CAL)csFile.WriteString(_T("Option CAL\n"));
		else if(g_EcuFile ==  PTP_CAL)csFile.WriteString(_T("Option APP_CAL\n"));
		else if(g_EcuFile ==  BOOT)csFile.WriteString(_T("Option BOOT\n"));
		else if(g_EcuFile ==  MFG)csFile.WriteString(_T("Option MFG\n"));

		csFile.WriteString(_T("[Flash_Page:CAL_FILES]\n")); 

		for(int i=0;i<m_ComboPath.nCalNum;i++)
		{
			str = m_ComboPath.szCalBuf[i];
			str += _T("\n");
			csFile.WriteString(str);
		}
		csFile.WriteString(_T("[Flash_Page:PTP_FILES]\n")); 

 		for(int i=0;i<m_ComboPath.nPtpNum;i++)
		{
			str = m_ComboPath.szPtpBuf[i];
			str += _T("\n");
 			csFile.WriteString(str);

		}
		csFile.WriteString(_T("[Flash_Page:CUT_FILES]\n")); 
	
 		for(int i=0;i<m_ComboPath.nCutNum;i++)
		{
			str = m_ComboPath.szCutBuf[i];
			str += _T("\n");
  			csFile.WriteString(str);
		}
 		csFile.WriteString(_T("[Flash_Page:BOOT_FILES]\n")); 
	
 		for(int i=0;i<m_ComboPath.nBootNum;i++)
		{
			str = m_ComboPath.szBootBuf[i];
			str += _T("\n");
  			csFile.WriteString(str);

		}
		csFile.WriteString(_T("[Flash_Page:MFG_FILES]\n")); 
	
 		for(int i=0;i<m_ComboPath.nMfgNum;i++)
		{
			str = m_ComboPath.szMfgBuf[i];
			str += _T("\n");
  			csFile.WriteString(str);

		}
		csFile.WriteString(_T("[Flash_Page:TSW_FILES]\n")); 
	
 		for(int i=0;i<m_ComboPath.nTswNum;i++)
		{
			str = m_ComboPath.szTswBuf[i];
			str += _T("\n");
  			csFile.WriteString(str);

		}
		
		//csFile.WriteString(_T("[Project_Description]\n")); 
		csFile.WriteString(_T("[Read_ECU_Information]\n"));
		if(m_bDefaultFlag==0)
		{
 			csFile.WriteString(_T("Default FALSE\n"));
		}
		else
		{
			csFile.WriteString(_T("Default TRUE\n"));
 		}

		csFile.WriteString(_T("[Checksum_Calculation]\n"));
		if(m_bChecksumFlag==0)
		{
 			csFile.WriteString(_T("Default FALSE\n"));
		}
		else
		{
			csFile.WriteString(_T("Default TRUE\n"));
 		}
		csFile.WriteString(_T("[Enable_Button_Stop]\n"));
		if(m_bEnableStopFlag==0)
		{
 			csFile.WriteString(_T("Default FALSE\n"));
		}
		else
		{
			csFile.WriteString(_T("Default TRUE\n"));
 		}
		csFile.WriteString(_T("[Bypass_10_02]\n"));
		if(m_bBypassFlag==0)
		{
 			csFile.WriteString(_T("Default FALSE\n"));
		}
		else
		{
			csFile.WriteString(_T("Default TRUE\n"));
 		}
		csFile.WriteString(_T("[End]\n")); 
		csFile.Close();
 
}
/* if the project is new ,open the save as dialog,else open the save dailog.*/
void CCANFLSAHTOOLDlg::OnProjectSaveproject()
{
	// TODO: 在此添加命令处理程序代码
 	 
	if(m_szFltPath == "")OnProjectSaveprojectas();
	else Saveproject(m_szFltPath);
	 
	 SetFocus(); 

}
/* display the value of kksum,kkpgmid and security in current file . */
void CCANFLSAHTOOLDlg::DisplayActionItem()
{
	CString st=_T("0");	 
	int i=0;

	for(;i<config->ChecksumNum;i++)
	{		 
   		config->ECUprg.SecAdd=0x7fff0f;
		ItemInfor.ActionItem[i].Label=_T("KKSUM");

 		ItemInfor.ActionItem[i].ActionType=_T("Merge");


		ItemInfor.ActionItem[i].Address.Format(_T("%08X"),config->ECUprg.ChecksumAddr[i]);
		ItemInfor.ActionItem[i].Value=_T("%");
		ItemInfor.ActionItem[i].Value+=config->ChecksumName[i];
		ItemInfor.ActionItem[i].Size.Format(_T("%d"),config->ECUprg.ChecksumSize[i]);
	}
	if(config->ECUprg.KkpgmIdSize!=0)
	{				 
 		ItemInfor.ActionItem[i].Label=_T("KKPGMID");
		ItemInfor.ActionItem[i].ActionType=_T("Display");


		ItemInfor.ActionItem[i].Address.Format(_T("%08X"),config->ECUprg.KkpgmIdAddr);
		ItemInfor.ActionItem[i].Value=_T("%NULL");
		ItemInfor.ActionItem[i].Size.Format(_T("%d"),config->ECUprg.KkpgmIdSize);
	i++;
	}
	
 	{
 		ItemInfor.ActionItem[i].Label=_T("SECURITY");
		ItemInfor.ActionItem[i].ActionType=_T("Display");


		ItemInfor.ActionItem[i].Address=_T("007FFF0F");
		ItemInfor.ActionItem[i].Value=_T("%NULL");
		ItemInfor.ActionItem[i].Size=_T("1");
		i++;
	}
	ItemInfor.ItemNum=i;
}
 /* deal with menu of editspecialactioneditor */
void CCANFLSAHTOOLDlg::OnEditSpecialactioneditor()
{
	// TODO: 在此添加命令处理程序代码
	m_bCurrentPro=0;
 
	int nResponse = m_pCheckSet->DoModal(); 	
	if(nResponse == IDCANCEL){ return;}
	//GetDlgItem(IDC_BUTTON_FLASH)->EnableWindow(false);


}
/* write the data stream to file
if type = 0;save current flash data stream to a new file.单此
type =1;save the data stream to the current exe path下的data文件夹。命令36传送的数据帧不保存到文件中。
从软件打开到关闭，所有的数据流。
*/
int CCANFLSAHTOOLDlg::Writedatastream(CString path,char type,int result)
{
	 WaitForSingleObject(WriteMsgMutex,INFINITE);
	//writestr.Empty ();
	strLog +=filebuf;
	char buf[100];
			
 
	if(type == 0)// save datastream
	{
		writestr.Empty ();
		writestr +=filebuf;
 		if(!csFile.Open(path,CFile::modeWrite | CFile::modeCreate| CFile::typeBinary))//| CFile::modeCreate
		{
			MessageBox(_T("failed"),_T("ERROR"));
			return 1;
		}
			int num=1;
		for(int i=0;i<listItemNum;i++)
		{
			switch(displayArray[i]->MsgState)
			{
			case MSGOUT:
  				sprintf_s(buf,"%06d  ",num);
				writestr +=buf;
				sprintf_s(buf,"%8.02f  ",displayArray[i]->opTime);
				writestr +=buf;
				writestr +="=>   ";
				num++;
				break;
			case MSGIN:			
 				sprintf_s(buf,"%06d  ",num);//displayArray[i]->serialNo
				writestr +=buf;
				sprintf_s(buf,"%8.02f  ",displayArray[i]->opTime);
				writestr +=buf;
				writestr +="<=   ";
				num++;
			break;
			case MSGINFO:
				writestr +="INFO:";
			break;
			case MSGERR:
				writestr +="ERROR:";
			break;
			default:break;
			}
			writestr +=displayArray[i]->data;
		}
		ReleaseMutex(WriteMsgMutex); 
		if(m_bECUFlag == 1)
		writestr +=strQueryInfor+strBootReason;
		DWORD len;
		DWORD size;
	 	if(m_DongleUtility.CheckDllIntegrity())  
		{
			len = m_DongleUtility.EncryptInputLen(writestr);
 			BYTE *ptdata = new BYTE[len]; 
 			size = m_DongleUtility.EncryptDatastream(writestr,ptdata);	 
 			csFile.Write(ptdata,size);
			delete ptdata;
		}
 		csFile.Close();
	}
	else//写log
	{
		 if(result == 0)
		 {
			 strLog +=_T("\r\nINFO: ECM Reprogram Successfully!\r\n");

		 }
		 else
		 {
			int num=1;
			for(int i=0;i<listItemNum;i++)
			{
				switch(displayArray[i]->MsgState)
				{
				case MSGOUT:
					//if(displayArray[i]->Iswrite == 0)
					{
  					sprintf_s(buf,"%06d  ",num);
					strLog +=buf;
					sprintf_s(buf,"%8.02f  ",displayArray[i]->opTime);
					strLog +=buf;
					strLog +="=>   ";
					num++;
					}
					break;
				case MSGIN:	
					//if(displayArray[i]->Iswrite == 0)
					{
 					sprintf_s(buf,"%06d  ",num);//displayArray[i]->serialNo
					strLog +=buf;
					sprintf_s(buf,"%8.02f  ",displayArray[i]->opTime);
					strLog +=buf;
					strLog +="<=   ";
					num++;
					}
				break;
				case MSGINFO:
					strLog +="INFO:";
				break;
				case MSGERR:
					strLog +="ERROR:";
				break;
				default:break;
				}
				//if(displayArray[i]->Iswrite == 0)
					strLog +=displayArray[i]->data;
			 
			}
			ReleaseMutex(WriteMsgMutex); 
 			 if(m_bECUFlag == 1)
			strLog +=strQueryInfor+strBootReason;
		 }
		 
		if(GetFileAttributes(m_szlogDirectory)==-1)CreateDirectory(m_szlogDirectory,NULL);

		 if(!csFilelog.Open(m_szFilelog,CFile::modeWrite | CFile::modeCreate| CFile::modeNoTruncate)) //| CFile::modeCreate
			MessageBox(_T("save log failed"),_T("ERROR"));
		 else
		 {
			 if(csFilelog.GetLength()==0)
			 {
				//MessageBox(_T("save log failed"),_T("ERROR"));
					CString str=_T("Time,App File,Cal File,Cut File,Boot File,Mfg File,Kksum(HEX),DelphiSideRailNumber,Result,PassCounter,FailCounter\n");
				csFilelog.WriteString(str);
			}
			 
			CString str;
			CString strfliename;
			switch(g_EcuFile)
			{
			case CUT:
				strfliename+=_T(",,");
				strfliename+=m_szCutPath;
				strfliename+=_T(",,");
				break;
			case CAL:
				strfliename+=_T(",");
				strfliename+=m_szCalPath;
				strfliename+=_T(",,,");
				break;
			case PTP_CAL:
				strfliename+=m_szPtpPath;
				strfliename+=_T(",");
				strfliename+=m_szCalPath;
				strfliename+=_T(",,,");
				break;
			case BOOT:
				strfliename+=_T(",,,");
				strfliename+=m_szBootPath;//m_szBootPath;
				strfliename+=_T(",");
				break;
			case MFG:
				strfliename+=_T(",,,,");
				strfliename+=m_szMfgPath;
				break;
			default:break;

				}
				strfliename+=_T(",");
				str=strdate;
				str+=strfliename;
			if(config->ChecksumNum>0)
			{
				CString temp;
				temp.Format(_T("%X,"),config->FileChecksum);
				str+=temp;
			}
			else str+=_T(",");
			str+=m_szSerialNum;
			str+=_T(",");
			if(Flash_Result<0)str += "Reprogram Failed.";

			switch(Flash_Result)
			{
			case CAN_RESULT_TIMEOUT:
			case XL_ERR_CMD_TIMEOUT: 
 				str += "timeout.";
				break;
			case CAN_RESULT_BUSOFF_ERROR:
			case XL_ERR_PORT_IS_OFFLINE:
 				str += " PORT is OFFLINE.check the CAN card";
				break;
			case CAN_RESULT_PASSIVE_ERROR:
				str += " BUS PASSIVE.check the CAN card";
				break;
			case CAN_RESULT_ACTIVE_ERROR:
				str += " BUS ACTIVE.check the CAN card";
				break;
			case CAN_RESULT_FRAME_ERROR:
				str += " Error frame detected.check the CAN card";
				break;
			case CAN2USB_RESULT_NOT_OPEN:
				str += " CAN card is not open.check the CAN card";
				break;
			case CAN_RESULT_ADD_ERROR:
				str += " Additional error causes.check the CAN card";
				break;
			case XL_ERR_QUEUE_IS_FULL:
				str += " QUEUE is FULL ACTIVE.check the CAN card";
				break;
			case XL_ERR_WRONG_PARAMETER:
			case CAN_RESULT_PARAM_ERROR:
				str += " PARAMETER ERROR,check the CAN card";
				break;
			case CAN_RESULT_SYSTEM_ERROR:
				str += " SYSTEM ERROR.check the CAN card";
				break;
			case CAN_RESULT_REV_PARAM_ERROR:
				str += " REV PARAM ERROR.check the CAN card";
				break;
			case CHECKSUM_ERROR:
				str += " Checksum error";
				break;
			case Nega_11:
 				str += "The server does not suppoort";
				break;
			case Nega_12:
 				str += "Send if the sub-function parameter in the request message is not supported";
				break;
			case Nega_13:
 				str += "The length of the message is wrong";
				break;
			case Nega_22:
 				str += "The code shall be returned if the criteria for the request are not met ";
				break;
			case Nega_24:
 				str += "The 'sendKey'sub-parameter is received without first receiving a 'requestSeed' request";
				break;
			case Nega_31:
 				str += "Request Out of Range";
				break;
			case Nega_33:
 				str += "Security Access Denied";
				break;
			case Nega_35:
 				str += "Send if the value of the KEY is not valid for the server";
				break;
			case Nega_36:
 				str += "Send if too many attempts with invalid KEY values are requested";
				break;
			case Nega_37:
 				str += "Send if the delay timer is active and a request is transmitted";
				break;
			case Nega_73:
 				str += "wrongBlockSequenceCounter";
				break;
			case Nega_78:
 				str += "Request Correctly Received - ResponsePending";
				break;
			case CAN_STOP:
				str += "Communication is STOP";
				break;
			case 0:
				str += "Reprogramming successfully completed.";
				break;
			default :
				 str += "check the CAN card";  
			}
			str +=_T("\n");
			csFilelog.SeekToEnd();
			csFilelog.WriteString(str);
			csFilelog.Close ();
		}
	}
	
	return 1;
}
/* save current flash data stream .*/
void CCANFLSAHTOOLDlg::OnToolsSavedatastream()
{
	// TODO: 在此添加命令处理程序代码
		LPCTSTR Types;
	Types =
		_T("PTP File (*.dmp)|*.dmp|");
	//Types =
	//	_T("PTP File (*.txt)|*.txt|");
	CFileDialog fileDialog(FALSE,_T("bin"),NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,Types);
 fileDialog.m_ofn.lpstrTitle =_T("Save File");
 	if(fileDialog.DoModal() ==IDOK)
	{	
		CString comPath;
		comPath.Empty();
		comPath = fileDialog.GetPathName();
		 
 		Writedatastream(comPath,0,1);
 	 
		MessageBox(_T("Save Data Stream Ok"),_T("SAVE"));
	 
	}
 SetFocus();
  	
}
 
BYTE CCANFLSAHTOOLDlg::getHexByte(CString& str,int *index)
{ 
	BYTE	value=0;
	int		i,j=0;
	int	len =str.GetLength();

	if(*index<0)
	{
		index=0;
	}
	else if(*index>=len)
	{
		*index=-1;
		return 0;
	}
	
	for(i=*index;(i<len)&&(j<2);i++)
	{
		if(isxdigit(str[i]))
		{
			j++;
			if(str[i]<='9')
				value =(value<<4) | (str[i]-'0');
			else if(str[i]>='A' && str[i]<='F')
				value =(value<<4) | (str[i]-'A' + 10);
			else 
				value =(value<<4) | (str[i]-'a' + 10);
			int	digits=0;
			for(int k=i+1;j==1 && k<len && isxdigit(str[k]);k++) 
				digits++;

			if(digits %2 ==0)
				j++;
		}
		else if(j>0)
		{
			break;
		}
	}

	if(j>0)
		*index=i;
	else
		*index=-1;

	return value;
}
/* deal with command file */
int CCANFLSAHTOOLDlg::readCommandFile(void)
{
	char FileRowBuf[1024];
	int tempOffset = 1;

	
	static int commitflag=0;
	CString strReadW;
	
	if(commitflag == 0)
	{
		commitflag++;
		CString temp;
		temp = "Start Communication...\r\n";
		m_pCANFlash->display(temp,0);
		Flash_Result = CANprotocol->CAN_TesterPresent(1);
		if(Flash_Result != 0)
		{
			temp.Format(_T("Communication Error，Error Code：%d。"),Flash_Result);
			m_pCANFlash->display(temp,1);

			CFailDlg pd(m_pWnd);
			pd.DoModal();

			commitflag=0;
			return -1;
		}

	}

	if(!csFile.ReadString(strReadW))
	{
		commitflag=0;
		return -1;
	}
	//CStringA strReadA(strReadW);
	//strcpy((char *)FileRowBuf,strReadA);

	//RW_Type = FileRowBuf[0];
	CString strU=strReadW;
	strU.MakeUpper();
 	if(strReadW.Find(_T('*')) == 0)
	{
		strU =strReadW.Mid(1);
		strU +="\r\n";
		m_pCANFlash->display(strU,0);
	}
	else if(strReadW.GetLength() ==0)
	return 1;
	else if(strReadW.Find(_T("EXPECTED")) != -1 || strReadW.Find(_T("DISPLAY ")) != -1 )
	{
		strReadW=+"\r\n";
		m_pCANFlash->display(strReadW ,0);
	}
	else if(strU.Find(_T("DELAY"))!= -1)
	{
		CString buf;
		strU=strReadW;
		while(1)
		{
			int n=strU.Find(_T(' '));
 			if(n != -1)
			{
				buf = strU.Mid(n+1);
				strU=buf;
			}
			else break;
		}
		CStringA strReadA(buf);
		strcpy_s((char *)FileRowBuf,1024,strReadA);

		char *p;
		int time=0;
		float delaytime;
		p = FileRowBuf;
		if(p != NULL)
		{
			p+=5;
			
			for(int i=0;i<10;i++)
			{
				if(isxdigit(*p))
				{
 					if(*p<='9')
						time = time *10 + (*p-'0');
				}
				else break;
				p++;
			}			
			//CString buf(FileRowBuf);
			strReadW += "\r\n";
			m_pCANFlash->display(strReadW,0);
			delaytime = (float)time/1000;
			LARGE_INTEGER lgPre,lgCur,lgFrq;
			QueryPerformanceFrequency(&lgFrq);
			double dqFreq;
			dqFreq = (double)lgFrq.QuadPart;
			QueryPerformanceCounter(&lgPre);
			while(1)
			{
				QueryPerformanceCounter(&lgCur);
				if((lgCur.QuadPart-lgPre.QuadPart)/dqFreq >delaytime)break;
			}
			//Sleep(time);
		}
	}
	else
	{
		CAN_msg	canMsg;		
		int num=strReadW.Find(_T(" "));
		CString id=strReadW.Left(num+1);
		canMsg.id=0;
		int		index=0;
		for(int i=0;i<num;i++)
		{
			if(isxdigit(id[i]))
			{
 				if(id[i]<='9')
					canMsg.id =(canMsg.id<<4) | (id[i]-'0');
				else if(id[i]>='A' && id[i]<='F')
					canMsg.id =(canMsg.id<<4) | (id[i]-'A' + 10);
				else 
					canMsg.id =(canMsg.id<<4) | (id[i]-'a' + 10);
			}			 
		}
		canMsg.len=0;

		memset(canMsg.data,0xff,8);
		while((index>=0) && canMsg.len<8)
		{
			canMsg.data[canMsg.len] =getHexByte(strReadW.Mid (num+1),&index);

			if(index>0)
				canMsg.len++;
			 
		}
		canMsg.data[canMsg.len]=0xff;
		canMsg.len = 8;
		canMsg.ch  = 1;

		canMsg.format = CAN_ID_FORMAT_STANDARD;
		canMsg.type   = CAN_FRAME_TYPE_DATA;

		
		CString str;
		CString s;
		for(int i=0;i<8;i++)//msg->len
		{
			str.Format(_T("%02x "),canMsg.data[i]);
			s+=str;
		}
		s+="\r\n";
		m_pCANFlash->display(s,0);
		Flash_Result=CANprotocol->CAN_CommandMessageProc(&canMsg,&num);
		if(Flash_Result == 0 ||(Flash_Result >= 0x11 && Flash_Result <= 0x37) || Flash_Result == 0x73 || Flash_Result == 0x78)
		{
			if(num == 1)
			{
				s.Empty();
				for(int i=0;i<8;i++)//msg->len
				{
					str.Format(_T("%02x "),canMsg.data[i]);
					s+=str;
				}
				s+="\r\n";
				if(Flash_Result == 0)m_pCANFlash->display(s,0);
				else m_pCANFlash->display(s,1);
			}
			else
			{	
				for(int j=0;j<num;j++)
				{
					s.Empty();
					for(int i=0;i<8;i++)//msg->len
					{
						str.Format(_T("%02x "),msg_Arr[j].data[i]);
						s+=str;
					}
					s+="\r\n";
					if(Flash_Result == 0)m_pCANFlash->display(s,0);
					else m_pCANFlash->display(s,1);
				}
			}
		}
		else
		{
			CString temp;
			if(Flash_Result == CAN_STOP)temp="CAN Communication is STOP\r\n";
			else
			temp.Format(_T("Communication Error，Error Code：%d。"),Flash_Result);
			m_pCANFlash->display(temp,1);
			CFailDlg pd(m_pWnd);
			pd.DoModal();

			return -1;
		}
	}
	return 1;
 
}
/*  open a new thread for execute the command */
DWORD WINAPI CCANFLSAHTOOLDlg:: CEcuFlashToolDlgCommandThread(LPVOID lpParameter)
{
	CCANFLSAHTOOLDlg	*dlg=(CCANFLSAHTOOLDlg*)lpParameter;

	dlg->csFile.Open(dlg->m_szCommandPath,CFile::modeRead);
	while(dlg->readCommandFile()>0);
	dlg->csFile.Close();
	return 1;
}
/*  execute the command file */
void CCANFLSAHTOOLDlg::OnToolsCommandfile()
{
	// TODO: 在此添加命令处理程序代码
	LPCTSTR Types;

	Types =
		_T("PTP File (*.cms)|*.cms|")
		_T("PTP File (*.*)|*.*|");
 	CFileDialog dlg(TRUE,_T("txt"),NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,Types);
	dlg.m_ofn.lpstrTitle =_T("Open File");
	if(dlg.DoModal() == IDCANCEL )
	{
		SetFocus();
		return;
	}
	SetFocus();
	AutoCheckCANcard();
	if(isCanFlag == 1)
	{
		m_szCommandPath.Empty();
		m_szCommandPath = dlg.GetPathName();
		g_StopFlag=0;
		CommandThread = ::CreateThread(NULL,
										NULL,
										(LPTHREAD_START_ROUTINE)CEcuFlashToolDlgCommandThread,
										(LPVOID)this,
										NULL,
										NULL); 
		CloseHandle(CommandThread);
	}
	else 
		MessageBox(_T("No CANCard Available!!"),_T("ERROR"));	
}

/* project exit*/
void CCANFLSAHTOOLDlg::OnProjectExit()
{
	// TODO: 在此添加命令处理程序代码
		 OnCancel();
}

/*  about dialog */
void CCANFLSAHTOOLDlg::OnHelpAbout()
{
	// TODO: 在此添加命令处理程序代码
		CAboutDlg dlgAbout;
	dlgAbout.DoModal();
}
//====================================================
//功能：当窗口的大小发生改变时，每个控件尺寸随之改变
//====================================================
void CCANFLSAHTOOLDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	 // UPDATE_EASYSIZE;
	// TODO: 在此处添加消息处理程序代码
	if(nType !=SIZE_MINIMIZED)//SIZE_MINIMIZED
	{  

		changenum++;
		// ChangeSize(IDC_EDIT_ECMTYPE,cx,cy);
		ChangeSize(IDC_COMBO_FILETYPE,cx,cy);
		ChangeSize(IDC_COMBO_ECMTYPE,cx,cy);
		 ChangeSize(IDC_RADIO_CAN2USB,cx,cy);
		ChangeSize(IDC_RADIO_CANCARD2,cx,cy);
		ChangeSize(IDC_RADIO_CANCARDXL,cx,cy);
		ChangeSize(IDC_RADIO_ICAN,cx,cy);
		ChangeSize(IDC_RADIO_ES581,cx,cy);
		ChangeSize(IDC_BUTTON_BUILD,cx,cy);

		ChangeSize(IDC_STATIC_PRG,cx,cy);
		 ChangeSize(IDC_COMBO_PRG,cx,cy);

		ChangeSize(IDC_COMBO_PTP,cx,cy);
		 ChangeSize(IDC_COMBO_CAL,cx,cy);
		ChangeSize(IDC_COMBO_CUT,cx,cy);
		 ChangeSize(IDC_STATIC_BOOT,cx,cy);
		ChangeSize(IDC_STATIC_MFG,cx,cy);
		 ChangeSize(IDC_COMBO_BOOT,cx,cy);
		ChangeSize(IDC_COMBO_MFG,cx,cy);
		 ChangeSize(IDC_BUTTON_BOOT,cx,cy);
		ChangeSize(IDC_BUTTON_MFG,cx,cy);
	 
		 ChangeSize(IDC_BUTTON_APP,cx,cy);
		ChangeSize(IDC_BUTTON_CAL,cx,cy);
		 ChangeSize(IDC_BUTTON_CUT,cx,cy);
 		ChangeSize(IDC_BUTTON_FLASH,cx,cy);
		ChangeSize(IDC_BUTTON_STOP,cx,cy);
		 ChangeSize(IDC_STATIC_INFOR,cx,cy);
		ChangeSize(IDC_STATIC_SETTING,cx,cy);

		 ChangeSize(IDC_CHECK_CLEARNV,cx,cy);
		ChangeSize(IDC_CHECK_READECU,cx,cy);
		 ChangeSize(IDC_STATIC_DIAGNOSTIC,cx,cy);


		ChangeSize(IDC_RICHEDIT21_MINFOR,cx,cy);
		ChangeSize(IDC_STATIC_TIME,cx,cy);
		ChangeSize(IDC_STATIC_STATE,cx,cy);
		 ChangeSize(IDC_STATIC_STATUS,cx,cy);
		ChangeSize(IDC_PROGRESS_FIASH,cx,cy);
		ChangeSize(IDC_STATIC_TIMEPAST,cx,cy);

		ChangeSize(IDC_STATIC_ECMTYPE,cx,cy);
		ChangeSize(IDC_STATIC_CANCARD,cx,cy);
		ChangeSize(IDC_STATIC_APP,cx,cy);
		 ChangeSize(IDC_STATIC_CAL,cx,cy);
		ChangeSize(IDC_STATIC_CUT,cx,cy);
		ChangeSize(IDC_STATIC_FILETYPE,cx,cy);
		ChangeSize(IDC_STATIC,cx,cy);

		ChangeSize(IDC_STATIC_FAIL,cx,cy);
		 ChangeSize(IDC_STATIC_1,cx,cy);
		ChangeSize(IDC_STATIC_PASS,cx,cy);
		ChangeSize(IDC_STATIC_2,cx,cy);
		ChangeSize(IDC_STATIC_FLASH,cx,cy);

		if(changenum>6)changenum=0;
		GetClientRect(&m_rect);//将变化后的对话框大小设为旧大小，否则无法还原控件原始位置
	}


}
/*  控件大小的缩放 */
void  CCANFLSAHTOOLDlg::ChangeSize(UINT nID,int x,int y)
{
	CWnd *pWnd;
	pWnd = GetDlgItem(nID);                       //获取控件句柄
	
	if(pWnd)                                           //判断是否为空，因为对话框创建时会调用此函数，而当时控件还未创建
	{
		 
		if(changenum>6)
		{
			
			CRect rect;                                //获取控件变化前大小
			pWnd->GetWindowRect(&rect);
			ScreenToClient(&rect);                          //将控件大小转换为在对话框中的区域坐标
		
			//　cx/m_rect.Width()为对话框在横向的变化比例
		                               
			rect.left = rect.left * x/ m_rect.Width()+4;      /**//////调整控件大小
			rect.right = rect.right* x/ m_rect.Width()+4;
			rect.top = rect.top * y/ m_rect.Height()+4;
			rect.bottom= rect.bottom* y/ m_rect.Height()+4;
			pWnd->MoveWindow(rect);                         //设置控件大小

		}
		else
		{
		CRect rect;                                //获取控件变化前大小
		pWnd->GetWindowRect(&rect);
		ScreenToClient(&rect);                          //将控件大小转换为在对话框中的区域坐标
		
		//　cx/m_rect.Width()为对话框在横向的变化比例
		                               
		rect.left = rect.left * x/ m_rect.Width();      /**//////调整控件大小
		rect.right = rect.right* x/ m_rect.Width();
		rect.top = rect.top * y/ m_rect.Height();
		rect.bottom= rect.bottom* y/ m_rect.Height();
		pWnd->MoveWindow(rect);                         //设置控件大小
		}
	}
}
/* 重载cancel，save all the data stream*/
void CCANFLSAHTOOLDlg::OnCancel() //重载OnCancel()
{
	/* int n=MessageBox(_T("是否要关闭程序"), NULL,MB_OK|MB_OKCANCEL);
	 if(n == IDOK )*/



	//if(m_pProjectEdit != NULL)delete m_pProjectEdit;
	//if(m_pCANFlash != NULL)delete m_pCANFlash;
	//if(querydlg != NULL)delete querydlg;	 
	//if(m_pDisplay != NULL)delete m_pDisplay;
	//if(m_pCheckSet != NULL)delete m_pDisplay;
	if(m_bflag==1){
		int n=MessageBox(_T("Do you want to close?"),_T("warning"),MB_OKCANCEL);//
	 if(n == IDCANCEL )return;

	}
	if(strLog!=_T(""))
	{
		CString path;
		SYSTEMTIME pcTime;
		GetLocalTime(&pcTime);
			 
		CString strpath=m_szlogDirectory;
		if(GetFileAttributes(m_szlogDirectory)==-1)CreateDirectory(m_szlogDirectory,NULL);
 
 		path.Format(_T("CANflash%02d%02d%02d%02d.dmp"),pcTime.wMonth,pcTime.wDay,pcTime.wHour,pcTime.wMinute);
		strpath+=path;
		if(!csFile.Open(strpath,CFile::modeWrite | CFile::modeCreate))//| CFile::modeCreate
		{
			MessageBox(_T("save dmp log failed"),_T("ERROR"));
			CDialog::OnCancel();
			return;
		}
		DWORD len;
		DWORD size;
	 	if(m_DongleUtility.CheckDllIntegrity())  
		{
			len = m_DongleUtility.EncryptInputLen(strLog);	
			len+=1000;
			BYTE *ptdata = new BYTE[len]; 
			size = m_DongleUtility.EncryptDatastream(strLog,ptdata);	 
			csFile.Write(ptdata,size);
			delete ptdata;
		}
		csFile.Close();
		//MessageBox(strpath);
	}
 	if(!csFilelog.Open(m_szFilelog,CFile::modeWrite | CFile::modeCreate| CFile::modeNoTruncate)) //| CFile::modeCreate
		MessageBox(_T("save log failed"),_T("ERROR"));
	else
	{
		if(csFilelog.GetLength()==0)
		{
		//MessageBox(_T("save log failed"),_T("ERROR"));
			CString str=_T("Time,App File,Cal File,Cut File,Boot file,Mfg File,Kksum(HEX),DelphiSideRailNumber,Result,PassCounter,FailCounter\n");
			csFilelog.WriteString(str);
		}
      csFilelog.SeekToEnd();
	  if(m_nPassNum != 0 || m_nFailNmu != 0)
	  {
		  CString str;
		  str.Format(_T(",,,,,,,,,%d,%d\n"),m_nPassNum,m_nFailNmu);
		  csFilelog.WriteString(str);
	  }
      csFilelog.Close();
   }
	
	if(!csFilelog.Open(m_szProLogName,CFile::modeWrite | CFile::modeCreate)) //| CFile::modeCreate
		MessageBox(_T("save Log failed"),_T("ERROR"));
	else
	{
      CString str;
      csFilelog.WriteString(_T("[Begin]\n"));

	for(int i=0;i<m_nMenuNum;i++)
	{
		csFilelog.WriteString(m_szProLog[i]);
		csFilelog.WriteString(_T("\n"));
	}
	csFilelog.WriteString(_T("[End]\n"));
      csFilelog.Close();
   }

	CDialog::OnCancel();
	
}
/*  display the data stream */
void CCANFLSAHTOOLDlg::OnToolsDisplaydatastream()
{
	// TODO: 在此添加命令处理程序代码
		 
	 displaystreamnum=0;
 	if(Display_flag == 0)//防止多次打开
	{

		testThread = ::CreateThread(NULL,
								   NULL,
								   (LPTHREAD_START_ROUTINE)CEcuFlashToolDlgTestThread,
								   (LPVOID)this,
								   NULL,
								   NULL);	 

	
		CloseHandle(testThread);		 
	}
}
/*  从队列里读取数据*/
CAN_interactMsg *getInteractMsg()//得到数据帧
{
	CAN_interactMsg *msg=NULL;

	WaitForSingleObject(interactMsgMutex,INFINITE);
	
	if(interactMsgHead.next !=NULL)
	{
		msg =interactMsgHead.next;
		interactMsgHead.next =msg->next;

		if(interactMsgHead.next==NULL)
			interactMsgTail=NULL;
	}

	ReleaseMutex(interactMsgMutex); 

	return msg;
}
/*  open a new thread for display data stream */
DWORD WINAPI CCANFLSAHTOOLDlg::CEcuFlashToolDlgTestThread(LPVOID lpParameter)
{
	CCANFLSAHTOOLDlg	*dlg=(CCANFLSAHTOOLDlg*)lpParameter;

	dlg->m_pDisplay = new cdisplay();
	Display_flag=1;
	int nResponse =  dlg->m_pDisplay->DoModal(); 	
  	delete dlg->m_pDisplay;
	return 1;
  
	 
}
/*  放入队列 */
void putInteractMsg(CAN_interactMsg *msg)//放入队列
{
	//static UINT	serialNo=0;

	msg->next	 =NULL;
 	//msg->serialNo=serialNo++;

	WaitForSingleObject(interactMsgMutex,INFINITE);
	
	if(interactMsgHead.next ==NULL)
	{
		interactMsgHead.next=msg;
		interactMsgTail		=msg;
	}
	else
	{
		interactMsgTail->next =msg;
		interactMsgTail		  =msg;
	}
 	SetEvent(interactMsgEvent);//设置
	ReleaseMutex(interactMsgMutex);
}
//把所有的message保存到streamArray数值里，作为displayDataStream使用。
DWORD WINAPI CCANFLSAHTOOLDlg::CEcuFlashToolDlgDisplayAllThread(LPVOID lpParameter)
{
	CCANFLSAHTOOLDlg	*dlg=(CCANFLSAHTOOLDlg*)lpParameter;	
	DWORD		 waitResult=WAIT_TIMEOUT;
	int			 msgNo=0;
	while(1)
	{
 		waitResult =WaitForSingleObject(interactMsgEvent,INFINITE);
		CAN_interactMsg *msg=getInteractMsg();
		CAN_interactMsg	*msgHead=NULL,*msgTail;

		msgNo=0;
		while(msg)
		{
			msgNo++;

			if(msgHead==NULL)
			{
				msgHead=msgTail=msg;
				msg->next=NULL;
			}
			else
			{
				msgTail->next=msg;
				msgTail=msg;
				msg->next =NULL;
			}

			if(msgNo>5000)
			{
				msgNo=0;

 				::SendMessage(dlg->m_hWnd,WM_USER+1,ADDMESS,(LPARAM)msgHead);

				msgHead=NULL;
			}
		 
				msg=getInteractMsg();
		}		
		if(msgHead !=NULL)
			::SendMessage(dlg->m_hWnd,WM_USER+1,ADDMESS,(LPARAM)msgHead);
	}
 	return 1;
	 
} 
/* 把菜单项禁用*/
LRESULT   CCANFLSAHTOOLDlg::OnKickIdle(WPARAM   wp, LPARAM   lCount)   
{  
	if( m_nDongleType != ENTERPRISE_VERSION) 
	{
		CMenu* pMenu=GetMenu (); 
		ASSERT_VALID (pMenu); 
 		pMenu->EnableMenuItem (ID_TOOLS_DISPLAYDATASTREAM,MF_BYCOMMAND | MF_GRAYED); 
	}
	  
	 
	return   0;   
}   


HBRUSH CCANFLSAHTOOLDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性
	 if (pWnd->GetDlgCtrlID() == IDC_STATIC_STATUS)
     {
         pDC->SetTextColor(RGB(0,0,255));
     }
	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}

/*  复选框 clear nv */
void CCANFLSAHTOOLDlg::OnBnClickedCheckClearnv()
{
	// TODO: 在此添加控件通知处理程序代码

	if( ((CButton *)(GetDlgItem(IDC_CHECK_CLEARNV)))->GetCheck() )
	{
		m_bNVFlag  = 1;
 	}
	else
	{
		m_bNVFlag  = 0;
 
	}
}

/*  复选框 read ecu */
void CCANFLSAHTOOLDlg::OnBnClickedCheckReadecu()
{
	// TODO: 在此添加控件通知处理程序代码
		if( ((CButton *)(GetDlgItem(IDC_CHECK_READECU)))->GetCheck() )
	{
		m_bECUFlag  = 1;
 	}
	else
	{
		m_bECUFlag  = 0;
 
	}
}

 
void CCANFLSAHTOOLDlg::OnBnClickedButtonBoot()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);

	LPCTSTR Types;
	Types =
		_T("PTP File (*.CUT;*.PTP)|*.CUT;*.PTP|")
		_T("All Files (*.*)|*.*|");
	CFileDialog dlg(TRUE,_T("bin"),NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,Types);
	dlg.m_ofn.lpstrTitle =_T("Open File");
	if(dlg.DoModal() == IDOK )
	{  
 		m_szBootPath = dlg.GetPathName();
 		AddComboFile(BOOT,m_szBootPath);
	
 	}
	SetFocus();
}


void CCANFLSAHTOOLDlg::OnBnClickedButtonMfg()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);

	LPCTSTR Types;
	Types =
		_T("PTP File (*.CUT;*.S19)|*.CUT;*.S19|")
		_T("All Files (*.*)|*.*|");
	CFileDialog dlg(TRUE,_T("bin"),NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,Types);
	dlg.m_ofn.lpstrTitle =_T("Open File");
	if(dlg.DoModal() == IDOK )
	{  
 		m_szMfgPath = dlg.GetPathName();
 		AddComboFile(MFG,m_szMfgPath);
 	}
	SetFocus();

}

/*  build cut file*/
void CCANFLSAHTOOLDlg::OnToolsBuildcutfile()
{
	// TODO: 在此添加命令处理程序代码
	if(!(CheckFlashItems()))//
	{
 		return;
	}
	if(buildreturn == 1){MessageBox(_T("The button buildcut is running,wait.."));return;}
	if(g_EcuFile == CAL){MessageBox(_T("CAL file can not build"));return;}
	else if(g_EcuFile == BOOT){MessageBox(_T("BOOT file can not build"));return;}
	else if(g_EcuFile == MFG){MessageBox(_T("MFG file can not build"));return;}
	else if(g_EcuFile == TSW){MessageBox(_T("TSW file can not build"));return;}
	if(g_EcuFile == CUT)
	{
		int result=OperateFile(0);
		if(result==-1)return;
	}
	m_pbuildcut = new CBuildCut();
	if(g_EcuFile == CUT)
	{
		m_pbuildcut->calstr=m_szCutPath;
	}
	else
	{
		m_pbuildcut->appstr=m_szPtpPath;
		m_pbuildcut->calstr=m_szCalPath;
	}
	m_pbuildcut->flag=g_EcuFile; 
	m_pbuildcut->mixflag = config->ECUprg.b_cal_app;
	int nResponse = m_pbuildcut->DoModal();
	if(nResponse != IDOK)return;
 		if(g_EcuFile == PTP_CAL) GetDlgItem(IDC_BUTTON_BUILD)->EnableWindow(false);
		GetDlgItem(IDC_BUTTON_FLASH)->EnableWindow(false);
	LPCTSTR Types;	 
 	Types =
		_T("PTP File (*.cut)|*.cut|");
	CFileDialog fileDialog(FALSE,_T("txt"),NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,Types);
	fileDialog.m_ofn.lpstrTitle =_T("Save File");//,theApp.GetMainWnd()->GetWindow  (GW_HWNDPREV),NULL

	if(fileDialog.DoModal() ==IDCANCEL)
	{SetFocus();return;}
	SetFocus();
	if(config->ChecksumNum > 0 && config->ECUprg.b_cal_app == MIXED)//
	{
		m_bMergeFlag=0;
		MergeActionItemCurrentValue();
	}
	
 	m_szBuildPath = fileDialog.GetPathName();
	BuildCutThread = ::CreateThread(NULL,
							NULL,
							(LPTHREAD_START_ROUTINE)CCANFLSAHTOOLDlgBuildCutThreadThread,
							(LPVOID)this,
							NULL,
							NULL);
	
	CloseHandle(BuildCutThread); 
	 
}
 /* set current special action dialog value */
int CCANFLSAHTOOLDlg::SetActionItemCurrentValue(int count,int type)
{
	CString str;	 
	int i=0;
	int index=0;
 
	unsigned char buf[100];
	u32 bAddress;
	u32 bSize;
	m_bMergeFlag=0;
	
	i=count; 
	bAddress=0;
 
	CStringA strA(ItemInfor.ActionItem[i].Address);//地址
	strcpy_s((char *)buf,100,strA);
	config->HextoAdd((char *)buf,&bAddress);

	index=0;//偏移值
	for(int j=0;j<config->banknum;j+=2)
	{
		if(bAddress >= config->ECUprg.Bankaddr[j]&& bAddress <= config->ECUprg.Bankaddr[j+1])						
		{
			index += bAddress - config->ECUprg.Bankaddr[j];
			break;
		}
		else if(bAddress >= config->ECUprg.Bankaddr[j]&& bAddress >= config->ECUprg.Bankaddr[j+1])//gai
		{
			index += config->ECUprg.Bankaddr[j+1]-config->ECUprg.Bankaddr[j]+1;

		}
	}
				 
	int bNum=0;
	getSize(ItemInfor.ActionItem[i].Size,&bSize);	//长度
	if(type == 0)//merge
	{
		CStringA cstrA(ItemInfor.ActionItem[i].Value);//得到设定值
		strcpy_s((char *)buf,100,cstrA);
		int len  = strlen((const char *)buf);
		config->CharToHex(buf);
		int tempOffset = 0;
		if(len %2 != 0)
		{
			cut_data[0] = buf[tempOffset];
			tempOffset++;
			for(unsigned short i = 1;i < bSize;i++)
			{
				cut_data[i] = buf[tempOffset];
				tempOffset++;
				cut_data[i] <<= 4;
				cut_data[i] += buf[tempOffset];
				tempOffset++;
			}
		}
		else
		{
		for(unsigned short i = 0;i < bSize;i++)
		{
			cut_data[i] = buf[tempOffset];
			tempOffset++;
			cut_data[i] <<= 4;
			cut_data[i] += buf[tempOffset];
			tempOffset++;
		}
		}		
		memcpy(&Calibration.totalTransData.FileData[index],cut_data,bSize);//根据地址改 值（merge）
	}
	else 
		memcpy(cut_data,&Calibration.totalTransData.FileData[index],bSize); //display
	cut_data[bSize]=0;
	str.Empty();
	ItemInfor.ActionItem[i].CurrentValue.Empty();
	CString strValue;
	for(u32 n=0;n<bSize;n++)
	{
		strValue.Format(_T("%02X"),(unsigned char)cut_data[n]);
		ItemInfor.ActionItem[i].CurrentValue += strValue;
	}
	return 1;
}
int CCANFLSAHTOOLDlg::ComputeChecksum()
{
	int dummy;
	return 0;
}
/*
merge属性的要把修改后的值写到文件里
*/
int CCANFLSAHTOOLDlg::MergeActionItemCurrentValue()
{
	CString str;	 
	int i=0;
	int index=0;
	m_bMergeFlag=0;
	//修改了buf里数据
	
	unsigned int size;
	unsigned int addr=0;
	for(i=0;i<ItemInfor.ItemNum;i++)//读取special action editor里
	{
		if(ItemInfor.ActionItem[i].Label != "KKSUM")//不是checksum
		{ 
			if(ItemInfor.ActionItem[i].ActionType == "Merge")//merge
			{
				SetActionItemCurrentValue(i,0);//0为merge 
				m_bMergeFlag=1;
			}
		}
	}

	
	if(m_bMergeFlag == 1)//修改后写入文件
	{
  		CString strpath=m_szExePath;
		strpath+="temp.txt";
		bool res=csFilecut.Open(strpath,CFile::modeWrite | CFile::modeCreate| CFile::typeBinary);
		 
		csFilecut.Write(Calibration.S0_Data,strlen(Calibration.S0_Data));
 		for(int i=0;i<config->banknum;i+=2)
		{				
			size=config->ECUprg.Bankaddr[i+1]-config->ECUprg.Bankaddr[i]+1;
			CombinationCUT(&Calibration.totalTransData.FileData[addr],size,config->ECUprg.Bankaddr[i]);
			addr+=size;
		}
		if(Calibration.S7_Flag==1)csFilecut.Write(Calibration.S7_Data,strlen(Calibration.S7_Data));

		csFilecut.Close();

		pOpFile->GetTransData(strpath,CUTFILE);//算出checksum值
	}
 
	 for(i=0;i<ItemInfor.ItemNum;i++)//kksum merge
	 { 
		 
		 if(ItemInfor.ActionItem[i].Label == "KKSUM"&&ItemInfor.ActionItem[i].ActionType == "Merge")
		 {
			config->m_checksum[0].Checksum=config->Checksum;
			config->m_checksum[0].FileChecksum=config->FileChecksum;

			 ItemInfor.ActionItem[i].Value.Format(_T("%04X"),config->Checksum);
			 SetActionItemCurrentValue(i,0);//0为merge
		
		 
		 }
	 }

	return 1;

} 
/*gserialNo:每次刷新时message的总条数。
streamArray：作为显示的buf。
*/
LRESULT CCANFLSAHTOOLDlg::onAddCANMsgToList(WPARAM flag, LPARAM lparam)
{
	if(flag==ADDMESS)
	{
 
		WaitForSingleObject(displayMsgMutex,INFINITE);
	
		CAN_interactMsg *interactmsg=(CAN_interactMsg*)lparam;

		while(interactmsg!=NULL)
		{
			//interactmsg->serialNo =gserialNo;

			streamArray[gserialNo++]=interactmsg;
			interactmsg=interactmsg->next;
		}
		ReleaseMutex(displayMsgMutex);
	}
	else if(flag == FAIL)
	{
		m_nFailNmu++;
		CString str;
		str.Format(_T("%d"), m_nPassNum);
		GetDlgItem(IDC_STATIC_PASS)->SetWindowTextW(str);
		str.Format(_T("%d"), m_nFailNmu);
		GetDlgItem(IDC_STATIC_FAIL)->SetWindowTextW(str);
 		m_bflag=0;
		CAN_interactMsg *interactmsg=new CAN_interactMsg;
		interactmsg->MsgState=MSGERR;
 
		sprintf_s(interactmsg->data,"%s","ECM Reprogram Failed!\r\n"); 
		interactmsg->Iswrite = 0;
		displayArray[listItemNum]=interactmsg;
		listItemNum++;
		m_faildlg.DoModal();
		GetDlgItem(IDC_BUTTON_FLASH)->EnableWindow(true);
		GetDlgItem(IDC_BUTTON_FLASH)->SetFocus();
		Writedatastream(_T("log.txt"),1,Flash_Result);
 		choose_flag=0;
	}
	else if(flag == SUCCESS)
	{
		m_nPassNum++;
		CString str;
		str.Format(_T("%d"), m_nPassNum);
		GetDlgItem(IDC_STATIC_PASS)->SetWindowTextW(str);
		str.Format(_T("%d"), m_nFailNmu);
		GetDlgItem(IDC_STATIC_FAIL)->SetWindowTextW(str);	
 		CAN_interactMsg *interactmsg=new CAN_interactMsg;
	 
		interactmsg->MsgState=MSGINFO ;
 		interactmsg->Iswrite = 0;
		sprintf_s(interactmsg->data,"%s"," ECM Reprogram Successfully!\r\n"); 
		displayArray[listItemNum]=interactmsg;
		listItemNum++;
		m_successdlg.DoModal();
		GetDlgItem(IDC_BUTTON_FLASH)->EnableWindow(true);
		GetDlgItem(IDC_BUTTON_FLASH)->SetFocus();
 	}
	else
	{
#if USBDOG

#else
 	  bool flag;	 
	 
			if(m_DongleUtility.CheckDllIntegrity())  
			{
				flag = m_DongleUtility.DelphiDongleCheck((DONGLE_VERSION)m_nDongleType);
				for(int i=0;i<5&&flag==false;i++)
				{
					flag = m_DongleUtility.DelphiDongleCheck((DONGLE_VERSION)m_nDongleType);
					Sleep(1000);
				}
				if(flag == false)
				{
 						MessageBox(_T("The software is not authorised"),_T("ERROR"),MB_ICONERROR|MB_OK);
			 		HWND vhWnd=NULL;
					vhWnd=::FindWindow(NULL,_T("Open File"));//CFileDialog
					if(vhWnd != NULL)::SendMessage(vhWnd,WM_CLOSE,(WPARAM)(LPCTSTR)"",16); 
  
 					vhWnd=::FindWindow(NULL,_T("Save File"));//CFileDialog
 					if(vhWnd != NULL) ::SendMessage(vhWnd,WM_CLOSE,(WPARAM)(LPCTSTR)"",16); 
 
					vhWnd=::FindWindow(NULL,_T("Project Editor"));//CFileDialog
 					if(vhWnd != NULL) ::SendMessage(vhWnd,WM_CLOSE,(WPARAM)(LPCTSTR)"",16); 
 	
					vhWnd=::FindWindow(NULL,_T("Special Action Setting"));//CFileDialog
 					if(vhWnd != NULL) ::SendMessage(vhWnd,WM_CLOSE,(WPARAM)(LPCTSTR)"",16); 

						vhWnd=::FindWindow(NULL,_T("Datastream"));//CFileDialog
 					if(vhWnd != NULL) ::SendMessage(vhWnd,WM_CLOSE,(WPARAM)(LPCTSTR)"",16); 
	
					vhWnd=::FindWindow(NULL,_T("Begin to m_pCANFlash"));//CFileDialog
 					if(vhWnd != NULL) ::SendMessage(vhWnd,WM_CLOSE,(WPARAM)(LPCTSTR)"",16); 
	
					vhWnd=::FindWindow(NULL,_T("Edit Special Check Item"));//CFileDialog
 					if(vhWnd != NULL) ::SendMessage(vhWnd,WM_CLOSE,(WPARAM)(LPCTSTR)"",16); 
	
					vhWnd=::FindWindow(NULL,_T("About CAN FlashTool"));//CFileDialog
 					if(vhWnd != NULL) ::SendMessage(vhWnd,WM_CLOSE,(WPARAM)(LPCTSTR)"",16); 

						vhWnd=::FindWindow(NULL,_T("ERROR"));//CFileDialog
 					if(vhWnd != NULL) ::SendMessage(vhWnd,WM_CLOSE,(WPARAM)(LPCTSTR)"",16); 
						vhWnd=::FindWindow(NULL,_T("SAVE"));//CFileDialog
 					if(vhWnd != NULL) 
 					::SendMessage(vhWnd,WM_CLOSE,(WPARAM)(LPCTSTR)"",16); 	
 						 OnCancel();	
  			 
			}  
 	  
		}
#endif
	}
	return 1;
}

 
void CCANFLSAHTOOLDlg::OnProjectSaveprojectas()
{
	// TODO: ÔÚ´ËÌí¼ÓÃüÁî´¦Àí³ÌÐò´úÂë
	LPCTSTR Types;
	Types =
		_T("FLT File (*.cpj)|*.cpj|");
	CFileDialog fileDialog(FALSE,_T("txt"),NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,Types);
	fileDialog.m_ofn.lpstrTitle =_T("Save File");
	

 	if(fileDialog.DoModal() ==IDOK)
	{
		m_szSaveFltfPath.Empty();
		m_szSaveFltfPath = fileDialog.GetPathName();
		Saveproject(m_szSaveFltfPath);
		AddPrgToMenu(m_szSaveFltfPath);
	}
 		
	 SetFocus(); 

}

//处理回车消息。只有当flash按钮按回车才执行。其他的按tab消息来处理
BOOL CCANFLSAHTOOLDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ÔÚ´ËÌí¼Ó×¨ÓÃ´úÂëºÍ/»òµ÷ÓÃ»ùÀà
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)  
	{  
		//    HWND hWnd=::GetFocus();
 		//iID=::GetDlgCtrlID(hWnd);
		CWnd *wnd = GetFocus ();  

		if (wnd != NULL)  
		{  
			UINT i = wnd->GetDlgCtrlID ();//IDC_BUTTON_FLASH;  
			if (i == IDC_BUTTON_FLASH)  
			{  
				SendMessage (WM_COMMAND, i, (LPARAM)wnd->m_hWnd);  
				return TRUE;  
			}  
		}  
		CWnd *mwnd = GetNextDlgTabItem (wnd);  
		if (mwnd)  
		{  
			mwnd->SetFocus();  
			return TRUE;  
		}  
	}  
	 //m_ctrlTT.RelayEvent(pMsg); 
	 //  m_ctrlTT.SetTipBkColor(RGB(0,0,192));//font color RGB( 255,0,192)
  m_ctrlTT.SetTipBkColor( RGB( 255,0,192));

	// CString str;
	////str.Format(_T("x=%d, y=%d"), pMsg->pt.x, pMsg->pt.y);
	//str=m_szConfigPath;
	//m_ctrlTT.AddTool(GetDlgItem(IDC_COMBO_PRG), str);
	 
	//str=m_szPtpPath;
 //	m_ctrlTT.AddTool(GetDlgItem(IDC_COMBO_PTP), str);
	return CDialogEx::PreTranslateMessage(pMsg);
}

BOOL CCANFLSAHTOOLDlg::SetTipText( UINT id, NMHDR * pTTTStruct, LRESULT * pResult )
{
 TOOLTIPTEXT *pTTT = (TOOLTIPTEXT *)pTTTStruct;    
 UINT nID =pTTTStruct->idFrom; //得到相应窗口ID，有可能是HWND
  
 if (pTTT->uFlags & TTF_IDISHWND)    //表明nID是否为HWND
 {
         nID = ::GetDlgCtrlID((HWND)nID);//从HWND得到ID值，当然你也可以通过HWND值来判断
		switch(nID)
		{
			case(IDC_COMBO_PRG):
				//m_szConfigPath
				pTTT->lpszText=m_szConfigPath.AllocSysString();       
 				return TRUE;
				break;
			case(IDC_COMBO_PTP):
			//设置相应的显示字串
				pTTT->lpszText=m_szPtpPath.AllocSysString();      
				return TRUE;
				break;
			case(IDC_COMBO_CAL):
			//设置相应的显示字串
				pTTT->lpszText=m_szCalPath.AllocSysString();      
				return TRUE;
				break;
			case(IDC_COMBO_CUT):
			//设置相应的显示字串
				pTTT->lpszText=m_szCutPath.AllocSysString();      
				return TRUE;
				break;


		}
 }
 return(FALSE);
}

void CCANFLSAHTOOLDlg::CombinationCUTtoCString(unsigned char buf[],unsigned int size,unsigned int add)
{CString axx;
	int linenum=size/16;
	CString stradd;
 	unsigned char cksum=0; 
	CString strdata;
	CString strs;
	CString data;
	int reusult;
	int addlen;
	char buffer[100];
	if(add>0xFFFFFF){strs="S315";addlen=3;}
	else if(add>0xFFFF){strs="S214";addlen=2;}
	else if(add>0xFF){strs="S113";addlen=1;}
	else {strs="S014";addlen=0;}
	//strs="S315";addlen=3;
		
CString strline;
 	for(int i=0;i<linenum;i++)
	{
		strline.Empty();
		
		cksum=0x12+addlen; 
		strline =strs.Left(4);
		
		if(add>0xFFFFFF){ 
			stradd.Format(_T("%08X"),add);}
		else if(add>0xFFFF){
			stradd.Format(_T("%06X"),add);		
		}
		else if(add>0xFF){
			stradd.Format(_T("%04X"),add);}
		//stradd.Format(_T("%08X"),add);	
		strline += stradd;

		for(int k=0;k<4;k++)
		{
			cksum += (add>>k*8)&0xff;
		}
		for(int j=0;j<16;j++)
		{
			strdata.Format(_T("%02X"),buf[i*16+j]);
			strline += strdata;
			cksum += buf[i*16+j];
		}		
		cksum = 0xff - cksum&0xff;
		add+=0x10;
		strdata.Format(_T("%02X"),cksum);
		strline+=strdata;
		strline+="\r\n";
	//	CStringA strlineA(strline);
 	 
	//strcpy_s((char *)buffer,100,strlineA);
	//csFilecut.Write(buffer,strline.GetLength());
	g_strArray.Add(strline);
	}
}
void CCANFLSAHTOOLDlg::BuildcutAndComputeCheckSum()
{
	// TODO: ÔÚ´ËÌí¼Ó¿Ø¼þÍ¨Öª´¦Àí³ÌÐò´úÂë
	 char len;
	unsigned int size;
	unsigned int addr=0;
	int flag=0;
	u32 add[2];
	u32 index;
 
	unsigned calSum;
 	g_strArray.RemoveAll();
					 
 	//if(g_EcuFile == PTP_CAL)
	{
		for(int j=0;j<config->ChecksumNum;j++)//校验值的个数
		{
			config->m_checksum[j].Checksum=0;
			for(int m=0;m<config->WordsumNum[0];m+=2)
			{
				for(int i=0;i<config->banknum;i+=2)
				{
					add[0]=config->ECUprg.Bankaddr[i];
					add[1]=config->ECUprg.Bankaddr[i+1];
					if(config->ECUprg.Wordsumaddr[j][m] >=add[0]&&config->ECUprg.Wordsumaddr[j][m]<=add[1])
					{
						if(g_EcuFile == CUT)	
							pOpFile->ReadDataFromFileTobuf_buildcut(m_szCutPath,add,0);
						else
						{
							pOpFile->ReadDataFromFileTobuf_buildcut(m_szPtpPath,add,0);
							pOpFile->ReadDataFromFileTobuf_buildcut(m_szCalPath,add,1);
						}
						size=config->ECUprg.Bankaddr[i+1]-config->ECUprg.Bankaddr[i]+1;
 
 						CombinationCUTtoCString(Calibration.totalTransData.FileData,size,config->ECUprg.Bankaddr[i]);
  						pOpFile->ReadDataFromCStringArrayTobuf_Checksum(add,1,j,1,&calSum);
						//index = config->ECUprg.ChecksumAddr[j] -add[0];
 						config->m_checksum[j].Checksum += calSum;
					}
 
				}
			}
			index = config->m_checksum[j].ChecksumAddr-add[0];
			config->Checksum = 0xffff-config->m_checksum[j].Checksum+1;
			config->m_checksum[j].Checksum=config->Checksum;
			config->m_checksum[j].FileChecksum=config->FileChecksum;

		}
 		if(g_EcuFile == PTP_CAL)
		{
			for(int i=0;i<config->banknum;i+=2)
			{	
				add[0]=config->ECUprg.Bankaddr[i];
				add[1]=config->ECUprg.Bankaddr[i+1];
				if(g_EcuFile == CUT)	
					pOpFile->ReadDataFromFileTobuf_buildcut(m_szCutPath,add,0);
				else
				{
					pOpFile->ReadDataFromFileTobuf_buildcut(m_szPtpPath,add,0);
					pOpFile->ReadDataFromFileTobuf_buildcut(m_szCalPath,add,1);
				}
 				size=config->ECUprg.Bankaddr[i+1]-config->ECUprg.Bankaddr[i]+1;
 			//	for(int j=0;j<config->ChecksumNum;j++)
				//{
				//	if(config->ECUprg.ChecksumAddr[j] >=add[0]&& config->ECUprg.ChecksumAddr[j]<=add[1])
				//	{
				//		index = config->m_checksum[j].ChecksumAddr-add[0];
				//		if(config->ECUprg.ChecksumOrder[j] == 1)//大端
				//		{
				//			Calibration.totalTransData.FileData[index]=(config->m_checksum[j].Checksum>>8)&0xff; 
				//			Calibration.totalTransData.FileData[index+1]=config->m_checksum[j].Checksum&0xff; 
				//		}
				//		else
				//		{
				//			Calibration.totalTransData.FileData[index]=config->m_checksum[j].Checksum&0xff; 
				//			Calibration.totalTransData.FileData[index+1]=(config->m_checksum[j].Checksum>>8)&0xff; 
				//		}

				//	}
				//}
				CombinationCUTtoCString(Calibration.totalTransData.FileData,size,config->ECUprg.Bankaddr[i]);
			} 
 		}
	}
}

/*
生成cut文件，分为MIXED（cut），separated（cut，app，cal）。
*/
DWORD WINAPI CCANFLSAHTOOLDlg:: CCANFLSAHTOOLDlgBuildCutPreThreadThread(LPVOID lpParameter)
{
	CCANFLSAHTOOLDlg	*dlg=(CCANFLSAHTOOLDlg*)lpParameter;
  	dlg->BuildcutAndComputeCheckSum();
	dlg->buildreturn=0;
	 return 1;
}
void CCANFLSAHTOOLDlg::OnBnClickedButtonBuild()
{
	// TODO: ÔÚ´ËÌí¼Ó¿Ø¼þÍ¨Öª´¦Àí³ÌÐò´úÂë
	CString str;
 	int i, length;
  	int result; 
	buildreturn=0;
	
	if((CheckFlashItems()) == false)//
	{
 		return;
	}
 	else
	{
			
		result=OperateFile(0);
		if(result == -1)
		{
			
			return;
		}
		GetDlgItem(IDC_BUTTON_FLASH)->EnableWindow(false);
		buildreturn=1;
		BuildCutPreThread = ::CreateThread(NULL,
							NULL,
							(LPTHREAD_START_ROUTINE)CCANFLSAHTOOLDlgBuildCutPreThreadThread,
							(LPVOID)this,
							NULL,
							NULL);	 

	
		CloseHandle(BuildCutPreThread);		
		
		 GetDlgItem(IDC_BUTTON_BUILD)->EnableWindow(false);
		 GetDlgItem(IDC_BUTTON_FLASH)->EnableWindow(true);
		 GetDlgItem(IDC_BUTTON_FLASH)->SetFocus();	

		

	}

}
int CCANFLSAHTOOLDlg::CollectUSBInfo()  
{  
    // 获取当前系统所有使用的设备  
	int result;
    DWORD dwFlag = (DIGCF_ALLCLASSES | DIGCF_PRESENT);  
   HDEVINFO hDevInfo = SetupDiGetClassDevs(NULL, NULL, NULL, dwFlag);  
   if( INVALID_HANDLE_VALUE == hDevInfo )  
   {  
       AfxMessageBox( _T("获取系统设备列表失败") );  
       return 0;  
   }  
  
    // 准备遍历所有设备查找USB  
    SP_DEVINFO_DATA sDevInfoData;  
    sDevInfoData.cbSize = sizeof(SP_DEVINFO_DATA);  
  
  
   //VID: ZYZW  公司的生产商号  
    const TCHAR NIKON_ID[] = _T("XYZW");  
   CString strText;  
   TCHAR szDIS[MAX_PATH]; // Device Identification Strings,   
   DWORD nSize = 0 ;  
   for(int i = 0; SetupDiEnumDeviceInfo(hDevInfo,i,&sDevInfoData); i++ )  
   {  
       nSize = 0;  
       if ( !SetupDiGetDeviceInstanceId(hDevInfo, &sDevInfoData, szDIS, sizeof(szDIS), &nSize) )  
       {  
            AfxMessageBox( _T("获取设备识别字符串失败") );  
            break;  
       }  
  
    // 设备识别串的前三个字符是否是"USB", 模板： USB\VID_XXXX&PID_XXXX\00000xxxxxxx  
    CString strDIS( szDIS );  
    strDIS.MakeUpper();  
    if( strDIS.Left( 3 ) == _T("USB") )  
    {  
       strText += strDIS;  
	   if(strText.Find(_T("VID_08E2&PID_0002"))!=-1)
	   {
			return 1;
	   }
       strText += _T("\r\n");  
       //int iVID_Pos = strDIS.Find( NIKON_ID );  
       //if( iVID_Pos == 8 )  
       //{  
       //   // VID: 厂商号  
       //    //  m_wndEditVID.SetWindowText( NIKON_ID  );  
       //   // PID :产品号  
       //     int iSlashPos = strDIS.ReverseFind(_T('\\'));  
       //   int iPID_Pos = strDIS.Find( _T("PID_") );  
       //   CString strProductID = strDIS.Mid( iPID_Pos+4, iSlashPos - iPID_Pos - 4 );  
       //  // m_wndEditPID.SetWindowText( strProductID  );  
       //   // 序列号  
       //    int iRight = strDIS.GetLength() - iSlashPos  -1;  
       //  CString strSerialNumber = strDIS.Right( iRight );  
       // // m_wndEditNUM.SetWindowText( strSerialNumber );  
       //}  
    }  
  }  
  
 // m_wndEditENUM.SetWindowText( strText );  
  
   // 释放设备  
   SetupDiDestroyDeviceInfoList(hDevInfo);  
   return 0;
}  


void CCANFLSAHTOOLDlg::OnBnClickedButtonTsw()
{
	// TODO: ÔÚ´ËÌí¼Ó¿Ø¼þÍ¨Öª´¦Àí³ÌÐò´úÂë
	UpdateData(true);

	LPCTSTR Types;
	Types =
		_T("PTP File (*.CUT;*.S19)|*.CUT;*.S19|")
		_T("All Files (*.*)|*.*|");
	CFileDialog dlg(TRUE,_T("bin"),NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,Types);
	dlg.m_ofn.lpstrTitle =_T("Open File");
	if(dlg.DoModal() == IDOK )
	{  
 		m_szTswPath = dlg.GetPathName();
 		AddComboFile(TSW,m_szTswPath);
 	}
	SetFocus();
}


void CCANFLSAHTOOLDlg::OnSelchangeComboTsw()
{
	// TODO: ÔÚ´ËÌí¼Ó¿Ø¼þÍ¨Öª´¦Àí³ÌÐò´úÂë
	UpdateData(false);
	int ctrl = m_ComboTsw.GetCurSel();
	m_szMfgPath = m_ComboPath.szTswBuf[ctrl];
	for(int i=ctrl;i>0;i--)
	{
		m_ComboPath.szTswBuf[i]= m_ComboPath.szTswBuf[i-1];
	}
	m_ComboPath.szTswBuf[0]=m_szTswPath;
	m_ComboTsw.ResetContent();
 	 
	for(int j=0;j<m_ComboPath.nMfgNum;j++)
	{
		m_ComboTsw.AddString(m_ComboPath.szTswBuf[j]);
		m_ComboTsw.SetItemData(j,j+1);
	}		
	m_ComboTsw.SetCurSel(0);



}
