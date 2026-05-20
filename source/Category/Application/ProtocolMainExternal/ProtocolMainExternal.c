/**
 *  @file       ProtocolMainExternal.c
 *
 *  @brief      The File containing UMain-DSP communication routines
 *
 *  @details    New Info Buffer,CRC calculated on buffer
 *
 *
 *  @copyright  Copyright 2016. Whirlpool Corporation. All rights reserved - CONFIDENTIAL
 */
//---------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------


//-------------------------------------- Include Files ----------------------------------------------------------------
#include "ProtocolMainExternal.h"
#include "ProtocolMainExternal_prv.h"
#include "CommandManager.h"
//#include "SensorsAlgorithms.h"
#include "ProtocolInfo.h"
#include "C_Extensions.h"
#include "FaultManager.h"
#include "Micro.h"
#include "Params.h"
#include "SRException.h"
#include "Uart.h"
#include <string.h>
#include "Crc16.h"
#include "mci_prm.h"

//-------------------------------------- PUBLIC (Variables) -----------------------------------------------------------

//-------------------------------------- PRIVATE (Variables, Constants & Defines) -------------------------------------

#if (APPLIANCE_TYPE == APPLIANCE_TYPE_WASHER)
#include "SRMotorSafetyInfoParamsMgr.h"
#include "MciSetWm.h"
#include "MciSensorsWm.h"
//#include "MotorSr_prm.h"
#elif (APPLIANCE_TYPE == APPLIANCE_TYPE_DISHWASHER)
#include "Gpio.h"
#include "MotorSr_Drain_prm.h"
#include "MotorSr_Wash_prm.h"
#include "SrMotorSafetyMgr.h"
#include "Mci.h"

#define RELAY_PUMPS_CONFIG()  Gpio__PinConfig(SWITCH_PUMPS_RELAY_PORT, SWITCH_PUMPS_RELAY_PIN, OUTPUT_PUSHPULL)
#define RELAY_PUMPS_OPEN()    Gpio__PinWrite(SWITCH_PUMPS_RELAY_PORT, SWITCH_PUMPS_RELAY_PIN, FALSE)
#define RELAY_PUMPS_CLOSE()   Gpio__PinWrite(SWITCH_PUMPS_RELAY_PORT, SWITCH_PUMPS_RELAY_PIN, TRUE)
#endif


enum
{
    TOKEN_PACKET_SEND_STX = 0,
    TOKEN_PACKET_SEND_EXTENDED,
    TOKEN_PACKET_SEND_HDR,
    TOKEN_PACKET_SEND_TYPE,
    TOKEN_PACKET_SEND_LENGTH,
    TOKEN_PACKET_SEND_DATA0,
    TOKEN_PACKET_SEND_CRCHI,
    TOKEN_PACKET_SEND_CRCLO,
    TOKEN_PACKET_SEND_SIZE
};

enum
{
    TOKEN_PACKET_RECEIVE_ACK = 0,
    TOKEN_PACKET_RECEIVE_STX,
    TOKEN_PACKET_RECEIVE_EXTENDED,
    TOKEN_PACKET_RECEIVE_HDR,
    TOKEN_PACKET_RECEIVE_TYPE,
    TOKEN_PACKET_RECEIVE_LENGTH,
    TOKEN_PACKET_RECEIVE_MINIMUM_SIZE,
    TOKEN_PACKET_RECEIVE_SEQUENCE_ID=TOKEN_PACKET_RECEIVE_MINIMUM_SIZE,

    // L1 load
    TOKEN_PACKET_RECEIVE_PILOT_TYPE_L1,         // multiple loads
    TOKEN_PACKET_RECEIVE_DISPL_SF_PARAMS_L1,    // multiple loads
    TOKEN_PACKET_RECEIVE_SPEEDHI_L1,            // multiple loads
    TOKEN_PACKET_RECEIVE_SPEEDLO_L1,            // multiple loads
    TOKEN_PACKET_RECEIVE_TIMEHI_L1,             // multiple loads
    TOKEN_PACKET_RECEIVE_TIMELO_L1,             // multiple loads
    TOKEN_PACKET_RECEIVE_FLAGSHI_L1,            // multiple loads
    TOKEN_PACKET_RECEIVE_FLAGSLO_L1,            // multiple loads

    // L2 load
    TOKEN_PACKET_RECEIVE_PILOT_TYPE_L2,         // multiple loads
    TOKEN_PACKET_RECEIVE_DISPL_SF_PARAMS_L2,    // multiple loads
    TOKEN_PACKET_RECEIVE_SPEEDHI_L2,            // multiple loads
    TOKEN_PACKET_RECEIVE_SPEEDLO_L2,            // multiple loads
    TOKEN_PACKET_RECEIVE_TIMEHI_L2,             // multiple loads
    TOKEN_PACKET_RECEIVE_TIMELO_L2,             // multiple loads
    TOKEN_PACKET_RECEIVE_FLAGSHI_L2,            // multiple loads
    TOKEN_PACKET_RECEIVE_FLAGSLO_L2,            // multiple loads

    TOKEN_PACKET_RECEIVE_DATATYPERAM,
    TOKEN_PACKET_RECEIVE_RAMREADSTARTHI,
    TOKEN_PACKET_RECEIVE_RAMREADSTARTLO,
    TOKEN_PACKET_RECEIVE_RAMREADENDHI,
    TOKEN_PACKET_RECEIVE_RAMREADENDLO,
    TOKEN_PACKET_RECEIVE_RAMWRITEADDRESSHI,
    TOKEN_PACKET_RECEIVE_RAMWRITEADDRESSLO,
    TOKEN_PACKET_RECEIVE_RAMWRITEDATAHI,
    TOKEN_PACKET_RECEIVE_RAMWRITEDATALO,
    TOKEN_PACKET_RECEIVE_EXTRAREQUESTHI,
    TOKEN_PACKET_RECEIVE_EXTRAREQUESTLO,
    TOKEN_PACKET_RECEIVE_CRCHI,
    TOKEN_PACKET_RECEIVE_CRCLO,
    TOKEN_PACKET_RECEIVE_SIZE
};

enum
{
    INFO_PACKET_SEND_STX = 0,
    INFO_PACKET_SEND_EXTENDED,
    INFO_PACKET_SEND_HDR,
    INFO_PACKET_SEND_TYPE,
    INFO_PACKET_SEND_LENGTH,
    INFO_PACKET_SEND_SEQUENCE_ID,
    INFO_PACKET_SEND_LENDATARAM,
    INFO_PACKET_SEND_DATATYPERAM,
    INFO_PACKET_SEND_CRCHI,
    INFO_PACKET_SEND_CRCLO,
    INFO_PACKET_SEND_FIXED_SIZE
};

enum
{
    READSETTING_STX = 0,
    READSETTING_EXTENDED,
    READSETTING_HDR,
    READSETTING_TYPE,
    READSETTING_LENGTH,
    READSETTING_POINTER,
    READSETTING_DISPL,
    READSETTING_CRCHI,
    READSETTING_CRCLO,
    READSETTING_SIZE
};

enum
{
    EXT_READSETTING_STX = 0,
    EXT_READSETTING_EXTENDED,
    EXT_READSETTING_HDR,
    EXT_READSETTING_TYPE,
    EXT_READSETTING_LENGTH,
    EXT_READSETTING_POINTER,
    EXT_READSETTING_DISPL,
    EXT_READSETTING_OFFSET_LO,
    EXT_READSETTING_OFFSET_HI,
    EXT_READSETTING_NUMBYTES,
    EXT_READSETTING_CRCHI,
    EXT_READSETTING_CRCLO,
    EXT_READSETTING_SIZE
};

enum
{
    READFLASH_RECEIVE_ACK = 0,
    READFLASH_RECEIVE_STX,
    READFLASH_RECEIVE_EXTENDED,
    READFLASH_RECEIVE_HDR,
    READFLASH_RECEIVE_TYPE,
    READFLASH_RECEIVE_LENGTH,
    READFLASH_RECEIVE_D0,
    READFLASH_RECEIVE_D1,
    READFLASH_RECEIVE_D2,
    READFLASH_RECEIVE_D3,
    READFLASH_RECEIVE_D4,
    READFLASH_RECEIVE_D5,
    READFLASH_RECEIVE_D6,
    READFLASH_RECEIVE_D7,
    READFLASH_RECEIVE_D8,
    READFLASH_RECEIVE_CRCHI,
    READFLASH_RECEIVE_CRCLO,
    READFLASH_RECEIVE_SIZE
};

//! INFO_PACKET_RECEIVE_DEFINITION
enum
{
    INFO_PACKET_SEND_ACK = 0,
    INFO_PACKET_RECEIVE_SIZE
};

typedef enum
{
    TOKEN                 = 0,
    INFO                  = 1,
    SETTINGFILELOADED     = 2,
    // put here loading tables of parameters
    DISPLDATA             = 3,
    SAFETYDATA            = 4,
    TRIPHASEDATA_1        = 5,    // PME_WASHER specific
    TRIPHASEDATA_2        = 6,    // PME_WASHER specific
    TRIPHASEDATA          = 7,    // PME_DISHWASHER specific
    MODULEDATA            = 8,    // PME_DISHWASHER specific
    MLIMITDATA            = 9,    // PME_WASHER specific
	MCISAFETYDATA      	  = 10,   // WINDY specific

	SBPM_FLOAT_DTC_CONTROL_1= 11,    // WINDY sBpm Floating specific
	SBPM_FLOAT_DTC_CONTROL_2= 12,    // WINDY sBpm Floating specific
	SBPM_FLOAT_MOTOR_LUT    = 13,    // WINDY sBpm Floating specific
	SBPM_FLOAT_SPEED_CTRL   = 14,    // WINDY sBpm Floating specific
	SBPM_FLOAT_MTPA_LUT     = 15,    // WINDY sBpm Floating specific
	SBPM_FLOAT_MTPV_LUT     = 16,    // WINDY sBpm Floating specific
	SBPM_FLOAT_OTE          = 17,    // WINDY sBpm Floating specific
	SBPM_FLOAT_SR_MOTOR     = 18,    // WINDY sBpm Floating specific
	SBPM_FLOAT_MCA_SET_WM   = 19,    // WINDY sBpm Floating specific
	SBPM_FLOAT_MCA_SENSOR_WM  = 20,  // WINDY sBpm Floating specific

    INVALID               = 256
} COMM_STATE_MACHINE_E_TYPE;

typedef enum
{
    MASTER_COMMUNICATION_STANDBY                    = 0,
    MASTER_COMMUNICATION_RECEIVING_IN_PROCESS       = 1,
    MASTER_COMMUNICATION_TRANSMITTING_IN_PROCESS    = 2,
    MASTER_COMMUNICATION_NEW_PACKET_READY           = 3,
    MASTER_COMMUNICATION_NEW_COMMAND_READY          = 4,
    MASTER_COMMUNICATION_DUMMY                      = 256
} MASTER_COMMUNICATION_RETURNS_E_TYPE;

typedef enum
{
    WAIT_FOR_ACK        = 0,
    WAIT_FOR_STX        = 1,
    WAIT_FOR_EXTENDED   = 2,
    WAIT_FOR_HDR        = 3,
    WAIT_FOR_TYPE       = 4,
    WAIT_FOR_LENGTH     = 5,
    WAIT_FOR_DATA       = 6,
    WAIT_FOR_DUMMY      = 256
} RECEIVE_STATE_MACHINE_E_TYPE;

typedef enum
{
    PARSER_STATE_RECEIVING  = 0,
    PARSER_STATE_ERROR      = 1,
    PARSER_STATE_COMPLETED  = 2,
    PARSER_STATE_DUMMY      = 256
} PARSER_RETURNS_E_TYPE;

// multiple loads
typedef enum
{
	EVALUATION_LOAD = 0,			//!< 0, evaluation state after a reset
	DRIVING_L1		= 1,			//!< 1, drive load L1
	DRIVING_L2		= 2,			//!< 2, drive load L2
	DRIVING_DUMMY	= 256
} DECODE_LOAD_STATE_MACHINE_ENUM_TYPE;

