/**
 *  @file
 *
 *  @brief      Private parameters for the ParamEst (Parameter Estimation) module.
 *
 *  @copyright  Copyright 2014 Whirlpool Corporation. All rights reserved - CONFIDENTIAL
 */
//---------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------
#ifndef PARAMEST_PRV_H_
#define PARAMEST_PRV_H_

#include "Log.h"
#include "Mci.h"
#include "BandLimiter.h"

//=====================================================================================================================
//-------------------------------------- PRIVATE (Extern Variables, Constants & Defines) ------------------------------
//=====================================================================================================================

//! list of module log messages
typedef enum
{
    MESSAGE_NONE = 0,
    CANNOT_CHANGE_MODEL_WHILE_MODULE_IS_ACTIVE,
    CANNOT_GET_PARAMETER_VALUE_SETUP_IS_NULL,
    INVALID_MODEL,
    INVALID_ADAPTATION_TYPE,
    INVALID_KALMAN_VALUE,
    INVALID_FORGETTING_FACTOR,
    INVALID_FILTER_ORDER,
    INVALID_PARAMETER_SELECTION,
    MODEL_UPDATE_VIOLATION,
    SETUP_PARAMETERS_NULL
} MODULE_PARAM_EST_LOG_MESSAGE_ID_TYPE;

//! Log module name for messages
#define MODULE_NAME MODULE_PARAM_EST


/**
 *  @brief  A request to change the Setup Model has been received when the module is active.
 *  @param  model   The Model being requested
 */
#define DEBUG_CANNOT_CHANGE_MODEL_WHILE_MODULE_IS_ACTIVE(model)     LOG_ADD_EXCEPTION(CANNOT_CHANGE_MODEL_WHILE_MODULE_IS_ACTIVE, model)

/**
 *  @brief  Request for parameter value was received but the module Setup is NULL.
 */
#define DEBUG_CANNOT_GET_PARAMETER_VALUE_SETUP_IS_NULL()    LOG_ADD_EXCEPTION(CANNOT_GET_PARAMETER_VALUE_SETUP_IS_NULL, 0)

/**
 *  @brief  The Environment is commanding a Setup update while the module is running.
 *  @param  model     The integer representing the choice of model
 */
#define DEBUG_CANNOT_UPDATE_MODEL_WHILE_RUNNING(model)     LOG_ADD_EVENT(MODEL_UPDATE_VIOLATION, model)

/**
 *  @brief  The Environment specified PARAM_EST_SETUP_TYPE variable PARAM_EST_ADAPT_TYPE Adapt_Type is invalid.
 *  @param  model = the invalid value the Environment is specifying
 */
#define DEBUG_INVALID_ADAPTATION_TYPE(adaptation_type)   LOG_ADD_EXCEPTION(INVALID_ADAPTATION_TYPE, adaptation_type)

/**
 *  @brief  The Environment specified PARAM_EST_SETUP_TYPE variable PARAM_EST_FILTER_TYPE Filter is invalid.
 *  @param  order     The value of the invalid filter order
 */
#define DEBUG_INVALID_FILTER_ORDER(order)   LOG_ADD_EXCEPTION(INVALID_FILTER_ORDER, order)

/**
 *  @brief  The Environment specified PARAM_EST_SETUP_TYPE variable Forgetting is invalid.
 *  @param  forgetting     The value of the invalid forgetting factor
 */
#define DEBUG_INVALID_FORGETTING_FACTOR(forgetting) LOG_ADD_EXCEPTION(INVALID_FORGETTING_FACTOR, forgetting)

/**
 *  @brief  The Environment specified PARAM_EST_SETUP_TYPE variable Kalman_Vector is invalid.
 *  @param  index     PARAM_EST_KALMAN_TUNING_TYPE
 *  @param  value     The invalid value
 */
#define DEBUG_INVALID_KALMAN_VALUE(index,value)     LOG_ADD_EXCEPTION(INVALID_KALMAN_VALUE, index)

/**
 *  @brief  The Environment specified PARAM_EST_SETUP_TYPE variable PARAM_EST_MODEL_TYPE Model is invalid.
 *  @param  model = the invalid value the Environment is specifying
 */
#define DEBUG_INVALID_MODEL(model)  LOG_ADD_EXCEPTION(INVALID_MODEL, model)

/**
 *  @brief  The Environment requested index for the PARAM_EST_PARAMETER_TYPE is invalid.
 *  @param  parameter = the integer representing the choice of parameter. Valid options are listed in PARAM_EST_PARAMETER_TYPE
 */
#define DEBUG_INVALID_PARAMETER_SELECTION(parameter) LOG_ADD_EXCEPTION(INVALID_PARAMETER_SELECTION, parameter);

