/**
 *  @file
 *
 *  @brief      Source file for the Motion module.
 *
 *  @details    Handles controlling drum.  Will only allow one activity to be active at a time.  Uses Arbitration
 *              to ensure consistent machine behavior.
 *

    @startuml{Motion_class_diagram.jpg}
        title Motion Class Diagram

        hide empty attributes
        hide empty methods
        hide circle
        skinparam classAttributeIconSize 0


        class Motion {
            + Motion__Initialize
            + Motion__Background
            + Motion__GetStatus : MOTION_STATUS_TYPE
            + Motion__Activity(activity : MOTION_ACTIVITY_TYPE, * profile : MOTION_PROFILE_TYPE, requestor : MOTION_REQUESTOR_TYPE)
            + Motion__Release(MOTION_REQUESTOR_TYPE)
        }
        class MOTION_PROFILE_TYPE << interface >>


        class Pulse {
            + Pulse__Entry(* profile : PULSE_PROFILE_TYPE)
            + Pulse__Do : PULSE_EVENT_TYPE
        }
        class PULSE_PROFILE_TYPE << interface >> {
            + Pulse_Behavior: PULSE_BEHAVIOR_TYPE
            + Spin_Profile : SPIN_DRUM_PROFILE_TYPE
            + Time_On_Msec: unsigned long
            + Time_Off_MSec: unsigned long
        }
        class PULSE_BEHAVIOR_TYPE << enumeration >> {
            Run Pulse Immediately
            Modify Pulse
            Load Pulse Into Queue
        }


        class StopDrum {
            + StopDrum__Entry
            + StopDrum__Do : STOP_DRUM_EVENT_TYPE
        }


        class SpinDrum {
            + SpinDrum__Entry(* profile : SPIN_DRUM_PROFILE_TYPE)
            + SpinDrum__Do : SPIN_DRUM_EVENT_TYPE
        }
        class SPIN_DRUM_PROFILE_TYPE << interface >> {
            + Drum_Rpm  : signed short
            + Drum_Ramp : unsigned short
        }



        class Wash {
            + Wash__SetProfile(* profile : WASH_PROFILE_TYPE)
            + Wash__Entry
            + Wash__Do : WASH_EVENT_TYPE
        }
        class WASH_PROFILE_TYPE << interface >> {
            + Spin_Drum : SPIN_DRUM_PROFILE_TYPE
            + Time_On_Msec : unsigned short
            + Time_Off_Msec : unsigned short
        }




        Environment ..> Motion

        Motion ....> Pulse
        Motion ....> Wash
        Motion ....> SpinDrum
        Motion ....> StopDrum
        Motion .|> MOTION_PROFILE_TYPE

        MOTION_PROFILE_TYPE ..|> WASH_PROFILE_TYPE
        MOTION_PROFILE_TYPE ..|> SPIN_DRUM_PROFILE_TYPE
        MOTION_PROFILE_TYPE ..|> PULSE_PROFILE_TYPE

        Pulse ...> StopDrum
        Pulse ...> SpinDrum
        Pulse .|> PULSE_PROFILE_TYPE
        PULSE_PROFILE_TYPE ..|> SPIN_DRUM_PROFILE_TYPE

        SpinDrum .|> SPIN_DRUM_PROFILE_TYPE
        SpinDrum ..> Mci

        StopDrum ..> Mci

        Wash ...> SpinDrum
        Wash ...> StopDrum
        Wash .|> WASH_PROFILE_TYPE
        WASH_PROFILE_TYPE ..|> SPIN_DRUM_PROFILE_TYPE

    @enduml


    @startuml{Motion_sequence_diagram_module_idle.jpg}
        title Motion Sequence Diagram

        participant "Environment" as env

        ==When the Motion module is idle, it will respond immediately to a command to enable an activity==
            env -> Motion : Motion__GetStatus()
            activate Motion
            env <-- Motion : MOTION_STATUS_IDLE
            deactivate Motion
            alt commanded activity spin
                env -> Motion : Motion__Activity(MOTION_ACTIVITY_SPIN)
                env -> Motion : Motion__GetStatus()
                activate Motion
                env <-- Motion : MOTION_STATUS_SPINNING
                deactivate Motion
            else commanded activity stop
                env -> Motion : Motion__Activity(MOTION_ACTIVITY_STOP)
                env -> Motion : Motion__GetStatus()
                activate Motion
                env <-- Motion : MOTION_STATUS_IDLE
                deactivate Motion
            else commanded activity wash
                env -> Motion : Motion__Activity(MOTION_ACTIVITY_WASH)
                env -> Motion : Motion__GetStatus()
                activate Motion
                env <-- Motion : MOTION_STATUS_WASHING
                deactivate Motion
            end

        ==When Motion is not idle, it needs to stop the current activity prior to enabling the commanded activity==
            env -> Motion : Motion__GetStatus()
            activate Motion
            alt currently spinning
                env <-- Motion : MOTION_STATUS_SPINNING
            else currently stopping
                env <-- Motion : MOTION_STATUS_STOPPING
            else currently washing
                env <-- Motion : MOTION_STATUS_WASHING
                deactivate Motion
            end
            alt commmanded MOTION_ACTIVITY_SPIN
                env -> Motion : Motion__Activity(MOTION_ACTIVITY_SPIN)
            else commmanded MOTION_ACTIVITY_STOP
                env -> Motion : Motion__Activity(MOTION_ACTIVITY_STOP)
            else commmanded MOTION_ACTIVITY_WASH
                env -> Motion : Motion__Activity(MOTION_ACTIVITY_WASH)
            end
            loop
                env -> Motion : Motion__GetStatus()
                activate Motion
                alt Motion is transitioning to commanded activity
                    env <-- Motion : MOTION_STATUS_STOPPING
                else commanded activity is active
                    alt commmanded MOTION_ACTIVITY_SPIN
                        env <-- Motion : MOTION_STATUS_SPINNING
                    else commmanded MOTION_ACTIVITY_STOP
                        env <-- Motion : MOTION_STATUS_IDLE
                    else commmanded MOTION_ACTIVITY_WASH
                        env <-- Motion : MOTION_STATUS_WASHING
                        deactivate Motion
                    end
                end
            end

        ==Motion will resume activity after an Error has been detected and recovered from==
            env -> Motion : Motion__GetStatus()
            activate Motion
            env <-- Motion : MOTION_STATUS_WASHING
            deactivate Motion
            Motion -> Motion : error detected
            env -> Motion : Motion__GetStatus()
            activate Motion
            env <-- Motion : MOTION_STATUS_ERROR
            deactivate Motion
            Motion -> Motion : error recovered
            env -> Motion : Motion__GetStatus()
            activate Motion
            env <-- Motion : MOTION_STATUS_WASHING
            deactivate Motion

    @enduml



    @startuml{Motion_state_machine.jpg}
        title Motion State Machine

        [*] --> Initialize

        Initialize : do / InitializeDo()
        Initialize --> Stopping : evModuleInitialized

        Idle : entry /\t VerifyActivityIsStop()
        Idle : do /\t VerifyDrumIsStopped() \n\t VerifyActivityIsStop()
        Idle : exit /\t SetMciDoClearAllFailure()
        Idle --> Error      : evMciErrorDetected
		Idle --> Pulse      : evEnableActivity \n [IsActivityPulse()] \n/ Pulse__Initialize()
        Idle --> Spin       : evEnableActivity \n [IsActivitySpin()]
        Idle --> Wash       : evEnableActivity \n [IsActivityWash()] \n/ WashSetProfile() \n Wash__Entry()
        Idle --> Stopping   : evDrumMovementDetected

        Error : entry /\t MotionError__Enable()
        Error : do /\t MotionErrorDo()
        Error --> Stopping  : MOTION_ERROR\n(MOTION_ERROR_EV_HANDLED)

		Pulse : entry /\t PulseEntry()
        Pulse : do /\t PulseDo()
        Pulse --> Error     : evMciErrorDetected
        Pulse --> Pulse     : evEnableActivity \n [IsActivityPulse()]
        Pulse --> Stopping  : evEnableActivity \n [!IsActivityPulse()]
        Pulse --> Stopping  : PULSE\n(PULSE_EV_SELF_TERMINATED) \n/ ResetActivity()
        Pulse --> Error     : PULSE\n(PULSE_EV_ERROR)

        Spin : entry /\t SpinDrumEntry()
        Spin : do /\t SpinDrumDo()
        Spin --> Error      : evMciErrorDetected
        Spin --> Error      : SPIN_DRUM\n(SPIN_DRUM_EV_ERROR)
        Spin --> Spin       : evEnableActivity \n [IsActivitySpin()]
        Spin --> Stopping   : evEnableActivity \n [!IsActivitySpin()]

        Stopping : entry /\t StopDrumEntry()
        Stopping : do /\t VerifyDrumHasStopped()
        Stopping --> Error  : evMciErrorDetected
        Stopping --> Idle   : STOP_DRUM\n(STOP_DRUM_EV_STOPPED)

        Wash : do /\t WashDo()
        Wash --> Error      : evMciErrorDetected
        Wash --> Stopping   : evEnableActivity \n [!IsActivityWash()]
        Wash --> Wash       : evEnableActivity \n [IsActivityWash()] \n/ WashSetProfile()
        Wash --> Error      : WASH\n(WASH_EV_ERROR)

    @enduml

 *
 *  $Header: Motion.c 1.10 2015/12/01 14:55:10EST Paul F Spindler (SPINDPF) Exp  $
 *
 *  @copyright  Copyright 2015-$Date: 2015/12/01 14:55:10EST $. Whirlpool Corporation. All rights reserved - CONFIDENTIAL
 */
