/**
 *  @file       
 *
 *  @brief      API011 Implementation
 *
 *  @details    This API has 2 main functions:
 *                  - Implements the main communication between ACU and HMI (Regulations, Status)
 *                  - Implements get/set for the appliance Mode
 *              There is also support to get the number of cycles, the Status variables configuration
 *              and the position of a function in the function configuration.
 *
 @startuml{API011_Sequence_Diagram.png}
 hide footbox
 autonumber
 title API011 Sequence Diagram
 box "HMI"
 participant Application as app
 participant API011_HMI as hmi
 end box

 box "ACU"
 participant API011_ACU as acu
 end box

 alt Normal operation, no user activity or application changing regulations.
 	 loop 10s
 	 hmi -> acu : request regulations (API011 opcode: 4)
 	 hmi -> hmi : <b>API011_Num_Of_Consecutive_Regulations_Requests = 1</b>
 	 hmi -> hmi : <b>API011_Num_Of_Consecutive_Status_Requests = 0</b>
 	 acu -> hmi : publish regulations (API011 opcode: 4, feedback)
 	 ... 5 secounds later ...
 	 hmi -> acu : request status (API011 opcode: 3)
 	 hmi -> hmi : <b>API011_Num_Of_Consecutive_Status_Requests = 1</b>
 	 hmi -> hmi : <b>API011_Num_Of_Consecutive_Regulations_Requests = 0</b>
 	 acu -> hmi : publish status (API011 opcode: 3, feedback)
 	 end


 else ACU detect state change

 	 hmi -> acu : request regulations (API011 opcode: 4)
 	 hmi -> hmi : <b>API011_Num_Of_Consecutive_Regulations_Requests++</b>
 	 hmi -> hmi : <b>API011_Num_Of_Consecutive_Status_Requests = 0</b>
 	 acu -> hmi : publish regulations (API011 opcode: 4, feedback)
 	 acu -> hmi : status change detected on ACU, publish status (API011 opcode: 3, feedback)
 	 hmi -> hmi : <b>API011_Num_Of_Consecutive_Regulations_Requests = 0 </b>
 	 ... 5 secounds later ...
 	 hmi -> acu : request regulations (API011 opcode: 4)
 	 hmi -> hmi : <b>API011_Num_Of_Consecutive_Regulations_Requests++</b>
 	 hmi -> hmi : <b>API011_Num_Of_Consecutive_Status_Requests = 0</b>
 	 acu -> hmi : publish regulations (API011 opcode: 4, feedback)
 	 acu -> hmi : status change detected on ACU, publish status (API011 opcode: 3, feedback)
 	 hmi -> hmi : <b>API011_Num_Of_Consecutive_Regulations_Requests = 0 </b>
 	 loop Standard loop, see "Normal operation, no user activity or application changing regulations."
 	 end


 else HMI sets regulations
 	 hmi -> acu : set regulations (API011 opcode: 5)
 	 hmi -> hmi : API011_Num_Of_Consecutive_Regulations_Sets++
 	 hmi -> hmi : 100ms timeout
 	 hmi -> acu : request regulations (API011 opcode: 4)
 	 hmi -> hmi : <b>API011_Num_Of_Consecutive_Regulations_Sets = 0</b>
 	 hmi -> hmi : <b>API011_Num_Of_Consecutive_Regulations_Requests++</b>
 	 acu -> hmi : publish regulations (API011 opcode: 4, feedback)
 	 loop Standard loop, see "Normal operation, no user activity or application changing regulations."
 	 end

 else <b> ABNORMAL SITUATION! SHALL BE AVOIDED </b> \n HMI sets regulations continuously. \n HMI sets regulations with period greater than 100ms and less than 5s.
 	 loop 5times
  	 hmi -> acu : set regulations (API011 opcode: 5)
 	 hmi -> hmi : <b>API011_Num_Of_Consecutive_Regulations_Sets++</b>
 	 ... 100 milliseconds later ...
 	 hmi -> acu : request regulations (API011 opcode: 4)
 	 hmi -> hmi : <b>API011_Num_Of_Consecutive_Regulations_Sets = 0</b>
 	 hmi -> hmi : <b>API011_Num_Of_Consecutive_Regulations_Requests++ (1,2,3,4,5)</b>
 	 acu -> hmi : publish regulations (API011 opcode: 4, feedback)
 	 end
 	 hmi -> hmi : <b>API011_Num_Of_Consecutive_Regulations_Requests >= </b> \n  <b> MAX_NUM_OF_CONSECUTIVE_REQUESTS </b>
 	 hmi -> acu : <b>request status (API011 opcode: 3) </b>
 	 acu -> hmi : <b>publish status (API011 opcode: 3, feedback) </b>
 	 hmi -> hmi : <b>API011_Num_Of_Consecutive_Regulations_Requests = 0</b>
 	 ... 100 milliseconds later ...
 	 hmi -> acu : request regulations (API011 opcode: 4)
 	 acu -> hmi : publish regulations (API011 opcode: 4, feedback)

 else <b> ABNORMAL SITUATION! SHALL BE AVOIDED </b> \n HMI sets regulations continuously. \n HMI sets regulations with period less than 100ms.
 	 loop 5 times
 	 hmi -> acu : set regulations (API011 opcode: 5)
 	 hmi -> hmi : <b>API011_Num_Of_Consecutive_Regulations_Sets++ (1)</b>
 	 hmi -> acu : set regulations (API011 opcode: 5)
 	 hmi -> hmi : <b>API011_Num_Of_Consecutive_Regulations_Sets++ (2)</b>
 	 hmi -> acu : set regulations (API011 opcode: 5)
 	 hmi -> hmi : <b>API011_Num_Of_Consecutive_Regulations_Sets++ (3)</b>
 	 hmi -> acu : set regulations (API011 opcode: 5)
 	 hmi -> hmi : <b>API011_Num_Of_Consecutive_Regulations_Sets++ (4)</b>
 	 hmi -> acu : set regulations (API011 opcode: 5)
 	 hmi -> hmi : <b>API011_Num_Of_Consecutive_Regulations_Sets++ (5)</b>
 	 hmi -> hmi : <b>API011_Num_Of_Consecutive_Regulations_Sets >= </b> \n  <b> MAX_NUM_OF_CONSECUTIVE_REQUESTS </b>
 	 hmi -> acu : request regulations (API011 opcode: 4)
 	 hmi -> hmi : <b>API011_Num_Of_Consecutive_Regulations_Sets = 0</b>
 	 hmi -> hmi : <b>API011_Num_Of_Consecutive_Regulations_Requests++ (1,2,3,4,5)</b>
 	 acu -> hmi : publish regulations (API011 opcode: 4, feedback)
 	 end
 	 hmi -> hmi : <b>API011_Num_Of_Consecutive_Regulations_Requests >= </b> \n  <b> MAX_NUM_OF_CONSECUTIVE_REQUESTS </b>
 	 hmi -> acu : <b>request status (API011 opcode: 3) </b>
 	 acu -> hmi : <b>publish status (API011 opcode: 3, feedback) </b>
 	 hmi -> hmi : <b>API011_Num_Of_Consecutive_Regulations_Requests = 0</b>
 	 ... 100 milliseconds later ...
 	 hmi -> acu : request regulations (API011 opcode: 4)
 	 acu -> hmi : publish regulations (API011 opcode: 4, feedback)
 end
 @enduml

 *  @copyright  Copyright 2015 Whirlpool Corporation. All rights reserved - CONFIDENTIAL
 */
