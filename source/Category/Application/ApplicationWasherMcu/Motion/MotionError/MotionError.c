/**
 *  @file
 *
 *  @brief      Source file for the MotionError module.
 *
 *  @details    Handles an error detected by the Motion module.
 *              Module is designed to be a child module executed by a parent.
 *

    @startuml{MotionError_sequence_diagram.jpg}
        title MotionError Sequence Diagram

        participant Parent
        participant "MotionError" as child

        == executing the MotionError module ==

            Parent -> child : MotionError__Enter()

            loop execute the child module state machine
                Parent -> child : MotionError__Do()
                activate child
                alt MotionError module is still handling the error
                    Parent <-- child : MOTION_ERROR_EV_NONE
                else MotionError has handled the error
                    Parent <-- child : MOTION_ERROR_EV_HANDLED
                    deactivate child
                end
            end

    @enduml



    @startuml{MotionError_state_machine.jpg}
        title "MotionError State Machine"

        [*] --> Delay

        Delay : enter /\t SetTimer()\n\t\t StopDrumEntry()
        Delay : do /\t DecrementTimer()
        Delay --> StopDrum : evTimerExpired

        StopDrum : do /\t StopDrumBackground()
        StopDrum --> [*] : STOP_DRUM(STOP_DRUM_EV_STOPPED)

    @enduml

 *
 *  $Header: MotionError.c 1.1 2015/06/29 14:28:48EDT Paul F Spindler (SPINDPF) Exp  $
 *
 *  @copyright  Copyright 2015-$Date: 2015/06/29 14:28:48EDT $. Whirlpool Corporation. All rights reserved - CONFIDENTIAL
 */
//---------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------


//-------------------------------------- Include Files ----------------------------------------------------------------
#include "C_Extensions.h"

#include "MotionError.h"
#include "MotionError_prv.h"

#include "StopDrum.h"

#include "string.h"

//=====================================================================================================================
//=== StateEngineCrank (v2.8) Auto-Generated Definitions for Motion Error State Machine
//=====================================================================================================================
// !! StateEngineCrank cannot update this code if you change the header !!


//! Unique names used to identify states of the Motion Error state machine.
typedef enum MOTION_ERROR_STATE_ENUM
{
    STATE_MOTION_ERROR_NONE      = 0,

    STATE_MOTION_ERROR_DELAY     = 1,               //!< (initial_state)
    STATE_MOTION_ERROR_STOP_DRUM = 2,

    STATE_MOTION_ERROR_END       = 3
} MOTION_ERROR_STATE_TYPE;

//! The state variable for the Motion Error state machine.
//! State machines with a history pseudostate may write to this variable during initialization.
//! Other state machines must only write to this variable through auto-generated functions.
static MOTION_ERROR_STATE_TYPE Motion_Error_State;

//! The transition variable for the Motion Error state machine.
//! Flag used by the state machine to track when a transition occurs in the middle of a state's
//! Entry() or Do() action that contains more than one function.
static BOOL_TYPE Motion_Error_Transitioned;

// -- Private Function Prototypes used by the state machine for customization functions --
static void DecrementTimer(void);
static void SetTimer(void);
static void StopDrumBackground(void);
static void StopDrumEntry(void);

// -- Private Function Prototypes used by the state machine for standard state machine functions --
static void EventTimerExpired(void);
static void ExternalEventStopDrum(uint8 event);
static void MotionErrorExecute(void);
static void MotionErrorInitialize(void);

// !! StateEngineCrank cannot update this code if you change the footer !!
// End of StateEngineCrank Auto-Generated Code.

//-------------------------------------- PRIVATE (Variables, Constants & Defines) -------------------------------------


//! list of module static variables
static struct
{
    uint16  Timer;      // 1 = 1 call of API function MotionError__Do()
} MotionError;

//-------------------------------------- PRIVATE (Function Prototypes) ------------------------------------------------


//=====================================================================================================================
//-------------------------------------- Public Functions -------------------------------------------------------------
//=====================================================================================================================

//---------------------------------------------------------------------------------------------------------------------
/**
 *  @brief  Executes the modules enter task/s.
 */
void MotionError__Enable(void)
{
    memset(&MotionError, 0, sizeof(MotionError));
    MotionErrorInitialize();
}


