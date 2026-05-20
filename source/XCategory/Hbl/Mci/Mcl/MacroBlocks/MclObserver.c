/**
 *  @file
 *  @brief       Motor Control Loop Observer .
 *  @details     This module implements the Observer module.
 *  @author      alessio.beato/luigi.fagnano  (only temporary, since it is not integrated in MKS)
 *  $Header: FOC/Mcl.c 1.12 2015/11/25 18:36:02CET Luigi Fagnano (FAGNAL) Exp  $
 * @copyright Copyright 2012 - $Date: 2015/11/25 18:36:02CET $. Whirlpool Corporation. All rights reserved � CONFIDENTIAL
*/
/*
 *
 *---------------------------------------------------------------------------------------------------------------------
 *---------------------------------------------------------------------------------------------------------------------
 */
//-------------------------------------- Include Files ----------------------------------------------------------------
#include "MclObserver.h"
#include "MclConfig.h"
#include "McMathCalc_macros.h"
#include "ClrkPark.h"
#include "Filters.h"
#include "MathCalc.h"
#if (OBS__INVERTER_DISTORSION_COMPENSATION == ENABLED)
#include "InverterComp.h"
#endif
#include "Mci_prm.h"



float32 Rotor_Curr_Id;
float32 Rotor_Curr_Iq;
float32 Rotor_Curr_Iq_Abs;
float32 Rotor_Curr_Id_Abs;

float32 Pole_Pairs_Inv;


//! PLL variables and parameters structure
typedef struct
{
    float kp;
    float ki;
    float in_alpha;
    float in_beta;
    float speed;
    float speed_old;
    float integral;
    float angle;
    float alpha;
    float beta;
    float error;
    float error_old;
    float integ_k;                                  //!< Constant for integration
    float speed_2pu;                                //!< pll convert to speed pu
} XPLL_TYPE;

XPLL_TYPE XPLLVars;

#ifndef PI
    #define PI  3.1415926535897932384626433832795f
#endif
#define  PARAMS_EMF_INT_CONSTANT    0.5f * TS

//-------------------------------------- PUBLIC (Variables) -----------------------------------------------------------



//-------------------------------------- PRIVATE (Variables, Constants & Defines) -------------------------------------
#if (OBS__INVERTER_DISTORSION_COMPENSATION == ENABLED)
ALPHA_BETA_COOR_SYST_F_TYPE Inverter_Distortion;
INVERTER_COMP_TYPE Inv_Dist_Pointer;
#endif
ALPHA_BETA_COOR_SYST_F_TYPE Stator_Emf;
ALPHA_BETA_COOR_SYST_F_TYPE Stator_Emf_Err;
ALPHA_BETA_COOR_SYST_F_TYPE Stator_Lambda;        //!< [Vs] Q30 format! - stator flux from integrated voltages (emf, losses, etc.)
ALPHA_BETA_COOR_SYST_F_TYPE Rotor_Lambda;
float32 Rotor_Lambda_Mod;
float32 Sin_Rot_k1;
float32 Cos_Rot_k1;
float32 EM_Torque;                               //!< [Nm] Q15 - instantaneous electromagnetic torque
float32 EM_Torque_Filt_Sum;


float32 dLambdas_alpha_old;
float32 dLambdas_beta_old;
float32 Rotor_Position;
float32 Rotor_Mech_Position;
float32 Flux_Error;                              //!< [Vs/rad] Q15 - flux error to close one loop in the observer stator resistance adaptation
XPLL_TYPE Speed_PLL;

float32 Obs_Speed_Stator_Flux;
float32 Obs_Speed_Rotor_Flux;

float32 Last_Mech_Rotor_Pos;
float32 Mech_Rotor_Pos_Acc_Err;

float32 Obs_Speed_Rotor_Flux_sum;                //!< Rotor flux speed history

float32 Error_Flux_Alpha_1;
float32 Error_Flux_Beta_1;

float32 Stator_Resistance_Corr;
float32 Err_Lim_Lower;
float32 Err_Lim_Upper;

uint32 Speed_Transition_Counter;
//-------------------------------------- PRIVATE (Function Prototypes) ------------------------------------------------

//=====================================================================================================================
//-------------------------------------- Public Functions -------------------------------------------------------------
//=====================================================================================================================
#if (OBS__VOLTAGE_PHASE_SHIFT == ENABLED)
void Voltage_Phase_Shift(MCL_OBSERVER_IO_F_TYPE *io, MCL_OBSERVER_PARAMS_TYPE *params);
#endif
#if (OBS__INVERTER_DISTORSION_COMPENSATION == ENABLED)
void InvDistortion_Comp(MCL_OBSERVER_IO_F_TYPE *io, MCL_OBSERVER_PARAMS_TYPE *params);
#endif
void Resistance_Adaptation(MCL_OBSERVER_IO_F_TYPE *io, MCL_OBSERVER_PARAMS_TYPE *params);
void Emf_Calc(MCL_OBSERVER_IO_F_TYPE *io);
void Inductance_Correction(MCL_OBSERVER_IO_F_TYPE *io, MCLOBSERVER_JOINT_PARAMS_TYPE *params);
void RotorFlux_Correction(MCL_OBSERVER_IO_F_TYPE *io, MCLOBSERVER_JOINT_PARAMS_TYPE *params);
void StatorFlux_Calc(MCL_OBSERVER_IO_F_TYPE *io, MCL_OBSERVER_PARAMS_TYPE *params);
void Speed_PLL_Calc(void);
void ElectroMagneticTorqueCalc(MCL_OBSERVER_IO_F_TYPE *io, MCL_OBSERVER_PARAMS_TYPE *params);
void Speed_Selector(MCL_OBSERVER_IO_F_TYPE *io, MCL_OBSERVER_PARAMS_TYPE *params);
void Speed_Selector_Check(MCL_OBSERVER_IO_F_TYPE *io, MCL_OBSERVER_PARAMS_TYPE *params);


