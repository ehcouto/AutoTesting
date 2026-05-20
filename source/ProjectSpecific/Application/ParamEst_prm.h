/**
 *  @file
 *
 *  @brief      Public parameters for the ParamEst (Parameter Estimation) module.
 *
 *  @copyright  Copyright 2014 Whirlpool Corporation. All rights reserved - CONFIDENTIAL
 */
//---------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------
#ifndef PARAMEST_PRM_H_
#define PARAMEST_PRM_H_

//=====================================================================================================================
//-------------------------------------- PUBLIC (Extern Variables, Constants & Defines) -------------------------------
//=====================================================================================================================

/**
 * Hard-coded parameters are enabled for this module
 */
#define PARAM_EST_HARD_CODED_PARAMETERS	1 // 1 - Hard-coded parameters enabled | 0 - Disabled

/**
 *  Parameter Estimation Model Backward Euler, with no viscous drag (coloumb friction only).
 *  Type: ENABLED or DISABLED
 */
#define MODEL_BK_EULER_B0       ENABLED

/**
 *  Parameter Estimation Model Backward Euler, with viscous drag and coloumb friction.
 *  Type: ENABLED or DISABLED.
 */
#define MODEL_BK_EULER          ENABLED

/**
 *  Parameter Estimation Model Continuous Model, with viscous drag and coloumb friction.
 *  Type: ENABLED or DISABLED
 */
#define MODEL_HYB_ACCEL         ENABLED

/**
 *  Parameter Estimation Model Backward Euler, with no viscous drag (coloumb friction only) and a Ball Balancer Ring
 *  Type: ENABLED or DISABLED
 */
#define MODEL_BK_EULER_B0_BB    ENABLED

#if MODEL_BK_EULER_B0 == ENABLED
#define PARAM_EST_MODEL_BK_EULER_B0_ENABLED
#endif

#if MODEL_BK_EULER == ENABLED
#define PARAM_EST_MODEL_BK_EULER_ENABLED
#endif

#if MODEL_HYB_ACCEL == ENABLED
#define PARAM_EST_MODEL_HYB_ACCEL_ENABLED
#endif

#if MODEL_BK_EULER_B0_BB == ENABLED
#define PARAM_EST_BK_EULER_B0_BB_ENABLED
#endif

/**
 *  Max order of digital filter (for storage allocation)
 *  Type: unsigned char
 *  Range: 1 to 255
 *  Resolution: 1 = 1 order
 */
#define PARAM_EST_FILTER_MAX_NUM_SECTIONS            3

#define PARAM_EST_P0_DEFAULT    1000.0f                  //!< Default initial covariance, should be > 0

//=====================================================================================================================
//-------------------------------------- PUBLIC (Function Prototypes) -------------------------------------------------
//=====================================================================================================================

#endif // PARAMEST_PRM_H_
