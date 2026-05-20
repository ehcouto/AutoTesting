/**
 *  @file
 *
 *  @brief      Source file for the StopDrum module.
 *
 *  @details    Handles interfacing to the Mci module and stopping the drum rotation.
 *              Module is designed to be a child module executed by a parent.
 *

    @startuml{StopDrum_sequence_diagram.png}
        Title StopDrum Sequence Diagram

        participant Parent
        participant child as "SpinDrum"

        == executing the StopDrum module ==

            Parent -> child : StopDrum__Entry()
            activate child
            child -> Mci : Mci__Stop()
            deactivate child

            loop executed by the Parent
                Parent -> child : StopDrum__Do()
                activate Parent
                activate child
                child -> Mci : check conditions for stopped
                activate Mci
                child <-- Mci
                deactivate Mci
                alt drum is rotating
                    Parent <-- child : STOP_DRUM_EV_NONE
                else drum has stopped
                    Parent <-- child : STOP_DRUM_EV_STOPPED
                    deactivate child
                    deactivate Parent
                end
            end
    @enduml

    @startuml{StopDrum_state_machine.png}
        title StopDrum State Machine

        [*] --> Stopping

        Stopping : entry /\t SetTimerToMaxTimeForMciToStop()\n\t\t SetEventToReturnNone()\n\t\t SendMciStopCommand()
        Stopping : do /\t VerifyMciHasStopped() \n\t DecrementTimer()
        Stopping --> Error      : evTimerExpired \n/ LogMciNotStopping()
        Stopping --> Error      : evMciRejectedCommandToStop
        Stopping --> Stopped    : evMciStopped

        Stopped : entry /\t SetEventToReturnStopped()
        Stopped : do /\t VerifyMciIsStopped()
        Stopped --> Stopping : evMciIsNotStopped

        Error : entry /\t SetTimerToErrorDelayTime()
        Error : do /\t DecrementTimer()
        Error --> Stopping : evTimerExpired

    @enduml

 *  $Header: StopDrum.c 1.8 2015/09/28 15:20:05EDT Paul F Spindler (SPINDPF) Exp  $
 *
 *  @copyright  Copyright 2015-$Date: 2015/09/28 15:20:05EDT $. Whirlpool Corporation. All rights reserved - CONFIDENTIAL
 */
//---------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------


//-------------------------------------- Include Files ----------------------------------------------------------------
#include "C_Extensions.h"

#include "StopDrum.h"
#include "StopDrum_prv.h"

#include "Log.h"
#include "Mci.h"
#include "MciSensorsWm.h"

#include "stdlib.h"
#include "string.h"

//=====================================================================================================================
//=== StateEngineCrank (v2.7) Auto-Generated Definitions for Stop Drum State Machine
//=====================================================================================================================
// !! StateEngineCrank cannot update this code if you change the header !!


//! Unique names used to identify states of the Stop Drum state machine.
typedef enum STOP_DRUM_STATE_ENUM
{
    STATE_STOP_DRUM_NONE     = 0,

    STATE_STOP_DRUM_STOPPING = 1,                   //!< (initial_state)
    STATE_STOP_DRUM_ERROR    = 2,
    STATE_STOP_DRUM_STOPPED  = 3,

    STATE_STOP_DRUM_END      = 4
} STOP_DRUM_STATE_TYPE;

//! The state variable for the Stop Drum state machine.
//! State machines with a history pseudostate may write to this variable during initialization.
//! Other state machines must only write to this variable through auto-generated functions.
static STOP_DRUM_STATE_TYPE Stop_Drum_State;

//! The transition variable for the Stop Drum state machine.
//! Flag used by the state machine to track when a transition occurs in the middle of a state's
//! Entry() or Do() action that contains more than one function.
static BOOL_TYPE Stop_Drum_Transitioned;

// -- Private Function Prototypes used by the state machine for customization functions --
static void DecrementTimer(void);
static void LogMciNotStopping(void);
static void SendMciStopCommand(void);
static void SetEventToReturnNone(void);
static void SetEventToReturnStopped(void);
static void SetTimerToErrorDelayTime(void);
static void SetTimerToMaxTimeForMciToStop(void);
static void VerifyMciHasStopped(void);
static void VerifyMciIsStopped(void);

// -- Private Function Prototypes used by the state machine for standard state machine functions --
static void EventMciIsNotStopped(void);
static void EventMciRejectedCommandToStop(void);
static void EventMciStopped(void);
static void EventTimerExpired(void);
static void StopDrumExecute(void);
static void StopDrumInitialize(void);

// !! StateEngineCrank cannot update this code if you change the footer !!
// End of StateEngineCrank Auto-Generated Code.

