/**
 *  @file
 *
 *  @brief      Source code for a High Pass Filter module.
 *
 *  @details    High Pass Filter module stores raw values in a buffer and applies filter coefficients to the raw values
 *              to calculate a filtered value.
 *
 *  $Header: HighPassFilter.c 1.1 2016/01/19 12:57:31EST Paul F Spindler (SPINDPF) Exp  $
 *
 *  @copyright  Copyright 2012 - $Date: 2016/01/19 12:57:31EST $  Whirlpool Corporation.  All rights reserved - CONFIDENTIAL.
 */

//-------------------------------------- Include Files ----------------------------------------------------------------
#include "C_Extensions.h"

#include "HighPassFilter.h" 
#include "HighPassFilter_prm.h"

#include "string.h"

//-------------------------------------- PRIVATE (Variables, Constants & Defines) -------------------------------------

//-------------------------------------- PRIVATE (Function Prototypes) ------------------------------------------------

static const DIGITAL_FILTER_SOS_TYPE *HP_Filter_Pointer[NUM_OF_HIGH_PASS_FILTERS];
static BIQUAD_STATE_TYPE HP_Filter_Storage[NUM_OF_HIGH_PASS_FILTERS][HP_FILTER_MAX_NUM_SOS_SECTIONS];

//=====================================================================================================================
//-------------------------------------- Public Functions -------------------------------------------------------------
//=====================================================================================================================

//---------------------------------------------------------------------------------------------------------------------
/**
 *  @brief      It Initializes the module HighPassFilter and its variables
 *  @param      control     pointer to the Application declaration of the control structure
 */
void HighPassFilter__Initialize(HIGH_PASS_FILTER_CONTROL_TYPE *control)
{
	HighPassFilter__ClearControl(control);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 *  @brief      Clears the Application definition of the Control structure
 *  @param      control     pointer to the Application definition of the control structure
 */
void HighPassFilter__ClearControl(HIGH_PASS_FILTER_CONTROL_TYPE *control)
{
	// Initialize filter setup pointers
	for (int i = 0; i<NUM_OF_HIGH_PASS_FILTERS; i++)
	{
		HP_Filter_Pointer[i] = GET_HIGH_PASS_FILTER_SETUP(i);
	}

	// Clear filter memory
	for (int i=0; i<NUM_OF_HIGH_PASS_FILTERS; i++)
	{
		DigitalFilter__ResetSOS(HP_Filter_Storage[i],HP_FILTER_MAX_NUM_SOS_SECTIONS);
	}
}

//---------------------------------------------------------------------------------------------------------------------
/**
 *  @brief      Processes a torque reading
 *  @param      control     pointer to the Application definition of the control structure
 *  @param      value       Application value in tenths
 *
 *  @return     status of the Application specific structure
 */
void HighPassFilter__ProcessData(HIGH_PASS_FILTER_CONTROL_TYPE *control, signed short value_x128)
{
	control->Raw_Value = (HIGH_PASS_FILTER_VALUE_TYPE)value_x128/128.0f;

	for (int i=0; i<NUM_OF_HIGH_PASS_FILTERS; i++)
	{
		control->Filtered_Value[i] = DigitalFilter__DF2TSOS(control->Raw_Value,*HP_Filter_Pointer[i],HP_Filter_Storage[i]);
	}
}

//---------------------------------------------------------------------------------------------------------------------
/**
 *  @brief      Returns the value of the requested High Pass Filter
 *  @param      control     pointer to the Application definition of the control structure
 *  @param      hpf_type    The index of the High Pass Filter.
 *
 *  @return     The filtered value
 */
HIGH_PASS_FILTER_VALUE_TYPE HighPassFilter__GetValue(HIGH_PASS_FILTER_CONTROL_TYPE *control, HIGH_PASS_FILTER_TYPE hpf_type)
{
	HIGH_PASS_FILTER_VALUE_TYPE return_value = 0.0f;
	if ( hpf_type < NUM_OF_HIGH_PASS_FILTERS )
	{
		return_value = control->Filtered_Value[hpf_type];
	}
	return (return_value);
}

