/**
 *  @file       CommandManager.c
 *
 *  @brief      Decoding and execution of commands coming from the protocol
 *
 *  $Header: $
 *
 *  @copyright  Copyright 2016-$Date$. Whirlpool Corporation. All rights reserved - CONFIDENTIAL
 */
//---------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------


//-------------------------------------- Include Files ----------------------------------------------------------------
#include "CommandManager.h"
#include "CommandManager_prv.h"
#include "MciSetWm.h"
#include "MciSensorsWm.h"
#include "Mci.h"
#include "IQMath.h"
#include "MathCalc.h"
#include "Params.h"

//-------------------------------------- PUBLIC (Variables) -----------------------------------------------------------

//-------------------------------------- PRIVATE (Variables, Constants & Defines) -------------------------------------
//! movement state machine definition
typedef enum MOVEMENT_STATE_MACHINE
{
    PAUSE_STATE                     = 0,    //!< 0
    STEADY_STATE                    = 1,    //!< 1, steady-state: Platform_Final_Speed == actual speed
    CONTROLLED_RAMP_STATE           = 2,    //!< 2, closed loop ramp-up or controlled ramp-down
	CONTROLLED_RAMP_TO_ZERO_STATE   = 3,    //!< 3, controlled ramp down to zero in case of Platform_Final_Speed == 0 or speed inversion requested
	FREE_RAMP_DOWN_TO_ZERO_STATE    = 4,    //!< 4, free ramp down to zero in case of Platform_Final_Speed == 0 or speed inversion requested
    FREE_RAMP_DOWN_STATE            = 5,    //!< 5, not controlled ramp down with |RampEndSpeed| <= MedReg_ThSpeed
    FVT_RUNNING_STATE               = 6,    //!< 6, FVT procedure in progress
    FVT_FINISHED_STATE              = 7,    //!< 7, FVT procedure finished
    FVT_ABORTED_STATE               = 8,    //!< 8, FVT procedure aborted
    MSM_DUMMY                       = 256
} MOVEMENT_STATE_MACHINE_ENUM_T;

//! master command definition
typedef enum DMC_RETURNS
{

    DMC_COMMAND_PAUSE           = 0,    //!< 0, Master Command Pause condition
    DMC_SPEED_INVERSION         = 1,    //!< 1, Speed inversion requested
    DMC_RAMP_UP                 = 2,    //!< 2, Ramp-up condition
    DMC_FREE_RAMP_DOWN          = 3,    //!< 3, not controlled Ramp-down
    DMC_CONTROLLED_RAMP_DOWN    = 4,    //!< 4, controlled Ramp-down
    DMC_STOP_RAMP               = 5,    //!< 5, stop ramp to spinning speed
    DMC_SKIP_COMMAND            = 6,    //!< 6, Skip Command
    DMC_NACK_CONDITION          = 7,    //!< 7, NACK Fault condition
    DMC_INVALID                 = 8,    //!< 8, invalid return condition
    DMC_START_FVT               = 9,    //!< 9, request for starting fvt
    DMC_STOP_FVT                = 10,   //!<10, request for stopping fvt
    DMC_DUMMY                   = 256
}DMC_RETURNS_E_T;

//! Define the main states of this controller
typedef enum
{
    MCI_NOT_DEFINED = 0,                            //!< 0  - reserved 0 index - shall not be used
    MCI_FVT_MODE,                                   //!< 1  - Function Verification Test (FVT) Mode.
    MCI_INITIALIZING,                               //!< 2  - initializing state
    MCI_IDLE,                                       //!< 3  - idling
    MCI_CLASS_B_STARTING,                           //!< 4  - hardware starting, checking and power up
    MCI_CLASS_B_STOPPING,                           //!< 5  - stopping Class B, executes before idling
    MCI_FAILED,                                     //!< 6  - something has failed
    MCI_STARTING,                                   //!< 7  - starting the motor, executes before running
    MCI_RUNNING,                                    //!< 8  - running
    MCI_STOPPING,                                   //!< 9  - stopping the motor, executes before Class B stopping
    MCI_DEBUGGING,                                  //!< 10 - debugging, shall not be used for production
    MCI_MANUAL_CONTROL,                             //!< 11 - manual control of inverter bridge, used for AC or DC, voltage or current control
} COPY_OF_MCI_CONTROL_STATE_TYPE;

static BOOL_TYPE ParamsReady;
static MOVEMENT_STATE_MACHINE_ENUM_T Movement_State;
static BOOL_TYPE Command_Speed_Negative; //!< TRUE if negative

static uint16 Power_On_Free_Down_Cnt;
static DMC_RETURNS_E_T DMC_State;

static sint16 Platform_Final_Speed;      // [rpm_M]
static sint16 Ramp_End_Speed;
static sint16 Ramp_Time_Or_Acc;

