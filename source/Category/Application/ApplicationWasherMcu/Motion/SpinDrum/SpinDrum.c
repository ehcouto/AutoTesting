/**
 *  @file
 *
 *  @brief      Source file for the SpinDrum module.
 *
 *  @details    Handles interfacing to the Mci module and spinning the drum.
 *              Module is designed to be a child module executed by a parent.
 *

    @startuml{SpinDrum_sequence_diagram.png}
        Title SpinDrum Sequence Diagram

        participant env as "Parent"
        participant module as "SpinDrum"
        participant mci as "Mci Module"

        activate env
        env -> module : SpinDrum__Entry(drum_speed, drum_ramp)
        activate module
        module -> mci : Mci__Run(drum_speed x drive_ratio, motor_ramp x drive_ratio)
        activate mci
        deactivate env
        deactivate module
        deactivate mci

        loop executed by the Parent
            env -> module : SpinDrum__Do()
            activate env
            activate module
            alt state == WaitForMciToRotate
                module -> mci : Mci__GetDigital(MCI_DI_MOTOR_RUNNING)
                activate mci
                module <-- mci : MCI_DI_MOTOR_RUNNING
                deactivate mci
                alt MCI_DI_MOTOR_RUNNING == 0
                    env <-- module : SPIN_DRUM_EV_NONE
                else MCI_DI_MOTOR_RUNNING != 0
                    env <-- module : SPIN_DRUM_EV_IS_ROTATING
                end
            else if state == RampingToTargetSpeed
                module -> mci : Mci__GetAnalog(MCI_AI_ACTUAL_SPEED)
                activate mci
                module <-- mci : MCI_AI_ACTUAL_SPEED
                deactivate mci
                alt MCI_AI_ACTUAL_SPEED x drive_ratio < drum_speed
                    env <-- module : SPIN_MOTOR_EV_NONE
                else MCI_AI_ACTUAL_SPEED x drive_ratio == drum_speed
                    env <-- module : SPIN_MOTOR_EV_TARGET_RPM_REACHED
                end
            else state == Plateau
                env <-- module : SPIN_EV_NONE
                deactivate module
                deactivate env
            end
        end
    @enduml

    @startuml{SpinDrum_state_machine.png}
        title SpinDrum State Machine

        [*] --> SendCommandMciRun

        SendCommandMciRun --> AccelForTimeWaitForMciToRotate            : evAccelForTime
        SendCommandMciRun --> ConstantAccelerationWaitForMciToRotate    : evConstantAccel
        SendCommandMciRun --> [*]                                       : evCannotRunProfile \n/ ReturnEvError()

        AccelForTimeWaitForMciToRotate : do / VerifyIfMciIsRotating()
        AccelForTimeWaitForMciToRotate : exit / SetReturnEvIsRotating()
        AccelForTimeWaitForMciToRotate --> AccelForTime : evMciRotating

        ConstantAccelerationWaitForMciToRotate : do / VerifyIfMciIsRotating()
        ConstantAccelerationWaitForMciToRotate : exit / SetReturnEvIsRotating()
        ConstantAccelerationWaitForMciToRotate --> ConstantAcceleration : evMciRotating

        AccelForTime : do /\t DecrementTimer()
        AccelForTime : exit / SetReturnEvRampFinished()
        AccelForTime --> Plateau    : evTimerExpired

        ConstantAcceleration : do / VerifyIfTargetMciRpmReached()
        ConstantAcceleration : exit / SetReturnEvRampFinished()
        ConstantAcceleration --> Plateau : evTargetRpmReached

    @enduml

 *
 *  $Header: SpinDrum.c 1.8 2015/09/28 15:22:07EDT Paul F Spindler (SPINDPF) Exp  $
 *
 *  @copyright  Copyright 2015-$Date: 2015/09/28 15:22:07EDT $. Whirlpool Corporation. All rights reserved - CONFIDENTIAL
 */
//---------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------


//-------------------------------------- Include Files ----------------------------------------------------------------
#include "C_Extensions.h"

#include "SpinDrum.h"

#include "Log.h"
#include "MciSensorsWm.h"

#include <stdlib.h>
#include <string.h>

