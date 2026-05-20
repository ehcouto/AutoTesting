/**
 *  @file
 *
 *  @brief      Source file for the RotateXDeg module.
 *
 *  @details    Sub state of the Motion module that handles the wash profile.
 *              Module is designed to be a child module executed by a parent.
 *

 *  $Header: RotateXDeg.c 1.0 2017/05/17 16:01:49EDT Marco Di Goro (DIGORM)  Exp  $
 *
 *  @copyright  Copyright 2017-$Date: 2017/05/17 16:01:49EDT $. Whirlpool Corporation. All rights reserved - CONFIDENTIAL
 */
//---------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------


//-------------------------------------- Include Files ----------------------------------------------------------------
#include "C_Extensions.h"
#include "RotateXDeg.h"

#include "Log.h"
#include "Mci.h"
#include "MciSensorsWm.h"
#include "SpinDrum.h"
#include "StopDrum.h"

#include "limits.h"
#include "stdlib.h"
#include "string.h"

//=====================================================================================================================
//=== StateEngineCrank (v2.8) Auto-Generated Definitions for RotateXDeg State Machine
//=====================================================================================================================
// !! StateEngineCrank cannot update this code if you change the header !!


//! Unique names used to identify states of the RotateXDeg state machine.
typedef enum ROTATEXDEG_STATE_ENUM
{
    STATE_ROTATEXDEG_NONE             = 0,

    STATE_ROTATEXDEG_INITIALIZE       = 1,                //!< (initial_state)
    STATE_ROTATEXDEG_CCW_WAIT_FOR_MCI = 2,
    STATE_ROTATEXDEG_CW_WAIT_FOR_MCI  = 3,
    STATE_ROTATEXDEG_CCW_TIME_ON      = 4,
    STATE_ROTATEXDEG_CCW_STOP         = 5,
    STATE_ROTATEXDEG_CW_TIME_ON       = 6,
    STATE_ROTATEXDEG_CW_STOP          = 7,

    STATE_ROTATEXDEG_END              = 8
} ROTATEXDEG_STATE_TYPE;

//! The state variable for the RotateXDeg state machine.
//! State machines with a history pseudostate may write to this variable during initialization.
//! Other state machines must only write to this variable through auto-generated functions.
static ROTATEXDEG_STATE_TYPE RotateXDeg_State;

//! The transition variable for the RotateXDeg state machine.
//! Flag used by the state machine to track when a transition occurs in the middle of a state's
//! Entry() or Do() action that contains more than one function.
static BOOL_TYPE RotateXDeg_Transitioned;

// -- Private Function Prototypes used by the state machine for customization functions --
static void ClearTimer(void);
static void DecrementTimer(void);
static void SetProfileTimeout(void);
static void SetEventToReturnCcwStrokeCompleted(void);
static void SetEventToReturnCwStrokeCompleted(void);
static void SetEventToReturnError(void);
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
static void RotateXDegExecute(void);
static void RotateXDegInitialize(void);

// !! StateEngineCrank cannot update this code if you change the footer !!
// End of StateEngineCrank Auto-Generated Code.

//-------------------------------------- PRIVATE (Variables, Constants & Defines) -------------------------------------

//! list of module Log messages
typedef enum
{
    MESSAGE_NONE = 0,
    PROFILE_NOT_DEFINED                 //!< data: na
} MODULE_ROTATEXDEG_LOG_MESSAGE_ID_TYPE;
#define MODULE_NAME MODULE_ROTATEXDEG

typedef uint16 ROTATEXDEG_TIMER_TYPE;     // 1 = 1 execution of the API RotateXDeg__Background()
#define ROTATEXDEG_TIMER_MAX  USHRT_MAX

//! list of module static variables
static struct
{
    ROTATEXDEG_EVENT_TYPE     Event_To_Return;
    ROTATEXDEG_PROFILE_TYPE * Profile;        // pointer to the Environment definition of the profile
    ANGULAR_DISTANCE_TYPE     Angular_Distance;
    unsigned short      	  Target_Degrees;        // the angular distance in degrees the drum shall rotate for under power
    ROTATEXDEG_TIMER_TYPE     Timeout;
} RotateXDeg;

//-------------------------------------- PRIVATE (Function Prototypes) ------------------------------------------------


//=====================================================================================================================
//-------------------------------------- Public Functions -------------------------------------------------------------
//=====================================================================================================================


//---------------------------------------------------------------------------------------------------------------------
/**
 *  @brief  Defines the RotateXDeg profile.
 *  @param  profile     Pointer to the environment definition of the ROTATEXDEG_PROFILE_TYPE
 */