static uint16 Free_Down_Cnt;
static uint32 Max_Acceleration;
static volatile uint32 Max_Deceleration;
static uint16 Max_Distr_Speed_Rpm_M;
static sint16 Min_Allowed_Speed;      	// [rpm_M]

#ifdef MOTOR_BPM_TYPE
static uint16 Ramp_Down_Flag;
#endif

#if MCI_FVT_TESTS == ENABLED
void ClearFvtResult(MCI_FVT_TEST_RESULT_TYPE *fvt_data_result);
MCI_FVT_TEST_RESULT_TYPE FvtResult;
#define FVT_TEST_FLAGS      0xFF
#endif

static COMMAND_PARAMS_TYPE *Command_Params;
static COMMAND_PARAMS_TYPE Command_Params_Ram;

static MCL_PARAMS_DISPL4_TYPE *Command_SpeedCntrl_Params;
static MCISENSORSWM_PARAMS_TYPE *Command_MciSensorsWm_Params;

static uint16 Fault;
PARAMS_LOADER_TYPE Command_Wm_Prm_Ram_1;
PARAMS_LOADER_TYPE Command_Wm_Prm_Ram_2;

//-------------------------------------- PRIVATE (Function Prototypes) ------------------------------------------------
static void ApplicationStateMachine(void);
void ExecuteMasterCommand(void);
void Stop_Motor(void);
uint16 Run_Motor(sint16 acceleration_limit);
//=====================================================================================================================
//-------------------------------------- Public Functions -------------------------------------------------------------
//=====================================================================================================================

//---------------------------------------------------------------------------------------------------------------------
/**
 *  @brief      It Initializes the module CommandManager and its variables.
 *
 */
void CommandManager__Initialize(void)
{
#ifdef MOTOR_CIM_TYPE
	MciSetWm__SoftStart(SOFT_START_INITIALIZATION);
#endif
	ParamsReady = FALSE;
    Free_Down_Cnt = 0;
    Ramp_End_Speed = 0;
    Movement_State = FREE_RAMP_DOWN_TO_ZERO_STATE;

    // Fictious Free_Down_Counter to avoid false PAUSE STATE
    Power_On_Free_Down_Cnt = POWERON_FREEDOWN_INIT;
    Command_Speed_Negative = FALSE;

    Fault = 0;

    // Stop the motor when command manager is initialize
    Mci__Stop(MOTOR0, 0);

    #if MCI_FVT_TESTS == ENABLED
    ClearFvtResult(&FvtResult);
    #endif
}




/**
 *  @brief      CommandManager 2ms handler.
 *
 */
void CommandManager__5msHandler(void)
{
    if (ParamsReady == TRUE)
    {
        ApplicationStateMachine();


        //DEBUG_FAGNAL
//
//        SF_Application_Params.TransmissionRatio = (uint16)((((uint32)Command_Params->Transmission_Ratio)<<15)/19800.0);
//        SF_Application_Params.Base_Voltage = 256;
//        SF_Application_Params.Base_Bus_Voltage = 444;
//        SF_Application_Params.Base_Current = 8220;
//        SF_Application_Params.Base_Torque = 40775;
//        SF_Application_Params.Base_Speed = 19800;
//        SF_Application_Params.Base_OverHeating = 200;
//        SF_Application_Params.Base_Bemf = 650;


      //  memcpy(SF_Application_Params.Dummy_Start, OldSettingFile, sizeof(OldSettingFile));

    }
    else
    {
#ifdef MCI_INTERNAL_PARAMS
        ParamsReady = TRUE;
        Command_Params = (COMMAND_PARAMS_TYPE *)&Command_Params_SF[0];

        Max_Acceleration = Command_Params->Max_Acceleration_Low_Speed;
        Max_Deceleration = Command_Params->Max_Deceleration_Low_Speed;

        Max_Distr_Speed_Rpm_M = (uint16)((MAX_DISTR_SPEED * Command_Params->Transmission_Ratio)>>8);

        Min_Allowed_Speed = 225; //[rpm-M]


#else
        if((Params__BasicLoader(SF_PTR_MCU_CLASS_A_MCI, SF_DISPL_MOTOR_SPEED_CTRL, &Command_Wm_Prm_Ram_1) == PASS)&& \
           (Params__BasicLoader(SF_PTR_MCU_MCA, SF_DISPL_MOTOR_MCI_SENSORS_WM, &Command_Wm_Prm_Ram_2) == PASS))
        {
            Command_SpeedCntrl_Params = (MCL_PARAMS_DISPL4_TYPE*)Command_Wm_Prm_Ram_1.Data;
            Command_MciSensorsWm_Params = (MCISENSORSWM_PARAMS_TYPE*)Command_Wm_Prm_Ram_2.Data;


            Command_Params = &Command_Params_Ram;

            Command_Params->Transmission_Ratio = (sint16)(Command_MciSensorsWm_Params->Transmission_Ratio*256.0f);
            Command_Params->Free_Down_Gain = 27300; //Slow Control Loop Frequency /(Threshold Wash * Transmission Ratio)  *32768
            Command_Params->Max_Allowed_Speed = (sint16)(0.93f*19800.0f/Command_MciSensorsWm_Params->Transmission_Ratio);

            Command_Params->Low_Regulators_Thr_Speed = (sint16)(Command_SpeedCntrl_Params->SpeedCtrlPrm.speed_gains_table[0].speed_abs);
            Command_Params->Med_Regulators_Thr_Speed = (sint16)(Command_SpeedCntrl_Params->SpeedCtrlPrm.speed_gains_table[1].speed_abs);
            Command_Params->High_Regulators_Thr_Speed = (sint16)(Command_SpeedCntrl_Params->SpeedCtrlPrm.speed_gains_table[2].speed_abs);

            Command_Params->Max_Deceleration_Low_Speed = (sint16)(70.0f*Command_MciSensorsWm_Params->Transmission_Ratio);
            Command_Params->Max_Deceleration_High_Speed = (sint16)(27.0f*Command_MciSensorsWm_Params->Transmission_Ratio);

            Command_Params->Max_Acceleration_Low_Speed = (sint16)(70.0f*Command_MciSensorsWm_Params->Transmission_Ratio);
            Command_Params->Max_Acceleration_High_Speed = (sint16)(27.0f*Command_MciSensorsWm_Params->Transmission_Ratio);

            Max_Acceleration = Command_Params->Max_Acceleration_Low_Speed;
            Max_Deceleration = Command_Params->Max_Deceleration_Low_Speed;

            Max_Distr_Speed_Rpm_M = (uint16)((MAX_DISTR_SPEED * Command_Params->Transmission_Ratio)>>8);

            Min_Allowed_Speed = 225; //[rpm-M]

            ParamsReady = TRUE;
        }
#endif

    }
}


