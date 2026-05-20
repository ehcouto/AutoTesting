/**
 *  @file
 *
 *  @brief      Source file for the Pulse module.
 *
 *  @details    Handles controlling a pulse.
 *

    @startuml{Pulse_sequence_diagram.jpg}
        title Pulse Sequence Diagram

        Motion -> Pulse : Pulse__Initialize()
        Motion -> Pulse : Pulse__Entry(profile)

        loop execute the Pulse module
            alt write a new pulse to the queue
                Motion -> Pulse : Pulse__Entry(profile)
            end
            Motion -> Pulse : Pulse__Do()
            alt  indicates all the profiles in the queue have been run
                activate Pulse
                activate Motion
                Motion <-- Pulse : PULSE_EV_SELF_TERMINATED
            else the module cannot perform its intended functionality
                Motion <-- Pulse : PULSE_EV_ERROR
            else no event to report
                Motion <-- Pulse : PULSE_EV_NONE
                deactivate Pulse
                deactivate Motion
            end
        end
    @enduml


    @startuml{Pulse_state_machine.jpg}
        title Pulse State Machine

        [*] --> TimeRamp

        TimeRamp : entry /\t LoadAndRunPulseFromQueue() \n\t\t PulseExecuting()
        TimeRamp : do /\t SpinDrumDo()
        TimeRamp --> [*]        : SPIN_DRUM\n(SPIN_DRUM_EV_ERROR) \n/ SetModuleEvError()
        TimeRamp --> TimeOn     : SPIN_DRUM\n(SPIN_DRUM_EV_RAMP_FINISHED)
        TimeRamp --> TimeRamp	: evRunPulseImmediately

        TimeOn : entry /\t ClearTimer()
        TimeOn : do /\t IncrementTimer() \n\t CheckIfTimeOnReached()
        TimeOn --> TimeRamp             : evRunPulseImmediately
        TimeOn --> TimeRamp             : evTimeOnReached \n [IsTimeOffZero() && \n !IsQueueEmpty()]
        TimeOn --> [*]                  : evTimeOnReached \n [IsTimeOffZero() && \n IsQueueEmpty()] \n/ SetModuleEvSelfTerminated()
        TimeOn --> TimeOffDeceleration  : evTimeOnReached \n [!IsTimeOffZero()]

        TimeOffDeceleration : entry /\t ClearTimer() \n\t\t StopDrumEntry()
        TimeOffDeceleration : do /\t IncrementTimer() \n\t StopDrumDo()
        TimeOffDeceleration --> TimeOffDrumStopped  : STOP_DRUM\n(STOP_DRUM_EV_STOPPED) \n [!IsQueueBehaviorRunImmediately()]
        TimeOffDeceleration --> TimeRamp			: STOP_DRUM\n(STOP_DRUM_EV_STOPPED) \n [IsQueueBehaviorRunImmediately()]

        TimeOffDrumStopped : do /\t IncrementTimer() \n\t CheckIfTimeOffReached()
        TimeOffDrumStopped --> [*]      : evTimeOffReached \n [IsQueueEmpty()] \n/ SetModuleEvSelfTerminated()
        TimeOffDrumStopped --> TimeRamp : evTimeOffReached \n [!IsQueueEmpty()]
        TimeOffDrumStopped --> TimeRamp	: evRunPulseImmediately

    @enduml

 *
 *  $Header: $
 *
 *  @copyright  Copyright 2016-$Date$. Whirlpool Corporation. All rights reserved - CONFIDENTIAL
 */
//---------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------


//-------------------------------------- Include Files ----------------------------------------------------------------
#include "SystemConfig.h"
#include "Pulse.h"

#include "API221MotionCtrl.h"
#include <limits.h>
#include "Log.h"
#include "MotionDeclare.h"
#include "SpinDrum.h"
#include "StopDrum.h"
#include <string.h>


//=====================================================================================================================
//=== StateEngineCrank (v2.8) Auto-Generated Definitions for Pulse State Machine
//=====================================================================================================================
// !! StateEngineCrank cannot update this code if you change the header !!


//! Unique names used to identify states of the Pulse state machine.
typedef enum PULSE_STATE_ENUM
{
    STATE_PULSE_NONE                  = 0,

    STATE_PULSE_TIME_RAMP             = 1,          //!< (initial_state)
    STATE_PULSE_TIME_ON               = 2,
    STATE_PULSE_TIME_OFF_DECELERATION = 3,
    STATE_PULSE_TIME_OFF_DRUM_STOPPED = 4,

    STATE_PULSE_END                   = 5
} PULSE_STATE_TYPE;

