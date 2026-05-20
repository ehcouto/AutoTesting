/**
 *  @defgroup   CLASS_B
 *  @file       SRMotorSafetyInfoParamsMgr.h
 *  @brief      Header file for the SRMotorSafetyInfoParams module.
 *
 *---------------------------------------------------------------------------------------------------------------------
 *------------------- Copyright 2012.  Whirlpool Corporation.  All rights reserved - CONFIDENTIAL ---------------------
 *---------------------------------------------------------------------------------------------------------------------
 */
#ifndef SRMOTORSAFETYINFPARAMSMGR_H_
    #define SRMOTORSAFETYINFPARAMSMGR_H_

#include "SRFlow.h"
#include "Mci_prm.h"
//=====================================================================================================================
//-------------------------------------- PUBLIC (Extern Variables, Constants & Defines) -------------------------------
//=====================================================================================================================

/**
 * Safety Information structure
 */
typedef struct
{
    uint16 Counter;              //!<  0  - Information counter
	uint16 Info_Flags;           //!<  1  - Safety  flags
	uint16 Crc;         	     //!<  2  - Crc of safety information
} SR_MOTOR_SAFETY_INFO_LIST_TYPE;

/// Call back used for SRFlow to log every 1ms when the internal clock is considered
#if (PLATFORM_USED == PLATFORM_2_5_BASED)
#define SRMOTORSAFETYINFOPARAMSMGR_1MS_FLOW_BEGIN()                      SRFlow__ISRLogEvent(SRFLOW_SRMOTORSAFETYINFOPARAMSMGR_1MS_ISR_BEGIN)
#define SRMOTORSAFETYINFOPARAMSMGR_1MS_FLOW_END()                        SRFlow__ISRLogEvent(SRFLOW_SRMOTORSAFETYINFOPARAMSMGR_1MS_ISR_END)
#else
#define SRMOTORSAFETYINFOPARAMSMGR_1MS_FLOW_BEGIN()
#define SRMOTORSAFETYINFOPARAMSMGR_1MS_FLOW_END()
#endif
//=====================================================================================================================
//-------------------------------------- PUBLIC (Function Prototypes) -------------------------------------------------
//=====================================================================================================================
void SRMotorSafetyInfoParmsMgr__Initialize(void);
SR_MOTOR_SAFETY_INFO_LIST_TYPE SRMotorSafetyInfoParmsMgr__GetInfo(void);
BOOL_TYPE SRMotorSafetyInfoParamsMgr__SetParams(void* set_this_address);
void SRMotorSafetyInfoParamsMgr__Handler1ms(void);

#endif // SRMOTORSAFETYINFPARAMSMGR_H_


