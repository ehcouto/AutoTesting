/**
 *  @file       MciSensorsWm.h
 *
 *  @brief      Header file for Indesit Mci Sensors Algorithms module
 *
 *  $Header: $
 *
 *  @copyright  Copyright 2016-$Date$. Whirlpool Corporation. All rights reserved - CONFIDENTIAL
 */
//---------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------
#ifndef MCISENSORSWM_H_
    #define MCISENSORSWM_H_


//-------------------------------------- Include Files ----------------------------------------------------------------
#include "C_Extensions.h"
#include "MciSensorsWm_prm.h"

//=====================================================================================================================
//-------------------------------------- PUBLIC (Extern Variables, Constants & Defines) -------------------------------
//=====================================================================================================================

#ifndef MCI_MEAN_VALUE_CALC
    #define MCI_MEAN_VALUE_CALC     DISABLED
#endif

#ifndef MCI_OVERHEATING_WASH
    #define MCI_OVERHEATING_WASH    DISABLED
#endif

#ifndef MCI_POWER_METER
    #define MCI_POWER_METER         DISABLED
#endif

#ifndef MCI_DRUM_LOAD_AVERAGE
    #define MCI_DRUM_LOAD_AVERAGE   DISABLED
#endif

#ifndef MCI_INVERT_DIRECTION
    #define MCI_INVERT_DIRECTION    DISABLED
#endif


//! Analog integer inputs to application
typedef enum
{
    /* MCI AI Channels
     * These channels are 1:1 with MCI_AI_TYPE; Speed an Torque channels will take care on Transmission Ratio
     */
    MCISENSORSWM_AI_TARGET_SPEED_RPM_S16 = 0,                //!<  0 - Target drum speed in RPM_Drum - x65536 - (1<<16)
    MCISENSORSWM_AI_SPEED_REFERENCE_RPM_S16,                 //!<  1 - Drum Speed reference in RPM_Drum - x65536 - (1<<16)
    MCISENSORSWM_AI_ACTUAL_SPEED_S16,                        //!<  2 - Estimated drum speed in RPM_Drum - x65536 - (1<<16)
    MCISENSORSWM_AI_SPEED_ERROR_RPM_S16,                     //!<  3 - Difference between estimated and reference speeds in RPM_Drum - x65536 - (1<<16)

    MCISENSORSWM_AI_MEAN_SPEED_S16,                          //!<  4 - Mean drum speed calculation in RPM_Drum - x65536 - (1<<16)
    MCISENSORSWM_AI_RMS_MOTOR_CURRENT_S16,                   //!<  5 - RMS Motor current - x65536 - (1<<16)
    MCISENSORSWM_AI_ACTIVE_POWER_S16,                        //!<  6 - Measured active power in Watts - x65536 - (1<<16)
    MCISENSORSWM_AI_SHAFT_POWER_S16,                         //!<  7 - Estimated shaft output power in Watts from estimated torque - x65536 - (1<<16)

    MCISENSORSWM_AI_ROTOR_TEMP_S16,                          //!<  8 - Rotor temperature in Celsius - x65536 - (1<<16)
    MCISENSORSWM_AI_STATOR_TEMP_S16,                         //!<  9 - Stator temperature in Celsius - x65536 - (1<<16)
    MCISENSORSWM_AI_LOAD_TORQUE_S16,                         //!< 10 - Read the motor load torque in Nm_Drum - x65536 - (1<<16)
    MCISENSORSWM_AI_SHAFT_POSITION_DEG_S16,                  //!< 11 - Estimated rotor position in deg - x65536 - (1<<16)

    MCISENSORSWM_AI_BUS_CURRENT_S16,                         //!< 12 - Bus current in Amps - x65536 - (1<<16)
    MCISENSORSWM_AI_BUS_VOLTAGE_S16,                         //!< 13 - Bus voltage in Volts - x65536 - (1<<16)
    MCISENSORSWM_AI_INVERTER_TEMP_S16,                       //!< 14 - Inverter temperature in Celsius - x65536 - (1<<16)
    MCISENSORSWM_AI_INVERTER_TEMP_RAW_DATA,                  //!< 15 - Inverter temperature in raw data [ADC counts]

    MCISENSORSWM_AI_VOLTAGE_PHASE_A_S16,                     //!< 16 - Motor phase A voltage in Volts - x65536 - (1<<16)
    MCISENSORSWM_AI_VOLTAGE_PHASE_B_S16,                     //!< 17 - Motor phase B voltage in Volts - x65536 - (1<<16)
    MCISENSORSWM_AI_VOLTAGE_PHASE_C_S16,                     //!< 18 - Motor phase C voltage in Volts - x65536 - (1<<16)
    MCISENSORSWM_AI_SPEED_LOOP_GAIN_TABLE_INDEX,             //!< 19 - Select index of speed loop gain table

    MCISENSORSWM_AI_CURR_PHASE_A_S16,                        //!< 20 - Phase current A in A - x65536 - (1<<16)
    MCISENSORSWM_AI_CURR_PHASE_B_S16,                        //!< 21 - Phase current B in A - x65536 - (1<<16)
    MCISENSORSWM_AI_CURR_PHASE_C_S16,                        //!< 22 - Phase current C in A - x65536 - (1<<16)
    MCISENSORSWM_AI_STATOR_RESISTANCE_S16,                   //!< 23 - Stator resistance in Ohm - x65536 - (1<<16)

    MCISENSORSWM_AI_INTERNAL_MCI_STATE,                      //!< 24 - Returns the internal Mci state


#if (MCI_MEAN_VALUE_CALC == ENABLED)
	MCISENSORSWM_AI_TORQUE_MEAN,                            //!< 25 - Motor torque mean value in Nm_Drum - x65536 - (1<<16)
    MCISENSORSWM_AI_BALANCE,                                //!< 26 - Load unbalance value - x65536 - (1<<16)
#endif

#if (MCI_OVERHEATING_WASH == ENABLED)
	MCISENSORSWM_AI_OVERHEATING_WASHING,                    //!< 27 - Overheating index of inverter IGBTs [pu, base A^2]
#endif

#if (MCI_DRUM_LOAD_AVERAGE == ENABLED)
    MCISENSORSWM_AI_DRUM_LOAD_AVERAGE,                      //!< 28 - Drum Load Average [Nm on Drum]
#endif

#if (MCI_POWER_METER == ENABLED)
    MCISENSORSWM_AI_MOTOR_LINE_POWER,                       //!< 29 - Inverter + Motor Line Power [W]
    MCISENSORSWM_AI_MOTOR_LINE_POWER_HI,                    //!< 30 - Inverter + Motor Line Energy [W]
    MCISENSORSWM_AI_MOTOR_LINE_POWER_LO,                    //!< 31 - Inverter + Motor Line Energy [W]
#endif

#if (MCI_MEAN_VALUE_CALC == ENABLED)
    MCISENSORSWM_AI_BALANCE_COUNT,                          //!< 29 - Drum revolutions before unbalance index calculation TODO:temporary diplacement
#endif

    MCISENSORSWM_AI_NR_OF_AI                                //!< Total declared of analog inputs
} MCISENSORSWM_AI_TYPE;



