/**
 *  @file
 *
 *  @brief      Source file for the Wash module.
 *
 *  @details    Sub state of the Motion module that handles the wash profile.
 *              Module is designed to be a child module executed by a parent.
 *

    @startuml{Wash_sequence_diagram.png}
        Title Wash Sequence Diagram

        participant Parent
        participant child as "Wash"

        == executing the Wash module ==
            Parent -> child : Wash__SetProfile(wash_profile)
            Parent -> child : Wash__Entry()

            loop executed by the Parent
                Parent -> child : Wash__Do()
                activate Parent
                activate child
                alt no event to return
                    Parent <-- child : WASH_EV_NONE
                else wash stroke in the cw direction has completed
                    Parent <-- child : WASH_EV_CW_STROKE_COMPLETED
                else wash stroke in the ccw direction has completed
                    Parent <-- child : WASH_EV_CCW_STROKE_COMPLETED
                else error has been detected
                    Parent <-- child : WASH_EV_ERROR
                    deactivate Parent
                    deactivate child
                end
            end

    @enduml


    @startuml{Wash_state_machine.png}
        title Wash State Machine

        [*] --> Initialize

        Initialize --> CcwWaitForMci    : evStartDirectionCcw
        Initialize --> CwWaitForMci     : evStartDirectionCw


        CcwWaitForMci : entry /\t ClearMciTempEstTime()\n\t\t SpinDrumCcw()
        CcwWaitForMci : do /\t IncrementMciTempEstTime()\n\t SpinDrumDo()
        CcwWaitForMci --> CcwTimeOn : SPIN_DRUM\n(SPIN_DRUM_EV_IS_ROTATING)
        CcwWaitForMci --> [*] : SPIN_DRUM\n(SPIN_DRUM_EV_ERROR)/\n SetEventToReturnError()

        CcwTimeOn   : entry / SetTimerToProfileTimeOn()
        CcwTimeOn   : do /\t DecrementTimer()
        CcwTimeOn --> CcwStop   : evTimerExpired

        CcwStop : entry /\t ClearTimer()\n\t\t StopDrumEntry()
        CcwStop : do /\t IncrementTimer()\n\t StopDrumDo()
        CcwStop : exit / SetEventToReturnCcwStrokeCompleted()
        CcwStop --> CcwTimeOff  : STOP_DRUM\n(STOP_DRUM_EV_STOPPED)

        CcwTimeOff  : entry / CalcRemainingTimeOff()
        CcwTimeOff  : do / DecrementTimer()
        CcwTimeOff --> CwWaitForMci : evTimerExpired


        CwWaitForMci : entry /\t ClearMciTempEstTime()\n\t\t SpinDrumCw()
        CwWaitForMci : do  / IncrementMciTempEstTime()\n\t SpinDrumDo()
        CwWaitForMci --> CwTimeOn : SPIN_DRUM\n(SPIN_DRUM_EV_IS_ROTATING)
        CwWaitForMci --> [*] : SPIN_DRUM\n(SPIN_DRUM_EV_ERROR)/\n SetEventToReturnError()

        CwTimeOn   : entry / SetTimerToProfileTimeOn()
        CwTimeOn   : do    / DecrementTimer()
        CwTimeOn --> CwStop   : evTimerExpired

        CwStop : entry /\t ClearTimer()\n\t\t StopDrumEntry()
        CwStop : do /\t IncrementTimer()\n\t StopDrumDo()
        CwStop : exit / SetEventToReturnCwStrokeCompleted()
        CwStop --> CwTimeOff  : STOP_DRUM\n(STOP_DRUM_EV_STOPPED)

        CwTimeOff  : entry / CalcRemainingTimeOff()
        CwTimeOff  : do / DecrementTimer()
        CwTimeOff --> CcwWaitForMci : evTimerExpired

    @enduml

 *  $Header: Wash.c 1.4 2015/07/22 16:02:06EDT Paul F Spindler (SPINDPF) Exp  $
 *
 *  @copyright  Copyright 2015-$Date: 2015/07/22 16:02:06EDT $. Whirlpool Corporation. All rights reserved - CONFIDENTIAL
 */