//---------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------

//-------------------------------------- Include Files ----------------------------------------------------------------
#include "SystemConfig.h"
#include "Reveal.h"
#include "API011AppCtrl.h" 
#include "API011AppCtrl_prv.h"

#if (SYSTEM_ACU_HANDLING == ENABLED)
    #include "ApplianceMgr.h"
    #include "Selector.h"
#endif

#if (NVDATA_STORE_APPLIANCE_AND_STATISTICS_FEATURE == ENABLED)
    #include "NVData.h"
#endif

#if (SYSTEM_ACU_HANDLING == ENABLED) || (SYSTEM_HMI_HANDLING == ENABLED)
    #include "Regulations.h"
    #include "Status.h"
#endif

#ifndef API011APPCTRL_LOG_FEATURE
#define API011APPCTRL_LOG_FEATURE ENABLED
#endif

#if ((SYSTEM_HMI_HANDLING == ENABLED) && (API011APPCTRL_LOG_FEATURE == ENABLED))
	#include "Log.h"
#endif

#include "Micro.h"
#include "Mode.h"
#include "SystemTimers.h"
#include "SettingFile.h"

//-------------------------------------- PUBLIC (Variables) -----------------------------------------------------------

#ifndef API0011APPCTRL_USE_PRIORITY_QUEUE
    #define API0011APPCTRL_USE_PRIORITY_QUEUE DISABLED
#endif

#ifndef API011APPCTRL__RECEIVED_ERROR
    #define API011APPCTRL__RECEIVED_ERROR(node,error)
#endif

#if (SYSTEM_HMI_HANDLING == ENABLED)
    #ifndef API011APPCTRL_REGULATION_RX_NOTIFICATION
        #error "API011APPCTRL_REGULATION_RX_NOTIFICATION must be defined in API011AppCtrl_prv.h. This macro is used to notify when Regulations are received from the ACU"
    #endif
#endif

#if (API0011APPCTRL_USE_PRIORITY_QUEUE == ENABLED)
        #define API011REVEAL__NEWFEEDBACK(api,opcode,node,size)   Reveal__PriorityNewFeedback(api,opcode,node,size)
        #define API011REVEAL__NEWCOMMAND(api,opcode,node,size)    Reveal__PriorityNewCommand(api,opcode,node,size)
#else
        #define API011REVEAL__NEWFEEDBACK(api,opcode,node,size)   Reveal__NewFeedback(api,opcode,node,size)
        #define API011REVEAL__NEWCOMMAND(api,opcode,node,size)    Reveal__NewCommand(api,opcode,node,size)
#endif
//-------------------------------------- PRIVATE (Variables, Constants & Defines) -------------------------------------
//! API011APPCTRL opcodes
typedef enum API011APPCTRL_OPCODE_ENUM
{
    API011APPCTRL_OPCODE_REQ_PUB_STATUS                     = 3,
    API011APPCTRL_OPCODE_REQ_PUB_REGULATIONS                = 4,
    API011APPCTRL_OPCODE_SET_REGULATIONS                    = 5,
    API011APPCTRL_OPCODE_REQ_PUB_MODE                       = 6,
    API011APPCTRL_OPCODE_SET_MODE                           = 7,
    API011APPCTRL_OPCODE_MODE_HEARTBEAT                     = 8,
    API011APPCTRL_OPCODE_REG_POSITION_BY_ID                 = 9,
    API011APPCTRL_OPCODE_REG_NUM_CYCLES                     = 10,
    API011APPCTRL_OPCODE_REG_PUB_STATUS_STRUCTURE           = 11,
    API011APPCTRL_OPCODE_REQ_PUB_FUNCTION_IDS               = 12,
    API011APPCTRL_OPCODE_REQ_PUB_CYCLE_NAMES                = 13,
    API011APPCTRL_OPCODE_RESTORE_FACTORY_REGULATION_INDEX   = 14,

    API011APPCTRL_OPCODE_ERROR = 31
} API011APPCTRL_OPCODE_TYPE;

