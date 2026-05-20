/**
 *  @file       
 *
 *  @brief      BandLimiter module for limiting the bandwidth of signals using digital filters
 *
 *  @details    Digital signals are characterized by their frequency content. In general, a digital signal's maximum
 *              bandwidth (frequency content) is half its sample rate. When downsampling (moving from a high sample rate to
 *              a lower one), it is necessary to ensure the source signal (with high sample rate) is band-limited to half the
 *              destination sample rate (low sample rate). That is, it must have all frequency content above half the destination
 *              sample rate removed before downsampling. This is to avoid a type of corruption in the data called aliasing.
 *              Aliasing occurs when a signal is sampled at less than the minimum rate (called the Nyquist rate) to capture all
 *              the frequency content in the signal. The purpose of this module is to remove frequency content using digital
 *              low-pass filters so that signals may be safely downsampled without aliasing.
 *
 *  @section    Applicable_Documents
 *					List here all the applicable documents if needed. <tr>	
 *
 *  $Header: BandLimiter.c 1.1 2016/02/02 13:40:32EST Guilherme Bencke Teixeira da Silva (BENCKG1) Exp  $
 *
 *  @copyright  Copyright 2015-$Date: 2016/02/02 13:40:32EST $. Whirlpool Corporation. All rights reserved - CONFIDENTIAL
 */
//---------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------


//-------------------------------------- Include Files ----------------------------------------------------------------
#include "C_Extensions.h"
#include <math.h>
#include "BandLimiter.h" 
#include "DigitalFilter.h"
#include "BandLimiter_prv.h"
//-------------------------------------- PUBLIC (Variables) -----------------------------------------------------------

//-------------------------------------- PRIVATE (Variables, Constants & Defines) -------------------------------------
//! Band specification structure. Comprised of arrays whose elements correspond to the signals
//! in the Signal_List member.
typedef struct {
    const BANDLIMITER_SIGNAL_TYPE   		*Signal_List;       				//!< Pointer to list of signals in band
    BOOL_TYPE                       		*Signal_Enable;     				//!< Signal enable array pointer. Each element enables/disables the
                                                        						//!< corresponding entry in the Signal_List.
    DIGITAL_FILTER_SOS_TYPE					filter;
    BIQUAD_STATE_TYPE						(*States)[BAND0_NUM_SECTIONS];		//!< Filter states storage
    float32                         		*Filtered_Signals;  				//!< Filter output array pointer. Each element holds the filtered output
                                                        						//!< for the corresponding element in the Signal_List.
    const uint8                    			Num_Signals;        				//!< Number of signals in band (length of all arrays in the structure)
} BAND_SPEC_TYPE;

//! Array number of elements macro.
#define NUM_ELEMENTS(array) (sizeof(array) / sizeof(array[0]))

CT_ASSERT ( NUM_ELEMENTS(BAND0_COEFFS) == BAND0_NUM_SECTIONS );

//! List of signals in Band 0
static const BANDLIMITER_SIGNAL_TYPE BAND0_SIGNAL_LIST[] =
{
    BANDLIMITER_SIGNAL_DRUM_TORQUE_NM,
    BANDLIMITER_SIGNAL_DRUM_SPEED_RAD_S,
    BANDLIMITER_SIGNAL_SIN_DRUM_ANGLE,
    BANDLIMITER_SIGNAL_COS_DRUM_ANGLE
};

#define BAND0_NUM_SIGNALS   NUM_ELEMENTS(BAND0_SIGNAL_LIST)     //!< Number of signals in Band 0

static BOOL_TYPE                Band0_Signal_Enable[BAND0_NUM_SIGNALS];     //!< Band 0 signal enable array
//! Band 0 filter storage array. First dimension is indexed by signal; second dimension is indexed by filter section.
static BIQUAD_STATE_TYPE        Band0_Filter_States[BAND0_NUM_SIGNALS][BAND0_NUM_SECTIONS];
static float32                  Band0_Filtered_Signals[BAND0_NUM_SIGNALS];  //!< Band 0 filter output storage array

