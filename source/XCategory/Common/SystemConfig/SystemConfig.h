/**
 *  @file
 *
 *  @brief      Public declarations that define the type of system the micro is supporting.
 *
 *  @details    Add descriptions of all selectable features to the following list:
 *
 *  API005_CONTROL_CYCLES_FEATURE                   Allows API005 to control appliance cycles
 *  API005_USE_LOADS_AND_GIS_FEATURE                Allows API005 to set and get Loads and GIs through Hbl
 *  API005_USE_OBJECTS_AND_EVENTS_FEATURE           Allows API005 to set objects and get events through HblHmi
 *  API005LOWLEVEL_FEATURE                          Provides PC control of the board I/O
 *  API010_USES_FIXED_VARIABLES_FEATURE             Indicates that API010 uses the Variables module for fixed variables.
 *  API010POLLVAR_FEATURE                           Reports platform variables and memory mapped variables over WIN.
 *  API013_CLIENT_FEATURE                           Controls application-layer functions on remote boards.
 *  API013_SERVER_FEATURE                           Provides application-layer functions through communications.
 *  API018_CLIENT_FEATURE                           Requests fault information and can generate a fault.
 *  API018_SERVER_FEATURE                           Publishes fault information and stores it in nonvolatile memory.
 *  API019_BOARD_DATA_FEEDBACK                      Indicates that the board supports API019 board data feedback for displaying on an HMI.
 *  API220MOTORCONTROL_FEATURE                      Enables API220 functionality on motor control boards with MCI.
 *  API221_CLIENT_FEATURE                           Sends motor control messages and receives motor status messages.
 *  API221_SERVER_FEATURE                           Receives motor control messages and sends motor status messages.
 *  CALIBRATION_DATA_FEATURE                        Uses Calibration Data to calibrate some GIs on FVT/EOL using API019
 *  COMMUNICATION_MONITOR_FEATURE                   Monitors the Communication faults
 *  CONNECTIVITY_FEATURE                            Enables connectivity data model, API147, and API151.
 *  CONNECTIVITY_HMI_FEATURE                        Enables API144 and API141.  Requires CONNECTIVITY_FEATURE ENABLED.
 *  FAULT_MANAGEMENT_FEATURE                        Enables firmware that depends on the Fault module to track error conditions and report faults.
 *  HBLACU_ZEROCROSS_FEATURE                        Enables Zerocross handling on HblAcu (Default: Enabled with SYSTEM_ACU_HANDLING defined, Disabled for other cases)
 *  HBLHMI_LLI_INITIALIZATION_FEATURE               Enables the initialization of LLIs in the HblHmi module (Disabled for P1)
 *  HMI_ACUVARIABLE_MODULE_FEATURE                  Enables the module that declares ACU variables instances for a P3 HMI
 *  HMI_VARIABLE_FEATURE                            Defines if Variable module handles HmiVariables indexes
 *  NVDATA_STORE_STATIC_FEATURE                     Allows API019 to store static data on supported board types.
 *  NVDATA_STORE_APPLIANCE_AND_STATISTICS_FEATURE   Allows management of the Appliance and Statistics segment data using the NVData module
 *  RECEIVE_STATUS_VARIABLES_FEATURE                Allows status variables to be updated over API011
 *  SETTINGFILE_MASTER_FILE_FEATURE                 Indicates that the board contains the master setting file for the system.
 *  SETTINGFILE_SECTION_DOWNLOAD_FEATURE            Indicates that the board will download a section of the setting file from the Main ACU.
 *  SFUPDATER_ACU_SECTION_ID                        The SF_ID_TYPE for the ACU section of the setting file. For ACU Expansion boards.
 *  SFUPDATER_HMI_SECTION_ID                        The SF_ID_TYPE for the HMI section of the setting file. For HMI and HMI Expansion boards.
 *  SFUPDATER_MCU_SECTION_ID                        The SF_ID_TYPE for the MCU section of the setting file. For MUC boards.
 *  TIME_ESTIMATION_FEATURE                         Enables cycle time estimation -- provides an Estimated_Remaining_Time[] variable.
 *
 *  @copyright  Copyright 2016-$Date$. Whirlpool Corporation. All rights reserved - CONFIDENTIAL
 */
