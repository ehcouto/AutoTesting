/**
 *  @file
 *  @brief       Motor Control Loop 3-phase motors: DQ Reference Generation Macro Block.
 *  @details     This module implements theDQ Reference Generation  module.
 *  @author      alessio.beato/luigi.fagnano  (only temporary, since it is not integrated in MKS)
 *  $Header: FOC/Mcl.h 1.3 2015/11/05 11:53:55CET Marcelo Silva (SILVAMC) Exp  $
 * @copyright Copyright 2012 - $Date: 2015/11/05 11:53:55CET $. Whirlpool Corporation. All rights reserved � CONFIDENTIAL
*/
/*
 *
 *---------------------------------------------------------------------------------------------------------------------
 *---------------------------------------------------------------------------------------------------------------------
 */
#ifndef MCL_OBSERVER_H_
    #define MCL_OBSERVER_H_


//-------------------------------------- Include Files ----------------------------------------------------------------
#include "C_Extensions.h"
#include "MclCrossTypes.h"
#include "MclConfig.h"



//=====================================================================================================================
//-------------------------------------- PUBLIC (Extern Variables, Constants & Defines) -------------------------------
//=====================================================================================================================


typedef struct
{
    float32 Stator_Resistance;                       //!< [Ohm] stator resistance
    float32 Stator_Resistance_Corr_Limits;           //!< [%] 17.15 adapted stator resistance limits as percentage
    float32 Stator_Resistance_Gain;                  //!< [Ohm/(s*Flux)] IQ30 stator resistance adaptation gain
    float32 Curr_Resistance_Threshold;               //!< [A] magnitude of currents to enable/disable gain adaptation
    float32 Curr_Resistance_Spd_Threshold;           //!< [rad/s or rpm] - speed to enable/disable gain adaptation
    float32 Phase_Advance_Coeff;                     //!< feedback voltage phase advance coefficient
    float32 Rotor_Flux_Nominal;                      //!< [Vs/rad] Q15 - Rotor magnets flux constant
    float32 Observer_G_gain;
    float32 Observer_G_integ_gain;
    float32 Observer_G_integ_limits;
    float32 Observer_G_integ_Spd_Threshold;
    float32 Pole_Pairs;

    uint32 Starting_Threshold;                      //!< [x Ts] - count time after starting

    float32 PLL_Kp;                                   //!< pll proportional gain
    float32 PLL_Ki;                                   //!< pll integral gain
    float32 Pll_Integ_K;                              //!< pll time integration constant

} MCL_OBSERVER_PARAMS_TYPE;


typedef struct
{
    MCL_OBSERVER_PARAMS_TYPE     *ObsPrm;
    MATHCALC_LUT_EXT_F_TYPE       Ld_lut;
    MATHCALC_LUT_EXT_F_TYPE       Lq_lut;
    MATHCALC_LUT_EXT_F_TYPE       Ldq_lut;
    MATHCALC_LUT_EXT_F_TYPE       Rotor_Flux_lut;
}MCLOBSERVER_JOINT_PARAMS_TYPE;


//=====================================================================================================================
//-------------------------------------- PUBLIC (Function Prototypes) -------------------------------------------------
//=====================================================================================================================

void MclObserver__ResetState(MCL_OBSERVER_IO_F_TYPE *io, MCLOBSERVER_JOINT_PARAMS_TYPE *params);
void MclObserver__Initialize(MCL_OBSERVER_IO_F_TYPE *io, MCLOBSERVER_JOINT_PARAMS_TYPE *params);
void MclObserver__RunningHandler(MCL_OBSERVER_IO_F_TYPE *io, MCLOBSERVER_JOINT_PARAMS_TYPE *params);
void MclObserver__1msHandler(MCL_OBSERVER_IO_F_TYPE *io, MCLOBSERVER_JOINT_PARAMS_TYPE *params);
void MclObserver__UpdateResistance25ms(sint32 updated_resistance);

#endif // MCL_OBSERVER_H_