//-------------------------------------- PRIVATE (Variables, Constants & Defines) -------------------------------------

//! list of module Log messages
typedef enum
{
    MESSAGE_NONE = 0,
    MCI_IS_NOT_STOPPING,                        //!< data: bits notifying what feedbacks are negative
    MCI_REJECTED_COMMAND_TO_STOP                //!< data: mci response
} MODULE_STOP_DRUM_LOG_MESSAGE_ID_TYPE;
#define MODULE_NAME MODULE_STOP_DRUM

//! precompiler statements for interfacing to the Mci module
#define MCI_IS_STOPPED()        ( (Mci__GetDigital(MOTOR0, MCI_DI_MOTOR_STOP) == 1) ? TRUE : FALSE )
#define MCI_IS_IDLE()           ( (Mci__GetDigital(MOTOR0, MCI_DI_IDLE) == 1) ? TRUE : FALSE )
#define MCI_ANY_ERROR_FOUND()   ( (Mci__GetError(MOTOR0, MCI_ERROR_ANY_ERROR_FOUND) == 0) ? FALSE : TRUE )
/**
 *  Module timer variable type declaration.
 *  Resolution: 1 = 1 call of API SpinDrum__Do()
 */
typedef uint16 TIMER_TYPE;

//! list of module static variables
static struct
{
    MOTION_DRUM_ACCELERATION_TYPE Drum_Deceleration;
    STOP_DRUM_EVENT_TYPE    Event_To_Return;
    TIMER_TYPE              Timer;
} StopDrum;

//-------------------------------------- PRIVATE (Function Prototypes) ------------------------------------------------

static BOOL_TYPE IsMciStopped(void);

//=====================================================================================================================
//-------------------------------------- Public Functions -------------------------------------------------------------
//=====================================================================================================================

//---------------------------------------------------------------------------------------------------------------------
/**
 *  @brief      Executes the modules enter task/s.
 */
