/**
 *  @file       
 *
 *  @brief      Digital Filter module.
 *
 *  @details    Generic digital filtering module with direct-form II and direct-form II transposed implementations for
 *              single-section filters and second-order section filters. The module stores no information, but expects
 *              storage to be provided by each caller. Caller also provides filter coefficients and filter order or
 *              number of sections. Single-section filters are susceptible to numerical precision and roundoff issues,
 *              especially for high sample rates, low bandwidth, and high filter order. In these situations, it is
 *              better to utilize a second-order sections implementation. Second-order section filters are recommended
 *              for filter orders above 3.
 *
 *  @section    Applicable_Documents
 *					List here all the applicable documents if needed. <tr>	
 *
 *  $Header: DigitalFilter.c 1.1 2016/02/02 14:39:20EST Guilherme Bencke Teixeira da Silva (BENCKG1) Exp  $
 *
 *  @copyright  Copyright 2015-$Date: 2016/02/02 14:39:20EST $. Whirlpool Corporation. All rights reserved - CONFIDENTIAL
 */
//---------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------


//-------------------------------------- Include Files ----------------------------------------------------------------
#include "C_Extensions.h"
#include "DigitalFilter.h" 
//-------------------------------------- PUBLIC (Variables) -----------------------------------------------------------

//-------------------------------------- PRIVATE (Variables, Constants & Defines) -------------------------------------

//-------------------------------------- PRIVATE (Function Prototypes) ------------------------------------------------

//=====================================================================================================================
//-------------------------------------- Public Functions -------------------------------------------------------------
//=====================================================================================================================

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Direct-form II single-section digital filtering routine
 *
 * @details Filters the input data by the provided filter. Direct-form II implementation. The leading denominator
 * coefficient is assumed to be normalized to 1. Single-section filters are susceptible to numerical precision and
 * roundoff issues, especially for high sample rates, low bandwidth, and high filter order. In these situations, it is
 * better to utilize a second-order sections implementation.
 * In the figure below, z^-1 (z-inverse) represents a delay, the bi's represent numerator coefficients, and the ai's
 * represent denominator coefficients. A filter state exists for each delay. n is the filter order. The state array is
 * arranged with element [0] corresponding to the uppermost delay in the diagram (smallest delay), and element [n]
 * corresponding to the bottom delay in the diagram (largest delay).
 *
 *       u  ----> ( - ) ------------o-----|b0>--> ( + ) ------> y
 *                  ^               |               ^
 *                  |               V               |
 *                  |            | z^-1 |           |
 *                  |               |               |
 *                ( + ) <--<a0|-----o-----|b1>--> ( + )
 *                  ^               |               ^
 *                  |               V               |
 *                  |            | z^-1 |           |
 *                  |               |               |
 *                ( + ) <--<a1|-----o-----|b2>--> ( + )
 *                  ^               |               ^
 *                  |                               |
 *                                  .
 *                                  .
 *                                  .
 *
 *                                  |
 *                                  V
 *                  ^            | z^-1 |           ^
 *                  |               |               |
 *                  '------<an|-----o-----|bn+1>----'
 *
 *
 * @param u = input signal to be filtered
 * @param filter = filter object by which to filter the input signal
 *
 * @return The filtered output
 */
#pragma optimize = none // Optimizations disabled to avoid faulty behavior with IAR C/C++ Compiler V2.60.5.1930 for Renesas RX
DIGITAL_FILTER_SIGNAL_DATA_TYPE DigitalFilter__DF2(DIGITAL_FILTER_SIGNAL_DATA_TYPE u, DIGITAL_FILTER_SINGLE_SECTION_TYPE filter, DIGITAL_FILTER_SIGNAL_DATA_TYPE *states)
{
    sint8 i;
    DIGITAL_FILTER_SIGNAL_DATA_TYPE y = 0.0f;

    for(i = (filter.Order-1); i >= 0; i--)      // loop over filter states from bottom of diagram to top
    {                                           // so that the states may be updated in sequence
        u -= filter.Den[i] * states[i];
        y += filter.Num[i+1] * states[i];

        if(i > 0)           // Do not try to fill the first state in this way
        {
        	states[i] = states[i-1];  // shift filter states to the next higher delay
        }
    }

    y += filter.Num[0] * u;
    states[0] = u;           // fill first state

    return y;
}

