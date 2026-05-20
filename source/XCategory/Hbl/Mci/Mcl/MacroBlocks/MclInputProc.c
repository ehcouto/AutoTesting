/**
 *  @file
 *  @brief       Motor Control Loop Input Processing.
 *  @details     This module implements the DC-Bus voltage filtering, the motor current clarke transformation.
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
#include "MclInputProc.h"
#include "MclConfig.h"
#include "McMathCalc_macros.h"
#include "Filters.h"
#include "MathCalc.h"
#include "ClrkPark.h"



//-------------------------------------- PUBLIC (Variables) -----------------------------------------------------------


#define MCL_INPUT_CALL_RATE_HZ              8000.0f // [Hz] define the call rate of the main handler
#define MCL_INPUT_1MS_CALL_RATE_HZ          1000.0f // [Hz] define the call rate of the main handler

#ifndef INV_3
    #define INV_3                           (float32) (1.0f/3.0f)
#endif


//-------------------------------------- PRIVATE (Variables, Constants & Defines) -------------------------------------
#if (INPUTPROC__VDCFILTER_MINIMUM == ENABLED)
static float32 DC_Bus_Min;                           // [V] Instant minimum DC bus voltage
static float32 DC_Bus_Min_temp;                      // [V] Instant minimum DC bus voltage - temporary

static float32 DC_Bus_Min_Sum;                       // [V] dc bus filtering history
static float32 DC_Bus_Min_Sum_2nd;                   // [V] dc bus filtering history
#endif

#if (INPUTPROC__VDC_RMS == ENABLED)
static float32  DC_Bus_RMS_Sum;
static volatile float32 DC_Bus_RMS;
#endif

//-------------------------------------- PRIVATE (Function Prototypes) ------------------------------------------------

static void VdcFilter(MCL_INPUT_PROC_IO_F_TYPE *io, MCL_INPUT_PROC_PARAMS_TYPE *params);
static void PhaseVoltageReconstruction(MCL_INPUT_PROC_IO_F_TYPE *io);
static void CurrentClarkTransform(MCL_INPUT_PROC_IO_F_TYPE *io);
static void CalcStatorCurrentMagnitude(MCL_INPUT_PROC_IO_F_TYPE *io);


//=====================================================================================================================
//-------------------------------------- Public Functions -------------------------------------------------------------
//=====================================================================================================================





//---------------------------------------------------------------------------------------------------------------------
/**
 *  @brief      Reset all Mcl Init Processing quantities.
 *  @details    This routine reset all Mcl Init Processing quantities, it has to be called at every time the pwm is switched off (motor stop or free down ramp).
 *
 *
 *  @param[in]     
 *  @param[out]
 *  @return        
 */