//---------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------
#ifndef _SYSTEMCONFIG_H_
#define _SYSTEMCONFIG_H_

#include "C_Extensions.h"
#include "SRSystemConfig.h"
#include "SystemConfig_prm.h"


#ifndef SYSTEM_ACU_HANDLING
    #error SYSTEM_ACU_HANDLING must be defined as either ENABLED or DISABLED in the SystemConfig_prm.h file.
#endif
#ifndef SYSTEM_HMI_HANDLING
    #error SYSTEM_HMI_HANDLING must be defined as either ENABLED or DISABLED in the SystemConfig_prm.h file.
#endif
#ifndef SYSTEM_ACU_EXPANSION_HANDLING
    #error SYSTEM_ACU_EXPANSION_HANDLING must be defined as either ENABLED or DISABLED in the SystemConfig_prm.h file.
#endif
#ifndef SYSTEM_HMI_EXPANSION_HANDLING
    #error SYSTEM_HMI_EXPANSION_HANDLING must be defined as either ENABLED or DISABLED in the SystemConfig_prm.h file.
#endif
#ifndef SYSTEM_MCU_HANDLING
    #error SYSTEM_MCU_HANDLING must be defined as either ENABLED or DISABLED in the SystemConfig_prm.h file.
#endif


//=====================================================================================================================
//-------------------------------------- PUBLIC (Extern Variables, Constants & Defines) -------------------------------
//=====================================================================================================================


//---------------------------------------------------------------------------------------------------------------------
// Public Declarations that are not dependent on the type of project (ACU, HMI, MCU, etc.)
//---------------------------------------------------------------------------------------------------------------------


// Enable/Disable communication features
#define API003DISC_FEATURE                              ENABLED
#define API009SYSTEM_FEATURE                            ENABLED
#define API019PRODUCTINFO_FEATURE                       ENABLED

// Enable/Disable other features
#ifndef CALIBRATION_DATA_FEATURE
    #define CALIBRATION_DATA_FEATURE                    DISABLED
#endif
#ifndef I2CMGR_FEATURE
    #define I2CMGR_FEATURE                              DISABLED
#endif


//---------------------------------------------------------------------------------------------------------------------
// Public Declarations that are dependent on the type of project (ACU, HMI, MCU, etc.)
//---------------------------------------------------------------------------------------------------------------------