//---------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------


//-------------------------------------- Include Files ----------------------------------------------------------------
#include "C_Extensions.h"

#include "Motion.h"
#include "Motion_prv.h"

#include "Arbitration.h"
#include "Log.h"
#include "Mci.h"
#include "MotionError.h"
#include "Pulse.h"
#include "RotateXDeg.h"
#include "SpinDrum.h"
#include "StopDrum.h"
#include "Utilities.h"
#include "Wash.h"

#include "stdlib.h"
#include "string.h"

//=====================================================================================================================
//=== StateEngineCrank (v2.8) Auto-Generated Definitions for Motion State Machine
//=====================================================================================================================
// !! StateEngineCrank cannot update this code if you change the header !!


//! Unique names used to identify states of the Motion state machine.
typedef enum MOTION_STATE_ENUM
{
    STATE_MOTION_NONE       	= 0,

    STATE_MOTION_INITIALIZE 	= 1,                    //!< (initial_state)
    STATE_MOTION_STOPPING   	= 2,
    STATE_MOTION_IDLE       	= 3,
    STATE_MOTION_ERROR      	= 4,
    STATE_MOTION_PULSE      	= 5,
    STATE_MOTION_SPIN       	= 6,
    STATE_MOTION_WASH       	= 7,
    STATE_MOTION_ROTATE_X_DEG 	= 8,

    STATE_MOTION_END        	= 9
} MOTION_STATE_TYPE;

//! The state variable for the Motion state machine.
//! State machines with a history pseudostate may write to this variable during initialization.
//! Other state machines must only write to this variable through auto-generated functions.
static MOTION_STATE_TYPE Motion_State;

//! The transition variable for the Motion state machine.
//! Flag used by the state machine to track when a transition occurs in the middle of a state's
//! Entry() or Do() action that contains more than one function.
static BOOL_TYPE Motion_Transitioned;

