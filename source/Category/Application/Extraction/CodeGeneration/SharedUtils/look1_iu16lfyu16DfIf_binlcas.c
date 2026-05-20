/*
 * File: look1_iu16lfyu16DfIf_binlcas.c
 *
 * Code generated for Simulink model 'Extraction'.
 *
 * Model version                  : 1.7455
 * Simulink Coder version         : 8.12 (R2017a) 16-Feb-2017
 * C/C++ source code generated on : Wed May 30 12:13:53 2018
 */

#include "rtwtypes.h"
#include "look1_iu16lfyu16DfIf_binlcas.h"

uint16 look1_iu16lfyu16DfIf_binlcas(uint16 u0, const uint16 bp0[], const
    uint16 table[], uint32 maxIndex)
{
    float32 frac;
    uint32 iRght;
    uint32 iLeft;
    uint32 bpIdx;

    /* Lookup 1-D
       Search method: 'binary'
       Use previous index: 'off'
       Interpolation method: 'Linear'
       Extrapolation method: 'Clip'
       Use last breakpoint for index at or above upper limit: 'on'
       Remove protection against out-of-range input in generated code: 'off'
       Rounding mode: 'simplest'
     */
    /* Prelookup - Index and Fraction
       Index Search method: 'binary'
       Extrapolation method: 'Clip'
       Use previous index: 'off'
       Use last breakpoint for index at or above upper limit: 'on'
       Remove protection against out-of-range input in generated code: 'off'
     */
    if (u0 <= bp0[0U])
    {
        iLeft = 0U;
        frac = 0.0F;
    }
    else if (u0 < bp0[maxIndex])
    {
        /* Binary Search */
        bpIdx = maxIndex >> 1U;
        iLeft = 0U;
        iRght = maxIndex;
        while (iRght - iLeft > 1U)
        {
            if (u0 < bp0[bpIdx])
            {
                iRght = bpIdx;
            }
            else
            {
                iLeft = bpIdx;
            }

            bpIdx = iRght + iLeft >> 1U;
        }

        frac = (float32)(uint16)((uint32)u0 - bp0[iLeft]) / (float32)
            (uint16)((uint32)bp0[iLeft + 1U] - bp0[iLeft]);
    }
    else
    {
        iLeft = maxIndex;
        frac = 0.0F;
    }

    /* Interpolation 1-D
       Interpolation method: 'Linear'
       Use last breakpoint for index at or above upper limit: 'on'
       Overflow mode: 'wrapping'
     */
    if (iLeft == maxIndex)
    {
        frac = table[iLeft];
    }
    else
    {
        frac = ((float32)table[iLeft + 1U] - (float32)table[iLeft]) * frac +
            (float32)table[iLeft];
    }

    return (uint16)frac;
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
