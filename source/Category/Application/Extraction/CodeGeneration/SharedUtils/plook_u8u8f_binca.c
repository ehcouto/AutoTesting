/*
 * File: plook_u8u8f_binca.c
 *
 * Code generated for Simulink model 'Extraction'.
 *
 * Model version                  : 1.7455
 * Simulink Coder version         : 8.12 (R2017a) 16-Feb-2017
 * C/C++ source code generated on : Wed May 30 12:13:53 2018
 */

#include "rtwtypes.h"
#include "binsearch_u8u8.h"
#include "plook_u8u8f_binca.h"

uint8 plook_u8u8f_binca(uint8 u, const uint8 bp[], uint32 maxIndex,
    float32 *fraction)
{
    uint8 bpIndex;

    /* Prelookup - Index and Fraction
       Index Search method: 'binary'
       Extrapolation method: 'Clip'
       Use previous index: 'off'
       Use last breakpoint for index at or above upper limit: 'on'
       Remove protection against out-of-range input in generated code: 'off'
     */
    if (u <= bp[0U])
    {
        bpIndex = 0U;
        *fraction = 0.0F;
    }
    else if (u < bp[maxIndex])
    {
        bpIndex = binsearch_u8u8(u, bp, maxIndex >> 1U, maxIndex);
        *fraction = (float32)(uint8)((uint32)u - bp[(uint32)bpIndex]) /
            (float32)(uint8)((uint32)bp[bpIndex + 1U] - bp[(uint32)
                                bpIndex]);
    }
    else
    {
        bpIndex = (uint8)maxIndex;
        *fraction = 0.0F;
    }

    return bpIndex;
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
