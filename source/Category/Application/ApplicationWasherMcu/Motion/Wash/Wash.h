/**
 *  @file       
 *
 *  @brief      API file for the Wash module.
 *
 *  $Header: Wash.h 1.3 2015/07/22 16:01:49EDT Paul F Spindler (SPINDPF) Exp  $
 *
 *  @copyright  Copyright 2015-$Date: 2015/07/22 16:01:49EDT $. Whirlpool Corporation. All rights reserved - CONFIDENTIAL
 */
//---------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------
#ifndef WASH_H_
#define WASH_H_

#include "WashDeclare.h"

//=====================================================================================================================
//-------------------------------------- PUBLIC (Extern Variables, Constants & Defines) -------------------------------
//=====================================================================================================================

//! list of module events
typedef enum
{
    WASH_EV_NONE = 0,               //!< no events to report
    WASH_EV_CW_STROKE_COMPLETED,    //!< module has completed 1 stroke in the clock-wise direction
    WASH_EV_CCW_STROKE_COMPLETED,   //!< module has completed 1 stroke in the clock-wise direction
    WASH_EV_ERROR                   //!< module cannot perform its intended functionality
} WASH_EVENT_TYPE;

//=====================================================================================================================
//-------------------------------------- PUBLIC (Function Prototypes) -------------------------------------------------
//=====================================================================================================================

void Wash__SetProfile(WASH_PROFILE_TYPE *);
void Wash__Entry(void);
WASH_EVENT_TYPE Wash__Do(void);

#endif // WASH_H_


