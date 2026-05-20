/**
 *  @file       Prescaler_prv.h
 *
 *  @brief
 *
 *  @section
 *
 *  $Header: Prescaler_prv.h 1.3 2015/12/11 14:40:24CET Maciej  Kobuszewski (KOBUSM) Exp  $
 *
 *  @copyright  Copyright 2013-$Date: 2015/12/11 14:40:24CET $. Whirlpool Corporation. All rights reserved - CONFIDENTIAL
 */
//---------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------
#ifndef PRESCALER_PRV_H_
    #define PRESCALER_PRV_H_

//#include "SensorsAlgorithms.h"
#include "ProtocolInfo.h"
#include "CommandManager.h"

/** @brief      List of tasks executed in 2ms slot of the timer interrupt.
 *  @details    Add pointers to routines that should run during this slot in the timer interrupt.
 */
TASK_TYPE Interrupt_2ms_Tasks[] =
{
    //SensorsAlgorithms__2msHandler,
    ProtocolInfo__2msHandler,
    //CommandManager__2msHandler,
	//-------------------------------------------------------------------------
    // Mandatory NULL_TASK must be at the end of the list
    NULL_TASK
};



#endif // PRESCALER_PRV_H_

