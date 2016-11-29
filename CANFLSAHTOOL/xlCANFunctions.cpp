/*----------------------------------------------------------------------------
| File        : xlCANFunctions.cpp
| Project     : CANFlashtool 
|
| Description : Shows the basic CAN functionality for the XL Driver Library
|-----------------------------------------------------------------------------
| $Author: liaoshaoyan      $Revision:  
| $Data    2014.04.09   
|-----------------------------------------------------------------------------*/
 

#include "stdafx.h"
 
#include "xlCANFunctions.h"
#include "debug.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// globals
//////////////////////////////////////////////////////////////////////

TStruct g_th;
BOOL    g_bThreadRun;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCANFunctions::CCANFunctions()
{
  m_xlChannelMask[CHAN01] = 0;
  m_xlChannelMask[CHAN02] = 0;
}

CCANFunctions::~CCANFunctions()
{
  CloseHandle(m_hThread);
  CloseHandle(m_hMsgEvent);
}

////////////////////////////////////////////////////////////////////////////

//! CANInit

//! Open the driver, get the channelmasks and create the RX thread.
//! 
//!
//////////////////////////////////////////////////////////////////////////// 

XLstatus CCANFunctions::CANInit()
{
  XLstatus        xlStatus = XL_ERROR;
  char            tmp[100];

  xlStatus = xlOpenDriver();
  sprintf_s(tmp, "xlOpenDriver, stat: %d", xlStatus);
  DEBUG(DEBUG_ADV, tmp);
  if (xlStatus != XL_SUCCESS) return xlStatus;

  // ---------------------------------------
  // Get/Set the application within VHWConf
  // ---------------------------------------
  xlStatus = canGetChannelMask();
  if ( (xlStatus) ) return XL_ERROR;
  if( (m_xlChannelMask[CHAN01] == 0) && (m_xlChannelMask[CHAN02] == 0) ) return XL_ERROR;
  //
  // ---------------------------------------
  // Open ONE port for both channels
  // ---------------------------------------
  xlStatus = canInit();
  if (xlStatus) return xlStatus;

  // ---------------------------------------
  // Create ONE thread for both channels
  // ---------------------------------------
   xlStatus = canCreateRxThread();
  if (xlStatus) return xlStatus;

  return xlStatus;
}

////////////////////////////////////////////////////////////////////////////

//! canGetChannelMask

//! parse the registry to get the channelmask
//!
//////////////////////////////////////////////////////////////////////////// 