//---------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------


//-------------------------------------- Include Files ----------------------------------------------------------------
#include "C_Extensions.h"

#include "Wash.h"

#include "Log.h"
#include "SpinDrum.h"
#include "StopDrum.h"

#include "limits.h"
#include "stdlib.h"
#include "string.h"

//=====================================================================================================================
//=== StateEngineCrank (v2.8) Auto-Generated Definitions for Wash State Machine
//=====================================================================================================================
// !! StateEngineCrank cannot update this code if you change the header !!


//! Unique names used to identify states of the Wash state machine.
typedef enum WASH_STATE_ENUM
{
    STATE_WASH_NONE             = 0,

    STATE_WASH_INITIALIZE       = 1,                //!< (initial_state)
    STATE_WASH_CCW_WAIT_FOR_MCI = 2,
    STATE_WASH_CW_WAIT_FOR_MCI  = 3,
    STATE_WASH_CCW_TIME_ON      = 4,
    STATE_WASH_CCW_STOP         = 5,
    STATE_WASH_CCW_TIME_OFF     = 6,
    STATE_WASH_CW_TIME_ON       = 7,
    STATE_WASH_CW_STOP          = 8,
    STATE_WASH_CW_TIME_OFF      = 9,

    STATE_WASH_END              = 10
} WASH_STATE_TYPE;

//! The state variable for the Wash state machine.
//! State machines with a history pseudostate may write to this variable during initialization.
//! Other state machines must only write to this variable through auto-generated functions.
static WASH_STATE_TYPE Wash_State;

//! The transition variable for the Wash state machine.
//! Flag used by the state machine to track when a transition occurs in the middle of a state's
//! Entry() or Do() action that contains more than one function.
static BOOL_TYPE Wash_Transitioned;

// -- Private Function Prototypes used by the state machine for customization functions --
static void CalcRemainingTimeOff(void);
static void ClearMciTempEstTime(void);
static void ClearTimer(void);
static void DecrementTimer(void);
static void IncrementMciTempEstTime(void);
static void IncrementTimer(void);
static void SetEventToReturnCcwStrokeCompleted(void);
static void SetEventToReturnCwStrokeCompleted(void);
static void SetEventToReturnError(void);
static void SetTimerToProfileTimeOn(void);
static void SpinDrumCcw(void);
static void SpinDrumCw(void);
static void SpinDrumDo(void);
static void StopDrumDo(void);
static void StopDrumEntry(void);

// -- Private Function Prototypes used by the state machine for standard state machine functions --
static void EventStartDirectionCcw(void);
static void EventStartDirectionCw(void);
static void EventTimerExpired(void);
static void ExternalEventSpinDrum(uint8 event);
static void ExternalEventStopDrum(uint8 event);
static void WashExecute(void);
static void WashInitialize(void);

// !! StateEngineCrank cannot update this code if you change the footer !!
// End of StateEngineCrank Auto-Generated Code.

//-------------------------------------- PRIVATE (Variables, Constants & Defines) -------------------------------------

//! list of module Log messages
typedef enum
{
    MESSAGE_NONE = 0,
    PROFILE_NOT_DEFINED                 //!< data: na
} MODULE_WASH_LOG_MESSAGE_ID_TYPE;
#define MODULE_NAME MODULE_WASH

typedef uint16 WASH_TIMER_TYPE;     // 1 = 1 execution of the API Wash__Background()
#define WASH_TIMER_MAX  USHRT_MAX

//! list of module static variables
static struct
{
    WASH_EVENT_TYPE     Event_To_Return;
    WASH_TIMER_TYPE     Mci_Temp_Est_Time;
    WASH_PROFILE_TYPE * Profile;        // pointer to the Environment definition of the profile
    WASH_TIMER_TYPE     Timer;
} Wash;

//-------------------------------------- PRIVATE (Function Prototypes) ------------------------------------------------


