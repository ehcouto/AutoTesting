/**
 *  @file
 *
 *  @brief      Source file for the Speed Maintain module.
 *
 *  @details    This module is a motion requestor that will maintain the
 *				running where there is no active motion requestor.
 **/

//-------------------------------------- Include Files ----------------------------------------------------------------

#include <stdlib.h>
#include <string.h>
#include "MciSensorsWm.h"
#include "Motion.h"
#include "Mci.h"

//-------------------------------------- PRIVATE (Variables, Constants & Defines) -------------------------------------

// Enable automatic speed monitoring
#define SPEED_MAINTAIN_SPEED_MONITOR_ENABLED			FALSE

// Speed monitor execution rate in milliseconds
#define SPEED_MAINTAIN_RATE_MS							200

// Speed maintain handler rate
#define SPEED_MAINTAIN_HANDLER_RATE_MS					25

// Target acceleration for speed commands
#define SPEED_MAINTAIN_TARGET_ACCELERATION				25

// Minimum speed error delta in order to issue a new command
#define SPEED_MAINTAIN_MIN_SPEED_ERROR					5

//! Minimum supported speed
#define SPEED_MAINTAIN_MIN_SPEED						(sint16)((float32)MIN_SPEED_RPM/MciSensorsWm__GetTransmissionRatio()) + 1

//! Maximum supported speed
#define SPEED_MAINTAIN_MAX_SPEED						(sint16)((float32)MAX_SPEED_RPM/MciSensorsWm__GetTransmissionRatio()) - 1

//! Maximum supported ramp
#define SPEED_MAINTAIN_MAX_RAMP							(sint16)((float32)MAX_RPM_PER_SEC_ACCEL/MciSensorsWm__GetTransmissionRatio()) - 1

//-------------------------------------- PRIVATE (Function Prototypes) ------------------------------------------------

static uint16 Speed_Maintain_Prescaler;
#if SPEED_MAINTAIN_SPEED_MONITOR_ENABLED
static MOTION_DRUM_VELOCITY_TYPE Speed_Maintain_Current_Drum_Speed;
#endif
static MOTION_DRUM_VELOCITY_TYPE Speed_Maintain_Mci_Target_Speed;
static MOTION_DRUM_VELOCITY_TYPE Speed_Maintain_Prev_Drum_Speed;
static MOTION_PROFILE_TYPE Speed_Maintain_Motion_Profile;
static BOOL_TYPE Speed_Maintain_Enabled;

static void StopMotor(void);

//=====================================================================================================================
//-------------------------------------- Public Functions -------------------------------------------------------------
//=====================================================================================================================

//---------------------------------------------------------------------------------------------------------------------
/**
 * Initialize the Speed Maintain module.
 */
void SpeedMaintain__Initialize(void)
{
#if SPEED_MAINTAIN_SPEED_MONITOR_ENABLED
	Speed_Maintain_Prev_Drum_Speed = 0;
#endif
	Speed_Maintain_Prescaler = SPEED_MAINTAIN_RATE_MS/SPEED_MAINTAIN_HANDLER_RATE_MS;
	Speed_Maintain_Enabled = FALSE;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * Queues a motion spin activity.
 * @param mot_prof - Motion profile that will be queued.
 */
void SpeedMaintain__SetSpinActivity(MOTION_PROFILE_TYPE motion_prof)
{
	Speed_Maintain_Prev_Drum_Speed = (MOTION_DRUM_VELOCITY_TYPE)(motion_prof.Spin.Target_Drum_Velocity);
	memcpy(&Speed_Maintain_Motion_Profile,&motion_prof,sizeof(MOTION_SPIN_PROFILE_TYPE));

	/* Check if the ramp rate is smaller than a certain value, to avoid the motor starting with a small ramp
	 rate in case it stops and has to be restarted with this command. */
	if (abs(Speed_Maintain_Motion_Profile.Spin.Drum_Acceleration) < SPEED_MAINTAIN_TARGET_ACCELERATION)
	{
		Speed_Maintain_Motion_Profile.Spin.Drum_Acceleration = (MOTION_DRUM_ACCELERATION_TYPE)SPEED_MAINTAIN_TARGET_ACCELERATION;
	}

	Motion__Activity(MOTION_ACTIVITY_SPIN, Speed_Maintain_Motion_Profile, MOTION_REQUESTOR_SPEED_MAINTAIN);
	Speed_Maintain_Enabled = TRUE;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * Releases Motion.
 */
void SpeedMaintain__ReleaseMotion(void)
{
	Motion__Release(MOTION_REQUESTOR_SPEED_MAINTAIN);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * Speed maintain handler function.
 */
void SpeedMaintain__Handler25ms(void)
{
	if (Speed_Maintain_Prescaler <= 0)
	{
#if SPEED_MAINTAIN_SPEED_MONITOR_ENABLED
		Speed_Maintain_Current_Drum_Speed = (MOTION_DRUM_VELOCITY_TYPE)((float32)MciSensorsWm__GetAnalog(MOTOR0, MCISENSORSWM_AI_SPEED_REFERENCE_RPM_S16)/65536.0f);

		if(abs(Speed_Maintain_Prev_Drum_Speed - Speed_Maintain_Current_Drum_Speed) > SPEED_MAINTAIN_MIN_SPEED_ERROR)
		{
			if (Speed_Maintain_Current_Drum_Speed > SPEED_MAINTAIN_MIN_SPEED && Speed_Maintain_Current_Drum_Speed < SPEED_MAINTAIN_MAX_SPEED)
			{
				Speed_Maintain_Prev_Drum_Speed = Speed_Maintain_Current_Drum_Speed;
				Speed_Maintain_Motion_Profile.Spin.Target_Drum_Velocity = (MOTION_DRUM_VELOCITY_TYPE)Speed_Maintain_Current_Drum_Speed;
				Speed_Maintain_Motion_Profile.Spin.Drum_Acceleration = (MOTION_DRUM_ACCELERATION_TYPE)SPEED_MAINTAIN_TARGET_ACCELERATION;
				Motion__Activity(MOTION_ACTIVITY_SPIN, Speed_Maintain_Motion_Profile, MOTION_REQUESTOR_SPEED_MAINTAIN);
			}
			else
			{
				SpeedMaintain__ReleaseMotion();
			}
		}
#endif

		if (Speed_Maintain_Enabled)
		{
			// Releases Motion if a MCI fault is detected or if the current motion requestor is not HA Extraction or SpeedMaintain.
			if (!(Motion__GetCurrentRequestor() == MOTION_REQUESTOR_EXTRACTION || Motion__GetCurrentRequestor() == MOTION_REQUESTOR_SPEED_MAINTAIN)
					|| Mci__GetErrorList(MOTOR0) != 0)
			{
				SpeedMaintain__ReleaseMotion();
				Speed_Maintain_Enabled = FALSE;
			}
		}

		Speed_Maintain_Prescaler = SPEED_MAINTAIN_RATE_MS/SPEED_MAINTAIN_HANDLER_RATE_MS;
	}
	else
	{
		Speed_Maintain_Prescaler--;
	}
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * Request motion to stop the motor.
 */
static void StopMotor(void)
{
	MOTION_PROFILE_TYPE stop_profile;
	stop_profile.Stop.Drum_Deceleration = (MOTION_DRUM_ACCELERATION_TYPE)SPEED_MAINTAIN_MAX_RAMP;
	Motion__Activity(MOTION_ACTIVITY_STOP, stop_profile, MOTION_REQUESTOR_SPEED_MAINTAIN);
}
