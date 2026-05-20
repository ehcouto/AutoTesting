/**
 *  @file
 *
 *  @brief      Source code for the Window Power module.
 *
 *  @details    Window Power uses the High Pass Filter to calculate the average of the signal power over a certain
 *              period of time.  Window Power is used by the Extraction Distribution algorithm.
 *
 *  @par        Module Owner
 *              Paul Spindler
 *
 *  $Header: WindowPower.c 1.2 2016/01/19 12:58:06EST Paul F Spindler (SPINDPF) Exp  $
 *
 *  @copyright  Copyright 2012 - $Date: 2016/01/19 12:58:06EST $  Whirlpool Corporation.  All rights reserved - CONFIDENTIAL.
 */

//-------------------------------------- Include Files ----------------------------------------------------------------
#include "C_Extensions.h"

#include "WindowPower.h"
#include "WindowPower_prv.h"

#include "AverageTorque.h"
#include "Callback.h"

#include "math.h"
#include <stdlib.h>
#include "string.h"

//-------------------------------------- PRIVATE (Variables, Constants & Defines) -------------------------------------

#define WINDOW_POWER_TIME_SEC   (WINDOW_POWER_ELEMENT_CALL_RATE_SEC *                   \
		(float)(WINDOW_POWER_ELEMENT_NUM_OF_READINGS_IN_SUM) *  \
		(float)(WINDOW_POWER_NUM_OF_ELEMENTS_IN_SUMMATION))

typedef enum
{
	STATE_INACTIVE = 0,
	STATE_ACTIVE
} WINDOW_POWER_STATE_TYPE;

// The Window Power Element properties
typedef struct
{
	unsigned char           Num_Of_Readings;
	WINDOW_POWER_VALUE_TYPE Sum_Of_Readings;
} WINDOW_POWER_ELEMENT_TYPE;

// The Window Power Summation properties
typedef struct
{
	WINDOW_POWER_VALUE_TYPE Buffer[WINDOW_POWER_MAX_NUM_OF_ELEMENTS_IN_SUMMATION];
	unsigned char           Index;
} WINDOW_POWER_SUMMATION_TYPE;


//! define the callback control structure module static variable
DEFINE_CALLBACK_CONTROL_STRUCTURE(WindowPower_Callback_Control_Structure, SIZE_OF_CALLBACK_REGISTER);


// structure defining the satelization power variables
static struct
{
	HIGH_PASS_FILTER_CONTROL_TYPE   High_Pass_Filter;
	unsigned char                   Num_Of_Average_Torque_Readings;
	WINDOW_POWER_STATE_TYPE         State;

	WINDOW_POWER_ELEMENT_TYPE       Element;
	WINDOW_POWER_SUMMATION_TYPE     Summation;
	WINDOW_POWER_VALUE_TYPE         Value;                                          // the value of the satelization power with filtering
} Window_Power;



//-------------------------------------- PRIVATE (Function Prototypes) ------------------------------------------------

static void AverageTorqueEventHandler(AVERAGE_TORQUE_EVENT_TYPE event);
static void CalculateWindowPowerValue(void);
static void ClearVariablesForNewCalculatedValue(void);
static void IncrementSummationIndex(void);

//=====================================================================================================================
//-------------------------------------- Public Functions -------------------------------------------------------------
//=====================================================================================================================

//---------------------------------------------------------------------------------------------------------------------
/**
 *  @brief      It Initializes the module WindowPower and its variables
 *
 */
