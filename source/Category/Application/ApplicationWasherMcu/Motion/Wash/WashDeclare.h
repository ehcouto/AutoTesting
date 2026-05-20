/**
 *  @file       
 *
 *  @brief      Public declarations for the Wash module.
 *
 *  $Header: WashDeclare.h 1.2 2015/04/20 10:00:43EDT Paul F Spindler (SPINDPF) Exp  $
 *
 *  @copyright  Copyright 2015-$Date: 2015/04/20 10:00:43EDT $. Whirlpool Corporation. All rights reserved - CONFIDENTIAL
 */
//---------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------
#ifndef WASHDECLARE_H_
#define WASHDECLARE_H_

#include "MotionDeclare.h"
#include "SpinDrumDeclare.h"

//=====================================================================================================================
//-------------------------------------- PUBLIC (Extern Variables, Constants & Defines) -------------------------------
//=====================================================================================================================

//! list of parameters for Wash
typedef struct
{
    SPIN_DRUM_PROFILE_TYPE      Spin;           //!< the +/- from the Target_Drum_Velocity is used to indicate the start direction
    uint16                      Time_Off;       //!< time off, 1 = 1 msec
    uint16                      Time_On;        //!< time on, 1 = 1msec
} WASH_PROFILE_TYPE;

#endif // WASHDECLARE_H_


