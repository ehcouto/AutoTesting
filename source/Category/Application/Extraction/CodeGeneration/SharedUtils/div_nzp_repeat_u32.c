/*
 * File: div_nzp_repeat_u32.c
 *
 * Code generated for Simulink model 'Extraction'.
 *
 * Model version                  : 1.7455
 * Simulink Coder version         : 8.12 (R2017a) 16-Feb-2017
 * C/C++ source code generated on : Wed May 30 12:13:53 2018
 */

#include "rtwtypes.h"
#include "div_nzp_repeat_u32.h"

uint32 div_nzp_repeat_u32(uint32 numerator, uint32 denominator, uint32
    nRepeatSub)
{
    uint32 quotient;
    uint32 iRepeatSub;
    boolean_T numeratorExtraBit;
    quotient = numerator / denominator;
    numerator %= denominator;
    for (iRepeatSub = 0U; iRepeatSub < nRepeatSub; iRepeatSub++)
    {
        numeratorExtraBit = numerator >= 2147483648U;
        numerator <<= 1U;
        quotient <<= 1U;
        if (numeratorExtraBit || numerator >= denominator)
        {
            quotient++;
            numerator -= denominator;
        }
    }

    return quotient;
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