/**
 * @brief Direct-form II second-order-sections digital filtering routine
 *
 * @details Filters the input data by the provided filter. Direct-form II cascaded second-order sections implementation.
 * The leading denominator and numerator coefficients for each section are assumed to be normalized to 1. Each section's
 * output feeds the subsequent section's input.
 * In the figure below, z^-1 (z-inverse) represents a delay, the bi's represent numerator coefficients, and the ai's
 * represent denominator coefficients. g represents the section's gain. Each section has 2 states, corresponding to the 2
 * delays in each section. The 2 elements inside the biquad array for each section are arranged with element [0] corresponding
 * to the uppermost delay in the diagram (smallest delay), and element [1] corresponding to the bottom delay in the diagram
 * (largest delay).
 *
 *                             Section 0                                                     Section n
 *  u ---|g>--> ( - ) ------------o-----------> ( + ) ------> . . . ---|g>--> ( - ) ------------o-----------> ( + ) ----> y
 *                ^               |               ^                             ^               |               ^
 *                |               V               |                             |               V               |
 *                |            | z^-1 |           |                             |            | z^-1 |           |
 *                |               |               |                             |               |               |
 *              ( + ) <--<a0|-----o-----|b0>--> ( + )                         ( + ) <--<a0|-----o-----|b0>--> ( + )
 *                ^               |               ^                             ^               |               ^
 *                |               V               |                             |               V               |
 *                |            | z^-1 |           |                             |            | z^-1 |           |
 *                |               |               |                             |               |               |
 *                '------<a1|-----o-----|b1>------'                             '------<a1|-----o-----|b1>------'
 *
 *
 * @param u = input signal to be filtered
 * @param filter = filter object by which to filter the input signal
 *
 * @return The filtered output
 */
#pragma optimize = none // Optimizations disabled to avoid faulty behavior with IAR C/C++ Compiler V2.60.5.1930 for Renesas RX
DIGITAL_FILTER_SIGNAL_DATA_TYPE DigitalFilter__DF2SOS(DIGITAL_FILTER_SIGNAL_DATA_TYPE u, DIGITAL_FILTER_SOS_TYPE filter, BIQUAD_STATE_TYPE *states)
{
    uint8 i;
    sint8 j;
    DIGITAL_FILTER_SIGNAL_DATA_TYPE y = 0.0f;

    for(i = 0; i < filter.Num_Sections; i++)        // Loop over the filter sections
    {
        u *= filter.Coeffs[i].Gain;                 // Apply section gain to section input
        y = 0;
        for(j = (BIQUAD_FILTER_ORDER - 1); j >= 0; j--)     // Loop over filter states from bottom of diagram (largest delay)
        {                                                   // to top (least delay) so that the states may be updated in sequence
            u -= filter.Coeffs[i].Den[j] * states[i][j];
            y += filter.Coeffs[i].Num[j] * states[i][j];

            if(j > 0)       // Do not try to fill the first state in this way
            {
            	states[i][j] = states[i][j-1];
            }
        }

        y += u;     // This is the final adder node. Leading numerator coefficient has been normalized to 1.
        states[i][0] = u;        // fill first state

        u = y;      // Set input in preparation for filtering in the next section
    }

    return y;
}

/**
 * @brief Direct-form II transposed single-section digital filtering routine
 *
 * @details Filters the input data by the provided filter. Direct-form II transposed implementation. The leading denominator
 * coefficient is assumed to be normalized to 1. Single-section filters are susceptible to numerical precision and
 * roundoff issues, especially for high sample rates, low bandwidth, and high order. In these situations, it is
 * better to utilize a second-order sections implementation.
 * In the figure below, z^-1 (z-inverse) represents a delay, the bi's represent numerator coefficients, and the ai's
 * represent denominator coefficients. A filter state exists for each delay. n is the filter order. The state array is
 * arranged with element [0] corresponding to the uppermost delay in the diagram (largest delay), and element [n]
 * corresponding to the bottom delay in the diagram (smallest delay).
 *
 *       u  --------o-----|b0>--->( + )-------------o-------> y
 *                  |               ^               |
 *                  |               |               |
 *                  |            | z^-1 |           |
 *                  |               ^               |
 *                  |               |               |
 *                  o-----|b1>--> ( + ) <--<a0|-----o
 *                  |               ^               |
 *                  |               |               |
 *                  |            | z^-1 |           |
 *                  |               ^               |
 *                  |               |               |
 *                  o-----|b2>--> ( + ) <--<a1|-----o
 *                  |               ^               |
 *                                  |
 *
 *                                  .
 *                                  .
 *                                  .
 *
 *                                  ^
 *                                  |
 *                  |            | z^-1 |           |
 *                  |               ^               |
 *                  |               |               |
 *                  o---|bn+1>--> ( + ) <--<an|-----o
 *
 * @param u = input signal to be filtered
 * @param filter = filter object by which to filter the input signal
 *
 * @return The filtered output
 */