//! API011APPCTRL errors
typedef enum API011APPCTRL_ERROR_ENUM
{
    API011APPCTRL_ERROR_NONE                    = 0,
    API011APPCTRL_ERROR_STATUS_NOT_LOADED       = 1,
    API011APPCTRL_ERROR_REGULATION_NOT_LOADED   = 2
}API011APPCTRL_ERROR_TYPE;

#if (SYSTEM_HMI_HANDLING == ENABLED)
    //! List of possible content for the main HMI to request from the main ACU
    typedef enum API011APPCTRL_MESSAGE_REQUEST_ENUM
    {
        API011APPCTRL_MESSAGE_REQUEST_REGULATIONS = 0,
        API011APPCTRL_MESSAGE_REQUEST_STATUS      = 1
    }API011APPCTRL_MESSAGE_REQUEST_TYPE;

    //! Period of standard API011AppCtrl requests.
	#define API011APPCTRL_STANDARD_REQUEST_PERIOD	((uint32)5000)
    //! Timeout to next request after set regulations event or abnormal situation detection (stuck sending same requests continuously).
	#define API011APPCTRL_TIMEOUT_TO_NEXT_REQUEST 	((uint32)100)
    //! Initial timeout for module timer
	#define API011APPCTRL_INITIAL_TIMEOUT 			((uint32)10)

    //! Timer used to handle API011AppCtrl sequence flow.
    static uint16 API011_Comm_Handle;
    //! Flag that indicates if polling is on or off
    static ON_OFF_TYPE API011_Hmi_Poll_Reg_Status;
    //! Additional timeout before polling is started
    static uint8 API011_Poll_Timeout;
    //! This is a flag to make the API alternate the request for Regulations and Status on the handler. TRUE requests Regulations.
    static API011APPCTRL_MESSAGE_REQUEST_TYPE API011AppCtrl_Request_Message_Content;

#ifndef MAX_NUM_OF_CONSECUTIVE_REQUESTS
    //! Defines maximum number of consecutive request of same type.
	#define MAX_NUM_OF_CONSECUTIVE_REQUESTS			(5)
#elif(MAX_NUM_OF_CONSECUTIVE_REQUESTS < 2)
	#error "MAX_NUM_OF_CONSECUTIVE_REQUESTS shall be greater than 1. Recommended value is 5."
#endif

    //! Counter used to count number of consecutive regulations requests.
    static uint8 API011_Num_Of_Consecutive_Regulations_Requests;
    //! Counter used to count number of consecutive status requests.
    static uint8 API011_Num_Of_Consecutive_Status_Requests;
    //! Counter used to count number of consecutive regulations sets.
    static uint8 API011_Num_Of_Consecutive_Regulations_Sets;

    #if(API011APPCTRL_LOG_FEATURE == ENABLED)
    //! IDs for Log messages
    typedef enum
    {
        TO_MANY_CONSECUTIVE_REGULATIONS_SETS = 0, 		//data: 0
		TO_MANY_CONSECUTIVE_REGULATIONS_REQUESTS,		//data: 0
		TO_MANY_CONSECUTIVE_STATUS_REQUESTS,			//data: 0
		MODULE_TIMER_NOT_CREATED						//data: 0
    } MODULE_API011APPCTRL_LOG_MESSAGE_ID_TYPE;
    //! ID for this module on the Log module.
    #define MODULE_NAME     MODULE_API011APPCTRL
	#endif //(API011APPCTRL_LOG_FEATURE == ENABLED)
#endif

#if (SYSTEM_ACU_HANDLING == ENABLED)
//! Keeps track of the last value reported from ApplianceMgr module on whether it is stable.
//! Used to detect transition from un-stable to stable so status can be published
static BOOL_TYPE API011_Status_Ready;
#endif

//-------------------------------------- PRIVATE (Function Prototypes) ------------------------------------------------
#if (SYSTEM_ACU_HANDLING == ENABLED)
	static void API011AppCtrlPublishStatus(void);
#endif

//=====================================================================================================================
//-------------------------------------- Public Functions -------------------------------------------------------------
//=====================================================================================================================

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief      It Initializes the module API011AppCtrl and its variables
 *
 */
void API011AppCtrl__Initialize(void)
{
    API011AppCtrl__PubMode();

    #if (SYSTEM_HMI_HANDLING == ENABLED)
        API011_Comm_Handle = SystemTimers__GetHandle(SYSTEMTIMERS_TYPE_DOWN_COUNTER);
        if (API011_Comm_Handle != SYSTEMTIMERS_FAIL)
        {
            SystemTimers__SetMs(API011_Comm_Handle, API011APPCTRL_INITIAL_TIMEOUT);
        }
		#if(API011APPCTRL_LOG_FEATURE == ENABLED)
        else
        {
        	LOG_ADD_EXCEPTION(MODULE_TIMER_NOT_CREATED, 0);
        }
		#endif

        if (Micro__GetResetMode() == MICRO_RESET_MODE_COLD)
        {
            API011_Hmi_Poll_Reg_Status = ON;
        }
        else
        {
            API011_Hmi_Poll_Reg_Status = OFF;
        }

        API011_Poll_Timeout = 200;

        API011AppCtrl_Request_Message_Content = API011APPCTRL_MESSAGE_REQUEST_REGULATIONS;
        API011_Num_Of_Consecutive_Regulations_Requests = 0;
        API011_Num_Of_Consecutive_Status_Requests = 0;
        API011_Num_Of_Consecutive_Regulations_Sets = 0;
    #endif
    
	#if (SYSTEM_ACU_HANDLING == ENABLED)
        API011_Status_Ready = FALSE;
    #endif
}

