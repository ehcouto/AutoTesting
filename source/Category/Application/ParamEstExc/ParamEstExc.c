/**
 *  @file       
 *
 *  @brief      Source file for the ParamEstExc (Parameter Estimation Excitation) module.
 *
 *  @details    Creates oscillations in the motor rpm to produce variation in the torque signal.
 *
 *  @software owner     Paul Spindler
 *  @technology owner   Stephen Keres
 *

    @startuml{ParamEstExc_state_machine.png}

        title Parameter Estimation Excitation State machine

        [*] --> Idle

        Idle : do / MonitorApiCommand
        Idle --> Active : ev_enable

        Active : enter / EnableModule
        Active : do / ModuleActive
        Active --> RpmOutOfRange : ev_rpm_out_of_range
        Active --> Stop : ev_disable

        Stop : enter / DisableModule
        Stop : do / ModuleDisabling
        Stop --> Idle : ev_module_disabled

        RpmOutOfRange : enter / DisableModule
        RpmOutOfRange : do / ModuleDisabling
        RpmOutOfRange --> StoppedForRpmOutOfRange : ev_module_disabled
        RpmOutOfRange --> Stop : ev_disable

        StoppedForRpmOutOfRange : do / CheckRpmRange
        StoppedForRpmOutOfRange --> Idle : ev_disable
        StoppedForRpmOutOfRange --> Active : ev_rpm_in_range

    @enduml

 *
 *  $Header: ParamEstExc.c 1.5 2015/04/07 13:22:04EDT Paul F Spindler (SPINDPF) Exp  $
 *
 *  @copyright  Copyright 2014-$Date: 2015/04/07 13:22:04EDT $. Whirlpool Corporation. All rights reserved - CONFIDENTIAL
 */
//---------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------


//-------------------------------------- Include Files ----------------------------------------------------------------
#include "C_Extensions.h"

#include "ParamEstExc.h"
#include "ParamEstExc_prv.h"

#include "limits.h"
#include "math.h"
#include "string.h"

//=====================================================================================================================
//=== StateEngineCrank (v1.5) Auto-Generated Definitions for Parameter Estimation Excitation State Machine
//=====================================================================================================================
// !! StateEngineCrank cannot update this code if you change the header !!


//! Unique names used to identify states of the Parameter Estimation Excitation state machine.
typedef enum PARAMETER_ESTIMATION_EXCITATION_STATE_ENUM
{
    STATE_NONE                         = 0,

    STATE_IDLE                         = 1,         //!< (initial_state)
    STATE_ACTIVE                       = 2,
    STATE_RPM_OUT_OF_RANGE             = 3,
    STATE_STOP                         = 4,
    STATE_STOPPED_FOR_RPM_OUT_OF_RANGE = 5,

    STATE_PARAMETER_ESTIMATION_EXCITATION_END = 6
} PARAMETER_ESTIMATION_EXCITATION_STATE_TYPE;

//! The state variable for the Parameter Estimation Excitation state machine.
//! State machines with a history pseudostate may write to this variable during initialization.
//! Other state machines must only write to this variable through auto-generated functions.
static PARAMETER_ESTIMATION_EXCITATION_STATE_TYPE Parameter_Estimation_Excitation_State;

// -- Private Function Prototypes used by the state machine for customization functions --
static void CheckRpmRange(void);
static void DisableModule(void);
static void EnableModule(void);
static void ModuleActive(void);
static void ModuleDisabling(void);
static void MonitorApiCommand(void);

// -- Private Function Prototypes used by the state machine for standard state machine functions --
static void EventDisable(void);
static void EventEnable(void);
static void EventModuleDisabled(void);
static void EventRpmInRange(void);
static void EventRpmOutOfRange(void);
static void ParameterEstimationExcitationExecute(void);
static void ParameterEstimationExcitationInitialize(void);

// !! StateEngineCrank cannot update this code if you change the footer !!
// End of StateEngineCrank Auto-Generated Code.

//-------------------------------------- PRIVATE (Variables, Constants & Defines) -------------------------------------

typedef unsigned char   DOWN_SAMPLER_COUNTER_TYPE;          // 1 = 1 call of the API ParamIDExcitation__Handler()
CT_ASSERT(INPUT_HANDLER_SLOT_PERIODS <= UCHAR_MAX);

