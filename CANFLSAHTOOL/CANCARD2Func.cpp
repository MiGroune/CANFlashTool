/*----------------------------------------------------------------------------
| File        : CANCARD2Func.cpp
| Project     : CANFlashtool 
|
| Description : Shows the basic CAN functionality for the CANCARD2 Driver Library
|-----------------------------------------------------------------------------
| $Author: liaoshaoyan      $Revision:  
| $Data    2014.04.09   
|-----------------------------------------------------------------------------*/
 
#include "StdAfx.h"
#include "CANCARD2Func.h"





CCANCARD2Func::CCANCARD2Func(void)
{
}


CCANCARD2Func::~CCANCARD2Func(void)
{
}

/***********************************************************
  Initialize_can_parameter
  Reset and initialization of CAN chips
  设置接收过滤：0x7e8   baud: 500k
************************************************************/
int CCANCARD2Func::Initialize_can_parameter(void)
{
	int i;
	int sw_version;
	int fw_version;
	int hw_version;
	int license;
	unsigned long SerialNumber;

   // Reset CAN chips of CAN 1 and 2
   if (CANPC_reset_chip() != 0)		//重置 0是成功  (3)
      {
      printf("-->Error in CANPC_reset_chip \n");
      return(-1);
      }

	// Display SW and HW version, chip type and licence (optionally)
   if (CANPC_get_serial_number(&SerialNumber) != 0)
      {
      printf("-->Error in CANPC_get_serial number \n\n\n");
      return(-1);
      }
   if (CANPC_get_version(&sw_version,&fw_version,&hw_version,&license,can_chip_type) != 0)//(4)
      {
      printf("-->Error in CANPC_get_version \n\n\n");
      return(-1);
      }
   else
      {
      printf("Software version: %u.%02u \n",
              sw_version / 100, sw_version % 100);
      printf("Firmware version: %u.%02u \n",
              fw_version / 100, fw_version % 100);
      printf("Hardware version: %x.%02x \n",
              hw_version % 0x100, hw_version / 0x100);
      for (i=0;i<=1;i++)
	  {
		  switch (can_chip_type[i])
		{
			  case 5:
				  printf("Chiptype CAN%1d: NEC72005\n", i+1);
				  break;
			  case 1000:
				  printf("Chiptype CAN%1d: SJA1000\n", i+1);
				  break;
			  case 527:
				  printf("Chiptype CAN%1d: 82527\n", i+1);
				  break;
			  case 200:
				  printf("Chiptype CAN%1d: 82C200\n", i+1);
				  break;
			  default:
				  break;
		}
	  }
	  if (license & 0x02)
         printf("CANcard licensed for CANalyzer \n");
      if (license & 0x04)
         printf("CANcard licensed for DDE Server \n");
      }
		printf("Serial#: %d\n", SerialNumber);
   
      
   // Set Bit timing for CAN 1 and 2
	m_baudrate=1;//设置波特率（5）
	if ((m_baudrate<0) || (m_baudrate>6)) m_baudrate=0;  //default setting
	if (CANPC_initialize_chip(presc[m_baudrate], sjw[m_baudrate], tseg1[m_baudrate], tseg2[m_baudrate], sample[m_baudrate]) != 0)
      {							//1
      printf("-->Error in CANPC_initialize_chip \n");
      return(-1);
      }

   if (CANPC_initialize_chip2(presc[m_baudrate], sjw[m_baudrate], tseg1[m_baudrate], tseg2[m_baudrate], sample[m_baudrate]) != 0)
      {
      printf("-->Error in CANPC_initialize_chip2 \n");
      return(-1);
      }

	// Set acceptance filter for CAN 1 and 2   //(7) ACCEPT_MASK_1过滤
   if (CANPC_set_acceptance(ACCEPT_MASK_1, ACCEPT_CODE_1, ACCEPT_MASK_XTD_1, ACCEPT_CODE_XTD_1) != 0)
      {
      printf("-->Error in CANPC_set_acceptance \n");
      return(-1);
      }

   if (CANPC_set_acceptance2(ACCEPT_MASK_2, ACCEPT_CODE_2, ACCEPT_MASK_XTD_2, ACCEPT_CODE_XTD_2) != 0)
      {
      printf("-->Error in CANPC_set_acceptance2 \n");
      return(-1);
      }

   // Set resynchronization
   if (CANPC_set_mode(SLEEPMODE_1, SPEEDMODE_1) != 0)//(6)
      {
      printf("-->Error in CANPC_set_mode \n");
      return(-1);
      }
   if (CANPC_set_mode2(SLEEPMODE_2, SPEEDMODE_2) != 0)
      {
      printf("-->Error in CANPC_set_mode2 \n");
      return(-1);
      }
   
   // Set output control (phys. layer -1: default  CAN High Speed)                               
   if (CANPC_set_output_control(OUTPUT_CONTROL_1) != 0)//(8)
      {
      printf("-->Error in CANPC_set_output_control \n");
      return(-1);
      }
   if (CANPC_set_output_control2(OUTPUT_CONTROL_2) != 0)
      {
      printf("-->Error in CANPC_set_output_control2 \n");
      return(-1);
      }

   return(0);
}// End of function

int CCANCARD2Func::CANInitialize(void)
{
	CANPC_RESSOURCES	crRessources;
	int			ret;

	crRessources.uSocket      = CANPC_SOCKET_AUTO;

	if ((ret = INIPC_initialize_board(&crRessources)) != 0)
	   {
	   //printf("-->Error in INIPC_initialize_board: ");
	   ////printInitErrorText(ret);
	   INIPC_close_board(); 
	   return(-1);
	   }


	 //Load firmware (2)
	if ((ret = CANPC_reset_board()) != 0)
	   {
	   //printf("-->Error in CANPC_reset_board: ");
	   ////printResetErrorText(ret);
	   INIPC_close_board();
	   return(-1);
	   }
		if (Initialize_can_parameter())
   		{
   		//printf("-->Initialization falied \n");
   	    INIPC_close_board();
   		return(-1);
   		}
		 
		return 0;
}