/**
 *  @brief      CommandManager Unlock Acceleration -> For EOL
 *
 */
void CommandManager__UnLockAcceleration(void)
{

    sint32 temp32_2;
    sint32 temp32;

    if (ParamsReady == TRUE)
    {
        // Ramp Limits
        temp32_2 = IQMATH__I17Q15MULI17Q15(19800,500);

        temp32 = (sint32)((SF_Application_Params.u16MaxDeltaSpeedSetPointHighSpeed_Hi<<16) + (uint16)(SF_Application_Params.u16MaxDeltaSpeedSetPointHighSpeed_Lo));
        temp32 = temp32>>1;
        Command_Params->Max_Acceleration_High_Speed = IQMATH__Q15MULQ15(temp32, temp32_2);
    }

}

/**
 *  @brief      Decode the Command coming from the ACU
 *
 *  @param[in]  command The command data coming from the ACU
 */
void CommandManager__DecodeMasterCommand(sint16 speed_rpm, sint16 ramp_rpm_per_s, uint16 flags)
{
    sint16 platform_final_speed_abs;      // [rpm_D]
    DMC_RETURNS_E_T dmc_state_temp = DMC_SKIP_COMMAND;
	sint32 speed_set_point;

#ifdef MOTOR_BPM_TYPE
    sint32 max_delta_speed_temp1;
	sint32 max_delta_speed_temp2;
	sint32 speed_set_point_abs;
	BOOL_TYPE command_speed_negative_old;
#endif

    if(ParamsReady == TRUE)
    {
        #if MCI_FVT_TESTS == ENABLED
        if (flags == FVT_TEST_FLAGS)
        {
            if ((speed_rpm != 0) && (Movement_State == PAUSE_STATE))
            {
                //
                dmc_state_temp = DMC_START_FVT;
            }

            if ((speed_rpm == 0) && (Movement_State != PAUSE_STATE))
            {
                    dmc_state_temp = DMC_STOP_FVT;
            }

        }
        else
        #endif
        {
            
    #ifdef MOTOR_BPM_TYPE
            command_speed_negative_old = Command_Speed_Negative;
    #endif

            platform_final_speed_abs = MATHCALC__ABS(speed_rpm);
            if(speed_rpm < 0)
            {
                Command_Speed_Negative = TRUE;
            }
            else
            {
                Command_Speed_Negative = FALSE;
            }
    #ifdef INDESIT_LEGACY
            if(FaultManager__GetFaultToPublish())
            {// [ fault condition --> skip command ]
                dmc_state_temp = DMC_SKIP_COMMAND;
            }
    #else
            if(FALSE)
            {

            }
    #endif
    #if (APPLIANCE_TYPE == APPLIANCE_TYPE_WASHER)
            else if(platform_final_speed_abs >= Command_Params->Max_Allowed_Speed)
            {// [ final speed not allowed --> set Nack condition ]
                dmc_state_temp = DMC_NACK_CONDITION;
            }
    #else//APPLIANCE_TYPE_DISHWASHER TODO
    //        else if ((DataType == PILOT_TYPE_WASH) && (TestBit(DOOR_STATUS,uw16Flags) == 0))
    //		{// Wash pump is active but Door Status is NOT_CLOSED
    //        	dmc_state_temp = DMC_SKIP_COMMAND;
    //		}
    #endif

            else if((((Mci__GetAnalog(MOTOR0, MCI_AI_INTERNAL_MCI_STATE)) != MCI_IDLE)&& //MCI_IDLE
                    ((Mci__GetAnalog(MOTOR0, MCI_AI_INTERNAL_MCI_STATE)) != MCI_RUNNING)) //MCI_RUNNING
                    &&(speed_rpm!= 0))
            {// [ if(free_ramp_flag == 1) <--> free ramp down in progress --> skip command ]
                dmc_state_temp = DMC_SKIP_COMMAND;
            }
            else if((((Mci__GetAnalog(MOTOR0, MCI_AI_INTERNAL_MCI_STATE)) != MCI_IDLE)&& //MCI_IDLE
                    ((Mci__GetAnalog(MOTOR0, MCI_AI_INTERNAL_MCI_STATE)) != MCI_RUNNING)&& //MCI_RUNNING
                    ((Mci__GetAnalog(MOTOR0, MCI_AI_INTERNAL_MCI_STATE)) != MCI_CLASS_B_STARTING)&& //MCI_CLASS_B_STARTING
                    ((Mci__GetAnalog(MOTOR0, MCI_AI_INTERNAL_MCI_STATE)) != MCI_MANUAL_CONTROL)&& //MCI_MANUAL_CONTROL
                    ((Mci__GetAnalog(MOTOR0, MCI_AI_INTERNAL_MCI_STATE)) != MCI_STARTING))  //MCI_STARTING
                    &&(speed_rpm == 0))
            {// [ if(free_ramp_flag == 1) <--> free ramp down in progress --> skip command ]
                dmc_state_temp = DMC_SKIP_COMMAND;
            }
            else if(Movement_State >= CONTROLLED_RAMP_TO_ZERO_STATE)
            {// [ if(free_ramp_flag == 1) <--> free ramp down in progress --> skip command ]
                dmc_state_temp = DMC_SKIP_COMMAND;
            }
    #ifdef MOTOR_CIM_TYPE
            else if(speed_rpm == 0)
            {// [ null final speed --> Master Command Pause ]
                
                //always free ramp down in case of PAUSE
                dmc_state_temp = DMC_COMMAND_PAUSE;
            }
    #endif
            else
            {// [ final speed not null --> Command Evaluation ]
                // [DONE: Platform_Final_Speed calculation ]
                if((FALSE == Mci__GetDigital(MOTOR0, MCI_DI_REACHED_TARGET_SPEED)) && // if(sRampParams.uw16RampStatus)
                   (TRUE == Mci__GetDigital(MOTOR0, MCI_DI_MOTOR_RUNNING)))
                {// [ ramp in progress ]
    #if (APPLIANCE_TYPE == APPLIANCE_TYPE_WASHER)
                    if(Ramp_End_Speed <= Max_Distr_Speed_Rpm_M)
                    {// [ in ramp with target speed < MAX_DISTR_SPEED --> skip command ]
                        dmc_state_temp = DMC_SKIP_COMMAND;
                    }
                    // [ in ramp with target speed > MAX_DISTR_SPEED ]
                    else if((speed_rpm >= 0) && (speed_rpm <= MAX_DISTR_SPEED))
                    {// [ if new target speed is >= 0, but <= MAX_DISTR_SPEED -->
                        // stop ramp to accept the new command]
                        dmc_state_temp = DMC_STOP_RAMP;
                    }
    #else
                    if(speed_rpm == 0)
                    {
                        // stop ramp to accept the new command]
                        dmc_state_temp = DMC_STOP_RAMP;
                    }
    #endif
                    else
                    {
                        dmc_state_temp = DMC_SKIP_COMMAND;
                    }
                }
                else
                {
                    // [ command accepted ]
                    Platform_Final_Speed = (sint16)((Command_Params->Transmission_Ratio * platform_final_speed_abs)>>8);
                    Ramp_Time_Or_Acc = ramp_rpm_per_s;
                    speed_set_point = Mci__GetAnalog(MOTOR0, MCI_AI_SPEED_REFERENCE_RPM_S16)>>16;

    #if (APPLIANCE_TYPE == APPLIANCE_TYPE_WASHER)
                    if((Platform_Final_Speed > Command_Params->Med_Regulators_Thr_Speed) && (speed_rpm<0))
                    {// [ if Platform_Final_Speed < -MedReg_ThSpeed --> set Nack condition ]
                        dmc_state_temp = DMC_NACK_CONDITION;
                    }
    #else
                    if(platform_final_speed_abs >= Command_Params->Max_Allowed_Speed)
                    {// [ final speed not allowed --> set skip condition ]
                        dmc_state_temp = DMC_SKIP_COMMAND;
                    }
    #endif
                    else if(speed_set_point == 0)
                    {// initial speed is zero

    #if (APPLIANCE_TYPE == APPLIANCE_TYPE_WASHER)
                        //-------------------- Extended Command Plausibility - begin --------------------
                        if (platform_final_speed_abs > MAX_DISTR_SPEED)
                        {
                            dmc_state_temp = DMC_NACK_CONDITION;
                        }
    #ifdef MOTOR_BPM_TYPE
                        else if(Platform_Final_Speed == speed_set_point)
                        {// [ final speed already reached --> skip command ]
                            // use the Last_Platform_Final_Speed to avoid conversion errors
                            dmc_state_temp = DMC_SKIP_COMMAND;
                        }
    #endif
                        else
                        {


    #ifdef MOTOR_BPM_TYPE
                            max_delta_speed_temp1 = (sint32)(platform_final_speed_abs * 10);
                            max_delta_speed_temp2 = (sint32)(Ramp_Time_Or_Acc * MIN_ACCELERATION_DRUM);

                            if(max_delta_speed_temp1 < max_delta_speed_temp2)
                            { // Acceleration is lower than minimum admitted at startup

                                Ramp_Time_Or_Acc = 0;

                                if (Platform_Final_Speed > Min_Allowed_Speed)
                                {
                                    Platform_Final_Speed = Min_Allowed_Speed;
                                }
                            }
                            else if (Platform_Final_Speed > Command_Params->Low_Regulators_Thr_Speed)
    #else
                            if (Platform_Final_Speed > Command_Params->Low_Regulators_Thr_Speed)
    #endif
                            {
                                Platform_Final_Speed = Command_Params->Low_Regulators_Thr_Speed;
                            }
                            //-------------------- Extended Command Plausibility - end --------------------
                            dmc_state_temp = DMC_RAMP_UP;
                        }
    #else //APPLIANCE_TYPE_DISHWASHER

                        if(Platform_Final_Speed == 0)
                        {// [ final speed already reached --> skip command ]
                            // use the Last_Platform_Final_Speed to avoid conversion errors
                            dmc_state_temp = DMC_SKIP_COMMAND;
                        }
                        else
                        {

                            if (Platform_Final_Speed < Min_Allowed_Speed)
                            {
                                Platform_Final_Speed = Min_Allowed_Speed;
                            }

                            dmc_state_temp = DMC_RAMP_UP;
                        }
    #endif
                    }
                    else
                    {
                        if(((speed_rpm > 0) && (speed_set_point < 0)) ||
                           ((speed_rpm < 0) && (speed_set_point > 0)))
                        {// [ if different sign between speed reference and platform speed --> set inversion speed ]
    #ifdef  MOTOR_CIM_TYPE
                            dmc_state_temp = DMC_SPEED_INVERSION;
    #else

                            if(speed_set_point < 0)
                            {
                                Command_Speed_Negative = TRUE;
                            }
                            else
                            {
                                Command_Speed_Negative = FALSE;
                            }

                            speed_set_point_abs = MATHCALC__ABS(speed_set_point);

                            if(speed_set_point_abs >= Min_Allowed_Speed)
                            {
                                Platform_Final_Speed = Min_Allowed_Speed -1;

                                dmc_state_temp = DMC_CONTROLLED_RAMP_DOWN;
                            }
                            else
                            {
                                dmc_state_temp = DMC_COMMAND_PAUSE;
                            }
    #endif
                        }
                        else if(Platform_Final_Speed == Ramp_End_Speed)
                        {// [ final speed already reached --> skip command ]
                            // use the Last_Platform_Final_Speed to avoid conversion errors
                            dmc_state_temp = DMC_SKIP_COMMAND;
                        }
                        else if (MATHCALC__ABS(speed_set_point) < Platform_Final_Speed)
                        { // [ set Ramp-up condition ]
                            dmc_state_temp = DMC_RAMP_UP;
                        }
                        else
                        { // [ Ramp-down condition: set FREE_RAMP_DOWN or CONTROLLED_RAMP_DOWN state ]

    #ifdef  MOTOR_CIM_TYPE
                            if(ramp_rpm_per_s == 0)
                            {// [ ramp down not controlled mode ]
                                dmc_state_temp = DMC_FREE_RAMP_DOWN;
                            }
                            else if(Platform_Final_Speed <= Command_Params->Med_Regulators_Thr_Speed)
                            {// [ ramp down not controlled mode ]
                                dmc_state_temp = DMC_FREE_RAMP_DOWN;
                            }
                            else
                            {
                                // [ controlled ramp down ]
                                dmc_state_temp = DMC_CONTROLLED_RAMP_DOWN;
                            }
    #else
                            if((Platform_Final_Speed >= Min_Allowed_Speed)||(Platform_Final_Speed == 0))
                            {

                                if(Platform_Final_Speed == 0)
                                {//Stop command
                                    speed_set_point_abs = MATHCALC__ABS(speed_set_point);

                                    if(speed_set_point_abs >= Min_Allowed_Speed)
                                    {//Forced a controlled ramp until the minimum allowed speed
                                        Platform_Final_Speed = Min_Allowed_Speed -1;
                                        Command_Speed_Negative = command_speed_negative_old;

                                        dmc_state_temp = DMC_CONTROLLED_RAMP_DOWN;
                                    }
                                    else
                                    {
                                        dmc_state_temp = DMC_COMMAND_PAUSE;
                                    }
                                }
                                else
                                {
                                    dmc_state_temp = DMC_CONTROLLED_RAMP_DOWN;
                                }
                            }
                            else
                            {
                                dmc_state_temp = DMC_SKIP_COMMAND;
                            }
    #endif
                        }
                    }
                }
            }
        }
        DMC_State = dmc_state_temp;
        ExecuteMasterCommand();
    }
}