//! The state variable for the Pulse state machine.
//! State machines with a history pseudostate may write to this variable during initialization.
//! Other state machines must only write to this variable through auto-generated functions.
static PULSE_STATE_TYPE Pulse_State;

//! The transition variable for the Pulse state machine.
//! Flag used by the state machine to track when a transition occurs in the middle of a state's
//! Entry() or Do() action that contains more than one function.
static BOOL_TYPE Pulse_Transitioned;

// -- Private Function Prototypes used by the state machine for customization functions --
static void CheckIfTimeOffReached(void);
static void CheckIfTimeOnReached(void);
static void ClearTimer(void);
static void IncrementTimer(void);
static BOOL_TYPE IsQueueBehaviorRunImmediately(void);
static BOOL_TYPE IsQueueEmpty(void);
static BOOL_TYPE IsTimeOffZero(void);
static void LoadAndRunPulseFromQueue(void);
static void PulseExecuting(void);
static void SetModuleEvError(void);
static void SetModuleEvSelfTerminated(void);
static void SpinDrumDo(void);
static void StopDrumDo(void);
static void StopDrumEntry(void);

// -- Private Function Prototypes used by the state machine for standard state machine functions --
static void EventRunPulseImmediately(void);
static void EventTimeOffReached(void);
static void EventTimeOnReached(void);
static void ExternalEventSpinDrum(uint8 event);
static void ExternalEventStopDrum(uint8 event);
static void PulseExecute(void);
static void PulseInitialize(void);

// !! StateEngineCrank cannot update this code if you change the footer !!
// End of StateEngineCrank Auto-Generated Code.

//-------------------------------------- PRIVATE (Variables, Constants & Defines) -------------------------------------

typedef enum
{
    COMMAND_MODIFY_BEHAVIOR_IGNORED_STATE_INVALID = 1,  //!< data: index of current state machine state
    INVALID_BEHAVIOR                                    //!< data: invalid behavior index
} MODULE_PULSE_LOG_MESSAGE_ID_TYPE;
#define MODULE_NAME MODULE_PULSE


#define PULSE_BEHAVIOR_PROCESSED    ((PULSE_BEHAVIOR_TYPE)(NUM_OF_PULSE_BEHAVIORS + 1))

typedef uint32 PULSE_TIMER_TYPE;        // 1 = 1 execution of Pulse__Do()

//! list of module static variables
static struct
{
    struct                                  // the currently active Pulse parameters that must be stored in RAM
    {
        PULSE_TIMER_TYPE    Time_On;
        PULSE_TIMER_TYPE    Time_Off;
    } Active;
    PULSE_EVENT_TYPE    Module_Event;       // the event to return to the parent
    struct
    {                                       // the queue
        PULSE_PROFILE_TYPE  Profile;
    } Queue;
    PULSE_TIMER_TYPE    Timer;
} Pulse;

//-------------------------------------- PRIVATE (Function Prototypes) ------------------------------------------------

static void SetActiveTimesFromProfile(uint32 time_off, uint32 time_on);
static void WriteProfileToQueue(const PULSE_PROFILE_TYPE * profile);

//=====================================================================================================================
//-------------------------------------- Public Functions -------------------------------------------------------------
//=====================================================================================================================

//---------------------------------------------------------------------------------------------------------------------
/**
 *  @brief      It Initializes the module Pulse and its variables
 *
 */
void Pulse__Initialize(void)
{
    Pulse_State = STATE_PULSE_NONE;
}


//---------------------------------------------------------------------------------------------------------------------
/**
 *  @brief      Executes the modules entry task/s.
 *
 *  @param      profile     pointer to the pulse profile
 */