void RotateXDeg__SetProfile(ROTATEXDEG_PROFILE_TYPE * profile)
{
    if ( profile == NULL )
    {
        LOG_ADD_EXCEPTION(PROFILE_NOT_DEFINED, 0);
    }
    else
    {
        RotateXDeg.Profile = profile;
    }
}


//---------------------------------------------------------------------------------------------------------------------
/**
 *  @brief  Executes the modules enter task/s.
 */
void RotateXDeg__Entry(void)
{
    if ( RotateXDeg.Profile == NULL )
    {
        LOG_ADD_EXCEPTION(PROFILE_NOT_DEFINED, 0);
    }
    else
    {
        RotateXDeg.Event_To_Return = ROTATEXDEG_EV_NONE;

        RotateXDegInitialize();
        if ( RotateXDeg.Profile->Spin.Target_Drum_Velocity >= 0 )
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
 *  @return     ROTATEXDEG_EVENT_TYPE
 */
ROTATEXDEG_EVENT_TYPE RotateXDeg__Do(void)
{
    if ( RotateXDeg.Profile == NULL )
    {
        return ( ROTATEXDEG_EV_NONE );
    }
    else
    {
        RotateXDegExecute();

        // define the module return event
        ROTATEXDEG_EVENT_TYPE ret_val = ROTATEXDEG_EV_NONE;
        if ( RotateXDeg.Event_To_Return != ROTATEXDEG_EV_NONE )
        {
            ret_val = RotateXDeg.Event_To_Return;
            RotateXDeg.Event_To_Return = ROTATEXDEG_EV_NONE;
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
 *  Clears the static variable Timer.
 */
static void ClearTimer(void)
{
    RotateXDeg.Timeout = 0;
}


//---------------------------------------------------------------------------------------------------------------------
/**
 *  Decrement the static variable Timer.
 */
static void DecrementTimer(void)
{
    RotateXDeg.Timeout = (RotateXDeg.Timeout > 0) ? RotateXDeg.Timeout - 1 : 0;
    if ( RotateXDeg.Timeout == 0 )
    {
        EventTimerExpired();
    }
}


//---------------------------------------------------------------------------------------------------------------------
/**
 *  Set static variable Timer to the Time_On.
 */
static void SetProfileTimeout(void)
{
    RotateXDeg.Timeout = RotateXDeg.Profile->Timeout / MOTION_HANDLER_EXECUTION_RATE;
}


//---------------------------------------------------------------------------------------------------------------------
/**
 *  Set event to return to the environment ccw stroke completed.
 */
static void SetEventToReturnCcwStrokeCompleted(void)
{
    RotateXDeg.Event_To_Return = ROTATEXDEG_EV_CCW_STROKE_COMPLETED;
}


//---------------------------------------------------------------------------------------------------------------------
/**
 *  Set event to return to the environment cw stroke completed.
 */
static void SetEventToReturnCwStrokeCompleted(void)
{
    RotateXDeg.Event_To_Return = ROTATEXDEG_EV_CW_STROKE_COMPLETED;
}


//---------------------------------------------------------------------------------------------------------------------
/**
 *  Set event to return to the environment error.
 */
static void SetEventToReturnError(void)
{
    RotateXDeg.Event_To_Return = ROTATEXDEG_EV_ERROR;
}


//---------------------------------------------------------------------------------------------------------------------
/**
 *  Command the drum to spin ccw.
 */
static void SpinDrumCcw(void)
{
    SPIN_DRUM_PROFILE_TYPE control = {
        .Drum_Acceleration.Type             = SPIN_DRUM_CONSTANT_ACCELERATION,
        .Drum_Acceleration.Value.Constant   = RotateXDeg.Profile->Spin.Drum_Acceleration.Value.Constant,
        .Target_Drum_Velocity               = abs(RotateXDeg.Profile->Spin.Target_Drum_Velocity) * -1    /* ensure the value is negative for the ccw direction */
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
        .Drum_Acceleration.Value.Constant   = RotateXDeg.Profile->Spin.Drum_Acceleration.Value.Constant,
        .Target_Drum_Velocity               = abs(RotateXDeg.Profile->Spin.Target_Drum_Velocity) /* ensure the value is positive for the cw direction */
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
//=== StateEngineCrank (v2.8) Auto-Generated Functions for RotateXDeg State Machine
//=====================================================================================================================
// !! StateEngineCrank cannot update this code if you change the header !!


//---------------------------------------------------------------------------------------------------------------------
/**
 * Process the Start Direction Ccw Event for the RotateXDeg state machine.
 *
 *              >>> DO NOT MODIFY THIS FUNCTION <<<
 *
 * This function was auto-generated and must not be edited manually.
 * Change the UML used to generate this function and regenerate the source code.
 */
static void EventStartDirectionCcw(void)
{
    switch (RotateXDeg_State)
    {
        case STATE_ROTATEXDEG_INITIALIZE:
            RotateXDeg_State = STATE_ROTATEXDEG_CCW_WAIT_FOR_MCI;
            RotateXDeg_Transitioned = FALSE;
            if (RotateXDeg_Transitioned == FALSE)
            {
                SpinDrumCcw();                      // Entry Function.
            }
            RotateXDeg_Transitioned = TRUE;
            break;

        default:
            // Event is ignored for all other values of RotateXDeg_State.
            break;
    }
}


//---------------------------------------------------------------------------------------------------------------------
/**
 * Process the Start Direction Cw Event for the RotateXDeg state machine.
 *
 *              >>> DO NOT MODIFY THIS FUNCTION <<<
 *
 * This function was auto-generated and must not be edited manually.
 * Change the UML used to generate this function and regenerate the source code.
 */
static void EventStartDirectionCw(void)
{
    switch (RotateXDeg_State)
    {
        case STATE_ROTATEXDEG_INITIALIZE:
            RotateXDeg_State = STATE_ROTATEXDEG_CW_WAIT_FOR_MCI;
            RotateXDeg_Transitioned = FALSE;
            if (RotateXDeg_Transitioned == FALSE)
            {
                SpinDrumCw();                       // Entry Function.
            }
            RotateXDeg_Transitioned = TRUE;
            break;

        default:
            // Event is ignored for all other values of RotateXDeg_State.
            break;
    }
}


//---------------------------------------------------------------------------------------------------------------------
/**
 * Process the Timer Expired Event for the RotateXDeg state machine.
 *
 *              >>> DO NOT MODIFY THIS FUNCTION <<<
 *
 * This function was auto-generated and must not be edited manually.
 * Change the UML used to generate this function and regenerate the source code.
 */
static void EventTimerExpired(void)
{
    switch (RotateXDeg_State)
    {
        case STATE_ROTATEXDEG_CCW_TIME_ON:
            RotateXDeg_State = STATE_ROTATEXDEG_CCW_STOP;
            RotateXDeg_Transitioned = FALSE;
            ClearTimer();                           // Entry Function.
            if (RotateXDeg_Transitioned == FALSE)
            {
                StopDrumEntry();                    // Entry Function.
            }
            RotateXDeg_Transitioned = TRUE;
            break;

        case STATE_ROTATEXDEG_CW_TIME_ON:
            RotateXDeg_State = STATE_ROTATEXDEG_CW_STOP;
            RotateXDeg_Transitioned = FALSE;
            ClearTimer();                           // Entry Function.
            if (RotateXDeg_Transitioned == FALSE)
            {
                StopDrumEntry();                    // Entry Function.
            }
            RotateXDeg_Transitioned = TRUE;
            break;
        default:
            // Event is ignored for all other values of RotateXDeg_State.
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
            switch (RotateXDeg_State)
            {
                case STATE_ROTATEXDEG_CCW_WAIT_FOR_MCI:
                    RotateXDeg_State = STATE_ROTATEXDEG_CCW_TIME_ON;
                    SetProfileTimeout();      // Entry Function.
                    RotateXDeg_Transitioned = TRUE;
                    break;

                case STATE_ROTATEXDEG_CW_WAIT_FOR_MCI:
                    RotateXDeg_State = STATE_ROTATEXDEG_CW_TIME_ON;
                    SetProfileTimeout();      // Entry Function.
                    RotateXDeg_Transitioned = TRUE;
                    break;

                default:
                    // Event is ignored for all other values of RotateXDeg_State.
                    break;
            }
            break;

        case SPIN_DRUM_EV_ERROR:
            switch (RotateXDeg_State)
            {
                case STATE_ROTATEXDEG_CCW_WAIT_FOR_MCI:
                case STATE_ROTATEXDEG_CW_WAIT_FOR_MCI:
                    SetEventToReturnError();        // Transition action function.
                    RotateXDeg_State = STATE_ROTATEXDEG_NONE;   // Terminate RotateXDeg state machine execution.
                    RotateXDeg_Transitioned = TRUE;
                    break;

                default:
                    // Event is ignored for all other values of RotateXDeg_State.
                    break;
            }
            break;

        default:
            // All other external Spin Drum events are ignored by the RotateXDeg state machine.
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
            switch (RotateXDeg_State)
            {
                case STATE_ROTATEXDEG_CCW_STOP:
                    SetEventToReturnCcwStrokeCompleted(); // Exit function.
                    RotateXDeg_Transitioned = TRUE;
                    break;

                case STATE_ROTATEXDEG_CW_STOP:
                    SetEventToReturnCwStrokeCompleted(); // Exit function.
                    RotateXDeg_Transitioned = TRUE;
                    break;

                default:
                    // Event is ignored for all other values of RotateXDeg_State.
                    break;
            }
            break;

        default:
            // All other external Stop Drum events are ignored by the RotateXDeg state machine.
            break;
    }
}


//---------------------------------------------------------------------------------------------------------------------
/**
 * The main execution function for the RotateXDeg state machine.
 * This function should be called periodically while the state machine is running.
 *
 *              >>> DO NOT MODIFY THIS FUNCTION <<<
 *
 * This function was auto-generated and must not be edited manually.
 * Change the UML used to generate this function and regenerate the source code.
 */
static void RotateXDegExecute(void)
{
    RotateXDeg_Transitioned = FALSE;
    switch(RotateXDeg_State)
    {
        case STATE_ROTATEXDEG_CCW_WAIT_FOR_MCI:
        case STATE_ROTATEXDEG_CW_WAIT_FOR_MCI:
            if (RotateXDeg_Transitioned == FALSE)
            {
                SpinDrumDo();                       // Do function.
            }
            break;

        case STATE_ROTATEXDEG_CCW_TIME_ON:
        case STATE_ROTATEXDEG_CW_TIME_ON:
            DecrementTimer();                       // Do function.

            if(RotateXDeg.Angular_Distance.Rotor_Position_Target == (MciSensorsWm__GetAnalog(MOTOR0, MCISENSORSWM_AI_SHAFT_POSITION_DEG_S16)>>16)) // Angular target reached
            {
            	if(RotateXDeg_State == STATE_ROTATEXDEG_CW_TIME_ON)
            	{
            		RotateXDeg_State = STATE_ROTATEXDEG_CW_STOP;
            	}
            	else if(RotateXDeg_State == STATE_ROTATEXDEG_CCW_TIME_ON)
            	{
            		RotateXDeg_State = STATE_ROTATEXDEG_CCW_STOP;
            	}
            }
            else if(RotateXDeg.Profile->Target_Rpm == (MciSensorsWm__GetAnalog(MOTOR0, MCISENSORSWM_AI_ACTUAL_SPEED_S16)>>16))
            {
            	RotateXDeg.Profile->Spin.Drum_Acceleration.Value.Constant = RotateXDeg.Profile->Second_Ramp_Rpm;
                RotateXDeg__SetProfile(RotateXDeg.Profile);


                if ( RotateXDeg.Profile->Spin.Target_Drum_Velocity >= 0 )
                {
                    EventStartDirectionCw();
                }
                else
                {
                    EventStartDirectionCcw();
                }
            }
            break;

        case STATE_ROTATEXDEG_CCW_STOP:
        case STATE_ROTATEXDEG_CW_STOP:
            if (RotateXDeg_Transitioned == FALSE)
            {
                StopDrumDo();                       // Do function.
            }
            break;

        default:
            // There is nothing to do for all other values of RotateXDeg_State.
            break;
    }
}


//---------------------------------------------------------------------------------------------------------------------
/**
 * The initialization function for the RotateXDeg state machine.
 * This function resets the state machine to the initial state.
 *
 *              >>> DO NOT MODIFY THIS FUNCTION <<<
 *
 * This function was auto-generated and must not be edited manually.
 * Change the UML used to generate this function and regenerate the source code.
 */
static void RotateXDegInitialize(void)
{
    RotateXDeg_State = STATE_ROTATEXDEG_INITIALIZE;
    RotateXDeg_Transitioned = TRUE;

    RotateXDeg.Angular_Distance.Previous_Rotor_Position = (Mci__GetAnalog(MOTOR0, MCI_AI_SHAFT_POSITION_DEG_S16)>>16);
    RotateXDeg.Angular_Distance.Rotor_Position_Target = RotateXDeg.Angular_Distance.Previous_Rotor_Position + (RotateXDeg.Target_Degrees * 10);
}


// !! StateEngineCrank cannot update this code if you change the footer !!
// End of StateEngineCrank Auto-Generated Code.
