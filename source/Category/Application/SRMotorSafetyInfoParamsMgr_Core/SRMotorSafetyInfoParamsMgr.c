/**
 *  @defgroup   CLASS_B
 *  @file       SRMotorSafetyInfoParamsMgr.c
 *  @brief      Safety information manager for the Indesit Communication Protocol
 *
 *  @details    This module handles the safety information that are sent through the serial
 *              channel to the ACU and provides a method for the safety parameters assignment
 *              to the Safety Motor Manager. \n
 *              A check for the communication proper operation is made in the 1 ms handler.
 *              In order for the check to work a communication counter must be reset by calling
 *              the SRMotorSafetyInfoParmsMgr__GetInfo method at least once every 8 seconds.
 *              In case the check fails the motor is stopped.
 *
 *
 *---------------------------------------------------------------------------------------------------------------------
 *------------------- Copyright 2012.  Whirlpool Corporation.  All rights reserved - CONFIDENTIAL ---------------------
 *---------------------------------------------------------------------------------------------------------------------
 */

//-------------------------------------- Include Files ----------------------------------------------------------------

// Standard Vortex libraries
#include "Compiler_defs.h"
#include "C_Extensions.h"
#include "Crc16.h"

// Standard Safety Relevant Class B libraries
#include "SRMotorSafetyInfoParamsMgr.h"
#include "SRMotorSafetyMgr.h"
#include "SRData.h"
#include "SRException.h"

static volatile SR_MOTOR_SAFETY_INFO_LIST_TYPE Sr_Info;

/**
 * Safety communication counter, used to check for the
 * proper communication operation.
 */
static volatile unsigned short SR_Communication_Cnt;
static volatile unsigned short NSR_Communication_Cnt;

//lint -emacro( 929, SR_CHECK_COMM_CNT )
#define SR_CHECK_COMM_CNT()                          SRData__CheckShort((unsigned short *)&SR_Communication_Cnt, (unsigned short *)&NSR_Communication_Cnt) //lint !e929
//lint -emacro( 929, SR_UPDATE_COMM_CNT )
#define SR_UPDATE_COMM_CNT(value)					 SRData__UpdateShort((unsigned short *)&SR_Communication_Cnt, (unsigned short *)&NSR_Communication_Cnt, (unsigned short)(value))

/**
 * Communication counter threshold for communication timeout.
 */
#define SR_COMMUNICATION_TIMEOUT_THR				(8000) // 8s @1 ms
#define SR_COMMUNICATION_FAIULURE_CODE				(0xE)
//=====================================================================================================================
//-------------------------------------- Public Functions -------------------------------------------------------------
//=====================================================================================================================



//---------------------------------------------------------------------------------------------------------------------
/**
 *  @brief      Initialize the safety info structure.
 *
 *  @details    Safety info initialized with zero values.
 */
void SRMotorSafetyInfoParmsMgr__Initialize(void)
{
	Sr_Info.Counter = 0;

	Sr_Info.Info_Flags = 0;

	Sr_Info.Crc = 0;

	SR_UPDATE_COMM_CNT(0); //Reset the communication counter
}

//


//---------------------------------------------------------------------------------------------------------------------
/**
 *  @brief      Return the safety info structure.
 *
 *  @details    Update the safety info and reset the communication counter.
 *
 *  @return     The safety info structure.
 */
SR_MOTOR_SAFETY_INFO_LIST_TYPE SRMotorSafetyInfoParmsMgr__GetInfo(void)
{
	uint8 buff[4];

	Sr_Info.Counter++;

	Sr_Info.Info_Flags = (SRMotorSafetyMgr__GetFlag(SR_FLAG_UNLOCK_DOOR_SPEED)<<BIT0) +
						 (SRMotorSafetyMgr__AnyFaultFound()<<BIT1);

	buff[0] = (uint8)(Sr_Info.Counter>>8);
	buff[1] = (uint8)(Sr_Info.Counter& 0x00FF);
	buff[2] = (uint8)(Sr_Info.Info_Flags>>8);
	buff[3] = (uint8)(Sr_Info.Info_Flags& 0x00FF);

	Sr_Info.Crc = Crc16__RangeProcess(buff,0xFFFF,4);


	//The info packet is sent, the flow communication is correct -> reset communication counter
	SR_UPDATE_COMM_CNT(0);

	return Sr_Info;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 *  @brief      Set Motor Class B parameters address
 *
 *  @param[in]  set_this_address The address of the safety parameters buffer.
 *
 *  @return     The response of the operation.
 *              TRUE - value accepted
 *              FALSE - value denied - default answer.
 *
 */

BOOL_TYPE SRMotorSafetyInfoParamsMgr__SetParams(void* set_this_address)
{
	BOOL_TYPE ret_val = FALSE;

	ret_val = SRMotorSafetyMgr__SetParams((void*)set_this_address);

	return ret_val;
}




//---------------------------------------------------------------------------------------------------------------------
/**
 *  @brief      General slot for call rate of 1ms.
 *
 *  @details    This handler checks if a information packet is not sent for 8 seconds.
 *              In this case the motor is stopped. Otherwise, the communication counter is
 *              incremented.
 *
 */
void SRMotorSafetyInfoParamsMgr__Handler1ms(void)
{
	unsigned short temp;
	SRMOTORSAFETYINFOPARAMSMGR_1MS_FLOW_BEGIN();

	temp = SR_CHECK_COMM_CNT();

	if (temp == SRDATA_OK)
	{
		if(SR_Communication_Cnt > SR_COMMUNICATION_TIMEOUT_THR) //Check if a information packet is not sent for 8 sec
		{// if the communication is down for more than 8 sec -> Stop the motor
			//SRMotorSafetyMgr__ForceStop();
		}
		else
		{
			temp = SR_Communication_Cnt;

			temp++;

			SR_UPDATE_COMM_CNT(temp); //Update the communication counter
		}
	}
	else
	{ // Safety data corrupted -> Reset the micro
		SRException__Queue(EXCEPTION_ISR_DATA_SHORT, 0, (uint32)(SR_COMMUNICATION_FAIULURE_CODE));
	}
	SRMOTORSAFETYINFOPARAMSMGR_1MS_FLOW_END();
}


