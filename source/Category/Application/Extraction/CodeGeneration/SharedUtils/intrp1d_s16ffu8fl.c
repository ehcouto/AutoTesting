/*
 * File: intrp1d_s16ffu8fl.c
 *
 * Code generated for Simulink model 'Extraction'.
 *
 * Model version                  : 1.7455
 * Simulink Coder version         : 8.12 (R2017a) 16-Feb-2017
 * C/C++ source code generated on : Wed May 30 12:13:53 2018
 */

#include "rtwtypes.h"
#include "intrp1d_s16ffu8fl.h"

float32 intrp1d_s16ffu8fl(uint8 bpIndex, float32 frac, const sint16 table[])
{
    uint32 offset_0d;

    /* Interpolation 1-D
       Interpolation method: 'Linear'
       Use last breakpoint for index at or above upper limit: 'off'
       Overflow mode: 'wrapping'
     */
    offset_0d = bpIndex;
    return ((float32)table[offset_0d + 1U] - (float32)table[offset_0d]) * frac
        + (float32)table[offset_0d];
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