//! Band specification list. This list contains the operating parameters for each band in the module.
static const BAND_SPEC_TYPE BAND_SPEC_LIST[] =
{
    {   // Band 0
        .Signal_List = BAND0_SIGNAL_LIST,
        .Signal_Enable = Band0_Signal_Enable,
        .Num_Signals = BAND0_NUM_SIGNALS,
		.filter = {
				.Coeffs = (DIGITAL_FILTER_BIQUAD_COEFF_TYPE*)(void*)BAND0_COEFFS,
				.Num_Sections = BAND0_NUM_SECTIONS
		},
		.States = Band0_Filter_States,
        .Filtered_Signals = Band0_Filtered_Signals
    }
};

//! Rotor angle, result of the integration of motor_speed * belt_ratio.
static float32 Rotor_Angle;
//! Previous motor speed, used for trapezoidal integration.
static float32 Previous_Omega;

CT_ASSERT ( NUM_ELEMENTS(BAND_SPEC_LIST) == BANDLIMITER_NUM_OF_BANDS );

//-------------------------------------- PRIVATE (Function Prototypes) ------------------------------------------------

static float32 FilterSignal(BANDLIMITER_SIGNAL_TYPE signal, DIGITAL_FILTER_SOS_TYPE filter, BIQUAD_STATE_TYPE *states);
static BOOL_TYPE IsSignalInBand(BANDLIMITER_BAND_TYPE band, BANDLIMITER_SIGNAL_TYPE signal);
static void IntegrateSpeed(void);

//=====================================================================================================================
//-------------------------------------- Public Functions -------------------------------------------------------------
//=====================================================================================================================

//---------------------------------------------------------------------------------------------------------------------
/**
 *  @brief      It Initializes the module BandLimiter and its variables
 *
 */
void BandLimiter__Initialize(void)
{
	Rotor_Angle = 0.0f;
	Previous_Omega = 0.0f;

    BandLimiter__ResetAll();
    BandLimiter__DisableAll();
    BandLimiter__EnableAll(); //TODO Remove this line
}

/**
 *  @brief      Get filtered signal.
 *
 *  @details    The core function of BandLimiter. Returns a signal which has been filtered according to the filter in
 *              #BAND_SPEC_LIST. The filtered signal is said to be "band-limited" to the passband of the filter. The band-
 *              limited signal may then be sampled at a frequency no smaller than the Nyquist rate, which is double
 *              the bandwidth of the signal. As long as the filtered signal is sampled with a rate above Nyquist, there
 *              will not be aliasing in the downsampled data. For example, a filter with significant attenuation above 50Hz
 *              will band-limit a signal to 50Hz. The filtered signal may then be sampled at any rate greater than or equal
 *              to 100Hz, which is the Nyquist rate for the filtered signal.
 *
 *  @param      band = Band containing signal to retreive.
 *  @param      signal = filtered signal to retrieve.
 *
 *  @return     The filtered signal. Units depend on the requested signal.
 */
float32 BandLimiter__GetSignal(BANDLIMITER_BAND_TYPE band, BANDLIMITER_SIGNAL_TYPE signal)
{
    uint8 i;
    float32 output = 0.0f;

    if(band >= BANDLIMITER_NUM_OF_BANDS)    // Invalid band selection by user (not in band list)
    {
        DEBUG_INVALID_BAND_GET_SELECTION(band);
    }
    else if(IsSignalInBand(band,signal) == FALSE)   // Queried signal not present in band
    {
        DEBUG_SIGNAL_TO_GET_NOT_IN_BAND(signal,band);
    }
    else    // requested signal is available
    {
        for(i = 0; i < BAND_SPEC_LIST[band].Num_Signals; i++)   // Loop over signals in band to find the requested signal
        {
            if(BAND_SPEC_LIST[band].Signal_List[i] == signal)
            {
                output = BAND_SPEC_LIST[band].Filtered_Signals[i];

                if(BAND_SPEC_LIST[band].Signal_Enable[i] == DISABLED)   // Requested signal is not enabled
                {
                    DEBUG_SIGNAL_TO_GET_NOT_ENABLED(signal,band);
                }

                break;
            }
        }
    }

    return output;
}

