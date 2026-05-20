/**
 *  @file       CommandManager_prm.h
 *
 *  @brief      Private parameters for the CommandManager module
 *
 *
 *  $Header: $
 *
 *  @copyright  Copyright 2016-$Date$. Whirlpool Corporation. All rights reserved - CONFIDENTIAL
 */
//---------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------
#ifndef COMMANDMANAGER_PRM_H_
#define COMMANDMANAGER_PRM_H_

//=====================================================================================================================
//-------------------------------------- PUBLIC (Extern Variables, Constants & Defines) -------------------------------
//=====================================================================================================================
#include "C_Extensions.h"
#include "Mci.h"



#if (MCI_FVT_TESTS == ENABLED)

extern MCI_FVT_TEST_RESULT_TYPE FvtResult;
#endif


#define INDESIT_LEGACY

#ifndef INDESIT_LEGACY

#define APPLIANCE_TYPE_DISHWASHER   1
#define APPLIANCE_TYPE_WASHER       2

#define NACK 				1
#define APPLIANCE_TYPE      APPLIANCE_TYPE_WASHER


#else
#include "ProtocolMainExternal.h"
#include "FaultManager.h"
#endif



typedef struct
{
    sint8 Integ_Gain;
    sint8 Prop_Gain;
}SHIFT_CONTROLLER_COMMAND_PARAMS_TYPE;

#if (APPLIANCE_TYPE == APPLIANCE_TYPE_WASHER)
typedef struct
{
    sint16 Transmission_Ratio;
    uint16 Free_Down_Gain;

    sint16 Max_Allowed_Speed;

    // Threshold speed
    sint16 Low_Regulators_Thr_Speed;
    sint16 Med_Regulators_Thr_Speed;
    sint16 High_Regulators_Thr_Speed;


    // Ramp Limits
    uint16 Max_Deceleration_Low_Speed;
    uint16 Max_Deceleration_High_Speed;

    uint16 Max_Acceleration_Low_Speed;
    uint16 Max_Acceleration_High_Speed;

}COMMAND_PARAMS_TYPE;

#else

typedef struct
{
	uint16 Params_Crc;

	sint16 Transmission_Ratio;
	uint16 Free_Down_Gain;

	sint16 Max_Allowed_Speed;
	sint16 Min_Allowed_Speed;

	// Ramp Limits
	uint16 Max_Deceleration_Low_Speed;
	uint16 Max_Deceleration_High_Speed;

	uint16 Max_Acceleration_Low_Speed;
	uint16 Max_Acceleration_High_Speed;

}COMMAND_PARAMS_TYPE;
#endif

#define POWERON_FREEDOWN_INIT       1000    // corresponds to 2 sec @500Hz Slow Control Loop
#define MAX_DISTR_SPEED             127     // rpm drum   Attention - main uC sends this value in rpm!!!!

#define HIGH_SPEED_THRESHOLD        (sint32)(0.45f * 19800.0f)
#define HIGH_SPEED_THRESHOLD_DEC	4500   // threshold speed for deceleration ramp limitation

#define MIN_ACCELERATION_DRUM   	25 // min acceleration rpm_drum/s at startup
#define STOP_RAMP_DELTA_SPEED       5 // the ramp is stopped to the actual speed + delta speed to avoid ramp down generation

#endif // COMMANDMANAGER_PRV_H_
