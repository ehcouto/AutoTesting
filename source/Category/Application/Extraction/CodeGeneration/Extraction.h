/*
 * File: Extraction.h
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

#ifndef RTW_HEADER_Extraction_h_
#define RTW_HEADER_Extraction_h_
#include <math.h>
#include <stddef.h>
#ifndef Extraction_COMMON_INCLUDES_
# define Extraction_COMMON_INCLUDES_
#include "rtwtypes.h"
#include "zero_crossing_types.h"
#include "ExtractionParams.h"
#include "Extraction_prv.h"
#include "Motion.h"
#endif                                 /* Extraction_COMMON_INCLUDES_ */

#include "Extraction_types.h"

/* Child system includes */
#include "Extraction.h"
#include "Extraction.h"
#include "Extraction.h"
#include "Extraction.h"
#include "Extraction.h"
#include "Extraction.h"
#include "Extraction.h"
#include "Extraction.h"
#include "Extraction.h"
#include "Extraction.h"
#include "Extraction.h"
#include "Extraction__ClearFault.h"
#include "Extraction__GetFault.h"
#include "Extraction__GetFloat.h"
#include "Extraction__GetInteger.h"
#include "Extraction__GetState.h"
#include "Extraction__Initialize.h"
#include "Extraction__IsRebInProgress.h"
#include "Extraction.h"
#include "Extraction.h"
#include "Extraction.h"
#include "Extraction.h"
#include "Extraction.h"
#include "Extraction.h"
#include "Extraction.h"
#include "Extraction.h"
#include "Extraction.h"
#include "Extraction.h"
#include "Extraction.h"
#include "Extraction.h"
#include "Extraction.h"
#include "Extraction.h"
#include "Extraction.h"
#include "Extraction.h"
#include "Extraction.h"
#include "Extraction.h"
#include "Extraction.h"
#include "Extraction.h"
#include "Extraction.h"
#include "Extraction.h"
#include "Extraction.h"
#include "Extraction.h"
#include "Extraction.h"
#include "Extraction.h"
#include "Extraction.h"

/* Includes for objects with custom storage classes. */
#include "ExtractionParams.h"
#include "Extraction_prm.h"

/* Macros for accessing real-time model data structure */

/* user code (top of header file) */
#include "ParamEst.h"
#include "ParamEstExc.h"
#include "DistributionUtils.h"
#include "Motion.h"
#include "ExtractionImpStructs.h"
#include "Accelerometer.h"
#include "AccelBaseline.h"

/* System '<Root>/Initialize' */
#include <string.h>

/* Block signals for system '<S204>/TriggerGen' */
typedef struct
{
    boolean_T trigger;                 /* '<S204>/TriggerGen' */
}
Extraction_B_TriggerGen_TYPE;

/* Block states (auto storage) for system '<S204>/TriggerGen' */
typedef struct
{
    uint32 triggerEventCounter;        /* '<S204>/TriggerGen' */
}
Extraction_DW_TriggerGen_TYPE;

/* Block signals for system '<S122>/UpdatePEAppSpecificSetup' */
typedef struct
{
    PARAM_EST_APP_SPECIFIC_SETUP_TYPE PEAppSpecificConfig;/* '<S175>/GetPEAppSpecificConfig' */
}
Extraction_B_UpdatePEAppSpecificSetup_TYPE;

/* Block signals for system '<S127>/Extr_LowSpd_OptRamp_Enabled' */
#if EXTR_VARIANT_OPT_RAMP_ENABLED

typedef struct
{
    V_THRESHOLD_TYPE thDefinition;     /* '<S153>/CalcOffsetsAndSlopes' */
    float32 mass;                      /* '<S150>/OptRampLogic' */
    float32 temp;                      /* '<S150>/OptRampLogic' */
    float32 waterTemp;                 /* '<S150>/OptRampLogic' */
    float32 Gain1;                     /* '<S154>/Gain1' */
    float32 Gain1_id;                  /* '<S155>/Gain1' */
    uint16 speedTarget;                /* '<S150>/OptRampLogic' */
    uint16 fs;                         /* '<S150>/OptRampLogic' */
    uint8 rebalAttempts;               /* '<S150>/OptRampLogic' */
    uint8 TempToPlateauTimeLUT;        /* '<S152>/TempToPlateauTimeLUT' */
    boolean_T mddpExecuted;            /* '<S150>/OptRampLogic' */
}
Extraction_B_Extr_LowSpd_OptRamp_Enabled_TYPE;

#endif                                 /* EXTR_VARIANT_OPT_RAMP_ENABLED */

/* Block states (auto storage) for system '<S127>/Extr_LowSpd_OptRamp_Enabled' */
#if EXTR_VARIANT_OPT_RAMP_ENABLED

typedef struct
{
    V_THRESHOLD_TYPE thSpec;           /* '<S150>/OptRampLogic' */
    sint32 sfEvent;                    /* '<S150>/OptRampLogic' */
    uint32 evOptRampEventCounter;      /* '<S150>/OptRampLogic' */
    uint32 evRedistEventCounter;       /* '<S150>/OptRampLogic' */
    uint32 temporalCounter_i1;         /* '<S150>/OptRampLogic' */
    uint8 is_c13_Extraction;           /* '<S150>/OptRampLogic' */
    uint8 is_OPT_RAMP;                 /* '<S150>/OptRampLogic' */
    uint8 is_WAITING_OPT_RAMP;         /* '<S150>/OptRampLogic' */
}
Extraction_DW_Extr_LowSpd_OptRamp_Enabled_TYPE;

#endif                                 /* EXTR_VARIANT_OPT_RAMP_ENABLED */

/* Zero-crossing (trigger) state for system '<S127>/Extr_LowSpd_OptRamp_Enabled' */
#if EXTR_VARIANT_OPT_RAMP_ENABLED

typedef struct
{
    ZCSigState OptRampLogic_Trig_ZCE[3];/* '<S150>/OptRampLogic' */
}
Extraction_ZCE_Extr_LowSpd_OptRamp_Enabled_TYPE;

#endif                                 /* EXTR_VARIANT_OPT_RAMP_ENABLED */

/* Block signals for system '<S86>/Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled' */
#if EXTR_VARIANT_HIGH_SPEED_PE_DISABLED

typedef struct
{
    HIGH_SPEED_STATE Extr_HighSpd_State_lz;/* '<S92>/Extr_HighSpd_SupervisorLogic' */
    uint16 Extr_HighSpd_TargetSpeed_ln;/* '<S92>/Extr_HighSpd_SupervisorLogic' */
}
Extraction_B_Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled_TYPE;

#endif                                 /* EXTR_VARIANT_HIGH_SPEED_PE_DISABLED */

/* Block states (auto storage) for system '<S86>/Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled' */
#if EXTR_VARIANT_HIGH_SPEED_PE_DISABLED

typedef struct
{
    SPIN_PULSE_TYPE SpinPulse;         /* '<S92>/Extr_HighSpd_SupervisorLogic' */
    sint32 sfEvent;                    /* '<S92>/Extr_HighSpd_SupervisorLogic' */
    uint32 Extr_HighSpd_EvStartPlateauEventCounter;/* '<S92>/Extr_HighSpd_SupervisorLogic' */
    uint32 Extr_HighSpd_EvStartRampEventCounter;/* '<S92>/Extr_HighSpd_SupervisorLogic' */
    uint32 Extr_HighSpd_EvCompleteEventCounter;/* '<S92>/Extr_HighSpd_SupervisorLogic' */
    uint32 Extr_HighSpd_EvStartRebalEventCounter;/* '<S92>/Extr_HighSpd_SupervisorLogic' */
    uint32 Extr_HighSpd_EvStopAllEventCounter;/* '<S92>/Extr_HighSpd_SupervisorLogic' */
    uint32 Extr_HighSpd_EvDropToDistSpeedEventCounter;/* '<S92>/Extr_HighSpd_SupervisorLogic' */
    uint32 temporalCounter_i1;         /* '<S92>/Extr_HighSpd_SupervisorLogic' */
    uint16 ExtPlateauDuration;         /* '<S92>/Extr_HighSpd_SupervisorLogic' */
    uint8 is_active_c48_Extraction;    /* '<S92>/Extr_HighSpd_SupervisorLogic' */
    uint8 is_c48_Extraction;           /* '<S92>/Extr_HighSpd_SupervisorLogic' */
    uint8 is_HIGH_SPEED_SUPERVISOR;    /* '<S92>/Extr_HighSpd_SupervisorLogic' */
    uint8 is_RUNNING;                  /* '<S92>/Extr_HighSpd_SupervisorLogic' */
}
Extraction_DW_Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled_TYPE;

#endif                                 /* EXTR_VARIANT_HIGH_SPEED_PE_DISABLED */

/* Zero-crossing (trigger) state for system '<S86>/Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled' */
#if EXTR_VARIANT_HIGH_SPEED_PE_DISABLED

typedef struct
{
    ZCSigState Extr_HighSpd_SupervisorLogic_Trig_ZCE_cb[19];/* '<S92>/Extr_HighSpd_SupervisorLogic' */
}
Extraction_ZCE_Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled_TYPE;

#endif                                 /* EXTR_VARIANT_HIGH_SPEED_PE_DISABLED */

/* Block signals for system '<S86>/Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled' */
#if EXTR_VARIANT_HIGH_SPEED_PE_ENABLED

typedef struct
{
    boolean_T zcEvent[19];
    SPIN_PULSE_TYPE r0;
    SPIN_PULSE_TYPE r1;
    SPIN_PULSE_TYPE r2;
    uint8 uv0[8];
    float32 unusedExpr;
    sint32 inputEventFiredFlag;
    HIGH_SPEED_STATE Extr_HighSpd_State_la;/* '<S93>/Extr_HighSpd_SupervisorLogic' */
    sint16 i0;
    uint16 Extr_HighSpd_TargetSpeed_ep;/* '<S93>/Extr_HighSpd_SupervisorLogic' */
    uint16 VibMapTargSpd;
    sint8 rtb_inputevents_hu_idx_1;
    sint8 rtb_inputevents_hu_idx_2;
    sint8 rtb_inputevents_hu_idx_3;
    sint8 rtb_inputevents_hu_idx_4;
    sint8 rtb_inputevents_hu_idx_5;
    sint8 rtb_inputevents_hu_idx_6;
    sint8 rtb_inputevents_hu_idx_7;
    sint8 rtb_inputevents_hu_idx_8;
    sint8 rtb_inputevents_hu_idx_9;
    sint8 rtb_inputevents_hu_idx_10;
    sint8 rtb_inputevents_hu_idx_11;
    sint8 rtb_inputevents_hu_idx_12;
    sint8 rtb_inputevents_hu_idx_13;
    sint8 rtb_inputevents_hu_idx_14;
    sint8 rtb_inputevents_hu_idx_15;
    sint8 rtb_inputevents_hu_idx_16;
    sint8 rtb_inputevents_hu_idx_17;
    sint8 rtb_inputevents_hu_idx_18;
    boolean_T b0;
}
Extraction_B_Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled_TYPE;

#endif                                 /* EXTR_VARIANT_HIGH_SPEED_PE_ENABLED */

/* Block states (auto storage) for system '<S86>/Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled' */
#if EXTR_VARIANT_HIGH_SPEED_PE_ENABLED

typedef struct
{
    SPIN_PULSE_TYPE SpinPulse;         /* '<S93>/Extr_HighSpd_SupervisorLogic' */
    float32 ExtPlateauElapsedTime;     /* '<S93>/Extr_HighSpd_SupervisorLogic' */
    float32 PlateauDuration;           /* '<S93>/Extr_HighSpd_SupervisorLogic' */
    sint32 sfEvent;                    /* '<S93>/Extr_HighSpd_SupervisorLogic' */
    uint32 Extr_HighSpd_EvStartPlateauEventCounter;/* '<S93>/Extr_HighSpd_SupervisorLogic' */
    uint32 Extr_HighSpd_EvStartRampEventCounter;/* '<S93>/Extr_HighSpd_SupervisorLogic' */
    uint32 Extr_HighSpd_EvCompleteEventCounter;/* '<S93>/Extr_HighSpd_SupervisorLogic' */
    uint32 Extr_HighSpd_EvStartRebalEventCounter;/* '<S93>/Extr_HighSpd_SupervisorLogic' */
    uint32 Extr_HighSpd_EvStopAllEventCounter;/* '<S93>/Extr_HighSpd_SupervisorLogic' */
    uint32 Extr_HighSpd_EvDropToDistSpeedEventCounter;/* '<S93>/Extr_HighSpd_SupervisorLogic' */
    HIGH_SPEED_PLATEAU_DECISION PlateauDecision;/* '<S93>/Extr_HighSpd_SupervisorLogic' */
    uint16 ExtPlateauDuration;         /* '<S93>/Extr_HighSpd_SupervisorLogic' */
    uint16 PrevTargetSpeed;            /* '<S93>/Extr_HighSpd_SupervisorLogic' */
    uint8 is_active_c17_Extraction;    /* '<S93>/Extr_HighSpd_SupervisorLogic' */
    uint8 is_c17_Extraction;           /* '<S93>/Extr_HighSpd_SupervisorLogic' */
    uint8 is_HIGH_SPEED_SUPERVISOR;    /* '<S93>/Extr_HighSpd_SupervisorLogic' */
    uint8 is_RUNNING;                  /* '<S93>/Extr_HighSpd_SupervisorLogic' */
    uint8 is_EXTENDED_PLATEAU;         /* '<S93>/Extr_HighSpd_SupervisorLogic' */
    uint8 Extr_HighSpd_RebalCntArray[25];/* '<S93>/Extr_HighSpd_SupervisorLogic' */
    boolean_T DeltaSpeedDropExecuted;  /* '<S93>/Extr_HighSpd_SupervisorLogic' */
    boolean_T FinalPlateauVerificationRequired;/* '<S93>/Extr_HighSpd_SupervisorLogic' */
    boolean_T ExtPlateauDurationIncreased;/* '<S93>/Extr_HighSpd_SupervisorLogic' */
    boolean_T DecPlatTime;             /* '<S93>/Extr_HighSpd_SupervisorLogic' */
}
Extraction_DW_Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled_TYPE;

#endif                                 /* EXTR_VARIANT_HIGH_SPEED_PE_ENABLED */

/* Zero-crossing (trigger) state for system '<S86>/Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled' */
#if EXTR_VARIANT_HIGH_SPEED_PE_ENABLED

typedef struct
{
    ZCSigState Extr_HighSpd_SupervisorLogic_Trig_ZCE[19];/* '<S93>/Extr_HighSpd_SupervisorLogic' */
}
Extraction_ZCE_Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled_TYPE;

#endif                                 /* EXTR_VARIANT_HIGH_SPEED_PE_ENABLED */

/* Block states (auto storage) for system '<S108>/UpdateSpinPulseConfig_Enabled' */
#if EXTR_VARIANT_HIGH_SPEED_PE_ENABLED

typedef struct
{
    HIGH_SPEED_PULSE_CONFIG_TYPE PrevPulseConfig_DSTATE;/* '<S109>/PrevPulseConfig' */
    LOAD_MASS_LIMIT_SETUP_TYPE PrevLoadMassLimitConfig_DSTATE;/* '<S109>/PrevLoadMassLimitConfig' */
    float32 PrevBaselineFriction_DSTATE;/* '<S109>/PrevBaselineFriction' */
}
Extraction_DW_UpdateSpinPulseConfig_Enabled_TYPE;

#endif                                 /* EXTR_VARIANT_HIGH_SPEED_PE_ENABLED */

/* Block states (auto storage) for system '<S63>/Extr_HighSpd_CEM_Enabled' */
#if EXTR_VARIANT_HIGH_SPEED_PE_ENABLED

typedef struct
{
    float32 FrictionSum;               /* '<S69>/Extr_HighSpd_CEMLogic' */
    float32 FrictionWindowAvgBuf[10];  /* '<S69>/Extr_HighSpd_CEMLogic' */
    sint32 sfEvent;                    /* '<S69>/Extr_HighSpd_CEMLogic' */
    uint32 Extr_HighSpd_EvCEMFailureDetectedEventCounter;/* '<S69>/Extr_HighSpd_CEMLogic' */
    uint32 temporalCounter_i2;         /* '<S69>/Extr_HighSpd_CEMLogic' */
    uint16 FrictionWindowSampleCount;  /* '<S69>/Extr_HighSpd_CEMLogic' */
    uint16 temporalCounter_i1;         /* '<S69>/Extr_HighSpd_CEMLogic' */
    uint8 is_active_c21_Extraction;    /* '<S69>/Extr_HighSpd_CEMLogic' */
    uint8 is_c21_Extraction;           /* '<S69>/Extr_HighSpd_CEMLogic' */
    uint8 is_MONITORING;               /* '<S69>/Extr_HighSpd_CEMLogic' */
    uint8 is_active_MONITORING;        /* '<S69>/Extr_HighSpd_CEMLogic' */
    uint8 is_CALCULATING_FRICTION;     /* '<S69>/Extr_HighSpd_CEMLogic' */
    uint8 is_active_CALCULATING_FRICTION;/* '<S69>/Extr_HighSpd_CEMLogic' */
    uint8 BufferIndex;                 /* '<S69>/Extr_HighSpd_CEMLogic' */
}
Extraction_DW_Extr_HighSpd_CEM_Enabled_TYPE;

#endif                                 /* EXTR_VARIANT_HIGH_SPEED_PE_ENABLED */

/* Zero-crossing (trigger) state for system '<S63>/Extr_HighSpd_CEM_Enabled' */
#if EXTR_VARIANT_HIGH_SPEED_PE_ENABLED

typedef struct
{
    ZCSigState Extr_HighSpd_CEMLogic_Trig_ZCE[2];/* '<S69>/Extr_HighSpd_CEMLogic' */
}
Extraction_ZCE_Extr_HighSpd_CEM_Enabled_TYPE;

#endif                                 /* EXTR_VARIANT_HIGH_SPEED_PE_ENABLED */

/* Block states (auto storage) for system '<S65>/Extr_HighSpd_LoadMassLimitCheck_Enabled' */
#if EXTR_VARIANT_HIGH_SPEED_PE_ENABLED

typedef struct
{
    uint32 Extr_HighSpd_EvLoadMassExceededEventCounter;/* '<S72>/Extr_HighSpd_LoadMassLimitCheckLogic' */
    uint32 temporalCounter_i1;         /* '<S72>/Extr_HighSpd_LoadMassLimitCheckLogic' */
    uint8 is_active_c36_Extraction;    /* '<S72>/Extr_HighSpd_LoadMassLimitCheckLogic' */
    uint8 is_c36_Extraction;           /* '<S72>/Extr_HighSpd_LoadMassLimitCheckLogic' */
}
Extraction_DW_Extr_HighSpd_LoadMassLimitCheck_Enabled_TYPE;

#endif                                 /* EXTR_VARIANT_HIGH_SPEED_PE_ENABLED */

/* Zero-crossing (trigger) state for system '<S65>/Extr_HighSpd_LoadMassLimitCheck_Enabled' */
#if EXTR_VARIANT_HIGH_SPEED_PE_ENABLED

typedef struct
{
    ZCSigState Extr_HighSpd_LoadMassLimitCheckLogic_Trig_ZCE[2];/* '<S72>/Extr_HighSpd_LoadMassLimitCheckLogic' */
}
Extraction_ZCE_Extr_HighSpd_LoadMassLimitCheck_Enabled_TYPE;

#endif                                 /* EXTR_VARIANT_HIGH_SPEED_PE_ENABLED */

/* Block signals for system '<S66>/Extr_HighSpd_PlateauLogic_Disabled' */
#if EXTR_VARIANT_HIGH_SPEED_PE_DISABLED