void PLL__fReset(XPLL_TYPE *XPLLVars);
void PLL__fCalc(XPLL_TYPE *XPLLVars);


//---------------------------------------------------------------------------------------------------------------------
/**
 *  @brief      Reset all Mcl Observer quantities.
 *  @details    This routine reset all q Reference Generator quantities, it has to be called at every time the pwm is switched off (motor stop or free down ramp).
 *
 *
 *  @param[in]     
 *  @param[out]
 *  @return        
 */
void MclObserver__ResetState(MCL_OBSERVER_IO_F_TYPE *io, MCLOBSERVER_JOINT_PARAMS_TYPE *params)
{
    float32 cmd_flux_level;
    SIN_COS_F_TYPE cmd_sin_cos_pos;


    // Private variables reset
    dLambdas_alpha_old = 0.0f;
    dLambdas_beta_old = 0.0f;

    Stator_Emf.Alpha = 0.0f;
    Stator_Emf.Beta = 0.0f;
    Stator_Emf_Err.Alpha = 0.0f;
    Stator_Emf_Err.Beta = 0.0f;

    // Initialize torque estimator
    EM_Torque = 0.0f;
    EM_Torque_Filt_Sum = 0.0f;

    PLL__fReset(&Speed_PLL);

    Obs_Speed_Stator_Flux = 0.0f;
    Obs_Speed_Rotor_Flux = 0.0f;

    Rotor_Position = *(io->Ctrl_Specific->DcInjection_Angle);
    MathCalc__SinCosF(Rotor_Position, &cmd_sin_cos_pos);

    io->Sin_Cos_Position_Flux->Sin = cmd_sin_cos_pos.Sin;
    io->Sin_Cos_Position_Flux->Cos = cmd_sin_cos_pos.Cos;

    Sin_Rot_k1 = io->Sin_Cos_Position_Flux->Sin;
    Cos_Rot_k1 = io->Sin_Cos_Position_Flux->Cos;

    // Calculate the flux magnitude
    cmd_flux_level = *(io->Ctrl_Specific->Rotor_Flux); // + ((io->Ctrl_Specific->Ldq_Ind->D * (*(io->Is_Abs)))>>15);
    *(io->Ctrl_Specific->Stator_Flux_Mag) = cmd_flux_level;
    Rotor_Lambda_Mod = cmd_flux_level;

    // Initialize the stator flux
    Stator_Lambda.Alpha  = Cos_Rot_k1 * cmd_flux_level;
    Stator_Lambda.Beta   = Sin_Rot_k1 * cmd_flux_level;

    // Estimated rotor flux is the same from stator
    Rotor_Lambda.Alpha = Stator_Lambda.Alpha;
    Rotor_Lambda.Beta = Stator_Lambda.Beta;


    Obs_Speed_Rotor_Flux_sum = 0.0f;

    Error_Flux_Alpha_1 = 0.0f;
    Error_Flux_Beta_1 = 0.0f;

    Err_Lim_Lower = 0.0f;
    Err_Lim_Upper = 0.0f;

#if (OBS__INVERTER_DISTORSION_COMPENSATION == ENABLED)
    Inv_Dist_Pointer.Is_ABC = io->Is_ABC;
    Inv_Dist_Pointer.Vdc = io->Vdc;
    Inv_Dist_Pointer.distortion = &Inverter_Distortion;
#endif

    Stator_Resistance_Corr = OBS__RESISTANCE_ADAPTATION_INIT_VALUE;
    Speed_Transition_Counter = 0;
    io->Ctrl_Specific->flags.bit.pll_engaged = 0;


}


//---------------------------------------------------------------------------------------------------------------------
/**
 *  @brief      Motor Control Loop initialization.
 *  @details    In this routine are called all initialization functions.
 *
 *
 *  @param[in]     
 *  @param[out]     
 *  @param[in]      
 *  @return        
 */
void MclObserver__Initialize(MCL_OBSERVER_IO_F_TYPE *io, MCLOBSERVER_JOINT_PARAMS_TYPE *params)
{
    // reset MCL quantities
    Inductance_Correction(io, params);  // to initialize Mcl_Quantities inductances
    *(io->Ctrl_Specific->Rotor_Flux) = params->ObsPrm->Rotor_Flux_Nominal;
	MclObserver__ResetState(io, params);
	Pole_Pairs_Inv = 1.0f/ params->ObsPrm->Pole_Pairs;
    // Init Pll parameters
    Speed_PLL.kp = params->ObsPrm->PLL_Kp;
    Speed_PLL.ki = params->ObsPrm->PLL_Ki;
    Speed_PLL.integ_k = params->ObsPrm->Pll_Integ_K;

}