void MclInputProc__ResetState(void)
{

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
void MclInputProc__Initialize(void)
{
#if (INPUTPROC__VDCFILTER_MINIMUM == ENABLED)
    DC_Bus_Min_Sum      = 0.0f;
    DC_Bus_Min_Sum_2nd  = 0.0f;
#endif
#if (INPUTPROC__VDC_RMS == ENABLED)
    DC_Bus_RMS_Sum = 0;
#endif


}



//---------------------------------------------------------------------------------------------------------------------
/**
 *  @brief     Input processing
 *  @details   Input processing:   motor speed reference absolute value
 *                                 motor phase currents swapping in order to work with a positive speed reference.
 *                                 dc voltage filtering
 *                                 alpha-beta motor phase currents calculation.
 *
 *  @param[in]     
 *  @param[out]     
 *  @param[in]      
 *  @return        
 */
void MclInputProc__RunningHandler(MCL_INPUT_PROC_IO_F_TYPE *io)
{


    //==========================================================================//
    //                                                                          //
    //                     Forward Clarke Transform                             //
    //       Transforms ABC currents to Alpha/Beta coordinate system            //
    //                                                                          //
    //==========================================================================//
    CurrentClarkTransform(io);


    //==========================================================================//
    //                                                                          //
    //                    Phase voltage reconstruction                          //
    //                                                                          //
    //==========================================================================//
    PhaseVoltageReconstruction(io);
}


void MclInputProc__1msRunningHandler(MCL_INPUT_PROC_IO_F_TYPE *io, MCL_INPUT_PROC_PARAMS_TYPE *params)
{
    //==========================================================================//
    //                                                                          //
    //              Calculating the stator current magnitude                    //
    //                                                                          //
    //==========================================================================//
    CalcStatorCurrentMagnitude(io);


    //==========================================================================//
    //                                                                          //
    //              Filter DC bus and track lowest voltage                      //
    //                                                                          //
    //==========================================================================//
    VdcFilter(io, params);
}



#ifdef __IAR_SYSTEMS_ICC__
#pragma inline = forced
#endif
//---------------------------------------------------------------------------------------------------------------------
/**
 *  @brief      Apply Clarke transform to the currents.
 *  @details    By applying Clarke transformation to the currents, it shall provide the alpha and beta currents
 *              from the measured currents a, b and c.
 *
 *  @param[in]  MCL_INPUT_PROC_IO_TYPE *io
 *  @param[out] MCL_INPUT_PROC_IO_TYPE *io
 *  @return     none
 */
void CurrentClarkTransform(MCL_INPUT_PROC_IO_F_TYPE *io)
{
    ClrkPark__DirectClarkeF(io->Is_ABC, io->Is_Alpha_Beta);
}




//---------------------------------------------------------------------------------------------------------------------
/**
 *  @brief      Calculate the peak current (magnitude) from alpha and beta currents
 *  @details    By applying Clarke transformation to the currents, it shall provide the alpha and beta currents
 *              from the measured currents a, b and c.
 *
 *  @param[in]  MCL_INPUT_PROC_IO_TYPE *io
 *  @param[out] MCL_INPUT_PROC_IO_TYPE *io
 *  @return     none
 */
void CalcStatorCurrentMagnitude(MCL_INPUT_PROC_IO_F_TYPE *io)
{
    float32 temp;

    temp  = (io->Is_Alpha_Beta->Alpha) * (io->Is_Alpha_Beta->Alpha);
    temp += (io->Is_Alpha_Beta->Beta)  * (io->Is_Alpha_Beta->Beta);

    *(io->Ctrl_Specific->Is_Abs) = MC_SQRT_F(temp);
}




//---------------------------------------------------------------------------------------------------------------------
/**
 *  @brief      Tracks the minimum instant dc bus voltage
 *  @details    This algorithm tracks the minimum instant dc bus voltage voltage, keep the minimum for certain
 *              amount of time before reseting the temporary minimum and update the value. In this algorithm,
 *              the minimum is always keep at the minimum of the instant voltage.
 *
 *  @param[in]  MCL_INPUT_PROC_IO_TYPE *io
 *  @param[out] MCL_INPUT_PROC_IO_TYPE *io
 *  @return     none
 */
void VdcFilter(MCL_INPUT_PROC_IO_F_TYPE *io, MCL_INPUT_PROC_PARAMS_TYPE *params)
{
#if (INPUTPROC__VDCFILTER_MINIMUM == ENABLED)
    static uint8 prescaler_25ms = (uint8)(MCL_INPUT_1MS_CALL_RATE_HZ/40.0f);
#endif
#if (INPUTPROC__VDC_RMS == ENABLED)
    static uint8 window_avg = 80;
    float32 Vdc;
#endif
#if (INPUTPROC__VDCFILTER_MINIMUM == ENABLED)
    // Filter the minimum instant dc bus voltage
    DC_Bus_Min_Sum = FILTERS__LOWPASSFILTER_F(DC_Bus_Min_Sum, DC_BUS_LPF_COEF, DC_Bus_Min);

	DC_Bus_Min_Sum_2nd = FILTERS__LOWPASSFILTER_F(DC_Bus_Min_Sum_2nd, DC_BUS_LPF_COEF, DC_Bus_Min_Sum);

	*(io->Vs_Max_Filt) = DC_Bus_Min_Sum_2nd;

	*(io->Vs_Max_Filt) = params->Max_Available_Voltage_Factor * *(io->Vs_Max_Filt);


    // every 25 ms, the temporary minimum is reset
    prescaler_25ms--;
    if(!prescaler_25ms)
    {
        prescaler_25ms = (uint8)(MCL_INPUT_1MS_CALL_RATE_HZ/40.0f);

        // This is to bring up the Dc_Bus_Pu_Min because in AtodISR it only get reduced.
        // dcbus_pu_min_target is reset periodically.
        if(DC_Bus_Min < DC_Bus_Min_temp)
        {
            DC_Bus_Min = DC_Bus_Min_temp;
        }

        //Reset dcbus_pu_min_target. Bring it up. It get reduced at AtodISR.
        DC_Bus_Min_temp = *(io->Vdc);
    }


    // Look for the minimum value
    if(*(io->Vdc) < DC_Bus_Min)
    {
        DC_Bus_Min = *(io->Vdc);
    }
    if(*(io->Vdc) < DC_Bus_Min_temp)
    {
        DC_Bus_Min_temp = *(io->Vdc);
    }
#endif
#if (INPUTPROC__VDC_RMS == ENABLED)

    Vdc = *(io->Vdc);
    DC_Bus_RMS_Sum += (Vdc * Vdc);

    // every 80 samples we update the RMS (8000Hz / 100Hz = 80)
    window_avg--;
    if(!window_avg)
    {
        window_avg = 80;
        DC_Bus_RMS = MC_SQRT_F(0.0125f * DC_Bus_RMS_Sum);  // RMS calculation
        DC_Bus_RMS_Sum = 0.0f;
    }
#endif

}



#ifdef __IAR_SYSTEMS_ICC__
#pragma inline = forced
#endif
//---------------------------------------------------------------------------------------------------------------------
/**
 *  @brief      Reconstruct applied phase voltage from the last pwm command duty cycles.
 *  @details
 *
 *  @param[in]  MCL_INPUT_PROC_IO_TYPE *io
 *  @param[out] MCL_INPUT_PROC_IO_TYPE *io
 *  @return     none
 */
void PhaseVoltageReconstruction(MCL_INPUT_PROC_IO_F_TYPE *io)
{
    float32 temp_vdc;
    float32 temp_common_mode;

    // Check if the command to enable lowers on was enabled, if it was enabled
    if( *(io->Ctrl_Specific->Lowers_On) )
    {
        // voltages applied to the motor is zero.
        io->Vs_Alpha_Beta_Rec->Alpha = 0.0f;
        io->Vs_Alpha_Beta_Rec->Beta = 0.0f;

        io->Vabc_Rec->A = 0.0f;
        io->Vabc_Rec->B = 0.0f;
        io->Vabc_Rec->C = 0.0f;

    }
    else
    {
        // Read the last duty cycle and scale it to the DC bus voltage.
        // Note: assume the DC bus voltage is kept constant within one pwm period.
        temp_vdc = *(io->Vdc);
        io->Vabc_Rec->A = (io->Duty->A * temp_vdc);
        io->Vabc_Rec->B = (io->Duty->B * temp_vdc);
        io->Vabc_Rec->C = (io->Duty->C * temp_vdc);

        //ClrkPark__DirectClarkeF(io->Vabc_Rec, io->Vs_Alpha_Beta_Rec);

        temp_common_mode = (io->Vabc_Rec->A) + (io->Vabc_Rec->B) + (io->Vabc_Rec->C);
        temp_common_mode = temp_common_mode * INV_3 ;

        io->Vabc_Rec->A -= temp_common_mode;
        io->Vabc_Rec->B -= temp_common_mode;
        io->Vabc_Rec->C -= temp_common_mode;

        ClrkPark__DirectClarkeF(io->Vabc_Rec, io->Vs_Alpha_Beta_Rec);

    }
}


