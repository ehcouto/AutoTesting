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
#ifndef MCL_DQ_REF_H_
    #define MCL_DQ_REF_H_


//-------------------------------------- Include Files ----------------------------------------------------------------
#include "C_Extensions.h"
#include "MclCrossTypes.h"


//=====================================================================================================================
//-------------------------------------- PUBLIC (Extern Variables, Constants & Defines) -------------------------------
//=====================================================================================================================

typedef struct
{
    float32 Rotor_Flux_Nominal;                      //!< [Vs/rad] - rotor flux
    float32 Max_Flux;                                //!< [Vs/rad] - Max abs flux; sqrt((Ls*Ismax)^2 + LambdaM^2)*K
    uint32 Starting_Threshold;                       //!< [x Ts] - count time after starting
    float32 Over_Flux_Decrement;                     //!< [Vs/rad] - decrement rate while forced flux is engaged
    float32 Over_Flux_Speed_Threshold;               //!< [rpm] - Speed limit where overflux is applied
    float32 Over_Flux_Ratio_Max;                     //!< [%] - Over flux percentage during starting
    float32 Torque_Err_K;                            //!< K for torque error compensation
    float32 Torque_Err_Sat;                          //!< torque error compensation saturation value
    float32 m_index;                                 //!< modulation index limitation
    float32  K_torque_inv;                           //!< 1/Ktorque constant [Nm/A] - equal to 1.5*p*Rotor_Lambda_M
} MCL_DQ_REF_PARAMS_TYPE;



typedef struct{
    MCL_DQ_REF_PARAMS_TYPE  *DqRefPrm;
    MATHCALC_LUT_EXT_F_TYPE   MTPAPrm;
} MCLDQREF_JOINT_PARAMS_TYPE;

//=====================================================================================================================
//-------------------------------------- PUBLIC (Function Prototypes) -------------------------------------------------
//=====================================================================================================================

void MclDqRef__ResetState(MCLDQREF_JOINT_PARAMS_TYPE *params);
void MclDqRef__Initialize(void);
void MclDqRef__RunningHandler(MCL_DQ_REF_IO_F_TYPE *io, MCLDQREF_JOINT_PARAMS_TYPE *params);
void MclDqRef__1msHandler(MCL_DQ_REF_IO_F_TYPE *io, MCLDQREF_JOINT_PARAMS_TYPE *params);

#endif // MCL_DQ_REF_H_