void StopDrum__Entry(STOP_DRUM_PROFILE_TYPE* profile)
{
    memset(&StopDrum, 0, sizeof(StopDrum));
    StopDrum.Drum_Deceleration = profile->Drum_Deceleration;
    StopDrumInitialize();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 *  @brief      Executes the module background task/s.
 *
 *  @return     STOP_DRUM_EVENT_TYPE
 */
STOP_DRUM_EVENT_TYPE StopDrum__Do(void)
{
    StopDrumExecute();
    return ( StopDrum.Event_To_Return );
}


//=====================================================================================================================
//-------------------------------------- Private Functions ------------------------------------------------------------
//=====================================================================================================================

//---------------------------------------------------------------------------------------------------------------------
/**
 *  Checks the conditions to determine if the Mci module is stopped
 *
 *  @return TRUE = Mci module is stopped; FALSE = Mci module is not stopped
 */
static BOOL_TYPE IsMciStopped(void)
{
    if (( MCI_IS_STOPPED() == TRUE ) &&
        ( MCI_IS_IDLE() == TRUE ) &&
        ( MCI_ANY_ERROR_FOUND() == FALSE ))
    {
        return ( TRUE );
    }
    else
    {
        return ( FALSE );
    }

}

//=====================================================================================================================
//  --- Private State Machine Customization Functions -----------------------------------------------------------------
//=====================================================================================================================


//---------------------------------------------------------------------------------------------------------------------
/**
 *  Handles decrementing the Timer and checking if it has expired.
 */
static void DecrementTimer(void)
{
    StopDrum.Timer = (StopDrum.Timer > 0) ? StopDrum.Timer - 1 : 0;
    if ( StopDrum.Timer == 0 )
    {
        EventTimerExpired();
    }
    // else Timer has not expired
}


//---------------------------------------------------------------------------------------------------------------------
/**
 *  Logs message that the Mci module has not reported the motor has stopped within a specified time limit.
 */
static void LogMciNotStopping(void)
{
    uint8 bit_field = 0x00;
    if ( MCI_IS_STOPPED() == FALSE )        bit_field |= 0x20;
    if ( MCI_IS_IDLE() == FALSE )           bit_field |= 0x40;
    if ( MCI_ANY_ERROR_FOUND() == TRUE )    bit_field |= 0x80;
    LOG_ADD_EVENT(MCI_IS_NOT_STOPPING, (bit_field << 8));
}


//---------------------------------------------------------------------------------------------------------------------
/**
 *  Sends the Mci module stop command.
 */
static void SendMciStopCommand(void)
{
    sint32 mci_ramp;

    MOTION__PREPROCESS_STOP();
    // NOTE: the Mci team has stated the ramp parameter has no affect on the software.  Regardless of the ramp value sent, when the Mci__Stop command is sent to the
    // Mci module, the Mci module will turn all lowers on to stop the motor.  The rpm deceleration rate can be controlled down to approximately 40rpm using the
    // Mci__Run command.  At 40rpm is when the Mci control switches from closed loop to open.  Below 40rpm, the Mci module will use all lowers on to stop the motor.
    // NOTE: the Mci team has stated the Mci__Stop command shall never return a response other than MCI_CMD_ACCEPTED.  To be safe, the stop module still handles non
    // MCI_CMD_ACCEPTED responses.
    mci_ramp = StopDrum.Drum_Deceleration;
    mci_ramp *= MciSensorsWm__GetTransmissionRatio();
    mci_ramp = abs(mci_ramp);
    MCI_CMD_RESPONSE_TYPE mci_response = Mci__Stop(MOTOR0, mci_ramp);
    switch ( mci_response )
    {
        case MCI_CMD_ACCEPTED:
            // do nothing
            break;
        default:
            LOG_ADD_EVENT(MCI_REJECTED_COMMAND_TO_STOP, mci_response);
            EventMciRejectedCommandToStop();
            break;
    }
}


//---------------------------------------------------------------------------------------------------------------------
/**
 *  Sets the module event to STOP_DRUM_EV_NONE.
 */
static void SetEventToReturnNone(void)
{
    StopDrum.Event_To_Return = STOP_DRUM_EV_NONE;
}


//---------------------------------------------------------------------------------------------------------------------
/**
 *  Sets the module event to STOP_DRUM_EV_STOPPED.
 */
static void SetEventToReturnStopped(void)
{
    StopDrum.Event_To_Return = STOP_DRUM_EV_STOPPED;
}


//---------------------------------------------------------------------------------------------------------------------
/**
 *  Sets the Timer to the time delay for retrying to stop the motor.
 */
static void SetTimerToErrorDelayTime(void)
{
    StopDrum.Timer = ERROR_DELAY_TIME;
}


//---------------------------------------------------------------------------------------------------------------------
/**
 *  Sets the Timer to the maximum amount of time the module will wait for the Mci to report it has stopped.
 */
static void SetTimerToMaxTimeForMciToStop(void)
{
    StopDrum.Timer = MAX_TIME_FOR_MCI_TO_STOP;
}


//---------------------------------------------------------------------------------------------------------------------
/**
 *  Checks if the Mci module is reporting that the motor has stopped.
 */
static void VerifyMciHasStopped(void)
{
    if ( IsMciStopped() == TRUE )
    {
        EventMciStopped();
    }
    // else Mci has not stopped
}


//---------------------------------------------------------------------------------------------------------------------
/**
 *  Verifies the Mci is reporting the motor is stopped.
 */
static void VerifyMciIsStopped(void)
{
    if ( IsMciStopped() == FALSE )
    {
        EventMciIsNotStopped();
    }
    // else conditions for Idle are met
}



//=====================================================================================================================
//=== StateEngineCrank (v2.7) Auto-Generated Functions for Stop Drum State Machine
//=====================================================================================================================
// !! StateEngineCrank cannot update this code if you change the header !!


//---------------------------------------------------------------------------------------------------------------------
/**
 * Process the Mci Is Not Stopped Event for the Stop Drum state machine.
 *
 *              >>> DO NOT MODIFY THIS FUNCTION <<<
 *
 * This function was auto-generated and must not be edited manually.
 * Change the UML used to generate this function and regenerate the source code.
 */
static void EventMciIsNotStopped(void)
{
    switch (Stop_Drum_State)
    {
        case STATE_STOP_DRUM_STOPPED:
            Stop_Drum_State = STATE_STOP_DRUM_STOPPING;
            Stop_Drum_Transitioned = FALSE;
            SetTimerToMaxTimeForMciToStop();        // Entry Function.
            if (Stop_Drum_Transitioned == FALSE)
            {
                SetEventToReturnNone();             // Entry Function.
                if (Stop_Drum_Transitioned == FALSE)
                {
                    SendMciStopCommand();           // Entry Function.
                }
            }
            Stop_Drum_Transitioned = TRUE;
            break;

        default:
            // Event is ignored for all other values of Stop_Drum_State.
            break;
    }
}


//---------------------------------------------------------------------------------------------------------------------
/**
 * Process the Mci Rejected Command To Stop Event for the Stop Drum state machine.
 *
 *              >>> DO NOT MODIFY THIS FUNCTION <<<
 *
 * This function was auto-generated and must not be edited manually.
 * Change the UML used to generate this function and regenerate the source code.
 */
static void EventMciRejectedCommandToStop(void)
{
    switch (Stop_Drum_State)
    {
        case STATE_STOP_DRUM_STOPPING:
            Stop_Drum_State = STATE_STOP_DRUM_ERROR;
            SetTimerToErrorDelayTime();             // Entry Function.
            Stop_Drum_Transitioned = TRUE;
            break;

        default:
            // Event is ignored for all other values of Stop_Drum_State.
            break;
    }
}


//---------------------------------------------------------------------------------------------------------------------
/**
 * Process the Mci Stopped Event for the Stop Drum state machine.
 *
 *              >>> DO NOT MODIFY THIS FUNCTION <<<
 *
 * This function was auto-generated and must not be edited manually.
 * Change the UML used to generate this function and regenerate the source code.
 */
static void EventMciStopped(void)
{
    switch (Stop_Drum_State)
    {
        case STATE_STOP_DRUM_STOPPING:
            Stop_Drum_State = STATE_STOP_DRUM_STOPPED;
            SetEventToReturnStopped();              // Entry Function.
            Stop_Drum_Transitioned = TRUE;
            break;

        default:
            // Event is ignored for all other values of Stop_Drum_State.
            break;
    }
}


//---------------------------------------------------------------------------------------------------------------------
/**
 * Process the Timer Expired Event for the Stop Drum state machine.
 *
 *              >>> DO NOT MODIFY THIS FUNCTION <<<
 *
 * This function was auto-generated and must not be edited manually.
 * Change the UML used to generate this function and regenerate the source code.
 */
static void EventTimerExpired(void)
{
    switch (Stop_Drum_State)
    {
        case STATE_STOP_DRUM_STOPPING:
            LogMciNotStopping();                    // Transition action function.
            Stop_Drum_State = STATE_STOP_DRUM_ERROR;
            SetTimerToErrorDelayTime();             // Entry Function.
            Stop_Drum_Transitioned = TRUE;
            break;

        case STATE_STOP_DRUM_ERROR:
            Stop_Drum_State = STATE_STOP_DRUM_STOPPING;
            Stop_Drum_Transitioned = FALSE;
            SetTimerToMaxTimeForMciToStop();        // Entry Function.
            if (Stop_Drum_Transitioned == FALSE)
            {
                SetEventToReturnNone();             // Entry Function.
                if (Stop_Drum_Transitioned == FALSE)
                {
                    SendMciStopCommand();           // Entry Function.
                }
            }
            Stop_Drum_Transitioned = TRUE;
            break;

        default:
            // Event is ignored for all other values of Stop_Drum_State.
            break;
    }
}


//---------------------------------------------------------------------------------------------------------------------
/**
 * The main execution function for the Stop Drum state machine.
 * This function should be called periodically while the state machine is running.
 *
 *              >>> DO NOT MODIFY THIS FUNCTION <<<
 *
 * This function was auto-generated and must not be edited manually.
 * Change the UML used to generate this function and regenerate the source code.
 */
static void StopDrumExecute(void)
{
    Stop_Drum_Transitioned = FALSE;
    switch(Stop_Drum_State)
    {
        case STATE_STOP_DRUM_STOPPING:
            VerifyMciHasStopped();                  // Do function.
            if (Stop_Drum_Transitioned == FALSE)
            {
                DecrementTimer();                   // Do function.
            }
            break;

        case STATE_STOP_DRUM_ERROR:
            DecrementTimer();                       // Do function.
            break;

        case STATE_STOP_DRUM_STOPPED:
            VerifyMciIsStopped();                   // Do function.
            break;

        default:
            // There is nothing to do for all other values of Stop_Drum_State.
            break;
    }
}


//---------------------------------------------------------------------------------------------------------------------
/**
 * The initialization function for the Stop Drum state machine.
 * This function resets the state machine to the initial state and executes the initial state's
 * entry function.
 *
 *              >>> DO NOT MODIFY THIS FUNCTION <<<
 *
 * This function was auto-generated and must not be edited manually.
 * Change the UML used to generate this function and regenerate the source code.
 */
static void StopDrumInitialize(void)
{
    Stop_Drum_State = STATE_STOP_DRUM_STOPPING;
    Stop_Drum_Transitioned = FALSE;
    SetTimerToMaxTimeForMciToStop();                // Entry Function.
    if (Stop_Drum_Transitioned == FALSE)
    {
        SetEventToReturnNone();                     // Entry Function.
        if (Stop_Drum_Transitioned == FALSE)
        {
            SendMciStopCommand();                   // Entry Function.
        }
    }
    Stop_Drum_Transitioned = TRUE;
}


// !! StateEngineCrank cannot update this code if you change the footer !!
// End of StateEngineCrank Auto-Generated Code.