//=====================================================================================================================
//-------------------------------------- Public Functions -------------------------------------------------------------
//=====================================================================================================================


//---------------------------------------------------------------------------------------------------------------------
/**
 *  @brief  Defines the Wash profile.
 *  @param  profile     Pointer to the environment definition of the WASH_PROFILE_TYPE
 */
void Wash__SetProfile(WASH_PROFILE_TYPE * profile)
{
    if ( profile == NULL )
    {
        LOG_ADD_EXCEPTION(PROFILE_NOT_DEFINED, 0);
    }
    else
    {
        Wash.Profile = profile;
    }
}


//---------------------------------------------------------------------------------------------------------------------
/**
 *  @brief  Executes the modules enter task/s.
 */
void Wash__Entry(void)
{
    if ( Wash.Profile == NULL )
    {
        LOG_ADD_EXCEPTION(PROFILE_NOT_DEFINED, 0);
    }
    else
    {
        Wash.Event_To_Return = WASH_EV_NONE;
        Wash.Mci_Temp_Est_Time = 0;
        Wash.Timer = 0;
        WashInitialize();
        if ( Wash.Profile->Spin.Target_Drum_Velocity >= 0 )
        {
            EventStartDirectionCw();
        }
        else
        {
            EventStartDirectionCcw();
        }
    }
}


//---------------------------------------------------------------------------------------------------------------------
/**
 *  @brief      Executes the module background task/s.
 *
 *  @return     WASH_EVENT_TYPE
 */
WASH_EVENT_TYPE Wash__Do(void)
{
    if ( Wash.Profile == NULL )
    {
        return ( WASH_EV_NONE );
    }
    else
    {
        WashExecute();

        // define the module return event
        WASH_EVENT_TYPE ret_val = WASH_EV_NONE;
        if ( Wash.Event_To_Return != WASH_EV_NONE )
        {
            ret_val = Wash.Event_To_Return;
            Wash.Event_To_Return = WASH_EV_NONE;
        }
        return ( ret_val );
    }
}


//=====================================================================================================================
//-------------------------------------- Private Functions ------------------------------------------------------------
//=====================================================================================================================


//=====================================================================================================================
//  --- Private State Machine Customization Functions -----------------------------------------------------------------
//=====================================================================================================================

//---------------------------------------------------------------------------------------------------------------------
/**
 *
 */
static void CalcRemainingTimeOff(void)
{
    #define COMMANDED_TIME_OFF()    (Wash.Profile->Time_Off / MOTION_HANDLER_EXECUTION_RATE)
    #define CURRENT_TIME_OFF()      (Wash.Timer)                    // time elapsed since the Mci stop command was sent to when it reported no movement
    #define MCI_DELAY_TIME()        (Wash.Mci_Temp_Est_Time)        // estimated time from when the Mci will actually run after being commanded to run
    sint16 calculated_remainiming_time_off = COMMANDED_TIME_OFF() - CURRENT_TIME_OFF() - MCI_DELAY_TIME();
    Wash.Timer = (calculated_remainiming_time_off >= 0) ? (WASH_TIMER_TYPE)calculated_remainiming_time_off : 0;
}


//---------------------------------------------------------------------------------------------------------------------
/**
 *  Clears the static variable Mci_Temp_Est_Time.
 */
static void ClearMciTempEstTime(void)
{
    Wash.Mci_Temp_Est_Time = 0;
}


//---------------------------------------------------------------------------------------------------------------------
/**
 *  Clears the static variable Timer.
 */
static void ClearTimer(void)
{
    Wash.Timer = 0;
}


//---------------------------------------------------------------------------------------------------------------------
/**
 *  Decrement the static variable Timer.
 */
static void DecrementTimer(void)
{
    Wash.Timer = (Wash.Timer > 0) ? Wash.Timer - 1 : 0;
    if ( Wash.Timer == 0 )
    {
        EventTimerExpired();
    }
}


//---------------------------------------------------------------------------------------------------------------------
/**
 *  Increments the static variable Mci_Temp_Est_Time.
 */