/**
 *  @brief  Indicates a NULL pointer address has been passed to the API ParamEst__Enable().
 */
#define DEBUG_SETUP_PARAMETERS_NULL()             LOG_ADD_EXCEPTION(SETUP_PARAMETERS_NULL, 0)


/**
 *  @brief  The torque applied to the drum by the motor or transmission.
 *  Type: float
 *  Resolution: 1.0 = 1.0 Newton-Meters
 */
#define INPUT_DRUM_TORQUE_NM()   ( BandLimiter__GetSignal(BANDLIMITER_BAND_0,BANDLIMITER_SIGNAL_DRUM_TORQUE_NM) )

/**
 *  @brief  The drum position in radians.
 *  Type: float
 *  Resolution: 1.0 = 1.0 radians
 */
#define INPUT_DRUM_POS_RADIANS()     ( atan2f(INPUT_SIN_DRUM_POS(),INPUT_COS_DRUM_POS()) )

/**
 *  @brief  The drum speed, in rad/s.
 *  Type: MOTOR_SPEED_TYPE
 */
#define INPUT_DRUM_SPEED_RAD_S()     ( (MOTOR_SPEED_TYPE)BandLimiter__GetSignal(BANDLIMITER_BAND_0, BANDLIMITER_SIGNAL_DRUM_SPEED_RAD_S) )

/**
 *  @brief  The sine of drum position.
 *  Type: float
 */
#define INPUT_SIN_DRUM_POS()     ( BandLimiter__GetSignal(BANDLIMITER_BAND_0, BANDLIMITER_SIGNAL_SIN_DRUM_ANGLE) )

/**
 *  @brief  The cosine of drum position.
 *  Type: float
 */
#define INPUT_COS_DRUM_POS()     ( BandLimiter__GetSignal(BANDLIMITER_BAND_0, BANDLIMITER_SIGNAL_COS_DRUM_ANGLE) )

/**
 *  @brief  the value of Pi
 *  Type: float
 */
#define INPUT_PI    ( (float)(PI) )

/**
 *  @brief  The (ms) time slot where the ParamID handler is installed.
 *          Time slot options: 1, 5, 25
 */
#define INPUT_HANDLER_TIME_SLOT         5

/**
 *  @brief  This is the down sample ratio of ParamID. It is the number of periods of the slot which correspond
 *          to a single execution of the ParamID algorithm. Must be an integer. The ParamID sample period is
 *          set by (INPUT_HANDLER_TIME_SLOT * INPUT_HANDLER_SLOT_PERIODS).
 *  Type:   DOWN_SAMPLER_COUNTER_TYPE
 */
#define INPUT_HANDLER_SLOT_PERIODS      2

/**
 *  @brief	Parameter Estimation hard-coded setup, to be used when GESE parameters are not in place.
 */
#if PARAM_EST_HARD_CODED_PARAMETERS

#define NUM_FILTERS 2

static const DIGITAL_FILTER_BIQUAD_COEFF_TYPE Param_Est_LS_Filter_Coeffs[] = {
		{   // Section 0
				.Num = {2.0f, 1.0f},
				.Den = {-1.93529439f, 0.9391207695f},
				.Gain = 0.0009566029767f
		},
		{   // Section 1
				.Num = {1.0f, 0.0f},
				.Den = {-0.9390624762f, 0.0f},
				.Gain = 0.03046874702f
		},
};

static const DIGITAL_FILTER_BIQUAD_COEFF_TYPE Param_Est_HS_Filter_Coeffs[] = {
		{   // Section 0
				.Num = {2.0f, 1.0f},
				.Den = {-0.75353729724884033f,  0.40630760788917542f},
				.Gain = 0.16319257020950317f
		},
		{   // Section 1
				.Num = {1.0f, 0.0f},
				.Den = {-0.2905268669128418f, 0.0f},
				.Gain = 0.3547365665435791f
		},
};

static const DIGITAL_FILTER_SOS_TYPE Param_Est_Filter_Bank[] = {
		{
				.Num_Sections = 2,
				.Coeffs = (DIGITAL_FILTER_BIQUAD_COEFF_TYPE*)Param_Est_LS_Filter_Coeffs
		},
		{
				.Num_Sections = 2,
				.Coeffs = (DIGITAL_FILTER_BIQUAD_COEFF_TYPE*)Param_Est_HS_Filter_Coeffs
		}
};

#endif // PARAM_EST_HARD_CODED_PARAMETERS

//=====================================================================================================================
//-------------------------------------- PRIVATE (Function Prototypes) ------------------------------------------------
//=====================================================================================================================

#endif // PARAMEST_PRV_H_


