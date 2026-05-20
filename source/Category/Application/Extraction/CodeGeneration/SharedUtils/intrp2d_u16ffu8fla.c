/*
 * File: intrp2d_u16ffu8fla.c
 *
 * Code generated for Simulink model 'Extraction'.
 *
 * Model version                  : 1.7455
 * Simulink Coder version         : 8.12 (R2017a) 16-Feb-2017
 * C/C++ source code generated on : Wed May 30 12:13:53 2018
 */

#include "rtwtypes.h"
#include "intrp2d_u16ffu8fla.h"

float32 intrp2d_u16ffu8fla(const uint8 bpIndex[], const float32 frac[],
    const uint16 table[], uint8 stride, const uint32 maxIndex[])
{
    float32 y;
    float32 yR_1d;
    uint32 offset_1d;

    /* Interpolation 2-D
       Interpolation method: 'Linear'
       Use last breakpoint for index at or above upper limit: 'on'
       Overflow mode: 'wrapping'
     */
    offset_1d = (uint32)bpIndex[1U] * stride + bpIndex[0U];
    if (bpIndex[0U] == maxIndex[0U])
    {
        y = table[offset_1d];
    }
    else
    {
        y = ((float32)table[offset_1d + 1U] - (float32)table[offset_1d]) *
            frac[0U] + (float32)table[offset_1d];
    }

    if (bpIndex[1U] == maxIndex[1U])
    {
    }
    else
    {
        offset_1d += stride;
        if (bpIndex[0U] == maxIndex[0U])
        {
            yR_1d = table[offset_1d];
        }
        else
        {
            yR_1d = ((float32)table[offset_1d + 1U] - (float32)table[offset_1d])
                * frac[0U] + (float32)table[offset_1d];
        }

        y += (yR_1d - y) * frac[1U];
    }

    return y;
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
