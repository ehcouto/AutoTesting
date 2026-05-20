/**
 *  @file
 *  @brief       Motor Control Loop Speed Controller .
 *  @details     This module implements the Speed Control module.
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
#include "MclDqRef.h"
#include "MclConfig.h"
#include "McMathCalc_macros.h"
#include "MathCalc.h"
#include "Braking.h"


//-------------------------------------- PUBLIC (Variables) -----------------------------------------------------------

#ifndef SQRT3_INV
    #define SQRT3_INV    (float32) (1.0f / 1.7320508075688772935274463415059f)
#endif

#ifndef DQREF__FLUX_ADAPTATION
    #define DQREF__FLUX_ADAPTATION  DISABLED
#endif

#define OVER_FLUX_ACTIVE_THR   1.0f

//-------------------------------------- PRIVATE (Variables, Constants & Defines) -------------------------------------

float32 Over_Flux_Ratio;
float32 Id_Braking;
float32 Flux_Min;

#if (DQREF__FLUX_ADAPTATION==ENABLED)
static float32 Flux_M_Braking;
#endif

//-------------------------------------- PRIVATE (Function Prototypes) ------------------------------------------------



//=====================================================================================================================
//-------------------------------------- Public Functions -------------------------------------------------------------
//=====================================================================================================================

float32 Calculate_Flux_MTPA(MCL_DQ_REF_IO_F_TYPE *io, MCLDQREF_JOINT_PARAMS_TYPE *params);
float32 Calculate_Flux_MTPV(MCL_DQ_REF_IO_F_TYPE *io, MCL_DQ_REF_PARAMS_TYPE *params);




//---------------------------------------------------------------------------------------------------------------------
/**
 *  @brief      Reset all Mcl Dq Reference Generator quantities.
 *  @details    This routine reset all q Reference Generator quantities, it has to be called at every
 *              time the pwm is switched off (motor stop or free down ramp).
 *
 *  @param[in]     
 *  @param[out]
 *  @return        
 */
