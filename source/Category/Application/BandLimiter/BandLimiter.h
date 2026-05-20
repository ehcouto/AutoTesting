/**
 *  @file       
 *
 *  @brief      API file for the BandLimiter module.
 *
 *  $Header: BandLimiter.h 1.1 2016/02/02 13:40:35EST Guilherme Bencke Teixeira da Silva (BENCKG1) Exp  $
 *
 *  @copyright  Copyright 2015-$Date: 2016/02/02 13:40:35EST $. Whirlpool Corporation. All rights reserved - CONFIDENTIAL
 */
//---------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------
#ifndef BANDLIMITER_H_
#define BANDLIMITER_H_

#include "C_Extensions.h"
#include "MotionDeclare.h"

//=====================================================================================================================
//-------------------------------------- PUBLIC (Extern Variables, Constants & Defines) -------------------------------
//=====================================================================================================================

//! Enumerated list of available signals for BandLimiter module
typedef enum
{
    BANDLIMITER_SIGNAL_DRUM_TORQUE_NM,      //!< Torque applied to drum by motor or transmission, in N-m
    BANDLIMITER_SIGNAL_DRUM_SPEED_RAD_S,    //!< Drum speed, in Rad/s
    BANDLIMITER_SIGNAL_SIN_DRUM_ANGLE,      //!< Sine of drum angle, dimensionless
    BANDLIMITER_SIGNAL_COS_DRUM_ANGLE,      //!< Cosine of drum angle, dimensionless
    BANDLIMITER_NUM_OF_SIGNALS
} BANDLIMITER_SIGNAL_TYPE;

//! Enumerated list of bands in BandLimiter module
typedef enum
{
    BANDLIMITER_BAND_0,
    BANDLIMITER_NUM_OF_BANDS
} BANDLIMITER_BAND_TYPE;
//=====================================================================================================================
//-------------------------------------- PUBLIC (Function Prototypes) -------------------------------------------------
//=====================================================================================================================
void BandLimiter__Initialize(void);
float32 BandLimiter__GetSignal(BANDLIMITER_BAND_TYPE band, BANDLIMITER_SIGNAL_TYPE signal);
void BandLimiter__ResetSignal(BANDLIMITER_BAND_TYPE band, BANDLIMITER_SIGNAL_TYPE signal);
void BandLimiter__ResetBand(BANDLIMITER_BAND_TYPE band);
void BandLimiter__ResetAll(void);
void BandLimiter__EnableSignal(BANDLIMITER_BAND_TYPE band, BANDLIMITER_SIGNAL_TYPE signal);
void BandLimiter__EnableBand(BANDLIMITER_BAND_TYPE band);
void BandLimiter__EnableAll(void);
void BandLimiter__DisableSignal(BANDLIMITER_BAND_TYPE band, BANDLIMITER_SIGNAL_TYPE signal);
void BandLimiter__DisableBand(BANDLIMITER_BAND_TYPE band);
void BandLimiter__DisableAll(void);
void BandLimiter__Handler(void);
#endif // BANDLIMITER_H_


