/**
 * @file
 * @brief       Implements an AP013 Remote Function server to interface with this remote function.
 *
 * @details
 *
 *      This module implements the Remote Function code for the standard API013 server interface.
 *
 *      A features:
 *          - Implements the four standard functions for Remote Functions.
 *          - A Background function that runs while the Remote Function is enabled.
 *          - A DoCommand handler to accept commands from the client.
 *          - A GetData handler to publish requested calculated results.
 *          - A GetStatus handler to report status if requested by client or pushed by server.
 *
 * @copyright   Copyright 2017 Whirlpool Corporation.  All rights reserved - CONFIDENTIAL.
 */

//  --- Include Files -------------------------------------------------------------------------------------------------
// -- This Module --
#include "C_Extensions.h"
#include "RemoteFunctionChildSafety.h"
// -- Other Modules --
#include <math.h>
#include "Mci.h"
#include "API013RemoteFunction.h"
#include "Motion.h"
#include "RemoteFunctionInterface.h"
#include "SettingFile.h"
#include <string.h>
#include "SystemConfig.h"
#include "SystemTimers.h"
#include "MciSensorsWm.h"

//  --- Private Declarations ------------------------------------------------------------------------------------------


// -- Private Constant Declarations --
#define CS_ACCEL_TO_LOWERS_ON                       32767   //!< [rpm/s] acceleration to make mci push lowers on after hitting the drum target angle


// -- Private Enumerated Constant Declarations --

//! Child safety status enumerator
typedef enum
{
    CHILD_SAFETY_ROUTINE_STATUS_IDLE = 0,
    CHILD_SAFETY_ROUTINE_STATUS_PROCESSING,
    CHILD_SAFETY_ROUTINE_STATUS_DONE,
    CHILD_SAFETY_ROUTINE_STATUS_STARTED,
    CHILD_SAFETY_ROUTINE_STATUS_RESUMED,
    CHILD_SAFETY_ROUTINE_STATUS_PAUSED,
    CHILD_SAFETY_ROUTINE_STATUS_INVALID
} CHILD_SAFETY_ROUTINE_STATUS_TYPE;



//! Child safety error declaration
typedef enum
{
    CHILD_SAFETY_ROUTINE_ERROR_NONE,                //!< No error was found
    CHILD_SAFETY_ROUTINE_ERROR_SETTING_FILE,        //!< Error found during parameters loading
    CHILD_SAFETY_ROUTINE_ERROR_TIMEOUT,             //!< Timeout elapsed
    CHILD_SAFETY_ROUTINE_ERROR_FOUND,               //!< An error not listed above, e.g., mci error
} CHILD_SAFETY_ROUTINE_ERROR_TYPE;



//! Child safety phase enumerator
typedef enum
{
    CHILD_SAFETY_ROUTINE_PHASE_NOT_DEFINED = 0,     //!< 0 - Not defined - it shall not be used
    CHILD_SAFETY_ROUTINE_PHASE_IDLE,                //!< 1 - Idle
    CHILD_SAFETY_ROUTINE_PHASE_INIT,                //!< 2 - Initialize (called once) all variables
    CHILD_SAFETY_ROUTINE_PHASE_WAIT_MCI_IDLE,       //!< 3 - Waiting for mci get into idle
    CHILD_SAFETY_ROUTINE_PHASE_ROTATING_X_DEG,      //!< 4 - Rotate drum by x degree according to settings file
    CHILD_SAFETY_ROUTINE_PHASE_PAUSE_TIME,          //!< 5 - Pause time after rotating x degree
    CHILD_SAFETY_ROUTINE_PHASE_END                  //!< 6 - Child safety movement performed
} CHILD_SAFETY_ROUTINE_PHASE_TYPE;



// -- Private Type Declarations --
typedef struct
{
   uint8 	Enable 			:1;                     //!< Child safety is enabled from settings file
   uint8 	not_used 		:7;                     //!< Not used
   uint8 	Acceleration_1;                         //!< [rpm/s] Acceleration_1
   uint8 	Acceleration_2;                         //!< Not used
   uint8 	Taget_Speed;                            //!< [rpm] target speed to command mci while moving
   uint8 	Child_Pause_Time;                       //!< [s] pause time after drum rotating a little bit
   uint8 	Timeout;                                //!< [s] timeout to perform whole algorithm
   uint8 	Subcycle_Priority;                      //!< Not used in MCU
   uint8 	Drum_Angle;                             //!< [degree] drum angle target - used to rotate drum
} CHILD_SAFETY_PARAMS_TYPE;

