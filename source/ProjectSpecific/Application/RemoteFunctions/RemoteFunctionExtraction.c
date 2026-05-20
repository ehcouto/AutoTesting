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
 * @copyright   Copyright 2016 Whirlpool Corporation.  All rights reserved - CONFIDENTIAL.
 */

//  --- Include Files -------------------------------------------------------------------------------------------------
#include "C_Extensions.h"
#include "SystemConfig.h"

#include "AccelBaseline.h"
#if (EXTR_FEATURE_ACCEL_ENABLED != 0)
#include "Accelerometer.h"
#endif /* (EXTR_FEATURE_ACCEL_ENABLED != 0) */
#include "API007App.h"
#include "RemoteFunctionExtraction.h"
#include "API013RemoteFunction_prm.h"
#include "RemoteFunctionInterface.h"
#include "API013RemoteFunction.h"
#include "Extraction.h"
#include "ExtractionParams.h"
#include "Utilities.h"
#include <string.h>
#include "JtagTestInterface.h"
#include "MciSensorsWm.h"
#include "Motion.h"

#define CLEAN_WASHER_LOAD_DET_MASK (((uint32)(1UL << EXTR_FAULT_CLEAN_WASHER_P2P_TORQUE_EXCEEDED)) | \
									((uint32)(1UL << EXTR_FAULT_CLEAN_WASHER_ACCEL_EXCEEDED)) | \
									((uint32)(1UL << EXTR_FAULT_CLEAN_WASHER_LOAD_MASS_EXCEEDED)) | \
									((uint32)(1UL << EXTR_FAULT_CLEAN_WASHER_UNBALANCE_EXCEEDED)))

//  --- Private Declarations ------------------------------------------------------------------------------------------
typedef enum INDEX_EXTRACTION_STATUS
{
	INDEX_EXTR_STATUS_ALGO       	   = 0,
	INDEX_EXTR_STATUS_ERR        	   = 1,
	INDEX_EXTR_REPORT_FLAGS  	   	   = 2,
	INDEX_EXTR_LOWSPD_LOADMASS_0	   = 3,
	INDEX_EXTR_LOWSPD_LOADMASS_1	   = 4,
	INDEX_EXTR_LOW_SPD_REB_CNT	   	   = 5,
	INDEX_EXTR_HIGHT_SPD_REB_CNT	   = 6,
	INDEX_EXTR_STATUS_FLAGS	   		   = 7,
	INDEX_EXTR_FAULT_FLAGS_1	   	   = 8,
	INDEX_EXTR_FAULT_FLAGS_0	   	   = 9,
	INDEX_EXTR_HIGHSPD_LOADMASS_0	   = 10,
	INDEX_EXTR_HIGHSPD_LOADMASS_1	   = 11,
	INDEX_EXTR_FAULT_FLAGS_3	   	   = 12,
	INDEX_EXTR_FAULT_FLAGS_2	   	   = 13,
	INDEX_EXTR_HIGHT_SP_STATUS		   = 14,
	EXTRACTION_STATUS_SIZE
} INDEX_EXTRACTION_STATUS_TYPE;

// Bit position of flags inside ExtractionProcess_status buffer[INDEX_EXTR_REPORT_FLAGS]
#define SETTING_FILE_ERROR_BIT			0
#define DATA_RECEIVED_BIT				1
#define MOTION_BUSY_BIT					2
#define ENABLE_BACKGR_FUNC				3

// Bit position of flags inside ExtractionProcess_status buffer[INDEX_EXTR_STATUS_FLAGS]
#define REBALANCE_IN_PROGRESS			0

#define REMOTE_FUNCTION_EXTRACTION_COUNT_DOWN_INCREMENT 	25

#define STOP_EXTRACTION()			{ Extraction__Stop(); Extraction__Handler5ms(); }
#define START_EXTRACTION()			{ Extraction__ResetLoadMass(); Extraction__Handler5ms(); Extraction__Start(); Extraction__Handler5ms(); }
#define NOTIFY_FOAM_DETECTION() { Extraction__NotifyFoamDetection(); Extraction__Handler5ms(); }
#define END_EXTRACTION()		{ Extraction__End(); Extraction__Handler5ms(); }
#define RESUME_EXTRACTION()		{ Extraction__Resume(); Extraction__Handler5ms(); }