#if (SYSTEM_ACU_HANDLING == ENABLED) && (SYSTEM_HMI_HANDLING == ENABLED)

    // The following configuration is for a board that is both the main ACU and the main HMI (P1).
    #if ((SYSTEM_ACU_EXPANSION_HANDLING == ENABLED) || (SYSTEM_HMI_EXPANSION_HANDLING == ENABLED))
        #error A P1 board cannot act as an expansion board.
    #endif
    #if (SYSTEM_MCU_HANDLING == ENABLED)
        #error A P1 board cannot act as an MCU board.
    #endif
    // Check for the following declarations below, if necessary.
    //      SYSTEM_EXPANSION_HANDLING may be ENABLED or DISABLED.

    // Enable/Disable communication features
    #define API005LOWLEVEL_FEATURE                          ENABLED
    #define API005_CONTROL_CYCLES_FEATURE                   ENABLED
    #define API005_USE_LOADS_AND_GIS_FEATURE                ENABLED
    #define API005_USE_OBJECTS_AND_EVENTS_FEATURE           ENABLED
    #define API007DATA_FEATURE                              ENABLED
    #define API010POLLVAR_FEATURE                           ENABLED
    #define API010_USES_FIXED_VARIABLES_FEATURE             ENABLED
    #define API011APPCTRL_FEATURE                           ENABLED
    #if (SYSTEM_EXPANSION_HANDLING == ENABLED)
        #define API012EXP_FEATURE                           ENABLED
    #else
        #define API012EXP_FEATURE                           DISABLED
    #endif
    #ifndef API013_CLIENT_FEATURE
        #define API013_CLIENT_FEATURE                       DISABLED
    #endif
    #define API013_SERVER_FEATURE                           DISABLED
    #ifndef API017TOUCH_FEATURE
        #define API017TOUCH_FEATURE                         DISABLED
    #endif
    #define API018_CLIENT_FEATURE                           ENABLED
    #define API018_SERVER_FEATURE                           ENABLED
    #ifndef API019_BOARD_DATA_FEEDBACK
        #define API019_BOARD_DATA_FEEDBACK                  DISABLED
    #endif
    #define API220MOTORCONTROL_FEATURE                      DISABLED
    #ifndef API221_CLIENT_FEATURE
        #define API221_CLIENT_FEATURE                       DISABLED
    #endif
    #define API221_SERVER_FEATURE                           DISABLED
    #ifndef COMMUNICATION_MONITOR_FEATURE
        #define COMMUNICATION_MONITOR_FEATURE               DISABLED
    #endif
    #ifndef CONNECTIVITY_HMI_FEATURE
        #define CONNECTIVITY_HMI_FEATURE                    DISABLED
    #endif
    #ifndef CONNECTIVITY_FEATURE
        #define CONNECTIVITY_FEATURE                        CONNECTIVITY_HMI_FEATURE
    #endif

    // Enable/Disable other features
    #define FAULT_MANAGEMENT_FEATURE                        ENABLED
    #ifndef HBLACU_ZEROCROSS_FEATURE
        #define HBLACU_ZEROCROSS_FEATURE                    ENABLED
    #endif
    #define HBLHMI_LLI_INITIALIZATION_FEATURE               DISABLED
    #define HMI_ACUVARIABLE_MODULE_FEATURE                  DISABLED
    #ifndef HMI_VARIABLE_FEATURE
        #define HMI_VARIABLE_FEATURE                        ENABLED
    #endif
    #define NVDATA_STORE_STATIC_FEATURE                     ENABLED
    #define NVDATA_STORE_APPLIANCE_AND_STATISTICS_FEATURE   ENABLED
    #define RECEIVE_STATUS_VARIABLES_FEATURE                DISABLED
    #define SETTINGFILE_MASTER_FILE_FEATURE                 ENABLED
    #ifndef TIME_ESTIMATION_FEATURE
        #define TIME_ESTIMATION_FEATURE                     DISABLED
    #endif


#elif (SYSTEM_ACU_HANDLING == ENABLED)

    // The following configuration is for a board that is the main ACU, but not the main HMI.
    #if (SYSTEM_ACU_EXPANSION_HANDLING == ENABLED)
        #error The main ACU board cannot act as an ACU expansion board.
    #endif
    #if (SYSTEM_MCU_HANDLING == ENABLED)
        #error The main ACU board cannot act as an MCU board.
    #endif
    // Check for the following declarations below, if necessary.
    //      SYSTEM_EXPANSION_HANDLING may be ENABLED or DISABLED.
    //      SYSTEM_HMI_EXPANSION_HANDLING may be ENABLED or DISABLED.

    // Enable/Disable communication features
    #define API005LOWLEVEL_FEATURE                          ENABLED
    #define API005_CONTROL_CYCLES_FEATURE                   ENABLED
    #define API005_USE_LOADS_AND_GIS_FEATURE                ENABLED
    #define API005_USE_OBJECTS_AND_EVENTS_FEATURE           DISABLED
    #define API007DATA_FEATURE                              ENABLED
    #define API010POLLVAR_FEATURE                           ENABLED
    #define API010_USES_FIXED_VARIABLES_FEATURE             ENABLED
    #define API011APPCTRL_FEATURE                           ENABLED
    #if (SYSTEM_EXPANSION_HANDLING == ENABLED)
        #define API012EXP_FEATURE                           ENABLED
    #else
        #define API012EXP_FEATURE                           DISABLED
    #endif
    #ifndef API013_CLIENT_FEATURE
        #define API013_CLIENT_FEATURE                       DISABLED
    #endif
    #define API013_SERVER_FEATURE                           DISABLED
    #define API017TOUCH_FEATURE                             DISABLED
    #define API018_CLIENT_FEATURE                           DISABLED
    #define API018_SERVER_FEATURE                           ENABLED
    #define API019_BOARD_DATA_FEEDBACK                      DISABLED
    #define API220MOTORCONTROL_FEATURE                      DISABLED
    #ifndef API221_CLIENT_FEATURE
        #define API221_CLIENT_FEATURE                       DISABLED
    #endif
    #define API221_SERVER_FEATURE                           DISABLED
    #define COMMUNICATION_MONITOR_FEATURE                   ENABLED
    #ifndef CONNECTIVITY_FEATURE
        #define CONNECTIVITY_FEATURE                        DISABLED
    #endif
    #define CONNECTIVITY_HMI_FEATURE                        DISABLED

    // Enable/Disable other features
    #define FAULT_MANAGEMENT_FEATURE                        ENABLED
    #ifndef HBLACU_ZEROCROSS_FEATURE
        #define HBLACU_ZEROCROSS_FEATURE                    ENABLED
    #endif
    #define HMI_ACUVARIABLE_MODULE_FEATURE                  DISABLED
    #define HMI_VARIABLE_FEATURE                            DISABLED
    #define NVDATA_STORE_STATIC_FEATURE                     ENABLED
    #define NVDATA_STORE_APPLIANCE_AND_STATISTICS_FEATURE   ENABLED
    #define RECEIVE_STATUS_VARIABLES_FEATURE                DISABLED
    #define SETTINGFILE_MASTER_FILE_FEATURE                 ENABLED
    #ifndef TIME_ESTIMATION_FEATURE
        #define TIME_ESTIMATION_FEATURE                     DISABLED
    #endif



