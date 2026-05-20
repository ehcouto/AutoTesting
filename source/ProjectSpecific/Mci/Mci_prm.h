/**
 *  @file       Mci_prm.h
 *  @brief      Basic description of file contents
 *
 *---------------------------------------------------------------------------------------------------------------------
 *------------------- Copyright 2007.  Whirlpool Corporation.  All rights reserved - CONFIDENTIAL ---------------------
 *---------------------------------------------------------------------------------------------------------------------
 */
#ifndef MCI_PRM_H_
	#define MCI_PRM_H_

#include "C_Extensions.h"
#include "SRMC_defs.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// PROJECT SPECIFIC: Windy sBPM Nucleus and PLT2.5 support //
/////////////////////////////////////////////////////////////
#define MOTOR_BPM_TYPE


// MOTOR COMMAND MODEs
#define MAIN_uC            0
#define FREEMASTER         1
#define SIL                2
#define MCI_BD             3
#define COMMAND_MANAGER_BD 4

#if (PLATFORM_USED == PLATFORM_2_5_BASED)
    #define COMMAND_MODE                    MCI_BD
    #define COM_MAIN_EXTERNAL               ENABLED
    #define MCI_INTERNAL_PARAMS             // uncomment it to use internal parameters
    #define OTE_SET_PARAMETERS_INTERNAL     // uncomment it to use internal parameters
#else
    #define COMMAND_MODE                    MAIN_uC
    #define COM_MAIN_EXTERNAL               DISABLED
    //#define MCI_INTERNAL_PARAMS             // uncomment it to use internal parameters
    //#define OTE_SET_PARAMETERS_INTERNAL     // uncomment it to use internal parameters

//***************** MCI_BD Mode - required steps ********************//
// in this file
// #define COMMAND_MODE     MCI_BD
// uncomment MCI_INTERNAL_PARAMS
// uncomment OTE_SET_PARAMETERS_INTERNAL
//
// in SurgeRelay_prm.h
// comment the line #define SRAPI20_CHECK()
//
// in SRMCUSpeedMonitor_prv.h
// leave empty the #define SRMCUSPEEDMONITOR__STOP_MOTOR()
//*******************************************************************//


#endif



// PROJECT SPECIFIC: Windy sBPM Nucleus and PLT2.5 support  - end //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////// PROJECT SPECIFIC PARAMETERS - begin /////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////
//                         BOARD DEPENDENT BASE QUANTITIES                            //
////////////////////////////////////////////////////////////////////////////////////////
#if(WINDY_STRIP_BOARD == 1)
#elif(WINDY_INTERNATION_BOARD == 1)
    #define MCI_MEASURE_INVERTER_TEMP
  	#define MODULE_TEMP_TO_STOP     			110                 //!< [degC] maximum inverter temperature to disable motor driving
    #define MODULE_TEMP_TO_START     			88                  //!< [degC] minimum inverter temperature to enable motor driving

#else
    #error "Need to define a board"
#endif



////////////////////////////////////////////////////////////////////////////////////////
//                                SPEED LIMITS PARAMETERS                             //
////////////////////////////////////////////////////////////////////////////////////////
#define MAX_SPEED_RPM                       19800               //!< [rpm] Define the maximum allowed speed
#define MIN_SPEED_RPM                       100                 //!< [rpm] Define the minimum allowed speed


////////////////////////////////////////////////////////////////////////////////////////
//                         SPEED REFERENCE PARAMETERS                                 //
////////////////////////////////////////////////////////////////////////////////////////
#define MAX_RPM_PER_SEC_ACCEL               4000                //!< [rpm/s] Maximum allowed acceleration
#define MAX_RPM_PER_SEC_DECEL               4000                //!< [rpm/s] Maximum allowed deceleration
#define MIN_RPM_PER_SEC_ACCEL               1                   //!< [rpm/s] Minimum allowed acceleration
#define MIN_RPM_PER_SEC_DECEL               1                   //!< [rpm/s] Minimum allowed deceleration
#define STARTING_ACCEL_VALUE                75.0f               //!< [rpm/s] Hard-coded acceleration from 0 to MIN_SPEED_FOR_FIXED_ACCEL rpm
#define MIN_SPEED_FOR_FIXED_ACCEL_VALUE     0.0f                //!< [rpm]   Speed threshold to apply starting acceleration (0.0f --> means not used)

