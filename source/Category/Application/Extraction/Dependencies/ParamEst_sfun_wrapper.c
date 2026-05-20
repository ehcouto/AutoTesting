

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
#include "DigitalFilter.h"
#include "ParamEst.h"

/* %%%-SFUNWIZ_wrapper_includes_Changes_BEGIN --- EDIT HERE TO _END */
#include <string.h>
/* %%%-SFUNWIZ_wrapper_includes_Changes_END --- EDIT HERE TO _BEGIN */
#define u_width 1
#define y_width 1
/*
 * Create external references here.  
 *
 */
/* %%%-SFUNWIZ_wrapper_externs_Changes_BEGIN --- EDIT HERE TO _END */
#define DEBUG_INVALID_COMMAND()

#define PE_P0 1000.0f

#define PE_WRAPPER_NUM_OUTPUTS	8

typedef enum pe_commands {
    INIT = 0,
    HANDLER,
    START,
    STOP,
    SETUP,
    RESET,
    COV_RESET,
	SELECT_OUTPUTS,
    READ_OUTPUTS
} PARAM_EST_COMMANDS;

static PARAM_EST_SETUP_TYPE PE_Setup;

static unsigned char Selected_Outputs[PE_WRAPPER_NUM_OUTPUTS];
/* %%%-SFUNWIZ_wrapper_externs_Changes_END --- EDIT HERE TO _BEGIN */

/*
 * Output functions
 *
 */
void ParamEst_sfun_Outputs_wrapper(const uint8_T *Command,
			const PARAM_EST_SETUP_TYPE *Setup,
			const uint8_T *Set_Inputs,
			const uint8_T *Outputs,
			const real32_T *Speed_RPM,
			const real32_T *SIN_Drum_Pos,
			const real32_T *COS_Drum_Pos,
			const real32_T *Torque_Nm,
			real32_T *J,
			real32_T *mgr,
			real32_T *mgr_theta,
			real32_T *b,
			real32_T *c,
			real32_T *f,
			real32_T *mgr_BB,
			real32_T *BB_phasediff)
{
/* %%%-SFUNWIZ_wrapper_Outputs_Changes_BEGIN --- EDIT HERE TO _END */
    PARAM_EST_COMMANDS com = (PARAM_EST_COMMANDS)Command[0];
    
#if defined(MATLAB_MEX_FILE)
    if(Set_Inputs[0])
    {
        ParamEst__SetInputs(Torque_Nm[0], Speed_RPM[0], SIN_Drum_Pos[0], COS_Drum_Pos[0]);
    }
#endif
        
    switch (com)
    {
        case INIT:
            ParamEst__Initialize();
            break;
        case HANDLER:
            ParamEst__Handler();
            break;
        case START:
            ParamEst__Enable(FALSE);
            break;
        case STOP:
            ParamEst__Disable();
            break;
        case SETUP:
            memcpy(&PE_Setup, Setup, sizeof(PARAM_EST_SETUP_TYPE));
            ParamEst__SetSetup(&PE_Setup);
            break;
        case RESET:
            ParamEst__Enable(TRUE);
            break;
        case COV_RESET:
            ParamEst__CovReset(PE_P0);
            break;
		case SELECT_OUTPUTS:
			memcpy(&Selected_Outputs, Outputs, PE_WRAPPER_NUM_OUTPUTS*sizeof(unsigned char));
			break;
        case READ_OUTPUTS:
            break;
        default:
            DEBUG_INVALID_COMMAND();
    }
    
    if (Selected_Outputs[0]) J[0] = ParamEst__GetParameterValue(PARAM_EST_INERTIA);
    if (Selected_Outputs[1]) mgr[0] = ParamEst__GetParameterValue(PARAM_EST_UBMAG);
    if (Selected_Outputs[2]) mgr_theta[0] = ParamEst__GetParameterValue(PARAM_EST_UBPHASE);
#if MODEL_BK_EULER == ENABLED || MODEL_HYB_ACCEL == ENABLED
    if (Selected_Outputs[3]) b[0] = ParamEst__GetParameterValue(PARAM_EST_VISCOUS);
#endif
    if (Selected_Outputs[4]) c[0] = ParamEst__GetParameterValue(PARAM_EST_COULOMB);
    if (Selected_Outputs[5]) f[0] = ParamEst__GetParameterValue(PARAM_EST_FRICTION);
#if MODEL_BK_EULER_B0_BB == ENABLED
    if (Selected_Outputs[6]) mgr_BB[0] = ParamEst__GetParameterValue(PARAM_EST_BBMAG);
    if (Selected_Outputs[7]) BB_phasediff[0] = ParamEst__GetParameterValue(PARAM_EST_BBPHASEDIFF);
#endif
/* %%%-SFUNWIZ_wrapper_Outputs_Changes_END --- EDIT HERE TO _BEGIN */
}