/**
 *  @brief      Get CommandManager last Fault
 *
 *  @return     Value of last fault detected by the module
 */
uint16 CommandManager__GetFault(void)
{
    return Fault;
}





//=====================================================================================================================
//-------------------------------------- Private Functions ------------------------------------------------------------
//=====================================================================================================================

void ExecuteMasterCommand(void)
{
    uint16 temp_fault = 0;
    sint32 acceleration = 0;

    switch (DMC_State)
    {
     //---------------------
        case DMC_SKIP_COMMAND:
        break;
    //---------------------

    //---------------------
        case DMC_NACK_CONDITION:
        temp_fault = NACK;
        break;
    //---------------------

    //---------------------
        case DMC_COMMAND_PAUSE:    
        
        if(Movement_State == PAUSE_STATE)
        {// [ Target_Flag = 1, free_ramp_flag == 0, ramp_down_to_zero_flag == 0, RampEndSpeed = 0 ]
            break;
        }

    //---------------------
        
    //---------------------
        case DMC_SPEED_INVERSION:
        Stop_Motor();
        
        break;
    //---------------------


    //---------------------
        case DMC_STOP_RAMP:
        Mci__Run(MOTOR0, (Mci__GetAnalog(MOTOR0, MCI_AI_SPEED_REFERENCE_RPM_S16)>>16) + STOP_RAMP_DELTA_SPEED, (MAX_RPM_PER_SEC_ACCEL - 1));
        break;
    //---------------------

    //---------------------
        case DMC_RAMP_UP:
#ifdef MOTOR_BPM_TYPE
        Ramp_Down_Flag = 0;
#endif
        if (Platform_Final_Speed > HIGH_SPEED_THRESHOLD)
        { // High Speed Region over 45% of max speed
            Max_Acceleration = Command_Params->Max_Acceleration_High_Speed;
        }
        else
        {  // Low Speed Region below 45% of max speed
            Max_Acceleration = Command_Params->Max_Acceleration_Low_Speed;
        }
        
        temp_fault = Run_Motor(Max_Acceleration);

        if(temp_fault == 0)
		{
			Movement_State = CONTROLLED_RAMP_STATE;
		}
        break;
    //---------------------
        
    //---------------------
        case DMC_CONTROLLED_RAMP_DOWN:
#ifdef MOTOR_BPM_TYPE
        Ramp_Down_Flag = 1;
#endif
        if ((Mci__GetAnalog(MOTOR0, MCI_AI_SPEED_REFERENCE_RPM_S16)>>16) > HIGH_SPEED_THRESHOLD_DEC)
        { // High Speed Region over 45% of max speed
            Max_Deceleration = Command_Params->Max_Deceleration_High_Speed;
        }
        else
        {  // Low Speed Region below 45% of max speed
            Max_Deceleration = Command_Params->Max_Deceleration_Low_Speed;
        }

        temp_fault = Run_Motor(Max_Deceleration);

        if(temp_fault == 0)
		{

#ifdef MOTOR_CIM_TYPE
    		Movement_State = CONTROLLED_RAMP_STATE;

#else
        	if (Platform_Final_Speed > Min_Allowed_Speed)
    		{
        		Movement_State = CONTROLLED_RAMP_STATE;
    		}
    		else
    		{
    			Movement_State = CONTROLLED_RAMP_TO_ZERO_STATE;
    		}
#endif

		}
        break;
    //---------------------

    //---------------------
        case DMC_FREE_RAMP_DOWN:

        acceleration = 0;

        temp_fault = Run_Motor(acceleration);

        if(temp_fault == 0)
   		{
        	 Movement_State = FREE_RAMP_DOWN_STATE;
   		}
        break;
    //---------------------

#if MCI_FVT_TESTS == ENABLED
    //---------------------
        case DMC_START_FVT:
        if (Movement_State == PAUSE_STATE)
        {
            // clear previous results and start the fvt sequence
            ClearFvtResult(&FvtResult);
            temp_fault = Mci__FvtStart(MOTOR0);

            if(temp_fault == 0)
            {
                 Movement_State = FVT_RUNNING_STATE;
            }
        }
        break;
    //---------------------

    //---------------------
        case DMC_STOP_FVT:
        if (Movement_State == FVT_RUNNING_STATE)
        {
            temp_fault = Mci__FvtStop(MOTOR0);

            if(temp_fault == 0)
            {
                 Movement_State = FVT_ABORTED_STATE;
            }
        }
        else if (Movement_State == FVT_FINISHED_STATE)
        {
            Movement_State = PAUSE_STATE;
        }
        break;
    //---------------------
#endif


    //---------------------
        default:
        break;
    //---------------------
    }

#if (APPLIANCE_TYPE == APPLIANCE_TYPE_WASHER)
    // Update Speed Regulator only if needed
    if (DMC_State < DMC_SKIP_COMMAND)
    {
//#ifdef MOTOR_BPM_TYPE
//    	if(Ramp_Down_Flag == 0)
//#endif
//    	{
//    		MciSetWm__UpdateSpeedRegulator(Ramp_End_Speed);
//    	}
    }
#endif
    //---------------------

    if (temp_fault)
    {
        Stop_Motor();
        Fault = temp_fault;
    }
}



