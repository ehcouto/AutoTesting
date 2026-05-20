/**
 * @file
 * @brief       Reads and averages specific MCU sampled values.
 *
 * @details
 *
 * @copyright   Copyright 2016 Whirlpool Corporation.  All rights reserved - CONFIDENTIAL.
 */

//  --- Include Files -------------------------------------------------------------------------------------------------

// -- This Module --
#include "SystemConfig.h"
#include "AnalysisAverage.h"

#ifndef API039MOTIONCTRL_FEATURE
	#define API039MOTIONCTRL_FEATURE DISABLED
#endif

#if (API039MOTIONCTRL_FEATURE == ENABLED)
// -- Other Modules --
#include "Mci.h"
#include "ParamEst.h"
#include "SensorsAlgorithms.h"
#include <string.h>


//  --- Private Declarations ------------------------------------------------------------------------------------------


// -- Private Constant Declarations --


// -- Private Enumerated Constant Declarations --

//! Enumerated list of values that can be averaged.
typedef enum AA_CHANNEL_ENUM
{
    AA_CHANNEL_UB_TORQUE_INDEX      = 0,
    AA_CHANNEL_UB_G                 = 1,
    AA_CHANNEL_END
} AA_CHANNEL_TYPE;

//! Data element ID for publishing calculated results.
typedef enum AA_DATA_ELEMENT_ID_ENUM
{
    AA_DATA_ELEMENT_SAMPLE          = 0,
    AA_DATA_ELEMENT_AVERAGE         = 1
} AA_DATA_ELEMENT_ID_TYPE;


// -- Private Structure Type Declarations --

typedef union AA_SOURCE_UNION
{
    uint8 Whole;
    struct AA_SOURCE_STRUCT
    {
        AA_CHANNEL_TYPE Channel     : 6;
        uint8 Publish_Sample        : 1;
        uint8 Publish_Average       : 1;
    } Field;
} AA_SOURCE_TYPE;

//! Variables used by the AnalysisAverage module.
typedef struct ANALYSIS_AVERAGE_DATA_STRUCT
{
    //! The sum of the samples taken so far in the current average calculation.
    uint32 Sum;

    //! The most recent calculated average of the samples.
    uint16 Result;

    //! The most recent sampled value.
    uint16 Sample;

    //! The number of samples taken so far in the average calculation.
    uint8 Sample_Count;

    //! The number of samples to average for each average calculation.
    uint8 Samples_To_Average;

    //! The type of data to sample and the type of publication to produce.
    //! Refer to the AA_SOURCE_TYPE for details.
    AA_SOURCE_TYPE Source;

    //! TRUE if the module is actively handling source samples.
    BOOL_TYPE Enabled               : 1;

    //! TRUE if the module is skipping samples before starting the averaging.
    BOOL_TYPE Skipping              : 1;

    //! TRUE if the calculated average is ready for publication.
    BOOL_TYPE Average_Ready         : 1;

    //! TRUE if the sampled value is ready for publication.
    BOOL_TYPE Sample_Ready          : 1;
} ANALYSIS_AVERAGE_DATA_TYPE;


//  --- Private Definitions -------------------------------------------------------------------------------------------


// -- Private Variable Definitions --

//! Define variables used by the AnalysisAverage module.
static ANALYSIS_AVERAGE_DATA_TYPE Analysis_Average;


//  --- Private Function Prototypes -----------------------------------------------------------------------------------


//=====================================================================================================================
//  --- Public Functions ----------------------------------------------------------------------------------------------
//=====================================================================================================================


//---------------------------------------------------------------------------------------------------------------------
/**
 * Initialize the AnalysisAverage module variables.
 */
void AnalysisAverage__Initialize(void)
{
    memset(&Analysis_Average, 0, sizeof(Analysis_Average));
}


//---------------------------------------------------------------------------------------------------------------------
/**
 * Check for a new sample and add it to the average calculation if found.
 * Prepare for publication if all the desired samples were received.
 */
void AnalysisAverage__Background5ms(void)
{
    uint16 new_sample;

    if (Analysis_Average.Enabled == TRUE)
    {
        // Measure the value based on the source and avoid reusing previously processed values.
        BOOL_TYPE new_sample_found = FALSE;
        switch (Analysis_Average.Source.Field.Channel)
        {
            case AA_CHANNEL_UB_TORQUE_INDEX:
                new_sample = (uint16)SensorsAlgorithms__GetAnalog(SENSORS_AI_BALANCE);
                // TODO: Figure out a better way to know that there is a new unbalance torque index sample.
                if (Analysis_Average.Sample != new_sample)
                {
                    Analysis_Average.Sample = new_sample;
                    Analysis_Average.Sample_Ready = TRUE;
                    new_sample_found = TRUE;
                }
                break;

            case AA_CHANNEL_UB_G:
                new_sample = (uint16)ParamEst__GetParameterValue(PARAM_EST_UBMAG);
                // TODO: Figure out a better way to know that there is a new unbalance magnitude sample.
                if (Analysis_Average.Sample != new_sample)
                {
                    Analysis_Average.Sample = new_sample;
                    Analysis_Average.Sample_Ready = TRUE;
                    new_sample_found = TRUE;
                }
                break;

            default:
                break;
        }

        // Accumulate individual samples and calculate the average.
        if (Analysis_Average.Samples_To_Average > 0)
        {
            if (Analysis_Average.Skipping == TRUE)
            {
                if (new_sample_found == TRUE)
                {
                    Analysis_Average.Sample_Count--;    // Count down while skipping samples.
                    if (Analysis_Average.Sample_Count == 0)
                    {
                        // Done skipping samples
                        Analysis_Average.Skipping = FALSE;
                    }
                }
            }
            else
            {
                if ((new_sample_found == TRUE) &&
                    (Analysis_Average.Sample_Count < Analysis_Average.Samples_To_Average))
                {
                    Analysis_Average.Sum += Analysis_Average.Sample;
                    Analysis_Average.Sample_Count++;    // Count up while averaging samples.
                }
                if ((Analysis_Average.Average_Ready == FALSE) &&
                    (Analysis_Average.Sample_Count >= Analysis_Average.Samples_To_Average))
                {
                    Analysis_Average.Result = Analysis_Average.Sum / Analysis_Average.Sample_Count;
                    Analysis_Average.Sum = 0;
                    Analysis_Average.Sample_Count = 0;
                    Analysis_Average.Average_Ready = TRUE;
                }
            }
        }
    }
}


