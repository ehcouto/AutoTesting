/**
 *  @file       
 *
 *  @brief      API file for the ParamEstExc (Parameter Estimation Excitation) module.
 *
 *  $Header: ParamEstExc.h 1.3 2015/03/13 09:16:40EDT Paul F Spindler (SPINDPF) Exp  $
 *
 *  @copyright  Copyright 2015-$Date: 2015/03/13 09:16:40EDT $. Whirlpool Corporation. All rights reserved - CONFIDENTIAL
 */
//---------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------
#ifndef PARAMESTEXC_H_
#define PARAMESTEXC_H_

#include "C_Extensions.h"

//=====================================================================================================================
//-------------------------------------- PUBLIC (Extern Variables, Constants & Defines) -------------------------------
//=====================================================================================================================

//!< Excitation method
typedef enum
{
    PARAM_EXC_METHOD_WN = 0,            //!< White noise acceleration signal: Fixed-time, random ramp rate commands
    PARAM_EXC_METHOD_PRBS,              //!< Pseudo-random binary acceleration: Random-time switching between 2 fixed ramp rates
    NUM_OF_PARAM_EXC_METHODS
} PARAM_EST_METHOD_ENUM;

//! Configuration parameters of the Parameter Estimation Excitation Setup.
typedef PACKED  struct
{
    float Amplitude;                    //!< Excitation amplitude (RPM/s)
    float Time;                         //!< Excitation time period (seconds)
    PARAM_EST_METHOD_ENUM Method;		//!< Excitation method

} PARAM_EST_EXC_SETUP_TYPE;

//=====================================================================================================================
//-------------------------------------- PUBLIC (Function Prototypes) -------------------------------------------------
//=====================================================================================================================

void ParamEstExc__Initialize(void);
void ParamEstExc__Handler(void);

BOOL_TYPE ParamEstExc__GetSetup(PARAM_EST_EXC_SETUP_TYPE *parameters);
BOOL_TYPE ParamEstExc__SetSetup(const PARAM_EST_EXC_SETUP_TYPE *parameters);
void ParamEstExc__Enable(void);
void ParamEstExc__Disable(void);
BOOL_TYPE ParamEstExc__IsRunning(void);

#endif // PARAMESTEXC_H_