static void ApplicationStateMachine(void)
{
    sint32 actual_speed_abs;
    sint32 delta_speed_abs;

#ifdef MOTOR_BPM_TYPE
    sint32 speed_set_point_abs;

    speed_set_point_abs = MATHCALC__ABS(Mci__GetAnalog(MOTOR0, MCI_AI_SPEED_REFERENCE_RPM_S16)>>16);
#endif

    actual_speed_abs =  MATHCALC__ABS( (Mci__GetAnalog(MOTOR0, MCI_AI_ACTUAL_SPEED_S16)>>16));
    delta_speed_abs = Ramp_End_Speed - actual_speed_abs;
    delta_speed_abs = MATHCALC__ABS(delta_speed_abs);

	if (Power_On_Free_Down_Cnt>0)
    {
        --Power_On_Free_Down_Cnt;
    }        
    else
    {

#if (APPLIANCE_TYPE == APPLIANCE_TYPE_WASHER)
#ifdef MOTOR_BPM_TYPE
//    	if(Ramp_Down_Flag == 1)
//    	{
//    		MciSetWm__UpdateSpeedRegulator(speed_set_point_abs);
//    	}
#endif
#endif
        switch (Movement_State)
        {
             //---------------------
            case PAUSE_STATE:
            // [ Target_Flag = 1, free_ramp_flag == 0, ramp_down_to_zero_flag == 0, RampEndSpeed = 0 ]
            
            break;
            //---------------------
             
             //---------------------
            case STEADY_STATE:
            // [ Target_Flag = 1, free_ramp_flag == 0, ramp_down_to_zero_flag == 0, RampEndSpeed != 0 ]
#ifdef MOTOR_BPM_TYPE
			Ramp_Down_Flag = 0;
#endif
            break;
            //---------------------
             
            //---------------------
            case CONTROLLED_RAMP_STATE:
            // [ Target_Flag = 0, free_ramp_flag == 0, ramp_down_to_zero_flag == 0, RampEndSpeed != 0 ]
            
            //--------------------- DONE ---------------------
            // [ Check Target Reached  ]
            if(TRUE == Mci__GetDigital(MOTOR0, MCI_DI_REACHED_TARGET_SPEED))
            {    // [if(Target Reached) --> eMovementStateMachine = STEADY_STATE
                Movement_State = STEADY_STATE;
            }
            
            break;
            //---------------------
            
            //---------------------
            case CONTROLLED_RAMP_TO_ZERO_STATE:
            // [ Target_Flag = 0, free_ramp_flag == 0, ramp_down_to_zero_flag == 0, RampEndSpeed != 0 ]

            // [ Check Min Speed Reached  ]
            if(actual_speed_abs < Min_Allowed_Speed)
			{
				Stop_Motor();
			}

            break;
            //---------------------

            //---------------------
            case FREE_RAMP_DOWN_TO_ZERO_STATE:
            // [ Target_Flag = 0, free_ramp_flag == 1, ramp_down_to_zero_flag == 1, RampEndSpeed == 0 ]
            if (Free_Down_Cnt!=0)
            {
                --Free_Down_Cnt;
            }
            else
            {
                if(Mci__GetAnalog(MOTOR0, MCI_AI_ACTUAL_SPEED_S16) == 0)
                {
                    Movement_State = PAUSE_STATE;
                }
            }
    
            break;
            //---------------------
             
            //---------------------
            case FREE_RAMP_DOWN_STATE:

            // [ Target_Flag = 1, free_ramp_flag == 1, ramp_down_to_zero_flag == 0, RampEndSpeed != 0 ]
            if(Ramp_End_Speed >= actual_speed_abs)
            {
            	Movement_State = STEADY_STATE;        // [ free_ramp_flag = 0 ]
            }
            break;
            //---------------------

#if MCI_FVT_TESTS == ENABLED
            //---------------------
            case FVT_RUNNING_STATE:
                if (Mci__FvtGetStatus(MOTOR0, MCI_FVT_STATUS_FINISHED))
                {
                    Movement_State = FVT_FINISHED_STATE;
                    // FVT is finished --> we can get the results
                    Mci__FvtGetResult(MOTOR0, &FvtResult);
                }
            break;
            //---------------------
            //---------------------
            case FVT_ABORTED_STATE:
                Movement_State = PAUSE_STATE;
            break;
            //---------------------


#endif


            default:
            break;
        }

    } 
}

