/**
 *  @file       MciSetWm_prm.h
 *
 *  @brief      Public Parameters of Indesit Wm Mci Set module
 *
 *  $Header: $
 *
 *  @copyright  Copyright 2016-$Date$. Whirlpool Corporation. All rights reserved - CONFIDENTIAL
 */
//---------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------
#ifndef MCISETWM_PRM_H
    #define MCISETWM_PRM_H


//-------------------------------------- Include Files ----------------------------------------------------------------
#include "C_Extensions.h"
#include "Mci.h"
//=====================================================================================================================
//-------------------------------------- PUBLIC (Extern Variables, Constants & Defines) -------------------------------
//=====================================================================================================================
//=====================================================================================================================
/**
 *    @brief  - Defines if the mci set wm parameters are loaded from setting file or defined internally the fw
 *    @details- If set to ENABLED the parameters are embedded into the fw
 *
 *    @param    - ENABLED / DISABLED
 *    @note     - "#define MCI_SET_PARAMETERS_INTERNAL   DISABLED"   // Mci sensors wm parameters loaded from setting file
 *              - "#define MCI_SET_PARAMETERS_INTERNAL   ENABLED"   // Mci  sensors wm parameters defined internally
 */
//=====================================================================================================================
#ifndef MCI_INTERNAL_PARAMS
#define MCI_SET_PARAMETERS_INTERNAL                DISABLED
#else
#define MCI_SET_PARAMETERS_INTERNAL                ENABLED
#endif

//=====================================================================================================================
/**
 *    @brief  - Defines if the mci speed regulator gains can be set by the method MciSetWm__UpdateSpeedRegulator()
 *    @details- If set to ENABLED the method MciSetWm__UpdateSpeedRegulator() require a speed reference as input
 *
 *    @param    - ENABLED / DISABLED
 *    @note     - "#define UPDATE_SPEED_REGULATOR   DISABLED"   // Mci Speed regulator gains are not set by the method MciSetWm__UpdateSpeedRegulator()
 *              - "#define UPDATE_SPEED_REGULATOR   ENABLED"   // Mci Speed regulator gains can be set by the method MciSetWm__UpdateSpeedRegulator()
 */
//=====================================================================================================================
#define UPDATE_SPEED_REGULATOR   DISABLED

//=====================================================================================================================
/**
 *    @brief  - Defines if the maximum motor spinning torque is limited by the method DynamicTorqueLimits()
 *    @details- If set to ENABLED the method DynamicTorqueLimits() set to mci a maximum motor torque reference according the setting file parameters
 *
 *    @param    - ENABLED / DISABLED
 *    @note     - "#define DYNAMIC_TORQUE_LIMITS   DISABLED"   // Maximum motor spinning torque is not set by DynamicTorqueLimits()
 *              - "#define DYNAMIC_TORQUE_LIMITS   ENABLED"   // Maximum motor spinning torque is set by DynamicTorqueLimits()
 */
//=====================================================================================================================
#define DYNAMIC_TORQUE_LIMITS   ENABLED


#ifdef MOTOR_CIM_TYPE

//=====================================================================================================================
/**
 *    @brief  - Defines if the speed fir filter taps can be set by the method MciSetWm__UpdateSpeedFirTaps()
 *    @details- If set to ENABLED the method MciSetWm__UpdateSpeedFirTaps() has to be called every 5 ms
 *
 *    @param    - ENABLED / DISABLED
 *    @note     - "#define UPDATE_SPEED_FIR_TAPS   DISABLED"   // Speed Fir Filter taps are not set by the method MciSetWm__UpdateSpeedFirTaps()
 *              - "#define UPDATE_SPEED_FIR_TAPS   ENABLED"   // Speed Fir Filter taps are  can be set by the method MciSetWm__UpdateSpeedFirTaps()
 */
//=====================================================================================================================
#define UPDATE_SPEED_FIR_TAPS   ENABLED


//=====================================================================================================================
/**
 *    @brief  - Defines if the maximum motor spinning torque is limited by the method DynamicTorqueLimits()
 *    @details- If set to ENABLED the MOTOR_CIM_TYPE shall be defined
 *    			MciSetWm__SoftStart(SOFT_START_INITIALIZATION) method shall be called after Mci initialization
 *    			MciSetWm__SoftStart(SOFT_START_ON_START) method shall be called after the Mci run command
 *    			MciSetWm__SoftStart(SOFT_START_ON_STOP) method shall be called before the Mci stop command
 *
 *    @param    - ENABLED / DISABLED
 *    @note     - "#define CIM_SOFT_START   DISABLED"   // The CIM starting torque is fixed and not calculated by softstart algorithm
 *              - "#define CIM_SOFT_START   ENABLED"   // The CIM starting torque is variable and calculated by softstart algorithm
 */
//=====================================================================================================================
#define CIM_SOFT_START   ENABLED


#endif


#ifdef MOTOR_CIM_TYPE

	#if (UPDATE_SPEED_FIR_TAPS == ENABLED)
	//Speed Fir Filter Taps parameters
	#define SPEED_TAPS_STARTUP          0x0204  // 2 taps_log => 4 taps value 0xZZ|YY
												// ZZ - taps_log & YY - taps value
	#define STARTUP_THR_SPEED_PERC      (0.01 * 19800)
	#endif

	#if (CIM_SOFT_START == ENABLED)
	//Soft start parameters
	#define SOFT_START_APP_MAX_COUNTER      32
	#define SOFT_START_APP_MED_COUNTER      20
	#define SOFT_START_APP_MIN_COUNTER      2

	#define SOFT_START_MAX_CNT_PERIOD   	20 // number of starting with maximum softstart
	#define SOFT_START_SPEED_TOLLERANCE 	3
	#endif