//=====================================================================================================================
//=== StateEngineCrank (v2.7) Auto-Generated Definitions for Spin Drum State Machine
//=====================================================================================================================
// !! StateEngineCrank cannot update this code if you change the header !!

//! An acceleration to use when the specified or calculated acceleration is 0.
#define DEFAULT_ACCELERATION 40

//! Unique names used to identify states of the Spin Drum state machine.
typedef enum SPIN_DRUM_STATE_ENUM
{
    STATE_SPIN_DRUM_NONE                                         = 0,

    STATE_SPIN_DRUM_SEND_COMMAND_MCI_RUN                         = 1, //!< (initial_state)
    STATE_SPIN_DRUM_ACCEL_FOR_TIME_WAIT_FOR_MCI_TO_ROTATE        = 2,
    STATE_SPIN_DRUM_CONSTANT_ACCELERATION_WAIT_FOR_MCI_TO_ROTATE = 3,
    STATE_SPIN_DRUM_ACCEL_FOR_TIME                               = 4,
    STATE_SPIN_DRUM_CONSTANT_ACCELERATION                        = 5,
    STATE_SPIN_DRUM_PLATEAU                                      = 6,

    STATE_SPIN_DRUM_END                                          = 7
} SPIN_DRUM_STATE_TYPE;

//! The state variable for the Spin Drum state machine.
//! State machines with a history pseudostate may write to this variable during initialization.
//! Other state machines must only write to this variable through auto-generated functions.
static SPIN_DRUM_STATE_TYPE Spin_Drum_State;

// -- Private Function Prototypes used by the state machine for customization functions --
static void DecrementTimer(void);
static void ReturnEvError(void);
static void SetReturnEvIsRotating(void);
static void SetReturnEvRampFinished(void);
static void VerifyIfMciIsRotating(void);
static void VerifyIfTargetMciRpmReached(void);

// -- Private Function Prototypes used by the state machine for standard state machine functions --
static void EventAccelForTime(void);
static void EventCannotRunProfile(void);
static void EventConstantAccel(void);
static void EventMciRotating(void);
static void EventTargetRpmReached(void);
static void EventTimerExpired(void);
static void SpinDrumExecute(void);
static void SpinDrumInitialize(void);

// !! StateEngineCrank cannot update this code if you change the footer !!
// End of StateEngineCrank Auto-Generated Code.

//-------------------------------------- PRIVATE (Variables, Constants & Defines) -------------------------------------
#ifndef SPINDRUM_MAX_NEGATIVE_DRUM_SPEED
#define SPINDRUM_MAX_NEGATIVE_DRUM_SPEED         -1
#elif (SPINDRUM_MAX_NEGATIVE_DRUM_SPEED < 0)
#error  "SPINDRUM_MAX_NEGATIVE_DRUM_SPEED must be equal or higher than 0 (it will be considered as -SPINDRUM_MAX_NEGATIVE_DRUM_SPEED)"
#endif

#ifndef SPINDRUM_MAX_START_DRUM_SPEED
#define SPINDRUM_MAX_START_DRUM_SPEED         -1
#elif (SPINDRUM_MAX_START_DRUM_SPEED < 0)
#error  "SPINDRUM_MAX_START_DRUM_SPEED must be equal or higher than 0."
#endif


//! list of module Log messages
typedef enum
{
    MESSAGE_NONE = 0,
    INVALID_ACCEL_TYPE,                 //!< data: index of invalid accel type
    INVALID_DRUM_ACCELERATION_TIME,     //!< data: na
    MCI_REJECTED_COMMAND_TO_RUN,        //!< data: mci response
    FORCED_DEFAULT_ACCELERATION,        //!< data: 0 = Specified Accel, 1 = Calculated Accel
    LIMITED_TO_MAX_ACCELERATION         //!< data: 0 = Specified Accel, 1 = Calculated Accel
} MODULE_SPIN_DRUM_LOG_MESSAGE_ID_TYPE;
#define MODULE_NAME MODULE_SPIN_DRUM

//! A macro to convert milliseconds to ticks where one tick is the time between calls to the Motion module handler.
#define MS_TO_TICKS(ms)                             ((ms) / MOTION_HANDLER_EXECUTION_RATE)

