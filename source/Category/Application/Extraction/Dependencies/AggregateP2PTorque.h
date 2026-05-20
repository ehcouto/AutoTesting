/**
 *  @file
 *
 *  @brief      API file for the AggregateP2PTorque module.
 *
 *  @details    See AggregateP2PTorque.c for details.
 *
 *  $Header: AggregateP2PTorque.h 1.1 2016/01/19 12:55:10EST Paul F Spindler (SPINDPF) Exp  $
 *
 *  @copyright  Copyright 2012 - $Date: 2016/01/19 12:55:10EST $  Whirlpool Corporation.  All rights reserved - CONFIDENTIAL.
 */

#ifndef AGGREGATEP2PTORQUE_H_
#define AGGREGATEP2PTORQUE_H_

#include "AverageTorqueDeclare.h"

//=====================================================================================================================
//-------------------------------------- PUBLIC (Extern Variables, Constants & Defines) -------------------------------
//=====================================================================================================================


//=====================================================================================================================
//-------------------------------------- PUBLIC (Function Prototypes) -------------------------------------------------
//=====================================================================================================================

void AggregateP2PTorque__Initialize(void);

void AggregateP2PTorque__Clear(void);
void AggregateP2PTorque__Disable(void);
void AggregateP2PTorque__Enable(void);

AVERAGE_TORQUE_TYPE AggregateP2PTorque__GetValue(void);

#endif /* AGGREGATEP2PTORQUE_H_ */
