/**
 *  @file       FaultManager.c
 *
 *  @brief      Fault collector and remapping to Indesit Legacy codes
 *
 *  $Header: $
 *
 *  @copyright  Copyright 2016-$Date$. Whirlpool Corporation. All rights reserved - CONFIDENTIAL
 */
//---------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------


//-------------------------------------- Include Files ----------------------------------------------------------------
#include "FaultManager.h"
//#include "SensorsAlgorithms.h"
#include "MciSetWm.h"
#include "CommandManager.h"
#include "ProtocolMainExternal.h"
#include "Mci.h"
#include "Gpio.h"
#include "Comparator.h"
#include "SRException.h"
#include "BoardDebugging.h"

//-------------------------------------- PUBLIC (Variables) -----------------------------------------------------------

//-------------------------------------- PRIVATE (Variables, Constants & Defines) -------------------------------------
#define CMP1_DAC_VOLTAGE_SELECT		48          // V_th = 2.5 V (VOSEL = 48)
#define RESET_DELAY_TIMEOUT         2000        // 2 s @ 1 ms handler

FAULT_ENUM_TYPE Fault_To_Publish;
FAULT_ENUM_TYPE Fault_To_Publish_Holder;

static uint16 F028486_Flag;
static uint16 Reset_Flag;
static uint16 Reset_Delay_Cnt;
static volatile uint16 Reset_Comparator_Status;

//-------------------------------------- PRIVATE (Function Prototypes) ------------------------------------------------
static void DecodeReset(void);
static void InitReset(void);


//=====================================================================================================================
//-------------------------------------- Public Functions -------------------------------------------------------------
//=====================================================================================================================

//---------------------------------------------------------------------------------------------------------------------
/**
 *  @brief      It Initializes the module FaultManager and its variables.
 *
 *
 */
void FaultManager__Initialize(void)
{
	InitReset();
    Fault_To_Publish = NO_FAULT;
    Fault_To_Publish_Holder = Fault_To_Publish;
    F028486_Flag = 0;
    Reset_Delay_Cnt = RESET_DELAY_TIMEOUT;  //the comparator state is not evaluated before 2 s from reset. Micro will be indipendent from RC circuit time constant (around 512ms). In that case the micro startup can have a duration until 2 s.
}


//---------------------------------------------------------------------------------------------------------------------
/**
 *  @brief      It Reset the module FaultManager and its variables.
 *
 *
 */
void FaultManager__Reset(void)
{
    Fault_To_Publish = NO_FAULT;
    Fault_To_Publish_Holder = Fault_To_Publish;
    F028486_Flag = 0;
}

/**
 *  @brief      Update the Fault To Publish.
 *
 *
 */
