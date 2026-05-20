/**
 *  @file
 *
 *  @brief      Public declarations for the SpinDrum module.
 *
 *  $Header: SpinDrumDeclare.h 1.1 2015/04/20 10:01:38EDT Paul F Spindler (SPINDPF) Exp  $
 *
 *  @copyright  Copyright 2015-$Date: 2015/04/20 10:01:38EDT $. Whirlpool Corporation. All rights reserved - CONFIDENTIAL
 */
//---------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------
#ifndef SPINDRUMDECLARE_H_
#define SPINDRUMDECLARE_H_

#include "MotionDeclare.h"

//=====================================================================================================================
//-------------------------------------- PUBLIC (Extern Variables, Constants & Defines) -------------------------------
//=====================================================================================================================

typedef enum
{
    SPIN_DRUM_CONSTANT_ACCELERATION = 0,    // acceleration rate is constant
    SPIN_DRUM_ACCELERATE_FOR_TIME        // acceleration rate is calculated (target_rpm - current_rpm) / Time = rpm/sec
} SPIN_DRUM_ACCEL_TYPE;

//! list of parameters for spinning the drum
typedef struct
{
    struct
    {
        SPIN_DRUM_ACCEL_TYPE    Type;       // defines the type of acceleration that the module will perform
        union
        {
            uint16                          Time;       //!< the time in milliseconds to go from the current rpm to the target rpm
            MOTION_DRUM_ACCELERATION_TYPE   Constant;   //!< a constant acceleration rate
        } Value;
    } Drum_Acceleration;
    MOTION_DRUM_VELOCITY_TYPE           Target_Drum_Velocity;
} SPIN_DRUM_PROFILE_TYPE;

#endif // SPINDRUMDECLARE_H_


