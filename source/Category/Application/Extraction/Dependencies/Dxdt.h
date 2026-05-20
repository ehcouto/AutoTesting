/**
 *  @file       
 *
 *  @brief      Basic description of file contents
 *
 *  @section    Applicable_Documents
 *					List here all the applicable documents if needed. <tr>	
 *
 *  $Header: Dxdt.h 1.3 2016/03/10 14:05:21EST Paul F Spindler (SPINDPF) Exp  $
 *
 *  @copyright  Copyright 2014-$Date: 2016/03/10 14:05:21EST $. Whirlpool Corporation. All rights reserved - CONFIDENTIAL
 */
//---------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------
#ifndef DXDT_H_
#define DXDT_H_

#include "AverageTorqueDeclare.h"
#include "limits.h"

//=====================================================================================================================
//-------------------------------------- PUBLIC (Extern Variables, Constants & Defines) -------------------------------
//=====================================================================================================================

//! Dxdt data
typedef struct
{
    AVERAGE_TORQUE_TYPE Max;
    AVERAGE_TORQUE_TYPE Min;
} DXDT_TORQUE_DATA_TYPE;

//! events the module can post to registered handlers
typedef enum
{
    DXDT_EV_NONE = 0,
    DXDT_EV_MAX_TORQUE_DETECTED,            // a Dxdt max (peak) of the Mci torque has been detected
    DXDT_EV_MIN_TORQUE_DETECTED             // a Dxdt min (trough) of the Mci torque has been detected
} DXDT_EVENT_TYPE;

//! function prototype for the Dxdt event handlers.
typedef void (*DXDT_EVENT_HANDLER_TYPE)(DXDT_EVENT_TYPE event);

//=====================================================================================================================
//-------------------------------------- PUBLIC (Function Prototypes) -------------------------------------------------
//=====================================================================================================================

void Dxdt__Initialize(void);

void Dxdt__Disable(void);
void Dxdt__Enable(void);
void Dxdt__GetData(DXDT_TORQUE_DATA_TYPE *data);

void Dxdt__RegisterEventHandler(DXDT_EVENT_HANDLER_TYPE event_handler);
void Dxdt__UnregisterEventHandler(DXDT_EVENT_HANDLER_TYPE event_handler);

#endif // DXDT_H_


