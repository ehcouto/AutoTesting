/**
 *  @file
 *
 *  @brief      Source file for the ParamEst (Parameter Estimation) file.
 *
 *  @details    GDM for Parameter Estimation algorithm.
 *              Parameter Estimation is a fundamental algorithm used to identify parameters in models of physical
 *              systems. The module is quite generic, and can be applied to various products and physical processes
 *              through use of the physical models embedded into this module. The module is based on Recursive Least
 *              Squares (RLS), a well-known algorithm. The module provides an API for the user interaction.
 *
 *  @software owner     Guilherme T Silva
 *  @technology owner   Guilherme T Silva
 *
 *  @section    Applicable_Documents
 *                  Ljung and Soderstrom, Theory and Practice of Recursive Identification, 1983
 *                  Soderstrom and Stoica, System Identification, 1989
 *                  Golub and Van Loan, Matrix Computations (4th Ed), 2012  <tr>
 *

    @startuml{ParamEst_state_machine.png}

        title Parameter Estimation State Machine

        [*] --> Idle

        Idle --> Active : ev_enable

        Active : enter / ActiveEnter
        Active : do / ActiveDo
        Active --> Idle : ev_disable

    @enduml

 *
 *  @copyright  Copyright 2014 Whirlpool Corporation. All rights reserved - CONFIDENTIAL
 */
//---------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------


//-------------------------------------- Include Files ----------------------------------------------------------------
#include "C_Extensions.h"

#include "ParamEst.h"
#include "ParamEst_prv.h"
#ifndef MATLAB_MEX_FILE
#include "SettingFile.h"
#endif

#include "limits.h"
#include "math.h"
#include "string.h"

//=====================================================================================================================
//=== StateEngineCrank (v1.5) Auto-Generated Definitions for Parameter Estimation State Machine
//=====================================================================================================================
// !! StateEngineCrank cannot update this code if you change the header !!


//! Unique names used to identify states of the Parameter Estimation state machine.
typedef enum PARAMETER_ESTIMATION_STATE_ENUM
{
    STATE_NONE   = 0,
    STATE_IDLE   = 1,                               //!< (initial_state)
    STATE_ACTIVE = 2,
    STATE_PARAMETER_ESTIMATION_END = 3
} PARAMETER_ESTIMATION_STATE_TYPE;

//! The state variable for the Parameter Estimation state machine.
//! State machines with a history pseudostate may write to this variable during initialization.
//! Other state machines must only write to this variable through auto-generated functions.
static PARAMETER_ESTIMATION_STATE_TYPE Parameter_Estimation_State;

// -- Private Function Prototypes used by the state machine for customization functions --
static void ActiveDo(void);
static void ActiveEnter(void);

// -- Private Function Prototypes used by the state machine for standard state machine functions --
static void EventDisable(void);
static void EventEnable(void);
static void ParameterEstimationExecute(void);
static void ParameterEstimationInitialize(void);

// !! StateEngineCrank cannot update this code if you change the footer !!
// End of StateEngineCrank Auto-Generated Code.

//-------------------------------------- PRIVATE (Variables, Constants & Defines) -------------------------------------

//! defines the regression delay count (in number of samples) for each model for population of the regressor and output
typedef unsigned char DELAY_RLS_TYPE;
static const DELAY_RLS_TYPE DELAY_RLS[] =
{
#if MODEL_BK_EULER_B0 == ENABLED
    1,  // PARAM_EST_MODEL_BK_EULER_B0
#endif
#if MODEL_BK_EULER == ENABLED
    1,  // PARAM_EST_MODEL_BK_EULER
#endif
#if MODEL_HYB_ACCEL == ENABLED
    1,  // PARAM_EST_MODEL_HYB_ACCEL
#endif
#if MODEL_BK_EULER_B0_BB == ENABLED
    1   // PARAM_EST_MODEL_BK_EULER_B0_BB
#endif
};
//CT_ASSERT ( (sizeof(DELAY_RLS) / sizeof(DELAY_RLS[0])) == PARAM_EST_NUM_OF_MODELS );

#define MSEC_TO_SEC(msec)       ((msec) / 1000.0f)                           //!< Conversion from milliseconds to seconds

//! The sample rate of the ParamID algorithm. The possible combinations of slot options and
//! integer downsample ratios will dictate the sample rates which may be achieved.
#define TS_PARAMID              MSEC_TO_SEC(INPUT_HANDLER_TIME_SLOT * INPUT_HANDLER_SLOT_PERIODS)

#define RPM_TO_RAD_S(rpm)       ((rpm) * 2.0f * INPUT_PI / 60.0f)             //!< Conversion from RPM to rad/s
#define DEG_TO_RAD(deg)         ((deg) / 180.0f * INPUT_PI)                  //!< Conversion from degrees to radians
#define SIGN(in)                (((in) > 0) ? 1 : (((in) < 0) ? -1 : 0))    //!< Signum (sign function) macro

/**
 * @brief   Wrap angle into specific range.
 * @details Calls WrapAngleRad with an upper limit parameter specified by #PARAM_EST_ANGLE_WRAP_UPPER_LIMIT
 * @param angle = the angle in radians to be wrapped
 */
#define PARAM_EST_ANGLE_WRAP(angle)       (WrapAngleRad(angle,PARAM_EST_ANGLE_WRAP_UPPER_LIMIT))

typedef unsigned char DOWN_SAMPLER_COUNTER_TYPE;          // 1 = 1 call of the ParamEst__Handler()
//CT_ASSERT(INPUT_HANDLER_SLOT_PERIODS <= UCHAR_MAX);

/**
 *  Variable type definition for the module motor speed.
 *  Resolution: 1.0 = 1.0 rpm
 */
typedef float MOTOR_SPEED_TYPE;

//! This enum stores names of all the possible entries of the phi (regressor) vector.

typedef enum REGRESSOR_ELEMENTS_ENUM
{
#if MODEL_BK_EULER == ENABLED || MODEL_HYB_ACCEL == ENABLED
    REG_ELE_SPEED,
#endif
    REG_ELE_TORQUE,
    REG_ELE_CONSTANT,
    REG_ELE_SIN_ANGLE,
    REG_ELE_COS_ANGLE,
#if MODEL_BK_EULER_B0_BB == ENABLED
    REG_ELE_SIN_BB_ANGLE,
    REG_ELE_COS_BB_ANGLE,
#endif
    N_THETA_MAX
} REGRESSOR_ELEMENTS_TYPE;

#if MODEL_BK_EULER_B0 == ENABLED
static const REGRESSOR_ELEMENTS_TYPE PHI_BK_EULER_B0[] =
{
    REG_ELE_TORQUE,
    REG_ELE_CONSTANT,
    REG_ELE_SIN_ANGLE,
    REG_ELE_COS_ANGLE
};
#endif

#if MODEL_BK_EULER == ENABLED || MODEL_HYB_ACCEL == ENABLED
static const REGRESSOR_ELEMENTS_TYPE PHI_BK_EULER[] =
{
    REG_ELE_SPEED,
    REG_ELE_TORQUE,
    REG_ELE_CONSTANT,
    REG_ELE_SIN_ANGLE,
    REG_ELE_COS_ANGLE
};
#endif

#if MODEL_BK_EULER_B0_BB == ENABLED
static const REGRESSOR_ELEMENTS_TYPE PHI_BK_EULER_B0_BB[] =
{
    REG_ELE_TORQUE,
    REG_ELE_CONSTANT,
    REG_ELE_SIN_ANGLE,
    REG_ELE_COS_ANGLE,
    REG_ELE_SIN_BB_ANGLE,
    REG_ELE_COS_BB_ANGLE
};
#endif

//! declaration of the variables used when calculating the regressor
typedef struct
{
#if MODEL_BK_EULER == ENABLED || MODEL_HYB_ACCEL == ENABLED
    float Phi[N_THETA_MAX];
#elif MODEL_BK_EULER_B0 == ENABLED || MODEL_BK_EULER_B0_BB == ENABLED
    float Phi[N_THETA_MAX+1];	//One more float to filter the speed on models that don't have it in their regressor
#endif
    float Y;
} REGRESSOR_FORM_TYPE;

//! PE Filter Parameters GESE structure
typedef PACKED struct PARAMETER_ESTIMATION_FILTER_STRUCT
{
	union
	{
		uint8 All;
		struct
		{
			uint8 Section_Number		:3;
			uint8 Unused				:5;
		} Bit;
	} Byte_0;

	uint8 Filter_Warmup;
	uint8 Unused_Byte_2;
	uint8 Unused_Byte_3;

	float32 First_Filter_Coeff;
} PARAMETER_ESTIMATION_FILTER_TYPE;