#pragma optimize = none     // Optimizations disabled to avoid faulty behavior with IAR C/C++ Compiler V2.60.5.1930 for Renesas RX
DIGITAL_FILTER_SIGNAL_DATA_TYPE DigitalFilter__DF2T(DIGITAL_FILTER_SIGNAL_DATA_TYPE u, DIGITAL_FILTER_SINGLE_SECTION_TYPE filter, DIGITAL_FILTER_SIGNAL_DATA_TYPE *states)
{
    DIGITAL_FILTER_SIGNAL_DATA_TYPE y = 0.0f;
    uint8 i;

    y = u * filter.Num[0] + states[0];

    for ( i = 0; i < filter.Order; i++)             // Loop over filter states, from top of diagram (largest delay) to bottom
    {                                               // (least delay) so that the states may be updated in sequence
        if(i < (filter.Order - 1) )         // fill all but the
        {
        	states[i] = states[i+1] + u * filter.Num[i+1] - y * filter.Den[i];
        }
        else
        {
        	states[i] = u * filter.Num[i+1] - y * filter.Den[i];
        }
    }
    return y;
}

/**
 * @brief Direct-form II transposed second-order-sections digital filtering routine
 *
 * @details Filters the input data by the provided filter. Direct-form II transposed cascaded second-order sections implementation.
 * The leading denominator and numerator coefficients for each section are assumed to be normalized to 1. Each section's
 * output feeds the subsequent section's input.
 * In the figure below, z^-1 (z-inverse) represents a delay, the bi's represent numerator coefficients, and the ai's
 * represent denominator coefficients. g represents the section's gain. Each section has 2 states, corresponding to the 2
 * delays in each section. The 2 elements inside the biquad array for each section are arranged with element [0] corresponding
 * to the uppermost delay in the diagram (largest delay), and element [1] corresponding to the bottom delay in the diagram
 * (smallest delay).
 *
 *                            Section 0                                                  Section n
 *    u ---|g>---o------------>( + )-------------o------->  . . .  ---|g>---o------------>( + )-------------o-------> y
 *               |               ^               |                          |               ^               |
 *               |               |               |                          |               |               |
 *               |            | z^-1 |           |                          |            | z^-1 |           |
 *               |               ^               |                          |               ^               |
 *               |               |               |                          |               |               |
 *               o-----|b0>--> ( + ) <--<a0|-----o                          o-----|b0>--> ( + ) <--<a0|-----o
 *               |               ^               |                          |               ^               |
 *               |               |               |                          |               |               |
 *               |            | z^-1 |           |                          |            | z^-1 |           |
 *               |               ^               |                          |               ^               |
 *               |               |               |                          |               |               |
 *               o-----|b1>--> ( + ) <--<a1|-----o                          o-----|b1>--> ( + ) <--<a1|-----o
 *
 * @param u = input signal to be filtered
 * @param filter = filter object by which to filter the input signal
 *
 * @return The filtered output
 */
//#pragma optimize = none // Optimizations disabled to avoid faulty behavior with IAR C/C++ Compiler V2.60.5.1930 for Renesas RX
DIGITAL_FILTER_SIGNAL_DATA_TYPE DigitalFilter__DF2TSOS(DIGITAL_FILTER_SIGNAL_DATA_TYPE u, DIGITAL_FILTER_SOS_TYPE filter, BIQUAD_STATE_TYPE *states)
{
    uint8 i,j;
    DIGITAL_FILTER_SIGNAL_DATA_TYPE y = 0.0f;

    for(i = 0; i < filter.Num_Sections; i++)
    {
        u *= filter.Coeffs[i].Gain;

        y = u + states[i][0];

        for ( j = 0; j < BIQUAD_FILTER_ORDER; j++)            // loop over delays
        {
            if(j < (BIQUAD_FILTER_ORDER - 1) )
            {
            	states[i][j] = states[i][j+1] + u * filter.Coeffs[i].Num[j] - y * filter.Coeffs[i].Den[j];
            }
            else
            {
            	states[i][j] = u * filter.Coeffs[i].Num[j] - y * filter.Coeffs[i].Den[j];
            }
        }
        u = y;
    }

    return y;
}

/**
 *  @brief  Reset States of second-order-sections digital filter
 *
 *  @param filter = filter whose states are to be reset
 *
 */
void  DigitalFilter__ResetSOS(BIQUAD_STATE_TYPE *states, uint8 num_sections)
{
    uint8 i,j;

    for(i = 0; i < num_sections; i++)
    {
        for(j = 0; j < BIQUAD_FILTER_ORDER; j++)
        {
        	states[i][j] = 0.0f;
        }
    }
}

/**
 *  @brief  Reset States of single-section digital filter
 *
 *  @param filter = filter whose states are to be reset
 *
 */
void  DigitalFilter__ResetSingleSection(DIGITAL_FILTER_SINGLE_SECTION_TYPE filter, DIGITAL_FILTER_SIGNAL_DATA_TYPE *states)
{
    uint8 i;

    for(i = 0; i < filter.Order; i++)
    {
    	states[i] = 0.0f;
    }
}


//=====================================================================================================================
//-------------------------------------- Private Functions ------------------------------------------------------------
//=====================================================================================================================


