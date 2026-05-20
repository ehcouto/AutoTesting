/**
 *  @file
 *  @brief       Motor Control Loop 3-phase motors.
 *  @details     This module implements the motor oriented control loop for 3-phase motors (DD, BPM, CIM).
 *  @author      alessio.beato/luigi.fagnano  (only temporary, since it is not integrated in MKS)
 *  $Header: FOC/Mcl.h 1.3 2015/11/05 11:53:55CET Marcelo Silva (SILVAMC) Exp  $
 * @copyright Copyright 2012 - $Date: 2015/11/05 11:53:55CET $. Whirlpool Corporation. All rights reserved � CONFIDENTIAL
*/
/*
 *
 *---------------------------------------------------------------------------------------------------------------------
 *---------------------------------------------------------------------------------------------------------------------
 */
#ifndef MCL_INPUT_PROC_H_
    #define MCL_INPUT_PROC_H_


//-------------------------------------- Include Files ----------------------------------------------------------------
#include "C_Extensions.h"
#include "MclCrossTypes.h"


//=====================================================================================================================
//-------------------------------------- PUBLIC (Extern Variables, Constants & Defines) -------------------------------
//=====================================================================================================================

typedef struct
{
    float32 Max_Available_Voltage_Factor;             //!< [%] Q15 - Limits the max vs voltage, this factor is correlated to the max pwm duty
} MCL_INPUT_PROC_PARAMS_TYPE;

//=====================================================================================================================
//-------------------------------------- PUBLIC (Function Prototypes) -------------------------------------------------
//=====================================================================================================================


void MclInputProc__ResetState(void);
void MclInputProc__Initialize(void);
void MclInputProc__RunningHandler(MCL_INPUT_PROC_IO_F_TYPE *io);
void MclInputProc__1msRunningHandler(MCL_INPUT_PROC_IO_F_TYPE *io, MCL_INPUT_PROC_PARAMS_TYPE *params);


#endif // MCL_INPUT_PROC_H_