// -- Private Function Prototypes used by the state machine for customization functions --
static void InitializeDo(void);
static BOOL_TYPE IsActivityPulse(void);
static BOOL_TYPE IsActivitySpin(void);
static BOOL_TYPE IsActivityWash(void);
static BOOL_TYPE IsActivityRotateXDeg(void);
static void MotionErrorDo(void);
static void PulseDo(void);
static void PulseEntry(void);
static void RotateXDegDo(void);
static void RotateXDegSetProfile(void);
static void ResetActivity(void);
static void SetMciDoClearAllFailure(void);
static void SpinDrumDo(void);
static void SpinDrumEntry(void);
static void StopDrumEntry(void);
static void VerifyActivityIsStop(void);
static void VerifyDrumHasStopped(void);
static void VerifyDrumIsStopped(void);
static void WashDo(void);
static void WashSetProfile(void);

// -- Private Function Prototypes used by the state machine for standard state machine functions --
static void EventDrumMovementDetected(void);
static void EventEnableActivity(void);
static void EventMciErrorDetected(void);
static void EventModuleInitialized(void);
static void ExternalEventMotionError(uint8 event);
static void ExternalEventPulse(uint8 event);
static void ExternalEventSpinDrum(uint8 event);
static void ExternalEventStopDrum(uint8 event);
static void ExternalEventWash(uint8 event);
static void ExternalEventRotateXDeg(uint8 event);
static void MotionExecute(void);
static void MotionInitialize(void);
static void MotionTerminate(void);

// !! StateEngineCrank cannot update this code if you change the footer !!
// End of StateEngineCrank Auto-Generated Code.

//-------------------------------------- PRIVATE (Variables, Constants & Defines) -------------------------------------

//! list of module Log messages
typedef enum
{
    MESSAGE_NONE = 0,
    INVALID_MOTION_BEHAVIOR,                        //!< data: requested motion behavior
    MCI_REJECTED_CLEAR_ALL_FAILURE,                 //!< data: Mci module response
    SPIN_MAX_DRUM_RPM_EXCEEDED,                     //!< data: commanded drum rpm
    UNCOMMANDED_MOVEMENT_DETECTED,                  //!< data: bit field with failed test result
    WASH_MAX_DRUM_RPM_EXCEEDED                      //!< data: commanded drum rpm
} MODULE_MOTION_LOG_MESSAGE_ID_TYPE;
#define MODULE_NAME MODULE_MOTION


#define MODULE_EXECUTION_TERMINATED()       ( ((Motion_State == STATE_MOTION_NONE) || (Motion_State == STATE_MOTION_END)) ? TRUE : FALSE )


//! list of all variables that need to be arbitrated
typedef struct
{
    MOTION_PROFILE_TYPE     Profile;
    MOTION_ACTIVITY_TYPE    Activity;
} ACTION_TYPE;

//! default
#define MOTION_DEFAULT_ACTIVITY \
{\
    .Profile.Stop = 0x00,\
    .Activity = MOTION_ACTIVITY_STOP\
}

//! control structure definition for the Arbitration module
DEFINE_ARBITRATION_CONTROL_STRUCTURE(Motion_Arb, ACTION_TYPE, MOTION_DEFAULT_ACTIVITY, MOTION_REQUESTOR_COUNT);

//! list of module static variables
static struct
{
    ACTION_TYPE             Action;
    union
    {
        PULSE_PROFILE_TYPE      			Pulse;
        SPIN_DRUM_PROFILE_TYPE  			SpinDrum;
        STOP_DRUM_PROFILE_TYPE  			Stop;
        WASH_PROFILE_TYPE       			Wash;
        ROTATEXDEG_PROFILE_TYPE    	Rotate_X_Deg;
    } Sub_Module_Profile;
    MOTION_REQUESTOR_TYPE   Requestor;
} Motion;

//-------------------------------------- PRIVATE (Function Prototypes) ------------------------------------------------

static void HandleArbitratedAction(void);

//=====================================================================================================================
//-------------------------------------- Public Functions -------------------------------------------------------------
//=====================================================================================================================

//---------------------------------------------------------------------------------------------------------------------
/**
 *  @brief      It Initializes the module Motion and its variables
 *
 */
void Motion__Initialize(void)
{
    memset(&Motion, 0, sizeof(Motion));
    Arbitration__Initialize(&Motion_Arb);
    MotionInitialize();
    MOTION__OPTIONAL_INITIALIZE();
}


//---------------------------------------------------------------------------------------------------------------------
/**
 *  Executes the modules background task/s.
 */
void Motion__Background(void)
{
    if ( MODULE_EXECUTION_TERMINATED() == FALSE)
    {
        if ( Mci__GetError(MOTOR0, MCI_ERROR_ANY_ERROR_FOUND) != 0 )
        {
            EventMciErrorDetected();
        }
        else
        {
            MotionExecute();
        }
    }
}


//---------------------------------------------------------------------------------------------------------------------
/**
 *  Executes the commanded action.
 *
 *  @param  behavior    the commanded activity
 *  @param  profile     the commanded activities profile, each activity can have its own profile
 *  @param  requestor   the commanded activity requestor
 */