//---------------------------------------------------------------------------------------------------------------------
/**
 * Background Analysis API Function: Execute a background analysis command.
 * @param command = The command that the background analysis should execute.
 *                  A command value of 0 indicates that the background analysis should disable itself.
 * @param params = A pointer to the parameter bytes that are being passed along with the command.
 *                  The params memory is only valid during the function call.
 * @param size = The number of parameter bytes that are being passed along with the command.
 * @retval ANALYSIS_COMMAND_RESPONSE_INACTIVE = The analysis is inactive. Do not call the background function.
 * @retval ANALYSIS_COMMAND_RESPONSE_ACTIVE = The analysis is active. Call the background function periodically.
 * @retval ANALYSIS_COMMAND_RESPONSE_INVALID = The command is not supported.
 */
ANALYSIS_COMMAND_RESPONSE_TYPE AnalysisAverage__DoCommand(uint8 command, uint8* params, uint8 size)
{
    ANALYSIS_COMMAND_RESPONSE_TYPE response = ANALYSIS_COMMAND_RESPONSE_INVALID;

    switch ((ANALYSIS_AVERAGE_COMMAND_TYPE)command)
    {
        case ANALYSIS_AVERAGE_COMMAND_DISABLE:
            response = ANALYSIS_COMMAND_RESPONSE_INACTIVE;
            break;

        case ANALYSIS_AVERAGE_COMMAND_ENABLE:
            if (size >= 3)
            {
                AA_SOURCE_TYPE source;
                source.Whole = params[0];
                if (source.Field.Channel < AA_CHANNEL_END)
                {
                    // Reset any previously active averaging.
                    memset(&Analysis_Average, 0, sizeof(Analysis_Average));
                    Analysis_Average.Source.Whole = source.Whole;

                    Analysis_Average.Sample_Count = params[1];
                    Analysis_Average.Skipping = (BOOL_TYPE)(Analysis_Average.Sample_Count > 0);

                    Analysis_Average.Samples_To_Average = params[2];
                    Analysis_Average.Enabled = TRUE;

                    response = ANALYSIS_COMMAND_RESPONSE_ACTIVE;
                }
            }
            break;

        default:
            break;
    }

    return response;
}


//---------------------------------------------------------------------------------------------------------------------
/**
 * Background Analysis API Function: Get the data elements associated with the background analysis.
 * @param first_data_element_id = The ID to use for data element #0. Data element #1 uses this value + 1, etc.
 * @param data_elements = Array of ANALYSIS_DATA_ELEMENT_TYPE structures. One for each data element.
 * @param count = The number of data elements that are being returned by this function.
 */
void AnalysisAverage__GetData(uint8 first_data_element_id, ANALYSIS_DATA_ELEMENT_TYPE* data_elements, uint8* count)
{
    uint8 result_count = 0;
    if ((Analysis_Average.Source.Field.Publish_Sample == TRUE) &&
        (Analysis_Average.Sample_Ready == TRUE))
    {
        data_elements[result_count].Id = first_data_element_id + AA_DATA_ELEMENT_SAMPLE;
        data_elements[result_count].Value_Hi_Byte = HIBYTE(Analysis_Average.Sample);
        data_elements[result_count].Value_Lo_Byte = LOBYTE(Analysis_Average.Sample);
        result_count++;
        Analysis_Average.Sample_Ready = FALSE;
    }
    if ((Analysis_Average.Source.Field.Publish_Average == TRUE) &&
        (Analysis_Average.Average_Ready == TRUE))
    {
        data_elements[result_count].Id = first_data_element_id + AA_DATA_ELEMENT_AVERAGE;
        data_elements[result_count].Value_Hi_Byte = HIBYTE(Analysis_Average.Result);
        data_elements[result_count].Value_Lo_Byte = LOBYTE(Analysis_Average.Result);
        result_count++;
        Analysis_Average.Average_Ready = FALSE;
    }
    *count = result_count;
}
#endif

//=====================================================================================================================
//  --- Private Functions ---------------------------------------------------------------------------------------------
//=====================================================================================================================