// -- Private Enumerated Constant Declarations --
typedef enum REMOTE_FUNCTION_EXTRACTION_COMMAND_ENUM
{
	REMOTE_FUNCTION_EXTRACTION_COMMAND_UPDATE = 2,
	REMOTE_FUNCTION_EXTRACTION_COMMAND_RESET = 3,
	REMOTE_FUNCTION_EXTRACTION_COMMAND_RETURN_TO_DISTR = 4,
	REMOTE_FUNCTION_COMMAND_RESUME = 5,
	REMOTE_FUNCTION_COMMAND_END = 6
} REMOTE_FUNCTION_EXTRACTION_COMMAND_TYPE;

//! Offset for the parameters in REMOTE_FUNCTION_COMMAND_ENABLE command
typedef enum ENABLE_PARAMS_DATA_ENUM
{
    ENABLE_PAR_SPIN_DISPLACEMENT_OFFS	= 0,
	ENABLE_PAR_TIMEOUT_HIGH_BYTE_OFFS,
	ENABLE_PAR_TIMEOUT_LOW_BYTE_OFFS,
	ENABLE_PAR_WASH_TEMPERATURE_OFFS,
	ENABLE_PAR_LOAD_HIGH_BYTE_MASS_OFFS,
	ENABLE_PAR_LOAD_LOW_BYTE_MASS_OFFS,
	ENABLE_PAR_VIBR_MAPPING_CFG_OFFS,
	ENABLE_PAR_HS_VEL_MIN_REQ_OFFS,
	ENABLE_PAR_HS_VEL_MAX_LIM_OFFS,
	ENABLE_PAR_HS_EXTD_PLATEAU_DUR_OFFS,
	ENABLE_PAR_MAX_ALLOWABLE_RAMP_TIME,
	ENABLE_PAR_DRUM_TARG_VEL_ERR,
	ENABLE_PAR_CNT
} ENABLE_PARAMS_DATA_TYPE;

//! Offset for the parameters in REMOTE_FUNCTION_EXTRACTION_COMMAND_UPDATE command
typedef enum UPDATE_PARAMS_DATA_ENUM
{
	UPDATE_PAR_TIMEOUT_HIGH_BYTE_OFFS 	= 0,
	UPDATE_PAR_TIMEOUT_LOW_BYTE_OFFS,
	UPDATE_PAR_WASH_TEMPERATURE_OFFS,
	UPDATE_PAR_LOAD_HIGH_BYTE_MASS_OFFS,
	UPDATE_PAR_LOAD_LOW_BYTE_MASS_OFFS,
	UPDATE_PAR_CNT
} UPDATE_PARAMS_DATA_TYPE;

//! Extraction High Speed parameters for MCU.
typedef struct EXTRACTION_HIGH_SPEED_DATA_STRUCT
{
	uint8 Velocity_Minimum_Required_Div10;
	uint8 Velocity_Maximum_Limit_Div10;
	uint8 Extended_Plateau_Duration_Div10;
	uint8 Param_Max_Allowable_ramp_time;
	uint8 Param_Drum_Targ_Vel_Err;
} EXTRACTION_HIGH_SPEED_DATA_TYPE;


// -- Private Variable Definitions --

static uint16 Remote_Function_Extraction_Countdown;
static uint16 Remote_Function_Extraction_Countdown_Start;
static uint8 Enabled_remote_function;
// static uint8 End_Cmd_Send;
static uint8 ExtractionProcess_status[EXTRACTION_STATUS_SIZE];
static uint8 ExtractionProcess_status_previous[EXTRACTION_STATUS_SIZE];
static uint16 Load_Mass;
static uint8 Water_Temperature;
EXTRACTION_HIGH_SPEED_DATA_TYPE High_Speed_Parameters;


//  --- Private Function Prototypes -----------------------------------------------------------------------------------

static void PrepareStatusData(void);
static void sendStatusData (uint8 server_function_index_local);
static void ParametersParser(uint8 *start_address);

//=====================================================================================================================
//  --- Public Functions ----------------------------------------------------------------------------------------------
//=====================================================================================================================

//---------------------------------------------------------------------------------------------------------------------
/**
 * Initialize all module variables.
 */
