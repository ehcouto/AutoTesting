/**
 *  @file
 *
 *  @brief   Extraction hard-coded parameters
 *
 *
 *  @copyright  Copyright 2017  $  Whirlpool Corporation.  All rights reserved - CONFIDENTIAL.
 */

#ifndef EXTRACTIONPARAMS_H
#define EXTRACTIONPARAMS_H

#include "C_Extensions.h"
#include "ExtractionImpStructs.h"
#include "ExtractionExpStructs.h"
#include "DigitalFilter.h"
#include "ParamEst.h"
#include "ParamEstExc.h"
#include "DistributionUtils.h"
#include "MciSensorsWm.h"
#include "Extraction_prm.h"

//=====================================================================================================================
//-------------------------------------- PUBLIC (Extern Variables, Constants & Defines) -------------------------------
//=====================================================================================================================

#define EXTRACTION_LOW_SPEED_PARAMETERS_IN_PLACE  	1

#define EXTRACTION_HIGH_SPEED_PARAMETERS_IN_PLACE 	1

#define EXTRACTION_ACCEL_PARAMETERS_IN_PLACE 		1

#define EXTRACTION_ATHLETE_PARAMETERS_IN_PLACE 		1

#define PE_HARD_CODED_INDEXES_OFFSET				200U

#define EXTR_CONFIG_INVALID_INDEX					255U

#define BASELINE_ARRAY_MAX_SIZE						25U

#define VIB_MAP_REF_ARRAY_MAX_SIZE					25U

//! The setup pointers are dereferenced by the Extraction generated code as external pointers.
extern const EXTRACTION_SETUP_TYPE *GESE_Extr_Setup;
extern const CLEAN_WASHER_SETUP_TYPE *GESE_Extr_CleanWasherSetup;

//! Low Speed Extraction setup
extern const MASS_CONVERSION_COEFFS_TYPE *GESE_Extr_MassConvCoeffs;
extern const UB_CONVERSION_COEFFS_TYPE *GESE_Extr_UnbConvCoeffs;
extern const FDR_SETUP_TYPE *GESE_Extr_FastDistRampSetup;
extern const OPT_RAMP_SETUP_TYPE *GESE_Extr_OptRampSetup;
extern const MDDP_SETUP_TYPE *GESE_Extr_MddpSetup;
extern const P2P_TORQUE_SETUP_TYPE *GESE_Extr_P2PTorqueSetup;
extern const SATELLIZATION_SETUP_TYPE *GESE_Extr_SatellizationSetup;
extern const DISTRIBUTION_PULSE_TYPE *GESE_Extr_DistProf;
extern uint8 GESE_Extr_DistProfPulseCnt;
extern const PARAM_EST_SETUP_INDEX_TYPE *GESE_Extr_ParamEstLowSpeedSetup;
extern DIGITAL_FILTER_SOS_TYPE GESE_Extr_HighPassFilterSetup[2];
extern const uint8 *GESE_Extr_RebalAttemptsArray;
extern const uint8 *GESE_Extr_TargetSpeedArray_Div10;
extern uint8 GESE_Extr_ExtUnbTargetCount;
extern const REBALANCE_TABLE_TYPE *GESE_Extr_RebalTable;
extern uint8 GESE_Extr_RebalTableCount;

//! PRE_LOOKUP data to be used by Low Speed Extraction as external variables.
extern PRE_LOOKUP_TYPE_UINT16 GESE_Extr_FastDistRampTargetSpeed_BP;
extern PRE_LOOKUP_TYPE_UINT16 GESE_Extr_MddpLoadMassX10_BP;
extern PRE_LOOKUP_TYPE_UINT16 GESE_Extr_OptRampLoadMassX100_BP;
extern PRE_LOOKUP_TYPE_UINT16 GESE_Extr_P2PTorqueLoadMassX10_BP;
extern PRE_LOOKUP_TYPE_UINT8 GESE_Extr_RebalAttempts_BP;
extern PRE_LOOKUP_TYPE_UINT8 GESE_Extr_TargetSpeedDiv10_BP;
extern PRE_LOOKUP_TYPE_UINT16 GESE_Extr_WaterTemp_BP;

//! LUT data to be used by Low Speed Extraction as external pointers.
extern const uint8 *GESE_Extr_MddpDuration_LUT;
extern const uint8 *GESE_Extr_MddpRamp_LUT;
extern const uint16 *GESE_Extr_MddpSpeed_LUT;
extern const uint8 *GESE_Extr_OptRampConvTime_LUT;
extern const uint16 *GESE_Extr_OptRampPhaseTolerance_LUT;
extern const uint16 *GESE_Extr_OptRampPhsAdvTgt_LUT;
extern const uint8 *GESE_Extr_OptRampPlateauTime_LUT;
extern const uint16 *GESE_Extr_OptRampUnbBeforeMDDP_LUT;
extern const uint16 *GESE_Extr_OptRampUnbAfterMDDP_LUT;
extern const sint16 *GESE_Extr_OptRampLowerOffsetBeforeMDDP_LUT;
extern const sint16 *GESE_Extr_OptRampLowerOffsetAfterMDDP_LUT;
extern const sint16 *GESE_Extr_OptRampUpperOffsetBeforeMDDP_LUT;
extern const sint16 *GESE_Extr_OptRampUpperOffsetAfterMDDP_LUT;
extern const uint16 *GESE_Extr_P2PTorqueX100_LUT;

extern uint8_T GESE_Extr_OptRampUnbLut_Size[2];