//---------------------------------------------------------------------------------------------------------------------
/**
 *  @brief      Executes the module background task/s.
 *
 *  @return     MOTION_ERROR_EVENT_TYPE
 */
MOTION_ERROR_EVENT_TYPE MotionError__Do(void)
{
    MotionErrorExecute();
    return ( (Motion_Error_State == STATE_MOTION_ERROR_NONE) ? MOTION_ERROR_EV_HANDLED : MOTION_ERROR_EV_NONE );
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
static void DecrementTimer(void)
{
    (void)StopDrum__Do();
    MotionError.Timer = (MotionError.Timer > 0) ? MotionError.Timer - 1 : 0;
    if ( MotionError.Timer == 0 )
    {
        EventTimerExpired();
    }
    // else MotionError.Timer > 0, do nothing
}


//---------------------------------------------------------------------------------------------------------------------
/**
 *
 */
static void SetTimer(void)
{
    MotionError.Timer = DELAY_TIME();
}


//---------------------------------------------------------------------------------------------------------------------
/**
 *
 */
static void StopDrumBackground(void)
{
    switch ( StopDrum__Do() )
    {
        default:
            // do nothing
            break;
        case STOP_DRUM_EV_STOPPED:
            ExternalEventStopDrum((uint8)STOP_DRUM_EV_STOPPED);
            break;
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
//=== StateEngineCrank (v2.8) Auto-Generated Functions for Motion Error State Machine
//=====================================================================================================================
// !! StateEngineCrank cannot update this code if you change the header !!


//---------------------------------------------------------------------------------------------------------------------
/**
 * Process the Timer Expired Event for the Motion Error state machine.
 *
 *              >>> DO NOT MODIFY THIS FUNCTION <<<
 *
 * This function was auto-generated and must not be edited manually.
 * Change the UML used to generate this function and regenerate the source code.
 */
static void EventTimerExpired(void)
{
    switch (Motion_Error_State)
    {
        case STATE_MOTION_ERROR_DELAY:
            Motion_Error_State = STATE_MOTION_ERROR_STOP_DRUM;
            Motion_Error_Transitioned = TRUE;
            break;

        default:
            // Event is ignored for all other values of Motion_Error_State.
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
            switch (Motion_Error_State)
            {
                case STATE_MOTION_ERROR_STOP_DRUM:
                    Motion_Error_State = STATE_MOTION_ERROR_NONE; // Terminate Motion Error state machine execution.
                    Motion_Error_Transitioned = TRUE;
                    break;

                default:
                    // Event is ignored for all other values of Motion_Error_State.
                    break;
            }
            break;

        default:
            // All other external Stop Drum events are ignored by the Motion Error state machine.
            break;
    }
}


//---------------------------------------------------------------------------------------------------------------------
/**
 * The main execution function for the Motion Error state machine.
 * This function should be called periodically while the state machine is running.
 *
 *              >>> DO NOT MODIFY THIS FUNCTION <<<
 *
 * This function was auto-generated and must not be edited manually.
 * Change the UML used to generate this function and regenerate the source code.
 */
static void MotionErrorExecute(void)
{
    Motion_Error_Transitioned = FALSE;
    switch(Motion_Error_State)
    {
        case STATE_MOTION_ERROR_DELAY:
            DecrementTimer();                       // Do function.
            break;

        case STATE_MOTION_ERROR_STOP_DRUM:
            StopDrumBackground();                   // Do function.
            break;

        default:
            // There is nothing to do for all other values of Motion_Error_State.
            break;
    }
}


//---------------------------------------------------------------------------------------------------------------------
/**
 * The initialization function for the Motion Error state machine.
 * This function resets the state machine to the initial state and executes the initial state's
 * entry function.
 *
 *              >>> DO NOT MODIFY THIS FUNCTION <<<
 *
 * This function was auto-generated and must not be edited manually.
 * Change the UML used to generate this function and regenerate the source code.
 */
static void MotionErrorInitialize(void)
{
    Motion_Error_State = STATE_MOTION_ERROR_DELAY;
    Motion_Error_Transitioned = FALSE;
    SetTimer();                                     // Entry Function.
    if (Motion_Error_Transitioned == FALSE)
    {
        StopDrumEntry();                            // Entry Function.
    }
    Motion_Error_Transitioned = TRUE;
}


// !! StateEngineCrank cannot update this code if you change the footer !!
// End of StateEngineCrank Auto-Generated Code.
