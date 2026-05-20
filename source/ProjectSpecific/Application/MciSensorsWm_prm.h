/**
 *  @file       MciSensorsWm_prm.h
 *
 *  @brief      Public Parameters of Indesit WM Mci Sensors module
 *
 *  $Header: $
 *
 *  @copyright  Copyright 2016-$Date$. Whirlpool Corporation. All rights reserved - CONFIDENTIAL
 */
//---------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------
#ifndef MCISENSORSWM_PRM_H
    #define MCISENSORSWM_PRM_H


//-------------------------------------- Include Files ----------------------------------------------------------------
#include "C_Extensions.h"
#include "Mci.h"
#include "SRMC_defs.h"
//=====================================================================================================================
//-------------------------------------- PUBLIC (Extern Variables, Constants & Defines) -------------------------------
//=====================================================================================================================
//=====================================================================================================================
/**
 *    @brief  - Defines if the mci sensors wm parameters are loaded from setting file or defined internally the fw
 *    @details- If set to ENABLED the parameters are embedded into the fw
 *
 *    @param    - ENABLED / DISABLED
 *    @note     - "#define MCI_SENSORS_PARAMETERS_INTERNAL   DISABLED"   // Mci sensors wm parameters loaded from setting file
 *              - "#define MCI_SENSORS_PARAMETERS_INTERNAL   ENABLED"   // Mci  sensors wm parameters defined internally
 */
//=====================================================================================================================
#ifndef MCI_INTERNAL_PARAMS
#define MCI_SENSORS_PARAMETERS_INTERNAL                DISABLED
#else
#define MCI_SENSORS_PARAMETERS_INTERNAL                ENABLED
#endif


//=====================================================================================================================
/**
 *    @brief  - Defines if the torque meaning and unbalance index calculations are activated
 *    @details-
 *
 *    @param    - ENABLED / DISABLED
 *    @note     - "#define MCI_MEAN_VALUE_CALC   DISABLED"   // MeanValueCalc() is not executed
 *              - "#define MCI_MEAN_VALUE_CALC   ENABLED"   // MeanValueCalc() is executed
 */
//=====================================================================================================================
#define MCI_MEAN_VALUE_CALC   ENABLED


//=====================================================================================================================
/**
 *    @brief  - Defines if the overheating washing index calculation is activated
 *    @details-
 *
 *    @param    - ENABLED / DISABLED
 *    @note     - "#define MCI_OVERHEATING_WASH   DISABLED"   // OverHeating_Washing() is not executed
 *              - "#define MCI_OVERHEATING_WASH   ENABLED"   //OverHeating_Washing() is executed
 */
//=====================================================================================================================
#define MCI_OVERHEATING_WASH   ENABLED


//=====================================================================================================================
/**
 *    @brief  - Defines if the mci power meter quantities calculation is activated
 *    @details-
 *
 *    @param    - ENABLED / DISABLED
 *    @note     - "#define MCI_POWER_METER   DISABLED"   // PowerMeter() is not executed
 *              - "#define MCI_POWER_METER   ENABLED"   // PowerMeter() is executed
 */
//=====================================================================================================================
#define MCI_POWER_METER         DISABLED


//=====================================================================================================================
/**
 *    @brief  - Defines if the drum load average calculation is activated
 *    @details-
 *
 *    @param    - ENABLED / DISABLED
 *    @note     - "#define MCI_DRUM_LOAD_AVERAGE   DISABLED"   // DrumLoadAverage() is not executed
 *              - "#define MCI_DRUM_LOAD_AVERAGE   ENABLED"   // DrumLoadAverage() is executed
 */
//=====================================================================================================================
#define MCI_DRUM_LOAD_AVERAGE   ENABLED

//=====================================================================================================================
/**
 *    @brief  - Defines default motor direction
 *    @details-
 *
 *    @param    - ENABLED / DISABLED
 *    @note     - "#define MCI_INVERT_DIRECTION   DISABLED"   // Motor has same direction as in Mci
 *              - "#define MCI_INVERT_DIRECTION   ENABLED"    // Inverted direction by default
 */
//=====================================================================================================================
#define MCI_INVERT_DIRECTION     DISABLED



#define TORQUE_NM256_2_PU_SHIFT     3


#if (PLATFORM_USED == PLATFORM_2_5_BASED)
#define RATE_FOR_UNBALANCE_COUNTER  8
#else
#define RATE_FOR_UNBALANCE_COUNTER  1
#endif

#define OH_WASHING_MAX_PERIOD		6400 	//32 s @ 5ms



typedef struct
{
#if (MCI_MEAN_VALUE_CALC == ENABLED)
	sint16 K_Turns;
	sint16 Mean_Over_Drum_Rev;
    // Threshold speed
    sint16 Spinning_Thr_Speed;
#endif

#if ((MCI_DRUM_LOAD_AVERAGE == ENABLED)||(MCI_MEAN_VALUE_CALC==ENABLED))
    uint16 Dummy;
    float Transmission_Ratio;
#endif

#if (MCI_POWER_METER == ENABLED)
    //---- Power Meter -----//
    sint16 KEnergy;
    sint16 PowerConversion;
    sint16 EnergyConversion;
#endif

}MCISENSORSWM_PARAMS_TYPE;

//=====================================================================================================================
//-------------------------------------- PUBLIC (Function Prototypes) -------------------------------------------------
//=====================================================================================================================


#endif /* MCISENSORSWM_PRM_H */