//---------------------------------------------------------------------------------------------------------------------
/**
 *  @brief     Observer
 *  @details   Observer:  rotor flux speed estimation
 *                        flux position estimation (FOC: rotor flux, DTC: stator flux)
 *  @param[in]     
 *  @param[out]     
 *  @param[in]      
 *  @return        
 */
void MclObserver__RunningHandler(MCL_OBSERVER_IO_F_TYPE *io, MCLOBSERVER_JOINT_PARAMS_TYPE *params)
{
#if (OBS__VOLTAGE_PHASE_SHIFT == ENABLED)
    //==========================================================================//
    //                                                                          //
    //             Rotate the input voltage according to the speed              //
    //                                                                          //
    //==========================================================================//
    Voltage_Phase_Shift(io, params->ObsPrm);
#endif


#if (OBS__INVERTER_DISTORSION_COMPENSATION == ENABLED)
    //==========================================================================//
    //                                                                          //
    //                     Inverter distortion compensation                     //
    //                                                                          //
    //==========================================================================//
    InvDistortion_Comp(io, params->ObsPrm);
#endif


    //==========================================================================//
    //                                                                          //
    //                   Electromotive Force (EMF) calculation                  //
    //                                                                          //
    //==========================================================================//
    Emf_Calc(io);



    //==========================================================================//
    //                                                                          //
    //                        Stator Flux Observer                              //
    //                                                                          //
    //==========================================================================//
    StatorFlux_Calc(io, params->ObsPrm);



    //==========================================================================//
    //                                                                          //
    //                 Phase Locked Loop to calculate speed                     //
    //                                                                          //
    //==========================================================================//
    Speed_PLL_Calc();


    //==========================================================================//
    //                                                                          //
    //    Algorithm to select which speed shall be used, e.g., from flux PLL    //
    //               rotor flux frame or from stator flux frame.                //
    //                                                                          //
    //==========================================================================//
    Speed_Selector(io, params->ObsPrm);


    //==========================================================================//
    //                                                                          //
    //                    Electromagnetic Torque calculation                    //
    //                                                                          //
    //==========================================================================//
    ElectroMagneticTorqueCalc(io, params->ObsPrm);

}




void MclObserver__1msHandler(MCL_OBSERVER_IO_F_TYPE *io, MCLOBSERVER_JOINT_PARAMS_TYPE *params)
{

    //==========================================================================//
    //                                                                          //
    //                         Inductance calculation                           //
    //                                                                          //
    //==========================================================================//
    Inductance_Correction(io, params);

    //==========================================================================//
    //                                                                          //
    //                         Rotor Flux calculation                           //
    //                                                                          //
    //==========================================================================//
    if (params->Rotor_Flux_lut.ptr_LUT != NULL)
    {
        RotorFlux_Correction(io,params);
    }

    //==========================================================================//
    //                                                                          //
    //                           Resistance adaptation                          //
    //                                                                          //
    //==========================================================================//
    Resistance_Adaptation(io, params->ObsPrm);

    Speed_Selector_Check(io, params->ObsPrm);



}



#if (OBS__VOLTAGE_PHASE_SHIFT == ENABLED)
//========================================================//
//          Phase Shift on reconstructed voltage          //
//========================================================//
void Voltage_Phase_Shift(MCL_OBSERVER_IO_F_TYPE *io, MCL_OBSERVER_PARAMS_TYPE *params)
{
    ALPHA_BETA_COOR_SYST_F_TYPE vs_alpha_beta;
    DQ_COOR_SYST_F_TYPE vs_dq;
    float32 angle_advance;
    SIN_COS_F_TYPE sin_cos_adv;
    ALPHA_BETA_COOR_SYST_F_TYPE tmp_dq_adv;


    vs_alpha_beta.Alpha = io->Vs_Alpha_Beta->Alpha;
    vs_alpha_beta.Beta  = io->Vs_Alpha_Beta->Beta;

    // from vs alpha\beta to vs in dq stator flux reference frame
    ClrkPark__DirectParkF(&vs_alpha_beta,&vs_dq,io->Sin_Cos_Position_Flux);

    angle_advance = ( *(io->Ctrl_Specific->Speed_Rotor_Observed) * params->Phase_Advance_Coeff );

    MathCalc__SinCosF(angle_advance,  &sin_cos_adv);

    ClrkPark__InverseParkF(&vs_dq, &tmp_dq_adv,  &sin_cos_adv);

    vs_dq.D = tmp_dq_adv.Alpha;
    vs_dq.Q = tmp_dq_adv.Beta;

    //==========================================================================//
    //                Inverse Park transformation (dq-alpha beta)               //
    //==========================================================================//
    ClrkPark__InverseParkF(&vs_dq, io->Vs_Alpha_Beta, io->Sin_Cos_Position_Flux);

}
#endif