//---------------------------------------------------------------------------------------------------------------------
/**
 *
 */
void API011AppCtrl__Handler(void)
{
    #if (SYSTEM_HMI_HANDLING == ENABLED)
        if (API011_Poll_Timeout > 0)
        {
            API011_Poll_Timeout--;
        }
        else
        {
            API011_Hmi_Poll_Reg_Status = ON;
        }

        if ((Mode__GetRemoteMode() == MODE_NORMAL) &&
            (Mode__GetMode() == MODE_NORMAL) &&
            (Mode__GetSubMode() != SUBMODE_NORMAL_EXT_CONTROL_ENGAGED) &&
            API011_Hmi_Poll_Reg_Status == ON)
        {
            if (API011_Comm_Handle != SYSTEMTIMERS_FAIL)
            {
                if (SystemTimers__GetState(API011_Comm_Handle) != SYSTEMTIMERS_STATE_RUNNING)
                {
                    SystemTimers__SetMs(API011_Comm_Handle, API011APPCTRL_STANDARD_REQUEST_PERIOD);

                    // Checking the flag to decide if it is time to request Regulations or Status.
                    if(API011AppCtrl_Request_Message_Content == API011APPCTRL_MESSAGE_REQUEST_REGULATIONS)
                    {
                        API011_Num_Of_Consecutive_Regulations_Requests++;	// Increment number of regulations requests.
                        API011_Num_Of_Consecutive_Status_Requests = 0;		// Clear counter for status requests.
                        API011_Num_Of_Consecutive_Regulations_Sets = 0;     // Clear counter for regulations sets.
                        API011AppCtrl__RequestRegulations(WIN_ACU_ADDRESS);
                        API011AppCtrl_Request_Message_Content = API011APPCTRL_MESSAGE_REQUEST_STATUS;
                    }
                    else
                    {
                        API011_Num_Of_Consecutive_Status_Requests++;		// Increment number of status requests.
                        API011_Num_Of_Consecutive_Regulations_Requests = 0; // Clear counter for regulations requests.
                        API011AppCtrl__RequestStatus(WIN_ACU_ADDRESS);
                        API011AppCtrl_Request_Message_Content = API011APPCTRL_MESSAGE_REQUEST_REGULATIONS;
                    }
                }
                else
                {
                	/*
                	 * Verify if any of counters reached the limit. If yes act immediately.
                	 */
                	if((API011_Num_Of_Consecutive_Regulations_Sets >= MAX_NUM_OF_CONSECUTIVE_REQUESTS) ||
                	   (API011_Num_Of_Consecutive_Status_Requests >= MAX_NUM_OF_CONSECUTIVE_REQUESTS))
                	{
                		/*
                		 * To many regulations sets without requesting regulations OR
                		 * to many status requests without requesting regulations.
                		 * In both cases Request Regulations packet needs to be sent to ACU.
                		 */
						#if(API011APPCTRL_LOG_FEATURE == ENABLED)
                		if(API011_Num_Of_Consecutive_Regulations_Sets >= MAX_NUM_OF_CONSECUTIVE_REQUESTS)
                		{
                			// Reaching this point indicates abnormal behavior. This shall be avoided.
                			LOG_ADD_EXCEPTION(TO_MANY_CONSECUTIVE_REGULATIONS_SETS, 0);
                		}
                		else
                		{
                			// Reaching this point indicates abnormal behavior. This shall be avoided.
                			LOG_ADD_EXCEPTION(TO_MANY_CONSECUTIVE_STATUS_REQUESTS, 0);
                		}
						#endif //(API011APPCTRL_LOG_FEATURE == ENABLED)

                		if(TRUE == API011AppCtrl__RequestRegulations(WIN_ACU_ADDRESS))						// Request regulations.
                		{
                			API011_Num_Of_Consecutive_Regulations_Sets = 0;     							// Clear counter for regulations sets.
                			API011_Num_Of_Consecutive_Status_Requests = 0;									// Clear counter for status requests.
                			API011_Num_Of_Consecutive_Regulations_Requests++;								// Increment number of regulations requests.
                			SystemTimers__SetMs(API011_Comm_Handle, API011APPCTRL_TIMEOUT_TO_NEXT_REQUEST); 									// Set timer to get status in 100ms.
                			API011AppCtrl_Request_Message_Content = API011APPCTRL_MESSAGE_REQUEST_STATUS;	// Regulations just requested. Change content to status request.
                		}
                	}
                	else if(API011_Num_Of_Consecutive_Regulations_Requests >= MAX_NUM_OF_CONSECUTIVE_REQUESTS)
                	{
                		/*
                		 * To many regulation requests without status request.
                		 * In that case Request Status packet needs to be sent to ACU.
                		 */
                		if(TRUE == API011AppCtrl__RequestStatus(WIN_ACU_ADDRESS))								// Request status
                		{
                			API011_Num_Of_Consecutive_Regulations_Requests = 0;									// Clear counter for regulations requests.
                			API011_Num_Of_Consecutive_Status_Requests++;										// Increment counter for status requests.
                			SystemTimers__SetMs(API011_Comm_Handle, API011APPCTRL_TIMEOUT_TO_NEXT_REQUEST); 										// Set timer to get regulations in 100ms.
                			API011AppCtrl_Request_Message_Content = API011APPCTRL_MESSAGE_REQUEST_REGULATIONS; 	// Status just requested. Change content to regulations request.
                		}
						#if(API011APPCTRL_LOG_FEATURE == ENABLED)
                		// Reaching this point indicates abnormal behavior. This shall be avoided.
                		LOG_ADD_EXCEPTION(TO_MANY_CONSECUTIVE_REGULATIONS_REQUESTS, 0);
						#endif //(API011APPCTRL_LOG_FEATURE == ENABLED)
                	}
                }
            }
        }
    #elif (SYSTEM_ACU_HANDLING == ENABLED)
        API011AppCtrlPublishStatus();
    #endif
}

