/*
 * File: intrp2d_u16ffu32fl.c
 *
 * Code generated for Simulink model 'Extraction'.
 *
 * Model version                  : 1.7455
 * Simulink Coder version         : 8.12 (R2017a) 16-Feb-2017
 * C/C++ source code generated on : Wed May 30 12:13:53 2018
 */

#include "rtwtypes.h"
#include "intrp2d_u16ffu32fl.h"

float32 intrp2d_u16ffu32fl(const uint32 bpIndex[], const float32 frac[],
    const uint16 table[], uint32 stride)
{
    float32 yL_1d;
    uint32 offset_1d;

    /* Interpolation 2-D
       Interpolation method: 'Linear'
       Use last breakpoint for index at or above upper limit: 'off'
       Overflow mode: 'wrapping'
     */
    offset_1d = bpIndex[1U] * stride + bpIndex[0U];
    yL_1d = ((float32)table[offset_1d + 1U] - (float32)table[offset_1d]) *
        frac[0U] + (float32)table[offset_1d];
    offset_1d += stride;
    return (((float32)table[offset_1d + 1U] - (float32)table[offset_1d]) *
            frac[0U] + (float32)table[offset_1d] - yL_1d) * frac[1U] + yL_1d;
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