void Pulse__Entry(const PULSE_PROFILE_TYPE * profile)
{
    switch ( profile->Behavior )
    {
        default:
            LOG_ADD_EVENT(INVALID_BEHAVIOR, profile->Behavior);
            break;
        case PULSE_BEHAVIOR_LOAD_PULSE_INTO_QUEUE:
            WriteProfileToQueue(profile);
            if ( STATE_PULSE_NONE == Pulse_State )
            {
                // The module state machine is NOT active, enable it
                PulseInitialize();
            }
            // else the module state machine is active, do nothing
            break;
        case PULSE_BEHAVIOR_RUN_IMMEDIATELY:
            WriteProfileToQueue(profile);
            if ( STATE_PULSE_NONE == Pulse_State )
            {
                // The module state machine is NOT active, enable it
                PulseInitialize();
            }
            else // The module state machine is active
            {
                EventRunPulseImmediately();
            }
            break;
        case PULSE_BEHAVIOR_MODIFY_PULSE:
            if ( STATE_PULSE_NONE == Pulse_State )
            {
                LOG_ADD_EVENT(COMMAND_MODIFY_BEHAVIOR_IGNORED_STATE_INVALID, Pulse_State);
            }
            else // module state machine is currently active
            {
                SetActiveTimesFromProfile(profile->Time_Off, profile->Time_On);
            }
            break;
    } // end switch
}


//---------------------------------------------------------------------------------------------------------------------
/**
 *  Executes the modules background task/s.
 *
 *  @return     The module event
 */
PULSE_EVENT_TYPE Pulse__Do(void)
{
    PulseExecute();
    return ( STATE_PULSE_NONE == Pulse_State ? Pulse.Module_Event : PULSE_EV_NONE );
}

//=====================================================================================================================
//-------------------------------------- Private Functions ------------------------------------------------------------
//=====================================================================================================================

//---------------------------------------------------------------------------------------------------------------------
/**
 *  The profile time off and on are in milliseconds and must be converted to PULSE_TIMER_TYPE.
 *
 *  Since the Pulse module is a child module to Motion, the execution rate of the
 *  Pulse module is determined by the execution rate of the Motion module's background function.
 *
 *  @param  time_off    the profile Time_Off
 *  @param  time_on     the profile Time_On
 */
static void SetActiveTimesFromProfile(uint32 time_off, uint32 time_on)
{
    Pulse.Active.Time_Off   = time_off / MOTION_HANDLER_EXECUTION_RATE;
    Pulse.Active.Time_On    = time_on / MOTION_HANDLER_EXECUTION_RATE;
}


//---------------------------------------------------------------------------------------------------------------------
/**
 *  Write the profile to the queue.
 *
 *  @param      profile     pointer to the pulse profile
 */
static void WriteProfileToQueue(const PULSE_PROFILE_TYPE * profile)
{
    memcpy(&Pulse.Queue.Profile, profile, sizeof(PULSE_PROFILE_TYPE));
}

//=====================================================================================================================
//  --- Private State Machine Customization Functions -----------------------------------------------------------------
//=====================================================================================================================


//---------------------------------------------------------------------------------------------------------------------
/**
 *  Checks if the static variable Timer has reached the commanded Time_Off.
 */
static void CheckIfTimeOffReached(void)
{
    if ( Pulse.Timer >= Pulse.Active.Time_Off )
    {
        EventTimeOffReached();
    }
    // else do nothing
}


//---------------------------------------------------------------------------------------------------------------------
/**
 *  Checks if the static variable Timer has reached the commanded Time_On.
 */
static void CheckIfTimeOnReached(void)
{
    if ( Pulse.Timer >= Pulse.Active.Time_On )
    {
        EventTimeOnReached();
    }
    // else do nothing
}


//---------------------------------------------------------------------------------------------------------------------
/**
 *  Clears the static variable Timer.
 */
static void ClearTimer(void)
{
    Pulse.Timer = 0;
}


//---------------------------------------------------------------------------------------------------------------------
/**
 *  Increments the static variable Timer and prevents overflow.
 */
static void IncrementTimer(void)
{
    Pulse.Timer = Pulse.Timer < ULONG_MAX ? Pulse.Timer + 1 : ULONG_MAX;
}


//---------------------------------------------------------------------------------------------------------------------
/**
 *  Checks to pulse behavior in the queue to determine if the pulse will overwrite the current pulse or not.
 *
 * @return  TRUE = pulse in queue needs to overwrite the current active pulse; FALSE = continue running the active pulse
 */
static BOOL_TYPE IsQueueBehaviorRunImmediately(void)
{
    return ( PULSE_BEHAVIOR_RUN_IMMEDIATELY == Pulse.Queue.Profile.Behavior ? TRUE : FALSE );
}


