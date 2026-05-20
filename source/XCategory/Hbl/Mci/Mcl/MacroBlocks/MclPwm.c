/**
 *  @file
 *  @brief       Motor Control Loop Pwm.
 *  @details     This module implements the Pwm module.
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

#include "MclPwm.h"
#include "MclConfig.h"
//#include "InverterComp.h"
#include "PwmModulation.h"
#include "ClrkPark.h"
#include "MclPwm_prv.h"


//-------------------------------------- PUBLIC (Variables) -----------------------------------------------------------


//-------------------------------------- PRIVATE (Variables, Constants & Defines) -------------------------------------
// Inverter Compensation types
#define INVERTER_LOSSES_COMP        0       // complete losses compensation
#define DEADTIME_COMP               1       // deadtime compensation

#define DUTY_INIT       0.5f

#ifndef RPM_TO_RADS
    #define RPM_TO_RADS     0.10471975511965977461542144610932f
#endif


//-------------------------------------- PRIVATE (Function Prototypes) ------------------------------------------------

//=====================================================================================================================
//-------------------------------------- Public Functions -------------------------------------------------------------
//=====================================================================================================================
#define SPEED_THRESHOLD             (0.05f)         //!< [%] speed hysteresis threshold

//INVERTER_COMP_TYPE Inverter_Dist_Pointer;
//ALPHA_BETA_COOR_SYST_TYPE Inverter_Distortion_Voltage;
float32 Speed_Threshold_High;                        //!< [absolute rpm] speed hysteresis high limits
float32 Speed_Threshold_Low;                         //!< [absolute rpm] speed hysteresis low limits
float32 Speed_Threshold;                             //!< [absolute rpm] speed hysteresis selector
uint8 Inverter_Compensation_Selector;               //!< inverter compensation selector
uint8 Pwm_Sector;                                   //!< Pwm sector calculation

//---------------------------------------------------------------------------------------------------------------------
/**
 *  @brief      Reset all Mcl Dq Reference Controllers quantities.
 *  @details    This routine reset all q Reference Controllers quantities, it has to be called at every time the pwm is switched off (motor stop or free down ramp).
 *
 *
 *  @param[in]     
 *  @param[out]
 *  @return        
 */
void MclPwm__ResetState(MCL_PWM_IO_F_TYPE *io, MCLPWM_JOINT_PARAMS_TYPE *params)
{
    io->Vs_Alpha_Beta->Alpha = 0;
    io->Vs_Alpha_Beta->Beta = 0;

    io->Duty->A = DUTY_INIT;
    io->Duty->B = DUTY_INIT;
    io->Duty->C = DUTY_INIT;

    Speed_Threshold_High = (params->PwmPrm->Speed_Threshold_InvComp) * (1.0f + SPEED_THRESHOLD) * RPM_TO_RADS;
    Speed_Threshold_Low  = (params->PwmPrm->Speed_Threshold_InvComp) * (1.0f - SPEED_THRESHOLD) * RPM_TO_RADS;
    Speed_Threshold = Speed_Threshold_High;
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
void MclPwm__Initialize(MCLPWM_JOINT_PARAMS_TYPE* params)
{
   // Inverter_Dist_Pointer.Is_ABC = io->Is_ABC;
   // Inverter_Dist_Pointer.Vdc = io->Vdc;
   // Inverter_Dist_Pointer.distortion = &Inverter_Distortion_Voltage;

    Inverter_Compensation_Selector = INVERTER_LOSSES_COMP;

    params->PwmModulationPrm.ptr_LUT      = Dutycycle_By_Current_LUT;
    params->PwmModulationPrm.sizeof_lut   = &sizeof_inv_comp_lut;
    params->PwmModulationPrm.step_inv     = &step_inv_pwm_comp;

}



//---------------------------------------------------------------------------------------------------------------------
/**
 *  @brief     Pwm
 *  @details   Pwm:          DC voltage compensation
 *                           Deadtime compensation
 *                           PWM Duties generation
 *  @param[in]     
 *  @param[out]     
 *  @param[in]      
 *  @return        
 */
void MclPwm__RunningHandler(MCL_PWM_IO_F_TYPE *io, MCLPWM_JOINT_PARAMS_TYPE *params)
{
	// space vector modulation (including ripple compensation)
	Pwm_Sector = PwmModulation__SpaceVectorModulationF(*(io->Vdc), io->Vs_Alpha_Beta, io->Duty_bc, FALSE);

	// This step is required since the voltage reconstruction uses the duties before compensation
	io->Duty->A = io->Duty_bc->A;
	io->Duty->B = io->Duty_bc->B;
	io->Duty->C = io->Duty_bc->C;

   #ifndef COMPILE_4_SIMULINK
    #if (OBS__INVERTER_DISTORSION_COMPENSATION == DISABLED)
	// inverter compensation strategy --> feedforward approach
	if(Inverter_Compensation_Selector == INVERTER_LOSSES_COMP)
	{
		PwmModulation__InverterLossCompF(io->Is_ABC, io->Duty, &params->PwmModulationPrm);
	}
	else
	{
		PwmModulation__DeadtimeCompensationGradientF(io->Is_ABC, io->Duty);
	}
    #endif
   #endif
}

void MclPwm__25msHandler(MCL_PWM_IO_F_TYPE *io)
{
    float32 temp_abs_speed;

    temp_abs_speed = *(io->Ctrl_Specific->Speed_Rotor_Observed_Mech_Abs);

    if(temp_abs_speed >= Speed_Threshold)
    {
        Speed_Threshold = Speed_Threshold_Low;

        Inverter_Compensation_Selector = DEADTIME_COMP;
    }
    if(temp_abs_speed <= Speed_Threshold)
    {
        Speed_Threshold = Speed_Threshold_High;

        Inverter_Compensation_Selector = INVERTER_LOSSES_COMP;
    }
}