#if (OBS__INVERTER_DISTORSION_COMPENSATION == ENABLED)
//==========================================================================//
//                        Deatime Compensation                              //
//==========================================================================//
void InvDistortion_Comp(MCL_OBSERVER_IO_F_TYPE *io, MCL_OBSERVER_PARAMS_TYPE *params)
{
    InverterComp__Calculate(&Inv_Dist_Pointer);
}
#endif


void Resistance_Adaptation(MCL_OBSERVER_IO_F_TYPE *io, MCL_OBSERVER_PARAMS_TYPE *params)
{
    float32 temp;
    float32 lim_lower;

    /*! Resistance adaptation is calculated as:
     * \f[
     * \begin{aligned}
     *      & R_s = R_s + R_{correction} \\
     *      & R_{correction} = R_{correction} + R_{gain} * \lambda_{error} \\
     *      & \lambda_{error} = \lambda_{m} - \hat{\lambda_{m}}
     * \end{aligned}
     * \f]
     */

    if((*(io->Ctrl_Specific->Speed_Rotor_Observed_Mech_Abs) <= (params->Curr_Resistance_Spd_Threshold)) || ((io->Ctrl_Specific->flags.bit.wrong_speed_direction)==1))
    {
        if( *(io->Ctrl_Specific->Is_Abs) > (params->Curr_Resistance_Threshold) )
        {
            temp = Stator_Resistance_Corr + Flux_Error * (params->Stator_Resistance_Gain);
            lim_lower = -params->Stator_Resistance_Corr_Limits*(*(io->Ctrl_Specific->Stator_Resistance));
            Stator_Resistance_Corr = MATHCALC__SATURATE(lim_lower, temp, 0);
        }
    }
    //else if((*(io->Ctrl_Specific->Speed_Rotor_Observed_Mech_Abs) > Curr_Resistance_Spd_Threshold_High) && ((io->Ctrl_Specific->flags.bit.wrong_speed_direction)==0))
    else if(io->Ctrl_Specific->flags.bit.wrong_speed_direction==0)
    {
        temp = Stator_Resistance_Corr + OBS__RESISTANCE_ADAPTATION_RECOVERY;
        if (temp>0)
        {
            temp = 0;
            if (*(io->Ctrl_Specific->Speed_Rotor_Observed_Mech_Abs) > params->Observer_G_integ_Spd_Threshold)
            {
                Err_Lim_Lower = -(params->Observer_G_integ_limits);
                Err_Lim_Upper = params->Observer_G_integ_limits;
            }
        }
        Stator_Resistance_Corr = temp;
    }

    if (Stator_Resistance_Corr < 0)
    {
        Err_Lim_Lower = 0.0f;
        Err_Lim_Upper = 0.0f;
    }
}

#ifdef __IAR_SYSTEMS_ICC__
#pragma inline = forced
#endif
void Emf_Calc(MCL_OBSERVER_IO_F_TYPE *io)
{
    /*! Emf calculation:
     * \f[
     * \begin{aligned}
     *      & Emf_{alpha} = v_{alpha} - R_{s} * i_{alpha} - vdt_{alpha} \\
     *      & Emf_{beta} =  v_{beta}  - R_{s} * i_{beta}  - vdt_{beta} \\
     * \end{aligned}
     * \f]
     */

    // EMF Alpha calculation
    Stator_Emf.Alpha  = (io->Vs_Alpha_Beta->Alpha) - (*(io->Ctrl_Specific->Stator_Resistance) * (io->Is_Alpha_Beta->Alpha));

    // EMF Beta calculation
    Stator_Emf.Beta   = (io->Vs_Alpha_Beta->Beta)  - (*(io->Ctrl_Specific->Stator_Resistance) * (io->Is_Alpha_Beta->Beta ));

#if (OBS__INVERTER_DISTORSION_COMPENSATION == ENABLED)
    Stator_Emf.Alpha -= Inverter_Distortion.Alpha;
    Stator_Emf.Beta -= Inverter_Distortion.Beta;
#endif

    Stator_Emf.Alpha -= Stator_Resistance_Corr * (io->Is_Alpha_Beta->Alpha);
    Stator_Emf.Beta  -= Stator_Resistance_Corr * (io->Is_Alpha_Beta->Beta );

}




void Inductance_Correction(MCL_OBSERVER_IO_F_TYPE *io, MCLOBSERVER_JOINT_PARAMS_TYPE *params)
{
    float32 temp;

    temp              = Rotor_Curr_Id_Abs;
    (io->Ctrl_Specific->Ldq_Ind->D)    = MathCalc__GetInterpolationFastF(temp, &params->Ld_lut);

    temp              = Rotor_Curr_Iq_Abs;
    (io->Ctrl_Specific->Ldq_Ind->Q)    = MathCalc__GetInterpolationFastF(temp, &params->Lq_lut);

    // Capturing the Ldq term from the table
    temp = Rotor_Curr_Iq_Abs;
    *(io->Ctrl_Specific->Ldq_Cross_Ind) = MathCalc__GetInterpolationFastF(temp, &params->Ldq_lut);

    // Capturing the Lqd term from the table
    temp = Rotor_Curr_Id_Abs;
    *(io->Ctrl_Specific->Lqd_Cross_Ind) = MathCalc__GetInterpolationFastF(temp, &params->Ldq_lut);

}