//---------------------------------------------------------------------------------------------------------------------
/**
 *  Determines if another pulse has been loaded into the queue.
 *
 * @return  TRUE = queue is empty; FALSE = queue contains a pulse to be run
 */
static BOOL_TYPE IsQueueEmpty(void)
{
    return ( PULSE_BEHAVIOR_PROCESSED == Pulse.Queue.Profile.Behavior ? TRUE : FALSE );
}


//---------------------------------------------------------------------------------------------------------------------
/**
 *  Returns the status of the static variable Active.Time_Off
 *
 * @return  TRUE = off time of active pulse is zero; FALSE = off time of active pulse not zero
 */
static BOOL_TYPE IsTimeOffZero(void)
{
    return ( Pulse.Active.Time_Off == 0 ? TRUE : FALSE );
}


//---------------------------------------------------------------------------------------------------------------------
/**
 *  Runs the oldest pulse in the Queue.
 */
static void LoadAndRunPulseFromQueue(void)
{
    SPIN_DRUM_PROFILE_TYPE profile = {
        .Drum_Acceleration.Type             = SPIN_DRUM_ACCELERATE_FOR_TIME,
        .Drum_Acceleration.Value.Constant   = Pulse.Queue.Profile.Spin.Drum_Acceleration.Value.Constant,
        .Target_Drum_Velocity               = Pulse.Queue.Profile.Spin.Target_Drum_Velocity
    };
    SpinDrum__Entry(&profile);
    SetActiveTimesFromProfile(Pulse.Queue.Profile.Time_Off, Pulse.Queue.Profile.Time_On);
    Pulse.Queue.Profile.Behavior = PULSE_BEHAVIOR_PROCESSED;

}

//---------------------------------------------------------------------------------------------------------------------
/**
 *  Function in state machine for defining a pulse is now running.
 */
static void PulseExecuting(void)
{
    API221MotionCtrl__PostEvent(0, API221_EVENT_PULSE_RUNNING);
}


//---------------------------------------------------------------------------------------------------------------------
/**
 *  Sets the static variable Module_Event to indicate an error has occurred and the module cannot complete its intended
 *  functionality.
 */
static void SetModuleEvError(void)
{
    Pulse.Module_Event = PULSE_EV_ERROR;
}


//---------------------------------------------------------------------------------------------------------------------
/**
 *  Set the static variable Module_Event to indicate the module has successfully run all pulses in the Queue and the
 *  Queue is empty.
 */
static void SetModuleEvSelfTerminated(void)
{
    Pulse.Module_Event = PULSE_EV_SELF_TERMINATED;
}


//---------------------------------------------------------------------------------------------------------------------
/**
 *  Executes the child module SpinDrum background task/s and handles the module return event.
 */
static void SpinDrumDo(void)
{
    SPIN_DRUM_EVENT_TYPE event = SpinDrum__Do();
    if ( SPIN_DRUM_EV_NONE != event )
    {
        ExternalEventSpinDrum((uint8)event);
    }
}


//---------------------------------------------------------------------------------------------------------------------
/**
 *  Executes the child module StopDrum background task/s and handles the return module event.
 */
static void StopDrumDo(void)
{
    STOP_DRUM_EVENT_TYPE event = StopDrum__Do();
    if ( STOP_DRUM_EV_NONE != event )
    {
        ExternalEventStopDrum((uint8)event);
    }
}


//---------------------------------------------------------------------------------------------------------------------
/**
 *
 */
static void StopDrumEntry(void)
{
    STOP_DRUM_PROFILE_TYPE profile;
    profile.Drum_Deceleration = DEFAULT_STOP_DECELERATION;
    StopDrum__Entry(&profile);
}


//=====================================================================================================================
//=== StateEngineCrank (v2.8) Auto-Generated Functions for Pulse State Machine
//=====================================================================================================================
// !! StateEngineCrank cannot update this code if you change the header !!


//---------------------------------------------------------------------------------------------------------------------
/**
 * Process the Run Pulse Immediately Event for the Pulse state machine.
 *
 *              >>> DO NOT MODIFY THIS FUNCTION <<<
 *
 * This function was auto-generated and must not be edited manually.
 * Change the UML used to generate this function and regenerate the source code.
 */