#elif (SYSTEM_HMI_HANDLING == ENABLED)

    // The following configuration is for a board that is the main HMI, but not the main ACU.
    #if (SYSTEM_HMI_EXPANSION_HANDLING == ENABLED)
        #error The main HMI board cannot act as an HMI expansion board.
    #endif
    #if (SYSTEM_MCU_HANDLING == ENABLED)
        #error The main HMI board cannot act as an MCU board.
    #endif
    // Check for the following declarations below, if necessary.
    //      SYSTEM_EXPANSION_HANDLING may be ENABLED or DISABLED.
    //      SYSTEM_ACU_EXPANSION_HANDLING may be ENABLED or DISABLED.


    // Enable/Disable communication features
    #define API005LOWLEVEL_FEATURE                          ENABLED
    #define API005_CONTROL_CYCLES_FEATURE                   DISABLED
    #if (SYSTEM_ACU_EXPANSION_HANDLING == ENABLED)
        #define API005_USE_LOADS_AND_GIS_FEATURE            ENABLED
    #else
        #ifndef API005_USE_LOADS_AND_GIS_FEATURE
            #define API005_USE_LOADS_AND_GIS_FEATURE        DISABLED
        #endif
    #endif
    #define API005_USE_OBJECTS_AND_EVENTS_FEATURE           ENABLED
    #define API007DATA_FEATURE                              ENABLED
    #define API010POLLVAR_FEATURE                           ENABLED
    #define API010_USES_FIXED_VARIABLES_FEATURE             ENABLED
    #define API011APPCTRL_FEATURE                           ENABLED
    #if (SYSTEM_EXPANSION_HANDLING == ENABLED)
        #define API012EXP_FEATURE                           ENABLED
    #else
        #define API012EXP_FEATURE                           DISABLED
    #endif
    #define API013_CLIENT_FEATURE                           DISABLED
    #ifndef API013_SERVER_FEATURE
        #define API013_SERVER_FEATURE                       DISABLED
    #endif
    #ifndef API017TOUCH_FEATURE
        #define API017TOUCH_FEATURE                         DISABLED
    #endif
    #define API018_CLIENT_FEATURE                           ENABLED
    #define API018_SERVER_FEATURE                           DISABLED
    #ifndef API019_BOARD_DATA_FEEDBACK
        #define API019_BOARD_DATA_FEEDBACK                  DISABLED
    #endif
    #define API220MOTORCONTROL_FEATURE                      DISABLED
    #define API221_CLIENT_FEATURE                           DISABLED
    #define API221_SERVER_FEATURE                           DISABLED
    #define COMMUNICATION_MONITOR_FEATURE                   ENABLED
    #ifndef CONNECTIVITY_HMI_FEATURE
        #define CONNECTIVITY_HMI_FEATURE                    DISABLED
    #endif
    #ifndef CONNECTIVITY_FEATURE
        #define CONNECTIVITY_FEATURE                        CONNECTIVITY_HMI_FEATURE
    #endif

    // Enable/Disable other features
    #define FAULT_MANAGEMENT_FEATURE                        DISABLED
    #ifndef HBLACU_ZEROCROSS_FEATURE
        #define HBLACU_ZEROCROSS_FEATURE                    DISABLED
    #endif
    #define HBLHMI_LLI_INITIALIZATION_FEATURE               ENABLED
    #define HMI_ACUVARIABLE_MODULE_FEATURE                  ENABLED
    #define HMI_VARIABLE_FEATURE                            ENABLED
    #define NVDATA_STORE_STATIC_FEATURE                     ENABLED
    #define NVDATA_STORE_APPLIANCE_AND_STATISTICS_FEATURE   DISABLED
    #define RECEIVE_STATUS_VARIABLES_FEATURE                ENABLED
    #define SETTINGFILE_MASTER_FILE_FEATURE                 DISABLED
    #define SFUPDATER_HMI_SECTION_ID                        SF_ID_HMI
    #define TIME_ESTIMATION_FEATURE                         DISABLED