void Motion__Activity(MOTION_ACTIVITY_TYPE activity, MOTION_PROFILE_TYPE profile, MOTION_REQUESTOR_TYPE requestor)
{
    ACTION_TYPE action;
    switch ( activity )
    {
        case MOTION_ACTIVITY_PULSE:
            action.Activity = MOTION_ACTIVITY_PULSE;
            if ( abs(profile.Pulse.Spin_Drum_Profile.Target_Drum_Velocity) > abs(MOTION_MAX_DRUM_RPM) )
            {
                LOG_ADD_EXCEPTION(SPIN_MAX_DRUM_RPM_EXCEEDED, profile.Pulse.Spin_Drum_Profile.Target_Drum_Velocity);
                return; // exit function
            }
            // else drum velocity has not exceeded the limit, do nothing
            break;
        case MOTION_ACTIVITY_SPIN:
            action.Activity = MOTION_ACTIVITY_SPIN;
            if ( abs(profile.Spin.Target_Drum_Velocity) > abs(MOTION_MAX_DRUM_RPM) )
            {
                LOG_ADD_EXCEPTION(SPIN_MAX_DRUM_RPM_EXCEEDED, profile.Spin.Target_Drum_Velocity);
                return; // exit function
            }
            break;
            // else the drum velocity has not exceeded the limit, do nothing
        case MOTION_ACTIVITY_STOP:
            action.Activity = MOTION_ACTIVITY_STOP;
            break;
        case MOTION_ACTIVITY_TERMINATE:
            action.Activity = MOTION_ACTIVITY_TERMINATE;
            break;
        case MOTION_ACTIVITY_WASH:
            action.Activity = MOTION_ACTIVITY_WASH;
            if ( abs(profile.Wash.Spin.Target_Drum_Velocity) > abs(MOTION_MAX_DRUM_RPM) )
            {
                LOG_ADD_EXCEPTION(WASH_MAX_DRUM_RPM_EXCEEDED, profile.Wash.Spin.Target_Drum_Velocity);
                return; // exit function
            }
            // else the drum velocity has not exceeded the limit, do nothing
            break;
        case MOTION_ACTIVITY_ROTATE_X_DEG:
            action.Activity = MOTION_ACTIVITY_ROTATE_X_DEG;
            if ( abs(profile.Rotate_X_Deg.Target_Rpm) > abs(MOTION_MAX_DRUM_RPM) )
            {
                LOG_ADD_EXCEPTION(SPIN_MAX_DRUM_RPM_EXCEEDED, profile.Rotate_X_Deg.Target_Rpm);
                return; // exit function
            }
            // else drum velocity has not exceeded the limit, do nothing
            break;
        default:
            LOG_ADD_EXCEPTION(INVALID_MOTION_BEHAVIOR, activity);
            return; // exit function
    }
    action.Profile = profile;
    Arbitration__RequestControl(&Motion_Arb, &action, requestor);
    HandleArbitratedAction();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 *  Passed Requestor is releasing control of the module.
 *
 *  @param  requestor   the requestor
 */
void Motion__Release(MOTION_REQUESTOR_TYPE requestor)
{
    Arbitration__ReleaseControl(&Motion_Arb, requestor);
    HandleArbitratedAction();
}


//---------------------------------------------------------------------------------------------------------------------
/**
 *  Returns the current status of the module
 *
 *  @return current status of the module
 */
MOTION_STATUS_TYPE Motion__GetStatus(void)
{
    // map each module state to a status
    static const MOTION_STATUS_TYPE MAP_STATE_TO_STATUS[] =
    {
        /* STATE_MOTION_NONE        	*/  MOTION_STATUS_TERMINATED,
        /* STATE_MOTION_INITIALIZE  	*/  MOTION_STATUS_STOPPING,
        /* STATE_MOTION_STOPPING    	*/  MOTION_STATUS_STOPPING,
        /* STATE_MOTION_IDLE        	*/  MOTION_STATUS_IDLE,
        /* STATE_MOTION_ERROR       	*/  MOTION_STATUS_ERROR,
        /* STATE_MOTION_PULSE       	*/  MOTION_STATUS_PULSE,
        /* STATE_MOTION_SPIN        	*/  MOTION_STATUS_SPINNING,
        /* STATE_MOTION_WASH        	*/  MOTION_STATUS_WASHING,
        /* STATE_MOTION_ROTATE_X_DEG	*/  MOTION_STATUS_ROTATING_X_DEX
    };
    CT_ASSERT(sizeof(MAP_STATE_TO_STATUS) / sizeof(MOTION_STATUS_TYPE) == STATE_MOTION_END);
    return ( MAP_STATE_TO_STATUS[Motion_State] );
}

//---------------------------------------------------------------------------------------------------------------------
/**
 *  Returns the current arbitration requestor of the module
 *
 *  @return current requestor of the module
 */
MOTION_REQUESTOR_TYPE Motion__GetCurrentRequestor(void)
{
	for (int i = 0; i < MOTION_REQUESTOR_COUNT; i++)
	{
		if (Utilities__GetBitInArray(Motion_Arb.Active_Bits,i))
		{
			return (MOTION_REQUESTOR_TYPE)i;
		}
	}
	return MOTION_REQUESTOR_COUNT;
}

//=====================================================================================================================
//-------------------------------------- Private Functions ------------------------------------------------------------
//=====================================================================================================================

//---------------------------------------------------------------------------------------------------------------------
/**
 *  Handle the arbitrated action
 */
static void HandleArbitratedAction(void)
{
    Arbitration__GetPrioritizedActionAndRequestor(&Motion_Arb, (void*)&Motion.Action, (uint8*)&Motion.Requestor);   //lint !e928 Suppress Info: cast from pointer to pointer [MISRA 2004 Rule 11.4]

    switch ( Motion.Action.Activity )
    {
        case MOTION_ACTIVITY_TERMINATE:
            if ( MODULE_EXECUTION_TERMINATED() == FALSE )
            {                                               // module is currently executing and needs to be terminated
                MotionTerminate();
            }
            // else module execution is already terminated, do nothing
            break;
        default:
            /*
             *  Check if module execution is currently terminated.  If it is, the state machine and any
             *  sub modules need to be re-initialized.
             */
            if ( MODULE_EXECUTION_TERMINATED() == TRUE )
            {
                MotionInitialize();                                     // reinitialize the state machine
            }
            // else module is not terminated, do nothing

            EventEnableActivity();                                      // handle the arbitrated action activity
            break;
    }
}


//=====================================================================================================================
//  --- Private State Machine Customization Functions -----------------------------------------------------------------
//=====================================================================================================================

//---------------------------------------------------------------------------------------------------------------------
/**
 *  Creates a delay to allow the modules that Motion is dependent on a chance to initialize prior to Motion sending
 *  them any commands.
 */
static void InitializeDo(void)
{
    EventModuleInitialized();
}


//---------------------------------------------------------------------------------------------------------------------
/**
 *
 * @return  TRUE = commmanded activity is Pulse; FALSE = not pulse
 */
static BOOL_TYPE IsActivityPulse(void)
{
    return ( MOTION_ACTIVITY_PULSE == Motion.Action.Activity ? TRUE : FALSE );
}


//---------------------------------------------------------------------------------------------------------------------
/**
 *
 * @return  TRUE = commanded activity is Spin; FALSE = not Spin
 */
static BOOL_TYPE IsActivitySpin(void)
{
    return ( MOTION_ACTIVITY_SPIN == Motion.Action.Activity ? TRUE : FALSE );
}


//---------------------------------------------------------------------------------------------------------------------
/**
 *
 * @return  TRUE = commanded activity is Wash; FALSE = not Wash
 */
static BOOL_TYPE IsActivityWash(void)
{
    return ( MOTION_ACTIVITY_WASH == Motion.Action.Activity ? TRUE : FALSE );
}

//---------------------------------------------------------------------------------------------------------------------
/**
 *
 * @return  TRUE = commanded activity is RotateXDeg; FALSE = not Wash
 */
static BOOL_TYPE IsActivityRotateXDeg(void)
{
    return ( MOTION_ACTIVITY_ROTATE_X_DEG == Motion.Action.Activity ? TRUE : FALSE );
}

//---------------------------------------------------------------------------------------------------------------------
/**
 *  Executes the MotionError module background function.
 */
static void MotionErrorDo(void)
{
    MOTION_ERROR_EVENT_TYPE event = MotionError__Do();
    if ( MOTION_ERROR_EV_NONE != event )
    {
        ExternalEventMotionError((uint8)event);
    }
    // else do nothing
}


//---------------------------------------------------------------------------------------------------------------------
/**
 *  Executes the Pulse state background task/s.
 */
static void PulseDo(void)
{
    PULSE_EVENT_TYPE event = Pulse__Do();
    if ( PULSE_EV_NONE != event )
    {
        ExternalEventPulse((uint8)event);
    }
}


//---------------------------------------------------------------------------------------------------------------------
/**
 *  Executes the Pulse state entry task/s.
 */
static void PulseEntry(void)
{
    MOTION_PULSE_PROFILE_TYPE * motion_pulse = &Motion.Action.Profile.Pulse;
    PULSE_PROFILE_TYPE * pulse = &Motion.Sub_Module_Profile.Pulse;
    pulse->Behavior                                 = motion_pulse->Behavior;
    pulse->Spin.Drum_Acceleration.Type              = SPIN_DRUM_ACCELERATE_FOR_TIME;
    pulse->Spin.Drum_Acceleration.Value.Time        = motion_pulse->Spin_Drum_Profile.Drum_Acceleration;
    pulse->Spin.Target_Drum_Velocity                = motion_pulse->Spin_Drum_Profile.Target_Drum_Velocity;
    pulse->Time_Off                                 = motion_pulse->Time_Off;
    pulse->Time_On                                  = motion_pulse->Time_On;
    Pulse__Entry((const PULSE_PROFILE_TYPE *)pulse);     //lint !e929 Suppress Info: cast from pointer to pointer [MISRA 2004 Rule 11.4]
}


//---------------------------------------------------------------------------------------------------------------------
/**
 *  Sets the Rotate X Deg state profile.
 */
static void RotateXDegSetProfile(void)
{
	MOTION_ROTATEXDEG_PROFILE_TYPE * motion_rotatexdeg 	= &Motion.Action.Profile.Rotate_X_Deg;
    ROTATEXDEG_PROFILE_TYPE * rotate 					= &Motion.Sub_Module_Profile.Rotate_X_Deg;
    rotate->Spin.Drum_Acceleration.Type           		= SPIN_DRUM_CONSTANT_ACCELERATION;
    rotate->Spin.Drum_Acceleration.Value.Constant 		= motion_rotatexdeg->Spin.Drum_Acceleration;
    rotate->Spin.Target_Drum_Velocity             		= motion_rotatexdeg->Spin.Target_Drum_Velocity;
    rotate->Timeout                               		= motion_rotatexdeg->Timeout;
    RotateXDeg__SetProfile(rotate);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 *  Resets the requested activity to the default activity.
 */
static void ResetActivity(void)
{
    ACTION_TYPE action = MOTION_DEFAULT_ACTIVITY;
    Motion.Action = action;
}


//---------------------------------------------------------------------------------------------------------------------
/**
 *  Clears the Mci module failure flag.
 */
static void SetMciDoClearAllFailure(void)
{
    uint8 ret_val = Mci__ClearError(MOTOR0);
    if ( 0 == ret_val )
    {
        LOG_ADD_EXCEPTION(MCI_REJECTED_CLEAR_ALL_FAILURE, ret_val);
    }
}


//---------------------------------------------------------------------------------------------------------------------
/**
 *  Executes the Spin state entry task/s.
 */
static void SpinDrumEntry(void)
{
    MOTION_SPIN_PROFILE_TYPE * motion_spin = &Motion.Action.Profile.Spin;
    SPIN_DRUM_PROFILE_TYPE * spin_drum = &Motion.Sub_Module_Profile.SpinDrum;
    spin_drum->Drum_Acceleration.Type           = SPIN_DRUM_CONSTANT_ACCELERATION;
    spin_drum->Drum_Acceleration.Value.Constant = motion_spin->Drum_Acceleration;
    spin_drum->Target_Drum_Velocity             = motion_spin->Target_Drum_Velocity;
    SpinDrum__Entry(spin_drum);
}


//---------------------------------------------------------------------------------------------------------------------
/**
 *  Executes the Spin state background task/s.
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
 *  Executes the Stop state entry tasks.
 */
static void StopDrumEntry(void)
{
    MOTION_STOP_PROFILE_TYPE* motion_stop = &Motion.Action.Profile.Stop;
    STOP_DRUM_PROFILE_TYPE* stop_drum = &Motion.Sub_Module_Profile.Stop;
    stop_drum->Drum_Deceleration = motion_stop->Drum_Deceleration;
    StopDrum__Entry(stop_drum);
}


//---------------------------------------------------------------------------------------------------------------------
/**
 *  Verifies that the current commanded activity is stop.
 */
static void VerifyActivityIsStop(void)
{
    if ( MOTION_ACTIVITY_STOP != Motion.Action.Activity )
    {
        EventEnableActivity();
    }
}


//---------------------------------------------------------------------------------------------------------------------
/**
 *  Verifies the StopDrum module has reported the drum has stopped.
 */
static void VerifyDrumHasStopped(void)
{
    if ( STOP_DRUM_EV_STOPPED == StopDrum__Do() )
    {
        ExternalEventStopDrum((uint8)STOP_DRUM_EV_STOPPED);
    }
}


//---------------------------------------------------------------------------------------------------------------------
/**
 *  Verifies the StopDrum is reporting the drum is not moving.
 */
static void VerifyDrumIsStopped(void)
{
    if ( STOP_DRUM_EV_STOPPED != StopDrum__Do() )
    {
        LOG_ADD_EVENT(UNCOMMANDED_MOVEMENT_DETECTED, 0);
        EventDrumMovementDetected();
    }
    // else drum is stopped, do nothing
}


//---------------------------------------------------------------------------------------------------------------------
/**
 *  Wash state background task/s.
 */
static void WashDo(void)
{
    WASH_EVENT_TYPE event = Wash__Do();
    if ( WASH_EV_NONE != event )
    {
        ExternalEventWash((uint8)event);
    }
    // else do nothing
}

//---------------------------------------------------------------------------------------------------------------------
/**
 *  RotateXDeg state background task/s.
 */
static void RotateXDegDo(void)
{
	ROTATEXDEG_EVENT_TYPE event = RotateXDeg__Do();
    if ( ROTATEXDEG_EV_NONE != event )
    {
        ExternalEventWash((uint8)event);
    }
    // else do nothing
}

//---------------------------------------------------------------------------------------------------------------------
/**
 *  Sets the Wash state profile.
 */
static void WashSetProfile(void)
{
    MOTION_WASH_PROFILE_TYPE * motion_wash = &Motion.Action.Profile.Wash;
    WASH_PROFILE_TYPE * wash = &Motion.Sub_Module_Profile.Wash;
    wash->Spin.Drum_Acceleration.Type           = SPIN_DRUM_CONSTANT_ACCELERATION;
    wash->Spin.Drum_Acceleration.Value.Constant = motion_wash->Spin.Drum_Acceleration;
    wash->Spin.Target_Drum_Velocity             = motion_wash->Spin.Target_Drum_Velocity;
    wash->Time_Off                              = motion_wash->Time_Off;
    wash->Time_On                               = motion_wash->Time_On;
    Wash__SetProfile(wash);
}


//=====================================================================================================================
//=== StateEngineCrank (v2.8) Auto-Generated Functions for Motion State Machine
//=====================================================================================================================
// !! StateEngineCrank cannot update this code if you change the header !!


//---------------------------------------------------------------------------------------------------------------------
/**
 * Process the Drum Movement Detected Event for the Motion state machine.
 *
 *              >>> DO NOT MODIFY THIS FUNCTION <<<
 *
 * This function was auto-generated and must not be edited manually.
 * Change the UML used to generate this function and regenerate the source code.
 */
static void EventDrumMovementDetected(void)
{
    switch (Motion_State)
    {
        case STATE_MOTION_IDLE:
            SetMciDoClearAllFailure();              // Exit function.
            Motion_State = STATE_MOTION_STOPPING;
            StopDrumEntry();                        // Entry Function.
            Motion_Transitioned = TRUE;
            break;

        default:
            // Event is ignored for all other values of Motion_State.
            break;
    }
}


//---------------------------------------------------------------------------------------------------------------------
/**
 * Process the Enable Activity Event for the Motion state machine.
 *
 *              >>> DO NOT MODIFY THIS FUNCTION <<<
 *
 * This function was auto-generated and must not be edited manually.
 * Change the UML used to generate this function and regenerate the source code.
 */
static void EventEnableActivity(void)
{
    switch (Motion_State)
    {
        case STATE_MOTION_IDLE:
            if (IsActivityPulse() == TRUE)
            {
                SetMciDoClearAllFailure();          // Exit function.
                Pulse__Initialize();                // Transition action function.
                Motion_State = STATE_MOTION_PULSE;
                PulseEntry();                       // Entry Function.
                Motion_Transitioned = TRUE;
            }
            else if (IsActivitySpin() == TRUE)
            {
                SetMciDoClearAllFailure();          // Exit function.
                Motion_State = STATE_MOTION_SPIN;
                SpinDrumEntry();                    // Entry Function.
                Motion_Transitioned = TRUE;
            }
            else if (IsActivityWash() == TRUE)
            {
                SetMciDoClearAllFailure();          // Exit function.
                WashSetProfile();                   // Transition action function.
                Wash__Entry();                      // Transition action function.
                Motion_State = STATE_MOTION_WASH;
                Motion_Transitioned = TRUE;
            }
            else if (IsActivityRotateXDeg() == TRUE)
            {
                SetMciDoClearAllFailure();          // Exit function.
                RotateXDegSetProfile();             // Transition action function.
                RotateXDeg__Entry();				// Transition action function.
                Motion_State = STATE_MOTION_ROTATE_X_DEG;
                Motion_Transitioned = TRUE;
            }
            break;

        case STATE_MOTION_PULSE:
            if (IsActivityPulse() == TRUE)
            {
                Motion_State = STATE_MOTION_PULSE;
                PulseEntry();                       // Entry Function.
                Motion_Transitioned = TRUE;
            }
            else if (IsActivityPulse() == FALSE)
            {
                Motion_State = STATE_MOTION_STOPPING;
                StopDrumEntry();                    // Entry Function.
                Motion_Transitioned = TRUE;
            }
            break;

        case STATE_MOTION_SPIN:
            if (IsActivitySpin() == TRUE)
            {
                Motion_State = STATE_MOTION_SPIN;
                SpinDrumEntry();                    // Entry Function.
                Motion_Transitioned = TRUE;
            }
            else if (IsActivitySpin() == FALSE)
            {
                Motion_State = STATE_MOTION_STOPPING;
                StopDrumEntry();                    // Entry Function.
                Motion_Transitioned = TRUE;
            }
            break;

        case STATE_MOTION_WASH:
            if (IsActivityWash() == FALSE)
            {
                Motion_State = STATE_MOTION_STOPPING;
                StopDrumEntry();                    // Entry Function.
                Motion_Transitioned = TRUE;
            }
            else if (IsActivityWash() == TRUE)
            {
                WashSetProfile();                   // Transition action function.
                Motion_State = STATE_MOTION_WASH;
                Motion_Transitioned = TRUE;
            }
            break;

        case STATE_MOTION_ROTATE_X_DEG:
            if (IsActivityRotateXDeg() == TRUE)
            {
                RotateXDegSetProfile();                   // Transition action function.
                Motion_State = STATE_MOTION_ROTATE_X_DEG;
                Motion_Transitioned = TRUE;
            }
            else if (IsActivityRotateXDeg() == FALSE)
            {
                Motion_State = STATE_MOTION_STOPPING;
                StopDrumEntry();                    // Entry Function.
                Motion_Transitioned = TRUE;
            }
            break;

        default:
            // Event is ignored for all other values of Motion_State.
            break;
    }
}


//---------------------------------------------------------------------------------------------------------------------
/**
 * Process the Mci Error Detected Event for the Motion state machine.
 *
 *              >>> DO NOT MODIFY THIS FUNCTION <<<
 *
 * This function was auto-generated and must not be edited manually.
 * Change the UML used to generate this function and regenerate the source code.
 */
static void EventMciErrorDetected(void)
{
    switch (Motion_State)
    {
        case STATE_MOTION_STOPPING:
        case STATE_MOTION_PULSE:
        case STATE_MOTION_SPIN:
        case STATE_MOTION_WASH:
            Motion_State = STATE_MOTION_ERROR;
            MotionError__Enable();                  // Entry Function.
            Motion_Transitioned = TRUE;
            break;

        case STATE_MOTION_IDLE:
            SetMciDoClearAllFailure();              // Exit function.
            Motion_State = STATE_MOTION_ERROR;
            MotionError__Enable();                  // Entry Function.
            Motion_Transitioned = TRUE;
            break;

        default:
            // Event is ignored for all other values of Motion_State.
            break;
    }
}


//---------------------------------------------------------------------------------------------------------------------
/**
 * Process the Module Initialized Event for the Motion state machine.
 *
 *              >>> DO NOT MODIFY THIS FUNCTION <<<
 *
 * This function was auto-generated and must not be edited manually.
 * Change the UML used to generate this function and regenerate the source code.
 */
static void EventModuleInitialized(void)
{
    switch (Motion_State)
    {
        case STATE_MOTION_INITIALIZE:
            Motion_State = STATE_MOTION_STOPPING;
            StopDrumEntry();                        // Entry Function.
            Motion_Transitioned = TRUE;
            break;

        default:
            // Event is ignored for all other values of Motion_State.
            break;
    }
}


//---------------------------------------------------------------------------------------------------------------------
/**
 * Process an external Motion Error event.
 *
 *              >>> DO NOT MODIFY THIS FUNCTION <<<
 *
 * This function was auto-generated and must not be edited manually.
 * Change the UML used to generate this function and regenerate the source code.
 *
 * @param event = The external event to be processed.
 */
static void ExternalEventMotionError(uint8 event)
{
    switch (event)
    {
        case MOTION_ERROR_EV_HANDLED:
            switch (Motion_State)
            {
                case STATE_MOTION_ERROR:
                    Motion_State = STATE_MOTION_STOPPING;
                    StopDrumEntry();                // Entry Function.
                    Motion_Transitioned = TRUE;
                    break;

                default:
                    // Event is ignored for all other values of Motion_State.
                    break;
            }
            break;

        default:
            // All other external Motion Error events are ignored by the Motion state machine.
            break;
    }
}


//---------------------------------------------------------------------------------------------------------------------
/**
 * Process an external Pulse event.
 *
 *              >>> DO NOT MODIFY THIS FUNCTION <<<
 *
 * This function was auto-generated and must not be edited manually.
 * Change the UML used to generate this function and regenerate the source code.
 *
 * @param event = The external event to be processed.
 */
static void ExternalEventPulse(uint8 event)
{
    switch (event)
    {
        case PULSE_EV_SELF_TERMINATED:
            switch (Motion_State)
            {
                case STATE_MOTION_PULSE:
                    ResetActivity();                // Transition action function.
                    Motion_State = STATE_MOTION_STOPPING;
                    StopDrumEntry();                // Entry Function.
                    Motion_Transitioned = TRUE;
                    break;

                default:
                    // Event is ignored for all other values of Motion_State.
                    break;
            }
            break;

        case PULSE_EV_ERROR:
            switch (Motion_State)
            {
                case STATE_MOTION_PULSE:
                    Motion_State = STATE_MOTION_ERROR;
                    MotionError__Enable();          // Entry Function.
                    Motion_Transitioned = TRUE;
                    break;

                default:
                    // Event is ignored for all other values of Motion_State.
                    break;
            }
            break;

        default:
            // All other external Pulse events are ignored by the Motion state machine.
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
            switch (Motion_State)
            {
                case STATE_MOTION_SPIN:
                    Motion_State = STATE_MOTION_ERROR;
                    MotionError__Enable();          // Entry Function.
                    Motion_Transitioned = TRUE;
                    break;

                default:
                    // Event is ignored for all other values of Motion_State.
                    break;
            }
            break;

        default:
            // All other external Spin Drum events are ignored by the Motion state machine.
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
            switch (Motion_State)
            {
                case STATE_MOTION_STOPPING:
                    Motion_State = STATE_MOTION_IDLE;
                    VerifyActivityIsStop();         // Entry Function.
                    Motion_Transitioned = TRUE;
                    break;

                default:
                    // Event is ignored for all other values of Motion_State.
                    break;
            }
            break;

        default:
            // All other external Stop Drum events are ignored by the Motion state machine.
            break;
    }
}


//---------------------------------------------------------------------------------------------------------------------
/**
 * Process an external Wash event.
 *
 *              >>> DO NOT MODIFY THIS FUNCTION <<<
 *
 * This function was auto-generated and must not be edited manually.
 * Change the UML used to generate this function and regenerate the source code.
 *
 * @param event = The external event to be processed.
 */
static void ExternalEventWash(uint8 event)
{
    switch (event)
    {
        case WASH_EV_ERROR:
            switch (Motion_State)
            {
                case STATE_MOTION_WASH:
                    Motion_State = STATE_MOTION_ERROR;
                    MotionError__Enable();          // Entry Function.
                    Motion_Transitioned = TRUE;
                    break;

                default:
                    // Event is ignored for all other values of Motion_State.
                    break;
            }
            break;

        default:
            // All other external Wash events are ignored by the Motion state machine.
            break;
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * Process an external RotateXDeg event.
 *
 *              >>> DO NOT MODIFY THIS FUNCTION <<<
 *
 * This function was auto-generated and must not be edited manually.
 * Change the UML used to generate this function and regenerate the source code.
 *
 * @param event = The external event to be processed.
 */
static void ExternalEventRotateXDeg(uint8 event)
{
    switch (event)
    {
        case ROTATEXDEG_EV_ERROR:
            switch (Motion_State)
            {
                case STATE_MOTION_ROTATE_X_DEG:
                    Motion_State = STATE_MOTION_ERROR;
                    MotionError__Enable();          // Entry Function.
                    Motion_Transitioned = TRUE;
                    break;

                default:
                    // Event is ignored for all other values of Motion_State.
                    break;
            }
            break;

        default:
            // All other external Wash events are ignored by the Motion state machine.
            break;
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * The main execution function for the Motion state machine.
 * This function should be called periodically while the state machine is running.
 *
 *              >>> DO NOT MODIFY THIS FUNCTION <<<
 *
 * This function was auto-generated and must not be edited manually.
 * Change the UML used to generate this function and regenerate the source code.
 */
static void MotionExecute(void)
{
    Motion_Transitioned = FALSE;
    switch(Motion_State)
    {
        case STATE_MOTION_INITIALIZE:
            InitializeDo();                         // Do function.
            break;

        case STATE_MOTION_STOPPING:
            VerifyDrumHasStopped();                 // Do function.
            break;

        case STATE_MOTION_IDLE:
            VerifyDrumIsStopped();                  // Do function.
            if (Motion_Transitioned == FALSE)
            {
                VerifyActivityIsStop();             // Do function.
            }
            break;

        case STATE_MOTION_ERROR:
            MotionErrorDo();                        // Do function.
            break;

        case STATE_MOTION_PULSE:
            PulseDo();                              // Do function.
            break;

        case STATE_MOTION_SPIN:
            SpinDrumDo();                           // Do function.
            break;

        case STATE_MOTION_WASH:
            WashDo();                               // Do function.
            break;
        case STATE_MOTION_ROTATE_X_DEG:             // Do function.
        	RotateXDegDo();
        	break;

        default:
            // There is nothing to do for all other values of Motion_State.
            break;
    }
}


//---------------------------------------------------------------------------------------------------------------------
/**
 * The initialization function for the Motion state machine.
 * This function resets the state machine to the initial state.
 *
 *              >>> DO NOT MODIFY THIS FUNCTION <<<
 *
 * This function was auto-generated and must not be edited manually.
 * Change the UML used to generate this function and regenerate the source code.
 */
static void MotionInitialize(void)
{
    Motion_State = STATE_MOTION_INITIALIZE;
    Motion_Transitioned = TRUE;
}


//---------------------------------------------------------------------------------------------------------------------
/**
 * The termination function for the Motion state machine.
 * This function disables the state machine. Calls to StateMachineExecute() will have no effect.
 * The StateMachineInitialize() function must be called again to restart the state machine.
 * If the current state has an exit function, it will be called.
 *
 *              >>> DO NOT MODIFY THIS FUNCTION <<<
 *
 * This function was auto-generated and must not be edited manually.
 * Change the UML used to generate this function and regenerate the source code.
 */
static void MotionTerminate(void)
{
    switch (Motion_State)
    {
        case STATE_MOTION_IDLE:
            SetMciDoClearAllFailure();              // Exit function.
            break;

        default:
            // There is nothing to do for all other values of Motion_State.
            break;
    }
    Motion_State = STATE_MOTION_NONE;
    Motion_Transitioned = TRUE;
}


// !! StateEngineCrank cannot update this code if you change the footer !!
// End of StateEngineCrank Auto-Generated Code.
