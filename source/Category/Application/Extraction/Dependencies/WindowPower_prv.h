/**
 *  @file
 *
 *  @brief      Private parameters for the Window Power module.
 *
 *  @details    See WindowPower.c details.
 *
 *  $Header: WindowPower_prv.h 1.2 2016/03/10 14:23:09EST Paul F Spindler (SPINDPF) Exp  $
 *
 *  @copyright  Copyright 2012 - $Date: 2016/03/10 14:23:09EST $  Whirlpool Corporation.  All rights reserved - CONFIDENTIAL.
 */

#ifndef WINDOWPOWER_PRV_H_
#define WINDOWPOWER_PRV_H_

#include "AverageTorque.h"
#include "DistributionUtils_prv.h"

//=====================================================================================================================
//-------------------------------------- PUBLIC (Extern Variables, Constants & Defines) -------------------------------
//=====================================================================================================================

/**
 *  defines the size of the module callback register
 *  Type: unsigned char
 *  Range: 1 to 255
 *  Resolution: 1 = 1 handler
 */
#define SIZE_OF_CALLBACK_REGISTER   1

/**
 *  defines the execution rate of the WindowPower module
 *  Type: unsigned char
 *  Range: 1 to 255
 *  Resolution: 1 = 10msec, input for average torque readings is the AverageTorque module, which posts a new torque reading every 10msec
 */
#define NUM_OF_AVERAGE_TORQUE_READINGS  1   // 10msec

#define WINDOW_POWER_ELEMENT_CALL_RATE_SEC             (0.010f)

/**
 *  Module input for getting the current Mci reference speed
 *  Type: signed short
 *  Range: 0 to 0xFFFF
 *  Resolution: 1 = 1/INPUT_REFERENCE_SPEED_MULTIPLIER rpm
 */
#define INPUT_REFERENCE_SPEED       DIST_UTILS_INPUT_SPEED_REF_X16

/**
 *  Speed multiplier
 *  Type: signed short
 *  Range: 0 to 0xFFFF
 *  Resolution: 1 rpm
 */
#define INPUT_REFERENCE_SPEED_MULTIPLIER	16

/**
 *  Speed threshold in order to use different filters in different speed ranges
 *  Type: signed short
 *  Range: 0 to 0xFFFF
 *  Resolution: 1 rpm
 */
#define SPEED_THRESHOLD_01		SATELLIZATION_HPF_SELECTION_SPEED

/**
 *  Module input for getting the average torque
 *  Type: AVERAGE_TORQUE_TYPE
 */
#define INPUT_AVERAGE_TORQUE()      ( AverageTorque__GetValue() )


#define WINDOW_POWER_MAX_NUM_OF_ELEMENTS_IN_SUMMATION	   20//!< NOTE: must be >= 2; the number of elements in the window.

//=====================================================================================================================
//-------------------------------------- PUBLIC (Function Prototypes) -------------------------------------------------
//=====================================================================================================================


#endif /* WINDOWPOWER_PRV_H_ */