/**
 *  Defines the +/- rpm from the target Target_Drum_Velocity for the module to verify it has reached the target speed
 *
 *  @return MCI_RPM_TYPE
 */
#define TARGET_RPM_REACHED_TOLERANCE  5

typedef sint32 MCI_RPM_TYPE;        //! variable type for the Mci__Run() argument speed_rpm
typedef sint32 MCI_RAMP_TYPE;       //! variable type for the Mci__Run() argument ramp_rpm_per_s

typedef uint16 SPIN_DRUM_TIMER_TYPE;    // 1 = 1 call of SpinDrum__Do()

//! list of module static variables
static struct
{
    struct
    {
        union
        {
            SPIN_DRUM_TIMER_TYPE    Timer;          // when Type == SPIN_DRUM_ACCELERATE_FOR_TIME, the time to accelerate for
            MCI_RPM_TYPE            Target_Mci_Rpm; // when Type == SPIN_DRUM_CONSTANT_ACCELERATION, the target mci velocity
        } Exit_Check;
    } Accel;
    SPIN_DRUM_EVENT_TYPE    Event_To_Return;    //!< event to return to the environment
} SpinDrum;

//-------------------------------------- PRIVATE (Function Prototypes) ------------------------------------------------

static void SendCommandToMciToRun(SPIN_DRUM_PROFILE_TYPE * control);

//=====================================================================================================================
//-------------------------------------- Public Functions -------------------------------------------------------------
//=====================================================================================================================

//---------------------------------------------------------------------------------------------------------------------
/**
 *  @brief  Executes the modules enter task/s.
 *  @param  control
 */