void RotorFlux_Correction(MCL_OBSERVER_IO_F_TYPE *io, MCLOBSERVER_JOINT_PARAMS_TYPE *params)
{
    float32 temp;

    temp = *(io->Ctrl_Specific->Is_Abs);
    *(io->Ctrl_Specific->Rotor_Flux) = MathCalc__GetInterpolationFastF(temp, &params->Rotor_Flux_lut);
}


//---------------------------------------------------------------------------------------------------------------------
/**
 *  @brief      Stator flux observer (Generic) which supports salient motors, it contains more updates from Prof. Radu.
 *
 *  @param      motor phase voltages and phase currents => which will result in stator flux, rotor flux and speed computations.
 *  @return     none.
 */
#ifdef __IAR_SYSTEMS_ICC__
#pragma inline = forced
#endif
void StatorFlux_Calc(MCL_OBSERVER_IO_F_TYPE *io, MCL_OBSERVER_PARAMS_TYPE *params)
{
    float32 temp_1;
    float32 temp_3;
    float32 temp_4;
    float32 dLambdas_alpha;
    float32 dLambdas_beta;
    MATHCALC_MOD_SIN_COS_F_TYPE trig_alpha_beta_mod;
    float32 lambda_mag_rot;

    float32 sin_rot;
    float32 cos_rot;

    float32 Lambda_d;
    float32 Lambda_q;
    float32 Lambdas_alpha_fback;
    float32 Lambdas_beta_fback;
    float32 error_flux_alpha;
    float32 error_flux_beta;

    float32 stator_flux_d_forward;

    float32 load_angle_cos;
    float32 load_angle_sin;

#if (OBS__ROTOR_MECH_POSITION  == ENABLED)  // not used
    float32 rotor_pos_delta;
#endif


    //******************************************************************//
    //                                                                  //
    //              Integrate the emf to find the stator flux           //
    //                                                                  //
    //******************************************************************//

    //// Alpha flux - trapezoidal integration
    dLambdas_alpha  = Stator_Emf.Alpha + Stator_Emf_Err.Alpha;
    Stator_Lambda.Alpha += (dLambdas_alpha + dLambdas_alpha_old) * PARAMS_EMF_INT_CONSTANT;
    dLambdas_alpha_old = dLambdas_alpha;



    //// Beta flux - trapezoidal integration
    dLambdas_beta  = Stator_Emf.Beta + Stator_Emf_Err.Beta;
    Stator_Lambda.Beta += (dLambdas_beta + dLambdas_beta_old) * PARAMS_EMF_INT_CONSTANT;
    dLambdas_beta_old = dLambdas_beta;



    //******************************************************************//
    //                                                                  //
    //                     Estimate rotor flux                          //
    //                                                                  //
    //******************************************************************//
    // Alpha axis rotor flux
    Rotor_Lambda.Alpha = Stator_Lambda.Alpha - (io->Ctrl_Specific->Ldq_Ind->Q) * io->Is_Alpha_Beta->Alpha;
    // Beta axis rotor flux
    Rotor_Lambda.Beta  = Stator_Lambda.Beta  - (io->Ctrl_Specific->Ldq_Ind->Q) * io->Is_Alpha_Beta->Beta;

    // Magnitude (modulus) rotor flux
    temp_1 =  Rotor_Lambda.Alpha*Rotor_Lambda.Alpha + Rotor_Lambda.Beta*Rotor_Lambda.Beta;
    Rotor_Lambda_Mod = MC_SQRT_F(temp_1);  // Result in Q17.15




    //******************************************************************//
    //                                                                  //
    //                   ROTOR Sin and Cos calculations                 //
    //                                                                  //
    //******************************************************************//
    trig_alpha_beta_mod.Alpha = Rotor_Lambda.Alpha;
    trig_alpha_beta_mod.Beta  = Rotor_Lambda.Beta;
    trig_alpha_beta_mod.Mod = Rotor_Lambda_Mod;
    MathCalc__GetSinCosF(&trig_alpha_beta_mod);
    sin_rot = trig_alpha_beta_mod.Sin;
    cos_rot = trig_alpha_beta_mod.Cos;





    //******************************************************************//
    //                                                                  //
    //             Id & Iq calculation => Transformation to             //
    //                     the ROTOR reference frame                    //
    //                                                                  //
    //******************************************************************//
    Rotor_Curr_Id = ((io->Is_Alpha_Beta->Alpha) * cos_rot) + ((io->Is_Alpha_Beta->Beta) * sin_rot);
    Rotor_Curr_Id_Abs = MATHCALC__FABS(Rotor_Curr_Id);
    *io->Ctrl_Specific->Id_Rotor_Current = Rotor_Curr_Id;


    Rotor_Curr_Iq = ((-(io->Is_Alpha_Beta->Alpha)) * sin_rot) + ((io->Is_Alpha_Beta->Beta) * cos_rot);
    Rotor_Curr_Iq_Abs = MATHCALC__FABS(Rotor_Curr_Iq);

    // Compute stator flux components in (d,q) frame from magnetic model
    Lambda_d  = *(io->Ctrl_Specific->Rotor_Flux) + Rotor_Curr_Id * (io->Ctrl_Specific->Ldq_Ind->D);
    Lambda_q  = Rotor_Curr_Iq * (io->Ctrl_Specific->Ldq_Ind->Q);

    Lambda_d  = Lambda_d - (Rotor_Curr_Iq_Abs * (*io->Ctrl_Specific->Ldq_Cross_Ind));
    Lambda_q  = Lambda_q - (Rotor_Curr_Id * (*io->Ctrl_Specific->Lqd_Cross_Ind));



    //******************************************************************//
    //                                                                  //
    //             Transform fluxes back to stationary frame            //
    //                                                                  //
    //******************************************************************//
    Lambdas_alpha_fback  = Lambda_d * cos_rot - Lambda_q * sin_rot;
    Lambdas_beta_fback   = Lambda_d * sin_rot + Lambda_q * cos_rot;

    // Q30 = Q30 + Q30
    error_flux_alpha = Lambdas_alpha_fback - Stator_Lambda.Alpha;

    error_flux_beta  = Lambdas_beta_fback  - Stator_Lambda.Beta;

    temp_1 = Error_Flux_Alpha_1 + error_flux_alpha * params->Observer_G_integ_gain;

    // limit the integral
    Error_Flux_Alpha_1 = MATHCALC__SATURATE(Err_Lim_Lower, temp_1, Err_Lim_Upper);

    Stator_Emf_Err.Alpha = error_flux_alpha * (params->Observer_G_gain);

    Stator_Emf_Err.Alpha += Error_Flux_Alpha_1;

    temp_1 = Error_Flux_Beta_1 + error_flux_beta *  params->Observer_G_integ_gain;

    // limit the integral
    Error_Flux_Beta_1 = MATHCALC__SATURATE(Err_Lim_Lower, temp_1, Err_Lim_Upper);


    Stator_Emf_Err.Beta  = error_flux_beta  * (params->Observer_G_gain);
    Stator_Emf_Err.Beta += Error_Flux_Beta_1;


    //******************************************************************//
    //                                                                  //
    //                 Stator flux magnitude and speed                  //
    //                                                                  //
    //******************************************************************//
    // Square the stator flux and beta, sum and take the square root.
    temp_3 = Stator_Lambda.Alpha * Stator_Lambda.Alpha;
    temp_4 = Stator_Lambda.Beta  * Stator_Lambda.Beta;
	temp_3 = temp_3 + temp_4;

	if (io->Ctrl_Specific->flags.bit.overflux_active == 1)
    {
	    lambda_mag_rot = MC_SQRT_F(Lambda_d * Lambda_d + Lambda_q * Lambda_q);
        *(io->Ctrl_Specific->Stator_Flux_Mag) = lambda_mag_rot;
    }
    else
    {
        *(io->Ctrl_Specific->Stator_Flux_Mag) = MC_SQRT_F(temp_3);
    }


    //******************************************************************//
    //                                                                  //
    //     Calculate speed from rotor frame and low pass filter it      //
    //                                                                  //
    //******************************************************************//
    temp_1 = (sin_rot * Cos_Rot_k1) - (Sin_Rot_k1 * cos_rot);
    Obs_Speed_Rotor_Flux = temp_1 * (1.0f/TS);

    //******************************************************************//
    //                                                                  //
    //                   STATOR Sin and Cos calculations                //
    //                                                                  //
    //******************************************************************//
    if (io->Ctrl_Specific->flags.bit.overflux_active == 1)
    {
        trig_alpha_beta_mod.Alpha = Lambdas_alpha_fback;
        trig_alpha_beta_mod.Beta  = Lambdas_beta_fback;
    }
    else
    {
        trig_alpha_beta_mod.Alpha = Stator_Lambda.Alpha;
        trig_alpha_beta_mod.Beta  = Stator_Lambda.Beta;
    }

    trig_alpha_beta_mod.Mod   = *(io->Ctrl_Specific->Stator_Flux_Mag);
    MathCalc__GetSinCosF(&trig_alpha_beta_mod);
    io->Sin_Cos_Position_Flux->Sin = trig_alpha_beta_mod.Sin;
    io->Sin_Cos_Position_Flux->Cos = trig_alpha_beta_mod.Cos;

    *(io->Ctrl_Specific->Id_stator_current) = (io->Is_Alpha_Beta->Alpha) * (io->Sin_Cos_Position_Flux->Cos) + (io->Is_Alpha_Beta->Beta) * (io->Sin_Cos_Position_Flux->Sin);

    *(io->Ctrl_Specific->Iq_stator_current) = -(io->Is_Alpha_Beta->Alpha) * (io->Sin_Cos_Position_Flux->Sin) + (io->Is_Alpha_Beta->Beta) * (io->Sin_Cos_Position_Flux->Cos);


#if (OBS__ROTOR_MECH_POSITION  == ENABLED)  // not used
    //******************************************************************//
    //                                                                  //
    //        Calculate accurately the mechanical rotor position        //
    //                                                                  //
    //******************************************************************//
    Rotor_Position = MC_ATAN2_F(cos_rot, sin_rot);
   // rotor_pos_delta = Rotor_Position - Last_Mech_Rotor_Pos;
   // Rotor_Mech_Position += (rotor_pos_delta + Mech_Rotor_Pos_Acc_Err) / (params->Pole_Pairs);
   // Mech_Rotor_Pos_Acc_Err = (rotor_pos_delta + Mech_Rotor_Pos_Acc_Err) % (params->Pole_Pairs);
   // Last_Mech_Rotor_Pos = Rotor_Position;
    *(io->Rotor_Position) = Rotor_Position;

#endif

    //******************************************************************//
    //                                                                  //
    //       Compute load angle using the same (d,q) stator flux        //
    //           components, but computed from (alpha,beta)             //
    //                    forward path components                       //
    //                                                                  //
    //******************************************************************//
    stator_flux_d_forward  = Stator_Lambda.Alpha * cos_rot + Stator_Lambda.Beta  * sin_rot;

    load_angle_cos = io->Sin_Cos_Position_Flux->Cos * cos_rot + io->Sin_Cos_Position_Flux->Sin *sin_rot;

    load_angle_sin = io->Sin_Cos_Position_Flux->Sin * cos_rot - io->Sin_Cos_Position_Flux->Cos *sin_rot;

    io->Ctrl_Specific->Load_Angle_Sin_Cos->Cos = load_angle_cos;
    io->Ctrl_Specific->Load_Angle_Sin_Cos->Sin = load_angle_sin;
#if(OBS__LOAD_ANGLE_CALCULATION == ENABLED)  // avoid performing load angle calculation if not needed
        *(io->Load_Angle) = MC_ATAN2_F((load_angle_cos), (load_angle_sin));
#endif



    //******************************************************************//
    //                                                                  //
    //              Compute error for stability improvement             //
    //                                                                  //
    //******************************************************************//
    //This equation helps on steady state stability at low load (rs correction works fine)
    //However the term LdObs*curr_id introduced a big error_flux at starting which may cause failures
    Flux_Error  = stator_flux_d_forward - Lambda_d;


    // Save the previous rotor sine and cosine
    Sin_Rot_k1 = sin_rot;
    Cos_Rot_k1 = cos_rot;
}