// Set your limit for RAM Read or put Min and Max to zero to disable RAM read
// If request is out of limit ram read will not be served
#if (uP_MODEL == uP_MODEL_RX)
    #define MIN_READABLE_ADDRESS    0x00000001
#define MAX_READABLE_ADDRESS        0x00003FFF
    #define READABLE_ADDRESS_OFFSET (MIN_READABLE_ADDRESS & 0xFFFF0000U)
#elif (uP_MODEL == uP_MODEL_MK3)
    #define MIN_READABLE_ADDRESS    0x1FFFE000//0x1FFF8000
    #define MAX_READABLE_ADDRESS    0x20001FFF // should be 0x20001FFF
    #define READABLE_ADDRESS_OFFSET (MIN_READABLE_ADDRESS & 0xFFFF0000U)
#endif

// Timings
#define COM_REQUEST_INTERVAL		2000  //250 ms * /1000 * Pwm Frequency
#define DELAY_TIMER_FREQ_KHZ        8
#define TOKEN_DELAY_TIME_MS         250
#define TOKEN_DELAY_TIME            TOKEN_DELAY_TIME_MS*DELAY_TIMER_FREQ_KHZ
#define RECEIVE_BYTE_TIMEOUT_MS     100
#define RECEIVE_BYTE_TIMEOUT        RECEIVE_BYTE_TIMEOUT_MS*DELAY_TIMER_FREQ_KHZ
#define BYTE_SEND_DELAY_MS          1
#define BYTE_SEND_DELAY             BYTE_SEND_DELAY_MS*DELAY_TIMER_FREQ_KHZ

// delay before trasmission in case of receive error
#define ERROR_DELAY_TIME_MS         100
#define ERROR_DELAY_TIME            ERROR_DELAY_TIME_MS*DELAY_TIMER_FREQ_KHZ

#define MAXIMUM_READABLE_RAM_LOCATIONS_INFO_WORDS   16
#define MAX_DELAY_BETWEEN_RAM_ADDRESS   (MAXIMUM_READABLE_RAM_LOCATIONS_INFO_WORDS<<1) // Multiplication by 2 for the byte addressing

#define MINIMAL_INFO_WORDS          (SIZE_OF_INFO_BUFFER/2)
#define MINIMAL_COMMON_WORDS        (SIZE_OF_COMMON_BUFFER/2)

#if (APPLIANCE_TYPE == APPLIANCE_TYPE_WASHER)
    #define INFO_PACKET_SEND_SIZE   INFO_PACKET_SEND_FIXED_SIZE + 2*MAXIMUM_READABLE_RAM_LOCATIONS_INFO_WORDS + 2 + 2*MINIMAL_INFO_WORDS + 2 + 2*MINIMAL_COMMON_WORDS
    #define MAX_COUNTER_AFTER_RAM   INFO_PACKET_SEND_SIZE-INFO_PACKET_SEND_SEQUENCE_ID-2-MINIMAL_INFO_WORDS*2-2-MINIMAL_COMMON_WORDS*2-2-1
#else
    #define INFO_PACKET_SEND_SIZE   INFO_PACKET_SEND_FIXED_SIZE + 2*MAXIMUM_READABLE_RAM_LOCATIONS_INFO_WORDS + 2*(2 + 2*MINIMAL_INFO_WORDS) + 2 + 2*MINIMAL_COMMON_WORDS
    #define MAX_COUNTER_AFTER_RAM   INFO_PACKET_SEND_SIZE-INFO_PACKET_SEND_SEQUENCE_ID-2*(2*MINIMAL_INFO_WORDS+2)-2-MINIMAL_COMMON_WORDS*2-2-1
#endif

#define NACK_BYTE                   0x55
#define ACK_BYTE                    0x5A
#define STX_BYTE                    0xA5
#define EXTENDED_BYTE               0xEE
#define HDR_BYTE                    0x01
#define TOKEN_TYPE_BYTE             0x6A
#define INFO_TYPE_BYTE              0x6B
#define DATA_TYPE_RAM               0x00
#define DATA_TYPE_COMMON            0xFF
#define TOKEN_CRCHI_BYTE            0x8C
#define TOKEN_CRCLO_BYTE            0x5E

#define READ_SETTINGFILE_DATA_TYPE_BYTE 0x4A

#define CRC_POLINOMIAL_VALUE        0x1021
#define CRC_INIT_VALUE              0xFFFF

#define TRIPHASE_MOTOR_CONTROL_AREA_POINTER 0x01

#define MAIN_POINTER                0x001
#define MAIN_DISPL                  0x000

#define IO_POINTER                  0x021

#ifndef MOTOR_SBPM_FLOAT
#define MINIMUM_APPLICATION_PARAMS_NUM  Params__SizeOfSFApp()
#define MINIMUM_SAFETY_PARAMS_NUM       Params__SizeOfSFSafe()
#define MINIMUM_MODULES_PARAMS_NUM      Params__SizeOfSFModule()
#define MINIMUM_MLIMIT_PARAMS_NUM       Params__SizeOfSFMLimit()


#define MINIMUM_APPLICATION_1_PARAMS_NUM    (Params__SizeOfSFApp()/2/2)*2 /* mast be even */
#define MINIMUM_APPLICATION_2_PARAMS_NUM    (MINIMUM_APPLICATION_PARAMS_NUM-MINIMUM_APPLICATION_1_PARAMS_NUM)
#endif


#define MASTER_COMMUNICATION_MAX_RESEND_PACKET  10

#define LEN_INFO                    SIZE_OF_INFO_BUFFER+1
#define LEN_COMMON                  SIZE_OF_COMMON_BUFFER+1

#if (APPLIANCE_TYPE == APPLIANCE_TYPE_DISHWASHER)
    #define DEFAULT_LOAD 		        PILOT_TYPE_WASH		// the wash pump is activated when relay is in rest position
#else
    #define DEFAULT_LOAD                0
#endif

static uint16 Com_Request_Interval;
static COMM_STATE_MACHINE_E_TYPE Master_Comm_Flags;
static uint16 Send_Buffer[INFO_PACKET_SEND_SIZE];
static uint16 Receive_Buffer[TOKEN_PACKET_RECEIVE_SIZE];
static uint16 SafetyParamsBuffer[MINIMUM_MCISAFETY_PARAMS_NUM/2];

static uint16 Bytes_To_Transmit;
static uint16 Bytes_To_Receive;
static uint16 Bytes_Transmitted;
static uint16 Bytes_Received;
static RECEIVE_STATE_MACHINE_E_TYPE Receive_State_Machine;
static uint16 Token_Time_Mark;
static uint16 Receive_Time_Mark;
static uint16 Send_Time_Mark;
static uint16 Attempts;
static uint16 Min_Setting_Bytes_Num;

static uint16* Ram_Read_Start_Address_Ptr;
// lint -esym(552,Ram_Read_End_Address_Ptr)
static uint16* Ram_Read_End_Address_Ptr;

// lint -esym(552,Seq_ID)
static uint16 Seq_ID;
static uint16 Params_Displ;
static uint16 Params_Displ_2;


static uint8 Prm_Displ_DTC_Ctrl;
static uint8 Prm_Displ_Motor_Lut;
static uint8 Prm_Displ_Speed_Ctrl;
static uint8 Prm_Displ_MTPA;
static uint8 Prm_Displ_MTPV;
static uint8 Prm_Displ_OTE;
static uint8 Prm_Displ_SR;
static uint8 Prm_Displ_Set_Wm;
static uint8 Prm_Displ_Sensor_Wm;

static uint16 Com_Safety_Flags;

static uint16 Min_Params_Num;
static uint16 Calculated_Crc;
static uint16 MainCrc;
static uint16 Calculated_Info_Crc;

//! Safety Protocol Variables
static uint16 Set_File_All_Crc;

static uint8* Set_File_Table;
static uint16 Table_Size;
static uint16 Buff_Safety_Displ;
static uint16 Crc_Application_1;

static uint16 Setting_File_Loading_Interrupted;

static uint16 Com_Timer_Cnt;
static uint16 Motor_Displ;
static DECODE_LOAD_STATE_MACHINE_ENUM_TYPE Decode_Load_State_Machine;
static uint16 Error_Delay;
static uint16 Module_Type;
static uint16 Modules_Displ;
static uint16 Displ_Sf_Params_L1;
static uint16 Displ_Sf_Params_L2;
static uint16 Dyn_Packet;

static COMMAND_DATA_TYPE Command_Data;
static COMMAND_DATA_TYPE Command_Data_L1;
static COMMAND_DATA_TYPE Command_Data_L2;

static uint16 Fault;
static uint16 COM_Global_Attempts;

#if (COMMAND_MODE==FREEMASTER)
//! Decode Master command handlin flag from FreeMaster 
static uint16 Master_Cmd_Force;
#endif

#if (COMMAND_MODE==SIL)
//! Decode Master command handlin flag from FreeMaster 
#include "Sil.h"
extern SIL_TYPE Sil;
#endif

//-------------------------------------- PRIVATE (Function Prototypes) ------------------------------------------------
static void ProtocolInitialization(COMM_STATE_MACHINE_E_TYPE comm_type);
static inline uint16 IsTxReady(void);
static uint16 MakeReadSettingFileDataBlock(uint16* buffer, uint16 pointer, uint16 displacement, uint16 offset, uint8 num_bytes);
static uint16 MakeInfoPacket(uint16* buffer);
static MASTER_COMMUNICATION_RETURNS_E_TYPE Rs232MasterCommunication(void);
static MASTER_COMMUNICATION_RETURNS_E_TYPE CheckResendPacket(uint16 resend_on_error);
static PARSER_RETURNS_E_TYPE RxParser(uint8 new_byte);
static PARSER_RETURNS_E_TYPE DisplProcessing(void);
static void DecodeLoadType(void);
static void InitPumps(void);
#if (uP_MODEL == uP_MODEL_MK3)
static inline uint32 TranslateAddress(uint32 address);
#endif /* uP_MODEL_MK3 */
static uint16 CalcCrc1(uint16 crc, uint8 data);
static uint16 CalcCrc(uint16 *buf, uint16 len);

//=====================================================================================================================
//-------------------------------------- Public Functions -------------------------------------------------------------
//=====================================================================================================================

//---------------------------------------------------------------------------------------------------------------------
/**
 *  @brief      It Initializes the module ProtocolMainExternal and its variables
 *
 */
void ProtocolMainExternal__Initialize(void)
{
    ProtocolInfo__Initialize();
#if (COM_MAIN_EXTERNAL == ENABLED)
    Uart__Initialize(PROTOCOLMAINEXTERNAL_COMPORT, PROTOCOLMAINEXTERNAL_BAUDRATE, UART_STOP_BIT_1, UART_PARITY_NONE);
    if (FaultManager__GetResetState() == TRUE)
    {
        // Reset the peripheral in case of external reset request
        Uart__Disable(PROTOCOLMAINEXTERNAL_COMPORT, UART_OPTION_ALL);
        Uart__Enable(PROTOCOLMAINEXTERNAL_COMPORT, UART_OPTION_ALL);
    }


//    SRMotorSafetyInfoParamsMgr__SetLoaded(FALSE);
    //Params__SetLoaded(FALSE);
    Params__SetReady(FALSE);
   // Params__SetUpdated(FALSE);

#endif //#if (COMM_MODE==MAIN_uC)

#if ((APPLIANCE_TYPE == APPLIANCE_TYPE_DISHWASHER) || (defined(INTERNAL_PARAMS)))

	ProtocolInitialization(TOKEN);  // initialization of Communication Protocol

#if (APPLIANCE_TYPE == APPLIANCE_TYPE_DISHWASHER)
	RELAY_PUMPS_CONFIG();
#else
	// We don't need a reset for the internal parameters to work
	Params__Initialize();
	Params__SetReady(TRUE);

	//Motor Sr parameters initialization
	//memcpy(SafetyParamsBuffer, &MotorSrParams, MINIMUM_MCISAFETY_PARAMS_NUM);
	//safety parameters Crc calculation to pass the check class B check in safety data (the excel file doesn't calculate it)
	//SafetyParamsBuffer[1] = Crc16__RangeProcess((uint8 *)(&SafetyParamsBuffer[2]),0xFFFF,(MINIMUM_MCISAFETY_PARAMS_NUM-4));
    //SafetyParamsBuffer[1] = CalcCrc(&SafetyParamsBuffer[2], MINIMUM_MCISAFETY_PARAMS_NUM-4);

	//SRMotorSafetyInfoParamsMgr__SetParams(&SafetyParamsBuffer[1]);
#endif

#else
	ProtocolInitialization(DISPLDATA); // initialization of Communication Protocol
#endif
	// initialization of ComRequestInterval
	Com_Request_Interval = TOKEN_DELAY_TIME;  // corresponding to 250ms (expressed in 8KHz cycles)

    Fault = 0;
    Motor_Displ = 0;
    Decode_Load_State_Machine = EVALUATION_LOAD;

    COM_Global_Attempts = 0;

    Setting_File_Loading_Interrupted = FALSE;
}


