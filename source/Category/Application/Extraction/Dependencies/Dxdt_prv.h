/**
 *  @file       
 *
 *  @brief      Private parameters for the Dxdt module.
 *
 *  $Header: Dxdt_prv.h 1.3 2016/01/19 13:01:33EST Paul F Spindler (SPINDPF) Exp  $
 *
 *  @copyright  Copyright 2014-$Date: 2016/01/19 13:01:33EST $. Whirlpool Corporation. All rights reserved - CONFIDENTIAL
 */
//---------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------
#ifndef DXDT_PRV_H_
#define DXDT_PRV_H_

#include "DistributionUtils_prv.h"

//=====================================================================================================================
//-------------------------------------- PRIVATE (Extern Variables, Constants & Defines) ------------------------------
//=====================================================================================================================

/**
 *  defines the size of the module callback register
 *  Type: unsigned char
 *  Range: 1 to 255
 *  Resolution: 1 = 1 handler
 */
#define SIZE_OF_CALLBACK_REGISTER   3


/**
 *  Configuration definition for dxdt calculations.
 *  The time base of the average torque measurements
 *
 *  Resolution: 1.0 = 1.0 seconds
 */
#define SNRD_TS_DER     ( 0.050f )

/**
 *  Configuration definition for dxdt calculations.
 *  AverageTorque module produces a reading every 10msec.  Defines the time base base for the Dxdt module measurement.
 *
 *  Resolution: 1 = 1 AverageTorque reading (AverageTorque is configured to provide readings every 10msec)
 */
#define DXDT_CONFIG_TIME_BASE   P2P_TORQUE_SNRD_SAMPLE_RATE

//! Maximum time base for Dxdt measurements
#define DXDT_CONFIG_MAX_TIME_BASE	10	//! 100ms

//=====================================================================================================================
//-------------------------------------- PRIVATE (Function Prototypes) ------------------------------------------------
//=====================================================================================================================

#endif // DXDT_PRV_H_