#ifdef __IAR_SYSTEMS_ICC__
#pragma inline = forced
#endif
/*==========================================================================*/
/*                         Speed Calculation                                */
/*==========================================================================*/
void Speed_PLL_Calc()
{
    Speed_PLL.in_alpha = Cos_Rot_k1;
    Speed_PLL.in_beta  = Sin_Rot_k1;

    PLL__fCalc(&Speed_PLL);
}





#ifdef __IAR_SYSTEMS_ICC__
#pragma inline = forced
#endif
void PLL__fReset(XPLL_TYPE *XPLLVars)
{
    XPLLVars->speed = 0.0f;
    XPLLVars->speed_old = 0.0f;
    XPLLVars->error = 0.0f;
    XPLLVars->error_old = 0.0f;
    XPLLVars->integral = 0.0f;
    XPLLVars->angle = 0.0f;

    XPLLVars->alpha = 0.0f;
    XPLLVars->beta = 0.0f;
    XPLLVars->in_alpha = 0.0f;
    XPLLVars->in_beta = 0.0f;
}




#ifdef __IAR_SYSTEMS_ICC__
#pragma inline = forced
#endif
void PLL__fCalc(XPLL_TYPE *XPLLVars)
{
    SIN_COS_F_TYPE sin_cos;

    MathCalc__SinCosF(XPLLVars->angle, &sin_cos);

    XPLLVars->alpha =  sin_cos.Cos;
    XPLLVars->beta  = -sin_cos.Sin;

    XPLLVars->error =  (XPLLVars->in_alpha * XPLLVars->beta) + (XPLLVars->in_beta * XPLLVars->alpha);

    XPLLVars->integral += (XPLLVars->error + XPLLVars->error_old) * (XPLLVars->ki);
    XPLLVars->error_old = XPLLVars->error;
    XPLLVars->speed = XPLLVars->integral + XPLLVars->error * XPLLVars->kp;
    XPLLVars->angle += (XPLLVars->speed + XPLLVars->speed_old) * XPLLVars->integ_k;
    XPLLVars->speed_old = XPLLVars->speed;

#if(OBS__PLL_FILTERED_OMEGA == ENABLED)
	// use integral + fraction of proportional as filtered speed
#ifdef OBS__PLL_FILTERED_OMEGA_COEF
    XPLLVars->speed = XPLLVars->integral + OBS__PLL_FILTERED_OMEGA_COEF*XPLLVars->error * XPLLVars->kp; //add this line to reduce speed ripple;
#else
    XPLLVars->speed = XPLLVars->integral;
#endif
#endif

    if(XPLLVars->angle > PI)
    {
        XPLLVars->angle -= (2.0f * PI);
    }

    if(XPLLVars->angle < (-PI))
    {
        XPLLVars->angle += (2.0f * PI);
    }
}



