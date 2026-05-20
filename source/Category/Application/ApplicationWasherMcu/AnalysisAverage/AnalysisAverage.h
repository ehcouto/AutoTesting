/**
 * @file
 * @brief       Public interface to the AnalysisAverage module.
 *
 * @details     Refer to the AnalysisAverage.c source file for more detailed information.
 *
 * @copyright   Copyright 2016 Whirlpool Corporation.  All rights reserved - CONFIDENTIAL.
 */

#ifndef ANALYSISAVERAGE_H_
#define ANALYSISAVERAGE_H_


//  --- Include Files -------------------------------------------------------------------------------------------------

// -- This Module --

// -- Other Modules --
#if (API039MOTIONCTRL_FEATURE == ENABLED)
#include "AnalysisInterface.h"


//=====================================================================================================================
//  --- Public Properties ---------------------------------------------------------------------------------------------
//=====================================================================================================================


// -- Public Constant Declarations --


// -- Public Enumerated Constant Declarations --

//! Commands accepted by the AnalysisMotorServer module.
typedef enum ANALYSIS_AVERAGE_COMMAND_ENUM
{
    ANALYSIS_AVERAGE_COMMAND_DISABLE = 0,
    ANALYSIS_AVERAGE_COMMAND_ENABLE,
    ANALYSIS_AVERAGE_COMMAND_CLEAR_ALL_ERRORS
} ANALYSIS_AVERAGE_COMMAND_TYPE;


// -- Public Type Declarations --


//=====================================================================================================================
//  --- Public Methods ------------------------------------------------------------------------------------------------
//=====================================================================================================================


void AnalysisAverage__Initialize(void);
void AnalysisAverage__Background5ms(void);
ANALYSIS_COMMAND_RESPONSE_TYPE AnalysisAverage__DoCommand(uint8 command, uint8* params, uint8 size);
void AnalysisAverage__GetData(uint8 first_data_element_id, ANALYSIS_DATA_ELEMENT_TYPE* data_elements, uint8* count);
#endif

#endif      // ANALYSISAVERAGE_H_
