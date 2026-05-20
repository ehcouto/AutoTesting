#ifndef EXTRACTION_PRV_H
#define EXTRACTION_PRV_H

#include "BandLimiter.h"
#include "ParamEst.h"
#include "Mci.h"
#include "MciSensorsWm.h"
#include "Motion.h"
#include "DistributionUtils.h"
#include "RemoteFunctionExtraction.h"
#include "MotionDeclare.h"
#include "MciSensorsWm.h"
#include "SpeedMaintain.h"
#if (EXTR_FEATURE_ACCEL_ENABLED == 1)
#include "AccelBaseline.h"
#include "Accelerometer.h"
#endif // (EXTR_FEATURE_ACCEL_ENABLED == 1)

//---------------------------------------------------------------------------------------------------------------------
/**
 * MCI Feedbacks
 */
// Cosine of estimated rotor position
#define EXTRACTION_COS_THETA_INPUT              			(BandLimiter__GetSignal(BANDLIMITER_BAND_0, BANDLIMITER_SIGNAL_COS_DRUM_ANGLE))
// Sine of estimated rotor position
#define EXTRACTION_SIN_THETA_INPUT              			(BandLimiter__GetSignal(BANDLIMITER_BAND_0, BANDLIMITER_SIGNAL_SIN_DRUM_ANGLE))
// Estimated speed input
#define EXTRACTION_SPEED_INPUT                  			(BandLimiter__GetSignal(BANDLIMITER_BAND_0, BANDLIMITER_SIGNAL_DRUM_SPEED_RAD_S)*30.0f/PI)
// Speed reference input
#define EXTRACTION_SPEED_REF_INPUT              			((float32)MciSensorsWm__GetAnalog(MOTOR0, MCISENSORSWM_AI_SPEED_REFERENCE_RPM_S16)/65536.0f)
// Estimated torque input
#define EXTRACTION_TORQUE_INPUT                 			BandLimiter__GetSignal(BANDLIMITER_BAND_0,BANDLIMITER_SIGNAL_DRUM_TORQUE_NM)
// Check if current Mci status is running
#define EXTRACTION_IS_MOTOR_RUNNING()						Mci__GetDigital(MOTOR0,MCI_DI_MOTOR_RUNNING)
// Retrive current Mci fault
#define EXTRACTION_GET_MCI_FAULT()							Mci__GetErrorList(MOTOR0)

//---------------------------------------------------------------------------------------------------------------------
/**
 * Parameter Estimation Feedbacks
 */
// Estimated ball balancer magnitude input
#define EXTRACTION_BB_MAG_INPUT				    			ParamEst__GetParameterValue(PARAM_EST_BBMAG)
// Estimated bb position related to load
#define EXTRACTION_BB_PHASE_DIFF_INPUT		    			ParamEst__GetParameterValue(PARAM_EST_BBPHASELAG)
// Estimated coulomb friction input
#define EXTRACTION_COULOMB_INPUT			    			ParamEst__GetParameterValue(PARAM_EST_COULOMB)
// Estimated inertia from PE
#define EXTRACTION_INERTIA_INPUT         	    			ParamEst__GetParameterValue(PARAM_EST_INERTIA)
// Estimated unbalance from PE
#define EXTRACTION_UB_INPUT              	    			ParamEst__GetParameterValue(PARAM_EST_UBMAG)
// Estimated viscous friction input
#define EXTRACTION_VISCOUS_INPUT			    			ParamEst__GetParameterValue(PARAM_EST_VISCOUS)
// Estimated friction input
#define EXTRACTION_FRICTION_INPUT							ParamEst__GetParameterValue(PARAM_EST_FRICTION)
// Estimated UB angle input
#define EXTRACTION_UB_ANGLE_INPUT							ParamEst__GetParameterValue(PARAM_EST_UBPHASE)

//---------------------------------------------------------------------------------------------------------------------
/**
 * Ouptut from DistributionUtils module
 */
#define GET_DISTRIBUTION_UTILS_OUTPUT		    			DistributionUtils__GetOutputs()

//---------------------------------------------------------------------------------------------------------------------
/**
 * Motion Commands
 */
// Call motor control stop command
#define EXTRACTION_STOP_MOTOR(arg)							Motion__Activity(MOTION_ACTIVITY_STOP,arg,MOTION_REQUESTOR_EXTRACTION); \
															SpeedMaintain__ReleaseMotion();
// Call motor control spin command
#define EXTRACTION_SPIN_COMMAND(arg)						Motion__Activity(MOTION_ACTIVITY_SPIN,arg,MOTION_REQUESTOR_EXTRACTION); \
															SpeedMaintain__SetSpinActivity(arg);