//----------------------------------------------------------------------------------------------------------
/**
 * @brief   This method parses messages target to the API017 sent to a broadcast or exclusively to this node for Reveal 4.x.
 * @param   buffer contains the Wide package that contains the message
 * @return  REVEAL_RECIPE_STATUS_TYPE that express the result of the parsing.
 */
REVEAL_RECIPE_STATUS_TYPE API011AppCtrl__CommandParser(REVEAL_MSG_TYPE * buffer)
{
    REVEAL_RECIPE_STATUS_TYPE retval;
    #if (SYSTEM_ACU_HANDLING == ENABLED)
    REVEAL_COMMAND_TYPE * ptr;
    uint8 size;
    CYCLE_NAME_TYPE cycle_name;
    uint8 compartment;
    uint8 selector_regulation_position = REGULATION_INVALID;
    uint8 num_of_selector_index_values;
    #endif

    MICRO_DECLARE_INTERRUPT_CONTEXT_LOCAL();

    retval = NVO_RS_READY;

	// data is a pointer that points to the payload
    switch ((API011APPCTRL_OPCODE_TYPE)buffer->op_code)
    {
        case API011APPCTRL_OPCODE_REQ_PUB_MODE:
            if (API011AppCtrl__PubMode() == FALSE)
            {
                retval = NVO_RS_BUSY;
            }
            break;
        case API011APPCTRL_OPCODE_SET_MODE:
            if (buffer->size == 1)
            {
                Mode__SetMode((MODE_TYPE)buffer->payload[0]);
            }
            else if (buffer->size == 2)
            {
                Mode__SetModeExtended((MODE_TYPE)buffer->payload[0],(SUBMODE_TYPE)buffer->payload[1]);
            }
            else
            {
                retval = NVO_RS_UNSUP_INVALID_PARAM;
            }
            break;
        case API011APPCTRL_OPCODE_MODE_HEARTBEAT:
            //buffer->payload[0] = heartbeat signature
            Mode__SpecialHeartBeat(MODE_SPECIAL_HEARTBEAT_PC_CONTROL);
            break;
    #if (SYSTEM_ACU_HANDLING == ENABLED)
        case API011APPCTRL_OPCODE_REQ_PUB_STATUS:
            if (API011AppCtrl__PubStatus() == FALSE)
            {
                retval = NVO_RS_REJECTED;
            }
            break;
        case API011APPCTRL_OPCODE_REQ_PUB_REGULATIONS:
            (void)API011AppCtrl__PubRegulations(buffer->node_id);
            break;
        case API011APPCTRL_OPCODE_SET_REGULATIONS:
            Regulations__SetRequestIndexBuffer(&buffer->payload[1],buffer->size-1);
            break;
        case API011APPCTRL_OPCODE_REG_POSITION_BY_ID:

            if (buffer->size == 1)
            {
                ptr = API011REVEAL__NEWFEEDBACK(API011APPCTRL_NUM, API011APPCTRL_OPCODE_REG_POSITION_BY_ID, REVEAL_BROADCAST_NODE,2);
                if (ptr != (REVEAL_COMMAND_TYPE *) NULL)
                {
                    Reveal__AddDataU8(ptr, buffer->payload[0]); // Reg ID
                    Reveal__AddDataU8(ptr, Regulations__GetPositionByFunctionID(buffer->payload[0],0)); // Reg Position
                }
                Reveal__QueueCommand(ptr);
            }
            else if (buffer->size == 2)
            {
                ptr = API011REVEAL__NEWFEEDBACK(API011APPCTRL_NUM, API011APPCTRL_OPCODE_REG_POSITION_BY_ID, REVEAL_BROADCAST_NODE,3);
                if (ptr != (REVEAL_COMMAND_TYPE *) NULL)
                {
                    Reveal__AddDataU8(ptr, buffer->payload[0]); // Reg ID
                    Reveal__AddDataU8(ptr, buffer->payload[1]); // Compartment
                    Reveal__AddDataU8(ptr, Regulations__GetPositionByFunctionID(buffer->payload[0],buffer->payload[1]));
                }
                Reveal__QueueCommand(ptr);
            }
            else
            {
                retval = NVO_RS_UNSUP_INVALID_PARAM;
            }

            break;
        case API011APPCTRL_OPCODE_REG_NUM_CYCLES:
            ptr = API011REVEAL__NEWFEEDBACK(API011APPCTRL_NUM, API011APPCTRL_OPCODE_REG_NUM_CYCLES, REVEAL_BROADCAST_NODE,APPLIANCE_MAX_COMPARTMENTS);
            if (ptr != (REVEAL_COMMAND_TYPE *) NULL)
            {
                uint8 compartment_index;

                for (compartment_index = 0; compartment_index < APPLIANCE_MAX_COMPARTMENTS; compartment_index++)    // Add number of cycles in each compartment
                {
                    Reveal__AddDataU8(ptr, Selector__GetNumberOfCycles(0, compartment_index));
                }

                Reveal__QueueCommand(ptr);
            }
            break;
        case API011APPCTRL_OPCODE_REG_PUB_STATUS_STRUCTURE:
            size = Status__GetSize();

            if (size > 0)
            {
                size = (size *3) + 1;

                ptr = API011REVEAL__NEWFEEDBACK(API011APPCTRL_NUM, API011APPCTRL_OPCODE_REG_PUB_STATUS_STRUCTURE, REVEAL_BROADCAST_NODE,size+1);
                if (ptr != (REVEAL_COMMAND_TYPE *) NULL)
                {
                    Status__BuildStatusInfoPackage(ptr);
                    Reveal__QueueCommand(ptr);
                }
            }
            else
            {
                ptr = API011REVEAL__NEWFEEDBACK(API011APPCTRL_NUM, API011APPCTRL_OPCODE_ERROR, REVEAL_BROADCAST_NODE,1);
                if (ptr != (REVEAL_COMMAND_TYPE *) NULL)
                {
                    Reveal__AddDataU8(ptr, API011APPCTRL_ERROR_STATUS_NOT_LOADED);
                    Reveal__QueueCommand(ptr);
                }
            }
            break;
        case API011APPCTRL_OPCODE_REQ_PUB_FUNCTION_IDS:
            ptr = API011REVEAL__NEWFEEDBACK(API011APPCTRL_NUM, API011APPCTRL_OPCODE_REQ_PUB_FUNCTION_IDS, REVEAL_BROADCAST_NODE, Regulations__GetTotalSize() * 2);

            if (ptr != (REVEAL_COMMAND_TYPE *) NULL)
            {
                REGULATIONS_CONFIG_TYPE * regulation_config;
                size = Regulations__GetTotalSize();

                for (uint8 function_position = 0; function_position < size; function_position++)    // Add function ID and compartment for each function position
                {
                    regulation_config = Regulations__GetRegulationConfig(function_position);
                    Reveal__AddDataU8(ptr, regulation_config->Function_ID);
                    Reveal__AddDataU8(ptr, regulation_config->Compartment);
                }

                Reveal__QueueCommand(ptr);
            }
            break;
        case API011APPCTRL_OPCODE_REQ_PUB_CYCLE_NAMES:
            compartment = buffer->payload[0];
            selector_regulation_position = Regulations__GetPositionByFunctionID(FUNC_ID_SELECTOR, compartment);
            num_of_selector_index_values = Regulations__GetRegulationConfig(selector_regulation_position)->Num_Regulations;

            ptr = API011REVEAL__NEWFEEDBACK(API011APPCTRL_NUM,API011APPCTRL_OPCODE_REQ_PUB_CYCLE_NAMES, REVEAL_BROADCAST_NODE, (1 + (2 * num_of_selector_index_values)));

            if (ptr != (REVEAL_COMMAND_TYPE *) NULL)
            {
                // Payload contains:
                // [0]: Compartment ID
                // [1,2]: Cycle Name of cycle indicated by the Main Value at index 1 in the regulation table of Selector regulation
                // [3,4]: Cycle Name of cycle indicated by the Main Value at index 2 in the regulation table of Selector regulation.
                // ......
                Reveal__AddDataU8(ptr, compartment);

				// Add the 2-byte cycle name for each cycle that can be accessed by the Selector regulation
                for (uint8 selector_index = 1; selector_index < num_of_selector_index_values; selector_index++)
                {
                    cycle_name = Selector__GetCycleNameId(PRIMARY_SETTING_FILE, compartment, Regulations__GetMainValueByPositionPlusIndex(selector_regulation_position, selector_index));
                    Reveal__AddDataU16(ptr, cycle_name);
                }
                Reveal__QueueCommand(ptr);
            }

            break;
    #endif //(SYSTEM_ACU_HANDLING == ENABLED)

        case API011APPCTRL_OPCODE_RESTORE_FACTORY_REGULATION_INDEX:

            //The blow code shall be protected. No other Threads shall be serviced here
            MICRO_SAVE_INTERRUPT_CONTEXT_LOCAL();

            #if (NVDATA_STORE_APPLIANCE_AND_STATISTICS_FEATURE == ENABLED)
                //Store the Data
                NVData__SetApplianceSegmentCorrupt();
            #endif

            Micro__ForceReset(MICRO_RESET_MODE_COLD);
            MICRO_RESTORE_INTERRUPT_CONTEXT_LOCAL();
            break;

        default:
			retval = NVO_RS_UNSUP_OP_CODE;
            break;
    }

    return (retval);
}