#elif (SYSTEM_MCU_HANDLING == ENABLED)

    // The following configuration is for an MCU board.

    // Enable/Disable communication features
    #define API005LOWLEVEL_FEATURE                          DISABLED
    #define API005_CONTROL_CYCLES_FEATURE                   DISABLED
    #define API005_USE_LOADS_AND_GIS_FEATURE                DISABLED
    #define API005_USE_OBJECTS_AND_EVENTS_FEATURE           DISABLED
    #ifndef API007DATA_FEATURE
        #define API007DATA_FEATURE                          ENABLED
    #endif
    #ifndef API010POLLVAR_FEATURE
        #define API010POLLVAR_FEATURE                       ENABLED
    #endif
    #ifndef API010_USES_FIXED_VARIABLES_FEATURE
        #define API010_USES_FIXED_VARIABLES_FEATURE         DISABLED
    #endif
    #ifndef API011APPCTRL_FEATURE
        #define API011APPCTRL_FEATURE                       DISABLED
    #endif
    #define API012EXP_FEATURE                               DISABLED
    #define API013_CLIENT_FEATURE                           DISABLED
    #ifndef API013_SERVER_FEATURE
        #define API013_SERVER_FEATURE                       DISABLED
    #endif
    #define API017TOUCH_FEATURE                             DISABLED
    #ifndef API018_CLIENT_FEATURE
        #define API018_CLIENT_FEATURE                       DISABLED
    #endif
    #define API018_SERVER_FEATURE                           DISABLED
    #define API019_BOARD_DATA_FEEDBACK                      DISABLED
    #ifndef API220MOTORCONTROL_FEATURE
        #define API220MOTORCONTROL_FEATURE                  DISABLED
    #endif
    #define API221_CLIENT_FEATURE                           DISABLED
    #ifndef API221_SERVER_FEATURE
        #define API221_SERVER_FEATURE                       ENABLED
    #endif
    #define COMMUNICATION_MONITOR_FEATURE                   DISABLED
    #ifndef CONNECTIVITY_FEATURE
        #define CONNECTIVITY_FEATURE                        DISABLED
    #endif
    #define CONNECTIVITY_HMI_FEATURE                        DISABLED

    // Enable/Disable other features
    #define FAULT_MANAGEMENT_FEATURE                        DISABLED
    #ifndef HBLACU_ZEROCROSS_FEATURE
        #define HBLACU_ZEROCROSS_FEATURE                    DISABLED
    #endif
    #define HMI_ACUVARIABLE_MODULE_FEATURE                  DISABLED
    #define HMI_VARIABLE_FEATURE                            DISABLED
    #ifndef NVDATA_STORE_STATIC_FEATURE
        #define NVDATA_STORE_STATIC_FEATURE                 DISABLED
    #endif
    #define NVDATA_STORE_APPLIANCE_AND_STATISTICS_FEATURE   DISABLED
    #define RECEIVE_STATUS_VARIABLES_FEATURE                ENABLED
    #define SETTINGFILE_MASTER_FILE_FEATURE                 DISABLED
    #define SFUPDATER_MCU_SECTION_ID                        SF_ID_MCU
    #define TIME_ESTIMATION_FEATURE                         DISABLED