typedef struct
{
    HIGH_SPEED_PLATEAU_DECISION Extr_HighSpd_PlateauDecision_m0;/* '<S74>/Extr_HighSpd_PlateauLogic' */
}
Extraction_B_Extr_HighSpd_PlateauLogic_Disabled_TYPE;

#endif                                 /* EXTR_VARIANT_HIGH_SPEED_PE_DISABLED */

/* Block states (auto storage) for system '<S66>/Extr_HighSpd_PlateauLogic_Disabled' */
#if EXTR_VARIANT_HIGH_SPEED_PE_DISABLED

typedef struct
{
    sint32 sfEvent;                    /* '<S74>/Extr_HighSpd_PlateauLogic' */
    uint32 Extr_HighSpd_EvPlateauCompleteEventCounter;/* '<S74>/Extr_HighSpd_PlateauLogic' */
    uint32 temporalCounter_i1;         /* '<S74>/Extr_HighSpd_PlateauLogic' */
    uint8 is_active_c50_Extraction;    /* '<S74>/Extr_HighSpd_PlateauLogic' */
    uint8 is_c50_Extraction;           /* '<S74>/Extr_HighSpd_PlateauLogic' */
}
Extraction_DW_Extr_HighSpd_PlateauLogic_Disabled_TYPE;

#endif                                 /* EXTR_VARIANT_HIGH_SPEED_PE_DISABLED */

/* Zero-crossing (trigger) state for system '<S66>/Extr_HighSpd_PlateauLogic_Disabled' */
#if EXTR_VARIANT_HIGH_SPEED_PE_DISABLED

typedef struct
{
    ZCSigState Extr_HighSpd_PlateauLogic_Trig_ZCE_lg[5];/* '<S74>/Extr_HighSpd_PlateauLogic' */
}
Extraction_ZCE_Extr_HighSpd_PlateauLogic_Disabled_TYPE;

#endif                                 /* EXTR_VARIANT_HIGH_SPEED_PE_DISABLED */

/* Block signals for system '<S66>/Extr_HighSpd_PlateauLogic_Enabled' */
#if EXTR_VARIANT_HIGH_SPEED_PE_ENABLED

typedef struct
{
    float32 friction;                  /* '<S75>/Extr_HighSpd_PlateauLogic' */
    HIGH_SPEED_PLATEAU_DECISION Extr_HighSpd_PlateauDecision_pf;/* '<S75>/Extr_HighSpd_PlateauLogic' */
    uint8 baseline_index;              /* '<S75>/Extr_HighSpd_PlateauLogic' */
}
Extraction_B_Extr_HighSpd_PlateauLogic_Enabled_TYPE;

#endif                                 /* EXTR_VARIANT_HIGH_SPEED_PE_ENABLED */

/* Block states (auto storage) for system '<S66>/Extr_HighSpd_PlateauLogic_Enabled' */
#if EXTR_VARIANT_HIGH_SPEED_PE_ENABLED

typedef struct
{
    float32 FrictionSum;               /* '<S75>/Extr_HighSpd_PlateauLogic' */
    float32 Extr_HighSpd_FrictionWindowAvgBuf[10];/* '<S75>/Extr_HighSpd_PlateauLogic' */
    float32 FactoryCalibrationFrictionSum;/* '<S75>/Extr_HighSpd_PlateauLogic' */
    sint32 sfEvent;                    /* '<S75>/Extr_HighSpd_PlateauLogic' */
    uint32 Extr_HighSpd_EvPlateauCompleteEventCounter;/* '<S75>/Extr_HighSpd_PlateauLogic' */
    uint32 temporalCounter_i1;         /* '<S75>/Extr_HighSpd_PlateauLogic' */
    uint32 temporalCounter_i2;         /* '<S75>/Extr_HighSpd_PlateauLogic' */
    uint32 temporalCounter_i3;         /* '<S75>/Extr_HighSpd_PlateauLogic' */
    uint16 FrictionWindowSampleCount;  /* '<S75>/Extr_HighSpd_PlateauLogic' */
    uint8 is_active_c3_Extraction;     /* '<S75>/Extr_HighSpd_PlateauLogic' */
    uint8 is_c3_Extraction;            /* '<S75>/Extr_HighSpd_PlateauLogic' */
    uint8 is_MONITOR;                  /* '<S75>/Extr_HighSpd_PlateauLogic' */
    uint8 is_active_MONITOR;           /* '<S75>/Extr_HighSpd_PlateauLogic' */
    uint8 is_FRICTION_AVERAGE;         /* '<S75>/Extr_HighSpd_PlateauLogic' */
    uint8 is_active_FRICTION_AVERAGE;  /* '<S75>/Extr_HighSpd_PlateauLogic' */
    uint8 is_PLATEAU_TIME;             /* '<S75>/Extr_HighSpd_PlateauLogic' */
    uint8 is_active_PLATEAU_TIME;      /* '<S75>/Extr_HighSpd_PlateauLogic' */
    uint8 Extr_HighSpd_FrictionWindowCnt;/* '<S75>/Extr_HighSpd_PlateauLogic' */
    uint8 BufferIndex;                 /* '<S75>/Extr_HighSpd_PlateauLogic' */
    uint8 FrictionAverageSamples;      /* '<S75>/Extr_HighSpd_PlateauLogic' */
    uint8 FrictionDrumRevolutions;     /* '<S75>/Extr_HighSpd_PlateauLogic' */
    uint8 BaselineFrictionCount;       /* '<S75>/Extr_HighSpd_PlateauLogic' */
    uint8 FrictionRangeSampleCount;    /* '<S75>/Extr_HighSpd_PlateauLogic' */
    uint8 temporalCounter_i4;          /* '<S75>/Extr_HighSpd_PlateauLogic' */
}
Extraction_DW_Extr_HighSpd_PlateauLogic_Enabled_TYPE;

#endif                                 /* EXTR_VARIANT_HIGH_SPEED_PE_ENABLED */

/* Zero-crossing (trigger) state for system '<S66>/Extr_HighSpd_PlateauLogic_Enabled' */
#if EXTR_VARIANT_HIGH_SPEED_PE_ENABLED

typedef struct
{
    ZCSigState Extr_HighSpd_PlateauLogic_Trig_ZCE[5];/* '<S75>/Extr_HighSpd_PlateauLogic' */
}
Extraction_ZCE_Extr_HighSpd_PlateauLogic_Enabled_TYPE;

#endif                                 /* EXTR_VARIANT_HIGH_SPEED_PE_ENABLED */

/* Block states (auto storage) for system '<S67>/Extr_HighSpd_RampLogic_Disabled' */
#if EXTR_VARIANT_HIGH_SPEED_PE_DISABLED

typedef struct
{
    uint32 Extr_HighSpd_EvRampCompleteEventCounter;/* '<S79>/Extr_HighSpd_RampLogic' */
    uint8 is_active_c49_Extraction;    /* '<S79>/Extr_HighSpd_RampLogic' */
    uint8 is_c49_Extraction;           /* '<S79>/Extr_HighSpd_RampLogic' */
}
Extraction_DW_Extr_HighSpd_RampLogic_Disabled_TYPE;

#endif                                 /* EXTR_VARIANT_HIGH_SPEED_PE_DISABLED */

/* Zero-crossing (trigger) state for system '<S67>/Extr_HighSpd_RampLogic_Disabled' */
#if EXTR_VARIANT_HIGH_SPEED_PE_DISABLED

typedef struct
{
    ZCSigState Extr_HighSpd_RampLogic_Trig_ZCE_ir[6];/* '<S79>/Extr_HighSpd_RampLogic' */
}
Extraction_ZCE_Extr_HighSpd_RampLogic_Disabled_TYPE;

#endif                                 /* EXTR_VARIANT_HIGH_SPEED_PE_DISABLED */

/* Block states (auto storage) for system '<S67>/Extr_HighSpd_RampLogic_Enabled' */
#if EXTR_VARIANT_HIGH_SPEED_PE_ENABLED

typedef struct
{
    uint32 Extr_HighSpd_EvRampCompleteEventCounter;/* '<S80>/Extr_HighSpd_RampLogic' */
    uint32 Extr_HighSpd_EvViscousLimitExceededEventCounter;/* '<S80>/Extr_HighSpd_RampLogic' */
    uint32 Extr_HighSpd_EvFrictionLimitExceededEventCounter;/* '<S80>/Extr_HighSpd_RampLogic' */
    uint32 temporalCounter_i1;         /* '<S80>/Extr_HighSpd_RampLogic' */
    sint16 PrevTargetSpeed;            /* '<S80>/Extr_HighSpd_RampLogic' */
    uint8 is_active_c8_Extraction;     /* '<S80>/Extr_HighSpd_RampLogic' */
    uint8 is_c8_Extraction;            /* '<S80>/Extr_HighSpd_RampLogic' */
    uint8 is_RAMPING;                  /* '<S80>/Extr_HighSpd_RampLogic' */
}
Extraction_DW_Extr_HighSpd_RampLogic_Enabled_TYPE;

#endif                                 /* EXTR_VARIANT_HIGH_SPEED_PE_ENABLED */

/* Zero-crossing (trigger) state for system '<S67>/Extr_HighSpd_RampLogic_Enabled' */
#if EXTR_VARIANT_HIGH_SPEED_PE_ENABLED

typedef struct
{
    ZCSigState Extr_HighSpd_RampLogic_Trig_ZCE[6];/* '<S80>/Extr_HighSpd_RampLogic' */
}
Extraction_ZCE_Extr_HighSpd_RampLogic_Enabled_TYPE;

#endif                                 /* EXTR_VARIANT_HIGH_SPEED_PE_ENABLED */

/* Block signals for system '<S180>/DistUtilsWrapperControl' */
typedef struct
{
    DIST_UTILS_DXDT_OUTPUT_TYPE DistUtils_o1;/* '<S185>/DistUtils' */
    float32 DistUtils_o2;              /* '<S185>/DistUtils' */
    float32 DistUtils_o3;              /* '<S185>/DistUtils' */
    float32 DistUtils_o4;              /* '<S185>/DistUtils' */
    uint8 DataTypeConversion;          /* '<S185>/Data Type Conversion' */
}
Extraction_B_DistUtilsWrapperControl_TYPE;

/* Block signals for system '<S180>/REBALANCE.RUNNING.DISTRIBUTION_RAMP.P2PTorqueThresholdCalc' */
typedef struct
{
    float32 Gain;                      /* '<S187>/Gain' */
}
Extraction_B_REBALANCERUNNINGDISTRIBUTION_RAMPP2PTorqueThresholdCalc_TYPE;

/* Block signals for system '<S56>/CalcFloorDispThreshold' */
#if EXTR_VARIANT_ACCELEROMETER_ENABLED

typedef struct
{
    float32 Sum;                       /* '<S58>/Sum' */
}
Extraction_B_CalcFloorDispThreshold_TYPE;

#endif                                 /* EXTR_VARIANT_ACCELEROMETER_ENABLED */

/* Block signals for system '<S26>/Extr_Accel_Enabled' */
#if EXTR_VARIANT_ACCELEROMETER_ENABLED

typedef struct
{
    uint16 tmpFor_Extr_Accel_VibMapRefDispArray_x100[25];
    float32 Abs_iy[3];                 /* '<S32>/Abs' */
    uint16 Extr_Accel_ExtPlateauTimeIncrement_nd;/* '<S31>/Extr_Accel_VibMapLogic' */
    uint8 Extr_Accel_VibMapSafeSpeed_div10_p5;/* '<S31>/Extr_Accel_VibMapLogic' */
    Extraction_B_CalcFloorDispThreshold_TYPE CalcFloorDispThreshold;/* '<S56>/CalcFloorDispThreshold' */
}
Extraction_B_Extr_Accel_Enabled_TYPE;

#endif                                 /* EXTR_VARIANT_ACCELEROMETER_ENABLED */

/* Block states (auto storage) for system '<S26>/Extr_Accel_Enabled' */
#if EXTR_VARIANT_ACCELEROMETER_ENABLED

typedef struct
{
    uint32 Extr_Accel_EvExcessiveFloorDispEventCounter;/* '<S31>/Extr_Accel_VibMapLogic' */
    uint32 temporalCounter_i1;         /* '<S31>/Extr_Accel_VibMapLogic' */
    uint32 Extr_Accel_EvCabHitEventCounter;/* '<S29>/Extr_Accel_CabHitLogic' */
    uint32 Extr_Accel_EvCleanWasherExceptionEventCounter;/* '<S29>/Extr_Accel_CabHitLogic' */
    uint32 temporalCounter_i1_cz;      /* '<S29>/Extr_Accel_CabHitLogic' */
    ACCEL_BASELINE_STATUS_TYPE AccelBaselineStatus;/* '<S30>/Extr_Accel_Supervisor' */
    ACCELEROMETER_STATUS_TYPE AccelStatus;/* '<S30>/Extr_Accel_Supervisor' */
    uint8 is_c26_Extraction;           /* '<S31>/Extr_Accel_VibMapLogic' */
    uint8 is_RUNNING;                  /* '<S31>/Extr_Accel_VibMapLogic' */
    uint8 is_MONITORING;               /* '<S31>/Extr_Accel_VibMapLogic' */
    uint8 is_active_c18_Extraction;    /* '<S30>/Extr_Accel_Supervisor' */
    uint8 is_c18_Extraction;           /* '<S30>/Extr_Accel_Supervisor' */
    uint8 is_SUPERVISOR_ENABLED;       /* '<S30>/Extr_Accel_Supervisor' */
    uint8 is_c33_Extraction;           /* '<S29>/Extr_Accel_CabHitLogic' */
    uint8 is_RUNNING_f3;               /* '<S29>/Extr_Accel_CabHitLogic' */
    uint8 is_MONITORING_ig;            /* '<S29>/Extr_Accel_CabHitLogic' */
    boolean_T HardThresholdCrossed;    /* '<S29>/Extr_Accel_CabHitLogic' */
    boolean_T CleanWasherThresholdCrossed;/* '<S29>/Extr_Accel_CabHitLogic' */
}
Extraction_DW_Extr_Accel_Enabled_TYPE;

#endif                                 /* EXTR_VARIANT_ACCELEROMETER_ENABLED */

/* Zero-crossing (trigger) state for system '<S26>/Extr_Accel_Enabled' */
#if EXTR_VARIANT_ACCELEROMETER_ENABLED

typedef struct
{
    ZCSigState Extr_Accel_CabHitLogic_Trig_ZCE[2];/* '<S29>/Extr_Accel_CabHitLogic' */
}
Extraction_ZCE_Extr_Accel_Enabled_TYPE;

#endif                                 /* EXTR_VARIANT_ACCELEROMETER_ENABLED */