//----------------------------------------------------------------------------------------------------------
/**
 * @brief This method parses feedbacks to the API011AppCtrl sent to a broadcast or exclusively to this node.
 * @param buffer contains the Wide package that contains the message
 */
void API011AppCtrl__FeedbackParser(REVEAL_MSG_TYPE * buffer)
{
    switch ((API011APPCTRL_OPCODE_TYPE)buffer->op_code)
    {
    #if (SYSTEM_HMI_HANDLING == ENABLED)
        case API011APPCTRL_OPCODE_REQ_PUB_STATUS:
            if (API011AppCtrl_Request_Message_Content == API011APPCTRL_MESSAGE_REQUEST_STATUS)
            {
                SystemTimers__SetMs(API011_Comm_Handle, API011APPCTRL_STANDARD_REQUEST_PERIOD);
                API011AppCtrl_Request_Message_Content = API011APPCTRL_MESSAGE_REQUEST_REGULATIONS;
            }
            API011_Num_Of_Consecutive_Regulations_Requests = 0;

            #define VARIABLE_PAYLOAD_SIZE       (buffer->size - 1)

            Status__SetACUStatus(buffer->payload[0], &buffer->payload[1],VARIABLE_PAYLOAD_SIZE);

            break;
        case API011APPCTRL_OPCODE_REQ_PUB_REGULATIONS:
            if (API011AppCtrl_Request_Message_Content == API011APPCTRL_MESSAGE_REQUEST_REGULATIONS)
            {
                SystemTimers__SetMs(API011_Comm_Handle, API011APPCTRL_STANDARD_REQUEST_PERIOD);
                API011AppCtrl_Request_Message_Content = API011APPCTRL_MESSAGE_REQUEST_STATUS;
            }
            Regulations__SetIndexBuffer(&buffer->payload[1],buffer->size-1);
            API011APPCTRL_REGULATION_RX_NOTIFICATION();
            break;
    #endif
        case API011APPCTRL_OPCODE_REQ_PUB_MODE:
            Mode__SaveRemoteMode((MODE_TYPE)buffer->payload[0]);
            Mode__SaveRemoteSubMode((SUBMODE_TYPE)buffer->payload[1]);
            break;
        case API011APPCTRL_OPCODE_ERROR:
            API011APPCTRL__RECEIVED_ERROR(buffer->node_id,buffer->payload[1]);
            break;
        default:
            break;
    }
}

