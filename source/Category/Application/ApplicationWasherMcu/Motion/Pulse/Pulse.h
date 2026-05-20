/**
 *  @file       
 *
 *  @brief      Public interface file for the Pulse module.
 *
 *  $Header: $
 *
 *  @copyright  Copyright 2016-$Date$. Whirlpool Corporation. All rights reserved - CONFIDENTIAL
 */
//---------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------
#ifndef SOURCE_PROJECTSPECIFIC_APPLICATION_MOTION_PULSE_PULSE_H_
#define SOURCE_PROJECTSPECIFIC_APPLICATION_MOTION_PULSE_PULSE_H_

#include "PulseDeclare.h"

//=====================================================================================================================
//-------------------------------------- PUBLIC (Extern Variables, Constants & Defines) -------------------------------
//=====================================================================================================================

//! list of module events
typedef enum
{
    PULSE_EV_NONE = 0,          //!< no events to report
    PULSE_EV_ERROR,             //!< the module cannot perform its intended functionality
    PULSE_EV_SELF_TERMINATED    //!< indicates all the profiles in the queue have been run
} PULSE_EVENT_TYPE;

//=====================================================================================================================
//-------------------------------------- PUBLIC (Function Prototypes) -------------------------------------------------
//=====================================================================================================================

void Pulse__Initialize(void);
void Pulse__Entry(const PULSE_PROFILE_TYPE *);
PULSE_EVENT_TYPE Pulse__Do(void);

#endif // SOURCE_PROJECTSPECIFIC_APPLICATION_MOTION_PULSE_PULSE_H_


