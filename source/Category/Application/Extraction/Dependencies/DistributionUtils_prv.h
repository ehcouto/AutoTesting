#ifndef DISTRIBUTIONUTILSPRV_H_
#define DISTRIBUTIONUTILSPRV_H_

#include "Extraction_prv.h"
#include "ExtractionParams.h"

//=====================================================================================================================
//-------------------------------------- PUBLIC (Extern Variables, Constants & Defines) -------------------------------

#define SATELLIZATION_SETUP			GESE_Extr_SatellizationSetup
#define PEAK_TO_PEAK_TORQUE_SETUP	GESE_Extr_P2PTorqueSetup

/* Torque input */
#define DIST_UTILS_INPUT_TORQUE_X128			(signed short)(EXTRACTION_TORQUE_INPUT*128.0f)

/* Speed reference input */
#define DIST_UTILS_INPUT_SPEED_REF_X16			(signed short)(EXTRACTION_SPEED_REF_INPUT*16.0f)

#define SATELLIZATION_CALC_MAX_VELOCITY			SATELLIZATION_SETUP->Max_Velocity

#define WINDOW_POWER_THRESHOLD					SATELLIZATION_SETUP->Window_Power_Threshold

/**
 *  The number of readings to add the sum
 *  Type: unsigned char
 *  Range: 1 to 255
 *  Resolution: 1 = 1 reading
 */
#define WINDOW_POWER_ELEMENT_NUM_OF_READINGS_IN_SUM		SATELLIZATION_SETUP->Window_Power_Num_Readings   //!< The number of power readings to accumulate the high pass filter torque values for

/**
 *  Type: unsigned char
 *  Range: 1 to 255
 */
#define WINDOW_POWER_NUM_OF_ELEMENTS_IN_SUMMATION		SATELLIZATION_SETUP->Window_Power_Num_Elements

/**
 *  Configuration definition for DXDT calculations.
 *  AverageTorque module produces a reading every 10msec.  Defines the time base base for the Dxdt module measurement.
 *
 *  Resolution: 1 = 1 AverageTorque reading (10msec)
 */
#define P2P_TORQUE_SNRD_SAMPLE_RATE		PEAK_TO_PEAK_TORQUE_SETUP->SNRD_Sample_Rate_Div_10

/**
 *  Speed threshold in order to use different filters in different speed ranges
 *  Type: signed short
 *  Range: 0 to 0xFFFF
 *  Resolution: 1 rpm
 */
#define SATELLIZATION_HPF_SELECTION_SPEED		SATELLIZATION_SETUP->HPF_Selection_Speed

#define HIGH_PASS_FILTER_SETUP_POINTER(index)	&GESE_Extr_HighPassFilterSetup[index]

#endif