XLstatus CCANFunctions::canGetChannelMask()
{
  XLstatus        xlStatus = XL_ERROR;
  char            tmp[100];

  // default values
  unsigned int  hwType     = 0;
  unsigned int  hwIndex    = 0;
  unsigned int  hwChannel  = 0;

  unsigned int  busType    = XL_BUS_TYPE_CAN;   
  unsigned int  i; 
 
  XLdriverConfig  xlDrvConfig;

   //check for hardware:
  xlStatus = xlGetDriverConfig(&xlDrvConfig);
  if (xlStatus) return xlStatus;
  
  // we check only if there is an application registered or not.
  xlStatus = xlGetApplConfig("xlCA", CHAN01, &hwType, &hwIndex, &hwChannel, busType); //xlCANcontrol
 appChannel=0;
  // Set the params into registry (default values...!)
  if (xlStatus) {
    DEBUG(DEBUG_ADV,"set in VHWConf");

    for (i=0; i < xlDrvConfig.channelCount; i++) {

      sprintf_s (tmp, "hwType: %d, bustype: %d, hwChannel: %d, cap: 0x%x", 
        xlDrvConfig.channel[i].hwType, 
        xlDrvConfig.channel[i].connectedBusType,
        xlDrvConfig.channel[i].hwChannel,
        xlDrvConfig.channel[i].channelBusCapabilities);
      DEBUG(DEBUG_ADV,tmp);

      // we search not the first CAN cabs
      if ( (xlDrvConfig.channel[i].channelBusCapabilities & XL_BUS_ACTIVE_CAP_CAN) && (appChannel < 2) ) {
		   
		 CString str(xlDrvConfig.channel[i].name);
		  if(str.Left(7)=="Virtual")break;

        hwType    = xlDrvConfig.channel[i].hwType;
        hwIndex   = xlDrvConfig.channel[i].hwIndex;
        hwChannel = xlDrvConfig.channel[i].hwChannel;

        xlStatus = xlSetApplConfig( // Registration of Application with default settings
        "xlCAN",             // Application Name
        appChannel,                 // Application channel 0 or 1
        hwType,                     // hwType  (CANcardXL...)    
        hwIndex,                    // Index of hardware (slot) (0,1,...)
        hwChannel,                  // Index of channel (connector) (0,1,...)
        busType);                   // the application is for CAN.

        m_xlChannelMask[appChannel] = xlGetChannelMask(hwType, hwIndex, hwChannel);
        sprintf_s (tmp, "Register CAN hWType: %d, CM: 0x%I64x", hwType, m_xlChannelMask[appChannel]);
        DEBUG(DEBUG_ADV,tmp);

      //  m_pHardware->InsertString(-1, xlDrvConfig.channel[i].name);

        appChannel++;
      }
     
    }
  }
  else {
    
    m_xlChannelMask[CHAN01] = xlGetChannelMask(hwType, hwIndex, hwChannel);
    sprintf_s (tmp, "Found CAN in VHWConf, hWType: %d, CM: 0x%I64x", hwType, m_xlChannelMask[CHAN01]);
    DEBUG(DEBUG_ADV,tmp);
 
    //for (i=0; i < xlDrvConfig.channelCount; i++) {

    //  if ( xlDrvConfig.channel[i].channelMask ==1 );//1m_xlChannelMask[CHAN01]
    //   // m_pHardware->AddString(xlDrvConfig.channel[i].name);
    //}

    // get the second channel
    xlStatus = xlGetApplConfig("xlCANcontrol", CHAN02, &hwType, &hwIndex, &hwChannel, busType); 
    if (xlStatus) return xlStatus;

    m_xlChannelMask[CHAN02] = xlGetChannelMask(hwType, hwIndex, hwChannel);
    sprintf_s (tmp, "Found CAN in VHWConf, hWType: %d, CM: 0x%I64x", hwType, m_xlChannelMask[CHAN02]);
    DEBUG(DEBUG_ADV,tmp);

    //for (i=0; i < xlDrvConfig.channelCount; i++) {

    //  if ( xlDrvConfig.channel[i].channelMask == m_xlChannelMask[CHAN02]);
    //  //  m_pHardware->AddString(xlDrvConfig.channel[i].name);
    //}
 
  }

  return xlStatus;
}

////////////////////////////////////////////////////////////////////////////

//! canInit

//! xlCANcontrol use ONE port for both channels.
//!
//////////////////////////////////////////////////////////////////////////// 

XLstatus CCANFunctions::canInit()
{
  XLstatus         xlStatus = XL_ERROR;
  XLaccess         xlPermissionMask;
  char             tmp[100];

  // ---------------------------------------
  // Open ONE port for both channels 
  // ---------------------------------------

  // calculate the channelMask for both channel 
  m_xlChannelMask_both = m_xlChannelMask[CHAN01] + m_xlChannelMask[CHAN02];
  xlPermissionMask = m_xlChannelMask_both;

  xlStatus = xlOpenPort(&m_xlPortHandle, "xlCANcontrol", m_xlChannelMask_both, &xlPermissionMask, 1024, XL_INTERFACE_VERSION, XL_BUS_TYPE_CAN);//256 
  sprintf_s(tmp, "xlOpenPort: PortHandle: %d; Permissionmask: 0x%I64x; Status: %d", m_xlPortHandle, xlPermissionMask, xlStatus);
  DEBUG(DEBUG_ADV, tmp);

  if (m_xlPortHandle == XL_INVALID_PORTHANDLE) return XL_ERROR;
  if (xlStatus == XL_ERR_INVALID_ACCESS) return xlStatus;

  return xlStatus;
}

////////////////////////////////////////////////////////////////////////////

//! canCreateRxThread

//! set the notification and creates the thread.
//!
////////////////////////////////////////////////////////////////////////////