static void EventRunPulseImmediately(void)
{
    switch (Pulse_State)
    {
        case STATE_PULSE_TIME_RAMP:
        case STATE_PULSE_TIME_ON:
        case STATE_PULSE_TIME_OFF_DRUM_STOPPED:
            Pulse_State = STATE_PULSE_TIME_RAMP;
            Pulse_Transitioned = FALSE;
            LoadAndRunPulseFromQueue();             // Entry Function.
            if (Pulse_Transitioned == FALSE)
            {
                PulseExecuting();                   // Entry Function.
            }
            Pulse_Transitioned = TRUE;
            break;

        default:
            // Event is ignored for all other values of Pulse_State.
            break;
    }
}


//---------------------------------------------------------------------------------------------------------------------
/**
 * Process the Time Off Reached Event for the Pulse state machine.
 *
 *              >>> DO NOT MODIFY THIS FUNCTION <<<
 *
 * This function was auto-generated and must not be edited manually.
 * Change the UML used to generate this function and regenerate the source code.
 */
static void EventTimeOffReached(void)
{
    switch (Pulse_State)
    {
        case STATE_PULSE_TIME_OFF_DRUM_STOPPED:
            if (IsQueueEmpty() == TRUE)
            {
                SetModuleEvSelfTerminated();        // Transition action function.
                Pulse_State = STATE_PULSE_NONE;     // Terminate Pulse state machine execution.
                Pulse_Transitioned = TRUE;
            }
            else if (IsQueueEmpty() == FALSE)
            {
                Pulse_State = STATE_PULSE_TIME_RAMP;
                Pulse_Transitioned = FALSE;
                LoadAndRunPulseFromQueue();         // Entry Function.
                if (Pulse_Transitioned == FALSE)
                {
                    PulseExecuting();               // Entry Function.
                }
                Pulse_Transitioned = TRUE;
            }
            break;

        default:
            // Event is ignored for all other values of Pulse_State.
            break;
    }
}


//---------------------------------------------------------------------------------------------------------------------
/**
 * Process the Time On Reached Event for the Pulse state machine.
 *
 *              >>> DO NOT MODIFY THIS FUNCTION <<<
 *
 * This function was auto-generated and must not be edited manually.
 * Change the UML used to generate this function and regenerate the source code.
 */
static void EventTimeOnReached(void)
{
    switch (Pulse_State)
    {
        case STATE_PULSE_TIME_ON:
            if ((IsTimeOffZero() == TRUE) &&
                (IsQueueEmpty() == FALSE))
            {
                Pulse_State = STATE_PULSE_TIME_RAMP;
                Pulse_Transitioned = FALSE;
                LoadAndRunPulseFromQueue();         // Entry Function.
                if (Pulse_Transitioned == FALSE)
                {
                    PulseExecuting();               // Entry Function.
                }
                Pulse_Transitioned = TRUE;
            }
            else if ((IsTimeOffZero() == TRUE) &&
                     (IsQueueEmpty() == TRUE))
            {
                SetModuleEvSelfTerminated();        // Transition action function.
                Pulse_State = STATE_PULSE_NONE;     // Terminate Pulse state machine execution.
                Pulse_Transitioned = TRUE;
            }
            else if (IsTimeOffZero() == FALSE)
            {
                Pulse_State = STATE_PULSE_TIME_OFF_DECELERATION;
                Pulse_Transitioned = FALSE;
                ClearTimer();                       // Entry Function.
                if (Pulse_Transitioned == FALSE)
                {
                    StopDrumEntry();                // Entry Function.
                }
                Pulse_Transitioned = TRUE;
            }
            break;

        default:
            // Event is ignored for all other values of Pulse_State.
            break;
    }
}


//---------------------------------------------------------------------------------------------------------------------
/**
 * Process an external Spin Drum event.
 *
 *              >>> DO NOT MODIFY THIS FUNCTION <<<
 *
 * This function was auto-generated and must not be edited manually.
 * Change the UML used to generate this function and regenerate the source code.
 *
 * @param event = The external event to be processed.
 */