void RemoteFunctionExtraction__Initialize (void)
{
	Remote_Function_Extraction_Countdown       = 0;
	Remote_Function_Extraction_Countdown_Start = 0;

    memset(&ExtractionProcess_status, 0, sizeof(ExtractionProcess_status));
    memset(&ExtractionProcess_status_previous, 0, sizeof(ExtractionProcess_status_previous));

    Load_Mass = 15;
    Water_Temperature = 25;

    High_Speed_Parameters.Velocity_Minimum_Required_Div10 = 0;
	High_Speed_Parameters.Velocity_Maximum_Limit_Div10 = 0;
	High_Speed_Parameters.Extended_Plateau_Duration_Div10 = 6;

    High_Speed_Parameters.Param_Max_Allowable_ramp_time = 5;
	High_Speed_Parameters.Param_Drum_Targ_Vel_Err = 10;

	Enabled_remote_function = FALSE;

	#if TEST_ACCELEROMETER
	AccelBaseline__MeasureBaseline();
	#endif
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * Remote Function API Function: Background function that is called periodically while the Remote Function is active.
 * @return ACTIVE if the Remote Function should be called periodically; otherwise INACTIVE.
 *                  If the Remote Function is active, and if this function returns INACTIVE,
 *                  then the REMOTE_FUNCTION_GET_DATA_ELEMENTS API function will be called one more time.
 */
ACTIVE_TYPE RemoteFunctionExtraction__Background(void)
{
    ACTIVE_TYPE retval;

    if ((Remote_Function_Extraction_Countdown > REMOTE_FUNCTION_EXTRACTION_COUNT_DOWN_INCREMENT) && (Enabled_remote_function == TRUE))
    {
    	Remote_Function_Extraction_Countdown -= REMOTE_FUNCTION_EXTRACTION_COUNT_DOWN_INCREMENT;
    	ExtractionProcess_status[INDEX_EXTR_STATUS_ERR] = EXTRACTION_NO_ERR;

        retval = ACTIVE;
    }
    else
    {
    	Remote_Function_Extraction_Countdown = 0;

		if (Enabled_remote_function == TRUE)
		{   // TimeOut Expired
			ExtractionProcess_status[INDEX_EXTR_STATUS_ERR] = EXTRACTION_ERR_STATUS_BACKFUNC;
		}

		// Trigger Extraction stop sequence
        retval = INACTIVE;
    }


    if (retval == ACTIVE)
    {
		#if TEST_ACCELEROMETER
    	//
		#endif
    }

	sendStatusData((uint8)API013_CLIENT_FUNCTION_EXTRACTION);
	//

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
REMOTE_FUNCTION_CONTROL_RESPONSE_TYPE  RemoteFunctionExtraction__DoCommand(uint8 command, uint8* params, uint8 size)
{
	REMOTE_FUNCTION_CONTROL_RESPONSE_TYPE response;

	if(SettingFile__IsValid() == FALSE)
	{
	    // When Setting file is not valid force an invalid command to prevent any action
	    command = REMOTE_FUNCTION_COMMAND_DISABLE;
	}

    switch (command)
    {
        case REMOTE_FUNCTION_COMMAND_DISABLE:
			{
				Remote_Function_Extraction_Countdown = 0;
				// Stop the Spin Activities with the displacement.
				Enabled_remote_function = FALSE;

				// Trigger Extraction stop sequence
				STOP_EXTRACTION();

#if (EXTR_FEATURE_ACCEL_ENABLED != 0)
				Accelerometer__Disable();
				Accelerometer__FlushData();
#endif /* (EXTR_FEATURE_ACCEL_ENABLED != 0) */

				ExtractionProcess_status[INDEX_EXTR_STATUS_ERR] = EXTRACTION_STATUS_DISABLE;
				ExtractionParams__ResetSettingFileError();
				response = REMOTE_FUNCTION_CONTROL_RESPONSE_INACTIVE;
				// End_Cmd_Send = FALSE;

				// Force status trasmission
				memset(&ExtractionProcess_status_previous, 0xFF, sizeof(ExtractionProcess_status_previous));
			}
            break;

        case REMOTE_FUNCTION_COMMAND_ENABLE:
        	response = REMOTE_FUNCTION_CONTROL_RESPONSE_INACTIVE;

			if ((params != NULL) && (size >= ENABLE_PAR_CNT))
			{
				if (ExtractionParams__Initialize(params[ENABLE_PAR_SPIN_DISPLACEMENT_OFFS], params[ENABLE_PAR_VIBR_MAPPING_CFG_OFFS]) == PASS)
				{
					ParametersParser(&params[ENABLE_PAR_TIMEOUT_HIGH_BYTE_OFFS]);

					High_Speed_Parameters.Velocity_Minimum_Required_Div10 = params[ENABLE_PAR_HS_VEL_MIN_REQ_OFFS];
					High_Speed_Parameters.Velocity_Maximum_Limit_Div10 = params[ENABLE_PAR_HS_VEL_MAX_LIM_OFFS];
					High_Speed_Parameters.Extended_Plateau_Duration_Div10 = params[ENABLE_PAR_HS_EXTD_PLATEAU_DUR_OFFS];

					High_Speed_Parameters.Param_Max_Allowable_ramp_time = params[ENABLE_PAR_MAX_ALLOWABLE_RAMP_TIME];
					High_Speed_Parameters.Param_Drum_Targ_Vel_Err = params[ENABLE_PAR_DRUM_TARG_VEL_ERR];

					Remote_Function_Extraction_Countdown_Start = Remote_Function_Extraction_Countdown;

					// Latch the Spin Activities with the displacement = params.
					Enabled_remote_function = TRUE;

					// Trigger Extraction start sequence
					START_EXTRACTION();

					ExtractionProcess_status[INDEX_EXTR_STATUS_ERR] = EXTRACTION_NO_ERR;
					response = REMOTE_FUNCTION_CONTROL_RESPONSE_ACTIVE;
				}
			}
			else
			{
				ExtractionProcess_status[INDEX_EXTR_STATUS_ERR] = EXTRACTION_ERR_COMMAND_PARAM;
			}

        	// Force status trasmission
        	memset(&ExtractionProcess_status_previous, 0xFF, sizeof(ExtractionProcess_status_previous));
        	break;

        case REMOTE_FUNCTION_EXTRACTION_COMMAND_UPDATE:
			{
				if ((params != NULL) && (size >= UPDATE_PAR_CNT))
				{
					ParametersParser(&params[UPDATE_PAR_TIMEOUT_HIGH_BYTE_OFFS]);
				}
				else
				{
					ExtractionProcess_status[INDEX_EXTR_STATUS_ERR] = EXTRACTION_ERR_COMMAND_PARAM;
				}

				response = REMOTE_FUNCTION_CONTROL_RESPONSE_ACTIVE;
			}
            break;

        case REMOTE_FUNCTION_EXTRACTION_COMMAND_RESET:
			{
				Remote_Function_Extraction_Countdown = Remote_Function_Extraction_Countdown_Start;

				response = REMOTE_FUNCTION_CONTROL_RESPONSE_ACTIVE;
			}
            break;

        case REMOTE_FUNCTION_EXTRACTION_COMMAND_RETURN_TO_DISTR:
			{
				Remote_Function_Extraction_Countdown = Remote_Function_Extraction_Countdown_Start;

				// Trigger extraction return to distribution velocity
				NOTIFY_FOAM_DETECTION();

			    // Force status trasmission
				memset(&ExtractionProcess_status_previous, 0xFF, sizeof(ExtractionProcess_status_previous));

				response = REMOTE_FUNCTION_CONTROL_RESPONSE_ACTIVE;
			}
            break;

        case REMOTE_FUNCTION_COMMAND_RESUME:
    		if ((params != NULL) && (size >= ENABLE_PAR_CNT))
    		{
				Remote_Function_Extraction_Countdown = Utilities__Merg2BytesTo16bits(params[ENABLE_PAR_TIMEOUT_HIGH_BYTE_OFFS], params[ENABLE_PAR_TIMEOUT_LOW_BYTE_OFFS]);
				Remote_Function_Extraction_Countdown_Start = Remote_Function_Extraction_Countdown;
				// Latch the Spin Activities with the displacement = params.
				Enabled_remote_function = TRUE;

				// Trigger Extraction start sequence
				RESUME_EXTRACTION();

				ExtractionProcess_status[INDEX_EXTR_STATUS_ERR] = EXTRACTION_NO_ERR;

				response = REMOTE_FUNCTION_CONTROL_RESPONSE_ACTIVE;
    		}
           	break;

        case REMOTE_FUNCTION_COMMAND_END:
			{
				// End extraction activity
				END_EXTRACTION();

				Remote_Function_Extraction_Countdown = 0;
				Enabled_remote_function = FALSE;
				ExtractionProcess_status[INDEX_EXTR_STATUS_ERR] = EXTRACTION_STATUS_DISABLE;
				ExtractionParams__ResetSettingFileError();
				response = REMOTE_FUNCTION_CONTROL_RESPONSE_INACTIVE;

				// Force status trasmission
				memset(&ExtractionProcess_status_previous, 0xFF, sizeof(ExtractionProcess_status_previous));
			}
           	break;


        default:
			{
				ExtractionProcess_status[INDEX_EXTR_STATUS_ERR] = EXTRACTION_STATUS_DISABLE;
				response = REMOTE_FUNCTION_CONTROL_RESPONSE_INVALID;
			}
            break;
    }

    sendStatusData((uint8)API013_CLIENT_FUNCTION_EXTRACTION);

    return response;
}


//---------------------------------------------------------------------------------------------------------------------
/**
 * Remote Function API Function: Get the current status of the Remote Function.
 * @param status = This function sets the referenced pointer to point to the buffer that holds the function status data.
 * @param size = This function sets the referenced byte to the number of bytes in the function status data.
 */
void RemoteFunctionExtraction__GetStatus(uint8** status, uint8* size)
{
	PrepareStatusData();

	memcpy (ExtractionProcess_status_previous, ExtractionProcess_status, sizeof(ExtractionProcess_status));

	*status = &ExtractionProcess_status[EXTRACTION_NO_ERR];
    *size = EXTRACTION_STATUS_SIZE;
}


//---------------------------------------------------------------------------------------------------------------------
/**
 * Return the value of the Water Temperature
 */
uint8 RemoteFunctionExtraction__GetTemperature(void)
{
	return Water_Temperature;
}


//---------------------------------------------------------------------------------------------------------------------
/**
 * Return the value of the Load Mass
 */
float32 RemoteFunctionExtraction__GetLoadMass(void)
{
	return (Load_Mass / 10.0f);
}


//---------------------------------------------------------------------------------------------------------------------
/**
 * Return the value of the High Speed Velocity Minimum Required parameter
 */
uint16 RemoteFunctionExtraction__HSVelocityMinReq(void)
{
	return (High_Speed_Parameters.Velocity_Minimum_Required_Div10 * 10);
}


//---------------------------------------------------------------------------------------------------------------------
/**
 * Return the value of the High Speed Velocity Maximum Limit parameter
 */
uint16 RemoteFunctionExtraction__HSVelocityMaxLim(void)
{
	return (High_Speed_Parameters.Velocity_Maximum_Limit_Div10 * 10);
}


//---------------------------------------------------------------------------------------------------------------------
/**
 * Return the value of the High Speed Velocity Maximum Limit parameter
 */
uint16 RemoteFunctionExtraction__HSExtdPlateauDur(void)
{
	return (High_Speed_Parameters.Extended_Plateau_Duration_Div10 * 10);
}


uint8 RemoteFunctionExtraction__ParMaxAllowRampTime (void)
{
	return (High_Speed_Parameters.Param_Max_Allowable_ramp_time);
}


uint8 RemoteFunctionExtraction__ParDrumTargVelErr (void)
{
	return (High_Speed_Parameters.Param_Drum_Targ_Vel_Err );
}

//=====================================================================================================================
//  --- Private Functions ---------------------------------------------------------------------------------------------
//=====================================================================================================================

static void PrepareStatusData (void)
{
	uint16 Extr_Load_Mass;
	uint32 Extr_Get_Fault_Bitmap;
	boolean_T Rebalance_In_Progress;

	// Management of Status MBCD and ERR datas.
	ExtractionProcess_status[INDEX_EXTR_STATUS_ALGO] = Extraction__GetState();

	// Test if a setting file error has been detected
	if (ExtractionParams__GetSettingFileError() == TRUE)
    {
    	BIT_SET(ExtractionProcess_status[INDEX_EXTR_REPORT_FLAGS], SETTING_FILE_ERROR_BIT);
    }
    else
    {
    	BIT_CLR(ExtractionProcess_status[INDEX_EXTR_REPORT_FLAGS], SETTING_FILE_ERROR_BIT);
    }

	// Test if Enable BackGround is Active
	if (Enabled_remote_function == TRUE)
    {
    	BIT_SET(ExtractionProcess_status[INDEX_EXTR_REPORT_FLAGS], ENABLE_BACKGR_FUNC);
    }
    else
    {
    	BIT_CLR(ExtractionProcess_status[INDEX_EXTR_REPORT_FLAGS], ENABLE_BACKGR_FUNC);
    }

	// Test if a data from API007 have not received
	if (API007App__AreAllDataReceived() == TRUE)
    {
    	BIT_SET(ExtractionProcess_status[INDEX_EXTR_REPORT_FLAGS], DATA_RECEIVED_BIT);
    }
    else
    {
    	BIT_CLR(ExtractionProcess_status[INDEX_EXTR_REPORT_FLAGS], DATA_RECEIVED_BIT);
    }

	// Test if a Motion module is idle
	if(Motion__GetStatus() == MOTION_STATUS_IDLE)
    {
    	BIT_CLR(ExtractionProcess_status[INDEX_EXTR_REPORT_FLAGS], MOTION_BUSY_BIT);
    }
    else
    {
    	BIT_SET(ExtractionProcess_status[INDEX_EXTR_REPORT_FLAGS], MOTION_BUSY_BIT);
    }
	////

	Extr_Load_Mass = (uint16)(Extraction__GetFloat(EXTR_FLOAT_FB_ENUM_LOAD_MASS_LOW_SPEED) * 10.0f + 0.5f);
	ExtractionProcess_status[INDEX_EXTR_LOWSPD_LOADMASS_0] = LOBYTE(Extr_Load_Mass);
	ExtractionProcess_status[INDEX_EXTR_LOWSPD_LOADMASS_1] = HIBYTE(Extr_Load_Mass);

	Extr_Load_Mass = (uint16)(Extraction__GetFloat(EXTR_FLOAT_FB_ENUM_LOAD_MASS_HIGH_SPEED) * 10.0f + 0.5f);
	ExtractionProcess_status[INDEX_EXTR_HIGHSPD_LOADMASS_0] = LOBYTE(Extr_Load_Mass);
	ExtractionProcess_status[INDEX_EXTR_HIGHSPD_LOADMASS_1] = HIBYTE(Extr_Load_Mass);
	////

	ExtractionProcess_status[INDEX_EXTR_LOW_SPD_REB_CNT]   = (uint8)Extraction__GetInteger(EXTR_INT32_FB_ENUM_REBAL_CNT_LOW_SPEED);
	ExtractionProcess_status[INDEX_EXTR_HIGHT_SPD_REB_CNT] = (uint8)Extraction__GetInteger(EXTR_INT32_FB_ENUM_REBAL_CNT_HIGH_SPEED);
	ExtractionProcess_status[INDEX_EXTR_HIGHT_SP_STATUS]   = (uint8)Extraction__GetInteger(EXTR_INT32_FB_ENUM_HIGH_SPEED_STATUS);


	Extraction__IsRebInProgress(&Rebalance_In_Progress);

	if (Rebalance_In_Progress == TRUE)
	{
		BIT_SET(ExtractionProcess_status[INDEX_EXTR_STATUS_FLAGS], REBALANCE_IN_PROGRESS);
	}
	else
	{
		BIT_CLR(ExtractionProcess_status[INDEX_EXTR_STATUS_FLAGS], REBALANCE_IN_PROGRESS);
	}

	Extr_Get_Fault_Bitmap = Extraction__GetFault() & CLEAN_WASHER_LOAD_DET_MASK; //! Temporary solution to avoid that ACU enters the fault condition improperly.

	ExtractionProcess_status[INDEX_EXTR_FAULT_FLAGS_1] = HIBYTE(LOWORD(Extr_Get_Fault_Bitmap));

	ExtractionProcess_status[INDEX_EXTR_FAULT_FLAGS_0] = LOBYTE(LOWORD(Extr_Get_Fault_Bitmap));

	ExtractionProcess_status[INDEX_EXTR_FAULT_FLAGS_3] = HIBYTE(HIWORD(Extr_Get_Fault_Bitmap));

	ExtractionProcess_status[INDEX_EXTR_FAULT_FLAGS_2] = LOBYTE(HIWORD(Extr_Get_Fault_Bitmap));

	if (Extr_Get_Fault_Bitmap != 0x0000)
	{
		Extraction__ClearFault();
	}
	//
}


static void sendStatusData (uint8 server_function_index_local)
{
	PrepareStatusData();

	if (memcmp (ExtractionProcess_status, ExtractionProcess_status_previous, EXTRACTION_STATUS_SIZE))
	{
		API013RemoteFunction__PublishStatus (server_function_index_local, ExtractionProcess_status, EXTRACTION_STATUS_SIZE);
		memcpy (ExtractionProcess_status_previous, ExtractionProcess_status, sizeof(ExtractionProcess_status));
	}
}

/**
 * @brief  Process and store data received through API013.
 *
 * @param start_address = pointer to the start address of the data.
 */
static void ParametersParser(uint8 *start_address)
{
	Remote_Function_Extraction_Countdown = Utilities__Merg2BytesTo16bits(start_address[0], start_address[1]);

    Water_Temperature = start_address[2];
    Load_Mass = Utilities__Merg2BytesTo16bits(start_address[3], start_address[4]);;
}