#if (EXTR_FEATURE_HIGH_SPEED_PE_ENABLED != 0)
//! High Speed Extraction setup
extern const HIGH_SPEED_CONFIG_TYPE *GESE_Extr_HighSpdConfig;
extern const BASELINE_FACTORY_CALIBRATION_SETUP_TYPE *GESE_Extr_BaselineFactoryCalibrationSetup;
extern const CEM_SETUP_TYPE *GESE_Extr_CEMSetup;
#endif //(EXTR_FEATURE_HIGH_SPEED_PE_ENABLED != 0)
extern const HIGH_SPEED_UNB_SETUP_TYPE *GESE_Extr_HighSpdUnbConfig;
extern const EXT_UNB_CHK_TARGET_TYPE *GESE_Extr_ExtUnbTargetSet;
extern const EXT_UNB_CHK_SETUP_TYPE *GESE_Extr_ExtUnbSetup;
extern const SPIN_PROFILE_TYPE *GESE_Extr_SpinProfileConfig;
extern const SPIN_PULSE_TYPE *GESE_Extr_SpinPulses;

//! LUT data to be used by High Speed Extraction as external variables.
extern PRE_LOOKUP_TYPE_UINT8 GESE_Extr_BaselineFrictionRefSpeed_BP;
extern PRE_LOOKUP_TYPE_UINT8 GESE_Extr_BaselineFrictionRefSpeed_Sig;
extern const uint16 *GESE_Extr_BaselineDefaultFrictionArray_x1000;
extern const uint8 *GESE_Extr_FactoryFrictionSelIndexArray;
extern const uint16 *GESE_Extr_FactoryCalScalingFactorArray;

//! Accelerometer related setup structs
#if (EXTR_FEATURE_ACCEL_ENABLED != 0)
extern const FLOOR_DISPLACEMENT_SETUP_TYPE *GESE_Extr_FloorDispSetup;
extern const VIB_MAP_FLOOR_DISPL_THRESHOLD_TYPE *GESE_Extr_VibMapFDThreshold;
extern const VIBRATION_MAPPING_SETUP_TYPE *GESE_Extr_VibMapSetup;
extern const CABINET_HIT_SETUP_TYPE *GESE_Extr_CabHitSetup;

//! Vibration Mapping reference speed array
extern PRE_LOOKUP_TYPE_UINT8 GESE_Extr_VibMapRefSpeedArray_div10;
#endif //(EXTR_FEATURE_ACCEL_ENABLED != 0)

//=====================================================================================================================
//-------------------------------------- Public Functions -------------------------------------------------------------
//=====================================================================================================================

PASS_FAIL_TYPE ExtractionParams__Initialize(uint8 extraction_set_index, uint8 vm_fd_threshold_config_index);
void ExtractionParams__SetSettingFileError(void);
void ExtractionParams__ResetSettingFileError(void);
BOOL_TYPE ExtractionParams__GetSettingFileError(void);
BOOL_TYPE ExtractionParams__IsLMSEnabled(void);
uint8 *ExtractionParams__GetBaselineFrictionFactoryArray(void);
void ExtractionParams__SetBaselineFrictionFactoryArray(uint8 *array, uint8 length);
uint8 *ExtractionParams__GetBaselineFrictionLifetimeArray(void);
void ExtractionParams__SetBaselineFrictionLifetimeArray(uint8 *array, uint8 length);
uint8 *ExtractionParams__GetFloorDisplacementRefArray(void);
void ExtractionParams__SetFloorDisplacementRefArray(uint8 *array, uint8 length);
void ExtractionParams__SetVibMapThrshCrossSpd(float32 speed);
float32 ExtractionParams__GetVibMapThrshCrossSpd(void);

//-------------------------------- Public functions used by Extraction ----------------------------------------

PARAM_EST_SETUP_TYPE ExtractionParams__GetParamEstSetup(uint8 index);
PARAM_EST_EXC_SETUP_TYPE ExtractionParams__GetParamEstExcSetup(uint8 index);
PARAM_EST_APP_SPECIFIC_SETUP_TYPE ExtractionParams__GetParamEstAppSpecificSetup(uint8 index);
BOOL_TYPE ExtractionParams__IsExtrConfigValid(void);
HIGH_SPEED_PULSE_CONFIG_TYPE ExtractionParams__GetHighSpdPulseConfig(uint8 index);
HIGH_SPEED_BASELINE_CONFIG_TYPE ExtractionParams__GetHighSpdBaselineConfig(uint8 index);
LOAD_MASS_LIMIT_SETUP_TYPE ExtractionParams__GetLoadMassLimitConfig(uint8 index);
uint16 ExtractionParams__GetBaselineFriction(uint8 index);
void ExtractionParams__SetBaselineFriction(uint8 index, uint16 value);
uint16 ExtractionParams__GetFactoryBaselineFriction(uint8 index);
void ExtractionParams__SetFactoryBaselineFriction(uint8 index, uint16 value);
void ExtractionParams__FilterAccelData(float32 *u, float32 *y);
void ExtractionParams__FilterVibMapAccel(float32 *u, float32 *y);
void ExtractionParams__FilterCabHitAccel(float32 *u, float32 *y);
uint16 ExtractionParams__GetVibMapRefDisp(uint8 index);
void ExtractionParams__SetVibMapRefDisp(uint8 index, uint16 value);
uint8 ExtractionParams__IsCabHitEnabled(void);
uint8 ExtractionParams__IsVibMapEnabled(void);
BOOL_TYPE ExtractionParams__LoadDistributionProfile(uint8 index);

//=====================================================================================================================
//-------------------------------------- Private Functions ------------------------------------------------------------
//=====================================================================================================================

#endif