#define MSEC_TO_SEC(msec)           ((msec) / 1000.0f)                           //!< Conversion from milliseconds to seconds

//! The sample rate of the Excitation generator. The possible combinations of slot options and
//! integer downsample ratios will dictate the sample rates which may be achieved.
#define TS_EXCITATION               MSEC_TO_SEC(INPUT_HANDLER_TIME_SLOT * INPUT_HANDLER_SLOT_PERIODS)

#define SIGN(in)                    (((in) > 0) ? 1 : (((in) < 0) ? -1 : 0))    //!< Signum (sign function) macro

/**
 *  Variable type definition for the module motor speed.
 *  Resolution: 1.0 = 1.0 rpm
 */
typedef float MOTOR_SPEED_TYPE;

//! Pseudo-random sequence from rand() normalized to be between 0 and 1. The random numbers are stored so there is
//! consistency in testing from run-to-run, and across microcontrollers, since rand() and srand() implementations vary.
static const float RANDOM_SEQUENCE[] =
{
    0.43874437f,
    0.38155845f,
    0.76551682f,
    0.79519993f,
    0.18687260f,
    0.48976439f,
    0.44558620f,
    0.64631301f,
    0.70936483f,
    0.75468665f,
    0.27602509f,
    0.67970270f,
    0.65509802f,
    0.16261174f,
    0.11899768f,
    0.49836406f,
    0.95974398f,
    0.34038574f,
    0.58526772f,
    0.22381194f
};
#define RANDOM_SEQUENCE_LENGTH  (sizeof(RANDOM_SEQUENCE) / sizeof(RANDOM_SEQUENCE[0]) )   //!< Length of pseudo-random sequence for use in excitation generation

//! list of API commands the module will react to
typedef enum
{
    API_CMD_DISABLE = 0,
    API_CMD_ENABLE
} API_CMD_TYPE;

static const PARAM_EST_EXC_SETUP_TYPE*    Setup_Parameters;              //!< Internal storage of Excitation configuration parameters address

//! definition of module static variables
static struct
{
    float                       Accel;                          //!< Acceleration excitation (RPM/s)
    MOTOR_SPEED_TYPE            W;                              //!< Speed excitation
    unsigned short              Generation_Counter;             //!< Counter for Excitation Generation
    signed char                 Sign_PRBS;                      //!< Current sign of PRBS acceleration
    unsigned char               Random_Sequence_Index;          //!< Current index in random sequence
    DOWN_SAMPLER_COUNTER_TYPE   Handler_Down_Sample_Counter;    //!< down sampler counter for the module execution
    API_CMD_TYPE                Api_Command;                    //!< Excitation enable/disable flag from user request
} Exc;  // Excitation


//-------------------------------------- PRIVATE (Function Prototypes) ------------------------------------------------

static void DecrementGenerationCounter(void);
static BOOL_TYPE IsModuleActive(void);
static void OutputSpeedReferenceGeneratorOffset(void);

//=====================================================================================================================
//-------------------------------------- Public Functions -------------------------------------------------------------
//=====================================================================================================================

//---------------------------------------------------------------------------------------------------------------------
/**
 *  @brief      It Initializes the module ParamEstExc and its variables
 *
 */