int CCANCARD2Func::InitializeFIFO(void)
{
	int sw_version;
	int fw_version;
	int hw_version;
	int license;

   if (CANPC_get_version(&sw_version,&fw_version,&hw_version,&license,can_chip_type) != 0)
      {
      printf("-->Error in CANPC_get_version \n\n\n");
      return(-1);
      }
      
    if (CANPC_enable_fifo() != 0)//(1)
    	{
    	printf("-->Error in CANPC_enable_fifo \n\n");
        return(-1);
        }
    if (CANPC_enable_timestamps() != 0)//
    	{
    	printf("-->Error in CANPC_enable_timestamps \n\n");
        return(-1);
        }
    if (CANPC_enable_fifo_transmit_ack() != 0)//(1)
    	{
    	printf("-->Error in CANPC_enable_fifo_transmit_ack \n\n");
        return(-1);
        }
    if (CANPC_enable_fifo_transmit_ack2() != 0)
    	{
    	printf("-->Error in CANPC_enable_fifo_transmit_ack2 \n\n");
        return(-1);
        }
	Xtd=0;
	return(0);
}

int CCANCARD2Func::ReadBusEventFIFO(void)
{
param_struct 	param;
int				frc;

	switch (frc=CANPC_read_ac( &param ))
		{
		case 1:
			diff_time =  param.Time - old_time;
			printf("RCV STD CAN%1u Id%8lx Dlc%1x Data%2x %2x %2x %2x %2x %2x %2x %2x T%8lx D%8lx\n",
					param.Can, param.Ident, param.DataLength,
					param.RCV_data[0], param.RCV_data[1],
					param.RCV_data[2], param.RCV_data[3],
					param.RCV_data[4], param.RCV_data[5],
					param.RCV_data[6], param.RCV_data[7],
					param.Time, diff_time);
			old_time = param.Time;
			break;
		case 2:
			diff_time =  param.Time - old_time;
			printf("RCV STD CAN%1u Id%8lx Dlc%1x REMOTE                      T%8lx D%8lx\n",
					param.Can, param.Ident, param.DataLength,
					param.Time, diff_time);
			old_time = param.Time;
			break;
		case 9:// not valid with AC2 with 82C200
			diff_time =  param.Time - old_time;
			printf("RCV XTD CAN%1u Id%8lx Dlc%1x Data%2x %2x %2x %2x %2x %2x %2x %2x T%8lx D%8lx\n",
					param.Can, param.Ident, param.DataLength,
					param.RCV_data[0], param.RCV_data[1],
					param.RCV_data[2], param.RCV_data[3],
					param.RCV_data[4], param.RCV_data[5],
					param.RCV_data[6], param.RCV_data[7],
					param.Time, diff_time);
			old_time = param.Time;
			break;
		case 12:// not valid with AC2 with 82C200
			diff_time =  param.Time - old_time;
			printf("RCV XTD CAN%1u Id%8lx Dlc%1x REMOTE                      T%8lx D%8lx\n",
					param.Can, param.Ident, param.DataLength,
					param.Time, diff_time);
			old_time = param.Time;
			break;
		case 3:
			diff_time =  param.Time - old_time;
			printf("ACK STD CAN%1u Id%8lx Dlc%1x Data%2x %2x %2x %2x %2x %2x %2x %2x T%8lx D%8lx\n",
					param.Can, param.Ident, param.DataLength,
					param.RCV_data[0], param.RCV_data[1],
					param.RCV_data[2], param.RCV_data[3],
					param.RCV_data[4], param.RCV_data[5],
					param.RCV_data[6], param.RCV_data[7],
					param.Time, diff_time);
			old_time = param.Time;
			break;
		case 8:
			diff_time =  param.Time - old_time;
			printf("ACK STD CAN%1u Id%8lx Dlc%1x REMOTE                      T%8lx D%8lx\n",
					param.Can, param.Ident, param.DataLength,
					param.Time, diff_time);
			old_time = param.Time;
			break;
		case 10:// not valid with AC2 with 82C200
			diff_time =  param.Time - old_time;
			printf("ACK XTD CAN%1u Id%8lx Dlc%1x Data%2x %2x %2x %2x %2x %2x %2x %2x T%8lx D%8lx\n",
					param.Can, param.Ident, param.DataLength,
					param.RCV_data[0], param.RCV_data[1],
					param.RCV_data[2], param.RCV_data[3],
					param.RCV_data[4], param.RCV_data[5],
					param.RCV_data[6], param.RCV_data[7],
					param.Time, diff_time);
			old_time = param.Time;
			break;
		case 11:// not valid with AC2 with 82C200
			diff_time =  param.Time - old_time;
			printf("ACK XTD CAN%1u Id%8lx Dlc%1x REMOTE                      T%8lx D%8lx\n",
					param.Can, param.Ident, param.DataLength,
					param.Time, diff_time);
			old_time = param.Time;
			break;
		case 5:
			diff_time =  param.Time - old_time;
			printf( "ERROR  CAN%1u Bus state: %1d                      T%8lx D%8lx\n",
					param.Can, param.Bus_state,
					param.Time, diff_time);
			old_time = param.Time;
			break;
		default:
			break;
		}
	return(frc);
}


