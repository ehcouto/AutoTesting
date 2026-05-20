/*
 * File: plook_u32u16u32n31_binc_f.c
 *
 * Code generated for Simulink model 'Extraction'.
 *
 * Model version                  : 1.7455
 * Simulink Coder version         : 8.12 (R2017a) 16-Feb-2017
 * C/C++ source code generated on : Wed May 30 12:13:53 2018
 */

#include "rtwtypes.h"
#include "binsearch_u32u16.h"
#include "div_nzp_repeat_u32.h"
#include "plook_u32u16u32n31_binc_f.h"

uint32 plook_u32u16u32n31_binc_f(uint16 u, const uint16 bp[], uint32
    maxIndex, uint32 *fraction)
{
    uint32 bpIndex;

    /* Prelookup - Index and Fraction
       Index Search method: 'binary'
       Extrapolation method: 'Clip'
       Use previous index: 'off'
       Use last breakpoint for index at or above upper limit: 'off'
       Remove protection against out-of-range input in generated code: 'off'
       Rounding mode: 'floor'
     */
    if (u <= bp[0U])
    {
        bpIndex = 0U;
        *fraction = 0U;
    }
    else if (u < bp[maxIndex])
    {
        bpIndex = binsearch_u32u16(u, bp, maxIndex >> 1U, maxIndex);
        *fraction = div_nzp_repeat_u32((uint32)(uint16)((uint32)u -
            bp[bpIndex]) << 16, (uint16)((uint32)bp[bpIndex + 1U] -
            bp[bpIndex]), 15U);
    }
    else
    {
        bpIndex = maxIndex - 1U;
        *fraction = 2147483648U;
    }

    return bpIndex;
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
