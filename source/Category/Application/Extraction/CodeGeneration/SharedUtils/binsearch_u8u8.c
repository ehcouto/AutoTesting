/*
 * File: binsearch_u8u8.c
 *
 * Code generated for Simulink model 'Extraction'.
 *
 * Model version                  : 1.7455
 * Simulink Coder version         : 8.12 (R2017a) 16-Feb-2017
 * C/C++ source code generated on : Wed May 30 12:13:53 2018
 */

#include "rtwtypes.h"
#include "binsearch_u8u8.h"

uint8 binsearch_u8u8(uint8 u, const uint8 bp[], uint32 startIndex,
                       uint32 maxIndex)
{
    uint32 iRght;
    uint32 iLeft;
    uint32 bpIdx;

    /* Binary Search */
    bpIdx = startIndex;
    iLeft = 0U;
    iRght = maxIndex;
    while (iRght - iLeft > 1U)
    {
        if (u < bp[bpIdx])
        {
            iRght = bpIdx;
        }
        else
        {
            iLeft = bpIdx;
        }

        bpIdx = iRght + iLeft >> 1U;
    }

    return (uint8)iLeft;
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