//! Child safety variables structure
typedef struct
{
   CHILD_SAFETY_ROUTINE_STATUS_TYPE Routine_Status  :3;
   CHILD_SAFETY_ROUTINE_PHASE_TYPE Routine_Phase    :3;
   CHILD_SAFETY_ROUTINE_ERROR_TYPE Routine_Error    :2;
} CHILD_SAFETY_ROUTINE_TYPE;


//! Child safety parameters - calculated from what GESE sent
typedef struct
{
    sint32 target_rpm;
    sint32 target_accel;
    sint32 target_deg;
    uint32 pause_after_run_sec;
    uint32 timeout;
} CHILD_SAFETY_PARAMETERS_TYPE;


// -- Private Variable Definitions --
static CHILD_SAFETY_PARAMS_TYPE *Child_Safety_Params;   //!< A point settings file
static CHILD_SAFETY_ROUTINE_TYPE Child_Safety_Status;   //!< Declare status structure
static uint16 Child_Safety_Timeout_Handler;             //!< Overall timeout handler
static uint16 Child_Safety_Wait_Timer_Handler;          //!< Wait timer after running
static CHILD_SAFETY_PARAMETERS_TYPE CS_Params;          //!< define the data used in the algorithm

// Move this defines to GESE
#define CS_SPEED_2_DRUM_POS_BELT_RATIO  1.0f            //! Define the belt ratio between motor and drum shafts.

// Local definition to use precompiler
#define CS_SPEED_SHIFT                  10              //! Define the speed resolution before integrating speed to calculate position
#define CS_SPEED_2_DRUM_POS_CALL_RATE   (25.0f/1000.0f)
#define CS_ANGLE_THRESHOLD_INT(angle)     ((angle)/(((CS_SPEED_2_DRUM_POS_CALL_RATE/CS_SPEED_2_DRUM_POS_BELT_RATIO)*(180.0f/30.0f))/(1<<(16-CS_SPEED_SHIFT))))


//  --- Private Function Prototypes -----------------------------------------------------------------------------------

static void sendStatusData (uint8 server_function_index_local);

//=====================================================================================================================
//  --- Public Functions ----------------------------------------------------------------------------------------------
//=====================================================================================================================

//---------------------------------------------------------------------------------------------------------------------
/**
 * Initialize all module variables.
 */
void RemoteFunctionChildSafety__Initialize(void)
{
    SETTINGFILE_LOADER_TYPE loader;

    // zero the status
    memset(&Child_Safety_Status, 0, sizeof(Child_Safety_Status));

    // Get timer handle for certain functionalities
    Child_Safety_Timeout_Handler = SystemTimers__GetHandle(SYSTEMTIMERS_TYPE_DOWN_COUNTER);
    Child_Safety_Wait_Timer_Handler = SystemTimers__GetHandle(SYSTEMTIMERS_TYPE_DOWN_COUNTER);

    // Read settings file and load data to the internal variables, if all timer handlers have been allocated properly
    if(((Child_Safety_Timeout_Handler != SYSTEMTIMERS_FAIL) &&
        (Child_Safety_Wait_Timer_Handler != SYSTEMTIMERS_FAIL)) &&
        (SettingFile__IsValid() == TRUE) &&
    	(SettingFile__BasicLoader(SF_PTR_MCU_MOTOR_APPLICATION_GENERIC_DATA, SF_DISPL_REMOTE_FUNCTION_CHILDSAFETY_PARAMS, &loader) == PASS))
    {
        Child_Safety_Params = (CHILD_SAFETY_PARAMS_TYPE *)loader.Data;	//loader.Data;	//lint !e927 Suppress Info: cast from pointer to pointer [MISRA 2004 Rule 11.4]

        CS_Params.target_rpm = Child_Safety_Params->Taget_Speed;
        CS_Params.target_accel = Child_Safety_Params->Acceleration_1;

        CS_Params.target_deg = (sint32)CS_ANGLE_THRESHOLD_INT((float)Child_Safety_Params->Drum_Angle);
        CS_Params.pause_after_run_sec = Child_Safety_Params->Child_Pause_Time;
        CS_Params.timeout = Child_Safety_Params->Timeout;

        Child_Safety_Status.Routine_Error = CHILD_SAFETY_ROUTINE_ERROR_NONE;
    }
    else
    {
        // Keep track of SF error
        Child_Safety_Status.Routine_Error = CHILD_SAFETY_ROUTINE_ERROR_SETTING_FILE;
    }
////// Temporary solution until GESE is solved
//    CS_Params.target_rpm = 40;
//    CS_Params.target_accel = 50;
//    CS_Params.target_deg = (sint32)CS_ANGLE_THRESHOLD_INT(120.0f);
//    CS_Params.pause_after_run_sec = 5;
//    Child_Safety_Status.Routine_Error = CHILD_SAFETY_ROUTINE_ERROR_NONE;
}




