/**
 *  @file
 *  @brief      This file contains the source code for the Smooth Noise Robust Differentiator (SNRD).
 *
 *  $Header: SNRD.c 1.2 2016/01/25 14:27:21EST Paul F Spindler (SPINDPF) Exp  $
 *
 *  @copyright  Copyright 2012 - $Date: 2016/01/25 14:27:21EST $  Whirlpool Corporation.  All rights reserved - CONFIDENTIAL.
 */

//-------------------------------------- Include Files ----------------------------------------------------------------

#include "C_Extensions.h"

#include "SNRD.h"



//-------------------------------------- PRIVATE (Variables, Constants & Defines) -------------------------------------

//-------------------------------------- PRIVATE (Function Prototypes) ------------------------------------------------


//=====================================================================================================================
//-------------------------------------- Public Functions -------------------------------------------------------------
//=====================================================================================================================

float SNRD__CalcDeriv(SNRD_DATA_TYPE *data)
{
    // dT/dtn = (14*(Tn-3 - Tn-5) + 14*(Tn-2 - Tn-6) + 6*(Tn-1 - Tn-7) +Tn - Tn-8) / 128 * h (sample freq)
    float slope;
    float numerator;

    numerator =  (14.0f * ((float)(data->Buffer[3] - data->Buffer[5])));      // first element of dT/dtn
    numerator += (14.0f * ((float)(data->Buffer[2] - data->Buffer[6])));      // second element of dT/dtn
    numerator += ( 6.0f * ((float)(data->Buffer[1] - data->Buffer[7])));      // third element of dT/dtn
    numerator += ( 1.0f * ((float)(data->Buffer[0] - data->Buffer[8])));      // fourth element of dT/dtn

    slope = numerator / (128.0f * data->Ts_Der);

    return (slope);
}

//=====================================================================================================================
//-------------------------------------- Private Functions ------------------------------------------------------------
//=====================================================================================================================
