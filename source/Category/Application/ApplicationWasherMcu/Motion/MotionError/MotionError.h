/**
 *  @file       
 *
 *  @brief      API file for the MotionError module.
 *
 *  $Header: MotionError.h 1.1 2015/06/29 14:28:49EDT Paul F Spindler (SPINDPF) Exp  $
 *
 *  @copyright  Copyright 2015-$Date: 2015/06/29 14:28:49EDT $. Whirlpool Corporation. All rights reserved - CONFIDENTIAL
 */
//---------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------
#ifndef SOURCE_APPSPECIFIC_SYSTEM_MOTION_MOTIONERROR_H_
#define SOURCE_APPSPECIFIC_SYSTEM_MOTION_MOTIONERROR_H_

//=====================================================================================================================
//-------------------------------------- PUBLIC (Extern Variables, Constants & Defines) -------------------------------
//=====================================================================================================================

//! list of module events
typedef enum
{
    MOTION_ERROR_EV_NONE = 0,
    MOTION_ERROR_EV_HANDLED     //!< the error has been handled
} MOTION_ERROR_EVENT_TYPE;

//=====================================================================================================================
//-------------------------------------- PUBLIC (Function Prototypes) -------------------------------------------------
//=====================================================================================================================

void MotionError__Enable(void);
MOTION_ERROR_EVENT_TYPE MotionError__Do(void);

#endif // SOURCE_APPSPECIFIC_SYSTEM_MOTION_MOTIONERROR_H_