////////////////////////////////////////////////////////////////////////////////////////
//                          CURRENT AND VOLTAGE LIMITS                                //
////////////////////////////////////////////////////////////////////////////////////////
#define MAXIMAL_MOTOR_CURRENT_ALLOWED       8.1f                //!< [A] Maximum allowed motor phase current
#define MINIMAL_VOLTAGE_RUN_MOTOR           140.0f              //!< [V] Minimal voltage which the motor will run
#define MIN_BUS_IDLE_VOLTAGE                100.0f              //!< [V] Minimal voltage which the motor return from failure to run
#define MAXIMAL_BUS_VOLTAGE_ALLOWED         430.0f              //!< [V] Maximum allowed bus voltage
#define MAXIMAL_IDLE_BUS_VOLTAGE            374.0f              //!< [V] Maximum idle bus voltage considering 265V AC mains

///////////////////////////// PROJECT SPECIFIC PARAMETERS - end /////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////






////////////////////////////////////////////////////////////////////////////////////////
//                         SWITCHING FREQUENCY AND CALL RATE                          //
////////////////////////////////////////////////////////////////////////////////////////
#define FS                                  16000.0f            //!< [Hz] PWM Frequency and ISR call rate
#define TS                                  0.000125f           //!< [s]  Control Call Rate Time

////////////////////////////////////////////////////////////////////////////////////////
//                         BOARD DEPENDENT BASES QUANTITIES                           //
////////////////////////////////////////////////////////////////////////////////////////
#define BASE_VOLTAGE                        (BASE_VOLTAGE_BUS/1.7320508075688772935274463415059f)  // < [V-phase] Base Phase Voltage
#define BASE_Z				                (float)(BASE_VOLTAGE/BASE_CURRENT)                     //!< [Ohm]     Base Resistance


////////////////////////////////////////////////////////////////////////////////////////
//                         SPEED REFERENCE PARAMETERS                                 //
////////////////////////////////////////////////////////////////////////////////////////
#define SPEEDREF__TS                        0.000125f     // [s] Speed reference generator call rate
#define SPEEDREF__MAX_RPM_PER_SEC_ACCEL     (float32) MAX_RPM_PER_SEC_ACCEL * SPEEDREF__TS * 2.0f * PI / 60.0f
#define SPEEDREF__MAX_RPM_PER_SEC_DECEL     (float32) MAX_RPM_PER_SEC_DECEL * SPEEDREF__TS * 2.0f * PI / 60.0f
#define SPEEDREF__MIN_RPM_PER_SEC_ACCEL     (float32) MIN_RPM_PER_SEC_ACCEL * SPEEDREF__TS * 2.0f * PI / 60.0f
#define SPEEDREF__MIN_RPM_PER_SEC_DECEL     (float32) MIN_RPM_PER_SEC_DECEL * SPEEDREF__TS * 2.0f * PI / 60.0f
// starting acceleration
#define SPEEDREF__STARTING_ACCEL            STARTING_ACCEL_VALUE * SPEEDREF__TS * 2.0f * PI / 60.0f
#define SPEEDREF__MIN_SPEED_FOR_FIXED_ACCEL MIN_SPEED_FOR_FIXED_ACCEL_VALUE * 2.0f * PI / 60.0f



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
//                      ADDITIONAL COMPONENTS AND FEATURES                            //
////////////////////////////////////////////////////////////////////////////////////////
#define SURGE_RELAY_USED                            //!< uncomment To use SURGE RELAY feature
#define MANUAL_INJECTION_FEATURE            ENABLED

