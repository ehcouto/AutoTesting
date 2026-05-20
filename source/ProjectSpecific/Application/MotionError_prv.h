/**
 *  @file       
 *
 *  @brief      Private parameters for the MotionError module.
 *
 *  $Header: MotionError_prv.h 1.1 2015/06/29 14:29:00EDT Paul F Spindler (SPINDPF) Exp  $
 *
 *  @copyright  Copyright 2015-$Date: 2015/06/29 14:29:00EDT $. Whirlpool Corporation. All rights reserved - CONFIDENTIAL
 */
//---------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------
#ifndef SOURCE_APPSPECIFICPRM_MOTION_MOTIONERROR_PRV_H_
#define SOURCE_APPSPECIFICPRM_MOTION_MOTIONERROR_PRV_H_

#include "MotionDeclare.h"

//=====================================================================================================================
//-------------------------------------- PRIVATE (Extern Variables, Constants & Defines) ------------------------------
//=====================================================================================================================

/**
 *  @brief  Delay time before module will verify the Mci module has stopped.
 *
 *  @return 1 = 1 call of the API MotionError__Do()
 */
#define DELAY_TIME()        ( 1 * 1000 / MOTION_HANDLER_EXECUTION_RATE )       // 1 second converted to number of calls

//=====================================================================================================================
//-------------------------------------- PRIVATE (Function Prototypes) ------------------------------------------------
//=====================================================================================================================

#endif // SOURCE_APPSPECIFICPRM_MOTION_MOTIONERROR_PRV_H_