// Call the tumble command
#define EXTRACTION_TUMBLE_COMMAND(arg)						Motion__Activity(MOTION_ACTIVITY_WASH,arg,MOTION_REQUESTOR_EXTRACTION)
// Release the motion module
#define EXTRACTION_RELEASE_MOTION()							Motion__Release(MOTION_REQUESTOR_EXTRACTION)

//---------------------------------------------------------------------------------------------------------------------
/**
 * Unbalance measurement from Indesit legacy logic
 */
#define EXTRACTION_UNBALANCE_INDEX_INPUT					(uint16)(MciSensorsWm__GetAnalog(MOTOR0, MCISENSORSWM_AI_BALANCE)>>16)
// Number of unbalance measurements taken in the current speed plateau
#define EXTRACTION_UNBALANCE_SAMPLE_COUNT_INPUT				(uint16)MciSensorsWm__GetAnalog(MOTOR0, MCISENSORSWM_AI_BALANCE_COUNT)

//---------------------------------------------------------------------------------------------------------------------
/**
 * ACU Feedbacks
 */
// Minimum speed to be reached
#define EXTRACTION_MIN_FINAL_SPEED							RemoteFunctionExtraction__HSVelocityMinReq()
// Final Plateau Duration in seconds
#define EXTRACTION_FINAL_PLATEAU_DURATION					RemoteFunctionExtraction__HSExtdPlateauDur()
// Maximum speed error percentage
#define EXTRACTION_MAX_SPEED_ERROR							RemoteFunctionExtraction__ParDrumTargVelErr()
// Ramp timeout in seconds
#define EXTRACTION_RAMP_TIMEOUT								RemoteFunctionExtraction__ParMaxAllowRampTime()
// Saturated load mass
#define EXTRACTION_SATURATED_LOAD_MASS_INPUT				RemoteFunctionExtraction__GetLoadMass()
// Water temperature
#define EXTRACTION_WATER_TEMP_INPUT							RemoteFunctionExtraction__GetTemperature()
// Final target speed
#define EXTRACTION_FINAL_SPEED				    			RemoteFunctionExtraction__HSVelocityMaxLim()

//---------------------------------------------------------------------------------------------------------------------
/**
 * Accelerometer Feedbacks
 */
#if (EXTR_FEATURE_ACCEL_ENABLED == 1)
	// Retrieve accelerometer baseline (static acceleration)
	#define	EXTRACTION_GET_ACCELEROMETER_BASELINE(arg)			AccelBaseline__GetBaseline(arg)
	// Retrieve accelerometer raw output (x, y, z)
	#define EXTRACTION_GET_ACCELEROMETER_RAW_DATA(arg)			Accelerometer__Read(arg)
	// Stop data acquisition from accelerometer
	#define EXTRACTION_DISABLE_ACCELEROMETER()					Accelerometer__Disable()
	// Start data acquisition from accelerometer at 400Hz
	#define EXTRACTION_ENABLE_ACCELEROMETER()					Accelerometer__Enable(ACCELEROMETER_MODE_400HZ)
	// Retrieve accelerometer status
	#define EXTRACTION_GET_ACCEL_STATUS()						Accelerometer__GetStatus()
	// Retrieve accel baseline status
	#define EXTRACTION_GET_ACCEL_BASELINE_STATUS()				AccelBaseline__GetStatus()
	// Start accel baseline measurement
	#define EXTRACTION_MEASURE_ACCEL_BASELINE(arg)				AccelBaseline__MeasureBaseline()
#else
	// Retrieve accelerometer baseline (static acceleration)
	#define	EXTRACTION_GET_ACCELEROMETER_BASELINE(arg)          0
	// Retrieve accelerometer raw output (x, y, z)
	#define EXTRACTION_GET_ACCELEROMETER_RAW_DATA(arg)          0
	// Stop data acquisition from accelerometer
	#define EXTRACTION_DISABLE_ACCELEROMETER()                  0
	// Start data acquisition from accelerometer at 400Hz
	#define EXTRACTION_ENABLE_ACCELEROMETER()                   0
	// Retrieve accelerometer status
	#define EXTRACTION_GET_ACCEL_STATUS()                       0
	// Retrieve accel baseline status
	#define EXTRACTION_GET_ACCEL_BASELINE_STATUS()              0
	// Start accel baseline measurement
	#define EXTRACTION_MEASURE_ACCEL_BASELINE(arg)              0
#endif //EXTR_FEATURE_ACCEL_ENABLED == 1)

#endif
