/**
 *  @file       FaultManager.h
 *
 *  @brief      Header file for Indesit Fault Manager module
 *
 *  $Header: $
 *
 *  @copyright  Copyright 2016-$Date$. Whirlpool Corporation. All rights reserved - CONFIDENTIAL
 */
//---------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------
#ifndef FAULT_MANAGER_H_
    #define FAULT_MANAGER_H_

#include "C_Extensions.h"

//=====================================================================================================================
//-------------------------------------- PUBLIC (Extern Variables, Constants & Defines) -------------------------------
//=====================================================================================================================

//! Indesit Fault/Warning Codes definition
typedef enum
{
    NO_FAULT                           = 0x0000,
    UNDERVOLTAGE                       = 0x0001,
    OVERTORQUE_WARNING                 = 0x0002,
    NACK                               = 0x0003,
    STALL_AFTER_COMMUTATION_WARNING    = 0x0004,
    OVERVOLTAGE                        = 0x0006,
    OVERCURRENT_WARNING                = 0x0007,
    INVERTED_SPEED_WARNING             = 0x0008,
    OBSERVER_FAULT_WARNING             = 0x0009,
    MAX_SPEED_WARNING                  = 0x000A,  
	MCI_LEGACY_SURGE_RELAY_OPEN        = 0x000B,

    HARDWARE_OVERCURRENT_A             = 0x0181,  //!</* FAST */  /* NEW_V1_2_MOD */
    HARDWARE_OVERCURRENT_B             = 0x0182,  //!</* FAST */  /* NEW_V1_2_MOD */
    HARDWARE_OVERCURRENT_C             = 0x0183,  //!</* FAST */  /* NEW_V1_2_MOD */
    ADC_OFFSET_ST_A_FAULT              = 0x0184,  //!</* FAST */  /* NEW_V1_2_MOD */
    ADC_OFFSET_ST_B_FAULT              = 0x0185,  //!</* FAST */  /* NEW_V1_2_MOD */
    ADC_OFFSET_ST_C_FAULT              = 0x0186,  //!</* FAST */  /* NEW_V1_2_MOD */
    HARDWARE_OVERVOLTAGE               = 0x0187,  //!</* FAST */  /* NEW_V1_2_MOD */
    HARDWARE_OVERCURRENT_START         = 0x0190,  //!</* FAST */  /* NEW_V1_2_MOD */

    UNDERCURRENT_FAULT                 = 0x0281,  //!<  /* FAST */
    OVERCURRENT_FAULT                  = 0x0282,  //!<  /* FAST */
    OVERCURRENT_SLOW_SPEED_FAULT       = 0x0283,  //!<  /* FAST */
    NO_COMMUTATION_AFTER_START_FAULT   = 0x0284,  //!</* SLOW */
    STALL_AFTER_COMMUTATION_FAULT      = 0x0285,  //!</* SLOW */
    STALL_LOW_SPEED_FAULT              = 0x0286,  //!</* SLOW */
    TARGET_ZERO_NOT_REACHED_FAULT      = 0x0287,  //!</* SLOW */
    MIN_TACH_FAULT                     = 0x0288,  //!</* SLOW */
    MAX_SPEED_CHANGE_FAULT             = 0x0289,  //!</* SLOW */
    OH_FAULT                           = 0x028A,  //!<
    OH_STOP_FAULT                      = 0x028B,  //!</* SLOW */
    MAX_TORQUE_FAULT                   = 0x028C,  //!</* SLOW */
    MCL_FAULT                          = 0x028D,  //!</* SLOW */
    
    MCI_LEGACY_SPD_SENSOR_FAILEDD      = 0x0291,
    MCI_LEGACY_ELECTRONIC_FAILED       = 0x0292,
    MCI_LEGACY_MOTOR_TEST_FAILED       = 0x0293,
    MCI_LEGACY_INVERTER_OVER_TEMP      = 0x0294,
    MCI_LEGACY_STATOR_OVER_TEMP        = 0x0295,
    MCI_LEGACY_LOCKED_ROTOR_BY_SD      = 0x0296,
    MCI_LEGACY_PHASE_LOST              = 0x0297,
    MCI_LEGACY_INT_DISABLED            = 0x0298,
    MCI_LEGACY_MOTOR_OVERLOADED        = 0x0299,
    MCI_INITIALIZATION_FAULT           = 0x029A,
	SR_DCBUS_OUT_OF_RANGE			   = 0x029B,
	MCI_LEGACY_FORCE2STOP			   = 0x029C,
	MCI_LEGACY_SHUTDOWN_FAILED		   = 0x029D,

    MODULETYPE_NET_FAULT               = 0x0381,

    INVALID_SETFILE_CRC                = 0x0980,  //!</* MAIN */
    INVALID_SETFILE_LENGTH             = 0x0990,  //!</* MAIN */
    SETFILE_NOT_LOADED                 = 0x09A0,  //!</* MAIN */

    ///* [ mask for Code/Subcode Selection for Fault variable ] */
    SUBCODE                            = 0x00FF,
    CODE                               = 0xFF00
}FAULT_ENUM_TYPE;

//=====================================================================================================================
//-------------------------------------- PUBLIC (Function Prototypes) -------------------------------------------------
//=====================================================================================================================

void FaultManager__Initialize(void);
void FaultManager__Reset(void);
void FaultManager__UpdateFault(void);
FAULT_ENUM_TYPE FaultManager__GetFaultToPublish(void);
BOOL_TYPE FaultManager__GetResetState(void);

#endif /* FAULT_MANAGER_H_ */