/* Block signals (auto storage) */
typedef struct
{
    PARAM_EST_SETUP_TYPE ParamEstSetup;/* '<S254>/GetParamEstSetup' */
    MOTOR_OUTPUT_TYPE BusCreator;      /* '<S21>/Bus Creator' */
    MOTOR_OUTPUT_TYPE BusCreator_pp;   /* '<S18>/Bus Creator' */
    MOTOR_OUTPUT_TYPE BusCreator_dv;   /* '<S261>/Bus Creator' */
    MOTOR_OUTPUT_TYPE Extr_Inputs_MotorSignals_os;
    boolean_T zcEvent[20];
    boolean_T HiddenBuf_InsertedFor_Extr_Sup_Logic_at_inport_0[18];
    boolean_T zcEvent_mb[18];
    sint8 inputevents_n2[18];          /* '<S193>/Extr_Sup_Logic' */
    PARAM_EST_EXC_SETUP_TYPE ParamEstExcSetup;/* '<S25>/GetParamEstExcSetup' */
    SPIN_PULSE_TYPE OutportBufferForExtr_HighSpd_PrevSpinPulse;/* '<S85>/LoadSpinPulse' */
    SPIN_PULSE_TYPE Extr_HighSpd_SpinPulse;/* '<S85>/LoadSpinPulse' */
    SPIN_PULSE_TYPE Selector1;         /* '<S87>/Selector1' */
    SPIN_PULSE_TYPE PrevSpinPulse;     /* '<S87>/PrevSpinPulse' */
    HIGH_SPEED_PULSE_CONFIG_TYPE OutportBufferForExtr_HighSpd_PrevPulseConfig;/* '<S85>/UpdateCurrentPulseConfig' */
    HIGH_SPEED_PULSE_CONFIG_TYPE OutportBufferForExtr_HighSpd_PulseConfig;/* '<S85>/UpdateCurrentPulseConfig' */
    HIGH_SPEED_PULSE_CONFIG_TYPE
        VariantMergeForOutportExtr_HighSpd_PrevPulseConfig;
    HIGH_SPEED_PULSE_CONFIG_TYPE VariantMergeForOutportExtr_HighSpd_PulseConfig;
    PARAM_EST_APP_SPECIFIC_SETUP_TYPE
        OutportBufferForExtr_HighSpd_ParamEstAppSpecificSetup;/* '<S85>/UpdateCurrentPulseConfig' */
    PARAM_EST_APP_SPECIFIC_SETUP_TYPE
        VariantMergeForOutportExtr_HighSpd_ParamEstAppSpecificSetup;
    HIGH_SPEED_BASELINE_CONFIG_TYPE
        VariantMergeForOutportExtr_HighSpd_BaselineConfig;
    LOAD_MASS_LIMIT_SETUP_TYPE OutportBufferForExtr_HighSpd_LoadMassLimitConfig;/* '<S85>/UpdateCurrentPulseConfig' */
    LOAD_MASS_LIMIT_SETUP_TYPE
        OutportBufferForExtr_HighSpd_PrevLoadMassLimitConfig;/* '<S85>/UpdateCurrentPulseConfig' */
    LOAD_MASS_LIMIT_SETUP_TYPE
        VariantMergeForOutportExtr_HighSpd_LoadMassLimitConfig;
    LOAD_MASS_LIMIT_SETUP_TYPE
        VariantMergeForOutportExtr_HighSpd_PrevLoadMassLimitConfig;
    PRE_LOOKUP_OUTPUT_TYPE LoadMassIndex;/* '<S140>/LoadMassPrelookup' */
    DISTRIBUTION_PULSE_TYPE current_pulse;/* '<S12>/Extr_Rebal_Logic' */
    float32 mass;                      /* '<S12>/Extr_Rebal_Logic' */
    float32 currentSpeed;              /* '<S12>/Extr_Rebal_Logic' */
    float32 VariantMergeForOutportExtr_HighSpd_LoadMass;
    float32 Extr_LowSpd_LoadMass_gp;   /* '<S121>/LoadMassSwitch' */
    float32 mass_m2;                   /* '<S126>/MDDPLogic' */
    float32 Extr_LowSpd_LoadMass_ec;   /* '<S125>/Extr_LowSpd_LoadMeasLogic' */
    float32 Inertia;                   /* '<S245>/ParamEst_sfun' */
    float32 UB;                        /* '<S245>/ParamEst_sfun' */
    float32 UB_Angle;                  /* '<S245>/ParamEst_sfun' */
    float32 Viscous_Friction;          /* '<S245>/ParamEst_sfun' */
    float32 Coulomb_Friction;          /* '<S245>/ParamEst_sfun' */
    float32 Friction;                  /* '<S245>/ParamEst_sfun' */
    float32 BB_mgr;                    /* '<S245>/ParamEst_sfun' */
    float32 BB_PhaseDiff;              /* '<S245>/ParamEst_sfun' */
    float32 ParamEstExc_sfun_o1;       /* '<S242>/ParamEstExc_sfun' */
    float32 Inertia_fr;                /* '<S241>/ParamEst_sfun' */
    float32 UB_mc;                     /* '<S241>/ParamEst_sfun' */
    float32 UB_Angle_bf;               /* '<S241>/ParamEst_sfun' */
    float32 Viscous_Friction_cp;       /* '<S241>/ParamEst_sfun' */
    float32 Coulomb_Friction_pq;       /* '<S241>/ParamEst_sfun' */
    float32 Friction_bt;               /* '<S241>/ParamEst_sfun' */
    float32 BB_mgr_nw;                 /* '<S241>/ParamEst_sfun' */
    float32 BB_PhaseDiff_m5;           /* '<S241>/ParamEst_sfun' */
    float32 Inertia_cz;                /* '<S264>/ParamEst_sfun' */
    float32 UB_ne;                     /* '<S264>/ParamEst_sfun' */
    float32 UB_Angle_b1;               /* '<S264>/ParamEst_sfun' */
    float32 Viscous_Friction_lc;       /* '<S264>/ParamEst_sfun' */
    float32 Coulomb_Friction_p2;       /* '<S264>/ParamEst_sfun' */
    float32 Friction_py;               /* '<S264>/ParamEst_sfun' */
    float32 BB_mgr_o1;                 /* '<S264>/ParamEst_sfun' */
    float32 BB_PhaseDiff_g1;           /* '<S264>/ParamEst_sfun' */
    float32 ParamEstExc_sfun_o1_kx;    /* '<S260>/ParamEstExc_sfun' */
    float32 Extr_LowSpd_LoadMass_prev;
    float32 Extr_HighSpd_LoadMass_prev;
    sint32 sfEvent_ha;
    sint32 i;
    DIST_UTILS_COMMAND_TYPE distUtilsCommand;/* '<S12>/Extr_Rebal_Logic' */
    EXTRACTION_STATE Extr_Sup_State_d3;/* '<S193>/Extr_Sup_Logic' */
    sint16 Extr_Sup_SpdCommand_hvd[2];
    sint16 prev_speed;                 /* '<S12>/Extr_Rebal_Logic' */
    sint16 Abs;                        /* '<S186>/Abs' */
    sint16 Extr_LowSpd_DistSpeed_lt;   /* '<S122>/Extr_LowSpd_SupervisorLogic' */
    uint16 VariantMergeForOutportExtr_HighSpd_PlateauDurationIncrement;
    uint16 MaxTargetSpeed;             /* '<S121>/MaxTargetSpeed' */
    uint16 mddpSpeedLUT;               /* '<S149>/mddpSpeedLUT' */
    uint16 Extr_LowSpd_Unbalance_oe;   /* '<S125>/Extr_LowSpd_LoadMeasLogic' */
    uint16 ub;                         /* '<S125>/Extr_LowSpd_LoadMeasLogic' */
    uint16 BalanceIndexToGrams;        /* '<S146>/BalanceIndexToGrams' */
    uint16 x10;                        /* '<S173>/x10' */
    sint8 VariantMergeForOutportExtr_HighSpd_CurrPulseInd;
    sint8 rtb_inputevents_mn_idx_1;
    sint8 rtb_inputevents_mn_idx_2;
    sint8 rtb_inputevents_mn_idx_3;
    sint8 rtb_inputevents_mn_idx_4;
    sint8 rtb_inputevents_mn_idx_5;
    sint8 rtb_inputevents_mn_idx_6;
    sint8 rtb_inputevents_mn_idx_7;
    sint8 rtb_inputevents_mn_idx_8;
    sint8 rtb_inputevents_mn_idx_9;
    sint8 rtb_inputevents_mn_idx_10;
    sint8 rtb_inputevents_mn_idx_11;
    sint8 rtb_inputevents_mn_idx_12;
    sint8 rtb_inputevents_mn_idx_13;
    sint8 rtb_inputevents_mn_idx_14;
    sint8 rtb_inputevents_mn_idx_15;
    sint8 rtb_inputevents_mn_idx_16;
    sint8 rtb_inputevents_mn_idx_17;
    sint8 rtb_inputevents_mn_idx_18;
    sint8 rtb_inputevents_mn_idx_19;
    uint8 VariantMergeForOutportExtr_HighSpd_DistSpeedTimeout;
    uint8 Extr_LowSpd_MddpDur_fy;      /* '<S126>/MDDPLogic' */
    uint8 mddpDurationLUT;             /* '<S149>/mddpDurationLUT' */
    uint8 mddpRampLUT;                 /* '<S149>/mddpRampLUT' */
    uint8 Extr_LowSpd_RebalCnt_lp;     /* '<S122>/Extr_LowSpd_SupervisorLogic' */
    uint8 Extr_LowSpd_RebalCnt_e4;     /* '<S122>/Extr_LowSpd_SupervisorLogic' */
    uint8 FixPtSum1;                   /* '<S246>/FixPt Sum1' */
    uint8 DataTypeConversion;          /* '<S21>/Data Type Conversion' */
    uint8 DataTypeConversion1;         /* '<S21>/Data Type Conversion1' */
    uint8 TmpSignalConversionAtoutput_selectorOutport1[8];/* '<S21>/output_selector' */
    uint8 DataTypeConversion_gf;       /* '<S19>/Data Type Conversion' */
    uint8 DataTypeConversion_ii;       /* '<S18>/Data Type Conversion' */
    uint8 DataTypeConversion1_ei;      /* '<S18>/Data Type Conversion1' */
    uint8 DataTypeConversion_pt;       /* '<S261>/Data Type Conversion' */
    uint8 DataTypeConversion_gr;       /* '<S260>/Data Type Conversion' */
    uint8 Extr_Inputs_MaxSpeedError_iv;
    uint8 Extr_Inputs_RampTimeout_dy;
    uint8 Extr_Sup_SpdComCounter_cy;
    boolean_T OutportBufferForACU_ExtrStop;/* '<S198>/EventGen' */
    boolean_T ACU_ExtrStop;            /* '<S198>/EventGen' */
    boolean_T OutportBufferForACU_ExtrStart;/* '<S197>/EventGen' */
    boolean_T ACU_ExtrStart;           /* '<S197>/EventGen' */
    boolean_T OutportBufferForACU_ExtrResume;/* '<S196>/EventGen' */
    boolean_T ACU_ExtrResume;          /* '<S196>/EventGen' */
    boolean_T OutportBufferForACU_ResetLoadMass;/* '<S195>/EventGen' */
    boolean_T ACU_ResetLoadMass;       /* '<S195>/EventGen' */
    boolean_T OutportBufferForACU_NotifyFoamDetection;/* '<S194>/EventGen' */
    boolean_T NotifyFoamDetection;     /* '<S194>/EventGen' */
    boolean_T VariantMergeForOutportExtr_Accel_EvCabHit;
    boolean_T VariantMergeForOutportExtr_Accel_VibMapEvExcessiveFloorDisp;
    boolean_T VariantMergeForOutportExtr_Accel_EvCleanWasherException;
    boolean_T OutportBufferForExtr_Rebal_EvCleanWasherException;/* '<S12>/Extr_Rebal_Logic' */
    boolean_T OutportBufferForExtr_Rebal_EvComplete;/* '<S12>/Extr_Rebal_Logic' */
    boolean_T OutportBufferForExtr_Rebal_EvP2PTorqFault;/* '<S12>/Extr_Rebal_Logic' */
    boolean_T OutportBufferForExtr_Rebal_EvShortRedistExceeded;/* '<S12>/Extr_Rebal_Logic' */
    boolean_T Extr_Rebal_EvComplete;   /* '<S12>/Extr_Rebal_Logic' */
    boolean_T Extr_Rebal_EvP2PTorqFault;/* '<S12>/Extr_Rebal_Logic' */
    boolean_T Extr_Rebal_EvShortRedistExceeded;/* '<S12>/Extr_Rebal_Logic' */
    boolean_T Extr_Rebal_EvCleanWasherException;/* '<S12>/Extr_Rebal_Logic' */
    boolean_T UnitDelay;               /* '<S61>/Unit Delay' */
    boolean_T HighSpdExtUnbCalculated; /* '<S61>/HighSpdExtUnbCalculated' */
    boolean_T Extr_HighSpd_EnableCEM;  /* '<S61>/EnableCEM' */
    boolean_T OutportBufferForExtr_HighSpd_EvExtndChkFailed;/* '<S61>/Extr_HighSpd_ExtUnbChk' */
    boolean_T VariantMergeForOutportExtr_HighSpd_EnableCEM;
    boolean_T VariantMergeForOutportExtr_HighSpd_EnableLoadMassLimitCheck;
    boolean_T VariantMergeForOutportExtr_HighSpd_EvRampComplete;
    boolean_T VariantMergeForOutportExtr_HighSpd_EvViscousLimitExceeded;
    boolean_T VariantMergeForOutportExtr_HighSpd_EvFrictionLimitExceeded;
    boolean_T VariantMergeForOutportExtr_HighSpd_EnableCEM_j4;
    boolean_T VariantMergeForOutportExtr_HighSpd_EnableLoadMassLimitCheck_hb;
    boolean_T VariantMergeForOutportExtr_HighSpd_EvPlateauComplete;
    boolean_T VariantMergeForOutportExtr_HighSpd_EvLoadMassExceeded;
    boolean_T Extr_HighSpd_ExtUnbCalculated;/* '<S64>/Extr_HighSpd_ExtUnbCheckLogic' */
    boolean_T Extr_HighSpd_EvExtndChkFailed;/* '<S64>/Extr_HighSpd_ExtUnbCheckLogic' */
    boolean_T VariantMergeForOutportExtr_HighSpd_EvCEMFailureDetected;
    boolean_T cov_reset_on_ramp;       /* '<S90>/MATLAB Function' */
    boolean_T VariantMergeForOutportExtr_HighSpd_EvComplete;
    boolean_T VariantMergeForOutportExtr_HighSpd_EvDropToDistSpeed;
    boolean_T VariantMergeForOutportExtr_HighSpd_EvStartPlateau;
    boolean_T VariantMergeForOutportExtr_HighSpd_EvStartRamp;
    boolean_T VariantMergeForOutportExtr_HighSpd_EvStartRebal;
    boolean_T VariantMergeForOutportExtr_HighSpd_EvStopAll;
    boolean_T OutportBufferForExtr_LowSpd_EvCleanWasherException;/* '<S121>/Extr_LowSpd_LoadMeas' */
    boolean_T OutportBufferForExtr_LowSpd_EvLoadMeasComplete;/* '<S121>/Extr_LowSpd_LoadMeas' */
    boolean_T OutportBufferForExtr_LowSpd_EvMddpComplete;/* '<S121>/Extr_LowSpd_MDDP' */
    boolean_T OutportBufferForExtr_LowSpd_EvUnbChkComplete;/* '<S121>/Extr_LowSpd_BasicUnbChk' */
    boolean_T OutportBufferForExtr_LowSpd_EvUnbChkRebalStart;/* '<S121>/Extr_LowSpd_BasicUnbChk' */
    boolean_T VariantMergeForOutportExt_LowSpd_EvOptRampComplete;
    boolean_T VariantMergeForOutportExt_LowSpd_EvRedist;
    boolean_T Extr_LowSpd_EvMddpComplete;/* '<S126>/MDDPLogic' */
    boolean_T Extr_LowSpd_EvLoadMeasComplete;/* '<S125>/Extr_LowSpd_LoadMeasLogic' */
    boolean_T Extr_LowSpd_EvCleanWasherException;/* '<S125>/Extr_LowSpd_LoadMeasLogic' */
    boolean_T Extr_LowSpd_MddpExecuted;
    boolean_T Extr_LowSpd_EvUnbChkRebalStart;/* '<S124>/Extr_LowSpd_BasicUnbCheck' */
    boolean_T Extr_LowSpd_EvUnbChkComplete;/* '<S124>/Extr_LowSpd_BasicUnbCheck' */
    boolean_T OutportBufferForExtr_LowSpd_EvComplete;/* '<S122>/Extr_LowSpd_SupervisorLogic' */
    boolean_T OutportBufferForExtr_LowSpd_EvOptRampStart;/* '<S122>/Extr_LowSpd_SupervisorLogic' */
    boolean_T OutportBufferForExtr_LowSpd_EvRebalStart;/* '<S122>/Extr_LowSpd_SupervisorLogic' */
    boolean_T OutportBufferForExtr_LowSpd_EvStop;/* '<S122>/Extr_LowSpd_SupervisorLogic' */
    boolean_T Extr_LowSpd_MddpExecuted_p5;/* '<S122>/Extr_LowSpd_SupervisorLogic' */
    boolean_T OutportBufferForExtr_LowSpd_PauseSpeedMonitor;/* '<S122>/Extr_LowSpd_SupervisorLogic' */
    boolean_T Extr_LowSpd_MddpExecuted_mb;/* '<S122>/Extr_LowSpd_SupervisorLogic' */
    boolean_T Extr_LowSpd_PauseSpeedMonitor;/* '<S122>/Extr_LowSpd_SupervisorLogic' */
    boolean_T Extr_LowSpd_EvDistributionStart;/* '<S122>/Extr_LowSpd_SupervisorLogic' */
    boolean_T Extr_LowSpd_EvLoadMeasStart;/* '<S122>/Extr_LowSpd_SupervisorLogic' */
    boolean_T Extr_LowSpd_EvOptRampStart;/* '<S122>/Extr_LowSpd_SupervisorLogic' */
    boolean_T Extr_LowSpd_EvMddpStart; /* '<S122>/Extr_LowSpd_SupervisorLogic' */
    boolean_T Extr_LowSpd_EvResetLoadMass;/* '<S122>/Extr_LowSpd_SupervisorLogic' */
    boolean_T Extr_LowSpd_EvRebalStart;/* '<S122>/Extr_LowSpd_SupervisorLogic' */
    boolean_T Extr_LowSpd_EvComplete;  /* '<S122>/Extr_LowSpd_SupervisorLogic' */
    boolean_T Extr_LowSpd_EvStop;      /* '<S122>/Extr_LowSpd_SupervisorLogic' */
    boolean_T Extr_LowSpd_EvStartBasicUnbChk;/* '<S122>/Extr_LowSpd_SupervisorLogic' */
    boolean_T Extr_LowSpd_EvResetShortRedist;/* '<S122>/Extr_LowSpd_SupervisorLogic' */
    boolean_T OutportBufferForExtr_Sup_CabHitEnabled;/* '<S193>/Extr_Sup_Logic' */
    boolean_T OutportBufferForExtr_Sup_ClearFault;/* '<S193>/Extr_Sup_Logic' */
    boolean_T OutportBufferForExtr_Sup_EvFoamDetected;/* '<S193>/Extr_Sup_Logic' */
    boolean_T OutportBufferForExtr_Sup_EvHighSpdStart;/* '<S193>/Extr_Sup_Logic' */
    boolean_T OutportBufferForExtr_Sup_EvRebalStart;/* '<S193>/Extr_Sup_Logic' */
    boolean_T OutportBufferForExtr_Sup_EvResetSoftCabHitCnt;/* '<S193>/Extr_Sup_Logic' */
    boolean_T Extr_Sup_EvSpeedErrorTimeout;/* '<S193>/Extr_Sup_MotorSpeedCheck' */
    boolean_T Extr_Sup_EvSpeedReached; /* '<S193>/Extr_Sup_MotorSpeedCheck' */
    boolean_T OutportBufferForExtr_Sup_EvStopAll;/* '<S193>/Extr_Sup_Logic' */
    boolean_T OutportBufferForExtr_Sup_ResetRebalCnt;/* '<S193>/Extr_Sup_Logic' */
    boolean_T OutportBufferForExtr_Sup_VibMapEnabled;/* '<S193>/Extr_Sup_Logic' */
    boolean_T Extr_Sup_EvSpeedErrorTimeout_ov;/* '<S193>/Extr_Sup_MotorSpeedCheck' */
    boolean_T Extr_Sup_EvSpeedReached_fh;/* '<S193>/Extr_Sup_MotorSpeedCheck' */
    boolean_T Extr_Sup_VibMapEnabled;  /* '<S193>/Extr_Sup_Logic' */
    boolean_T Extr_Sup_CabHitEnabled;  /* '<S193>/Extr_Sup_Logic' */
    boolean_T Extr_Sup_EvLowSpdStart;  /* '<S193>/Extr_Sup_Logic' */
    boolean_T Extr_Sup_EvHighSpdStart; /* '<S193>/Extr_Sup_Logic' */
    boolean_T Extr_Sup_EvStopAll;      /* '<S193>/Extr_Sup_Logic' */
    boolean_T Extr_Sup_ResetLoadMass;  /* '<S193>/Extr_Sup_Logic' */
    boolean_T Extr_Sup_EvRebalStart;   /* '<S193>/Extr_Sup_Logic' */
    boolean_T Extr_Sup_ResetRebalCnt;  /* '<S193>/Extr_Sup_Logic' */
    boolean_T Extr_Sup_EvFoamDetected; /* '<S193>/Extr_Sup_Logic' */
    boolean_T Extr_Sup_ClearFault;     /* '<S193>/Extr_Sup_Logic' */
    boolean_T Extr_Sup_EvResetSoftCabHitCnt;/* '<S193>/Extr_Sup_Logic' */
    boolean_T OutportBufferForACU_ExtrEnd;/* '<S192>/EventGen' */
    boolean_T ACU_ExtrEnd;             /* '<S192>/EventGen' */
    boolean_T ParamEstExc_sfun_o2;     /* '<S242>/ParamEstExc_sfun' */
    boolean_T ParamEstExc_sfun_o2_lq;  /* '<S260>/ParamEstExc_sfun' */
    boolean_T OutportBufferForExtr_Sup_ResetLoadMass;
    boolean_T OutportBufferForExtr_LowSpd_EvMddpStart;
    boolean_T OutportBufferForExtr_LowSpd_EvResetShortRedist;
    boolean_T OutportBufferForExtr_LowSpd_EvStartBasicUnbChk;
    boolean_T zcEvent_idx_0;
    boolean_T zcEvent_idx_1;
    boolean_T zcEvent_idx_2;
    boolean_T zcEvent_idx_3;
    boolean_T zcEvent_idx_6;
    boolean_T zcEvent_idx_7;
    boolean_T zcEvent_idx_8;
    boolean_T zcEvent_idx_9;
    boolean_T zcEvent_idx_10;
    BOOL_TYPE VariantMergeForOutportExtr_HighSpd_RegressDecisionOnPrevPulse;
    Extraction_B_TriggerGen_TYPE sf_TriggerGen_gl;/* '<S27>/TriggerGen' */

#if EXTR_VARIANT_ACCELEROMETER_ENABLED

    Extraction_B_Extr_Accel_Enabled_TYPE Extr_Accel_Enabled_gt;/* '<S26>/Extr_Accel_Enabled' */

#define EXTRACTION_B_TYPE_VARIANT_EXISTS
#endif                                 /* EXTR_VARIANT_ACCELEROMETER_ENABLED */

    Extraction_B_TriggerGen_TYPE sf_TriggerGen_dq;/* '<S184>/TriggerGen' */
    Extraction_B_REBALANCERUNNINGDISTRIBUTION_RAMPP2PTorqueThresholdCalc_TYPE
        REBALANCERUNNINGDISTRIBUTION_RAMPP2PTorqueThresholdCalc;/* '<S180>/REBALANCE.RUNNING.DISTRIBUTION_RAMP.P2PTorqueThresholdCalc' */
    Extraction_B_DistUtilsWrapperControl_TYPE DistUtilsWrapperControl;/* '<S180>/DistUtilsWrapperControl' */
    Extraction_B_TriggerGen_TYPE sf_TriggerGen_bc;/* '<S68>/TriggerGen' */

#if EXTR_VARIANT_HIGH_SPEED_PE_ENABLED

    Extraction_B_Extr_HighSpd_PlateauLogic_Enabled_TYPE
        Extr_HighSpd_PlateauLogic_Enabled;/* '<S66>/Extr_HighSpd_PlateauLogic_Enabled' */

#define EXTRACTION_B_TYPE_VARIANT_EXISTS
#endif                                 /* EXTR_VARIANT_HIGH_SPEED_PE_ENABLED */

#if EXTR_VARIANT_HIGH_SPEED_PE_DISABLED

    Extraction_B_Extr_HighSpd_PlateauLogic_Disabled_TYPE
        Extr_HighSpd_PlateauLogic_Disabled;/* '<S66>/Extr_HighSpd_PlateauLogic_Disabled' */

#define EXTRACTION_B_TYPE_VARIANT_EXISTS
#endif                                 /* EXTR_VARIANT_HIGH_SPEED_PE_DISABLED */

    Extraction_B_TriggerGen_TYPE sf_TriggerGen_md;/* '<S88>/TriggerGen' */

#if EXTR_VARIANT_HIGH_SPEED_PE_ENABLED

    Extraction_B_Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled_TYPE
        Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled;/* '<S86>/Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled' */

#define EXTRACTION_B_TYPE_VARIANT_EXISTS
#endif                                 /* EXTR_VARIANT_HIGH_SPEED_PE_ENABLED */

#if EXTR_VARIANT_HIGH_SPEED_PE_DISABLED

    Extraction_B_Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled_TYPE
        Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled;/* '<S86>/Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled' */

#define EXTRACTION_B_TYPE_VARIANT_EXISTS
#endif                                 /* EXTR_VARIANT_HIGH_SPEED_PE_DISABLED */

    Extraction_B_TriggerGen_TYPE sf_TriggerGen_kb;/* '<S130>/TriggerGen' */

#if EXTR_VARIANT_OPT_RAMP_ENABLED

    Extraction_B_Extr_LowSpd_OptRamp_Enabled_TYPE Extr_LowSpd_OptRamp_Enabled;/* '<S127>/Extr_LowSpd_OptRamp_Enabled' */

#define EXTRACTION_B_TYPE_VARIANT_EXISTS
#endif                                 /* EXTR_VARIANT_OPT_RAMP_ENABLED */

    Extraction_B_TriggerGen_TYPE sf_TriggerGen_ih;/* '<S174>/TriggerGen' */
    Extraction_B_UpdatePEAppSpecificSetup_TYPE UpdatePEAppSpecificSetup;/* '<S122>/UpdatePEAppSpecificSetup' */
    Extraction_B_TriggerGen_TYPE sf_TriggerGen;/* '<S204>/TriggerGen' */
}
Extraction_B_TYPE;

