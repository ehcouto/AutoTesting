/**
 *  @file       
 *
 *  @brief      API file for the Wash module.
 *
 *  $Header: RotateXDeg.h 1.0 2017/05/17 16:01:49EDT Marco Di Goro (DIGORM) Exp  $
 *
 *  @copyright  Copyright 2017-$Date: 2017/05/17 16:01:49EDT $. Whirlpool Corporation. All rights reserved - CONFIDENTIAL
 */
//---------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------
#ifndef ROTATEXDEG_H_
#define ROTATEXDEG_H_

#include "RotateXDegDeclare.h"

//=====================================================================================================================
//-------------------------------------- PUBLIC (Extern Variables, Constants & Defines) -------------------------------
//=====================================================================================================================

//! list of module events
typedef enum
{
    ROTATEXDEG_EV_NONE = 0,               //!< no events to report
    ROTATEXDEG_EV_CW_STROKE_COMPLETED,    //!< module has completed 1 stroke in the clock-wise direction
    ROTATEXDEG_EV_CCW_STROKE_COMPLETED,   //!< module has completed 1 stroke in the counter-clock-wise direction
    ROTATEXDEG_EV_ERROR                   //!< module cannot perform its intended functionality
} ROTATEXDEG_EVENT_TYPE;

//=====================================================================================================================
//-------------------------------------- PUBLIC (Function Prototypes) -------------------------------------------------
//=====================================================================================================================

void RotateXDeg__SetProfile(ROTATEXDEG_PROFILE_TYPE *);
void RotateXDeg__Entry(void);
ROTATEXDEG_EVENT_TYPE RotateXDeg__Do(void);

#endif // ROTATEXDEG_H_