void FaultManager__UpdateFault(void)
{
    FAULT_ENUM_TYPE actual_fault;
    actual_fault = NO_FAULT;

    // MCI Fault handling
    if(TRUE == Mci__GetError(MOTOR0, MCI_ERROR_ANY_ERROR_FOUND))
    {    
        if(TRUE == Mci__GetError(MOTOR0, MCI_ERROR_HW_OVER_CURRENT))
        {
            actual_fault = HARDWARE_OVERCURRENT_A;
        }
//        else if(TRUE == Mci__GetError(MOTOR0, MCI_DI_MOTOR_UNDER_OVERCURRENT))
//        {
//        	actual_fault = UNDERCURRENT_FAULT;
//        }
        else if(TRUE == Mci__GetError(MOTOR0, MCI_ERROR_SW_OVER_CURRENT))
        {
        	actual_fault = OVERCURRENT_FAULT;
        }
        else if(TRUE == Mci__GetError(MOTOR0, MCI_ERROR_DCBUS_OVER_VOLTAGE))
        {
            actual_fault = OVERVOLTAGE;
        }
        else if(TRUE == Mci__GetError(MOTOR0,  MCI_ERROR_DCBUS_UNDER_VOLTAGE))
        {
            actual_fault = UNDERVOLTAGE;
        }
        else if(TRUE == Mci__GetDigital(MOTOR0, MCI_DI_OVER_TORQUE))
        {
            actual_fault = OVERTORQUE_WARNING;
        }
        else if(TRUE == Mci__GetError(MOTOR0, MCI_ERROR_OBSERVER_FAILURE))
       {
           actual_fault = MCL_FAULT;
       }
        else if(TRUE == Mci__GetError(MOTOR0, MCI_ERROR_SPEED_CHECK_FAILED))
        {
            actual_fault = TARGET_ZERO_NOT_REACHED_FAULT;
        }
        else if(TRUE == Mci__GetError(MOTOR0, MCI_ERROR_INPUTCAPTURE_PLAUSIBILITY_MIN_FAILED))
        {
            actual_fault = MIN_TACH_FAULT;
        }
        else if(TRUE == Mci__GetError(MOTOR0, MCI_ERROR_INPUTCAPTURE_PLAUSIBILITY_MAX_FAILED))
        {
            actual_fault = MAX_SPEED_CHANGE_FAULT;
        }
//        else if(TRUE == Mci__GetError(MOTOR0, MCI_ERROR_SPEED_CHECK_FAILED))
//        {
//            actual_fault = MCI_LEGACY_SPD_SENSOR_FAILEDD;
//        }
        else if(TRUE == Mci__GetError(MOTOR0, MCI_ERROR_CURRENT_SENSOR_FAILED))
        {
            actual_fault = MCI_LEGACY_ELECTRONIC_FAILED;
        }
        else if(TRUE == Mci__GetError(MOTOR0, MCI_ERROR_VOLTAGE_SENSOR_FAILED))
        {
            actual_fault = MCI_LEGACY_MOTOR_TEST_FAILED;
        }
        else if(TRUE == Mci__GetError(MOTOR0, MCI_ERROR_INVERTER_OVER_TEMP))
        {
            actual_fault = MCI_LEGACY_INVERTER_OVER_TEMP;
        }
        else if(TRUE == Mci__GetError(MOTOR0, MCI_ERROR_STATOR_OVER_TEMP))
        {
            actual_fault = MCI_LEGACY_STATOR_OVER_TEMP;
        }
        else if(TRUE == Mci__GetError(MOTOR0, MCI_ERROR_LOCKED_ROTOR_AT_STARTUP))
        {
            actual_fault = NO_COMMUTATION_AFTER_START_FAULT;
            F028486_Flag = 1;
        }
        else if(TRUE == Mci__GetError(MOTOR0, MCI_ERROR_LOCKED_ROTOR_IN_RUNNING))
        {
            actual_fault = STALL_AFTER_COMMUTATION_FAULT;
        }
//        else if(TRUE == Mci__GetError(MOTOR0, MCI_DI_UNDERTORQUE))
//        {
//            actual_fault = STALL_LOW_SPEED_FAULT;
//            F028486_Flag = 1;
//        }
        else if(TRUE == Mci__GetError(MOTOR0, MCI_ERROR_DCBUS_OUT_OF_RANGE))
        {
            actual_fault = SR_DCBUS_OUT_OF_RANGE;
        }
        else if(TRUE == Mci__GetError(MOTOR0, MCI_ERROR_PHASE_LOST))
        {
            actual_fault = MCI_LEGACY_PHASE_LOST;
        }
        else if(TRUE == Mci__GetError(MOTOR0, MCI_ERROR_INT_DISABLED))
        {    
            actual_fault = MCI_LEGACY_INT_DISABLED;
        }
        else if(TRUE == Mci__GetError(MOTOR0, MCI_ERROR_MOTOR_OVERHEATING))
        {
            actual_fault = OH_FAULT;
        }
        else if(TRUE == Mci__GetError(MOTOR0, MCI_ERROR_CLASS_B_FORCE2STOP))
        {
            actual_fault = MCI_LEGACY_FORCE2STOP;
        }
        else if(TRUE == Mci__GetError(MOTOR0, MCI_ERROR_SHUTDOWN_HARDWARE_FAILED))
		{
		   actual_fault = MCI_LEGACY_SHUTDOWN_FAILED;
		}

        else if(TRUE == Mci__GetError(MOTOR0, MCI_ERROR_INITIALIZATION_FAILED))
        {
            actual_fault = MCI_INITIALIZATION_FAULT;
        }
    }
    // CommandManager Fault Handling
    else if ((FAULT_ENUM_TYPE) CommandManager__GetFault() != NO_FAULT)
    {
        actual_fault = (FAULT_ENUM_TYPE) CommandManager__GetFault();
    }
    // ProtocolMainExternal Fault Handling
    else if ((FAULT_ENUM_TYPE) ProtocolMainExternal__GetFault() != NO_FAULT)
    {
        actual_fault = (FAULT_ENUM_TYPE)ProtocolMainExternal__GetFault();
    }
    
    if(Fault_To_Publish == NO_FAULT) // Not overwrite a fault if it is already present
    {
    	Fault_To_Publish = (FAULT_ENUM_TYPE)actual_fault;
    }


#if (COMMAND_MODE == MAIN_uC)
#if((WINDY_STRIP_BOARD == 1)|| (DEA_702_BOARD == 1))
    // decode a reset command
    DecodeReset();
#endif
#endif
}