/* Block states (auto storage) for system '<Root>' */
typedef struct
{
    SPIN_PULSE_TYPE PrevSpinPulse_DSTATE;/* '<S87>/PrevSpinPulse' */
    DISTRIBUTION_PULSE_TYPE CurrentPulse;/* '<S12>/Extr_Rebal_Logic' */
    float32 ExtUnbSum;                 /* '<S64>/Extr_HighSpd_ExtUnbCheckLogic' */
    float32 InertiaBuffer[15];         /* '<S125>/Extr_LowSpd_LoadMeasLogic' */
    float32 InertiaSum;                /* '<S125>/Extr_LowSpd_LoadMeasLogic' */
    float32 UbSum;                     /* '<S125>/Extr_LowSpd_LoadMeasLogic' */
    float32 MaxSpeedError;             /* '<S193>/Extr_Sup_MotorSpeedCheck' */
    float32 Extr_LowSpd_LoadMass_start;/* '<S193>/Extr_Sup_LoadMassMerge' */
    float32 Extr_HighSpd_LoadMass_start;/* '<S193>/Extr_Sup_LoadMassMerge' */
    sint32 sfEvent;                    /* '<S12>/Extr_Rebal_Logic' */
    sint32 sfEvent_pr;                 /* '<S64>/Extr_HighSpd_ExtUnbCheckLogic' */
    sint32 sfEvent_o4;                 /* '<S126>/MDDPLogic' */
    sint32 sfEvent_ak;                 /* '<S125>/Extr_LowSpd_LoadMeasLogic' */
    sint32 sfEvent_k0;                 /* '<S124>/Extr_LowSpd_BasicUnbCheck' */
    sint32 sfEvent_f2;                 /* '<S122>/Extr_LowSpd_SupervisorLogic' */
    uint32 ACU_ExtrStopEventCounter;   /* '<S198>/EventGen' */
    uint32 ACU_ExtrStartEventCounter;  /* '<S197>/EventGen' */
    uint32 ACU_ExtrResumeEventCounter; /* '<S196>/EventGen' */
    uint32 ACU_ResetLoadMassEventCounter;/* '<S195>/EventGen' */
    uint32 NotifyFoamDetectionEventCounter;/* '<S194>/EventGen' */
    uint32 Extr_Rebal_EvCompleteEventCounter;/* '<S12>/Extr_Rebal_Logic' */
    uint32 Extr_Rebal_EvP2PTorqFaultEventCounter;/* '<S12>/Extr_Rebal_Logic' */
    uint32 Extr_Rebal_EvShortRedistExceededEventCounter;/* '<S12>/Extr_Rebal_Logic' */
    uint32 Extr_Rebal_EvCleanWasherExceptionEventCounter;/* '<S12>/Extr_Rebal_Logic' */
    uint32 temporalCounter_i1;         /* '<S12>/Extr_Rebal_Logic' */
    uint32 Extr_HighSpd_EvExtndChkFailedEventCounter;/* '<S64>/Extr_HighSpd_ExtUnbCheckLogic' */
    uint32 Extr_LowSpd_EvMddpCompleteEventCounter;/* '<S126>/MDDPLogic' */
    uint32 temporalCounter_i1_f4;      /* '<S126>/MDDPLogic' */
    uint32 Extr_LowSpd_EvLoadMeasCompleteEventCounter;/* '<S125>/Extr_LowSpd_LoadMeasLogic' */
    uint32 Extr_LowSpd_EvCleanWasherExceptionEventCounter;/* '<S125>/Extr_LowSpd_LoadMeasLogic' */
    uint32 temporalCounter_i1_pw;      /* '<S125>/Extr_LowSpd_LoadMeasLogic' */
    uint32 Extr_LowSpd_EvUnbChkRebalStartEventCounter;/* '<S124>/Extr_LowSpd_BasicUnbCheck' */
    uint32 Extr_LowSpd_EvUnbChkCompleteEventCounter;/* '<S124>/Extr_LowSpd_BasicUnbCheck' */
    uint32 Extr_LowSpd_EvDistributionStartEventCounter;/* '<S122>/Extr_LowSpd_SupervisorLogic' */
    uint32 Extr_LowSpd_EvLoadMeasStartEventCounter;/* '<S122>/Extr_LowSpd_SupervisorLogic' */
    uint32 Extr_LowSpd_EvOptRampStartEventCounter;/* '<S122>/Extr_LowSpd_SupervisorLogic' */
    uint32 Extr_LowSpd_EvMddpStartEventCounter;/* '<S122>/Extr_LowSpd_SupervisorLogic' */
    uint32 Extr_LowSpd_EvResetLoadMassEventCounter;/* '<S122>/Extr_LowSpd_SupervisorLogic' */
    uint32 Extr_LowSpd_EvRebalStartEventCounter;/* '<S122>/Extr_LowSpd_SupervisorLogic' */
    uint32 Extr_LowSpd_EvCompleteEventCounter;/* '<S122>/Extr_LowSpd_SupervisorLogic' */
    uint32 Extr_LowSpd_EvStopEventCounter;/* '<S122>/Extr_LowSpd_SupervisorLogic' */
    uint32 Extr_LowSpd_EvStartBasicUnbChkEventCounter;/* '<S122>/Extr_LowSpd_SupervisorLogic' */
    uint32 Extr_LowSpd_EvResetShortRedistEventCounter;/* '<S122>/Extr_LowSpd_SupervisorLogic' */
    uint32 temporalCounter_i1_jq;      /* '<S122>/Extr_LowSpd_SupervisorLogic' */
    uint32 Extr_Sup_EvSpeedErrorTimeoutEventCounter;/* '<S193>/Extr_Sup_MotorSpeedCheck' */
    uint32 Extr_Sup_EvSpeedReachedEventCounter;/* '<S193>/Extr_Sup_MotorSpeedCheck' */
    uint32 Extr_Sup_EvLowSpdStartEventCounter;/* '<S193>/Extr_Sup_Logic' */
    uint32 Extr_Sup_EvHighSpdStartEventCounter;/* '<S193>/Extr_Sup_Logic' */
    uint32 Extr_Sup_EvStopAllEventCounter;/* '<S193>/Extr_Sup_Logic' */
    uint32 Extr_Sup_ResetLoadMassEventCounter;/* '<S193>/Extr_Sup_Logic' */
    uint32 Extr_Sup_EvRebalStartEventCounter;/* '<S193>/Extr_Sup_Logic' */
    uint32 Extr_Sup_ResetRebalCntEventCounter;/* '<S193>/Extr_Sup_Logic' */
    uint32 Extr_Sup_EvFoamDetectedEventCounter;/* '<S193>/Extr_Sup_Logic' */
    uint32 Extr_Sup_ClearFaultEventCounter;/* '<S193>/Extr_Sup_Logic' */
    uint32 Extr_Sup_EvResetSoftCabHitCntEventCounter;/* '<S193>/Extr_Sup_Logic' */
    uint32 ACU_ExtrEndEventCounter;    /* '<S192>/EventGen' */
    sint16 RampAccel;                  /* '<S12>/Extr_Rebal_Logic' */
    sint16 TargetSpeed;                /* '<S12>/Extr_Rebal_Logic' */
    uint16 MotionDelay;                /* '<S12>/Extr_Rebal_Logic' */
    uint16 InitialUnbalanceCnt;        /* '<S64>/Extr_HighSpd_ExtUnbCheckLogic' */
    uint16 MCI_RawUnbalanceCnt_prev;   /* '<S64>/Extr_HighSpd_ExtUnbCheckLogic' */
    uint16 MCI_RawUnbalanceCnt_start;  /* '<S64>/Extr_HighSpd_ExtUnbCheckLogic' */
    uint16 SampleCnt;                  /* '<S125>/Extr_LowSpd_LoadMeasLogic' */
    boolean_T UnitDelay_DSTATE;        /* '<S61>/Unit Delay' */
    uint8 is_active_c24_Extraction;    /* '<S12>/Extr_Rebal_Logic' */
    uint8 is_c24_Extraction;           /* '<S12>/Extr_Rebal_Logic' */
    uint8 is_REBALANCE;                /* '<S12>/Extr_Rebal_Logic' */
    uint8 is_RUNNING;                  /* '<S12>/Extr_Rebal_Logic' */
    uint8 is_DISTRIBUTION_RAMP;        /* '<S12>/Extr_Rebal_Logic' */
    uint8 is_RAMPING;                  /* '<S12>/Extr_Rebal_Logic' */
    uint8 P2PTorqueDelay;              /* '<S12>/Extr_Rebal_Logic' */
    uint8 is_c45_Extraction;           /* '<S64>/Extr_HighSpd_ExtUnbCheckLogic' */
    uint8 is_RUNNING_im;               /* '<S64>/Extr_HighSpd_ExtUnbCheckLogic' */
    uint8 is_MONITORING_UNBALANCE;     /* '<S64>/Extr_HighSpd_ExtUnbCheckLogic' */
    uint8 tgt;                         /* '<S64>/Extr_HighSpd_ExtUnbCheckLogic' */
    uint8 is_c11_Extraction;           /* '<S126>/MDDPLogic' */
    uint8 is_STATE_RUNNING;            /* '<S126>/MDDPLogic' */
    uint8 is_c9_Extraction;            /* '<S125>/Extr_LowSpd_LoadMeasLogic' */
    uint8 is_RUNNING_PE_MEASUREMENT;   /* '<S125>/Extr_LowSpd_LoadMeasLogic' */
    uint8 InertiaSampleCount;          /* '<S125>/Extr_LowSpd_LoadMeasLogic' */
    uint8 is_active_c42_Extraction;    /* '<S124>/Extr_LowSpd_BasicUnbCheck' */
    uint8 is_c42_Extraction;           /* '<S124>/Extr_LowSpd_BasicUnbCheck' */
    uint8 is_BASIC_UNB_CHK;            /* '<S124>/Extr_LowSpd_BasicUnbCheck' */
    uint8 is_c15_Extraction;           /* '<S122>/Extr_LowSpd_SupervisorLogic' */
    uint8 is_LOW_SPEED_EXTRACTION;     /* '<S122>/Extr_LowSpd_SupervisorLogic' */
    uint8 is_DROPPING_TO_DIST_SPEED;   /* '<S122>/Extr_LowSpd_SupervisorLogic' */
    uint8 is_RUNNING_jv;               /* '<S122>/Extr_LowSpd_SupervisorLogic' */
    uint8 is_active_c47_Extraction;    /* '<S193>/Extr_Sup_MotorSpeedCheck' */
    uint8 is_c47_Extraction;           /* '<S193>/Extr_Sup_MotorSpeedCheck' */
    uint8 Extr_Sup_SpdComCounter_start;/* '<S193>/Extr_Sup_MotorSpeedCheck' */
    uint8 is_EXTRACTION;               /* '<S193>/Extr_Sup_Logic' */
    uint8 is_RUNNING_kz;               /* '<S193>/Extr_Sup_Logic' */
    uint8 is_EXTRACTION_hb;            /* '<S193>/Extr_Sup_Logic' */
    uint8 is_active_c20_Extraction;    /* '<S193>/Extr_Sup_LoadMassMerge' */
    uint8 PrevSetupIndex_PreviousInput;/* '<S253>/PrevSetupIndex' */
    boolean_T HighSpdExtUnbCalculated_PreviousInput;/* '<S61>/HighSpdExtUnbCalculated' */
    boolean_T doneDoubleBufferReInit;  /* '<S64>/Extr_HighSpd_ExtUnbCheckLogic' */
    boolean_T AntifoamRequested;       /* '<S122>/Extr_LowSpd_SupervisorLogic' */
    boolean_T DistributionRequested;   /* '<S122>/Extr_LowSpd_SupervisorLogic' */
    boolean_T ExtrComplete;            /* '<S193>/Extr_Sup_Logic' */
    boolean_T FaultDetected;           /* '<S193>/Extr_Sup_Logic' */
    Extraction_DW_TriggerGen_TYPE sf_TriggerGen_gl;/* '<S27>/TriggerGen' */

#if EXTR_VARIANT_ACCELEROMETER_ENABLED

    Extraction_DW_Extr_Accel_Enabled_TYPE Extr_Accel_Enabled_gt;/* '<S26>/Extr_Accel_Enabled' */

#define EXTRACTION_DW_TYPE_VARIANT_EXISTS
#endif                                 /* EXTR_VARIANT_ACCELEROMETER_ENABLED */

    Extraction_DW_TriggerGen_TYPE sf_TriggerGen_dq;/* '<S184>/TriggerGen' */
    Extraction_DW_TriggerGen_TYPE sf_TriggerGen_bc;/* '<S68>/TriggerGen' */

#if EXTR_VARIANT_HIGH_SPEED_PE_ENABLED

    Extraction_DW_Extr_HighSpd_RampLogic_Enabled_TYPE
        Extr_HighSpd_RampLogic_Enabled;/* '<S67>/Extr_HighSpd_RampLogic_Enabled' */

#define EXTRACTION_DW_TYPE_VARIANT_EXISTS
#endif                                 /* EXTR_VARIANT_HIGH_SPEED_PE_ENABLED */

#if EXTR_VARIANT_HIGH_SPEED_PE_DISABLED

    Extraction_DW_Extr_HighSpd_RampLogic_Disabled_TYPE
        Extr_HighSpd_RampLogic_Disabled;/* '<S67>/Extr_HighSpd_RampLogic_Disabled' */

#define EXTRACTION_DW_TYPE_VARIANT_EXISTS
#endif                                 /* EXTR_VARIANT_HIGH_SPEED_PE_DISABLED */

#if EXTR_VARIANT_HIGH_SPEED_PE_ENABLED

    Extraction_DW_Extr_HighSpd_PlateauLogic_Enabled_TYPE
        Extr_HighSpd_PlateauLogic_Enabled;/* '<S66>/Extr_HighSpd_PlateauLogic_Enabled' */

#define EXTRACTION_DW_TYPE_VARIANT_EXISTS
#endif                                 /* EXTR_VARIANT_HIGH_SPEED_PE_ENABLED */

#if EXTR_VARIANT_HIGH_SPEED_PE_DISABLED

    Extraction_DW_Extr_HighSpd_PlateauLogic_Disabled_TYPE
        Extr_HighSpd_PlateauLogic_Disabled;/* '<S66>/Extr_HighSpd_PlateauLogic_Disabled' */

#define EXTRACTION_DW_TYPE_VARIANT_EXISTS
#endif                                 /* EXTR_VARIANT_HIGH_SPEED_PE_DISABLED */

#if EXTR_VARIANT_HIGH_SPEED_PE_ENABLED

    Extraction_DW_Extr_HighSpd_LoadMassLimitCheck_Enabled_TYPE
        Extr_HighSpd_LoadMassLimitCheck_Enabled;/* '<S65>/Extr_HighSpd_LoadMassLimitCheck_Enabled' */

#define EXTRACTION_DW_TYPE_VARIANT_EXISTS
#endif                                 /* EXTR_VARIANT_HIGH_SPEED_PE_ENABLED */

#if EXTR_VARIANT_HIGH_SPEED_PE_ENABLED

    Extraction_DW_Extr_HighSpd_CEM_Enabled_TYPE Extr_HighSpd_CEM_Enabled;/* '<S63>/Extr_HighSpd_CEM_Enabled' */

#define EXTRACTION_DW_TYPE_VARIANT_EXISTS
#endif                                 /* EXTR_VARIANT_HIGH_SPEED_PE_ENABLED */

#if EXTR_VARIANT_HIGH_SPEED_PE_ENABLED

    Extraction_DW_UpdateSpinPulseConfig_Enabled_TYPE
        UpdateSpinPulseConfig_Enabled; /* '<S108>/UpdateSpinPulseConfig_Enabled' */

#define EXTRACTION_DW_TYPE_VARIANT_EXISTS
#endif                                 /* EXTR_VARIANT_HIGH_SPEED_PE_ENABLED */

    Extraction_DW_TriggerGen_TYPE sf_TriggerGen_md;/* '<S88>/TriggerGen' */

#if EXTR_VARIANT_HIGH_SPEED_PE_ENABLED

    Extraction_DW_Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled_TYPE
        Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled;/* '<S86>/Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled' */

#define EXTRACTION_DW_TYPE_VARIANT_EXISTS
#endif                                 /* EXTR_VARIANT_HIGH_SPEED_PE_ENABLED */

#if EXTR_VARIANT_HIGH_SPEED_PE_DISABLED

    Extraction_DW_Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled_TYPE
        Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled;/* '<S86>/Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled' */

#define EXTRACTION_DW_TYPE_VARIANT_EXISTS
#endif                                 /* EXTR_VARIANT_HIGH_SPEED_PE_DISABLED */

    Extraction_DW_TriggerGen_TYPE sf_TriggerGen_kb;/* '<S130>/TriggerGen' */

#if EXTR_VARIANT_OPT_RAMP_ENABLED

    Extraction_DW_Extr_LowSpd_OptRamp_Enabled_TYPE Extr_LowSpd_OptRamp_Enabled;/* '<S127>/Extr_LowSpd_OptRamp_Enabled' */

#define EXTRACTION_DW_TYPE_VARIANT_EXISTS
#endif                                 /* EXTR_VARIANT_OPT_RAMP_ENABLED */

    Extraction_DW_TriggerGen_TYPE sf_TriggerGen_ih;/* '<S174>/TriggerGen' */
    Extraction_DW_TriggerGen_TYPE sf_TriggerGen;/* '<S204>/TriggerGen' */
}
Extraction_DW_TYPE;

