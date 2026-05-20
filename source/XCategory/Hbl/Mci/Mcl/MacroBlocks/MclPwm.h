/**
 *  @file
 *  @brief       Motor Control Loop 3-phase motors: DQ Reference Controllers Macro Block.
 *  @details     This module implements theDQ Reference Controllers  module.
 *  @author      alessio.beato/luigi.fagnano  (only temporary, since it is not integrated in MKS)
 *  $Header: FOC/Mcl.h 1.3 2015/11/05 11:53:55CET Marcelo Silva (SILVAMC) Exp  $
 * @copyright Copyright 2012 - $Date: 2015/11/05 11:53:55CET $. Whirlpool Corporation. All rights reserved � CONFIDENTIAL
*/
/*
 *
 *---------------------------------------------------------------------------------------------------------------------
 *---------------------------------------------------------------------------------------------------------------------
 */
#ifndef MCL_PWM_H_
    #define MCL_PWM_H_


//-------------------------------------- Include Files ----------------------------------------------------------------
#include "C_Extensions.h"
#include "MclCrossTypes.h"


//=====================================================================================================================
//-------------------------------------- PUBLIC (Extern Variables, Constants & Defines) -------------------------------
//=====================================================================================================================

typedef struct
{
    float32 Speed_Threshold_InvComp;                 //!< [rad/s or rpm] - Q17.15 - speed to enable or disable inverter comp
} MCL_PWM_PARAMS_TYPE;

typedef struct{
//! Pwm Parameters
    MCL_PWM_PARAMS_TYPE		 *PwmPrm;
    MATHCALC_LUT_EXT_F_TYPE   PwmModulationPrm;
} MCLPWM_JOINT_PARAMS_TYPE;

//=====================================================================================================================
//-------------------------------------- PUBLIC (Function Prototypes) -------------------------------------------------
//=====================================================================================================================

void MclPwm__ResetState(MCL_PWM_IO_F_TYPE *io, MCLPWM_JOINT_PARAMS_TYPE *params);
void MclPwm__Initialize(MCLPWM_JOINT_PARAMS_TYPE *params);
void MclPwm__RunningHandler(MCL_PWM_IO_F_TYPE *io, MCLPWM_JOINT_PARAMS_TYPE *params);
void MclPwm__25msHandler(MCL_PWM_IO_F_TYPE *io);

#endif // MCL_PWM_H_
