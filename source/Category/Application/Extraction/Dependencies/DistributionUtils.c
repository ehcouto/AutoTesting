//=====================================================================================================================
//-------------------------------------- PUBLIC (Extern Variables, Constants & Defines) -------------------------------
//=====================================================================================================================

#include <string.h>
#include "AggregateP2PTorque.h"
#include "AverageTorque.h"
#include "Dxdt.h"
#include "Satelization.h"
#include "WindowPower.h"
#if defined(MATLAB_MEX_FILE)
#include "MotorInterface.h"
#include "ExtractionParams.h"
#endif
#include "DistributionUtils.h"
#include "AverageTorque_prv.h"

//-------------------------------------- PRIVATE (Variables, Constants & Defines) -------------------------------------

static uint8_T Handler5msPrescaler;
static DIST_UTILS_DXDT_OUTPUT_TYPE DxdtOutput;

//-------------------------------------- PRIVATE (Function Prototypes) ------------------------------------------------

static void DxdtEventHandler(DXDT_EVENT_TYPE event);

//=====================================================================================================================
//-------------------------------------- Public Functions -------------------------------------------------------------
//=====================================================================================================================

void DistributionUtils__Initialize(void)
{
    #if defined(MATLAB_MEX_FILE)
        Extraction__InitializeParams(0);
    #endif

	Handler5msPrescaler = 0;
	memset(&DxdtOutput, 0, sizeof(DIST_UTILS_DXDT_OUTPUT_TYPE));

	WindowPower__Initialize();
	Satelization__Initialize();
	AggregateP2PTorque__Initialize();
	Dxdt__Initialize();
	AverageTorque__Initialize();
}

void DistributionUtils__EnableDxdt(void)
{
	Dxdt__RegisterEventHandler(&DxdtEventHandler);
	AggregateP2PTorque__Enable();
	AverageTorque__Enable();
	Dxdt__Enable();
}

void DistributionUtils__EnableSatelization(void)
{
	WindowPower__Enable();
	Satelization__Disable();
	Satelization__Enable();
}

void DistributionUtils__Disable(void)
{
	AggregateP2PTorque__Clear();
	AggregateP2PTorque__Disable();
	AverageTorque__Disable();
	Dxdt__Disable();
	WindowPower__Disable();
	Satelization__Disable();
	memset(&DxdtOutput, 0, sizeof(DIST_UTILS_DXDT_OUTPUT_TYPE));
}

void DistributionUtils__Handler1ms(void)
{
	AverageTorque__Handler1msec();
	if (Handler5msPrescaler == 0)
	{
		AverageTorque__Handler5msec();
		Handler5msPrescaler = 4;
	}
	else
		Handler5msPrescaler--;
}

DIST_UTILS_OUTPUT_TYPE DistributionUtils__GetOutputs()
{
	DIST_UTILS_OUTPUT_TYPE du;
	memcpy(&du,&DxdtOutput,sizeof(DIST_UTILS_DXDT_OUTPUT_TYPE));
	du.agreggateP2PTorque = (real32_T)AggregateP2PTorque__GetValue()/128.0f;
	du.satelizationSpeed = (real32_T)Satelization__GetRpmX16()/16.0f;
	du.windowPower = (real32_T)WindowPower__GetValue();
	return du;
}

void DistributionUtils__Execute(const uint8_T *command,
	const real32_T *speed_ref,
	const real32_T *torque,
	DIST_UTILS_DXDT_OUTPUT_TYPE *dxdtOutput,
	real32_T *agreggateP2PTorque,
	real32_T *satelizationSpeed,
	real32_T *windowPower)
{
	DIST_UTILS_COMMAND_TYPE com = (DIST_UTILS_COMMAND_TYPE)command[0];

	switch (com)
	{
	case DIST_UTILS_INIT:
		DistributionUtils__Initialize();
		break;
	case DIST_UTILS_ENABLE_DXDT:
		DistributionUtils__EnableDxdt();
		break;
	case DIST_UTILS_ENABLE_SATELIZATION:
		DistributionUtils__EnableSatelization();
		break;
	case DIST_UTILS_DISABLE:
		DistributionUtils__Disable();
		break;
	case DIST_UTILS_HANDLER_1MS:
		DistributionUtils__Handler1ms();
		break;
#if defined(MATLAB_MEX_FILE)
	case DIST_UTILS_SET_INPUTS:
		MotorInterface__SetTorque(torque[0]);
		MotorInterface__SetSpeedRef(speed_ref[0]);
		break;
#endif
	case DIST_UTILS_RESET_COUNTERS:
		DxdtOutput.PeakCount = 0;
		DxdtOutput.TroughCount = 0;
		break;
	}

#if defined(MATLAB_MEX_FILE)
	dxdtOutput[0] = DxdtOutput;
	agreggateP2PTorque[0] = (real32_T)AggregateP2PTorque__GetValue()/128.0f;
	satelizationSpeed[0] = (real32_T)Satelization__GetRpmX16()/16.0f;
	windowPower[0] = WindowPower__GetValue();
#endif
}

//=====================================================================================================================
//-------------------------------------- Private Functions ------------------------------------------------------------
//=====================================================================================================================

static void DxdtEventHandler(DXDT_EVENT_TYPE event)
{
	DXDT_TORQUE_DATA_TYPE dxdt;
	Dxdt__GetData(&dxdt);
	switch (event)
	{
	case DXDT_EV_MAX_TORQUE_DETECTED:
		// peak detected
		DxdtOutput.PeakCount++;
		DxdtOutput.PeakValue = (real32_T)dxdt.Max/128.0f;
		break;
	case DXDT_EV_MIN_TORQUE_DETECTED:
		// trough detected            
		DxdtOutput.TroughCount++;
		DxdtOutput.TroughValue = (real32_T)dxdt.Min/128.0f;
		break;
	default:
		break;
	}
}
