/**
 *  @file
 *
 *  @brief      Public declarations for the Pulse module.
 *
 *  $Header: $
 *
 *  @copyright  Copyright 2016-$Date$. Whirlpool Corporation. All rights reserved - CONFIDENTIAL
 */
//---------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------
#ifndef SOURCE_PROJECTSPECIFIC_APPLICATION_MOTION_PULSE_PULSEDECLARE_H_
#define SOURCE_PROJECTSPECIFIC_APPLICATION_MOTION_PULSE_PULSEDECLARE_H_

#include "SpinDrumDeclare.h"

//=====================================================================================================================
//-------------------------------------- PUBLIC (Extern Variables, Constants & Defines) -------------------------------
//=====================================================================================================================

//! list of Pulse behaviors
typedef enum PULSE_BEHAVIOR_ENUM
{
    PULSE_BEHAVIOR_RUN_IMMEDIATELY = 1,     // the pulse profile is to be run immediately
    PULSE_BEHAVIOR_MODIFY_PULSE,            // modify the currently active pulse
    PULSE_BEHAVIOR_LOAD_PULSE_INTO_QUEUE,   // loads the passed pulse into the queue
    NUM_OF_PULSE_BEHAVIORS
} PULSE_BEHAVIOR_TYPE;

//! declaration of the Pulse profile
typedef struct PULSE_PROFILE_STRUCT
{
    PULSE_BEHAVIOR_TYPE     Behavior;
    SPIN_DRUM_PROFILE_TYPE  Spin;
    uint32                  Time_On;            // 1 = 1msec
    uint32                  Time_Off;           // 1 = 1msec
} PULSE_PROFILE_TYPE;

#endif // SOURCE_PROJECTSPECIFIC_APPLICATION_MOTION_PULSE_PULSEDECLARE_H_