//---------------------------------------------------------------------------------------------------------------------
/**
 * Remote Function API Function: Background function that is called periodically while the Remote Function is active.
 * @return ACTIVE if the Remote Function should be called periodically; otherwise INACTIVE.
 *                  If the Remote Function is active, and if this function returns INACTIVE,
 *                  then the REMOTE_FUNCTION_GET_DATA_ELEMENTS API function will be called one more time.
 */
ACTIVE_TYPE RemoteFunctionChildSafety__Background(void)
{
    static sint32 drum_position_s32;
    ACTIVE_TYPE retval;
    MOTION_PROFILE_TYPE profile;
    uint32 temp_mci_error_list;

    switch(Child_Safety_Status.Routine_Status)
    {
        case CHILD_SAFETY_ROUTINE_STATUS_PROCESSING:

            // Check if the timeout is elapsed
            if(SystemTimers__GetState(Child_Safety_Timeout_Handler) == SYSTEMTIMERS_STATE_DONE)
            {
                Child_Safety_Status.Routine_Error = CHILD_SAFETY_ROUTINE_ERROR_TIMEOUT;
                Child_Safety_Status.Routine_Status = CHILD_SAFETY_ROUTINE_STATUS_DONE;
            }

            temp_mci_error_list = Mci__GetErrorList(MOTOR0);

            // Check if there is error from motor control
            if(temp_mci_error_list)
            {
            	if(Child_Safety_Status.Routine_Phase >= CHILD_SAFETY_ROUTINE_PHASE_ROTATING_X_DEG)
            	{
					Child_Safety_Status.Routine_Error = CHILD_SAFETY_ROUTINE_ERROR_FOUND;
					Child_Safety_Status.Routine_Status = CHILD_SAFETY_ROUTINE_STATUS_DONE;
            	}
            	else
            	{
            		Mci__ClearError(MOTOR0);
            	}
            }

            if(Child_Safety_Status.Routine_Error == CHILD_SAFETY_ROUTINE_ERROR_NONE)
            {
                switch(Child_Safety_Status.Routine_Phase)
                {

                    // Just initialize variables used by remote child safety algorithm
                    case CHILD_SAFETY_ROUTINE_PHASE_INIT:

                            //TODO: MOTION_ACTIVITY_ROTATE_X_DEG is not working (Motion module has to be reviewed) - Skip this phase. It is doing nothing.
//                        // Set motion to child safety
//                        profile.Rotate_X_Deg.Spin.Drum_Acceleration = CS_Params.target_accel;
//                        profile.Rotate_X_Deg.Spin.Target_Drum_Velocity = CS_Params.target_rpm;
//
//                        profile.Rotate_X_Deg.Timeout = CS_Params.timeout;
//                        profile.Rotate_X_Deg.Degrees = Child_Safety_Params->Drum_Angle;
//                        profile.Rotate_X_Deg.Target_Rpm = CS_Params.target_rpm;
//                        profile.Rotate_X_Deg.Second_Ramp_Rpm = Child_Safety_Params->Acceleration_2;
//
//                        // request to motion to lock the motion to child safety
//                        Motion__Activity(MOTION_ACTIVITY_ROTATE_X_DEG, profile, MOTION_REQUESTOR_CHILD_SAFETY);
                        Child_Safety_Status.Routine_Phase = CHILD_SAFETY_ROUTINE_PHASE_WAIT_MCI_IDLE;
                        break;



                        // Wait until Mci is ready
                    case CHILD_SAFETY_ROUTINE_PHASE_WAIT_MCI_IDLE:
                        if( (Mci__GetDigital(MOTOR0,MCI_DI_IDLE) == TRUE) &&
                        	(temp_mci_error_list == FALSE))
                        {
                            Child_Safety_Status.Routine_Phase = CHILD_SAFETY_ROUTINE_PHASE_ROTATING_X_DEG;

                            profile.Spin.Target_Drum_Velocity = (MOTION_DRUM_VELOCITY_TYPE)CS_Params.target_rpm;
                            profile.Spin.Drum_Acceleration = (MOTION_DRUM_ACCELERATION_TYPE)CS_Params.target_accel;
                            Motion__Activity(MOTION_ACTIVITY_SPIN, profile, MOTION_REQUESTOR_CHILD_SAFETY);

                            drum_position_s32 = 0;
                        }
                        break;




                    // Rotate the drum from certain degrees
                    case CHILD_SAFETY_ROUTINE_PHASE_ROTATING_X_DEG:

                        //Mci__Run(MOTOR0, CS_Params.target_rpm, CS_Params.target_accel);

                        //drum_position_s32 += (Mci__GetAnalog(MOTOR0, MCI_AI_ACTUAL_SPEED_S16)>>CS_SPEED_SHIFT);
                        drum_position_s32 += (MciSensorsWm__GetAnalog(MOTOR0, MCI_AI_ACTUAL_SPEED_S16)>>CS_SPEED_SHIFT);

                        // If travelled angle is met.
                        if(abs(drum_position_s32) >= CS_Params.target_deg)
                        {
                            // go to the next phase
                            Child_Safety_Status.Routine_Phase = CHILD_SAFETY_ROUTINE_PHASE_PAUSE_TIME;

                            // load pause time
                            SystemTimers__SetMs(Child_Safety_Wait_Timer_Handler, (CS_Params.pause_after_run_sec * 1000));

                            // request to stop motor
                            //Mci__Stop(MOTOR0, CS_ACCEL_TO_LOWERS_ON);
                            profile.Stop.Drum_Deceleration = CS_ACCEL_TO_LOWERS_ON;
                            Motion__Activity(MOTION_ACTIVITY_STOP, profile, MOTION_REQUESTOR_CHILD_SAFETY);
                            //Motion__Release(MOTION_REQUESTOR_CHILD_SAFETY);

                        }
                        break;



                    case CHILD_SAFETY_ROUTINE_PHASE_PAUSE_TIME:

                        // request to stop motor
                        //Mci__Stop(MOTOR0, CS_ACCEL_TO_LOWERS_ON);
                        //profile.Stop.Drum_Deceleration = CS_ACCEL_TO_LOWERS_ON;
                        //Motion__Activity(MOTION_ACTIVITY_STOP, profile, MOTION_REQUESTOR_CHILD_SAFETY);
                        //Motion__Release(MOTION_REQUESTOR_CHILD_SAFETY);

                        // check if the pause time is over
                        if(SystemTimers__GetState(Child_Safety_Wait_Timer_Handler) == SYSTEMTIMERS_STATE_DONE)
                        {
                            // go to end
                            Child_Safety_Status.Routine_Phase = CHILD_SAFETY_ROUTINE_PHASE_END;
                        }
                        break;



                    case CHILD_SAFETY_ROUTINE_PHASE_END:
                        // go the done
                        Child_Safety_Status.Routine_Status = CHILD_SAFETY_ROUTINE_STATUS_DONE;
                        break;



                    case CHILD_SAFETY_ROUTINE_PHASE_IDLE:
                        // in idle, just load the timeout timer
                        SystemTimers__SetMs(Child_Safety_Timeout_Handler, (CS_Params.timeout*1000));
                        break;



                    default:
                        // left blank intentionally.
                        break;
                }
                retval = ACTIVE;
            }
            else
            {
                Child_Safety_Status.Routine_Status = CHILD_SAFETY_ROUTINE_STATUS_DONE;
            }
            break;



        case CHILD_SAFETY_ROUTINE_STATUS_DONE:
            // Publish one event that child safety was performed and publish its results
            sendStatusData((uint8)API013_CLIENT_FUNCTION_CHILD_SAFETY);

            // go to idle
            Child_Safety_Status.Routine_Status = CHILD_SAFETY_ROUTINE_STATUS_IDLE;

            // release motion for other tasks
            Motion__Release(MOTION_REQUESTOR_CHILD_SAFETY);

            // Inactivate remote function module
            retval = INACTIVE;
            break;



        default:
            // left blank intentionally.
            break;
    }

    return retval;
}


