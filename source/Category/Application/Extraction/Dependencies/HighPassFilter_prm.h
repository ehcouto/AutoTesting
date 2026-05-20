/**
 *  @file
 *
 *  @brief      Public parameters for the High Pass Filter module.
 *
 *  @details    See HighPassFilter.c details.
 *
 *  $Header: HighPassFilter_prm.h 1.1 2016/01/19 13:01:45EST Paul F Spindler (SPINDPF) Exp  $
 *
 *  @copyright  Copyright 2012 - $Date: 2016/01/19 13:01:45EST $  Whirlpool Corporation.  All rights reserved - CONFIDENTIAL.
 */
#ifndef HIGHPASSFILTER_PRM_H_
#define HIGHPASSFILTER_PRM_H_

#include "DistributionUtils_prv.h"

//=====================================================================================================================
//-------------------------------------- PUBLIC (Extern Variables, Constants & Defines) -------------------------------
//=====================================================================================================================

#define GET_HIGH_PASS_FILTER_SETUP(index)	HIGH_PASS_FILTER_SETUP_POINTER(index)

// Maximum number of second-order sections
#define HP_FILTER_MAX_NUM_SOS_SECTIONS	5

// List of High Pass Filter implemented
// Each filter is used for a specific rpm range.
// NOTE: all filters must be running throughout the entire distribution ramp.
typedef enum
{
    HPF_1 = 0,
    HPF_2,
    NUM_OF_HIGH_PASS_FILTERS
} HIGH_PASS_FILTER_TYPE;

//=====================================================================================================================
//-------------------------------------- PUBLIC (Function Prototypes) -------------------------------------------------
//=====================================================================================================================


#endif /* HIGHPASSFILTER_PRM_H_ */
