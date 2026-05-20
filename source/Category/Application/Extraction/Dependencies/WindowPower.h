/**
 *  @file
 *
 *  @brief      API file for the Window Power module.
 *
 *  @details    See WindowPower.c details.
 *
 *  $Header: WindowPower.h 1.2 2016/01/19 12:58:11EST Paul F Spindler (SPINDPF) Exp  $
 *
 *  @copyright  Copyright 2012 - $Date: 2016/01/19 12:58:11EST $  Whirlpool Corporation.  All rights reserved - CONFIDENTIAL.
 */

#ifndef WINDOWPOWER_H_
#define WINDOWPOWER_H_


#include "HighPassFilter.h"

//=====================================================================================================================
//-------------------------------------- PUBLIC (Extern Variables, Constants & Defines) -------------------------------
//=====================================================================================================================

typedef float WINDOW_POWER_VALUE_TYPE;

typedef enum
{
    WINDOW_POWER_EV_NONE = 0,
    WINDOW_POWER_EV_NEW_VALUE_CALCULATED
} WINDOW_POWER_EVENT_TYPE;

//! function prototype for WindowPower event handlers.
typedef void (*WINDOW_POWER_EVENT_HANDLER_TYPE)(WINDOW_POWER_EVENT_TYPE event);

//=====================================================================================================================
//-------------------------------------- PUBLIC (Function Prototypes) -------------------------------------------------
//=====================================================================================================================

void WindowPower__Initialize(void);

void WindowPower__Disable(void);
void WindowPower__Enable(void);
WINDOW_POWER_VALUE_TYPE WindowPower__GetValue(void);
HIGH_PASS_FILTER_VALUE_TYPE WindowPower__GetHighPassFilteredTorque(HIGH_PASS_FILTER_TYPE hpf_type);

void WindowPower__RegisterEventHandler(WINDOW_POWER_EVENT_HANDLER_TYPE event_handler);
void WindowPower__UnregisterEventHandler(WINDOW_POWER_EVENT_HANDLER_TYPE event_handler);

#endif /* WINDOWPOWER_H_ */
