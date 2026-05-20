/**
 *  @file
 *
 *  @brief      Public declarations for the Motion module.
 *
 *  $Header: MotionDeclare.h 1.6 2016/03/02 10:38:23EST Paul F Spindler (SPINDPF) Exp  $
 *
 *  @copyright  Copyright 2015-$Date: 2016/03/02 10:38:23EST $. Whirlpool Corporation. All rights reserved - CONFIDENTIAL
 */
//---------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------
#ifndef MOTIONDECLARE_H_
#define MOTIONDECLARE_H_

#include "MotionDeclare_prm.h"

//=====================================================================================================================
//-------------------------------------- PUBLIC (Extern Variables, Constants & Defines) -------------------------------
//=====================================================================================================================

/**
 *  Defines the variable type for the basket/drum speed on a washing machine.
 *  @details    + values indicate is the drum rotating in the Clock Wise direction.
 *              - values indicate is the drum rotating in the Counter Clock Wise direction.
 *              The direction is reference from the access point in the machine for loading clothes.
 *  @resolution 1 = 1rpm
 */
typedef sint16 MOTION_DRUM_VELOCITY_TYPE;


/**
 *  Defines the variable type for the basket/drum ramp rate in a washing machine.
 *  @details    direction independent.
 *  @resolution 1 = 1rpm/sec
 */
typedef uint16 MOTION_DRUM_ACCELERATION_TYPE;

/**
 *  Defines the excitation rate of the Motion__Background() function
 *  @resolution 1 = 1msec
 */
#define MOTION_HANDLER_EXECUTION_RATE   25


/**
 *  Default deceleration to use when stopping the motor.
 *  @resolution 1 = 1 rpm/sec
 */
#define DEFAULT_STOP_DECELERATION 100

#ifndef MOTION__OPTIONAL_INITIALIZE
#define MOTION__OPTIONAL_INITIALIZE()
#endif

#ifndef MOTION__PREPROCESS_RUN
#define MOTION__PREPROCESS_RUN()
#endif

#ifndef MOTION__PREPROCESS_STOP
#define MOTION__PREPROCESS_STOP()
#endif


#endif // MOTIONDECLARE_H_


