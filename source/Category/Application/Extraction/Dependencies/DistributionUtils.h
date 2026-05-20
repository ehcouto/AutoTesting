#ifndef DISTUTILS_H_
#define DISTUTILS_H_

//=====================================================================================================================
//-------------------------------------- PUBLIC (Extern Variables, Constants & Defines) -------------------------------
//=====================================================================================================================

#if defined(MATLAB_MEX_FILE)
#include "tmwtypes.h"
#include "simstruc_types.h"
#else
#include "rtwtypes.h"
#endif
#include "C_Extensions.h"

typedef enum
{
    DIST_UTILS_INIT = 0,
	DIST_UTILS_ENABLE_DXDT,
	DIST_UTILS_ENABLE_SATELIZATION,
    DIST_UTILS_DISABLE,
    DIST_UTILS_HANDLER_1MS,
    DIST_UTILS_SET_INPUTS,
    DIST_UTILS_RESET_COUNTERS,
} DIST_UTILS_COMMAND_TYPE;

typedef struct
{
	uint16_T PeakCount;
	uint16_T TroughCount;
	real32_T PeakValue;
	real32_T TroughValue;
} DIST_UTILS_DXDT_OUTPUT_TYPE;

typedef struct
{
  DIST_UTILS_DXDT_OUTPUT_TYPE Dxdt;
  real32_T agreggateP2PTorque;
  real32_T satelizationSpeed;
  real32_T windowPower;
} DIST_UTILS_OUTPUT_TYPE;

//! P2P torque setup structure
typedef PACKED struct
{
	unsigned char SNRD_Sample_Rate_Div_10;	//! SNRD sample rate /10
}P2P_TORQUE_SETUP_TYPE;

//! Satellization detection configuration structure
typedef PACKED struct
{
	float Window_Power_Threshold;				//! The value of Satellization_Window_Average_Power below which
												//! indicates that the clothes have satellized in the drum.
	unsigned char Window_Power_Num_Readings;	//! Number of values of power signal accumulated in each element
												//! of the running summation used to calculate Window Power.
	unsigned char Window_Power_Num_Elements;	//! Number of available elements of the running summation stored in memory.
	unsigned char Max_Velocity;					//! The drum must be spinning less than this value in order to
												//! perform the satellization velocity calculation.
	unsigned char HPF_Selection_Speed;			//! Speed threshold to select which high pass filter to use to calculate
												//! Satellization_Window_Average_Power
} SATELLIZATION_SETUP_TYPE;


//=====================================================================================================================
//-------------------------------------- PUBLIC (Function Prototypes) -------------------------------------------------
//=====================================================================================================================

void DistributionUtils__Initialize(void);
void DistributionUtils__EnableDxdt(void);
void DistributionUtils__EnableSatelization(void);
void DistributionUtils__Disable(void);
void DistributionUtils__Handler1ms(void);
DIST_UTILS_OUTPUT_TYPE DistributionUtils__GetOutputs();
void DistributionUtils__Execute(const uint8_T *command,
	const real32_T *speed_ref,
	const real32_T *torque,
	DIST_UTILS_DXDT_OUTPUT_TYPE *dxdtOutput,
	real32_T *agreggateP2PTorque,
	real32_T *satelizationSpeed,
	real32_T *windowPower);

#endif