/**
 *  @brief      Reset filter states and filtered output for a signal.
 *
 *  @param      band = Band containing signal to reset.
 *  @param      signal = filtered signal to reset.
 */
void BandLimiter__ResetSignal(BANDLIMITER_BAND_TYPE band, BANDLIMITER_SIGNAL_TYPE signal)
{
    uint8 i;

    if(band >= BANDLIMITER_NUM_OF_BANDS)    // Invalid band selection by user (not in band list)
    {
        DEBUG_INVALID_BAND_RESET_SELECTION(band);
    }
    else if(IsSignalInBand(band,signal) ==  FALSE)   // Queried signal not present in band
    {
        DEBUG_SIGNAL_TO_RESET_NOT_IN_BAND(signal,band);
    }
    else        // requested signal is available
    {
        for(i = 0; i < BAND_SPEC_LIST[band].Num_Signals; i++)   // Loop over signals in band to find the requested signal
        {
            if(BAND_SPEC_LIST[band].Signal_List[i] == signal)
            {
                BAND_SPEC_LIST[band].Filtered_Signals[i] = 0.0f;     // Reset filtered signal
                DigitalFilter__ResetSOS(BAND_SPEC_LIST[band].States[i], BAND_SPEC_LIST[band].filter.Num_Sections);   // Reset filter states
                break;
            }
        }
    }
}

/**
 *  @brief      Reset filter states and filtered output for all signals in band.
 *
 *  @param      band = Band whose signals are to be reset
 */
void BandLimiter__ResetBand(BANDLIMITER_BAND_TYPE band)
{
    uint8 i;

    if(band >= BANDLIMITER_NUM_OF_BANDS)    // Invalid band selection by user (not in band list)
    {
        DEBUG_INVALID_BAND_RESET_SELECTION(band);
    }
    else    // Band selection is valid
    {
        for(i = 0; i < BAND_SPEC_LIST[band].Num_Signals; i++)   // Loop over signals in band
        {
            BAND_SPEC_LIST[band].Filtered_Signals[i] = 0.0f;     // Reset filtered signal
            DigitalFilter__ResetSOS(BAND_SPEC_LIST[band].States[i], BAND_SPEC_LIST[band].filter.Num_Sections);   // Reset filter states
        }
    }
}

/**
 *  @brief      Reset filter states and filtered output for all signals in all bands in module
 */
void BandLimiter__ResetAll(void)
{
    BANDLIMITER_BAND_TYPE i;

    for(i = (BANDLIMITER_BAND_TYPE)0; i < BANDLIMITER_NUM_OF_BANDS; i++)    // Loop over bands in module
    {
        BandLimiter__ResetBand(i);      // Reset all signals in band
    }
}

/**
 *  @brief      Enable filtering for specified signal.
 *
 *  @param      band = Band containing signal to enable.
 *  @param      signal = signal for which to enable filtering.
 */
void BandLimiter__EnableSignal(BANDLIMITER_BAND_TYPE band, BANDLIMITER_SIGNAL_TYPE signal)
{
    uint8 i;

    if(band >= BANDLIMITER_NUM_OF_BANDS)    // Invalid band selection by user (not in band list)
    {
        DEBUG_INVALID_BAND_ENABLE_SELECTION(band);
    }
    else if(IsSignalInBand(band,signal) ==  FALSE)  // Queried signal not present in band
    {
        DEBUG_SIGNAL_TO_ENABLE_NOT_IN_BAND(signal,band);
    }
    else        // requested signal is available
    {
        for(i = 0; i < BAND_SPEC_LIST[band].Num_Signals; i++)   // Loop over signals in band to find the requested signal
        {
            if(BAND_SPEC_LIST[band].Signal_List[i] == signal)
            {
                BAND_SPEC_LIST[band].Signal_Enable[i] = TRUE;   // enable signal
                break;
            }
        }
    }
}

