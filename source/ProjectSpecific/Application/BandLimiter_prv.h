/**
 *  @file       
 *
 *  @brief      Private parameters for the BandLimiter module.
 *
 *  $Header: BandLimiter_prv.h 1.1 2016/02/02 11:45:34EST Guilherme Bencke Teixeira da Silva (BENCKG1) Exp  $
 *
 *  @copyright  Copyright 2015-$Date: 2016/02/02 11:45:34EST $. Whirlpool Corporation. All rights reserved - CONFIDENTIAL
 */
//---------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------
#ifndef BANDLIMITER_PRV_H_
#define BANDLIMITER_PRV_H_

//=====================================================================================================================
//-------------------------------------- PUBLIC (Extern Variables, Constants & Defines) -------------------------------
//=====================================================================================================================
#include "C_Extensions.h"
#include "MciSensorsWm.h"
#include "Log.h"

//! Number of second-order filter sections in Band 0 filter
#define BAND0_NUM_SECTIONS 1

//! Filter coefficient array for Band 0. Must have #BAND0_NUM_SECTIONS elements.
// 2nd-order butterworth lowpass. Design parameters: Fs (sample freq) = 4kHz, Fc (Cutoff freq) = 30Hz
// Designed with the intention of filtering before downsampling to 100Hz. After passing through the filter, the signals
// will have minimal content above the 50Hz Nyquist frequency.
static const DIGITAL_FILTER_BIQUAD_COEFF_TYPE BAND0_COEFFS[] =
{
    {   // Section 0
        .Num = {2.0f, 1.0f},
        .Den = {-1.9333802461624146f,  0.93552893400192261f},
        .Gain = 0.00053716974798589945f
    }
};

#define BANDLIMITER_TS 					(float32)1.0f/4000.0f	//!< BandLimiter is installed in a 250us slot

#define DEG_TO_RAD(deg)                 ((deg) / 180.0f * PI)      	//!< Conversion from degrees to radians
#define RPM_TO_RAD_S(rpm)               ((rpm) / 30.0f * PI)		//!< Conversion from degrees to radians

//! The torque applied to the drum by the motor or transmission, in Newton-meters. Type: float (or will be promoted
//! internally to float)
#define INPUT_SIGNAL_DRUM_TORQUE_NM     ( (float32)MciSensorsWm__GetAnalog(MOTOR0, MCISENSORSWM_AI_LOAD_TORQUE_S16) / 65536.0f )
//! The speed of the drum, in rotations per minute. Type: float (or will be promoted internally to float)
#define INPUT_SIGNAL_DRUM_SPEED_RAD_S   (RPM_TO_RAD_S((float32)MciSensorsWm__GetAnalog(MOTOR0, MCISENSORSWM_AI_ACTUAL_SPEED_S16) / 65536.0f ))
//! The sine of the drum angular position. Type: float (or will be promoted internally to float)

#define INPUT_SIGNAL_SIN_DRUM_ANGLE     ( sinf(Rotor_Angle) )
//! The cosine of the drum angular position. Type: float (or will be promoted internally to float)
#define INPUT_SIGNAL_COS_DRUM_ANGLE     ( cosf(Rotor_Angle) )

//! list of module log messages
typedef enum
{
    MESSAGE_NONE = 0,              //!< MESSAGE_NONE
    INVALID_BAND_GET_SELECTION,    //!< INVALID_BAND_GET_SELECTION
    SIGNAL_TO_GET_NOT_IN_BAND,     //!< SIGNAL_TO_GET_NOT_IN_BAND
    SIGNAL_TO_GET_NOT_ENABLED,     //!< SIGNAL_TO_GET_NOT_ENABLED
    INVALID_BAND_RESET_SELECTION,  //!< INVALID_BAND_RESET_SELECTION
    SIGNAL_TO_RESET_NOT_IN_BAND,   //!< SIGNAL_TO_RESET_NOT_IN_BAND
    INVALID_BAND_ENABLE_SELECTION, //!< INVALID_BAND_ENABLE_SELECTION
    SIGNAL_TO_ENABLE_NOT_IN_BAND,  //!< SIGNAL_TO_ENABLE_NOT_IN_BAND
    INVALID_BAND_DISABLE_SELECTION,//!< INVALID_BAND_DISABLE_SELECTION
    SIGNAL_TO_DISABLE_NOT_IN_BAND, //!< SIGNAL_TO_DISABLE_NOT_IN_BAND
    CASE_MISSING_FOR_INPUT_SIGNAL  //!< CASE_MISSING_FOR_INPUT_SIGNAL
} MODULE_BANDLIMITER_LOG_MESSAGE_ID_TYPE;

