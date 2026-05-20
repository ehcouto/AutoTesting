/**
 *  @file
 *  @brief      Params
 *
 *
 *  @copyright  Copyright 2016. Whirlpool Corporation. All rights reserved - CONFIDENTIAL
 */
//---------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------
#ifndef PARAMS_H_
#define PARAMS_H_

#include "Params_prm.h"
#include "Mcl.h"
#include "SRMotorSafetyMgr_prv.h"
#include "SettingFile.h"
//=====================================================================================================================
//-------------------------------------- PUBLIC (Extern Variables, Constants & Defines) -------------------------------
//=====================================================================================================================
//! Definition of the data structure for a displacement in the setting file
typedef struct
{
    uint8* Data;                                   // Pointer to the displacement data.
    uint16 Length;                                  // Length of displacement data in bytes.
} PARAMS_LOADER_TYPE;



//This STRUCTURE IS TO BE BACKWARD COMPATIBLE WITH OLD sBPM EOL TEST (Max Acceleration Unlock)
typedef struct
{
    uint16 Dummy_Start[4];

    uint16 TransmissionRatio;

    uint16 Dummy_Middle_1[20-5];

    uint16 Base_Voltage;
    uint16 Base_Bus_Voltage;
    uint16 Base_Current;
    uint16 Base_Torque;
    uint16 Base_Speed;
    uint16 Base_OverHeating;
    uint16 Base_Bemf;

    uint16 Dummy_Middle_2[154-27];

    /* Max Acceleration */
    uint16 u16MaxDeltaSpeedSetPoint_Hi;
    uint16 u16MaxDeltaSpeedSetPoint_Lo;
    uint16 u16MaxDeltaSpeedSetPointHighSpeed_Hi;
    uint16 u16MaxDeltaSpeedSetPointHighSpeed_Lo;

    /* Max Deceleration */
    uint16 u16MaxDeltaSpeedSetPoint_Dec_Hi;
    uint16 u16MaxDeltaSpeedSetPoint_Dec_Lo;
    uint16 u16MaxDeltaSpeedSetPointHighSpeed_Dec_Hi;
    uint16 u16MaxDeltaSpeedSetPointHighSpeed_Dec_Lo;
    /************ DYNAMIC TORQUE TABLE PARAMETERS - END *************/

    uint16 Dummy_End[4];

}SF_APPLICATION_PARAMS_TYPE;

extern SF_APPLICATION_PARAMS_TYPE SF_Application_Params; //To be backward compatible with EOL -> Max acceleration key unlock.

extern BOOL_TYPE Parms_Ready;

extern uint8 Mci_ClassA_Dtc[sizeof(MCL_PARAMS_DISPL1_TYPE)];
extern uint8 Mci_ClassA_Motor_Lut[sizeof(MCL_PARAMS_DISPL3_TYPE)];
extern uint8 Mci_ClassA_Speed_Ctrl[sizeof(MCL_PARAMS_DISPL4_TYPE)];
extern uint8 Mci_ClassA_Mtpa[sizeof(MCL_PARAMS_DISPL9_TYPE)];
extern uint8 Mci_ClassA_Mtpv[sizeof(MCL_PARAMS_DISPL10_TYPE)];
extern uint8 Mci_ClassA_Ote[51*4];

extern uint8 Mci_ClassB_Prm[SAFETY_MOTOR_PARAMETERS_NUM];

extern uint8 Mca_Sensor_Wm[6*2];
extern uint8 Mca_Set_Wm[70*2];

//=====================================================================================================================
//-------------------------------------- PUBLIC (Function Prototypes) -------------------------------------------------
//=====================================================================================================================
void Params__Initialize(void);
void Params__SlowHandler(void);
PASS_FAIL_TYPE Params__BasicLoader(SETTINGFILE_OFFSET_TYPE pointer_id, uint16 displacement, PARAMS_LOADER_TYPE * sf_data_ptr);

#define Params__IsReady()       Parms_Ready
#define Params__SetReady(is_ready)  {Parms_Ready = (BOOL_TYPE)is_ready;}

#endif // PARAMS_H_
