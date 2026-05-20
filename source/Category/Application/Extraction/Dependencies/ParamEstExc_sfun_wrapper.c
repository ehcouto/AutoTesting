

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
#include "ParamEstExc.h"
#include <string.h>


/* %%%-SFUNWIZ_wrapper_includes_Changes_BEGIN --- EDIT HERE TO _END */
 
/* %%%-SFUNWIZ_wrapper_includes_Changes_END --- EDIT HERE TO _BEGIN */
#define u_width 1
#define y_width 1
/*
 * Create external references here.  
 *
 */
/* %%%-SFUNWIZ_wrapper_externs_Changes_BEGIN --- EDIT HERE TO _END */
typedef enum {
	EXC_INIT,
	EXC_HANDLER,
	EXC_SETUP,
	EXC_SET_DATA,
	EXC_ENABLE,
	EXC_DISABLE
} PARAM_EST_EXC_COMMAND_TYPE;

static PARAM_EST_EXC_SETUP_TYPE PE_Exc_Setup_Static;
/* %%%-SFUNWIZ_wrapper_externs_Changes_END --- EDIT HERE TO _BEGIN */

/*
 * Output functions
 *
 */
void ParamEstExc_sfun_Outputs_wrapper(const PARAM_EST_EXC_SETUP_TYPE *PE_Exc_Setup,
			const uint8_T *Command,
			const real32_T *w,
			real32_T *w_offset,
			boolean_T *is_running)
{
/* %%%-SFUNWIZ_wrapper_Outputs_Changes_BEGIN --- EDIT HERE TO _END */
PARAM_EST_EXC_COMMAND_TYPE com = (PARAM_EST_EXC_COMMAND_TYPE)Command[0];

switch(com)
{
    case EXC_INIT:
        ParamEstExc__Initialize();
        break;
    case EXC_HANDLER:
        ParamEstExc__Handler();
        break;
    case EXC_SETUP:
        memcpy(&PE_Exc_Setup_Static,PE_Exc_Setup,sizeof(PE_Exc_Setup_Static));
        ParamEstExc__SetSetup(&PE_Exc_Setup_Static);
        break;
    case EXC_SET_DATA:
#if defined(MATLAB_MEX_FILE)
        ParamEstExc__SetOmega(w[0]);
#endif
        break;
    case EXC_ENABLE:
        ParamEstExc__Enable();
        break;
    case EXC_DISABLE:
        ParamEstExc__Disable();
        break;
    default:
        break;
}

#if defined(MATLAB_MEX_FILE)
w_offset[0] = ParamEstExc__GetRefGenOffset();
is_running[0] = ParamEstExc__IsRunning();
#endif
/* %%%-SFUNWIZ_wrapper_Outputs_Changes_END --- EDIT HERE TO _BEGIN */
}
