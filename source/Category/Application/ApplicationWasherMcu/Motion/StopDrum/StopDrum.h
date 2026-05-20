/**
 *  @file
 *
 *  @brief      API file for the StopDrum module.
 *
 *  $Header: StopDrum.h 1.3 2015/07/15 12:20:12EDT Paul F Spindler (SPINDPF) Exp  $
 *
 *  @copyright  Copyright 2015,2017 Whirlpool Corporation. All rights reserved - CONFIDENTIAL
 */
//---------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------
#ifndef STOPDRUM_H_
#define STOPDRUM_H_

#include "MotionDeclare.h"

//=====================================================================================================================
//-------------------------------------- PUBLIC (Extern Variables, Constants & Defines) -------------------------------
//=====================================================================================================================

//! list of module events
typedef enum
{
    STOP_DRUM_EV_NONE = 0,      //!< no events to report
    STOP_DRUM_EV_STOPPED        //!< the drum has stopped
} STOP_DRUM_EVENT_TYPE;

typedef struct STOP_DRUM_PROFILE_STRUCT
{
    MOTION_DRUM_ACCELERATION_TYPE Drum_Deceleration;
} STOP_DRUM_PROFILE_TYPE;

//=====================================================================================================================
//-------------------------------------- PUBLIC (Function Prototypes) -------------------------------------------------
//=====================================================================================================================

void StopDrum__Entry(STOP_DRUM_PROFILE_TYPE* profile);
STOP_DRUM_EVENT_TYPE StopDrum__Do(void);

#endif // STOPDRUM_H_