#elif ((SYSTEM_ACU_EXPANSION_HANDLING == ENABLED) && (SYSTEM_HMI_EXPANSION_HANDLING == ENABLED))

    // The following configuration is for a board that expands both the main ACU and the main HMI.
    #if (SYSTEM_EXPANSION_HANDLING == ENABLED)
        #error An expansion board cannot control other expansion boards directly.
    #endif


    // TODO: Fill this section when we have an example board.
    // Enable/Disable communication features

    // Enable/Disable other features



#elif (SYSTEM_ACU_EXPANSION_HANDLING == ENABLED)

    // The following configuration is for an ACU expansion board that is not the main HMI.
    #if (SYSTEM_EXPANSION_HANDLING == ENABLED)
        #error An expansion board cannot control other expansion boards directly.
    #endif

    // Enable/Disable communication features
    #define API005LOWLEVEL_FEATURE                          ENABLED
    #define API005_CONTROL_CYCLES_FEATURE                   DISABLED
    #define API005_USE_LOADS_AND_GIS_FEATURE                ENABLED
    #define API005_USE_OBJECTS_AND_EVENTS_FEATURE           DISABLED
    #define API007DATA_FEATURE                              ENABLED
    #ifndef API010POLLVAR_FEATURE
        #define API010POLLVAR_FEATURE                       ENABLED
    #endif
    #ifndef API010_USES_FIXED_VARIABLES_FEATURE
        #define API010_USES_FIXED_VARIABLES_FEATURE         DISABLED
    #endif
    #ifndef API011APPCTRL_FEATURE
        #define API011APPCTRL_FEATURE                       DISABLED
    #endif
    #define API012EXP_FEATURE                               ENABLED
    #define API013_CLIENT_FEATURE                           DISABLED
    #ifndef API013_SERVER_FEATURE
        #define API013_SERVER_FEATURE                       DISABLED
    #endif
    #define API017TOUCH_FEATURE                             DISABLED
    #ifndef API018_CLIENT_FEATURE
        #define API018_CLIENT_FEATURE                       DISABLED
    #endif
    #define API018_SERVER_FEATURE                           DISABLED
    #define API019_BOARD_DATA_FEEDBACK                      DISABLED
    #define API220MOTORCONTROL_FEATURE                      DISABLED
    #define API221_CLIENT_FEATURE                           DISABLED
    #define API221_SERVER_FEATURE                           DISABLED
    #define COMMUNICATION_MONITOR_FEATURE                   DISABLED
    #ifndef CONNECTIVITY_FEATURE
        #define CONNECTIVITY_FEATURE                        DISABLED
    #endif
    #define CONNECTIVITY_HMI_FEATURE                        DISABLED

    // Enable/Disable other features
    #define FAULT_MANAGEMENT_FEATURE                        DISABLED
    #ifndef HBLACU_ZEROCROSS_FEATURE
        #define HBLACU_ZEROCROSS_FEATURE                    DISABLED
    #endif
    #define HMI_ACUVARIABLE_MODULE_FEATURE                  DISABLED
    #define HMI_VARIABLE_FEATURE                            DISABLED
    #define NVDATA_STORE_STATIC_FEATURE                     ENABLED
    #define NVDATA_STORE_APPLIANCE_AND_STATISTICS_FEATURE   DISABLED
    #define RECEIVE_STATUS_VARIABLES_FEATURE                DISABLED
    #define SETTINGFILE_MASTER_FILE_FEATURE                 DISABLED
    #define TIME_ESTIMATION_FEATURE                         DISABLED