void Stop_Motor()
{

#ifdef MOTOR_CIM_TYPE
	//Soft Start calculation
	MciSetWm__SoftStart(SOFT_START_ON_STOP);
#endif

    // [DONE: Free_Down_Cnt calculation ]
    // Free_Down_Gain is different from -32768 for definition --> we do not have risk of saturation in the multiplication
    Free_Down_Cnt = IQMATH__Q15MULQ15(MATHCALC__ABS((Mci__GetAnalog(MOTOR0, MCI_AI_SPEED_REFERENCE_RPM_S16)>>16)), Command_Params->Free_Down_Gain);

    Ramp_End_Speed = 0;

#ifdef MOTOR_BPM_TYPE
    Platform_Final_Speed = 0;
    Ramp_Down_Flag = 0;
#endif

    Movement_State = FREE_RAMP_DOWN_TO_ZERO_STATE;

    Mci__Stop(MOTOR0, Command_Params->Max_Acceleration_Low_Speed);
}



//---------------------------------------------------------------------------------------------------------------------
/**
 *  @brief     Run motor
 *  @details
 *
 *
 *  @param[in]
 *  @param[out]
 *  @return
 */
uint16 Run_Motor(sint16 acceleration_limit)
{
    sint16 temp_fault = 0;
    sint16 speed = 0;
    sint32 acceleration = 0;

	// [ final speed in RPM format ]
	Ramp_End_Speed = Platform_Final_Speed;

	speed = Ramp_End_Speed;
	if(Command_Speed_Negative == TRUE)
	{
		speed = -speed;
	}

	if(acceleration_limit != 0)
	{

#ifdef INDESIT_LEGACY
		if(Ramp_Time_Or_Acc == 0)
		{
			Ramp_Time_Or_Acc = 1; //avoid division by 0
		}

		acceleration = (speed - (Mci__GetAnalog(MOTOR0, MCI_AI_SPEED_REFERENCE_RPM_S16)>>16)) * 10;
		acceleration = (sint32)((MATHCALC__ABS(acceleration)) / Ramp_Time_Or_Acc); // Ramp_Time_Or_Acc Resolution is 100ms
#else
		if(Ramp_Time_Or_Acc == 0)
		{
			Ramp_Time_Or_Acc = acceleration_limit; 
		}
		
		
		acceleration = Ramp_Time_Or_Acc;
#endif

		if(acceleration > acceleration_limit)
		{
			acceleration = acceleration_limit;
		}
		else if(acceleration < 1)
		{
			acceleration = 2;
		}
	}
	else
	{
	     acceleration = 0;
	}

	temp_fault = Mci__Run(MOTOR0, speed, acceleration);

	if(temp_fault)
	{
		temp_fault = NACK;
	}
	else
	{
		temp_fault = 0;
	}

#ifdef MOTOR_CIM_TYPE
    //Soft Start calculation
	MciSetWm__SoftStart(SOFT_START_ON_START);
#endif

	return temp_fault;

}

#if MCI_FVT_TESTS == ENABLED
void ClearFvtResult(MCI_FVT_TEST_RESULT_TYPE *fvt_data_result)
{
    fvt_data_result->Error_List                         = 0;
    fvt_data_result->OC_Test_Found                      = 0;
    fvt_data_result->Sensors_Test_Avg_Curr_A            = 0;
    fvt_data_result->Sensors_Test_Avg_Curr_B            = 0;
    fvt_data_result->Sensors_Test_Avg_Curr_C            = 0;
    fvt_data_result->Sensors_Test_Avg_DC_Bus_Volt       = 0;
    fvt_data_result->Sensors_Test_Avg_Phase_Volt_A      = 0;
    fvt_data_result->Sensors_Test_Avg_Phase_Volt_B      = 0;
    fvt_data_result->Sensors_Test_Avg_Phase_Volt_C      = 0;
    fvt_data_result->Surge_Test_Avg_DC_Bus_Volt_Close   = 0;
    fvt_data_result->Surge_Test_Avg_DC_Bus_Volt_Open    = 0;
}
#endif