/* Zero-crossing (trigger) state */
typedef struct
{

#if EXTR_VARIANT_ACCELEROMETER_ENABLED

    Extraction_ZCE_Extr_Accel_Enabled_TYPE Extr_Accel_Enabled_gt;/* '<S26>/Extr_Accel_Enabled' */

#define EXTRACTION_PREVZCX_TYPE_VARIANT_EXISTS
#endif                                 /* EXTR_VARIANT_ACCELEROMETER_ENABLED */

    ZCSigState Extr_Rebal_Logic_Trig_ZCE[11];/* '<S12>/Extr_Rebal_Logic' */

#if EXTR_VARIANT_HIGH_SPEED_PE_ENABLED

    Extraction_ZCE_Extr_HighSpd_RampLogic_Enabled_TYPE
        Extr_HighSpd_RampLogic_Enabled;/* '<S67>/Extr_HighSpd_RampLogic_Enabled' */

#define EXTRACTION_PREVZCX_TYPE_VARIANT_EXISTS
#endif                                 /* EXTR_VARIANT_HIGH_SPEED_PE_ENABLED */

#if EXTR_VARIANT_HIGH_SPEED_PE_DISABLED

    Extraction_ZCE_Extr_HighSpd_RampLogic_Disabled_TYPE
        Extr_HighSpd_RampLogic_Disabled;/* '<S67>/Extr_HighSpd_RampLogic_Disabled' */

#define EXTRACTION_PREVZCX_TYPE_VARIANT_EXISTS
#endif                                 /* EXTR_VARIANT_HIGH_SPEED_PE_DISABLED */

#if EXTR_VARIANT_HIGH_SPEED_PE_ENABLED

    Extraction_ZCE_Extr_HighSpd_PlateauLogic_Enabled_TYPE
        Extr_HighSpd_PlateauLogic_Enabled;/* '<S66>/Extr_HighSpd_PlateauLogic_Enabled' */

#define EXTRACTION_PREVZCX_TYPE_VARIANT_EXISTS
#endif                                 /* EXTR_VARIANT_HIGH_SPEED_PE_ENABLED */

#if EXTR_VARIANT_HIGH_SPEED_PE_DISABLED

    Extraction_ZCE_Extr_HighSpd_PlateauLogic_Disabled_TYPE
        Extr_HighSpd_PlateauLogic_Disabled;/* '<S66>/Extr_HighSpd_PlateauLogic_Disabled' */

#define EXTRACTION_PREVZCX_TYPE_VARIANT_EXISTS
#endif                                 /* EXTR_VARIANT_HIGH_SPEED_PE_DISABLED */

#if EXTR_VARIANT_HIGH_SPEED_PE_ENABLED

    Extraction_ZCE_Extr_HighSpd_LoadMassLimitCheck_Enabled_TYPE
        Extr_HighSpd_LoadMassLimitCheck_Enabled;/* '<S65>/Extr_HighSpd_LoadMassLimitCheck_Enabled' */

#define EXTRACTION_PREVZCX_TYPE_VARIANT_EXISTS
#endif                                 /* EXTR_VARIANT_HIGH_SPEED_PE_ENABLED */

    ZCSigState Extr_HighSpd_ExtUnbCheckLogic_Trig_ZCE[4];/* '<S64>/Extr_HighSpd_ExtUnbCheckLogic' */

#if EXTR_VARIANT_HIGH_SPEED_PE_ENABLED

    Extraction_ZCE_Extr_HighSpd_CEM_Enabled_TYPE Extr_HighSpd_CEM_Enabled;/* '<S63>/Extr_HighSpd_CEM_Enabled' */

#define EXTRACTION_PREVZCX_TYPE_VARIANT_EXISTS
#endif                                 /* EXTR_VARIANT_HIGH_SPEED_PE_ENABLED */

#if EXTR_VARIANT_HIGH_SPEED_PE_ENABLED

    Extraction_ZCE_Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled_TYPE
        Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled;/* '<S86>/Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled' */

#define EXTRACTION_PREVZCX_TYPE_VARIANT_EXISTS
#endif                                 /* EXTR_VARIANT_HIGH_SPEED_PE_ENABLED */

#if EXTR_VARIANT_HIGH_SPEED_PE_DISABLED

    Extraction_ZCE_Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled_TYPE
        Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled;/* '<S86>/Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled' */

#define EXTRACTION_PREVZCX_TYPE_VARIANT_EXISTS
#endif                                 /* EXTR_VARIANT_HIGH_SPEED_PE_DISABLED */

#if EXTR_VARIANT_OPT_RAMP_ENABLED

    Extraction_ZCE_Extr_LowSpd_OptRamp_Enabled_TYPE Extr_LowSpd_OptRamp_Enabled;/* '<S127>/Extr_LowSpd_OptRamp_Enabled' */

#define EXTRACTION_PREVZCX_TYPE_VARIANT_EXISTS
#endif                                 /* EXTR_VARIANT_OPT_RAMP_ENABLED */

    ZCSigState MDDPLogic_Trig_ZCE[4];  /* '<S126>/MDDPLogic' */
    ZCSigState Extr_LowSpd_LoadMeasLogic_Trig_ZCE[4];/* '<S125>/Extr_LowSpd_LoadMeasLogic' */
    ZCSigState Extr_LowSpd_BasicUnbCheck_Trig_ZCE[3];/* '<S124>/Extr_LowSpd_BasicUnbCheck' */
    ZCSigState Extr_LowSpd_SupervisorLogic_Trig_ZCE[20];/* '<S122>/Extr_LowSpd_SupervisorLogic' */
    ZCSigState Extr_Sup_MotorSpeedCheck_Trig_ZCE[2];/* '<S193>/Extr_Sup_MotorSpeedCheck' */
    ZCSigState Extr_Sup_Logic_Trig_ZCE[18];/* '<S193>/Extr_Sup_Logic' */
}
Extraction_PrevZCX_TYPE;

/* External inputs (root inport signals with auto storage) */
typedef struct
{
    PARAM_EST_OUTPUT_TYPE ParamEst_Data;/* '<Root>/ParamEst_Data' */
    DIST_UTILS_OUTPUT_TYPE DistUtils_Data;/* '<Root>/DistUtils_Data' */
    MOTOR_OUTPUT_TYPE MCI_MotorSignals;/* '<Root>/MCI_MotorSignals' */
    sint16 Accel_FiltData[3];          /* '<Root>/Accel_FiltData' */
    float32 ACU_LoadMass;              /* '<Root>/ACU_LoadMass' */
    float32 ACU_WaterTemp;             /* '<Root>/ACU_WaterTemp' */
    uint16 ACU_MinTargetSpeed;         /* '<Root>/ACU_MinTargetSpeed' */
    uint16 ACU_MaxTargetSpeed;         /* '<Root>/ACU_MaxTargetSpeed' */
    uint16 ACU_ExtPlateauDuration;     /* '<Root>/ACU_ExtPlateauDuration' */
    uint8 ACU_MaxSpeedError;           /* '<Root>/ACU_MaxSpeedError' */
    uint8 ACU_RampTimeout;             /* '<Root>/ACU_RampTimeout' */
    uint16 MCI_RawUnbalance;           /* '<Root>/MCI_RawUnbalance' */
    uint16 MCI_RawUnbalanceCnt;        /* '<Root>/MCI_RawUnbalanceCnt' */
}
Extraction_ExtU_TYPE;

/* Imported (extern) block parameters */
extern BALANCE_INDEX_CONVERSION_TABLE_TYPE GESE_Extr_BalanceIndexConversionLut;/* Variable: GESE_Extr_BalanceIndexConversionLut
                                                                      * Referenced by: '<S146>/BalanceIndexToGrams'
                                                                      */

/* Block signals (auto storage) */
extern Extraction_B_TYPE Extraction_B;

/* Block states (auto storage) */
extern Extraction_DW_TYPE Extraction_DW;

/* External inputs (root inport signals with auto storage) */
extern Extraction_ExtU_TYPE Extraction_U;

/* External data declarations for dependent source files */
extern const float32 Extraction_R32GND;/* float32 ground */
extern const SPIN_PULSE_TYPE Extraction_rtZSPIN_PULSE_TYPE;/* SPIN_PULSE_TYPE ground */
extern const HIGH_SPEED_PULSE_CONFIG_TYPE
    Extraction_rtZHIGH_SPEED_PULSE_CONFIG_TYPE;/* HIGH_SPEED_PULSE_CONFIG_TYPE ground */
extern const LOAD_MASS_LIMIT_SETUP_TYPE Extraction_rtZLOAD_MASS_LIMIT_SETUP_TYPE;/* LOAD_MASS_LIMIT_SETUP_TYPE ground */

/*
 * Exported Global Signals
 *
 * Note: Exported global signals are block signals with an exported global
 * storage class designation.  Code generation will declare the memory for
 * these signals and export their symbols.
 *
 */
extern float32 Extr_Rebal_DistP2PThresh;/* '<S12>/Extr_Rebal_Logic' */
extern float32 Extr_Rebal_DistP2PTorque;/* '<S12>/Extr_Rebal_Logic' */
extern float32 Extr_HighSpd_ExtUnbAvg; /* '<S64>/Extr_HighSpd_ExtUnbCheckLogic' */
extern float32 Extr_HighSpd_ExtUnbAvgThreshold;/* '<S64>/Extr_HighSpd_ExtUnbCheckLogic' */
extern float32 Extr_HighSpd_PlateauDuration;/* '<S90>/MATLAB Function' */
extern float32 Extr_HighSpd_BaselineFriction;/*  */
extern float32 Extr_HighSpd_PrevBaselineFriction;/*  */
extern float32 Extr_Sup_RampTime;      /* '<S193>/Extr_Sup_MotorSpeedCheck' */
extern float32 Extr_Sup_RampTimeout;   /* '<S193>/Extr_Sup_MotorSpeedCheck' */
extern EXT_UNB_CHECK_STATE Extr_HighSpd_ExtUnbState;/* '<S64>/Extr_HighSpd_ExtUnbCheckLogic' */
extern HIGH_SPEED_PLATEAU_DECISION Extr_HighSpd_PlateauDecision;/*  */
extern LOAD_MEAS_STATE Extr_LowSpd_LoadMeasState;/* '<S125>/Extr_LowSpd_LoadMeasLogic' */
extern MDDP_STATE Extr_LowSpd_MddpState;/* '<S126>/MDDPLogic' */
extern LOW_SPEED_STATE Extr_LowSpd_State;/* '<S122>/Extr_LowSpd_SupervisorLogic' */
extern EXTR_REBAL_STATE Extr_Rebal_State;/* '<S12>/Extr_Rebal_Logic' */
extern EXTR_SUP_SPEED_ERROR_MONITOR_STATE Extr_Sup_SpeedErrorMonitorState;/* '<S193>/Extr_Sup_MotorSpeedCheck' */
extern sint16 Extr_LowSpd_DistSpeed;   /* '<S122>/DistSpeed' */
extern sint16 Extr_Sup_SpdCommand[2];  /* '<S23>/SpinCommand' */
extern uint16 Extr_Accel_ExtPlateauTimeIncrement;/*  */
extern uint16 Extr_HighSpd_TargetSpeed;/*  */
extern uint16 Extr_LowSpd_MaxTargetSpeed;/* '<S121>/SpeedLimit' */
extern uint16 Extr_LowSpd_UnbChkSpdLimit;/* '<S124>/Extr_LowSpd_BasicUnbCheck' */
extern uint16 Extr_LowSpd_UnbTargetArray[25];/* '<S124>/Extr_LowSpd_BasicUnbCheck' */
extern uint8 Extr_Accel_VibMapSafeSpeed_div10;/*  */
extern uint8 Extr_Rebal_Index;         /* '<S183>/MATLAB Function' */
extern uint8 Extr_Rebal_RebalCnt;      /* '<S12>/Extr_Rebal_Logic' */
extern uint8 Extr_Rebal_PulseIndex;    /* '<S12>/Extr_Rebal_Logic' */
extern uint8 Extr_Rebal_ShrtRedistCnt; /* '<S12>/Extr_Rebal_Logic' */
extern uint8 Extr_HighSpd_ExtUnbSampleCnt;/* '<S64>/Extr_HighSpd_ExtUnbCheckLogic' */
extern uint8 Extr_HighSpd_BaselineFrictionIndex;/*  */
extern uint8 Extr_HighSpd_BaselineSpeed;/*  */
extern uint8 Extr_LowSpd_MddpDur;      /* '<S126>/MddpDuration' */
extern boolean_T Extr_Sup_SpdMonitorPaused;/* '<S193>/Extr_Sup_MotorSpeedCheck' */

#if EXTR_VARIANT_ACCELEROMETER_ENABLED

extern float32 Extr_Accel_VibMapFloorDisp;/* '<S31>/Extr_Accel_VibMapLogic' */

#endif                                 /* EXTR_VARIANT_ACCELEROMETER_ENABLED */

#if EXTR_VARIANT_ACCELEROMETER_ENABLED

extern float32 Extr_Accel_VibMapFloorDispThreshold;/* '<S31>/Extr_Accel_VibMapLogic' */

#endif                                 /* EXTR_VARIANT_ACCELEROMETER_ENABLED */

#if EXTR_VARIANT_ACCELEROMETER_ENABLED

extern float32 Extr_Accel_VibMapFiltAccel[3];/* '<S55>/SquaredAcceleration' */

#endif                                 /* EXTR_VARIANT_ACCELEROMETER_ENABLED */

#if EXTR_VARIANT_ACCELEROMETER_ENABLED

extern float32 Extr_Accel_Baseline[3]; /* '<S30>/Extr_Accel_Supervisor' */

#endif                                 /* EXTR_VARIANT_ACCELEROMETER_ENABLED */

#if EXTR_VARIANT_ACCELEROMETER_ENABLED

extern float32 Extr_Accel_FiltData[3]; /* '<S45>/FilterAccelerometerData' */

#endif                                 /* EXTR_VARIANT_ACCELEROMETER_ENABLED */

#if EXTR_VARIANT_ACCELEROMETER_ENABLED

extern float32 Extr_Accel_CabHitFiltAccel[3];/* '<S33>/SquaredAcceleration' */

#endif                                 /* EXTR_VARIANT_ACCELEROMETER_ENABLED */

#if EXTR_VARIANT_ACCELEROMETER_ENABLED

extern CAB_HIT_STATE Extr_Accel_CabHitState;/* '<S29>/Extr_Accel_CabHitLogic' */

#endif                                 /* EXTR_VARIANT_ACCELEROMETER_ENABLED */

#if EXTR_VARIANT_ACCELEROMETER_ENABLED

extern EXTR_ACCEL_STATE Extr_Accel_State;/* '<S30>/Extr_Accel_Supervisor' */

#endif                                 /* EXTR_VARIANT_ACCELEROMETER_ENABLED */

#if EXTR_VARIANT_ACCELEROMETER_ENABLED

extern VIB_MAP_STATE Extr_Accel_VibMapState;/* '<S31>/Extr_Accel_VibMapLogic' */

#endif                                 /* EXTR_VARIANT_ACCELEROMETER_ENABLED */

#if EXTR_VARIANT_ACCELEROMETER_ENABLED

extern sint16 Extr_Inputs_AccelBaselineRaw[3];/* '<S39>/GetAccelBaselineMacro' */

#endif                                 /* EXTR_VARIANT_ACCELEROMETER_ENABLED */

#if EXTR_VARIANT_ACCELEROMETER_ENABLED

extern sint16 Extr_Inputs_AccelRawData[3];/* '<S45>/GetAccelData' */

#endif                                 /* EXTR_VARIANT_ACCELEROMETER_ENABLED */

#if EXTR_VARIANT_ACCELEROMETER_ENABLED

extern uint16 Extr_Accel_CommErrors;   /* '<S30>/Extr_Accel_Supervisor' */

#endif                                 /* EXTR_VARIANT_ACCELEROMETER_ENABLED */

#if EXTR_VARIANT_ACCELEROMETER_ENABLED

extern uint8 Extr_Accel_SoftCabHitCounter;/* '<S29>/Extr_Accel_CabHitLogic' */

#endif                                 /* EXTR_VARIANT_ACCELEROMETER_ENABLED */

#if EXTR_VARIANT_ACCELEROMETER_ENABLED

extern boolean_T Extr_Accel_Acquiring; /* '<S30>/Extr_Accel_Supervisor' */

#endif                                 /* EXTR_VARIANT_ACCELEROMETER_ENABLED */

#if EXTR_VARIANT_HIGH_SPEED_PE_ENABLED

extern float32 Extr_HighSpd_RampViscousThreshold;/* '<S80>/Extr_HighSpd_RampLogic' */

#endif                                 /* EXTR_VARIANT_HIGH_SPEED_PE_ENABLED */

#if EXTR_VARIANT_HIGH_SPEED_PE_ENABLED

extern float32 Extr_HighSpd_RampFrictionThreshold;/* '<S80>/Extr_HighSpd_RampLogic' */

#endif                                 /* EXTR_VARIANT_HIGH_SPEED_PE_ENABLED */

#if EXTR_VARIANT_HIGH_SPEED_PE_ENABLED

extern float32 Extr_HighSpd_RampFrictionDelta;/* '<S80>/Extr_HighSpd_RampLogic' */

#endif                                 /* EXTR_VARIANT_HIGH_SPEED_PE_ENABLED */

#if EXTR_VARIANT_HIGH_SPEED_PE_ENABLED

extern HIGH_SPEED_RAMP_STATE Extr_HighSpd_RampState;/* '<S80>/Extr_HighSpd_RampLogic' */

#endif                                 /* EXTR_VARIANT_HIGH_SPEED_PE_ENABLED */

#if EXTR_VARIANT_HIGH_SPEED_PE_DISABLED

extern HIGH_SPEED_RAMP_STATE Extr_HighSpd_RampState;/* '<S79>/Extr_HighSpd_RampLogic' */

#endif                                 /* EXTR_VARIANT_HIGH_SPEED_PE_DISABLED */

#if EXTR_VARIANT_HIGH_SPEED_PE_ENABLED

extern float32 Extr_HighSpd_FrictionAvg;/* '<S75>/Extr_HighSpd_PlateauLogic' */

#endif                                 /* EXTR_VARIANT_HIGH_SPEED_PE_ENABLED */

#if EXTR_VARIANT_HIGH_SPEED_PE_ENABLED

extern float32 Extr_HighSpd_FrictionRng;/* '<S75>/Extr_HighSpd_PlateauLogic' */

#endif                                 /* EXTR_VARIANT_HIGH_SPEED_PE_ENABLED */

#if EXTR_VARIANT_HIGH_SPEED_PE_ENABLED

extern float32 Extr_HighSpd_DeltaFrictionAvg;/* '<S75>/Extr_HighSpd_PlateauLogic' */

#endif                                 /* EXTR_VARIANT_HIGH_SPEED_PE_ENABLED */

#if EXTR_VARIANT_HIGH_SPEED_PE_ENABLED

extern HIGH_SPEED_PLATEAU_STATE Extr_HighSpd_PlateauState;/* '<S75>/Extr_HighSpd_PlateauLogic' */

#endif                                 /* EXTR_VARIANT_HIGH_SPEED_PE_ENABLED */

#if EXTR_VARIANT_HIGH_SPEED_PE_ENABLED

extern boolean_T Extr_HighSpd_PlateauTOnDone;/* '<S75>/Extr_HighSpd_PlateauLogic' */

#endif                                 /* EXTR_VARIANT_HIGH_SPEED_PE_ENABLED */

#if EXTR_VARIANT_HIGH_SPEED_PE_DISABLED