//---------------------------------------------------------------------------------------------------------------------
/**
 *  @brief      ProtocolMainExternal 250us handler
 *
 */
void ProtocolMainExternal__250usHandler(void)
{
    // Increment by 2 to simulate DELAY_TIMER_FREQ_KHZ = 8kHz frequency
    Com_Timer_Cnt += 2;

    // wait delay before transmitting in case of error
    if (Error_Delay > 1)
    {
        // Decrement by 2 to simulate DELAY_TIMER_FREQ_KHZ = 8kHz frequency
        Error_Delay -= 2;
    }
    else
    {
        Error_Delay = 0;
    }
}


//---------------------------------------------------------------------------------------------------------------------
/**
 *  @brief      ProtocolMainExternal 1ms handler
 *
 */
void ProtocolMainExternal__1msHandler(void)
{
	MASTER_COMMUNICATION_RETURNS_E_TYPE main_communication_return;

#if (COM_MAIN_EXTERNAL == ENABLED)
	if (FaultManager__GetResetState() == TRUE)
	{
	    // Do not communicate in case of external reset request
	    main_communication_return = MASTER_COMMUNICATION_STANDBY;
	}
	else
	{
	    main_communication_return = Rs232MasterCommunication();
	}
#else
	ProtocolInfo__GetInfo();
#endif

#if (COMMAND_MODE==MAIN_uC)

    // Main Communication Manager
    if (main_communication_return==MASTER_COMMUNICATION_NEW_COMMAND_READY)  // new command ready ?
    {
#elif (COMMAND_MODE==FREEMASTER)
    if(Master_Cmd_Force)
    {
        Master_Cmd_Force = 0;

#elif (COMMAND_MODE==MCI_BD)
    if(0)
    {
#elif (COMMAND_MODE==SIL)
    if(Sil.Cmd.Force == 1)
    {
        Sil.Cmd.Force = 0;
		Command_Data_L1.Speed = Sil.Cmd.Speed;
		Command_Data_L1.Ramp_Time = Sil.Cmd.Time;
		Command_Data_L1.Cmd_Flags = Sil.Cmd.Flags;
#elif (COMMAND_MODE==COMMAND_MANAGER_BD)
	{
#else
#endif
        #if (APPLIANCE_TYPE == APPLIANCE_TYPE_DISHWASHER)
		// Decode Load Type
		DecodeLoadType();

		#if (COMMAND_MODE==FREEMASTER)
			switch (Decode_Load_State_Machine)
			{
				case DRIVING_L1:
					Command_Data.Data_Type = PILOT_TYPE_DRAIN;
					break;

				case DRIVING_L2:
					Command_Data.Data_Type = PILOT_TYPE_WASH;
					break;

				default:
					break;
			}
		#endif

		#ifdef INTERNAL_PARAMS
	    InitPumps();
		#endif

	    CommandManager__DecodeMasterCommand(Command_Data.Speed, Command_Data.Ramp_Time);
		#else
		CommandManager__DecodeMasterCommand(Command_Data_L1.Speed, Command_Data_L1.Ramp_Time, Command_Data_L1.Cmd_Flags);


//        //Meaning request
//        if(BIT_TEST(Command_Data_L1.Cmd_Flags,MEANING_REQUEST) != 0)
//        {
//            SensorsAlgorithms__MeaningRequest(TRUE);
//        }
//        else
//        {
//            SensorsAlgorithms__MeaningRequest(FALSE);
//        }
//
//        //Limit normative request
//        if(BIT_TEST(Command_Data_L1.Cmd_Flags,VDC_NORM_REQUEST) != 0)
//        {
//            SensorsAlgorithms__LimitNormRequest(TRUE);
//        }
//        else
//        {
//            SensorsAlgorithms__LimitNormRequest(FALSE);
//        }
		#endif //#if (APPLIANCE_TYPE == APPLIANCE_TYPE_DISHWASHER)


    }
}


/**
 *  @brief      Get the last ProtocolMainExternal Fault
 *
 *  @return     Value of last fault detected by the module
 *
 */
uint16 ProtocolMainExternal__GetFault(void)
{
    return Fault;
}



/**
 *  @brief      Get the last ProtocolMainExternal Fault
 *
 *  @return     Value of last fault detected by the module
 *
 */
COMMAND_DATA_TYPE* ProtocolMainExternal__GetCommandData(LOAD_ENUM_TYPE load)
{

    COMMAND_DATA_TYPE* read_command;

    switch(load)
	{
		case LOAD_0:
			read_command = &Command_Data;
			break;

		case LOAD_1:
			read_command = &Command_Data_L1;
			break;

		case LOAD_2:
			read_command = &Command_Data_L2;
			break;

		default:
		    // Default value if no valid load provided
		    read_command = &Command_Data;
			break;
	};

    return(read_command);
}

#if (APPLIANCE_TYPE == APPLIANCE_TYPE_DISHWASHER)
//---------------------------------------------------------------------------------------------------------------------
/**
 *  @brief      Returns pointer to commond feedback struct
 *
 *  @param      comm_type - initial state
 */
//void ProtocolMainExternal__SetIPMType(IPM_TYPE ipm_type)
//{
//   Module_Type = ipm_type;
//}
#endif
   
//=====================================================================================================================
//-------------------------------------- Private Functions ------------------------------------------------------------
//=====================================================================================================================
//---------------------------------------------------------------------------------------------------------------------
/**
 *  @brief      Protocol initialisation
 *
 *  @param      comm_type - initial state
 */
void ProtocolInitialization(COMM_STATE_MACHINE_E_TYPE comm_type)
{
    Token_Time_Mark = Com_Timer_Cnt;
    Bytes_To_Transmit = 0;
    Bytes_To_Receive = 0;
    Error_Delay = 0;
    Attempts = 0;
    Master_Comm_Flags = comm_type;
    Ram_Read_Start_Address_Ptr = NULL;
    Ram_Read_End_Address_Ptr = NULL;
    Com_Safety_Flags = 0;
}


//---------------------------------------------------------------------------------------------------------------------
/**
 *  @brief      If Return a save time mark using Send_Time_Mark=Com_Timer_Cnt
 *
 *  @return     Return 1 or 0 depending on SCI Resisters. If ready to send another char return 1, else return 0
 */
inline uint16 IsTxReady(void)
{
    uint16 ret_val;
    ret_val = 0;

    if(TRUE == Uart__GetStatus(PROTOCOLMAINEXTERNAL_COMPORT, UART_STATUS_TX_EMPTY))
    {
        if((uint16)(Com_Timer_Cnt - Send_Time_Mark) >= BYTE_SEND_DELAY)
        {
            ret_val = 1;
        }
    }
    else
    {
        Send_Time_Mark = Com_Timer_Cnt;
    }

    return ret_val;
}


//---------------------------------------------------------------------------------------------------------------------
/**
 *  @brief      Makes read setting file data block
 *
 *  @param      buffer - Transmission buffer
 *  @return     size of block
 */
uint16 MakeReadSettingFileDataBlock(uint16* buffer, uint16 pointer, uint16 displacement, uint16 offset, uint8 num_bytes)
{
    uint16 crc;
    uint16 read_setting_size;

    *(buffer+READSETTING_STX)       = STX_BYTE;
    *(buffer+READSETTING_EXTENDED)  = EXTENDED_BYTE;
    *(buffer+READSETTING_HDR)       = HDR_BYTE;
    *(buffer+READSETTING_TYPE)      = READ_SETTINGFILE_DATA_TYPE_BYTE;

    if(num_bytes == 0)
    {
        *(buffer+READSETTING_LENGTH)    = READSETTING_CRCHI-READSETTING_LENGTH-1;
        *(buffer+READSETTING_POINTER)   = pointer;              // FIXED POINTER
        *(buffer+READSETTING_DISPL)     = displacement;         // FIXED DISPLACEMENT

        crc=CalcCrc(buffer,READSETTING_CRCHI);
        *(buffer+READSETTING_CRCHI)     = ((crc >> 8) & 0x00FF);
        *(buffer+READSETTING_CRCLO)     = ((crc) & 0x00FF);

        read_setting_size = READSETTING_SIZE;
    }
    else
    {
        *(buffer+EXT_READSETTING_LENGTH)    = EXT_READSETTING_CRCHI-EXT_READSETTING_LENGTH-1;
        *(buffer+EXT_READSETTING_POINTER)   = pointer;          // FIXED POINTER
        *(buffer+EXT_READSETTING_DISPL)     = displacement;     // FIXED DISPLACEMENT

        *(buffer+EXT_READSETTING_OFFSET_LO) = (offset) & 0x00FF;
        *(buffer+EXT_READSETTING_OFFSET_HI) = offset>>8;
        *(buffer+EXT_READSETTING_NUMBYTES)  = num_bytes;

        crc=CalcCrc(buffer,EXT_READSETTING_CRCHI);
        *(buffer+EXT_READSETTING_CRCHI)     = ((crc >> 8) & 0x00FF);
        *(buffer+EXT_READSETTING_CRCLO)     = ((crc) & 0x00FF);

        read_setting_size = EXT_READSETTING_SIZE;
    }

    return (read_setting_size);
}


//---------------------------------------------------------------------------------------------------------------------
/**
 *  @brief      Makes token packet
 *
 *  @param      buffer - Transmission buffer
 *  @return     size of block
 */
inline uint16 MakeTokenPacket(uint16* buffer)
{
    *(buffer+TOKEN_PACKET_SEND_STX)     = STX_BYTE;
    *(buffer+TOKEN_PACKET_SEND_EXTENDED)= EXTENDED_BYTE;
    *(buffer+TOKEN_PACKET_SEND_HDR)     = HDR_BYTE;
    *(buffer+TOKEN_PACKET_SEND_TYPE)    = TOKEN_TYPE_BYTE;
    *(buffer+TOKEN_PACKET_SEND_LENGTH)  = TOKEN_PACKET_SEND_CRCHI-TOKEN_PACKET_SEND_LENGTH-1;
    *(buffer+TOKEN_PACKET_SEND_DATA0)   = TOKEN_TYPE_BYTE;
    *(buffer+TOKEN_PACKET_SEND_CRCHI)   = TOKEN_CRCHI_BYTE; // attention: must be change accordingly to CRC calculation
    *(buffer+TOKEN_PACKET_SEND_CRCLO)   = TOKEN_CRCLO_BYTE; // attention: must be change accordingly to CRC calculation

    return (TOKEN_PACKET_SEND_SIZE);
}


//---------------------------------------------------------------------------------------------------------------------
/**
 *  @brief      Makes info packet
 *
 *
 *  @param      buffer - Transmission buffer
 *  @return     size of block
 */
uint16 MakeInfoPacket(uint16* buffer)
{
    uint16 counter;
    uint16 i;
    uint16 *buff_ptr;
    uint16 *ram_read_start_address_ptr_temp;
	sint16 k1;
    uint16 packet_len;
    PROTOCOL_INFO_TYPE* protocol_info;

    protocol_info = ProtocolInfo__GetInfo();
    packet_len = 1+2+MINIMAL_INFO_WORDS*2; // initialize at minimum size

    counter = 1;

    //------------------------------------------------------

    buff_ptr = (buffer+INFO_PACKET_SEND_SEQUENCE_ID);

    *(buffer+INFO_PACKET_SEND_STX)      = STX_BYTE;
    *(buffer+INFO_PACKET_SEND_EXTENDED) = EXTENDED_BYTE;
    *(buffer+INFO_PACKET_SEND_HDR)      = HDR_BYTE;
    *(buffer+INFO_PACKET_SEND_TYPE)     = INFO_TYPE_BYTE;
    *(buff_ptr)                         = Seq_ID;

   	if (Ram_Read_Start_Address_Ptr != NULL)
    {   // Ram/ Ident Table reading request
        packet_len += 2+(Ram_Read_End_Address_Ptr-Ram_Read_Start_Address_Ptr+1)*2; // add read ram

        *(buffer+INFO_PACKET_SEND_LENDATARAM)=(uint16)(1+(Ram_Read_End_Address_Ptr-Ram_Read_Start_Address_Ptr+1)*2);
        *(buffer+INFO_PACKET_SEND_DATATYPERAM)=DATA_TYPE_RAM;
        counter = 3;

        ram_read_start_address_ptr_temp = Ram_Read_Start_Address_Ptr;

        while (ram_read_start_address_ptr_temp <= Ram_Read_End_Address_Ptr)
        {
            if(counter > MAX_COUNTER_AFTER_RAM)
            {
                SRException__Queue(EXCEPTION_SEND_PACKET_FAIL, 0, 0);
            }
            else
            {
                *(buff_ptr+counter)= (*(ram_read_start_address_ptr_temp) >> 8);
                counter++;
                *(buff_ptr+counter)= (*(ram_read_start_address_ptr_temp++) & 0x00FF);
                counter++;
            }
        }
    }

    if(Decode_Load_State_Machine != DRIVING_L2)
    {// we are driving the load in the first place --> we have to write the first part of InfoBuffer
     // we are in "Evaluation" phase --> we have to write the first part of InfoBuffer
        k1 = 0;
    }
    else
    {
        k1 = MINIMAL_INFO_WORDS*2 + 2;
    }

    // Put len info & datatype
    *(buff_ptr+counter) = LEN_INFO;
    counter++;
    *(buff_ptr+counter) = Command_Data_L1.Data_Type;
    counter++;

    Buff_Safety_Displ = INFO_PACKET_SEND_SEQUENCE_ID+counter;

   	// Put minimal info related to driven load
    for (i=0;i<MINIMAL_INFO_WORDS;i++)
    {
        *(buff_ptr+k1+counter)= (((uint16*)protocol_info)[i]) >> 8;
        counter++;
        *(buff_ptr+k1+counter)= (((uint16*)protocol_info)[i]) & 0x00FF;
        counter++;
    }

	if (Dyn_Packet == 1) // --> two loads
	{
		packet_len += 2+MINIMAL_INFO_WORDS*2; // add second load

		*(buff_ptr+counter) = LEN_INFO;			// LENinfoL2 = 1(=DataTypeL2) + MINIMAL_INFO_WORDS (related to load L2)
		counter++;
		*(buff_ptr+counter) = Command_Data_L2.Data_Type;	// DataTypeL2 coming from main_uC
		counter++;

		// Put zeros info related to not-driven load
		for (i=0;i<MINIMAL_INFO_WORDS;i++)
		{
			*(buff_ptr-k1+counter)= 0;
			counter++;
			*(buff_ptr-k1+counter)= 0;
			counter++;
		}
	}

    packet_len += 2+MINIMAL_COMMON_WORDS*2; // add common buffer
    // Put common info & datatype
    *(buff_ptr+counter) = LEN_COMMON;
    counter++;
    *(buff_ptr+counter) = DATA_TYPE_COMMON;
    counter++;

    // Put minimal info
    for (i=0;i<MINIMAL_COMMON_WORDS;i++)
    {
        *(buff_ptr+counter)= (((uint16*)protocol_info)[i + MINIMAL_INFO_WORDS]) >> 8;
        counter++;
        *(buff_ptr+counter)= (((uint16*)protocol_info)[i + MINIMAL_INFO_WORDS]) & 0x00FF;
        counter++;
    }

    // Write Packet Len
    *(buffer+INFO_PACKET_SEND_LENGTH) = packet_len;

    counter = counter+2; // CRC Size

    return ((INFO_PACKET_SEND_SEQUENCE_ID+counter));
}





//---------------------------------------------------------------------------------------------------------------------
/**
 *  @brief      Master communication
 *
 *  @ref        MOD_SMI_3
 *
 *  @return     Communication state
 */
MASTER_COMMUNICATION_RETURNS_E_TYPE Rs232MasterCommunication()
{
    MASTER_COMMUNICATION_RETURNS_E_TYPE comm_state;
    uint8 rx_byte;

    comm_state = MASTER_COMMUNICATION_STANDBY;

    if (Bytes_To_Transmit)
    {
        comm_state = MASTER_COMMUNICATION_TRANSMITTING_IN_PROCESS;

        // Transmission in process
        if (IsTxReady())
        {
            if(Master_Comm_Flags == INFO)
            {
                if(Bytes_Transmitted == 0)
                {
                    Calculated_Info_Crc = CRC_INIT_VALUE;
                }

                // Crc Info Packet
                if(Bytes_To_Transmit == 2)
                {
                    Send_Buffer[Bytes_Transmitted] = Calculated_Info_Crc >> 8;
                    Send_Buffer[Bytes_Transmitted+1] = (Calculated_Info_Crc & 0x00FF);
                }
                else
                {
                    Calculated_Info_Crc = CalcCrc1(Calculated_Info_Crc, (uint8)Send_Buffer[Bytes_Transmitted]);
                }
            }
            Uart__WriteByte(PROTOCOLMAINEXTERNAL_COMPORT, ((uint8)Send_Buffer[Bytes_Transmitted++]));

            if (!(--Bytes_To_Transmit))
            {
                // Transmission finished
                Bytes_Transmitted=0;
                Bytes_Received=0;
                Receive_State_Machine=WAIT_FOR_ACK;
                Receive_Time_Mark=Com_Timer_Cnt;
                // Clear SCI Input data register
                if (Uart__GetStatus(PROTOCOLMAINEXTERNAL_COMPORT, UART_STATUS_RX_AVAILABLE) > 0)
                {
                    Uart__ReadByte(PROTOCOLMAINEXTERNAL_COMPORT);
                }

                //------------------------------------------------------------------------------

                if (Master_Comm_Flags == TOKEN)
                {
                    if (Com_Safety_Flags == 0x000F)
                    { // if Transmitted TOKEN after ACK
                        Com_Safety_Flags = 0x0007;
                    }

                }
                if (Master_Comm_Flags == INFO)
                {
                    if (Com_Safety_Flags == 0x0003)
                    { // if Transmitted INFO after ACK
                        Com_Safety_Flags = 0x0001;
                    }
                }

                comm_state = MASTER_COMMUNICATION_RECEIVING_IN_PROCESS;
            }
        }
    }
    else if (Bytes_To_Receive)
    {
        // Reception in process
        if ((uint16)(Com_Timer_Cnt - Receive_Time_Mark)>RECEIVE_BYTE_TIMEOUT)
        {// Byte Timeout Fail
            comm_state = (CheckResendPacket(2));
        }
        else if (Uart__GetStatus(PROTOCOLMAINEXTERNAL_COMPORT, UART_STATUS_RX_AVAILABLE) > 0)
        {
            rx_byte = Uart__ReadByte(PROTOCOLMAINEXTERNAL_COMPORT);
            Receive_Time_Mark=Com_Timer_Cnt;

            switch (Master_Comm_Flags)
            {

#ifndef MOTOR_SBPM_FLOAT

                case SAFETYDATA:
                    Set_File_Table = (uint16 *)Params__GetSFSafe();
                    Table_Size = Params__SizeOfSFSafe();
                    Min_Params_Num = MINIMUM_SAFETY_PARAMS_NUM;
                    break;

                case TRIPHASEDATA_1:
                    Set_File_Table = (uint16 *)Params__GetSFApp();
                    Table_Size = MINIMUM_APPLICATION_1_PARAMS_NUM;
                    Min_Params_Num = MINIMUM_APPLICATION_1_PARAMS_NUM;
                    break;

                case TRIPHASEDATA_2:
                    Set_File_Table = (uint16 *)Params__GetSFApp()+(MINIMUM_APPLICATION_1_PARAMS_NUM/2);
                    Table_Size = MINIMUM_APPLICATION_2_PARAMS_NUM;
                    Min_Params_Num = MINIMUM_APPLICATION_2_PARAMS_NUM;
                    break;

                case TRIPHASEDATA:
                    Set_File_Table = (uint16 *)Params__GetSFApp();
                    Table_Size = MINIMUM_APPLICATION_PARAMS_NUM;
                    Min_Params_Num = MINIMUM_APPLICATION_PARAMS_NUM;
                    break;

                case MODULEDATA:
                    Set_File_Table = (uint16 *)Params__GetSFModule();
                    Table_Size = MINIMUM_MODULES_PARAMS_NUM;
                    Min_Params_Num = MINIMUM_MODULES_PARAMS_NUM;
                    break;

                case MLIMITDATA:
                    Set_File_Table = (uint16 *)Params__GetSFMLimit();
                    Table_Size = MINIMUM_MLIMIT_PARAMS_NUM;
                    Min_Params_Num = MINIMUM_MLIMIT_PARAMS_NUM;
                    break;

                case MCISAFETYDATA:
					 Set_File_Table = (uint16 *)(&SafetyParamsBuffer[0]);
					 Table_Size = MINIMUM_MCISAFETY_PARAMS_NUM;
					 Min_Params_Num = MINIMUM_MCISAFETY_PARAMS_NUM;
					 break;
#else

                case SBPM_FLOAT_DTC_CONTROL_1:
                    Set_File_Table = Mci_ClassA_Dtc;
                    Table_Size = 0xFF;
                    Min_Params_Num = 0xFF;
                    break;

                case SBPM_FLOAT_DTC_CONTROL_2:
                    Set_File_Table = Mci_ClassA_Dtc + 0xFF;
                    Table_Size = sizeof(Mci_ClassA_Dtc)-0xFF;
                    Min_Params_Num = sizeof(Mci_ClassA_Dtc)-0xFF;
                    break;

                case SBPM_FLOAT_MOTOR_LUT:
                    Set_File_Table = Mci_ClassA_Motor_Lut;
                    Table_Size = sizeof(Mci_ClassA_Motor_Lut);
                    Min_Params_Num = sizeof(Mci_ClassA_Motor_Lut);
                    break;

                case SBPM_FLOAT_SPEED_CTRL:
                    Set_File_Table = Mci_ClassA_Speed_Ctrl;
                    Table_Size = sizeof(Mci_ClassA_Speed_Ctrl);
                    Min_Params_Num = sizeof(Mci_ClassA_Speed_Ctrl);
                    break;

                case SBPM_FLOAT_MTPA_LUT:
                    Set_File_Table = Mci_ClassA_Mtpa;
                    Table_Size = sizeof(Mci_ClassA_Mtpa);
                    Min_Params_Num = sizeof(Mci_ClassA_Mtpa);
                    break;

                case SBPM_FLOAT_MTPV_LUT:
                    Set_File_Table = Mci_ClassA_Mtpv;
                    Table_Size = sizeof(Mci_ClassA_Mtpv);
                    Min_Params_Num =  sizeof(Mci_ClassA_Mtpv);
                    break;

                case SBPM_FLOAT_OTE:
                    Set_File_Table = Mci_ClassA_Ote;
                    Table_Size = sizeof(Mci_ClassA_Ote);
                    Min_Params_Num =  sizeof(Mci_ClassA_Ote);
                    break;

                 case SBPM_FLOAT_SR_MOTOR:
                    Set_File_Table = Mci_ClassB_Prm;
                    Table_Size = sizeof(Mci_ClassB_Prm);
                    Min_Params_Num = sizeof(Mci_ClassB_Prm);
                    break;

                 case SBPM_FLOAT_MCA_SET_WM:
                    Set_File_Table = Mca_Set_Wm;
                    Table_Size = sizeof(Mca_Set_Wm);
                    Min_Params_Num = sizeof(Mca_Set_Wm);
                    break;

                 case SBPM_FLOAT_MCA_SENSOR_WM:
                    Set_File_Table = Mca_Sensor_Wm;
                    Table_Size = sizeof(Mca_Sensor_Wm);
                    Min_Params_Num = sizeof(Mca_Sensor_Wm);
                    break;
#endif



                default:
                    break;
            }

            switch (RxParser(rx_byte))
            {
                case PARSER_STATE_RECEIVING:
                    comm_state = MASTER_COMMUNICATION_RECEIVING_IN_PROCESS;
                    break;

                case PARSER_STATE_ERROR:
                    comm_state = CheckResendPacket(1);
                    break;

                case PARSER_STATE_COMPLETED:
                    // Reception Completed
                    CheckResendPacket(0);
                    if (Com_Safety_Flags == 0)
                    {
                        // reset Safety Flags
                        Com_Safety_Flags = 0x000F;
                    }

                    if(Master_Comm_Flags==INFO)
                    {
                        comm_state = MASTER_COMMUNICATION_NEW_COMMAND_READY;
                    }
                    else
                    {
                        comm_state = MASTER_COMMUNICATION_NEW_PACKET_READY;
                    }
                    break;

                default:
                    break;
            }
        }
        else
        {
            comm_state = MASTER_COMMUNICATION_RECEIVING_IN_PROCESS;
        }
    }
    else if (Error_Delay == 0)  // wait delay before transmitting in case of error
    {
        switch (Master_Comm_Flags)
        {
            case DISPLDATA:
                Bytes_To_Transmit=MakeReadSettingFileDataBlock(Send_Buffer, MAIN_POINTER, Motor_Displ, 0x0, 0x0);
                Bytes_To_Receive=TOKEN_PACKET_RECEIVE_MINIMUM_SIZE+2;
                comm_state = MASTER_COMMUNICATION_TRANSMITTING_IN_PROCESS;
                break;

#ifndef MOTOR_SBPM_FLOAT
            case SAFETYDATA:
                Bytes_To_Transmit=MakeReadSettingFileDataBlock(Send_Buffer, IO_POINTER, (Params_Displ&0x00FF), 0x0, 0x0);
                Bytes_To_Receive=TOKEN_PACKET_RECEIVE_MINIMUM_SIZE+2;
                comm_state = MASTER_COMMUNICATION_TRANSMITTING_IN_PROCESS;
                break;

            case TRIPHASEDATA_1:
                Bytes_To_Transmit=MakeReadSettingFileDataBlock(Send_Buffer, IO_POINTER, Params_Displ>>8, 0x0, MINIMUM_APPLICATION_1_PARAMS_NUM);
                Bytes_To_Receive=TOKEN_PACKET_RECEIVE_MINIMUM_SIZE+2;
                comm_state = MASTER_COMMUNICATION_TRANSMITTING_IN_PROCESS;
                break;

            case TRIPHASEDATA_2:
                Bytes_To_Transmit=MakeReadSettingFileDataBlock(Send_Buffer, IO_POINTER, Params_Displ>>8, MINIMUM_APPLICATION_1_PARAMS_NUM, MINIMUM_APPLICATION_2_PARAMS_NUM);
                Bytes_To_Receive=TOKEN_PACKET_RECEIVE_MINIMUM_SIZE+2;
                comm_state = MASTER_COMMUNICATION_TRANSMITTING_IN_PROCESS;
                break;


            case TRIPHASEDATA:
                Bytes_To_Transmit=MakeReadSettingFileDataBlock(Send_Buffer, IO_POINTER, Params_Displ>>8, 0x0, 0x0);
                Bytes_To_Receive=TOKEN_PACKET_RECEIVE_MINIMUM_SIZE+2;
                comm_state = MASTER_COMMUNICATION_TRANSMITTING_IN_PROCESS;
                break;

            case MODULEDATA:
                Bytes_To_Transmit=MakeReadSettingFileDataBlock(Send_Buffer, IO_POINTER, (Modules_Displ&0x00FF), 0x0, 0x0);
                Bytes_To_Receive=TOKEN_PACKET_RECEIVE_MINIMUM_SIZE+2;
                comm_state = MASTER_COMMUNICATION_TRANSMITTING_IN_PROCESS;
                break;

            case MLIMITDATA:
				Bytes_To_Transmit=MakeReadSettingFileDataBlock(Send_Buffer, IO_POINTER, Params_Displ_2>>8, 0x0, 0x0);
				Bytes_To_Receive=TOKEN_PACKET_RECEIVE_MINIMUM_SIZE+2;
				comm_state = MASTER_COMMUNICATION_TRANSMITTING_IN_PROCESS;
				break;

            case MCISAFETYDATA:
				Bytes_To_Transmit=MakeReadSettingFileDataBlock(Send_Buffer, IO_POINTER, (Params_Displ_2&0x00FF), 0x0, 0x0);
				Bytes_To_Receive=TOKEN_PACKET_RECEIVE_MINIMUM_SIZE+2;
				comm_state = MASTER_COMMUNICATION_TRANSMITTING_IN_PROCESS;
				break;


#else //sBPM Floating

            case SBPM_FLOAT_DTC_CONTROL_1:
                Bytes_To_Transmit=MakeReadSettingFileDataBlock(Send_Buffer, IO_POINTER, (uint16)Prm_Displ_DTC_Ctrl, 0x0, 0xFF);
                Bytes_To_Receive=TOKEN_PACKET_RECEIVE_MINIMUM_SIZE+2;
                comm_state = MASTER_COMMUNICATION_TRANSMITTING_IN_PROCESS;
                break;

            case SBPM_FLOAT_DTC_CONTROL_2:
                Bytes_To_Transmit=MakeReadSettingFileDataBlock(Send_Buffer, IO_POINTER, (uint16)Prm_Displ_DTC_Ctrl, 0xFF, sizeof(Mci_ClassA_Dtc)-0xFF);
                Bytes_To_Receive=TOKEN_PACKET_RECEIVE_MINIMUM_SIZE+2;
                comm_state = MASTER_COMMUNICATION_TRANSMITTING_IN_PROCESS;
                break;

            case SBPM_FLOAT_MOTOR_LUT:
                Bytes_To_Transmit=MakeReadSettingFileDataBlock(Send_Buffer, IO_POINTER, (uint16)Prm_Displ_Motor_Lut, 0x0, 0x0);
                Bytes_To_Receive=TOKEN_PACKET_RECEIVE_MINIMUM_SIZE+2;
                comm_state = MASTER_COMMUNICATION_TRANSMITTING_IN_PROCESS;
                break;

            case SBPM_FLOAT_SPEED_CTRL:
                Bytes_To_Transmit=MakeReadSettingFileDataBlock(Send_Buffer, IO_POINTER, (uint16)Prm_Displ_Speed_Ctrl, 0x0, 0x0);
                Bytes_To_Receive=TOKEN_PACKET_RECEIVE_MINIMUM_SIZE+2;
                comm_state = MASTER_COMMUNICATION_TRANSMITTING_IN_PROCESS;
                break;


            case SBPM_FLOAT_MTPA_LUT:
                Bytes_To_Transmit=MakeReadSettingFileDataBlock(Send_Buffer, IO_POINTER, (uint16)Prm_Displ_MTPA, 0x0, 0x0);
                Bytes_To_Receive=TOKEN_PACKET_RECEIVE_MINIMUM_SIZE+2;
                comm_state = MASTER_COMMUNICATION_TRANSMITTING_IN_PROCESS;
                break;

            case SBPM_FLOAT_MTPV_LUT:
                Bytes_To_Transmit=MakeReadSettingFileDataBlock(Send_Buffer, IO_POINTER, (uint16)Prm_Displ_MTPV, 0x0, 0x0);
                Bytes_To_Receive=TOKEN_PACKET_RECEIVE_MINIMUM_SIZE+2;
                comm_state = MASTER_COMMUNICATION_TRANSMITTING_IN_PROCESS;
                break;

            case SBPM_FLOAT_OTE:
                Bytes_To_Transmit=MakeReadSettingFileDataBlock(Send_Buffer, IO_POINTER, (uint16)Prm_Displ_OTE, 0x0, 0x0);
                Bytes_To_Receive=TOKEN_PACKET_RECEIVE_MINIMUM_SIZE+2;
                comm_state = MASTER_COMMUNICATION_TRANSMITTING_IN_PROCESS;
                break;

            case SBPM_FLOAT_SR_MOTOR:
                Bytes_To_Transmit=MakeReadSettingFileDataBlock(Send_Buffer, IO_POINTER, (uint16)Prm_Displ_SR, 0x0, 0x0);
                Bytes_To_Receive=TOKEN_PACKET_RECEIVE_MINIMUM_SIZE+2;
                comm_state = MASTER_COMMUNICATION_TRANSMITTING_IN_PROCESS;
                break;

            case SBPM_FLOAT_MCA_SET_WM:
                Bytes_To_Transmit=MakeReadSettingFileDataBlock(Send_Buffer, IO_POINTER, (uint16)Prm_Displ_Set_Wm, 0x0, 0x0);
                Bytes_To_Receive=TOKEN_PACKET_RECEIVE_MINIMUM_SIZE+2;
                comm_state = MASTER_COMMUNICATION_TRANSMITTING_IN_PROCESS;
                break;

            case SBPM_FLOAT_MCA_SENSOR_WM:
                Bytes_To_Transmit=MakeReadSettingFileDataBlock(Send_Buffer, IO_POINTER, (uint16)Prm_Displ_Sensor_Wm, 0x0, 0x0);
                Bytes_To_Receive=TOKEN_PACKET_RECEIVE_MINIMUM_SIZE+2;
                comm_state = MASTER_COMMUNICATION_TRANSMITTING_IN_PROCESS;
                break;

#endif

            case SETTINGFILELOADED:
                //SYSTEM_RESET();


				#if (APPLIANCE_TYPE == APPLIANCE_TYPE_DISHWASHER)
            	InitPumps();
				#else

//				// setting File Parameters reading finished
				#ifndef INTERNAL_PARAMS
            	// send the ID   -- Class A: copying from Flash to RAM
				MciSetWm__Initialize();
				MciSensorsWm__Initialize();
				CommandManager__Initialize();
				Params__Initialize();
				#endif

				//SRMotorSafetyInfoParamsMgr__SetParams(&SafetyParamsBuffer[1]);

				Params__SetReady(TRUE);
				ProtocolInitialization(TOKEN);  // initialization of Communication Protocol lint !e641
			    Com_Timer_Cnt+=Com_Request_Interval;    // to immediately start communication
				#endif




                break; // lint !e527

            case TOKEN:
                if ((uint16)(Com_Timer_Cnt - Token_Time_Mark)>Com_Request_Interval)
                {
                    // Prepare token request packet
                    Bytes_To_Transmit=MakeTokenPacket(Send_Buffer);
                    Bytes_To_Receive=TOKEN_PACKET_RECEIVE_MINIMUM_SIZE+2;        // +2 because the minimum size should also consider 16 BIT CRC
                    Token_Time_Mark=Com_Timer_Cnt;
                    comm_state = MASTER_COMMUNICATION_TRANSMITTING_IN_PROCESS;
                }
                else
                {
                    comm_state = MASTER_COMMUNICATION_STANDBY;
                }
                break;

            case INFO:
                Bytes_To_Transmit=MakeInfoPacket(Send_Buffer);
                Bytes_To_Receive=INFO_PACKET_RECEIVE_SIZE;
                comm_state = MASTER_COMMUNICATION_TRANSMITTING_IN_PROCESS;
                break;

            default:
                comm_state = MASTER_COMMUNICATION_STANDBY;
                break;
        }
    }
    else
    {   // wait delay before transmitting in case of error
        comm_state = MASTER_COMMUNICATION_STANDBY;
    }

    return comm_state;
}


//---------------------------------------------------------------------------------------------------------------------
/**
 *  @brief      Check Resend Packet
 *
 *  @ref        MOD_SMI_1
 *  @ref        MOD_SMI_3
 *
 *  @param      resend_on_error - Receiving error state (0 -> OK, !=0 -> KO)
 *  @return     Transmission state
 *              - MASTER_COMMUNICATION_NEW_PACKET_READY: a new packet can be transmitted
 *              - MASTER_COMMUNICATION_STANDBY: transmission is waiting
 */
MASTER_COMMUNICATION_RETURNS_E_TYPE CheckResendPacket(uint16 resend_on_error)
{
    MASTER_COMMUNICATION_RETURNS_E_TYPE comm_state;

    uint32  ram_write_address;
    uint16  ram_write_data;
    uint32  ram_read_start;
    uint32  ram_read_end;
    uint16  extra_request_lo;
    uint16  extra_request_hi;
    sint16  buf_ptr_shift;
    uint16  temp_ptr;
    uint16  i;

    Bytes_To_Transmit=0;
    Bytes_To_Receive =0;

    if(resend_on_error == 0)
    {
        Attempts=0;

        switch (Send_Buffer[TOKEN_PACKET_SEND_TYPE])
        {
            case READ_SETTINGFILE_DATA_TYPE_BYTE:

                if (Master_Comm_Flags > DISPLDATA)
                {

#ifndef MOTOR_SBPM_FLOAT

                    if (Master_Comm_Flags == TRIPHASEDATA_2)
                    {
                        Set_File_Table = (uint16 *)Params__GetSFApp();
                    }

                    if ((Set_File_All_Crc == *(Set_File_Table))||(Master_Comm_Flags == TRIPHASEDATA_1))  // checksum on buffer
#else
                    if(1) //Setting File Crc check is skipped
#endif
                    {
                        //---------------------------------------------------------------------------------------

                        if(Receive_Buffer[READFLASH_RECEIVE_LENGTH] < Min_Params_Num)
                        {// the number of parameters is less then the minimum expected  --> fault!
                            Fault = (FAULT_ENUM_TYPE)((uint16)INVALID_SETFILE_LENGTH + Master_Comm_Flags - DISPLDATA);
                            Master_Comm_Flags=TOKEN;

                            break;
                        }

                        switch (Master_Comm_Flags)
                        {

#ifndef MOTOR_SBPM_FLOAT

                            case SAFETYDATA:
                                #if (APPLIANCE_TYPE == APPLIANCE_TYPE_DISHWASHER)
                                    Master_Comm_Flags = TRIPHASEDATA;
                                #else
                                    Master_Comm_Flags = TRIPHASEDATA_1;
                                #endif //#if (APPLIANCE_TYPE == APPLIANCE_TYPE_DISHWASHER)
                                break;

                            case TRIPHASEDATA:
                                Master_Comm_Flags = MODULEDATA;
                                break;

                            case TRIPHASEDATA_1:
                                Master_Comm_Flags = TRIPHASEDATA_2;
                                Crc_Application_1 = Set_File_All_Crc;
                                break;

                            case TRIPHASEDATA_2:
								#ifdef MOTOR_BPM_TYPE
                            	Master_Comm_Flags = MCISAFETYDATA;
								#else
                            	Master_Comm_Flags = MLIMITDATA;
								#endif
                                break;

                            case MODULEDATA:
								Master_Comm_Flags = SETTINGFILELOADED;
								break;

                            case MLIMITDATA:
                                Master_Comm_Flags = MCISAFETYDATA;
                                break;

                            case MCISAFETYDATA:
                                Master_Comm_Flags = SETTINGFILELOADED;
                                break;
#else
                            case SBPM_FLOAT_DTC_CONTROL_1:
                                Master_Comm_Flags = SBPM_FLOAT_DTC_CONTROL_2;
                                break;

                            case SBPM_FLOAT_DTC_CONTROL_2:
                                Master_Comm_Flags = SBPM_FLOAT_MOTOR_LUT;
                                break;

                            case SBPM_FLOAT_MOTOR_LUT:
                                Master_Comm_Flags = SBPM_FLOAT_SPEED_CTRL;
                                break;

                            case SBPM_FLOAT_SPEED_CTRL:
                                Master_Comm_Flags = TOKEN;
                                //Setting file loading is interrupted by a TOKEN-COMMAND-INFO-ACK sequence in order to reset the ACU timeout (2 s).
                                Setting_File_Loading_Interrupted = TRUE; 
                                break;

                            case SBPM_FLOAT_MTPA_LUT:
                                Master_Comm_Flags = SBPM_FLOAT_MTPV_LUT;
                                break;

                            case SBPM_FLOAT_MTPV_LUT:
                                Master_Comm_Flags = SBPM_FLOAT_OTE;
                                break;

                            case SBPM_FLOAT_OTE:
                                Master_Comm_Flags = SBPM_FLOAT_SR_MOTOR;
                                break;

                            case SBPM_FLOAT_SR_MOTOR:
                                Master_Comm_Flags = SBPM_FLOAT_MCA_SET_WM;
                                break;

                            case SBPM_FLOAT_MCA_SET_WM:
                                Master_Comm_Flags = SBPM_FLOAT_MCA_SENSOR_WM;
                                break;

                            case SBPM_FLOAT_MCA_SENSOR_WM:
                                Master_Comm_Flags = SETTINGFILELOADED;
                                break;
#endif
                            default:
                                break;
                        }
                    }
                    else
                    {
                        Fault = (FAULT_ENUM_TYPE)((uint16)INVALID_SETFILE_CRC + Master_Comm_Flags - DISPLDATA);
                        Master_Comm_Flags = TOKEN;
                    }
                }
                else
                {	   // we receive Displacement data
#ifndef MOTOR_SBPM_FLOAT
                    Master_Comm_Flags = SAFETYDATA;
#else
                    Master_Comm_Flags = SBPM_FLOAT_DTC_CONTROL_1;
#endif
                }

                break;

            case INFO_TYPE_BYTE:

                //---------------------------------------------------------------------------------------

                #ifndef INTERNAL_PARAMS
                    // multiple loads: check the load and send the right packet - begin
                    if((HIBYTE(FaultManager__GetFaultToPublish()) == 0x09) || (FaultManager__GetFaultToPublish()== MODULETYPE_NET_FAULT) ||(Params__IsReady() == TRUE))
                    {// we have already loaded the setting file or we are choosing the correct load
                        Master_Comm_Flags = TOKEN;            // send the TOKEN packet
                    }
                    else
                    {// we have to load the setting file (<--> sFlags.TriphaseParamsReadFlag == 1)

                        if(Setting_File_Loading_Interrupted == TRUE)
                        {
                            Setting_File_Loading_Interrupted = FALSE;
                            Master_Comm_Flags = SBPM_FLOAT_MTPA_LUT;    // send the TRIPHASEDATA packet
                        }
                        else
                        {
                            Master_Comm_Flags = DISPLDATA;              // send the TRIPHASEDATA packet
                        }
                    }
                    // multiple loads: check the load and send the right packet - end
                #else
                    // Info sending finished..., wait for next token
                    Master_Comm_Flags = TOKEN;
                #endif
                if (Com_Safety_Flags == 0x0001)
                {     // Received ACK after INFO
                    Com_Safety_Flags = 0;
                }
                else
                {    // sequence is not correct: reset only the flags, but not reset the counter
                    Com_Safety_Flags = 0x000F;
                }
                break;

            case TOKEN_TYPE_BYTE:

                // Check if resend token for the second time & Read Data
                Seq_ID                          = (Receive_Buffer[TOKEN_PACKET_RECEIVE_SEQUENCE_ID]);

#if (COMMAND_MODE==MAIN_uC)
                Command_Data_L1.Speed          =  (sint16)((Receive_Buffer[TOKEN_PACKET_RECEIVE_SPEEDHI_L1] << 8) + (Receive_Buffer[TOKEN_PACKET_RECEIVE_SPEEDLO_L1]));
                Command_Data_L1.Ramp_Time      = (Receive_Buffer[TOKEN_PACKET_RECEIVE_TIMEHI_L1]  << 8) + (Receive_Buffer[TOKEN_PACKET_RECEIVE_TIMELO_L1]);
                Command_Data_L1.Cmd_Flags      = (Receive_Buffer[TOKEN_PACKET_RECEIVE_FLAGSHI_L1] << 8) + (Receive_Buffer[TOKEN_PACKET_RECEIVE_FLAGSLO_L1]);
#endif
                Command_Data_L1.Data_Type      = (Receive_Buffer[TOKEN_PACKET_RECEIVE_PILOT_TYPE_L1]);
                Displ_Sf_Params_L1              = (Receive_Buffer[TOKEN_PACKET_RECEIVE_DISPL_SF_PARAMS_L1]);

                Ram_Read_Start_Address_Ptr = NULL;
                Ram_Read_End_Address_Ptr   = NULL;

                // DYNAMIC LOAD COMMAND PACKET
                Dyn_Packet = 0;//  --> one load
                if ((Receive_Buffer[TOKEN_PACKET_RECEIVE_LENGTH]>TOKEN_PACKET_RECEIVE_PILOT_TYPE_L2-TOKEN_PACKET_RECEIVE_SEQUENCE_ID) &&
                    (Receive_Buffer[TOKEN_PACKET_RECEIVE_PILOT_TYPE_L2] != DATA_TYPE_RAM))
                {
#if (COMMAND_MODE==MAIN_uC)
                	// save Speed, Time and Flags of Load2
                    Command_Data_L2.Speed      = (sint16)((Receive_Buffer[TOKEN_PACKET_RECEIVE_SPEEDHI_L2] << 8) + Receive_Buffer[TOKEN_PACKET_RECEIVE_SPEEDLO_L2]);
                    Command_Data_L2.Ramp_Time  = ((Receive_Buffer[TOKEN_PACKET_RECEIVE_TIMEHI_L2]  << 8) + Receive_Buffer[TOKEN_PACKET_RECEIVE_TIMELO_L2]);
                    Command_Data_L2.Cmd_Flags  = ((Receive_Buffer[TOKEN_PACKET_RECEIVE_FLAGSHI_L2] << 8) + Receive_Buffer[TOKEN_PACKET_RECEIVE_FLAGSLO_L2]);
#endif
                    // save PilotType and Displacement of Load2
                    Command_Data_L2.Data_Type  = Receive_Buffer[TOKEN_PACKET_RECEIVE_PILOT_TYPE_L2];
                    Displ_Sf_Params_L2          = Receive_Buffer[TOKEN_PACKET_RECEIVE_DISPL_SF_PARAMS_L2];
                    buf_ptr_shift = 0;
                    Dyn_Packet = 1;  // --> Two loads
                }
                else
                {
                    buf_ptr_shift = (uint16)(TOKEN_PACKET_RECEIVE_PILOT_TYPE_L2 - TOKEN_PACKET_RECEIVE_DATATYPERAM);
                }

                if (Receive_Buffer[TOKEN_PACKET_RECEIVE_LENGTH]>TOKEN_PACKET_RECEIVE_EXTRAREQUESTHI-TOKEN_PACKET_RECEIVE_SEQUENCE_ID+buf_ptr_shift)
                {// an ExtraRequest has been requested
                    temp_ptr = TOKEN_PACKET_RECEIVE_EXTRAREQUESTHI+buf_ptr_shift;
                    extra_request_hi=Receive_Buffer[temp_ptr++];
                    extra_request_lo=Receive_Buffer[temp_ptr]>>1;

                    if (extra_request_hi==1)
                    {
                        if ((extra_request_lo == 0)||(extra_request_lo > MAXIMUM_READABLE_RAM_LOCATIONS_INFO_WORDS * 2))
                        {
                            extra_request_lo = MAXIMUM_READABLE_RAM_LOCATIONS_INFO_WORDS;
                        }

                        //Ram_Read_Start_Address_Ptr = (uint16*)&(IdentTable);
                        Ram_Read_Start_Address_Ptr = (uint16*)ProtocolInfo__GetIdent();
                        Ram_Read_End_Address_Ptr   = Ram_Read_Start_Address_Ptr + (extra_request_lo - 1);

                    }
                }
                else if (Receive_Buffer[TOKEN_PACKET_RECEIVE_LENGTH]>TOKEN_PACKET_RECEIVE_DATATYPERAM-TOKEN_PACKET_RECEIVE_SEQUENCE_ID+buf_ptr_shift)
                {// a RAM Read has been requested

                    temp_ptr = TOKEN_PACKET_RECEIVE_RAMREADSTARTHI+buf_ptr_shift;
                    ram_read_start = Receive_Buffer[temp_ptr++]<<8;
                    ram_read_start += Receive_Buffer[temp_ptr++];

                    // temp_ptr point to TOKEN_PACKET_RECEIVE_RAMREADENDHI
                    ram_read_end  = Receive_Buffer[temp_ptr++]<<8;
                    ram_read_end += Receive_Buffer[temp_ptr];

#if (uP_MODEL == uP_MODEL_MK3)
                    // Workaround for the KV30 memory map
                    // SRAM is in the range 0x1FFF_E000-0x2000_1FFF but we have only
                    // 16 bit for the address specification in the protocol
                    ram_read_start = TranslateAddress(ram_read_start<<1);
                    ram_read_end = TranslateAddress(ram_read_end<<1);
#endif /* uP_MODEL_MK3 */

                    if((ram_read_start>=MIN_READABLE_ADDRESS) && (ram_read_end<=MAX_READABLE_ADDRESS) && ((ram_read_end-ram_read_start) < MAX_DELAY_BETWEEN_RAM_ADDRESS))
                    {
                        Ram_Read_Start_Address_Ptr = (uint16 *) ram_read_start;
                        Ram_Read_End_Address_Ptr   = (uint16 *) ram_read_end;
                    }


                    if (Receive_Buffer[TOKEN_PACKET_RECEIVE_LENGTH]>TOKEN_PACKET_RECEIVE_RAMWRITEADDRESSHI-TOKEN_PACKET_RECEIVE_SEQUENCE_ID+buf_ptr_shift)
                    {// a RAM Write has been requested
                        temp_ptr = TOKEN_PACKET_RECEIVE_RAMWRITEADDRESSHI+buf_ptr_shift;
                        ram_write_address = Receive_Buffer[temp_ptr++]<<8;
                        ram_write_address += Receive_Buffer[temp_ptr++];
#if (uP_MODEL == uP_MODEL_MK3)
                        ram_write_address = TranslateAddress(ram_write_address<<1);
#endif /* uP_MODEL_MK3 */
                        // temp_ptr point to TOKEN_PACKET_RECEIVE_RAMWRITEDATAHI
                        ram_write_data = Receive_Buffer[temp_ptr++]<<8;
                        ram_write_data += Receive_Buffer[temp_ptr];

                        // execute WRITE RAM Request
                        if ((ram_write_address<=MAX_READABLE_ADDRESS) && (ram_write_address>=MIN_READABLE_ADDRESS))
                        {
                            *(uint16*) ram_write_address = ram_write_data;
                        }
                    }
                }

                // Token OK....
                Master_Comm_Flags=INFO;

                //---------------------------------------------------------------------------------------

                if (Com_Safety_Flags == 0x0007)
                { /* Received COMMAND after TOKEN */
                    Com_Safety_Flags = 0x0003;
                }
                break;

            default:
                break;
        }

        comm_state = MASTER_COMMUNICATION_NEW_PACKET_READY;
    }
    else
    {
        Attempts++;
        COM_Global_Attempts++;

        // Resend the same packet due to communication error

       // ---------------------------------------------------------------------------------------

        if (Attempts>MASTER_COMMUNICATION_MAX_RESEND_PACKET)
        {
            if(Send_Buffer[TOKEN_PACKET_SEND_TYPE] == READ_SETTINGFILE_DATA_TYPE_BYTE)
            {
                Fault = (FAULT_ENUM_TYPE)((uint16)SETFILE_NOT_LOADED +  Master_Comm_Flags - DISPLDATA);
            }

            Master_Comm_Flags=TOKEN;
        }

        if (resend_on_error == 1)
        {
            Error_Delay  = ERROR_DELAY_TIME;      // no error delay in case of receive timeout (ResendOnError ==2)
        }
        comm_state = MASTER_COMMUNICATION_STANDBY;
    }

    return comm_state;
}


//---------------------------------------------------------------------------------------------------------------------
/**
 *  @brief      Check Resend Packet
 *
 *  @ref        MOD_SMI_2
 *
 *  @param      new_byte Received byte
 *  @return     Parser state
 *              - PARSER_STATE_RECEIVING: receiving in progress
 *              - PARSER_STATE_ERROR: parsing error
 *              - PARSER_STATE_COMPLETED: Packet receiving and parsing complete
 */
PARSER_RETURNS_E_TYPE RxParser(uint8 new_byte)
{
    PARSER_RETURNS_E_TYPE rx_parser_state;
    rx_parser_state = PARSER_STATE_RECEIVING;

    switch (Receive_State_Machine)
    {
        case WAIT_FOR_ACK:
            if (new_byte==ACK_BYTE)
            {
                // initialize CRC value
                Calculated_Crc = CRC_INIT_VALUE;

                if (Master_Comm_Flags != TRIPHASEDATA_2)
                {
                    Set_File_All_Crc = CRC_INIT_VALUE;
                }
                else
                {
                    Set_File_All_Crc = Crc_Application_1;
                }
                Receive_State_Machine=WAIT_FOR_STX;
            }
            else
            {
                rx_parser_state = PARSER_STATE_ERROR;
            }
            break;

        case WAIT_FOR_STX:
            if (new_byte==STX_BYTE)
            {
                Receive_State_Machine=WAIT_FOR_EXTENDED;
            }
            else
            {
                rx_parser_state = PARSER_STATE_ERROR;
            }
            break;

        case WAIT_FOR_EXTENDED:
            if (new_byte==EXTENDED_BYTE)
            {
                Receive_State_Machine=WAIT_FOR_HDR;
            }
            else
            {
                rx_parser_state = PARSER_STATE_ERROR;
            }
            break;

        case WAIT_FOR_HDR:
            if (new_byte==HDR_BYTE)
            {
                Receive_State_Machine=WAIT_FOR_TYPE;
            }
            else
            {
                rx_parser_state = PARSER_STATE_ERROR;
            }
            break;

        case WAIT_FOR_TYPE:
            if (new_byte==(uint8)Send_Buffer[TOKEN_PACKET_SEND_TYPE])
            {
                Receive_State_Machine=WAIT_FOR_LENGTH;
            }
            else
            {
                rx_parser_state = PARSER_STATE_ERROR;
            }
            break;

        case WAIT_FOR_LENGTH:
            Bytes_To_Receive+=new_byte;
            Receive_State_Machine=WAIT_FOR_DATA;

            if (Master_Comm_Flags>DISPLDATA)
            {
                if(Table_Size<new_byte)
                {
                    Min_Setting_Bytes_Num = Table_Size;
                }
                else
                {
                    Min_Setting_Bytes_Num = new_byte;
                }
            }
            break;

        default:
            break;
    }

    if(rx_parser_state != PARSER_STATE_ERROR)
    {
        if ((Bytes_To_Receive - Bytes_Received) > 2)
        {
            // # Byte Received, @ Byte Not Received, x Byte Processed
            // Packet [###########x@@]

            //---------------------------------------------------------------------------------------

            if(Receive_State_Machine > WAIT_FOR_STX)
            {
                Calculated_Crc = CalcCrc1(Calculated_Crc, new_byte);
            }
            //---------------------------------------------------------------------------------------

            if (Master_Comm_Flags>DISPLDATA && (Bytes_Received > READFLASH_RECEIVE_LENGTH))
            {
                if (Bytes_Received <= (READFLASH_RECEIVE_LENGTH + Min_Setting_Bytes_Num))
                {

#ifndef MOTOR_SBPM_FLOAT

                    if (Bytes_Received & 1)
                    {
                        // READING high BYTE
                        *((uint8 *)Set_File_Table + ((Bytes_Received) - READFLASH_RECEIVE_LENGTH-2)) = (uint8)new_byte;
                    }
                    else
                    {
                        // READING low BYTE
                        *((uint8 *)Set_File_Table + ((Bytes_Received) - READFLASH_RECEIVE_LENGTH)) = (uint8)new_byte;
                    }
#else

                    *(Set_File_Table + (Bytes_Received - READFLASH_RECEIVE_LENGTH -1)) = new_byte;

#endif
                }

                //---------------------------------------------------------------------------------------

                if (Master_Comm_Flags != TRIPHASEDATA_2)
                {
                    if(Bytes_Received > READFLASH_RECEIVE_D1)    // Params__Get()->Params_Crc is not included
                    {   // we are receiving the setting file data
                        // (CRC on Buffer)
                        Set_File_All_Crc = CalcCrc1(Set_File_All_Crc, new_byte);
                    }
                }
                else
                {
                    // we are receiving the setting file data
                    // (CRC on Buffer)
                    Set_File_All_Crc = CalcCrc1(Set_File_All_Crc, new_byte);
                }
                //---------------------------------------------------------------------------------------
            }
            else
            {
                if (Bytes_Received >= sizeof(Receive_Buffer))
                {
                    rx_parser_state = PARSER_STATE_ERROR;
                }
                else
                {
                    Receive_Buffer[Bytes_Received]=(uint16)new_byte;
                }
            }
        }
        else if((Bytes_To_Receive - Bytes_Received) == 2)
        {
            // # Byte Received, @ Byte Not Received, x Byte Processed
            // Packet [############x@]

            MainCrc= ((uint16)new_byte << 8);
        }
        else // Ricezione Completata
        {
            // # Byte Received, @ Byte Not Received, x Byte Processed
            // Packet [#############x]

            rx_parser_state = PARSER_STATE_COMPLETED;

            if (Bytes_To_Receive!=INFO_PACKET_RECEIVE_SIZE)
            {
                MainCrc = MainCrc|((uint16)new_byte);

                if (Calculated_Crc!=MainCrc)
                {
                    // checksum fail
                    rx_parser_state = PARSER_STATE_ERROR;
                }
                else if (Master_Comm_Flags == DISPLDATA)
                {
                    #if (APPLIANCE_TYPE == APPLIANCE_TYPE_DISHWASHER)
                        rx_parser_state = DisplProcessing();
                    #else
#ifdef MOTOR_CIM_TYPE
                        if (Receive_Buffer[READFLASH_RECEIVE_LENGTH] >= 4)
                        {
                            Params_Displ  = ((Receive_Buffer[READFLASH_RECEIVE_D0] << 8) | (Receive_Buffer[READFLASH_RECEIVE_D1]));
                            Params_Displ_2 = ((Receive_Buffer[READFLASH_RECEIVE_D2] << 8) | (Receive_Buffer[READFLASH_RECEIVE_D3]));
                        }
                        else
                        {   /* invalid len */
                            rx_parser_state = PARSER_STATE_ERROR;
                        }
#else // ARK3 mode

                       	// check on packet len: must be 4
                       	if (Receive_Buffer[READFLASH_RECEIVE_LENGTH] >= 4)
                       	{
#ifdef MOTOR_SBPM_FLOAT
                          	Prm_Displ_DTC_Ctrl   = Receive_Buffer[READFLASH_RECEIVE_D0];
                           	Prm_Displ_Motor_Lut  = Receive_Buffer[READFLASH_RECEIVE_D1];
                           	Prm_Displ_Speed_Ctrl = Receive_Buffer[READFLASH_RECEIVE_D2];
                           	Prm_Displ_MTPA       = Receive_Buffer[READFLASH_RECEIVE_D3];
                           	Prm_Displ_MTPV       = Receive_Buffer[READFLASH_RECEIVE_D4];
                           	Prm_Displ_OTE        = Receive_Buffer[READFLASH_RECEIVE_D5];
                           	Prm_Displ_SR         = Receive_Buffer[READFLASH_RECEIVE_D6];
                           	Prm_Displ_Set_Wm     = Receive_Buffer[READFLASH_RECEIVE_D7];
                           	Prm_Displ_Sensor_Wm  = Receive_Buffer[READFLASH_RECEIVE_D8];
#else
							Params_Displ_2  = ((Receive_Buffer[READFLASH_RECEIVE_D0] << 8) | (Receive_Buffer[READFLASH_RECEIVE_D1]));
                            Params_Displ    = ((Receive_Buffer[READFLASH_RECEIVE_D2] << 8) | (Receive_Buffer[READFLASH_RECEIVE_D3]));
#endif
                       	}
                       	else
                       	{    // invalid len
                           	rx_parser_state = PARSER_STATE_ERROR;
                       	}
#endif // ARK2
                    #endif  //#if (APPLIANCE_TYPE == APPLIANCE_TYPE_DISHWASHER)
                }
            }
        }
    }

    Bytes_Received++;

    return rx_parser_state;
}

#if (APPLIANCE_TYPE == APPLIANCE_TYPE_DISHWASHER)
//---------------------------------------------------------------------------------------------------------------------
/**
 *  @brief      Displacement processing
 *
 *  @return     Displacement state
 */
PARSER_RETURNS_E_TYPE DisplProcessing(void)
{
    PARSER_RETURNS_E_TYPE rx_displ_state;
    uint16 displ_len_expected;
    rx_displ_state = PARSER_STATE_RECEIVING;

    displ_len_expected = 4 + ((Module_Type>>1)<<1);

    if (Receive_Buffer[READFLASH_RECEIVE_LENGTH] >= displ_len_expected)
    {
        Params_Displ = ((Receive_Buffer[READFLASH_RECEIVE_D0] << 8) | (Receive_Buffer[READFLASH_RECEIVE_D1]));
        Modules_Displ = Receive_Buffer[READFLASH_RECEIVE_D2+Module_Type];

        if(Modules_Displ != 0x00FF)
        {
            rx_displ_state = PARSER_STATE_COMPLETED;
        }
        else
        {
            rx_displ_state = PARSER_STATE_ERROR;
        }
    }
    else
    {
        rx_displ_state = PARSER_STATE_ERROR;
    }

    return rx_displ_state;
}


//---------------------------------------------------------------------------------------------------------------------
/**
 *  @brief      Decode Load Type
 *
 *  @remarks    This function to undestand what is the motor to drive
 *              hp: the application must avoid that [(PrevSpeedL1 != 0) && (PrevSpeedL2 != 0)] at same time
 *
 *  @ref        MOD_SMI_2
 *
 */
void DecodeLoadType(void)
{
    switch (Decode_Load_State_Machine)
    {
        default:
        case EVALUATION_LOAD:    // 0
            // Evaluation State after a reset: what load to drive?
            if (Command_Data_L1.Speed != 0)
            {// we have received a speed command not null for L1 load
                Decode_Load_State_Machine = DRIVING_L1;
                Motor_Displ = Displ_Sf_Params_L1;    // load the correct setting file
                Command_Data.Data_Type = Command_Data_L1.Data_Type;        // added for Tools <--> save the PilotType for Tools
            }
            else if(Command_Data_L2.Speed != 0)
            {// we have received a speed command not null for L2 load
                Decode_Load_State_Machine = DRIVING_L2;
                Motor_Displ = Displ_Sf_Params_L2;    // load the correct setting file
                Command_Data.Data_Type = Command_Data_L2.Data_Type;        // added for Tools <--> save the PilotType for Tools
            }
            else
            {// (PrevSpeedL1 == 0) && (PrevSpeedL2 == 0)
                // both speed commands are null
                // choose the default load according to the motor activated when the relay is in rest position
                // to avoid relay noise after each reset (every 32sec in case of null commands)
                if ((Command_Data_L1.Data_Type == DEFAULT_LOAD) || (Command_Data_L2.Data_Type == 0)) // L1 must be driven in case of single load (Command_Data_L2.Data_Type == 0)
                {
                    Decode_Load_State_Machine = DRIVING_L1;
                    Motor_Displ = Displ_Sf_Params_L1;    // load the correct setting file
                    Command_Data.Data_Type = Command_Data_L1.Data_Type;        // added for Tools <--> save the PilotType for Tools
                }
                else
                {
                    Decode_Load_State_Machine = DRIVING_L2;
                    Motor_Displ = Displ_Sf_Params_L2;    // load the correct setting file
                    Command_Data.Data_Type = Command_Data_L2.Data_Type;        // added for Tools <--> save the PilotType for Tools
                }
            }

            Command_Data.Ramp_Time = 0;
            Command_Data.Speed = 0;
            break;

        case DRIVING_L1:        // 1
            Motor_Displ = Displ_Sf_Params_L1;
            Command_Data.Data_Type = Command_Data_L1.Data_Type;
            // we are driving L1 load: what next?
            if(Command_Data_L2.Speed != 0)
            {// we have received a speed command not null for L2 load

                Command_Data.Ramp_Time = 0;
                Command_Data.Speed = 0;

                if((Params__IsUpdateAllowed() == TRUE) || (Params__IsReady() == FALSE))
                {
                    Decode_Load_State_Machine = DRIVING_L2;                 // switch load
                    Params__SetReady(FALSE);
                    Motor_Displ = Displ_Sf_Params_L2;                       // load the correct setting file
                    Command_Data.Data_Type = Command_Data_L2.Data_Type;   // added for Tools <--> save the PilotType for Tools
                }
            }
            else
            {// [ (PrevSpeedL1 != 0) || ((PrevSpeedL1 == 0) && (PrevSpeedL2 == 0)) ] --> drive load L1

                // set the commands for L1 load
                Command_Data.Ramp_Time = Command_Data_L1.Ramp_Time;
                Command_Data.Speed = Command_Data_L1.Speed;
                Command_Data.Cmd_Flags = Command_Data_L1.Cmd_Flags;
            }
            break;

        case DRIVING_L2:        // 2
            Motor_Displ = Displ_Sf_Params_L2; 
            Command_Data.Data_Type = Command_Data_L2.Data_Type;
            // we are driving L2 load: what next?
            if(Command_Data_L1.Speed != 0)
            {// we have received a speed command not null for L1 load

                Command_Data.Ramp_Time = 0;
                Command_Data.Speed = 0;

                if((Params__IsUpdateAllowed() == TRUE) || (Params__IsReady() == FALSE))
                {
                    Decode_Load_State_Machine = DRIVING_L1;                 // switch load
                    Params__SetReady(FALSE);
                    Motor_Displ = Displ_Sf_Params_L1;                       // load the correct setting file
                    Command_Data.Data_Type = Command_Data_L1.Data_Type;   // added for Tools <--> save the PilotType for Tool
                }
            }
            else
            {// [ (PrevSpeedL2 != 0) || ((PrevSpeedL1 == 0) && (PrevSpeedL2 == 0)) ] --> drive load L2

                // set the commands for L2 load
                Command_Data.Ramp_Time = Command_Data_L2.Ramp_Time;
                Command_Data.Speed = Command_Data_L2.Speed;
                Command_Data.Cmd_Flags = Command_Data_L1.Cmd_Flags;
            }
            break;
    }
}


//---------------------------------------------------------------------------------------------------------------------
/**
 *  @brief      It Initializes the module DishWasher Pumps according the decode load
 *
 */
void InitPumps(void)
{

	if((Params__IsReady() == FALSE)&&(Decode_Load_State_Machine!=EVALUATION_LOAD))
	{

		SensorsAlgorithms__Initialize();
		CommandManager__Initialize();
		Mci__SetDigital(MOTOR0, MCI_DO_CLEAR_ALL_FAILURE, TRUE);
		FaultManager__Initialize();
		ProtocolInfo__Initialize();

		switch (Decode_Load_State_Machine)
		{
			case DRIVING_L1:

				//Motor Sr parameters initialization
				memcpy(SafetyParamsBuffer, &MotorSrDrainParams, MINIMUM_MCISAFETY_PARAMS_NUM);
				//safety parameters Crc calculation to pass the check class B check in safety data (the excel file doesn't calculate it)
				SafetyParamsBuffer[1] = Crc16__RangeProcess((uint8 *)(&SafetyParamsBuffer[2]),0xFFFF,(MINIMUM_MCISAFETY_PARAMS_NUM-4));
				//SafetyParamsBuffer[1] = CalcCrc(&SafetyParamsBuffer[2], MINIMUM_MCISAFETY_PARAMS_NUM-4);

				SRMotorSafetyMgr__SetParams(&SafetyParamsBuffer[1]);
				Params__InitializeDrain();
				RELAY_PUMPS_OPEN();
				break;

			case DRIVING_L2:

				//Motor Sr parameters initialization
				memcpy(SafetyParamsBuffer, &MotorSrWashParams, MINIMUM_MCISAFETY_PARAMS_NUM);
				//safety parameters Crc calculation to pass the check class B check in safety data (the excel file doesn't calculate it)
				SafetyParamsBuffer[1] = Crc16__RangeProcess((uint8 *)(&SafetyParamsBuffer[2]),0xFFFF,(MINIMUM_MCISAFETY_PARAMS_NUM-4));
				//SafetyParamsBuffer[1] = CalcCrc(&SafetyParamsBuffer[2], MINIMUM_MCISAFETY_PARAMS_NUM-4);

				SRMotorSafetyMgr__SetParams(&SafetyParamsBuffer[1]);
				Params__InitializeWash();
				RELAY_PUMPS_CLOSE();
				break;

			default:
				break;
		}


		// We don't need a reset for the internal parameters to work
		ProtocolInitialization(TOKEN);  // initialization of Communication Protocol
		Params__SetReady(TRUE);
	}
}
#endif


#if (uP_MODEL == uP_MODEL_MK3)
/**
 *  @brief      Address translation for KV30
 *              Workaround for the KV30 memory map
 *              SRAM is in the range 0x1FFF_E000-0x2000_1FFF but we have only
 *              16 bit for the address specification in the protocol
 *
 *  @param      address Value to check and convert if needed
 *  @return     The converted address value
 */
static inline uint32 TranslateAddress(uint32 address)
{
    if (address >= 0xE000)
    {
        address += (0x1FFF0000);
    }
    else if (address < 0x2000)
    {
        address += (0x20000000);
    }
    else
    {
        // invalid address
        // make sure the MIN_READABLE_ADDRESS and MAX_READABLE_ADDRESS
        // are configured accordingly
        address += (0x1FFF0000);
    }

    return address;
}
#endif /* uP_MODEL_MK3 */


//---------------------------------------------------------------------------------------------------------------------
/**
 *  @brief      CRC calculation on the reception buffer
 *
 *  @ref        MOD_R1_6_1
 *
 *  @param      crc CRC on the previous step
 *  @param      byte Received byte to process
 *  @return     Current step CRC value
 */
uint16 CalcCrc1(uint16 crc, uint8 data)
{
    uint32 crc32;
    uint16 i;

    crc32 = crc ^ ((data << 8) & 0xFF00); // data byte is coupled into high byte
    for (i = 0; i < 8; i++)
    {
        crc32 = crc32 << 1;
        if (crc32 > 0xFFFF) // Test if bit 16 is set after shifting
        {
            crc32 = crc32 ^ CRC_POLINOMIAL_VALUE;
        }
        crc32 = crc32 & 0xFFFF; // clear bit 16 (if set)
    }

    return (uint16)crc32;
}


//---------------------------------------------------------------------------------------------------------------------
/**
 *  @brief      CRC calculation on the transmission buffer
 *
 *  @ref        MOD_R1_6_1
 *
 *  @param      *buf Transmission buffer
 *  @param      len Buffer length
 *  @return     CRC value
 */
uint16 CalcCrc(uint16 *buf, uint16 len)
{
    uint16 crc = CRC_INIT_VALUE;

    while (len--)
    {
        crc = CalcCrc1(crc, (uint8)*buf++);
    }
    return (uint16)(crc & 0x0000FFFF);
}