/**
 *  @brief      Enable filtering for all signals in specified band.
 *
 *  @param      band = Band whose signals are to be enabled
 */
void BandLimiter__EnableBand(BANDLIMITER_BAND_TYPE band)
{
    uint8 i;

    if(band >= BANDLIMITER_NUM_OF_BANDS)    // Invalid band selection by user (not in band list)
    {
        DEBUG_INVALID_BAND_ENABLE_SELECTION(band);
    }
    else
    {
        for(i = 0; i < BAND_SPEC_LIST[band].Num_Signals; i++)   // Loop over signals in band
        {
            BAND_SPEC_LIST[band].Signal_Enable[i] = TRUE;   // enable signal
        }
    }
}

/**
 *  @brief      Enable filtering for all signals in all bands in module
 */
void BandLimiter__EnableAll(void)
{
    BANDLIMITER_BAND_TYPE i;

    for(i = (BANDLIMITER_BAND_TYPE)0; i < BANDLIMITER_NUM_OF_BANDS; i++)    // Loop over bands in module
    {
        BandLimiter__EnableBand(i);     // Enable all signals in band
    }
}

/**
 *  @brief      Disable filtering for specified signal.
 *
 *  @param      band = Band containing signal to disable.
 *  @param      signal = signal for which to disable filtering.
 */
void BandLimiter__DisableSignal(BANDLIMITER_BAND_TYPE band, BANDLIMITER_SIGNAL_TYPE signal)
{
    uint8 i;

    if(band >= BANDLIMITER_NUM_OF_BANDS)    // Invalid band selection by user (not in band list)
    {
        DEBUG_INVALID_BAND_DISABLE_SELECTION(band);
    }
    else if(IsSignalInBand(band,signal) ==  FALSE)  // Queried signal not present in band
    {
        DEBUG_SIGNAL_TO_DISABLE_NOT_IN_BAND(signal,band);
    }
    else        // requested signal is available
    {
        for(i = 0; i < BAND_SPEC_LIST[band].Num_Signals; i++)   // Loop over signals in band to find the requested signal
        {
            if(BAND_SPEC_LIST[band].Signal_List[i] == signal)
            {
                BAND_SPEC_LIST[band].Signal_Enable[i] = FALSE;  // disable signal
                break;
            }
        }
    }
}

/**
 *  @brief      Disable filtering for all signals in specified band.
 *
 *  @param      band = Band whose signals are to be disabled
 */
void BandLimiter__DisableBand(BANDLIMITER_BAND_TYPE band)
{
    uint8 i;

    if(band >= BANDLIMITER_NUM_OF_BANDS)    // Invalid band selection by user (not in band list)
    {
        DEBUG_INVALID_BAND_DISABLE_SELECTION(band);
    }
    else
    {
        for(i = 0; i < BAND_SPEC_LIST[band].Num_Signals; i++)   // Loop over signals in band
        {
            BAND_SPEC_LIST[band].Signal_Enable[i] = FALSE;  // disable signal
        }
    }
}

/**
 *  @brief      Disable filtering for all signals in all bands in module
 */
void BandLimiter__DisableAll(void)
{
    BANDLIMITER_BAND_TYPE i;

    for(i = (BANDLIMITER_BAND_TYPE)0; i < BANDLIMITER_NUM_OF_BANDS; i++)    // Loop over bands in module
    {
        BandLimiter__DisableBand(i);        // Disable all signals in band
    }
}

/**
 *  @brief      The bandlimiter module handler.
 *
 *  @details    Must be called at a consistent sample rate. Ideally, the sample rate should be high enough to avoid aliasing
 *              before applying the filter. That is, the sample rate should be high enough to capture all native frequency
 *              content in the input signals. For example, a signal updated at a rate of 8kHz such as the motor torque can
 *              have frequency content up to 4kHz. To sample this content without aliasing, a sample rate of 8kHz would be
 *              required. However, since the fastest slot available in vortex is 4kHz, 4kHz (250us) is the desired installation
 *              slot for the handler. Regardless of where the handler is installed, the filters configured for the module must be
 *              designed with its sample rate in mind. That is, filter coefficients assume a sample rate, and changing the
 *              handler time slot will change the filter characteristics.
 */