#endif

#if (DYNAMIC_TORQUE_LIMITS == ENABLED)
#define TORQUE_LIM_LENGHT               8
#define VDC_PERIOD_MIN              	500
#endif

typedef struct
{
    sint8 Integ_Gain;
    sint8 Prop_Gain;
}SHIFT_CONTROLLER_MCISET_PARAMS_TYPE;

typedef struct
{
    // Threshold speed
    sint16 Low_Regulators_Thr_Speed;
    sint16 Med_Regulators_Thr_Speed;
    sint16 High_Regulators_Thr_Speed;


    // Speed current controllers
    uint16 Prop_Gain_Speed_Low_Lo;      //!< low speed controller proportional gain mantissa value - low part of Q17.15
    uint16 Prop_Gain_Speed_Low_Hi;      //!< low speed controller proportional gain mantissa value - high part of Q17.15
    uint16 Integ_Gain_Speed_Low_Lo;     //!< low speed controller integral gain mantissa value - low part of Q17.15
    uint16 Integ_Gain_Speed_Low_Hi;     //!< low speed controller integral gain mantissa value - high part of Q17.15
    SHIFT_CONTROLLER_MCISET_PARAMS_TYPE Gain_Shift_Speed_Low; //!< low speed controller proportional/integral gain exponent value

    // Speed Pi regulator Medium Region Gains
    uint16 Prop_Gain_Speed_Med_Lo;      //!< medium speed controller proportional gain mantissa value - low part of Q17.15
    uint16 Prop_Gain_Speed_Med_Hi;      //!< medium speed controller proportional gain mantissa value - high part of Q17.15
    uint16 Integ_Gain_Speed_Med_Lo;     //!< medium speed controller integral gain mantissa value - low part of Q17.15
    uint16 Integ_Gain_Speed_Med_Hi;     //!< medium speed controller integral gain mantissa value - high part of Q17.15
    SHIFT_CONTROLLER_MCISET_PARAMS_TYPE Gain_Shift_Speed_Med; //!< medium speed controller proportional/integral gain exponent value

    // Speed Pi regulator High Region Gains
    uint16 Prop_Gain_Speed_High_Lo;     //!< high speed controller proportional gain mantissa value - low part of Q17.15
    uint16 Prop_Gain_Speed_High_Hi;     //!< high speed controller proportional gain mantissa value - high part of Q17.15
    uint16 Integ_Gain_Speed_High_Lo;    //!< high speed controller integral gain mantissa value - low part of Q17.15
    uint16 Integ_Gain_Speed_High_Hi;    //!< high speed controller integral gain mantissa value - high part of Q17.15
    SHIFT_CONTROLLER_MCISET_PARAMS_TYPE Gain_Shift_Speed_High; //!< high speed controller proportional/integral gain exponent value

    // Speed Pi regulator Very High Region Gains
    uint16 Prop_Gain_Speed_Very_High_Lo;     //!< very high speed controller proportional gain mantissa value - low part of Q17.15
    uint16 Prop_Gain_Speed_Very_High_Hi;     //!< very high speed controller proportional gain mantissa value - high part of Q17.15
    uint16 Integ_Gain_Speed_Very_High_Lo;    //!< very high speed controller integral gain mantissa value - low part of Q17.15
    uint16 Integ_Gain_Speed_Very_High_Hi;    //!< very high speed controller integral gain mantissa value - high part of Q17.15
    SHIFT_CONTROLLER_MCISET_PARAMS_TYPE Gain_Shift_Speed_Very_High; //!< very high speed controller proportional/integral gain exponent value



    // DYNAMIC LIMITS TABLE
    sint16 SpeedLimit[TORQUE_LIM_LENGHT];       //!< Speed limits table for dynamic torque limitations
    sint16 TorqueLimit[TORQUE_LIM_LENGHT];      //!< Torque limits table for dynamic torque limitations
    sint16 TorqueLimit_Norm[TORQUE_LIM_LENGHT]; //!< Torque limits table for normative dynamic torque limitations
    sint16 MLimit[TORQUE_LIM_LENGHT];           //!< angular coefficient for linear dynamic torque limitations
    sint16 MLimit_Norm[TORQUE_LIM_LENGHT];      //!< angular coefficient for normative linear dynamic torque limitations

    sint16 Vdc_Norm;                            //!< bulk voltage thrshold over to active the normative limitations

    //Torque Limitation
    sint16 Torque_Ref_Max;      //!< Q1.15 speed controller upper limit in NO-SPINNING region - torque value


    //Startup
    uint16 Soft_Start_Rel;
    sint16 Torque_Ref_Max_Startup;  //!< Q1.15 max Torque reference during the open-loop phase at startup

    // Tacho Filter taps
    sint16 Speed_Taps_High;       //!< Number of speed FIR filter taps in high speed region: high part value, low part log
    sint16 Speed_Taps_Med;        //!< Number of speed FIR filter taps in medium speed region: high part value, low part log
    sint16 Speed_Taps_Low;        //!< Number of speed FIR filter taps in low speed region: high part value, low part log



}MCISETWM_PARAMS_TYPE;

//=====================================================================================================================
//-------------------------------------- PUBLIC (Function Prototypes) -------------------------------------------------
//=====================================================================================================================


#endif /* MCISETWM_PRM_H */