//---------------------------------------------------------------------------------------------------------------------
/**
 * Remote Function API Function: Execute a Remote Function command.
 * @param command = The command that the Remote Function should execute.
 *                  A command value of 0 indicates that the Remote Function should disable itself.
 *                  A command value of 1 indicates that the Remote Function should enable itself.
 * @param params = A pointer to the parameter bytes that are being passed along with the command.
 *                  The params memory is only valid during the function call.
 * @param size = The number of parameter bytes that are being passed along with the command.
 * @retval ANALYSIS_COMMAND_RESPONSE_INACTIVE = The analysis is inactive. Do not call the remote function.
 * @retval ANALYSIS_COMMAND_RESPONSE_ACTIVE = The analysis is active. Call the remote function periodically.
 * @retval ANALYSIS_COMMAND_RESPONSE_INVALID = The command is not supported.
 */
REMOTE_FUNCTION_CONTROL_RESPONSE_TYPE  RemoteFunctionChildSafety__DoCommand(uint8 command, uint8* params, uint8 size)
{
	REMOTE_FUNCTION_CONTROL_RESPONSE_TYPE response;
	MOTION_PROFILE_TYPE profile;

	response = REMOTE_FUNCTION_CONTROL_RESPONSE_INVALID;

    switch(command)
    {

        case REMOTE_FUNCTION_COMMAND_DISABLE:

            Child_Safety_Status.Routine_Status = CHILD_SAFETY_ROUTINE_STATUS_IDLE;

            // Reset error if they were not related to settings file parameters
            if(Child_Safety_Status.Routine_Error != CHILD_SAFETY_ROUTINE_ERROR_SETTING_FILE)
            {
                Child_Safety_Status.Routine_Error = CHILD_SAFETY_ROUTINE_ERROR_NONE;
            }

            response = REMOTE_FUNCTION_CONTROL_RESPONSE_INACTIVE;

            //Mci__Stop(MOTOR0, 50);
            profile.Stop.Drum_Deceleration = 50;
            Motion__Activity(MOTION_ACTIVITY_STOP, profile, MOTION_REQUESTOR_CHILD_SAFETY);

            // release motion for other tasks
            Motion__Release(MOTION_REQUESTOR_CHILD_SAFETY);
            break;




        case REMOTE_FUNCTION_COMMAND_ENABLE:

            // Accept command if errors were not related to settings file parameters
            if(Child_Safety_Status.Routine_Error != CHILD_SAFETY_ROUTINE_ERROR_SETTING_FILE)
            {
                if( (Child_Safety_Status.Routine_Status == CHILD_SAFETY_ROUTINE_STATUS_IDLE) ||
                    (Child_Safety_Status.Routine_Status == CHILD_SAFETY_ROUTINE_STATUS_DONE) )
                {
                	Mci__ClearError(MOTOR0);
                    SystemTimers__SetMs(Child_Safety_Timeout_Handler, (CS_Params.timeout*1000));

                    Child_Safety_Status.Routine_Phase = CHILD_SAFETY_ROUTINE_PHASE_INIT;
                    Child_Safety_Status.Routine_Status = CHILD_SAFETY_ROUTINE_STATUS_PROCESSING;
                    Child_Safety_Status.Routine_Error = CHILD_SAFETY_ROUTINE_ERROR_NONE;
                }

                response = REMOTE_FUNCTION_CONTROL_RESPONSE_ACTIVE;
            }
            break;



        default:
            //
            break;
    }

    sendStatusData((uint8)API013_CLIENT_FUNCTION_CHILD_SAFETY);

    return response;
}


//---------------------------------------------------------------------------------------------------------------------
/**
 * Remote Function API Function: Get the current status of the Remote Function.
 * @param status = This function sets the referenced pointer to point to the buffer that holds the function status data.
 * @param size = This function sets the referenced byte to the number of bytes in the function status data.
 */
void RemoteFunctionChildSafety__GetStatus(uint8** status, uint8* size)
{
	sendStatusData((uint8)API013_CLIENT_FUNCTION_CHILD_SAFETY);
}


//=====================================================================================================================
//  --- Private Functions ---------------------------------------------------------------------------------------------
//=====================================================================================================================

static void sendStatusData (uint8 server_function_index_local)
{
	API013RemoteFunction__PublishStatus (server_function_index_local, ((uint8*)&Child_Safety_Status), sizeof(Child_Safety_Status));
}