extern HIGH_SPEED_PLATEAU_STATE Extr_HighSpd_PlateauState;/* '<S74>/Extr_HighSpd_PlateauLogic' */

#endif                                 /* EXTR_VARIANT_HIGH_SPEED_PE_DISABLED */

#if EXTR_VARIANT_HIGH_SPEED_PE_ENABLED

extern float32 Extr_HighSpd_LoadMassLimit;/* '<S72>/Extr_HighSpd_LoadMassLimitCheckLogic' */

#endif                                 /* EXTR_VARIANT_HIGH_SPEED_PE_ENABLED */

#if EXTR_VARIANT_HIGH_SPEED_PE_ENABLED

extern float32 Extr_HighSpd_CEMFrictionThreshold;/* '<S69>/Extr_HighSpd_CEMLogic' */

#endif                                 /* EXTR_VARIANT_HIGH_SPEED_PE_ENABLED */

#if EXTR_VARIANT_HIGH_SPEED_PE_ENABLED

extern float32 Extr_HighSpd_CEMAvgFrictionDelta;/* '<S69>/Extr_HighSpd_CEMLogic' */

#endif                                 /* EXTR_VARIANT_HIGH_SPEED_PE_ENABLED */

#if EXTR_VARIANT_HIGH_SPEED_PE_ENABLED

extern CEM_STATE Extr_HighSpd_CEMState;/* '<S69>/Extr_HighSpd_CEMLogic' */

#endif                                 /* EXTR_VARIANT_HIGH_SPEED_PE_ENABLED */

#if EXTR_VARIANT_ACCELEROMETER_ENABLED

extern uint16 Extr_Accel_VibMapRefDisp;/* '<S119>/VerifyVibMapFloorDisplacementMATLAB' */

#endif                                 /* EXTR_VARIANT_ACCELEROMETER_ENABLED */

#if EXTR_VARIANT_OPT_RAMP_ENABLED

extern float32 Extr_LowSpd_OptRampUpperTh;/* '<S150>/OptRampLogic' */

#endif                                 /* EXTR_VARIANT_OPT_RAMP_ENABLED */

#if EXTR_VARIANT_OPT_RAMP_ENABLED

extern float32 Extr_LowSpd_OptRampLowerTh;/* '<S150>/OptRampLogic' */

#endif                                 /* EXTR_VARIANT_OPT_RAMP_ENABLED */

#if EXTR_VARIANT_OPT_RAMP_ENABLED

extern float32 Extr_LowSpd_OptRampUnbMass;/* '<S150>/OptRampLogic' */

#endif                                 /* EXTR_VARIANT_OPT_RAMP_ENABLED */

#if EXTR_VARIANT_OPT_RAMP_ENABLED

extern float32 Extr_LowSpd_OptRampBBMass;/* '<S150>/OptRampLogic' */

#endif                                 /* EXTR_VARIANT_OPT_RAMP_ENABLED */

#if EXTR_VARIANT_OPT_RAMP_ENABLED

extern float32 Extr_LowSpd_OptRampORA2BBA;/* '<S150>/OptRampLogic' */

#endif                                 /* EXTR_VARIANT_OPT_RAMP_ENABLED */

#if EXTR_VARIANT_OPT_RAMP_ENABLED

extern float32 Extr_LowSpd_OptRampPhsTol;/* '<S150>/OptRampLogic' */

#endif                                 /* EXTR_VARIANT_OPT_RAMP_ENABLED */

#if EXTR_VARIANT_OPT_RAMP_ENABLED

extern float32 Extr_LowSpd_OptRampPhsAdvTgt;/* '<S150>/OptRampLogic' */

#endif                                 /* EXTR_VARIANT_OPT_RAMP_ENABLED */

#if EXTR_VARIANT_OPT_RAMP_ENABLED

extern OPT_RAMP_STATE Extr_LowSpd_OptRampState;/* '<S150>/OptRampLogic' */

#endif                                 /* EXTR_VARIANT_OPT_RAMP_ENABLED */

#if EXTR_VARIANT_OPT_RAMP_ENABLED

extern OPT_RAMP_EXIT_REASON Extr_LowSpd_OptRampExitReason;/* '<S150>/OptRampLogic' */

#endif                                 /* EXTR_VARIANT_OPT_RAMP_ENABLED */

#if EXTR_VARIANT_OPT_RAMP_ENABLED

extern uint16 Extr_LowSpd_OptRampConvUnb;/* '<S153>/Max' */

#endif                                 /* EXTR_VARIANT_OPT_RAMP_ENABLED */

#if EXTR_VARIANT_OPT_RAMP_ENABLED

extern uint8 Extr_LowSpd_OptRampPlatTime;/* '<S150>/OptRampLogic' */

#endif                                 /* EXTR_VARIANT_OPT_RAMP_ENABLED */

#if EXTR_VARIANT_OPT_RAMP_ENABLED

extern uint8 Extr_LowSpd_OptRampConvTime;/* '<S153>/SumpTempToConvTime' */

#endif                                 /* EXTR_VARIANT_OPT_RAMP_ENABLED */

extern PARAM_EST_OUTPUT_TYPE Extr_Inputs_ParamEstData;/* '<S221>/Bus Creator' */
extern DIST_UTILS_OUTPUT_TYPE Extr_Inputs_DistUtilsData;/* '<S14>/GetDistUtilsOutput' */
extern MOTOR_OUTPUT_TYPE Extr_Inputs_MotorSignals;/* '<S216>/Bus Creator' */
extern float32 Extr_Inputs_WaterTemp;  /* '<S14>/GetWaterTemp' */
extern float32 Extr_Inputs_AcuLoadMass;/* '<S14>/GetSaturatedLoadMass' */
extern uint16 Extr_Inputs_MinFinalSpeed;/* '<S14>/GetMinSpeed' */
extern uint16 Extr_Inputs_UnbalanceIndexCnt;/* '<S14>/GetMciRawUnbalanceCount' */
extern uint16 Extr_Inputs_UnbalanceIndex;/* '<S14>/GetMciRawUnbalance' */
extern uint16 Extr_Inputs_MaxSpeed;    /* '<S14>/GetFinalSpeed' */
extern uint16 Extr_Inputs_ExtPlateauDuration;/* '<S14>/GetExtPlateauDuration' */
extern uint8 Extr_Inputs_RampTimeout;  /* '<S14>/GetRampTimeout' */
extern uint8 Extr_Inputs_MaxSpeedError;/* '<S14>/GetMaxSpeedError' */

/*
 * Exported States
 *
 * Note: Exported states are block states with an exported global
 * storage class designation.  Code generation will declare the memory for these
 * states and exports their symbols.
 *
 */
extern float32 Extr_LowSpd_LoadMass;   /* Simulink.Signal object 'Extr_LowSpd_LoadMass' */
extern float32 Extr_Accel_MaxAccelSpeed;/* Simulink.Signal object 'Extr_Accel_MaxAccelSpeed' */
extern float32 Extr_Accel_MaxAccel;    /* Simulink.Signal object 'Extr_Accel_MaxAccel' */
extern float32 Extr_Sup_LoadMass;      /* Simulink.Signal object 'Extr_Sup_LoadMass' */
extern float32 Extr_HighSpd_ViscousFriction;/* Simulink.Signal object 'Extr_HighSpd_ViscousFriction' */
extern float32 Extr_HighSpd_PrevPlateauTime;/* Simulink.Signal object 'Extr_HighSpd_PrevPlateauTime' */
extern float32 Extr_HighSpd_Friction;  /* Simulink.Signal object 'Extr_HighSpd_Friction' */
extern float32 Extr_HighSpd_ReferenceFriction;/* Simulink.Signal object 'Extr_HighSpd_ReferenceFriction' */
extern float32 Extr_Accel_MaxVibMapFloorDisp;/* Simulink.Signal object 'Extr_Accel_MaxVibMapFloorDisp' */
extern HIGH_SPEED_STATE Extr_HighSpd_State;/* Simulink.Signal object 'Extr_HighSpd_State' */
extern EXTRACTION_STATE Extr_Sup_State;/* Simulink.Signal object 'Extr_Sup_State' */
extern uint16 Extr_LowSpd_Unbalance;   /* Simulink.Signal object 'Extr_LowSpd_Unbalance' */
extern uint16 Extr_HighSpd_CEMEventCounter;/* Simulink.Signal object 'Extr_HighSpd_CEMEventCounter' */
extern uint16 Extr_Accel_SoftCabHitCounterOverall;/* Simulink.Signal object 'Extr_Accel_SoftCabHitCounterOverall' */
extern uint16 Extr_Accel_HardCabHitCounter;/* Simulink.Signal object 'Extr_Accel_HardCabHitCounter' */
extern uint16 Extr_LowSpd_MddpSpd;     /* '<S126>/MDDPLogic' */
extern sint8 Extr_HighSpd_CurrPulseInd;/* Simulink.Signal object 'Extr_HighSpd_CurrPulseInd' */
extern uint8 Extr_Sup_SpdComCounter;   /* Simulink.Signal object 'Extr_Sup_SpdComCounter' */
extern uint8 Extr_LowSpd_RebalCnt;     /* Simulink.Signal object 'Extr_LowSpd_RebalCnt' */
extern uint8 Extr_HighSpd_RebalCnt;    /* Simulink.Signal object 'Extr_HighSpd_RebalCnt' */
extern uint8 Extr_LowSpd_MddpRmp;      /* '<S126>/MDDPLogic' */
extern boolean_T Extr_Rebal_RebInProgress;/* Simulink.Signal object 'Extr_Rebal_RebInProgress' */

/* Model entry point functions */
void Extraction_initialize(void);

/* Exported entry point functions */
void Extraction__Handler5ms(void);

/* Exported entry point functions */
void Extraction__Start(void);

/* Exported entry point functions */
void Extraction__Stop(void);

/* Exported entry point functions */
void Extraction__Resume(void);

/* Exported entry point functions */
void Extraction__End(void);

/* Exported entry point functions */
void Extraction__ResetLoadMass(void);

/* Exported entry point functions */
void Extraction__NotifyFoamDetection(void);

/* Exported data declaration */

/* Declaration for custom storage class: Global */
#if EXTR_VARIANT_HIGH_SPEED_PE_ENABLED

extern EXTR_HIGH_SPD_FAULT Extr_HighSpd_Fault;

#endif                                 /* EXTR_VARIANT_HIGH_SPEED_PE_ENABLED */

#if EXTR_VARIANT_HIGH_SPEED_PE_DISABLED

extern EXTR_HIGH_SPD_FAULT Extr_HighSpd_Fault;

#endif                                 /* EXTR_VARIANT_HIGH_SPEED_PE_DISABLED */

extern EXTR_LOW_SPD_FAULT Extr_LowSpd_Fault;
extern EXTR_REBAL_FAULT Extr_Rebal_Fault;
extern uint32 Extr_Sup_Fault;

/*-
 * These blocks were eliminated from the model due to optimizations:
 *
 * Block '<S110>/Data Type Duplicate' : Unused code path elimination
 * Block '<S110>/Data Type Propagation' : Unused code path elimination
 * Block '<S17>/Display' : Unused code path elimination
 * Block '<S241>/Bus Creator' : Unused code path elimination
 * Block '<S245>/Bus Creator' : Unused code path elimination
 * Block '<S246>/FixPt Data Type Duplicate' : Unused code path elimination
 * Block '<S264>/Bus Creator' : Unused code path elimination
 * Block '<S124>/UnbChkSpdLimit' : Eliminate redundant signal conversion block
 */