static void IncrementMciTempEstTime(void)
{
    Wash.Mci_Temp_Est_Time = (Wash.Mci_Temp_Est_Time < WASH_TIMER_MAX) ? Wash.Mci_Temp_Est_Time + 1 : WASH_TIMER_MAX;
}


//---------------------------------------------------------------------------------------------------------------------
/**
 *  Increments the static variable Timer and prevents overflow.
 */
static void IncrementTimer(void)
{
    Wash.Timer = (Wash.Timer < WASH_TIMER_MAX) ? Wash.Timer + 1 : WASH_TIMER_MAX;
}


//---------------------------------------------------------------------------------------------------------------------
/**
 *  Set static variable Timer to the Time_On.
 */
static void SetTimerToProfileTimeOn(void)
{
    Wash.Timer = Wash.Profile->Time_On / MOTION_HANDLER_EXECUTION_RATE;
}


//---------------------------------------------------------------------------------------------------------------------
/**
 *  Set event to return to the environment ccw stroke completed.
 */
static void SetEventToReturnCcwStrokeCompleted(void)
{
    Wash.Event_To_Return = WASH_EV_CCW_STROKE_COMPLETED;
}


//---------------------------------------------------------------------------------------------------------------------
/**
 *  Set event to return to the environment cw stroke completed.
 */
static void SetEventToReturnCwStrokeCompleted(void)
{
    Wash.Event_To_Return = WASH_EV_CW_STROKE_COMPLETED;
}


//---------------------------------------------------------------------------------------------------------------------
/**
 *  Set event to return to the environment error.
 */
static void SetEventToReturnError(void)
{
    Wash.Event_To_Return = WASH_EV_ERROR;
}


//---------------------------------------------------------------------------------------------------------------------
/**
 *  Command the drum to spin ccw.
 */
static void SpinDrumCcw(void)
{
    SPIN_DRUM_PROFILE_TYPE control = {
        .Drum_Acceleration.Type             = SPIN_DRUM_CONSTANT_ACCELERATION,
        .Drum_Acceleration.Value.Constant   = Wash.Profile->Spin.Drum_Acceleration.Value.Constant,
        .Target_Drum_Velocity               = abs(Wash.Profile->Spin.Target_Drum_Velocity) * -1    /* ensure the value is negative for the ccw direction */
    };
    SpinDrum__Entry(&control);
}


//---------------------------------------------------------------------------------------------------------------------
/**
 *  Command the drum to spin cw.
 */
static void SpinDrumCw(void)
{
    SPIN_DRUM_PROFILE_TYPE control = {
        .Drum_Acceleration.Type             = SPIN_DRUM_CONSTANT_ACCELERATION,
        .Drum_Acceleration.Value.Constant   = Wash.Profile->Spin.Drum_Acceleration.Value.Constant,
        .Target_Drum_Velocity               = abs(Wash.Profile->Spin.Target_Drum_Velocity) /* ensure the value is positive for the cw direction */
    };
    SpinDrum__Entry(&control);
}


//---------------------------------------------------------------------------------------------------------------------
/**
 *  Wait for drum to start rotating.
 */
static void SpinDrumDo(void)
{
    SPIN_DRUM_EVENT_TYPE event = SpinDrum__Do();
    if ( event != SPIN_DRUM_EV_NONE )
    {
        ExternalEventSpinDrum((uint8)event);
    }
    // else do nothing
}


//---------------------------------------------------------------------------------------------------------------------
/**
 *  Wait for the drum to stop rotating.
 */
