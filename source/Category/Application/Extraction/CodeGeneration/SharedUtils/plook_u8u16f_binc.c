/*
 * File: plook_u8u16f_binc.c
 *
 * Code generated for Simulink model 'Extraction'.
 *
 * Model version                  : 1.7455
 * Simulink Coder version         : 8.12 (R2017a) 16-Feb-2017
 * C/C++ source code generated on : Wed May 30 12:13:53 2018
 */

#include "rtwtypes.h"
#include "binsearch_u8u16.h"
#include "plook_u8u16f_binc.h"

uint8 plook_u8u16f_binc(uint16 u, const uint16 bp[], uint32 maxIndex,
    float32 *fraction)
{
    uint8 bpIndex;

    /* Prelookup - Index and Fraction
       Index Search method: 'binary'
       Extrapolation method: 'Clip'
       Use previous index: 'off'
       Use last breakpoint for index at or above upper limit: 'off'
       Remove protection against out-of-range input in generated code: 'off'
     */
    if (u <= bp[0U])
    {
        bpIndex = 0U;
        *fraction = 0.0F;
    }
    else if (u < bp[maxIndex])
    {
        bpIndex = binsearch_u8u16(u, bp, maxIndex >> 1U, maxIndex);
        *fraction = (float32)(uint16)((uint32)u - bp[(uint32)bpIndex]) /
            (float32)(uint16)((uint32)bp[bpIndex + 1U] - bp[(uint32)
            bpIndex]);
    }
    else
    {
        bpIndex = (uint8)(maxIndex - 1U);
        *fraction = 1.0F;
    }

    return bpIndex;
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