#elif (SYSTEM_HMI_EXPANSION_HANDLING == ENABLED)

    // The following configuration is for an HMI expansion board that is not the main ACU.
    #if (SYSTEM_EXPANSION_HANDLING == ENABLED)
        #error An expansion board cannot control other expansion boards directly.
    #endif


    // Enable/Disable communication features
    #define API005LOWLEVEL_FEATURE                          ENABLED
    #define API005_CONTROL_CYCLES_FEATURE                   DISABLED


    #define API005_USE_LOADS_AND_GIS_FEATURE                DISABLED


    #define API005_USE_OBJECTS_AND_EVENTS_FEATURE           ENABLED
    #define API007DATA_FEATURE                              ENABLED
    #define API010POLLVAR_FEATURE                           ENABLED
    #define API010_USES_FIXED_VARIABLES_FEATURE             ENABLED
    #define API011APPCTRL_FEATURE                           ENABLED

    #define API012EXP_FEATURE                               ENABLED

    #define API013_CLIENT_FEATURE                           DISABLED
    #ifndef API013_SERVER_FEATURE
        #define API013_SERVER_FEATURE                       DISABLED
    #endif
    #ifndef API017TOUCH_FEATURE
        #define API017TOUCH_FEATURE                         DISABLED
    #endif
    #define API018_CLIENT_FEATURE                           ENABLED
    #define API018_SERVER_FEATURE                           DISABLED
	#ifndef API019_BOARD_DATA_FEEDBACK
        #define API019_BOARD_DATA_FEEDBACK                  DISABLED
    #endif
    #define API220MOTORCONTROL_FEATURE                      DISABLED
    #define API221_CLIENT_FEATURE                           DISABLED
    #define API221_SERVER_FEATURE                           DISABLED
    #define COMMUNICATION_MONITOR_FEATURE                   DISABLED
    #ifndef CONNECTIVITY_HMI_FEATURE
        #define CONNECTIVITY_HMI_FEATURE                    DISABLED
    #endif
    #ifndef CONNECTIVITY_FEATURE
        #define CONNECTIVITY_FEATURE                        CONNECTIVITY_HMI_FEATURE
    #endif

    // Enable/Disable other features
    #define FAULT_MANAGEMENT_FEATURE                        DISABLED
    #define HBLACU_ZEROCROSS_FEATURE                        DISABLED
    #define HBLHMI_LLI_INITIALIZATION_FEATURE               DISABLED
	#define HMI_ACUVARIABLE_MODULE_FEATURE                  DISABLED
    #define HMI_VARIABLE_FEATURE                            DISABLED
    #define NVDATA_STORE_STATIC_FEATURE                     ENABLED
    #define NVDATA_STORE_APPLIANCE_AND_STATISTICS_FEATURE   DISABLED
    #define RECEIVE_STATUS_VARIABLES_FEATURE                ENABLED
    #define SETTINGFILE_MASTER_FILE_FEATURE                 DISABLED
	#define TIME_ESTIMATION_FEATURE                         DISABLED

	// Check for necessary configurations.
	#ifndef SFUPDATER_HMI_SECTION_ID
		#error Must define HMI Expansion SF updater Section ID.
	#endif


#else

    // The following configuration is invalid. Missing SYSTEM_X_HANDLING macro declarations in SystemConfig_prm.h.
    #error The board type cannot be determined. You must declare the SYSTEM_X_HANDLING macros in SystemConfig_prm.h.



#endif      // SYSTEM_MCU_HANDLING


#if (defined(SFUPDATER_HMI_SECTION_ID) || defined(SFUPDATER_ACU_SECTION_ID) || defined(SFUPDATER_MCU_SECTION_ID))
    #define SETTINGFILE_SECTION_DOWNLOAD_FEATURE ENABLED
#else
    #define SETTINGFILE_SECTION_DOWNLOAD_FEATURE DISABLED
#endif


// CONNECTIVITY_HMI_FEATURE cannot be enabled if CONNECTIVITY_FEATURE is DISABLED.
CT_ASSERT((CONNECTIVITY_FEATURE == ENABLED) || (CONNECTIVITY_HMI_FEATURE == DISABLED));


#endif // _SYSTEMCONFIG_H_
