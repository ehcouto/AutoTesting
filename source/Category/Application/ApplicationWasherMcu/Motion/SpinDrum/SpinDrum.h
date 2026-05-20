/**
 *  @file       
 *
 *  @brief      API file for the SpinDrum module.
 *
 *  $Header: SpinDrum.h 1.5 2015/07/15 13:21:43EDT Paul F Spindler (SPINDPF) Exp  $
 *
 *  @copyright  Copyright 2015-$Date: 2015/07/15 13:21:43EDT $. Whirlpool Corporation. All rights reserved - CONFIDENTIAL
 */
//---------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------
#ifndef SPINDRUM_H_
#define SPINDRUM_H_

#include "SpinDrumDeclare.h"

//=====================================================================================================================
//-------------------------------------- PUBLIC (Extern Variables, Constants & Defines) -------------------------------
//=====================================================================================================================

//! list of module events
typedef enum
{
    SPIN_DRUM_EV_NONE = 0,                  //!< no events to report

    SPIN_DRUM_EV_ERROR,                     //!< the module cannot perform its intended functionality
    SPIN_DRUM_EV_IS_ROTATING,               /**
                                             *  due to the Mci module, there is varying time delay from when the drum is commanded to rotate to when it actually
                                             *  starts to rotate.  This event is posted when the drum actually starts rotating.
                                             */
    SPIN_DRUM_EV_RAMP_FINISHED              //!< drum has completed the acceleration phase
} SPIN_DRUM_EVENT_TYPE;


//=====================================================================================================================
//-------------------------------------- PUBLIC (Function Prototypes) -------------------------------------------------
//=====================================================================================================================

void SpinDrum__Entry(SPIN_DRUM_PROFILE_TYPE * control);
SPIN_DRUM_EVENT_TYPE SpinDrum__Do(void);

#endif // SPINDRUM_H_


