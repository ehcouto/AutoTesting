/**
 *  @file
 *
 *  @brief      Private interface file for the Satelization module.
 *
 *  @details    See Satelization.c for details.
 *
 *  $Header: Satelization_prv.h 1.1 2016/01/19 13:02:01EST Paul F Spindler (SPINDPF) Exp  $
 *
 *  @copyright  Copyright 2012 - $Date: 2016/01/19 13:02:01EST $  Whirlpool Corporation.  All rights reserved - CONFIDENTIAL.
 */
#ifndef SATELIZATION_PRV_H_
#define SATELIZATION_PRV_H_

#include "DistributionUtils_prv.h"

//=====================================================================================================================
//-------------------------------------- PUBLIC (Extern Variables, Constants & Defines) -------------------------------
//=====================================================================================================================

/**
 *  Module input for getting the current Mci reference speed
 *  Type: signed short
 *  Range: 0 to 0xFFFF
 *  Resolution: 1 = rpm
 */
#define INPUT_REFERENCE_SPEED       DIST_UTILS_INPUT_SPEED_REF_X16

#define MAX_SATELIZATION_RPM_X16	SATELLIZATION_CALC_MAX_VELOCITY*16
#define SATELIZATION_WINDOW_POWER_THRESHOLD   WINDOW_POWER_THRESHOLD

//=====================================================================================================================
//-------------------------------------- PUBLIC (Function Prototypes) -------------------------------------------------
//=====================================================================================================================



#endif /* SATELIZATION_PRV_H_ */