/**
 *  @brief      Get the Fault To Publish.
 *
 *  @return     Value of the fault, as updated by the FaultManager__UpdateFault
 *
 */
FAULT_ENUM_TYPE FaultManager__GetFaultToPublish(void)
{
	COMMAND_DATA_TYPE *command_data;

	command_data = ProtocolMainExternal__GetCommandData(LOAD_1);

	if((command_data->Speed != 0)||(HIBYTE(Fault_To_Publish) == 0x09))
	{
		Fault_To_Publish_Holder = Fault_To_Publish;
	}

	return Fault_To_Publish_Holder;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 *  @brief      Get the Reset state
 *
 *  @return     TRUE if reset request, FALSE otherwise
 */
BOOL_TYPE FaultManager__GetResetState(void)
{
    return (BOOL_TYPE) Reset_Flag;
}

//=====================================================================================================================
//-------------------------------------- Private Functions ------------------------------------------------------------
//=====================================================================================================================

static void InitReset(void)
{
    Comparator__Config(COMPARATOR_CHANNEL1, COMPARATOR_INVERTED_INPUT0, COMPARATOR_NONINVERTED_INPUT7, COMPARATOR_MODE_CONTINUOUS, CMP1_DAC_VOLTAGE_SELECT);
}


static void DecodeReset(void)
{

    Reset_Comparator_Status = Comparator__GetStatus(COMPARATOR_CHANNEL1);

    if ((Reset_Comparator_Status == 0)&&(Reset_Delay_Cnt == 0))
	{// Comparator Status is zero and Reset Delay is expired
	    Reset_Flag = 1;

		//SensorsAlgorithms__Initialize();
	    ProtocolMainExternal__Initialize();
	    MciSetWm__Initialize();
	    MciSensorsWm__Initialize();
		CommandManager__Initialize();	
		//BoardDebugging__Initialize();
		//Mci__SetDigital(MOTOR0, MCI_DO_CLEAR_ALL_FAILURE, TRUE);
		Mci__ClearError(MOTOR0);

		Fault_To_Publish = NO_FAULT;
		Fault_To_Publish_Holder = Fault_To_Publish;

#ifdef MOTOR_CIM_TYPE
		if(F028486_Flag == 1)
		{
			F028486_Flag = 0;
			MciSetWm__SoftStart(SOFT_START_MAX_REQUEST);
		}
#endif
	}
	else
	{
		if(Reset_Delay_Cnt > 0)
		{
	    	Reset_Delay_Cnt--;
		}	
		
	    Reset_Flag = 0;
	}
}