static void ExternalEventSpinDrum(uint8 event)
{
    switch (event)
    {
        case SPIN_DRUM_EV_ERROR:
            switch (Pulse_State)
            {
                case STATE_PULSE_TIME_RAMP:
                    SetModuleEvError();             // Transition action function.
                    Pulse_State = STATE_PULSE_NONE; // Terminate Pulse state machine execution.
                    Pulse_Transitioned = TRUE;
                    break;

                default:
                    // Event is ignored for all other values of Pulse_State.
                    break;
            }
            break;

        case SPIN_DRUM_EV_RAMP_FINISHED:
            switch (Pulse_State)
            {
                case STATE_PULSE_TIME_RAMP:
                    Pulse_State = STATE_PULSE_TIME_ON;
                    ClearTimer();                   // Entry Function.
                    Pulse_Transitioned = TRUE;
                    break;

                default:
                    // Event is ignored for all other values of Pulse_State.
                    break;
            }
            break;

        default:
            // All other external Spin Drum events are ignored by the Pulse state machine.
            break;
    }
}


//---------------------------------------------------------------------------------------------------------------------
/**
 * Process an external Stop Drum event.
 *
 *              >>> DO NOT MODIFY THIS FUNCTION <<<
 *
 * This function was auto-generated and must not be edited manually.
 * Change the UML used to generate this function and regenerate the source code.
 *
 * @param event = The external event to be processed.
 */
static void ExternalEventStopDrum(uint8 event)
{
    switch (event)
    {
        case STOP_DRUM_EV_STOPPED:
            switch (Pulse_State)
            {
                case STATE_PULSE_TIME_OFF_DECELERATION:
                    if (IsQueueBehaviorRunImmediately() == FALSE)
                    {
                        Pulse_State = STATE_PULSE_TIME_OFF_DRUM_STOPPED;
                        Pulse_Transitioned = TRUE;
                    }
                    else if (IsQueueBehaviorRunImmediately() == TRUE)
                    {
                        Pulse_State = STATE_PULSE_TIME_RAMP;
                        Pulse_Transitioned = FALSE;
                        LoadAndRunPulseFromQueue(); // Entry Function.
                        if (Pulse_Transitioned == FALSE)
                        {
                            PulseExecuting();       // Entry Function.
                        }
                        Pulse_Transitioned = TRUE;
                    }
                    break;

                default:
                    // Event is ignored for all other values of Pulse_State.
                    break;
            }
            break;

        default:
            // All other external Stop Drum events are ignored by the Pulse state machine.
            break;
    }
}


//---------------------------------------------------------------------------------------------------------------------
/**
 * The main execution function for the Pulse state machine.
 * This function should be called periodically while the state machine is running.
 *
 *              >>> DO NOT MODIFY THIS FUNCTION <<<
 *
 * This function was auto-generated and must not be edited manually.
 * Change the UML used to generate this function and regenerate the source code.
 */
static void PulseExecute(void)
{
    Pulse_Transitioned = FALSE;
    switch(Pulse_State)
    {
        case STATE_PULSE_TIME_RAMP:
            SpinDrumDo();                           // Do function.
            break;

        case STATE_PULSE_TIME_ON:
            IncrementTimer();                       // Do function.
            if (Pulse_Transitioned == FALSE)
            {
                CheckIfTimeOnReached();             // Do function.
            }
            break;

        case STATE_PULSE_TIME_OFF_DECELERATION:
            IncrementTimer();                       // Do function.
            if (Pulse_Transitioned == FALSE)
            {
                StopDrumDo();                       // Do function.
            }
            break;

        case STATE_PULSE_TIME_OFF_DRUM_STOPPED:
            IncrementTimer();                       // Do function.
            if (Pulse_Transitioned == FALSE)
            {
                CheckIfTimeOffReached();            // Do function.
            }
            break;

        default:
            // There is nothing to do for all other values of Pulse_State.
            break;
    }
}


//---------------------------------------------------------------------------------------------------------------------
/**
 * The initialization function for the Pulse state machine.
 * This function resets the state machine to the initial state and executes the initial state's
 * entry function.
 *
 *              >>> DO NOT MODIFY THIS FUNCTION <<<
 *
 * This function was auto-generated and must not be edited manually.
 * Change the UML used to generate this function and regenerate the source code.
 */
static void PulseInitialize(void)
{
    Pulse_State = STATE_PULSE_TIME_RAMP;
    Pulse_Transitioned = FALSE;
    LoadAndRunPulseFromQueue();                     // Entry Function.
    if (Pulse_Transitioned == FALSE)
    {
        PulseExecuting();                           // Entry Function.
    }
    Pulse_Transitioned = TRUE;
}


// !! StateEngineCrank cannot update this code if you change the footer !!
// End of StateEngineCrank Auto-Generated Code.