//----------------------------------------------------------------------------------------------------------
/**
 * @brief This method is used from the Application to request the status from a given node.
 * @param address
 * @return
 */
BOOL_TYPE API011AppCtrl__RequestStatus(uint8 address)
{
	#if (SYSTEM_HMI_HANDLING == ENABLED)
    	return (Reveal__QueueMessageNoData((uint8)API011APPCTRL_NUM,(uint8)API011APPCTRL_OPCODE_REQ_PUB_STATUS,address,TRUE,(BOOL_TYPE)API0011APPCTRL_USE_PRIORITY_QUEUE));
	#else
		return (FALSE);
	#endif
}

//----------------------------------------------------------------------------------------------------------
/**
 * @brief This method is used to Request Regulations from a given Node.
 * @param address
 * @return
 */
BOOL_TYPE API011AppCtrl__RequestRegulations(uint8 address)
{
	#if (SYSTEM_HMI_HANDLING == ENABLED)
	    return (Reveal__QueueMessageNoData((uint8)API011APPCTRL_NUM,(uint8)API011APPCTRL_OPCODE_REQ_PUB_REGULATIONS,address,TRUE,(BOOL_TYPE)API0011APPCTRL_USE_PRIORITY_QUEUE));
	#else
		return (FALSE);
	#endif
}


//----------------------------------------------------------------------------------------------------------
/**
 * @brief This method is used to Set Regulations on a given node.
 * @param address
 * @param regulation
 * @param size
 * @return
 */
BOOL_TYPE API011AppCtrl__SetRegulations(uint8 address)
{
    #if (SYSTEM_HMI_HANDLING == ENABLED)
        REVEAL_COMMAND_TYPE * ptr;
        uint8 * ptd;
        BOOL_TYPE retval;
        uint8 size;

        retval = FALSE;

        size = Regulations__GetTotalSize();
        ptd = (uint8 *)Regulations__GetIndexRequestBufferPtr(); //lint !e926 cast from pointer to pointer [MISRA 2004 Rule 11.4]
        if ((size > 0) &&
            (ptd != NULL))
        {
            ptr = API011REVEAL__NEWCOMMAND(API011APPCTRL_NUM, API011APPCTRL_OPCODE_SET_REGULATIONS, address,size+1);
            if (ptr != (REVEAL_COMMAND_TYPE *) NULL)
            {
                Reveal__AddDataU8(ptr, 0);      // The requirement for this data ("product type") has not yet been defined.
                Reveal__AddDataU8Buffer(ptr, ptd,size);
                Reveal__QueueCommand(ptr);
                retval = TRUE;
                API011_Num_Of_Consecutive_Regulations_Sets++;
                SystemTimers__SetMs(API011_Comm_Handle, API011APPCTRL_TIMEOUT_TO_NEXT_REQUEST);
                API011AppCtrl_Request_Message_Content = API011APPCTRL_MESSAGE_REQUEST_REGULATIONS;
            }
        }
        return (retval);
    #else
        return (FALSE);
    #endif
}

//----------------------------------------------------------------------------------------------------------
/**
 * @brief This method is used to publish the current Mode as a broadcast feedback.
 * @param mode
 * @return
 */
BOOL_TYPE API011AppCtrl__PubMode(void)
{
    REVEAL_COMMAND_TYPE * ptr;
    ptr = API011REVEAL__NEWFEEDBACK(API011APPCTRL_NUM, API011APPCTRL_OPCODE_REQ_PUB_MODE, REVEAL_BROADCAST_NODE,2);

    if (ptr != (REVEAL_COMMAND_TYPE *) NULL)
    {
        Reveal__AddDataU8(ptr, Mode__GetMode());
        Reveal__AddDataU8(ptr, Mode__GetSubMode());
        Reveal__QueueCommand(ptr);
        return (TRUE);
    }
    else
    {
        return (FALSE);
    }
}