void WindowPower__Initialize(void)
{
	memset(&Window_Power, 0, sizeof(Window_Power));
	Callback__Initialize(&WindowPower_Callback_Control_Structure);
	HighPassFilter__Initialize(&Window_Power.High_Pass_Filter);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 *  @brief      Disables the module.
 */
void WindowPower__Disable(void)
{
	AverageTorque__UnregisterCallback(&AverageTorqueEventHandler);
	Window_Power.State = STATE_INACTIVE;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 *  @brief      Enables the module.
 */
void WindowPower__Enable(void)
{
	switch ( Window_Power.State )
	{
	case STATE_INACTIVE:
		ClearVariablesForNewCalculatedValue();
		Window_Power.State = STATE_ACTIVE;
		HighPassFilter__ClearControl(&Window_Power.High_Pass_Filter);
		AverageTorque__RegisterCallback(&AverageTorqueEventHandler);
		break;
	case STATE_ACTIVE:
		// do nothing, module is already active
		break;
	default:
		break;
	}
}

//---------------------------------------------------------------------------------------------------------------------
/**
 *  @brief      Returns the value of the calculated Window Power.
 *
 *  @return     The Window Power.
 */
WINDOW_POWER_VALUE_TYPE WindowPower__GetValue(void)
{
	return ( Window_Power.Value );
}

//---------------------------------------------------------------------------------------------------------------------
/**
 *  @brief      Returns the value of the calculated Window Power High Pass Filtered Torque.
 *
 *  @return     High Pass Filtered torque value.
 */
HIGH_PASS_FILTER_VALUE_TYPE WindowPower__GetHighPassFilteredTorque(HIGH_PASS_FILTER_TYPE hpf_type)
{
	return ( HighPassFilter__GetValue(&Window_Power.High_Pass_Filter, hpf_type) );
}

//---------------------------------------------------------------------------------------------------------------------
/**
 *  @brief      Registers an event handler with the module
 *  @param      event_handler   The callback function to register
 */
void WindowPower__RegisterEventHandler(WINDOW_POWER_EVENT_HANDLER_TYPE event_handler)
{
	Callback__Register(&WindowPower_Callback_Control_Structure, (CALLBACK_HANDLER_TYPE)event_handler);
}


//---------------------------------------------------------------------------------------------------------------------
/**
 *  @brief      Unregisters an event handler with the module
 *  @param      event_handler    The callback function to register
 */
void WindowPower__UnregisterEventHandler(WINDOW_POWER_EVENT_HANDLER_TYPE event_handler)
{
	Callback__Unregister(&WindowPower_Callback_Control_Structure, (CALLBACK_HANDLER_TYPE)event_handler);
}


//=====================================================================================================================
//-------------------------------------- Private Functions ------------------------------------------------------------
//=====================================================================================================================

//---------------------------------------------------------------------------------------------------------------------
/**
 *  @brief      Handles events posted from the AverageTorque module
 *  @param      event   event posted from the AverageTorque module
 */
static void AverageTorqueEventHandler(AVERAGE_TORQUE_EVENT_TYPE event)
{
	switch ( event )
	{
	case AVERAGE_TORQUE_EV_NEW_VALUE_CALCULATED:
		Window_Power.Num_Of_Average_Torque_Readings++;
		if ( Window_Power.Num_Of_Average_Torque_Readings >= NUM_OF_AVERAGE_TORQUE_READINGS )
		{
			Window_Power.Num_Of_Average_Torque_Readings = 0;

			HighPassFilter__ProcessData(&Window_Power.High_Pass_Filter, INPUT_AVERAGE_TORQUE());
			CalculateWindowPowerValue();
		}
		// else wait for another torque reading
		break;
	default:
		break;
	}
}


//---------------------------------------------------------------------------------------------------------------------
/**
 *  @brief      Calculates the Window Power Value.
 */
static void CalculateWindowPowerValue(void)
{
	HIGH_PASS_FILTER_TYPE high_pass_filter_type = (abs(INPUT_REFERENCE_SPEED) > (SPEED_THRESHOLD_01*INPUT_REFERENCE_SPEED_MULTIPLIER)) ? HPF_1 : HPF_2;
	WINDOW_POWER_VALUE_TYPE power = powf(HighPassFilter__GetValue(&Window_Power.High_Pass_Filter, high_pass_filter_type), 2.0f);
	Window_Power.Element.Sum_Of_Readings += power;                                                        // cumulative signal power
	Window_Power.Element.Num_Of_Readings++;
	if (Window_Power.Element.Num_Of_Readings >= WINDOW_POWER_ELEMENT_NUM_OF_READINGS_IN_SUM)
	{
		Window_Power.Summation.Buffer[Window_Power.Summation.Index] = Window_Power.Element.Sum_Of_Readings;    // overwrite the oldest value in the buffer with the newest
		IncrementSummationIndex();
		memset(&Window_Power.Element, 0, sizeof(Window_Power.Element));                                         // reset the Element structure

		// sum all elements in Summation.Buffer
		WINDOW_POWER_VALUE_TYPE sum_of_buffer = 0.0f;
		for (uint8 index = 0; index < WINDOW_POWER_NUM_OF_ELEMENTS_IN_SUMMATION; index++)
		{
			sum_of_buffer += Window_Power.Summation.Buffer[index];
		}
		Window_Power.Value = sum_of_buffer / WINDOW_POWER_TIME_SEC;
	}

	Callback__Notify(&WindowPower_Callback_Control_Structure, (CALLBACK_EVENT_TYPE)WINDOW_POWER_EV_NEW_VALUE_CALCULATED);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 *  @brief      Clears the variables used to calculate the Window Power value.
 */
static void ClearVariablesForNewCalculatedValue(void)
{
	memset(&Window_Power.Element, 0, sizeof(Window_Power.Element));
	memset(&Window_Power.Summation, 0, sizeof(Window_Power.Summation));
	Window_Power.Value = 0.0f;
	Window_Power.Num_Of_Average_Torque_Readings = 0;
}


//---------------------------------------------------------------------------------------------------------------------
/**
 *  @brief      Increments the static variable Summation.Index and handles overflow.
 */
static void IncrementSummationIndex(void)
{
	Window_Power.Summation.Index += 1;                                                              // increment the buffer index
	if (Window_Power.Summation.Index >= WINDOW_POWER_NUM_OF_ELEMENTS_IN_SUMMATION)
	{                                                                                               // buffer rollover
		Window_Power.Summation.Index = 0;                                                           // reset the buffer index
	}
}
