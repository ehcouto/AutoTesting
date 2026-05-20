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
#ifndef ROTATEXDEGDECLARE_H_
#define ROTATEXDEGDECLARE_H_

#include "MotionDeclare.h"
#include "SpinDrumDeclare.h"

//=====================================================================================================================
//-------------------------------------- PUBLIC (Extern Variables, Constants & Defines) -------------------------------
//=====================================================================================================================

//! list of parameters for Wash
typedef struct
{
    SPIN_DRUM_PROFILE_TYPE      Spin;
    uint16						Timeout;
    uint8  						Degrees;        // the angular distance in degrees the drum shall rotate for under power
    sint8  						Target_Rpm;     // the target drum rpm to spin at; + = cw direction; - = ccw direction
    uint8						Second_Ramp_Rpm;
} ROTATEXDEG_PROFILE_TYPE;

//! Rotate X Degrees variables that track the angular distance traveled
typedef struct
{
    signed short    Previous_Rotor_Position;    // the previous rotor position in degrees
    unsigned short  Sum;                        // sum of total angular distance traveled by the rotor
    signed short    Rotor_Position_Offset;
    signed short    Rotor_Position_Target;
} ANGULAR_DISTANCE_TYPE;
#endif // ROTATEXDEGDECLARE_H_