//----------------------------------------------------------------------------------------------------------
/**
 * @brief This method published the Status when requested, via command or function call.
 * @details Multiple messages are sent if the data does not fit in the messages
 * @return
 *  @retval - TRUE(1) - Able to build the message with all the status data
 *  @retval - FALSE(0) - Failed to build the message
 */
BOOL_TYPE API011AppCtrl__PubStatus(void)
{
	#if (SYSTEM_ACU_HANDLING == ENABLED)
        REVEAL_COMMAND_TYPE * ptr           = NULL;
        uint8 variable_list_track           = 0;
        BOOL_TYPE status                    = FALSE;
        BOOL_TYPE variables_pending         = FALSE;

        if ((API011_Status_Ready == TRUE) &&    // Only publish status when the ApplianceMgr module has reported that it is stable AND
            (Status__IsValid() == TRUE))        // status configuration is vaild
        {
            //The below logic sends multiple messages if all the data does not fit in one message
            do
            {
                #define VARIABLE_PAYLOAD_ALLOWED_LENGTH         (REVEAL_PAYLOAD_SIZE - 1)

                ptr = Reveal__NewFeedback(API011APPCTRL_NUM, API011APPCTRL_OPCODE_REQ_PUB_STATUS, REVEAL_BROADCAST_NODE, VARIABLE_PAYLOAD_ALLOWED_LENGTH);
                if (ptr != (REVEAL_COMMAND_TYPE *)NULL)
                {
                    Reveal__AddDataU8(ptr, variable_list_track);
                    if (Status__BuildPackage(ptr, &variable_list_track, &variables_pending, VARIABLE_PAYLOAD_ALLOWED_LENGTH) == PASS)
                    {
                        if (variables_pending == TRUE)
                        {
                            //set the mmp bit if their are more than one message needs to be send
                            ptr->opcode |= REVEAL_MMP_BIT;
                        }
                        status = Reveal__QueueCommand(ptr);
                    }
                    else
                    {
                        status = FALSE;
                    }
                }
                else
                {
                    status = FALSE;
                }
            }while((variables_pending == TRUE) && (status == TRUE));
        }

        return(status);
	#else
		return (FALSE);
	#endif
}

//----------------------------------------------------------------------------------------------------------
/**
 * @brief This method published the Regulations when requested, via command of function call.
 * @return
 */
BOOL_TYPE API011AppCtrl__PubRegulations(uint8 node_id)
{
	#if (SYSTEM_ACU_HANDLING == ENABLED)
        REVEAL_COMMAND_TYPE * ptr;
        uint8 size;
        uint8 * ptd;
        BOOL_TYPE retval;

        retval = FALSE;
        size = Regulations__GetTotalSize();
        ptd = (uint8 *)Regulations__GetIndexBufferPtr();

        if (ptd != NULL)
        {
            ptr = API011REVEAL__NEWFEEDBACK(API011APPCTRL_NUM, API011APPCTRL_OPCODE_REQ_PUB_REGULATIONS, node_id, size+1);
            if (ptr != (REVEAL_COMMAND_TYPE *) NULL)
            {
                Reveal__AddDataU8(ptr, 0);  // The requirement for this data ("product type") has not yet been defined.
                Reveal__AddDataU8Buffer(ptr, ptd,size);
                Reveal__QueueCommand(ptr);
                retval = TRUE;
            }
        }

        if ((size == 0) ||
            (ptd == NULL))
        {
            ptr = API011REVEAL__NEWFEEDBACK(API011APPCTRL_NUM, API011APPCTRL_OPCODE_ERROR, node_id,1);
            if (ptr != (REVEAL_COMMAND_TYPE *) NULL)
            {
                Reveal__AddDataU8(ptr, API011APPCTRL_ERROR_REGULATION_NOT_LOADED);
                Reveal__QueueCommand(ptr);
            }
        }

        return (retval);
	#else
		return (FALSE);
	#endif
}

//----------------------------------------------------------------------------------------------------------
/**
 * @brief This method is used to control the polling process from the HMI to the ACU.
 * @param state
 */
void API011AppCtrl__SetRegStatusPolling(ON_OFF_TYPE state)
{
	#if (SYSTEM_HMI_HANDLING == ENABLED)
        API011_Hmi_Poll_Reg_Status = state;
	#endif
}
//=====================================================================================================================
//-------------------------------------- Private Functions ------------------------------------------------------------
//=====================================================================================================================

#if (SYSTEM_ACU_HANDLING == ENABLED)
//----------------------------------------------------------------------------------------------------------
/**
 * Local function to allow publishing Status on Event base. (ON CHANGE)
 * Here we check if the CRC of the Status variables has changed using a CRC16.
 */
static void API011AppCtrlPublishStatus(void)
{
    if (API011_Status_Ready == TRUE)                 // Wait for status variables to be ready before publishing
    {
        if (Status__HasChanged() == TRUE)            // Publish if any status variable has changed
        {                                            // Note: Calling Status__HasChanged() affects the logic that determines if the status has changed
            API011AppCtrl__PubStatus();
        }
    }
    else if ((ApplianceMgr__IsStable() == TRUE) &&
             (Status__IsValid() == TRUE))           // Don't want to send status message if the setting file configuration is not valid
    {
        API011_Status_Ready = TRUE;
        API011AppCtrl__PubStatus();
        Status__HasChanged();                       // Update logic in Status module so it doesn't repeat the same message
    }
}
#endif