void ParamEstExc__Initialize(void)
{
    memset(&Exc, 0, sizeof(Exc));
    Setup_Parameters = NULL;
    ParameterEstimationExcitationInitialize();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 *  @brief      The Parameter Estimation Excitation handler.
 *  @details    Shall be called from the slot defined by INPUT_HANDLER_SLOT_PERIODS.
 */
void ParamEstExc__Handler(void)
{
    Exc.Handler_Down_Sample_Counter = (Exc.Handler_Down_Sample_Counter > 0) ? Exc.Handler_Down_Sample_Counter - 1 : 0;      // decrement and prevent underflow
    ParameterEstimationExcitationExecute();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 *  @brief  Enables the Parameter Estimation Excitation module.
 */
void ParamEstExc__Enable(void)
{
	if ( Setup_Parameters == NULL )
	{
		DEBUG_SETUP_PARAMETER_IS_NULL();
	}
	else
	{
		Exc.Api_Command = API_CMD_ENABLE;
	}
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief   Disables the Parameter Estimation Excitation module.
 */
void ParamEstExc__Disable(void)
{
    Exc.Api_Command = API_CMD_DISABLE;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief   Return the status of the Parameter Estimation Excitation module.
 *
 * @return  TRUE = module is active; FALSE = module is inactive
 */
BOOL_TYPE ParamEstExc__IsRunning(void)
{
    return ( IsModuleActive() );
}

//---------------------------------------------------------------------------------------------------------------------
/**
 *  @brief  Returns the current Parameter Estimation Excitation Setup.
 *  @param  setup   pointer to the Environment definition of the PARAM_EST_EXC_SETUP_TYPE
 *  @return The current Setup is not NULL
 */
BOOL_TYPE ParamEstExc__GetSetup(PARAM_EST_EXC_SETUP_TYPE *parameters)
{
	BOOL_TYPE ret = FALSE;
    if ( Setup_Parameters != NULL )
    {
    	ret = TRUE;
    	if ( parameters != NULL )
    		memcpy(parameters, Setup_Parameters, sizeof(PARAM_EST_EXC_SETUP_TYPE));
    }
    return ret;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 *  @brief  Set the module's functional parameters
 *  @param  parameters  Environment specific setup.
 *  @return Is the new ParamEstExc setup valid?
 */
BOOL_TYPE ParamEstExc__SetSetup(const PARAM_EST_EXC_SETUP_TYPE *parameters)
{
    BOOL_TYPE config_valid = TRUE;
    if ( parameters == NULL )
    {
        DEBUG_SETUP_PARAMETER_IS_NULL();
        return config_valid = FALSE;
    }
    else
    {
        if ( parameters->Time < INPUT_EXCITATION_MIN_TIME )
        {
            DEBUG_INVALID_EXC_TIME(parameters->Time);
            config_valid = FALSE;
        }
        // else parameters.Time >= INPUT_EXCITATION_MIN_TIME, time is valid, do nothing

        // If the user selects an invalid excitation method, reject the configuration.
        if ( parameters->Method >= NUM_OF_PARAM_EXC_METHODS )
        {
            DEBUG_INVALID_EXC_METHOD(parameters->Method);
            config_valid = FALSE;
        }

        if ( config_valid == TRUE )
        {
            Setup_Parameters = parameters;
        }
    }

    return config_valid;
}


//=====================================================================================================================
//-------------------------------------- Private Functions ------------------------------------------------------------
//=====================================================================================================================

//---------------------------------------------------------------------------------------------------------------------
/**
 *  @brief      Decrement the static variable Generation_Counter and prevent underflow.
 */
static void DecrementGenerationCounter(void)
{
    Exc.Generation_Counter = (Exc.Generation_Counter > 0) ? Exc.Generation_Counter - 1 : 0;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 *  @brief  Returns if the module is inactive or active
 *
 *  @return TRUE = module is active; FALSE = module is inactive
 */
BOOL_TYPE IsModuleActive(void)
{
    return ( (Parameter_Estimation_Excitation_State == STATE_IDLE) ? FALSE : TRUE );
}

//---------------------------------------------------------------------------------------------------------------------
/**
 *  @brief      Calculates and outputs the speed offset.
 */
static void OutputSpeedReferenceGeneratorOffset(void)
{
    Exc.W += Exc.Accel * TS_EXCITATION;     // Integrate acceleration reference to get speed reference
    OUTPUT_SPEED_REF_GEN_OFFSET(Exc.W);     // Send excitation to speed reference generator
}


//=====================================================================================================================
//-- Private State Machine Customization Functions --------------------------------------------------------------------
//=====================================================================================================================

//---------------------------------------------------------------------------------------------------------------------
/**
 *  @brief      Monitors the static variable Api_Command to determine if the module should be enabled.
 */
static void MonitorApiCommand(void)
{
    switch ( Exc.Api_Command )
    {
        case API_CMD_DISABLE:
            break;
        case API_CMD_ENABLE:
            EventEnable();
            break;
        default:
            break;
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 *  @brief      Definition of the necessary tasks to enable the algorithm.
 */
static void EnableModule(void)
{
    Exc.Handler_Down_Sample_Counter = 0;
    Exc.Generation_Counter = 0;
    Exc.Random_Sequence_Index = 0;
    Exc.Accel = 0.0f;
    Exc.W = 0.0f;
    Exc.Sign_PRBS = 1;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 *  @brief      Performs the necessary tasks when the algorithm is active.
 */
static void ModuleActive(void)
{
    if ( Exc.Handler_Down_Sample_Counter == 0 )
    {
        Exc.Handler_Down_Sample_Counter = INPUT_HANDLER_SLOT_PERIODS;

        MOTOR_SPEED_TYPE motor_speed = fabsf(INPUT_MOTOR_SPEED());
        // Perform check to stop the module if the speed drops too low
        if ( motor_speed < fabsf(INPUT_MIN_SPEED) )
        {
            DEBUG_MIN_SPEED_VIOLATION(motor_speed);
            EventRpmOutOfRange();
        }
        else if ( Exc.Api_Command == API_CMD_DISABLE )
        {
            EventDisable();
        }
        else
        {
            DecrementGenerationCounter();
            if ( Exc.Generation_Counter == 0 )
            {                                           // go to the next random number in the sequence
                if ( Exc.Random_Sequence_Index < RANDOM_SEQUENCE_LENGTH )
                {                                                       // choose the next number in the random sequence
                    switch ( Setup_Parameters->Method )
                    {
                        case PARAM_EXC_METHOD_PRBS:          // Pseudorandom Binary Sequence (random time two-level excitation)
                            Exc.Accel = Exc.Sign_PRBS * Setup_Parameters->Amplitude;
                            Exc.Sign_PRBS = -Exc.Sign_PRBS;     // flip the acceleration to the other level

                            // The time to next switch is chosen randomly as U[INPUT_EXC_MIN_TIME, Time]
                            // Mod_Exc is the number of periods of exc. gen. corresponding to this time
                            Exc.Generation_Counter = (unsigned short)((((Setup_Parameters->Time - INPUT_EXCITATION_MIN_TIME) * RANDOM_SEQUENCE[Exc.Random_Sequence_Index]) + INPUT_EXCITATION_MIN_TIME) / TS_EXCITATION);
                            break;
                        case PARAM_EXC_METHOD_WN:            // White noise (fixed-time random amplitude excitation)
                            // The time to next switch is fixed.
                            // Define is the number of periods of exc. gen. corresponding to this time
                            Exc.Generation_Counter = (unsigned short)(Setup_Parameters->Time / TS_EXCITATION);

                            // The next acceleration rate is a random number U(-Amplitude, Amplitude)
                            Exc.Accel = 2.0f * Setup_Parameters->Amplitude * (RANDOM_SEQUENCE[Exc.Random_Sequence_Index] - 0.5f);
                            break;
                        default:        // Code should never get here, invalid excitation method
                            // an invalid excitation method is posted by macro DEBUG_INVALID_EXCITATION_METHOD in ParamEstExc__Start()
                            break;
                    }
                    Exc.Random_Sequence_Index++;
                }
                else // Random_Sequence_Index >= RANDOM_SEQUENCE_LENGTH
                {
                    // the excitation random sequence has been completed, bring W_Exc back to zero
                    if ( Exc.Random_Sequence_Index != 0 )
                    {
                        // the if statement is required due to the IAR Compiler optimizing out the following line of code
                        Exc.Random_Sequence_Index = 0;
                    }
                    Exc.Accel = - SIGN(Exc.W) * Setup_Parameters->Amplitude;                        // Accelerate back to W_Ref = 0
                    Exc.Generation_Counter = (unsigned int)((-Exc.W / Exc.Accel) / TS_EXCITATION);  // This time will result in zeroing the W_Ref to correct for any drift
                }
            }
            // else Exc.Generation_Counter > 0, do nothing
            OutputSpeedReferenceGeneratorOffset();
        }
    }
    // else Exc.Handler_Down_Sample_Counter > 0, do nothing
}

//---------------------------------------------------------------------------------------------------------------------
/**
 *  @brief      Definition of the necessary tasks to disable the algorithm.
 */
static void DisableModule(void)
{
    if ( Exc.W != 0 )
    {
        Exc.Accel = - SIGN(Exc.W) * Setup_Parameters->Amplitude;                            // Accelerate back to W_Ref = 0
        Exc.Generation_Counter = (unsigned short)((-Exc.W / Exc.Accel) / TS_EXCITATION);    // This time will result in zeroing the W_Ref to correct for any drift.
    }
    else
    {
        Exc.Generation_Counter = 0;
    }

    if ( Exc.Generation_Counter == 0 )
    {
        EventModuleDisabled();
    }
    // else stay in state
}

//---------------------------------------------------------------------------------------------------------------------
/**
 *  @brief      Performs the necessary tasks to disable the algorithm.
 */
static void ModuleDisabling(void)
{
    if ( Exc.Handler_Down_Sample_Counter == 0 )
    {
        Exc.Handler_Down_Sample_Counter = INPUT_HANDLER_SLOT_PERIODS;
        DecrementGenerationCounter();
        if ( Exc.Generation_Counter == 0 )
        {
            EventModuleDisabled();
        }
        // else Exc.Generation_Counter > 0, do nothing
        OutputSpeedReferenceGeneratorOffset();
    }
    // else Exc.Handler_Down_Sample_Counter > 0, do nothing
}

//---------------------------------------------------------------------------------------------------------------------
/**
 *  @brief      Monitors the rpm and re-enables the module when the rpm is greater than the minimum.
 */
static void CheckRpmRange(void)
{
    if ( Exc.Api_Command == API_CMD_DISABLE )
    {
        EventDisable();
    }
    else if ( fabsf(INPUT_MOTOR_SPEED()) >= fabsf(INPUT_MIN_SPEED) )
    {
        EventRpmInRange();
    }
    // else stay in state, min speed not met
}


//=====================================================================================================================
//=== StateEngineCrank (v1.5) Auto-Generated Functions for Parameter Estimation Excitation State Machine
//=====================================================================================================================
// !! StateEngineCrank cannot update this code if you change the header !!


//---------------------------------------------------------------------------------------------------------------------
/**
 * Process the Ev Disable Event for the Parameter Estimation Excitation state machine.
 *
 *              >>> DO NOT MODIFY THIS FUNCTION <<<
 *
 * This function was auto-generated and must not be edited manually.
 * Change the UML used to generate this function and regenerate the source code.
 */
static void EventDisable(void)
{
    switch (Parameter_Estimation_Excitation_State)
    {
        case STATE_ACTIVE:
            Parameter_Estimation_Excitation_State = STATE_STOP;
            DisableModule();                        // STATE_STOP enter function.
            break;

        case STATE_RPM_OUT_OF_RANGE:
            Parameter_Estimation_Excitation_State = STATE_STOP;
            DisableModule();                        // STATE_STOP enter function.
            break;

        case STATE_STOPPED_FOR_RPM_OUT_OF_RANGE:
            Parameter_Estimation_Excitation_State = STATE_IDLE;
            break;

        default:
            // Event is ignored for all other values of Parameter_Estimation_Excitation_State.
            break;
    }
}


//---------------------------------------------------------------------------------------------------------------------
/**
 * Process the Ev Enable Event for the Parameter Estimation Excitation state machine.
 *
 *              >>> DO NOT MODIFY THIS FUNCTION <<<
 *
 * This function was auto-generated and must not be edited manually.
 * Change the UML used to generate this function and regenerate the source code.
 */
static void EventEnable(void)
{
    switch (Parameter_Estimation_Excitation_State)
    {
        case STATE_IDLE:
            Parameter_Estimation_Excitation_State = STATE_ACTIVE;
            EnableModule();                         // STATE_ACTIVE enter function.
            break;

        default:
            // Event is ignored for all other values of Parameter_Estimation_Excitation_State.
            break;
    }
}


//---------------------------------------------------------------------------------------------------------------------
/**
 * Process the Ev Module Disabled Event for the Parameter Estimation Excitation state machine.
 *
 *              >>> DO NOT MODIFY THIS FUNCTION <<<
 *
 * This function was auto-generated and must not be edited manually.
 * Change the UML used to generate this function and regenerate the source code.
 */
static void EventModuleDisabled(void)
{
    switch (Parameter_Estimation_Excitation_State)
    {
        case STATE_RPM_OUT_OF_RANGE:
            Parameter_Estimation_Excitation_State = STATE_STOPPED_FOR_RPM_OUT_OF_RANGE;
            break;

        case STATE_STOP:
            Parameter_Estimation_Excitation_State = STATE_IDLE;
            break;

        default:
            // Event is ignored for all other values of Parameter_Estimation_Excitation_State.
            break;
    }
}


//---------------------------------------------------------------------------------------------------------------------
/**
 * Process the Ev Rpm In Range Event for the Parameter Estimation Excitation state machine.
 *
 *              >>> DO NOT MODIFY THIS FUNCTION <<<
 *
 * This function was auto-generated and must not be edited manually.
 * Change the UML used to generate this function and regenerate the source code.
 */
static void EventRpmInRange(void)
{
    switch (Parameter_Estimation_Excitation_State)
    {
        case STATE_STOPPED_FOR_RPM_OUT_OF_RANGE:
            Parameter_Estimation_Excitation_State = STATE_ACTIVE;
            EnableModule();                         // STATE_ACTIVE enter function.
            break;

        default:
            // Event is ignored for all other values of Parameter_Estimation_Excitation_State.
            break;
    }
}


//---------------------------------------------------------------------------------------------------------------------
/**
 * Process the Ev Rpm Out Of Range Event for the Parameter Estimation Excitation state machine.
 *
 *              >>> DO NOT MODIFY THIS FUNCTION <<<
 *
 * This function was auto-generated and must not be edited manually.
 * Change the UML used to generate this function and regenerate the source code.
 */
static void EventRpmOutOfRange(void)
{
    switch (Parameter_Estimation_Excitation_State)
    {
        case STATE_ACTIVE:
            Parameter_Estimation_Excitation_State = STATE_RPM_OUT_OF_RANGE;
            DisableModule();                        // STATE_RPM_OUT_OF_RANGE enter function.
            break;

        default:
            // Event is ignored for all other values of Parameter_Estimation_Excitation_State.
            break;
    }
}


//---------------------------------------------------------------------------------------------------------------------
/**
 * The main execution function for the Parameter Estimation Excitation state machine.
 * This function should be called periodically while the state machine is running.
 *
 *              >>> DO NOT MODIFY THIS FUNCTION <<<
 *
 * This function was auto-generated and must not be edited manually.
 * Change the UML used to generate this function and regenerate the source code.
 */
static void ParameterEstimationExcitationExecute(void)
{
    switch (Parameter_Estimation_Excitation_State)
    {
        case STATE_IDLE:
            MonitorApiCommand();                    // STATE_IDLE do function.
            break;

        case STATE_ACTIVE:
            ModuleActive();                         // STATE_ACTIVE do function.
            break;

        case STATE_RPM_OUT_OF_RANGE:
            ModuleDisabling();                      // STATE_RPM_OUT_OF_RANGE do function.
            break;

        case STATE_STOP:
            ModuleDisabling();                      // STATE_STOP do function.
            break;

        case STATE_STOPPED_FOR_RPM_OUT_OF_RANGE:
            CheckRpmRange();                        // STATE_STOPPED_FOR_RPM_OUT_OF_RANGE do function.
            break;

        default:
            // There is nothing to do for all other values of Parameter_Estimation_Excitation_State.
            break;
    }
}


//---------------------------------------------------------------------------------------------------------------------
/**
 * The initialization function for the Parameter Estimation Excitation state machine.
 * This function resets the state machine to the initial state.
 *
 *              >>> DO NOT MODIFY THIS FUNCTION <<<
 *
 * This function was auto-generated and must not be edited manually.
 * Change the UML used to generate this function and regenerate the source code.
 */
static void ParameterEstimationExcitationInitialize(void)
{
    Parameter_Estimation_Excitation_State = STATE_IDLE;
}


// !! StateEngineCrank cannot update this code if you change the footer !!
// End of StateEngineCrank Auto-Generated Code.
