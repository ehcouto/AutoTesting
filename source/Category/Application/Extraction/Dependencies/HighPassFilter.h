/**
 *  @file
 *
 *  @brief      API for the High Pass Filter module.
 *
 *  @details    See HighPassFilter.c details.
 *
 *  $Header: HighPassFilter.h 1.1 2016/01/19 12:57:39EST Paul F Spindler (SPINDPF) Exp  $
 *
 *  @copyright  Copyright 2012 - $Date: 2016/01/19 12:57:39EST $  Whirlpool Corporation.  All rights reserved - CONFIDENTIAL.
 */
#ifndef HIGHPASSFILTER_H_
#define HIGHPASSFILTER_H_

#include "Callback.h"
#include "HighPassFilter_prm.h"
#include "math.h"
#include "DigitalFilter.h"

//=====================================================================================================================
//-------------------------------------- PUBLIC (Extern Variables, Constants & Defines) -------------------------------
//=====================================================================================================================

//! the data type of the high pass filtered torque value
typedef float HIGH_PASS_FILTER_VALUE_TYPE;

//! control structure the module can process
typedef struct
{
	HIGH_PASS_FILTER_VALUE_TYPE			Raw_Value;
    HIGH_PASS_FILTER_VALUE_TYPE			Filtered_Value[NUM_OF_HIGH_PASS_FILTERS];
} HIGH_PASS_FILTER_CONTROL_TYPE;

//=====================================================================================================================
//-------------------------------------- PUBLIC (Function Prototypes) -------------------------------------------------
//=====================================================================================================================

void HighPassFilter__Initialize(HIGH_PASS_FILTER_CONTROL_TYPE *control);

void HighPassFilter__ClearControl(HIGH_PASS_FILTER_CONTROL_TYPE *control);
void HighPassFilter__ProcessData(HIGH_PASS_FILTER_CONTROL_TYPE *control, signed short value);
HIGH_PASS_FILTER_VALUE_TYPE HighPassFilter__GetValue(HIGH_PASS_FILTER_CONTROL_TYPE *control, HIGH_PASS_FILTER_TYPE hpf_type);

#endif /* HIGHPASSFILTER_H_ */