static void StopDrumDo(void)
{
    STOP_DRUM_EVENT_TYPE event = StopDrum__Do();
    if ( event != STOP_DRUM_EV_NONE )
    {
        ExternalEventStopDrum((uint8)event);
    }
    // else do nothing
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
//=== StateEngineCrank (v2.8) Auto-Generated Functions for Wash State Machine
//=====================================================================================================================
// !! StateEngineCrank cannot update this code if you change the header !!


//---------------------------------------------------------------------------------------------------------------------
/**
 * Process the Start Direction Ccw Event for the Wash state machine.
 *
 *              >>> DO NOT MODIFY THIS FUNCTION <<<
 *
 * This function was auto-generated and must not be edited manually.
 * Change the UML used to generate this function and regenerate the source code.
 */
static void EventStartDirectionCcw(void)
{
    switch (Wash_State)
    {
        case STATE_WASH_INITIALIZE:
            Wash_State = STATE_WASH_CCW_WAIT_FOR_MCI;
            Wash_Transitioned = FALSE;
            ClearMciTempEstTime();                  // Entry Function.
            if (Wash_Transitioned == FALSE)
            {
                SpinDrumCcw();                      // Entry Function.
            }
            Wash_Transitioned = TRUE;
            break;

        default:
            // Event is ignored for all other values of Wash_State.
            break;
    }
}


//---------------------------------------------------------------------------------------------------------------------
/**
 * Process the Start Direction Cw Event for the Wash state machine.
 *
 *              >>> DO NOT MODIFY THIS FUNCTION <<<
 *
 * This function was auto-generated and must not be edited manually.
 * Change the UML used to generate this function and regenerate the source code.
 */
static void EventStartDirectionCw(void)
{
    switch (Wash_State)
    {
        case STATE_WASH_INITIALIZE:
            Wash_State = STATE_WASH_CW_WAIT_FOR_MCI;
            Wash_Transitioned = FALSE;
            ClearMciTempEstTime();                  // Entry Function.
            if (Wash_Transitioned == FALSE)
            {
                SpinDrumCw();                       // Entry Function.
            }
            Wash_Transitioned = TRUE;
            break;

        default:
            // Event is ignored for all other values of Wash_State.
            break;
    }
}


//---------------------------------------------------------------------------------------------------------------------
/**
 * Process the Timer Expired Event for the Wash state machine.
 *
 *              >>> DO NOT MODIFY THIS FUNCTION <<<
 *
 * This function was auto-generated and must not be edited manually.
 * Change the UML used to generate this function and regenerate the source code.
 */
static void EventTimerExpired(void)
{
    switch (Wash_State)
    {
        case STATE_WASH_CCW_TIME_ON:
            Wash_State = STATE_WASH_CCW_STOP;
            Wash_Transitioned = FALSE;
            ClearTimer();                           // Entry Function.
            if (Wash_Transitioned == FALSE)
            {
                StopDrumEntry();                    // Entry Function.
            }
            Wash_Transitioned = TRUE;
            break;

        case STATE_WASH_CCW_TIME_OFF:
            Wash_State = STATE_WASH_CW_WAIT_FOR_MCI;
            Wash_Transitioned = FALSE;
            ClearMciTempEstTime();                  // Entry Function.
            if (Wash_Transitioned == FALSE)
            {
                SpinDrumCw();                       // Entry Function.
            }
            Wash_Transitioned = TRUE;
            break;

        case STATE_WASH_CW_TIME_ON:
            Wash_State = STATE_WASH_CW_STOP;
            Wash_Transitioned = FALSE;
            ClearTimer();                           // Entry Function.
            if (Wash_Transitioned == FALSE)
            {
                StopDrumEntry();                    // Entry Function.
            }
            Wash_Transitioned = TRUE;
            break;

        case STATE_WASH_CW_TIME_OFF:
            Wash_State = STATE_WASH_CCW_WAIT_FOR_MCI;
            Wash_Transitioned = FALSE;
            ClearMciTempEstTime();                  // Entry Function.
            if (Wash_Transitioned == FALSE)
            {
                SpinDrumCcw();                      // Entry Function.
            }
            Wash_Transitioned = TRUE;
            break;

        default:
            // Event is ignored for all other values of Wash_State.
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
        case SPIN_DRUM_EV_IS_ROTATING:
            switch (Wash_State)
            {
                case STATE_WASH_CCW_WAIT_FOR_MCI:
                    Wash_State = STATE_WASH_CCW_TIME_ON;
                    SetTimerToProfileTimeOn();      // Entry Function.
                    Wash_Transitioned = TRUE;
                    break;

                case STATE_WASH_CW_WAIT_FOR_MCI:
                    Wash_State = STATE_WASH_CW_TIME_ON;
                    SetTimerToProfileTimeOn();      // Entry Function.
                    Wash_Transitioned = TRUE;
                    break;

                default:
                    // Event is ignored for all other values of Wash_State.
                    break;
            }
            break;

        case SPIN_DRUM_EV_ERROR:
            switch (Wash_State)
            {
                case STATE_WASH_CCW_WAIT_FOR_MCI:
                case STATE_WASH_CW_WAIT_FOR_MCI:
                    SetEventToReturnError();        // Transition action function.
                    Wash_State = STATE_WASH_NONE;   // Terminate Wash state machine execution.
                    Wash_Transitioned = TRUE;
                    break;

                default:
                    // Event is ignored for all other values of Wash_State.
                    break;
            }
            break;

        default:
            // All other external Spin Drum events are ignored by the Wash state machine.
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
            switch (Wash_State)
            {
                case STATE_WASH_CCW_STOP:
                    SetEventToReturnCcwStrokeCompleted(); // Exit function.
                    Wash_State = STATE_WASH_CCW_TIME_OFF;
                    CalcRemainingTimeOff();         // Entry Function.
                    Wash_Transitioned = TRUE;
                    break;

                case STATE_WASH_CW_STOP:
                    SetEventToReturnCwStrokeCompleted(); // Exit function.
                    Wash_State = STATE_WASH_CW_TIME_OFF;
                    CalcRemainingTimeOff();         // Entry Function.
                    Wash_Transitioned = TRUE;
                    break;

                default:
                    // Event is ignored for all other values of Wash_State.
                    break;
            }
            break;

        default:
            // All other external Stop Drum events are ignored by the Wash state machine.
            break;
    }
}


//---------------------------------------------------------------------------------------------------------------------
/**
 * The main execution function for the Wash state machine.
 * This function should be called periodically while the state machine is running.
 *
 *              >>> DO NOT MODIFY THIS FUNCTION <<<
 *
 * This function was auto-generated and must not be edited manually.
 * Change the UML used to generate this function and regenerate the source code.
 */
static void WashExecute(void)
{
    Wash_Transitioned = FALSE;
    switch(Wash_State)
    {
        case STATE_WASH_CCW_WAIT_FOR_MCI:
        case STATE_WASH_CW_WAIT_FOR_MCI:
            IncrementMciTempEstTime();              // Do function.
            if (Wash_Transitioned == FALSE)
            {
                SpinDrumDo();                       // Do function.
            }
            break;

        case STATE_WASH_CCW_TIME_ON:
        case STATE_WASH_CCW_TIME_OFF:
        case STATE_WASH_CW_TIME_ON:
        case STATE_WASH_CW_TIME_OFF:
            DecrementTimer();                       // Do function.
            break;

        case STATE_WASH_CCW_STOP:
        case STATE_WASH_CW_STOP:
            IncrementTimer();                       // Do function.
            if (Wash_Transitioned == FALSE)
            {
                StopDrumDo();                       // Do function.
            }
            break;

        default:
            // There is nothing to do for all other values of Wash_State.
            break;
    }
}


//---------------------------------------------------------------------------------------------------------------------
/**
 * The initialization function for the Wash state machine.
 * This function resets the state machine to the initial state.
 *
 *              >>> DO NOT MODIFY THIS FUNCTION <<<
 *
 * This function was auto-generated and must not be edited manually.
 * Change the UML used to generate this function and regenerate the source code.
 */
static void WashInitialize(void)
{
    Wash_State = STATE_WASH_INITIALIZE;
    Wash_Transitioned = TRUE;
}


// !! StateEngineCrank cannot update this code if you change the footer !!
// End of StateEngineCrank Auto-Generated Code.
