/**
 *  @file
 *
 *  @brief      API file for the Motion module.
 *
 *  $Header: Motion.h 1.8 2016/03/02 10:38:43EST Paul F Spindler (SPINDPF) Exp  $
 *
 *  @copyright  Copyright 2015-$Date: 2016/03/02 10:38:43EST $. Whirlpool Corporation. All rights reserved - CONFIDENTIAL
 */
//---------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------
#ifndef MOTION_H_
#define MOTION_H_

#include "MotionDeclare.h"
#include "PulseDeclare.h"

//=====================================================================================================================
//-------------------------------------- PUBLIC (Extern Variables, Constants & Defines) -------------------------------
//=====================================================================================================================

//! list of module status's
typedef enum
{
    MOTION_STATUS_IDLE = 0,     	//!< the module is currently Idle, drum is stopped, no errors have been detected
    MOTION_STATUS_ERROR,        	//!< an error has been detected and the module is currently handling the error
    MOTION_STATUS_PULSE,        	//!< the module is currently running a pulse
    MOTION_STATUS_SPINNING,     	//!< the module is currently spinning the drum for an indeterminate period of time
    MOTION_STATUS_STOPPING,     	//!< the module is currently stopping the drum
    MOTION_STATUS_TERMINATED,   	//!< module execution was terminated
    MOTION_STATUS_WASHING,      	//!< the module is currently moving the drum in a wash pattern for an indeterminate period of time
    MOTION_STATUS_ROTATING_X_DEX,   //!< the module is currently moving the drum to a targeted drum angle
} MOTION_STATUS_TYPE;

//! list of motion behaviors
typedef enum
{
    MOTION_ACTIVITY_STOP = 0,       //!< stops the drum
    MOTION_ACTIVITY_PULSE,          //!< runs a pulse
    MOTION_ACTIVITY_SPIN,           //!< spins the drum
    MOTION_ACTIVITY_TERMINATE,      //!< disables the Motion module, state machine execution is terminated
    MOTION_ACTIVITY_WASH,           //!< drum will run the wash profile
    MOTION_ACTIVITY_ROTATE_X_DEG,   //!< drum will rotate for X degrees
} MOTION_ACTIVITY_TYPE;

//! profile for MOTION_ACITIVITY_SPIN
typedef struct
{
    MOTION_DRUM_ACCELERATION_TYPE       Drum_Acceleration;
    MOTION_DRUM_VELOCITY_TYPE           Target_Drum_Velocity;
} MOTION_SPIN_PROFILE_TYPE;



//! profile for MOTION_ACTIVITY_PULSE
typedef struct
{
    PULSE_BEHAVIOR_TYPE         Behavior;
    MOTION_SPIN_PROFILE_TYPE    Spin_Drum_Profile;
    uint32                      Time_On;            // 1 = 1msec
    uint32                      Time_Off;           // 1 = 1msec
} MOTION_PULSE_PROFILE_TYPE;

//! profile for MOTION_ACTIVITY_WASH
typedef struct
{
    MOTION_SPIN_PROFILE_TYPE    Spin;           //!< the +/- from the Target_Drum_Velocity is used to indicate the start direction
    uint16                      Time_Off;       //!< time off, 1 = 1 msec
    uint16                      Time_On;        //!< time on, 1 = 1msec
} MOTION_WASH_PROFILE_TYPE;

typedef struct
{
    MOTION_DRUM_ACCELERATION_TYPE       Drum_Deceleration;
} MOTION_STOP_PROFILE_TYPE;

//! Structure defining the motion rotate x degrees profile parameters
typedef struct
{
	MOTION_SPIN_PROFILE_TYPE    Spin;
    uint16						Timeout;
    uint8  						Degrees;        // the angular distance in degrees the drum shall rotate for under power
    sint8  						Target_Rpm;     // the target drum rpm to spin at; + = cw direction; - = ccw direction
    uint8						Second_Ramp_Rpm;
} MOTION_ROTATEXDEG_PROFILE_TYPE;

//! each MOTION_ACTIVITY_TYPE can have an associated profile
typedef union
{
	MOTION_PULSE_PROFILE_TYPE   		Pulse;              //!< profile definition for MOTION_ACTIVITY_PULSE
	MOTION_STOP_PROFILE_TYPE    		Stop;               //!< profile definition for MOTION_ACTIVITY_STOP
    MOTION_SPIN_PROFILE_TYPE    		Spin;               //!< profile definition for the MOTION_ACTIVITY_SPIN
    uint8                       		Terminate;          //!< MOTION_ACTIVITY_TERMINATE has no parameters
    MOTION_WASH_PROFILE_TYPE    		Wash;               //!< profile definition for the MOTION_ACTIVITY_WASH
    MOTION_ROTATEXDEG_PROFILE_TYPE		Rotate_X_Deg;		//!< profile definition for MOTION_ACTIVITY_ROTATE_X_DEG
} MOTION_PROFILE_TYPE;

//! list of module requestors
typedef enum
{
    // Highest priority requestor
	MOTION_REQUESTOR_CHILD_SAFETY 	= 0,
	MOTION_REQUESTOR_EXTRACTION 	= 1,
    MOTION_REQUESTOR_API221 		= 2,
    MOTION_REQUESTOR_API039 		= 3,
	MOTION_REQUESTOR_SPEED_MAINTAIN = 4,
    // Lowest priority requestor

    MOTION_REQUESTOR_COUNT                          //!< Number of requestors supported by the module.
} MOTION_REQUESTOR_TYPE;

//=====================================================================================================================
//-------------------------------------- PUBLIC (Function Prototypes) -------------------------------------------------
//=====================================================================================================================

void Motion__Initialize(void);
void Motion__Background(void);

void Motion__Activity(MOTION_ACTIVITY_TYPE activity, MOTION_PROFILE_TYPE profile, MOTION_REQUESTOR_TYPE requestor);
void Motion__Release(MOTION_REQUESTOR_TYPE requestor);

MOTION_STATUS_TYPE Motion__GetStatus(void);
MOTION_REQUESTOR_TYPE Motion__GetCurrentRequestor(void);

#endif // MOTION_H_


