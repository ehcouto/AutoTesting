/**
 *  @file       
 *
 *  @brief      Private declarations for the AverageTorque module.
 *
 *  $Header: AverageTorque_prv.h 1.1 2016/01/19 13:00:23EST Paul F Spindler (SPINDPF) Exp  $
 *
 *  @copyright  Copyright 2014-$Date: 2016/01/19 13:00:23EST $. Whirlpool Corporation. All rights reserved - CONFIDENTIAL
 */
//---------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------
#ifndef AVERAGETORQUE_PRV_H_
#define AVERAGETORQUE_PRV_H_

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
#define SIZE_OF_CALLBACK_REGISTER   2

/**
 *  defines the output rate of the average torque value
 *  Type: unsigned char
 *  Range: 1 to 255
 *  Resolution: 1 = 1msec
 */
#define CALCULATED_AVERAGE_TORQUE_DATA_RATE     10          // 10msec

/**
 *  Module input for the torque.
 *  Type: AVERAGE_TORQUE_TYPE
 */
#define INPUT_TORQUE        DIST_UTILS_INPUT_TORQUE_X128

//=====================================================================================================================
//-------------------------------------- PUBLIC (Function Prototypes) -------------------------------------------------
//=====================================================================================================================

#endif // AVERAGETORQUE_PRV_H_