#define S_T_NUM_OF_ROWS (2 * N_THETA_MAX)                       							//!< Number of rows in S_T (S-transposed) matrix
//! This array holds the square-root covariance matrix in its upper half. P is the covariance matrix, and P = S * S'.
//! The top half of this array stores S' (S-transpose, or S_T).
//! In its lower half it holds the matrix V', where R = V * V' is the covariance matrix added to P in the Kalman
//! adaptation method. That is P = S*S' + V*V'. If the Kalman method is not chosen, the lower half is not used.
static float S_T[S_T_NUM_OF_ROWS][N_THETA_MAX];
static float Theta[N_THETA_MAX];                    //!< The vector of identified digital parameters
static float Lambda_Sqrt_Inv;						//!< 1 / sqrt(Lambda) Stored for ease of computation

static BIQUAD_STATE_TYPE Filter_Storage[N_THETA_MAX+1][PARAM_EST_FILTER_MAX_NUM_SECTIONS];	//!< One extra slot to filter the speed when models that don't have this signal in the regressor are in use

static unsigned char N_Theta;               //!< Number of digital parameters to identify. Based on the chosen model.
static unsigned char Phi_Idx[N_THETA_MAX];  //!< The vector of indices for ordering the phi (regressor) vector
static float Speed_Previous;                //!< Previous speed (rad/s)
static float Residual;                      //!< RLS Residual (y - y_hat)

#if MODEL_BK_EULER_B0_BB == ENABLED
static float Alpha_UB;                      //!< Drum angle in radians
static float Alpha_BB;                      //!< Modified drum angle used to represent ball balancer movement (ball balancer angle), generated by multiplying
                                            //!< drum angle #Alpha_UB by the BB_Lag parameter in #Setup_Parameters
#endif

static const PARAM_EST_SETUP_TYPE *Setup_Parameters;        //!< Internal storage of ParamID configuration parameters address

static DIGITAL_FILTER_SOS_TYPE Param_Est_Filter;			//!< Internal storage for the pre-filter configuration parameters address

static BOOL_TYPE Sf_Parameters_Fault;						//!< Setting file error flag

//! definition of module static variables
static struct
{
    DOWN_SAMPLER_COUNTER_TYPE   Handler_Down_Sample_Counter;
    DELAY_RLS_TYPE              Delay_Counter;
    BOOL_TYPE                   Reset;
} ParamID;

#ifdef MATLAB_MEX_FILE
static float Torque_Input;
static float Speed_Input;
static float Sin_Drum_Pos_Input;
static float Cos_Drum_Pos_Input;
#endif

//-------------------------------------- PRIVATE (Function Prototypes) ------------------------------------------------

static void HandleFilterReset(const PARAM_EST_SETUP_TYPE *new_parameters);
static BOOL_TYPE IsModuleActive(void);
static BOOL_TYPE IsSetupAdaptationKalmanValid(const PARAM_EST_SETUP_TYPE *parameters);
static BOOL_TYPE IsSetupAdaptationForgettingValid(const PARAM_EST_SETUP_TYPE *parameters);
static void SetupModel(const PARAM_EST_SETUP_TYPE *parameters);
static BOOL_TYPE VerifySetup(const PARAM_EST_SETUP_TYPE *parameters, const DIGITAL_FILTER_SOS_TYPE *filter);

static float ParamIdRLS(float y, float *phi);
static void ParamIdQR(float a[][N_THETA_MAX]);
static void DFiltNChan(float *u, unsigned char n_chan);
static float WrapAngleRad(float angle,float ulim);
static void ResetFilter(void);
static DIGITAL_FILTER_SOS_TYPE GetFilterConfig(unsigned char index);
static void ResetTheta(void);
static REGRESSOR_FORM_TYPE GetRegressorInputs(void);
static void SetSettingFileError();
static void ClearSettingFileError();

#if MODEL_BK_EULER_B0_BB == ENABLED
static float BBAngle(float drum_angle, float lag);
#endif

//=====================================================================================================================
//-------------------------------------- Public Functions -------------------------------------------------------------
//=====================================================================================================================

//---------------------------------------------------------------------------------------------------------------------
/**
 *  @brief      It Initializes the module ParamEst and its variables
 *
 */
//#pragma optimize=size no_inline no_unroll
void ParamEst__Initialize(void)
{
    ParamEst__CovReset(PARAM_EST_P0_DEFAULT);                                  // Initialize the covariance matrix w/ default
    Setup_Parameters = NULL;
    Speed_Previous = 0.0f;
#if MODEL_BK_EULER_B0_BB == ENABLED
    Alpha_UB = 0.0f;
    Alpha_BB = 0.0f;
#endif
    Residual = 0;
    ResetTheta();
    ResetFilter();
    memset(&ParamID, 0, sizeof(ParamID));
    ParameterEstimationInitialize();
    ClearSettingFileError();
}

#ifdef MATLAB_MEX_FILE
/**
 *  @brief  Sets Parameter Estimation inputs when PE is set to
 *          receive data instead of polling it from other modules
 *  @param  torque_nm   Torque in N*m
 *  @param  speed_rpm   Speed in RPM
 *  @param  sin_drum_pos    Drum angle sine
 *  @param  cos_drum_pos    Drum angle cosine
 *
 */
void ParamEst__SetInputs(float torque_nm, float speed_rpm, float sin_drum_pos, float cos_drum_pos)
{
    Torque_Input = torque_nm;
    Speed_Input = speed_rpm;
    Sin_Drum_Pos_Input = sin_drum_pos;
    Cos_Drum_Pos_Input = cos_drum_pos;
}
#endif

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief   The ParamID handler.
 * @details INPUT_HANDLER_TIME_SLOT shall define the call rate of this function.
 */