//! Log module name for messages
#define MODULE_NAME MODULE_BANDLIMITER

/**
 *  @brief  The API BandLimiter__GetSignal was called with an invalid band selection
 *  @param  band = the invalid user-selected band
 */
#define DEBUG_INVALID_BAND_GET_SELECTION(band)  LOG_ADD_EXCEPTION(INVALID_BAND_GET_SELECTION, band)

/**
 *  @brief  The API BandLimiter__GetSignal was called with a signal not present in the selected band
 *  @param  signal = user-selected signal
 *  @param  band = user-selected band
 */
#define DEBUG_SIGNAL_TO_GET_NOT_IN_BAND(signal,band)    LOG_ADD_EXCEPTION(SIGNAL_TO_GET_NOT_IN_BAND, signal)

/**
 *  @brief  The API BandLimiter__GetSignal was called with a signal that is not enabled
 *  @param  signal = user-selected signal
 *  @param  band = user-selected band
 */
#define DEBUG_SIGNAL_TO_GET_NOT_ENABLED(signal,band)    LOG_ADD_EXCEPTION(SIGNAL_TO_GET_NOT_ENABLED, signal)

/**
 *  @brief  The API BandLimiter__ResetSignal or BandLimiter__ResetBand was called with an invalid band selection
 *  @param  band = the invalid user-selected band
 */
#define DEBUG_INVALID_BAND_RESET_SELECTION(band)    LOG_ADD_EXCEPTION(INVALID_BAND_RESET_SELECTION, band)

/**
 *  @brief  The API BandLimiter__ResetSignal was called with a signal not present in the selected band
 *  @param  signal = user-selected signal
 *  @param  band = user-selected band
 */
#define DEBUG_SIGNAL_TO_RESET_NOT_IN_BAND(signal,band)  LOG_ADD_EXCEPTION(SIGNAL_TO_RESET_NOT_IN_BAND, signal)

/**
 *  @brief  The API BandLimiter__EnableSignal or BandLimiter__EnableBand was called with an invalid band selection
 *  @param  band = the invalid user-selected band
 */
#define DEBUG_INVALID_BAND_ENABLE_SELECTION(band)   LOG_ADD_EXCEPTION(INVALID_BAND_ENABLE_SELECTION, band)

/**
 *  @brief  The API BandLimiter__EnableSignal was called with a signal not present in the selected band
 *  @param  signal = user-selected signal
 *  @param  band = user-selected band
 */
#define DEBUG_SIGNAL_TO_ENABLE_NOT_IN_BAND(signal,band)     LOG_ADD_EXCEPTION(SIGNAL_TO_ENABLE_NOT_IN_BAND, signal)

/**
 *  @brief  The API BandLimiter__DisableSignal or BandLimiter__DisableBand was called with an invalid band selection
 *  @param  band = the invalid user-selected band
 */
#define DEBUG_INVALID_BAND_DISABLE_SELECTION(band)      LOG_ADD_EXCEPTION(INVALID_BAND_DISABLE_SELECTION, band)

/**
 *  @brief  The API BandLimiter__DisableSignal was called with a signal not present in the selected band
 *  @param  signal = user-selected signal
 *  @param  band = user-selected band
 */
#define DEBUG_SIGNAL_TO_DISABLE_NOT_IN_BAND(signal,band)    LOG_ADD_EXCEPTION(SIGNAL_TO_DISABLE_NOT_IN_BAND, signal)

/**
 *  @brief  Signal case missing in private function FilterSignal. Case needs to be present for each signal in
 *          #BANDLIMITER_SIGNAL_TYPE
 *  @param  signal = user-selected signal
 */
#define DEBUG_CASE_MISSING_FOR_INPUT_SIGNAL(signal)     LOG_ADD_EXCEPTION(CASE_MISSING_FOR_INPUT_SIGNAL, signal)


//=====================================================================================================================
//-------------------------------------- PUBLIC (Function Prototypes) -------------------------------------------------
//=====================================================================================================================

#endif // BANDLIMITER_PRV_H_