/*-
 * The generated code includes comments that allow you to trace directly
 * back to the appropriate location in the model.  The basic format
 * is <system>/block_name, where system is the system number (uniquely
 * assigned by Simulink) and block_name is the name of the block.
 *
 * Use the MATLAB hilite_system command to trace the generated code back
 * to the model.  For example,
 *
 * hilite_system('<S3>')    - opens system 3
 * hilite_system('<S3>/Kp') - opens and selects block Kp which resides in S3
 *
 * Here is the system hierarchy for this model
 *
 * '<Root>' : 'Extraction'
 * '<S1>'   : 'Extraction/ClearFault'
 * '<S2>'   : 'Extraction/Extraction'
 * '<S3>'   : 'Extraction/GetFault'
 * '<S4>'   : 'Extraction/GetFloat'
 * '<S5>'   : 'Extraction/GetInteger'
 * '<S6>'   : 'Extraction/GetState'
 * '<S7>'   : 'Extraction/Initialize'
 * '<S8>'   : 'Extraction/IsRebalInProgress'
 * '<S9>'   : 'Extraction/Extraction/Extr_Accel'
 * '<S10>'  : 'Extraction/Extraction/Extr_HighSpd'
 * '<S11>'  : 'Extraction/Extraction/Extr_LowSpd'
 * '<S12>'  : 'Extraction/Extraction/Extr_Rebalance'
 * '<S13>'  : 'Extraction/Extraction/Extr_Supervisor'
 * '<S14>'  : 'Extraction/Extraction/Extr_UpdateInputs'
 * '<S15>'  : 'Extraction/Extraction/GetMciFault'
 * '<S16>'  : 'Extraction/Extraction/IsAccelEnabled'
 * '<S17>'  : 'Extraction/Extraction/IsMotorRunning'
 * '<S18>'  : 'Extraction/Extraction/ParamEstControl'
 * '<S19>'  : 'Extraction/Extraction/ParamEstExcControl'
 * '<S20>'  : 'Extraction/Extraction/ReleaseMotion'
 * '<S21>'  : 'Extraction/Extraction/SelectParamEstOutputs'
 * '<S22>'  : 'Extraction/Extraction/SetFault'
 * '<S23>'  : 'Extraction/Extraction/SetSpeedCommand'
 * '<S24>'  : 'Extraction/Extraction/StopMotor'
 * '<S25>'  : 'Extraction/Extraction/UpdateParamEstSetup'
 * '<S26>'  : 'Extraction/Extraction/Extr_Accel/Extr_Accel_Enabled'
 * '<S27>'  : 'Extraction/Extraction/Extr_Accel/TriggerGen'
 * '<S28>'  : 'Extraction/Extraction/Extr_Accel/Extr_Accel_Enabled/Extr_Accel_Enabled'
 * '<S29>'  : 'Extraction/Extraction/Extr_Accel/Extr_Accel_Enabled/Extr_Accel_Enabled/Extr_Accel_CabHit'
 * '<S30>'  : 'Extraction/Extraction/Extr_Accel/Extr_Accel_Enabled/Extr_Accel_Enabled/Extr_Accel_Supervisor'
 * '<S31>'  : 'Extraction/Extraction/Extr_Accel/Extr_Accel_Enabled/Extr_Accel_Enabled/Extr_Accel_VibMap'
 * '<S32>'  : 'Extraction/Extraction/Extr_Accel/Extr_Accel_Enabled/Extr_Accel_Enabled/UpdateMaximumAcceleration'
 * '<S33>'  : 'Extraction/Extraction/Extr_Accel/Extr_Accel_Enabled/Extr_Accel_Enabled/Extr_Accel_CabHit/AccelInputFilter'
 * '<S34>'  : 'Extraction/Extraction/Extr_Accel/Extr_Accel_Enabled/Extr_Accel_Enabled/Extr_Accel_CabHit/Extr_Accel_CabHitLogic'
 * '<S35>'  : 'Extraction/Extraction/Extr_Accel/Extr_Accel_Enabled/Extr_Accel_Enabled/Extr_Accel_CabHit/AccelInputFilter/FilterAccelerometerData'
 * '<S36>'  : 'Extraction/Extraction/Extr_Accel/Extr_Accel_Enabled/Extr_Accel_Enabled/Extr_Accel_Supervisor/AccelSampleReader'
 * '<S37>'  : 'Extraction/Extraction/Extr_Accel/Extr_Accel_Enabled/Extr_Accel_Enabled/Extr_Accel_Supervisor/AccelerometerControl'
 * '<S38>'  : 'Extraction/Extraction/Extr_Accel/Extr_Accel_Enabled/Extr_Accel_Enabled/Extr_Accel_Supervisor/Extr_Accel_Supervisor'
 * '<S39>'  : 'Extraction/Extraction/Extr_Accel/Extr_Accel_Enabled/Extr_Accel_Enabled/Extr_Accel_Supervisor/GetAccelBaseline'
 * '<S40>'  : 'Extraction/Extraction/Extr_Accel/Extr_Accel_Enabled/Extr_Accel_Enabled/Extr_Accel_Supervisor/GetAccelBaselineStatus'
 * '<S41>'  : 'Extraction/Extraction/Extr_Accel/Extr_Accel_Enabled/Extr_Accel_Enabled/Extr_Accel_Supervisor/GetAccelErrorCount'
 * '<S42>'  : 'Extraction/Extraction/Extr_Accel/Extr_Accel_Enabled/Extr_Accel_Enabled/Extr_Accel_Supervisor/GetAccelStatus'
 * '<S43>'  : 'Extraction/Extraction/Extr_Accel/Extr_Accel_Enabled/Extr_Accel_Enabled/Extr_Accel_Supervisor/MeasureBaseline'
 * '<S44>'  : 'Extraction/Extraction/Extr_Accel/Extr_Accel_Enabled/Extr_Accel_Enabled/Extr_Accel_Supervisor/RunSelfTest'
 * '<S45>'  : 'Extraction/Extraction/Extr_Accel/Extr_Accel_Enabled/Extr_Accel_Enabled/Extr_Accel_Supervisor/AccelSampleReader/Subsystem'
 * '<S46>'  : 'Extraction/Extraction/Extr_Accel/Extr_Accel_Enabled/Extr_Accel_Enabled/Extr_Accel_Supervisor/AccelSampleReader/Subsystem/FilterAccelerometerData'
 * '<S47>'  : 'Extraction/Extraction/Extr_Accel/Extr_Accel_Enabled/Extr_Accel_Enabled/Extr_Accel_Supervisor/AccelSampleReader/Subsystem/GetAccelData'
 * '<S48>'  : 'Extraction/Extraction/Extr_Accel/Extr_Accel_Enabled/Extr_Accel_Enabled/Extr_Accel_Supervisor/AccelerometerControl/AccelerometerControlMacro'
 * '<S49>'  : 'Extraction/Extraction/Extr_Accel/Extr_Accel_Enabled/Extr_Accel_Enabled/Extr_Accel_Supervisor/GetAccelBaseline/GetAccelBaselineMacro'
 * '<S50>'  : 'Extraction/Extraction/Extr_Accel/Extr_Accel_Enabled/Extr_Accel_Enabled/Extr_Accel_Supervisor/GetAccelBaselineStatus/GetAccelBaselineStatusMacro'
 * '<S51>'  : 'Extraction/Extraction/Extr_Accel/Extr_Accel_Enabled/Extr_Accel_Enabled/Extr_Accel_Supervisor/GetAccelErrorCount/GetAccelErrorCountMacro'
 * '<S52>'  : 'Extraction/Extraction/Extr_Accel/Extr_Accel_Enabled/Extr_Accel_Enabled/Extr_Accel_Supervisor/GetAccelStatus/GetAccelStatusMacro'
 * '<S53>'  : 'Extraction/Extraction/Extr_Accel/Extr_Accel_Enabled/Extr_Accel_Enabled/Extr_Accel_Supervisor/MeasureBaseline/MeasureBaselineMacro'
 * '<S54>'  : 'Extraction/Extraction/Extr_Accel/Extr_Accel_Enabled/Extr_Accel_Enabled/Extr_Accel_Supervisor/RunSelfTest/RunSelfTestMacro'
 * '<S55>'  : 'Extraction/Extraction/Extr_Accel/Extr_Accel_Enabled/Extr_Accel_Enabled/Extr_Accel_VibMap/AccelInputFilter'
 * '<S56>'  : 'Extraction/Extraction/Extr_Accel/Extr_Accel_Enabled/Extr_Accel_Enabled/Extr_Accel_VibMap/Extr_Accel_VibMapLogic'
 * '<S57>'  : 'Extraction/Extraction/Extr_Accel/Extr_Accel_Enabled/Extr_Accel_Enabled/Extr_Accel_VibMap/AccelInputFilter/FilterAccelerometerData'
 * '<S58>'  : 'Extraction/Extraction/Extr_Accel/Extr_Accel_Enabled/Extr_Accel_Enabled/Extr_Accel_VibMap/Extr_Accel_VibMapLogic/CalcFloorDispThreshold'
 * '<S59>'  : 'Extraction/Extraction/Extr_Accel/Extr_Accel_Enabled/Extr_Accel_Enabled/UpdateMaximumAcceleration/If Action Subsystem'
 * '<S60>'  : 'Extraction/Extraction/Extr_Accel/TriggerGen/TriggerGen'
 * '<S61>'  : 'Extraction/Extraction/Extr_HighSpd/Extr_HighSpd_Algorithms'
 * '<S62>'  : 'Extraction/Extraction/Extr_HighSpd/Extr_HighSpd_Supervisor'
 * '<S63>'  : 'Extraction/Extraction/Extr_HighSpd/Extr_HighSpd_Algorithms/Extr_HighSpd_CEM'
 * '<S64>'  : 'Extraction/Extraction/Extr_HighSpd/Extr_HighSpd_Algorithms/Extr_HighSpd_ExtUnbChk'
 * '<S65>'  : 'Extraction/Extraction/Extr_HighSpd/Extr_HighSpd_Algorithms/Extr_HighSpd_LoadMassLimitCheck'
 * '<S66>'  : 'Extraction/Extraction/Extr_HighSpd/Extr_HighSpd_Algorithms/Extr_HighSpd_PlateauLogic'
 * '<S67>'  : 'Extraction/Extraction/Extr_HighSpd/Extr_HighSpd_Algorithms/Extr_HighSpd_RampLogic'
 * '<S68>'  : 'Extraction/Extraction/Extr_HighSpd/Extr_HighSpd_Algorithms/TriggerGen'
 * '<S69>'  : 'Extraction/Extraction/Extr_HighSpd/Extr_HighSpd_Algorithms/Extr_HighSpd_CEM/Extr_HighSpd_CEM_Enabled'
 * '<S70>'  : 'Extraction/Extraction/Extr_HighSpd/Extr_HighSpd_Algorithms/Extr_HighSpd_CEM/Extr_HighSpd_CEM_Enabled/Extr_HighSpd_CEMLogic'
 * '<S71>'  : 'Extraction/Extraction/Extr_HighSpd/Extr_HighSpd_Algorithms/Extr_HighSpd_ExtUnbChk/Extr_HighSpd_ExtUnbCheckLogic'
 * '<S72>'  : 'Extraction/Extraction/Extr_HighSpd/Extr_HighSpd_Algorithms/Extr_HighSpd_LoadMassLimitCheck/Extr_HighSpd_LoadMassLimitCheck_Enabled'
 * '<S73>'  : 'Extraction/Extraction/Extr_HighSpd/Extr_HighSpd_Algorithms/Extr_HighSpd_LoadMassLimitCheck/Extr_HighSpd_LoadMassLimitCheck_Enabled/Extr_HighSpd_LoadMassLimitCheckLogic'
 * '<S74>'  : 'Extraction/Extraction/Extr_HighSpd/Extr_HighSpd_Algorithms/Extr_HighSpd_PlateauLogic/Extr_HighSpd_PlateauLogic_Disabled'
 * '<S75>'  : 'Extraction/Extraction/Extr_HighSpd/Extr_HighSpd_Algorithms/Extr_HighSpd_PlateauLogic/Extr_HighSpd_PlateauLogic_Enabled'
 * '<S76>'  : 'Extraction/Extraction/Extr_HighSpd/Extr_HighSpd_Algorithms/Extr_HighSpd_PlateauLogic/Extr_HighSpd_PlateauLogic_Disabled/Extr_HighSpd_PlateauLogic'
 * '<S77>'  : 'Extraction/Extraction/Extr_HighSpd/Extr_HighSpd_Algorithms/Extr_HighSpd_PlateauLogic/Extr_HighSpd_PlateauLogic_Enabled/Extr_HighSpd_PlateauLogic'
 * '<S78>'  : 'Extraction/Extraction/Extr_HighSpd/Extr_HighSpd_Algorithms/Extr_HighSpd_PlateauLogic/Extr_HighSpd_PlateauLogic_Enabled/Extr_HighSpd_PlateauLogic/FRICTION_BASED_PLATEAU.MONITOR.UpdateFactoryCalibrationFriction'
 * '<S79>'  : 'Extraction/Extraction/Extr_HighSpd/Extr_HighSpd_Algorithms/Extr_HighSpd_RampLogic/Extr_HighSpd_RampLogic_Disabled'
 * '<S80>'  : 'Extraction/Extraction/Extr_HighSpd/Extr_HighSpd_Algorithms/Extr_HighSpd_RampLogic/Extr_HighSpd_RampLogic_Enabled'
 * '<S81>'  : 'Extraction/Extraction/Extr_HighSpd/Extr_HighSpd_Algorithms/Extr_HighSpd_RampLogic/Extr_HighSpd_RampLogic_Disabled/Extr_HighSpd_RampLogic'
 * '<S82>'  : 'Extraction/Extraction/Extr_HighSpd/Extr_HighSpd_Algorithms/Extr_HighSpd_RampLogic/Extr_HighSpd_RampLogic_Enabled/Extr_HighSpd_RampLogic'
 * '<S83>'  : 'Extraction/Extraction/Extr_HighSpd/Extr_HighSpd_Algorithms/Extr_HighSpd_RampLogic/Extr_HighSpd_RampLogic_Enabled/Extr_HighSpd_RampLogic/UpdateFactoryCalibrationFriction'
 * '<S84>'  : 'Extraction/Extraction/Extr_HighSpd/Extr_HighSpd_Algorithms/TriggerGen/TriggerGen'
 * '<S85>'  : 'Extraction/Extraction/Extr_HighSpd/Extr_HighSpd_Supervisor/Extr_HighSpd_Supervisor'
 * '<S86>'  : 'Extraction/Extraction/Extr_HighSpd/Extr_HighSpd_Supervisor/Extr_HighSpd_Supervisor/Extr_HighSpd_SupervisorLogic'
 * '<S87>'  : 'Extraction/Extraction/Extr_HighSpd/Extr_HighSpd_Supervisor/Extr_HighSpd_Supervisor/LoadSpinPulse'
 * '<S88>'  : 'Extraction/Extraction/Extr_HighSpd/Extr_HighSpd_Supervisor/Extr_HighSpd_Supervisor/TriggerGen'
 * '<S89>'  : 'Extraction/Extraction/Extr_HighSpd/Extr_HighSpd_Supervisor/Extr_HighSpd_Supervisor/UpdateCurrentPulseConfig'
 * '<S90>'  : 'Extraction/Extraction/Extr_HighSpd/Extr_HighSpd_Supervisor/Extr_HighSpd_Supervisor/UpdatePlateauTime'
 * '<S91>'  : 'Extraction/Extraction/Extr_HighSpd/Extr_HighSpd_Supervisor/Extr_HighSpd_Supervisor/VerifyVibMapFloorDisplacement'
 * '<S92>'  : 'Extraction/Extraction/Extr_HighSpd/Extr_HighSpd_Supervisor/Extr_HighSpd_Supervisor/Extr_HighSpd_SupervisorLogic/Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled'
 * '<S93>'  : 'Extraction/Extraction/Extr_HighSpd/Extr_HighSpd_Supervisor/Extr_HighSpd_Supervisor/Extr_HighSpd_SupervisorLogic/Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled'
 * '<S94>'  : 'Extraction/Extraction/Extr_HighSpd/Extr_HighSpd_Supervisor/Extr_HighSpd_Supervisor/Extr_HighSpd_SupervisorLogic/Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled/Extr_HighSpd_SupervisorLogic'
 * '<S95>'  : 'Extraction/Extraction/Extr_HighSpd/Extr_HighSpd_Supervisor/Extr_HighSpd_Supervisor/Extr_HighSpd_SupervisorLogic/Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled/ClearBaselineFrictionArrays'
 * '<S96>'  : 'Extraction/Extraction/Extr_HighSpd/Extr_HighSpd_Supervisor/Extr_HighSpd_Supervisor/Extr_HighSpd_SupervisorLogic/Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled/DecreaseCurrentBaselineFriction'
 * '<S97>'  : 'Extraction/Extraction/Extr_HighSpd/Extr_HighSpd_Supervisor/Extr_HighSpd_Supervisor/Extr_HighSpd_SupervisorLogic/Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled/Extr_HighSpd_SupervisorLogic'
 * '<S98>'  : 'Extraction/Extraction/Extr_HighSpd/Extr_HighSpd_Supervisor/Extr_HighSpd_Supervisor/Extr_HighSpd_SupervisorLogic/Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled/IncreaseCEMEventCounter'
 * '<S99>'  : 'Extraction/Extraction/Extr_HighSpd/Extr_HighSpd_Supervisor/Extr_HighSpd_Supervisor/Extr_HighSpd_SupervisorLogic/Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled/IncreaseCurrentBaselineFriction'
 * '<S100>' : 'Extraction/Extraction/Extr_HighSpd/Extr_HighSpd_Supervisor/Extr_HighSpd_Supervisor/Extr_HighSpd_SupervisorLogic/Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled/UpdateBaselineFrictionArray'
 * '<S101>' : 'Extraction/Extraction/Extr_HighSpd/Extr_HighSpd_Supervisor/Extr_HighSpd_Supervisor/Extr_HighSpd_SupervisorLogic/Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled/VerifyBaselineFrictionArray'
 * '<S102>' : 'Extraction/Extraction/Extr_HighSpd/Extr_HighSpd_Supervisor/Extr_HighSpd_Supervisor/Extr_HighSpd_SupervisorLogic/Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled/ClearBaselineFrictionArrays/ClearBaselineFrictionArraysMATLAB'
 * '<S103>' : 'Extraction/Extraction/Extr_HighSpd/Extr_HighSpd_Supervisor/Extr_HighSpd_Supervisor/Extr_HighSpd_SupervisorLogic/Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled/DecreaseCurrentBaselineFriction/DecreaseCurrentBaselineFrictionMATLAB'
 * '<S104>' : 'Extraction/Extraction/Extr_HighSpd/Extr_HighSpd_Supervisor/Extr_HighSpd_Supervisor/Extr_HighSpd_SupervisorLogic/Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled/IncreaseCurrentBaselineFriction/IncreaseCurrentBaselineFrictionMATLAB'
 * '<S105>' : 'Extraction/Extraction/Extr_HighSpd/Extr_HighSpd_Supervisor/Extr_HighSpd_Supervisor/Extr_HighSpd_SupervisorLogic/Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled/UpdateBaselineFrictionArray/UpdateBaselineFrictionArrayMATLAB'
 * '<S106>' : 'Extraction/Extraction/Extr_HighSpd/Extr_HighSpd_Supervisor/Extr_HighSpd_Supervisor/Extr_HighSpd_SupervisorLogic/Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled/VerifyBaselineFrictionArray/VerifyBaselineFrictionArrayMATLAB'
 * '<S107>' : 'Extraction/Extraction/Extr_HighSpd/Extr_HighSpd_Supervisor/Extr_HighSpd_Supervisor/TriggerGen/TriggerGen'
 * '<S108>' : 'Extraction/Extraction/Extr_HighSpd/Extr_HighSpd_Supervisor/Extr_HighSpd_Supervisor/UpdateCurrentPulseConfig/UpdateSpinPulseConfig'
 * '<S109>' : 'Extraction/Extraction/Extr_HighSpd/Extr_HighSpd_Supervisor/Extr_HighSpd_Supervisor/UpdateCurrentPulseConfig/UpdateSpinPulseConfig/UpdateSpinPulseConfig_Enabled'
 * '<S110>' : 'Extraction/Extraction/Extr_HighSpd/Extr_HighSpd_Supervisor/Extr_HighSpd_Supervisor/UpdateCurrentPulseConfig/UpdateSpinPulseConfig/UpdateSpinPulseConfig_Enabled/BaselineFrictionClipped'
 * '<S111>' : 'Extraction/Extraction/Extr_HighSpd/Extr_HighSpd_Supervisor/Extr_HighSpd_Supervisor/UpdateCurrentPulseConfig/UpdateSpinPulseConfig/UpdateSpinPulseConfig_Enabled/GetBaselineConfig'
 * '<S112>' : 'Extraction/Extraction/Extr_HighSpd/Extr_HighSpd_Supervisor/Extr_HighSpd_Supervisor/UpdateCurrentPulseConfig/UpdateSpinPulseConfig/UpdateSpinPulseConfig_Enabled/GetLoadMassLimitConfig'
 * '<S113>' : 'Extraction/Extraction/Extr_HighSpd/Extr_HighSpd_Supervisor/Extr_HighSpd_Supervisor/UpdateCurrentPulseConfig/UpdateSpinPulseConfig/UpdateSpinPulseConfig_Enabled/GetPEAppSpecificConfig'
 * '<S114>' : 'Extraction/Extraction/Extr_HighSpd/Extr_HighSpd_Supervisor/Extr_HighSpd_Supervisor/UpdateCurrentPulseConfig/UpdateSpinPulseConfig/UpdateSpinPulseConfig_Enabled/GetPulseConfig'
 * '<S115>' : 'Extraction/Extraction/Extr_HighSpd/Extr_HighSpd_Supervisor/Extr_HighSpd_Supervisor/UpdateCurrentPulseConfig/UpdateSpinPulseConfig/UpdateSpinPulseConfig_Enabled/UpdateBaselineFriction'
 * '<S116>' : 'Extraction/Extraction/Extr_HighSpd/Extr_HighSpd_Supervisor/Extr_HighSpd_Supervisor/UpdatePlateauTime/MATLAB Function'
 * '<S117>' : 'Extraction/Extraction/Extr_HighSpd/Extr_HighSpd_Supervisor/Extr_HighSpd_Supervisor/VerifyVibMapFloorDisplacement/VerifyVibMapFloorDispSubsystem'
 * '<S118>' : 'Extraction/Extraction/Extr_HighSpd/Extr_HighSpd_Supervisor/Extr_HighSpd_Supervisor/VerifyVibMapFloorDisplacement/VerifyVibMapFloorDispSubsystem/VerifyVibMapFloorDispDisabled'
 * '<S119>' : 'Extraction/Extraction/Extr_HighSpd/Extr_HighSpd_Supervisor/Extr_HighSpd_Supervisor/VerifyVibMapFloorDisplacement/VerifyVibMapFloorDispSubsystem/VerifyVibMapFloorDispEnabled'
 * '<S120>' : 'Extraction/Extraction/Extr_HighSpd/Extr_HighSpd_Supervisor/Extr_HighSpd_Supervisor/VerifyVibMapFloorDisplacement/VerifyVibMapFloorDispSubsystem/VerifyVibMapFloorDispEnabled/VerifyVibMapFloorDisplacementMATLAB'
 * '<S121>' : 'Extraction/Extraction/Extr_LowSpd/Extr_LowSpd_Algorithms'
 * '<S122>' : 'Extraction/Extraction/Extr_LowSpd/Extr_LowSpd_Supervisor'
 * '<S123>' : 'Extraction/Extraction/Extr_LowSpd/Extr_LowSpd_Algorithms/CalcORAToBBAngle'
 * '<S124>' : 'Extraction/Extraction/Extr_LowSpd/Extr_LowSpd_Algorithms/Extr_LowSpd_BasicUnbChk'
 * '<S125>' : 'Extraction/Extraction/Extr_LowSpd/Extr_LowSpd_Algorithms/Extr_LowSpd_LoadMeas'
 * '<S126>' : 'Extraction/Extraction/Extr_LowSpd/Extr_LowSpd_Algorithms/Extr_LowSpd_MDDP'
 * '<S127>' : 'Extraction/Extraction/Extr_LowSpd/Extr_LowSpd_Algorithms/Extr_LowSpd_OptRamp'
 * '<S128>' : 'Extraction/Extraction/Extr_LowSpd/Extr_LowSpd_Algorithms/FinalSpeedToUnb'
 * '<S129>' : 'Extraction/Extraction/Extr_LowSpd/Extr_LowSpd_Algorithms/LoadMassToUpperOffset'
 * '<S130>' : 'Extraction/Extraction/Extr_LowSpd/Extr_LowSpd_Algorithms/TriggerGen'
 * '<S131>' : 'Extraction/Extraction/Extr_LowSpd/Extr_LowSpd_Algorithms/CalcORAToBBAngle/ConstrainAngle'
 * '<S132>' : 'Extraction/Extraction/Extr_LowSpd/Extr_LowSpd_Algorithms/CalcORAToBBAngle/TheoreticalORAToBB'
 * '<S133>' : 'Extraction/Extraction/Extr_LowSpd/Extr_LowSpd_Algorithms/CalcORAToBBAngle/ConstrainAngle/Compare To Zero'
 * '<S134>' : 'Extraction/Extraction/Extr_LowSpd/Extr_LowSpd_Algorithms/CalcORAToBBAngle/ConstrainAngle/If Action Subsystem1'
 * '<S135>' : 'Extraction/Extraction/Extr_LowSpd/Extr_LowSpd_Algorithms/CalcORAToBBAngle/ConstrainAngle/If Action Subsystem2'
 * '<S136>' : 'Extraction/Extraction/Extr_LowSpd/Extr_LowSpd_Algorithms/CalcORAToBBAngle/TheoreticalORAToBB/ConstrainAngle'
 * '<S137>' : 'Extraction/Extraction/Extr_LowSpd/Extr_LowSpd_Algorithms/CalcORAToBBAngle/TheoreticalORAToBB/ConstrainAngle/Compare To Zero'
 * '<S138>' : 'Extraction/Extraction/Extr_LowSpd/Extr_LowSpd_Algorithms/CalcORAToBBAngle/TheoreticalORAToBB/ConstrainAngle/If Action Subsystem1'
 * '<S139>' : 'Extraction/Extraction/Extr_LowSpd/Extr_LowSpd_Algorithms/CalcORAToBBAngle/TheoreticalORAToBB/ConstrainAngle/If Action Subsystem2'
 * '<S140>' : 'Extraction/Extraction/Extr_LowSpd/Extr_LowSpd_Algorithms/Extr_LowSpd_BasicUnbChk/CalculateUnbalanceTargetArray'
 * '<S141>' : 'Extraction/Extraction/Extr_LowSpd/Extr_LowSpd_Algorithms/Extr_LowSpd_BasicUnbChk/Extr_LowSpd_BasicUnbCheck'
 * '<S142>' : 'Extraction/Extraction/Extr_LowSpd/Extr_LowSpd_Algorithms/Extr_LowSpd_BasicUnbChk/CalculateUnbalanceTargetArray/CalculateUnbalanceTarget'
 * '<S143>' : 'Extraction/Extraction/Extr_LowSpd/Extr_LowSpd_Algorithms/Extr_LowSpd_BasicUnbChk/CalculateUnbalanceTargetArray/MATLAB Function'
 * '<S144>' : 'Extraction/Extraction/Extr_LowSpd/Extr_LowSpd_Algorithms/Extr_LowSpd_BasicUnbChk/CalculateUnbalanceTargetArray/CalculateUnbalanceTarget/CalculateUnbalanceTargetSubsystem'
 * '<S145>' : 'Extraction/Extraction/Extr_LowSpd/Extr_LowSpd_Algorithms/Extr_LowSpd_LoadMeas/Extr_LowSpd_LoadMeasLogic'
 * '<S146>' : 'Extraction/Extraction/Extr_LowSpd/Extr_LowSpd_Algorithms/Extr_LowSpd_LoadMeas/Extr_LowSpd_LoadMeasLogic/ConvertBalIndexToGrams'
 * '<S147>' : 'Extraction/Extraction/Extr_LowSpd/Extr_LowSpd_Algorithms/Extr_LowSpd_MDDP/MDDPLogic'
 * '<S148>' : 'Extraction/Extraction/Extr_LowSpd/Extr_LowSpd_Algorithms/Extr_LowSpd_MDDP/MDDPLogic/PlateauSetup'
 * '<S149>' : 'Extraction/Extraction/Extr_LowSpd/Extr_LowSpd_Algorithms/Extr_LowSpd_MDDP/MDDPLogic/PlateauSetup/CalcMDDPParameters'
 * '<S150>' : 'Extraction/Extraction/Extr_LowSpd/Extr_LowSpd_Algorithms/Extr_LowSpd_OptRamp/Extr_LowSpd_OptRamp_Enabled'
 * '<S151>' : 'Extraction/Extraction/Extr_LowSpd/Extr_LowSpd_Algorithms/Extr_LowSpd_OptRamp/Extr_LowSpd_OptRamp_Enabled/OptRampLogic'
 * '<S152>' : 'Extraction/Extraction/Extr_LowSpd/Extr_LowSpd_Algorithms/Extr_LowSpd_OptRamp/Extr_LowSpd_OptRamp_Enabled/OptRampLogic/CalculateOptRampTargets'
 * '<S153>' : 'Extraction/Extraction/Extr_LowSpd/Extr_LowSpd_Algorithms/Extr_LowSpd_OptRamp/Extr_LowSpd_OptRamp_Enabled/OptRampLogic/CalculateUBThresholds'
 * '<S154>' : 'Extraction/Extraction/Extr_LowSpd/Extr_LowSpd_Algorithms/Extr_LowSpd_OptRamp/Extr_LowSpd_OptRamp_Enabled/OptRampLogic/CalculateOptRampTargets/Degrees to Radians'
 * '<S155>' : 'Extraction/Extraction/Extr_LowSpd/Extr_LowSpd_Algorithms/Extr_LowSpd_OptRamp/Extr_LowSpd_OptRamp_Enabled/OptRampLogic/CalculateOptRampTargets/Degrees to Radians2'
 * '<S156>' : 'Extraction/Extraction/Extr_LowSpd/Extr_LowSpd_Algorithms/Extr_LowSpd_OptRamp/Extr_LowSpd_OptRamp_Enabled/OptRampLogic/CalculateUBThresholds/CalcOffsetsAndSlopes'
 * '<S157>' : 'Extraction/Extraction/Extr_LowSpd/Extr_LowSpd_Algorithms/Extr_LowSpd_OptRamp/Extr_LowSpd_OptRamp_Enabled/OptRampLogic/CalculateUBThresholds/InvertPrelookup'
 * '<S158>' : 'Extraction/Extraction/Extr_LowSpd/Extr_LowSpd_Algorithms/Extr_LowSpd_OptRamp/Extr_LowSpd_OptRamp_Enabled/OptRampLogic/CalculateUBThresholds/LoadMassToLowerOffset'
 * '<S159>' : 'Extraction/Extraction/Extr_LowSpd/Extr_LowSpd_Algorithms/Extr_LowSpd_OptRamp/Extr_LowSpd_OptRamp_Enabled/OptRampLogic/CalculateUBThresholds/RebalCountToUnbBlock'
 * '<S160>' : 'Extraction/Extraction/Extr_LowSpd/Extr_LowSpd_Algorithms/Extr_LowSpd_OptRamp/Extr_LowSpd_OptRamp_Enabled/OptRampLogic/CalculateUBThresholds/LoadMassToLowerOffset/If Action Subsystem'
 * '<S161>' : 'Extraction/Extraction/Extr_LowSpd/Extr_LowSpd_Algorithms/Extr_LowSpd_OptRamp/Extr_LowSpd_OptRamp_Enabled/OptRampLogic/CalculateUBThresholds/LoadMassToLowerOffset/If Action Subsystem1'
 * '<S162>' : 'Extraction/Extraction/Extr_LowSpd/Extr_LowSpd_Algorithms/Extr_LowSpd_OptRamp/Extr_LowSpd_OptRamp_Enabled/OptRampLogic/CalculateUBThresholds/RebalCountToUnbBlock/UnbCalcAfterMddp'
 * '<S163>' : 'Extraction/Extraction/Extr_LowSpd/Extr_LowSpd_Algorithms/Extr_LowSpd_OptRamp/Extr_LowSpd_OptRamp_Enabled/OptRampLogic/CalculateUBThresholds/RebalCountToUnbBlock/UnbCalcBeforeMddp'
 * '<S164>' : 'Extraction/Extraction/Extr_LowSpd/Extr_LowSpd_Algorithms/FinalSpeedToUnb/FinalSpeedToUnbSubsystem'
 * '<S165>' : 'Extraction/Extraction/Extr_LowSpd/Extr_LowSpd_Algorithms/FinalSpeedToUnb/FinalSpeedToUnbSubsystem/UnbCalcAfterMddp'
 * '<S166>' : 'Extraction/Extraction/Extr_LowSpd/Extr_LowSpd_Algorithms/FinalSpeedToUnb/FinalSpeedToUnbSubsystem/UnbCalcBeforeMddp'
 * '<S167>' : 'Extraction/Extraction/Extr_LowSpd/Extr_LowSpd_Algorithms/LoadMassToUpperOffset/LoadMassToUpperOffsetBlock'
 * '<S168>' : 'Extraction/Extraction/Extr_LowSpd/Extr_LowSpd_Algorithms/LoadMassToUpperOffset/LoadMassToUpperOffsetBlock/If Action Subsystem'
 * '<S169>' : 'Extraction/Extraction/Extr_LowSpd/Extr_LowSpd_Algorithms/LoadMassToUpperOffset/LoadMassToUpperOffsetBlock/If Action Subsystem1'
 * '<S170>' : 'Extraction/Extraction/Extr_LowSpd/Extr_LowSpd_Algorithms/TriggerGen/TriggerGen'
 * '<S171>' : 'Extraction/Extraction/Extr_LowSpd/Extr_LowSpd_Supervisor/Extr_LowSpd_SupervisorLogic'
 * '<S172>' : 'Extraction/Extraction/Extr_LowSpd/Extr_LowSpd_Supervisor/LoadDistributionSpeed'
 * '<S173>' : 'Extraction/Extraction/Extr_LowSpd/Extr_LowSpd_Supervisor/RetrieveMaxTargetSpeed'
 * '<S174>' : 'Extraction/Extraction/Extr_LowSpd/Extr_LowSpd_Supervisor/TriggerGen'
 * '<S175>' : 'Extraction/Extraction/Extr_LowSpd/Extr_LowSpd_Supervisor/UpdatePEAppSpecificSetup'
 * '<S176>' : 'Extraction/Extraction/Extr_LowSpd/Extr_LowSpd_Supervisor/LoadDistributionSpeed/LoadDistributionSpeedMATLAB'
 * '<S177>' : 'Extraction/Extraction/Extr_LowSpd/Extr_LowSpd_Supervisor/RetrieveMaxTargetSpeed/RetrieveMaxTargetSpeedMATLAB'
 * '<S178>' : 'Extraction/Extraction/Extr_LowSpd/Extr_LowSpd_Supervisor/TriggerGen/TriggerGen'
 * '<S179>' : 'Extraction/Extraction/Extr_LowSpd/Extr_LowSpd_Supervisor/UpdatePEAppSpecificSetup/GetPEAppSpecificConfig'
 * '<S180>' : 'Extraction/Extraction/Extr_Rebalance/Extr_Rebal_Logic'
 * '<S181>' : 'Extraction/Extraction/Extr_Rebalance/FindInitialDistributionPulse'
 * '<S182>' : 'Extraction/Extraction/Extr_Rebalance/LoadRebalanceProfile'
 * '<S183>' : 'Extraction/Extraction/Extr_Rebalance/LoadRebalanceProfileIndex'
 * '<S184>' : 'Extraction/Extraction/Extr_Rebalance/TriggerGen'
 * '<S185>' : 'Extraction/Extraction/Extr_Rebalance/Extr_Rebal_Logic/DistUtilsWrapperControl'
 * '<S186>' : 'Extraction/Extraction/Extr_Rebalance/Extr_Rebal_Logic/REBALANCE.RUNNING.DISTRIBUTION_RAMP.CalculateRampRate'
 * '<S187>' : 'Extraction/Extraction/Extr_Rebalance/Extr_Rebal_Logic/REBALANCE.RUNNING.DISTRIBUTION_RAMP.P2PTorqueThresholdCalc'
 * '<S188>' : 'Extraction/Extraction/Extr_Rebalance/FindInitialDistributionPulse/MATLAB Function'
 * '<S189>' : 'Extraction/Extraction/Extr_Rebalance/LoadRebalanceProfile/MATLAB Function'
 * '<S190>' : 'Extraction/Extraction/Extr_Rebalance/LoadRebalanceProfileIndex/MATLAB Function'
 * '<S191>' : 'Extraction/Extraction/Extr_Rebalance/TriggerGen/TriggerGen'
 * '<S192>' : 'Extraction/Extraction/Extr_Supervisor/EndCommand'
 * '<S193>' : 'Extraction/Extraction/Extr_Supervisor/Extr_Supervisor'
 * '<S194>' : 'Extraction/Extraction/Extr_Supervisor/NotifyFoamDetection'
 * '<S195>' : 'Extraction/Extraction/Extr_Supervisor/ResetLoadMass'
 * '<S196>' : 'Extraction/Extraction/Extr_Supervisor/ResumeCommand'
 * '<S197>' : 'Extraction/Extraction/Extr_Supervisor/StartCommand'
 * '<S198>' : 'Extraction/Extraction/Extr_Supervisor/StopCommand'
 * '<S199>' : 'Extraction/Extraction/Extr_Supervisor/EndCommand/EventGen'
 * '<S200>' : 'Extraction/Extraction/Extr_Supervisor/Extr_Supervisor/Extr_Sup_LoadMassMerge'
 * '<S201>' : 'Extraction/Extraction/Extr_Supervisor/Extr_Supervisor/Extr_Sup_Logic'
 * '<S202>' : 'Extraction/Extraction/Extr_Supervisor/Extr_Supervisor/Extr_Sup_MotorSpeedCheck'
 * '<S203>' : 'Extraction/Extraction/Extr_Supervisor/Extr_Supervisor/ResetInitialPulseConfig'
 * '<S204>' : 'Extraction/Extraction/Extr_Supervisor/Extr_Supervisor/TriggerGen'
 * '<S205>' : 'Extraction/Extraction/Extr_Supervisor/Extr_Supervisor/ResetInitialPulseConfig/ResetInitialPulseConfigStates'
 * '<S206>' : 'Extraction/Extraction/Extr_Supervisor/Extr_Supervisor/ResetInitialPulseConfig/ResetInitialPulseConfigStates/ResetInitialPulseConfig_HighSpeedParamEstEnabled'
 * '<S207>' : 'Extraction/Extraction/Extr_Supervisor/Extr_Supervisor/TriggerGen/TriggerGen'
 * '<S208>' : 'Extraction/Extraction/Extr_Supervisor/NotifyFoamDetection/EventGen'
 * '<S209>' : 'Extraction/Extraction/Extr_Supervisor/ResetLoadMass/EventGen'
 * '<S210>' : 'Extraction/Extraction/Extr_Supervisor/ResumeCommand/EventGen'
 * '<S211>' : 'Extraction/Extraction/Extr_Supervisor/StartCommand/EventGen'
 * '<S212>' : 'Extraction/Extraction/Extr_Supervisor/StopCommand/EventGen'
 * '<S213>' : 'Extraction/Extraction/Extr_UpdateInputs/GetDistUtilsOutput'
 * '<S214>' : 'Extraction/Extraction/Extr_UpdateInputs/GetExtPlateauDuration'
 * '<S215>' : 'Extraction/Extraction/Extr_UpdateInputs/GetFinalSpeed'
 * '<S216>' : 'Extraction/Extraction/Extr_UpdateInputs/GetMCIOutputs'
 * '<S217>' : 'Extraction/Extraction/Extr_UpdateInputs/GetMaxSpeedError'
 * '<S218>' : 'Extraction/Extraction/Extr_UpdateInputs/GetMciRawUnbalance'
 * '<S219>' : 'Extraction/Extraction/Extr_UpdateInputs/GetMciRawUnbalanceCount'
 * '<S220>' : 'Extraction/Extraction/Extr_UpdateInputs/GetMinSpeed'
 * '<S221>' : 'Extraction/Extraction/Extr_UpdateInputs/GetPEOutput'
 * '<S222>' : 'Extraction/Extraction/Extr_UpdateInputs/GetRampTimeout'
 * '<S223>' : 'Extraction/Extraction/Extr_UpdateInputs/GetSaturatedLoadMass'
 * '<S224>' : 'Extraction/Extraction/Extr_UpdateInputs/GetWaterTemp'
 * '<S225>' : 'Extraction/Extraction/Extr_UpdateInputs/GetMCIOutputs/GetCosTheta'
 * '<S226>' : 'Extraction/Extraction/Extr_UpdateInputs/GetMCIOutputs/GetSinTheta'
 * '<S227>' : 'Extraction/Extraction/Extr_UpdateInputs/GetMCIOutputs/GetSpeed'
 * '<S228>' : 'Extraction/Extraction/Extr_UpdateInputs/GetMCIOutputs/GetSpeedRef'
 * '<S229>' : 'Extraction/Extraction/Extr_UpdateInputs/GetMCIOutputs/GetTorque'
 * '<S230>' : 'Extraction/Extraction/Extr_UpdateInputs/GetPEOutput/GetBBMag'
 * '<S231>' : 'Extraction/Extraction/Extr_UpdateInputs/GetPEOutput/GetBBPhaseDiff'
 * '<S232>' : 'Extraction/Extraction/Extr_UpdateInputs/GetPEOutput/GetCoulombFriction'
 * '<S233>' : 'Extraction/Extraction/Extr_UpdateInputs/GetPEOutput/GetFriction'
 * '<S234>' : 'Extraction/Extraction/Extr_UpdateInputs/GetPEOutput/GetInertia'
 * '<S235>' : 'Extraction/Extraction/Extr_UpdateInputs/GetPEOutput/GetUB'
 * '<S236>' : 'Extraction/Extraction/Extr_UpdateInputs/GetPEOutput/GetUBAngle'
 * '<S237>' : 'Extraction/Extraction/Extr_UpdateInputs/GetPEOutput/GetViscousFriction'
 * '<S238>' : 'Extraction/Extraction/GetMciFault/ReleaseMotionMacro'
 * '<S239>' : 'Extraction/Extraction/IsMotorRunning/Compare To Zero'
 * '<S240>' : 'Extraction/Extraction/IsMotorRunning/IsMotorRunningMacro'
 * '<S241>' : 'Extraction/Extraction/ParamEstControl/ParamEst'
 * '<S242>' : 'Extraction/Extraction/ParamEstExcControl/ParamEstExc'
 * '<S243>' : 'Extraction/Extraction/ReleaseMotion/ReleaseMotionMacro'
 * '<S244>' : 'Extraction/Extraction/SelectParamEstOutputs/Enumerated Constant'
 * '<S245>' : 'Extraction/Extraction/SelectParamEstOutputs/ParamEst'
 * '<S246>' : 'Extraction/Extraction/SetSpeedCommand/SpdComCntIncrement'
 * '<S247>' : 'Extraction/Extraction/SetSpeedCommand/SpinCommand'
 * '<S248>' : 'Extraction/Extraction/SetSpeedCommand/StopMotorCommand'
 * '<S249>' : 'Extraction/Extraction/SetSpeedCommand/StopMotorCommand/SpinCommand'
 * '<S250>' : 'Extraction/Extraction/UpdateParamEstSetup/GetParamEstExcSetup'
 * '<S251>' : 'Extraction/Extraction/UpdateParamEstSetup/GetParamEstSetup'
 * '<S252>' : 'Extraction/Extraction/UpdateParamEstSetup/GetParamEstSetup_'
 * '<S253>' : 'Extraction/Extraction/UpdateParamEstSetup/SetParamEstSetup'
 * '<S254>' : 'Extraction/Extraction/UpdateParamEstSetup/GetParamEstSetup/CodeGen'
 * '<S255>' : 'Extraction/Extraction/UpdateParamEstSetup/GetParamEstSetup/Simulation'
 * '<S256>' : 'Extraction/Extraction/UpdateParamEstSetup/GetParamEstSetup/CodeGen/GetParamEstSetup'
 * '<S257>' : 'Extraction/Extraction/UpdateParamEstSetup/SetParamEstSetup/If Action Subsystem'
 * '<S258>' : 'Extraction/Extraction/UpdateParamEstSetup/SetParamEstSetup/If Action Subsystem/SetPeSetup'
 * '<S259>' : 'Extraction/Extraction/UpdateParamEstSetup/SetParamEstSetup/If Action Subsystem/StopPe'
 * '<S260>' : 'Extraction/Extraction/UpdateParamEstSetup/SetParamEstSetup/If Action Subsystem/SetPeSetup/SetParamEstExcSetup'
 * '<S261>' : 'Extraction/Extraction/UpdateParamEstSetup/SetParamEstSetup/If Action Subsystem/SetPeSetup/SetParamEstSetup'
 * '<S262>' : 'Extraction/Extraction/UpdateParamEstSetup/SetParamEstSetup/If Action Subsystem/SetPeSetup/SetParamEstExcSetup/Enumerated Constant'
 * '<S263>' : 'Extraction/Extraction/UpdateParamEstSetup/SetParamEstSetup/If Action Subsystem/SetPeSetup/SetParamEstSetup/Enumerated Constant'
 * '<S264>' : 'Extraction/Extraction/UpdateParamEstSetup/SetParamEstSetup/If Action Subsystem/SetPeSetup/SetParamEstSetup/ParamEst'
 * '<S265>' : 'Extraction/GetFloat/If Action Subsystem'
 * '<S266>' : 'Extraction/GetFloat/If Action Subsystem1'
 * '<S267>' : 'Extraction/GetFloat/If Action Subsystem2'
 * '<S268>' : 'Extraction/GetFloat/If Action Subsystem3'
 * '<S269>' : 'Extraction/GetFloat/If Action Subsystem4'
 * '<S270>' : 'Extraction/GetFloat/If Action Subsystem5'
 * '<S271>' : 'Extraction/GetFloat/If Action Subsystem6'
 * '<S272>' : 'Extraction/GetFloat/If Action Subsystem7'
 * '<S273>' : 'Extraction/GetFloat/If Action Subsystem8'
 * '<S274>' : 'Extraction/GetInteger/If Action Subsystem'
 * '<S275>' : 'Extraction/GetInteger/If Action Subsystem1'
 * '<S276>' : 'Extraction/GetInteger/If Action Subsystem2'
 * '<S277>' : 'Extraction/GetInteger/If Action Subsystem3'
 * '<S278>' : 'Extraction/GetInteger/If Action Subsystem4'
 * '<S279>' : 'Extraction/GetInteger/If Action Subsystem5'
 * '<S280>' : 'Extraction/GetInteger/If Action Subsystem6'
 * '<S281>' : 'Extraction/GetInteger/If Action Subsystem7'
 * '<S282>' : 'Extraction/Initialize/ExtractionInitialize'
 */

/*-
 * Requirements for '<Root>': Extraction
 */
#endif                                 /* RTW_HEADER_Extraction_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