void MclDqRef__ResetState(MCLDQREF_JOINT_PARAMS_TYPE* params)
{
    Over_Flux_Ratio 	= 1.0f;
    Flux_Min 			= Over_Flux_Ratio*params->DqRefPrm->Rotor_Flux_Nominal;
    Id_Braking        	= 0.0f;
#if (DQREF__FLUX_ADAPTATION==ENABLED)
    Flux_M_Braking      = params->DqRefPrm->Rotor_Flux_Nominal;
#endif
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
float32 My_Flux_Gain;
void MclDqRef__Initialize(void)
{
	My_Flux_Gain = 1.0f;
}





//---------------------------------------------------------------------------------------------------------------------
/**
 *  @brief     Dq Reference Generation
 *  @details   Dq Reference Gen: Mtpa
 *                               Flux Weaking
 *                               d-reference generation (FOC: current Id, DTC: flux reference)
 *                               q-reference generation (FOC: current Id, DTC: torque reference)
 *                               torque reference generation
 *  @param[in]     
 *  @param[out]     
 *  @param[in]      
 *  @return        
 */
uint8 My_WbLim_Flag;
uint8 My_Mflux_Flag;
void MclDqRef__RunningHandler(MCL_DQ_REF_IO_F_TYPE *io, MCLDQREF_JOINT_PARAMS_TYPE *params)
{
    float32 flux_ref_mtpa;
    float32 flux_limit_by_speed;
    float32 temp_f;

    *(io->Q_Ref) = *(io->Torque_Ref);

    //===========================================================================//
    //                   MTPV stator flux reference calculation                  //
    //           Calculate the flux limit for field weakening operation          //
    //===========================================================================//
    flux_limit_by_speed = Calculate_Flux_MTPV(io, params->DqRefPrm);
    *(io->Ctrl_Specific->Flux_Limit_By_Speed) = flux_limit_by_speed;


    //===========================================================================//
    //                 MTPA - Maximum Torque Per Ampere calculation              //
    //===========================================================================//

    // MTPA BASED ON LUT
    flux_ref_mtpa = Calculate_Flux_MTPA(io,params);

    //flux_ref_mtpa = My_Flux_Gain * flux_ref_mtpa;

    if(flux_ref_mtpa > flux_limit_by_speed)
    {
    	My_WbLim_Flag = 1;
    }
    else
    {
    	My_WbLim_Flag = 0;
    }


    // Select the appropriate flux reference
    temp_f = MATHCALC__SATURATE(Flux_Min,  flux_ref_mtpa, flux_limit_by_speed);

    *(io->D_Ref) = temp_f*My_Flux_Gain;

    if(*(io->D_Ref) > params->DqRefPrm->Rotor_Flux_Nominal)
    {
    	My_Mflux_Flag = 0;
    }
    else
    {
    	My_Mflux_Flag = 1;
    }


    // Overflux strategy (low speed & low torque)
	if (*(io->Ctrl_Specific->Speed_Rotor_Observed_Mech_Abs) < params->DqRefPrm->Over_Flux_Speed_Threshold)
	{
		Over_Flux_Ratio += DQREF__OVERFLUX_CHARGING;
		Over_Flux_Ratio = MATHCALC__SATURATE(1.0f, Over_Flux_Ratio,params->DqRefPrm->Over_Flux_Ratio_Max);
	}

    if((Over_Flux_Ratio>DQREF__MIN_FLUX)&&(Id_Braking == 0))
    {
        Over_Flux_Ratio -= params->DqRefPrm->Over_Flux_Decrement;      //Decrease the starting flux factor towards zero in a rate of 125us.
    }
    else
    {
    	Over_Flux_Ratio = DQREF__MIN_FLUX;
    }

    Flux_Min = Over_Flux_Ratio*params->DqRefPrm->Rotor_Flux_Nominal;

}

void MclDqRef__1msHandler(MCL_DQ_REF_IO_F_TYPE *io, MCLDQREF_JOINT_PARAMS_TYPE *params)
{
    if( *(io->Ctrl_Specific->Elapsed_Time_After_Starting) > (params->DqRefPrm->Starting_Threshold) )
    {
        Id_Braking = Braking__DqRefHandler(io, params->DqRefPrm);
    }

    // manage overflux flag
    if (Over_Flux_Ratio>OVER_FLUX_ACTIVE_THR)
    {
        io->Ctrl_Specific->flags.bit.overflux_active = 1;
    }
    else
    {
        io->Ctrl_Specific->flags.bit.overflux_active = 0;
    }
}



#ifdef __IAR_SYSTEMS_ICC__
#pragma inline = forced
#endif
float32 Calculate_Flux_MTPV(MCL_DQ_REF_IO_F_TYPE *io, MCL_DQ_REF_PARAMS_TYPE *params)
{
    float32 flux_limit_by_speed;
    float32 speed_abs_rotor;
    float32 vs_max;
    float32 temp_1_s32;
    float32 temp_2_s32;
    float32 temp_3_s32;
    float32 torque_error_factor;
    float32 iq_stator_current_abs;

    iq_stator_current_abs = MATHCALC__FABS(*(io->Ctrl_Specific->Iq_stator_current));
    speed_abs_rotor = MATHCALC__FABS(*(io->Ctrl_Specific->Speed_Rotor_Observed));

//    MATHCALC__FILTER_CALC(speed_abs_rotor, speed_abs_rotor_sum, FILTER_SHIFT_FS_1KHZ_FC_1HZ);
//    speed_abs_rotor = MATHCALC__FILTER_GET_FILTERED_VAR(speed_abs_rotor_sum, FILTER_SHIFT_FS_1KHZ_FC_1HZ);

    speed_abs_rotor = speed_abs_rotor + 0.1f;                              // Prevent zero division

    vs_max = (*(io->Vdc) * SQRT3_INV) * params->m_index;

    // Complete field weakening equation

    // (Rs * Id)
    temp_1_s32 = (*(io->Ctrl_Specific->Stator_Resistance) * *(io->Ctrl_Specific->Id_stator_current));

    // (Rs * Id)^2
    temp_1_s32 = (temp_1_s32 * temp_1_s32);

    // Vs_max^2
    temp_2_s32 = vs_max * vs_max;

    // sqrt(Vs_max^2 - Rs*Id^2)
    temp_1_s32 = MC_SQRT_F(temp_2_s32 - temp_1_s32);

    // Rs * Iq
    temp_2_s32 = *(io->Ctrl_Specific->Stator_Resistance) * iq_stator_current_abs;

    // sqrt(Vs_max^2 - Rs*Id^2) - Rs*Iq
    temp_1_s32 = temp_1_s32 - temp_2_s32;
    *(io->Ctrl_Specific->Vs_Flux) = (temp_1_s32);

    // (sqrt(Vs_max^2 - Rs*Id^2) - Rs*Iq)/abs(speed_r)
    flux_limit_by_speed = temp_1_s32/speed_abs_rotor;


    // Force at maximum flux at starting (prevent glitches on max flux)
    if( *(io->Ctrl_Specific->Elapsed_Time_After_Starting) < (params->Starting_Threshold) )
    {
        flux_limit_by_speed = params->Max_Flux;
    }
    else
    {

        temp_3_s32 = *(io->Q_Ref) - *(io->Ctrl_Specific->Torque);
        torque_error_factor = (temp_3_s32 * params->Torque_Err_K);
        torque_error_factor = MATHCALC__SATURATE_DIRECT(0, torque_error_factor, params->Torque_Err_Sat);

        flux_limit_by_speed -= flux_limit_by_speed *  torque_error_factor;

        // Limit the maximum flux
        flux_limit_by_speed = MATHCALC__SATURATE_DIRECT(0, flux_limit_by_speed, (params->Max_Flux));
    }
    return flux_limit_by_speed;
}





#ifdef __IAR_SYSTEMS_ICC__
#pragma inline = forced
#endif
float32 Calculate_Flux_MTPA(MCL_DQ_REF_IO_F_TYPE *io, MCLDQREF_JOINT_PARAMS_TYPE *params)
{

    //Id injection equations: general formulation
    float32 iq_braking;
    float32 ld;
    float32 lq;
    float32 flux_m;
    float32 flux_d;
    float32 flux_q;
    float32 flux_ref_mtpa;
    float32 total_squared_flux;
    float32 temp_1;


    // understand if there is a braking request (check for Id braking)
    if (Id_Braking == 0)
    {// motoring mode --> MTPA Trajectory
        temp_1 =  MATHCALC__FABS(*(io->Torque_Ref));
        flux_ref_mtpa = MathCalc__GetInterpolationFastF(temp_1, &params->MTPAPrm);
#if (DQREF__FLUX_ADAPTATION==ENABLED)
        Flux_M_Braking = params->DqRefPrm->Rotor_Flux_Nominal;      // reset to nominal value when not braking
#endif
    }
    else
    {// braking mode --> non-regenerative trajectory
        ld = io->Ctrl_Specific->Ldq_Ind->D;
        lq = io->Ctrl_Specific->Ldq_Ind->Q;
        flux_m = params->DqRefPrm->Rotor_Flux_Nominal;
#if (DQREF__FLUX_ADAPTATION==ENABLED)
        // adaptive calculation of magnet flux
        Flux_M_Braking += DQREF__G_FLUX_ADAPT_GAIN * (Id_Braking - *io->Ctrl_Specific->Id_Rotor_Current);
        // saturate if needed
        if (Flux_M_Braking < DQREF__FLUX_ADAPT_SAT_LOW * flux_m)
        {
            Flux_M_Braking = DQREF__FLUX_ADAPT_SAT_LOW * flux_m;
        }
        else if (Flux_M_Braking > DQREF__FLUX_ADAPT_SAT_HI * flux_m)
        {
            Flux_M_Braking = DQREF__FLUX_ADAPT_SAT_HI * flux_m;
        }
        flux_m = Flux_M_Braking;
#else
        flux_m = params->DqRefPrm->Rotor_Flux_Nominal;
#endif
        // iq_braking given torque_ref and id_braking
        temp_1 = flux_m + (ld-lq)*Id_Braking;
        iq_braking = *(io->Torque_Ref) * params->DqRefPrm->K_torque_inv * params->DqRefPrm->Rotor_Flux_Nominal / temp_1;

        flux_d = flux_m + ld * Id_Braking;
        flux_q = lq * iq_braking;

        total_squared_flux = flux_d * flux_d + flux_q * flux_q;
        flux_ref_mtpa = MC_SQRT_F(total_squared_flux);
    }
    return flux_ref_mtpa;

}