void BandLimiter__Handler(void)
{
    BANDLIMITER_BAND_TYPE i;
    BANDLIMITER_SIGNAL_TYPE j;

    IntegrateSpeed();

    for(i = (BANDLIMITER_BAND_TYPE)0; i < BANDLIMITER_NUM_OF_BANDS; i++)
    {
        for(j = (BANDLIMITER_SIGNAL_TYPE)0; j < BAND_SPEC_LIST[i].Num_Signals; j++)
        {
            if(BAND_SPEC_LIST[i].Signal_Enable[j] == TRUE)  // if the signal is enabled
            {
                BAND_SPEC_LIST[i].Filtered_Signals[j] = FilterSignal(BAND_SPEC_LIST[i].Signal_List[j], BAND_SPEC_LIST[i].filter, BAND_SPEC_LIST[i].States[j]);
            }
        }
    }
}

//=====================================================================================================================
//-------------------------------------- Private Functions ------------------------------------------------------------
//=====================================================================================================================

/**
 *  @brief      Filter input signal by filter
 *
 *  @param      signal = input signal to filter.
 *  @param      filter = digital filter to use for filtering.
 *
 *  @return     The filtered signal. Units depend on requested signal.
 */
static float32 FilterSignal(BANDLIMITER_SIGNAL_TYPE signal, DIGITAL_FILTER_SOS_TYPE filter, BIQUAD_STATE_TYPE *states)
{
    float32 input = 0.0f;

    switch(signal)
    {
        case BANDLIMITER_SIGNAL_DRUM_TORQUE_NM:
            input = INPUT_SIGNAL_DRUM_TORQUE_NM;
            break;
        case BANDLIMITER_SIGNAL_DRUM_SPEED_RAD_S:
            input = INPUT_SIGNAL_DRUM_SPEED_RAD_S;
            break;
        case BANDLIMITER_SIGNAL_SIN_DRUM_ANGLE:
            input = INPUT_SIGNAL_SIN_DRUM_ANGLE;
            break;
        case BANDLIMITER_SIGNAL_COS_DRUM_ANGLE:
            input = INPUT_SIGNAL_COS_DRUM_ANGLE;
            break;
        default:                                            // A case is not present for the input signal; code should
            DEBUG_CASE_MISSING_FOR_INPUT_SIGNAL(signal);    // never get here as there should be a case for each signal.
            break;
    }

    return DigitalFilter__DF2TSOS(input, filter, states);
}

/**
 *  @brief      Check whether signal is available in chosen band
 *
 *  @param      band = band to check against
 *  @param      signal = signal to query
 *
 *  @return     true/false indicating whether the signal is available
 */
static BOOL_TYPE IsSignalInBand(BANDLIMITER_BAND_TYPE band, BANDLIMITER_SIGNAL_TYPE signal)
{
    uint8 i;
    BOOL_TYPE signal_in_band = FALSE;

    for(i = 0; i < BAND_SPEC_LIST[band].Num_Signals; i++)
    {
        if(BAND_SPEC_LIST[band].Signal_List[i] == signal)
        {
            signal_in_band = TRUE;
            break;
        }
    }

    return signal_in_band;
}

/**
 *  @brief      Integrate the speed signal to calculate position
 */
static void IntegrateSpeed(void)
{
	Rotor_Angle = Rotor_Angle + BANDLIMITER_TS*0.5f*(Previous_Omega + INPUT_SIGNAL_DRUM_SPEED_RAD_S);
	if(Rotor_Angle > (2.0f*PI))
	{
		Rotor_Angle = Rotor_Angle - (2.0f*PI);
	}
	Previous_Omega = INPUT_SIGNAL_DRUM_SPEED_RAD_S;
}

