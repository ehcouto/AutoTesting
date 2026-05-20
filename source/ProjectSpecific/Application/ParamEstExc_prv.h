/**
 *  @file       
 *
 *  @brief      Private parameters for the ParamEstExc (Parameter Estimation Excitation) module.
 *
 *  $Header: ParamEstExc_prv.h 1.1 2016/02/02 14:22:03EST Guilherme Bencke Teixeira da Silva (BENCKG1) Exp  $
 *
 *  @copyright  Copyright 2015-$Date: 2016/02/02 14:22:03EST $. Whirlpool Corporation. All rights reserved - CONFIDENTIAL
 */
//---------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------
#ifndef PARAMESTEXC_PRV_H_
#define PARAMESTEXC_PRV_H_

#include "Log.h"
#include "Mci.h"
#include "MciSensorsWm.h"

//=====================================================================================================================
//-------------------------------------- PRIVATE (Extern Variables, Constants & Defines) ------------------------------
//=====================================================================================================================

//! list of module log messages
typedef enum
{
    MESSAGE_NONE = 0,
    CANNOT_ENABLE_MODULE_SETUP_ADDRESS_IS_NULL, //!< data: na
    INVALID_METHOD,                             //!< data: the number being requested
    INVALID_TIME,                               //!< data: time being requested
    MIN_SPEED_VIOLATION,                        //!< data: speed detected under the min speed
    SETUP_PARAMETER_IS_NULL                     //!< data: na
} MODULE_PARAM_EST_EXCITATION_LOG_MESSAGE_ID_TYPE;

//! Log module name for messages
#define MODULE_NAME MODULE_PARAM_EST_EXCITATION         // Parameter Estimation Excitation

/**
 *  @brief  The PARAM_EST_EXC_SETUP_TYPE variable Time is invalid.
 *  @param exc_time    The value of the invalid excitation time
 */
#define DEBUG_INVALID_EXC_TIME(exc_time)     LOG_ADD_EXCEPTION(INVALID_TIME, exc_time)

/**
 *  @brief  The PARAM_EST_EXC_SETUP_TYPE variable PARAM_EST_METHOD_ENUM Method is invalid.
 *  @param  method = the invalid method index being passed
 */
#define DEBUG_INVALID_EXC_METHOD(method)        LOG_ADD_EXCEPTION(INVALID_METHOD, method)

/**
 *  @brief    The API command ParamEstExc__Enable was received but the Setup_Parameters address is NULL.  Module will not be enabled.
 */
#define DEBUG_CANNOT_ENABLE_EXCITATION_SETUP_IS_NULL()      LOG_ADD_EXCEPTION(CANNOT_ENABLE_MODULE_SETUP_ADDRESS_IS_NULL, 0);

/**
 *  @brief    The API ParamEstExc__Setup() was called and the parameter was NULL.
 */
#define DEBUG_SETUP_PARAMETER_IS_NULL()     LOG_ADD_EXCEPTION(SETUP_PARAMETER_IS_NULL, 0)

/**
 *  @brief  The minimum speed required for the module to be active has not been reached.
 *  @param speed = the speed in RPM (rounded to nearest) when the violation occurred
 */
#define DEBUG_MIN_SPEED_VIOLATION(speed)    LOG_ADD_EVENT(MIN_SPEED_VIOLATION, speed)



/**
 *  @brief  the current motor rpm
 *  Type: MOTOR_SPEED_TYPE
 */
#define INPUT_MOTOR_SPEED()     (((MOTOR_SPEED_TYPE)Mci__GetAnalog(MOTOR0, MCI_AI_ACTUAL_SPEED_S16)*(1.0f/65536.0f)))

/**
 *  @brief  Minimum speed (RPM) for running the module. Below this speed, the module will not start, and
 *          if it is running, it will stop.
 *  Type: MOTOR_SPEED_TYPE
 */
#define INPUT_MIN_SPEED     ( (MOTOR_SPEED_TYPE)(30.0f) )

/**
 *  @brief  Minimum hold time for excitation generation
 *  Type:       float
 *  Resolution: 1.0 = 1.0seconds
 */
#define INPUT_EXCITATION_MIN_TIME      0.1f

/**
 *  @brief      The (ms) time slot where the ParamEstExc__Handler() is installed.
 *  @details    It is recommended to use a 1ms sample time or faster to avoid torque pulsations and audible noise.
 *  Type:       1, 5 or 25
 */
#define INPUT_HANDLER_TIME_SLOT        1

/**
 *  @brief      This is the downsample ratio of Excitation.
 *  @details    It is the number of periods of the slot which correspond to a single execution of the Excitation
 *              generation. Must be an integer.
 *              The Excitation sample period is set by (INPUT_HANDLER_TIME_SLOT * INPUT_HANDLER_SLOT_PERIODS).
 */
#define INPUT_HANDLER_SLOT_PERIODS     1

/**
 *  @brief  adds an offset to the speed reference generator
 *  Type: MOTOR_SPEED_TYPE
 */
#define OUTPUT_SPEED_REF_GEN_OFFSET(value)		Mci__SetAnalog(MOTOR0, MCI_AO_ADD_DELTA_SPEED_REF_S16, (sint32)(MciSensorsWm__GetTransmissionRatio() * value * 65536.0f))
//=====================================================================================================================
//-------------------------------------- PRIVATE (Function Prototypes) ------------------------------------------------
//=====================================================================================================================

#endif // PARAMESTEXC_PRV_H_