//#pragma optimize=size no_inline no_unroll
void ParamEst__Handler(void)
{
    ParamID.Handler_Down_Sample_Counter = (ParamID.Handler_Down_Sample_Counter > 0) ? ParamID.Handler_Down_Sample_Counter - 1 : 0;
    ParameterEstimationExecute();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 *  @brief  Start the ParamID module
 *  @param  reset   TRUE/FALSE flag to indicate whether to reset the module, or resume from the state when
 *                  the module was stopped. If enabled, the covariance matrix and parameter vector are reset.
 *                  Parameter only has affect on module behavior if the module is inactive when API command is received.
 */
//#pragma optimize=size no_inline no_unroll
void ParamEst__Enable(BOOL_TYPE reset)
{
    if ( Setup_Parameters == NULL )
    {
        DEBUG_SETUP_PARAMETERS_NULL();
    }
    else
    {
		ParamID.Reset = reset;
		EventEnable();
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Stop the ParamID module
 */
void ParamEst__Disable(void)
{
    EventDisable();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Covariance Reset Function
 *
 * @details Resets the covariance matrix used by the ParamIdRLS function. This is done as an
 * adaptation method. It is not required for the user to call this function. It places p0 on
 * the diagonal and zeros elsewhere.
 *
 * @param p0 = the value to place on the main diagonal entries of the covariance matrix.
 *
 * @return The function writes to the private array S_T.
 */
//#pragma optimize=size no_inline no_unroll
void ParamEst__CovReset(float p0)
{
    float s0 = sqrtf(p0);
    unsigned char j;
    unsigned char i;

    for ( j = 0; j < N_THETA_MAX; j++ )
    {
        for ( i = 0; i < S_T_NUM_OF_ROWS; i++ )
        {
            S_T[i][j] = (i == j) ? s0 : 0.0f;
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 *  @brief  Returns the current ParamId Setup.
 *  @param  parameters   Pointer to the Environment definition of the PARAM_EST_SETUP_TYPE
 *  @return The current Setup is not NULL
 */
void ParamEst__GetSetup(PARAM_EST_SETUP_TYPE *parameters)
{
    if ( Setup_Parameters != NULL )
    {
    	if ( parameters != NULL )
    		memcpy(parameters, Setup_Parameters, sizeof(PARAM_EST_SETUP_TYPE));
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 *  @brief  Set the module's functional parameters
 *  @param  parameters  Environment specific setup.
 *  @return Is the new ParamEst setup valid?
 */
//#pragma optimize=size no_inline no_unroll
void ParamEst__SetSetup(const PARAM_EST_SETUP_TYPE *parameters)
{
	DIGITAL_FILTER_SOS_TYPE filter;
	if ( parameters == NULL )
	{
		DEBUG_SETUP_PARAMETERS_NULL();
		return; // do not execute the remainder of the function
	}
	else
	{
		filter = GetFilterConfig(parameters->Filter_Configuration_Index);
		if ( VerifySetup(parameters,&filter) == TRUE )
		{

			// if statement defines checks applicable only when the module is currently active
			if ( IsModuleActive() == TRUE )
			{
				if ( parameters->Model_Config.Struct.Model != Setup_Parameters->Model_Config.Struct.Model )
				{
					DEBUG_CANNOT_CHANGE_MODEL_WHILE_MODULE_IS_ACTIVE(parameters->Model_Config.Struct.Model);
					return; // do not execute the remainder of the function
				}
				else
				{
					HandleFilterReset(parameters);
				}
			}

			SetupModel(parameters);
			Lambda_Sqrt_Inv = (parameters->Model_Config.Struct.Adapt_Type == PARAM_EST_ADAPTATION_KALMAN) ? 1.0f : 1.0f / sqrtf(parameters->Forgetting);
			Setup_Parameters = parameters;  // Copy the configuration address to the module
			Param_Est_Filter = filter;	// Copy the filter configuration address to the module
		}
	}
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Return the state of the ParamID module
 *
 * @return The state of the ParamID module
 */
BOOL_TYPE ParamEst__IsRunning(void)
{
    return ( IsModuleActive() );
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Query parameter value from ParamID module
 *
 * @details Calculates the relevant continuous-time parameters from the digital parameters. This
 * function is where the model of the physical system is used to "make sense" of the identified parameters.
 *
 * @param parameter = name of the parameter to query
 *
 * @return The value of the requested parameter
 */
//#pragma optimize=size no_inline no_unroll
float ParamEst__GetParameterValue(PARAM_EST_PARAMETER_TYPE parameter)
{
    float result = 0.0f;
#if MODEL_BK_EULER_B0_BB == ENABLED
    float angle_ub = 0.0f;
    float angle_bb = 0.0f;
#endif

    if ( Setup_Parameters == NULL )
    {
        DEBUG_CANNOT_GET_PARAMETER_VALUE_SETUP_IS_NULL();
    }
    else if ( parameter == PARAM_EST_RESIDUAL )
    {
        result = Residual;
    }
    else
    {
        switch ( Setup_Parameters->Model_Config.Struct.Model )
        {
#if MODEL_BK_EULER_B0 == ENABLED
            // w_dot(t) = [T(t), -1, -sin(alpha), -cos(alpha)] * [1/J; c/J; mgr*cos(beta)/J; mgr*sin(beta)/J]
            case PARAM_EST_MODEL_BK_EULER_B0:
                if ( Theta[Phi_Idx[REG_ELE_TORQUE]] != 0.0f )
                {
                    switch ( parameter )
                    {
                        case PARAM_EST_INERTIA:
                            result = 1.0f / Theta[Phi_Idx[REG_ELE_TORQUE]];
                            break;
                        case PARAM_EST_COULOMB:                                       // -fallthrough
                        case PARAM_EST_FRICTION:
                            result = Theta[Phi_Idx[REG_ELE_CONSTANT]] / Theta[Phi_Idx[REG_ELE_TORQUE]];
                            break;
                        case PARAM_EST_UBMAG:
                            // This is the norm
                            result = sqrtf(Theta[Phi_Idx[REG_ELE_SIN_ANGLE]] * Theta[Phi_Idx[REG_ELE_SIN_ANGLE]] +
                                    Theta[Phi_Idx[REG_ELE_COS_ANGLE]] * Theta[Phi_Idx[REG_ELE_COS_ANGLE]]) /
                                    Theta[Phi_Idx[REG_ELE_TORQUE]];
                            break;
                        case PARAM_EST_UBPHASE:
                            result = PARAM_EST_ANGLE_WRAP(atan2f(Theta[Phi_Idx[REG_ELE_COS_ANGLE]], Theta[Phi_Idx[REG_ELE_SIN_ANGLE]]));
                            break;
                        default:        // Invalid parameter selection by user
                            DEBUG_INVALID_PARAMETER_SELECTION(parameter);
                            break;
                    }
                }
                break;
#endif
#if MODEL_BK_EULER == ENABLED
            // w(t) = [w(t-1), T(t), -1, -sin(alpha), -cos(alpha)] * [A*J/Ts; A; A*c; A*mgr*cos(beta); A*mgr*sin(beta)]
            // where A = Ts/(J+b*Ts)
            case PARAM_EST_MODEL_BK_EULER:
                if ( Theta[Phi_Idx[REG_ELE_TORQUE]] != 0.0f )
                {
                    switch ( parameter )
                    {
                        case PARAM_EST_INERTIA:
                            result = Theta[Phi_Idx[REG_ELE_SPEED]] / Theta[Phi_Idx[REG_ELE_TORQUE]] * TS_PARAMID;
                            break;
                        case PARAM_EST_VISCOUS:
                            result = (1.0f - Theta[Phi_Idx[REG_ELE_SPEED]]) / Theta[Phi_Idx[REG_ELE_TORQUE]];
                            break;
                        case PARAM_EST_COULOMB:
                            result = Theta[Phi_Idx[REG_ELE_CONSTANT]] / Theta[Phi_Idx[REG_ELE_TORQUE]];
                            break;
                        case PARAM_EST_FRICTION:        // b * w + c
                            result = ParamEst__GetParameterValue(PARAM_EST_VISCOUS) * Speed_Previous + ParamEst__GetParameterValue(PARAM_EST_COULOMB);
                            break;
                        case PARAM_EST_UBMAG:
                            result = sqrtf(Theta[Phi_Idx[REG_ELE_SIN_ANGLE]] * Theta[Phi_Idx[REG_ELE_SIN_ANGLE]] +
                                    Theta[Phi_Idx[REG_ELE_COS_ANGLE]] * Theta[Phi_Idx[REG_ELE_COS_ANGLE]]) /
                                    Theta[Phi_Idx[REG_ELE_TORQUE]];
                            break;
                        case PARAM_EST_UBPHASE:
                            result = PARAM_EST_ANGLE_WRAP(atan2f(Theta[Phi_Idx[REG_ELE_COS_ANGLE]], Theta[Phi_Idx[REG_ELE_SIN_ANGLE]]));
                            break;
                        default:        // Invalid parameter selection by user
                            DEBUG_INVALID_PARAMETER_SELECTION(parameter);
                            break;
                    }
                }
                break;
#endif
#if MODEL_HYB_ACCEL
            // w_dot(t) = [-w(t), T(t), -1, -sin(alpha(t)), -cos(alpha(t))] * [b/J; 1/J; c/J; mgr*cos(beta)/J; mgr*sin(beta)/J]
            case PARAM_EST_MODEL_HYB_ACCEL:
                if ( Theta[Phi_Idx[REG_ELE_TORQUE]] != 0.0f )
                {
                    switch ( parameter )
                    {
                        case PARAM_EST_INERTIA:
                            result = 1.0f / Theta[Phi_Idx[REG_ELE_TORQUE]];
                            break;
                        case PARAM_EST_VISCOUS:
                            result = Theta[Phi_Idx[REG_ELE_SPEED]] / Theta[Phi_Idx[REG_ELE_TORQUE]];
                            break;
                        case PARAM_EST_COULOMB:
                            result = Theta[Phi_Idx[REG_ELE_CONSTANT]] / Theta[Phi_Idx[REG_ELE_TORQUE]];
                            break;
                        case PARAM_EST_FRICTION:        // b * w + c
                            result = ParamEst__GetParameterValue(PARAM_EST_VISCOUS) * Speed_Previous + ParamEst__GetParameterValue(PARAM_EST_COULOMB);
                            break;
                        case PARAM_EST_UBMAG:
                            result = sqrtf(Theta[Phi_Idx[REG_ELE_SIN_ANGLE]] * Theta[Phi_Idx[REG_ELE_SIN_ANGLE]] +
                                    Theta[Phi_Idx[REG_ELE_COS_ANGLE]] * Theta[Phi_Idx[REG_ELE_COS_ANGLE]]) /
                                    Theta[Phi_Idx[REG_ELE_TORQUE]];
                            break;
                        case PARAM_EST_UBPHASE:
                            result = PARAM_EST_ANGLE_WRAP(atan2f(Theta[Phi_Idx[REG_ELE_COS_ANGLE]], Theta[Phi_Idx[REG_ELE_SIN_ANGLE]]));
                            break;
                        default:        // Invalid parameter selection by user
                            DEBUG_INVALID_PARAMETER_SELECTION(parameter);
                            break;
                    }
                }
                break;
#endif
#if MODEL_BK_EULER_B0_BB == ENABLED
            case PARAM_EST_MODEL_BK_EULER_B0_BB:

                if(Theta[Phi_Idx[REG_ELE_TORQUE]] != 0.0f)
                {
                    switch( parameter )
                    {
                        case PARAM_EST_INERTIA:
                            result = 1.0f / Theta[Phi_Idx[REG_ELE_TORQUE]];
                            break;
                        case PARAM_EST_COULOMB:                                       // -fallthrough
                        case PARAM_EST_FRICTION:
                            result = Theta[Phi_Idx[REG_ELE_CONSTANT]] / Theta[Phi_Idx[REG_ELE_TORQUE]];
                            break;
                        case PARAM_EST_UBMAG:
                            // This is the norm
                            result = sqrtf(Theta[Phi_Idx[REG_ELE_SIN_ANGLE]] * Theta[Phi_Idx[REG_ELE_SIN_ANGLE]] +
                            Theta[Phi_Idx[REG_ELE_COS_ANGLE]] * Theta[Phi_Idx[REG_ELE_COS_ANGLE]]) /
                            Theta[Phi_Idx[REG_ELE_TORQUE]];
                            break;
                        case PARAM_EST_UBPHASE:
                            result = PARAM_EST_ANGLE_WRAP(atan2f(Theta[Phi_Idx[REG_ELE_COS_ANGLE]], Theta[Phi_Idx[REG_ELE_SIN_ANGLE]]));
                            break;
                        case PARAM_EST_BBMAG:
                            result = sqrtf(Theta[Phi_Idx[REG_ELE_SIN_BB_ANGLE]] * Theta[Phi_Idx[REG_ELE_SIN_BB_ANGLE]] +
                            Theta[Phi_Idx[REG_ELE_COS_BB_ANGLE]] * Theta[Phi_Idx[REG_ELE_COS_BB_ANGLE]]) /
                            Theta[Phi_Idx[REG_ELE_TORQUE]];
                            break;
                        case PARAM_EST_BBPHASE:
                            result = PARAM_EST_ANGLE_WRAP(atan2f(Theta[Phi_Idx[REG_ELE_COS_BB_ANGLE]], Theta[Phi_Idx[REG_ELE_SIN_BB_ANGLE]]));
                            break;
                        case PARAM_EST_BBPHASEDIFF:
                            angle_ub = WrapAngleRad(ParamEst__GetParameterValue(PARAM_EST_UBPHASE) + Alpha_UB, 2*INPUT_PI); // instantaneous load unbalance position
                            angle_bb = WrapAngleRad(ParamEst__GetParameterValue(PARAM_EST_BBPHASE) + Alpha_BB, 2*INPUT_PI); // instantaneous ball balancer position

                            result = PARAM_EST_ANGLE_WRAP(angle_ub - angle_bb);
                            break;
                        case PARAM_EST_BBPHASELAG:
                            // Since whether the balls are lagging or leading the unbalance is relative to the direction of rotation, it is
                            // necessary to invert the sign of the phase difference when the drum is rotating in the negative direction. In other words,
                            // whether the balls are "behind" or "in front of" the unbalance depends on the direction the unbalance is rotating.
                            if(SIGN(INPUT_DRUM_SPEED_RAD_S()) == -1)
                            {
                                result = PARAM_EST_ANGLE_WRAP(-ParamEst__GetParameterValue(PARAM_EST_BBPHASEDIFF));
                            }
                            else
                            {
                                result = ParamEst__GetParameterValue(PARAM_EST_BBPHASEDIFF);
                            }

                            break;
                        default:        // Invalid parameter selection by user
                            DEBUG_INVALID_PARAMETER_SELECTION(parameter);
                            break;
                    }
                }
                break;
#endif
            default:                    // Code should never get here, Invalid model selection
                // an invalid Model is posted by macro DEBUG_INVALID_MODEL in ParamEst__Enable()
                break;
        }
    }
    return ( result );
}

//---------------------------------------------------------------------------------------------------------------------
/**
 *	@brief	Get the setting file error flag
 */
BOOL_TYPE ParamEst__GetSettingFileError()
{
	return Sf_Parameters_Fault;
}

//=====================================================================================================================
//-------------------------------------- Private Functions ------------------------------------------------------------
//=====================================================================================================================

/**
 *  @brief      Compares filter1 and filter2 coefficients, returns TRUE if they are the same
 *  @param      filter1      filter to be compared to filter2
 *  @param      filter2      filter to be compared to filter1
 */
//#pragma optimize=size no_inline no_unroll
static BOOL_TYPE CompareFilters(DIGITAL_FILTER_SOS_TYPE filter1, DIGITAL_FILTER_SOS_TYPE filter2)
{
	for (int i = 0; i < filter1.Num_Sections; i++)
	{
		for (int j = 0; j < filter2.Num_Sections; j++)
		{
			for (int k = 0; k < BIQUAD_FILTER_ORDER-1; k++)
			{
				if ((filter1.Coeffs[i].Num[k] != filter2.Coeffs[j].Num[k]) ||
						(filter1.Coeffs[i].Den[k] != filter2.Coeffs[j].Den[k]))
				{
					return FALSE;
				}
			}
		}
	}
	return TRUE;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 *  @brief      Performs checks to verify if fiter needs to be reset.
 *  @param      new_parameters      pointer to the parameters
 */
//#pragma optimize=size no_inline no_unroll
static void HandleFilterReset(const PARAM_EST_SETUP_TYPE *new_parameters)
{
    #define RESET_FILTER()  ResetFilter();\
                            ParamID.Delay_Counter = 0

    if ( new_parameters->Filter_Configuration_Index != FILTER_DISABLED )
    {
        if (( Setup_Parameters->Filter_Configuration_Index == FILTER_DISABLED ) ||                  				// the filter is switched on
            ( Setup_Parameters->Filter_Configuration_Index != new_parameters->Filter_Configuration_Index))         	// the index of the filter has changed
        {
            RESET_FILTER();
        }
    }
    // else parameters->Enable == FALSE, do nothing
}

//---------------------------------------------------------------------------------------------------------------------
/**
 *  @brief  Returns if the module is inactive or active
 *
 *  @return TRUE = module is active; FALSE = module is inactive
 */
static BOOL_TYPE IsModuleActive(void)
{
    return (( Parameter_Estimation_State == STATE_IDLE) ? FALSE : TRUE );
}

//---------------------------------------------------------------------------------------------------------------------
/**
 *  @brief      Verifies if the values are valid when the Adaptation type is PARAM_EST_ADAPTATION_KALMAN.
 *  @param      parameters      pointer to the parameters
 *
 *  @return     TRUE = values are valid; FALSE = values are not valid
 */
//#pragma optimize=size no_inline no_unroll
static BOOL_TYPE IsSetupAdaptationKalmanValid(const PARAM_EST_SETUP_TYPE *parameters)
{
    BOOL_TYPE ret_val = TRUE;
    unsigned char i;
    for ( i = 0; i < PARAM_EST_LENGTH_KALMAN; i++ )
    {
        if ( parameters->Kalman_Vector[i] < 0.0f )
        {
            DEBUG_INVALID_KALMAN_VALUE(i, parameters->Kalman_Vector[i]);
            ret_val = FALSE;
            break;
        }
    }
    return ( ret_val );
}

//---------------------------------------------------------------------------------------------------------------------
/**
 *  @brief      Verifies if the values are valid when the Adaptation type is PARAM_EST_ADAPTATION_FORGETTING.
 *  @param      parameters      pointer to the parameters
 *
 *  @return     TRUE = values are valid; FALSE = values are not valid
 */
//#pragma optimize=size no_inline no_unroll
static BOOL_TYPE IsSetupAdaptationForgettingValid(const PARAM_EST_SETUP_TYPE *parameters)
{
    BOOL_TYPE ret_val = TRUE;
    if (( parameters->Forgetting > 1.0f ) ||
        ( parameters->Forgetting < 0.1f ))
    {
        DEBUG_INVALID_FORGETTING_FACTOR(parameters->Forgetting);
        ret_val = FALSE;
    }
    return ( ret_val );
}

//---------------------------------------------------------------------------------------------------------------------
/**
 *  @brief      Sets up the Model.
 *  @param      parameters      pointer to the parameters
 */
//#pragma optimize=size no_inline no_unroll
static void SetupModel(const PARAM_EST_SETUP_TYPE *parameters)
{
    unsigned char i;

    switch ( parameters->Model_Config.Struct.Model )
    {
#if MODEL_BK_EULER_B0 == ENABLED
        case PARAM_EST_MODEL_BK_EULER_B0:
            N_Theta = sizeof(PHI_BK_EULER_B0) / sizeof(PHI_BK_EULER_B0[0]);    // Length of regressor
            // Set up the indices of data elements in regressor
            for ( i = 0; i < N_Theta; i++ )
            {
                Phi_Idx[PHI_BK_EULER_B0[i]] = i;
            }
            break;
#endif
#if MODEL_BK_EULER == ENABLED
        case PARAM_EST_MODEL_BK_EULER:
#endif
#if MODEL_HYB_ACCEL == ENABLED
        case PARAM_EST_MODEL_HYB_ACCEL:
#endif
#if MODEL_BK_EULER == ENABLED || MODEL_HYB_ACCEL == ENABLED
            N_Theta = sizeof(PHI_BK_EULER) / sizeof(PHI_BK_EULER[0]);
            // Set up the indices of data elements in regressor
            for ( i = 0; i < N_Theta; i++ )
            {
                Phi_Idx[PHI_BK_EULER[i]] = i;
            }
            break;
#endif
#if MODEL_BK_EULER_B0_BB == ENABLED
        case PARAM_EST_MODEL_BK_EULER_B0_BB:
            N_Theta = sizeof(PHI_BK_EULER_B0_BB) / sizeof(PHI_BK_EULER_B0_BB[0]);
            for ( i = 0; i < N_Theta; i++ )
            {
                Phi_Idx[PHI_BK_EULER_B0_BB[i]] = i;
            }
            break;
#endif
        default:    // Invalid model, but code will never get here since it has already been checked
            break;
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Wrapper function for filtering multiple channels of data
 *
 * @details Calls FilterDF2T in a loop for multiple channels of data.
 *
 * @param u = Pointer to 1-D array that holds the data to be filtered. Each element in u corresponds
 * to a channel of the input signal. The result of the function is to overwrite this array's
 * contents with the filtered data.
 * The function overwrites input u to store the filtered output data.
 *
 * @param n_chan = Number of channels contained in u. This can be less than or equal to the array's
 * size.
 */
//#pragma optimize=size no_inline no_unroll
static void DFiltNChan(float *u, unsigned char n_chan)
{
    unsigned char i;

    for(i=0; i<n_chan; i++)             // loop over channels
    {
    	u[i] = DigitalFilter__DF2TSOS(u[i], Param_Est_Filter, Filter_Storage[i]);
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Ball balancer angle construction
 *
 * @details Constructs an angle in radians which moves at a frequency defined by
 * the basket frequency multiplied by the lag parameter. This angle is used to estimate ball
 * balancer position in the RLS algorithm.
 *
 * @param w_drum        Drum speed in rad/s
 * @param lag           Factor by which the frequency of the drum is multiplied to construct
 * the ball balancer angle.
 *
 * @return The ball balancer angle.
 */
//#pragma optimize=size no_inline no_unroll
#if MODEL_BK_EULER_B0_BB == ENABLED
static float BBAngle(float w_drum, float lag)
{
    return PARAM_EST_ANGLE_WRAP(Alpha_BB + w_drum * TS_PARAMID * lag);
}
#endif

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Angle wrapping function
 *
 * @detail Wraps angles in radians into a range specified by the upper limit parameter. Only works for angles
 * within 1 period in either direction of the principal range. For example, for a range of [0,2*PI], it will
 * wrap 3*PI to PI, but it will not do the same for 5*PI.
 *
 * @param angle = the angle in radians to wrap inside the principal range
 * @param ulim = the upper limit of the principal range. The principal range is taken as [ulim - 2*PI, ulim]
 *
 * @return the angle wrapped into the principal range
 */
//#pragma optimize=size no_inline no_unroll
static float WrapAngleRad(float angle,float ulim)
{

    if (angle >= ulim)
    {
        angle -= 2.0f * PI;
    }
    else if (angle < (ulim - 2.0 * PI))
    {
        angle += 2.0f * PI;
    }

    return angle;

}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Recursive Least Squares Algorithm, ParamID's core function
 *
 * @details This algorithm computes the parameter estimates from the data. It is not model-specific
 * and is applicable to any system written in regressor form (y = phi' * theta). Implementation follows
 * Potter's square root algorithm with forgetting and/or Kalman-style adaptation using QR decomposition.
 * For convenience, S' (S-transpose) is updated instead of S. See Ljung and Soderstrom,
 * Theory and Practice of Recursive Identification, page 327-329 for details. In the comments, Matlab-
 * style notation is used to show the underlying matrix computations.
 *
 * @param y = output in the model of the physical system.
 *
 * @param phi = pointer to 1-D input array which represents the regressor.
 *
 * @return The function writes to the static array Theta as its output, as well as returning the residual (error).
 */
//#pragma optimize=size no_inline no_unroll
static float ParamIdRLS(float y, float *phi)
{
    float f[N_THETA_MAX];
    float l_t[N_THETA_MAX];
    float beta;
    float alpha;
    float e;
    float y_hat;
    float e_by_beta;
    unsigned char i;
    unsigned char j;

    // write all elements in the arrays to remove PC Lint warnings
    for ( REGRESSOR_ELEMENTS_TYPE regressor_element = (REGRESSOR_ELEMENTS_TYPE)0; regressor_element < N_THETA_MAX; regressor_element++ )
    {
        f[regressor_element] = 0.0f;
        l_t[regressor_element] = 0.0f;
    }

    // y_hat = phi'*Theta
    // This is the one-step ahead prediction of the output (prediction step).
    y_hat = 0;
    for(i=0; i<N_Theta; i++)
    {
        y_hat += Theta[i] * phi[i]; // one-step ahead prediction y_hat(t|t-1)
    }

    // This is the innovation step where we compare estimated output to actual output to
    // form the error signal used for correction.
    e = y - y_hat;

    // f = S_T*phi
    for(i=0; i<N_Theta; i++) // rows
    {
        f[i] = 0; // f is a column vector
        for(j=0; j<N_Theta; j++) // cols
        {
            f[i] += S_T[i][j] * phi[j];
        }
    }

    // beta = Lambda + f'*f
    if( (Setup_Parameters->Model_Config.Struct.Adapt_Type == PARAM_EST_ADAPTATION_FORGETTING) ||
        (Setup_Parameters->Model_Config.Struct.Adapt_Type == PARAM_EST_ADAPTATION_BOTH) )
    {
        beta = Setup_Parameters->Forgetting;
    }
    else
    {
        beta = 1.0f;
    }

    for(i=0; i<N_Theta; i++)
    {
        beta += f[i] * f[i];
    }

    e_by_beta = e / beta;

    if( (Setup_Parameters->Model_Config.Struct.Adapt_Type == PARAM_EST_ADAPTATION_FORGETTING) ||
        (Setup_Parameters->Model_Config.Struct.Adapt_Type == PARAM_EST_ADAPTATION_BOTH) )
    {
        alpha = 1.0f / (beta + sqrtf(beta * Setup_Parameters->Forgetting));
    }
    else
    {
        alpha = 1.0f / (beta + sqrtf(beta));
    }

    // l_t = f'*S_T
    for(j=0; j<N_Theta; j++) // cols
    {
        l_t[j] = 0.0f;
        for(i=0; i<N_Theta; i++) // rows
        {
            l_t[j] += f[i] * S_T[i][j];
        }
    }

    // Parameter Update. This is the correction step where we modify the estimated parameters
    // based on the innovation (error in prediction).
    // Theta = Theta + l_t'*(e/beta)
    for(i=0; i<N_Theta; i++)
    {
        Theta[i] += l_t[i] * e_by_beta;
    }

    // Square root covariance update.

    // S_T = (S_T - alpha*f*l_t)/sqrtf(Lambda)
    for(i=0; i<N_Theta; i++) // rows
    {
        for(j=0; j<N_Theta; j++) // cols
        {
            S_T[i][j] = (S_T[i][j] - alpha * f[i] * l_t[j]) * Lambda_Sqrt_Inv;
        }
    }

    // If using Kalman approach, now do QR factorization to update S_T
    if (( Setup_Parameters->Model_Config.Struct.Adapt_Type == PARAM_EST_ADAPTATION_KALMAN ) ||
        ( Setup_Parameters->Model_Config.Struct.Adapt_Type == PARAM_EST_ADAPTATION_BOTH ))
    {
        // Now concatenate S' vertically with V' to incorporate additive term to covariance matrix.
        // V' is a diagonal matrix that satisfies V*V' = R_1. The covariance matrix P = S * S' is modified
        // in this approach by adding R_1 = V*V' to it. That is, P <- P + R_1 = S*S' + V*V' is performed.

        // Reorder Kalman Vector to match phi. This is done according to each model, to link parameters'
        // adaptation rates to the corresponding elements in the regressor.
        switch(Setup_Parameters->Model_Config.Struct.Model)
        {
            default:
#if MODEL_BK_EULER_B0 == ENABLED
            case PARAM_EST_MODEL_BK_EULER_B0:
                S_T[Phi_Idx[REG_ELE_TORQUE]+N_Theta] [Phi_Idx[REG_ELE_TORQUE]] = Setup_Parameters->Kalman_Vector[PARAM_EST_KALMAN_INERTIA];
                S_T[Phi_Idx[REG_ELE_CONSTANT]+N_Theta] [Phi_Idx[REG_ELE_CONSTANT]] = Setup_Parameters->Kalman_Vector[PARAM_EST_KALMAN_COULOMB];
                S_T[Phi_Idx[REG_ELE_SIN_ANGLE]+N_Theta] [Phi_Idx[REG_ELE_SIN_ANGLE]] = Setup_Parameters->Kalman_Vector[PARAM_EST_KALMAN_UNBALANCE];
                S_T[Phi_Idx[REG_ELE_COS_ANGLE]+N_Theta] [Phi_Idx[REG_ELE_COS_ANGLE]] = Setup_Parameters->Kalman_Vector[PARAM_EST_KALMAN_UNBALANCE];
                break;
#endif

            // For the backward Euler case, Kalman tuning doesn't allow us to strictly separate the adaptation of inertia and
            // viscous friction. They are arbitrarily assigned below. Avoid using the Kalman adaptation method for this model
            // choice if the goal is to tune inertia and friction adaptation independently.
#if MODEL_BK_EULER == ENABLED
            case PARAM_EST_MODEL_BK_EULER:
#endif
#if MODEL_HYB_ACCEL == ENABLED
            case PARAM_EST_MODEL_HYB_ACCEL:
#endif
#if MODEL_BK_EULER == ENABLED || MODEL_HYB_ACCEL == ENABLED
                S_T[Phi_Idx[REG_ELE_TORQUE]+N_Theta] [Phi_Idx[REG_ELE_TORQUE]] = Setup_Parameters->Kalman_Vector[PARAM_EST_KALMAN_INERTIA];
                S_T[Phi_Idx[REG_ELE_SPEED]+N_Theta] [Phi_Idx[REG_ELE_SPEED]] = Setup_Parameters->Kalman_Vector[PARAM_EST_KALMAN_VISCOUS];
                S_T[Phi_Idx[REG_ELE_CONSTANT]+N_Theta] [Phi_Idx[REG_ELE_CONSTANT]] = Setup_Parameters->Kalman_Vector[PARAM_EST_KALMAN_COULOMB];
                S_T[Phi_Idx[REG_ELE_SIN_ANGLE]+N_Theta] [Phi_Idx[REG_ELE_SIN_ANGLE]] = Setup_Parameters->Kalman_Vector[PARAM_EST_KALMAN_UNBALANCE];
                S_T[Phi_Idx[REG_ELE_COS_ANGLE]+N_Theta] [Phi_Idx[REG_ELE_COS_ANGLE]] = Setup_Parameters->Kalman_Vector[PARAM_EST_KALMAN_UNBALANCE];
                break;
#endif
#if MODEL_BK_EULER_B0_BB == ENABLED
            case PARAM_EST_MODEL_BK_EULER_B0_BB:
                S_T[Phi_Idx[REG_ELE_TORQUE]+N_Theta] [Phi_Idx[REG_ELE_TORQUE]] = Setup_Parameters->Kalman_Vector[PARAM_EST_KALMAN_INERTIA];
                S_T[Phi_Idx[REG_ELE_CONSTANT]+N_Theta] [Phi_Idx[REG_ELE_CONSTANT]] = Setup_Parameters->Kalman_Vector[PARAM_EST_KALMAN_COULOMB];
                S_T[Phi_Idx[REG_ELE_SIN_ANGLE]+N_Theta] [Phi_Idx[REG_ELE_SIN_ANGLE]] = Setup_Parameters->Kalman_Vector[PARAM_EST_KALMAN_UNBALANCE];
                S_T[Phi_Idx[REG_ELE_COS_ANGLE]+N_Theta] [Phi_Idx[REG_ELE_COS_ANGLE]] = Setup_Parameters->Kalman_Vector[PARAM_EST_KALMAN_UNBALANCE];
                S_T[Phi_Idx[REG_ELE_SIN_BB_ANGLE]+N_Theta] [Phi_Idx[REG_ELE_SIN_BB_ANGLE]] = Setup_Parameters->Kalman_Vector[PARAM_EST_KALMAN_BALLBALANCER];
                S_T[Phi_Idx[REG_ELE_COS_BB_ANGLE]+N_Theta] [Phi_Idx[REG_ELE_COS_BB_ANGLE]] = Setup_Parameters->Kalman_Vector[PARAM_EST_KALMAN_BALLBALANCER];
                break;
#endif
        }

        ParamIdQR(S_T);
    }

    return e;

}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief QR decomposition of the input matrix. Called by the ParamIdRLS function.
 *
 * @details Performs QR (orthogonal-triangular) decomposition of the input matrix A
 * so that A = Q * R. Uses the Householder method. This function overwrites the input
 * array A with R. Q is not needed, and is thus not stored. See Golub and Van Loan,
 * Matrix Computations (4th Ed), section 5.2.2 for details. In the comments, Matlab-
 * style notation is used to show the underlying matrix computations.
 *
 * @param   a = pointer to 2-D array to decompose. A is overwritten with the result R.
 *          The function overwrites the input array with the result.
 */
//#pragma optimize=size no_inline no_unroll
static void ParamIdQR(float a[][N_THETA_MAX])
{
    float sigma;
    float mu;
    float beta;
    float v0;
    unsigned char j;
    unsigned char i;
    unsigned char k;
    unsigned char m;


    for ( j = 0; j < N_Theta; j++ ) // Loop across columns of A to introduce zeros
    {
        m = 2 * N_Theta - 1;    // This is the lowest relevant row in A.

        // Set the lower limit of subcolumn j to exclude lower zeros. m is the index of the last nonzero entry.
        while (( a[m][j] == 0 ) &&
               ( m > j ))
        {
            m--;
        }

        // m = j + N_Theta; // This indicates the location of the last nonzero element of column A(:,j),
        // since the bottom half of A is diagonal (R_1 matrix)


        // The following block computes the householder vector v. The first component is stored in v0,
        // the remaining components are the subdiagonal elements of column j. In the comments that follow,
        // x denotes the subcolumn being zero'd. That is, x = A(j:m,j).

        // sigma = x(2:m)'*x(2:m)
        // This is the norm squared of x(2:m)
        sigma = 0;
        for ( i = j + 1; i < m + 1; i++ )
        {
            sigma += a[i][j] * a[i][j];
        }

        v0 = 1;
        if (( sigma == 0 ) &&
            ( a[j][j] >= 0 ))
        {
            continue; // do nothing - the vector is already  zero'd
        }
        else if (( sigma == 0 ) &&
                 ( a[j][j] < 0 ))  // Vector is zero'd, but x(1) is negative.
        {
            beta = -2;
            mu = -a[j][j];
        }
        else    // Most situations, where x is not zero'd yet.
        {
            // mu is the 2-norm of x. That is, mu = norm(x,2).
            mu = sqrtf(a[j][j]*a[j][j] + sigma);

            // This computes v0, the first component in the householder vector.
            // The remaining components of v are just the 2nd-last components of x.
            if ( a[j][j] <= 0 )
            {
                v0 = a[j][j] - mu;
            }
            else
            {
                v0 = -sigma / (a[j][j] + mu);
            }

            beta = 2.0f / (sigma + v0 * v0);
        }


        // Now we apply the Householder matrix to the subcolumns of A to introduce zeros.
        // In the section that follows, the operations perform the multiplication H * A.
        // H is not stored, but is used in factored form H = I - beta*v*v'.
        // This block calculates H * A = (I - v*v') * A = A - (v * (v' * A(j:m,k)))
        // v is the vector [v0;A(j+1:m,j)].

        a[j][j] = mu;   // This is the only nonzero element in subcolumn A(j:m,j).

        for ( k = j + 1; k < N_Theta; k++ )     // loop over remaining subcolumns (right of column j)
        {
            // Calculate the scalar quantity v' * A(j:m,k). mu is reused for storage of this quantity.
            mu = v0 * a[j][k];
            for ( i = j + 1; i < m + 1; i++ )
            {
                mu += a[i][j] * a[i][k];    // The elements of v are a[i][j]
            }

            // Now complete the computation by taking beta * v * (v' * A(j:m,k))
            // and subtracting this vector from A(j:m,k).
            a[j][k] -= beta * mu * v0;
            for ( i = j + 1; i < m + 1; i++ )
            {
                a[i][k] -= beta * mu * a[i][j]; // The elements of v are a[i][j]
            }
        }

        // Set subdiagonal entries of column j to zero
        for ( i = j + 1; i < m + 1; i++ )
        {
            a[i][j] = 0;
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Reset Filter States (storage) to zero
 */
//#pragma optimize=size no_inline no_unroll
static void ResetFilter()
{
	for (int i = 0; i <= N_THETA_MAX; i++)
	{
		DigitalFilter__ResetSOS(Filter_Storage[i], PARAM_EST_FILTER_MAX_NUM_SECTIONS);
	}
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Reset Theta (parameter vector) to zero
 */
//#pragma optimize=size no_inline no_unroll
static void ResetTheta(void)
{
    REGRESSOR_ELEMENTS_TYPE i;

    for ( i = (REGRESSOR_ELEMENTS_TYPE)0; i < N_THETA_MAX; i++ )
    {
        Theta[i] = 0.0f;
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 *	@brief	Set the setting file error flag
 */
static void SetSettingFileError()
{
	Sf_Parameters_Fault = TRUE;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 *	@brief	Clear the setting file error flag
 */
static void ClearSettingFileError()
{
	Sf_Parameters_Fault = FALSE;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Retrieve the filter configuration structure pointer from GESE
 *
 * @return  Pointer to the filter configuration structure.
 */
static DIGITAL_FILTER_SOS_TYPE GetFilterConfig(unsigned char index)
{
	DIGITAL_FILTER_SOS_TYPE pe_filter;

#ifndef MATLAB_MEX_FILE
	SETTINGFILE_LOADER_TYPE loaded_data;
	PARAMETER_ESTIMATION_FILTER_TYPE *PE_filter_ptr;

	pe_filter.Num_Sections = 0;
	pe_filter.Coeffs = NULL;

	if (index != FILTER_DISABLED)
	{
		if (SettingFile__BasicLoader(SF_PTR_MCU_PARAMETER_ESTIMATION, index, &loaded_data) == PASS)
		{
			ClearSettingFileError();

			PE_filter_ptr = (PARAMETER_ESTIMATION_FILTER_TYPE *)loaded_data.Data;	//lint !e927 Suppress Info: cast from pointer to pointer [MISRA 2004 Rule 11.4]

			pe_filter.Num_Sections = PE_filter_ptr->Byte_0.Bit.Section_Number;
			pe_filter.Coeffs = (DIGITAL_FILTER_BIQUAD_COEFF_TYPE *)&(PE_filter_ptr->First_Filter_Coeff);	//lint !e929 Suppress Info: cast from pointer to pointer [MISRA 2004 Rule 11.4]
		}
		else
		{
#if (PARAM_EST_HARD_CODED_PARAMETERS != 0)
			if (index < NUM_FILTERS)
			{
				pe_filter = Param_Est_Filter_Bank[index];
			}
			else
#endif // ((PARAM_EST_HARD_CODED_PARAMETERS != 0)
				SetSettingFileError();
		}
	}
	else
	{
		ClearSettingFileError();
	}
#else
#if (PARAM_EST_HARD_CODED_PARAMETERS != 0)
			if (index < NUM_FILTERS)
			{
				pe_filter = Param_Est_Filter_Bank[index];
			}
#endif // ((PARAM_EST_HARD_CODED_PARAMETERS != 0)
#endif // ifndef MATLAB_MEX_FILE

	return pe_filter;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Get the regressor inputs based on the Environment defined Model.
 *
 * @return  The regressor Y and Phi values.
 */
//#pragma optimize=size no_inline no_unroll
static REGRESSOR_FORM_TYPE GetRegressorInputs(void)
{
    REGRESSOR_FORM_TYPE  regressor;
    regressor.Y = 0.0f;
    // write all elements in the arrays to remove PC Lint warnings
    for ( REGRESSOR_ELEMENTS_TYPE element = (REGRESSOR_ELEMENTS_TYPE)0; element < N_THETA_MAX; element++ )
    {
        regressor.Phi[element] = 0.0f;
    }

    // Set up the regressor and the output for the model realization chosen.
    switch ( Setup_Parameters->Model_Config.Struct.Model )
    {
#if MODEL_BK_EULER_B0 == ENABLED
        case PARAM_EST_MODEL_BK_EULER_B0:
            regressor.Phi[Phi_Idx[REG_ELE_TORQUE]] = INPUT_DRUM_TORQUE_NM();
            regressor.Phi[Phi_Idx[REG_ELE_CONSTANT]] = -1.0f;                                      // constant
            regressor.Phi[Phi_Idx[REG_ELE_SIN_ANGLE]] = -INPUT_SIN_DRUM_POS();     // sin(alpha) (Rad)
            regressor.Phi[Phi_Idx[REG_ELE_COS_ANGLE]] = -INPUT_COS_DRUM_POS();     // cos(alpha) (Rad)

            // Append speed to phi only for filtering (not part of the actual regressor). The last element of phi is simply holding current speed.
            regressor.Phi[N_Theta] = INPUT_DRUM_SPEED_RAD_S();           // Speed (Rad/s)

            if ( Setup_Parameters->Filter_Configuration_Index != FILTER_DISABLED )
            {
                DFiltNChan(regressor.Phi, N_Theta + 1);     // Filter the output and regressor (N_Theta + 1 because of appended speed)
            }

            // y is acceleration (backward difference rule).
            regressor.Y = (regressor.Phi[N_Theta] - Speed_Previous) / TS_PARAMID;

            Speed_Previous = regressor.Phi[N_Theta];
            break;
#endif
#if MODEL_BK_EULER == ENABLED
        case PARAM_EST_MODEL_BK_EULER:
            regressor.Phi[Phi_Idx[REG_ELE_SPEED]] = INPUT_DRUM_SPEED_RAD_S();              // Speed (Rad/s)
            regressor.Phi[Phi_Idx[REG_ELE_TORQUE]] = INPUT_DRUM_TORQUE_NM();
            regressor.Phi[Phi_Idx[REG_ELE_CONSTANT]] = -1.0f;                                      // constant
            regressor.Phi[Phi_Idx[REG_ELE_SIN_ANGLE]] = -INPUT_SIN_DRUM_POS();     // sin(alpha) (Rad)
            regressor.Phi[Phi_Idx[REG_ELE_COS_ANGLE]] = -INPUT_COS_DRUM_POS();     // cos(alpha) (Rad)

            if ( Setup_Parameters->Filter_Configuration_Index != FILTER_DISABLED )
            {
                DFiltNChan(regressor.Phi, N_Theta);     // Filter the regressor
            }

            // Y is current speed, and comes from regressor, so is already filtered
            regressor.Y = regressor.Phi[Phi_Idx[REG_ELE_SPEED]];        // Form the output (current speed)
            regressor.Phi[Phi_Idx[REG_ELE_SPEED]] = Speed_Previous;     // Actual speed element in regressor is previous speed

            Speed_Previous = regressor.Y;
            break;
#endif
#if MODEL_HYB_ACCEL == ENABLED
        case PARAM_EST_MODEL_HYB_ACCEL:
            regressor.Phi[Phi_Idx[REG_ELE_SPEED]] = -INPUT_DRUM_SPEED_RAD_S();             // Speed (Rad/s)
            regressor.Phi[Phi_Idx[REG_ELE_TORQUE]] = INPUT_DRUM_TORQUE_NM();
            regressor.Phi[Phi_Idx[REG_ELE_CONSTANT]] = -1.0f;                                        // constant
            regressor.Phi[Phi_Idx[REG_ELE_SIN_ANGLE]] = -INPUT_SIN_DRUM_POS();     // sin(alpha) (Rad)
            regressor.Phi[Phi_Idx[REG_ELE_COS_ANGLE]] = -INPUT_COS_DRUM_POS();     // cos(alpha) (Rad)

            if ( Setup_Parameters->Filter_Configuration_Index != FILTER_DISABLED )
            {
                DFiltNChan(regressor.Phi, N_Theta);     // Filter the regressor
            }

            // y is acceleration (backward difference rule), and is linear function of regressor, so is already filtered
            regressor.Y = (-regressor.Phi[Phi_Idx[REG_ELE_SPEED]] - Speed_Previous) / TS_PARAMID;

            Speed_Previous = -regressor.Phi[Phi_Idx[REG_ELE_SPEED]];
            break;
#endif
#if MODEL_BK_EULER_B0_BB == ENABLED
        case PARAM_EST_MODEL_BK_EULER_B0_BB:
            Alpha_BB = BBAngle(INPUT_DRUM_SPEED_RAD_S(),Setup_Parameters->BB_Lag);
            Alpha_UB = PARAM_EST_ANGLE_WRAP(INPUT_DRUM_POS_RADIANS());

            regressor.Phi[Phi_Idx[REG_ELE_TORQUE]] = INPUT_DRUM_TORQUE_NM();         // Torque (N-m)
            regressor.Phi[Phi_Idx[REG_ELE_CONSTANT]] = -1.0f;                                    // constant
            regressor.Phi[Phi_Idx[REG_ELE_SIN_ANGLE]] = -INPUT_SIN_DRUM_POS();                      // sin(alpha) (Rad)
            regressor.Phi[Phi_Idx[REG_ELE_COS_ANGLE]] = -INPUT_COS_DRUM_POS();                      // cos(alpha) (Rad)
            regressor.Phi[Phi_Idx[REG_ELE_SIN_BB_ANGLE]] = -sinf(Alpha_BB);                   // sin(alpha_bb) (Rad)
            regressor.Phi[Phi_Idx[REG_ELE_COS_BB_ANGLE]] = -cosf(Alpha_BB);                   // cos(alpha_bb) (Rad)

            // Append speed to phi only for filtering (not part of the actual regressor). The last element of phi is simply holding current speed.
            regressor.Phi[N_Theta] = INPUT_DRUM_SPEED_RAD_S();                          // Speed (Rad/s)

            if( Setup_Parameters->Filter_Configuration_Index != FILTER_DISABLED )
            {
                DFiltNChan(regressor.Phi, N_Theta + 1);     // Filter the output and regressor (N_Theta + 1 because of appended speed)
            }

            // y is acceleration (backward difference rule).
            regressor.Y = (regressor.Phi[N_Theta] - Speed_Previous) / TS_PARAMID;

            Speed_Previous = regressor.Phi[N_Theta];
            break;
#endif
        default:                // Code can never get here
            // an invalid Model is posted by macro DEBUG_INVALID_MODEL in ParamEst__Enable()
            break;
    }

    return ( regressor );
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief   Verifies the passed Setup values are valid.
 *
 * @return  TRUE = parameters are valid; FALSE = parameters are invalid
 */
//#pragma optimize=size no_inline no_unroll
static BOOL_TYPE VerifySetup(const PARAM_EST_SETUP_TYPE *parameters, const DIGITAL_FILTER_SOS_TYPE *filter)
{
    BOOL_TYPE config_valid = TRUE;

    // verify the Setup Model
    if ( parameters->Model_Config.Struct.Model >= PARAM_EST_NUM_OF_MODELS )
    {
       config_valid = FALSE;
       DEBUG_INVALID_MODEL(parameters->Model_Config.Struct.Model);
    }
    // else Model is valid

    // verify the Setup Adapt_Type and Forgetting value
    switch ( parameters->Model_Config.Struct.Adapt_Type )
    {
        case PARAM_EST_ADAPTATION_FORGETTING:
            if ( IsSetupAdaptationForgettingValid(parameters) == FALSE )
            {
                config_valid = FALSE;
            }
            // else setup is valid
            break;
        case PARAM_EST_ADAPTATION_KALMAN:
            if ( IsSetupAdaptationKalmanValid(parameters) == FALSE )
            {
                config_valid = FALSE;
            }
            // else setup is valid
            break;
        case PARAM_EST_ADAPTATION_BOTH:
            if (( IsSetupAdaptationForgettingValid(parameters) == FALSE ) ||
                ( IsSetupAdaptationKalmanValid(parameters) == FALSE ))
            {
                config_valid = FALSE;
            }
            // else setup is valid
            break;
        default:
            config_valid = FALSE;
            DEBUG_INVALID_ADAPTATION_TYPE(parameters->Model_Config.Struct.Adapt_Type);
            break;
    }

    // verify the Setup Filter
    if (( parameters->Filter_Configuration_Index != FILTER_DISABLED ) &&
        ( filter->Num_Sections > PARAM_EST_FILTER_MAX_NUM_SECTIONS ) &&
		( filter->Num_Sections == 0 ))
    {
        config_valid = FALSE;
        DEBUG_INVALID_FILTER_ORDER(filter->Num_Sections);
    }

    return ( config_valid );
}

//=====================================================================================================================
//-- Private State Machine Customization Functions --------------------------------------------------------------------
//=====================================================================================================================

//---------------------------------------------------------------------------------------------------------------------
/**
 *  @brief      Tasks to perform when enabling the algorithm.
 */
//#pragma optimize=size no_inline no_unroll
static void ActiveEnter(void)
{
    if ( ParamID.Reset == TRUE )
    {
        ParamEst__CovReset(PARAM_EST_P0_DEFAULT);
        ResetFilter();
        ResetTheta();
    }
    ParamID.Handler_Down_Sample_Counter = INPUT_HANDLER_SLOT_PERIODS;
    Speed_Previous = 0.0f;
    ParamID.Delay_Counter = 0;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 *  @brief      Tasks to perform when the algorithm is active.
 */
//#pragma optimize=size no_inline no_unroll
static void ActiveDo(void)
{
    if ( ParamID.Handler_Down_Sample_Counter == 0 )      // This is the down sampler of the handler to reach the desired sample rate.
    {
        ParamID.Handler_Down_Sample_Counter = INPUT_HANDLER_SLOT_PERIODS;
        REGRESSOR_FORM_TYPE regressor = GetRegressorInputs();
        if ( ParamID.Delay_Counter >= DELAY_RLS[Setup_Parameters->Model_Config.Struct.Model] )
        {
            Residual = ParamIdRLS(regressor.Y, regressor.Phi);      // run the regression
        }
        else
        {
            ParamID.Delay_Counter++;
        }
    }
    // else down sampler timer not expired
}

//=====================================================================================================================
//=== StateEngineCrank (v1.5) Auto-Generated Functions for Parameter Estimation State Machine
//=====================================================================================================================
// !! StateEngineCrank cannot update this code if you change the header !!


//---------------------------------------------------------------------------------------------------------------------
/**
 * Process the Ev Disable Event for the Parameter Estimation state machine.
 *
 *              >>> DO NOT MODIFY THIS FUNCTION <<<
 *
 * This function was auto-generated and must not be edited manually.
 * Change the UML used to generate this function and regenerate the source code.
 */
static void EventDisable(void)
{
    switch (Parameter_Estimation_State)
    {
        case STATE_ACTIVE:
            Parameter_Estimation_State = STATE_IDLE;
            break;

        default:
            // Event is ignored for all other values of Parameter_Estimation_State.
            break;
    }
}


//---------------------------------------------------------------------------------------------------------------------
/**
 * Process the Ev Enable Event for the Parameter Estimation state machine.
 *
 *              >>> DO NOT MODIFY THIS FUNCTION <<<
 *
 * This function was auto-generated and must not be edited manually.
 * Change the UML used to generate this function and regenerate the source code.
 */
static void EventEnable(void)
{
    switch (Parameter_Estimation_State)
    {
        case STATE_IDLE:
            Parameter_Estimation_State = STATE_ACTIVE;
            ActiveEnter();                          // STATE_ACTIVE enter function.
            break;

        default:
            // Event is ignored for all other values of Parameter_Estimation_State.
            break;
    }
}


//---------------------------------------------------------------------------------------------------------------------
/**
 * The main execution function for the Parameter Estimation state machine.
 * This function should be called periodically while the state machine is running.
 *
 *              >>> DO NOT MODIFY THIS FUNCTION <<<
 *
 * This function was auto-generated and must not be edited manually.
 * Change the UML used to generate this function and regenerate the source code.
 */
static void ParameterEstimationExecute(void)
{
    switch (Parameter_Estimation_State)
    {
        case STATE_ACTIVE:
            ActiveDo();                             // STATE_ACTIVE do function.
            break;

        default:
            // There is nothing to do for all other values of Parameter_Estimation_State.
            break;
    }
}


//---------------------------------------------------------------------------------------------------------------------
/**
 * The initialization function for the Parameter Estimation state machine.
 * This function resets the state machine to the initial state.
 *
 *              >>> DO NOT MODIFY THIS FUNCTION <<<
 *
 * This function was auto-generated and must not be edited manually.
 * Change the UML used to generate this function and regenerate the source code.
 */
static void ParameterEstimationInitialize(void)
{
    Parameter_Estimation_State = STATE_IDLE;
}


// !! StateEngineCrank cannot update this code if you change the footer !!
// End of StateEngineCrank Auto-Generated Code.
