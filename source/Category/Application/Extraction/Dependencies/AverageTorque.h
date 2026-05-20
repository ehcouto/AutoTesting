/**
 *  @file       
 *
 *  @brief      API file for the AverageTorque module.
 *
 *  $Header: AverageTorque.h 1.2 2016/01/19 12:55:31EST Paul F Spindler (SPINDPF) Exp  $
 *
 *  @copyright  Copyright 2014-$Date: 2016/01/19 12:55:31EST $. Whirlpool Corporation. All rights reserved - CONFIDENTIAL
 */
//---------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------
#ifndef AVERAGETORQUE_H_
#define AVERAGETORQUE_H_

#include "AverageTorqueDeclare.h"

//=====================================================================================================================
//-------------------------------------- PUBLIC (Extern Variables, Constants & Defines) -------------------------------
//=====================================================================================================================

//! List of module events.
typedef enum
{
    AVERAGE_TORQUE_EV_NONE = 0,
    AVERAGE_TORQUE_EV_NEW_VALUE_CALCULATED      // a average torque value has been calculated
} AVERAGE_TORQUE_EVENT_TYPE;

//! function prototype for AverageTorque event handlers.
typedef void (*AVERAGE_TORQUE_EVENT_HANDLER_TYPE)(AVERAGE_TORQUE_EVENT_TYPE event);

//=====================================================================================================================
//-------------------------------------- PUBLIC (Function Prototypes) -------------------------------------------------
//=====================================================================================================================

void AverageTorque__Initialize(void);
void AverageTorque__Handler1msec(void);
void AverageTorque__Handler5msec(void);

void AverageTorque__Disable(void);
void AverageTorque__Enable(void);
AVERAGE_TORQUE_TYPE AverageTorque__GetValue(void);

void AverageTorque__RegisterCallback(AVERAGE_TORQUE_EVENT_HANDLER_TYPE event_handler);
void AverageTorque__UnregisterCallback(AVERAGE_TORQUE_EVENT_HANDLER_TYPE event_handler);

#endif // AVERAGETORQUE_H_


