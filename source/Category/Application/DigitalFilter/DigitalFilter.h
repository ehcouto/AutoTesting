/**
 *  @file       
 *
 *  @brief      Digital Filter module.
 *
 *  @section    Applicable_Documents
 *					List here all the applicable documents if needed. <tr>	
 *
 *  $Header: DigitalFilter.h 1.1 2016/02/02 14:39:22EST Guilherme Bencke Teixeira da Silva (BENCKG1) Exp  $
 *
 *  @copyright  Copyright 2015-$Date: 2016/02/02 14:39:22EST $. Whirlpool Corporation. All rights reserved - CONFIDENTIAL
 */
//---------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------
#ifndef DIGITALFILTER_H_
#define DIGITALFILTER_H_

#include "C_Extensions.h"

//=====================================================================================================================
//-------------------------------------- PUBLIC (Extern Variables, Constants & Defines) -------------------------------
//=====================================================================================================================
//! Order of biquadratic (second-order) filter.
#define BIQUAD_FILTER_ORDER 2

//! Input/output signal data type for digital filters.
typedef float32 DIGITAL_FILTER_SIGNAL_DATA_TYPE;

//! Second-order filter section (biquad) coefficient structure. Used to store the coefficients for a single section of a
//! #DIGITAL_FILTER_SOS_TYPE filter.
typedef PACKED struct
{
    float32 Num[BIQUAD_FILTER_ORDER];               //!< Numerator coefficient array. Leading coefficient assumed to be 1.
    float32 Den[BIQUAD_FILTER_ORDER];               //!< Denominator coefficient array. Leading coefficient assumed to be 1.
    float32 Gain;                                   //!< Gain applied to input of the section.
} DIGITAL_FILTER_BIQUAD_COEFF_TYPE;

//! Second-order filter section (biquad) state type. Used to store the states for a single section of a #DIGITAL_FILTER_SOS_TYPE
//! filter.
typedef DIGITAL_FILTER_SIGNAL_DATA_TYPE BIQUAD_STATE_TYPE[BIQUAD_FILTER_ORDER];

//! Second-order-sections filter specification structure.
typedef struct
{
    DIGITAL_FILTER_BIQUAD_COEFF_TYPE    *Coeffs;        //!< Pointer to filter coefficient structure, possibly as the first element
                                                        //!< of an array. Each element in such an array holds the coefficient structure
                                                        //!<  for a single section of the filter. Array must have Num_Sections elements.
    uint8                               Num_Sections;   //!< Number of second-order sections in the filter.
} DIGITAL_FILTER_SOS_TYPE;

//! Single-section filter specification structure.
typedef struct
{
    //! Pointer to Filter Numerator Coefficients. Numerator coefficient array should have (Order + 1) elements.
    float32     *Num;

    //! Pointer to Filter Denominator Coefficients. Denominator coefficient array should have Order elements.
    float32     *Den;
    uint8       Order;              	//!< Order of filter (number of delays/length of storage)
} DIGITAL_FILTER_SINGLE_SECTION_TYPE;


//=====================================================================================================================
//-------------------------------------- PUBLIC (Function Prototypes) -------------------------------------------------
//=====================================================================================================================
DIGITAL_FILTER_SIGNAL_DATA_TYPE DigitalFilter__DF2(DIGITAL_FILTER_SIGNAL_DATA_TYPE u, DIGITAL_FILTER_SINGLE_SECTION_TYPE filter, DIGITAL_FILTER_SIGNAL_DATA_TYPE *states);
DIGITAL_FILTER_SIGNAL_DATA_TYPE DigitalFilter__DF2SOS(DIGITAL_FILTER_SIGNAL_DATA_TYPE u, DIGITAL_FILTER_SOS_TYPE filter, BIQUAD_STATE_TYPE *states);
DIGITAL_FILTER_SIGNAL_DATA_TYPE DigitalFilter__DF2T(DIGITAL_FILTER_SIGNAL_DATA_TYPE u, DIGITAL_FILTER_SINGLE_SECTION_TYPE filter, DIGITAL_FILTER_SIGNAL_DATA_TYPE *states);
DIGITAL_FILTER_SIGNAL_DATA_TYPE DigitalFilter__DF2TSOS(DIGITAL_FILTER_SIGNAL_DATA_TYPE u, DIGITAL_FILTER_SOS_TYPE filter, BIQUAD_STATE_TYPE *states);
void  DigitalFilter__ResetSOS(BIQUAD_STATE_TYPE *states, uint8 num_sections);
void  DigitalFilter__ResetSingleSection(DIGITAL_FILTER_SINGLE_SECTION_TYPE filter, DIGITAL_FILTER_SIGNAL_DATA_TYPE *states);
#endif // DIGITALFILTER_H_


