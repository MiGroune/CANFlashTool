#pragma once
#pragma comment(lib,"CANCARD.lib")

#include "can_def.h"
#include "canlay2.h"

 const int presc[7] ={ 1, 1, 2, 4, 4, 8,32};
const int sjw[7]   ={ 2, 4, 4, 4, 4, 4, 4};
const int tseg1[7] ={ 4, 8, 8, 8,11,11,16};
const int tseg2[7] ={ 3, 7, 7, 7, 8, 8, 8};
const int sample[7]={ 0, 0, 0, 0, 0, 0, 0};
// Acceptance filter
#define	ACCEPT_MASK_1			0x7e8
#define	ACCEPT_CODE_1 			0xffff
#define	ACCEPT_MASK_XTD_1		0x00000000L
#define	ACCEPT_CODE_XTD_1		0x00000000L

#define	ACCEPT_MASK_2			0x7e8
#define	ACCEPT_CODE_2 			0xffff
#define	ACCEPT_MASK_XTD_2		0x00000000L
#define	ACCEPT_CODE_XTD_2		0x00000000L
// Resynchronization
#define	SLEEPMODE_1				0
#define	SPEEDMODE_1				0
                                           
#define	SLEEPMODE_2				0
#define	SPEEDMODE_2				0
// Phys. layer (-1: default - CAN High Speed)
#define	OUTPUT_CONTROL_1		-1                                           
#define	OUTPUT_CONTROL_2		-1                                           

/////////////fifo
static unsigned long 	diff_time,old_time,Time;           // Time of event
static int	Xtd;          // 0:11 bit Identifier,	1: 29bit Identifier

class CCANCARD2Func
{
public:
	CCANCARD2Func(void);
	~CCANCARD2Func(void);


	int CCANCARD2Func::Initialize_can_parameter(void);
	int CCANCARD2Func::CANInitialize(void);
	int CCANCARD2Func::InitializeFIFO(void);
	int CCANCARD2Func::ReadBusEventFIFO(void);

	int	can_chip_type[2];
	int m_baudrate;
};