#ifdef __IAR_SYSTEMS_ICC__
#pragma inline = forced
#endif
void Speed_Selector(MCL_OBSERVER_IO_F_TYPE *io, MCL_OBSERVER_PARAMS_TYPE *params)
{
    float32 temp_speed;

    //==========================================================================//
    //                       Low pass filter for speed                          //
    //==========================================================================//
    Obs_Speed_Rotor_Flux_sum = FILTERS__LOWPASSFILTER_F(Obs_Speed_Rotor_Flux_sum,OBS_SPEED_LPF_COEF,Obs_Speed_Rotor_Flux);




    if(io->Ctrl_Specific->flags.bit.pll_engaged == 1)
    {
        // Read speed from the flux PLL
        temp_speed = Speed_PLL.speed;
    }
    else
    {
        temp_speed = Obs_Speed_Rotor_Flux_sum;
    }


    // Update the speed variables
    *(io->Speed_Rotor_Observed) = temp_speed;
    *(io->Ctrl_Specific->Speed_Rotor_Observed_Mech) = temp_speed * Pole_Pairs_Inv;
    *(io->Ctrl_Specific->Speed_Rotor_Observed_Mech_Abs) = MATHCALC__FABS(*(io->Ctrl_Specific->Speed_Rotor_Observed_Mech));
}


