/**
 *  @file       
 *
 *  @brief      Private parameters for the StopDrum module.
 *
 *  $Header: StopDrum_prv.h 1.3 2015/09/28 15:19:52EDT Paul F Spindler (SPINDPF) Exp  $
 *
 *  @copyright  Copyright 2015-$Date: 2015/09/28 15:19:52EDT $. Whirlpool Corporation. All rights reserved - CONFIDENTIAL
 */
//---------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------
#ifndef STOPDRUM_PRV_H_
#define STOPDRUM_PRV_H_

#include "MotionDeclare.h"

//=====================================================================================================================
//-------------------------------------- PUBLIC (Extern Variables, Constants & Defines) -------------------------------
//=====================================================================================================================

/**
 *  Defines the maximum amount of time for the Mci module to stop after being sent the command.
 *  @return TIMER_TYPE
 */
#define MAX_TIME_FOR_MCI_TO_STOP    ( 30 * 1000 / MOTION_HANDLER_EXECUTION_RATE )       // 30 seconds

/**
 *  Defines the minimum amount of time between Mci stop commands.
 *  @return TIMER_TYPE
 */
#define ERROR_DELAY_TIME  ( 10 * 1000 / MOTION_HANDLER_EXECUTION_RATE )       // 5 seconds

//=====================================================================================================================
//-------------------------------------- PUBLIC (Function Prototypes) -------------------------------------------------
//=====================================================================================================================

#endif // STOPDRUM_PRV_H_


