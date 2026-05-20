/*
 * File: rt_modf.c
 *
 * Code generated for Simulink model 'Extraction'.
 *
 * Model version                  : 1.7455
 * Simulink Coder version         : 8.12 (R2017a) 16-Feb-2017
 * C/C++ source code generated on : Wed May 30 12:13:53 2018
 */

#include "rtwtypes.h"
#include <float.h>
#include <math.h>
#include "rt_modf.h"

float32 rt_modf(float32 u0, float32 u1)
{
    float32 y;
    boolean_T yEq;
    float32 q;
    y = u0;
    if (u0 == 0.0F)
    {
        y = 0.0F;
    }
    else
    {
        if (u1 != 0.0F)
        {
            y = fmodf(u0, u1);
            yEq = y == 0.0F;
            if (!yEq && u1 > floorf(u1))
            {
                q = fabsf(u0 / u1);
                yEq = fabsf(q - floorf(q + 0.5F)) <= FLT_EPSILON * q;
            }

            if (yEq)
            {
                y = 0.0F;
            }
            else
            {
                if (u0 < 0.0F != u1 < 0.0F)
                {
                    y += u1;
                }
            }
        }
    }

    return y;
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
