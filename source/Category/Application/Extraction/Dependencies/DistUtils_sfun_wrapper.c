

/*
 * Include Files
 *
 */
#if defined(MATLAB_MEX_FILE)
#include "tmwtypes.h"
#include "simstruc_types.h"
#else
#include "rtwtypes.h"
#endif
#include "DistributionUtils.h"

/* %%%-SFUNWIZ_wrapper_includes_Changes_BEGIN --- EDIT HERE TO _END */
#include "DistributionUtils.h"
/* %%%-SFUNWIZ_wrapper_includes_Changes_END --- EDIT HERE TO _BEGIN */
#define u_width 1
#define y_width 1
/*
 * Create external references here.  
 *
 */
/* %%%-SFUNWIZ_wrapper_externs_Changes_BEGIN --- EDIT HERE TO _END */
 
/* %%%-SFUNWIZ_wrapper_externs_Changes_END --- EDIT HERE TO _BEGIN */

/*
 * Output functions
 *
 */
void DistUtils_sfun_Outputs_wrapper(const uint8_T *command,
			const real32_T *speed_ref,
			const real32_T *torque,
			DIST_UTILS_DXDT_OUTPUT_TYPE *dxdtOutput,
			real32_T *agreggateP2PTorque,
			real32_T *satelizationSpeed,
			real32_T *windowPower)
{
/* %%%-SFUNWIZ_wrapper_Outputs_Changes_BEGIN --- EDIT HERE TO _END */
DistributionUtils__Execute(command, speed_ref,
		torque, dxdtOutput, agreggateP2PTorque, satelizationSpeed, windowPower);
/* %%%-SFUNWIZ_wrapper_Outputs_Changes_END --- EDIT HERE TO _BEGIN */
}