//! Analog float inputs to application
typedef enum
{
#if (MCI_MEAN_VALUE_CALC == ENABLED)
    MCISENSORSWM_FLOAT_AI_TORQUE_MEAN = 0,            //!<  0 - Motor torque mean value
#endif

#if (MCI_OVERHEATING_WASH == ENABLED)
    MCISENSORSWM_FLOAT_AI_OVERHEATING_WASHING,        //!<  1 - Overheating index of inverter IGBTs [pu, base A^2]
#endif

#if (MCI_DRUM_LOAD_AVERAGE == ENABLED)
    MCISENSORSWM_FLOAT_AI_DRUM_LOAD_AVERAGE,          //!<  2 - Drum Load Average [Nm on Drum]
#endif

    MCISENSORSWM_FLOAT_AI_NR_OF_AI                    //!< Total declared of analog inputs
} MCISENSORSWM_FLOAT_AI_TYPE;

//=====================================================================================================================
//-------------------------------------- PUBLIC (Function Prototypes) -------------------------------------------------
//=====================================================================================================================

void MciSensorsWm__Initialize(void);
BOOL_TYPE MciSensorsWm__IsParamsReady(void);
void MciSensorsWm__1msHandler(void);
void MciSensorsWm__5msHandler(void);
sint16 MciSensorsWm__GetIndesitAnalog(MCISENSORSWM_AI_TYPE read_data);
sint32 MciSensorsWm__GetAnalog(MOTOR_ENUM_TYPE motor, MCISENSORSWM_AI_TYPE read_data);
float MciSensorsWm__GetFloatAnalog(MCISENSORSWM_FLOAT_AI_TYPE read_data);
float32 MciSensorsWm__GetTransmissionRatio(void);

#endif /* MCISENSORSWM_H_ */



