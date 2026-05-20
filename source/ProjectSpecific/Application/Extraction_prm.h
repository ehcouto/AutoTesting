/**
 * @file
 * @brief       Public parameters for the API221MotionCtrl module.
 *
 * @details     Refer to the C source file for more detailed information.
 *
 * @copyright   Copyright 2017 Whirlpool Corporation.  All rights reserved - CONFIDENTIAL.
 */

//  --- Include Files -------------------------------------------------------------------------------------------------

// -- This Module --
#include "C_Extensions.h"
#include "Mci.h"
#include "MciSensorsWm.h"

// -- Other Modules --

//=====================================================================================================================
//  --- Public Properties ---------------------------------------------------------------------------------------------
//=====================================================================================================================

// -- Public Constant Declarations --

//! Extraction execution rate, in Hz.
#define FS_EXTRACTION							200U

//! Maximum supported acceleration.
#define MAX_RAMP_ACCEL							(sint16)((float32)MAX_RPM_PER_SEC_ACCEL/MciSensorsWm__GetTransmissionRatio()) - 1

//! Minimum supported speed
#define EXTR_MIN_SPEED							(sint16)((float32)MIN_SPEED_RPM/MciSensorsWm__GetTransmissionRatio()) + 1

//! Maximum supported speed
#define EXTR_MAX_SPEED							(sint16)((float32)MAX_SPEED_RPM/MciSensorsWm__GetTransmissionRatio()) - 1

//! Enable the compilation of accelerometer related features.
#define EXTR_FEATURE_ACCEL_ENABLED				0	// 1 - Enabled | 0 - Disabled

//! Enable the compilation of optimal ramp feature.
#define EXTR_FEATURE_OPT_RAMP_ENABLED			0	// 1 - Enabled | 0 - Disabled

//! Enable the ParamEst based high speed logic.
#define EXTR_FEATURE_HIGH_SPEED_PE_ENABLED		0	// 1 - Enabled | 0 - Disabled

// -- Public Enumerated Constant Declarations --

// -- Public Type Declarations --

//=====================================================================================================================
//  --- Public Methods ------------------------------------------------------------------------------------------------
//=====================================================================================================================

#ifndef EXTRACTION_PRM_H_
#define EXTRACTION_PRM_H_

#endif /* EXTRACTION_PRM_H_ */