#ifdef __IAR_SYSTEMS_ICC__
#pragma inline = forced
#endif
void Speed_Selector_Check(MCL_OBSERVER_IO_F_TYPE *io, MCL_OBSERVER_PARAMS_TYPE *params)
{
    BOOL_TYPE speed_cond_1;
    BOOL_TYPE speed_cond_2;
    BOOL_TYPE speed_cond_3;

    float32 thr_speed;

    thr_speed = params->Curr_Resistance_Spd_Threshold * params->Pole_Pairs;

    speed_cond_1 = (BOOL_TYPE) (MATHCALC__FABS(Obs_Speed_Rotor_Flux_sum) > thr_speed);
    speed_cond_2 = (BOOL_TYPE) (MATHCALC__FABS(Obs_Speed_Rotor_Flux_sum-Speed_PLL.speed) < OBS__PLL_ENGAGE_ERROR_THR);
    speed_cond_3 = (BOOL_TYPE) (io->Ctrl_Specific->flags.bit.wrong_speed_direction == 0);

    // condition for speed commutation to pll
    if (io->Ctrl_Specific->flags.bit.pll_engaged == 0)
    {
        if (speed_cond_1  && speed_cond_2 && speed_cond_3)
        {
            Speed_Transition_Counter++;
        }
        else
        {
            Speed_Transition_Counter = 0;
        }
        if( Speed_Transition_Counter > (params->Starting_Threshold))
        {
            io->Ctrl_Specific->flags.bit.pll_engaged = 1;
            io->Ctrl_Specific->flags.bit.wait_pll_engagement = 0;
        }
    }
    else
    {
        if (speed_cond_1 == FALSE)
        {
            io->Ctrl_Specific->flags.bit.pll_engaged = 0;
            io->Ctrl_Specific->flags.bit.wait_pll_engagement = 1;
        }
    }


    if( *(io->Ctrl_Specific->Lowers_On) )
    {
        // Rotor speed is being used when applying lowers on because since PLL gets lost.
        io->Ctrl_Specific->flags.bit.pll_engaged = 0;
    }

}


//==========================================================================//
//                       Torque estimator                                   //
/*! Torque estimation is given by:
 * \f[
 * \begin{aligned}
 *      & \hat{T}_e = \hat{\lambda}_{s_{alpha}} * i_{beta} - \hat{\lambda}_{s_{beta}} * i_{alpha} \\
 * \end{aligned}
 * \f]
 */
//==========================================================================//
//uint8 EM_Torque_By_Flux = 1;
#ifdef __IAR_SYSTEMS_ICC__
#pragma inline = forced
#endif
void ElectroMagneticTorqueCalc(MCL_OBSERVER_IO_F_TYPE *io, MCL_OBSERVER_PARAMS_TYPE *params)
{
    float32 tmp_1;
    float32 tmp_2;

    if(io->Ctrl_Specific->flags.bit.overflux_active == 0)
    {
    	EM_Torque = Stator_Lambda.Alpha * io->Is_Alpha_Beta->Beta - Stator_Lambda.Beta * io->Is_Alpha_Beta->Alpha;
    }
    else
    {
		tmp_1  = *(io->Ctrl_Specific->Rotor_Flux) * Rotor_Curr_Iq;
		tmp_2 = (Rotor_Curr_Iq*Rotor_Curr_Id);
		EM_Torque = tmp_1 + ((io->Ctrl_Specific->Ldq_Ind->D - io->Ctrl_Specific->Ldq_Ind->Q)*tmp_2);
    }

    // Low pass filter the electromagnetic torque
    EM_Torque = 1.5f * params->Pole_Pairs * EM_Torque;

    EM_Torque_Filt_Sum = FILTERS__LOWPASSFILTER_F(EM_Torque_Filt_Sum, EM_TORQUE_LPF_COEF, EM_Torque);
    *(io->Ctrl_Specific->Torque) = EM_Torque_Filt_Sum;
}
