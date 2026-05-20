/**
 *  @file
 *
 *  @brief      API file for the Satelization module.
 *
 *  @details    See Satelization.c for details.
 *
 *  $Header: Satelization.h 1.1 2016/01/19 12:57:57EST Paul F Spindler (SPINDPF) Exp  $
 *
 *  @copyright  Copyright 2012 - $Date: 2016/01/19 12:57:57EST $  Whirlpool Corporation.  All rights reserved - CONFIDENTIAL.
 */
#ifndef SATELIZATION_H_
#define SATELIZATION_H_

//=====================================================================================================================
//-------------------------------------- PUBLIC (Extern Variables, Constants & Defines) -------------------------------
//=====================================================================================================================

#define INVALID_SATELIZATION_RPM    0

//=====================================================================================================================
//-------------------------------------- PUBLIC (Function Prototypes) -------------------------------------------------
//=====================================================================================================================

void Satelization__Initialize(void);


void Satelization__Disable(void);
void Satelization__Enable(void);

signed short Satelization__GetRpmX16(void);

#endif /* SATELIZATION_H_ */