void SpinDrum__Entry(SPIN_DRUM_PROFILE_TYPE * control)
{
    memset(&SpinDrum, 0, sizeof(SpinDrum));
    SpinDrumInitialize();
    SendCommandToMciToRun(control);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 *  @brief  Executes the modules background task/s.
 *
 *  @return SPIN_DRUM_EVENT_TYPE
 */
SPIN_DRUM_EVENT_TYPE SpinDrum__Do(void)
{
    SpinDrumExecute();

    // define the function return value
    SPIN_DRUM_EVENT_TYPE ret_val = SPIN_DRUM_EV_NONE;
    if ( SPIN_DRUM_EV_NONE != SpinDrum.Event_To_Return )
    {
        ret_val = SpinDrum.Event_To_Return;
        SpinDrum.Event_To_Return = SPIN_DRUM_EV_NONE;       // clear the variable so the event is only returned once
    }
    return ( ret_val );
}


//=====================================================================================================================
//-------------------------------------- Private Functions ------------------------------------------------------------
//=====================================================================================================================


//---------------------------------------------------------------------------------------------------------------------
/**
 *  Sends the command to the Mci module to run.
 *
 *  @param  profile     pointer to the profile to run
 */
static void SendCommandToMciToRun(SPIN_DRUM_PROFILE_TYPE * control)
{
    MCI_CMD_RESPONSE_TYPE mci_response = MCI_CMD_ACCEPTED;
    MCI_RPM_TYPE current_mci_rpm = (MCI_RPM_TYPE) MciSensorsWm__GetAnalog(MOTOR0, MCISENSORSWM_AI_SPEED_REFERENCE_RPM_S16)>>16;
    MCI_RPM_TYPE target_mci_rpm = control->Target_Drum_Velocity;
    MCI_RAMP_TYPE mci_ramp;

    #if (SPINDRUM_MAX_NEGATIVE_DRUM_SPEED > 0)
        if (target_mci_rpm < (-SPINDRUM_MAX_NEGATIVE_DRUM_SPEED))
        {
            mci_response = MCI_CMD_SPEED_OUT_OF_RANGE;
        }
    #endif

    #if (SPINDRUM_MAX_START_DRUM_SPEED > 0)
        if (((BOOL_TYPE) Mci__GetDigital(MOTOR0, MCI_DI_IDLE)) == TRUE)
        {
            MCI_RPM_TYPE abs_rpm = (target_mci_rpm >0) ? target_mci_rpm : -target_mci_rpm;
            if (abs_rpm > SPINDRUM_MAX_START_DRUM_SPEED)
            {
                mci_response = MCI_CMD_SPEED_OUT_OF_RANGE;
            }
        }
    #endif

    if (mci_response == MCI_CMD_ACCEPTED)
    {
        switch ( control->Drum_Acceleration.Type )
        {
            default:
                LOG_ADD_ERROR(INVALID_ACCEL_TYPE, control->Drum_Acceleration.Type);
                EventCannotRunProfile();
                return;     // exit function
            case SPIN_DRUM_CONSTANT_ACCELERATION:
                mci_ramp = control->Drum_Acceleration.Value.Constant;
                mci_ramp *= MciSensorsWm__GetTransmissionRatio();
                SpinDrum.Accel.Exit_Check.Target_Mci_Rpm = target_mci_rpm;
                SpinDrum.Accel.Exit_Check.Target_Mci_Rpm *= MciSensorsWm__GetTransmissionRatio();
                break;
            case SPIN_DRUM_ACCELERATE_FOR_TIME:
                /* calculate the Mci ramp rate */
                if (control->Drum_Acceleration.Value.Time > 0)
                {
                    /* calculate the Mci ramp rate */
                    mci_ramp = 1000L * abs(target_mci_rpm - current_mci_rpm);
                    mci_ramp /= control->Drum_Acceleration.Value.Time;
                    mci_ramp *= MciSensorsWm__GetTransmissionRatio();
                    /* calculate the number of execution ticks for the time */
                    SpinDrum.Accel.Exit_Check.Timer = MS_TO_TICKS(control->Drum_Acceleration.Value.Time);
                }
                else
                {
                    mci_ramp = MAX_RPM_PER_SEC_ACCEL;
                }

                // Set minimum Check Timer to 1
                if (SpinDrum.Accel.Exit_Check.Timer == 0)
                {
                    SpinDrum.Accel.Exit_Check.Timer = 1;
                }
                break;
        } // end switch

        target_mci_rpm *= MciSensorsWm__GetTransmissionRatio();
        mci_ramp = abs(mci_ramp); // Acceleration is always a positive number

        // Fix a calculation error in the acceleration.
        if (mci_ramp < MIN_RPM_PER_SEC_ACCEL)
        {
            mci_ramp = DEFAULT_ACCELERATION;
            LOG_ADD_EXCEPTION(FORCED_DEFAULT_ACCELERATION, control->Drum_Acceleration.Type);
        }
        else if (mci_ramp > MAX_RPM_PER_SEC_ACCEL)
        {
            mci_ramp = MAX_RPM_PER_SEC_ACCEL;
            LOG_ADD_EXCEPTION(LIMITED_TO_MAX_ACCELERATION, control->Drum_Acceleration.Type);
        }

	    // send the command to the Mci module to run
	    mci_response = Mci__Run(MOTOR0, target_mci_rpm, mci_ramp);
	}
	
    switch ( mci_response )
    {
        case MCI_CMD_ACCEPTED:

            MOTION__POSTPROCESS_RUN();

            if ( SPIN_DRUM_ACCELERATE_FOR_TIME == control->Drum_Acceleration.Type )
            {
                EventAccelForTime();
            }
            else // SPIN_DRUM_CONSTANT_ACCELERATION == control->Drum_Acceleration.Type
            {
                EventConstantAccel();
            }
            break;
        default:
            LOG_ADD_ERROR(MCI_REJECTED_COMMAND_TO_RUN, mci_response);
            EventCannotRunProfile();
            break;
    }
}



//=====================================================================================================================
//  --- Private State Machine Customization Functions -----------------------------------------------------------------
//=====================================================================================================================

//---------------------------------------------------------------------------------------------------------------------
/**
 *
 */
static void DecrementTimer(void)
{
    SpinDrum.Accel.Exit_Check.Timer = 0 < SpinDrum.Accel.Exit_Check.Timer ? SpinDrum.Accel.Exit_Check.Timer - 1 : 0;
    if ( 0 == SpinDrum.Accel.Exit_Check.Timer )
    {
        EventTimerExpired();
    }
    // else do nothing
}


//---------------------------------------------------------------------------------------------------------------------
/**
 *
 */
static void ReturnEvError(void)
{
    SpinDrum.Event_To_Return = SPIN_DRUM_EV_ERROR;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 *
 */
static void SetReturnEvIsRotating(void)
{
    SpinDrum.Event_To_Return = SPIN_DRUM_EV_IS_ROTATING;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 *
 */
static void SetReturnEvRampFinished(void)
{
    SpinDrum.Event_To_Return = SPIN_DRUM_EV_RAMP_FINISHED;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 *
 */
static void VerifyIfMciIsRotating(void)
{
    if ( 0 != Mci__GetDigital(MOTOR0, MCI_DI_MOTOR_RUNNING) )
    {
        EventMciRotating();
    }
}


//---------------------------------------------------------------------------------------------------------------------
/**
 *
 */
static void VerifyIfTargetMciRpmReached(void)
{
    MCI_RPM_TYPE mci_rpm = Mci__GetAnalog(MOTOR0, MCI_AI_ACTUAL_SPEED_S16)>>16;
    if (( mci_rpm <= SpinDrum.Accel.Exit_Check.Target_Mci_Rpm + TARGET_RPM_REACHED_TOLERANCE ) &&
        ( mci_rpm >= SpinDrum.Accel.Exit_Check.Target_Mci_Rpm - TARGET_RPM_REACHED_TOLERANCE ))
    {
        EventTargetRpmReached();
    }
    // else target rpm not reached
}



//=====================================================================================================================
//=== StateEngineCrank (v2.7) Auto-Generated Functions for Spin Drum State Machine
//=====================================================================================================================
// !! StateEngineCrank cannot update this code if you change the header !!


//---------------------------------------------------------------------------------------------------------------------
/**
 * Process the Accel For Time Event for the Spin Drum state machine.
 *
 *              >>> DO NOT MODIFY THIS FUNCTION <<<
 *
 * This function was auto-generated and must not be edited manually.
 * Change the UML used to generate this function and regenerate the source code.
 */
static void EventAccelForTime(void)
{
    switch (Spin_Drum_State)
    {
        case STATE_SPIN_DRUM_SEND_COMMAND_MCI_RUN:
            Spin_Drum_State = STATE_SPIN_DRUM_ACCEL_FOR_TIME_WAIT_FOR_MCI_TO_ROTATE;
            break;

        default:
            // Event is ignored for all other values of Spin_Drum_State.
            break;
    }
}


//---------------------------------------------------------------------------------------------------------------------
/**
 * Process the Cannot Run Profile Event for the Spin Drum state machine.
 *
 *              >>> DO NOT MODIFY THIS FUNCTION <<<
 *
 * This function was auto-generated and must not be edited manually.
 * Change the UML used to generate this function and regenerate the source code.
 */
static void EventCannotRunProfile(void)
{
    switch (Spin_Drum_State)
    {
        case STATE_SPIN_DRUM_SEND_COMMAND_MCI_RUN:
            ReturnEvError();                        // Transition action function.
            Spin_Drum_State = STATE_SPIN_DRUM_NONE; // Terminate Spin Drum state machine execution.
            break;

        default:
            // Event is ignored for all other values of Spin_Drum_State.
            break;
    }
}


//---------------------------------------------------------------------------------------------------------------------
/**
 * Process the Constant Accel Event for the Spin Drum state machine.
 *
 *              >>> DO NOT MODIFY THIS FUNCTION <<<
 *
 * This function was auto-generated and must not be edited manually.
 * Change the UML used to generate this function and regenerate the source code.
 */
static void EventConstantAccel(void)
{
    switch (Spin_Drum_State)
    {
        case STATE_SPIN_DRUM_SEND_COMMAND_MCI_RUN:
            Spin_Drum_State = STATE_SPIN_DRUM_CONSTANT_ACCELERATION_WAIT_FOR_MCI_TO_ROTATE;
            break;

        default:
            // Event is ignored for all other values of Spin_Drum_State.
            break;
    }
}


//---------------------------------------------------------------------------------------------------------------------
/**
 * Process the Mci Rotating Event for the Spin Drum state machine.
 *
 *              >>> DO NOT MODIFY THIS FUNCTION <<<
 *
 * This function was auto-generated and must not be edited manually.
 * Change the UML used to generate this function and regenerate the source code.
 */
static void EventMciRotating(void)
{
    switch (Spin_Drum_State)
    {
        case STATE_SPIN_DRUM_ACCEL_FOR_TIME_WAIT_FOR_MCI_TO_ROTATE:
            SetReturnEvIsRotating();                // Exit function.
            Spin_Drum_State = STATE_SPIN_DRUM_ACCEL_FOR_TIME;
            break;

        case STATE_SPIN_DRUM_CONSTANT_ACCELERATION_WAIT_FOR_MCI_TO_ROTATE:
            SetReturnEvIsRotating();                // Exit function.
            Spin_Drum_State = STATE_SPIN_DRUM_CONSTANT_ACCELERATION;
            break;

        default:
            // Event is ignored for all other values of Spin_Drum_State.
            break;
    }
}


//---------------------------------------------------------------------------------------------------------------------
/**
 * Process the Target Rpm Reached Event for the Spin Drum state machine.
 *
 *              >>> DO NOT MODIFY THIS FUNCTION <<<
 *
 * This function was auto-generated and must not be edited manually.
 * Change the UML used to generate this function and regenerate the source code.
 */
static void EventTargetRpmReached(void)
{
    switch (Spin_Drum_State)
    {
        case STATE_SPIN_DRUM_CONSTANT_ACCELERATION:
            SetReturnEvRampFinished();              // Exit function.
            Spin_Drum_State = STATE_SPIN_DRUM_PLATEAU;
            break;

        default:
            // Event is ignored for all other values of Spin_Drum_State.
            break;
    }
}


//---------------------------------------------------------------------------------------------------------------------
/**
 * Process the Timer Expired Event for the Spin Drum state machine.
 *
 *              >>> DO NOT MODIFY THIS FUNCTION <<<
 *
 * This function was auto-generated and must not be edited manually.
 * Change the UML used to generate this function and regenerate the source code.
 */
static void EventTimerExpired(void)
{
    switch (Spin_Drum_State)
    {
        case STATE_SPIN_DRUM_ACCEL_FOR_TIME:
            SetReturnEvRampFinished();              // Exit function.
            Spin_Drum_State = STATE_SPIN_DRUM_PLATEAU;
            break;

        default:
            // Event is ignored for all other values of Spin_Drum_State.
            break;
    }
}


//---------------------------------------------------------------------------------------------------------------------
/**
 * The main execution function for the Spin Drum state machine.
 * This function should be called periodically while the state machine is running.
 *
 *              >>> DO NOT MODIFY THIS FUNCTION <<<
 *
 * This function was auto-generated and must not be edited manually.
 * Change the UML used to generate this function and regenerate the source code.
 */
static void SpinDrumExecute(void)
{
    switch(Spin_Drum_State)
    {
        case STATE_SPIN_DRUM_ACCEL_FOR_TIME_WAIT_FOR_MCI_TO_ROTATE:
        case STATE_SPIN_DRUM_CONSTANT_ACCELERATION_WAIT_FOR_MCI_TO_ROTATE:
            VerifyIfMciIsRotating();                // Do function.
            break;

        case STATE_SPIN_DRUM_ACCEL_FOR_TIME:
            DecrementTimer();                       // Do function.
            break;

        case STATE_SPIN_DRUM_CONSTANT_ACCELERATION:
            VerifyIfTargetMciRpmReached();          // Do function.
            break;

        default:
            // There is nothing to do for all other values of Spin_Drum_State.
            break;
    }
}


//---------------------------------------------------------------------------------------------------------------------
/**
 * The initialization function for the Spin Drum state machine.
 * This function resets the state machine to the initial state.
 *
 *              >>> DO NOT MODIFY THIS FUNCTION <<<
 *
 * This function was auto-generated and must not be edited manually.
 * Change the UML used to generate this function and regenerate the source code.
 */
static void SpinDrumInitialize(void)
{
    Spin_Drum_State = STATE_SPIN_DRUM_SEND_COMMAND_MCI_RUN;
}


// !! StateEngineCrank cannot update this code if you change the footer !!
// End of StateEngineCrank Auto-Generated Code.