XLstatus CCANFunctions::canCreateRxThread()
{
  
  XLstatus      xlStatus = XL_ERROR;
  DWORD         ThreadId=0;
  char          tmp[100];
  
  if (m_xlPortHandle!= XL_INVALID_PORTHANDLE) {

    // Send a event for each Msg!!!
    xlStatus = xlSetNotification (m_xlPortHandle, &m_hMsgEvent, 1);
    sprintf_s(tmp, "SetNotification '%d', xlStatus: %d", m_hMsgEvent, xlStatus);
    DEBUG(DEBUG_ADV, tmp);

    // for the RxThread
    //g_th.xlPortHandle = m_xlPortHandle;
    //g_th.hMsgEvent    = m_hMsgEvent; 
    //g_th.pOutput      = m_pOutput;
   
    //m_hThread = CreateThread(0, 0x1000, RxThread, (LPVOID) &g_th, 0, &ThreadId);
    //sprintf_s(tmp, "CreateThread %d", m_hThread);
    //DEBUG(DEBUG_ADV, tmp);
    
  }
  return xlStatus;
}


////////////////////////////////////////////////////////////////////////////

//! CANGoOnBus

//! set the selected baudrate and go on bus.
//!
////////////////////////////////////////////////////////////////////////////

XLstatus CCANFunctions::CANGoOnBus(unsigned long baudrate)
{
  XLstatus      xlStatus = XL_ERROR;
  char          tmp[100];

  xlStatus = xlCanSetChannelBitrate(m_xlPortHandle, m_xlChannelMask_both, baudrate);
  sprintf_s(tmp, "SetBaudrate: %d, stat: %d", baudrate, xlStatus);
  DEBUG(DEBUG_ADV, tmp);

  xlStatus = xlActivateChannel(m_xlPortHandle, m_xlChannelMask_both, XL_BUS_TYPE_CAN, XL_ACTIVATE_RESET_CLOCK);
  sprintf_s(tmp, "ActivateChannel, stat: %d",  xlStatus);
  DEBUG(DEBUG_ADV, tmp);

  return xlStatus;
}

////////////////////////////////////////////////////////////////////////////

//! CANGoOffBus

//! Deactivate the channel
//!
////////////////////////////////////////////////////////////////////////////

XLstatus CCANFunctions::CANGoOffBus()
{
  XLstatus      xlStatus = XL_ERROR;
  char          tmp[100];

  xlStatus = xlDeactivateChannel(m_xlPortHandle, m_xlChannelMask_both);
  sprintf_s(tmp, "DeactivateChannel, stat: %d",  xlStatus);
  DEBUG(DEBUG_ADV, tmp);

  return xlStatus;
}

////////////////////////////////////////////////////////////////////////////

//! CANSend

//! transmit a CAN message to the selected channel with the give values.
//!
////////////////////////////////////////////////////////////////////////////

XLstatus CCANFunctions::CANSend(XLevent xlEvent, int channel)
{
  
  XLstatus      xlStatus;
  char          tmp[100];
  unsigned int  messageCount = 1;

  xlStatus = xlCanTransmit(m_xlPortHandle, m_xlChannelMask[channel], &messageCount, &xlEvent);
  sprintf_s(tmp, "Transmit, mc: %d, channel: %d, stat: %d",  messageCount, channel, xlStatus);
  DEBUG(DEBUG_ADV, tmp);

  return xlStatus;
}

////////////////////////////////////////////////////////////////////////////

//! CANResetFilter

//! Reset the acceptancefilter
//!
////////////////////////////////////////////////////////////////////////////

XLstatus CCANFunctions::CANResetFilter()
{
  XLstatus      xlStatus;
  char          tmp[100];

  xlStatus = xlCanResetAcceptance(m_xlPortHandle, m_xlChannelMask_both, XL_CAN_STD);
  sprintf_s(tmp, "CanResetAcceptance, stat: %d",  xlStatus);
  DEBUG(DEBUG_ADV, tmp);

  return xlStatus;
}

////////////////////////////////////////////////////////////////////////////

//! CANSetFilter

//! Reset the acceptancefilter
//!
////////////////////////////////////////////////////////////////////////////

