/*----------------------------------------------------------------------------
| File        : xlCANFunctions.h
| Project     : Vector CAN Example 
|
| Description : shows the basic CAN functionality for the XL Driver Library
|-----------------------------------------------------------------------------
| $Author: Harald $    $Locker: $   $Revision: 4 $
| $Header: /VCANDRV/XLAPI/samples/xlCANcontrol/xlCANFunctions.h 4     15.06.05 15:16 Harald $
|-----------------------------------------------------------------------------
| Copyright (c) 2004 by Vector Informatik GmbH.  All rights reserved.
|---------------------------------------------------------------------------*/

#if !defined(AFX_XLCANFUNCTIONS_H__48DFA4A9_72B2_48FE_80D5_D318A80C4B3A__INCLUDED_)
#define AFX_XLCANFUNCTIONS_H__48DFA4A9_72B2_48FE_80D5_D318A80C4B3A__INCLUDED_

#include "vxlapi.h"

#define CHAN01 0
#define CHAN02 1
#pragma comment(lib,"vxlapi.lib")
 
typedef struct {
    XLportHandle xlPortHandle; 
    HANDLE       hMsgEvent;
    CListBox    *pOutput;
} TStruct;

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CCANFunctions  
{
public:
	CCANFunctions();
	virtual ~CCANFunctions();

  XLstatus CANInit();
  XLstatus CANGoOnBus(unsigned long baudrate);
  XLstatus CANGoOffBus();
  XLstatus CANSend(XLevent xlEvent, int channel);
  XLstatus CANResetFilter();
  XLstatus CANSetFilter(unsigned long first_id, unsigned long last_id);
  
  CListBox *m_pOutput;
  CListBox *m_pHardware;
   XLportHandle     m_xlPortHandle;            //!< and one port
 XLaccess         m_xlChannelMask[2];        //!< we support only two channels

XLstatus CCANFunctions::CANreadxlEvent(XLevent *xlEventR,DWORD timeout,unsigned int channel);
XLstatus CCANFunctions::demoCleanUp(void);
   unsigned int  appChannel;
private:
  XLstatus         canGetChannelMask();
  XLstatus         canInit();
  XLstatus         canCreateRxThread();

 
 
  XLaccess         m_xlChannelMask_both;

  HANDLE           m_hThread;
  XLhandle         m_hMsgEvent;
};

DWORD     WINAPI RxThread( PVOID par );


#endif // !defined(AFX_XLCANFUNCTIONS_H__48DFA4A9_72B2_48FE_80D5_D318A80C4B3A__INCLUDED_)
