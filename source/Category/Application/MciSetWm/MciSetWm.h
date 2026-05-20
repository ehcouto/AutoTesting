/**
 *  @file       MciSetWm.h
 *
 *  @brief      Header file for Indesit Wm Mci Setting Algorithms module
 *
 *  $Header: $
 *
 *  @copyright  Copyright 2016-$Date$. Whirlpool Corporation. All rights reserved - CONFIDENTIAL
 */
//---------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------
#ifndef MCISETWm_H_
    #define MCISETWm_H_


//-------------------------------------- Include Files ----------------------------------------------------------------
#include "C_Extensions.h"
#include "MciSetWm_prm.h"

//=====================================================================================================================
//-------------------------------------- PUBLIC (Extern Variables, Constants & Defines) -------------------------------
//=====================================================================================================================
#ifdef MOTOR_CIM_TYPE
//! soft start state
typedef enum SOFT_START_STATE
{
	SOFT_START_INITIALIZATION       = 0,    //!< 0  soft start initialization
	SOFT_START_ON_START             = 1,    //!< 1, soft start update on motor starting: it shall be called after the Mci run command
	SOFT_START_ON_STOP	            = 2,    //!< 2, soft start update on motor stopping: it shall be called before the Mci stop command
	SOFT_START_MAX_REQUEST   		= 3,    //!< 3, request of maximum softstart application for the next 20 motor's starts.
    SS_DUMMY                       	= 256
} SOFT_START_STATE_ENUM_T;
#endif
//=====================================================================================================================
//-------------------------------------- PUBLIC (Function Prototypes) -------------------------------------------------
//=====================================================================================================================

void MciSetWm__Initialize(void);
BOOL_TYPE MciSetWm__IsParamsReady(void);
void MciSetWm__5msHandler(void);


#if (DYNAMIC_TORQUE_LIMITS == ENABLED)
void MciSetWm__LimitNormRequest(BOOL_TYPE limitnorm_request);
#endif

#ifdef MOTOR_CIM_TYPE

#if (CIM_SOFT_START == ENABLED)
uint8 MciSetWm__SoftStart(SOFT_START_STATE_ENUM_T soft_start_state);
#endif

#endif

#endif /* MCISETWM_H_ */