XLstatus CCANFunctions::CANSetFilter(unsigned long first_id, unsigned long last_id)
{
  XLstatus      xlStatus;
  char          tmp[100];
  
  // because there all filters open, we close all.
    xlStatus = xlCanSetChannelAcceptance(m_xlPortHandle, m_xlChannelMask_both, first_id, 0xFFF, XL_CAN_STD);

  //xlStatus = xlCanSetChannelAcceptance(m_xlPortHandle, m_xlChannelMask_both, 0xFFF, 0xFFF, XL_CAN_STD);
  sprintf_s(tmp, "CanSetChannelAcceptance, stat: %d",  xlStatus);
  DEBUG(DEBUG_ADV, tmp);
  
  // and now we can set the acceptance filter range.
  //xlStatus = xlCanAddAcceptanceRange(m_xlPortHandle, m_xlChannelMask_both, first_id, last_id);
  //sprintf_s(tmp, "CanAddAcceptanceRange, firstID: %d, lastID: %d, stat: %d",  first_id, last_id, xlStatus);
  //DEBUG(DEBUG_ADV, tmp);
  
  
  return xlStatus;
}
XLstatus CCANFunctions::demoCleanUp(void)
{
  XLstatus xlStatus;
    
  if (m_xlPortHandle != XL_INVALID_PORTHANDLE) {
    xlStatus = xlClosePort(m_xlPortHandle);
    printf("- ClosePort        : PH(0x%x), %s\n", m_xlPortHandle, xlGetErrorString(xlStatus));
  }

  m_xlPortHandle = XL_INVALID_PORTHANDLE;
  xlCloseDriver();

  return XL_SUCCESS;    // No error handling
}
///////////////////////////////////////////////////////////////////////////

//! receive xlevent

//! thread to readout the message queue and parse the incoming messages
//!
////////////////////////////////////////////////////////////////////////////


XLstatus CCANFunctions::CANreadxlEvent(XLevent *xlEvent,DWORD timeout,unsigned int channel) 
{

  XLstatus        xlStatus;

  unsigned int    msgsrx = 1;
  XLevent         xlEventRec; 
  char            tmp[110];
  CString         str;
  
  g_bThreadRun = TRUE;
  if (g_bThreadRun) { 
   
    WaitForSingleObject(m_hMsgEvent,timeout);
    
    xlStatus = XL_SUCCESS;
   
    if (!xlStatus) {
        
	  msgsrx = 1;
      xlStatus = xlReceive(m_xlPortHandle, &msgsrx, &xlEventRec);	
		
	  if ( xlStatus!=XL_ERR_QUEUE_IS_EMPTY ) {
          
        sprintf_s(tmp, "%s", xlGetEventString(&xlEventRec));

        DEBUG(DEBUG_ADV, tmp);
		if(xlEventRec.tag==1)
		{
			if(xlEventRec.tagData.msg.flags == 0x40 || xlEventRec.tagData.msg.flags == 0x00)
			{
				//if(xlEventRec.chanIndex==channel)
					memcpy(xlEvent,&xlEventRec,sizeof(XLevent));
				//else return 13;
			}
			else return 1;
		}
       }

	}
    }
  return xlStatus;
}

///////////////////////////////////////////////////////////////////////////

//! RxThread

//! thread to readout the message queue and parse the incoming messages
//!
////////////////////////////////////////////////////////////////////////////

DWORD WINAPI RxThread(LPVOID par) 
{

  XLstatus        xlStatus;

  unsigned int    msgsrx = 1;
  XLevent         xlEvent; 
  char            tmp[110];
  CString         str;
  
  g_bThreadRun = TRUE;

  TStruct *g_th;

  g_th = (TStruct*) par;  

  sprintf_s(tmp, "thread: SetNotification '%d'", g_th->hMsgEvent);
  DEBUG(DEBUG_ADV, tmp);

  while (g_bThreadRun) { 
   
    WaitForSingleObject(g_th->hMsgEvent,10);
    
    xlStatus = XL_SUCCESS;
   
    while (!xlStatus) {
        
	  msgsrx = 1;
     // xlStatus = xlReceive(g_th->xlPortHandle, &msgsrx, &xlEvent);	
		
	  if ( xlStatus!=XL_ERR_QUEUE_IS_EMPTY ) {
          
        sprintf_s(tmp, "%s", xlGetEventString(&xlEvent));

        DEBUG(DEBUG_ADV, tmp);
       // g_th->pOutput->InsertString(-1, tmp);
		
        //g_th->pOutput->SetCurSel(g_th->pOutput->GetCount()-1);
      }
    
    }
          
  }
  return NO_ERROR; 
}
