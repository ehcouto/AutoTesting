/*
 * File: Extraction_types.h
 *
 * Code generated for Simulink model 'Extraction'.
 *
 * Model version                  : 1.7461
 * Simulink Coder version         : 8.12 (R2017a) 16-Feb-2017
 * C/C++ source code generated on : Wed Jan 16 10:53:40 2019
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: NXP->Cortex-M4
 * Code generation objective: ROM efficiency
 * Validation result: Not run
 */

#ifndef RTW_HEADER_Extraction_types_h_
#define RTW_HEADER_Extraction_types_h_
#include "DistributionUtils.h"
#include "ParamEstExc.h"
#include "ExtractionExpStructs.h"
#include "ExtractionImpStructs.h"
#include "AccelBaseline.h"
#include "Accelerometer.h"
#include "C_Extensions.h"
#include "ParamEst.h"

/* Includes for objects with custom storage classes. */
#include "Extraction_prm.h"

/*
 * Check that imported macros with storage class "ImportedDefine" are defined
 */
#ifndef EXTR_FEATURE_ACCEL_ENABLED
#error The variable for the parameter "EXTR_FEATURE_ACCEL_ENABLED" is not defined
#endif

#ifndef EXTR_FEATURE_HIGH_SPEED_PE_ENABLED
#error The variable for the parameter "EXTR_FEATURE_HIGH_SPEED_PE_ENABLED" is not defined
#endif

#ifndef EXTR_FEATURE_OPT_RAMP_ENABLED
#error The variable for the parameter "EXTR_FEATURE_OPT_RAMP_ENABLED" is not defined
#endif

/* Model Code Variants */
#ifndef EXTR_VARIANT_ACCELEROMETER_DISABLED
#define EXTR_VARIANT_ACCELEROMETER_DISABLED (EXTR_FEATURE_ACCEL_ENABLED == 0)
#endif

#ifndef EXTR_VARIANT_ACCELEROMETER_ENABLED
#define EXTR_VARIANT_ACCELEROMETER_ENABLED (EXTR_FEATURE_ACCEL_ENABLED == 1)
#endif

#ifndef EXTR_VARIANT_HIGH_SPEED_PE_DISABLED
#define EXTR_VARIANT_HIGH_SPEED_PE_DISABLED (EXTR_FEATURE_HIGH_SPEED_PE_ENABLED == 0)
#endif

#ifndef EXTR_VARIANT_HIGH_SPEED_PE_ENABLED
#define EXTR_VARIANT_HIGH_SPEED_PE_ENABLED (EXTR_FEATURE_HIGH_SPEED_PE_ENABLED == 1)
#endif

#ifndef EXTR_VARIANT_OPT_RAMP_DISABLED
#define EXTR_VARIANT_OPT_RAMP_DISABLED (EXTR_FEATURE_OPT_RAMP_ENABLED == 0)
#endif

#ifndef EXTR_VARIANT_OPT_RAMP_ENABLED
#define EXTR_VARIANT_OPT_RAMP_ENABLED  (EXTR_FEATURE_OPT_RAMP_ENABLED == 1)
#endif

/* Exactly one variant for '<S9>/Extr_Accel_Enabled' should be active */
#if ((EXTR_VARIANT_ACCELEROMETER_DISABLED) ? 1 : 0) + ((EXTR_VARIANT_ACCELEROMETER_ENABLED) ? 1 : 0) != 1
#error Exactly one variant for '<S9>/Extr_Accel_Enabled' should be active
#endif

/* Exactly one variant for '<S61>/Extr_HighSpd_CEM' should be active */
#if ((EXTR_VARIANT_HIGH_SPEED_PE_DISABLED) ? 1 : 0) + ((EXTR_VARIANT_HIGH_SPEED_PE_ENABLED) ? 1 : 0) != 1
#error Exactly one variant for '<S61>/Extr_HighSpd_CEM' should be active
#endif

/* Exactly one variant for '<S61>/Extr_HighSpd_LoadMassLimitCheck' should be active */
#if ((EXTR_VARIANT_HIGH_SPEED_PE_DISABLED) ? 1 : 0) + ((EXTR_VARIANT_HIGH_SPEED_PE_ENABLED) ? 1 : 0) != 1
#error Exactly one variant for '<S61>/Extr_HighSpd_LoadMassLimitCheck' should be active
#endif

/* Exactly one variant for '<S61>/Extr_HighSpd_PlateauLogic' should be active */
#if ((EXTR_VARIANT_HIGH_SPEED_PE_DISABLED) ? 1 : 0) + ((EXTR_VARIANT_HIGH_SPEED_PE_ENABLED) ? 1 : 0) != 1
#error Exactly one variant for '<S61>/Extr_HighSpd_PlateauLogic' should be active
#endif

/* Exactly one variant for '<S61>/Extr_HighSpd_RampLogic' should be active */
#if ((EXTR_VARIANT_HIGH_SPEED_PE_DISABLED) ? 1 : 0) + ((EXTR_VARIANT_HIGH_SPEED_PE_ENABLED) ? 1 : 0) != 1
#error Exactly one variant for '<S61>/Extr_HighSpd_RampLogic' should be active
#endif

/* Exactly one variant for '<S85>/Extr_HighSpd_SupervisorLogic' should be active */
#if ((EXTR_VARIANT_HIGH_SPEED_PE_DISABLED) ? 1 : 0) + ((EXTR_VARIANT_HIGH_SPEED_PE_ENABLED) ? 1 : 0) != 1
#error Exactly one variant for '<S85>/Extr_HighSpd_SupervisorLogic' should be active
#endif

/* Exactly one variant for '<S89>/UpdateSpinPulseConfig' should be active */
#if ((EXTR_VARIANT_HIGH_SPEED_PE_DISABLED) ? 1 : 0) + ((EXTR_VARIANT_HIGH_SPEED_PE_ENABLED) ? 1 : 0) != 1
#error Exactly one variant for '<S89>/UpdateSpinPulseConfig' should be active
#endif

/* Exactly one variant for '<S91>/VerifyVibMapFloorDispSubsystem' should be active */
#if ((EXTR_VARIANT_ACCELEROMETER_DISABLED) ? 1 : 0) + ((EXTR_VARIANT_ACCELEROMETER_ENABLED) ? 1 : 0) != 1
#error Exactly one variant for '<S91>/VerifyVibMapFloorDispSubsystem' should be active
#endif

/* Exactly one variant for '<S121>/Extr_LowSpd_OptRamp' should be active */
#if ((EXTR_VARIANT_OPT_RAMP_DISABLED) ? 1 : 0) + ((EXTR_VARIANT_OPT_RAMP_ENABLED) ? 1 : 0) != 1
#error Exactly one variant for '<S121>/Extr_LowSpd_OptRamp' should be active
#endif

/* Exactly one variant for '<S203>/ResetInitialPulseConfigStates' should be active */
#if ((EXTR_VARIANT_HIGH_SPEED_PE_DISABLED) ? 1 : 0) + ((EXTR_VARIANT_HIGH_SPEED_PE_ENABLED) ? 1 : 0) != 1
#error Exactly one variant for '<S203>/ResetInitialPulseConfigStates' should be active
#endif
#endif                                 /* RTW_HEADER_Extraction_types_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
