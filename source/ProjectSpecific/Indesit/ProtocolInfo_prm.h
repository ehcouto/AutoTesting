/**
 *  @file       ProtocolInfo_prm.h
 *
 *  @brief      Public Parameters for the ProtocolInfo module
 *
 *
 *  $Header: $
 *
 *  @copyright  Copyright 2016-$Date$. Whirlpool Corporation. All rights reserved - CONFIDENTIAL
 */
//---------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------
#ifndef PROTOCOLINFO_PRM_H_
#define PROTOCOLINFO_PRM_H_

#include "C_Extensions.h"
#include "Mci_prm.h"
//=====================================================================================================================
//-------------------------------------- PUBLIC (Extern Variables, Constants & Defines) -------------------------------
//=====================================================================================================================
// Possible values of uP_MODEL
#define uP_MODEL_DSC        8
#define uP_MODEL_RX         9
#define uP_MODEL_MK3        14
// Possible values of APPLIANCE_TYPE
#define APPLIANCE_TYPE_DISHWASHER   1
#define APPLIANCE_TYPE_WASHER       2

// Identification
#define uP_MODEL            uP_MODEL_MK3
#define uPManufacturer      0
#define BoardType           3
#define APPLIANCE_TYPE      APPLIANCE_TYPE_WASHER
#define ExtendedAddr        0
#define WriteBlockSize      0
#define IdentFormat         0
#define Alignment           1
#define ErrorChecking       1
#define ProjectCode         6

#define FW_VERSION_A        0x08
#define FW_VERSION_B        0x04

#if DEBUG_MASTERCOMMANDER == DISABLED
// production version
#define FW_VERSION_C        0x99 //to be written as 0x_dec
#define FW_VERSION_PROTOCOL 0x99 //to be written as 0x_hex
#else
// debug version
#define FW_VERSION_C        0x04 //to be written as 0x_dec
#define FW_VERSION_PROTOCOL 0x04 //to be written as 0x_hex
#endif

#define RamAddrHi           0x00
#define RamAddrLo           0x01
#define EEPAddrHi           0
#define EEPAddrLo           0

#define EEPSize             00



#if (APPLIANCE_TYPE == APPLIANCE_TYPE_WASHER)

#ifdef MOTOR_BPM_TYPE
#define MOTOR_SBPM_FLOAT

#ifdef MCI_INTERNAL_PARAMS
    #define INTERNAL_PARAMS
#endif


#define MINIMUM_MCISAFETY_PARAMS_NUM    (54-6+2) // - 6 byte for tacho + 2 byte for padding
#else
#define MINIMUM_MCISAFETY_PARAMS_NUM    (54)
#endif


#define SIZE_OF_INFO_BUFFER     (8*2)
#define SIZE_OF_COMMON_BUFFER   (9*2)

typedef struct
{
    // INFO_BUFFER_T - Update SIZE_OF_INFO_BUFFER when this section changed
    uint16 Safety_Cnt;
    uint16 Fault;
    sint16 Omega;
    uint16 Crc_Safety;
    sint16 Me_Mean;
    uint16 Balance;
    uint16 Fdb_Flags;
    uint16 Safety_Flags;

    // COMMON_BUFFER_T - Update SIZE_OF_COMMON_BUFFER when this section changed
    uint16 Active_Power;
    uint16 Total_Energy_Hi;
    uint16 Total_Energy_Lo;
    sint16 OH_Spinning_Index;
    sint16 OH_Washing_Index;
    sint16 OH_Motor_Index;
    sint16 Me_Mean_Window;
    sint16 Balance_Window;
    uint16 Version;
}PROTOCOL_INFO_TYPE;

#elif (APPLIANCE_TYPE == APPLIANCE_TYPE_DISHWASHER)

#undef MOTOR_CIM_TYPE
#define MOTOR_BPM_TYPE

#define MINIMUM_MCISAFETY_PARAMS_NUM    (18)

//TODO -> Move it in virtual sensor module
typedef enum VIRTUAL_SENSOR_STATE_MACHINE
{
	VUOTO_SICUREZZA 		= 0,	/* [ 0 ] */
	PIENO_RABBOCCO			= 1,	/* [ 1, pieno per rabbocco ] */
	PIENO_RESISTENZA		= 2,	/* [ 2, pieno per resistenza ] */
	VUOTO_FILTRI_SCHIUMA	= 3,	/* [ 3, vuoto dovuto a filtri o schiuma ] */
	INDEFINITO				= 4,	/* [ 4, stato indefinito <--> vel nulla] - 07.02.01 */
	BASSA_TENSIONE			= 5,	/* [ 5, tensione di bus non sufficiente per garantire una certa velocità] - 07.02.03 */
	DEFAULT					= 6,	/* [ 6, stato di default] - VS New Architecture - 07.02.05 */
	VS_DUMMY  				= 256
} VIRTUAL_SENSOR_STATE_MACHINE_ENUM_T;


#define SIZE_OF_INFO_BUFFER     (6*2)
#define SIZE_OF_COMMON_BUFFER   (4*2)

typedef struct
{
    // INFO_BUFFER_T - Update SIZE_OF_INFO_BUFFER when this section changed
	uint16 				Fault;
	sint16 				Omega;
	VIRTUAL_SENSOR_STATE_MACHINE_ENUM_T 	Virtual_Sensor_State_Machine;
	sint16				OH_Motor_Index;
	sint16				Mean;
	uint16				Fdb_Flags;

    // COMMON_BUFFER_T - Update SIZE_OF_COMMON_BUFFER when this section changed
	uint16				Active_Power;
	uint16				Total_Energy_Hi;
	uint16 				Total_Energy_Lo;
	sint16				Vdc_Max_Copy;
}PROTOCOL_INFO_TYPE;

#define SWITCH_PUMPS_RELAY_PORT            PORTE
#define SWITCH_PUMPS_RELAY_PIN             19
#endif
//=====================================================================================================================
//-------------------------------------- PUBLIC (Function Prototypes) -------------------------------------------------
//=====================================================================================================================

#endif // PROTOCOLINFO_PRM_H_