///////////// FVT SECTION  -begin //////////////////////////
#define MCI_FVT_TESTS                       ENABLED // DISABLED
// Injection parameters for FVT
#define INJECTION_LEVEL                     0.6f        //! Amps
#define INJECTION_LEVEL_RATE                1.0f        //! Amps per second
#define INJECTION_LEVEL_POSITION            90.0f       //! Electrical Degrees
// Translating into MCLFVT notation
#define MCLFVT_INJECTION_LEVEL              (float32)(INJECTION_LEVEL);
#define MCLFVT_INJECTION_LEVEL_RATE         (float32)(INJECTION_LEVEL_RATE * TS);
#define MCLFVT_INJECTION_POSITION           (float32)(INJECTION_LEVEL_POSITION /180.0f * PI);
#define MCLFVT_OC_VOLTAGE_LEVEL             -210.0f

#ifdef SURGE_RELAY_USED
//#define SURGE_RELAY_FVT       //! uncomment to test the surge relay during the fvt procedure
#endif
//#define OC_FVT                //! uncomment to test the overcurrent complete circuit during the fvt procedure
///////////////  FVT SECTION  -end //////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////
//                                MISCELLANEOUS DEFINES                               //
////////////////////////////////////////////////////////////////////////////////////////
#ifndef PI
    #define PI 3.1415926535897932384626433832795f
#endif

//! Converts a value into TRUE or FALSE; 0 is FALSE, otherwise is TRUE
#define GET_TRUE_FALSE(value)           (BOOL_TYPE)(((value) == 0) ? (FALSE) : (TRUE))





////////////////////////////////////////////////////////////////////////////////////////
//                                   DEBUG FEATURES                                   //
////////////////////////////////////////////////////////////////////////////////////////
//---------- BOARD DEBUGGING ------------//
#ifndef COMPILE_4_SIMULINK
#define BOARD_DEBUGGING_CALLBACKS   ENABLED
#else
#define BOARD_DEBUGGING_CALLBACKS   DISABLED
#endif

#if (BOARD_DEBUGGING_CALLBACKS == ENABLED)

    #include "BoardDebugging.h"

    // ---------------  DEBUGGING CALLBACKS ----------------//
    #define MCI_INITIALIZE_CALLBACK()       BoardDebugging__Initialize();
    #define MCI_END_PWM_CALLBACK()          BoardDebugging__PwmHandler();
    #define MCI_HALLS_CALLBACK()
    #define MCI_250US_CALLBACK()            BoardDebugging__250usHandler();
    #define MCI_1MS_CALLBACK()
    #define MCI_5MS_CALLBACK()
    #define MCI_25MS_CALLBACK()             BoardDebugging__25msHandler();


    #define RESET_FROM_TOOLS            ENABLED

    //---------- MASTER COMMMANDER ------------//
    #define DEBUG_MASTERCOMMANDER       ENABLED


    //---------- SPI and BEAGLE ------------//
    #define FEATURE_MCI_CONTROLS_SPI DISABLED//ENABLED
    #if (FEATURE_MCI_CONTROLS_SPI == ENABLED)
        #define USE_BEAGLE
    #endif

#endif


//--------------------- SIMULINK ------------//
#ifdef COMPILE_4_SIMULINK
    #undef USE_BEAGLE
    #define MCI_INTERNAL_PARAMS
    #define BOARD_DEBUGGING_CALLBACKS      DISABLED

    #define SKIP_RESISTANCE_ESTIMATION    // uncomment it skip the first resistance estimation alignment

    #define MCI_INITIALIZE_CALLBACK()
    #define MCI_END_PWM_CALLBACK()
    #define MCI_HALLS_CALLBACK()
    #define MCI_250US_CALLBACK()
    #define MCI_1MS_CALLBACK()
    #define MCI_5MS_CALLBACK()
    #define MCI_25MS_CALLBACK()
#endif



#endif // MCI_PRM_H_



