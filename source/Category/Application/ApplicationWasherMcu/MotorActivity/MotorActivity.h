/**
 * @file
 * @brief       Public interface to the MotorActivity module.
 *
 * @details     Refer to the MotorActivity.c source file for more detailed information.
 *
 * @copyright   Copyright 2016 Whirlpool Corporation.  All rights reserved - CONFIDENTIAL.
 */

#ifndef MOTORACTIVITY_H_
#define MOTORACTIVITY_H_


//  --- Include Files -------------------------------------------------------------------------------------------------

// -- This Module --

// -- Other Modules --
#include "Motion.h"


//=====================================================================================================================
//  --- Public Properties ---------------------------------------------------------------------------------------------
//=====================================================================================================================

// -- Public Constant Declarations --


// -- Public Type Declarations --

typedef enum MOTOR_ACTIVITY_COMMAND_ENUM
{
    MOTOR_ACTIVITY_STOP     = 0,
    MOTOR_ACTIVITY_RUN      = 1,
    MOTOR_ACTIVITY_PULSE    = 2
} MOTOR_ACTIVITY_COMMAND_TYPE;


//=====================================================================================================================
//  --- Public Methods ------------------------------------------------------------------------------------------------
//=====================================================================================================================

BOOL_TYPE MotorActivity__Stop(uint16 deceleration, MOTION_REQUESTOR_TYPE requestor);
BOOL_TYPE MotorActivity__Run(sint16 velocity, uint16 acceleration, MOTION_REQUESTOR_TYPE requestor);
BOOL_TYPE MotorActivity__Set(uint8* parameters, uint8 size, MOTION_REQUESTOR_TYPE requestor);
void MotorActivity__GetStatus(uint8** buffer, uint8* size);


#endif      /* MOTORACTIVITY_H_ */
