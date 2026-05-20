#include <stdlib.h>
#include <String.h>
#include "C_Extensions.h"
#include "Extraction.h"
#include "ExtractionParams.h"
#include "JtagTestInterface.h"
#include "MCI.h"
#include "Motion.h"
#include "RemoteFunctionExtraction.h"

#define JTAG_DEBUG_FS 25

typedef enum
{
	RAMP_PROFILE_STATE_IDLE,
	RAMP_PROFILE_STATE_RAMP,
	RAMP_PROFILE_STATE_PLATEAU
} STEPPED_RAMP_PROFILE_STATE_TYPE;

typedef struct
{
	sint32 SpeedIncrement;
	sint32 PlateauTime;
	sint32 InitialSpeed;
	sint32 SpeedTarget;
} STEPPED_RAMP_PROFILE_TYPE;

typedef enum
{
	NONE = -1,
	STOP_MOTOR = 0,
	START_MOTOR = 1,
	RELEASE_MOTION = 2,
	STOP_EXTRACTION = 3,
	START_EXTRACTION = 4,
	END_EXTRACTION = 5,
	STOP_STEPPED_RAMP_PROFILE,
	START_STEPPED_RAMP_PROFILE,
	NOTIFY_FOAM_DETECTION,
	RESUME_EXTRACTION,
	MEASURE_ACCEL_BASELINE
} JTAG_COMMAND_TYPE;

static JTAG_COMMAND_TYPE Jtag_Command = NONE;
static sint32 Jtag_Mci_SpeedRef;
static sint32 Jtag_Mci_Ramp;
static STEPPED_RAMP_PROFILE_TYPE Jtag_Stepped_Ramp_Profile;
static STEPPED_RAMP_PROFILE_STATE_TYPE Jtag_Stepped_Ramp_Profile_State;
static uint32 Stepped_Ramp_Profile_Timer;
static uint8 Extraction_Params[] = {0, 0, 0, 0, 25, 10, 0, 40, 180, 1, 20, 255};
static float Extraction_Float_Fb[EXTR_FLOAT_FB_ENUM_COUNT];
static sint32 Extraction_Int_Fb[EXTR_INT32_FB_ENUM_COUNT];
static MOTION_REQUESTOR_TYPE Jtag_Motion_Requestor;
static uint8 Jtag_Extraction_Index;

void SteppedRampProfileHandler(JTAG_COMMAND_TYPE command);
void SpinMotor(sint32 speed, sint32 ramp);
void StopMotor(sint32 ramp);
void UpdateExtractionFbArrays(void);

void JtagTestInterface__Init(void)
{
	Jtag_Command = NONE;
	Jtag_Mci_SpeedRef = 90;
	Jtag_Mci_Ramp = 50;
	Jtag_Stepped_Ramp_Profile.SpeedIncrement = 5;
	Jtag_Stepped_Ramp_Profile.PlateauTime = 10;
	Jtag_Stepped_Ramp_Profile.InitialSpeed = 60;
	Jtag_Stepped_Ramp_Profile.SpeedTarget = 400;
	Jtag_Motion_Requestor = MOTION_REQUESTOR_EXTRACTION;
	Jtag_Extraction_Index = 0;
}

void JtagTestInterface__Handler(void)
{
	switch(Jtag_Command)
	{
	case STOP_MOTOR:
		StopMotor(200);
		break;
	case START_MOTOR:
		SpinMotor(Jtag_Mci_SpeedRef,Jtag_Mci_Ramp);
		break;
	case RELEASE_MOTION:
		Motion__Release(Jtag_Motion_Requestor);
		break;
	case STOP_EXTRACTION:
//		Extraction__Stop();
		RemoteFunctionExtraction__DoCommand(REMOTE_FUNCTION_COMMAND_DISABLE, Extraction_Params, sizeof(Extraction_Params));
		break;
	case START_EXTRACTION:
//		ExtractionParams__Initialize(0,0);
//		Extraction__ResetLoadMass();
//		Extraction__Start();
//		RemoteFunctionExtraction__DoCommand(6, Extraction_Params, sizeof(Extraction_Params));
		Extraction_Params[0] = Jtag_Extraction_Index;
		RemoteFunctionExtraction__DoCommand(REMOTE_FUNCTION_COMMAND_ENABLE, Extraction_Params, sizeof(Extraction_Params));
		break;
	case END_EXTRACTION:
//		Extraction__Stop();
		RemoteFunctionExtraction__DoCommand(6, Extraction_Params, sizeof(Extraction_Params));
		break;
	case NOTIFY_FOAM_DETECTION:
//		Extraction__NotifyFoamDetection();
		RemoteFunctionExtraction__DoCommand(4, Extraction_Params, sizeof(Extraction_Params));
		break;
	case RESUME_EXTRACTION:
//		Extraction__NotifyFoamDetection();
		RemoteFunctionExtraction__DoCommand(5, Extraction_Params, sizeof(Extraction_Params));
		break;
	}

	SteppedRampProfileHandler(Jtag_Command);

	UpdateExtractionFbArrays();

	Jtag_Command = NONE;
}

void JtagTestInterface__StartRamp(void)
{
	Jtag_Command = START_STEPPED_RAMP_PROFILE;
}

void JtagTestInterface__StopRamp(void)
{
	Jtag_Command = STOP_STEPPED_RAMP_PROFILE;
}

void SpinMotor(sint32 speed, sint32 ramp)
{
	MOTION_PROFILE_TYPE motion_prof;
	motion_prof.Spin.Drum_Acceleration = ramp;
	motion_prof.Spin.Target_Drum_Velocity = speed;
	Motion__Activity(MOTION_ACTIVITY_SPIN,motion_prof,Jtag_Motion_Requestor);
}

void StopMotor(sint32 ramp)
{
	MOTION_PROFILE_TYPE motion_prof;
	motion_prof.Stop.Drum_Deceleration = ramp;
	Motion__Activity(MOTION_ACTIVITY_STOP,motion_prof,Jtag_Motion_Requestor);
}

void SteppedRampProfileHandler(JTAG_COMMAND_TYPE command)
{
	if (command == STOP_STEPPED_RAMP_PROFILE)
	{
		Jtag_Stepped_Ramp_Profile_State = RAMP_PROFILE_STATE_IDLE;
		StopMotor(200);
		return;
	}

	switch(Jtag_Stepped_Ramp_Profile_State)
	{
	case RAMP_PROFILE_STATE_IDLE:
		if (command == START_STEPPED_RAMP_PROFILE)
		{
			Jtag_Mci_SpeedRef = Jtag_Stepped_Ramp_Profile.InitialSpeed;
			SpinMotor(Jtag_Mci_SpeedRef,Jtag_Mci_Ramp);
			Jtag_Stepped_Ramp_Profile_State = RAMP_PROFILE_STATE_RAMP;
		}
		break;
	case RAMP_PROFILE_STATE_RAMP:
		if (abs(abs((Mci__GetAnalog(MOTOR0, MCI_AI_SPEED_REFERENCE_RPM_S16)>>12)) - abs(16*Jtag_Mci_SpeedRef)) <= 8)
		{
			Stepped_Ramp_Profile_Timer = (uint32)(Jtag_Stepped_Ramp_Profile.PlateauTime * JTAG_DEBUG_FS);
			Jtag_Stepped_Ramp_Profile_State = RAMP_PROFILE_STATE_PLATEAU;
		}
		break;
	case RAMP_PROFILE_STATE_PLATEAU:
		if (Stepped_Ramp_Profile_Timer > 0)
		{
			Stepped_Ramp_Profile_Timer--;
		}
		else
		{
			if (abs(abs((Mci__GetAnalog(MOTOR0, MCI_AI_SPEED_REFERENCE_RPM_S16)>>12)) - abs(16*Jtag_Stepped_Ramp_Profile.SpeedTarget)) <= 8)
			{
				Jtag_Stepped_Ramp_Profile_State = RAMP_PROFILE_STATE_IDLE;
				StopMotor(200);
			}
			else
			{
				Jtag_Mci_SpeedRef = Jtag_Mci_SpeedRef + Jtag_Stepped_Ramp_Profile.SpeedIncrement;
				if ( Jtag_Mci_SpeedRef > Jtag_Stepped_Ramp_Profile.SpeedTarget )
				{
					Jtag_Mci_SpeedRef = Jtag_Stepped_Ramp_Profile.SpeedTarget;
				}
				Jtag_Stepped_Ramp_Profile_State = RAMP_PROFILE_STATE_RAMP;
				SpinMotor(Jtag_Mci_SpeedRef,Jtag_Mci_Ramp);
			}
			break;
		}
	}
}

#pragma optimize = none
void UpdateExtractionFbArrays(void)
{
	for (int ii = 0; ii < EXTR_FLOAT_FB_ENUM_COUNT; ii++)
	{
		Extraction_Float_Fb[ii] = Extraction__GetFloat((EXTR_FLOAT_FB_ENUM)ii);
	}

	for (int ii = 0; ii < EXTR_INT32_FB_ENUM_COUNT; ii++)
	{
		Extraction_Int_Fb[ii] = Extraction__GetInteger((EXTR_INT32_FB_ENUM)ii);
	}
}

