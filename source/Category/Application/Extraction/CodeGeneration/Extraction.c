/*
 * File: Extraction.c
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

#include "Extraction.h"
#include "intrp1d_s16ffu8fl.h"
#include "intrp1d_u16ffu8fl.h"
#include "intrp1d_u16ffu8fla.h"
#include "intrp1d_u8ffu8fl.h"
#include "intrp2d_u16ffu32fl.h"
#include "intrp2d_u16ffu8fla.h"
#include "look1_iu16lfyu16DfIf_binlcas.h"
#include "plook_u32u16u32n31_binc_f.h"
#include "plook_u8fu16f_binc.h"
#include "plook_u8u16f_binc.h"
#include "plook_u8u8f_binc.h"
#include "plook_u8u8f_binca.h"
#include "rt_modf.h"

/*
 * Check that imported macros with storage class "ImportedDefine" are defined
 */
#ifndef EXTR_MAX_SPEED
#error The variable for the parameter "EXTR_MAX_SPEED" is not defined
#endif

#ifndef EXTR_MIN_SPEED
#error The variable for the parameter "EXTR_MIN_SPEED" is not defined
#endif

#ifndef MAX_RAMP_ACCEL
#error The variable for the parameter "MAX_RAMP_ACCEL" is not defined
#endif

#ifndef FS_EXTRACTION
#error The variable for the parameter "FS_EXTRACTION" is not defined
#endif

/* Named constants for Chart: '<S150>/OptRampLogic' */
#define Extraction_IN_IDLE             ((uint8)1U)
#define Extraction_IN_LIMITS_CONVERGING ((uint8)1U)
#define Extraction_IN_NO_ACTIVE_CHILD_pr ((uint8)0U)
#define Extraction_IN_OPT_RAMP         ((uint8)2U)
#define Extraction_IN_PLATEAU          ((uint8)2U)
#define Extraction_IN_RAMP_AT_OPTIMAL_TIME ((uint8)1U)
#define Extraction_IN_UB_LARGE         ((uint8)2U)
#define Extraction_IN_UB_SMALL         ((uint8)3U)
#define Extraction_IN_WAITING_OPT_RAMP ((uint8)4U)
#define Extraction_event_evStart       (2)
#define Extraction_event_evStop        (3)
#define Extraction_event_localStop     (0)
#define Extraction_event_ticks         (1)

/* Named constants for Chart: '<S92>/Extr_HighSpd_SupervisorLogic' */
#define Extraction_IN_CONFIGURING_PULSE ((uint8)1U)
#define Extraction_IN_DIST_SPEED       ((uint8)2U)
#define Extraction_IN_EXTENDED_PLATEAU_DELAY ((uint8)3U)
#define Extraction_IN_FAULT_INVALID_SETUP ((uint8)1U)
#define Extraction_IN_HIGH_SPEED_COMPLETE ((uint8)4U)
#define Extraction_IN_HIGH_SPEED_SUPERVISOR ((uint8)1U)
#define Extraction_IN_IDLE_bo          ((uint8)2U)
#define Extraction_IN_NO_ACTIVE_CHILD_cn ((uint8)0U)
#define Extraction_IN_PLATEAU_ac       ((uint8)5U)
#define Extraction_IN_RAMP             ((uint8)6U)
#define Extraction_IN_REBALANCE        ((uint8)3U)
#define Extraction_IN_RUNNING          ((uint8)4U)
#define Extraction_event_Extr_Accel_EvCabHit (11)
#define Extraction_event_Extr_Accel_VibMapEvExcessiveFloorDisp (9)
#define Extraction_event_Extr_HighSpd_EvCEMFailureDetected (8)
#define Extraction_event_Extr_HighSpd_EvExtndChkFailed (13)
#define Extraction_event_Extr_HighSpd_EvFrictionLimitExceeded1 (19)
#define Extraction_event_Extr_HighSpd_EvLoadMassExceeded (10)
#define Extraction_event_Extr_HighSpd_EvPlateauComplete (6)
#define Extraction_event_Extr_HighSpd_EvRampComplete (5)
#define Extraction_event_Extr_HighSpd_EvViscousLimitExceeded (7)
#define Extraction_event_Extr_LowSpd_EvComplete (12)
#define Extraction_event_Extr_Sup_ClearFault (15)
#define Extraction_event_Extr_Sup_EvFoamDetected (4)
#define Extraction_event_Extr_Sup_EvHighSpdStart (2)
#define Extraction_event_Extr_Sup_EvRebalStart (16)
#define Extraction_event_Extr_Sup_EvSpeedErrorTimeout (17)
#define Extraction_event_Extr_Sup_EvSpeedReached (18)
#define Extraction_event_Extr_Sup_EvStopAll (3)
#define Extraction_event_Extr_Sup_ResetRebalCnt (14)
#define Extraction_event_Local_EvStop  (0)
#define Extraction_event_Trigger       (1)

/* Named constants for Chart: '<S93>/Extr_HighSpd_SupervisorLogic' */
#define Extraction_IN_CONFIGURING_PULSE_l3 ((uint8)1U)
#define Extraction_IN_DIST_SPEED_le    ((uint8)2U)
#define Extraction_IN_EXTENDED_PLATEAU ((uint8)3U)
#define Extraction_IN_EXTENDED_PLATEAU_DELAY_ps ((uint8)1U)
#define Extraction_IN_EXTENDED_PLATEAU_RAMPING ((uint8)2U)
#define Extraction_IN_HIGH_SPEED_COMPLETE_fn ((uint8)4U)
#define Extraction_IN_HIGH_SPEED_SUPERVISOR_ig ((uint8)1U)
#define Extraction_IN_IDLE_ow          ((uint8)1U)
#define Extraction_IN_NO_ACTIVE_CHILD_fr ((uint8)0U)
#define Extraction_IN_PLATEAU_COMPLETE ((uint8)6U)
#define Extraction_IN_PLATEAU_mt       ((uint8)5U)
#define Extraction_IN_RAMP_bl          ((uint8)7U)
#define Extraction_IN_REBALANCE_nu     ((uint8)2U)
#define Extraction_IN_RUNNING_ny       ((uint8)3U)
#define Extraction_event_EvStartRebal  (0)
#define Extraction_event_Extr_Accel_EvCabHit_ns (12)
#define Extraction_event_Extr_Accel_VibMapEvExcessiveFloorDisp_ed (10)
#define Extraction_event_Extr_HighSpd_EvCEMFailureDetected_gz (9)
#define Extraction_event_Extr_HighSpd_EvExtndChkFailed_fo (14)
#define Extraction_event_Extr_HighSpd_EvFrictionLimitExceeded (20)
#define Extraction_event_Extr_HighSpd_EvLoadMassExceeded_ha (11)
#define Extraction_event_Extr_HighSpd_EvPlateauComplete_mw (7)
#define Extraction_event_Extr_HighSpd_EvRampComplete_b3 (6)
#define Extraction_event_Extr_HighSpd_EvViscousLimitExceeded_gz (8)
#define Extraction_event_Extr_LowSpd_EvComplete_kv (13)
#define Extraction_event_Extr_Sup_ClearFault_kr (16)
#define Extraction_event_Extr_Sup_EvFoamDetected_md (5)
#define Extraction_event_Extr_Sup_EvHighSpdStart_px (3)
#define Extraction_event_Extr_Sup_EvRebalStart_ba (17)
#define Extraction_event_Extr_Sup_EvSpeedErrorTimeout_o2 (18)
#define Extraction_event_Extr_Sup_EvSpeedReached_a1 (19)
#define Extraction_event_Extr_Sup_EvStopAll_dp (4)
#define Extraction_event_Extr_Sup_ResetRebalCnt_fl (15)
#define Extraction_event_Local_EvStop_bj (1)
#define Extraction_event_Trigger_o5    (2)

/* Named constants for Chart: '<S69>/Extr_HighSpd_CEMLogic' */
#define Extraction_IN_EVALUATING_FRICTION ((uint8)1U)
#define Extraction_IN_FILLING_FRICTION_BUFFER ((uint8)1U)
#define Extraction_IN_IDLE_kt          ((uint8)1U)
#define Extraction_IN_MONITORING_AVG_FRICTION ((uint8)2U)
#define Extraction_IN_NO_ACTIVE_CHILD_iq ((uint8)0U)
#define Extraction_IN_RUNNING_pc       ((uint8)2U)
#define Extraction_event_EvStop        (0)
#define Extraction_event_EvWindowComplete (1)
#define Extraction_event_Extr_HighSpd_EvStopAll (3)
#define Extraction_event_ticks_nt      (2)

/* Named constants for Chart: '<S72>/Extr_HighSpd_LoadMassLimitCheckLogic' */
#define Extraction_IN_IDLE_kp          ((uint8)1U)
#define Extraction_IN_RUNNING_fk       ((uint8)2U)
#define Extraction_event_Extr_HighSpd_EvStopAll_e4 (1)
#define Extraction_event_Trigger_cm    (0)

/* Named constants for Chart: '<S74>/Extr_HighSpd_PlateauLogic' */
#define Extraction_IN_IDLE_jj          ((uint8)1U)
#define Extraction_IN_NO_ACTIVE_CHILD_mr ((uint8)0U)
#define Extraction_IN_WAITING_PLATEAU_TIME ((uint8)2U)
#define Extraction_event_EvStop_bp     (0)
#define Extraction_event_Extr_Accel_VibMapEvExcessiveFloorDisp_h3 (4)
#define Extraction_event_Extr_HighSpd_EvLoadMassExceeded_in (5)
#define Extraction_event_Extr_HighSpd_EvStartPlateau (2)
#define Extraction_event_Extr_HighSpd_EvStopAll_d3 (3)
#define Extraction_event_ticks_bn      (1)

/* Named constants for Chart: '<S75>/Extr_HighSpd_PlateauLogic' */
#define Extraction_IN_BASIC_PLATEAU    ((uint8)1U)
#define Extraction_IN_CALCULATING_FRICTION_BASELINE ((uint8)1U)
#define Extraction_IN_CALIBRATION_END  ((uint8)2U)
#define Extraction_IN_EVALUATING_FRICTION_df ((uint8)1U)
#define Extraction_IN_FILLING_FRICTION_BUFFER_io ((uint8)3U)
#define Extraction_IN_FRICTION_BASED_PLATEAU ((uint8)2U)
#define Extraction_IN_IDLE_fw          ((uint8)3U)
#define Extraction_IN_MONITORING_FRICTION ((uint8)4U)
#define Extraction_IN_MONITORING_PLATEAU_SETTLING ((uint8)5U)
#define Extraction_IN_MONITORING_PLATEAU_TIME ((uint8)1U)
#define Extraction_IN_NO_ACTIVE_CHILD_jm ((uint8)0U)
#define Extraction_IN_WAITING_PE_STABILIZATION_TIME ((uint8)6U)
#define Extraction_IN_WAITING_PE_STABILIZATION_TIME_mr ((uint8)2U)
#define Extraction_IN_WAITING_PLATEAU_TIME_fx ((uint8)7U)
#define Extraction_event_EvStop_p4     (0)
#define Extraction_event_EvWindowComplete_ld (1)
#define Extraction_event_Extr_Accel_VibMapEvExcessiveFloorDisp_af (5)
#define Extraction_event_Extr_HighSpd_EvLoadMassExceeded_js (6)
#define Extraction_event_Extr_HighSpd_EvStartPlateau_i3 (3)
#define Extraction_event_Extr_HighSpd_EvStopAll_bu (4)
#define Extraction_event_ticks_pn      (2)

/* Named constants for Chart: '<S79>/Extr_HighSpd_RampLogic' */
#define Extraction_IN_IDLE_fb          ((uint8)1U)
#define Extraction_IN_RAMPING          ((uint8)2U)
#define Extraction_event_Extr_HighSpd_EvLoadMassExceeded_oi (3)
#define Extraction_event_Extr_HighSpd_EvStartRamp (1)
#define Extraction_event_Extr_HighSpd_EvStopAll_du (2)
#define Extraction_event_Extr_Sup_EvSpeedErrorTimeout_p2 (4)
#define Extraction_event_Extr_Sup_EvSpeedReached_c4 (5)
#define Extraction_event_ticks_gd      (0)

/* Named constants for Chart: '<S80>/Extr_HighSpd_RampLogic' */
#define Extraction_IN_IDLE_iz          ((uint8)1U)
#define Extraction_IN_MONITORING       ((uint8)1U)
#define Extraction_IN_NO_ACTIVE_CHILD_l5 ((uint8)0U)
#define Extraction_IN_RAMPING_it       ((uint8)2U)
#define Extraction_IN_WAITING_PE_COV_RESET_TIME ((uint8)2U)
#define Extraction_IN_WAITING_STABILIZATION_TIME ((uint8)3U)
#define Extraction_event_Extr_HighSpd_EvLoadMassExceeded_ef (3)
#define Extraction_event_Extr_HighSpd_EvStartRamp_oc (1)
#define Extraction_event_Extr_HighSpd_EvStopAll_gt (2)
#define Extraction_event_Extr_Sup_EvSpeedErrorTimeout_nh (4)
#define Extraction_event_Extr_Sup_EvSpeedReached_ix (5)
#define Extraction_event_ticks_eh      (0)

/* Named constants for Chart: '<S29>/Extr_Accel_CabHitLogic' */
#define Extraction_IN_FILTER_WARM_UP   ((uint8)1U)
#define Extraction_IN_IDLE_ii          ((uint8)1U)
#define Extraction_IN_MONITORING_SOFT_CAB_HIT ((uint8)1U)
#define Extraction_IN_MONITORING_SPEED ((uint8)3U)
#define Extraction_IN_MONITORING_co    ((uint8)2U)
#define Extraction_IN_NO_ACTIVE_CHILD_pf ((uint8)0U)
#define Extraction_IN_RUNNING_iz       ((uint8)2U)
#define Extraction_IN_SOFT_THRESHOLD_CROSSED ((uint8)2U)
#define Extraction_event_Extr_Sup_EvResetSoftCabHitCnt (1)
#define Extraction_event_Trigger_l4    (0)

/* Named constants for Chart: '<S30>/Extr_Accel_Supervisor' */
#define Extraction_IN_ACCELEROMETER_IDLE ((uint8)1U)
#define Extraction_IN_DISABLING_ACCELEROMETER ((uint8)2U)
#define Extraction_IN_MEASURING_ACCELERATION ((uint8)3U)
#define Extraction_IN_MEASURING_BASELINE ((uint8)4U)
#define Extraction_IN_SENDING_COMMAND  ((uint8)5U)
#define Extraction_IN_STARTING_ACCELEROMETER ((uint8)6U)
#define Extraction_IN_SUPERVISOR_DISABLED ((uint8)1U)
#define Extraction_IN_SUPERVISOR_ENABLED ((uint8)2U)
#define Extraction_IN_SUPERVISOR_IDLE  ((uint8)3U)

/* Named constants for Chart: '<S31>/Extr_Accel_VibMapLogic' */
#define Extraction_IN_FD_THRESHOLD_CROSSED ((uint8)1U)
#define Extraction_IN_FILTER_WARM_UP_ig ((uint8)2U)
#define Extraction_IN_MONITORING_ACCELERATION ((uint8)3U)
#define Extraction_IN_MONITORING_FLOOR_DISPLACEMENT ((uint8)4U)
#define Extraction_IN_MONITORING_SPEED_dr ((uint8)2U)
#define Extraction_IN_MONITORING_my    ((uint8)1U)

/* Named constants for Chart: '<S193>/Extr_Sup_Logic' */
#define Extraction_IN_CONFIGURING      ((uint8)1U)
#define Extraction_IN_EXECUTING_ANTIFOAM ((uint8)1U)
#define Extraction_IN_EXTRACTION_COMPLETE ((uint8)2U)
#define Extraction_IN_EXTRACTION_gg    ((uint8)2U)
#define Extraction_IN_HIGH_SPEED       ((uint8)1U)
#define Extraction_IN_IDLE_kf          ((uint8)3U)
#define Extraction_IN_LOW_SPEED        ((uint8)2U)
#define Extraction_IN_MOTOR_STOPPING   ((uint8)4U)
#define Extraction_IN_REBALANCE_hg     ((uint8)3U)
#define Extraction_IN_RUNNING_hm       ((uint8)5U)
#define Extraction_event_ACU_ExtrEnd   (4)
#define Extraction_event_ACU_ExtrResume (3)
#define Extraction_event_ACU_ExtrStart (1)
#define Extraction_event_ACU_ExtrStop  (2)
#define Extraction_event_ACU_NotifFoamDetec (6)
#define Extraction_event_ACU_ResetLoadMass (5)
#define Extraction_event_Extr_Accel_EvCabHit_ja (17)
#define Extraction_event_Extr_Accel_EvCleanWasherException (14)
#define Extraction_event_Extr_HighSpd_EvComplete (10)
#define Extraction_event_Extr_HighSpd_EvRebalStart (11)
#define Extraction_event_Extr_LowSpd_EvCleanWasherException (16)
#define Extraction_event_Extr_LowSpd_EvComplete_jg (9)
#define Extraction_event_Extr_LowSpd_EvRebalStart (7)
#define Extraction_event_Extr_Rebal_EvCleanWasherException (15)
#define Extraction_event_Extr_Rebal_EvComplete (8)
#define Extraction_event_Extr_Rebal_EvShortRedistExceeded (13)
#define Extraction_event_Extr_Rebal_P2PTorqueFault (12)
#define Extraction_event_ticks_ml      (0)

/* Named constants for Chart: '<S193>/Extr_Sup_MotorSpeedCheck' */
#define Extraction_IN_MONITORING_SPEED_COMMAND ((uint8)1U)
#define Extraction_IN_MOTOR_ON         ((uint8)2U)
#define Extraction_event_Extr_Sup_EvStopAll_g1 (1)
#define Extraction_event_Trigger_d3    (0)

/* Named constants for Chart: '<S122>/Extr_LowSpd_SupervisorLogic' */
#define Extraction_IN_BASIC_UNBALANCE_CHECK ((uint8)1U)
#define Extraction_IN_DISTRIBUTION     ((uint8)2U)
#define Extraction_IN_DROPPING_TO_DIST_SPEED ((uint8)1U)
#define Extraction_IN_EXECUTING_ANTIFOAM_ROUTINE ((uint8)1U)
#define Extraction_IN_IDLE_jy          ((uint8)2U)
#define Extraction_IN_LOAD_MEASUREMENT ((uint8)3U)
#define Extraction_IN_LOW_SPEED_COMPLETED ((uint8)4U)
#define Extraction_IN_LOW_SPEED_EXTRACTION ((uint8)1U)
#define Extraction_IN_MDDP             ((uint8)5U)
#define Extraction_IN_OPTIMAL_RAMP     ((uint8)6U)
#define Extraction_IN_PE_INIT          ((uint8)3U)
#define Extraction_IN_RUNNING_e4       ((uint8)4U)
#define Extraction_IN_SETTING_DIST_SPEED ((uint8)2U)
#define Extraction_IN_WAITING_DIST_SPEED_TIMEOUT ((uint8)3U)
#define Extraction_event_EvDistCompleted (0)
#define Extraction_event_EvStop_o5     (1)
#define Extraction_event_Ext_LowSpd_EvOptRampComplete (6)
#define Extraction_event_Extr_Accel_EvCabHit_kw (11)
#define Extraction_event_Extr_HighSpd_EvDropToDistSpeed (13)
#define Extraction_event_Extr_LowSpd_EvLoadMeasComplete (5)
#define Extraction_event_Extr_LowSpd_EvMddpComplete (8)
#define Extraction_event_Extr_LowSpd_EvOptRampRedist (7)
#define Extraction_event_Extr_LowSpd_EvUnbChkComplete (15)
#define Extraction_event_Extr_LowSpd_EvUnbChkRebalStart (16)
#define Extraction_event_Extr_Rebal_EvComplete_j3 (4)
#define Extraction_event_Extr_Rebal_EvShortRedistExceeded_bq (14)
#define Extraction_event_Extr_Sup_ClearFault_ba (18)
#define Extraction_event_Extr_Sup_EvFoamDetected_l0 (12)
#define Extraction_event_Extr_Sup_EvLowSpdStart (3)
#define Extraction_event_Extr_Sup_EvRebalStart_jz (19)
#define Extraction_event_Extr_Sup_EvResetLoadMass (10)
#define Extraction_event_Extr_Sup_EvSpeedErrorTimeout_cn (20)
#define Extraction_event_Extr_Sup_EvSpeedReached_pu (21)
#define Extraction_event_Extr_Sup_EvStopAll_ne (9)
#define Extraction_event_Extr_Sup_ResetRebalCnt_ay (17)
#define Extraction_event_Trigger_o1    (2)

/* Named constants for Chart: '<S124>/Extr_LowSpd_BasicUnbCheck' */
#define Extraction_IN_BASIC_UNB_CHK    ((uint8)1U)
#define Extraction_IN_COMPLETE_ST      ((uint8)1U)
#define Extraction_IN_START_REBAL_ST   ((uint8)2U)
#define Extraction_event_Extr_HighSpd_EvStopAll_ks (3)
#define Extraction_event_Extr_LowSpd_EvUnbChkComplete_local (0)
#define Extraction_event_Extr_LowSpd_StartBasicUnbChkEv (2)
#define Extraction_event_Trigger_db    (1)

/* Named constants for Chart: '<S125>/Extr_LowSpd_LoadMeasLogic' */
#define Extraction_IN_AVERAGING_MEASUREMENTS ((uint8)1U)
#define Extraction_IN_IDLE_gt          ((uint8)1U)
#define Extraction_IN_MEASUREMENT_COMPLETE ((uint8)2U)
#define Extraction_IN_PARAM_EST_SETUP  ((uint8)3U)
#define Extraction_IN_RUNNING_PE_MEASUREMENT ((uint8)2U)
#define Extraction_IN_VERIFYING_LIMITS ((uint8)3U)
#define Extraction_IN_WAITING_BALANCE_INDEX ((uint8)4U)
#define Extraction_IN_WAITING_CONVERGENCE ((uint8)4U)
#define Extraction_event_Extr_LowSpd_EvLoadMeasStart (2)
#define Extraction_event_Extr_LowSpd_EvResetLoadMass (4)
#define Extraction_event_Extr_LowSpd_EvStop (3)
#define Extraction_event_LocalStop     (0)
#define Extraction_event_ticks_hm      (1)

/* Named constants for Chart: '<S126>/MDDPLogic' */
#define Extraction_IN_CALC_MDDP_PARAMETERS ((uint8)1U)
#define Extraction_IN_PLATEAU_END      ((uint8)3U)
#define Extraction_IN_PLATEAU_it       ((uint8)2U)
#define Extraction_IN_RAMPING_DOWN     ((uint8)5U)
#define Extraction_IN_RAMPING_ce       ((uint8)4U)
#define Extraction_IN_STATE_RUNNING    ((uint8)2U)
#define Extraction_event_Extr_Sup_EvSpeedReached_fi (4)
#define Extraction_event_evStart_eo    (2)
#define Extraction_event_evStop_m2     (3)
#define Extraction_event_localStop_ix  (0)

/* Named constants for Chart: '<S64>/Extr_HighSpd_ExtUnbCheckLogic' */
#define Extraction_IN_CALCULATING_AVG_UNB ((uint8)1U)
#define Extraction_IN_MONITORING_UNBALANCE ((uint8)1U)
#define Extraction_IN_RUNNING_eh       ((uint8)2U)
#define Extraction_IN_SEARCHING_SPEED  ((uint8)2U)
#define Extraction_IN_SKIPPING_SAMPLES ((uint8)2U)
#define Extraction_event_EvNoMatchFound (0)
#define Extraction_event_Extr_HighSpd_EvPlateauComplete_dk (5)
#define Extraction_event_Extr_HighSpd_EvStartExtndUnbChkEv (3)
#define Extraction_event_Extr_HighSpd_EvStopAll_pq (4)

/* Named constants for Chart: '<S12>/Extr_Rebal_Logic' */
#define Extraction_IN_DETECTING_PEAKS_AND_TROUGHS ((uint8)1U)
#define Extraction_IN_DISTRIBUTION_RAMP ((uint8)1U)
#define Extraction_IN_DROPPING_SPEED   ((uint8)1U)
#define Extraction_IN_EXECUTING_STATIC_PROFILE ((uint8)2U)
#define Extraction_IN_MOTOR_OFF        ((uint8)2U)
#define Extraction_IN_MOTOR_STARTING   ((uint8)2U)
#define Extraction_IN_MOTOR_STOPPING_lj ((uint8)3U)
#define Extraction_IN_PLATEAU_p1       ((uint8)3U)
#define Extraction_IN_RAMPING_TO_DIST_SPEED ((uint8)2U)
#define Extraction_IN_REBALANCE_bo     ((uint8)1U)
#define Extraction_IN_RUNNING_jg       ((uint8)3U)
#define Extraction_IN_VERIFYING_LOAD_DISTRIBUTION ((uint8)3U)
#define Extraction_event_EvLocalStop   (0)
#define Extraction_event_EvRampComplete (1)
#define Extraction_event_Extr_Accel_EvCabHit_ii (12)
#define Extraction_event_Extr_LowSpd_EvDistributionStart (6)
#define Extraction_event_Extr_LowSpd_EvResetShortRedist (7)
#define Extraction_event_Extr_Sup_ClearFault_oj (9)
#define Extraction_event_Extr_Sup_EvFoamDetected_f3 (8)
#define Extraction_event_Extr_Sup_EvRebalStart_dl (3)
#define Extraction_event_Extr_Sup_EvSpeedErrorTimeout_le (11)
#define Extraction_event_Extr_Sup_EvSpeedReached_ok (10)
#define Extraction_event_Extr_Sup_ResetRebalCnt_d5 (5)
#define Extraction_event_Extr_Sup_StopAll (4)
#ifndef UCHAR_MAX
#include <limits.h>
#endif

#if ( UCHAR_MAX != (0xFFU) ) || ( SCHAR_MAX != (0x7F) )
#error Code was generated for compiler with different sized uchar/char. \
Consider adjusting Test hardware word size settings on the \
Hardware Implementation pane to match your compiler word sizes as \
defined in limits.h of the compiler. Alternatively, you can \
select the Test hardware is the same as production hardware option and \
select the Enable portable word sizes option on the Code Generation > \
Verification pane for ERT based targets, which will disable the \
preprocessor word size checks.
#endif

#if ( USHRT_MAX != (0xFFFFU) ) || ( SHRT_MAX != (0x7FFF) )
#error Code was generated for compiler with different sized ushort/short. \
Consider adjusting Test hardware word size settings on the \
Hardware Implementation pane to match your compiler word sizes as \
defined in limits.h of the compiler. Alternatively, you can \
select the Test hardware is the same as production hardware option and \
select the Enable portable word sizes option on the Code Generation > \
Verification pane for ERT based targets, which will disable the \
preprocessor word size checks.
#endif

#if ( UINT_MAX != (0xFFFFFFFFU) ) || ( INT_MAX != (0x7FFFFFFF) )
#error Code was generated for compiler with different sized uint/int. \
Consider adjusting Test hardware word size settings on the \
Hardware Implementation pane to match your compiler word sizes as \
defined in limits.h of the compiler. Alternatively, you can \
select the Test hardware is the same as production hardware option and \
select the Enable portable word sizes option on the Code Generation > \
Verification pane for ERT based targets, which will disable the \
preprocessor word size checks.
#endif

#if ( ULONG_MAX != (0xFFFFFFFFU) ) || ( LONG_MAX != (0x7FFFFFFF) )
#error Code was generated for compiler with different sized ulong/long. \
Consider adjusting Test hardware word size settings on the \
Hardware Implementation pane to match your compiler word sizes as \
defined in limits.h of the compiler. Alternatively, you can \
select the Test hardware is the same as production hardware option and \
select the Enable portable word sizes option on the Code Generation > \
Verification pane for ERT based targets, which will disable the \
preprocessor word size checks.
#endif

#if EXTR_VARIANT_HIGH_SPEED_PE_ENABLED
#ifndef BASELINE_ARRAY_MAX_SIZE
#error The variable for the parameter "BASELINE_ARRAY_MAX_SIZE" is not defined
#endif
#endif                                 /* EXTR_VARIANT_HIGH_SPEED_PE_ENABLED */

/* user code (top of source file) */
/* System '<Root>/Initialize' */
/* Clears model states and signals */
void ClearInernalStates()
{
    // Clear system states
    memset(&Extraction_DW,0,sizeof(Extraction_DW_TYPE));

    // Clear system signals
    memset(&Extraction_B,0,sizeof(Extraction_B_TYPE));
}

const float32 Extraction_R32GND = 0.0F;/* float32 ground */
const SPIN_PULSE_TYPE Extraction_rtZSPIN_PULSE_TYPE =
{
    0U,                                /* Target_Speed_Div10 */
    0U,                                /* Dist_Time */

    {
        0U, 0U
    }
    ,                                  /* Ramp_Time */

    {
        0U, 0U
    }
    ,                                  /* Plateau_Time */
    0U,                                /* Param_Est_Config_Set_Index */
    0U,                                /* Param_Est_App_Specific_Config_Index */
    0U,                                /* High_Speed_Config_Index */
    0U,                                /* CEM_Plateau_Threshold */
    0U,                                /* Baseline_Config_Index */
    0U                                 /* Load_Mass_Config_Index */
} ;                                    /* SPIN_PULSE_TYPE ground */

const HIGH_SPEED_PULSE_CONFIG_TYPE Extraction_rtZHIGH_SPEED_PULSE_CONFIG_TYPE =
{
    0.0F,                              /* Ramp_Viscous_Friction_Threshold */
    0U,                                /* Plateau_Settling_Threshold_x1000 */
    0U,                                /* Plateau_Advance_Threshold */
    0U,                                /* Plateau_Monitor_Threshold */
    0U,                                /* Plateau_Hold_Threshold */
    0U,                                /* Plateau_Max_Averages */
    0U                                 /* Plateau_Monitor_Samples */
} ;                                    /* HIGH_SPEED_PULSE_CONFIG_TYPE ground */

const LOAD_MASS_LIMIT_SETUP_TYPE Extraction_rtZLOAD_MASS_LIMIT_SETUP_TYPE =
{
    0U,                                /* Threshold_x10 */
    0U                                 /* Delta_Time_x10 */
} ;                                    /* LOAD_MASS_LIMIT_SETUP_TYPE ground */

/* Exported block signals */
float32 Extr_Rebal_DistP2PThresh;      /* '<S12>/Extr_Rebal_Logic' */
float32 Extr_Rebal_DistP2PTorque;      /* '<S12>/Extr_Rebal_Logic' */
float32 Extr_HighSpd_ExtUnbAvg;        /* '<S64>/Extr_HighSpd_ExtUnbCheckLogic' */
float32 Extr_HighSpd_ExtUnbAvgThreshold;/* '<S64>/Extr_HighSpd_ExtUnbCheckLogic' */
float32 Extr_HighSpd_PlateauDuration;  /* '<S90>/MATLAB Function' */
float32 Extr_HighSpd_BaselineFriction; /*  */
float32 Extr_HighSpd_PrevBaselineFriction;/*  */
float32 Extr_Sup_RampTime;             /* '<S193>/Extr_Sup_MotorSpeedCheck' */
float32 Extr_Sup_RampTimeout;          /* '<S193>/Extr_Sup_MotorSpeedCheck' */
EXT_UNB_CHECK_STATE Extr_HighSpd_ExtUnbState;/* '<S64>/Extr_HighSpd_ExtUnbCheckLogic' */
HIGH_SPEED_PLATEAU_DECISION Extr_HighSpd_PlateauDecision;/*  */
LOAD_MEAS_STATE Extr_LowSpd_LoadMeasState;/* '<S125>/Extr_LowSpd_LoadMeasLogic' */
MDDP_STATE Extr_LowSpd_MddpState;      /* '<S126>/MDDPLogic' */
LOW_SPEED_STATE Extr_LowSpd_State;     /* '<S122>/Extr_LowSpd_SupervisorLogic' */
EXTR_REBAL_STATE Extr_Rebal_State;     /* '<S12>/Extr_Rebal_Logic' */
EXTR_SUP_SPEED_ERROR_MONITOR_STATE Extr_Sup_SpeedErrorMonitorState;/* '<S193>/Extr_Sup_MotorSpeedCheck' */
sint16 Extr_LowSpd_DistSpeed;          /* '<S122>/DistSpeed' */
sint16 Extr_Sup_SpdCommand[2];         /* '<S23>/SpinCommand' */
uint16 Extr_Accel_ExtPlateauTimeIncrement;/*  */
uint16 Extr_HighSpd_TargetSpeed;       /*  */
uint16 Extr_LowSpd_MaxTargetSpeed;     /* '<S121>/SpeedLimit' */
uint16 Extr_LowSpd_UnbChkSpdLimit;     /* '<S124>/Extr_LowSpd_BasicUnbCheck' */
uint16 Extr_LowSpd_UnbTargetArray[25]; /* '<S124>/Extr_LowSpd_BasicUnbCheck' */
uint8 Extr_Accel_VibMapSafeSpeed_div10;/*  */
uint8 Extr_Rebal_Index;                /* '<S183>/MATLAB Function' */
uint8 Extr_Rebal_RebalCnt;             /* '<S12>/Extr_Rebal_Logic' */
uint8 Extr_Rebal_PulseIndex;           /* '<S12>/Extr_Rebal_Logic' */
uint8 Extr_Rebal_ShrtRedistCnt;        /* '<S12>/Extr_Rebal_Logic' */
uint8 Extr_HighSpd_ExtUnbSampleCnt;    /* '<S64>/Extr_HighSpd_ExtUnbCheckLogic' */
uint8 Extr_HighSpd_BaselineFrictionIndex;/*  */
uint8 Extr_HighSpd_BaselineSpeed;      /*  */
uint8 Extr_LowSpd_MddpDur;             /* '<S126>/MddpDuration' */
boolean_T Extr_Sup_SpdMonitorPaused;   /* '<S193>/Extr_Sup_MotorSpeedCheck' */

#if EXTR_VARIANT_ACCELEROMETER_ENABLED

float32 Extr_Accel_VibMapFloorDisp;    /* '<S31>/Extr_Accel_VibMapLogic' */

#endif                                 /* EXTR_VARIANT_ACCELEROMETER_ENABLED */

#if EXTR_VARIANT_ACCELEROMETER_ENABLED

float32 Extr_Accel_VibMapFloorDispThreshold;/* '<S31>/Extr_Accel_VibMapLogic' */

#endif                                 /* EXTR_VARIANT_ACCELEROMETER_ENABLED */

#if EXTR_VARIANT_ACCELEROMETER_ENABLED

float32 Extr_Accel_VibMapFiltAccel[3]; /* '<S55>/SquaredAcceleration' */

#endif                                 /* EXTR_VARIANT_ACCELEROMETER_ENABLED */

#if EXTR_VARIANT_ACCELEROMETER_ENABLED

float32 Extr_Accel_Baseline[3];        /* '<S30>/Extr_Accel_Supervisor' */

#endif                                 /* EXTR_VARIANT_ACCELEROMETER_ENABLED */

#if EXTR_VARIANT_ACCELEROMETER_ENABLED

float32 Extr_Accel_FiltData[3];        /* '<S45>/FilterAccelerometerData' */

#endif                                 /* EXTR_VARIANT_ACCELEROMETER_ENABLED */

#if EXTR_VARIANT_ACCELEROMETER_ENABLED

float32 Extr_Accel_CabHitFiltAccel[3]; /* '<S33>/SquaredAcceleration' */

#endif                                 /* EXTR_VARIANT_ACCELEROMETER_ENABLED */

#if EXTR_VARIANT_ACCELEROMETER_ENABLED

CAB_HIT_STATE Extr_Accel_CabHitState;  /* '<S29>/Extr_Accel_CabHitLogic' */

#endif                                 /* EXTR_VARIANT_ACCELEROMETER_ENABLED */

#if EXTR_VARIANT_ACCELEROMETER_ENABLED

EXTR_ACCEL_STATE Extr_Accel_State;     /* '<S30>/Extr_Accel_Supervisor' */

#endif                                 /* EXTR_VARIANT_ACCELEROMETER_ENABLED */

#if EXTR_VARIANT_ACCELEROMETER_ENABLED

VIB_MAP_STATE Extr_Accel_VibMapState;  /* '<S31>/Extr_Accel_VibMapLogic' */

#endif                                 /* EXTR_VARIANT_ACCELEROMETER_ENABLED */

#if EXTR_VARIANT_ACCELEROMETER_ENABLED

sint16 Extr_Inputs_AccelBaselineRaw[3];/* '<S39>/GetAccelBaselineMacro' */

#endif                                 /* EXTR_VARIANT_ACCELEROMETER_ENABLED */

#if EXTR_VARIANT_ACCELEROMETER_ENABLED

sint16 Extr_Inputs_AccelRawData[3];    /* '<S45>/GetAccelData' */

#endif                                 /* EXTR_VARIANT_ACCELEROMETER_ENABLED */

#if EXTR_VARIANT_ACCELEROMETER_ENABLED

uint16 Extr_Accel_CommErrors;          /* '<S30>/Extr_Accel_Supervisor' */

#endif                                 /* EXTR_VARIANT_ACCELEROMETER_ENABLED */

#if EXTR_VARIANT_ACCELEROMETER_ENABLED

uint8 Extr_Accel_SoftCabHitCounter;    /* '<S29>/Extr_Accel_CabHitLogic' */

#endif                                 /* EXTR_VARIANT_ACCELEROMETER_ENABLED */

#if EXTR_VARIANT_ACCELEROMETER_ENABLED

boolean_T Extr_Accel_Acquiring;        /* '<S30>/Extr_Accel_Supervisor' */

#endif                                 /* EXTR_VARIANT_ACCELEROMETER_ENABLED */

#if EXTR_VARIANT_HIGH_SPEED_PE_ENABLED

float32 Extr_HighSpd_RampViscousThreshold;/* '<S80>/Extr_HighSpd_RampLogic' */

#endif                                 /* EXTR_VARIANT_HIGH_SPEED_PE_ENABLED */

#if EXTR_VARIANT_HIGH_SPEED_PE_ENABLED

float32 Extr_HighSpd_RampFrictionThreshold;/* '<S80>/Extr_HighSpd_RampLogic' */

#endif                                 /* EXTR_VARIANT_HIGH_SPEED_PE_ENABLED */

#if EXTR_VARIANT_HIGH_SPEED_PE_ENABLED

float32 Extr_HighSpd_RampFrictionDelta;/* '<S80>/Extr_HighSpd_RampLogic' */

#endif                                 /* EXTR_VARIANT_HIGH_SPEED_PE_ENABLED */

#if EXTR_VARIANT_HIGH_SPEED_PE_ENABLED

HIGH_SPEED_RAMP_STATE Extr_HighSpd_RampState;/* '<S80>/Extr_HighSpd_RampLogic' */

#endif                                 /* EXTR_VARIANT_HIGH_SPEED_PE_ENABLED */

#if EXTR_VARIANT_HIGH_SPEED_PE_DISABLED

HIGH_SPEED_RAMP_STATE Extr_HighSpd_RampState;/* '<S79>/Extr_HighSpd_RampLogic' */

#endif                                 /* EXTR_VARIANT_HIGH_SPEED_PE_DISABLED */

#if EXTR_VARIANT_HIGH_SPEED_PE_ENABLED

float32 Extr_HighSpd_FrictionAvg;      /* '<S75>/Extr_HighSpd_PlateauLogic' */

#endif                                 /* EXTR_VARIANT_HIGH_SPEED_PE_ENABLED */

#if EXTR_VARIANT_HIGH_SPEED_PE_ENABLED

float32 Extr_HighSpd_FrictionRng;      /* '<S75>/Extr_HighSpd_PlateauLogic' */

#endif                                 /* EXTR_VARIANT_HIGH_SPEED_PE_ENABLED */

#if EXTR_VARIANT_HIGH_SPEED_PE_ENABLED

float32 Extr_HighSpd_DeltaFrictionAvg; /* '<S75>/Extr_HighSpd_PlateauLogic' */

#endif                                 /* EXTR_VARIANT_HIGH_SPEED_PE_ENABLED */

#if EXTR_VARIANT_HIGH_SPEED_PE_ENABLED

HIGH_SPEED_PLATEAU_STATE Extr_HighSpd_PlateauState;/* '<S75>/Extr_HighSpd_PlateauLogic' */

#endif                                 /* EXTR_VARIANT_HIGH_SPEED_PE_ENABLED */

#if EXTR_VARIANT_HIGH_SPEED_PE_ENABLED

boolean_T Extr_HighSpd_PlateauTOnDone; /* '<S75>/Extr_HighSpd_PlateauLogic' */

#endif                                 /* EXTR_VARIANT_HIGH_SPEED_PE_ENABLED */

#if EXTR_VARIANT_HIGH_SPEED_PE_DISABLED

HIGH_SPEED_PLATEAU_STATE Extr_HighSpd_PlateauState;/* '<S74>/Extr_HighSpd_PlateauLogic' */

#endif                                 /* EXTR_VARIANT_HIGH_SPEED_PE_DISABLED */

#if EXTR_VARIANT_HIGH_SPEED_PE_ENABLED

float32 Extr_HighSpd_LoadMassLimit;    /* '<S72>/Extr_HighSpd_LoadMassLimitCheckLogic' */

#endif                                 /* EXTR_VARIANT_HIGH_SPEED_PE_ENABLED */

#if EXTR_VARIANT_HIGH_SPEED_PE_ENABLED

float32 Extr_HighSpd_CEMFrictionThreshold;/* '<S69>/Extr_HighSpd_CEMLogic' */

#endif                                 /* EXTR_VARIANT_HIGH_SPEED_PE_ENABLED */

#if EXTR_VARIANT_HIGH_SPEED_PE_ENABLED

float32 Extr_HighSpd_CEMAvgFrictionDelta;/* '<S69>/Extr_HighSpd_CEMLogic' */

#endif                                 /* EXTR_VARIANT_HIGH_SPEED_PE_ENABLED */

#if EXTR_VARIANT_HIGH_SPEED_PE_ENABLED

CEM_STATE Extr_HighSpd_CEMState;       /* '<S69>/Extr_HighSpd_CEMLogic' */

#endif                                 /* EXTR_VARIANT_HIGH_SPEED_PE_ENABLED */

#if EXTR_VARIANT_ACCELEROMETER_ENABLED

uint16 Extr_Accel_VibMapRefDisp;       /* '<S119>/VerifyVibMapFloorDisplacementMATLAB' */

#endif                                 /* EXTR_VARIANT_ACCELEROMETER_ENABLED */

#if EXTR_VARIANT_OPT_RAMP_ENABLED

float32 Extr_LowSpd_OptRampUpperTh;    /* '<S150>/OptRampLogic' */

#endif                                 /* EXTR_VARIANT_OPT_RAMP_ENABLED */

#if EXTR_VARIANT_OPT_RAMP_ENABLED

float32 Extr_LowSpd_OptRampLowerTh;    /* '<S150>/OptRampLogic' */

#endif                                 /* EXTR_VARIANT_OPT_RAMP_ENABLED */

#if EXTR_VARIANT_OPT_RAMP_ENABLED

float32 Extr_LowSpd_OptRampUnbMass;    /* '<S150>/OptRampLogic' */

#endif                                 /* EXTR_VARIANT_OPT_RAMP_ENABLED */

#if EXTR_VARIANT_OPT_RAMP_ENABLED

float32 Extr_LowSpd_OptRampBBMass;     /* '<S150>/OptRampLogic' */

#endif                                 /* EXTR_VARIANT_OPT_RAMP_ENABLED */

#if EXTR_VARIANT_OPT_RAMP_ENABLED

float32 Extr_LowSpd_OptRampORA2BBA;    /* '<S150>/OptRampLogic' */

#endif                                 /* EXTR_VARIANT_OPT_RAMP_ENABLED */

#if EXTR_VARIANT_OPT_RAMP_ENABLED

float32 Extr_LowSpd_OptRampPhsTol;     /* '<S150>/OptRampLogic' */

#endif                                 /* EXTR_VARIANT_OPT_RAMP_ENABLED */

#if EXTR_VARIANT_OPT_RAMP_ENABLED

float32 Extr_LowSpd_OptRampPhsAdvTgt;  /* '<S150>/OptRampLogic' */

#endif                                 /* EXTR_VARIANT_OPT_RAMP_ENABLED */

#if EXTR_VARIANT_OPT_RAMP_ENABLED

OPT_RAMP_STATE Extr_LowSpd_OptRampState;/* '<S150>/OptRampLogic' */

#endif                                 /* EXTR_VARIANT_OPT_RAMP_ENABLED */

#if EXTR_VARIANT_OPT_RAMP_ENABLED

OPT_RAMP_EXIT_REASON Extr_LowSpd_OptRampExitReason;/* '<S150>/OptRampLogic' */

#endif                                 /* EXTR_VARIANT_OPT_RAMP_ENABLED */

#if EXTR_VARIANT_OPT_RAMP_ENABLED

uint16 Extr_LowSpd_OptRampConvUnb;     /* '<S153>/Max' */

#endif                                 /* EXTR_VARIANT_OPT_RAMP_ENABLED */

#if EXTR_VARIANT_OPT_RAMP_ENABLED

uint8 Extr_LowSpd_OptRampPlatTime;     /* '<S150>/OptRampLogic' */

#endif                                 /* EXTR_VARIANT_OPT_RAMP_ENABLED */

#if EXTR_VARIANT_OPT_RAMP_ENABLED

uint8 Extr_LowSpd_OptRampConvTime;     /* '<S153>/SumpTempToConvTime' */

#endif                                 /* EXTR_VARIANT_OPT_RAMP_ENABLED */

PARAM_EST_OUTPUT_TYPE Extr_Inputs_ParamEstData;/* '<S221>/Bus Creator' */
DIST_UTILS_OUTPUT_TYPE Extr_Inputs_DistUtilsData;/* '<S14>/GetDistUtilsOutput' */
MOTOR_OUTPUT_TYPE Extr_Inputs_MotorSignals;/* '<S216>/Bus Creator' */
float32 Extr_Inputs_WaterTemp;         /* '<S14>/GetWaterTemp' */
float32 Extr_Inputs_AcuLoadMass;       /* '<S14>/GetSaturatedLoadMass' */
uint16 Extr_Inputs_MinFinalSpeed;      /* '<S14>/GetMinSpeed' */
uint16 Extr_Inputs_UnbalanceIndexCnt;  /* '<S14>/GetMciRawUnbalanceCount' */
uint16 Extr_Inputs_UnbalanceIndex;     /* '<S14>/GetMciRawUnbalance' */
uint16 Extr_Inputs_MaxSpeed;           /* '<S14>/GetFinalSpeed' */
uint16 Extr_Inputs_ExtPlateauDuration; /* '<S14>/GetExtPlateauDuration' */
uint8 Extr_Inputs_RampTimeout;         /* '<S14>/GetRampTimeout' */
uint8 Extr_Inputs_MaxSpeedError;       /* '<S14>/GetMaxSpeedError' */

/* Exported block states */
float32 Extr_LowSpd_LoadMass;          /* Simulink.Signal object 'Extr_LowSpd_LoadMass' */
float32 Extr_Accel_MaxAccelSpeed;      /* Simulink.Signal object 'Extr_Accel_MaxAccelSpeed' */
float32 Extr_Accel_MaxAccel;           /* Simulink.Signal object 'Extr_Accel_MaxAccel' */
float32 Extr_Sup_LoadMass;             /* Simulink.Signal object 'Extr_Sup_LoadMass' */
float32 Extr_HighSpd_ViscousFriction;  /* Simulink.Signal object 'Extr_HighSpd_ViscousFriction' */
float32 Extr_HighSpd_PrevPlateauTime;  /* Simulink.Signal object 'Extr_HighSpd_PrevPlateauTime' */
float32 Extr_HighSpd_Friction;         /* Simulink.Signal object 'Extr_HighSpd_Friction' */
float32 Extr_HighSpd_ReferenceFriction;/* Simulink.Signal object 'Extr_HighSpd_ReferenceFriction' */
float32 Extr_Accel_MaxVibMapFloorDisp; /* Simulink.Signal object 'Extr_Accel_MaxVibMapFloorDisp' */
HIGH_SPEED_STATE Extr_HighSpd_State;   /* Simulink.Signal object 'Extr_HighSpd_State' */
EXTRACTION_STATE Extr_Sup_State;       /* Simulink.Signal object 'Extr_Sup_State' */
uint16 Extr_LowSpd_Unbalance;          /* Simulink.Signal object 'Extr_LowSpd_Unbalance' */
uint16 Extr_HighSpd_CEMEventCounter;   /* Simulink.Signal object 'Extr_HighSpd_CEMEventCounter' */
uint16 Extr_Accel_SoftCabHitCounterOverall;/* Simulink.Signal object 'Extr_Accel_SoftCabHitCounterOverall' */
uint16 Extr_Accel_HardCabHitCounter;   /* Simulink.Signal object 'Extr_Accel_HardCabHitCounter' */
uint16 Extr_LowSpd_MddpSpd;            /* '<S126>/MDDPLogic' */
sint8 Extr_HighSpd_CurrPulseInd;       /* Simulink.Signal object 'Extr_HighSpd_CurrPulseInd' */
uint8 Extr_Sup_SpdComCounter;          /* Simulink.Signal object 'Extr_Sup_SpdComCounter' */
uint8 Extr_LowSpd_RebalCnt;            /* Simulink.Signal object 'Extr_LowSpd_RebalCnt' */
uint8 Extr_HighSpd_RebalCnt;           /* Simulink.Signal object 'Extr_HighSpd_RebalCnt' */
uint8 Extr_LowSpd_MddpRmp;             /* '<S126>/MDDPLogic' */
boolean_T Extr_Rebal_RebInProgress;    /* Simulink.Signal object 'Extr_Rebal_RebInProgress' */

/* Block signals (auto storage) */
Extraction_B_TYPE Extraction_B;

/* Block states (auto storage) */
Extraction_DW_TYPE Extraction_DW;

/* Previous zero-crossings (trigger) states */
Extraction_PrevZCX_TYPE Extraction_PrevZCX;

/* External inputs (root inport signals with auto storage) */
Extraction_ExtU_TYPE Extraction_U;

/* Exported data definition */

/* Definition for custom storage class: Global */
#if EXTR_VARIANT_HIGH_SPEED_PE_ENABLED

EXTR_HIGH_SPD_FAULT Extr_HighSpd_Fault;

#endif                                 /* EXTR_VARIANT_HIGH_SPEED_PE_ENABLED */

#if EXTR_VARIANT_HIGH_SPEED_PE_DISABLED

EXTR_HIGH_SPD_FAULT Extr_HighSpd_Fault;

#endif                                 /* EXTR_VARIANT_HIGH_SPEED_PE_DISABLED */

EXTR_LOW_SPD_FAULT Extr_LowSpd_Fault;
EXTR_REBAL_FAULT Extr_Rebal_Fault;
uint32 Extr_Sup_Fault;
extern const PARAM_EST_SETUP_TYPE rtCP_pooled_q3rGmjMUhtl1;
extern const PARAM_EST_EXC_SETUP_TYPE rtCP_pooled_cI07u4KO8Mke;
extern const float32 rtCP_pooled_luOqsJIXWKnl;
extern const uint8 rtCP_pooled_e6HU8n7YauPz;
extern const uint8 rtCP_pooled_1H5yfYlyUd9Z[8];

#define rtCP_NullPeSetup_Value         rtCP_pooled_q3rGmjMUhtl1  /* Computed Parameter: rtCP_NullPeSetup_Value
                                                                  * Referenced by: '<S18>/NullPeSetup'
                                                                  */
#define rtCP_Constant6_Value           rtCP_pooled_q3rGmjMUhtl1  /* Computed Parameter: rtCP_Constant6_Value
                                                                  * Referenced by: '<S21>/Constant6'
                                                                  */
#define rtCP_Constant_Value            rtCP_pooled_cI07u4KO8Mke  /* Computed Parameter: rtCP_Constant_Value
                                                                  * Referenced by: '<S19>/Constant'
                                                                  */
#define rtCP_Constant_Value_g3         rtCP_pooled_luOqsJIXWKnl  /* Computed Parameter: rtCP_Constant_Value_g3
                                                                  * Referenced by: '<S185>/Constant'
                                                                  */
#define rtCP_Constant1_Value_lc        rtCP_pooled_luOqsJIXWKnl  /* Computed Parameter: rtCP_Constant1_Value_lc
                                                                  * Referenced by: '<S185>/Constant1'
                                                                  */
#define rtCP_Constant_Value_cy         rtCP_pooled_e6HU8n7YauPz  /* Computed Parameter: rtCP_Constant_Value_cy
                                                                  * Referenced by: '<S261>/Constant'
                                                                  */
#define rtCP_Constant6_Value_oz        rtCP_pooled_1H5yfYlyUd9Z  /* Computed Parameter: rtCP_Constant6_Value_oz
                                                                  * Referenced by: '<S261>/Constant6'
                                                                  */
#define rtCP_Constant6_Value_pe        rtCP_pooled_1H5yfYlyUd9Z  /* Computed Parameter: rtCP_Constant6_Value_pe
                                                                  * Referenced by: '<S18>/Constant6'
                                                                  */
#ifdef __cplusplus

extern "C"
{

#endif

    extern void ParamEst_sfun_Outputs_wrapper(const uint8 *Command,
        const PARAM_EST_SETUP_TYPE *Setup,
        const uint8 *Set_Inputs,
        const uint8 *Outputs,
        const float32 *Speed_RPM,
        const float32 *SIN_Drum_Pos,
        const float32 *COS_Drum_Pos,
        const float32 *Torque_Nm,
        float32 *J,
        float32 *mgr,
        float32 *mgr_theta,
        float32 *b,
        float32 *c,
        float32 *f,
        float32 *mgr_BB,
        float32 *BB_phaselag);

#ifdef __cplusplus

}

#endif

#ifdef __cplusplus

extern "C"
{

#endif

    extern void ParamEstExc_sfun_Outputs_wrapper(const PARAM_EST_EXC_SETUP_TYPE *
        PE_Exc_Setup,
        const uint8 *Command,
        const float32 *w,
        float32 *w_offset,
        boolean_T *is_running);

#ifdef __cplusplus

}

#endif

#ifdef __cplusplus

extern "C"
{

#endif

    extern void DistUtils_sfun_Outputs_wrapper(const uint8 *command,
        const float32 *speed_ref,
        const float32 *torque,
        DIST_UTILS_DXDT_OUTPUT_TYPE *dxdtOutput,
        float32 *agreggateP2PTorque,
        float32 *satelizationSpeed,
        float32 *windowPower);

#ifdef __cplusplus

}

#endif

void Extraction_Extr_UpdateInputs(void);
void Extraction_TriggerGen(Extraction_B_TriggerGen_TYPE *localB,
    Extraction_DW_TriggerGen_TYPE *localDW);
void Extraction_UpdatePEAppSpecificSetup
    (Extraction_B_UpdatePEAppSpecificSetup_TYPE *localB);
void Extraction_IfActionSubsystem1(float32 rtu_angle, float32 *rty_calc_angle);

#if EXTR_VARIANT_HIGH_SPEED_PE_ENABLED

void Extraction_FRICTION_BASED_PLATEAUMONITORUpdateFactoryCalibrationFriction
    (float32 rtu_friction, uint8 rtu_baseline_index);

#endif                                 /* EXTR_VARIANT_HIGH_SPEED_PE_ENABLED */

void Extraction_DistUtilsWrapperControl(DIST_UTILS_COMMAND_TYPE
    rtu_distUtilsCommand, Extraction_B_DistUtilsWrapperControl_TYPE *localB);
void Extraction_REBALANCERUNNINGDISTRIBUTION_RAMPP2PTorqueThresholdCalc(float32
    rtu_mass, float32 rtu_currentSpeed,
    Extraction_B_REBALANCERUNNINGDISTRIBUTION_RAMPP2PTorqueThresholdCalc_TYPE
    *localB);

#if EXTR_VARIANT_ACCELEROMETER_ENABLED

void Extraction_CalcFloorDispThreshold(const MOTOR_OUTPUT_TYPE
    *rtu_motor_signals, const VIB_MAP_FLOOR_DISPL_THRESHOLD_TYPE
    *rtu_fd_threshold_setup, Extraction_B_CalcFloorDispThreshold_TYPE *localB);

#endif                                 /* EXTR_VARIANT_ACCELEROMETER_ENABLED */

void Extraction_GetMciFault(uint32 *rty_fault);
void Extraction_IsAccelEnabled(boolean_T *rty_y);
void Extraction_UpdateParamEstSetup_Init(void);
void Extraction_UpdateParamEstSetup(uint8 rtu_index);
void Extraction_IsMotorRunning(boolean_T *rty_motorRunningFlag);
void Extraction_ParamEstControl(PARAM_EST_COMMANDS rtu_command);
void Extraction_ParamEstExcControl(PARAM_EST_EXC_COMMANDS rtu_command);
void Extraction_ReleaseMotion(void);
void Extraction_SelectParamEstOutputs(const uint8 rtu_output_selector[8]);
void Extraction_SetFault(EXTR_FAULT rtu_fault);
void Extraction_StopMotorCommand(sint16 rtu_decel);
void Extraction_SetSpeedCommand(sint16 rtu_speed, sint16 rtu_ramp);
void Extraction_StopMotor(void);
void Extraction_ResetInitialPulseConfig(void);
void Extraction_LoadDistributionSpeed(sint16 *rty_speed);
void Extraction_RetrieveMaxTargetSpeed(void);
void Extraction_CalculateUnbalanceTarget(uint8 rtu_RowIndex, uint16
    *rty_UnbTarget);
void Extraction_CalculateUnbalanceTargetArray(uint16 rty_UnbTargetArray[25]);
void Extraction_CalcORAToBBAngle(float32 rtu_bb_phase_lag, float32
    rtu_target_angle, float32 *rty_angle);
void Extraction_FinalSpeedToUnb(boolean_T rtu_mddp_executed, const
    PRE_LOOKUP_OUTPUT_TYPE *rtu_speed_index, const PRE_LOOKUP_OUTPUT_TYPE
    *rtu_load_mass_index, uint16 *rty_unb);
void Extraction_LoadMassToUpperOffset(boolean_T rtu_mddp_executed, const
    PRE_LOOKUP_OUTPUT_TYPE *rtu_load_mass_index, sint16 *rty_upper_offset);

#if EXTR_VARIANT_HIGH_SPEED_PE_ENABLED

void Extraction_ClearBaselineFrictionArrays(void);

#endif                                 /* EXTR_VARIANT_HIGH_SPEED_PE_ENABLED */

#if EXTR_VARIANT_HIGH_SPEED_PE_ENABLED

void Extraction_DecreaseCurrentBaselineFriction(sint8 rtu_pulse_ind);

#endif                                 /* EXTR_VARIANT_HIGH_SPEED_PE_ENABLED */

#if EXTR_VARIANT_HIGH_SPEED_PE_ENABLED

void Extraction_IncreaseCEMEventCounter(void);

#endif                                 /* EXTR_VARIANT_HIGH_SPEED_PE_ENABLED */

#if EXTR_VARIANT_HIGH_SPEED_PE_ENABLED

void Extraction_IncreaseCurrentBaselineFriction(sint8 rtu_pulse_ind);

#endif                                 /* EXTR_VARIANT_HIGH_SPEED_PE_ENABLED */

#if EXTR_VARIANT_HIGH_SPEED_PE_ENABLED

void Extraction_UpdateBaselineFrictionArray(void);

#endif                                 /* EXTR_VARIANT_HIGH_SPEED_PE_ENABLED */

#if EXTR_VARIANT_HIGH_SPEED_PE_ENABLED

void Extraction_VerifyBaselineFrictionArray(void);

#endif                                 /* EXTR_VARIANT_HIGH_SPEED_PE_ENABLED */

void Extraction_LoadSpinPulse(sint8 rtu_index, SPIN_PULSE_TYPE *rty_pulse);
void Extraction_UpdateSpinPulseConfig(sint8 rtu_index);
void Extraction_UpdatePlateauTime(const SPIN_PULSE_TYPE *rtu_spin_pulse,
    boolean_T rtu_dec_plat_time_flag, float32 rtu_time_override, float32
    *rty_duration);
void Extraction_VerifyVibMapFloorDisplacement(uint8 rtu_target_speed_div10,
    uint16 *rty_speed_limit, boolean_T *rty_decrease_plateau_time);
void Extraction_FindInitialDistributionPulse(float32 rtu_speed, uint8 *rty_index);
void Extraction_LoadRebalanceProfile(uint8 rtu_index, boolean_T *rty_pass);
void Extraction_LoadRebalanceProfileIndex(uint8 rtu_rebal_count, uint8
    *rty_index);

#if EXTR_VARIANT_ACCELEROMETER_ENABLED

void Extraction_AccelerometerControl(boolean_T rtu_enabled);

#endif                                 /* EXTR_VARIANT_ACCELEROMETER_ENABLED */

#if EXTR_VARIANT_ACCELEROMETER_ENABLED

void Extraction_GetAccelBaseline(float32 rty_Accel_Baseline[3]);

#endif                                 /* EXTR_VARIANT_ACCELEROMETER_ENABLED */

#if EXTR_VARIANT_ACCELEROMETER_ENABLED

void Extraction_GetAccelBaselineStatus(ACCEL_BASELINE_STATUS_TYPE
    *rty_Accel_Baseline_Status);

#endif                                 /* EXTR_VARIANT_ACCELEROMETER_ENABLED */

#if EXTR_VARIANT_ACCELEROMETER_ENABLED

void Extraction_GetAccelStatus(ACCELEROMETER_STATUS_TYPE *rty_Accel_Status);

#endif                                 /* EXTR_VARIANT_ACCELEROMETER_ENABLED */

#if EXTR_VARIANT_ACCELEROMETER_ENABLED

void Extraction_MeasureBaseline(void);

#endif                                 /* EXTR_VARIANT_ACCELEROMETER_ENABLED */

#if EXTR_VARIANT_OPT_RAMP_ENABLED

/* Forward declaration for local functions */
static float32 Extraction_ConvertMgrToGrams(float32 ub_h, float32 coeffs_K2_h,
    float32 coeffs_K4_h, sint16 coeffs_K1_h, sint16 coeffs_K3_h);
static void Extraction_broadcast_localStop(void);
static boolean_T Extraction_BallBalancerAtOptimalRampAngle(float32
    oraToBBAngle_h, float32 window_h);
static void Extraction_c13_Extraction(void);
static void Extraction_chartstep_c13_Extraction(void);

#endif                                 /* EXTR_VARIANT_OPT_RAMP_ENABLED */

#if EXTR_VARIANT_HIGH_SPEED_PE_DISABLED

/* Forward declaration for local functions */
static void Extraction_enter_atomic_HIGH_SPEED_COMPLETE(void);
static void Extraction_enter_atomic_CONFIGURING_PULSE(void);
static void Extraction_enter_internal_RUNNING(void);
static void Extraction_RUNNING(void);
static void Extraction_c48_Extraction(void);
static void Extraction_chartstep_c48_Extraction(void);

#endif                                 /* EXTR_VARIANT_HIGH_SPEED_PE_DISABLED */

#if EXTR_VARIANT_HIGH_SPEED_PE_ENABLED

/* Forward declaration for local functions */
static void Extraction_broadcast_Local_EvStop(void);
static void Extraction_enter_atomic_HIGH_SPEED_COMPLETE_dq(void);
static void Extraction_enter_atomic_CONFIGURING_PULSE_ad(void);
static void Extraction_enter_internal_RUNNING_n0(void);
static void Extraction_broadcast_EvStartRebal(void);
static void Extraction_EXTENDED_PLATEAU(void);
static void Extraction_PLATEAU(void);
static void Extraction_PLATEAU_COMPLETE(void);
static void Extraction_RAMP(void);
static void Extraction_enter_atomic_REBALANCE(void);
static void Extraction_RUNNING_ob(void);
static void Extraction_c17_Extraction(void);
static void Extraction_chartstep_c17_Extraction(void);

#endif                                 /* EXTR_VARIANT_HIGH_SPEED_PE_ENABLED */

#if EXTR_VARIANT_HIGH_SPEED_PE_ENABLED

/* Forward declaration for local functions */
static void Extraction_enter_atomic_EVALUATING_FRICTION(void);
static float32 Extraction_CalculateFrictionBaseline(float32 targSpeed_p, float32
    prevSpeed_p, float32 currBaseline_p, float32 prevBaseline_p, float32
    currentSpeed_p, uint8 spinPulse_CEM_Plateau_Threshold_p, uint8
    prevSpinPulse_CEM_Plateau_Threshold_p);
static float32 Extraction_CalcAverageFriction(const float32 vec_data_p[10],
    uint8 vec_length_p);
static void Extraction_c21_Extraction(void);
static void Extraction_chartstep_c21_Extraction(void);

#endif                                 /* EXTR_VARIANT_HIGH_SPEED_PE_ENABLED */

#if EXTR_VARIANT_HIGH_SPEED_PE_ENABLED

/* Forward declaration for local functions */
static float32 Extraction_CalculateLoadMass(float32 J_b, float32 coeffs_A_b,
    float32 coeffs_B_b, float32 coeffs_C_b);
static float32 Extraction_CalculateLoadMassLimit(float32 currThreshold_b,
    float32 prevThreshold_b, float32 currentSpeed_b, float32 targSpeed_b,
    float32 prevSpeed_b);
static void Extraction_chartstep_c36_Extraction(const sint32 *sfEvent_b);

#endif                                 /* EXTR_VARIANT_HIGH_SPEED_PE_ENABLED */

#if EXTR_VARIANT_HIGH_SPEED_PE_DISABLED

/* Forward declaration for local functions */
static void Extraction_broadcast_EvStop(void);
static void Extraction_c50_Extraction(void);
static void Extraction_chartstep_c50_Extraction(void);

#endif                                 /* EXTR_VARIANT_HIGH_SPEED_PE_DISABLED */

#if EXTR_VARIANT_HIGH_SPEED_PE_ENABLED

/* Forward declaration for local functions */
static void Extraction_broadcast_EvStop_hv(void);
static void Extraction_CalcAverageAndRange(const float32 vec_data_g[10], uint8
    vec_length_g, float32 *vec_avg_g, float32 *vec_rng_g);
static void Extraction_enter_atomic_MONITORING_PLATEAU_SETTLING(void);
static HIGH_SPEED_PLATEAU_DECISION Extraction_VerifyPlateauFriction(float32
    friction_avg_g, uint8 pulse_config_Plateau_Advance_Threshold_g, uint8
    pulse_config_Plateau_Monitor_Threshold_g, uint8
    pulse_config_Plateau_Hold_Threshold_g, float32 default_friction_g);
static uint16 Extraction_CalcNewFrictionBaseline(float32 delta_friction_g,
    float32 baseline_friction_g, uint16 baseline_config_Friction_Max_x1000_g,
    uint16 baseline_config_Friction_Min_x1000_g, uint8
    baseline_config_Baseline_Update_Threshold_g, uint8
    baseline_config_Percent_Of_Current_Threshold_g);
static void Extraction_enter_atomic_EVALUATING_FRICTION_er(void);
static void Extraction_exit_internal_FRICTION_BASED_PLATEAU(void);
static void Extraction_FRICTION_BASED_PLATEAU(void);
static void Extraction_c3_Extraction(void);
static void Extraction_chartstep_c3_Extraction(void);

#endif                                 /* EXTR_VARIANT_HIGH_SPEED_PE_ENABLED */

#if EXTR_VARIANT_HIGH_SPEED_PE_DISABLED

/* Forward declaration for local functions */
static void Extraction_chartstep_c49_Extraction(const sint32 *sfEvent_a);

#endif                                 /* EXTR_VARIANT_HIGH_SPEED_PE_DISABLED */

#if EXTR_VARIANT_HIGH_SPEED_PE_ENABLED

/* Forward declaration for local functions */
static float32 Extraction_CalculateInterpolatedThreshold(float32 targSpeed_l,
    float32 prevSpeed_l, float32 currThreshold_l, float32 prevThreshold_l,
    float32 currentSpeed_l);
static void Extraction_enter_atomic_MONITORING(void);
static uint16 Extraction_CalcNewFrictionBaseline_b1(float32 delta_friction_l,
    float32 baseline_friction_l, uint16 baseline_config_Friction_Max_x1000_l,
    uint16 baseline_config_Friction_Min_x1000_l, uint8
    baseline_config_Baseline_Update_Threshold_l, uint8
    baseline_config_Percent_Of_Current_Threshold_l);
static void Extraction_enter_atomic_RAMPING(void);
static void Extraction_chartstep_c8_Extraction(const sint32 *sfEvent_l);

#endif                                 /* EXTR_VARIANT_HIGH_SPEED_PE_ENABLED */

#if EXTR_VARIANT_ACCELEROMETER_ENABLED

/* Forward declaration for local functions */
static void Extraction_RUNNING_nh(const boolean_T *CabHitEnabled_j, const sint32
    *sfEvent_j);
static void Extraction_chartstep_c33_Extraction(const boolean_T *CabHitEnabled_j,
    const sint32 *sfEvent_j);
static float32 Extraction_CalcFloorDisplacement(const float32 accel_data_j[3],
    float32 floor_disp_setup_FD_Calc_Friction_Coeff_j, float32
    floor_disp_setup_FD_Calc_Velocity_Ref_Coeff_j, float32
    floor_disp_setup_FD_Calc_Velocity_Actual_Coeff_j, float32
    floor_disp_setup_FD_Calc_Accel_X_Coeff_j, float32
    floor_disp_setup_FD_Calc_Accel_Y_Coeff_j, float32
    floor_disp_setup_FD_Calc_Accel_XX_Coeff_j, float32
    floor_disp_setup_FD_Calc_Accel_YX_Coeff_j, float32
    floor_disp_setup_FD_Calc_Accel_ZX_Coeff_j, float32
    floor_disp_setup_FD_Calc_Accel_YY_Coeff_j, float32
    floor_disp_setup_FD_Calc_Accel_ZY_Coeff_j, float32
    floor_disp_setup_FD_Calc_Accel_ZZ_Coeff_j, float32
    floor_disp_setup_FD_Calc_Intercept_j, float32 pe_friction_j, float32
    motor_signals_Speed_j, float32 motor_signals_Speed_Ref_j);
static void Extraction_UpdateVibMapRefFloorDisp(uint8
    vib_map_setup_Percentage_Of_Current_j, uint8
    vib_map_setup_Reference_Update_Limit_x100_j, float32 floor_disp_j, uint8
    spin_pulse_Target_Speed_Div10_j);
static uint8 Extraction_CalcSafeSpeed(float32 th_cross_speed_j, uint8
    vib_map_setup_Max_Delta_Rpm_Div10_j, const uint16 floor_disp_array_x100_j[25],
    uint8 speed_ref_array_div10_N_j, const uint8 speed_ref_array_div10_BP_j[25],
    uint8 spin_pulse_Target_Speed_Div10_j);
static uint16 Extraction_CalcPlateauTimeIncrement(uint8 target_speed_div10_j,
    uint8 safe_speed_div10_j, sint8 vib_map_setup_Plateau_Duration_Ext_M_x10_j,
    sint8 vib_map_setup_Plateau_Duration_Ext_B_Div10_j);

#endif                                 /* EXTR_VARIANT_ACCELEROMETER_ENABLED */

/* Forward declaration for local functions */
static void Extraction_RUNNING_oa(const sint32 *sfEvent_ha);
static void Extraction_exit_internal_EXTRACTION(void);
static void Extraction_chartstep_c7_Extraction(const sint32 *sfEvent_ha);
static void Extraction_enter_atomic_MOTOR_ON(const uint8
    *Extr_Inputs_MaxSpeedError_iv, const uint8 *Extr_Inputs_RampTimeout_dy,
    const MOTOR_OUTPUT_TYPE *Extr_Inputs_MotorSignals_os, sint16
    Extr_Sup_SpdCommand_hvd[2]);
static void Extraction_chartstep_c47_Extraction(const uint8
    *Extr_Inputs_MaxSpeedError_iv, const uint8 *Extr_Inputs_RampTimeout_dy,
    const MOTOR_OUTPUT_TYPE *Extr_Inputs_MotorSignals_os, uint8
    *Extr_Sup_SpdComCounter_cy, sint16 Extr_Sup_SpdCommand_hvd[2], sint32
    *sfEvent_pq);
static void Extraction_enter_atomic_LOW_SPEED_COMPLETED(void);
static void Extraction_RUNNING_lv(void);
static void Extraction_exit_internal_LOW_SPEED_EXTRACTION(void);
static void Extraction_c15_Extraction(void);
static void Extraction_chartstep_c15_Extraction(void);
static float32 Extraction_CalculateAverageLoadMass(const float32 inertia_buf[15],
    uint8 n_samp);
static float32 Extraction_CalculateLoadMass_o3(float32 J, float32 coeffs_A,
    float32 coeffs_B, float32 coeffs_C);
static float32 Extraction_ConvertMgrToGrams_ey(float32 ub, float32 coeffs_K2,
    float32 coeffs_K4, sint16 coeffs_K1, sint16 coeffs_K3);
static void Extraction_c9_Extraction(void);
static void Extraction_chartstep_c9_Extraction(void);
static void Extraction_broadcast_Extr_LowSpd_EvUnbChkComplete_local(void);
static void Extraction_enter_atomic_COMPLETE_ST(void);
static void Extraction_c42_Extraction(void);
static void Extraction_chartstep_c42_Extraction(void);
static void Extraction_enter_atomic_PLATEAU_END(void);
static void Extraction_c11_Extraction(void);
static void Extraction_chartstep_c11_Extraction(void);
static uint16 Extraction_GetUnbalanceTarget(const EXT_UNB_CHK_TARGET_TYPE
    unb_targ_set[25], uint8 b_index);
static uint8 Extraction_GetSpeedTarget(const EXT_UNB_CHK_TARGET_TYPE
    unb_targ_set[25], uint8 b_index);
static void Extraction_c45_Extraction(void);
static void Extraction_chartstep_c45_Extraction(void);
static void Extraction_broadcast_EvLocalStop(void);
static void Extraction_enter_atomic_DISTRIBUTION_RAMP(void);
static void Extraction_enter_internal_RUNNING_lp(void);
static void Extraction_broadcast_EvRampComplete(void);
static sint16 Extraction_CalcDropSpeed(uint8 setup_Short_Redist_Delta_Speed_Drop,
    uint8 setup_Short_Redist_Min_Velocity, float32 satelizationSpeed, float32
    speedRef);
static void Extraction_DISTRIBUTION_RAMP(void);
static void Extraction_RUNNING_bk(void);
static void Extraction_REBALANCE(void);
static void Extraction_c24_Extraction(void);
static void Extraction_chartstep_c24_Extraction(void);

/* Output and update for Simulink Function: '<S2>/GetMciFault' */
void Extraction_GetMciFault(uint32 *rty_fault)
{
    /* MATLAB Function: '<S15>/ReleaseMotionMacro' */
    /* MATLAB Function 'CreateFunctionMacro': '<S238>:1' */
    // Retrieve the current Mci fault	();
    /* '<S238>:1:8' */
    *rty_fault = EXTRACTION_GET_MCI_FAULT();//();
}

/* Output and update for Simulink Function: '<S2>/IsAccelEnabled' */
void Extraction_IsAccelEnabled(boolean_T *rty_y)
{
    /* SignalConversion: '<S16>/TmpSignal ConversionAtyInport1' incorporates:
     *  Logic: '<S16>/Logical Operator'
     *  SignalConversion: '<S16>/TmpLatchAtExtr_Sup_CabHitEnabledOutport1'
     *  SignalConversion: '<S16>/TmpLatchAtExtr_Sup_VibMapEnabledOutport1'
     */
    *rty_y = Extraction_B.OutportBufferForExtr_Sup_VibMapEnabled ||
        Extraction_B.OutportBufferForExtr_Sup_CabHitEnabled;
}

/* System initialize for Simulink Function: '<S2>/UpdateParamEstSetup' */
void Extraction_UpdateParamEstSetup_Init(void)
{
    /* SystemInitialize for Atomic SubSystem: '<S25>/SetParamEstSetup' */
    /* InitializeConditions for Memory: '<S253>/PrevSetupIndex' */
    Extraction_DW.PrevSetupIndex_PreviousInput = MAX_uint8_T;

    /* End of SystemInitialize for SubSystem: '<S25>/SetParamEstSetup' */
}

/* Output and update for Simulink Function: '<S2>/UpdateParamEstSetup' */
void Extraction_UpdateParamEstSetup(uint8 rtu_index)
{
    /* MATLAB Function: '<S254>/GetParamEstSetup' incorporates:
     *  SignalConversion: '<S25>/TmpSignal ConversionAtindexOutport1'
     */
    /* MATLAB Function 'Extraction/UpdateParamEstSetup/GetParamEstSetup/CodeGen/GetParamEstSetup': '<S256>:1' */
    /* '<S256>:1:5' */
    Extraction_B.ParamEstSetup = ExtractionParams__GetParamEstSetup(rtu_index);

    /* MATLAB Function: '<S25>/GetParamEstExcSetup' incorporates:
     *  SignalConversion: '<S25>/TmpSignal ConversionAtindexOutport1'
     */
    /* MATLAB Function 'Extraction/UpdateParamEstSetup/GetParamEstExcSetup': '<S250>:1' */
    /* '<S250>:1:5' */
    Extraction_B.ParamEstExcSetup = ExtractionParams__GetParamEstExcSetup
        (rtu_index);

    /* Outputs for Atomic SubSystem: '<S25>/SetParamEstSetup' */
    /* If: '<S253>/If' incorporates:
     *  DataStoreRead: '<S253>/Data Store Read'
     *  Logic: '<S253>/Logical Operator'
     *  Memory: '<S253>/PrevSetupIndex'
     *  RelationalOperator: '<S253>/Relational Operator'
     *  SignalConversion: '<S25>/TmpSignal ConversionAtindexOutport1'
     */
    if (ExtractionParams__IsExtrConfigValid() != 0 && rtu_index !=
            Extraction_DW.PrevSetupIndex_PreviousInput)
    {
        /* Outputs for IfAction SubSystem: '<S253>/If Action Subsystem' incorporates:
         *  ActionPort: '<S257>/Action Port'
         */
        /* S-Function (fcncallgen): '<S257>/Function-Call Generator' incorporates:
         *  SubSystem: '<S257>/StopPe'
         */
        /* FunctionCaller: '<S259>/Function Caller' incorporates:
         *  Constant: '<S259>/PeStopConst'
         */
        Extraction_ParamEstControl(PE_STOP);

        /* S-Function (fcncallgen): '<S257>/Function-Call Generator' incorporates:
         *  SubSystem: '<S257>/SetPeSetup'
         */
        /* BusCreator: '<S261>/Bus Creator' incorporates:
         *  Constant: '<S261>/Constant1'
         *  Constant: '<S261>/Constant2'
         *  Constant: '<S261>/Constant3'
         *  Constant: '<S261>/Constant4'
         *  Constant: '<S261>/Constant5'
         */
        Extraction_B.BusCreator_dv.Torque = 0.0F;
        Extraction_B.BusCreator_dv.Speed = 0.0F;
        Extraction_B.BusCreator_dv.Sin_Theta = 0.0F;
        Extraction_B.BusCreator_dv.Cos_Theta = 0.0F;
        Extraction_B.BusCreator_dv.Speed_Ref = 0.0F;

        /* DataTypeConversion: '<S261>/Data Type Conversion' incorporates:
         *  Constant: '<S263>/Constant'
         */
        Extraction_B.DataTypeConversion_pt = (uint8)PE_SETUP;

        /* S-Function (ParamEst_sfun): '<S264>/ParamEst_sfun' incorporates:
         *  Constant: '<S261>/Constant'
         *  Constant: '<S261>/Constant6'
         */
        ParamEst_sfun_Outputs_wrapper(&Extraction_B.DataTypeConversion_pt,
            &Extraction_B.ParamEstSetup, &rtCP_Constant_Value_cy,
            rtCP_Constant6_Value_oz, &Extraction_B.BusCreator_dv.Speed,
            &Extraction_B.BusCreator_dv.Sin_Theta,
            &Extraction_B.BusCreator_dv.Cos_Theta,
            &Extraction_B.BusCreator_dv.Torque, &Extraction_B.Inertia_cz,
            &Extraction_B.UB_ne, &Extraction_B.UB_Angle_b1,
            &Extraction_B.Viscous_Friction_lc, &Extraction_B.Coulomb_Friction_p2,
            &Extraction_B.Friction_py, &Extraction_B.BB_mgr_o1,
            &Extraction_B.BB_PhaseDiff_g1);

        /* DataTypeConversion: '<S260>/Data Type Conversion' incorporates:
         *  Constant: '<S262>/Constant'
         */
        Extraction_B.DataTypeConversion_gr = (uint8)EXC_SETUP;

        /* S-Function (ParamEstExc_sfun): '<S260>/ParamEstExc_sfun' */
        ParamEstExc_sfun_Outputs_wrapper(&Extraction_B.ParamEstExcSetup,
            &Extraction_B.DataTypeConversion_gr, (float32*)&Extraction_R32GND,
            &Extraction_B.ParamEstExc_sfun_o1_kx,
            &Extraction_B.ParamEstExc_sfun_o2_lq);

        /* End of Outputs for S-Function (fcncallgen): '<S257>/Function-Call Generator' */
        /* End of Outputs for SubSystem: '<S253>/If Action Subsystem' */
    }

    /* End of If: '<S253>/If' */

    /* Update for Memory: '<S253>/PrevSetupIndex' incorporates:
     *  SignalConversion: '<S25>/TmpSignal ConversionAtindexOutport1'
     */
    Extraction_DW.PrevSetupIndex_PreviousInput = rtu_index;

    /* End of Outputs for SubSystem: '<S25>/SetParamEstSetup' */
}

/* Output and update for Simulink Function: '<S2>/IsMotorRunning' */
void Extraction_IsMotorRunning(boolean_T *rty_motorRunningFlag)
{
    uint8 y;

    /* MATLAB Function: '<S17>/IsMotorRunningMacro' */
    /* MATLAB Function 'Extraction/IsMotorRunning/IsMotorRunningMacro': '<S240>:1' */
    // Check if the MCI module is in idle state	();
    /* '<S240>:1:8' */
    y = EXTRACTION_IS_MOTOR_RUNNING(); //();

    /* SignalConversion: '<S17>/TmpSignal ConversionAtyInport1' incorporates:
     *  Abs: '<S17>/Abs'
     *  Constant: '<S239>/Constant'
     *  Logic: '<S17>/Logical Operator'
     *  MATLAB Function: '<S17>/IsMotorRunningMacro'
     *  RelationalOperator: '<S239>/Compare'
     *  SignalConversion: '<S17>/TmpLatchAtMotorSpeedOutport1'
     */
    *rty_motorRunningFlag = y != 0 && fabsf(Extr_Inputs_MotorSignals.Speed) >
        0.0F;
}

/* Output and update for Simulink Function: '<S2>/ParamEstControl' */
void Extraction_ParamEstControl(PARAM_EST_COMMANDS rtu_command)
{
    /* BusCreator: '<S18>/Bus Creator' incorporates:
     *  Constant: '<S18>/Constant1'
     *  Constant: '<S18>/Constant2'
     *  Constant: '<S18>/Constant3'
     *  Constant: '<S18>/Constant4'
     *  Constant: '<S18>/Constant5'
     */
    Extraction_B.BusCreator_pp.Torque = 0.0F;
    Extraction_B.BusCreator_pp.Speed = 0.0F;
    Extraction_B.BusCreator_pp.Sin_Theta = 0.0F;
    Extraction_B.BusCreator_pp.Cos_Theta = 0.0F;
    Extraction_B.BusCreator_pp.Speed_Ref = 0.0F;

    /* DataTypeConversion: '<S18>/Data Type Conversion' incorporates:
     *  SignalConversion: '<S18>/TmpSignal ConversionAtcommandOutport1'
     */
    Extraction_B.DataTypeConversion_ii = (uint8)rtu_command;

    /* DataTypeConversion: '<S18>/Data Type Conversion1' incorporates:
     *  Constant: '<S18>/Constant'
     */
    Extraction_B.DataTypeConversion1_ei = 0U;

    /* S-Function (ParamEst_sfun): '<S241>/ParamEst_sfun' incorporates:
     *  Constant: '<S18>/Constant6'
     *  Constant: '<S18>/NullPeSetup'
     */
    ParamEst_sfun_Outputs_wrapper(&Extraction_B.DataTypeConversion_ii,
        &rtCP_NullPeSetup_Value, &Extraction_B.DataTypeConversion1_ei,
        rtCP_Constant6_Value_pe, &Extraction_B.BusCreator_pp.Speed,
        &Extraction_B.BusCreator_pp.Sin_Theta,
        &Extraction_B.BusCreator_pp.Cos_Theta,
        &Extraction_B.BusCreator_pp.Torque, &Extraction_B.Inertia_fr,
        &Extraction_B.UB_mc, &Extraction_B.UB_Angle_bf,
        &Extraction_B.Viscous_Friction_cp, &Extraction_B.Coulomb_Friction_pq,
        &Extraction_B.Friction_bt, &Extraction_B.BB_mgr_nw,
        &Extraction_B.BB_PhaseDiff_m5);
}

/* Output and update for Simulink Function: '<S2>/ParamEstExcControl' */
void Extraction_ParamEstExcControl(PARAM_EST_EXC_COMMANDS rtu_command)
{
    /* DataTypeConversion: '<S19>/Data Type Conversion' incorporates:
     *  SignalConversion: '<S19>/TmpSignal ConversionAtcommandOutport1'
     */
    Extraction_B.DataTypeConversion_gf = (uint8)rtu_command;

    /* S-Function (ParamEstExc_sfun): '<S242>/ParamEstExc_sfun' incorporates:
     *  Constant: '<S19>/Constant'
     */
    ParamEstExc_sfun_Outputs_wrapper(&rtCP_Constant_Value,
        &Extraction_B.DataTypeConversion_gf, (float32*)&Extraction_R32GND,
        &Extraction_B.ParamEstExc_sfun_o1, &Extraction_B.ParamEstExc_sfun_o2);
}

/* Output and update for Simulink Function: '<S2>/ReleaseMotion' */
void Extraction_ReleaseMotion(void)
{
    /* MATLAB Function: '<S20>/ReleaseMotionMacro' */
    /* MATLAB Function 'CreateFunctionMacro': '<S243>:1' */
    // Release the motion module	();
    /* '<S243>:1:8' */
    EXTRACTION_RELEASE_MOTION();       //();
}

/* Output and update for Simulink Function: '<S2>/SelectParamEstOutputs' */
void Extraction_SelectParamEstOutputs(const uint8 rtu_output_selector[8])
{
    /* BusCreator: '<S21>/Bus Creator' incorporates:
     *  Constant: '<S21>/Constant1'
     *  Constant: '<S21>/Constant2'
     *  Constant: '<S21>/Constant3'
     *  Constant: '<S21>/Constant4'
     *  Constant: '<S21>/Constant5'
     */
    Extraction_B.BusCreator.Torque = 0.0F;
    Extraction_B.BusCreator.Speed = 0.0F;
    Extraction_B.BusCreator.Sin_Theta = 0.0F;
    Extraction_B.BusCreator.Cos_Theta = 0.0F;
    Extraction_B.BusCreator.Speed_Ref = 0.0F;

    /* DataTypeConversion: '<S21>/Data Type Conversion' incorporates:
     *  Constant: '<S244>/Constant'
     */
    Extraction_B.DataTypeConversion = (uint8)PE_SELECT_OUTPUTS;

    /* DataTypeConversion: '<S21>/Data Type Conversion1' incorporates:
     *  Constant: '<S21>/Constant'
     */
    Extraction_B.DataTypeConversion1 = 0U;

    /* SignalConversion: '<S21>/TmpSignal ConversionAtoutput_selectorOutport1' */
    Extraction_B.TmpSignalConversionAtoutput_selectorOutport1[0] =
        rtu_output_selector[0];
    Extraction_B.TmpSignalConversionAtoutput_selectorOutport1[1] =
        rtu_output_selector[1];
    Extraction_B.TmpSignalConversionAtoutput_selectorOutport1[2] =
        rtu_output_selector[2];
    Extraction_B.TmpSignalConversionAtoutput_selectorOutport1[3] =
        rtu_output_selector[3];
    Extraction_B.TmpSignalConversionAtoutput_selectorOutport1[4] =
        rtu_output_selector[4];
    Extraction_B.TmpSignalConversionAtoutput_selectorOutport1[5] =
        rtu_output_selector[5];
    Extraction_B.TmpSignalConversionAtoutput_selectorOutport1[6] =
        rtu_output_selector[6];
    Extraction_B.TmpSignalConversionAtoutput_selectorOutport1[7] =
        rtu_output_selector[7];

    /* S-Function (ParamEst_sfun): '<S245>/ParamEst_sfun' incorporates:
     *  Constant: '<S21>/Constant6'
     */
    ParamEst_sfun_Outputs_wrapper(&Extraction_B.DataTypeConversion,
        &rtCP_Constant6_Value, &Extraction_B.DataTypeConversion1,
        &Extraction_B.TmpSignalConversionAtoutput_selectorOutport1[0],
        &Extraction_B.BusCreator.Speed, &Extraction_B.BusCreator.Sin_Theta,
        &Extraction_B.BusCreator.Cos_Theta, &Extraction_B.BusCreator.Torque,
        &Extraction_B.Inertia, &Extraction_B.UB, &Extraction_B.UB_Angle,
        &Extraction_B.Viscous_Friction, &Extraction_B.Coulomb_Friction,
        &Extraction_B.Friction, &Extraction_B.BB_mgr, &Extraction_B.BB_PhaseDiff);
}

/* Output and update for Simulink Function: '<S2>/SetFault' */
void Extraction_SetFault(EXTR_FAULT rtu_fault)
{
    uint32 Extr_Sup_Fault_0;

    /* DataStoreWrite: '<S22>/ExtrFaultOut' */
    Extr_Sup_Fault_0 = Extr_Sup_Fault;

    /* S-Function (sfix_bitop): '<S22>/OR' incorporates:
     *  ArithShift: '<S22>/LeftShift'
     *  Constant: '<S22>/ConstOne'
     *  SignalConversion: '<S22>/TmpSignal ConversionAtfaultOutport1'
     */
    Extr_Sup_Fault_0 |= 1U << rtu_fault;

    /* DataStoreWrite: '<S22>/ExtrFaultOut' */
    Extr_Sup_Fault = Extr_Sup_Fault_0;
}

/* Output and update for Simulink Function: '<S23>/StopMotorCommand' */
void Extraction_StopMotorCommand(sint16 rtu_decel)
{
    MOTION_PROFILE_TYPE b;

    /* MATLAB Function: '<S248>/SpinCommand' incorporates:
     *  SignalConversion: '<S248>/TmpSignal ConversionAtdecelOutport1'
     */
    /* MATLAB Function 'Extraction/SetSpeedCommand/StopMotorCommand/SpinCommand': '<S249>:1' */
    /* '<S249>:1:6' */
    b.Stop.Drum_Deceleration = rtu_decel;

    /* '<S249>:1:9' */
    EXTRACTION_STOP_MOTOR(b);
}

/* Output and update for Simulink Function: '<S2>/SetSpeedCommand' */
void Extraction_SetSpeedCommand(sint16 rtu_speed, sint16 rtu_ramp)
{
    MOTION_PROFILE_TYPE b;
    sint16 tmp;
    sint16 tmp_0;
    sint16 spd_command_idx_0;
    sint16 spd_command_idx_1;
    sint32 tmp_1;
    uint8 Extr_Sup_SpdComCounter_0;

    /* DataStoreWrite: '<S23>/SpdComCounterWrite' */
    Extr_Sup_SpdComCounter_0 = Extr_Sup_SpdComCounter;

    /* Sum: '<S246>/FixPt Sum1' incorporates:
     *  Constant: '<S246>/FixPt Constant'
     */
    Extraction_B.FixPtSum1 = (uint8)(Extr_Sup_SpdComCounter_0 + 1U);

    /* MATLAB Function: '<S23>/SpinCommand' incorporates:
     *  SignalConversion: '<S23>/TmpSignal ConversionAtrampOutport1'
     *  SignalConversion: '<S23>/TmpSignal ConversionAtspeedOutport1'
     */
    Extr_Sup_SpdCommand[0] = rtu_speed;
    Extr_Sup_SpdCommand[1] = rtu_ramp;

    /* MATLAB Function 'Extraction/SetSpeedCommand/SpinCommand': '<S247>:1' */
    /* '<S247>:1:1' */
    spd_command_idx_0 = Extr_Sup_SpdCommand[0];
    spd_command_idx_1 = Extr_Sup_SpdCommand[1];
    if (Extr_Sup_SpdCommand[0] < 0)
    {
        tmp_1 = -Extr_Sup_SpdCommand[0];
        if (tmp_1 > 32767)
        {
            tmp_1 = 32767;
        }

        tmp = (sint16)tmp_1;
    }
    else
    {
        tmp = Extr_Sup_SpdCommand[0];
    }

    if (((sint16)EXTR_MIN_SPEED) < 0)
    {
        tmp_1 = -((sint16)EXTR_MIN_SPEED);
        if (tmp_1 > 32767)
        {
            tmp_1 = 32767;
        }

        tmp_0 = (sint16)tmp_1;
    }
    else
    {
        tmp_0 = ((sint16)EXTR_MIN_SPEED);
    }

    if (tmp < tmp_0)
    {
        /* '<S247>:1:6' */
        /* '<S247>:1:7' */
        spd_command_idx_0 = 0;
    }
    else
    {
        if (Extr_Sup_SpdCommand[0] < 0)
        {
            tmp_1 = -Extr_Sup_SpdCommand[0];
            if (tmp_1 > 32767)
            {
                tmp_1 = 32767;
            }

            tmp = (sint16)tmp_1;
        }
        else
        {
            tmp = Extr_Sup_SpdCommand[0];
        }

        if (((sint16)EXTR_MAX_SPEED) < 0)
        {
            tmp_1 = -((sint16)EXTR_MAX_SPEED);
            if (tmp_1 > 32767)
            {
                tmp_1 = 32767;
            }

            tmp_0 = (sint16)tmp_1;
        }
        else
        {
            tmp_0 = ((sint16)EXTR_MAX_SPEED);
        }

        if (tmp > tmp_0)
        {
            /* '<S247>:1:9' */
            /* '<S247>:1:10' */
            if (Extr_Sup_SpdCommand[0] > 0)
            {
                tmp = 1;
            }
            else if (Extr_Sup_SpdCommand[0] < 0)
            {
                tmp = -1;
            }
            else
            {
                tmp = 0;
            }

            tmp_1 = ((sint16)EXTR_MAX_SPEED) * tmp;
            if (tmp_1 > 32767)
            {
                tmp_1 = 32767;
            }

            spd_command_idx_0 = (sint16)tmp_1;
        }
    }

    if (Extr_Sup_SpdCommand[1] < 0)
    {
        tmp_1 = -Extr_Sup_SpdCommand[1];
        if (tmp_1 > 32767)
        {
            tmp_1 = 32767;
        }

        tmp = (sint16)tmp_1;
    }
    else
    {
        tmp = Extr_Sup_SpdCommand[1];
    }

    if (tmp < 2)
    {
        /* '<S247>:1:14' */
        /* '<S247>:1:15' */
        spd_command_idx_1 = 2;
    }
    else
    {
        if (Extr_Sup_SpdCommand[1] < 0)
        {
            tmp_1 = -Extr_Sup_SpdCommand[1];
            if (tmp_1 > 32767)
            {
                tmp_1 = 32767;
            }

            tmp = (sint16)tmp_1;
        }
        else
        {
            tmp = Extr_Sup_SpdCommand[1];
        }

        if (((sint16)MAX_RAMP_ACCEL) < 0)
        {
            tmp_1 = -((sint16)MAX_RAMP_ACCEL);
            if (tmp_1 > 32767)
            {
                tmp_1 = 32767;
            }

            tmp_0 = (sint16)tmp_1;
        }
        else
        {
            tmp_0 = ((sint16)MAX_RAMP_ACCEL);
        }

        if (tmp > tmp_0)
        {
            /* '<S247>:1:17' */
            /* '<S247>:1:18' */
            if (((sint16)MAX_RAMP_ACCEL) < 0)
            {
                tmp_1 = -((sint16)MAX_RAMP_ACCEL);
                if (tmp_1 > 32767)
                {
                    tmp_1 = 32767;
                }

                spd_command_idx_1 = (sint16)tmp_1;
            }
            else
            {
                spd_command_idx_1 = ((sint16)MAX_RAMP_ACCEL);
            }
        }
    }

    if (spd_command_idx_0 != 0)
    {
        /* '<S247>:1:22' */
        /* '<S247>:1:23' */
        tmp = spd_command_idx_1;
        if (spd_command_idx_1 < 0)
        {
            tmp = 0;
        }

        b.Spin.Drum_Acceleration = (uint16)tmp;
        b.Spin.Target_Drum_Velocity = spd_command_idx_0;

        /* '<S247>:1:26' */
        EXTRACTION_SPIN_COMMAND(b);
    }
    else
    {
        /* '<S247>:1:31' */
        Extraction_StopMotorCommand(spd_command_idx_1);
    }

    Extr_Sup_SpdCommand[0] = spd_command_idx_0;
    Extr_Sup_SpdCommand[1] = spd_command_idx_1;

    /* End of MATLAB Function: '<S23>/SpinCommand' */
    Extr_Sup_SpdComCounter_0 = Extraction_B.FixPtSum1;

    /* DataStoreWrite: '<S23>/SpdComCounterWrite' */
    Extr_Sup_SpdComCounter = Extr_Sup_SpdComCounter_0;
}

/* Output and update for Simulink Function: '<S2>/StopMotor' */
void Extraction_StopMotor(void)
{
    /* FunctionCaller: '<S24>/Function Caller' incorporates:
     *  Constant: '<S24>/ramp'
     *  Constant: '<S24>/speed'
     */
    Extraction_SetSpeedCommand(0, 100);
}

/* Output and update for function-call system: '<S2>/Extr_UpdateInputs' */
void Extraction_Extr_UpdateInputs(void)
{
    float32 y;
    float32 y_0;
    float32 y_1;
    float32 y_2;
    float32 y_3;
    float32 y_4;
    float32 y_5;
    float32 y_6;

    /* Outputs for Atomic SubSystem: '<S14>/GetPEOutput' */
    /* MATLAB Function: '<S221>/GetInertia' */
    /* MATLAB Function 'CreateParameterMacro': '<S234>:1' */
    // Estimated inertia input	();
    /* '<S234>:1:7' */
    y = EXTRACTION_INERTIA_INPUT;      //();

    /* MATLAB Function: '<S221>/GetUB' */
    /* MATLAB Function 'CreateParameterMacro': '<S235>:1' */
    // Estimated unbalance input	();
    /* '<S235>:1:7' */
    y_0 = EXTRACTION_UB_INPUT;         //();

    /* MATLAB Function: '<S221>/GetUBAngle' */
    /* MATLAB Function 'CreateParameterMacro': '<S236>:1' */
    // Estimated ub angle input	();
    /* '<S236>:1:7' */
    y_1 = EXTRACTION_UB_ANGLE_INPUT;   //();

    /* MATLAB Function: '<S221>/GetViscousFriction' */
    /* MATLAB Function 'CreateParameterMacro': '<S237>:1' */
    // Estimated viscous friction input	();
    /* '<S237>:1:7' */
    y_2 = EXTRACTION_VISCOUS_INPUT;    //();

    /* MATLAB Function: '<S221>/GetCoulombFriction' */
    /* MATLAB Function 'CreateParameterMacro': '<S232>:1' */
    // Estimated coulomb friction input	();
    /* '<S232>:1:7' */
    y_3 = EXTRACTION_COULOMB_INPUT;    //();

    /* MATLAB Function: '<S221>/GetFriction' */
    /* MATLAB Function 'CreateParameterMacro': '<S233>:1' */
    // Estimated friction input	();
    /* '<S233>:1:7' */
    y_4 = EXTRACTION_FRICTION_INPUT;   //();

    /* MATLAB Function: '<S221>/GetBBMag' */
    /* MATLAB Function 'CreateParameterMacro': '<S230>:1' */
    // Estimated ball balancer magnitude input	();
    /* '<S230>:1:7' */
    y_5 = EXTRACTION_BB_MAG_INPUT;     //();

    /* MATLAB Function: '<S221>/GetBBPhaseDiff' */
    /* MATLAB Function 'CreateParameterMacro': '<S231>:1' */
    // Estimated bb position related to load unbalance	();
    /* '<S231>:1:7' */
    y_6 = EXTRACTION_BB_PHASE_DIFF_INPUT;//();

    /* BusCreator: '<S221>/Bus Creator' incorporates:
     *  MATLAB Function: '<S221>/GetBBMag'
     *  MATLAB Function: '<S221>/GetBBPhaseDiff'
     *  MATLAB Function: '<S221>/GetCoulombFriction'
     *  MATLAB Function: '<S221>/GetFriction'
     *  MATLAB Function: '<S221>/GetInertia'
     *  MATLAB Function: '<S221>/GetUB'
     *  MATLAB Function: '<S221>/GetUBAngle'
     *  MATLAB Function: '<S221>/GetViscousFriction'
     */
    Extr_Inputs_ParamEstData.Inertia = y;
    Extr_Inputs_ParamEstData.UB = y_0;
    Extr_Inputs_ParamEstData.UB_Angle = y_1;
    Extr_Inputs_ParamEstData.Viscous_Friction = y_2;
    Extr_Inputs_ParamEstData.Coulomb_Friction = y_3;
    Extr_Inputs_ParamEstData.Friction = y_4;
    Extr_Inputs_ParamEstData.BB_mgr = y_5;
    Extr_Inputs_ParamEstData.BB_PhaseLag = y_6;

    /* End of Outputs for SubSystem: '<S14>/GetPEOutput' */

    /* MATLAB Function: '<S14>/GetDistUtilsOutput' */
    /* MATLAB Function 'Extraction/Extr_UpdateInputs/GetDistUtilsOutput': '<S213>:1' */
    // Ouptut from DistUtils module.	();
    /* '<S213>:1:6' */
    Extr_Inputs_DistUtilsData = GET_DISTRIBUTION_UTILS_OUTPUT;//();

    /* Outputs for Atomic SubSystem: '<S14>/GetMCIOutputs' */
    /* MATLAB Function: '<S216>/GetTorque' */
    /* MATLAB Function 'CreateParameterMacro': '<S229>:1' */
    // Estimated torque input	();
    /* '<S229>:1:7' */
    y = EXTRACTION_TORQUE_INPUT;       //();

    /* MATLAB Function: '<S216>/GetSpeed' */
    /* MATLAB Function 'CreateParameterMacro': '<S227>:1' */
    // Estimated speed input	();
    /* '<S227>:1:7' */
    y_0 = EXTRACTION_SPEED_INPUT;      //();

    /* MATLAB Function: '<S216>/GetSinTheta' */
    /* MATLAB Function 'CreateParameterMacro': '<S226>:1' */
    // Sine of estimated rotor position	();
    /* '<S226>:1:7' */
    y_1 = EXTRACTION_SIN_THETA_INPUT;  //();

    /* MATLAB Function: '<S216>/GetCosTheta' */
    /* MATLAB Function 'CreateParameterMacro': '<S225>:1' */
    // Cosine of estimated rotor position	();
    /* '<S225>:1:7' */
    y_2 = EXTRACTION_COS_THETA_INPUT;  //();

    /* MATLAB Function: '<S216>/GetSpeedRef' */
    /* MATLAB Function 'CreateParameterMacro': '<S228>:1' */
    // Speed reference input	();
    /* '<S228>:1:7' */
    y_3 = EXTRACTION_SPEED_REF_INPUT;  //();

    /* BusCreator: '<S216>/Bus Creator' incorporates:
     *  MATLAB Function: '<S216>/GetCosTheta'
     *  MATLAB Function: '<S216>/GetSinTheta'
     *  MATLAB Function: '<S216>/GetSpeed'
     *  MATLAB Function: '<S216>/GetSpeedRef'
     *  MATLAB Function: '<S216>/GetTorque'
     */
    Extr_Inputs_MotorSignals.Torque = y;
    Extr_Inputs_MotorSignals.Speed = y_0;
    Extr_Inputs_MotorSignals.Sin_Theta = y_1;
    Extr_Inputs_MotorSignals.Cos_Theta = y_2;
    Extr_Inputs_MotorSignals.Speed_Ref = y_3;

    /* End of Outputs for SubSystem: '<S14>/GetMCIOutputs' */

    /* MATLAB Function: '<S14>/GetSaturatedLoadMass' */
    /* MATLAB Function 'CreateParameterMacro': '<S223>:1' */
    // Saturated load mass	();
    /* '<S223>:1:7' */
    Extr_Inputs_AcuLoadMass = EXTRACTION_SATURATED_LOAD_MASS_INPUT;//();

    /* MATLAB Function: '<S14>/GetWaterTemp' */
    /* MATLAB Function 'CreateParameterMacro': '<S224>:1' */
    // Water temperature	();
    /* '<S224>:1:7' */
    Extr_Inputs_WaterTemp = EXTRACTION_WATER_TEMP_INPUT;//();

    /* MATLAB Function: '<S14>/GetFinalSpeed' */
    /* MATLAB Function 'CreateParameterMacro': '<S215>:1' */
    // Final target speed	();
    /* '<S215>:1:7' */
    Extr_Inputs_MaxSpeed = EXTRACTION_FINAL_SPEED;//();

    /* MATLAB Function: '<S14>/GetMinSpeed' */
    /* MATLAB Function 'CreateParameterMacro': '<S220>:1' */
    // Minimum speed to be reached	();
    /* '<S220>:1:7' */
    Extr_Inputs_MinFinalSpeed = EXTRACTION_MIN_FINAL_SPEED;//();

    /* MATLAB Function: '<S14>/GetExtPlateauDuration' */
    /* MATLAB Function 'CreateParameterMacro': '<S214>:1' */
    // Final Plateau Duration in seconds	();
    /* '<S214>:1:7' */
    Extr_Inputs_ExtPlateauDuration = EXTRACTION_FINAL_PLATEAU_DURATION;//();

    /* MATLAB Function: '<S14>/GetMciRawUnbalance' */
    /* MATLAB Function 'CreateParameterMacro': '<S218>:1' */
    // Unbalance measurement from Indesit legacy logic	();
    /* '<S218>:1:7' */
    Extr_Inputs_UnbalanceIndex = EXTRACTION_UNBALANCE_INDEX_INPUT;//();

    /* MATLAB Function: '<S14>/GetMaxSpeedError' */
    /* MATLAB Function 'CreateParameterMacro': '<S217>:1' */
    // Maximum speed error percentage	();
    /* '<S217>:1:7' */
    Extr_Inputs_MaxSpeedError = EXTRACTION_MAX_SPEED_ERROR;//();

    /* MATLAB Function: '<S14>/GetRampTimeout' */
    /* MATLAB Function 'CreateParameterMacro': '<S222>:1' */
    // Ramp timeout in seconds	();
    /* '<S222>:1:7' */
    Extr_Inputs_RampTimeout = EXTRACTION_RAMP_TIMEOUT;//();

    /* MATLAB Function: '<S14>/GetMciRawUnbalanceCount' */
    /* MATLAB Function 'CreateParameterMacro': '<S219>:1' */
    // Number of unbalance measurements taken in the current speed plateau.	();
    /* '<S219>:1:7' */
    Extr_Inputs_UnbalanceIndexCnt = EXTRACTION_UNBALANCE_SAMPLE_COUNT_INPUT;//();
}

/*
 * Output and update for atomic system:
 *    '<S204>/TriggerGen'
 *    '<S174>/TriggerGen'
 *    '<S130>/TriggerGen'
 *    '<S88>/TriggerGen'
 *    '<S68>/TriggerGen'
 *    '<S184>/TriggerGen'
 *    '<S27>/TriggerGen'
 */
void Extraction_TriggerGen(Extraction_B_TriggerGen_TYPE *localB,
    Extraction_DW_TriggerGen_TYPE *localDW)
{
    uint32 triggerEventCounter;
    triggerEventCounter = localDW->triggerEventCounter;

    /* Gateway: TriggerGen/TriggerGen */
    /* During: TriggerGen/TriggerGen */
    /* Entry Internal: TriggerGen/TriggerGen */
    /* Transition: '<S207>:2' */
    /* Event: '<S207>:3' */
    triggerEventCounter++;
    if (triggerEventCounter > 0U)
    {
        localB->trigger = !localB->trigger;
        triggerEventCounter--;
    }

    localDW->triggerEventCounter = triggerEventCounter;
}

/* Output and update for Simulink Function: '<S193>/ResetInitialPulseConfig' */
void Extraction_ResetInitialPulseConfig(void)
{
    /* StateWriter: '<S203>/State Writer' incorporates:
     *  Constant: '<S203>/Constant'
     */
    Extraction_DW.PrevSpinPulse_DSTATE = Extraction_rtZSPIN_PULSE_TYPE;

    /* Outputs for Atomic SubSystem: '<S203>/ResetInitialPulseConfigStates' */
#if EXTR_VARIANT_HIGH_SPEED_PE_ENABLED

    /* Output and update for atomic system: '<S205>/ResetInitialPulseConfig_HighSpeedParamEstEnabled' */

    /* StateWriter: '<S206>/State Writer2' incorporates:
     *  Constant: '<S206>/Constant1'
     */
    Extraction_DW.UpdateSpinPulseConfig_Enabled.PrevPulseConfig_DSTATE =
        Extraction_rtZHIGH_SPEED_PULSE_CONFIG_TYPE;

    /* StateWriter: '<S206>/State Writer3' incorporates:
     *  Constant: '<S206>/Constant2'
     */
    Extraction_DW.UpdateSpinPulseConfig_Enabled.PrevLoadMassLimitConfig_DSTATE =
        Extraction_rtZLOAD_MASS_LIMIT_SETUP_TYPE;

    /* StateWriter: '<S206>/State Writer1' incorporates:
     *  Constant: '<S206>/Constant3'
     */
    Extraction_DW.UpdateSpinPulseConfig_Enabled.PrevBaselineFriction_DSTATE =
        0.0F;

#endif                                 /* EXTR_VARIANT_HIGH_SPEED_PE_ENABLED */

    /* End of Outputs for SubSystem: '<S203>/ResetInitialPulseConfigStates' */
}

/* Output and update for function-call system: '<S122>/UpdatePEAppSpecificSetup' */
void Extraction_UpdatePEAppSpecificSetup
    (Extraction_B_UpdatePEAppSpecificSetup_TYPE *localB)
{
    /* MATLAB Function: '<S175>/GetPEAppSpecificConfig' incorporates:
     *  DataStoreRead: '<S175>/DataStoreRead4'
     */
    /* MATLAB Function 'Extraction/Extr_LowSpd/Extr_LowSpd_Supervisor/UpdatePEAppSpecificSetup/GetPEAppSpecificConfig': '<S179>:1' */
    /* '<S179>:1:5' */
    localB->PEAppSpecificConfig = ExtractionParams__GetParamEstAppSpecificSetup
        ((*GESE_Extr_ParamEstLowSpeedSetup).Param_Est_App_Specific_Setup_Index);
}

/* Output and update for Simulink Function: '<S122>/LoadDistributionSpeed' */
void Extraction_LoadDistributionSpeed(sint16 *rty_speed)
{
    boolean_T pass;

    /* MATLAB Function: '<S172>/LoadDistributionSpeedMATLAB' */
    /* MATLAB Function 'Extraction/Extr_LowSpd/Extr_LowSpd_Supervisor/LoadDistributionSpeed/LoadDistributionSpeedMATLAB': '<S176>:1' */
    /* '<S176>:1:8' */
    *rty_speed = 0;

    /* '<S176>:1:12' */
    pass = ExtractionParams__LoadDistributionProfile((*GESE_Extr_Setup).
        Distribution_Profile_Index);
    if (pass)
    {
        /* '<S176>:1:23' */
        /* '<S176>:1:24' */
        *rty_speed = (sint16)(GESE_Extr_DistProf[GESE_Extr_DistProfPulseCnt - 1]
                              .Target_Speed_Div2 * 2);
    }

    /* End of MATLAB Function: '<S172>/LoadDistributionSpeedMATLAB' */
}

/* Output and update for Simulink Function: '<S122>/RetrieveMaxTargetSpeed' */
void Extraction_RetrieveMaxTargetSpeed(void)
{
    uint8 ii;
    uint8 rtb_max_speed;

    /* MATLAB Function: '<S173>/RetrieveMaxTargetSpeedMATLAB' */
    /* MATLAB Function 'Extraction/Extr_LowSpd/Extr_LowSpd_Supervisor/RetrieveMaxTargetSpeed/RetrieveMaxTargetSpeedMATLAB': '<S177>:1' */
    /* '<S177>:1:6' */
    rtb_max_speed = 0U;

    /* '<S177>:1:8' */
    for (ii = 1U; ii <= (*GESE_Extr_SpinProfileConfig).Spin_Pulse_Count; ii++)
    {
        /* '<S177>:1:8' */
        if (GESE_Extr_SpinPulses[ii - 1].Target_Speed_Div10 > rtb_max_speed)
        {
            /* '<S177>:1:9' */
            /* '<S177>:1:10' */
            rtb_max_speed = GESE_Extr_SpinPulses[ii - 1].Target_Speed_Div10;
        }
    }

    /* End of MATLAB Function: '<S173>/RetrieveMaxTargetSpeedMATLAB' */

    /* Gain: '<S173>/x10' */
    Extraction_B.x10 = (uint16)(10U * rtb_max_speed);
}

/* Output and update for Simulink Function: '<S140>/CalculateUnbalanceTarget' */
void Extraction_CalculateUnbalanceTarget(uint8 rtu_RowIndex, uint16
    *rty_UnbTarget)
{
    PRE_LOOKUP_OUTPUT_TYPE rtb_LoadMassIndex;
    PRE_LOOKUP_OUTPUT_TYPE rtb_BusCreator;

    /* SignalConversion: '<S142>/TmpLatchAtLoadMassIndexOutport1' */
    rtb_LoadMassIndex = Extraction_B.LoadMassIndex;

    /* Outputs for Atomic SubSystem: '<S142>/CalculateUnbalanceTargetSubsystem' */
    /* BusCreator: '<S144>/Bus Creator' incorporates:
     *  Constant: '<S144>/Constant'
     *  SignalConversion: '<S142>/TmpSignal ConversionAtRowIndexOutport1'
     */
    rtb_BusCreator.k = rtu_RowIndex;
    rtb_BusCreator.f = 0.0F;

    /* FunctionCaller: '<S144>/FunctionCaller_FinalSpeedToUnb' incorporates:
     *  SignalConversion: '<S142>/TmpLatchAtExtr_LowSpd_MddpExecutedOutport1'
     */
    Extraction_FinalSpeedToUnb(Extraction_B.Extr_LowSpd_MddpExecuted,
        &rtb_BusCreator, &rtb_LoadMassIndex, rty_UnbTarget);

    /* End of Outputs for SubSystem: '<S142>/CalculateUnbalanceTargetSubsystem' */
}

/* Output and update for Simulink Function: '<S124>/CalculateUnbalanceTargetArray' */
void Extraction_CalculateUnbalanceTargetArray(uint16 rty_UnbTargetArray[25])
{
    boolean_T b_EXTR_FEATURE_OPT_RAMP_ENABLED;
    uint8 ii;
    uint16 tmp;
    sint16 rtb_FunctionCaller;
    sint32 i;
    uint32 qY;
    sint16 tmp_0;
    uint32 tmp_1;

    /* SignalConversion: '<S140>/TmpLatchAtExtr_LowSpd_MddpExecutedOutport1' */
    Extraction_B.Extr_LowSpd_MddpExecuted =
        Extraction_B.Extr_LowSpd_MddpExecuted_p5;

    /* PreLookup: '<S140>/LoadMassPrelookup' incorporates:
     *  DataTypeConversion: '<S140>/Data Type Conversion'
     *  Gain: '<S140>/Gain1'
     *  SignalConversion: '<S140>/TmpLatchAtExtr_LowSpd_LoadMassOutport1'
     */
    Extraction_B.LoadMassIndex.k = plook_u8u16f_binc((uint16)(100.0F *
        Extraction_B.Extr_LowSpd_LoadMass_gp),
        (&(GESE_Extr_OptRampLoadMassX100_BP.BP[0])),
        GESE_Extr_OptRampLoadMassX100_BP.N - 1U, &Extraction_B.LoadMassIndex.f);

    /* FunctionCaller: '<S140>/Function Caller' */
    Extraction_LoadMassToUpperOffset(Extraction_B.Extr_LowSpd_MddpExecuted,
        &Extraction_B.LoadMassIndex, &rtb_FunctionCaller);

    /* MATLAB Function: '<S140>/MATLAB Function' incorporates:
     *  Constant: '<S140>/OptRampUnbLutSize'
     */
    b_EXTR_FEATURE_OPT_RAMP_ENABLED = ((boolean_T)EXTR_FEATURE_OPT_RAMP_ENABLED);

    /* MATLAB Function 'Extraction/Extr_LowSpd/Extr_LowSpd_Algorithms/Extr_LowSpd_BasicUnbChk/CalculateUnbalanceTargetArray/MATLAB Function': '<S143>:1' */
    /* '<S143>:1:5' */
    for (i = 0; i < 25; i++)
    {
        rty_UnbTargetArray[i] = MAX_uint16_T;
    }

    qY = GESE_Extr_OptRampUnbLut_Size[0] - 1U;
    if (qY > GESE_Extr_OptRampUnbLut_Size[0])
    {
        qY = 0U;
    }

    /* '<S143>:1:7' */
    for (ii = 0U; ii <= (uint8)qY; ii++)
    {
        /* '<S143>:1:7' */
        if ((*GESE_Extr_Setup).OR_Enabled != 0 &&
                b_EXTR_FEATURE_OPT_RAMP_ENABLED)
        {
            /* '<S143>:1:8' */
            /* '<S143>:1:9' */
            Extraction_CalculateUnbalanceTarget(ii, &tmp);
            tmp_0 = rtb_FunctionCaller;
            if (rtb_FunctionCaller < 0)
            {
                tmp_0 = 0;
            }

            tmp_1 = (uint32)tmp + tmp_0;
            if (tmp_1 > 65535U)
            {
                tmp_1 = 65535U;
            }

            rty_UnbTargetArray[ii] = (uint16)tmp_1;
        }
        else
        {
            /* '<S143>:1:11' */
            Extraction_CalculateUnbalanceTarget(ii, &rty_UnbTargetArray[ii]);
        }
    }

    /* End of MATLAB Function: '<S140>/MATLAB Function' */
}

/* Function for Chart: '<S150>/OptRampLogic' */
#if EXTR_VARIANT_OPT_RAMP_ENABLED

static float32 Extraction_ConvertMgrToGrams(float32 ub_h, float32 coeffs_K2_h,
    float32 coeffs_K4_h, sint16 coeffs_K1_h, sint16 coeffs_K3_h)
{
    /* MATLAB Function 'ConvertMgrToGrams': '<S151>:374' */
    /*  Converts the unbalance measurement from moment (m*g*r) in N*m to mass in grams. */
    /* '<S151>:374:4' */
    return ((float32)coeffs_K1_h + 1000.0F * coeffs_K2_h * ub_h) * coeffs_K4_h +
        (float32)coeffs_K3_h;
}

#endif                                 /* EXTR_VARIANT_OPT_RAMP_ENABLED */

/* Function for Chart: '<S150>/OptRampLogic' */
#if EXTR_VARIANT_OPT_RAMP_ENABLED

static void Extraction_broadcast_localStop(void)
{
    sint32 b_previousEvent_h;

    /* Event: '<S151>:312' */
    b_previousEvent_h = Extraction_DW.Extr_LowSpd_OptRamp_Enabled.sfEvent;
    Extraction_DW.Extr_LowSpd_OptRamp_Enabled.sfEvent =
        Extraction_event_localStop;
    Extraction_c13_Extraction();
    Extraction_DW.Extr_LowSpd_OptRamp_Enabled.sfEvent = b_previousEvent_h;
}

#endif                                 /* EXTR_VARIANT_OPT_RAMP_ENABLED */

/* Function for Chart: '<S150>/OptRampLogic' */
#if EXTR_VARIANT_OPT_RAMP_ENABLED

static boolean_T Extraction_BallBalancerAtOptimalRampAngle(float32
    oraToBBAngle_h, float32 window_h)
{
    boolean_T okToRamp_h;

    /* MATLAB Function 'BallBalancerAtOptimalRampAngle': '<S151>:272' */
    /*  Definition of load_to_bb_angle (bb stands for Ball Balancers). */
    /*  The ParamEst module is expected to return a value between 0 and +/-PI for PARAM_EST_BBPHASEDIFF. */
    /*  A value outside of this range will result in erroneous behavior in the Environment code. */
    /*   */
    /*  Example: LOAD is always at 0 degrees and the Ball Balancers are referenced from the LOAD. */
    /*  In this example, the load_to_bb_angle = +90 because the Ball Balancers are located */
    /*  +90 degrees from the LOAD. */
    /*   */
    /*                       +/-180deg */
    /*                          | */
    /*              -135deg            +135deg */
    /*                     \          / */
    /*   */
    /*          -90deg  -                - +90deg     Ball Balancers */
    /*   */
    /*                     /          \ */
    /*                -45deg           +45deg */
    /*                          | */
    /*                          0deg */
    /*   */
    /*                         LOAD */
    /*   */
    /*  Get the Parameter Estimation estimated angle from the load to the ball balancers and verify */
    /*  it is within -PI to +PI.    */
    if (oraToBBAngle_h >= -fabsf(window_h))
    {
        /* '<S151>:272:29' */
        if (oraToBBAngle_h <= fabsf(window_h))
        {
            /* '<S151>:272:29' */
            /* '<S151>:272:30' */
            okToRamp_h = true;
        }
        else
        {
            /* '<S151>:272:32' */
            okToRamp_h = false;
        }
    }
    else
    {
        /* '<S151>:272:32' */
        okToRamp_h = false;
    }

    return okToRamp_h;
}

#endif                                 /* EXTR_VARIANT_OPT_RAMP_ENABLED */

/* Function for Chart: '<S150>/OptRampLogic' */
#if EXTR_VARIANT_OPT_RAMP_ENABLED

static void Extraction_c13_Extraction(void)
{
    uint8 rtb_TempPreLookup_k_h;
    float32 rtb_TempPreLookup_f_h;
    float32 rtb_TempToTargetPhaseTolerance;
    PRE_LOOKUP_OUTPUT_TYPE rtb_WaterTempPreLookup;
    PRE_LOOKUP_OUTPUT_TYPE rtb_LoadMassPrelookup;
    uint16 rtb_FunctionCaller_FinalSpeedToUnb_oo;
    sint32 rtb_DataTypeConversion1;
    sint16 rtb_Merge_n4;
    sint32 rtb_DataTypeConversion2;
    uint32 maxIdx_h[2];
    float32 frac_h[2];
    uint8 bpIndex_h[2];
    sint8 u0_h;

    /* During: Extraction/Extr_LowSpd/Extr_LowSpd_Algorithms/Extr_LowSpd_OptRamp/Extr_LowSpd_OptRamp_Enabled/OptRampLogic */
    switch (Extraction_DW.Extr_LowSpd_OptRamp_Enabled.is_c13_Extraction)
    {
      case Extraction_IN_IDLE:
        /* During 'IDLE': '<S151>:172' */
        if (Extraction_DW.Extr_LowSpd_OptRamp_Enabled.sfEvent ==
                Extraction_event_evStart)
        {
            /* Transition: '<S151>:176' */
            if (Extraction_DW.Extr_LowSpd_OptRamp_Enabled.is_c13_Extraction !=
                    Extraction_IN_IDLE)
            {
            }
            else
            {
                /* Exit 'IDLE': '<S151>:172' */
                Extr_LowSpd_OptRampExitReason = OPT_RAMP_EXIT_REASON_NONE;
                Extraction_DW.Extr_LowSpd_OptRamp_Enabled.is_c13_Extraction =
                    Extraction_IN_NO_ACTIVE_CHILD_pr;
            }

            if (Extraction_DW.Extr_LowSpd_OptRamp_Enabled.is_c13_Extraction ==
                    Extraction_IN_OPT_RAMP)
            {
            }
            else
            {
                Extraction_DW.Extr_LowSpd_OptRamp_Enabled.is_c13_Extraction =
                    Extraction_IN_OPT_RAMP;

                /* Entry 'OPT_RAMP': '<S151>:265' */
                /* Simulink Function 'CalculateOptRampTargets': '<S151>:348' */
                Extraction_B.Extr_LowSpd_OptRamp_Enabled.waterTemp =
                    Extr_Inputs_WaterTemp;

                /* Outputs for Function Call SubSystem: '<S151>/CalculateOptRampTargets' */
                /* PreLookup: '<S152>/TempPreLookup' */
                rtb_TempPreLookup_k_h = plook_u8fu16f_binc
                    (Extraction_B.Extr_LowSpd_OptRamp_Enabled.waterTemp,
                     (&(GESE_Extr_WaterTemp_BP.BP[0])), GESE_Extr_WaterTemp_BP.N
                     - 1U, &rtb_TempPreLookup_f_h);

                /* Interpolation_n-D: '<S152>/TempToTargetPhaseAdvace' incorporates:
                 *  Constant: '<S152>/PlateauTimePhaseAdvanceTarget'
                 */
                rtb_TempToTargetPhaseTolerance = intrp1d_u16ffu8fl
                    (rtb_TempPreLookup_k_h, rtb_TempPreLookup_f_h,
                     (&(GESE_Extr_OptRampPhsAdvTgt_LUT[0])));

                /* Gain: '<S154>/Gain1' incorporates:
                 *  UnaryMinus: '<S152>/Unary Minus'
                 */
                Extraction_B.Extr_LowSpd_OptRamp_Enabled.Gain1 = 0.0174532924F *
                    -rtb_TempToTargetPhaseTolerance;

                /* Interpolation_n-D: '<S152>/TempToTargetPhaseTolerance' incorporates:
                 *  Constant: '<S152>/PlateauTimePhaseTolerance'
                 */
                rtb_TempToTargetPhaseTolerance = intrp1d_u16ffu8fl
                    (rtb_TempPreLookup_k_h, rtb_TempPreLookup_f_h,
                     (&(GESE_Extr_OptRampPhaseTolerance_LUT[0])));

                /* Gain: '<S155>/Gain1' */
                Extraction_B.Extr_LowSpd_OptRamp_Enabled.Gain1_id =
                    0.0174532924F * rtb_TempToTargetPhaseTolerance;

                /* Interpolation_n-D: '<S152>/TempToPlateauTimeLUT' incorporates:
                 *  Constant: '<S152>/PlateauTimeLutData'
                 */
                rtb_TempPreLookup_f_h = intrp1d_u8ffu8fl(rtb_TempPreLookup_k_h,
                    rtb_TempPreLookup_f_h, (&(GESE_Extr_OptRampPlateauTime_LUT[0])));
                Extraction_B.Extr_LowSpd_OptRamp_Enabled.TempToPlateauTimeLUT =
                    (uint8)rtb_TempPreLookup_f_h;

                /* End of Outputs for SubSystem: '<S151>/CalculateOptRampTargets' */
                Extr_LowSpd_OptRampPlatTime =
                    Extraction_B.Extr_LowSpd_OptRamp_Enabled.TempToPlateauTimeLUT;
                Extr_LowSpd_OptRampPhsAdvTgt =
                    Extraction_B.Extr_LowSpd_OptRamp_Enabled.Gain1;
                Extr_LowSpd_OptRampPhsTol =
                    Extraction_B.Extr_LowSpd_OptRamp_Enabled.Gain1_id;

                /* Simulink Function 'CalculateUBThresholds': '<S151>:288' */
                Extraction_B.Extr_LowSpd_OptRamp_Enabled.mass =
                    Extraction_B.Extr_LowSpd_LoadMass_gp;
                Extraction_B.Extr_LowSpd_OptRamp_Enabled.speedTarget =
                    Extraction_B.MaxTargetSpeed;
                Extraction_B.Extr_LowSpd_OptRamp_Enabled.mddpExecuted =
                    Extraction_B.Extr_LowSpd_MddpExecuted_p5;
                Extraction_B.Extr_LowSpd_OptRamp_Enabled.rebalAttempts =
                    Extraction_B.Extr_LowSpd_RebalCnt_lp;
                Extraction_B.Extr_LowSpd_OptRamp_Enabled.temp =
                    Extr_Inputs_WaterTemp;
                Extraction_B.Extr_LowSpd_OptRamp_Enabled.fs = ((uint16)
                    FS_EXTRACTION);

                /* Outputs for Function Call SubSystem: '<S151>/CalculateUBThresholds' */
                /* PreLookup: '<S153>/SpeedPreLookup' incorporates:
                 *  Gain: '<S153>/Gain'
                 */
                rtb_WaterTempPreLookup.k = plook_u8u8f_binc((uint8)(52429U *
                    Extraction_B.Extr_LowSpd_OptRamp_Enabled.speedTarget >> 19),
                    (&(GESE_Extr_TargetSpeedDiv10_BP.BP[0])),
                    GESE_Extr_TargetSpeedDiv10_BP.N - 1U,
                    &rtb_WaterTempPreLookup.f);

                /* Sum: '<S157>/Subtract2' incorporates:
                 *  Constant: '<S153>/SpeedPrelookupArraySize'
                 *  Constant: '<S157>/Constant3'
                 *  DataTypeConversion: '<S157>/Data Type Conversion'
                 */
                u0_h = (sint8)((sint8)GESE_Extr_OptRampUnbLut_Size[0] - 1);

                /* MinMax: '<S157>/MinMax' */
                if (u0_h > 0)
                {
                }
                else
                {
                    u0_h = 0;
                }

                /* End of MinMax: '<S157>/MinMax' */

                /* BusCreator: '<S157>/Bus Creator' incorporates:
                 *  Constant: '<S157>/Constant2'
                 *  Sum: '<S157>/Subtract'
                 *  Sum: '<S157>/Subtract1'
                 */
                rtb_WaterTempPreLookup.k = (uint8)((uint8)((uint32)u0_h -
                    rtb_WaterTempPreLookup.k) - 1U);
                rtb_WaterTempPreLookup.f = 1.0F - rtb_WaterTempPreLookup.f;

                /* DataTypeConversion: '<S153>/Data Type Conversion' incorporates:
                 *  Gain: '<S153>/Gain1'
                 */
                rtb_FunctionCaller_FinalSpeedToUnb_oo = (uint16)(100.0F *
                    Extraction_B.Extr_LowSpd_OptRamp_Enabled.mass);

                /* PreLookup: '<S153>/LoadMassPrelookup' */
                rtb_LoadMassPrelookup.k = plook_u8u16f_binc
                    (rtb_FunctionCaller_FinalSpeedToUnb_oo,
                     (&(GESE_Extr_OptRampLoadMassX100_BP.BP[0])),
                     GESE_Extr_OptRampLoadMassX100_BP.N - 1U,
                     &rtb_LoadMassPrelookup.f);

                /* FunctionCaller: '<S153>/FunctionCaller_FinalSpeedToUnb' */
                Extraction_FinalSpeedToUnb
                    (Extraction_B.Extr_LowSpd_OptRamp_Enabled.mddpExecuted,
                     &rtb_WaterTempPreLookup, &rtb_LoadMassPrelookup,
                     &rtb_FunctionCaller_FinalSpeedToUnb_oo);

                /* PreLookup: '<S153>/RebalPreLookup' */
                rtb_TempPreLookup_k_h = plook_u8u8f_binca
                    (Extraction_B.Extr_LowSpd_OptRamp_Enabled.rebalAttempts,
                     (&(GESE_Extr_RebalAttempts_BP.BP[0])),
                     GESE_Extr_RebalAttempts_BP.N - 1U, &rtb_TempPreLookup_f_h);

                /* If: '<S159>/If' */
                if (Extraction_B.Extr_LowSpd_OptRamp_Enabled.mddpExecuted)
                {
                    /* Outputs for IfAction SubSystem: '<S159>/UnbCalcAfterMddp' incorporates:
                     *  ActionPort: '<S162>/Action Port'
                     */
                    /* Interpolation_n-D: '<S162>/RebalToUB' */
                    maxIdx_h[0U] = GESE_Extr_TargetSpeedDiv10_BP.N - 1U;
                    maxIdx_h[1U] = GESE_Extr_OptRampLoadMassX100_BP.N - 1U;
                    if (rtb_TempPreLookup_f_h < 0.0F)
                    {
                        rtb_TempPreLookup_f_h = 0.0F;
                    }
                    else
                    {
                        if (rtb_TempPreLookup_f_h > 1.0F)
                        {
                            rtb_TempPreLookup_f_h = 1.0F;
                        }
                    }

                    frac_h[0] = rtb_TempPreLookup_f_h;
                    rtb_TempPreLookup_f_h = rtb_LoadMassPrelookup.f;
                    if (rtb_LoadMassPrelookup.f < 0.0F)
                    {
                        rtb_TempPreLookup_f_h = 0.0F;
                    }
                    else
                    {
                        if (rtb_LoadMassPrelookup.f > 1.0F)
                        {
                            rtb_TempPreLookup_f_h = 1.0F;
                        }
                    }

                    frac_h[1] = rtb_TempPreLookup_f_h;
                    bpIndex_h[0] = rtb_TempPreLookup_k_h;
                    bpIndex_h[1] = rtb_LoadMassPrelookup.k;
                    rtb_TempPreLookup_f_h = intrp2d_u16ffu8fla(bpIndex_h, frac_h,
                        (&(GESE_Extr_OptRampUnbAfterMDDP_LUT[0])),
                        GESE_Extr_TargetSpeedDiv10_BP.N, maxIdx_h);
                    Extr_LowSpd_OptRampConvUnb = (uint16)rtb_TempPreLookup_f_h;

                    /* End of Interpolation_n-D: '<S162>/RebalToUB' */
                    /* End of Outputs for SubSystem: '<S159>/UnbCalcAfterMddp' */
                }
                else
                {
                    /* Outputs for IfAction SubSystem: '<S159>/UnbCalcBeforeMddp' incorporates:
                     *  ActionPort: '<S163>/Action Port'
                     */
                    /* Interpolation_n-D: '<S163>/RebalToUB' */
                    maxIdx_h[0U] = GESE_Extr_TargetSpeedDiv10_BP.N - 1U;
                    maxIdx_h[1U] = GESE_Extr_OptRampLoadMassX100_BP.N - 1U;
                    if (rtb_TempPreLookup_f_h < 0.0F)
                    {
                        rtb_TempPreLookup_f_h = 0.0F;
                    }
                    else
                    {
                        if (rtb_TempPreLookup_f_h > 1.0F)
                        {
                            rtb_TempPreLookup_f_h = 1.0F;
                        }
                    }

                    frac_h[0] = rtb_TempPreLookup_f_h;
                    rtb_TempPreLookup_f_h = rtb_LoadMassPrelookup.f;
                    if (rtb_LoadMassPrelookup.f < 0.0F)
                    {
                        rtb_TempPreLookup_f_h = 0.0F;
                    }
                    else
                    {
                        if (rtb_LoadMassPrelookup.f > 1.0F)
                        {
                            rtb_TempPreLookup_f_h = 1.0F;
                        }
                    }

                    frac_h[1] = rtb_TempPreLookup_f_h;
                    bpIndex_h[0] = rtb_TempPreLookup_k_h;
                    bpIndex_h[1] = rtb_LoadMassPrelookup.k;
                    rtb_TempPreLookup_f_h = intrp2d_u16ffu8fla(bpIndex_h, frac_h,
                        (&(GESE_Extr_OptRampUnbBeforeMDDP_LUT[0])),
                        GESE_Extr_TargetSpeedDiv10_BP.N, maxIdx_h);
                    Extr_LowSpd_OptRampConvUnb = (uint16)rtb_TempPreLookup_f_h;

                    /* End of Interpolation_n-D: '<S163>/RebalToUB' */
                    /* End of Outputs for SubSystem: '<S159>/UnbCalcBeforeMddp' */
                }

                /* End of If: '<S159>/If' */

                /* MinMax: '<S153>/Max' */
                if (rtb_FunctionCaller_FinalSpeedToUnb_oo >
                        Extr_LowSpd_OptRampConvUnb)
                {
                    Extr_LowSpd_OptRampConvUnb =
                        rtb_FunctionCaller_FinalSpeedToUnb_oo;
                }

                /* End of MinMax: '<S153>/Max' */

                /* DataTypeConversion: '<S153>/Data Type Conversion1' */
                rtb_DataTypeConversion1 = Extr_LowSpd_OptRampConvUnb;

                /* FunctionCaller: '<S153>/Function Caller' */
                Extraction_LoadMassToUpperOffset
                    (Extraction_B.Extr_LowSpd_OptRamp_Enabled.mddpExecuted,
                     &rtb_LoadMassPrelookup, &rtb_Merge_n4);

                /* DataTypeConversion: '<S153>/Data Type Conversion2' */
                rtb_DataTypeConversion2 = rtb_Merge_n4;

                /* If: '<S158>/If' */
                if (Extraction_B.Extr_LowSpd_OptRamp_Enabled.mddpExecuted)
                {
                    /* Outputs for IfAction SubSystem: '<S158>/If Action Subsystem' incorporates:
                     *  ActionPort: '<S160>/Action Port'
                     */
                    /* Interpolation_n-D: '<S160>/LoadMassToLowerOffset' */
                    rtb_TempPreLookup_f_h = rtb_LoadMassPrelookup.f;
                    if (rtb_LoadMassPrelookup.f < 0.0F)
                    {
                        rtb_TempPreLookup_f_h = 0.0F;
                    }
                    else
                    {
                        if (rtb_LoadMassPrelookup.f > 1.0F)
                        {
                            rtb_TempPreLookup_f_h = 1.0F;
                        }
                    }

                    rtb_TempPreLookup_k_h = rtb_LoadMassPrelookup.k;
                    if (rtb_LoadMassPrelookup.k >
                            GESE_Extr_OptRampLoadMassX100_BP.N - 2U)
                    {
                        rtb_TempPreLookup_k_h = (uint8)
                            (GESE_Extr_OptRampLoadMassX100_BP.N - 2U);
                    }

                    rtb_Merge_n4 = (sint16)intrp1d_s16ffu8fl
                        (rtb_TempPreLookup_k_h, rtb_TempPreLookup_f_h,
                         (&(GESE_Extr_OptRampLowerOffsetAfterMDDP_LUT[0])));

                    /* End of Interpolation_n-D: '<S160>/LoadMassToLowerOffset' */
                    /* End of Outputs for SubSystem: '<S158>/If Action Subsystem' */
                }
                else
                {
                    /* Outputs for IfAction SubSystem: '<S158>/If Action Subsystem1' incorporates:
                     *  ActionPort: '<S161>/Action Port'
                     */
                    /* Interpolation_n-D: '<S161>/LoadMassToLowerOffset' */
                    rtb_TempPreLookup_f_h = rtb_LoadMassPrelookup.f;
                    if (rtb_LoadMassPrelookup.f < 0.0F)
                    {
                        rtb_TempPreLookup_f_h = 0.0F;
                    }
                    else
                    {
                        if (rtb_LoadMassPrelookup.f > 1.0F)
                        {
                            rtb_TempPreLookup_f_h = 1.0F;
                        }
                    }

                    rtb_TempPreLookup_k_h = rtb_LoadMassPrelookup.k;
                    if (rtb_LoadMassPrelookup.k >
                            GESE_Extr_OptRampLoadMassX100_BP.N - 2U)
                    {
                        rtb_TempPreLookup_k_h = (uint8)
                            (GESE_Extr_OptRampLoadMassX100_BP.N - 2U);
                    }

                    rtb_Merge_n4 = (sint16)intrp1d_s16ffu8fl
                        (rtb_TempPreLookup_k_h, rtb_TempPreLookup_f_h,
                         (&(GESE_Extr_OptRampLowerOffsetBeforeMDDP_LUT[0])));

                    /* End of Interpolation_n-D: '<S161>/LoadMassToLowerOffset' */
                    /* End of Outputs for SubSystem: '<S158>/If Action Subsystem1' */
                }

                /* End of If: '<S158>/If' */

                /* PreLookup: '<S153>/WaterTempPreLookup' */
                rtb_WaterTempPreLookup.k = plook_u8fu16f_binc
                    (Extraction_B.Extr_LowSpd_OptRamp_Enabled.temp,
                     (&(GESE_Extr_WaterTemp_BP.BP[0])), GESE_Extr_WaterTemp_BP.N
                     - 1U, &rtb_WaterTempPreLookup.f);

                /* Interpolation_n-D: '<S153>/SumpTempToConvTime' incorporates:
                 *  Constant: '<S153>/ConvTimeLutData'
                 */
                rtb_TempPreLookup_f_h = intrp1d_u8ffu8fl
                    (rtb_WaterTempPreLookup.k, rtb_WaterTempPreLookup.f,
                     (&(GESE_Extr_OptRampConvTime_LUT[0])));
                Extr_LowSpd_OptRampConvTime = (uint8)rtb_TempPreLookup_f_h;

                /* MATLAB Function: '<S153>/CalcOffsetsAndSlopes' incorporates:
                 *  DataTypeConversion: '<S153>/Data Type Conversion3'
                 *  DataTypeConversion: '<S153>/Data Type Conversion4'
                 *  DataTypeConversion: '<S153>/Data Type Conversion5'
                 */
                /* MATLAB Function 'Extraction/Extr_LowSpd/Extr_LowSpd_Algorithms/Extr_LowSpd_OptRamp/Extr_LowSpd_OptRamp_Enabled/OptRampLogic/CalculateUBThresholds/CalcOffsetsAndSlopes': '<S156>:1' */
                /* '<S156>:1:3' */
                Extraction_B.Extr_LowSpd_OptRamp_Enabled.thDefinition.Upper.Slope
                    = -(float32)rtb_DataTypeConversion2 / (float32)
                    (Extraction_B.Extr_LowSpd_OptRamp_Enabled.fs *
                     Extr_LowSpd_OptRampConvTime);

                /* '<S156>:1:4' */
                Extraction_B.Extr_LowSpd_OptRamp_Enabled.thDefinition.Upper.Offset
                    = (float32)(rtb_DataTypeConversion2 +
                                rtb_DataTypeConversion1);

                /* '<S156>:1:5' */
                Extraction_B.Extr_LowSpd_OptRamp_Enabled.thDefinition.Lower.Slope
                    = -(float32)rtb_Merge_n4 / (float32)
                    (Extraction_B.Extr_LowSpd_OptRamp_Enabled.fs *
                     Extr_LowSpd_OptRampConvTime);

                /* '<S156>:1:6' */
                Extraction_B.Extr_LowSpd_OptRamp_Enabled.thDefinition.Lower.Offset
                    = (float32)(rtb_Merge_n4 + rtb_DataTypeConversion1);

                /* End of Outputs for SubSystem: '<S151>/CalculateUBThresholds' */
                Extraction_DW.Extr_LowSpd_OptRamp_Enabled.thSpec =
                    Extraction_B.Extr_LowSpd_OptRamp_Enabled.thDefinition;
            }

            /* Entry Internal 'OPT_RAMP': '<S151>:265' */
            /* Transition: '<S151>:264' */
            if (Extraction_DW.Extr_LowSpd_OptRamp_Enabled.is_OPT_RAMP ==
                    Extraction_IN_WAITING_OPT_RAMP)
            {
            }
            else
            {
                Extraction_DW.Extr_LowSpd_OptRamp_Enabled.is_OPT_RAMP =
                    Extraction_IN_WAITING_OPT_RAMP;

                /* Entry 'WAITING_OPT_RAMP': '<S151>:263' */
                Extraction_CalcORAToBBAngle(Extr_Inputs_ParamEstData.BB_PhaseLag,
                    Extr_LowSpd_OptRampPhsAdvTgt, &Extr_LowSpd_OptRampORA2BBA);
                Extr_LowSpd_OptRampUnbMass = Extraction_ConvertMgrToGrams
                    (Extr_Inputs_ParamEstData.UB, (*GESE_Extr_UnbConvCoeffs).K2,
                     (*GESE_Extr_UnbConvCoeffs).K4, (*GESE_Extr_UnbConvCoeffs).
                     K1, (*GESE_Extr_UnbConvCoeffs).K3);
                Extr_LowSpd_OptRampBBMass = Extraction_ConvertMgrToGrams
                    (Extr_Inputs_ParamEstData.BB_mgr, (*GESE_Extr_UnbConvCoeffs)
                     .K2, (*GESE_Extr_UnbConvCoeffs).K4,
                     (*GESE_Extr_UnbConvCoeffs).K1, (*GESE_Extr_UnbConvCoeffs).
                     K3);
            }

            /* Entry Internal 'WAITING_OPT_RAMP': '<S151>:263' */
            /* Transition: '<S151>:205' */
            if (Extraction_DW.Extr_LowSpd_OptRamp_Enabled.is_WAITING_OPT_RAMP ==
                Extraction_IN_PLATEAU)
            {
            }
            else
            {
                Extraction_DW.Extr_LowSpd_OptRamp_Enabled.is_WAITING_OPT_RAMP =
                    Extraction_IN_PLATEAU;
                Extraction_DW.Extr_LowSpd_OptRamp_Enabled.temporalCounter_i1 =
                    0U;
                Extr_LowSpd_OptRampState = OPT_RAMP_STATE_PLATEAU;

                /* Entry 'PLATEAU': '<S151>:204' */
                /* MATLAB Function 'UpdateUBThresholds': '<S151>:210' */
                /* '<S151>:210:3' */
                /* '<S151>:210:4' */
                Extr_LowSpd_OptRampUpperTh =
                    Extraction_DW.Extr_LowSpd_OptRamp_Enabled.thSpec.Upper.Offset;
                Extr_LowSpd_OptRampLowerTh =
                    Extraction_DW.Extr_LowSpd_OptRamp_Enabled.thSpec.Lower.Offset;
            }
        }
        break;

      case Extraction_IN_OPT_RAMP:
        /* During 'OPT_RAMP': '<S151>:265' */
        if (Extraction_DW.Extr_LowSpd_OptRamp_Enabled.sfEvent ==
                Extraction_event_evStop ||
                Extraction_DW.Extr_LowSpd_OptRamp_Enabled.sfEvent ==
                Extraction_event_localStop)
        {
            /* Transition: '<S151>:177' */
            /* Exit Internal 'OPT_RAMP': '<S151>:265' */
            Extraction_DW.Extr_LowSpd_OptRamp_Enabled.is_OPT_RAMP =
                Extraction_IN_NO_ACTIVE_CHILD_pr;

            /* Exit Internal 'WAITING_OPT_RAMP': '<S151>:263' */
            Extraction_DW.Extr_LowSpd_OptRamp_Enabled.is_WAITING_OPT_RAMP =
                Extraction_IN_NO_ACTIVE_CHILD_pr;
            Extraction_DW.Extr_LowSpd_OptRamp_Enabled.is_c13_Extraction =
                Extraction_IN_IDLE;
            Extr_LowSpd_OptRampState = OPT_RAMP_STATE_IDLE;
        }
        else
        {
            switch (Extraction_DW.Extr_LowSpd_OptRamp_Enabled.is_OPT_RAMP)
            {
              case Extraction_IN_RAMP_AT_OPTIMAL_TIME:
                /* During 'RAMP_AT_OPTIMAL_TIME': '<S151>:225' */
                /* Transition: '<S151>:316' */
                /* Transition: '<S151>:315' */
                Extraction_broadcast_localStop();
                break;

              case Extraction_IN_UB_LARGE:
                /* During 'UB_LARGE': '<S151>:241' */
                /* Transition: '<S151>:319' */
                /* Transition: '<S151>:315' */
                Extraction_broadcast_localStop();
                break;

              case Extraction_IN_UB_SMALL:
                /* During 'UB_SMALL': '<S151>:304' */
                /* Transition: '<S151>:317' */
                /* Transition: '<S151>:315' */
                Extraction_broadcast_localStop();
                break;

              case Extraction_IN_WAITING_OPT_RAMP:
                /* During 'WAITING_OPT_RAMP': '<S151>:263' */
                if (Extr_LowSpd_OptRampUnbMass >= Extr_LowSpd_OptRampLowerTh &&
                        Extr_LowSpd_OptRampUnbMass <= Extr_LowSpd_OptRampUpperTh
                    && (Extraction_BallBalancerAtOptimalRampAngle
                        (Extr_LowSpd_OptRampORA2BBA, Extr_LowSpd_OptRampPhsTol) &&
                        (uint16)roundf(Extr_LowSpd_OptRampBBMass) >
                        (*GESE_Extr_OptRampSetup).BB_Magnitude_Threshold))
                {
                    /* Transition: '<S151>:248' */
                    /* Exit Internal 'WAITING_OPT_RAMP': '<S151>:263' */
                    Extraction_DW.Extr_LowSpd_OptRamp_Enabled.is_WAITING_OPT_RAMP
                        = Extraction_IN_NO_ACTIVE_CHILD_pr;
                    Extraction_DW.Extr_LowSpd_OptRamp_Enabled.is_OPT_RAMP =
                        Extraction_IN_RAMP_AT_OPTIMAL_TIME;
                    Extr_LowSpd_OptRampState =
                        OPT_RAMP_STATE_RAMP_AT_OPTIMAL_TIME;

                    /* Entry 'RAMP_AT_OPTIMAL_TIME': '<S151>:225' */
                    Extr_LowSpd_OptRampExitReason =
                        OPT_RAMP_EXIT_REASON_RAMP_AT_OPTIMAL_TIME;

                    /* Event: '<S151>:307' */
                    Extraction_DW.Extr_LowSpd_OptRamp_Enabled.evOptRampEventCounter
                        ++;
                }
                else if (Extr_LowSpd_OptRampUnbMass > Extr_LowSpd_OptRampUpperTh)
                {
                    /* Transition: '<S151>:243' */
                    /* Exit Internal 'WAITING_OPT_RAMP': '<S151>:263' */
                    Extraction_DW.Extr_LowSpd_OptRamp_Enabled.is_WAITING_OPT_RAMP
                        = Extraction_IN_NO_ACTIVE_CHILD_pr;
                    Extraction_DW.Extr_LowSpd_OptRamp_Enabled.is_OPT_RAMP =
                        Extraction_IN_UB_LARGE;
                    Extr_LowSpd_OptRampState = OPT_RAMP_STATE_UB_LARGE;

                    /* Entry 'UB_LARGE': '<S151>:241' */
                    Extr_LowSpd_OptRampExitReason =
                        OPT_RAMP_EXIT_REASON_UB_LARGE;

                    /* Event: '<S151>:308' */
                    Extraction_DW.Extr_LowSpd_OptRamp_Enabled.evRedistEventCounter
                        ++;
                }
                else if (Extr_LowSpd_OptRampUnbMass < Extr_LowSpd_OptRampLowerTh)
                {
                    /* Transition: '<S151>:266' */
                    /* Exit Internal 'WAITING_OPT_RAMP': '<S151>:263' */
                    Extraction_DW.Extr_LowSpd_OptRamp_Enabled.is_WAITING_OPT_RAMP
                        = Extraction_IN_NO_ACTIVE_CHILD_pr;
                    Extraction_DW.Extr_LowSpd_OptRamp_Enabled.is_OPT_RAMP =
                        Extraction_IN_UB_SMALL;
                    Extr_LowSpd_OptRampState = OPT_RAMP_STATE_UB_SMALL;

                    /* Entry 'UB_SMALL': '<S151>:304' */
                    Extr_LowSpd_OptRampExitReason =
                        OPT_RAMP_EXIT_REASON_UB_SMALL;

                    /* Event: '<S151>:307' */
                    Extraction_DW.Extr_LowSpd_OptRamp_Enabled.evOptRampEventCounter
                        ++;
                }
                else
                {
                    Extraction_CalcORAToBBAngle
                        (Extr_Inputs_ParamEstData.BB_PhaseLag,
                         Extr_LowSpd_OptRampPhsAdvTgt,
                         &Extr_LowSpd_OptRampORA2BBA);
                    Extr_LowSpd_OptRampUnbMass = Extraction_ConvertMgrToGrams
                        (Extr_Inputs_ParamEstData.UB, (*GESE_Extr_UnbConvCoeffs)
                         .K2, (*GESE_Extr_UnbConvCoeffs).K4,
                         (*GESE_Extr_UnbConvCoeffs).K1,
                         (*GESE_Extr_UnbConvCoeffs).K3);
                    Extr_LowSpd_OptRampBBMass = Extraction_ConvertMgrToGrams
                        (Extr_Inputs_ParamEstData.BB_mgr,
                         (*GESE_Extr_UnbConvCoeffs).K2,
                         (*GESE_Extr_UnbConvCoeffs).K4,
                         (*GESE_Extr_UnbConvCoeffs).K1,
                         (*GESE_Extr_UnbConvCoeffs).K3);
                    switch
                        (Extraction_DW.Extr_LowSpd_OptRamp_Enabled.is_WAITING_OPT_RAMP)
                    {
                      case Extraction_IN_LIMITS_CONVERGING:
                        /* During 'LIMITS_CONVERGING': '<S151>:208' */
                        /* MATLAB Function 'UpdateUBThresholds': '<S151>:210' */
                        /* '<S151>:210:3' */
                        /* '<S151>:210:4' */
                        Extr_LowSpd_OptRampUpperTh =
                            Extraction_DW.Extr_LowSpd_OptRamp_Enabled.thSpec.Upper.Offset
                            + Extraction_DW.Extr_LowSpd_OptRamp_Enabled.thSpec.Upper.Slope
                            * (float32)
                            Extraction_DW.Extr_LowSpd_OptRamp_Enabled.temporalCounter_i1;
                        Extr_LowSpd_OptRampLowerTh =
                            Extraction_DW.Extr_LowSpd_OptRamp_Enabled.thSpec.Lower.Offset
                            + Extraction_DW.Extr_LowSpd_OptRamp_Enabled.thSpec.Lower.Slope
                            * (float32)
                            Extraction_DW.Extr_LowSpd_OptRamp_Enabled.temporalCounter_i1;
                        break;

                      case Extraction_IN_PLATEAU:
                        /* During 'PLATEAU': '<S151>:204' */
                        if (Extraction_DW.Extr_LowSpd_OptRamp_Enabled.sfEvent ==
                            Extraction_event_ticks &&
                                Extraction_DW.Extr_LowSpd_OptRamp_Enabled.temporalCounter_i1
                            >= (uint16)((uint32)Extr_LowSpd_OptRampPlatTime *
                                        ((uint16)FS_EXTRACTION)))
                        {
                            /* Transition: '<S151>:209' */
                            Extraction_DW.Extr_LowSpd_OptRamp_Enabled.is_WAITING_OPT_RAMP
                                = Extraction_IN_LIMITS_CONVERGING;
                            Extraction_DW.Extr_LowSpd_OptRamp_Enabled.temporalCounter_i1
                                = 0U;
                            Extr_LowSpd_OptRampState =
                                OPT_RAMP_STATE_LIMITS_CONVERGING;

                            /* Entry 'LIMITS_CONVERGING': '<S151>:208' */
                            /* MATLAB Function 'UpdateUBThresholds': '<S151>:210' */
                            /* '<S151>:210:3' */
                            /* '<S151>:210:4' */
                            Extr_LowSpd_OptRampUpperTh =
                                Extraction_DW.Extr_LowSpd_OptRamp_Enabled.thSpec.Upper.Offset
                                + Extraction_DW.Extr_LowSpd_OptRamp_Enabled.thSpec.Upper.Slope
                                * (float32)
                                Extraction_DW.Extr_LowSpd_OptRamp_Enabled.temporalCounter_i1;
                            Extr_LowSpd_OptRampLowerTh =
                                Extraction_DW.Extr_LowSpd_OptRamp_Enabled.thSpec.Lower.Offset
                                + Extraction_DW.Extr_LowSpd_OptRamp_Enabled.thSpec.Lower.Slope
                                * (float32)
                                Extraction_DW.Extr_LowSpd_OptRamp_Enabled.temporalCounter_i1;
                        }
                        break;
                    }
                }
                break;
            }
        }
        break;
    }
}

#endif                                 /* EXTR_VARIANT_OPT_RAMP_ENABLED */

/* Function for Chart: '<S150>/OptRampLogic' */
#if EXTR_VARIANT_OPT_RAMP_ENABLED

static void Extraction_chartstep_c13_Extraction(void)
{
    Extraction_c13_Extraction();
}

#endif                                 /* EXTR_VARIANT_OPT_RAMP_ENABLED */

/*
 * Output and update for action system:
 *    '<S131>/If Action Subsystem1'
 *    '<S136>/If Action Subsystem1'
 */
void Extraction_IfActionSubsystem1(float32 rtu_angle, float32 *rty_calc_angle)
{
    /* Sum: '<S134>/Add' incorporates:
     *  Constant: '<S134>/TwoPi'
     */
    *rty_calc_angle = rtu_angle + 6.28318548F;
}

/* Output and update for Simulink Function: '<S121>/CalcORAToBBAngle' */
void Extraction_CalcORAToBBAngle(float32 rtu_bb_phase_lag, float32
    rtu_target_angle, float32 *rty_angle)
{
    float32 rtb_MathFunction;
    float32 rtb_Merge;

    /* Math: '<S136>/Math Function' incorporates:
     *  Constant: '<S132>/Pi1'
     *  Constant: '<S136>/Pi1'
     *  Constant: '<S136>/TwoPi'
     *  SignalConversion: '<S123>/TmpSignal ConversionAtbb_phase_lagOutport1'
     *  Sum: '<S132>/Add1'
     *  Sum: '<S136>/Add1'
     */
    rtb_MathFunction = rt_modf(rtu_bb_phase_lag + 3.14159274F + 3.14159274F,
        6.28318548F);

    /* If: '<S136>/If' incorporates:
     *  Constant: '<S137>/Constant'
     *  Inport: '<S139>/angle'
     *  RelationalOperator: '<S137>/Compare'
     */
    if (rtb_MathFunction < 0.0F)
    {
        /* Outputs for IfAction SubSystem: '<S136>/If Action Subsystem1' incorporates:
         *  ActionPort: '<S138>/Action Port'
         */
        Extraction_IfActionSubsystem1(rtb_MathFunction, &rtb_Merge);

        /* End of Outputs for SubSystem: '<S136>/If Action Subsystem1' */
    }
    else
    {
        /* Outputs for IfAction SubSystem: '<S136>/If Action Subsystem2' incorporates:
         *  ActionPort: '<S139>/Action Port'
         */
        rtb_Merge = rtb_MathFunction;

        /* End of Outputs for SubSystem: '<S136>/If Action Subsystem2' */
    }

    /* End of If: '<S136>/If' */

    /* Math: '<S131>/Math Function' incorporates:
     *  Constant: '<S131>/Pi1'
     *  Constant: '<S131>/TwoPi'
     *  Constant: '<S136>/Pi'
     *  SignalConversion: '<S123>/TmpSignal ConversionAttarget_angleOutport1'
     *  Sum: '<S123>/Subtract'
     *  Sum: '<S131>/Add1'
     *  Sum: '<S136>/Add'
     */
    rtb_MathFunction = rt_modf(rtb_Merge - 3.14159274F - rtu_target_angle +
        3.14159274F, 6.28318548F);

    /* If: '<S131>/If' incorporates:
     *  Constant: '<S133>/Constant'
     *  Inport: '<S135>/angle'
     *  RelationalOperator: '<S133>/Compare'
     */
    if (rtb_MathFunction < 0.0F)
    {
        /* Outputs for IfAction SubSystem: '<S131>/If Action Subsystem1' incorporates:
         *  ActionPort: '<S134>/Action Port'
         */
        Extraction_IfActionSubsystem1(rtb_MathFunction, &rtb_Merge);

        /* End of Outputs for SubSystem: '<S131>/If Action Subsystem1' */
    }
    else
    {
        /* Outputs for IfAction SubSystem: '<S131>/If Action Subsystem2' incorporates:
         *  ActionPort: '<S135>/Action Port'
         */
        rtb_Merge = rtb_MathFunction;

        /* End of Outputs for SubSystem: '<S131>/If Action Subsystem2' */
    }

    /* End of If: '<S131>/If' */

    /* SignalConversion: '<S123>/TmpSignal ConversionAtangleInport1' incorporates:
     *  Constant: '<S131>/Pi'
     *  Sum: '<S131>/Add'
     */
    *rty_angle = rtb_Merge - 3.14159274F;
}

/* Output and update for Simulink Function: '<S121>/FinalSpeedToUnb' */
void Extraction_FinalSpeedToUnb(boolean_T rtu_mddp_executed, const
    PRE_LOOKUP_OUTPUT_TYPE *rtu_speed_index, const PRE_LOOKUP_OUTPUT_TYPE
    *rtu_load_mass_index, uint16 *rty_unb)
{
    float32 fractionTmp_0d;
    uint32 maxIdx[2];
    float32 frac[2];
    uint8 bpIndex[2];

    /* If: '<S164>/If' incorporates:
     *  SignalConversion: '<S128>/TmpSignal ConversionAtmddp_executedOutport1'
     */
    if (rtu_mddp_executed)
    {
        /* Outputs for IfAction SubSystem: '<S164>/UnbCalcAfterMddp' incorporates:
         *  ActionPort: '<S165>/Action Port'
         */
        /* Interpolation_n-D: '<S165>/FinalSpeedToUB' incorporates:
         *  SignalConversion: '<S128>/TmpSignal ConversionAtload_mass_indexOutport1'
         *  SignalConversion: '<S128>/TmpSignal ConversionAtspeed_indexOutport1'
         */
        maxIdx[0U] = GESE_Extr_TargetSpeedDiv10_BP.N - 1U;
        maxIdx[1U] = GESE_Extr_OptRampLoadMassX100_BP.N - 1U;
        fractionTmp_0d = rtu_speed_index->f;
        if (rtu_speed_index->f < 0.0F)
        {
            fractionTmp_0d = 0.0F;
        }
        else
        {
            if (rtu_speed_index->f > 1.0F)
            {
                fractionTmp_0d = 1.0F;
            }
        }

        frac[0] = fractionTmp_0d;
        fractionTmp_0d = rtu_load_mass_index->f;
        if (rtu_load_mass_index->f < 0.0F)
        {
            fractionTmp_0d = 0.0F;
        }
        else
        {
            if (rtu_load_mass_index->f > 1.0F)
            {
                fractionTmp_0d = 1.0F;
            }
        }

        frac[1] = fractionTmp_0d;
        bpIndex[0] = rtu_speed_index->k;
        bpIndex[1] = rtu_load_mass_index->k;
        fractionTmp_0d = intrp2d_u16ffu8fla(bpIndex, frac,
            (&(GESE_Extr_OptRampUnbAfterMDDP_LUT[0])),
            GESE_Extr_TargetSpeedDiv10_BP.N, maxIdx);
        *rty_unb = (uint16)fractionTmp_0d;

        /* End of Interpolation_n-D: '<S165>/FinalSpeedToUB' */
        /* End of Outputs for SubSystem: '<S164>/UnbCalcAfterMddp' */
    }
    else
    {
        /* Outputs for IfAction SubSystem: '<S164>/UnbCalcBeforeMddp' incorporates:
         *  ActionPort: '<S166>/Action Port'
         */
        /* Interpolation_n-D: '<S166>/FinalSpeedToUB' incorporates:
         *  SignalConversion: '<S128>/TmpSignal ConversionAtload_mass_indexOutport1'
         *  SignalConversion: '<S128>/TmpSignal ConversionAtspeed_indexOutport1'
         */
        maxIdx[0U] = GESE_Extr_TargetSpeedDiv10_BP.N - 1U;
        maxIdx[1U] = GESE_Extr_OptRampLoadMassX100_BP.N - 1U;
        fractionTmp_0d = rtu_speed_index->f;
        if (rtu_speed_index->f < 0.0F)
        {
            fractionTmp_0d = 0.0F;
        }
        else
        {
            if (rtu_speed_index->f > 1.0F)
            {
                fractionTmp_0d = 1.0F;
            }
        }

        frac[0] = fractionTmp_0d;
        fractionTmp_0d = rtu_load_mass_index->f;
        if (rtu_load_mass_index->f < 0.0F)
        {
            fractionTmp_0d = 0.0F;
        }
        else
        {
            if (rtu_load_mass_index->f > 1.0F)
            {
                fractionTmp_0d = 1.0F;
            }
        }

        frac[1] = fractionTmp_0d;
        bpIndex[0] = rtu_speed_index->k;
        bpIndex[1] = rtu_load_mass_index->k;
        fractionTmp_0d = intrp2d_u16ffu8fla(bpIndex, frac,
            (&(GESE_Extr_OptRampUnbBeforeMDDP_LUT[0])),
            GESE_Extr_TargetSpeedDiv10_BP.N, maxIdx);
        *rty_unb = (uint16)fractionTmp_0d;

        /* End of Interpolation_n-D: '<S166>/FinalSpeedToUB' */
        /* End of Outputs for SubSystem: '<S164>/UnbCalcBeforeMddp' */
    }

    /* End of If: '<S164>/If' */
}

/* Output and update for Simulink Function: '<S121>/LoadMassToUpperOffset' */
void Extraction_LoadMassToUpperOffset(boolean_T rtu_mddp_executed, const
    PRE_LOOKUP_OUTPUT_TYPE *rtu_load_mass_index, sint16 *rty_upper_offset)
{
    float32 fractionTmp_0d;
    uint8 indexTmp_0d;

    /* If: '<S167>/If' incorporates:
     *  SignalConversion: '<S129>/TmpSignal ConversionAtmddp_executedOutport1'
     */
    if (rtu_mddp_executed)
    {
        /* Outputs for IfAction SubSystem: '<S167>/If Action Subsystem' incorporates:
         *  ActionPort: '<S168>/Action Port'
         */
        /* Interpolation_n-D: '<S168>/LoadMassToUpperOffset' incorporates:
         *  SignalConversion: '<S129>/TmpSignal ConversionAtload_mass_indexOutport1'
         */
        fractionTmp_0d = rtu_load_mass_index->f;
        if (rtu_load_mass_index->f < 0.0F)
        {
            fractionTmp_0d = 0.0F;
        }
        else
        {
            if (rtu_load_mass_index->f > 1.0F)
            {
                fractionTmp_0d = 1.0F;
            }
        }

        indexTmp_0d = rtu_load_mass_index->k;
        if (rtu_load_mass_index->k > GESE_Extr_OptRampLoadMassX100_BP.N - 2U)
        {
            indexTmp_0d = (uint8)(GESE_Extr_OptRampLoadMassX100_BP.N - 2U);
        }

        *rty_upper_offset = (sint16)intrp1d_s16ffu8fl(indexTmp_0d,
            fractionTmp_0d, (&(GESE_Extr_OptRampUpperOffsetAfterMDDP_LUT[0])));

        /* End of Interpolation_n-D: '<S168>/LoadMassToUpperOffset' */
        /* End of Outputs for SubSystem: '<S167>/If Action Subsystem' */
    }
    else
    {
        /* Outputs for IfAction SubSystem: '<S167>/If Action Subsystem1' incorporates:
         *  ActionPort: '<S169>/Action Port'
         */
        /* Interpolation_n-D: '<S169>/LoadMassToUpperOffset' incorporates:
         *  SignalConversion: '<S129>/TmpSignal ConversionAtload_mass_indexOutport1'
         */
        fractionTmp_0d = rtu_load_mass_index->f;
        if (rtu_load_mass_index->f < 0.0F)
        {
            fractionTmp_0d = 0.0F;
        }
        else
        {
            if (rtu_load_mass_index->f > 1.0F)
            {
                fractionTmp_0d = 1.0F;
            }
        }

        indexTmp_0d = rtu_load_mass_index->k;
        if (rtu_load_mass_index->k > GESE_Extr_OptRampLoadMassX100_BP.N - 2U)
        {
            indexTmp_0d = (uint8)(GESE_Extr_OptRampLoadMassX100_BP.N - 2U);
        }

        *rty_upper_offset = (sint16)intrp1d_s16ffu8fl(indexTmp_0d,
            fractionTmp_0d, (&(GESE_Extr_OptRampUpperOffsetBeforeMDDP_LUT[0])));

        /* End of Interpolation_n-D: '<S169>/LoadMassToUpperOffset' */
        /* End of Outputs for SubSystem: '<S167>/If Action Subsystem1' */
    }

    /* End of If: '<S167>/If' */
}

/* Function for Chart: '<S92>/Extr_HighSpd_SupervisorLogic' */
#if EXTR_VARIANT_HIGH_SPEED_PE_DISABLED

static void Extraction_enter_atomic_HIGH_SPEED_COMPLETE(void)
{
    sint32 b_previousEvent_b;

    /* Entry 'HIGH_SPEED_COMPLETE': '<S94>:51' */
    /* Event: '<S94>:27' */
    Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.Extr_HighSpd_EvCompleteEventCounter
        ++;

    /* Event: '<S94>:30' */
    b_previousEvent_b =
        Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.sfEvent;
    Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.sfEvent
        = Extraction_event_Local_EvStop;
    Extraction_c48_Extraction();
    Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.sfEvent
        = b_previousEvent_b;
}

#endif                                 /* EXTR_VARIANT_HIGH_SPEED_PE_DISABLED */

/* Function for Chart: '<S92>/Extr_HighSpd_SupervisorLogic' */
#if EXTR_VARIANT_HIGH_SPEED_PE_DISABLED

static void Extraction_enter_atomic_CONFIGURING_PULSE(void)
{
    uint16 VibMapTargSpd_b;
    boolean_T DecPlatTime_b;
    SPIN_PULSE_TYPE tmp_b;
    SPIN_PULSE_TYPE tmp_bh;
    float32 unusedExpr_b;

    /* Entry 'CONFIGURING_PULSE': '<S94>:142' */
    Extraction_LoadSpinPulse
        (Extraction_B.VariantMergeForOutportExtr_HighSpd_CurrPulseInd, &tmp_bh);
    Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.SpinPulse
        = tmp_bh;
    Extraction_VerifyVibMapFloorDisplacement
        (Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.SpinPulse.Target_Speed_Div10,
         &VibMapTargSpd_b, &DecPlatTime_b);
    tmp_b =
        Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.SpinPulse;
    Extraction_UpdatePlateauTime(&tmp_b, DecPlatTime_b, -1.0F, &unusedExpr_b);
    Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.Extr_HighSpd_TargetSpeed_ln
        = (uint16)
        (Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.SpinPulse.Target_Speed_Div10
         * 10);
    if (VibMapTargSpd_b <
            Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.Extr_HighSpd_TargetSpeed_ln)
    {
        Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.Extr_HighSpd_TargetSpeed_ln
            = VibMapTargSpd_b;
    }

    if (Extr_LowSpd_MaxTargetSpeed <
            Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.Extr_HighSpd_TargetSpeed_ln)
    {
        Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.Extr_HighSpd_TargetSpeed_ln
            = Extr_LowSpd_MaxTargetSpeed;
    }

    if (Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.Extr_HighSpd_TargetSpeed_ln
        < Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.SpinPulse.Target_Speed_Div10
        * 10)
    {
        Extr_HighSpd_Fault = EXTR_HIGH_SPD_FAULT_MAX_SPEED_REACHED;
        Extraction_SetFault(EXTR_FAULT_HIGH_SPD_SPEED_LIMIT_EXCEEDED);
    }
}

#endif                                 /* EXTR_VARIANT_HIGH_SPEED_PE_DISABLED */

/* Function for Chart: '<S92>/Extr_HighSpd_SupervisorLogic' */
#if EXTR_VARIANT_HIGH_SPEED_PE_DISABLED

static void Extraction_enter_internal_RUNNING(void)
{
    /* Entry Internal 'RUNNING': '<S94>:7' */
    /* Transition: '<S94>:12' */
    if ((*GESE_Extr_SpinProfileConfig).Spin_Pulse_Count > 0)
    {
        /* Transition: '<S94>:110' */
        if (Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.is_RUNNING
            == Extraction_IN_CONFIGURING_PULSE)
        {
        }
        else
        {
            Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.is_RUNNING
                = Extraction_IN_CONFIGURING_PULSE;
            Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.Extr_HighSpd_State_lz
                = HIGH_SPEED_STATE_CONFIGURING_PULSE;
            Extraction_enter_atomic_CONFIGURING_PULSE();
        }
    }
    else
    {
        /* Transition: '<S94>:111' */
        /* Transition: '<S94>:345' */
        /* Transition: '<S94>:162' */
        if (Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.is_RUNNING
            == Extraction_IN_HIGH_SPEED_COMPLETE)
        {
        }
        else
        {
            Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.is_RUNNING
                = Extraction_IN_HIGH_SPEED_COMPLETE;
            Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.Extr_HighSpd_State_lz
                = HIGH_SPEED_STATE_HIGH_SPEED_COMPLETE;
            Extraction_enter_atomic_HIGH_SPEED_COMPLETE();
        }
    }
}

#endif                                 /* EXTR_VARIANT_HIGH_SPEED_PE_DISABLED */

/* Function for Chart: '<S92>/Extr_HighSpd_SupervisorLogic' */
#if EXTR_VARIANT_HIGH_SPEED_PE_DISABLED

static void Extraction_RUNNING(void)
{
    sint32 tmp_b;
    uint32 qY_b;
    boolean_T guard1 = false;
    boolean_T guard2 = false;
    boolean_T guard3 = false;

    /* During 'RUNNING': '<S94>:7' */
    if (Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.sfEvent
        == Extraction_event_Extr_Sup_EvStopAll ||
            Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.sfEvent
        == Extraction_event_Local_EvStop)
    {
        /* Transition: '<S94>:20' */
        /* Exit Internal 'RUNNING': '<S94>:7' */
        Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.is_RUNNING
            = Extraction_IN_NO_ACTIVE_CHILD_cn;

        /* Event: '<S94>:81' */
        Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.Extr_HighSpd_EvStopAllEventCounter
            ++;
        Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.is_HIGH_SPEED_SUPERVISOR
            = Extraction_IN_IDLE_bo;
        Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.Extr_HighSpd_State_lz
            = HIGH_SPEED_STATE_IDLE;

        /* Entry 'IDLE': '<S94>:5' */
        Extraction_B.VariantMergeForOutportExtr_HighSpd_CurrPulseInd = 0;
    }
    else if (!(ExtractionParams__IsExtrConfigValid() != 0))
    {
        /* Transition: '<S94>:44' */
        Extr_HighSpd_Fault = EXTR_HIGH_SPD_FAULT_INVALID_SETUP;
        Extraction_SetFault(EXTR_FAULT_HIGH_SPD_INVALID_CONFIG);

        /* Exit Internal 'RUNNING': '<S94>:7' */
        Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.is_RUNNING
            = Extraction_IN_NO_ACTIVE_CHILD_cn;
        Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.is_HIGH_SPEED_SUPERVISOR
            = Extraction_IN_FAULT_INVALID_SETUP;
        Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.Extr_HighSpd_State_lz
            = HIGH_SPEED_STATE_FAULT_INVALID_SETUP;

        /* Entry 'FAULT_INVALID_SETUP': '<S94>:41' */
        Extraction_StopMotor();

        /* Event: '<S94>:27' */
        Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.Extr_HighSpd_EvCompleteEventCounter
            ++;

        /* Event: '<S94>:81' */
        Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.Extr_HighSpd_EvStopAllEventCounter
            ++;
    }
    else if (Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.sfEvent
             == Extraction_event_Extr_Sup_EvFoamDetected)
    {
        /* Transition: '<S94>:230' */
        /* Exit Internal 'RUNNING': '<S94>:7' */
        Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.is_RUNNING
            = Extraction_IN_NO_ACTIVE_CHILD_cn;
        Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.is_HIGH_SPEED_SUPERVISOR
            = Extraction_IN_NO_ACTIVE_CHILD_cn;
        Extraction_SetSpeedCommand(Extr_LowSpd_DistSpeed, 45);

        /* Event: '<S94>:81' */
        Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.Extr_HighSpd_EvStopAllEventCounter
            ++;
        Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.is_HIGH_SPEED_SUPERVISOR
            = Extraction_IN_IDLE_bo;
        Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.Extr_HighSpd_State_lz
            = HIGH_SPEED_STATE_IDLE;

        /* Entry 'IDLE': '<S94>:5' */
        Extraction_B.VariantMergeForOutportExtr_HighSpd_CurrPulseInd = 0;
    }
    else
    {
        if (Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.sfEvent
            == Extraction_event_Extr_Accel_VibMapEvExcessiveFloorDisp)
        {
            Extr_HighSpd_Fault = EXTR_HIGH_SPD_FAULT_VIB_MAP_FLOOR_DISP;
            Extraction_SetFault(EXTR_FAULT_HIGH_SPD_VIB_MAP_FLOOR_DISP);
        }

        guard1 = false;
        guard2 = false;
        guard3 = false;
        if (Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.sfEvent
            == Extraction_event_Extr_HighSpd_EvExtndChkFailed)
        {
            /* Transition: '<S94>:285' */
            Extr_HighSpd_Fault = EXTR_HIGH_SPD_FAULT_EXT_UNB_CHK_FAILED;
            Extraction_SetFault(EXTR_FAULT_HIGH_SPD_EXT_UNB_CHK_FAILED);
            guard3 = true;
        }
        else if (Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.sfEvent
                 == Extraction_event_Extr_Sup_EvSpeedErrorTimeout &&
                 Extr_Inputs_MaxSpeedError < 255)
        {
            /* Transition: '<S94>:311' */
            Extr_HighSpd_Fault = EXTR_HIGH_SPD_FAULT_SPEED_ERROR_EXCEEDED;
            Extraction_SetFault(EXTR_FAULT_SPEED_ERROR_EXCEEDED);

            /* Transition: '<S94>:313' */
            guard3 = true;
        }
        else if (Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.sfEvent
                 == Extraction_event_Extr_Accel_EvCabHit)
        {
            /* Transition: '<S94>:242' */
            Extr_HighSpd_Fault = EXTR_HIGH_SPD_FAULT_CAB_HIT_DETECTED;
            Extraction_SetFault(EXTR_FAULT_HIGH_SPD_CAB_HIT_DETECTED);
            if (Extr_HighSpd_RebalCnt >= (*GESE_Extr_HighSpdUnbConfig).
                    Max_Redistributions)
            {
                /* Transition: '<S94>:334' */
                Extraction_StopMotor();
                Extraction_SetFault
                    (EXTR_FAULT_HIGH_SPEED_REBALANCE_COUNT_EXCEEDED);

                /* Event: '<S94>:81' */
                Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.Extr_HighSpd_EvStopAllEventCounter
                    ++;

                /* Event: '<S94>:27' */
                Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.Extr_HighSpd_EvCompleteEventCounter
                    ++;

                /* Exit Internal 'RUNNING': '<S94>:7' */
                Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.is_RUNNING
                    = Extraction_IN_NO_ACTIVE_CHILD_cn;
                Extraction_enter_internal_RUNNING();
            }
            else
            {
                /* Transition: '<S94>:337' */
                guard1 = true;
            }
        }
        else
        {
            switch
                (Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.is_RUNNING)
            {
              case Extraction_IN_CONFIGURING_PULSE:
                /* During 'CONFIGURING_PULSE': '<S94>:142' */
                /* Transition: '<S94>:143' */
                Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.is_RUNNING
                    = Extraction_IN_RAMP;
                Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.Extr_HighSpd_State_lz
                    = HIGH_SPEED_STATE_RAMP;

                /* Entry 'RAMP': '<S94>:11' */
                /* Event: '<S94>:26' */
                Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.Extr_HighSpd_EvStartRampEventCounter
                    ++;
                break;

              case Extraction_IN_DIST_SPEED:
                /* During 'DIST_SPEED': '<S94>:244' */
                if (Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.sfEvent
                    == Extraction_event_Extr_LowSpd_EvComplete)
                {
                    /* Transition: '<S94>:255' */
                    /* Transition: '<S94>:295' */
                    /* Transition: '<S94>:296' */
                    qY_b = (*GESE_Extr_SpinProfileConfig).Spin_Pulse_Count - 1U;
                    if (qY_b > (*GESE_Extr_SpinProfileConfig).Spin_Pulse_Count)
                    {
                        qY_b = 0U;
                    }

                    if (Extraction_B.VariantMergeForOutportExtr_HighSpd_CurrPulseInd
                        >= (sint32)qY_b)
                    {
                        /* Transition: '<S94>:164' */
                        /* Transition: '<S94>:22' */
                        Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.is_RUNNING
                            = Extraction_IN_EXTENDED_PLATEAU_DELAY;
                        Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.temporalCounter_i1
                            = 0U;
                        Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.Extr_HighSpd_State_lz
                            = HIGH_SPEED_STATE_EXTENDED_PLATEAU_DELAY;

                        /* Entry 'EXTENDED_PLATEAU_DELAY': '<S94>:62' */
                        Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.ExtPlateauDuration
                            = Extr_Inputs_ExtPlateauDuration;
                    }
                    else
                    {
                        /* Transition: '<S94>:18' */
                        tmp_b =
                            Extraction_B.VariantMergeForOutportExtr_HighSpd_CurrPulseInd
                            + 1;
                        if (tmp_b > 127)
                        {
                            tmp_b = 127;
                        }

                        Extraction_B.VariantMergeForOutportExtr_HighSpd_CurrPulseInd
                            = (sint8)tmp_b;

                        /* Transition: '<S94>:166' */
                        Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.is_RUNNING
                            = Extraction_IN_CONFIGURING_PULSE;
                        Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.Extr_HighSpd_State_lz
                            = HIGH_SPEED_STATE_CONFIGURING_PULSE;
                        Extraction_enter_atomic_CONFIGURING_PULSE();
                    }
                }
                break;

              case Extraction_IN_EXTENDED_PLATEAU_DELAY:
                /* During 'EXTENDED_PLATEAU_DELAY': '<S94>:62' */
                if (Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.sfEvent
                    == Extraction_event_Trigger &&
                        Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.temporalCounter_i1
                    >= (uint32)
                        Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.ExtPlateauDuration
                    * ((uint16)FS_EXTRACTION))
                {
                    /* Transition: '<S94>:75' */
                    Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.is_RUNNING
                        = Extraction_IN_HIGH_SPEED_COMPLETE;
                    Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.Extr_HighSpd_State_lz
                        = HIGH_SPEED_STATE_HIGH_SPEED_COMPLETE;
                    Extraction_enter_atomic_HIGH_SPEED_COMPLETE();
                }
                else
                {
                    if (Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.sfEvent
                        == Extraction_event_Extr_Accel_VibMapEvExcessiveFloorDisp)
                    {
                        if (Extr_Accel_VibMapSafeSpeed_div10 <
                                Extr_Inputs_MotorSignals.Speed)
                        {
                            Extraction_SetSpeedCommand((sint16)
                                (Extr_Accel_VibMapSafeSpeed_div10 * 10), 30);
                        }

                        qY_b = (uint32)Extr_Inputs_ExtPlateauDuration +
                            Extr_Accel_ExtPlateauTimeIncrement;
                        if (qY_b > 65535U)
                        {
                            qY_b = 65535U;
                        }

                        Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.ExtPlateauDuration
                            = (uint16)qY_b;
                    }
                }
                break;

              case Extraction_IN_HIGH_SPEED_COMPLETE:
                /* During 'HIGH_SPEED_COMPLETE': '<S94>:51' */
                break;

              case Extraction_IN_PLATEAU_ac:
                /* During 'PLATEAU': '<S94>:13' */
                if (Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.sfEvent
                    == Extraction_event_Extr_HighSpd_EvPlateauComplete)
                {
                    /* Transition: '<S94>:247' */
                    /* Transition: '<S94>:347' */
                    if (Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.SpinPulse.Dist_Time
                        > 0)
                    {
                        /* Transition: '<S94>:248' */
                        Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.is_RUNNING
                            = Extraction_IN_DIST_SPEED;
                        Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.Extr_HighSpd_State_lz
                            = HIGH_SPEED_STATE_DIST_SPEED;

                        /* Entry 'DIST_SPEED': '<S94>:244' */
                        Extraction_B.VariantMergeForOutportExtr_HighSpd_DistSpeedTimeout
                            =
                            Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.SpinPulse.Dist_Time;

                        /* Event: '<S94>:257' */
                        Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.Extr_HighSpd_EvDropToDistSpeedEventCounter
                            ++;
                    }
                    else
                    {
                        /* Transition: '<S94>:250' */
                        /* Transition: '<S94>:253' */
                        /* Transition: '<S94>:254' */
                        /* Transition: '<S94>:295' */
                        /* Transition: '<S94>:296' */
                        qY_b = (*GESE_Extr_SpinProfileConfig).Spin_Pulse_Count -
                            1U;
                        if (qY_b > (*GESE_Extr_SpinProfileConfig).
                                Spin_Pulse_Count)
                        {
                            qY_b = 0U;
                        }

                        if (Extraction_B.VariantMergeForOutportExtr_HighSpd_CurrPulseInd
                            >= (sint32)qY_b)
                        {
                            /* Transition: '<S94>:164' */
                            /* Transition: '<S94>:22' */
                            Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.is_RUNNING
                                = Extraction_IN_EXTENDED_PLATEAU_DELAY;
                            Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.temporalCounter_i1
                                = 0U;
                            Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.Extr_HighSpd_State_lz
                                = HIGH_SPEED_STATE_EXTENDED_PLATEAU_DELAY;

                            /* Entry 'EXTENDED_PLATEAU_DELAY': '<S94>:62' */
                            Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.ExtPlateauDuration
                                = Extr_Inputs_ExtPlateauDuration;
                        }
                        else
                        {
                            /* Transition: '<S94>:18' */
                            tmp_b =
                                Extraction_B.VariantMergeForOutportExtr_HighSpd_CurrPulseInd
                                + 1;
                            if (tmp_b > 127)
                            {
                                tmp_b = 127;
                            }

                            Extraction_B.VariantMergeForOutportExtr_HighSpd_CurrPulseInd
                                = (sint8)tmp_b;

                            /* Transition: '<S94>:166' */
                            Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.is_RUNNING
                                = Extraction_IN_CONFIGURING_PULSE;
                            Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.Extr_HighSpd_State_lz
                                = HIGH_SPEED_STATE_CONFIGURING_PULSE;
                            Extraction_enter_atomic_CONFIGURING_PULSE();
                        }
                    }
                }
                break;

              case Extraction_IN_RAMP:
                /* During 'RAMP': '<S94>:11' */
                if (Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.sfEvent
                    == Extraction_event_Extr_HighSpd_EvRampComplete)
                {
                    /* Transition: '<S94>:14' */
                    guard2 = true;
                }
                else
                {
                    if (Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.sfEvent
                        == Extraction_event_Extr_Sup_EvSpeedErrorTimeout)
                    {
                        /* Transition: '<S94>:340' */
                        Extr_HighSpd_Fault =
                            EXTR_HIGH_SPD_FAULT_SPEED_ERROR_EXCEEDED;
                        Extraction_SetFault(EXTR_FAULT_SPEED_ERROR_EXCEEDED);
                        guard2 = true;
                    }
                }
                break;
            }
        }

        if (guard3)
        {
            /* Transition: '<S94>:331' */
            if (Extr_HighSpd_RebalCnt < (*GESE_Extr_HighSpdUnbConfig).
                    Max_Redistributions)
            {
                /* Transition: '<S94>:336' */
                guard1 = true;
            }
            else
            {
                /* Transition: '<S94>:300' */
                /*  Rebalance count exceeded */
                Extraction_SetFault
                    (EXTR_FAULT_HIGH_SPEED_REBALANCE_COUNT_EXCEEDED);

                /* Transition: '<S94>:86' */
                /* Exit Internal 'RUNNING': '<S94>:7' */
                Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.is_RUNNING
                    = Extraction_IN_NO_ACTIVE_CHILD_cn;
                if (Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.is_RUNNING
                    == Extraction_IN_EXTENDED_PLATEAU_DELAY)
                {
                }
                else
                {
                    Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.is_RUNNING
                        = Extraction_IN_EXTENDED_PLATEAU_DELAY;
                    Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.temporalCounter_i1
                        = 0U;
                    Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.Extr_HighSpd_State_lz
                        = HIGH_SPEED_STATE_EXTENDED_PLATEAU_DELAY;

                    /* Entry 'EXTENDED_PLATEAU_DELAY': '<S94>:62' */
                    Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.ExtPlateauDuration
                        = Extr_Inputs_ExtPlateauDuration;
                }
            }
        }

        if (guard2)
        {
            Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.is_RUNNING
                = Extraction_IN_PLATEAU_ac;
            Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.Extr_HighSpd_State_lz
                = HIGH_SPEED_STATE_PLATEAU;

            /* Entry 'PLATEAU': '<S94>:13' */
            /* Event: '<S94>:25' */
            Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.Extr_HighSpd_EvStartPlateauEventCounter
                ++;
        }

        if (guard1)
        {
            /* Transition: '<S94>:68' */
            /* Exit Internal 'RUNNING': '<S94>:7' */
            Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.is_RUNNING
                = Extraction_IN_NO_ACTIVE_CHILD_cn;
            Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.is_HIGH_SPEED_SUPERVISOR
                = Extraction_IN_REBALANCE;
            Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.Extr_HighSpd_State_lz
                = HIGH_SPEED_STATE_REBALANCE;

            /* Entry 'REBALANCE': '<S94>:138' */
            tmp_b = (sint32)(Extr_HighSpd_RebalCnt + 1U);
            if ((uint32)tmp_b > 255U)
            {
                tmp_b = 255;
            }

            Extr_HighSpd_RebalCnt = (uint8)tmp_b;

            /* Event: '<S94>:74' */
            Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.Extr_HighSpd_EvStartRebalEventCounter
                ++;

            /* Event: '<S94>:81' */
            Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.Extr_HighSpd_EvStopAllEventCounter
                ++;
        }
    }
}

#endif                                 /* EXTR_VARIANT_HIGH_SPEED_PE_DISABLED */

/* Function for Chart: '<S92>/Extr_HighSpd_SupervisorLogic' */
#if EXTR_VARIANT_HIGH_SPEED_PE_DISABLED

static void Extraction_c48_Extraction(void)
{
    /* During: Extraction/Extr_HighSpd/Extr_HighSpd_Supervisor/Extr_HighSpd_Supervisor/Extr_HighSpd_SupervisorLogic/Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled/Extr_HighSpd_SupervisorLogic */
    if (Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.is_active_c48_Extraction
        == 0U)
    {
        /* Entry: Extraction/Extr_HighSpd/Extr_HighSpd_Supervisor/Extr_HighSpd_Supervisor/Extr_HighSpd_SupervisorLogic/Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled/Extr_HighSpd_SupervisorLogic */
        Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.is_active_c48_Extraction
            = 1U;

        /* Entry Internal: Extraction/Extr_HighSpd/Extr_HighSpd_Supervisor/Extr_HighSpd_Supervisor/Extr_HighSpd_SupervisorLogic/Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled/Extr_HighSpd_SupervisorLogic */
        /* Transition: '<S94>:291' */
        Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.is_c48_Extraction
            = Extraction_IN_HIGH_SPEED_SUPERVISOR;

        /* Entry Internal 'HIGH_SPEED_SUPERVISOR': '<S94>:290' */
        /* Transition: '<S94>:6' */
        if (Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.is_HIGH_SPEED_SUPERVISOR
            == Extraction_IN_IDLE_bo)
        {
        }
        else
        {
            Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.is_HIGH_SPEED_SUPERVISOR
                = Extraction_IN_IDLE_bo;
            Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.Extr_HighSpd_State_lz
                = HIGH_SPEED_STATE_IDLE;

            /* Entry 'IDLE': '<S94>:5' */
            Extraction_B.VariantMergeForOutportExtr_HighSpd_CurrPulseInd = 0;
        }
    }
    else if (Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.is_c48_Extraction
             != Extraction_IN_HIGH_SPEED_SUPERVISOR)
    {
    }
    else
    {
        /* During 'HIGH_SPEED_SUPERVISOR': '<S94>:290' */
        if (Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.sfEvent
            == Extraction_event_Extr_Sup_ResetRebalCnt)
        {
            Extr_HighSpd_RebalCnt = 0U;
        }

        if (Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.sfEvent
            == Extraction_event_Extr_Sup_ClearFault)
        {
            Extr_HighSpd_Fault = EXTR_HIGH_SPD_FAULT_NONE;
        }

        switch
            (Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.is_HIGH_SPEED_SUPERVISOR)
        {
          case Extraction_IN_FAULT_INVALID_SETUP:
            /* During 'FAULT_INVALID_SETUP': '<S94>:41' */
            /* Transition: '<S94>:46' */
            Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.is_HIGH_SPEED_SUPERVISOR
                = Extraction_IN_IDLE_bo;
            Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.Extr_HighSpd_State_lz
                = HIGH_SPEED_STATE_IDLE;

            /* Entry 'IDLE': '<S94>:5' */
            Extraction_B.VariantMergeForOutportExtr_HighSpd_CurrPulseInd = 0;
            break;

          case Extraction_IN_IDLE_bo:
            /* During 'IDLE': '<S94>:5' */
            if (Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.sfEvent
                == Extraction_event_Extr_Sup_EvHighSpdStart)
            {
                /* Transition: '<S94>:10' */
                Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.is_HIGH_SPEED_SUPERVISOR
                    = Extraction_IN_RUNNING;

                /* Entry 'RUNNING': '<S94>:7' */
                Extr_HighSpd_ReferenceFriction = 0.0F;
                Extraction_B.VariantMergeForOutportExtr_HighSpd_CurrPulseInd = 0;
                Extraction_enter_internal_RUNNING();
            }
            break;

          case Extraction_IN_REBALANCE:
            /* During 'REBALANCE': '<S94>:138' */
            switch
                (Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.sfEvent)
            {
              case Extraction_event_Extr_Sup_EvHighSpdStart:
                /* Transition: '<S94>:139' */
                Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.is_HIGH_SPEED_SUPERVISOR
                    = Extraction_IN_RUNNING;

                /* Entry 'RUNNING': '<S94>:7' */
                Extr_HighSpd_ReferenceFriction = 0.0F;
                Extraction_B.VariantMergeForOutportExtr_HighSpd_CurrPulseInd = 0;
                Extraction_enter_internal_RUNNING();
                break;

              case Extraction_event_Extr_Sup_EvStopAll:
                /* Transition: '<S94>:141' */
                Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.is_HIGH_SPEED_SUPERVISOR
                    = Extraction_IN_IDLE_bo;
                Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.Extr_HighSpd_State_lz
                    = HIGH_SPEED_STATE_IDLE;

                /* Entry 'IDLE': '<S94>:5' */
                Extraction_B.VariantMergeForOutportExtr_HighSpd_CurrPulseInd = 0;
                break;
            }
            break;

          case Extraction_IN_RUNNING:
            Extraction_RUNNING();
            break;
        }
    }
}

#endif                                 /* EXTR_VARIANT_HIGH_SPEED_PE_DISABLED */

/* Function for Chart: '<S92>/Extr_HighSpd_SupervisorLogic' */
#if EXTR_VARIANT_HIGH_SPEED_PE_DISABLED

static void Extraction_chartstep_c48_Extraction(void)
{
    Extraction_c48_Extraction();
}

#endif                                 /* EXTR_VARIANT_HIGH_SPEED_PE_DISABLED */

/* Output and update for Simulink Function: '<S93>/ClearBaselineFrictionArrays' */
#if EXTR_VARIANT_HIGH_SPEED_PE_ENABLED

void Extraction_ClearBaselineFrictionArrays(void)
{
    sint32 ii;

    /* MATLAB Function: '<S95>/ClearBaselineFrictionArraysMATLAB' incorporates:
     *  Constant: '<S95>/BaselineArrayMaxSizeConst'
     */
    /* MATLAB Function 'Extraction/Extr_HighSpd/Extr_HighSpd_Supervisor/Extr_HighSpd_Supervisor/Extr_HighSpd_SupervisorLogic/Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled/ClearBaselineFrictionArrays/ClearBaselineFrictionArraysMATLAB': '<S102>:1' */
    /* '<S102>:1:6' */
    for (ii = 0; ii + 1 <= ((sint32)BASELINE_ARRAY_MAX_SIZE); ii++)
    {
        /* '<S102>:1:6' */
        /* '<S102>:1:7' */
        ExtractionParams__SetBaselineFriction( ii , MAX_uint16_T);

        /* '<S102>:1:8' */
        ExtractionParams__SetFactoryBaselineFriction( ii , MAX_uint16_T);
    }

    /* End of MATLAB Function: '<S95>/ClearBaselineFrictionArraysMATLAB' */
}

#endif                                 /* EXTR_VARIANT_HIGH_SPEED_PE_ENABLED */

/* Output and update for Simulink Function: '<S93>/DecreaseCurrentBaselineFriction' */
#if EXTR_VARIANT_HIGH_SPEED_PE_ENABLED

void Extraction_DecreaseCurrentBaselineFriction(sint8 rtu_pulse_ind)
{
    /* MATLAB Function: '<S96>/DecreaseCurrentBaselineFrictionMATLAB' incorporates:
     *  SignalConversion: '<S96>/TmpLatchAtExtr_HighSpd_BaselineConfigOutport1'
     *  SignalConversion: '<S96>/TmpSignal ConversionAtpulse_indOutport1'
     */
    /* MATLAB Function 'Extraction/Extr_HighSpd/Extr_HighSpd_Supervisor/Extr_HighSpd_Supervisor/Extr_HighSpd_SupervisorLogic/Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled/DecreaseCurrentBaselineFriction/DecreaseCurrentBaselineFrictionMATLAB': '<S103>:1' */
    /*  Req 250641 */
    /* '<S103>:1:7' */
    ExtractionParams__SetBaselineFriction( rtu_pulse_ind - 1 , (uint16)(sint32)
        roundf((float32)ExtractionParams__GetBaselineFriction( rtu_pulse_ind - 1
        ) * (1.0F - (float32)(*GESE_Extr_CEMSetup).Static_Decrement_x10 / 10.0F /
             100.0F)));

    /*  Req 255259 */
    if (ExtractionParams__GetBaselineFriction( rtu_pulse_ind - 1 ) >
            Extraction_B.VariantMergeForOutportExtr_HighSpd_BaselineConfig.Friction_Max_x1000)
    {
        /* '<S103>:1:11' */
        /* '<S103>:1:12' */
        ExtractionParams__SetBaselineFriction( rtu_pulse_ind - 1 ,
            Extraction_B.VariantMergeForOutportExtr_HighSpd_BaselineConfig.Friction_Max_x1000);
    }
    else
    {
        /*  Req 255258 */
        if (ExtractionParams__GetBaselineFriction( rtu_pulse_ind - 1 ) <
                Extraction_B.VariantMergeForOutportExtr_HighSpd_BaselineConfig.Friction_Min_x1000)
        {
            /* '<S103>:1:15' */
            /* '<S103>:1:16' */
            ExtractionParams__SetBaselineFriction( rtu_pulse_ind - 1 ,
                Extraction_B.VariantMergeForOutportExtr_HighSpd_BaselineConfig.Friction_Min_x1000);
        }
    }

    /* End of MATLAB Function: '<S96>/DecreaseCurrentBaselineFrictionMATLAB' */
}

#endif                                 /* EXTR_VARIANT_HIGH_SPEED_PE_ENABLED */

/* Output and update for Simulink Function: '<S93>/IncreaseCEMEventCounter' */
#if EXTR_VARIANT_HIGH_SPEED_PE_ENABLED

void Extraction_IncreaseCEMEventCounter(void)
{
    uint16 Extr_HighSpd_CEMEventCounter_0;

    /* DataStoreWrite: '<S98>/Data Store Write' */
    Extr_HighSpd_CEMEventCounter_0 = Extr_HighSpd_CEMEventCounter;

    /* Sum: '<S98>/Sum' incorporates:
     *  Constant: '<S98>/Constant'
     */
    Extr_HighSpd_CEMEventCounter_0++;

    /* DataStoreWrite: '<S98>/Data Store Write' */
    Extr_HighSpd_CEMEventCounter = Extr_HighSpd_CEMEventCounter_0;
}

#endif                                 /* EXTR_VARIANT_HIGH_SPEED_PE_ENABLED */

/* Output and update for Simulink Function: '<S93>/IncreaseCurrentBaselineFriction' */
#if EXTR_VARIANT_HIGH_SPEED_PE_ENABLED

void Extraction_IncreaseCurrentBaselineFriction(sint8 rtu_pulse_ind)
{
    float32 tmp;

    /* MATLAB Function: '<S99>/IncreaseCurrentBaselineFrictionMATLAB' incorporates:
     *  SignalConversion: '<S99>/TmpLatchAtExtr_HighSpd_BaselineConfigOutport1'
     *  SignalConversion: '<S99>/TmpSignal ConversionAtpulse_indOutport1'
     */
    /* MATLAB Function 'Extraction/Extr_HighSpd/Extr_HighSpd_Supervisor/Extr_HighSpd_Supervisor/Extr_HighSpd_SupervisorLogic/Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled/IncreaseCurrentBaselineFriction/IncreaseCurrentBaselineFrictionMATLAB': '<S104>:1' */
    /*  Req 250641 */
    /* '<S104>:1:7' */
    tmp = roundf((float32)ExtractionParams__GetBaselineFriction( rtu_pulse_ind -
                  1 ) * (1.0F + (float32)(*GESE_Extr_CEMSetup).
                         Static_Increment_x10 / 10.0F / 100.0F));
    if (tmp < 65536.0F)
    {
        ExtractionParams__SetBaselineFriction( rtu_pulse_ind - 1 , (uint16)tmp);
    }
    else
    {
        ExtractionParams__SetBaselineFriction( rtu_pulse_ind - 1 , MAX_uint16_T);
    }

    /*  Req 255259 */
    if (ExtractionParams__GetBaselineFriction( rtu_pulse_ind - 1 ) >
            Extraction_B.VariantMergeForOutportExtr_HighSpd_BaselineConfig.Friction_Max_x1000)
    {
        /* '<S104>:1:11' */
        /* '<S104>:1:12' */
        ExtractionParams__SetBaselineFriction( rtu_pulse_ind - 1 ,
            Extraction_B.VariantMergeForOutportExtr_HighSpd_BaselineConfig.Friction_Max_x1000);
    }
    else
    {
        /*  Req 255258 */
        if (ExtractionParams__GetBaselineFriction( rtu_pulse_ind - 1 ) <
                Extraction_B.VariantMergeForOutportExtr_HighSpd_BaselineConfig.Friction_Min_x1000)
        {
            /* '<S104>:1:15' */
            /* '<S104>:1:16' */
            ExtractionParams__SetBaselineFriction( rtu_pulse_ind - 1 ,
                Extraction_B.VariantMergeForOutportExtr_HighSpd_BaselineConfig.Friction_Min_x1000);
        }
    }

    /* End of MATLAB Function: '<S99>/IncreaseCurrentBaselineFrictionMATLAB' */
}

#endif                                 /* EXTR_VARIANT_HIGH_SPEED_PE_ENABLED */

/* Output and update for Simulink Function: '<S93>/UpdateBaselineFrictionArray' */
#if EXTR_VARIANT_HIGH_SPEED_PE_ENABLED

void Extraction_UpdateBaselineFrictionArray(void)
{
    uint8 ii;
    float32 tmp;
    sint32 tmp_0;

    /* MATLAB Function: '<S100>/UpdateBaselineFrictionArrayMATLAB' */
    /* MATLAB Function 'Extraction/Extr_HighSpd/Extr_HighSpd_Supervisor/Extr_HighSpd_Supervisor/Extr_HighSpd_SupervisorLogic/Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled/UpdateBaselineFrictionArray/UpdateBaselineFrictionArrayMATLAB': '<S105>:1' */
    /* '<S105>:1:10' */
    for (ii = 1U; ii <= GESE_Extr_BaselineFrictionRefSpeed_Sig.N; ii++)
    {
        /* '<S105>:1:10' */
        /* '<S105>:1:11' */
        tmp_0 = (sint32)(1U + GESE_Extr_FactoryFrictionSelIndexArray[ii - 1]);
        if ((uint32)tmp_0 > 255U)
        {
            tmp_0 = 255;
        }

        if (ExtractionParams__GetFactoryBaselineFriction( tmp_0 - 1 ) < 65535)
        {
            /* '<S105>:1:13' */
            tmp_0 = (sint32)(1U + GESE_Extr_FactoryFrictionSelIndexArray[ii - 1]);
            if ((uint32)tmp_0 > 255U)
            {
                tmp_0 = 255;
            }

            if (ExtractionParams__GetFactoryBaselineFriction( tmp_0 - 1 ) > 0)
            {
                /* '<S105>:1:13' */
                /* '<S105>:1:14' */
                tmp_0 = (sint32)(1U + GESE_Extr_FactoryFrictionSelIndexArray[ii
                                 - 1]);
                if ((uint32)tmp_0 > 255U)
                {
                    tmp_0 = 255;
                }

                tmp = roundf((float32)GESE_Extr_FactoryCalScalingFactorArray[ii
                             - 1] / 100.0F * (float32)
                             ExtractionParams__GetFactoryBaselineFriction( tmp_0
                              - 1 ));
                if (tmp < 65536.0F)
                {
                    ExtractionParams__SetBaselineFriction( ii - 1 , (uint16)tmp);
                }
                else
                {
                    ExtractionParams__SetBaselineFriction( ii - 1 , MAX_uint16_T);
                }
            }
            else
            {
                /* '<S105>:1:19' */
                ExtractionParams__SetBaselineFriction( ii - 1 ,
                    GESE_Extr_BaselineDefaultFrictionArray_x1000[ii - 1]);
            }
        }
        else
        {
            /* '<S105>:1:19' */
            ExtractionParams__SetBaselineFriction( ii - 1 ,
                GESE_Extr_BaselineDefaultFrictionArray_x1000[ii - 1]);
        }
    }

    /* End of MATLAB Function: '<S100>/UpdateBaselineFrictionArrayMATLAB' */
}

#endif                                 /* EXTR_VARIANT_HIGH_SPEED_PE_ENABLED */

/* Output and update for Simulink Function: '<S93>/VerifyBaselineFrictionArray' */
#if EXTR_VARIANT_HIGH_SPEED_PE_ENABLED

void Extraction_VerifyBaselineFrictionArray(void)
{
    uint8 ii;
    float32 tmp;
    sint32 tmp_0;

    /* MATLAB Function: '<S101>/VerifyBaselineFrictionArrayMATLAB' */
    /* MATLAB Function 'Extraction/Extr_HighSpd/Extr_HighSpd_Supervisor/Extr_HighSpd_Supervisor/Extr_HighSpd_SupervisorLogic/Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled/VerifyBaselineFrictionArray/VerifyBaselineFrictionArrayMATLAB': '<S106>:1' */
    /* '<S106>:1:10' */
    for (ii = 1U; ii <= GESE_Extr_BaselineFrictionRefSpeed_Sig.N; ii++)
    {
        /* '<S106>:1:10' */
        if (ExtractionParams__GetBaselineFriction( ii - 1 ) == 65535 ||
                ExtractionParams__GetBaselineFriction( ii - 1 ) == 0)
        {
            /* '<S106>:1:11' */
            /* '<S106>:1:12' */
            /*         %% Load factory friction value as pointed by the friction selection array */
            /* '<S106>:1:14' */
            /*         %% Check if the factory friction value is valid */
            tmp_0 = (sint32)(1U + GESE_Extr_FactoryFrictionSelIndexArray[ii - 1]);
            if ((uint32)tmp_0 > 255U)
            {
                tmp_0 = 255;
            }

            if (ExtractionParams__GetFactoryBaselineFriction( tmp_0 - 1 ) <
                    65535)
            {
                /* '<S106>:1:17' */
                tmp_0 = (sint32)(1U + GESE_Extr_FactoryFrictionSelIndexArray[ii
                                 - 1]);
                if ((uint32)tmp_0 > 255U)
                {
                    tmp_0 = 255;
                }

                if (ExtractionParams__GetFactoryBaselineFriction( tmp_0 - 1 ) >
                        0)
                {
                    /* '<S106>:1:17' */
                    /*             %% Scale the factory friction value using the scaling array */
                    /* '<S106>:1:19' */
                    /* '<S106>:1:21' */
                    tmp_0 = (sint32)(1U +
                                     GESE_Extr_FactoryFrictionSelIndexArray[ii -
                                     1]);
                    if ((uint32)tmp_0 > 255U)
                    {
                        tmp_0 = 255;
                    }

                    tmp = roundf((float32)
                                 GESE_Extr_FactoryCalScalingFactorArray[ii - 1] /
                                 100.0F * (float32)
                                 ExtractionParams__GetFactoryBaselineFriction
                                 ( tmp_0 - 1 ));
                    if (tmp < 65536.0F)
                    {
                        ExtractionParams__SetBaselineFriction( ii - 1 , (uint16)
                            tmp);
                    }
                    else
                    {
                        ExtractionParams__SetBaselineFriction( ii - 1 ,
                            MAX_uint16_T);
                    }
                }
                else
                {
                    /* '<S106>:1:23' */
                    ExtractionParams__SetBaselineFriction( ii - 1 ,
                        GESE_Extr_BaselineDefaultFrictionArray_x1000[ii - 1]);
                }
            }
            else
            {
                /* '<S106>:1:23' */
                ExtractionParams__SetBaselineFriction( ii - 1 ,
                    GESE_Extr_BaselineDefaultFrictionArray_x1000[ii - 1]);
            }
        }
    }

    /* End of MATLAB Function: '<S101>/VerifyBaselineFrictionArrayMATLAB' */
}

#endif                                 /* EXTR_VARIANT_HIGH_SPEED_PE_ENABLED */

/* Function for Chart: '<S93>/Extr_HighSpd_SupervisorLogic' */
#if EXTR_VARIANT_HIGH_SPEED_PE_ENABLED

static void Extraction_broadcast_Local_EvStop(void)
{
    sint32 b_previousEvent_l;

    /* Event: '<S97>:30' */
    b_previousEvent_l =
        Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.sfEvent;
    Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.sfEvent =
        Extraction_event_Local_EvStop_bj;
    Extraction_c17_Extraction();
    Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.sfEvent =
        b_previousEvent_l;
}

#endif                                 /* EXTR_VARIANT_HIGH_SPEED_PE_ENABLED */

/* Function for Chart: '<S93>/Extr_HighSpd_SupervisorLogic' */
#if EXTR_VARIANT_HIGH_SPEED_PE_ENABLED

static void Extraction_enter_atomic_HIGH_SPEED_COMPLETE_dq(void)
{
    /* Entry 'HIGH_SPEED_COMPLETE': '<S97>:51' */
    /* Event: '<S97>:27' */
    Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.Extr_HighSpd_EvCompleteEventCounter
        ++;
    Extraction_broadcast_Local_EvStop();
}

#endif                                 /* EXTR_VARIANT_HIGH_SPEED_PE_ENABLED */

/* Function for Chart: '<S93>/Extr_HighSpd_SupervisorLogic' */
#if EXTR_VARIANT_HIGH_SPEED_PE_ENABLED

static void Extraction_enter_atomic_CONFIGURING_PULSE_ad(void)
{
    /* Entry 'CONFIGURING_PULSE': '<S97>:142' */
    Extraction_LoadSpinPulse
        (Extraction_B.VariantMergeForOutportExtr_HighSpd_CurrPulseInd,
         &Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.r1);
    Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.SpinPulse
        = Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.r1;
    if ((*GESE_Extr_SpinProfileConfig).High_Speed_Enabled != 0)
    {
        Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.uv0[0]
            = 0U;
        Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.uv0[1]
            = 0U;
        Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.uv0[2]
            = 0U;
        Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.uv0[3]
            = 0U;
        Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.uv0[4]
            = 0U;
        Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.uv0[5]
            = 0U;
        Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.uv0[6]
            = 0U;
        Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.uv0[7]
            = 0U;
        Extraction_SelectParamEstOutputs
            (Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.uv0);
        Extraction_UpdateParamEstSetup
            (Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.SpinPulse.Param_Est_Config_Set_Index);
    }

    Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.PrevTargetSpeed
        =
        Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.Extr_HighSpd_TargetSpeed_ep;
    Extraction_VerifyVibMapFloorDisplacement
        (Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.SpinPulse.Target_Speed_Div10,
         &Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.VibMapTargSpd,
         &Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.DecPlatTime);
    Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.r0 =
        Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.SpinPulse;
    Extraction_UpdatePlateauTime
        (&Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.r0,
         Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.DecPlatTime,
         -1.0F,
         &Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.PlateauDuration);
    Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.Extr_HighSpd_TargetSpeed_ep
        = (uint16)
        (Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.SpinPulse.Target_Speed_Div10
         * 10);
    if (Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.VibMapTargSpd
        < Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.Extr_HighSpd_TargetSpeed_ep)
    {
        Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.Extr_HighSpd_TargetSpeed_ep
            =
            Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.VibMapTargSpd;
    }

    if (Extr_LowSpd_MaxTargetSpeed <
            Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.Extr_HighSpd_TargetSpeed_ep)
    {
        Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.Extr_HighSpd_TargetSpeed_ep
            = Extr_LowSpd_MaxTargetSpeed;
    }

    if (Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.SpinPulse.Target_Speed_Div10
        * 10 >
            Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.Extr_HighSpd_TargetSpeed_ep)
    {
        if (Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.PrevTargetSpeed
            > 0)
        {
            Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.Extr_HighSpd_TargetSpeed_ep
                =
                Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.PrevTargetSpeed;
        }
        else
        {
            Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.i0
                = Extr_LowSpd_DistSpeed;
            if (Extr_LowSpd_DistSpeed < 0)
            {
                Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.i0
                    = 0;
            }

            Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.Extr_HighSpd_TargetSpeed_ep
                = (uint16)
                Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.i0;
        }

        Extr_HighSpd_Fault = EXTR_HIGH_SPD_FAULT_MAX_SPEED_REACHED;
        Extraction_SetFault(EXTR_FAULT_HIGH_SPD_SPEED_LIMIT_EXCEEDED);
    }

    if (Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.PlateauDecision
        != HIGH_SPEED_PLATEAU_DECISION_REGRESS)
    {
        Extraction_B.VariantMergeForOutportExtr_HighSpd_RegressDecisionOnPrevPulse
            = false;
    }
}

#endif                                 /* EXTR_VARIANT_HIGH_SPEED_PE_ENABLED */

/* Function for Chart: '<S93>/Extr_HighSpd_SupervisorLogic' */
#if EXTR_VARIANT_HIGH_SPEED_PE_ENABLED

static void Extraction_enter_internal_RUNNING_n0(void)
{
    /* Entry Internal 'RUNNING': '<S97>:7' */
    /* Transition: '<S97>:12' */
    if ((*GESE_Extr_SpinProfileConfig).Spin_Pulse_Count > 0)
    {
        /* Transition: '<S97>:110' */
        if (Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.is_RUNNING
            == Extraction_IN_CONFIGURING_PULSE_l3)
        {
        }
        else
        {
            Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.is_RUNNING
                = Extraction_IN_CONFIGURING_PULSE_l3;
            Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.Extr_HighSpd_State_la
                = HIGH_SPEED_STATE_CONFIGURING_PULSE;
            Extraction_enter_atomic_CONFIGURING_PULSE_ad();
        }
    }
    else
    {
        /* Transition: '<S97>:111' */
        /* Transition: '<S97>:162' */
        if (Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.is_RUNNING
            == Extraction_IN_HIGH_SPEED_COMPLETE_fn)
        {
        }
        else
        {
            Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.is_RUNNING
                = Extraction_IN_HIGH_SPEED_COMPLETE_fn;
            Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.Extr_HighSpd_State_la
                = HIGH_SPEED_STATE_HIGH_SPEED_COMPLETE;
            Extraction_enter_atomic_HIGH_SPEED_COMPLETE_dq();
        }
    }
}

#endif                                 /* EXTR_VARIANT_HIGH_SPEED_PE_ENABLED */

/* Function for Chart: '<S93>/Extr_HighSpd_SupervisorLogic' */
#if EXTR_VARIANT_HIGH_SPEED_PE_ENABLED

static void Extraction_broadcast_EvStartRebal(void)
{
    sint32 b_previousEvent_l;

    /* Event: '<S97>:156' */
    b_previousEvent_l =
        Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.sfEvent;
    Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.sfEvent =
        Extraction_event_EvStartRebal;
    Extraction_c17_Extraction();
    Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.sfEvent =
        b_previousEvent_l;
}

#endif                                 /* EXTR_VARIANT_HIGH_SPEED_PE_ENABLED */

/* Function for Chart: '<S93>/Extr_HighSpd_SupervisorLogic' */
#if EXTR_VARIANT_HIGH_SPEED_PE_ENABLED

static void Extraction_EXTENDED_PLATEAU(void)
{
    boolean_T b_sf_internal_predicateOutput_l;
    uint16 y_l;
    uint16 x_l;
    SPIN_PULSE_TYPE tmp_l;
    float32 unusedExpr_l;
    uint32 qY_l;

    /* During 'EXTENDED_PLATEAU': '<S97>:420' */
    unusedExpr_l = roundf
        (Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.ExtPlateauElapsedTime);
    if (unusedExpr_l < 65536.0F)
    {
        if (unusedExpr_l >= 0.0F)
        {
            y_l = (uint16)unusedExpr_l;
        }
        else
        {
            y_l = 0U;
        }
    }
    else
    {
        y_l = MAX_uint16_T;
    }

    if (y_l >=
            Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.ExtPlateauDuration)
    {
        /* Transition: '<S97>:75' */
        if (Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.FinalPlateauVerificationRequired
            && !Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.DeltaSpeedDropExecuted
            && !Extraction_B.VariantMergeForOutportExtr_HighSpd_RegressDecisionOnPrevPulse
            && Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.PlateauDecision
            != HIGH_SPEED_PLATEAU_DECISION_ADVANCE)
        {
            /* Transition: '<S97>:200' */
            /* Transition: '<S97>:202' */
            /* Transition: '<S97>:203' */
            /* Exit Internal 'EXTENDED_PLATEAU': '<S97>:420' */
            Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.is_EXTENDED_PLATEAU
                = Extraction_IN_NO_ACTIVE_CHILD_fr;
            Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.FinalPlateauVerificationRequired
                = false;
            Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.is_RUNNING
                = Extraction_IN_PLATEAU_mt;
            Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.Extr_HighSpd_State_la
                = HIGH_SPEED_STATE_PLATEAU;

            /* Entry 'PLATEAU': '<S97>:13' */
            /* Event: '<S97>:25' */
            Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.Extr_HighSpd_EvStartPlateauEventCounter
                ++;
        }
        else
        {
            /* Transition: '<S97>:198' */
            /* Exit Internal 'EXTENDED_PLATEAU': '<S97>:420' */
            Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.is_EXTENDED_PLATEAU
                = Extraction_IN_NO_ACTIVE_CHILD_fr;
            Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.is_RUNNING
                = Extraction_IN_HIGH_SPEED_COMPLETE_fn;
            Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.Extr_HighSpd_State_la
                = HIGH_SPEED_STATE_HIGH_SPEED_COMPLETE;
            Extraction_enter_atomic_HIGH_SPEED_COMPLETE_dq();
        }
    }
    else
    {
        if (Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.sfEvent
            == Extraction_event_Extr_Accel_VibMapEvExcessiveFloorDisp_ed &&
                !Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.ExtPlateauDurationIncreased)
        {
            y_l = (uint16)
                (Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.Extr_HighSpd_TargetSpeed_ep
                 / 10U);
            x_l = (uint16)((uint32)
                           Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.Extr_HighSpd_TargetSpeed_ep
                           - (uint16)(y_l * 10U));
            if (x_l > 0 && x_l >= 5)
            {
                y_l++;
            }

            if (y_l > 255)
            {
                y_l = 255U;
            }

            b_sf_internal_predicateOutput_l = Extr_Accel_VibMapSafeSpeed_div10
                != y_l;
        }
        else
        {
            b_sf_internal_predicateOutput_l = false;
        }

        if (b_sf_internal_predicateOutput_l)
        {
            /* Transition: '<S97>:423' */
            Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.Extr_HighSpd_TargetSpeed_ep
                = (uint16)(Extr_Accel_VibMapSafeSpeed_div10 * 10);
            qY_l = (uint32)Extr_Inputs_ExtPlateauDuration +
                Extr_Accel_ExtPlateauTimeIncrement;
            if (qY_l > 65535U)
            {
                qY_l = 65535U;
            }

            Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.ExtPlateauDuration
                = (uint16)qY_l;
            Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.ExtPlateauDurationIncreased
                = true;

            /* Exit Internal 'EXTENDED_PLATEAU': '<S97>:420' */
            Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.is_EXTENDED_PLATEAU
                = Extraction_IN_NO_ACTIVE_CHILD_fr;
            if (Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.is_EXTENDED_PLATEAU
                == Extraction_IN_EXTENDED_PLATEAU_RAMPING)
            {
            }
            else
            {
                Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.is_EXTENDED_PLATEAU
                    = Extraction_IN_EXTENDED_PLATEAU_RAMPING;
                Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.Extr_HighSpd_State_la
                    = HIGH_SPEED_STATE_EXTENDED_PLATEAU_RAMPING;

                /* Entry 'EXTENDED_PLATEAU_RAMPING': '<S97>:422' */
                if (Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.Extr_HighSpd_TargetSpeed_ep
                    < Extr_Inputs_MotorSignals.Speed)
                {
                    y_l =
                        Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.Extr_HighSpd_TargetSpeed_ep;
                    if (Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.Extr_HighSpd_TargetSpeed_ep
                        > 32767)
                    {
                        y_l = 32767U;
                    }

                    Extraction_SetSpeedCommand((sint16)y_l, 30);
                }
            }
        }
        else
        {
            switch
                (Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.is_EXTENDED_PLATEAU)
            {
              case Extraction_IN_EXTENDED_PLATEAU_DELAY_ps:
                /* During 'EXTENDED_PLATEAU_DELAY': '<S97>:62' */
                if (Extr_HighSpd_PlateauDecision ==
                        HIGH_SPEED_PLATEAU_DECISION_REGRESS &&
                        !Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.DeltaSpeedDropExecuted)
                {
                    /* Transition: '<S97>:323' */
                    /* Transition: '<S97>:324' */
                    unusedExpr_l = roundf(Extr_Inputs_MotorSignals.Speed);
                    if (unusedExpr_l < 65536.0F)
                    {
                        if (unusedExpr_l >= 0.0F)
                        {
                            y_l = (uint16)unusedExpr_l;
                        }
                        else
                        {
                            y_l = 0U;
                        }
                    }
                    else
                    {
                        y_l = MAX_uint16_T;
                    }

                    qY_l = (uint32)y_l - 5 * (*GESE_Extr_HighSpdConfig).
                        Ext_Plateau_Delta_Speed_Drop_Div5;
                    if (qY_l > y_l)
                    {
                        qY_l = 0U;
                    }

                    Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.Extr_HighSpd_TargetSpeed_ep
                        = (uint16)qY_l;
                    Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.DeltaSpeedDropExecuted
                        = true;

                    /* Transition: '<S97>:268' */
                    /* Transition: '<S97>:228' */
                    Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.is_EXTENDED_PLATEAU
                        = Extraction_IN_NO_ACTIVE_CHILD_fr;
                    Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.is_RUNNING
                        = Extraction_IN_EXTENDED_PLATEAU;

                    /* Entry 'EXTENDED_PLATEAU': '<S97>:420' */
                    Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.ExtPlateauDuration
                        = Extr_Inputs_ExtPlateauDuration;
                    tmp_l =
                        Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.SpinPulse;
                    Extraction_UpdatePlateauTime(&tmp_l,
                        Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.DecPlatTime,
                        Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.ExtPlateauDuration,
                        &unusedExpr_l);

                    /* Entry Internal 'EXTENDED_PLATEAU': '<S97>:420' */
                    /* Transition: '<S97>:424' */
                    if (Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.is_EXTENDED_PLATEAU
                        == Extraction_IN_EXTENDED_PLATEAU_RAMPING)
                    {
                    }
                    else
                    {
                        Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.is_EXTENDED_PLATEAU
                            = Extraction_IN_EXTENDED_PLATEAU_RAMPING;
                        Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.Extr_HighSpd_State_la
                            = HIGH_SPEED_STATE_EXTENDED_PLATEAU_RAMPING;

                        /* Entry 'EXTENDED_PLATEAU_RAMPING': '<S97>:422' */
                        if (Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.Extr_HighSpd_TargetSpeed_ep
                            < Extr_Inputs_MotorSignals.Speed)
                        {
                            y_l =
                                Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.Extr_HighSpd_TargetSpeed_ep;
                            if (Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.Extr_HighSpd_TargetSpeed_ep
                                > 32767)
                            {
                                y_l = 32767U;
                            }

                            Extraction_SetSpeedCommand((sint16)y_l, 30);
                        }
                    }
                }
                else
                {
                    Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.ExtPlateauElapsedTime
                        += 1.0F / (float32)((uint16)FS_EXTRACTION);
                }
                break;

              case Extraction_IN_EXTENDED_PLATEAU_RAMPING:
                /* During 'EXTENDED_PLATEAU_RAMPING': '<S97>:422' */
                if (Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.sfEvent
                    == Extraction_event_Extr_Sup_EvSpeedReached_a1 ||
                        Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.Extr_HighSpd_TargetSpeed_ep
                    >= Extr_Inputs_MotorSignals.Speed)
                {
                    /* Transition: '<S97>:425' */
                    /* Transition: '<S97>:441' */
                    Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.is_EXTENDED_PLATEAU
                        = Extraction_IN_EXTENDED_PLATEAU_DELAY_ps;
                    Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.Extr_HighSpd_State_la
                        = HIGH_SPEED_STATE_EXTENDED_PLATEAU_DELAY;

                    /* Entry 'EXTENDED_PLATEAU_DELAY': '<S97>:62' */
                    /* Event: '<S97>:25' */
                    Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.Extr_HighSpd_EvStartPlateauEventCounter
                        ++;
                    Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.ExtPlateauElapsedTime
                        += 1.0F / (float32)((uint16)FS_EXTRACTION);
                }
                break;
            }
        }
    }
}

#endif                                 /* EXTR_VARIANT_HIGH_SPEED_PE_ENABLED */

/* Function for Chart: '<S93>/Extr_HighSpd_SupervisorLogic' */
#if EXTR_VARIANT_HIGH_SPEED_PE_ENABLED

static void Extraction_PLATEAU(void)
{
    uint8 is_RUNNING_l;
    EXTR_HIGH_SPD_FAULT Extr_HighSpd_Fault_l;
    Extr_HighSpd_Fault_l = Extr_HighSpd_Fault;
    is_RUNNING_l =
        Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.is_RUNNING;

    /* During 'PLATEAU': '<S97>:13' */
    if (Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.sfEvent
        == Extraction_event_Extr_HighSpd_EvPlateauComplete_mw)
    {
        /* Transition: '<S97>:247' */
        if (is_RUNNING_l != Extraction_IN_PLATEAU_mt)
        {
        }
        else
        {
            /* Exit 'PLATEAU': '<S97>:13' */
            Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.PlateauDecision
                = Extr_HighSpd_PlateauDecision;
            if (Extr_HighSpd_PlateauDecision ==
                    HIGH_SPEED_PLATEAU_DECISION_REGRESS)
            {
                Extr_HighSpd_Fault_l =
                    EXTR_HIGH_SPD_FAULT_PLATEAU_REGRESS_DECISION;
                Extraction_SetFault(EXTR_FAULT_HIGH_SPD_PLATEAU_REGRESS_DECISION);
            }

            if (Extr_HighSpd_PlateauDecision == HIGH_SPEED_PLATEAU_DECISION_HOLD)
            {
                Extr_HighSpd_Fault_l = EXTR_HIGH_SPD_FAULT_PLATEAU_HOLD_DECISION;
                Extraction_SetFault(EXTR_FAULT_HIGH_SPD_PLATEAU_HOLD_DECISION);
            }
        }

        is_RUNNING_l = Extraction_IN_PLATEAU_COMPLETE;
        Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.Extr_HighSpd_State_la
            = HIGH_SPEED_STATE_PLATEAU_COMPLETE;
    }

    Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.is_RUNNING
        = is_RUNNING_l;
    Extr_HighSpd_Fault = Extr_HighSpd_Fault_l;
}

#endif                                 /* EXTR_VARIANT_HIGH_SPEED_PE_ENABLED */

/* Function for Chart: '<S93>/Extr_HighSpd_SupervisorLogic' */
#if EXTR_VARIANT_HIGH_SPEED_PE_ENABLED

static void Extraction_PLATEAU_COMPLETE(void)
{
    SPIN_PULSE_TYPE tmp_l;
    SPIN_PULSE_TYPE tmp_lu;
    SPIN_PULSE_TYPE tmp_lul;
    float32 unusedExpr_l;
    uint16 tmp_lulf;
    sint32 tmp_lulfk;
    uint32 qY_l;
    boolean_T guard1 = false;
    boolean_T guard2 = false;
    boolean_T guard3 = false;
    boolean_T guard4 = false;

    /* During 'PLATEAU_COMPLETE': '<S97>:330' */
    /* Transition: '<S97>:331' */
    guard1 = false;
    guard2 = false;
    guard3 = false;
    guard4 = false;
    if (Extr_HighSpd_Fault == EXTR_HIGH_SPD_FAULT_MAX_SPEED_REACHED)
    {
        /* Transition: '<S97>:302' */
        Extr_HighSpd_Fault = EXTR_HIGH_SPD_FAULT_NONE;

        /* Transition: '<S97>:303' */
        /* Transition: '<S97>:228' */
        guard1 = true;
    }
    else if (Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.SpinPulse.Dist_Time
             <= 0)
    {
        /* Transition: '<S97>:250' */
        /* Transition: '<S97>:253' */
        /* Transition: '<S97>:254' */
        qY_l = (*GESE_Extr_SpinProfileConfig).Spin_Pulse_Count - 1U;
        if (qY_l > (*GESE_Extr_SpinProfileConfig).Spin_Pulse_Count)
        {
            qY_l = 0U;
        }

        if (Extraction_B.VariantMergeForOutportExtr_HighSpd_CurrPulseInd >=
                (sint32)qY_l)
        {
            /* Transition: '<S97>:439' */
            /* Transition: '<S97>:440' */
            Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.is_RUNNING
                = Extraction_IN_EXTENDED_PLATEAU;

            /* Entry 'EXTENDED_PLATEAU': '<S97>:420' */
            Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.ExtPlateauDuration
                = Extr_Inputs_ExtPlateauDuration;
            tmp_lu =
                Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.SpinPulse;
            Extraction_UpdatePlateauTime(&tmp_lu,
                Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.DecPlatTime,
                Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.ExtPlateauDuration,
                &unusedExpr_l);
            if (Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.is_EXTENDED_PLATEAU
                == Extraction_IN_EXTENDED_PLATEAU_DELAY_ps)
            {
            }
            else
            {
                Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.is_EXTENDED_PLATEAU
                    = Extraction_IN_EXTENDED_PLATEAU_DELAY_ps;
                Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.Extr_HighSpd_State_la
                    = HIGH_SPEED_STATE_EXTENDED_PLATEAU_DELAY;

                /* Entry 'EXTENDED_PLATEAU_DELAY': '<S97>:62' */
                /* Event: '<S97>:25' */
                Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.Extr_HighSpd_EvStartPlateauEventCounter
                    ++;
                Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.ExtPlateauElapsedTime
                    += 1.0F / (float32)((uint16)FS_EXTRACTION);
            }
        }
        else
        {
            /* Transition: '<S97>:318' */
            if (Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.PlateauDecision
                == HIGH_SPEED_PLATEAU_DECISION_REGRESS)
            {
                /* Transition: '<S97>:307' */
                Extraction_B.VariantMergeForOutportExtr_HighSpd_RegressDecisionOnPrevPulse
                    = true;
                if (Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.PrevTargetSpeed
                    < Extr_Inputs_MinFinalSpeed && Extr_HighSpd_RebalCnt <
                        (*GESE_Extr_HighSpdUnbConfig).Max_Redistributions)
                {
                    /* Transition: '<S97>:339' */
                    /*  Rebalance */
                    Extraction_broadcast_EvStartRebal();
                }
                else if (Extraction_B.VariantMergeForOutportExtr_HighSpd_CurrPulseInd
                         <= 0)
                {
                    /* Transition: '<S97>:326' */
                    /* Transition: '<S97>:327' */
                    Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.PlateauDecision
                        = HIGH_SPEED_PLATEAU_DECISION_ADVANCE;
                    Extraction_B.VariantMergeForOutportExtr_HighSpd_CurrPulseInd
                        = -1;
                    guard2 = true;
                }
                else
                {
                    /* Transition: '<S97>:366' */
                    if (Extr_HighSpd_RebalCnt < (*GESE_Extr_HighSpdUnbConfig).
                            Max_Redistributions)
                    {
                        /* Transition: '<S97>:383' */
                        /*  Regress */
                        tmp_lulfk =
                            Extraction_B.VariantMergeForOutportExtr_HighSpd_CurrPulseInd
                            - 1;
                        if (tmp_lulfk < -128)
                        {
                            tmp_lulfk = -128;
                        }

                        Extraction_B.VariantMergeForOutportExtr_HighSpd_CurrPulseInd
                            = (sint8)tmp_lulfk;
                        guard3 = true;
                    }
                    else
                    {
                        /* Transition: '<S97>:384' */
                        /*  Ext Plateau */
                        /* Transition: '<S97>:394' */
                        Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.Extr_HighSpd_TargetSpeed_ep
                            =
                            Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.PrevTargetSpeed;
                        tmp_lulfk =
                            Extraction_B.VariantMergeForOutportExtr_HighSpd_CurrPulseInd
                            - 1;
                        if (tmp_lulfk < -128)
                        {
                            tmp_lulfk = -128;
                        }

                        Extraction_B.VariantMergeForOutportExtr_HighSpd_CurrPulseInd
                            = (sint8)tmp_lulfk;
                        Extraction_LoadSpinPulse
                            (Extraction_B.VariantMergeForOutportExtr_HighSpd_CurrPulseInd,
                             &tmp_lul);
                        Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.SpinPulse
                            = tmp_lul;
                        guard4 = true;
                    }
                }
            }
            else
            {
                /* Transition: '<S97>:341' */
                guard4 = true;
            }
        }
    }
    else
    {
        /* Transition: '<S97>:248' */
        guard2 = true;
    }

    if (guard4)
    {
        /* Transition: '<S97>:392' */
        if (Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.PlateauDecision
            == HIGH_SPEED_PLATEAU_DECISION_ADVANCE &&
                !Extraction_B.VariantMergeForOutportExtr_HighSpd_RegressDecisionOnPrevPulse)
        {
            /* Transition: '<S97>:88' */
            /*  Advance */
            tmp_lulfk =
                Extraction_B.VariantMergeForOutportExtr_HighSpd_CurrPulseInd + 1;
            if (tmp_lulfk > 127)
            {
                tmp_lulfk = 127;
            }

            Extraction_B.VariantMergeForOutportExtr_HighSpd_CurrPulseInd =
                (sint8)tmp_lulfk;
            if (Extraction_B.VariantMergeForOutportExtr_HighSpd_CurrPulseInd >=
                    (*GESE_Extr_SpinProfileConfig).Spin_Pulse_Count)
            {
                /* Transition: '<S97>:164' */
                Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.FinalPlateauVerificationRequired
                    = false;

                /* Transition: '<S97>:353' */
                /* Transition: '<S97>:22' */
                guard1 = true;
            }
            else
            {
                /* Transition: '<S97>:309' */
                guard3 = true;
            }
        }
        else
        {
            /* Transition: '<S97>:355' */
            if (Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.PlateauDecision
                == HIGH_SPEED_PLATEAU_DECISION_HOLD)
            {
                /* Transition: '<S97>:354' */
                /*  Hold */
                if (Extr_HighSpd_RebalCnt < (*GESE_Extr_HighSpdUnbConfig).
                        Max_Redistributions &&
                        Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.Extr_HighSpd_TargetSpeed_ep
                    < Extr_Inputs_MinFinalSpeed)
                {
                    /* Transition: '<S97>:359' */
                    Extraction_broadcast_EvStartRebal();
                }
                else
                {
                    /* Transition: '<S97>:357' */
                    /*  Ext Plateau */
                    /* Transition: '<S97>:353' */
                    /* Transition: '<S97>:22' */
                    guard1 = true;
                }
            }
            else
            {
                /* Transition: '<S97>:212' */
                /*  PlateauDecision == MONITOR */
                guard1 = true;
            }
        }
    }

    if (guard3)
    {
        /* Transition: '<S97>:18' */
        /* Transition: '<S97>:166' */
        Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.is_RUNNING
            = Extraction_IN_CONFIGURING_PULSE_l3;
        Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.Extr_HighSpd_State_la
            = HIGH_SPEED_STATE_CONFIGURING_PULSE;
        Extraction_enter_atomic_CONFIGURING_PULSE_ad();
    }

    if (guard2)
    {
        Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.is_RUNNING
            = Extraction_IN_DIST_SPEED_le;
        Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.Extr_HighSpd_State_la
            = HIGH_SPEED_STATE_DIST_SPEED;

        /* Entry 'DIST_SPEED': '<S97>:244' */
        Extraction_B.VariantMergeForOutportExtr_HighSpd_DistSpeedTimeout =
            Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.SpinPulse.Dist_Time;

        /* Event: '<S97>:257' */
        Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.Extr_HighSpd_EvDropToDistSpeedEventCounter
            ++;
    }

    if (guard1)
    {
        Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.is_RUNNING
            = Extraction_IN_EXTENDED_PLATEAU;

        /* Entry 'EXTENDED_PLATEAU': '<S97>:420' */
        Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.ExtPlateauDuration
            = Extr_Inputs_ExtPlateauDuration;
        tmp_l =
            Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.SpinPulse;
        Extraction_UpdatePlateauTime(&tmp_l,
            Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.DecPlatTime,
            Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.ExtPlateauDuration,
            &unusedExpr_l);

        /* Entry Internal 'EXTENDED_PLATEAU': '<S97>:420' */
        /* Transition: '<S97>:424' */
        if (Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.is_EXTENDED_PLATEAU
            == Extraction_IN_EXTENDED_PLATEAU_RAMPING)
        {
        }
        else
        {
            Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.is_EXTENDED_PLATEAU
                = Extraction_IN_EXTENDED_PLATEAU_RAMPING;
            Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.Extr_HighSpd_State_la
                = HIGH_SPEED_STATE_EXTENDED_PLATEAU_RAMPING;

            /* Entry 'EXTENDED_PLATEAU_RAMPING': '<S97>:422' */
            if (Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.Extr_HighSpd_TargetSpeed_ep
                < Extr_Inputs_MotorSignals.Speed)
            {
                tmp_lulf =
                    Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.Extr_HighSpd_TargetSpeed_ep;
                if (Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.Extr_HighSpd_TargetSpeed_ep
                    > 32767)
                {
                    tmp_lulf = 32767U;
                }

                Extraction_SetSpeedCommand((sint16)tmp_lulf, 30);
            }
        }
    }
}

#endif                                 /* EXTR_VARIANT_HIGH_SPEED_PE_ENABLED */

/* Function for Chart: '<S93>/Extr_HighSpd_SupervisorLogic' */
#if EXTR_VARIANT_HIGH_SPEED_PE_ENABLED

static void Extraction_RAMP(void)
{
    uint32 qY_l;
    uint16 tmp_l;
    sint32 tmp_lu;
    sint32 tmp_lul;
    sint32 sfEvent_l;
    uint32 Extr_HighSpd_EvStartPlateauEventCounter_l;
    uint8 is_EXTENDED_PLATEAU_l;
    HIGH_SPEED_STATE Extr_HighSpd_State_la_l;
    uint32 Extr_HighSpd_EvStopAllEventCounter_l;
    uint32 Extr_HighSpd_EvStartRebalEventCounter_l;
    uint8 Extr_HighSpd_RebalCnt_l;
    EXTR_HIGH_SPD_FAULT Extr_HighSpd_Fault_l;
    uint8 is_RUNNING_l;
    boolean_T guard1 = false;
    boolean_T guard2 = false;
    is_RUNNING_l =
        Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.is_RUNNING;
    Extr_HighSpd_Fault_l = Extr_HighSpd_Fault;
    Extr_HighSpd_RebalCnt_l = Extr_HighSpd_RebalCnt;
    Extr_HighSpd_EvStartRebalEventCounter_l =
        Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.Extr_HighSpd_EvStartRebalEventCounter;
    Extr_HighSpd_EvStopAllEventCounter_l =
        Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.Extr_HighSpd_EvStopAllEventCounter;
    Extr_HighSpd_State_la_l =
        Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.Extr_HighSpd_State_la;
    is_EXTENDED_PLATEAU_l =
        Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.is_EXTENDED_PLATEAU;
    Extr_HighSpd_EvStartPlateauEventCounter_l =
        Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.Extr_HighSpd_EvStartPlateauEventCounter;
    sfEvent_l =
        Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.sfEvent;

    /* During 'RAMP': '<S97>:11' */
    if (sfEvent_l == Extraction_event_Extr_HighSpd_EvRampComplete_b3)
    {
        /* Transition: '<S97>:14' */
        if (Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.PlateauDuration
            > 0.0F)
        {
            /* Transition: '<S97>:465' */
            is_RUNNING_l = Extraction_IN_PLATEAU_mt;
            Extr_HighSpd_State_la_l = HIGH_SPEED_STATE_PLATEAU;

            /* Entry 'PLATEAU': '<S97>:13' */
            /* Event: '<S97>:25' */
            Extr_HighSpd_EvStartPlateauEventCounter_l++;
        }
        else
        {
            /* Transition: '<S97>:467' */
            /* Transition: '<S97>:469' */
            Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.PlateauDecision
                = HIGH_SPEED_PLATEAU_DECISION_ADVANCE;

            /* Transition: '<S97>:470' */
            is_RUNNING_l = Extraction_IN_PLATEAU_COMPLETE;
            Extr_HighSpd_State_la_l = HIGH_SPEED_STATE_PLATEAU_COMPLETE;
        }
    }
    else
    {
        guard1 = false;
        guard2 = false;
        if (sfEvent_l == Extraction_event_Extr_HighSpd_EvViscousLimitExceeded_gz)
        {
            /* Transition: '<S97>:57' */
            Extr_HighSpd_Fault_l = EXTR_HIGH_SPD_FAULT_RAMP_VISCOUS_EXCEEDED;
            Extraction_SetFault(EXTR_FAULT_HIGH_SPD_RAMP_VISCOUS_EXCEEDED);
            guard2 = true;
        }
        else
        {
            if (sfEvent_l ==
                    Extraction_event_Extr_HighSpd_EvFrictionLimitExceeded)
            {
                /* Transition: '<S97>:434' */
                Extr_HighSpd_Fault_l =
                    EXTR_HIGH_SPD_FAULT_RAMP_FRICTION_EXCEEDED;
                Extraction_SetFault(EXTR_FAULT_HIGH_SPD_RAMP_FRICTION_EXCEEDED);

                /* Transition: '<S97>:435' */
                guard2 = true;
            }
        }

        if (guard2)
        {
            if (Extr_Inputs_MotorSignals.Speed - (float32)
                    (*GESE_Extr_HighSpdConfig).Delta_Speed_Drop >=
                    Extr_Inputs_MinFinalSpeed)
            {
                /* Transition: '<S97>:61' */
                guard1 = true;
            }
            else
            {
                /* Transition: '<S97>:66' */
                if (Extr_HighSpd_RebalCnt_l < (*GESE_Extr_HighSpdUnbConfig).
                        Max_Redistributions)
                {
                    /* Transition: '<S97>:68' */
                    /* Transition: '<S97>:454' */
                    is_RUNNING_l = Extraction_IN_NO_ACTIVE_CHILD_fr;
                    Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.is_HIGH_SPEED_SUPERVISOR
                        = Extraction_IN_REBALANCE_nu;
                    Extr_HighSpd_State_la_l = HIGH_SPEED_STATE_REBALANCE;

                    /* Entry 'REBALANCE': '<S97>:138' */
                    tmp_lul = (sint32)(Extr_HighSpd_RebalCnt_l + 1U);
                    if ((uint32)tmp_lul > 255U)
                    {
                        tmp_lul = 255;
                    }

                    Extr_HighSpd_RebalCnt_l = (uint8)tmp_lul;
                    tmp_lul =
                        Extraction_B.VariantMergeForOutportExtr_HighSpd_CurrPulseInd
                        + 1;
                    if (tmp_lul > 127)
                    {
                        tmp_lul = 127;
                    }

                    tmp_lu =
                        Extraction_B.VariantMergeForOutportExtr_HighSpd_CurrPulseInd
                        + 1;
                    if (tmp_lu > 127)
                    {
                        tmp_lu = 127;
                    }

                    tmp_lu = (sint32)
                        (Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.Extr_HighSpd_RebalCntArray
                         [tmp_lu - 1] + 1U);
                    if ((uint32)tmp_lu > 255U)
                    {
                        tmp_lu = 255;
                    }

                    Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.Extr_HighSpd_RebalCntArray
                        [tmp_lul - 1] = (uint8)tmp_lu;
                    tmp_lul =
                        Extraction_B.VariantMergeForOutportExtr_HighSpd_CurrPulseInd
                        + 1;
                    if (tmp_lul > 127)
                    {
                        tmp_lul = 127;
                    }

                    if (Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.Extr_HighSpd_RebalCntArray
                            [tmp_lul - 1] >
                            Extraction_B.VariantMergeForOutportExtr_HighSpd_BaselineConfig.Max_Redistributions)
                    {
                        /*  Req 250641 */
                        tmp_lul =
                            Extraction_B.VariantMergeForOutportExtr_HighSpd_CurrPulseInd
                            + 1;
                        if (tmp_lul > 127)
                        {
                            tmp_lul = 127;
                        }

                        Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.Extr_HighSpd_RebalCntArray
                            [tmp_lul - 1] = 0U;
                        tmp_lul =
                            Extraction_B.VariantMergeForOutportExtr_HighSpd_CurrPulseInd
                            + 1;
                        if (tmp_lul > 127)
                        {
                            tmp_lul = 127;
                        }

                        Extraction_IncreaseCurrentBaselineFriction((sint8)
                            tmp_lul);
                    }

                    /* Event: '<S97>:74' */
                    Extr_HighSpd_EvStartRebalEventCounter_l++;

                    /* Event: '<S97>:81' */
                    Extr_HighSpd_EvStopAllEventCounter_l++;
                }
                else
                {
                    /* Transition: '<S97>:283' */
                    Extraction_SetFault
                        (EXTR_FAULT_HIGH_SPEED_REBALANCE_COUNT_EXCEEDED);

                    /* Transition: '<S97>:297' */
                    guard1 = true;
                }
            }
        }

        if (guard1)
        {
            /* Transition: '<S97>:90' */
            Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.unusedExpr
                = roundf(Extr_Inputs_MotorSignals.Speed);
            if (Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.unusedExpr
                < 65536.0F)
            {
                if (Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.unusedExpr
                    >= 0.0F)
                {
                    tmp_l = (uint16)
                        Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.unusedExpr;
                }
                else
                {
                    tmp_l = 0U;
                }
            }
            else
            {
                tmp_l = MAX_uint16_T;
            }

            qY_l = (uint32)tmp_l - (*GESE_Extr_HighSpdConfig).Delta_Speed_Drop;
            if (qY_l > tmp_l)
            {
                qY_l = 0U;
            }

            Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.Extr_HighSpd_TargetSpeed_ep
                = (uint16)qY_l;
            Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.DeltaSpeedDropExecuted
                = true;

            /* Transition: '<S97>:268' */
            /* Transition: '<S97>:228' */
            is_RUNNING_l = Extraction_IN_EXTENDED_PLATEAU;

            /* Entry 'EXTENDED_PLATEAU': '<S97>:420' */
            Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.ExtPlateauDuration
                = Extr_Inputs_ExtPlateauDuration;
            Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.r2
                =
                Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.SpinPulse;
            Extraction_UpdatePlateauTime
                (&Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.r2,
                 Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.DecPlatTime,
                 Extr_Inputs_ExtPlateauDuration,
                 &Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.unusedExpr);

            /* Entry Internal 'EXTENDED_PLATEAU': '<S97>:420' */
            /* Transition: '<S97>:424' */
            if (is_EXTENDED_PLATEAU_l == Extraction_IN_EXTENDED_PLATEAU_RAMPING)
            {
            }
            else
            {
                is_EXTENDED_PLATEAU_l = Extraction_IN_EXTENDED_PLATEAU_RAMPING;
                Extr_HighSpd_State_la_l =
                    HIGH_SPEED_STATE_EXTENDED_PLATEAU_RAMPING;

                /* Entry 'EXTENDED_PLATEAU_RAMPING': '<S97>:422' */
                if ((uint16)qY_l < Extr_Inputs_MotorSignals.Speed)
                {
                    tmp_l = (uint16)qY_l;
                    if ((uint16)qY_l > 32767)
                    {
                        tmp_l = 32767U;
                    }

                    Extraction_SetSpeedCommand((sint16)tmp_l, 30);
                }
            }
        }
    }

    Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.Extr_HighSpd_EvStartPlateauEventCounter
        = Extr_HighSpd_EvStartPlateauEventCounter_l;
    Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.is_EXTENDED_PLATEAU
        = is_EXTENDED_PLATEAU_l;
    Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.Extr_HighSpd_State_la
        = Extr_HighSpd_State_la_l;
    Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.Extr_HighSpd_EvStopAllEventCounter
        = Extr_HighSpd_EvStopAllEventCounter_l;
    Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.Extr_HighSpd_EvStartRebalEventCounter
        = Extr_HighSpd_EvStartRebalEventCounter_l;
    Extr_HighSpd_RebalCnt = Extr_HighSpd_RebalCnt_l;
    Extr_HighSpd_Fault = Extr_HighSpd_Fault_l;
    Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.is_RUNNING
        = is_RUNNING_l;
}

#endif                                 /* EXTR_VARIANT_HIGH_SPEED_PE_ENABLED */

/* Function for Chart: '<S93>/Extr_HighSpd_SupervisorLogic' */
#if EXTR_VARIANT_HIGH_SPEED_PE_ENABLED

static void Extraction_enter_atomic_REBALANCE(void)
{
    sint32 tmp_l;
    sint32 tmp_lu;
    sint32 tmp_lul;
    uint32 Extr_HighSpd_EvStopAllEventCounter_l;
    uint32 Extr_HighSpd_EvStartRebalEventCounter_l;
    uint8 Extr_HighSpd_RebalCnt_l;
    Extr_HighSpd_RebalCnt_l = Extr_HighSpd_RebalCnt;
    Extr_HighSpd_EvStartRebalEventCounter_l =
        Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.Extr_HighSpd_EvStartRebalEventCounter;
    Extr_HighSpd_EvStopAllEventCounter_l =
        Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.Extr_HighSpd_EvStopAllEventCounter;

    /* Entry 'REBALANCE': '<S97>:138' */
    tmp_lul = (sint32)(Extr_HighSpd_RebalCnt_l + 1U);
    if ((uint32)tmp_lul > 255U)
    {
        tmp_lul = 255;
    }

    tmp_l = Extraction_B.VariantMergeForOutportExtr_HighSpd_CurrPulseInd + 1;
    if (tmp_l > 127)
    {
        tmp_l = 127;
    }

    tmp_lu = Extraction_B.VariantMergeForOutportExtr_HighSpd_CurrPulseInd + 1;
    if (tmp_lu > 127)
    {
        tmp_lu = 127;
    }

    tmp_lu = (sint32)
        (Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.Extr_HighSpd_RebalCntArray
         [tmp_lu - 1] + 1U);
    if ((uint32)tmp_lu > 255U)
    {
        tmp_lu = 255;
    }

    Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.Extr_HighSpd_RebalCntArray
        [tmp_l - 1] = (uint8)tmp_lu;
    tmp_l = Extraction_B.VariantMergeForOutportExtr_HighSpd_CurrPulseInd + 1;
    if (tmp_l > 127)
    {
        tmp_l = 127;
    }

    if (Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.Extr_HighSpd_RebalCntArray
            [tmp_l - 1] >
            Extraction_B.VariantMergeForOutportExtr_HighSpd_BaselineConfig.Max_Redistributions)
    {
        /*  Req 250641 */
        tmp_l = Extraction_B.VariantMergeForOutportExtr_HighSpd_CurrPulseInd + 1;
        if (tmp_l > 127)
        {
            tmp_l = 127;
        }

        Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.Extr_HighSpd_RebalCntArray
            [tmp_l - 1] = 0U;
        tmp_l = Extraction_B.VariantMergeForOutportExtr_HighSpd_CurrPulseInd + 1;
        if (tmp_l > 127)
        {
            tmp_l = 127;
        }

        Extraction_IncreaseCurrentBaselineFriction((sint8)tmp_l);
    }

    /* Event: '<S97>:74' */
    /* Event: '<S97>:81' */
    Extr_HighSpd_EvStopAllEventCounter_l++;
    Extr_HighSpd_EvStartRebalEventCounter_l++;
    Extr_HighSpd_RebalCnt_l = (uint8)tmp_lul;
    Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.Extr_HighSpd_EvStopAllEventCounter
        = Extr_HighSpd_EvStopAllEventCounter_l;
    Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.Extr_HighSpd_EvStartRebalEventCounter
        = Extr_HighSpd_EvStartRebalEventCounter_l;
    Extr_HighSpd_RebalCnt = Extr_HighSpd_RebalCnt_l;
}

#endif                                 /* EXTR_VARIANT_HIGH_SPEED_PE_ENABLED */

/* Function for Chart: '<S93>/Extr_HighSpd_SupervisorLogic' */
#if EXTR_VARIANT_HIGH_SPEED_PE_ENABLED

static void Extraction_RUNNING_ob(void)
{
    SPIN_PULSE_TYPE tmp_l;
    SPIN_PULSE_TYPE tmp_lu;
    SPIN_PULSE_TYPE tmp_lul;
    SPIN_PULSE_TYPE tmp_lulf;
    float32 unusedExpr_l;
    sint32 tmp_lulfk;
    uint16 tmp_lulfkl;
    sint32 tmp_lulfklt;
    uint32 qY_l;
    boolean_T guard1 = false;
    boolean_T guard2 = false;
    boolean_T guard3 = false;
    boolean_T guard4 = false;
    boolean_T guard5 = false;
    boolean_T guard6 = false;
    boolean_T guard7 = false;
    boolean_T guard8 = false;
    boolean_T guard9 = false;

    /* During 'RUNNING': '<S97>:7' */
    if (Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.sfEvent
        == Extraction_event_Extr_Sup_EvStopAll_dp ||
            Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.sfEvent
        == Extraction_event_Local_EvStop_bj)
    {
        /* Transition: '<S97>:20' */
        /* Exit Internal 'RUNNING': '<S97>:7' */
        if (Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.is_RUNNING
            == Extraction_IN_PLATEAU_mt)
        {
            /* Exit 'PLATEAU': '<S97>:13' */
            Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.PlateauDecision
                = Extr_HighSpd_PlateauDecision;
            if (Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.PlateauDecision
                == HIGH_SPEED_PLATEAU_DECISION_REGRESS)
            {
                Extr_HighSpd_Fault =
                    EXTR_HIGH_SPD_FAULT_PLATEAU_REGRESS_DECISION;
                Extraction_SetFault(EXTR_FAULT_HIGH_SPD_PLATEAU_REGRESS_DECISION);
            }

            if (Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.PlateauDecision
                == HIGH_SPEED_PLATEAU_DECISION_HOLD)
            {
                Extr_HighSpd_Fault = EXTR_HIGH_SPD_FAULT_PLATEAU_HOLD_DECISION;
                Extraction_SetFault(EXTR_FAULT_HIGH_SPD_PLATEAU_HOLD_DECISION);
            }

            Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.is_RUNNING
                = Extraction_IN_NO_ACTIVE_CHILD_fr;
        }
        else
        {
            Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.is_RUNNING
                = Extraction_IN_NO_ACTIVE_CHILD_fr;

            /* Exit Internal 'EXTENDED_PLATEAU': '<S97>:420' */
            Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.is_EXTENDED_PLATEAU
                = Extraction_IN_NO_ACTIVE_CHILD_fr;
        }

        /* Event: '<S97>:81' */
        Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.Extr_HighSpd_EvStopAllEventCounter
            ++;
        Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.is_HIGH_SPEED_SUPERVISOR
            = Extraction_IN_IDLE_ow;
        Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.Extr_HighSpd_State_la
            = HIGH_SPEED_STATE_IDLE;

        /* Entry 'IDLE': '<S97>:5' */
        Extraction_B.VariantMergeForOutportExtr_HighSpd_CurrPulseInd = 0;
    }
    else
    {
        guard1 = false;
        guard2 = false;
        guard3 = false;
        guard4 = false;
        guard5 = false;
        guard6 = false;
        guard7 = false;
        guard8 = false;
        guard9 = false;
        if (!(ExtractionParams__IsExtrConfigValid() != 0))
        {
            /* Transition: '<S97>:44' */
            Extr_HighSpd_Fault = EXTR_HIGH_SPD_FAULT_INVALID_SETUP;
            Extraction_SetFault(EXTR_FAULT_HIGH_SPD_INVALID_CONFIG);
            Extraction_StopMotor();

            /* Event: '<S97>:81' */
            Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.Extr_HighSpd_EvStopAllEventCounter
                ++;

            /* Event: '<S97>:27' */
            Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.Extr_HighSpd_EvCompleteEventCounter
                ++;

            /* Transition: '<S97>:46' */
            guard1 = true;
        }
        else if (Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.sfEvent
                 == Extraction_event_Extr_Sup_EvFoamDetected_md)
        {
            /* Transition: '<S97>:230' */
            /* Event: '<S97>:81' */
            Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.Extr_HighSpd_EvStopAllEventCounter
                ++;
            guard1 = true;
        }
        else if (Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.sfEvent
                 == Extraction_event_Extr_Sup_EvRebalStart_ba)
        {
            /* Transition: '<S97>:473' */
            /* Exit Internal 'RUNNING': '<S97>:7' */
            if (Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.is_RUNNING
                == Extraction_IN_PLATEAU_mt)
            {
                /* Exit 'PLATEAU': '<S97>:13' */
                Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.PlateauDecision
                    = Extr_HighSpd_PlateauDecision;
                if (Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.PlateauDecision
                    == HIGH_SPEED_PLATEAU_DECISION_REGRESS)
                {
                    Extr_HighSpd_Fault =
                        EXTR_HIGH_SPD_FAULT_PLATEAU_REGRESS_DECISION;
                    Extraction_SetFault
                        (EXTR_FAULT_HIGH_SPD_PLATEAU_REGRESS_DECISION);
                }

                if (Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.PlateauDecision
                    == HIGH_SPEED_PLATEAU_DECISION_HOLD)
                {
                    Extr_HighSpd_Fault =
                        EXTR_HIGH_SPD_FAULT_PLATEAU_HOLD_DECISION;
                    Extraction_SetFault
                        (EXTR_FAULT_HIGH_SPD_PLATEAU_HOLD_DECISION);
                }

                Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.is_RUNNING
                    = Extraction_IN_NO_ACTIVE_CHILD_fr;
            }
            else
            {
                Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.is_RUNNING
                    = Extraction_IN_NO_ACTIVE_CHILD_fr;

                /* Exit Internal 'EXTENDED_PLATEAU': '<S97>:420' */
                Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.is_EXTENDED_PLATEAU
                    = Extraction_IN_NO_ACTIVE_CHILD_fr;
            }

            Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.is_HIGH_SPEED_SUPERVISOR
                = Extraction_IN_REBALANCE_nu;
            Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.Extr_HighSpd_State_la
                = HIGH_SPEED_STATE_REBALANCE;
            Extraction_enter_atomic_REBALANCE();
        }
        else
        {
            if (Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.sfEvent
                == Extraction_event_Extr_Accel_VibMapEvExcessiveFloorDisp_ed)
            {
                Extr_HighSpd_Fault = EXTR_HIGH_SPD_FAULT_VIB_MAP_FLOOR_DISP;
                Extraction_SetFault(EXTR_FAULT_HIGH_SPD_VIB_MAP_FLOOR_DISP);
            }

            Extr_HighSpd_Friction = Extr_Inputs_ParamEstData.Friction;
            Extr_HighSpd_ViscousFriction =
                Extr_Inputs_ParamEstData.Viscous_Friction;
            if (Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.sfEvent
                == Extraction_event_Extr_HighSpd_EvExtndChkFailed_fo)
            {
                /* Transition: '<S97>:285' */
                Extr_HighSpd_Fault = EXTR_HIGH_SPD_FAULT_EXT_UNB_CHK_FAILED;
                Extraction_SetFault(EXTR_FAULT_HIGH_SPD_EXT_UNB_CHK_FAILED);

                /* Transition: '<S97>:408' */
                guard7 = true;
            }
            else if (Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.sfEvent
                     == Extraction_event_EvStartRebal)
            {
                /* Transition: '<S97>:405' */
                guard7 = true;
            }
            else if (Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.sfEvent
                     == Extraction_event_Extr_Sup_EvSpeedErrorTimeout_o2)
            {
                /* Transition: '<S97>:413' */
                Extr_HighSpd_Fault = EXTR_HIGH_SPD_FAULT_SPEED_ERROR_EXCEEDED;
                Extraction_SetFault(EXTR_FAULT_SPEED_ERROR_EXCEEDED);
                if (Extr_Inputs_MaxSpeedError < 255)
                {
                    /* Transition: '<S97>:412' */
                    /* Transition: '<S97>:408' */
                    guard7 = true;
                }
                else
                {
                    /* Transition: '<S97>:429' */
                    /* Transition: '<S97>:430' */
                    /* Exit Internal 'RUNNING': '<S97>:7' */
                    if (Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.is_RUNNING
                        == Extraction_IN_PLATEAU_mt)
                    {
                        /* Exit 'PLATEAU': '<S97>:13' */
                        Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.PlateauDecision
                            = Extr_HighSpd_PlateauDecision;
                        if (Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.PlateauDecision
                            == HIGH_SPEED_PLATEAU_DECISION_REGRESS)
                        {
                            Extr_HighSpd_Fault =
                                EXTR_HIGH_SPD_FAULT_PLATEAU_REGRESS_DECISION;
                            Extraction_SetFault
                                (EXTR_FAULT_HIGH_SPD_PLATEAU_REGRESS_DECISION);
                        }

                        if (Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.PlateauDecision
                            == HIGH_SPEED_PLATEAU_DECISION_HOLD)
                        {
                            Extr_HighSpd_Fault =
                                EXTR_HIGH_SPD_FAULT_PLATEAU_HOLD_DECISION;
                            Extraction_SetFault
                                (EXTR_FAULT_HIGH_SPD_PLATEAU_HOLD_DECISION);
                        }

                        Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.is_RUNNING
                            = Extraction_IN_NO_ACTIVE_CHILD_fr;
                    }
                    else
                    {
                        Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.is_RUNNING
                            = Extraction_IN_NO_ACTIVE_CHILD_fr;

                        /* Exit Internal 'EXTENDED_PLATEAU': '<S97>:420' */
                        Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.is_EXTENDED_PLATEAU
                            = Extraction_IN_NO_ACTIVE_CHILD_fr;
                    }

                    if (Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.is_RUNNING
                        == Extraction_IN_HIGH_SPEED_COMPLETE_fn)
                    {
                    }
                    else
                    {
                        Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.is_RUNNING
                            = Extraction_IN_HIGH_SPEED_COMPLETE_fn;
                        Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.Extr_HighSpd_State_la
                            = HIGH_SPEED_STATE_HIGH_SPEED_COMPLETE;
                        Extraction_enter_atomic_HIGH_SPEED_COMPLETE_dq();
                    }
                }
            }
            else if (Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.sfEvent
                     == Extraction_event_Extr_Accel_EvCabHit_ns)
            {
                /* Transition: '<S97>:242' */
                Extr_HighSpd_Fault = EXTR_HIGH_SPD_FAULT_CAB_HIT_DETECTED;
                Extraction_SetFault(EXTR_FAULT_HIGH_SPD_CAB_HIT_DETECTED);

                /* Transition: '<S97>:485' */
                guard8 = true;
            }
            else if (Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.sfEvent
                     == Extraction_event_Extr_HighSpd_EvLoadMassExceeded_ha)
            {
                /* Transition: '<S97>:233' */
                Extr_HighSpd_Fault =
                    EXTR_HIGH_SPD_FAULT_LOAD_MASS_LIMIT_EXCEEDED;
                Extraction_SetFault(EXTR_FAULT_HIGH_SPD_LOAD_MASS_LIMIT_EXCEEDED);
                guard3 = true;
            }
            else if (Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.sfEvent
                     == Extraction_event_Extr_HighSpd_EvCEMFailureDetected_gz)
            {
                /* Transition: '<S97>:444' */
                Extr_HighSpd_Fault = EXTR_HIGH_SPD_FAULT_CEM_EVENT;
                Extraction_SetFault(EXTR_FAULT_HIGH_SPD_CEM_EVENT);
                Extraction_IncreaseCEMEventCounter();
                if (Extr_Inputs_MotorSignals.Speed > (float32)
                        (*GESE_Extr_CEMSetup).Clothing_Drag_Max_Velocity_Div10 *
                    10.0F)
                {
                    /* Transition: '<S97>:446' */
                    tmp_lulfklt =
                        Extraction_B.VariantMergeForOutportExtr_HighSpd_CurrPulseInd
                        + 1;
                    if (tmp_lulfklt > 127)
                    {
                        tmp_lulfklt = 127;
                    }

                    Extraction_DecreaseCurrentBaselineFriction((sint8)
                        tmp_lulfklt);

                    /* Transition: '<S97>:450' */
                }
                else
                {
                    /* Transition: '<S97>:448' */
                    /* Transition: '<S97>:451' */
                }

                if (Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.SpinPulse.Target_Speed_Div10
                    <= (*GESE_Extr_CEMSetup).Clothing_Drag_Max_Velocity_Div10)
                {
                    /* Transition: '<S97>:479' */
                    guard8 = true;
                }
                else
                {
                    /* Transition: '<S97>:457' */
                    /* Transition: '<S97>:483' */
                    guard4 = true;
                }
            }
            else
            {
                switch
                    (Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.is_RUNNING)
                {
                  case Extraction_IN_CONFIGURING_PULSE_l3:
                    /* During 'CONFIGURING_PULSE': '<S97>:142' */
                    /* Transition: '<S97>:143' */
                    Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.is_RUNNING
                        = Extraction_IN_RAMP_bl;
                    Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.Extr_HighSpd_State_la
                        = HIGH_SPEED_STATE_RAMP;

                    /* Entry 'RAMP': '<S97>:11' */
                    if ((*GESE_Extr_SpinProfileConfig).High_Speed_Enabled != 0)
                    {
                        Extraction_ParamEstExcControl(EXC_ENABLE);
                        Extraction_ParamEstControl(PE_START);
                    }

                    /* Event: '<S97>:26' */
                    Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.Extr_HighSpd_EvStartRampEventCounter
                        ++;
                    break;

                  case Extraction_IN_DIST_SPEED_le:
                    /* During 'DIST_SPEED': '<S97>:244' */
                    if (Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.sfEvent
                        == Extraction_event_Extr_LowSpd_EvComplete_kv)
                    {
                        /* Transition: '<S97>:255' */
                        qY_l = (*GESE_Extr_SpinProfileConfig).Spin_Pulse_Count -
                            1U;
                        if (qY_l > (*GESE_Extr_SpinProfileConfig).
                                Spin_Pulse_Count)
                        {
                            qY_l = 0U;
                        }

                        if (Extraction_B.VariantMergeForOutportExtr_HighSpd_CurrPulseInd
                            >= (sint32)qY_l)
                        {
                            /* Transition: '<S97>:439' */
                            /* Transition: '<S97>:440' */
                            Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.is_RUNNING
                                = Extraction_IN_EXTENDED_PLATEAU;

                            /* Entry 'EXTENDED_PLATEAU': '<S97>:420' */
                            Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.ExtPlateauDuration
                                = Extr_Inputs_ExtPlateauDuration;
                            tmp_lu =
                                Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.SpinPulse;
                            Extraction_UpdatePlateauTime(&tmp_lu,
                                Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.DecPlatTime,
                                Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.ExtPlateauDuration,
                                &unusedExpr_l);
                            if (Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.is_EXTENDED_PLATEAU
                                == Extraction_IN_EXTENDED_PLATEAU_DELAY_ps)
                            {
                            }
                            else
                            {
                                Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.is_EXTENDED_PLATEAU
                                    = Extraction_IN_EXTENDED_PLATEAU_DELAY_ps;
                                Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.Extr_HighSpd_State_la
                                    = HIGH_SPEED_STATE_EXTENDED_PLATEAU_DELAY;

                                /* Entry 'EXTENDED_PLATEAU_DELAY': '<S97>:62' */
                                /* Event: '<S97>:25' */
                                Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.Extr_HighSpd_EvStartPlateauEventCounter
                                    ++;
                                Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.ExtPlateauElapsedTime
                                    += 1.0F / (float32)((uint16)FS_EXTRACTION);
                            }
                        }
                        else
                        {
                            /* Transition: '<S97>:318' */
                            if (Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.PlateauDecision
                                == HIGH_SPEED_PLATEAU_DECISION_REGRESS)
                            {
                                /* Transition: '<S97>:307' */
                                Extraction_B.VariantMergeForOutportExtr_HighSpd_RegressDecisionOnPrevPulse
                                    = true;
                                if (Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.PrevTargetSpeed
                                    < Extr_Inputs_MinFinalSpeed &&
                                        Extr_HighSpd_RebalCnt <
                                        (*GESE_Extr_HighSpdUnbConfig).
                                        Max_Redistributions)
                                {
                                    /* Transition: '<S97>:339' */
                                    /*  Rebalance */
                                    Extraction_broadcast_EvStartRebal();
                                }
                                else if (Extraction_B.VariantMergeForOutportExtr_HighSpd_CurrPulseInd
                                         <= 0)
                                {
                                    /* Transition: '<S97>:326' */
                                    /* Transition: '<S97>:327' */
                                    Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.PlateauDecision
                                        = HIGH_SPEED_PLATEAU_DECISION_ADVANCE;
                                    Extraction_B.VariantMergeForOutportExtr_HighSpd_CurrPulseInd
                                        = -1;
                                    Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.is_RUNNING
                                        = Extraction_IN_DIST_SPEED_le;
                                    Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.Extr_HighSpd_State_la
                                        = HIGH_SPEED_STATE_DIST_SPEED;

                                    /* Entry 'DIST_SPEED': '<S97>:244' */
                                    Extraction_B.VariantMergeForOutportExtr_HighSpd_DistSpeedTimeout
                                        =
                                        Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.SpinPulse.Dist_Time;

                                    /* Event: '<S97>:257' */
                                    Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.Extr_HighSpd_EvDropToDistSpeedEventCounter
                                        ++;
                                }
                                else
                                {
                                    /* Transition: '<S97>:366' */
                                    if (Extr_HighSpd_RebalCnt <
                                            (*GESE_Extr_HighSpdUnbConfig).
                                            Max_Redistributions)
                                    {
                                        /* Transition: '<S97>:383' */
                                        /*  Regress */
                                        tmp_lulfklt =
                                            Extraction_B.VariantMergeForOutportExtr_HighSpd_CurrPulseInd
                                            - 1;
                                        if (tmp_lulfklt < -128)
                                        {
                                            tmp_lulfklt = -128;
                                        }

                                        Extraction_B.VariantMergeForOutportExtr_HighSpd_CurrPulseInd
                                            = (sint8)tmp_lulfklt;
                                        guard5 = true;
                                    }
                                    else
                                    {
                                        /* Transition: '<S97>:384' */
                                        /*  Ext Plateau */
                                        /* Transition: '<S97>:394' */
                                        Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.Extr_HighSpd_TargetSpeed_ep
                                            =
                                            Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.PrevTargetSpeed;
                                        tmp_lulfklt =
                                            Extraction_B.VariantMergeForOutportExtr_HighSpd_CurrPulseInd
                                            - 1;
                                        if (tmp_lulfklt < -128)
                                        {
                                            tmp_lulfklt = -128;
                                        }

                                        Extraction_B.VariantMergeForOutportExtr_HighSpd_CurrPulseInd
                                            = (sint8)tmp_lulfklt;
                                        Extraction_LoadSpinPulse
                                            (Extraction_B.VariantMergeForOutportExtr_HighSpd_CurrPulseInd,
                                             &tmp_lulf);
                                        Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.SpinPulse
                                            = tmp_lulf;
                                        guard9 = true;
                                    }
                                }
                            }
                            else
                            {
                                /* Transition: '<S97>:341' */
                                guard9 = true;
                            }
                        }
                    }
                    break;

                  case Extraction_IN_EXTENDED_PLATEAU:
                    Extraction_EXTENDED_PLATEAU();
                    break;

                  case Extraction_IN_HIGH_SPEED_COMPLETE_fn:
                    /* During 'HIGH_SPEED_COMPLETE': '<S97>:51' */
                    break;

                  case Extraction_IN_PLATEAU_mt:
                    Extraction_PLATEAU();
                    break;

                  case Extraction_IN_PLATEAU_COMPLETE:
                    Extraction_PLATEAU_COMPLETE();
                    break;

                  case Extraction_IN_RAMP_bl:
                    Extraction_RAMP();
                    break;
                }
            }
        }

        if (guard9)
        {
            /* Transition: '<S97>:392' */
            if (Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.PlateauDecision
                == HIGH_SPEED_PLATEAU_DECISION_ADVANCE &&
                    !Extraction_B.VariantMergeForOutportExtr_HighSpd_RegressDecisionOnPrevPulse)
            {
                /* Transition: '<S97>:88' */
                /*  Advance */
                tmp_lulfklt =
                    Extraction_B.VariantMergeForOutportExtr_HighSpd_CurrPulseInd
                    + 1;
                if (tmp_lulfklt > 127)
                {
                    tmp_lulfklt = 127;
                }

                Extraction_B.VariantMergeForOutportExtr_HighSpd_CurrPulseInd =
                    (sint8)tmp_lulfklt;
                if (Extraction_B.VariantMergeForOutportExtr_HighSpd_CurrPulseInd
                    >= (*GESE_Extr_SpinProfileConfig).Spin_Pulse_Count)
                {
                    /* Transition: '<S97>:164' */
                    Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.FinalPlateauVerificationRequired
                        = false;

                    /* Transition: '<S97>:353' */
                    /* Transition: '<S97>:22' */
                    guard6 = true;
                }
                else
                {
                    /* Transition: '<S97>:309' */
                    guard5 = true;
                }
            }
            else
            {
                /* Transition: '<S97>:355' */
                if (Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.PlateauDecision
                    == HIGH_SPEED_PLATEAU_DECISION_HOLD)
                {
                    /* Transition: '<S97>:354' */
                    /*  Hold */
                    if (Extr_HighSpd_RebalCnt < (*GESE_Extr_HighSpdUnbConfig).
                            Max_Redistributions &&
                            Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.Extr_HighSpd_TargetSpeed_ep
                        < Extr_Inputs_MinFinalSpeed)
                    {
                        /* Transition: '<S97>:359' */
                        Extraction_broadcast_EvStartRebal();
                    }
                    else
                    {
                        /* Transition: '<S97>:357' */
                        /*  Ext Plateau */
                        /* Transition: '<S97>:353' */
                        /* Transition: '<S97>:22' */
                        guard6 = true;
                    }
                }
                else
                {
                    /* Transition: '<S97>:212' */
                    /*  PlateauDecision == MONITOR */
                    guard6 = true;
                }
            }
        }

        if (guard8)
        {
            if (Extr_HighSpd_RebalCnt < (*GESE_Extr_HighSpdUnbConfig).
                    Max_Redistributions)
            {
                /* Transition: '<S97>:460' */
                guard2 = true;
            }
            else
            {
                /* Transition: '<S97>:480' */
                Extraction_SetFault
                    (EXTR_FAULT_HIGH_SPEED_REBALANCE_COUNT_EXCEEDED);
                guard4 = true;
            }
        }

        if (guard7)
        {
            /* Transition: '<S97>:409' */
            if (Extr_HighSpd_RebalCnt < (*GESE_Extr_HighSpdUnbConfig).
                    Max_Redistributions)
            {
                /* Transition: '<S97>:68' */
                guard2 = true;
            }
            else
            {
                /* Transition: '<S97>:283' */
                Extraction_SetFault
                    (EXTR_FAULT_HIGH_SPEED_REBALANCE_COUNT_EXCEEDED);
                guard3 = true;
            }
        }

        if (guard6)
        {
            Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.is_RUNNING
                = Extraction_IN_EXTENDED_PLATEAU;

            /* Entry 'EXTENDED_PLATEAU': '<S97>:420' */
            Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.ExtPlateauDuration
                = Extr_Inputs_ExtPlateauDuration;
            tmp_lul =
                Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.SpinPulse;
            Extraction_UpdatePlateauTime(&tmp_lul,
                Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.DecPlatTime,
                Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.ExtPlateauDuration,
                &unusedExpr_l);

            /* Entry Internal 'EXTENDED_PLATEAU': '<S97>:420' */
            /* Transition: '<S97>:424' */
            if (Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.is_EXTENDED_PLATEAU
                == Extraction_IN_EXTENDED_PLATEAU_RAMPING)
            {
            }
            else
            {
                Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.is_EXTENDED_PLATEAU
                    = Extraction_IN_EXTENDED_PLATEAU_RAMPING;
                Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.Extr_HighSpd_State_la
                    = HIGH_SPEED_STATE_EXTENDED_PLATEAU_RAMPING;

                /* Entry 'EXTENDED_PLATEAU_RAMPING': '<S97>:422' */
                if (Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.Extr_HighSpd_TargetSpeed_ep
                    < Extr_Inputs_MotorSignals.Speed)
                {
                    tmp_lulfkl =
                        Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.Extr_HighSpd_TargetSpeed_ep;
                    if (Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.Extr_HighSpd_TargetSpeed_ep
                        > 32767)
                    {
                        tmp_lulfkl = 32767U;
                    }

                    Extraction_SetSpeedCommand((sint16)tmp_lulfkl, 30);
                }
            }
        }

        if (guard5)
        {
            /* Transition: '<S97>:18' */
            /* Transition: '<S97>:166' */
            Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.is_RUNNING
                = Extraction_IN_CONFIGURING_PULSE_l3;
            Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.Extr_HighSpd_State_la
                = HIGH_SPEED_STATE_CONFIGURING_PULSE;
            Extraction_enter_atomic_CONFIGURING_PULSE_ad();
        }

        if (guard4)
        {
            /* Transition: '<S97>:484' */
            Extraction_StopMotor();

            /* Event: '<S97>:27' */
            Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.Extr_HighSpd_EvCompleteEventCounter
                ++;
            Extraction_broadcast_Local_EvStop();
            if (Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.is_HIGH_SPEED_SUPERVISOR
                != Extraction_IN_RUNNING_ny)
            {
            }
            else
            {
                /* Exit Internal 'RUNNING': '<S97>:7' */
                if (Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.is_RUNNING
                    == Extraction_IN_PLATEAU_mt)
                {
                    /* Exit 'PLATEAU': '<S97>:13' */
                    Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.PlateauDecision
                        = Extr_HighSpd_PlateauDecision;
                    if (Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.PlateauDecision
                        == HIGH_SPEED_PLATEAU_DECISION_REGRESS)
                    {
                        Extr_HighSpd_Fault =
                            EXTR_HIGH_SPD_FAULT_PLATEAU_REGRESS_DECISION;
                        Extraction_SetFault
                            (EXTR_FAULT_HIGH_SPD_PLATEAU_REGRESS_DECISION);
                    }

                    if (Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.PlateauDecision
                        == HIGH_SPEED_PLATEAU_DECISION_HOLD)
                    {
                        Extr_HighSpd_Fault =
                            EXTR_HIGH_SPD_FAULT_PLATEAU_HOLD_DECISION;
                        Extraction_SetFault
                            (EXTR_FAULT_HIGH_SPD_PLATEAU_HOLD_DECISION);
                    }

                    Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.is_RUNNING
                        = Extraction_IN_NO_ACTIVE_CHILD_fr;
                }
                else
                {
                    Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.is_RUNNING
                        = Extraction_IN_NO_ACTIVE_CHILD_fr;

                    /* Exit Internal 'EXTENDED_PLATEAU': '<S97>:420' */
                    Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.is_EXTENDED_PLATEAU
                        = Extraction_IN_NO_ACTIVE_CHILD_fr;
                }

                Extraction_enter_internal_RUNNING_n0();
            }
        }

        if (guard3)
        {
            /* Transition: '<S97>:297' */
            /* Transition: '<S97>:90' */
            unusedExpr_l = roundf(Extr_Inputs_MotorSignals.Speed);
            if (unusedExpr_l < 65536.0F)
            {
                if (unusedExpr_l >= 0.0F)
                {
                    tmp_lulfkl = (uint16)unusedExpr_l;
                }
                else
                {
                    tmp_lulfkl = 0U;
                }
            }
            else
            {
                tmp_lulfkl = MAX_uint16_T;
            }

            qY_l = (uint32)tmp_lulfkl - (*GESE_Extr_HighSpdConfig).
                Delta_Speed_Drop;
            if (qY_l > tmp_lulfkl)
            {
                qY_l = 0U;
            }

            Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.Extr_HighSpd_TargetSpeed_ep
                = (uint16)qY_l;
            Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.DeltaSpeedDropExecuted
                = true;

            /* Transition: '<S97>:268' */
            /* Transition: '<S97>:228' */
            /* Exit Internal 'RUNNING': '<S97>:7' */
            if (Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.is_RUNNING
                == Extraction_IN_PLATEAU_mt)
            {
                /* Exit 'PLATEAU': '<S97>:13' */
                Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.PlateauDecision
                    = Extr_HighSpd_PlateauDecision;
                if (Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.PlateauDecision
                    == HIGH_SPEED_PLATEAU_DECISION_REGRESS)
                {
                    Extr_HighSpd_Fault =
                        EXTR_HIGH_SPD_FAULT_PLATEAU_REGRESS_DECISION;
                    Extraction_SetFault
                        (EXTR_FAULT_HIGH_SPD_PLATEAU_REGRESS_DECISION);
                }

                if (Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.PlateauDecision
                    == HIGH_SPEED_PLATEAU_DECISION_HOLD)
                {
                    Extr_HighSpd_Fault =
                        EXTR_HIGH_SPD_FAULT_PLATEAU_HOLD_DECISION;
                    Extraction_SetFault
                        (EXTR_FAULT_HIGH_SPD_PLATEAU_HOLD_DECISION);
                }

                Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.is_RUNNING
                    = Extraction_IN_NO_ACTIVE_CHILD_fr;
            }
            else
            {
                Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.is_RUNNING
                    = Extraction_IN_NO_ACTIVE_CHILD_fr;

                /* Exit Internal 'EXTENDED_PLATEAU': '<S97>:420' */
                Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.is_EXTENDED_PLATEAU
                    = Extraction_IN_NO_ACTIVE_CHILD_fr;
            }

            if (Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.is_RUNNING
                == Extraction_IN_EXTENDED_PLATEAU)
            {
            }
            else
            {
                Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.is_RUNNING
                    = Extraction_IN_EXTENDED_PLATEAU;

                /* Entry 'EXTENDED_PLATEAU': '<S97>:420' */
                Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.ExtPlateauDuration
                    = Extr_Inputs_ExtPlateauDuration;
                tmp_l =
                    Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.SpinPulse;
                Extraction_UpdatePlateauTime(&tmp_l,
                    Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.DecPlatTime,
                    Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.ExtPlateauDuration,
                    &unusedExpr_l);
            }

            /* Entry Internal 'EXTENDED_PLATEAU': '<S97>:420' */
            /* Transition: '<S97>:424' */
            if (Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.is_EXTENDED_PLATEAU
                == Extraction_IN_EXTENDED_PLATEAU_RAMPING)
            {
            }
            else
            {
                Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.is_EXTENDED_PLATEAU
                    = Extraction_IN_EXTENDED_PLATEAU_RAMPING;
                Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.Extr_HighSpd_State_la
                    = HIGH_SPEED_STATE_EXTENDED_PLATEAU_RAMPING;

                /* Entry 'EXTENDED_PLATEAU_RAMPING': '<S97>:422' */
                if (Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.Extr_HighSpd_TargetSpeed_ep
                    < Extr_Inputs_MotorSignals.Speed)
                {
                    tmp_lulfkl =
                        Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.Extr_HighSpd_TargetSpeed_ep;
                    if (Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.Extr_HighSpd_TargetSpeed_ep
                        > 32767)
                    {
                        tmp_lulfkl = 32767U;
                    }

                    Extraction_SetSpeedCommand((sint16)tmp_lulfkl, 30);
                }
            }
        }

        if (guard2)
        {
            /* Transition: '<S97>:454' */
            /* Exit Internal 'RUNNING': '<S97>:7' */
            if (Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.is_RUNNING
                == Extraction_IN_PLATEAU_mt)
            {
                /* Exit 'PLATEAU': '<S97>:13' */
                Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.PlateauDecision
                    = Extr_HighSpd_PlateauDecision;
                if (Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.PlateauDecision
                    == HIGH_SPEED_PLATEAU_DECISION_REGRESS)
                {
                    Extr_HighSpd_Fault =
                        EXTR_HIGH_SPD_FAULT_PLATEAU_REGRESS_DECISION;
                    Extraction_SetFault
                        (EXTR_FAULT_HIGH_SPD_PLATEAU_REGRESS_DECISION);
                }

                if (Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.PlateauDecision
                    == HIGH_SPEED_PLATEAU_DECISION_HOLD)
                {
                    Extr_HighSpd_Fault =
                        EXTR_HIGH_SPD_FAULT_PLATEAU_HOLD_DECISION;
                    Extraction_SetFault
                        (EXTR_FAULT_HIGH_SPD_PLATEAU_HOLD_DECISION);
                }

                Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.is_RUNNING
                    = Extraction_IN_NO_ACTIVE_CHILD_fr;
            }
            else
            {
                Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.is_RUNNING
                    = Extraction_IN_NO_ACTIVE_CHILD_fr;

                /* Exit Internal 'EXTENDED_PLATEAU': '<S97>:420' */
                Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.is_EXTENDED_PLATEAU
                    = Extraction_IN_NO_ACTIVE_CHILD_fr;
            }

            Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.is_HIGH_SPEED_SUPERVISOR
                = Extraction_IN_REBALANCE_nu;
            Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.Extr_HighSpd_State_la
                = HIGH_SPEED_STATE_REBALANCE;

            /* Entry 'REBALANCE': '<S97>:138' */
            tmp_lulfklt = (sint32)(Extr_HighSpd_RebalCnt + 1U);
            if ((uint32)tmp_lulfklt > 255U)
            {
                tmp_lulfklt = 255;
            }

            Extr_HighSpd_RebalCnt = (uint8)tmp_lulfklt;
            tmp_lulfklt =
                Extraction_B.VariantMergeForOutportExtr_HighSpd_CurrPulseInd + 1;
            if (tmp_lulfklt > 127)
            {
                tmp_lulfklt = 127;
            }

            tmp_lulfk =
                Extraction_B.VariantMergeForOutportExtr_HighSpd_CurrPulseInd + 1;
            if (tmp_lulfk > 127)
            {
                tmp_lulfk = 127;
            }

            tmp_lulfk = (sint32)
                (Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.Extr_HighSpd_RebalCntArray
                 [tmp_lulfk - 1] + 1U);
            if ((uint32)tmp_lulfk > 255U)
            {
                tmp_lulfk = 255;
            }

            Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.Extr_HighSpd_RebalCntArray
                [tmp_lulfklt - 1] = (uint8)tmp_lulfk;
            tmp_lulfklt =
                Extraction_B.VariantMergeForOutportExtr_HighSpd_CurrPulseInd + 1;
            if (tmp_lulfklt > 127)
            {
                tmp_lulfklt = 127;
            }

            if (Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.Extr_HighSpd_RebalCntArray
                    [tmp_lulfklt - 1] >
                    Extraction_B.VariantMergeForOutportExtr_HighSpd_BaselineConfig.Max_Redistributions)
            {
                /*  Req 250641 */
                tmp_lulfklt =
                    Extraction_B.VariantMergeForOutportExtr_HighSpd_CurrPulseInd
                    + 1;
                if (tmp_lulfklt > 127)
                {
                    tmp_lulfklt = 127;
                }

                Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.Extr_HighSpd_RebalCntArray
                    [tmp_lulfklt - 1] = 0U;
                tmp_lulfklt =
                    Extraction_B.VariantMergeForOutportExtr_HighSpd_CurrPulseInd
                    + 1;
                if (tmp_lulfklt > 127)
                {
                    tmp_lulfklt = 127;
                }

                Extraction_IncreaseCurrentBaselineFriction((sint8)tmp_lulfklt);
            }

            /* Event: '<S97>:74' */
            Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.Extr_HighSpd_EvStartRebalEventCounter
                ++;

            /* Event: '<S97>:81' */
            Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.Extr_HighSpd_EvStopAllEventCounter
                ++;
        }

        if (guard1)
        {
            /* Exit Internal 'RUNNING': '<S97>:7' */
            if (Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.is_RUNNING
                == Extraction_IN_PLATEAU_mt)
            {
                /* Exit 'PLATEAU': '<S97>:13' */
                Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.PlateauDecision
                    = Extr_HighSpd_PlateauDecision;
                if (Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.PlateauDecision
                    == HIGH_SPEED_PLATEAU_DECISION_REGRESS)
                {
                    Extr_HighSpd_Fault =
                        EXTR_HIGH_SPD_FAULT_PLATEAU_REGRESS_DECISION;
                    Extraction_SetFault
                        (EXTR_FAULT_HIGH_SPD_PLATEAU_REGRESS_DECISION);
                }

                if (Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.PlateauDecision
                    == HIGH_SPEED_PLATEAU_DECISION_HOLD)
                {
                    Extr_HighSpd_Fault =
                        EXTR_HIGH_SPD_FAULT_PLATEAU_HOLD_DECISION;
                    Extraction_SetFault
                        (EXTR_FAULT_HIGH_SPD_PLATEAU_HOLD_DECISION);
                }

                Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.is_RUNNING
                    = Extraction_IN_NO_ACTIVE_CHILD_fr;
            }
            else
            {
                Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.is_RUNNING
                    = Extraction_IN_NO_ACTIVE_CHILD_fr;

                /* Exit Internal 'EXTENDED_PLATEAU': '<S97>:420' */
                Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.is_EXTENDED_PLATEAU
                    = Extraction_IN_NO_ACTIVE_CHILD_fr;
            }

            Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.is_HIGH_SPEED_SUPERVISOR
                = Extraction_IN_IDLE_ow;
            Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.Extr_HighSpd_State_la
                = HIGH_SPEED_STATE_IDLE;

            /* Entry 'IDLE': '<S97>:5' */
            Extraction_B.VariantMergeForOutportExtr_HighSpd_CurrPulseInd = 0;
        }
    }
}

#endif                                 /* EXTR_VARIANT_HIGH_SPEED_PE_ENABLED */

/* Function for Chart: '<S93>/Extr_HighSpd_SupervisorLogic' */
#if EXTR_VARIANT_HIGH_SPEED_PE_ENABLED

static void Extraction_c17_Extraction(void)
{
    sint32 ii_l;

    /* During: Extraction/Extr_HighSpd/Extr_HighSpd_Supervisor/Extr_HighSpd_Supervisor/Extr_HighSpd_SupervisorLogic/Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled/Extr_HighSpd_SupervisorLogic */
    if (Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.is_active_c17_Extraction
        == 0U)
    {
        /* Entry: Extraction/Extr_HighSpd/Extr_HighSpd_Supervisor/Extr_HighSpd_Supervisor/Extr_HighSpd_SupervisorLogic/Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled/Extr_HighSpd_SupervisorLogic */
        Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.is_active_c17_Extraction
            = 1U;

        /* Entry Internal: Extraction/Extr_HighSpd/Extr_HighSpd_Supervisor/Extr_HighSpd_Supervisor/Extr_HighSpd_SupervisorLogic/Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled/Extr_HighSpd_SupervisorLogic */
        /* Transition: '<S97>:291' */
        Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.is_c17_Extraction
            = Extraction_IN_HIGH_SPEED_SUPERVISOR_ig;

        /* Entry Internal 'HIGH_SPEED_SUPERVISOR': '<S97>:290' */
        /* Transition: '<S97>:6' */
        if (Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.is_HIGH_SPEED_SUPERVISOR
            == Extraction_IN_IDLE_ow)
        {
        }
        else
        {
            Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.is_HIGH_SPEED_SUPERVISOR
                = Extraction_IN_IDLE_ow;
            Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.Extr_HighSpd_State_la
                = HIGH_SPEED_STATE_IDLE;

            /* Entry 'IDLE': '<S97>:5' */
            Extraction_B.VariantMergeForOutportExtr_HighSpd_CurrPulseInd = 0;
        }
    }
    else if (Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.is_c17_Extraction
             != Extraction_IN_HIGH_SPEED_SUPERVISOR_ig)
    {
    }
    else
    {
        /* During 'HIGH_SPEED_SUPERVISOR': '<S97>:290' */
        if (Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.sfEvent
            == Extraction_event_Extr_Sup_ResetRebalCnt_fl)
        {
            Extr_HighSpd_RebalCnt = 0U;
            for (ii_l = 1; ii_l <= ((sint32)BASELINE_ARRAY_MAX_SIZE); ii_l++)
            {
                Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.Extr_HighSpd_RebalCntArray
                    [ii_l - 1] = 0U;
            }
        }

        if (Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.sfEvent
            == Extraction_event_Extr_Sup_ClearFault_kr)
        {
            Extr_HighSpd_Fault = EXTR_HIGH_SPD_FAULT_NONE;
        }

        switch
            (Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.is_HIGH_SPEED_SUPERVISOR)
        {
          case Extraction_IN_IDLE_ow:
            /* During 'IDLE': '<S97>:5' */
            if (Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.sfEvent
                == Extraction_event_Extr_Sup_EvHighSpdStart_px)
            {
                /* Transition: '<S97>:10' */
                if (Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.is_HIGH_SPEED_SUPERVISOR
                    != Extraction_IN_IDLE_ow)
                {
                }
                else
                {
                    /* Exit 'IDLE': '<S97>:5' */
                    Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.DeltaSpeedDropExecuted
                        = false;
                    Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.FinalPlateauVerificationRequired
                        = true;
                    Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.ExtPlateauDurationIncreased
                        = false;
                    Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.is_HIGH_SPEED_SUPERVISOR
                        = Extraction_IN_NO_ACTIVE_CHILD_fr;
                }

                if (Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.is_HIGH_SPEED_SUPERVISOR
                    == Extraction_IN_RUNNING_ny)
                {
                }
                else
                {
                    Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.is_HIGH_SPEED_SUPERVISOR
                        = Extraction_IN_RUNNING_ny;

                    /* Entry 'RUNNING': '<S97>:7' */
                    Extr_HighSpd_ReferenceFriction = 0.0F;
                    Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.Extr_HighSpd_TargetSpeed_ep
                        = 0U;
                    Extraction_B.VariantMergeForOutportExtr_HighSpd_CurrPulseInd
                        = 0;
                    Extr_HighSpd_PrevPlateauTime = -1.0F;
                    Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.ExtPlateauElapsedTime
                        = 0.0F;
                    Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.PlateauDecision
                        = HIGH_SPEED_PLATEAU_DECISION_NONE;
                    if ((*GESE_Extr_SpinProfileConfig).
                            Factory_Calibration_Enabled != 0)
                    {
                        Extraction_ClearBaselineFrictionArrays();
                    }
                    else
                    {
                        Extraction_VerifyBaselineFrictionArray();
                    }
                }

                Extraction_enter_internal_RUNNING_n0();
            }
            break;

          case Extraction_IN_REBALANCE_nu:
            /* During 'REBALANCE': '<S97>:138' */
            switch
                (Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.sfEvent)
            {
              case Extraction_event_Extr_Sup_EvHighSpdStart_px:
                /* Transition: '<S97>:139' */
                Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.is_HIGH_SPEED_SUPERVISOR
                    = Extraction_IN_RUNNING_ny;

                /* Entry 'RUNNING': '<S97>:7' */
                Extr_HighSpd_ReferenceFriction = 0.0F;
                Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.Extr_HighSpd_TargetSpeed_ep
                    = 0U;
                Extraction_B.VariantMergeForOutportExtr_HighSpd_CurrPulseInd = 0;
                Extr_HighSpd_PrevPlateauTime = -1.0F;
                Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.ExtPlateauElapsedTime
                    = 0.0F;
                Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.PlateauDecision
                    = HIGH_SPEED_PLATEAU_DECISION_NONE;
                if ((*GESE_Extr_SpinProfileConfig).Factory_Calibration_Enabled
                        != 0)
                {
                    Extraction_ClearBaselineFrictionArrays();
                }
                else
                {
                    Extraction_VerifyBaselineFrictionArray();
                }

                Extraction_enter_internal_RUNNING_n0();
                break;

              case Extraction_event_Extr_Sup_EvStopAll_dp:
                /* Transition: '<S97>:141' */
                Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.is_HIGH_SPEED_SUPERVISOR
                    = Extraction_IN_IDLE_ow;
                Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.Extr_HighSpd_State_la
                    = HIGH_SPEED_STATE_IDLE;

                /* Entry 'IDLE': '<S97>:5' */
                Extraction_B.VariantMergeForOutportExtr_HighSpd_CurrPulseInd = 0;
                break;
            }
            break;

          case Extraction_IN_RUNNING_ny:
            Extraction_RUNNING_ob();
            break;
        }
    }
}

#endif                                 /* EXTR_VARIANT_HIGH_SPEED_PE_ENABLED */

/* Function for Chart: '<S93>/Extr_HighSpd_SupervisorLogic' */
#if EXTR_VARIANT_HIGH_SPEED_PE_ENABLED

static void Extraction_chartstep_c17_Extraction(void)
{
    Extraction_c17_Extraction();
}

#endif                                 /* EXTR_VARIANT_HIGH_SPEED_PE_ENABLED */

/* Output and update for Simulink Function: '<S85>/LoadSpinPulse' */
void Extraction_LoadSpinPulse(sint8 rtu_index, SPIN_PULSE_TYPE *rty_pulse)
{
    /* FunctionCaller: '<S87>/Function Caller' incorporates:
     *  SignalConversion: '<S87>/TmpSignal ConversionAtindexOutport1'
     */
    Extraction_UpdateSpinPulseConfig(rtu_index);

    /* Selector: '<S87>/Selector1' incorporates:
     *  DataStoreRead: '<S87>/DataStoreRead2'
     *  SignalConversion: '<S87>/TmpSignal ConversionAtindexOutport1'
     */
    Extraction_B.Selector1 = GESE_Extr_SpinPulses[rtu_index];

    /* SignalConversion: '<S87>/TmpSignal ConversionAtpulseInport1' */
    *rty_pulse = Extraction_B.Selector1;

    /* UnitDelay: '<S87>/PrevSpinPulse' */
    Extraction_B.PrevSpinPulse = Extraction_DW.PrevSpinPulse_DSTATE;

    /* Update for UnitDelay: '<S87>/PrevSpinPulse' */
    Extraction_DW.PrevSpinPulse_DSTATE = Extraction_B.Selector1;
}

/* Output and update for Simulink Function: '<S85>/UpdateCurrentPulseConfig' */
void Extraction_UpdateSpinPulseConfig(sint8 rtu_index)
{
    /* local block i/o variables */
    sint8 rtb_TmpSignalConversionAtindexOutport1_czi;

    /* SignalConversion: '<S89>/TmpSignal ConversionAtindexOutport1' */
    rtb_TmpSignalConversionAtindexOutport1_czi = rtu_index;

    /* Outputs for Atomic SubSystem: '<S89>/UpdateSpinPulseConfig' */
#if EXTR_VARIANT_HIGH_SPEED_PE_ENABLED

    /* Output and update for atomic system: '<S108>/UpdateSpinPulseConfig_Enabled' */
    {
        float32 rtb_MaxFriction;
        uint16 rtb_BaselineDefaultFrictionLUT[25];
        sint32 i_m;
        float32 PrevBaselineFriction_DSTATE_m;

        /* UnitDelay: '<S109>/PrevBaselineFriction' */
        PrevBaselineFriction_DSTATE_m =
            Extraction_DW.UpdateSpinPulseConfig_Enabled.PrevBaselineFriction_DSTATE;

        /* PreLookup: '<S115>/SpeedRefPreLookup' incorporates:
         *  DataStoreRead: '<S109>/DataStoreRead2'
         *  Selector: '<S109>/Selector1'
         */
        Extr_HighSpd_BaselineFrictionIndex = plook_u8u8f_binca
            (GESE_Extr_SpinPulses[rtb_TmpSignalConversionAtindexOutport1_czi].
             Target_Speed_Div10, (&(GESE_Extr_BaselineFrictionRefSpeed_BP.BP[0])),
             GESE_Extr_BaselineFrictionRefSpeed_BP.N - 1U, &rtb_MaxFriction);

        /* DataStoreRead: '<S115>/BaselineDefaultFrictionLUT' */
        for (i_m = 0; i_m < 25; i_m++)
        {
            rtb_BaselineDefaultFrictionLUT[i_m] =
                ExtractionParams__GetBaselineFriction( i_m );
        }

        /* End of DataStoreRead: '<S115>/BaselineDefaultFrictionLUT' */

        /* Interpolation_n-D: '<S115>/Interpolation Using Prelookup' */
        rtb_MaxFriction = intrp1d_u16ffu8fla(Extr_HighSpd_BaselineFrictionIndex,
            rtb_MaxFriction, rtb_BaselineDefaultFrictionLUT,
            GESE_Extr_BaselineFrictionRefSpeed_BP.N - 1U);

        /* Gain: '<S115>/Gain' incorporates:
         *  DataTypeConversion: '<S115>/Data Type Conversion'
         *  Interpolation_n-D: '<S115>/Interpolation Using Prelookup'
         */
        Extr_HighSpd_BaselineFriction = 0.001F * (float32)(uint16)
            rtb_MaxFriction;

        /* MATLAB Function: '<S109>/GetBaselineConfig' incorporates:
         *  DataStoreRead: '<S109>/DataStoreRead2'
         *  Selector: '<S109>/Selector1'
         */
        /* MATLAB Function 'Extraction/Extr_HighSpd/Extr_HighSpd_Supervisor/Extr_HighSpd_Supervisor/UpdateCurrentPulseConfig/UpdateSpinPulseConfig/UpdateSpinPulseConfig_Enabled/GetBaselineConfig': '<S111>:1' */
        /* '<S111>:1:5' */
        Extraction_B.VariantMergeForOutportExtr_HighSpd_BaselineConfig =
            ExtractionParams__GetHighSpdBaselineConfig
            (GESE_Extr_SpinPulses[rtb_TmpSignalConversionAtindexOutport1_czi].
             Baseline_Config_Index);

        /* Gain: '<S109>/MaxFriction' */
        rtb_MaxFriction = 0.000999987125F * (float32)
            Extraction_B.VariantMergeForOutportExtr_HighSpd_BaselineConfig.Friction_Max_x1000;

        /* Switch: '<S110>/Switch2' incorporates:
         *  RelationalOperator: '<S110>/LowerRelop1'
         */
        if (Extr_HighSpd_BaselineFriction > rtb_MaxFriction)
        {
            Extr_HighSpd_BaselineFriction = rtb_MaxFriction;
        }
        else
        {
            /* Gain: '<S109>/MinFriction' */
            rtb_MaxFriction = 0.000999987125F * (float32)
                Extraction_B.VariantMergeForOutportExtr_HighSpd_BaselineConfig.Friction_Min_x1000;

            /* Switch: '<S110>/Switch' incorporates:
             *  RelationalOperator: '<S110>/UpperRelop'
             */
            if (Extr_HighSpd_BaselineFriction < rtb_MaxFriction)
            {
                Extr_HighSpd_BaselineFriction = rtb_MaxFriction;
            }

            /* End of Switch: '<S110>/Switch' */
        }

        /* End of Switch: '<S110>/Switch2' */

        /* MATLAB Function: '<S109>/GetLoadMassLimitConfig' incorporates:
         *  DataStoreRead: '<S109>/DataStoreRead2'
         *  Selector: '<S109>/Selector1'
         */
        /* MATLAB Function 'Extraction/Extr_HighSpd/Extr_HighSpd_Supervisor/Extr_HighSpd_Supervisor/UpdateCurrentPulseConfig/UpdateSpinPulseConfig/UpdateSpinPulseConfig_Enabled/GetLoadMassLimitConfig': '<S112>:1' */
        /* '<S112>:1:5' */
        Extraction_B.VariantMergeForOutportExtr_HighSpd_LoadMassLimitConfig =
            ExtractionParams__GetLoadMassLimitConfig
            (GESE_Extr_SpinPulses[rtb_TmpSignalConversionAtindexOutport1_czi].
             Load_Mass_Config_Index);

        /* MATLAB Function: '<S109>/GetPEAppSpecificConfig' incorporates:
         *  DataStoreRead: '<S109>/DataStoreRead2'
         *  Selector: '<S109>/Selector1'
         */
        /* MATLAB Function 'Extraction/Extr_HighSpd/Extr_HighSpd_Supervisor/Extr_HighSpd_Supervisor/UpdateCurrentPulseConfig/UpdateSpinPulseConfig/UpdateSpinPulseConfig_Enabled/GetPEAppSpecificConfig': '<S113>:1' */
        /* '<S113>:1:5' */
        Extraction_B.VariantMergeForOutportExtr_HighSpd_ParamEstAppSpecificSetup
            = ExtractionParams__GetParamEstAppSpecificSetup
            (GESE_Extr_SpinPulses[rtb_TmpSignalConversionAtindexOutport1_czi].
             Param_Est_App_Specific_Config_Index);

        /* MATLAB Function: '<S109>/GetPulseConfig' incorporates:
         *  DataStoreRead: '<S109>/DataStoreRead2'
         *  Selector: '<S109>/Selector1'
         */
        /* MATLAB Function 'Extraction/Extr_HighSpd/Extr_HighSpd_Supervisor/Extr_HighSpd_Supervisor/UpdateCurrentPulseConfig/UpdateSpinPulseConfig/UpdateSpinPulseConfig_Enabled/GetPulseConfig': '<S114>:1' */
        /* '<S114>:1:5' */
        Extraction_B.VariantMergeForOutportExtr_HighSpd_PulseConfig =
            ExtractionParams__GetHighSpdPulseConfig
            (GESE_Extr_SpinPulses[rtb_TmpSignalConversionAtindexOutport1_czi].
             High_Speed_Config_Index);

        /* UnitDelay: '<S109>/PrevBaselineFriction' */
        Extr_HighSpd_PrevBaselineFriction = PrevBaselineFriction_DSTATE_m;

        /* UnitDelay: '<S109>/PrevLoadMassLimitConfig' */
        Extraction_B.VariantMergeForOutportExtr_HighSpd_PrevLoadMassLimitConfig =
            Extraction_DW.UpdateSpinPulseConfig_Enabled.PrevLoadMassLimitConfig_DSTATE;

        /* UnitDelay: '<S109>/PrevPulseConfig' */
        Extraction_B.VariantMergeForOutportExtr_HighSpd_PrevPulseConfig =
            Extraction_DW.UpdateSpinPulseConfig_Enabled.PrevPulseConfig_DSTATE;

        /* Selector: '<S115>/Selector' incorporates:
         *  DataStoreRead: '<S115>/BaselineFrictionRefSpeedArray'
         */
        Extr_HighSpd_BaselineSpeed =
            GESE_Extr_BaselineFrictionRefSpeed_Sig.BP[Extr_HighSpd_BaselineFrictionIndex];

        /* Update for UnitDelay: '<S109>/PrevLoadMassLimitConfig' */
        Extraction_DW.UpdateSpinPulseConfig_Enabled.PrevLoadMassLimitConfig_DSTATE
            =
            Extraction_B.VariantMergeForOutportExtr_HighSpd_LoadMassLimitConfig;

        /* Update for UnitDelay: '<S109>/PrevPulseConfig' */
        Extraction_DW.UpdateSpinPulseConfig_Enabled.PrevPulseConfig_DSTATE =
            Extraction_B.VariantMergeForOutportExtr_HighSpd_PulseConfig;
        PrevBaselineFriction_DSTATE_m = Extr_HighSpd_BaselineFriction;

        /* UnitDelay: '<S109>/PrevBaselineFriction' */
        Extraction_DW.UpdateSpinPulseConfig_Enabled.PrevBaselineFriction_DSTATE =
            PrevBaselineFriction_DSTATE_m;
    }

#endif                                 /* EXTR_VARIANT_HIGH_SPEED_PE_ENABLED */

    /* End of Outputs for SubSystem: '<S89>/UpdateSpinPulseConfig' */
}

/* Output and update for Simulink Function: '<S85>/UpdatePlateauTime' */
void Extraction_UpdatePlateauTime(const SPIN_PULSE_TYPE *rtu_spin_pulse,
    boolean_T rtu_dec_plat_time_flag, float32 rtu_time_override, float32
    *rty_duration)
{
    float32 loadMassLimits_idx_0;
    float32 loadMassLimits_idx_1;
    uint32 qY;
    float32 Extr_Sup_LoadMass_0;
    float32 Extr_HighSpd_PrevPlateauTime_0;

    /* MATLAB Function: '<S90>/MATLAB Function' */
    Extr_HighSpd_PrevPlateauTime_0 = Extr_HighSpd_PrevPlateauTime;

    /* DataStoreRead: '<S90>/Data Store Read1' */
    Extr_Sup_LoadMass_0 = Extr_Sup_LoadMass;

    /* MATLAB Function: '<S90>/MATLAB Function' incorporates:
     *  DataStoreRead: '<S90>/Data Store Read'
     *  SignalConversion: '<S90>/TmpSignal ConversionAtdec_plat_time_flagOutport1'
     *  SignalConversion: '<S90>/TmpSignal ConversionAtspin_pulseOutport1'
     *  SignalConversion: '<S90>/TmpSignal ConversionAttime_overrideOutport1'
     */
    /* MATLAB Function 'Extraction/Extr_HighSpd/Extr_HighSpd_Supervisor/Extr_HighSpd_Supervisor/UpdatePlateauTime/MATLAB Function': '<S116>:1' */
    if (rtu_time_override >= 0.0F)
    {
        /* '<S116>:1:5' */
        /* '<S116>:1:6' */
        Extr_HighSpd_PlateauDuration = rtu_time_override;
    }
    else if (rtu_dec_plat_time_flag)
    {
        /* '<S116>:1:8' */
        /* '<S116>:1:9' */
        Extr_HighSpd_PlateauDuration = 0.0F;
    }
    else
    {
        /* '<S116>:1:11' */
        /* '<S116>:1:25' */
        loadMassLimits_idx_0 = (float32)(*GESE_Extr_SpinProfileConfig).
            Load_Mass_x10[0] / 10.0F;
        loadMassLimits_idx_1 = (float32)(*GESE_Extr_SpinProfileConfig).
            Load_Mass_x10[1] / 10.0F;
        if ((*GESE_Extr_SpinProfileConfig).Inertia_Dependent_Profile_Enabled !=
                0)
        {
            if (Extr_Sup_LoadMass_0 <= loadMassLimits_idx_0)
            {
                /* '<S116>:1:28' */
                /* '<S116>:1:29' */
                Extr_HighSpd_PlateauDuration = rtu_spin_pulse->Plateau_Time[0];
            }
            else if (Extr_Sup_LoadMass_0 >= loadMassLimits_idx_1)
            {
                /* '<S116>:1:31' */
                /* '<S116>:1:32' */
                Extr_HighSpd_PlateauDuration = rtu_spin_pulse->Plateau_Time[1];
            }
            else
            {
                /* '<S116>:1:34' */
                /* '<S116>:1:35' */
                loadMassLimits_idx_1 -= loadMassLimits_idx_0;

                /*  Verify division by zero */
                if (loadMassLimits_idx_1 == 0.0F)
                {
                    /* '<S116>:1:37' */
                    /* '<S116>:1:38' */
                    Extr_HighSpd_PlateauDuration = rtu_spin_pulse->Plateau_Time
                        [1];
                }
                else
                {
                    /* '<S116>:1:40' */
                    qY = (uint32)rtu_spin_pulse->Plateau_Time[1] -
                        rtu_spin_pulse->Plateau_Time[0];
                    if (qY > rtu_spin_pulse->Plateau_Time[1])
                    {
                        qY = 0U;
                    }

                    Extr_HighSpd_PlateauDuration = (float32)(sint32)qY *
                        (Extr_Sup_LoadMass_0 - loadMassLimits_idx_0) /
                        loadMassLimits_idx_1 + (float32)
                        rtu_spin_pulse->Plateau_Time[0];
                }
            }
        }
        else
        {
            /* '<S116>:1:45' */
            Extr_HighSpd_PlateauDuration = rtu_spin_pulse->Plateau_Time[0];
        }
    }

    if (Extr_HighSpd_PrevPlateauTime_0 == 0.0F)
    {
        /* '<S116>:1:15' */
        /* '<S116>:1:16' */
        Extraction_B.cov_reset_on_ramp = false;
    }
    else
    {
        /* '<S116>:1:18' */
        Extraction_B.cov_reset_on_ramp = true;
    }

    /* SignalConversion: '<S90>/TmpSignal ConversionAtdurationInport1' incorporates:
     *  MATLAB Function: '<S90>/MATLAB Function'
     */
    /* '<S116>:1:21' */
    *rty_duration = Extr_HighSpd_PlateauDuration;

    /* MATLAB Function: '<S90>/MATLAB Function' */
    Extr_HighSpd_PrevPlateauTime_0 = Extr_HighSpd_PlateauDuration;
    Extr_HighSpd_PrevPlateauTime = Extr_HighSpd_PrevPlateauTime_0;
}

/* Output and update for Simulink Function: '<S85>/VerifyVibMapFloorDisplacement' */
void Extraction_VerifyVibMapFloorDisplacement(uint8 rtu_target_speed_div10,
    uint16 *rty_speed_limit, boolean_T *rty_decrease_plateau_time)
{
    /* local block i/o variables */
    float32 rtb_Extr_LowSpd_LoadMass_kt;
    uint16 rtb_VariantMergeForOutportExtr_HighSpd_TargetSpeed;
    uint8 rtb_TmpSignalConversionAttarget_speed_div10Outport1;
    boolean_T rtb_VariantMergeForOutportExtr_HighSpd_DecreasePlateauTime;

    /* SignalConversion: '<S91>/TmpLatchAtExtr_LowSpd_LoadMassOutport1' */
    rtb_Extr_LowSpd_LoadMass_kt = Extraction_B.Extr_LowSpd_LoadMass_gp;

    /* SignalConversion: '<S91>/TmpSignal ConversionAttarget_speed_div10Outport1' */
    rtb_TmpSignalConversionAttarget_speed_div10Outport1 = rtu_target_speed_div10;

    /* Outputs for Atomic SubSystem: '<S91>/VerifyVibMapFloorDispSubsystem' */
#if EXTR_VARIANT_ACCELEROMETER_DISABLED

    /* Output and update for atomic system: '<S117>/VerifyVibMapFloorDispDisabled' */

    /* Constant: '<S118>/ZERO' */
    Extraction_B.VariantMergeForOutportExtr_HighSpd_PlateauDurationIncrement =
        0U;

    /* Constant: '<S118>/MAX_UINT16' */
    rtb_VariantMergeForOutportExtr_HighSpd_TargetSpeed = MAX_uint16_T;

    /* Constant: '<S118>/FALSE' */
    rtb_VariantMergeForOutportExtr_HighSpd_DecreasePlateauTime = false;

#elif EXTR_VARIANT_ACCELEROMETER_ENABLED

    /* Output and update for atomic system: '<S117>/VerifyVibMapFloorDispEnabled' */
    {
        uint8 jj_e;
        boolean_T ref_disp_exceeded_e;
        uint8 b_ii_e;
        float32 time_e;
        float32 rtb_Sum;
        float32 y_idx_1_e;
        sint32 q0_e;
        uint32 qY_e;
        uint16 tmp_e;
        uint16 Extr_Accel_VibMapRefDisp_e;

        /* Sum: '<S119>/Sum' incorporates:
         *  DataStoreRead: '<S119>/DataStoreRead'
         *  Fcn: '<S119>/Fcn'
         *  Gain: '<S119>/x10'
         *  Product: '<S119>/Product'
         */
        rtb_Sum = powf(10.0F * (float32)
                       rtb_TmpSignalConversionAttarget_speed_div10Outport1,
                       (*GESE_Extr_VibMapFDThreshold).Displacement_Threshold_B) *
            (*GESE_Extr_VibMapFDThreshold).Displacement_Threshold_A +
            (*GESE_Extr_VibMapFDThreshold).Displacement_Threshold_C;

        /* MATLAB Function: '<S119>/VerifyVibMapFloorDisplacementMATLAB' incorporates:
         *  DataStoreRead: '<S119>/DataStoreRead1'
         *  DataStoreRead: '<S119>/DataStoreRead2'
         *  DataStoreRead: '<S119>/DataStoreRead3'
         */
        /* MATLAB Function 'Extraction/Extr_HighSpd/Extr_HighSpd_Supervisor/Extr_HighSpd_Supervisor/VerifyVibMapFloorDisplacement/VerifyVibMapFloorDispSubsystem/VerifyVibMapFloorDispEnabled/VerifyVibMapFloorDisplacementMATLAB': '<S120>:1' */
        /* '<S120>:1:14' */
        Extraction_B.VariantMergeForOutportExtr_HighSpd_PlateauDurationIncrement
            = 0U;

        /* '<S120>:1:15' */
        rtb_VariantMergeForOutportExtr_HighSpd_TargetSpeed = (uint16)
            (rtb_TmpSignalConversionAttarget_speed_div10Outport1 * 10);

        /* '<S120>:1:16' */
        rtb_VariantMergeForOutportExtr_HighSpd_DecreasePlateauTime = false;

        /* '<S120>:1:17' */
        Extr_Accel_VibMapRefDisp_e = 0U;
        if (ExtractionParams__IsVibMapEnabled() != 0)
        {
            /* '<S120>:1:53' */
            ref_disp_exceeded_e = false;

            /* '<S120>:1:54' */
            for (b_ii_e = 1U; b_ii_e <= GESE_Extr_VibMapRefSpeedArray_div10.N;
                    b_ii_e++)
            {
                /* '<S120>:1:54' */
                if (GESE_Extr_VibMapRefSpeedArray_div10.BP[b_ii_e - 1] ==
                        rtb_TmpSignalConversionAttarget_speed_div10Outport1 &&
                        ExtractionParams__GetVibMapRefDisp( b_ii_e - 1 ) < 65535)
                {
                    /* '<S120>:1:55' */
                    /* '<S120>:1:56' */
                    time_e = roundf(rtb_Sum);
                    if (time_e < 65536.0F)
                    {
                        if (time_e >= 0.0F)
                        {
                            tmp_e = (uint16)time_e;
                        }
                        else
                        {
                            tmp_e = 0U;
                        }
                    }
                    else
                    {
                        tmp_e = MAX_uint16_T;
                    }

                    qY_e = tmp_e * 100U;
                    if (qY_e > 65535U)
                    {
                        qY_e = 65535U;
                    }

                    if (ExtractionParams__GetVibMapRefDisp( b_ii_e - 1 ) >
                            (sint32)qY_e)
                    {
                        /* '<S120>:1:57' */
                        /* '<S120>:1:58' */
                        ref_disp_exceeded_e = true;
                    }
                }
            }

            if (ref_disp_exceeded_e)
            {
                /* '<S120>:1:19' */
                if (rtb_TmpSignalConversionAttarget_speed_div10Outport1 <
                        (*GESE_Extr_VibMapSetup).Velocity_Dwell_Threshold_Div10)
                {
                    /* '<S120>:1:25' */
                    /* '<S120>:1:26' */
                    time_e = roundf(ExtractionParams__GetVibMapThrshCrossSpd());
                    if (time_e < 65536.0F)
                    {
                        if (time_e >= 0.0F)
                        {
                            rtb_VariantMergeForOutportExtr_HighSpd_TargetSpeed =
                                (uint16)time_e;
                        }
                        else
                        {
                            rtb_VariantMergeForOutportExtr_HighSpd_TargetSpeed =
                                0U;
                        }
                    }
                    else
                    {
                        rtb_VariantMergeForOutportExtr_HighSpd_TargetSpeed =
                            MAX_uint16_T;
                    }

                    /*  Check if next pulses are associated with reference */
                    /*  floor displacements larger than the threhsold */
                    /* '<S120>:1:29' */
                    for (b_ii_e = 1U; b_ii_e <= (*GESE_Extr_SpinProfileConfig).
                            Spin_Pulse_Count; b_ii_e++)
                    {
                        /* '<S120>:1:29' */
                        if (GESE_Extr_SpinPulses[b_ii_e - 1].Target_Speed_Div10 >
                            rtb_TmpSignalConversionAttarget_speed_div10Outport1)
                        {
                            /* '<S120>:1:30' */
                            /* '<S120>:1:31' */
                            for (jj_e = 1U; jj_e <=
                                    GESE_Extr_VibMapRefSpeedArray_div10.N; jj_e
                                    ++)
                            {
                                /* '<S120>:1:31' */
                                if (GESE_Extr_VibMapRefSpeedArray_div10.BP[jj_e
                                        - 1] ==
                                        rtb_TmpSignalConversionAttarget_speed_div10Outport1)
                                {
                                    /* '<S120>:1:32' */
                                    /* '<S120>:1:33' */
                                    Extr_Accel_VibMapRefDisp_e = (uint16)(sint32)
                                        roundf((float32)
                                               ExtractionParams__GetVibMapRefDisp
                                               ( jj_e - 1 ) / 100.0F);
                                    time_e = roundf(rtb_Sum);
                                    if (time_e < 65536.0F)
                                    {
                                        if (time_e >= 0.0F)
                                        {
                                            tmp_e = (uint16)time_e;
                                        }
                                        else
                                        {
                                            tmp_e = 0U;
                                        }
                                    }
                                    else
                                    {
                                        tmp_e = MAX_uint16_T;
                                    }

                                    qY_e = 100U * tmp_e;
                                    if (qY_e > 65535U)
                                    {
                                        qY_e = 65535U;
                                    }

                                    if (ExtractionParams__GetVibMapRefDisp( jj_e
                                         - 1 ) > (sint32)qY_e)
                                    {
                                        /* '<S120>:1:34' */
                                        /* '<S120>:1:35' */
                                        time_e = (float32)
                                            (*GESE_Extr_SpinProfileConfig).
                                            Load_Mass_x10[0] / 10.0F;
                                        y_idx_1_e = (float32)
                                            (*GESE_Extr_SpinProfileConfig).
                                            Load_Mass_x10[1] / 10.0F;
                                        if (rtb_Extr_LowSpd_LoadMass_kt <=
                                                time_e)
                                        {
                                            /* '<S120>:1:67' */
                                            /* '<S120>:1:68' */
                                            time_e = GESE_Extr_SpinPulses[b_ii_e
                                                - 1].Plateau_Time[0];
                                        }
                                        else if (rtb_Extr_LowSpd_LoadMass_kt >=
                                                 y_idx_1_e)
                                        {
                                            /* '<S120>:1:70' */
                                            /* '<S120>:1:71' */
                                            time_e = GESE_Extr_SpinPulses[b_ii_e
                                                - 1].Plateau_Time[1];
                                        }
                                        else
                                        {
                                            /* '<S120>:1:73' */
                                            /* '<S120>:1:74' */
                                            /* '<S120>:1:75' */
                                            q0_e = GESE_Extr_SpinPulses[b_ii_e -
                                                1].Plateau_Time[1];
                                            qY_e = (uint32)q0_e -
                                                GESE_Extr_SpinPulses[b_ii_e - 1]
                                                .Plateau_Time[0];
                                            if (qY_e > (uint32)q0_e)
                                            {
                                                qY_e = 0U;
                                            }

                                            time_e = (float32)(sint32)qY_e *
                                                (rtb_Extr_LowSpd_LoadMass_kt -
                                                 time_e) / (y_idx_1_e - time_e)
                                                + (float32)
                                                GESE_Extr_SpinPulses[b_ii_e - 1]
                                                .Plateau_Time[0];
                                        }

                                        /* '<S120>:1:78' */
                                        time_e = roundf(time_e);
                                        if (time_e < 65536.0F)
                                        {
                                            if (time_e >= 0.0F)
                                            {
                                                tmp_e = (uint16)time_e;
                                            }
                                            else
                                            {
                                                tmp_e = 0U;
                                            }
                                        }
                                        else
                                        {
                                            tmp_e = MAX_uint16_T;
                                        }

                                        qY_e = (uint32)
                                            Extraction_B.VariantMergeForOutportExtr_HighSpd_PlateauDurationIncrement
                                            + tmp_e;
                                        if (qY_e > 65535U)
                                        {
                                            qY_e = 65535U;
                                        }

                                        Extraction_B.VariantMergeForOutportExtr_HighSpd_PlateauDurationIncrement
                                            = (uint16)qY_e;
                                    }
                                }
                            }
                        }
                    }
                }
                else
                {
                    /* '<S120>:1:43' */
                    rtb_VariantMergeForOutportExtr_HighSpd_DecreasePlateauTime =
                        true;
                }
            }
        }

        /* End of MATLAB Function: '<S119>/VerifyVibMapFloorDisplacementMATLAB' */
        Extr_Accel_VibMapRefDisp = Extr_Accel_VibMapRefDisp_e;
    }

#endif                                 /* EXTR_VARIANT_ACCELEROMETER_DISABLED */

    /* End of Outputs for SubSystem: '<S91>/VerifyVibMapFloorDispSubsystem' */

    /* SignalConversion: '<S91>/TmpSignal ConversionAtdecrease_plateau_timeInport1' */
    *rty_decrease_plateau_time =
        rtb_VariantMergeForOutportExtr_HighSpd_DecreasePlateauTime;

    /* SignalConversion: '<S91>/TmpSignal ConversionAtspeed_limitInport1' */
    *rty_speed_limit = rtb_VariantMergeForOutportExtr_HighSpd_TargetSpeed;
}

/* Function for Chart: '<S69>/Extr_HighSpd_CEMLogic' */
#if EXTR_VARIANT_HIGH_SPEED_PE_ENABLED

static void Extraction_enter_atomic_EVALUATING_FRICTION(void)
{
    sint32 b_previousEvent_p;
    float32 tmp_p;

    /* Entry 'EVALUATING_FRICTION': '<S70>:102' */
    Extraction_DW.Extr_HighSpd_CEM_Enabled.FrictionSum =
        Extr_Inputs_ParamEstData.Friction;
    if (Extr_Inputs_MotorSignals.Speed != 0.0F)
    {
        tmp_p = roundf((float32)((*GESE_Extr_CEMSetup).Friction_Window_Drum_Rev *
                        ((uint16)FS_EXTRACTION)) /
                       (Extr_Inputs_MotorSignals.Speed / 60.0F));
        if (tmp_p < 65536.0F)
        {
            if (tmp_p >= 0.0F)
            {
                Extraction_DW.Extr_HighSpd_CEM_Enabled.FrictionWindowSampleCount
                    = (uint16)tmp_p;
            }
            else
            {
                Extraction_DW.Extr_HighSpd_CEM_Enabled.FrictionWindowSampleCount
                    = 0U;
            }
        }
        else
        {
            Extraction_DW.Extr_HighSpd_CEM_Enabled.FrictionWindowSampleCount =
                MAX_uint16_T;
        }
    }
    else
    {
        /* Event: '<S70>:116' */
        b_previousEvent_p = Extraction_DW.Extr_HighSpd_CEM_Enabled.sfEvent;
        Extraction_DW.Extr_HighSpd_CEM_Enabled.sfEvent = Extraction_event_EvStop;
        Extraction_c21_Extraction();
        Extraction_DW.Extr_HighSpd_CEM_Enabled.sfEvent = b_previousEvent_p;
    }
}

#endif                                 /* EXTR_VARIANT_HIGH_SPEED_PE_ENABLED */

/* Function for Chart: '<S69>/Extr_HighSpd_CEMLogic' */
#if EXTR_VARIANT_HIGH_SPEED_PE_ENABLED

static float32 Extraction_CalculateFrictionBaseline(float32 targSpeed_p, float32
    prevSpeed_p, float32 currBaseline_p, float32 prevBaseline_p, float32
    currentSpeed_p, uint8 spinPulse_CEM_Plateau_Threshold_p, uint8
    prevSpinPulse_CEM_Plateau_Threshold_p)
{
    float32 threshold_p;
    float32 prevTh_p;
    float32 den_p;

    /* MATLAB Function 'CalculateFrictionBaseline': '<S70>:137' */
    /* '<S70>:137:3' */
    threshold_p = currBaseline_p * ((float32)spinPulse_CEM_Plateau_Threshold_p /
        100.0F + 1.0F);

    /* '<S70>:137:4' */
    prevTh_p = prevBaseline_p * ((float32)prevSpinPulse_CEM_Plateau_Threshold_p /
        100.0F + 1.0F);
    if (prevSpeed_p != 0.0F)
    {
        /* '<S70>:137:6' */
        /* '<S70>:137:7' */
        /* '<S70>:137:8' */
        den_p = targSpeed_p - prevSpeed_p;

        /*  Verify division by zero */
        if (den_p == 0.0F)
        {
            /* '<S70>:137:10' */
            /* '<S70>:137:11' */
        }
        else
        {
            /* '<S70>:137:13' */
            threshold_p = (threshold_p - prevTh_p) * (currentSpeed_p -
                prevSpeed_p) / den_p + prevTh_p;
        }
    }
    else
    {
        /* '<S70>:137:16' */
    }

    return threshold_p;
}

#endif                                 /* EXTR_VARIANT_HIGH_SPEED_PE_ENABLED */

/* Function for Chart: '<S69>/Extr_HighSpd_CEMLogic' */
#if EXTR_VARIANT_HIGH_SPEED_PE_ENABLED

static float32 Extraction_CalcAverageFriction(const float32 vec_data_p[10],
    uint8 vec_length_p)
{
    float32 vec_avg_p;
    float32 vec_sum_p;
    uint8 ii_p;

    /* MATLAB Function 'CalcAverageFriction': '<S70>:124' */
    /* '<S70>:124:3' */
    vec_sum_p = 0.0F;

    /*  Verify division by zero */
    if (vec_length_p == 0)
    {
        /* '<S70>:124:6' */
        /* '<S70>:124:7' */
        vec_avg_p = 0.0F;
    }
    else
    {
        /* '<S70>:124:11' */
        for (ii_p = 1U; ii_p <= vec_length_p; ii_p++)
        {
            /* '<S70>:124:11' */
            /* '<S70>:124:12' */
            vec_sum_p += vec_data_p[ii_p - 1];
        }

        /* '<S70>:124:15' */
        vec_avg_p = vec_sum_p / (float32)vec_length_p;
    }

    return vec_avg_p;
}

#endif                                 /* EXTR_VARIANT_HIGH_SPEED_PE_ENABLED */

/* Function for Chart: '<S69>/Extr_HighSpd_CEMLogic' */
#if EXTR_VARIANT_HIGH_SPEED_PE_ENABLED

static void Extraction_c21_Extraction(void)
{
    uint8 ii_p;
    sint32 b_previousEvent_p;

    /* During: Extraction/Extr_HighSpd/Extr_HighSpd_Algorithms/Extr_HighSpd_CEM/Extr_HighSpd_CEM_Enabled/Extr_HighSpd_CEMLogic */
    if (Extraction_DW.Extr_HighSpd_CEM_Enabled.is_active_c21_Extraction == 0U)
    {
        /* Entry: Extraction/Extr_HighSpd/Extr_HighSpd_Algorithms/Extr_HighSpd_CEM/Extr_HighSpd_CEM_Enabled/Extr_HighSpd_CEMLogic */
        Extraction_DW.Extr_HighSpd_CEM_Enabled.is_active_c21_Extraction = 1U;

        /* Entry Internal: Extraction/Extr_HighSpd/Extr_HighSpd_Algorithms/Extr_HighSpd_CEM/Extr_HighSpd_CEM_Enabled/Extr_HighSpd_CEMLogic */
        /* Transition: '<S70>:4' */
        Extraction_DW.Extr_HighSpd_CEM_Enabled.is_c21_Extraction =
            Extraction_IN_IDLE_kt;
        Extr_HighSpd_CEMState = CEM_STATE_IDLE;
    }
    else
    {
        switch (Extraction_DW.Extr_HighSpd_CEM_Enabled.is_c21_Extraction)
        {
          case Extraction_IN_IDLE_kt:
            /* During 'IDLE': '<S70>:3' */
            if (Extraction_B.Extr_HighSpd_EnableCEM)
            {
                /* Transition: '<S70>:6' */
                Extraction_DW.Extr_HighSpd_CEM_Enabled.is_c21_Extraction =
                    Extraction_IN_RUNNING_pc;
                Extr_HighSpd_CEMState = CEM_STATE_RUNNING;

                /* Entry 'RUNNING': '<S70>:5' */
                for (ii_p = 1U; ii_p <= (*GESE_Extr_CEMSetup).
                        Friction_Avg_Samples; ii_p++)
                {
                    Extraction_DW.Extr_HighSpd_CEM_Enabled.FrictionWindowAvgBuf[ii_p
                        - 1] = 0.0F;
                }

                Extraction_DW.Extr_HighSpd_CEM_Enabled.FrictionSum = 0.0F;
                Extraction_DW.Extr_HighSpd_CEM_Enabled.BufferIndex = 1U;

                /* Entry Internal 'RUNNING': '<S70>:5' */
                Extraction_DW.Extr_HighSpd_CEM_Enabled.is_active_MONITORING = 1U;

                /* Entry Internal 'MONITORING': '<S70>:108' */
                /* Transition: '<S70>:112' */
                if (Extraction_DW.Extr_HighSpd_CEM_Enabled.is_MONITORING ==
                        Extraction_IN_FILLING_FRICTION_BUFFER)
                {
                }
                else
                {
                    Extraction_DW.Extr_HighSpd_CEM_Enabled.is_MONITORING =
                        Extraction_IN_FILLING_FRICTION_BUFFER;
                    Extraction_DW.Extr_HighSpd_CEM_Enabled.temporalCounter_i2 =
                        0U;
                }

                Extraction_DW.Extr_HighSpd_CEM_Enabled.is_active_CALCULATING_FRICTION
                    = 1U;

                /* Entry Internal 'CALCULATING_FRICTION': '<S70>:101' */
                /* Transition: '<S70>:105' */
                if (Extraction_DW.Extr_HighSpd_CEM_Enabled.is_CALCULATING_FRICTION
                    == Extraction_IN_EVALUATING_FRICTION)
                {
                }
                else
                {
                    Extraction_DW.Extr_HighSpd_CEM_Enabled.is_CALCULATING_FRICTION
                        = Extraction_IN_EVALUATING_FRICTION;
                    Extraction_DW.Extr_HighSpd_CEM_Enabled.temporalCounter_i1 =
                        0U;
                    Extraction_enter_atomic_EVALUATING_FRICTION();
                }
            }
            break;

          case Extraction_IN_RUNNING_pc:
            /* During 'RUNNING': '<S70>:5' */
            if (!Extraction_B.Extr_HighSpd_EnableCEM ||
                    (Extraction_DW.Extr_HighSpd_CEM_Enabled.sfEvent ==
                     Extraction_event_Extr_HighSpd_EvStopAll ||
                     Extraction_DW.Extr_HighSpd_CEM_Enabled.sfEvent ==
                     Extraction_event_EvStop))
            {
                /* Transition: '<S70>:67' */
                /* Transition: '<S70>:95' */
                /* Exit Internal 'RUNNING': '<S70>:5' */
                /* Exit Internal 'CALCULATING_FRICTION': '<S70>:101' */
                if (Extraction_DW.Extr_HighSpd_CEM_Enabled.is_CALCULATING_FRICTION
                    != Extraction_IN_EVALUATING_FRICTION)
                {
                }
                else
                {
                    /* Exit 'EVALUATING_FRICTION': '<S70>:102' */
                    Extraction_DW.Extr_HighSpd_CEM_Enabled.FrictionWindowAvgBuf[Extraction_DW.Extr_HighSpd_CEM_Enabled.BufferIndex
                        - 1] =
                        Extraction_DW.Extr_HighSpd_CEM_Enabled.FrictionSum /
                        (float32)
                        Extraction_DW.Extr_HighSpd_CEM_Enabled.FrictionWindowSampleCount;
                    b_previousEvent_p = (sint32)
                        (Extraction_DW.Extr_HighSpd_CEM_Enabled.BufferIndex + 1U);
                    if ((uint32)b_previousEvent_p > 255U)
                    {
                        b_previousEvent_p = 255;
                    }

                    Extraction_DW.Extr_HighSpd_CEM_Enabled.BufferIndex = (uint8)
                        b_previousEvent_p;
                    if (Extraction_DW.Extr_HighSpd_CEM_Enabled.BufferIndex >
                            (*GESE_Extr_CEMSetup).Friction_Avg_Samples)
                    {
                        Extraction_DW.Extr_HighSpd_CEM_Enabled.BufferIndex = 1U;
                    }

                    Extraction_DW.Extr_HighSpd_CEM_Enabled.is_CALCULATING_FRICTION
                        = Extraction_IN_NO_ACTIVE_CHILD_iq;
                }

                Extraction_DW.Extr_HighSpd_CEM_Enabled.is_active_CALCULATING_FRICTION
                    = 0U;

                /* Exit Internal 'MONITORING': '<S70>:108' */
                Extraction_DW.Extr_HighSpd_CEM_Enabled.is_MONITORING =
                    Extraction_IN_NO_ACTIVE_CHILD_iq;
                Extraction_DW.Extr_HighSpd_CEM_Enabled.is_active_MONITORING = 0U;
                Extraction_DW.Extr_HighSpd_CEM_Enabled.is_c21_Extraction =
                    Extraction_IN_IDLE_kt;
                Extr_HighSpd_CEMState = CEM_STATE_IDLE;
            }
            else
            {
                if (Extraction_DW.Extr_HighSpd_CEM_Enabled.is_active_MONITORING ==
                    0U)
                {
                }
                else
                {
                    /* During 'MONITORING': '<S70>:108' */
                    switch (Extraction_DW.Extr_HighSpd_CEM_Enabled.is_MONITORING)
                    {
                      case Extraction_IN_FILLING_FRICTION_BUFFER:
                        /* During 'FILLING_FRICTION_BUFFER': '<S70>:111' */
                        if (Extraction_DW.Extr_HighSpd_CEM_Enabled.sfEvent ==
                                Extraction_event_EvWindowComplete &&
                                Extraction_DW.Extr_HighSpd_CEM_Enabled.temporalCounter_i2
                            >= (*GESE_Extr_CEMSetup).Friction_Avg_Samples)
                        {
                            /* Transition: '<S70>:114' */
                            Extraction_DW.Extr_HighSpd_CEM_Enabled.is_MONITORING
                                = Extraction_IN_MONITORING_AVG_FRICTION;

                            /* Entry 'MONITORING_AVG_FRICTION': '<S70>:113' */
                            Extr_HighSpd_CEMFrictionThreshold =
                                Extraction_CalculateFrictionBaseline((float32)
                                Extraction_B.Extr_HighSpd_SpinPulse.Target_Speed_Div10
                                * 10.0F, (float32)
                                Extraction_B.OutportBufferForExtr_HighSpd_PrevSpinPulse.Target_Speed_Div10
                                * 10.0F, Extr_HighSpd_BaselineFriction,
                                Extr_HighSpd_PrevBaselineFriction,
                                Extr_Inputs_MotorSignals.Speed,
                                Extraction_B.Extr_HighSpd_SpinPulse.CEM_Plateau_Threshold,
                                Extraction_B.OutportBufferForExtr_HighSpd_PrevSpinPulse.CEM_Plateau_Threshold);
                            Extr_HighSpd_CEMAvgFrictionDelta =
                                Extraction_CalcAverageFriction
                                (Extraction_DW.Extr_HighSpd_CEM_Enabled.FrictionWindowAvgBuf,
                                 (*GESE_Extr_CEMSetup).Friction_Avg_Samples) -
                                Extr_HighSpd_ReferenceFriction;
                            if (!((*GESE_Extr_SpinProfileConfig).
                                    Factory_Calibration_Enabled != 0) &&
                                    Extr_HighSpd_CEMAvgFrictionDelta >
                                    Extr_HighSpd_CEMFrictionThreshold)
                            {
                                /* Event: '<S70>:128' */
                                Extraction_DW.Extr_HighSpd_CEM_Enabled.Extr_HighSpd_EvCEMFailureDetectedEventCounter
                                    ++;
                            }
                        }
                        break;

                      case Extraction_IN_MONITORING_AVG_FRICTION:
                        /* During 'MONITORING_AVG_FRICTION': '<S70>:113' */
                        if (Extraction_DW.Extr_HighSpd_CEM_Enabled.sfEvent ==
                                Extraction_event_EvWindowComplete)
                        {
                            /* Transition: '<S70>:130' */
                            Extraction_DW.Extr_HighSpd_CEM_Enabled.is_MONITORING
                                = Extraction_IN_MONITORING_AVG_FRICTION;

                            /* Entry 'MONITORING_AVG_FRICTION': '<S70>:113' */
                            Extr_HighSpd_CEMFrictionThreshold =
                                Extraction_CalculateFrictionBaseline((float32)
                                Extraction_B.Extr_HighSpd_SpinPulse.Target_Speed_Div10
                                * 10.0F, (float32)
                                Extraction_B.OutportBufferForExtr_HighSpd_PrevSpinPulse.Target_Speed_Div10
                                * 10.0F, Extr_HighSpd_BaselineFriction,
                                Extr_HighSpd_PrevBaselineFriction,
                                Extr_Inputs_MotorSignals.Speed,
                                Extraction_B.Extr_HighSpd_SpinPulse.CEM_Plateau_Threshold,
                                Extraction_B.OutportBufferForExtr_HighSpd_PrevSpinPulse.CEM_Plateau_Threshold);
                            Extr_HighSpd_CEMAvgFrictionDelta =
                                Extraction_CalcAverageFriction
                                (Extraction_DW.Extr_HighSpd_CEM_Enabled.FrictionWindowAvgBuf,
                                 (*GESE_Extr_CEMSetup).Friction_Avg_Samples) -
                                Extr_HighSpd_ReferenceFriction;
                            if (!((*GESE_Extr_SpinProfileConfig).
                                    Factory_Calibration_Enabled != 0) &&
                                    Extr_HighSpd_CEMAvgFrictionDelta >
                                    Extr_HighSpd_CEMFrictionThreshold)
                            {
                                /* Event: '<S70>:128' */
                                Extraction_DW.Extr_HighSpd_CEM_Enabled.Extr_HighSpd_EvCEMFailureDetectedEventCounter
                                    ++;
                            }
                        }
                        break;
                    }
                }

                if (Extraction_DW.Extr_HighSpd_CEM_Enabled.is_active_CALCULATING_FRICTION
                    == 0U ||
                        Extraction_DW.Extr_HighSpd_CEM_Enabled.is_CALCULATING_FRICTION
                    != Extraction_IN_EVALUATING_FRICTION)
                {
                }
                else
                {
                    /* During 'CALCULATING_FRICTION': '<S70>:101' */
                    /* During 'EVALUATING_FRICTION': '<S70>:102' */
                    if (Extraction_DW.Extr_HighSpd_CEM_Enabled.sfEvent ==
                            Extraction_event_ticks_nt &&
                            Extraction_DW.Extr_HighSpd_CEM_Enabled.temporalCounter_i1
                        >= Extraction_DW.Extr_HighSpd_CEM_Enabled.FrictionWindowSampleCount)
                    {
                        /* Transition: '<S70>:99' */
                        if (Extraction_DW.Extr_HighSpd_CEM_Enabled.is_CALCULATING_FRICTION
                            != Extraction_IN_EVALUATING_FRICTION)
                        {
                        }
                        else
                        {
                            /* Exit 'EVALUATING_FRICTION': '<S70>:102' */
                            Extraction_DW.Extr_HighSpd_CEM_Enabled.FrictionWindowAvgBuf
                                [Extraction_DW.Extr_HighSpd_CEM_Enabled.BufferIndex
                                - 1] =
                                Extraction_DW.Extr_HighSpd_CEM_Enabled.FrictionSum
                                / (float32)
                                Extraction_DW.Extr_HighSpd_CEM_Enabled.FrictionWindowSampleCount;
                            b_previousEvent_p = (sint32)
                                (Extraction_DW.Extr_HighSpd_CEM_Enabled.BufferIndex
                                 + 1U);
                            if ((uint32)b_previousEvent_p > 255U)
                            {
                                b_previousEvent_p = 255;
                            }

                            Extraction_DW.Extr_HighSpd_CEM_Enabled.BufferIndex =
                                (uint8)b_previousEvent_p;
                            if (Extraction_DW.Extr_HighSpd_CEM_Enabled.BufferIndex
                                > (*GESE_Extr_CEMSetup).Friction_Avg_Samples)
                            {
                                Extraction_DW.Extr_HighSpd_CEM_Enabled.BufferIndex
                                    = 1U;
                            }

                            Extraction_DW.Extr_HighSpd_CEM_Enabled.is_CALCULATING_FRICTION
                                = Extraction_IN_NO_ACTIVE_CHILD_iq;
                        }

                        /* Event: '<S70>:115' */
                        b_previousEvent_p =
                            Extraction_DW.Extr_HighSpd_CEM_Enabled.sfEvent;
                        Extraction_DW.Extr_HighSpd_CEM_Enabled.sfEvent =
                            Extraction_event_EvWindowComplete;
                        if (Extraction_DW.Extr_HighSpd_CEM_Enabled.temporalCounter_i2
                            < MAX_uint32_T)
                        {
                            Extraction_DW.Extr_HighSpd_CEM_Enabled.temporalCounter_i2
                                ++;
                        }

                        Extraction_c21_Extraction();
                        Extraction_DW.Extr_HighSpd_CEM_Enabled.sfEvent =
                            b_previousEvent_p;
                        if (Extraction_DW.Extr_HighSpd_CEM_Enabled.is_CALCULATING_FRICTION
                            == Extraction_IN_NO_ACTIVE_CHILD_iq &&
                                !(Extraction_DW.Extr_HighSpd_CEM_Enabled.is_active_CALCULATING_FRICTION
                                  == 0U))
                        {
                            Extraction_DW.Extr_HighSpd_CEM_Enabled.is_CALCULATING_FRICTION
                                = Extraction_IN_EVALUATING_FRICTION;
                            Extraction_DW.Extr_HighSpd_CEM_Enabled.temporalCounter_i1
                                = 0U;
                            Extraction_enter_atomic_EVALUATING_FRICTION();
                        }
                    }
                    else
                    {
                        Extraction_DW.Extr_HighSpd_CEM_Enabled.FrictionSum +=
                            Extr_Inputs_ParamEstData.Friction;
                    }
                }
            }
            break;
        }
    }
}

#endif                                 /* EXTR_VARIANT_HIGH_SPEED_PE_ENABLED */

/* Function for Chart: '<S69>/Extr_HighSpd_CEMLogic' */
#if EXTR_VARIANT_HIGH_SPEED_PE_ENABLED

static void Extraction_chartstep_c21_Extraction(void)
{
    Extraction_c21_Extraction();
}

#endif                                 /* EXTR_VARIANT_HIGH_SPEED_PE_ENABLED */

/* Function for Chart: '<S72>/Extr_HighSpd_LoadMassLimitCheckLogic' */
#if EXTR_VARIANT_HIGH_SPEED_PE_ENABLED

static float32 Extraction_CalculateLoadMass(float32 J_b, float32 coeffs_A_b,
    float32 coeffs_B_b, float32 coeffs_C_b)
{
    float32 mass_b;

    /* MATLAB Function 'CalculateLoadMass': '<S73>:26' */
    /*  Converts the ienrtia measurement to mass in Kg. */
    /* '<S73>:26:4' */
    mass_b = coeffs_A_b * (J_b * J_b) + coeffs_B_b * J_b + coeffs_C_b;
    if (mass_b < 0.0F)
    {
        /* '<S73>:26:6' */
        /* '<S73>:26:7' */
        mass_b = 0.0F;
    }

    return mass_b;
}

#endif                                 /* EXTR_VARIANT_HIGH_SPEED_PE_ENABLED */

/* Function for Chart: '<S72>/Extr_HighSpd_LoadMassLimitCheckLogic' */
#if EXTR_VARIANT_HIGH_SPEED_PE_ENABLED

static float32 Extraction_CalculateLoadMassLimit(float32 currThreshold_b,
    float32 prevThreshold_b, float32 currentSpeed_b, float32 targSpeed_b,
    float32 prevSpeed_b)
{
    float32 massLimit_b;
    float32 den_b;

    /* MATLAB Function 'CalculateLoadMassLimit': '<S73>:11' */
    if (prevSpeed_b != 0.0F)
    {
        /* '<S73>:11:3' */
        /* '<S73>:11:4' */
        /* '<S73>:11:5' */
        den_b = targSpeed_b - prevSpeed_b;

        /*  Verify division by zero */
        if (den_b == 0.0F)
        {
            /* '<S73>:11:7' */
            /* '<S73>:11:8' */
            massLimit_b = currThreshold_b;
        }
        else
        {
            /* '<S73>:11:10' */
            massLimit_b = (currThreshold_b - prevThreshold_b) * (currentSpeed_b
                - prevSpeed_b) / den_b + prevThreshold_b;
        }
    }
    else
    {
        /* '<S73>:11:13' */
        massLimit_b = currThreshold_b;
    }

    return massLimit_b;
}

#endif                                 /* EXTR_VARIANT_HIGH_SPEED_PE_ENABLED */

/* Function for Chart: '<S72>/Extr_HighSpd_LoadMassLimitCheckLogic' */
#if EXTR_VARIANT_HIGH_SPEED_PE_ENABLED

static void Extraction_chartstep_c36_Extraction(const sint32 *sfEvent_b)
{
    uint8 is_active_c36_Extraction_b;
    uint8 is_c36_Extraction_b;
    uint32 temporalCounter_i1_b;
    float32 Extr_HighSpd_LoadMassLimit_b;
    uint32 Extr_HighSpd_EvLoadMassExceededEventCounter_b;
    Extr_HighSpd_EvLoadMassExceededEventCounter_b =
        Extraction_DW.Extr_HighSpd_LoadMassLimitCheck_Enabled.Extr_HighSpd_EvLoadMassExceededEventCounter;
    Extr_HighSpd_LoadMassLimit_b = Extr_HighSpd_LoadMassLimit;
    temporalCounter_i1_b =
        Extraction_DW.Extr_HighSpd_LoadMassLimitCheck_Enabled.temporalCounter_i1;
    is_c36_Extraction_b =
        Extraction_DW.Extr_HighSpd_LoadMassLimitCheck_Enabled.is_c36_Extraction;
    is_active_c36_Extraction_b =
        Extraction_DW.Extr_HighSpd_LoadMassLimitCheck_Enabled.is_active_c36_Extraction;

    /* During: Extraction/Extr_HighSpd/Extr_HighSpd_Algorithms/Extr_HighSpd_LoadMassLimitCheck/Extr_HighSpd_LoadMassLimitCheck_Enabled/Extr_HighSpd_LoadMassLimitCheckLogic */
    if (is_active_c36_Extraction_b == 0U)
    {
        /* Entry: Extraction/Extr_HighSpd/Extr_HighSpd_Algorithms/Extr_HighSpd_LoadMassLimitCheck/Extr_HighSpd_LoadMassLimitCheck_Enabled/Extr_HighSpd_LoadMassLimitCheckLogic */
        is_active_c36_Extraction_b = 1U;

        /* Entry Internal: Extraction/Extr_HighSpd/Extr_HighSpd_Algorithms/Extr_HighSpd_LoadMassLimitCheck/Extr_HighSpd_LoadMassLimitCheck_Enabled/Extr_HighSpd_LoadMassLimitCheckLogic */
        /* Transition: '<S73>:13' */
        is_c36_Extraction_b = Extraction_IN_IDLE_kp;
    }
    else if (is_c36_Extraction_b == Extraction_IN_IDLE_kp)
    {
        /* During 'IDLE': '<S73>:12' */
        if (Extraction_B.UnitDelay)
        {
            /* Transition: '<S73>:15' */
            is_c36_Extraction_b = Extraction_IN_RUNNING_fk;
            temporalCounter_i1_b = 0U;

            /* Entry 'RUNNING': '<S73>:14' */
            Extraction_B.VariantMergeForOutportExtr_HighSpd_LoadMass =
                Extraction_CalculateLoadMass(Extr_Inputs_ParamEstData.Inertia, (*
                GESE_Extr_MassConvCoeffs).A, (*GESE_Extr_MassConvCoeffs).B,
                (*GESE_Extr_MassConvCoeffs).C);
            if ((*GESE_Extr_SpinProfileConfig).Load_Mass_Limit_Check_Enabled !=
                    0)
            {
                Extr_HighSpd_LoadMassLimit_b = Extraction_CalculateLoadMassLimit
                    ((float32)
                     Extraction_B.OutportBufferForExtr_HighSpd_LoadMassLimitConfig.Threshold_x10
                     / 10.0F, (float32)
                     Extraction_B.OutportBufferForExtr_HighSpd_PrevLoadMassLimitConfig.Threshold_x10
                     / 10.0F, Extr_Inputs_MotorSignals.Speed, (float32)
                     Extraction_B.Extr_HighSpd_SpinPulse.Target_Speed_Div10 *
                     10.0F, (float32)
                     Extraction_B.OutportBufferForExtr_HighSpd_PrevSpinPulse.Target_Speed_Div10
                     * 10.0F);
            }
        }
    }
    else
    {
        /* During 'RUNNING': '<S73>:14' */
        if ((*GESE_Extr_SpinProfileConfig).Load_Mass_Limit_Check_Enabled != 0 &&
            Extraction_B.VariantMergeForOutportExtr_HighSpd_LoadMass >
                Extr_HighSpd_LoadMassLimit_b)
        {
            /* Transition: '<S73>:51' */
            /* Event: '<S73>:52' */
            Extr_HighSpd_EvLoadMassExceededEventCounter_b++;
            is_c36_Extraction_b = Extraction_IN_IDLE_kp;
        }
        else if (*sfEvent_b == Extraction_event_Extr_HighSpd_EvStopAll_e4 ||
                 !Extraction_B.UnitDelay)
        {
            /* Transition: '<S73>:54' */
            /* Transition: '<S73>:16' */
            is_c36_Extraction_b = Extraction_IN_IDLE_kp;
        }
        else
        {
            if (*sfEvent_b == Extraction_event_Trigger_cm &&
                    temporalCounter_i1_b >= (uint32)((float32)(((uint16)
                    FS_EXTRACTION) *
                    Extraction_B.OutportBufferForExtr_HighSpd_LoadMassLimitConfig.Delta_Time_x10)
                 / 10.0F))
            {
                /* Transition: '<S73>:45' */
                is_c36_Extraction_b = Extraction_IN_RUNNING_fk;
                temporalCounter_i1_b = 0U;

                /* Entry 'RUNNING': '<S73>:14' */
                Extraction_B.VariantMergeForOutportExtr_HighSpd_LoadMass =
                    Extraction_CalculateLoadMass
                    (Extr_Inputs_ParamEstData.Inertia,
                     (*GESE_Extr_MassConvCoeffs).A, (*GESE_Extr_MassConvCoeffs).
                     B, (*GESE_Extr_MassConvCoeffs).C);
                if ((*GESE_Extr_SpinProfileConfig).Load_Mass_Limit_Check_Enabled
                    != 0)
                {
                    Extr_HighSpd_LoadMassLimit_b =
                        Extraction_CalculateLoadMassLimit((float32)
                        Extraction_B.OutportBufferForExtr_HighSpd_LoadMassLimitConfig.Threshold_x10
                        / 10.0F, (float32)
                        Extraction_B.OutportBufferForExtr_HighSpd_PrevLoadMassLimitConfig.Threshold_x10
                        / 10.0F, Extr_Inputs_MotorSignals.Speed, (float32)
                        Extraction_B.Extr_HighSpd_SpinPulse.Target_Speed_Div10 *
                        10.0F, (float32)
                        Extraction_B.OutportBufferForExtr_HighSpd_PrevSpinPulse.Target_Speed_Div10
                        * 10.0F);
                }
            }
        }
    }

    Extraction_DW.Extr_HighSpd_LoadMassLimitCheck_Enabled.is_active_c36_Extraction
        = is_active_c36_Extraction_b;
    Extraction_DW.Extr_HighSpd_LoadMassLimitCheck_Enabled.is_c36_Extraction =
        is_c36_Extraction_b;
    Extraction_DW.Extr_HighSpd_LoadMassLimitCheck_Enabled.temporalCounter_i1 =
        temporalCounter_i1_b;
    Extr_HighSpd_LoadMassLimit = Extr_HighSpd_LoadMassLimit_b;
    Extraction_DW.Extr_HighSpd_LoadMassLimitCheck_Enabled.Extr_HighSpd_EvLoadMassExceededEventCounter
        = Extr_HighSpd_EvLoadMassExceededEventCounter_b;
}

#endif                                 /* EXTR_VARIANT_HIGH_SPEED_PE_ENABLED */

/* Function for Chart: '<S74>/Extr_HighSpd_PlateauLogic' */
#if EXTR_VARIANT_HIGH_SPEED_PE_DISABLED

static void Extraction_broadcast_EvStop(void)
{
    sint32 b_previousEvent_j;

    /* Event: '<S76>:60' */
    b_previousEvent_j = Extraction_DW.Extr_HighSpd_PlateauLogic_Disabled.sfEvent;
    Extraction_DW.Extr_HighSpd_PlateauLogic_Disabled.sfEvent =
        Extraction_event_EvStop_bp;
    Extraction_c50_Extraction();
    Extraction_DW.Extr_HighSpd_PlateauLogic_Disabled.sfEvent = b_previousEvent_j;
}

#endif                                 /* EXTR_VARIANT_HIGH_SPEED_PE_DISABLED */

/* Function for Chart: '<S74>/Extr_HighSpd_PlateauLogic' */
#if EXTR_VARIANT_HIGH_SPEED_PE_DISABLED

static void Extraction_c50_Extraction(void)
{
    static const uint8 b_j[8] =
    {
        1U, 0U, 0U, 0U, 0U, 1U, 0U, 0U
    };

    /* During: Extraction/Extr_HighSpd/Extr_HighSpd_Algorithms/Extr_HighSpd_PlateauLogic/Extr_HighSpd_PlateauLogic_Disabled/Extr_HighSpd_PlateauLogic */
    if (Extraction_DW.Extr_HighSpd_PlateauLogic_Disabled.is_active_c50_Extraction
        == 0U)
    {
        /* Entry: Extraction/Extr_HighSpd/Extr_HighSpd_Algorithms/Extr_HighSpd_PlateauLogic/Extr_HighSpd_PlateauLogic_Disabled/Extr_HighSpd_PlateauLogic */
        Extraction_DW.Extr_HighSpd_PlateauLogic_Disabled.is_active_c50_Extraction
            = 1U;

        /* Entry Internal: Extraction/Extr_HighSpd/Extr_HighSpd_Algorithms/Extr_HighSpd_PlateauLogic/Extr_HighSpd_PlateauLogic_Disabled/Extr_HighSpd_PlateauLogic */
        /* Transition: '<S76>:4' */
        if (Extraction_DW.Extr_HighSpd_PlateauLogic_Disabled.is_c50_Extraction ==
            Extraction_IN_IDLE_jj)
        {
        }
        else
        {
            Extraction_DW.Extr_HighSpd_PlateauLogic_Disabled.is_c50_Extraction =
                Extraction_IN_IDLE_jj;
            Extr_HighSpd_PlateauState = HIGH_SPEED_PLATEAU_STATE_IDLE;

            /* Entry 'IDLE': '<S76>:3' */
            Extraction_B.VariantMergeForOutportExtr_HighSpd_EnableCEM_j4 = false;
            Extraction_B.VariantMergeForOutportExtr_HighSpd_EnableLoadMassLimitCheck_hb
                = false;
        }
    }
    else
    {
        switch
            (Extraction_DW.Extr_HighSpd_PlateauLogic_Disabled.is_c50_Extraction)
        {
          case Extraction_IN_IDLE_jj:
            /* During 'IDLE': '<S76>:3' */
            if (Extraction_DW.Extr_HighSpd_PlateauLogic_Disabled.sfEvent ==
                    Extraction_event_Extr_HighSpd_EvStartPlateau)
            {
                /* Transition: '<S76>:6' */
                if (Extraction_DW.Extr_HighSpd_PlateauLogic_Disabled.is_c50_Extraction
                    != Extraction_IN_IDLE_jj)
                {
                }
                else
                {
                    /* Exit 'IDLE': '<S76>:3' */
                    Extraction_B.VariantMergeForOutportExtr_HighSpd_EnableCEM_j4
                        = false;
                    Extraction_B.VariantMergeForOutportExtr_HighSpd_EnableLoadMassLimitCheck_hb
                        = false;
                    Extraction_B.Extr_HighSpd_PlateauLogic_Disabled.Extr_HighSpd_PlateauDecision_m0
                        = HIGH_SPEED_PLATEAU_DECISION_NONE;
                    Extraction_SelectParamEstOutputs(b_j);
                    Extraction_DW.Extr_HighSpd_PlateauLogic_Disabled.is_c50_Extraction
                        = Extraction_IN_NO_ACTIVE_CHILD_mr;
                }

                if (Extraction_DW.Extr_HighSpd_PlateauLogic_Disabled.is_c50_Extraction
                    == Extraction_IN_WAITING_PLATEAU_TIME)
                {
                }
                else
                {
                    Extraction_DW.Extr_HighSpd_PlateauLogic_Disabled.is_c50_Extraction
                        = Extraction_IN_WAITING_PLATEAU_TIME;
                    Extraction_DW.Extr_HighSpd_PlateauLogic_Disabled.temporalCounter_i1
                        = 0U;
                    Extr_HighSpd_PlateauState =
                        HIGH_SPEED_PLATEAU_STATE_WAITING_PLATEAU_TIME;

                    /* Entry 'WAITING_PLATEAU_TIME': '<S76>:5' */
                    if (Extr_HighSpd_PlateauDuration == 0.0F)
                    {
                        /* Event: '<S76>:7' */
                        Extraction_DW.Extr_HighSpd_PlateauLogic_Disabled.Extr_HighSpd_EvPlateauCompleteEventCounter
                            ++;
                        Extraction_broadcast_EvStop();
                    }
                }
            }
            break;

          case Extraction_IN_WAITING_PLATEAU_TIME:
            /* During 'WAITING_PLATEAU_TIME': '<S76>:5' */
            if (Extraction_DW.Extr_HighSpd_PlateauLogic_Disabled.sfEvent ==
                    Extraction_event_Extr_HighSpd_EvStopAll_d3 ||
                    Extraction_DW.Extr_HighSpd_PlateauLogic_Disabled.sfEvent ==
                    Extraction_event_EvStop_bp ||
                    Extraction_DW.Extr_HighSpd_PlateauLogic_Disabled.sfEvent ==
                    Extraction_event_Extr_HighSpd_EvLoadMassExceeded_in)
            {
                /* Transition: '<S76>:67' */
                /* Transition: '<S76>:214' */
                if (Extraction_DW.Extr_HighSpd_PlateauLogic_Disabled.is_c50_Extraction
                    != Extraction_IN_WAITING_PLATEAU_TIME)
                {
                }
                else
                {
                    /* Exit 'WAITING_PLATEAU_TIME': '<S76>:5' */
                    Extraction_B.Extr_HighSpd_PlateauLogic_Disabled.Extr_HighSpd_PlateauDecision_m0
                        = HIGH_SPEED_PLATEAU_DECISION_ADVANCE;
                    Extraction_DW.Extr_HighSpd_PlateauLogic_Disabled.is_c50_Extraction
                        = Extraction_IN_NO_ACTIVE_CHILD_mr;
                }

                if (Extraction_DW.Extr_HighSpd_PlateauLogic_Disabled.is_c50_Extraction
                    == Extraction_IN_IDLE_jj)
                {
                }
                else
                {
                    Extraction_DW.Extr_HighSpd_PlateauLogic_Disabled.is_c50_Extraction
                        = Extraction_IN_IDLE_jj;
                    Extr_HighSpd_PlateauState = HIGH_SPEED_PLATEAU_STATE_IDLE;

                    /* Entry 'IDLE': '<S76>:3' */
                    Extraction_B.VariantMergeForOutportExtr_HighSpd_EnableCEM_j4
                        = false;
                    Extraction_B.VariantMergeForOutportExtr_HighSpd_EnableLoadMassLimitCheck_hb
                        = false;
                }
            }
            else
            {
                if (Extraction_B.HighSpdExtUnbCalculated && (float32)
                        Extraction_DW.Extr_HighSpd_PlateauLogic_Disabled.temporalCounter_i1
                    / (float32)((uint16)FS_EXTRACTION) >=
                        Extr_HighSpd_PlateauDuration + (float32)
                        Extraction_B.VariantMergeForOutportExtr_HighSpd_PlateauDurationIncrement)
                {
                    /* Event: '<S76>:7' */
                    Extraction_DW.Extr_HighSpd_PlateauLogic_Disabled.Extr_HighSpd_EvPlateauCompleteEventCounter
                        ++;
                    Extraction_broadcast_EvStop();
                }

                if (Extraction_DW.Extr_HighSpd_PlateauLogic_Disabled.is_c50_Extraction
                    != Extraction_IN_WAITING_PLATEAU_TIME ||
                        !(Extraction_DW.Extr_HighSpd_PlateauLogic_Disabled.sfEvent
                          ==
                          Extraction_event_Extr_Accel_VibMapEvExcessiveFloorDisp_h3))
                {
                }
                else
                {
                    /* Event: '<S76>:7' */
                    Extraction_DW.Extr_HighSpd_PlateauLogic_Disabled.Extr_HighSpd_EvPlateauCompleteEventCounter
                        ++;
                    Extraction_broadcast_EvStop();
                }
            }
            break;
        }
    }
}

#endif                                 /* EXTR_VARIANT_HIGH_SPEED_PE_DISABLED */

/* Function for Chart: '<S74>/Extr_HighSpd_PlateauLogic' */
#if EXTR_VARIANT_HIGH_SPEED_PE_DISABLED

static void Extraction_chartstep_c50_Extraction(void)
{
    Extraction_c50_Extraction();
}

#endif                                 /* EXTR_VARIANT_HIGH_SPEED_PE_DISABLED */

/*
 * Output and update for function-call system:
 *    '<S77>/FRICTION_BASED_PLATEAU.MONITOR.UpdateFactoryCalibrationFriction'
 *    '<S82>/UpdateFactoryCalibrationFriction'
 */
#if EXTR_VARIANT_HIGH_SPEED_PE_ENABLED

void Extraction_FRICTION_BASED_PLATEAUMONITORUpdateFactoryCalibrationFriction
    (float32 rtu_friction, uint8 rtu_baseline_index)
{
    /* Assignment: '<S78>/Assignment' incorporates:
     *  Gain: '<S78>/Gain'
     */
    ExtractionParams__SetFactoryBaselineFriction( rtu_baseline_index , (uint16)
        (1000.0F * rtu_friction));
}

#endif                                 /* EXTR_VARIANT_HIGH_SPEED_PE_ENABLED */

/* Function for Chart: '<S75>/Extr_HighSpd_PlateauLogic' */
#if EXTR_VARIANT_HIGH_SPEED_PE_ENABLED

static void Extraction_broadcast_EvStop_hv(void)
{
    sint32 b_previousEvent_g;

    /* Event: '<S77>:60' */
    b_previousEvent_g = Extraction_DW.Extr_HighSpd_PlateauLogic_Enabled.sfEvent;
    Extraction_DW.Extr_HighSpd_PlateauLogic_Enabled.sfEvent =
        Extraction_event_EvStop_p4;
    Extraction_c3_Extraction();
    Extraction_DW.Extr_HighSpd_PlateauLogic_Enabled.sfEvent = b_previousEvent_g;
}

#endif                                 /* EXTR_VARIANT_HIGH_SPEED_PE_ENABLED */

/* Function for Chart: '<S75>/Extr_HighSpd_PlateauLogic' */
#if EXTR_VARIANT_HIGH_SPEED_PE_ENABLED

static void Extraction_CalcAverageAndRange(const float32 vec_data_g[10], uint8
    vec_length_g, float32 *vec_avg_g, float32 *vec_rng_g)
{
    float32 vec_max_g;
    float32 vec_min_g;
    float32 vec_sum_g;
    uint8 ii_g;

    /* MATLAB Function 'CalcAverageAndRange': '<S77>:269' */
    /* '<S77>:269:3' */
    vec_max_g = -3.402823466E+38F;

    /* '<S77>:269:4' */
    vec_min_g = 3.402823466E+38F;

    /* '<S77>:269:5' */
    vec_sum_g = 0.0F;

    /*  Verify division by zero */
    if (vec_length_g == 0)
    {
        /* '<S77>:269:8' */
        /* '<S77>:269:9' */
        *vec_avg_g = 0.0F;

        /* '<S77>:269:10' */
        *vec_rng_g = 0.0F;
    }
    else
    {
        /* '<S77>:269:14' */
        for (ii_g = 1U; ii_g <= vec_length_g; ii_g++)
        {
            /* '<S77>:269:14' */
            /* '<S77>:269:15' */
            vec_sum_g += vec_data_g[ii_g - 1];
            if (vec_data_g[ii_g - 1] < vec_min_g)
            {
                /* '<S77>:269:16' */
                /* '<S77>:269:17' */
                vec_min_g = vec_data_g[ii_g - 1];
            }

            if (vec_data_g[ii_g - 1] > vec_max_g)
            {
                /* '<S77>:269:19' */
                /* '<S77>:269:20' */
                vec_max_g = vec_data_g[ii_g - 1];
            }
        }

        /* '<S77>:269:24' */
        *vec_avg_g = vec_sum_g / (float32)vec_length_g;

        /* '<S77>:269:25' */
        *vec_rng_g = vec_max_g - vec_min_g;
    }
}

#endif                                 /* EXTR_VARIANT_HIGH_SPEED_PE_ENABLED */

/* Function for Chart: '<S75>/Extr_HighSpd_PlateauLogic' */
#if EXTR_VARIANT_HIGH_SPEED_PE_ENABLED

static void Extraction_enter_atomic_MONITORING_PLATEAU_SETTLING(void)
{
    sint32 tmp_g;

    /* Entry 'MONITORING_PLATEAU_SETTLING': '<S77>:97' */
    tmp_g = (sint32)
        (Extraction_DW.Extr_HighSpd_PlateauLogic_Enabled.FrictionRangeSampleCount
         + 1U);
    if ((uint32)tmp_g > 255U)
    {
        tmp_g = 255;
    }

    Extraction_DW.Extr_HighSpd_PlateauLogic_Enabled.FrictionRangeSampleCount =
        (uint8)tmp_g;
    Extraction_CalcAverageAndRange
        (Extraction_DW.Extr_HighSpd_PlateauLogic_Enabled.Extr_HighSpd_FrictionWindowAvgBuf,
         Extraction_DW.Extr_HighSpd_PlateauLogic_Enabled.FrictionAverageSamples,
         &Extr_HighSpd_FrictionAvg, &Extr_HighSpd_FrictionRng);
    if (Extraction_DW.Extr_HighSpd_PlateauLogic_Enabled.FrictionRangeSampleCount
        > Extraction_B.OutportBufferForExtr_HighSpd_PulseConfig.Plateau_Max_Averages)
    {
        Extraction_B.Extr_HighSpd_PlateauLogic_Enabled.Extr_HighSpd_PlateauDecision_pf
            = HIGH_SPEED_PLATEAU_DECISION_REGRESS;

        /* Event: '<S77>:7' */
        Extraction_DW.Extr_HighSpd_PlateauLogic_Enabled.Extr_HighSpd_EvPlateauCompleteEventCounter
            ++;
        Extraction_broadcast_EvStop_hv();
    }
}

#endif                                 /* EXTR_VARIANT_HIGH_SPEED_PE_ENABLED */

/* Function for Chart: '<S75>/Extr_HighSpd_PlateauLogic' */
#if EXTR_VARIANT_HIGH_SPEED_PE_ENABLED

static HIGH_SPEED_PLATEAU_DECISION Extraction_VerifyPlateauFriction(float32
    friction_avg_g, uint8 pulse_config_Plateau_Advance_Threshold_g, uint8
    pulse_config_Plateau_Monitor_Threshold_g, uint8
    pulse_config_Plateau_Hold_Threshold_g, float32 default_friction_g)
{
    HIGH_SPEED_PLATEAU_DECISION decision_g;

    /* MATLAB Function 'VerifyPlateauFriction': '<S77>:256' */
    if (friction_avg_g <= default_friction_g * (1.0F + (float32)
            pulse_config_Plateau_Advance_Threshold_g / 100.0F))
    {
        /* '<S77>:256:3' */
        /* '<S77>:256:4' */
        decision_g = HIGH_SPEED_PLATEAU_DECISION_ADVANCE;
    }
    else if (friction_avg_g <= default_friction_g * (1.0F + (float32)
              pulse_config_Plateau_Monitor_Threshold_g / 100.0F))
    {
        /* '<S77>:256:6' */
        /* '<S77>:256:7' */
        decision_g = HIGH_SPEED_PLATEAU_DECISION_MONITOR;
    }
    else if (friction_avg_g <= default_friction_g * (1.0F + (float32)
              pulse_config_Plateau_Hold_Threshold_g / 100.0F))
    {
        /* '<S77>:256:9' */
        /* '<S77>:256:10' */
        decision_g = HIGH_SPEED_PLATEAU_DECISION_HOLD;
    }
    else
    {
        /* '<S77>:256:12' */
        decision_g = HIGH_SPEED_PLATEAU_DECISION_REGRESS;
    }

    return decision_g;
}

#endif                                 /* EXTR_VARIANT_HIGH_SPEED_PE_ENABLED */

/* Function for Chart: '<S75>/Extr_HighSpd_PlateauLogic' */
#if EXTR_VARIANT_HIGH_SPEED_PE_ENABLED

static uint16 Extraction_CalcNewFrictionBaseline(float32 delta_friction_g,
    float32 baseline_friction_g, uint16 baseline_config_Friction_Max_x1000_g,
    uint16 baseline_config_Friction_Min_x1000_g, uint8
    baseline_config_Baseline_Update_Threshold_g, uint8
    baseline_config_Percent_Of_Current_Threshold_g)
{
    uint16 new_baseline_x1000_g;
    float32 tmp_g;

    /* MATLAB Function 'CalcNewFrictionBaseline': '<S77>:246' */
    /* '<S77>:246:3' */
    tmp_g = roundf(baseline_friction_g * 1000.0F);
    if (tmp_g < 65536.0F)
    {
        if (tmp_g >= 0.0F)
        {
            new_baseline_x1000_g = (uint16)tmp_g;
        }
        else
        {
            new_baseline_x1000_g = 0U;
        }
    }
    else
    {
        new_baseline_x1000_g = MAX_uint16_T;
    }

    /*  Req 250678 */
    if (delta_friction_g <= baseline_friction_g * (1.0F + (float32)
            baseline_config_Baseline_Update_Threshold_g / 100.0F))
    {
        /* '<S77>:246:6' */
        /* '<S77>:246:7' */
        tmp_g = roundf(((delta_friction_g - baseline_friction_g) * (float32)
                        baseline_config_Percent_Of_Current_Threshold_g / 100.0F
                        + baseline_friction_g) * 1000.0F);
        if (tmp_g < 65536.0F)
        {
            if (tmp_g >= 0.0F)
            {
                new_baseline_x1000_g = (uint16)tmp_g;
            }
            else
            {
                new_baseline_x1000_g = 0U;
            }
        }
        else
        {
            new_baseline_x1000_g = MAX_uint16_T;
        }

        /*  Req 255259 */
        if (new_baseline_x1000_g > baseline_config_Friction_Max_x1000_g)
        {
            /* '<S77>:246:10' */
            /* '<S77>:246:11' */
            new_baseline_x1000_g = baseline_config_Friction_Max_x1000_g;
        }
        else
        {
            /*  Req 255258 */
            if (new_baseline_x1000_g < baseline_config_Friction_Min_x1000_g)
            {
                /* '<S77>:246:14' */
                /* '<S77>:246:15' */
                new_baseline_x1000_g = baseline_config_Friction_Min_x1000_g;
            }
        }
    }

    return new_baseline_x1000_g;
}

#endif                                 /* EXTR_VARIANT_HIGH_SPEED_PE_ENABLED */

/* Function for Chart: '<S75>/Extr_HighSpd_PlateauLogic' */
#if EXTR_VARIANT_HIGH_SPEED_PE_ENABLED

static void Extraction_enter_atomic_EVALUATING_FRICTION_er(void)
{
    float32 tmp_g;

    /* Entry 'EVALUATING_FRICTION': '<S77>:72' */
    Extraction_DW.Extr_HighSpd_PlateauLogic_Enabled.FrictionSum =
        Extr_Inputs_ParamEstData.Friction;
    if (Extr_Inputs_MotorSignals.Speed != 0.0F)
    {
        tmp_g = roundf((float32)
                       (Extraction_DW.Extr_HighSpd_PlateauLogic_Enabled.FrictionDrumRevolutions
                        * ((uint16)FS_EXTRACTION)) /
                       (Extr_Inputs_MotorSignals.Speed / 60.0F));
        if (tmp_g < 65536.0F)
        {
            if (tmp_g >= 0.0F)
            {
                Extraction_DW.Extr_HighSpd_PlateauLogic_Enabled.FrictionWindowSampleCount
                    = (uint16)tmp_g;
            }
            else
            {
                Extraction_DW.Extr_HighSpd_PlateauLogic_Enabled.FrictionWindowSampleCount
                    = 0U;
            }
        }
        else
        {
            Extraction_DW.Extr_HighSpd_PlateauLogic_Enabled.FrictionWindowSampleCount
                = MAX_uint16_T;
        }
    }
    else
    {
        Extraction_broadcast_EvStop_hv();
    }
}

#endif                                 /* EXTR_VARIANT_HIGH_SPEED_PE_ENABLED */

/* Function for Chart: '<S75>/Extr_HighSpd_PlateauLogic' */
#if EXTR_VARIANT_HIGH_SPEED_PE_ENABLED

static void Extraction_exit_internal_FRICTION_BASED_PLATEAU(void)
{
    sint32 tmp_g;
    uint8 is_active_MONITOR_g;
    uint8 is_FRICTION_AVERAGE_g;
    uint8 Extr_HighSpd_FrictionWindowCnt_g;
    uint8 BufferIndex_g;
    uint8 is_MONITOR_g;
    is_MONITOR_g = Extraction_DW.Extr_HighSpd_PlateauLogic_Enabled.is_MONITOR;
    BufferIndex_g = Extraction_DW.Extr_HighSpd_PlateauLogic_Enabled.BufferIndex;
    Extr_HighSpd_FrictionWindowCnt_g =
        Extraction_DW.Extr_HighSpd_PlateauLogic_Enabled.Extr_HighSpd_FrictionWindowCnt;
    is_FRICTION_AVERAGE_g =
        Extraction_DW.Extr_HighSpd_PlateauLogic_Enabled.is_FRICTION_AVERAGE;
    is_active_MONITOR_g =
        Extraction_DW.Extr_HighSpd_PlateauLogic_Enabled.is_active_MONITOR;

    /* Exit Internal 'FRICTION_BASED_PLATEAU': '<S77>:5' */
    /* Exit Internal 'PLATEAU_TIME': '<S77>:86' */
    Extraction_DW.Extr_HighSpd_PlateauLogic_Enabled.is_PLATEAU_TIME =
        Extraction_IN_NO_ACTIVE_CHILD_jm;
    Extraction_DW.Extr_HighSpd_PlateauLogic_Enabled.is_active_PLATEAU_TIME = 0U;

    /* Exit Internal 'FRICTION_AVERAGE': '<S77>:79' */
    if (is_FRICTION_AVERAGE_g == Extraction_IN_EVALUATING_FRICTION_df)
    {
        /* Exit 'EVALUATING_FRICTION': '<S77>:72' */
        tmp_g = (sint32)(Extr_HighSpd_FrictionWindowCnt_g + 1U);
        if ((uint32)tmp_g > 255U)
        {
            tmp_g = 255;
        }

        Extr_HighSpd_FrictionWindowCnt_g = (uint8)tmp_g;
        Extraction_DW.Extr_HighSpd_PlateauLogic_Enabled.Extr_HighSpd_FrictionWindowAvgBuf
            [BufferIndex_g - 1] =
            Extraction_DW.Extr_HighSpd_PlateauLogic_Enabled.FrictionSum /
            (float32)
            Extraction_DW.Extr_HighSpd_PlateauLogic_Enabled.FrictionWindowSampleCount;
        tmp_g = (sint32)(BufferIndex_g + 1U);
        if ((uint32)tmp_g > 255U)
        {
            tmp_g = 255;
        }

        BufferIndex_g = (uint8)tmp_g;
        if ((uint8)tmp_g >
                Extraction_DW.Extr_HighSpd_PlateauLogic_Enabled.FrictionAverageSamples)
        {
            BufferIndex_g = 1U;
        }

        is_FRICTION_AVERAGE_g = Extraction_IN_NO_ACTIVE_CHILD_jm;
    }
    else
    {
        is_FRICTION_AVERAGE_g = Extraction_IN_NO_ACTIVE_CHILD_jm;
    }

    Extraction_DW.Extr_HighSpd_PlateauLogic_Enabled.is_active_FRICTION_AVERAGE =
        0U;

    /* Exit Internal 'MONITOR': '<S77>:78' */
    if (is_MONITOR_g == Extraction_IN_WAITING_PLATEAU_TIME_fx)
    {
        /* Exit 'WAITING_PLATEAU_TIME': '<S77>:149' */
        if (Extraction_B.Extr_HighSpd_SpinPulse.Target_Speed_Div10 ==
                (*GESE_Extr_HighSpdConfig).Reference_Plateau_Velocity_Div10)
        {
            Extr_HighSpd_ReferenceFriction = Extr_HighSpd_FrictionAvg;
        }

        is_MONITOR_g = Extraction_IN_NO_ACTIVE_CHILD_jm;
    }
    else
    {
        is_MONITOR_g = Extraction_IN_NO_ACTIVE_CHILD_jm;
    }

    if (is_active_MONITOR_g == 0U)
    {
    }
    else
    {
        Extr_HighSpd_PlateauState = HIGH_SPEED_PLATEAU_STATE_NONE;

        /* Exit 'MONITOR': '<S77>:78' */
        is_active_MONITOR_g = 0U;
    }

    Extraction_DW.Extr_HighSpd_PlateauLogic_Enabled.is_active_MONITOR =
        is_active_MONITOR_g;
    Extraction_DW.Extr_HighSpd_PlateauLogic_Enabled.is_FRICTION_AVERAGE =
        is_FRICTION_AVERAGE_g;
    Extraction_DW.Extr_HighSpd_PlateauLogic_Enabled.Extr_HighSpd_FrictionWindowCnt
        = Extr_HighSpd_FrictionWindowCnt_g;
    Extraction_DW.Extr_HighSpd_PlateauLogic_Enabled.BufferIndex = BufferIndex_g;
    Extraction_DW.Extr_HighSpd_PlateauLogic_Enabled.is_MONITOR = is_MONITOR_g;
}

#endif                                 /* EXTR_VARIANT_HIGH_SPEED_PE_ENABLED */

/* Function for Chart: '<S75>/Extr_HighSpd_PlateauLogic' */
#if EXTR_VARIANT_HIGH_SPEED_PE_ENABLED

static void Extraction_FRICTION_BASED_PLATEAU(void)
{
    sint32 b_previousEvent_g;
    uint32 tmp_g;

    /* During 'FRICTION_BASED_PLATEAU': '<S77>:5' */
    if (Extraction_DW.Extr_HighSpd_PlateauLogic_Enabled.sfEvent ==
            Extraction_event_Extr_HighSpd_EvStopAll_bu ||
            Extraction_DW.Extr_HighSpd_PlateauLogic_Enabled.sfEvent ==
            Extraction_event_EvStop_p4 ||
            Extraction_DW.Extr_HighSpd_PlateauLogic_Enabled.sfEvent ==
            Extraction_event_Extr_HighSpd_EvLoadMassExceeded_js)
    {
        /* Transition: '<S77>:67' */
        /* Transition: '<S77>:214' */
        Extraction_exit_internal_FRICTION_BASED_PLATEAU();
        Extraction_DW.Extr_HighSpd_PlateauLogic_Enabled.is_c3_Extraction =
            Extraction_IN_IDLE_fw;

        /* Entry 'IDLE': '<S77>:3' */
        Extraction_B.VariantMergeForOutportExtr_HighSpd_EnableCEM_j4 = false;
        Extraction_B.VariantMergeForOutportExtr_HighSpd_EnableLoadMassLimitCheck_hb
            = false;
    }
    else
    {
        if (Extraction_DW.Extr_HighSpd_PlateauLogic_Enabled.sfEvent ==
                Extraction_event_Extr_Accel_VibMapEvExcessiveFloorDisp_af)
        {
            if (Extraction_B.Extr_HighSpd_PlateauLogic_Enabled.Extr_HighSpd_PlateauDecision_pf
                == HIGH_SPEED_PLATEAU_DECISION_ADVANCE)
            {
                /* Event: '<S77>:7' */
                Extraction_DW.Extr_HighSpd_PlateauLogic_Enabled.Extr_HighSpd_EvPlateauCompleteEventCounter
                    ++;
                Extraction_broadcast_EvStop_hv();
            }
            else
            {
                if (Extraction_B.Extr_HighSpd_PlateauLogic_Enabled.Extr_HighSpd_PlateauDecision_pf
                    != HIGH_SPEED_PLATEAU_DECISION_MONITOR)
                {
                    Extraction_B.Extr_HighSpd_PlateauLogic_Enabled.Extr_HighSpd_PlateauDecision_pf
                        = HIGH_SPEED_PLATEAU_DECISION_REGRESS;

                    /* Event: '<S77>:7' */
                    Extraction_DW.Extr_HighSpd_PlateauLogic_Enabled.Extr_HighSpd_EvPlateauCompleteEventCounter
                        ++;
                    Extraction_broadcast_EvStop_hv();
                }
            }
        }

        if (Extraction_DW.Extr_HighSpd_PlateauLogic_Enabled.is_c3_Extraction !=
                Extraction_IN_FRICTION_BASED_PLATEAU)
        {
        }
        else
        {
            if (Extraction_DW.Extr_HighSpd_PlateauLogic_Enabled.is_active_MONITOR
                == 0U)
            {
            }
            else
            {
                /* During 'MONITOR': '<S77>:78' */
                switch
                    (Extraction_DW.Extr_HighSpd_PlateauLogic_Enabled.is_MONITOR)
                {
                  case Extraction_IN_CALCULATING_FRICTION_BASELINE:
                    /* During 'CALCULATING_FRICTION_BASELINE': '<S77>:185' */
                    if (Extraction_DW.Extr_HighSpd_PlateauLogic_Enabled.BaselineFrictionCount
                        >= (*GESE_Extr_BaselineFactoryCalibrationSetup).
                            Plateau_Samples)
                    {
                        /* Transition: '<S77>:191' */
                        Extraction_DW.Extr_HighSpd_PlateauLogic_Enabled.is_MONITOR
                            = Extraction_IN_CALIBRATION_END;
                        Extr_HighSpd_PlateauState =
                            HIGH_SPEED_PLATEAU_STATE_CALIBRATION_END;

                        /* Entry 'CALIBRATION_END': '<S77>:195' */
                        Extr_HighSpd_FrictionAvg =
                            Extraction_DW.Extr_HighSpd_PlateauLogic_Enabled.FactoryCalibrationFrictionSum
                            / (float32)
                            (*GESE_Extr_BaselineFactoryCalibrationSetup).
                            Plateau_Samples;
                        Extr_HighSpd_DeltaFrictionAvg = Extr_HighSpd_FrictionAvg
                            - Extr_HighSpd_ReferenceFriction;
                        if (Extr_HighSpd_DeltaFrictionAvg < 0.0F)
                        {
                            Extr_HighSpd_DeltaFrictionAvg = 0.0F;
                        }

                        /* Simulink Function 'UpdateFactoryCalibrationFriction': '<S77>:264' */
                        Extraction_B.Extr_HighSpd_PlateauLogic_Enabled.friction =
                            Extr_HighSpd_DeltaFrictionAvg;
                        Extraction_B.Extr_HighSpd_PlateauLogic_Enabled.baseline_index
                            = Extr_HighSpd_BaselineFrictionIndex;

                        /* Outputs for Function Call SubSystem: '<S77>/FRICTION_BASED_PLATEAU.MONITOR.UpdateFactoryCalibrationFriction' */
                        Extraction_FRICTION_BASED_PLATEAUMONITORUpdateFactoryCalibrationFriction
                            (Extraction_B.Extr_HighSpd_PlateauLogic_Enabled.friction,
                             Extraction_B.Extr_HighSpd_PlateauLogic_Enabled.baseline_index);

                        /* End of Outputs for SubSystem: '<S77>/FRICTION_BASED_PLATEAU.MONITOR.UpdateFactoryCalibrationFriction' */
                        Extraction_B.Extr_HighSpd_PlateauLogic_Enabled.Extr_HighSpd_PlateauDecision_pf
                            = HIGH_SPEED_PLATEAU_DECISION_ADVANCE;
                        if (Extraction_B.Extr_HighSpd_SpinPulse.Target_Speed_Div10
                            == (*GESE_Extr_HighSpdConfig).
                                Reference_Plateau_Velocity_Div10)
                        {
                            Extr_HighSpd_ReferenceFriction =
                                Extr_HighSpd_FrictionAvg;
                        }

                        /* Event: '<S77>:7' */
                        Extraction_DW.Extr_HighSpd_PlateauLogic_Enabled.Extr_HighSpd_EvPlateauCompleteEventCounter
                            ++;
                        Extraction_broadcast_EvStop_hv();
                    }
                    else
                    {
                        if (Extraction_DW.Extr_HighSpd_PlateauLogic_Enabled.sfEvent
                            == Extraction_event_EvWindowComplete_ld)
                        {
                            /* Transition: '<S77>:189' */
                            Extraction_DW.Extr_HighSpd_PlateauLogic_Enabled.is_MONITOR
                                = Extraction_IN_CALCULATING_FRICTION_BASELINE;
                            Extr_HighSpd_PlateauState =
                                HIGH_SPEED_PLATEAU_STATE_CALCULATING_FRICTION_BASELINE;

                            /* Entry 'CALCULATING_FRICTION_BASELINE': '<S77>:185' */
                            b_previousEvent_g = (sint32)
                                (Extraction_DW.Extr_HighSpd_PlateauLogic_Enabled.BaselineFrictionCount
                                 + 1U);
                            if ((uint32)b_previousEvent_g > 255U)
                            {
                                b_previousEvent_g = 255;
                            }

                            Extraction_DW.Extr_HighSpd_PlateauLogic_Enabled.BaselineFrictionCount
                                = (uint8)b_previousEvent_g;
                            Extraction_CalcAverageAndRange
                                (Extraction_DW.Extr_HighSpd_PlateauLogic_Enabled.Extr_HighSpd_FrictionWindowAvgBuf,
                                 Extraction_DW.Extr_HighSpd_PlateauLogic_Enabled.FrictionAverageSamples,
                                 &Extr_HighSpd_FrictionAvg,
                                 &Extr_HighSpd_FrictionRng);
                            Extraction_DW.Extr_HighSpd_PlateauLogic_Enabled.FactoryCalibrationFrictionSum
                                += Extr_HighSpd_FrictionAvg;
                        }
                    }
                    break;

                  case Extraction_IN_CALIBRATION_END:
                    /* During 'CALIBRATION_END': '<S77>:195' */
                    break;

                  case Extraction_IN_FILLING_FRICTION_BUFFER_io:
                    /* During 'FILLING_FRICTION_BUFFER': '<S77>:69' */
                    if (Extraction_DW.Extr_HighSpd_PlateauLogic_Enabled.sfEvent ==
                        Extraction_event_EvWindowComplete_ld &&
                            Extraction_DW.Extr_HighSpd_PlateauLogic_Enabled.temporalCounter_i4
                        >= Extraction_DW.Extr_HighSpd_PlateauLogic_Enabled.FrictionAverageSamples)
                    {
                        /* Transition: '<S77>:92' */
                        Extraction_DW.Extr_HighSpd_PlateauLogic_Enabled.is_MONITOR
                            = Extraction_IN_MONITORING_PLATEAU_SETTLING;
                        Extr_HighSpd_PlateauState =
                            HIGH_SPEED_PLATEAU_STATE_MONITORING_PLATEAU_SETTLING;
                        Extraction_enter_atomic_MONITORING_PLATEAU_SETTLING();
                    }
                    break;

                  case Extraction_IN_MONITORING_FRICTION:
                    /* During 'MONITORING_FRICTION': '<S77>:85' */
                    if (Extraction_B.Extr_HighSpd_PlateauLogic_Enabled.Extr_HighSpd_PlateauDecision_pf
                        == HIGH_SPEED_PLATEAU_DECISION_ADVANCE)
                    {
                        /* Transition: '<S77>:150' */
                        Extraction_DW.Extr_HighSpd_PlateauLogic_Enabled.is_MONITOR
                            = Extraction_IN_WAITING_PLATEAU_TIME_fx;
                        Extr_HighSpd_PlateauState =
                            HIGH_SPEED_PLATEAU_STATE_WAITING_PLATEAU_TIME;
                    }
                    else
                    {
                        if (Extraction_DW.Extr_HighSpd_PlateauLogic_Enabled.sfEvent
                            == Extraction_event_EvWindowComplete_ld)
                        {
                            /* Transition: '<S77>:90' */
                            Extraction_DW.Extr_HighSpd_PlateauLogic_Enabled.is_MONITOR
                                = Extraction_IN_MONITORING_FRICTION;
                            Extr_HighSpd_PlateauState =
                                HIGH_SPEED_PLATEAU_STATE_MONITORING_FRICTION;

                            /* Entry 'MONITORING_FRICTION': '<S77>:85' */
                            Extraction_CalcAverageAndRange
                                (Extraction_DW.Extr_HighSpd_PlateauLogic_Enabled.Extr_HighSpd_FrictionWindowAvgBuf,
                                 Extraction_DW.Extr_HighSpd_PlateauLogic_Enabled.FrictionAverageSamples,
                                 &Extr_HighSpd_FrictionAvg,
                                 &Extr_HighSpd_FrictionRng);
                            Extr_HighSpd_DeltaFrictionAvg =
                                Extr_HighSpd_FrictionAvg -
                                Extr_HighSpd_ReferenceFriction;
                            Extraction_B.Extr_HighSpd_PlateauLogic_Enabled.Extr_HighSpd_PlateauDecision_pf
                                = Extraction_VerifyPlateauFriction
                                (Extr_HighSpd_DeltaFrictionAvg,
                                 Extraction_B.OutportBufferForExtr_HighSpd_PulseConfig.Plateau_Advance_Threshold,
                                 Extraction_B.OutportBufferForExtr_HighSpd_PulseConfig.Plateau_Monitor_Threshold,
                                 Extraction_B.OutportBufferForExtr_HighSpd_PulseConfig.Plateau_Hold_Threshold,
                                 Extr_HighSpd_BaselineFriction);

                            /*  Update friction baseline */
                            if (Extraction_B.Extr_HighSpd_SpinPulse.Target_Speed_Div10
                                == Extr_HighSpd_BaselineSpeed &&
                                    !Extraction_B.VariantMergeForOutportExtr_HighSpd_RegressDecisionOnPrevPulse)
                            {
                                b_previousEvent_g = (sint32)
                                    (Extr_HighSpd_BaselineFrictionIndex + 1U);
                                if ((uint32)b_previousEvent_g > 255U)
                                {
                                    b_previousEvent_g = 255;
                                }

                                ExtractionParams__SetBaselineFriction
                                    ( b_previousEvent_g - 1 ,
                                     Extraction_CalcNewFrictionBaseline
                                     (Extr_HighSpd_DeltaFrictionAvg,
                                      Extr_HighSpd_BaselineFriction,
                                      Extraction_B.VariantMergeForOutportExtr_HighSpd_BaselineConfig.Friction_Max_x1000,
                                      Extraction_B.VariantMergeForOutportExtr_HighSpd_BaselineConfig.Friction_Min_x1000,
                                      Extraction_B.VariantMergeForOutportExtr_HighSpd_BaselineConfig.Baseline_Update_Threshold,
                                      Extraction_B.VariantMergeForOutportExtr_HighSpd_BaselineConfig.Percent_Of_Current_Threshold));
                            }
                        }
                    }
                    break;

                  case Extraction_IN_MONITORING_PLATEAU_SETTLING:
                    /* During 'MONITORING_PLATEAU_SETTLING': '<S77>:97' */
                    if (Extraction_DW.Extr_HighSpd_PlateauLogic_Enabled.sfEvent ==
                        Extraction_event_EvWindowComplete_ld)
                    {
                        /* Transition: '<S77>:123' */
                        Extraction_DW.Extr_HighSpd_PlateauLogic_Enabled.is_MONITOR
                            = Extraction_IN_MONITORING_PLATEAU_SETTLING;
                        Extr_HighSpd_PlateauState =
                            HIGH_SPEED_PLATEAU_STATE_MONITORING_PLATEAU_SETTLING;
                        Extraction_enter_atomic_MONITORING_PLATEAU_SETTLING();
                    }
                    else
                    {
                        if (Extr_HighSpd_FrictionRng < (float32)
                                Extraction_B.OutportBufferForExtr_HighSpd_PulseConfig.Plateau_Settling_Threshold_x1000
                            / 1000.0F)
                        {
                            /* Transition: '<S77>:98' */
                            if ((*GESE_Extr_SpinProfileConfig).
                                    Factory_Calibration_Enabled != 0)
                            {
                                /* Transition: '<S77>:187' */
                                Extraction_DW.Extr_HighSpd_PlateauLogic_Enabled.is_MONITOR
                                    =
                                    Extraction_IN_CALCULATING_FRICTION_BASELINE;
                                Extr_HighSpd_PlateauState =
                                    HIGH_SPEED_PLATEAU_STATE_CALCULATING_FRICTION_BASELINE;

                                /* Entry 'CALCULATING_FRICTION_BASELINE': '<S77>:185' */
                                b_previousEvent_g = (sint32)
                                    (Extraction_DW.Extr_HighSpd_PlateauLogic_Enabled.BaselineFrictionCount
                                     + 1U);
                                if ((uint32)b_previousEvent_g > 255U)
                                {
                                    b_previousEvent_g = 255;
                                }

                                Extraction_DW.Extr_HighSpd_PlateauLogic_Enabled.BaselineFrictionCount
                                    = (uint8)b_previousEvent_g;
                                Extraction_CalcAverageAndRange
                                    (Extraction_DW.Extr_HighSpd_PlateauLogic_Enabled.Extr_HighSpd_FrictionWindowAvgBuf,
                                     Extraction_DW.Extr_HighSpd_PlateauLogic_Enabled.FrictionAverageSamples,
                                     &Extr_HighSpd_FrictionAvg,
                                     &Extr_HighSpd_FrictionRng);
                                Extraction_DW.Extr_HighSpd_PlateauLogic_Enabled.FactoryCalibrationFrictionSum
                                    += Extr_HighSpd_FrictionAvg;
                            }
                            else
                            {
                                /* Transition: '<S77>:188' */
                                Extraction_DW.Extr_HighSpd_PlateauLogic_Enabled.is_MONITOR
                                    = Extraction_IN_MONITORING_FRICTION;
                                Extr_HighSpd_PlateauState =
                                    HIGH_SPEED_PLATEAU_STATE_MONITORING_FRICTION;

                                /* Entry 'MONITORING_FRICTION': '<S77>:85' */
                                Extraction_CalcAverageAndRange
                                    (Extraction_DW.Extr_HighSpd_PlateauLogic_Enabled.Extr_HighSpd_FrictionWindowAvgBuf,
                                     Extraction_DW.Extr_HighSpd_PlateauLogic_Enabled.FrictionAverageSamples,
                                     &Extr_HighSpd_FrictionAvg,
                                     &Extr_HighSpd_FrictionRng);
                                Extr_HighSpd_DeltaFrictionAvg =
                                    Extr_HighSpd_FrictionAvg -
                                    Extr_HighSpd_ReferenceFriction;
                                Extraction_B.Extr_HighSpd_PlateauLogic_Enabled.Extr_HighSpd_PlateauDecision_pf
                                    = Extraction_VerifyPlateauFriction
                                    (Extr_HighSpd_DeltaFrictionAvg,
                                     Extraction_B.OutportBufferForExtr_HighSpd_PulseConfig.Plateau_Advance_Threshold,
                                     Extraction_B.OutportBufferForExtr_HighSpd_PulseConfig.Plateau_Monitor_Threshold,
                                     Extraction_B.OutportBufferForExtr_HighSpd_PulseConfig.Plateau_Hold_Threshold,
                                     Extr_HighSpd_BaselineFriction);

                                /*  Update friction baseline */
                                if (Extraction_B.Extr_HighSpd_SpinPulse.Target_Speed_Div10
                                    == Extr_HighSpd_BaselineSpeed &&
                                        !Extraction_B.VariantMergeForOutportExtr_HighSpd_RegressDecisionOnPrevPulse)
                                {
                                    b_previousEvent_g = (sint32)
                                        (Extr_HighSpd_BaselineFrictionIndex + 1U);
                                    if ((uint32)b_previousEvent_g > 255U)
                                    {
                                        b_previousEvent_g = 255;
                                    }

                                    ExtractionParams__SetBaselineFriction
                                        ( b_previousEvent_g - 1 ,
                                         Extraction_CalcNewFrictionBaseline
                                         (Extr_HighSpd_DeltaFrictionAvg,
                                          Extr_HighSpd_BaselineFriction,
                                          Extraction_B.VariantMergeForOutportExtr_HighSpd_BaselineConfig.Friction_Max_x1000,
                                          Extraction_B.VariantMergeForOutportExtr_HighSpd_BaselineConfig.Friction_Min_x1000,
                                          Extraction_B.VariantMergeForOutportExtr_HighSpd_BaselineConfig.Baseline_Update_Threshold,
                                          Extraction_B.VariantMergeForOutportExtr_HighSpd_BaselineConfig.Percent_Of_Current_Threshold));
                                }
                            }
                        }
                    }
                    break;

                  case Extraction_IN_WAITING_PE_STABILIZATION_TIME:
                    /* During 'WAITING_PE_STABILIZATION_TIME': '<S77>:107' */
                    tmp_g = (uint32)
                        Extraction_B.OutportBufferForExtr_HighSpd_ParamEstAppSpecificSetup.Stabilization_Time
                        * ((uint16)FS_EXTRACTION);
                    if (tmp_g > 65535U)
                    {
                        tmp_g = 65535U;
                    }

                    if (Extraction_DW.Extr_HighSpd_PlateauLogic_Enabled.sfEvent ==
                        Extraction_event_ticks_pn &&
                            Extraction_DW.Extr_HighSpd_PlateauLogic_Enabled.temporalCounter_i2
                        >= tmp_g)
                    {
                        /* Transition: '<S77>:108' */
                        Extraction_DW.Extr_HighSpd_PlateauLogic_Enabled.is_MONITOR
                            = Extraction_IN_FILLING_FRICTION_BUFFER_io;
                        Extraction_DW.Extr_HighSpd_PlateauLogic_Enabled.temporalCounter_i4
                            = 0U;
                        Extr_HighSpd_PlateauState =
                            HIGH_SPEED_PLATEAU_STATE_FILLING_FRICTION_BUFFER;

                        /* Entry 'FILLING_FRICTION_BUFFER': '<S77>:69' */
                        Extraction_B.VariantMergeForOutportExtr_HighSpd_EnableCEM_j4
                            = true;
                        Extraction_B.VariantMergeForOutportExtr_HighSpd_EnableLoadMassLimitCheck_hb
                            = true;
                    }
                    break;

                  case Extraction_IN_WAITING_PLATEAU_TIME_fx:
                    /* During 'WAITING_PLATEAU_TIME': '<S77>:149' */
                    break;
                }
            }

            if (Extraction_DW.Extr_HighSpd_PlateauLogic_Enabled.is_c3_Extraction
                != Extraction_IN_FRICTION_BASED_PLATEAU)
            {
            }
            else
            {
                if (Extraction_DW.Extr_HighSpd_PlateauLogic_Enabled.is_active_FRICTION_AVERAGE
                    == 0U)
                {
                }
                else
                {
                    /* During 'FRICTION_AVERAGE': '<S77>:79' */
                    switch
                        (Extraction_DW.Extr_HighSpd_PlateauLogic_Enabled.is_FRICTION_AVERAGE)
                    {
                      case Extraction_IN_EVALUATING_FRICTION_df:
                        /* During 'EVALUATING_FRICTION': '<S77>:72' */
                        if (Extraction_DW.Extr_HighSpd_PlateauLogic_Enabled.sfEvent
                            == Extraction_event_ticks_pn &&
                                Extraction_DW.Extr_HighSpd_PlateauLogic_Enabled.temporalCounter_i3
                            >= Extraction_DW.Extr_HighSpd_PlateauLogic_Enabled.FrictionWindowSampleCount)
                        {
                            /* Transition: '<S77>:75' */
                            if (Extraction_DW.Extr_HighSpd_PlateauLogic_Enabled.is_FRICTION_AVERAGE
                                != Extraction_IN_EVALUATING_FRICTION_df)
                            {
                            }
                            else
                            {
                                /* Exit 'EVALUATING_FRICTION': '<S77>:72' */
                                b_previousEvent_g = (sint32)
                                    (Extraction_DW.Extr_HighSpd_PlateauLogic_Enabled.Extr_HighSpd_FrictionWindowCnt
                                     + 1U);
                                if ((uint32)b_previousEvent_g > 255U)
                                {
                                    b_previousEvent_g = 255;
                                }

                                Extraction_DW.Extr_HighSpd_PlateauLogic_Enabled.Extr_HighSpd_FrictionWindowCnt
                                    = (uint8)b_previousEvent_g;
                                Extraction_DW.Extr_HighSpd_PlateauLogic_Enabled.Extr_HighSpd_FrictionWindowAvgBuf
                                    [Extraction_DW.Extr_HighSpd_PlateauLogic_Enabled.BufferIndex
                                    - 1] =
                                    Extraction_DW.Extr_HighSpd_PlateauLogic_Enabled.FrictionSum
                                    / (float32)
                                    Extraction_DW.Extr_HighSpd_PlateauLogic_Enabled.FrictionWindowSampleCount;
                                b_previousEvent_g = (sint32)
                                    (Extraction_DW.Extr_HighSpd_PlateauLogic_Enabled.BufferIndex
                                     + 1U);
                                if ((uint32)b_previousEvent_g > 255U)
                                {
                                    b_previousEvent_g = 255;
                                }

                                Extraction_DW.Extr_HighSpd_PlateauLogic_Enabled.BufferIndex
                                    = (uint8)b_previousEvent_g;
                                if (Extraction_DW.Extr_HighSpd_PlateauLogic_Enabled.BufferIndex
                                    > Extraction_DW.Extr_HighSpd_PlateauLogic_Enabled.FrictionAverageSamples)
                                {
                                    Extraction_DW.Extr_HighSpd_PlateauLogic_Enabled.BufferIndex
                                        = 1U;
                                }

                                Extraction_DW.Extr_HighSpd_PlateauLogic_Enabled.is_FRICTION_AVERAGE
                                    = Extraction_IN_NO_ACTIVE_CHILD_jm;
                            }

                            /* Event: '<S77>:101' */
                            b_previousEvent_g =
                                Extraction_DW.Extr_HighSpd_PlateauLogic_Enabled.sfEvent;
                            Extraction_DW.Extr_HighSpd_PlateauLogic_Enabled.sfEvent
                                = Extraction_event_EvWindowComplete_ld;
                            if (Extraction_DW.Extr_HighSpd_PlateauLogic_Enabled.temporalCounter_i4
                                < 255U)
                            {
                                Extraction_DW.Extr_HighSpd_PlateauLogic_Enabled.temporalCounter_i4
                                    ++;
                            }

                            Extraction_c3_Extraction();
                            Extraction_DW.Extr_HighSpd_PlateauLogic_Enabled.sfEvent
                                = b_previousEvent_g;
                            if (Extraction_DW.Extr_HighSpd_PlateauLogic_Enabled.is_FRICTION_AVERAGE
                                == Extraction_IN_NO_ACTIVE_CHILD_jm &&
                                    !(Extraction_DW.Extr_HighSpd_PlateauLogic_Enabled.is_active_FRICTION_AVERAGE
                                      == 0U))
                            {
                                Extraction_DW.Extr_HighSpd_PlateauLogic_Enabled.is_FRICTION_AVERAGE
                                    = Extraction_IN_EVALUATING_FRICTION_df;
                                Extraction_DW.Extr_HighSpd_PlateauLogic_Enabled.temporalCounter_i3
                                    = 0U;
                                Extraction_enter_atomic_EVALUATING_FRICTION_er();
                            }
                        }
                        else
                        {
                            Extraction_DW.Extr_HighSpd_PlateauLogic_Enabled.FrictionSum
                                += Extr_Inputs_ParamEstData.Friction;
                        }
                        break;

                      case Extraction_IN_WAITING_PE_STABILIZATION_TIME_mr:
                        /* During 'WAITING_PE_STABILIZATION_TIME': '<S77>:124' */
                        tmp_g = (uint32)
                            Extraction_B.OutportBufferForExtr_HighSpd_ParamEstAppSpecificSetup.Stabilization_Time
                            * ((uint16)FS_EXTRACTION);
                        if (tmp_g > 65535U)
                        {
                            tmp_g = 65535U;
                        }

                        if (Extraction_DW.Extr_HighSpd_PlateauLogic_Enabled.sfEvent
                            == Extraction_event_ticks_pn &&
                                Extraction_DW.Extr_HighSpd_PlateauLogic_Enabled.temporalCounter_i3
                            >= tmp_g)
                        {
                            /* Transition: '<S77>:125' */
                            Extraction_DW.Extr_HighSpd_PlateauLogic_Enabled.is_FRICTION_AVERAGE
                                = Extraction_IN_EVALUATING_FRICTION_df;
                            Extraction_DW.Extr_HighSpd_PlateauLogic_Enabled.temporalCounter_i3
                                = 0U;
                            Extraction_enter_atomic_EVALUATING_FRICTION_er();
                        }
                        break;
                    }
                }

                if (Extraction_DW.Extr_HighSpd_PlateauLogic_Enabled.is_c3_Extraction
                    != Extraction_IN_FRICTION_BASED_PLATEAU ||
                        Extraction_DW.Extr_HighSpd_PlateauLogic_Enabled.is_active_PLATEAU_TIME
                    == 0U ||
                        Extraction_DW.Extr_HighSpd_PlateauLogic_Enabled.is_PLATEAU_TIME
                    != Extraction_IN_MONITORING_PLATEAU_TIME ||
                        (*GESE_Extr_SpinProfileConfig).
                        Factory_Calibration_Enabled != 0)
                {
                }
                else
                {
                    /* During 'PLATEAU_TIME': '<S77>:86' */
                    /* During 'MONITORING_PLATEAU_TIME': '<S77>:87' */
                    Extr_HighSpd_PlateauTOnDone = (float32)
                        Extraction_DW.Extr_HighSpd_PlateauLogic_Enabled.temporalCounter_i1
                        / (float32)((uint16)FS_EXTRACTION) >=
                        Extr_HighSpd_PlateauDuration + (float32)
                        Extraction_B.VariantMergeForOutportExtr_HighSpd_PlateauDurationIncrement;
                    if (Extr_HighSpd_PlateauTOnDone &&
                            Extraction_B.HighSpdExtUnbCalculated &&
                            Extraction_DW.Extr_HighSpd_PlateauLogic_Enabled.Extr_HighSpd_FrictionWindowCnt
                        >= Extraction_B.OutportBufferForExtr_HighSpd_PulseConfig.Plateau_Monitor_Samples
                        && Extraction_B.Extr_HighSpd_PlateauLogic_Enabled.Extr_HighSpd_PlateauDecision_pf
                        != HIGH_SPEED_PLATEAU_DECISION_NONE ||
                            Extraction_B.Extr_HighSpd_PlateauLogic_Enabled.Extr_HighSpd_PlateauDecision_pf
                        == HIGH_SPEED_PLATEAU_DECISION_REGRESS ||
                            Extr_HighSpd_PlateauTOnDone &&
                            Extraction_B.Extr_HighSpd_PlateauLogic_Enabled.Extr_HighSpd_PlateauDecision_pf
                        == HIGH_SPEED_PLATEAU_DECISION_ADVANCE)
                    {
                        /* Event: '<S77>:7' */
                        Extraction_DW.Extr_HighSpd_PlateauLogic_Enabled.Extr_HighSpd_EvPlateauCompleteEventCounter
                            ++;
                        Extraction_broadcast_EvStop_hv();
                    }
                }
            }
        }
    }
}

#endif                                 /* EXTR_VARIANT_HIGH_SPEED_PE_ENABLED */

/* Function for Chart: '<S75>/Extr_HighSpd_PlateauLogic' */
#if EXTR_VARIANT_HIGH_SPEED_PE_ENABLED

static void Extraction_c3_Extraction(void)
{
    uint8 ii_g;
    static const uint8 b_g[8] =
    {
        1U, 0U, 0U, 0U, 0U, 1U, 0U, 0U
    };

    /* During: Extraction/Extr_HighSpd/Extr_HighSpd_Algorithms/Extr_HighSpd_PlateauLogic/Extr_HighSpd_PlateauLogic_Enabled/Extr_HighSpd_PlateauLogic */
    if (Extraction_DW.Extr_HighSpd_PlateauLogic_Enabled.is_active_c3_Extraction ==
        0U)
    {
        /* Entry: Extraction/Extr_HighSpd/Extr_HighSpd_Algorithms/Extr_HighSpd_PlateauLogic/Extr_HighSpd_PlateauLogic_Enabled/Extr_HighSpd_PlateauLogic */
        Extraction_DW.Extr_HighSpd_PlateauLogic_Enabled.is_active_c3_Extraction =
            1U;

        /* Entry Internal: Extraction/Extr_HighSpd/Extr_HighSpd_Algorithms/Extr_HighSpd_PlateauLogic/Extr_HighSpd_PlateauLogic_Enabled/Extr_HighSpd_PlateauLogic */
        /* Transition: '<S77>:4' */
        if (Extraction_DW.Extr_HighSpd_PlateauLogic_Enabled.is_c3_Extraction ==
                Extraction_IN_IDLE_fw)
        {
        }
        else
        {
            Extraction_DW.Extr_HighSpd_PlateauLogic_Enabled.is_c3_Extraction =
                Extraction_IN_IDLE_fw;

            /* Entry 'IDLE': '<S77>:3' */
            Extraction_B.VariantMergeForOutportExtr_HighSpd_EnableCEM_j4 = false;
            Extraction_B.VariantMergeForOutportExtr_HighSpd_EnableLoadMassLimitCheck_hb
                = false;
        }
    }
    else
    {
        switch (Extraction_DW.Extr_HighSpd_PlateauLogic_Enabled.is_c3_Extraction)
        {
          case Extraction_IN_BASIC_PLATEAU:
            /* During 'BASIC_PLATEAU': '<S77>:219' */
            if (Extraction_DW.Extr_HighSpd_PlateauLogic_Enabled.sfEvent ==
                    Extraction_event_Extr_HighSpd_EvStopAll_bu ||
                    Extraction_DW.Extr_HighSpd_PlateauLogic_Enabled.sfEvent ==
                    Extraction_event_EvStop_p4)
            {
                /* Transition: '<S77>:228' */
                if (Extraction_DW.Extr_HighSpd_PlateauLogic_Enabled.is_c3_Extraction
                    != Extraction_IN_BASIC_PLATEAU)
                {
                }
                else
                {
                    /* Exit 'BASIC_PLATEAU': '<S77>:219' */
                    Extraction_B.Extr_HighSpd_PlateauLogic_Enabled.Extr_HighSpd_PlateauDecision_pf
                        = HIGH_SPEED_PLATEAU_DECISION_ADVANCE;
                    Extraction_DW.Extr_HighSpd_PlateauLogic_Enabled.is_c3_Extraction
                        = Extraction_IN_NO_ACTIVE_CHILD_jm;
                }

                if (Extraction_DW.Extr_HighSpd_PlateauLogic_Enabled.is_c3_Extraction
                    == Extraction_IN_IDLE_fw)
                {
                }
                else
                {
                    Extraction_DW.Extr_HighSpd_PlateauLogic_Enabled.is_c3_Extraction
                        = Extraction_IN_IDLE_fw;

                    /* Entry 'IDLE': '<S77>:3' */
                    Extraction_B.VariantMergeForOutportExtr_HighSpd_EnableCEM_j4
                        = false;
                    Extraction_B.VariantMergeForOutportExtr_HighSpd_EnableLoadMassLimitCheck_hb
                        = false;
                }
            }
            else
            {
                if ((float32)
                        Extraction_DW.Extr_HighSpd_PlateauLogic_Enabled.temporalCounter_i1
                    / (float32)((uint16)FS_EXTRACTION) >=
                        Extr_HighSpd_PlateauDuration + (float32)
                        Extraction_B.VariantMergeForOutportExtr_HighSpd_PlateauDurationIncrement
                    && Extraction_B.HighSpdExtUnbCalculated)
                {
                    /* Event: '<S77>:7' */
                    Extraction_DW.Extr_HighSpd_PlateauLogic_Enabled.Extr_HighSpd_EvPlateauCompleteEventCounter
                        ++;
                    Extraction_broadcast_EvStop_hv();
                }

                if (Extraction_DW.Extr_HighSpd_PlateauLogic_Enabled.is_c3_Extraction
                    != Extraction_IN_BASIC_PLATEAU ||
                        !(Extraction_DW.Extr_HighSpd_PlateauLogic_Enabled.sfEvent
                          ==
                          Extraction_event_Extr_Accel_VibMapEvExcessiveFloorDisp_af))
                {
                }
                else
                {
                    /* Event: '<S77>:7' */
                    Extraction_DW.Extr_HighSpd_PlateauLogic_Enabled.Extr_HighSpd_EvPlateauCompleteEventCounter
                        ++;
                    Extraction_broadcast_EvStop_hv();
                }
            }
            break;

          case Extraction_IN_FRICTION_BASED_PLATEAU:
            Extraction_FRICTION_BASED_PLATEAU();
            break;

          case Extraction_IN_IDLE_fw:
            /* During 'IDLE': '<S77>:3' */
            if (Extraction_DW.Extr_HighSpd_PlateauLogic_Enabled.sfEvent ==
                    Extraction_event_Extr_HighSpd_EvStartPlateau_i3)
            {
                /* Transition: '<S77>:6' */
                if (Extr_HighSpd_PlateauDuration == 0.0F)
                {
                    /* Transition: '<S77>:251' */
                    /* Transition: '<S77>:248' */
                    /* Event: '<S77>:7' */
                    Extraction_DW.Extr_HighSpd_PlateauLogic_Enabled.Extr_HighSpd_EvPlateauCompleteEventCounter
                        ++;
                    Extraction_B.Extr_HighSpd_PlateauLogic_Enabled.Extr_HighSpd_PlateauDecision_pf
                        = HIGH_SPEED_PLATEAU_DECISION_ADVANCE;
                    if (Extraction_DW.Extr_HighSpd_PlateauLogic_Enabled.is_c3_Extraction
                        != Extraction_IN_IDLE_fw)
                    {
                    }
                    else
                    {
                        /* Exit 'IDLE': '<S77>:3' */
                        Extraction_B.VariantMergeForOutportExtr_HighSpd_EnableCEM_j4
                            = false;
                        Extraction_B.VariantMergeForOutportExtr_HighSpd_EnableLoadMassLimitCheck_hb
                            = false;
                        Extraction_DW.Extr_HighSpd_PlateauLogic_Enabled.Extr_HighSpd_FrictionWindowCnt
                            = 0U;
                        Extraction_DW.Extr_HighSpd_PlateauLogic_Enabled.BaselineFrictionCount
                            = 0U;
                        Extraction_B.Extr_HighSpd_PlateauLogic_Enabled.Extr_HighSpd_PlateauDecision_pf
                            = HIGH_SPEED_PLATEAU_DECISION_NONE;
                        Extraction_DW.Extr_HighSpd_PlateauLogic_Enabled.FrictionSum
                            = 0.0F;
                        Extraction_DW.Extr_HighSpd_PlateauLogic_Enabled.BufferIndex
                            = 1U;
                        Extraction_DW.Extr_HighSpd_PlateauLogic_Enabled.FactoryCalibrationFrictionSum
                            = 0.0F;
                        Extraction_DW.Extr_HighSpd_PlateauLogic_Enabled.is_c3_Extraction
                            = Extraction_IN_NO_ACTIVE_CHILD_jm;
                    }

                    if (Extraction_DW.Extr_HighSpd_PlateauLogic_Enabled.is_c3_Extraction
                        == Extraction_IN_IDLE_fw)
                    {
                    }
                    else
                    {
                        Extraction_DW.Extr_HighSpd_PlateauLogic_Enabled.is_c3_Extraction
                            = Extraction_IN_IDLE_fw;

                        /* Entry 'IDLE': '<S77>:3' */
                        Extraction_B.VariantMergeForOutportExtr_HighSpd_EnableCEM_j4
                            = false;
                        Extraction_B.VariantMergeForOutportExtr_HighSpd_EnableLoadMassLimitCheck_hb
                            = false;
                    }
                }
                else
                {
                    /* Transition: '<S77>:250' */
                    if ((*GESE_Extr_SpinProfileConfig).High_Speed_Enabled != 0)
                    {
                        /* Transition: '<S77>:222' */
                        if (Extraction_DW.Extr_HighSpd_PlateauLogic_Enabled.is_c3_Extraction
                            != Extraction_IN_IDLE_fw)
                        {
                        }
                        else
                        {
                            /* Exit 'IDLE': '<S77>:3' */
                            Extraction_B.VariantMergeForOutportExtr_HighSpd_EnableCEM_j4
                                = false;
                            Extraction_B.VariantMergeForOutportExtr_HighSpd_EnableLoadMassLimitCheck_hb
                                = false;
                            Extraction_DW.Extr_HighSpd_PlateauLogic_Enabled.Extr_HighSpd_FrictionWindowCnt
                                = 0U;
                            Extraction_DW.Extr_HighSpd_PlateauLogic_Enabled.BaselineFrictionCount
                                = 0U;
                            Extraction_B.Extr_HighSpd_PlateauLogic_Enabled.Extr_HighSpd_PlateauDecision_pf
                                = HIGH_SPEED_PLATEAU_DECISION_NONE;
                            Extraction_DW.Extr_HighSpd_PlateauLogic_Enabled.FrictionSum
                                = 0.0F;
                            Extraction_DW.Extr_HighSpd_PlateauLogic_Enabled.BufferIndex
                                = 1U;
                            Extraction_DW.Extr_HighSpd_PlateauLogic_Enabled.FactoryCalibrationFrictionSum
                                = 0.0F;
                            Extraction_DW.Extr_HighSpd_PlateauLogic_Enabled.is_c3_Extraction
                                = Extraction_IN_NO_ACTIVE_CHILD_jm;
                        }

                        if (Extraction_DW.Extr_HighSpd_PlateauLogic_Enabled.is_c3_Extraction
                            == Extraction_IN_FRICTION_BASED_PLATEAU)
                        {
                        }
                        else
                        {
                            Extraction_DW.Extr_HighSpd_PlateauLogic_Enabled.is_c3_Extraction
                                = Extraction_IN_FRICTION_BASED_PLATEAU;

                            /* Entry 'FRICTION_BASED_PLATEAU': '<S77>:5' */
                            Extraction_SelectParamEstOutputs(b_g);
                            for (ii_g = 1U; ii_g <= (*GESE_Extr_HighSpdConfig).
                                    Friction_Average_Samples; ii_g++)
                            {
                                Extraction_DW.Extr_HighSpd_PlateauLogic_Enabled.Extr_HighSpd_FrictionWindowAvgBuf
                                    [ii_g - 1] = 0.0F;
                            }

                            Extraction_DW.Extr_HighSpd_PlateauLogic_Enabled.FrictionRangeSampleCount
                                = 0U;
                        }

                        /* Entry Internal 'FRICTION_BASED_PLATEAU': '<S77>:5' */
                        if (Extraction_DW.Extr_HighSpd_PlateauLogic_Enabled.is_active_MONITOR
                            == 1U)
                        {
                        }
                        else
                        {
                            Extraction_DW.Extr_HighSpd_PlateauLogic_Enabled.is_active_MONITOR
                                = 1U;

                            /* Entry 'MONITOR': '<S77>:78' */
                            if (!((*GESE_Extr_SpinProfileConfig).
                                    Factory_Calibration_Enabled != 0))
                            {
                                Extraction_DW.Extr_HighSpd_PlateauLogic_Enabled.FrictionAverageSamples
                                    = (*GESE_Extr_HighSpdConfig).
                                    Friction_Average_Samples;
                                Extraction_DW.Extr_HighSpd_PlateauLogic_Enabled.FrictionDrumRevolutions
                                    = (*GESE_Extr_HighSpdConfig).
                                    Friction_Drum_Revolutions;
                            }
                            else
                            {
                                Extraction_DW.Extr_HighSpd_PlateauLogic_Enabled.FrictionAverageSamples
                                    =
                                    (*GESE_Extr_BaselineFactoryCalibrationSetup)
                                    .Average_Samples;
                                Extraction_DW.Extr_HighSpd_PlateauLogic_Enabled.FrictionDrumRevolutions
                                    =
                                    (*GESE_Extr_BaselineFactoryCalibrationSetup)
                                    .Friction_Drum_Revolutions;
                            }
                        }

                        /* Entry Internal 'MONITOR': '<S77>:78' */
                        /* Transition: '<S77>:71' */
                        if (Extraction_DW.Extr_HighSpd_PlateauLogic_Enabled.is_MONITOR
                            == Extraction_IN_WAITING_PE_STABILIZATION_TIME)
                        {
                        }
                        else
                        {
                            Extraction_DW.Extr_HighSpd_PlateauLogic_Enabled.is_MONITOR
                                = Extraction_IN_WAITING_PE_STABILIZATION_TIME;
                            Extraction_DW.Extr_HighSpd_PlateauLogic_Enabled.temporalCounter_i2
                                = 0U;
                            Extr_HighSpd_PlateauState =
                                HIGH_SPEED_PLATEAU_STATE_WAITING_PE_STABILIZATION_TIME;
                        }

                        Extraction_DW.Extr_HighSpd_PlateauLogic_Enabled.is_active_FRICTION_AVERAGE
                            = 1U;

                        /* Entry Internal 'FRICTION_AVERAGE': '<S77>:79' */
                        /* Transition: '<S77>:80' */
                        if (Extraction_DW.Extr_HighSpd_PlateauLogic_Enabled.is_FRICTION_AVERAGE
                            == Extraction_IN_WAITING_PE_STABILIZATION_TIME_mr)
                        {
                        }
                        else
                        {
                            Extraction_DW.Extr_HighSpd_PlateauLogic_Enabled.is_FRICTION_AVERAGE
                                = Extraction_IN_WAITING_PE_STABILIZATION_TIME_mr;
                            Extraction_DW.Extr_HighSpd_PlateauLogic_Enabled.temporalCounter_i3
                                = 0U;
                        }

                        Extraction_DW.Extr_HighSpd_PlateauLogic_Enabled.is_active_PLATEAU_TIME
                            = 1U;

                        /* Entry Internal 'PLATEAU_TIME': '<S77>:86' */
                        /* Transition: '<S77>:88' */
                        if (Extraction_DW.Extr_HighSpd_PlateauLogic_Enabled.is_PLATEAU_TIME
                            == Extraction_IN_MONITORING_PLATEAU_TIME)
                        {
                        }
                        else
                        {
                            Extraction_DW.Extr_HighSpd_PlateauLogic_Enabled.is_PLATEAU_TIME
                                = Extraction_IN_MONITORING_PLATEAU_TIME;
                            Extraction_DW.Extr_HighSpd_PlateauLogic_Enabled.temporalCounter_i1
                                = 0U;
                        }
                    }
                    else
                    {
                        /* Transition: '<S77>:221' */
                        if (Extraction_DW.Extr_HighSpd_PlateauLogic_Enabled.is_c3_Extraction
                            != Extraction_IN_IDLE_fw)
                        {
                        }
                        else
                        {
                            /* Exit 'IDLE': '<S77>:3' */
                            Extraction_B.VariantMergeForOutportExtr_HighSpd_EnableCEM_j4
                                = false;
                            Extraction_B.VariantMergeForOutportExtr_HighSpd_EnableLoadMassLimitCheck_hb
                                = false;
                            Extraction_DW.Extr_HighSpd_PlateauLogic_Enabled.Extr_HighSpd_FrictionWindowCnt
                                = 0U;
                            Extraction_DW.Extr_HighSpd_PlateauLogic_Enabled.BaselineFrictionCount
                                = 0U;
                            Extraction_B.Extr_HighSpd_PlateauLogic_Enabled.Extr_HighSpd_PlateauDecision_pf
                                = HIGH_SPEED_PLATEAU_DECISION_NONE;
                            Extraction_DW.Extr_HighSpd_PlateauLogic_Enabled.FrictionSum
                                = 0.0F;
                            Extraction_DW.Extr_HighSpd_PlateauLogic_Enabled.BufferIndex
                                = 1U;
                            Extraction_DW.Extr_HighSpd_PlateauLogic_Enabled.FactoryCalibrationFrictionSum
                                = 0.0F;
                            Extraction_DW.Extr_HighSpd_PlateauLogic_Enabled.is_c3_Extraction
                                = Extraction_IN_NO_ACTIVE_CHILD_jm;
                        }

                        if (Extraction_DW.Extr_HighSpd_PlateauLogic_Enabled.is_c3_Extraction
                            == Extraction_IN_BASIC_PLATEAU)
                        {
                        }
                        else
                        {
                            Extraction_DW.Extr_HighSpd_PlateauLogic_Enabled.is_c3_Extraction
                                = Extraction_IN_BASIC_PLATEAU;
                            Extraction_DW.Extr_HighSpd_PlateauLogic_Enabled.temporalCounter_i1
                                = 0U;
                        }
                    }
                }
            }
            break;
        }
    }
}

#endif                                 /* EXTR_VARIANT_HIGH_SPEED_PE_ENABLED */

/* Function for Chart: '<S75>/Extr_HighSpd_PlateauLogic' */
#if EXTR_VARIANT_HIGH_SPEED_PE_ENABLED

static void Extraction_chartstep_c3_Extraction(void)
{
    Extraction_c3_Extraction();
}

#endif                                 /* EXTR_VARIANT_HIGH_SPEED_PE_ENABLED */

/* Function for Chart: '<S79>/Extr_HighSpd_RampLogic' */
#if EXTR_VARIANT_HIGH_SPEED_PE_DISABLED

static void Extraction_chartstep_c49_Extraction(const sint32 *sfEvent_a)
{
    float32 loadMassLimits_idx_0_a;
    float32 loadMassLimits_idx_1_a;
    uint16 tmp_a;
    uint32 qY_a;
    sint16 tmp_ad;
    uint8 is_c49_Extraction_a;
    HIGH_SPEED_RAMP_STATE Extr_HighSpd_RampState_a;
    uint8 is_active_c49_Extraction_a;
    uint32 Extr_HighSpd_EvRampCompleteEventCounter_a;
    boolean_T guard1 = false;
    Extr_HighSpd_EvRampCompleteEventCounter_a =
        Extraction_DW.Extr_HighSpd_RampLogic_Disabled.Extr_HighSpd_EvRampCompleteEventCounter;
    is_active_c49_Extraction_a =
        Extraction_DW.Extr_HighSpd_RampLogic_Disabled.is_active_c49_Extraction;
    Extr_HighSpd_RampState_a = Extr_HighSpd_RampState;
    is_c49_Extraction_a =
        Extraction_DW.Extr_HighSpd_RampLogic_Disabled.is_c49_Extraction;

    /* During: Extraction/Extr_HighSpd/Extr_HighSpd_Algorithms/Extr_HighSpd_RampLogic/Extr_HighSpd_RampLogic_Disabled/Extr_HighSpd_RampLogic */
    if (is_active_c49_Extraction_a == 0U)
    {
        /* Entry: Extraction/Extr_HighSpd/Extr_HighSpd_Algorithms/Extr_HighSpd_RampLogic/Extr_HighSpd_RampLogic_Disabled/Extr_HighSpd_RampLogic */
        is_active_c49_Extraction_a = 1U;

        /* Entry Internal: Extraction/Extr_HighSpd/Extr_HighSpd_Algorithms/Extr_HighSpd_RampLogic/Extr_HighSpd_RampLogic_Disabled/Extr_HighSpd_RampLogic */
        /* Transition: '<S81>:4' */
        is_c49_Extraction_a = Extraction_IN_IDLE_fb;
        Extr_HighSpd_RampState_a = HIGH_SPEED_RAMP_STATE_IDLE;

        /* Entry 'IDLE': '<S81>:3' */
        Extraction_B.VariantMergeForOutportExtr_HighSpd_EnableLoadMassLimitCheck
            = false;
    }
    else if (is_c49_Extraction_a == Extraction_IN_IDLE_fb)
    {
        Extraction_B.VariantMergeForOutportExtr_HighSpd_EnableLoadMassLimitCheck
            = false;

        /* During 'IDLE': '<S81>:3' */
        if (*sfEvent_a == Extraction_event_Extr_HighSpd_EvStartRamp)
        {
            /* Transition: '<S81>:6' */
            is_c49_Extraction_a = Extraction_IN_RAMPING;
            Extr_HighSpd_RampState_a = HIGH_SPEED_RAMP_STATE_RAMPING;

            /* Entry 'RAMPING': '<S81>:5' */
            Extraction_B.VariantMergeForOutportExtr_HighSpd_EnableLoadMassLimitCheck
                = false;

            /* MATLAB Function 'CalculateRampAcceleration': '<S81>:36' */
            /* '<S81>:36:3' */
            loadMassLimits_idx_0_a = (float32)(*GESE_Extr_SpinProfileConfig).
                Load_Mass_x10[0] / 10.0F;
            loadMassLimits_idx_1_a = (float32)(*GESE_Extr_SpinProfileConfig).
                Load_Mass_x10[1] / 10.0F;
            if ((*GESE_Extr_SpinProfileConfig).Inertia_Dependent_Profile_Enabled
                != 0)
            {
                if (Extraction_B.Extr_LowSpd_LoadMass_gp <=
                        loadMassLimits_idx_0_a)
                {
                    /* '<S81>:36:6' */
                    /* '<S81>:36:7' */
                    loadMassLimits_idx_0_a =
                        Extraction_B.Extr_HighSpd_SpinPulse.Ramp_Time[0];
                }
                else if (Extraction_B.Extr_LowSpd_LoadMass_gp >=
                         loadMassLimits_idx_1_a)
                {
                    /* '<S81>:36:9' */
                    /* '<S81>:36:10' */
                    loadMassLimits_idx_0_a =
                        Extraction_B.Extr_HighSpd_SpinPulse.Ramp_Time[1];
                }
                else
                {
                    /* '<S81>:36:12' */
                    /* '<S81>:36:13' */
                    loadMassLimits_idx_1_a -= loadMassLimits_idx_0_a;

                    /*  Verify division by zero */
                    if (loadMassLimits_idx_1_a == 0.0F)
                    {
                        /* '<S81>:36:15' */
                        /* '<S81>:36:16' */
                        loadMassLimits_idx_0_a =
                            Extraction_B.Extr_HighSpd_SpinPulse.Ramp_Time[1];
                    }
                    else
                    {
                        /* '<S81>:36:18' */
                        qY_a = (uint32)
                            Extraction_B.Extr_HighSpd_SpinPulse.Ramp_Time[1] -
                            Extraction_B.Extr_HighSpd_SpinPulse.Ramp_Time[0];
                        if (qY_a >
                                Extraction_B.Extr_HighSpd_SpinPulse.Ramp_Time[1])
                        {
                            qY_a = 0U;
                        }

                        loadMassLimits_idx_0_a = (float32)(sint32)qY_a *
                            (Extraction_B.Extr_LowSpd_LoadMass_gp -
                             loadMassLimits_idx_0_a) / loadMassLimits_idx_1_a +
                            (float32)
                            Extraction_B.Extr_HighSpd_SpinPulse.Ramp_Time[0];
                    }
                }
            }
            else
            {
                /* '<S81>:36:23' */
                loadMassLimits_idx_0_a =
                    Extraction_B.Extr_HighSpd_SpinPulse.Ramp_Time[0];
            }

            /*  Verify division by zero */
            if (loadMassLimits_idx_0_a == 0.0F)
            {
                /* '<S81>:36:27' */
                /* '<S81>:36:28' */
                loadMassLimits_idx_0_a = 1.0F;
            }

            /* '<S81>:36:31' */
            tmp_a = Extr_HighSpd_TargetSpeed;
            if (Extr_HighSpd_TargetSpeed > 32767)
            {
                tmp_a = 32767U;
            }

            loadMassLimits_idx_0_a = roundf(fabsf((float32)
                Extr_HighSpd_TargetSpeed - Extr_Inputs_MotorSignals.Speed_Ref) /
                loadMassLimits_idx_0_a);
            if (loadMassLimits_idx_0_a < 32768.0F)
            {
                if (loadMassLimits_idx_0_a >= -32768.0F)
                {
                    tmp_ad = (sint16)loadMassLimits_idx_0_a;
                }
                else
                {
                    tmp_ad = MIN_int16_T;
                }
            }
            else
            {
                tmp_ad = MAX_int16_T;
            }

            Extraction_SetSpeedCommand((sint16)tmp_a, tmp_ad);
        }
    }
    else
    {
        Extraction_B.VariantMergeForOutportExtr_HighSpd_EnableLoadMassLimitCheck
            = false;

        /* During 'RAMPING': '<S81>:5' */
        guard1 = false;
        if (*sfEvent_a == Extraction_event_Extr_HighSpd_EvStopAll_du)
        {
            /* Transition: '<S81>:67' */
            guard1 = true;
        }
        else if (*sfEvent_a == Extraction_event_Extr_Sup_EvSpeedReached_c4 ||
                 *sfEvent_a == Extraction_event_Extr_Sup_EvSpeedErrorTimeout_p2)
        {
            /* Transition: '<S81>:120' */
            /* Event: '<S81>:7' */
            Extr_HighSpd_EvRampCompleteEventCounter_a++;
            guard1 = true;
        }
        else
        {
            if (*sfEvent_a ==
                    Extraction_event_Extr_HighSpd_EvLoadMassExceeded_oi)
            {
                /* Transition: '<S81>:96' */
                guard1 = true;
            }
        }

        if (guard1)
        {
            is_c49_Extraction_a = Extraction_IN_IDLE_fb;
            Extr_HighSpd_RampState_a = HIGH_SPEED_RAMP_STATE_IDLE;

            /* Entry 'IDLE': '<S81>:3' */
            Extraction_B.VariantMergeForOutportExtr_HighSpd_EnableLoadMassLimitCheck
                = false;
        }
    }

    Extraction_DW.Extr_HighSpd_RampLogic_Disabled.is_c49_Extraction =
        is_c49_Extraction_a;
    Extr_HighSpd_RampState = Extr_HighSpd_RampState_a;
    Extraction_DW.Extr_HighSpd_RampLogic_Disabled.is_active_c49_Extraction =
        is_active_c49_Extraction_a;
    Extraction_DW.Extr_HighSpd_RampLogic_Disabled.Extr_HighSpd_EvRampCompleteEventCounter
        = Extr_HighSpd_EvRampCompleteEventCounter_a;
}

#endif                                 /* EXTR_VARIANT_HIGH_SPEED_PE_DISABLED */

/* Function for Chart: '<S80>/Extr_HighSpd_RampLogic' */
#if EXTR_VARIANT_HIGH_SPEED_PE_ENABLED

static float32 Extraction_CalculateInterpolatedThreshold(float32 targSpeed_l,
    float32 prevSpeed_l, float32 currThreshold_l, float32 prevThreshold_l,
    float32 currentSpeed_l)
{
    float32 threshold_l;
    float32 den_l;

    /* MATLAB Function 'CalculateInterpolatedThreshold': '<S82>:76' */
    if (prevSpeed_l != 0.0F)
    {
        /* '<S82>:76:3' */
        /* '<S82>:76:4' */
        /* '<S82>:76:5' */
        den_l = targSpeed_l - prevSpeed_l;

        /*  Verify division by zero */
        if (den_l == 0.0F)
        {
            /* '<S82>:76:7' */
            /* '<S82>:76:8' */
            threshold_l = currThreshold_l;
        }
        else
        {
            /* '<S82>:76:10' */
            threshold_l = (currThreshold_l - prevThreshold_l) * (currentSpeed_l
                - prevSpeed_l) / den_l + prevThreshold_l;
        }
    }
    else
    {
        /* '<S82>:76:13' */
        threshold_l = currThreshold_l;
    }

    return threshold_l;
}

#endif                                 /* EXTR_VARIANT_HIGH_SPEED_PE_ENABLED */

/* Function for Chart: '<S80>/Extr_HighSpd_RampLogic' */
#if EXTR_VARIANT_HIGH_SPEED_PE_ENABLED

static void Extraction_enter_atomic_MONITORING(void)
{
    static const uint8 b_l[8] =
    {
        1U, 0U, 0U, 1U, 1U, 1U, 0U, 0U
    };

    sint16 PrevTargetSpeed_l;
    float32 Extr_HighSpd_RampViscousThreshold_l;
    float32 Extr_HighSpd_RampFrictionThreshold_l;
    Extr_HighSpd_RampFrictionThreshold_l = Extr_HighSpd_RampFrictionThreshold;
    Extr_HighSpd_RampViscousThreshold_l = Extr_HighSpd_RampViscousThreshold;
    PrevTargetSpeed_l =
        Extraction_DW.Extr_HighSpd_RampLogic_Enabled.PrevTargetSpeed;

    /* Entry 'MONITORING': '<S82>:31' */
    if ((*GESE_Extr_SpinProfileConfig).High_Speed_Enabled != 0)
    {
        Extraction_SelectParamEstOutputs(b_l);
        Extraction_B.VariantMergeForOutportExtr_HighSpd_EnableLoadMassLimitCheck
            = true;
        Extraction_B.VariantMergeForOutportExtr_HighSpd_EnableCEM = true;
    }
    else
    {
        Extr_HighSpd_RampViscousThreshold_l = 0.0F;
        Extr_HighSpd_RampFrictionThreshold_l = 0.0F;
        Extraction_B.VariantMergeForOutportExtr_HighSpd_EnableLoadMassLimitCheck
            = false;
        Extraction_B.VariantMergeForOutportExtr_HighSpd_EnableCEM = false;
    }

    if ((*GESE_Extr_SpinProfileConfig).High_Speed_Enabled != 0)
    {
        Extr_HighSpd_RampViscousThreshold_l =
            Extraction_CalculateInterpolatedThreshold((float32)
            Extr_HighSpd_TargetSpeed, (float32)PrevTargetSpeed_l,
            Extraction_B.OutportBufferForExtr_HighSpd_PulseConfig.Ramp_Viscous_Friction_Threshold,
            Extraction_B.OutportBufferForExtr_HighSpd_PrevPulseConfig.Ramp_Viscous_Friction_Threshold,
            Extr_Inputs_MotorSignals.Speed);
        Extr_HighSpd_RampFrictionThreshold_l =
            Extraction_CalculateInterpolatedThreshold((float32)
            Extr_HighSpd_TargetSpeed, (float32)PrevTargetSpeed_l,
            Extr_HighSpd_BaselineFriction * (1.0F + (float32)
            Extraction_B.OutportBufferForExtr_HighSpd_PulseConfig.Plateau_Hold_Threshold
            / 100.0F), Extr_HighSpd_PrevBaselineFriction * (1.0F + (float32)
            Extraction_B.OutportBufferForExtr_HighSpd_PrevPulseConfig.Plateau_Hold_Threshold
            / 100.0F), Extr_Inputs_MotorSignals.Speed);
        Extr_HighSpd_RampFrictionDelta = Extr_Inputs_ParamEstData.Friction -
            Extr_HighSpd_ReferenceFriction;
    }

    Extr_HighSpd_RampViscousThreshold = Extr_HighSpd_RampViscousThreshold_l;
    Extr_HighSpd_RampFrictionThreshold = Extr_HighSpd_RampFrictionThreshold_l;
}

#endif                                 /* EXTR_VARIANT_HIGH_SPEED_PE_ENABLED */

/* Function for Chart: '<S80>/Extr_HighSpd_RampLogic' */
#if EXTR_VARIANT_HIGH_SPEED_PE_ENABLED

static uint16 Extraction_CalcNewFrictionBaseline_b1(float32 delta_friction_l,
    float32 baseline_friction_l, uint16 baseline_config_Friction_Max_x1000_l,
    uint16 baseline_config_Friction_Min_x1000_l, uint8
    baseline_config_Baseline_Update_Threshold_l, uint8
    baseline_config_Percent_Of_Current_Threshold_l)
{
    uint16 new_baseline_x1000_l;
    float32 tmp_l;

    /* MATLAB Function 'CalcNewFrictionBaseline': '<S82>:149' */
    /* '<S82>:149:3' */
    tmp_l = roundf(baseline_friction_l * 1000.0F);
    if (tmp_l < 65536.0F)
    {
        if (tmp_l >= 0.0F)
        {
            new_baseline_x1000_l = (uint16)tmp_l;
        }
        else
        {
            new_baseline_x1000_l = 0U;
        }
    }
    else
    {
        new_baseline_x1000_l = MAX_uint16_T;
    }

    /*  Req 250678 */
    if (delta_friction_l <= baseline_friction_l * (1.0F + (float32)
            baseline_config_Baseline_Update_Threshold_l / 100.0F))
    {
        /* '<S82>:149:6' */
        /* '<S82>:149:7' */
        tmp_l = roundf(((delta_friction_l - baseline_friction_l) * (float32)
                        baseline_config_Percent_Of_Current_Threshold_l / 100.0F
                        + baseline_friction_l) * 1000.0F);
        if (tmp_l < 65536.0F)
        {
            if (tmp_l >= 0.0F)
            {
                new_baseline_x1000_l = (uint16)tmp_l;
            }
            else
            {
                new_baseline_x1000_l = 0U;
            }
        }
        else
        {
            new_baseline_x1000_l = MAX_uint16_T;
        }

        /*  Req 255259 */
        if (new_baseline_x1000_l > baseline_config_Friction_Max_x1000_l)
        {
            /* '<S82>:149:10' */
            /* '<S82>:149:11' */
            new_baseline_x1000_l = baseline_config_Friction_Max_x1000_l;
        }
        else
        {
            /*  Req 255258 */
            if (new_baseline_x1000_l < baseline_config_Friction_Min_x1000_l)
            {
                /* '<S82>:149:14' */
                /* '<S82>:149:15' */
                new_baseline_x1000_l = baseline_config_Friction_Min_x1000_l;
            }
        }
    }

    return new_baseline_x1000_l;
}

#endif                                 /* EXTR_VARIANT_HIGH_SPEED_PE_ENABLED */

/* Function for Chart: '<S80>/Extr_HighSpd_RampLogic' */
#if EXTR_VARIANT_HIGH_SPEED_PE_ENABLED

static void Extraction_enter_atomic_RAMPING(void)
{
    float32 loadMassLimits_idx_0_l;
    float32 loadMassLimits_idx_1_l;
    uint16 tmp_l;
    uint32 qY_l;
    sint16 tmp_lp;

    /* Entry 'RAMPING': '<S82>:5' */
    Extraction_DW.Extr_HighSpd_RampLogic_Enabled.PrevTargetSpeed = (sint16)
        (Extraction_B.OutportBufferForExtr_HighSpd_PrevSpinPulse.Target_Speed_Div10
         * 10);

    /* MATLAB Function 'CalculateRampAcceleration': '<S82>:127' */
    /* '<S82>:127:3' */
    loadMassLimits_idx_0_l = (float32)(*GESE_Extr_SpinProfileConfig).
        Load_Mass_x10[0] / 10.0F;
    loadMassLimits_idx_1_l = (float32)(*GESE_Extr_SpinProfileConfig).
        Load_Mass_x10[1] / 10.0F;
    if ((*GESE_Extr_SpinProfileConfig).Inertia_Dependent_Profile_Enabled != 0)
    {
        if (Extraction_B.Extr_LowSpd_LoadMass_gp <= loadMassLimits_idx_0_l)
        {
            /* '<S82>:127:6' */
            /* '<S82>:127:7' */
            loadMassLimits_idx_0_l =
                Extraction_B.Extr_HighSpd_SpinPulse.Ramp_Time[0];
        }
        else if (Extraction_B.Extr_LowSpd_LoadMass_gp >= loadMassLimits_idx_1_l)
        {
            /* '<S82>:127:9' */
            /* '<S82>:127:10' */
            loadMassLimits_idx_0_l =
                Extraction_B.Extr_HighSpd_SpinPulse.Ramp_Time[1];
        }
        else
        {
            /* '<S82>:127:12' */
            /* '<S82>:127:13' */
            loadMassLimits_idx_1_l -= loadMassLimits_idx_0_l;

            /*  Verify division by zero */
            if (loadMassLimits_idx_1_l == 0.0F)
            {
                /* '<S82>:127:15' */
                /* '<S82>:127:16' */
                loadMassLimits_idx_0_l =
                    Extraction_B.Extr_HighSpd_SpinPulse.Ramp_Time[1];
            }
            else
            {
                /* '<S82>:127:18' */
                qY_l = (uint32)Extraction_B.Extr_HighSpd_SpinPulse.Ramp_Time[1]
                    - Extraction_B.Extr_HighSpd_SpinPulse.Ramp_Time[0];
                if (qY_l > Extraction_B.Extr_HighSpd_SpinPulse.Ramp_Time[1])
                {
                    qY_l = 0U;
                }

                loadMassLimits_idx_0_l = (float32)(sint32)qY_l *
                    (Extraction_B.Extr_LowSpd_LoadMass_gp -
                     loadMassLimits_idx_0_l) / loadMassLimits_idx_1_l + (float32)
                    Extraction_B.Extr_HighSpd_SpinPulse.Ramp_Time[0];
            }
        }
    }
    else
    {
        /* '<S82>:127:23' */
        loadMassLimits_idx_0_l = Extraction_B.Extr_HighSpd_SpinPulse.Ramp_Time[0];
    }

    /*  Verify division by zero */
    if (loadMassLimits_idx_0_l == 0.0F)
    {
        /* '<S82>:127:27' */
        /* '<S82>:127:28' */
        loadMassLimits_idx_0_l = 1.0F;
    }

    /* '<S82>:127:31' */
    tmp_l = Extr_HighSpd_TargetSpeed;
    if (Extr_HighSpd_TargetSpeed > 32767)
    {
        tmp_l = 32767U;
    }

    loadMassLimits_idx_0_l = roundf(fabsf((float32)Extr_HighSpd_TargetSpeed -
        Extr_Inputs_MotorSignals.Speed_Ref) / loadMassLimits_idx_0_l);
    if (loadMassLimits_idx_0_l < 32768.0F)
    {
        if (loadMassLimits_idx_0_l >= -32768.0F)
        {
            tmp_lp = (sint16)loadMassLimits_idx_0_l;
        }
        else
        {
            tmp_lp = MIN_int16_T;
        }
    }
    else
    {
        tmp_lp = MAX_int16_T;
    }

    Extraction_SetSpeedCommand((sint16)tmp_l, tmp_lp);
    Extr_HighSpd_RampFrictionDelta = 0.0F;
}

#endif                                 /* EXTR_VARIANT_HIGH_SPEED_PE_ENABLED */

/* Function for Chart: '<S80>/Extr_HighSpd_RampLogic' */
#if EXTR_VARIANT_HIGH_SPEED_PE_ENABLED

static void Extraction_chartstep_c8_Extraction(const sint32 *sfEvent_l)
{
    static const uint8 b_l[8] =
    {
        1U, 0U, 0U, 1U, 1U, 1U, 0U, 0U
    };

    sint32 tmp_l;
    uint32 tmp_lp;
    uint8 is_RAMPING_l;
    uint8 is_c8_Extraction_l;
    uint32 temporalCounter_i1_l;
    HIGH_SPEED_RAMP_STATE Extr_HighSpd_RampState_l;
    uint8 is_active_c8_Extraction_l;
    uint32 Extr_HighSpd_EvRampCompleteEventCounter_l;
    uint32 Extr_HighSpd_EvViscousLimitExceededEventCounter_l;
    uint32 Extr_HighSpd_EvFrictionLimitExceededEventCounter_l;
    boolean_T guard1 = false;
    Extr_HighSpd_EvFrictionLimitExceededEventCounter_l =
        Extraction_DW.Extr_HighSpd_RampLogic_Enabled.Extr_HighSpd_EvFrictionLimitExceededEventCounter;
    Extr_HighSpd_EvViscousLimitExceededEventCounter_l =
        Extraction_DW.Extr_HighSpd_RampLogic_Enabled.Extr_HighSpd_EvViscousLimitExceededEventCounter;
    Extr_HighSpd_EvRampCompleteEventCounter_l =
        Extraction_DW.Extr_HighSpd_RampLogic_Enabled.Extr_HighSpd_EvRampCompleteEventCounter;
    is_active_c8_Extraction_l =
        Extraction_DW.Extr_HighSpd_RampLogic_Enabled.is_active_c8_Extraction;
    Extr_HighSpd_RampState_l = Extr_HighSpd_RampState;
    temporalCounter_i1_l =
        Extraction_DW.Extr_HighSpd_RampLogic_Enabled.temporalCounter_i1;
    is_c8_Extraction_l =
        Extraction_DW.Extr_HighSpd_RampLogic_Enabled.is_c8_Extraction;
    is_RAMPING_l = Extraction_DW.Extr_HighSpd_RampLogic_Enabled.is_RAMPING;

    /* During: Extraction/Extr_HighSpd/Extr_HighSpd_Algorithms/Extr_HighSpd_RampLogic/Extr_HighSpd_RampLogic_Enabled/Extr_HighSpd_RampLogic */
    if (is_active_c8_Extraction_l == 0U)
    {
        /* Entry: Extraction/Extr_HighSpd/Extr_HighSpd_Algorithms/Extr_HighSpd_RampLogic/Extr_HighSpd_RampLogic_Enabled/Extr_HighSpd_RampLogic */
        is_active_c8_Extraction_l = 1U;

        /* Entry Internal: Extraction/Extr_HighSpd/Extr_HighSpd_Algorithms/Extr_HighSpd_RampLogic/Extr_HighSpd_RampLogic_Enabled/Extr_HighSpd_RampLogic */
        /* Transition: '<S82>:4' */
        is_c8_Extraction_l = Extraction_IN_IDLE_iz;
        Extr_HighSpd_RampState_l = HIGH_SPEED_RAMP_STATE_IDLE;

        /* Entry 'IDLE': '<S82>:3' */
        Extraction_B.VariantMergeForOutportExtr_HighSpd_EnableCEM = false;
        Extraction_B.VariantMergeForOutportExtr_HighSpd_EnableLoadMassLimitCheck
            = false;
    }
    else if (is_c8_Extraction_l == Extraction_IN_IDLE_iz)
    {
        /* During 'IDLE': '<S82>:3' */
        if (*sfEvent_l == Extraction_event_Extr_HighSpd_EvStartRamp_oc)
        {
            /* Transition: '<S82>:6' */
            /* Transition: '<S82>:114' */
            if (Extraction_B.cov_reset_on_ramp)
            {
                /* Transition: '<S82>:112' */
                /* Exit 'IDLE': '<S82>:3' */
                Extraction_B.VariantMergeForOutportExtr_HighSpd_EnableCEM =
                    false;
                Extraction_B.VariantMergeForOutportExtr_HighSpd_EnableLoadMassLimitCheck
                    = false;
                is_c8_Extraction_l = Extraction_IN_RAMPING_it;
                Extraction_enter_atomic_RAMPING();

                /* Entry Internal 'RAMPING': '<S82>:5' */
                /* Transition: '<S82>:23' */
                is_RAMPING_l = Extraction_IN_WAITING_STABILIZATION_TIME;
                temporalCounter_i1_l = 0U;
                Extr_HighSpd_RampState_l =
                    HIGH_SPEED_RAMP_STATE_WAITING_STABILIZATION_TIME;
            }
            else
            {
                /* Transition: '<S82>:108' */
                /* Transition: '<S82>:107' */
                /* Exit 'IDLE': '<S82>:3' */
                Extraction_B.VariantMergeForOutportExtr_HighSpd_EnableCEM =
                    false;
                Extraction_B.VariantMergeForOutportExtr_HighSpd_EnableLoadMassLimitCheck
                    = false;
                is_c8_Extraction_l = Extraction_IN_RAMPING_it;
                Extraction_enter_atomic_RAMPING();
                is_RAMPING_l = Extraction_IN_MONITORING;
                Extr_HighSpd_RampState_l = HIGH_SPEED_RAMP_STATE_MONITORING;
                Extraction_enter_atomic_MONITORING();
            }
        }
    }
    else
    {
        /* During 'RAMPING': '<S82>:5' */
        guard1 = false;
        if (*sfEvent_l == Extraction_event_Extr_Sup_EvSpeedReached_ix)
        {
            /* Transition: '<S82>:43' */
            /* Event: '<S82>:7' */
            Extr_HighSpd_EvRampCompleteEventCounter_l++;
            if ((*GESE_Extr_SpinProfileConfig).Factory_Calibration_Enabled != 0 &&
                (*GESE_Extr_SpinProfileConfig).High_Speed_Enabled != 0 &&
                    Extr_HighSpd_PlateauDuration == 0.0F)
            {
                /* Outputs for Function Call SubSystem: '<S82>/UpdateFactoryCalibrationFriction' */

                /* Transition: '<S82>:166' */
                /* Transition: '<S82>:167' */
                /* Simulink Function 'UpdateFactoryCalibrationFriction': '<S82>:170' */
                Extraction_FRICTION_BASED_PLATEAUMONITORUpdateFactoryCalibrationFriction
                    (Extr_HighSpd_RampFrictionDelta,
                     Extr_HighSpd_BaselineFrictionIndex);

                /* End of Outputs for SubSystem: '<S82>/UpdateFactoryCalibrationFriction' */
            }
            else if ((*GESE_Extr_SpinProfileConfig).High_Speed_Enabled != 0 &&
                     Extr_HighSpd_PlateauDuration == 0.0F)
            {
                /* Transition: '<S82>:160' */
                /* Transition: '<S82>:157' */
                tmp_l = (sint32)(Extr_HighSpd_BaselineFrictionIndex + 1U);
                if ((uint32)tmp_l > 255U)
                {
                    tmp_l = 255;
                }

                ExtractionParams__SetBaselineFriction( tmp_l - 1 ,
                    Extraction_CalcNewFrictionBaseline_b1
                    (Extr_HighSpd_RampFrictionDelta,
                     Extr_HighSpd_BaselineFriction,
                     Extraction_B.VariantMergeForOutportExtr_HighSpd_BaselineConfig.Friction_Max_x1000,
                     Extraction_B.VariantMergeForOutportExtr_HighSpd_BaselineConfig.Friction_Min_x1000,
                     Extraction_B.VariantMergeForOutportExtr_HighSpd_BaselineConfig.Baseline_Update_Threshold,
                     Extraction_B.VariantMergeForOutportExtr_HighSpd_BaselineConfig.Percent_Of_Current_Threshold));
            }
            else
            {
                /* Transition: '<S82>:174' */
            }

            guard1 = true;
        }
        else if (*sfEvent_l == Extraction_event_Extr_HighSpd_EvStopAll_gt ||
                 *sfEvent_l ==
                 Extraction_event_Extr_HighSpd_EvLoadMassExceeded_ef)
        {
            /* Transition: '<S82>:67' */
            /* Transition: '<S82>:96' */
            guard1 = true;
        }
        else if (*sfEvent_l == Extraction_event_Extr_Sup_EvSpeedErrorTimeout_nh)
        {
            /* Transition: '<S82>:144' */
            /* Event: '<S82>:7' */
            Extr_HighSpd_EvRampCompleteEventCounter_l++;
            guard1 = true;
        }
        else
        {
            switch (is_RAMPING_l)
            {
              case Extraction_IN_MONITORING:
                /* During 'MONITORING': '<S82>:31' */
                if ((*GESE_Extr_SpinProfileConfig).High_Speed_Enabled != 0 &&
                        !((*GESE_Extr_SpinProfileConfig).
                          Factory_Calibration_Enabled != 0) &&
                        Extr_Inputs_ParamEstData.Viscous_Friction >=
                        Extr_HighSpd_RampViscousThreshold)
                {
                    /* Transition: '<S82>:56' */
                    /* Transition: '<S82>:100' */
                    is_RAMPING_l = Extraction_IN_NO_ACTIVE_CHILD_l5;

                    /* Exit 'RAMPING': '<S82>:5' */
                    if (Extr_HighSpd_RampFrictionDelta < 0.0F)
                    {
                        Extr_HighSpd_RampFrictionDelta = 0.0F;
                    }

                    /* Event: '<S82>:48' */
                    Extr_HighSpd_EvViscousLimitExceededEventCounter_l++;
                    is_c8_Extraction_l = Extraction_IN_IDLE_iz;
                    Extr_HighSpd_RampState_l = HIGH_SPEED_RAMP_STATE_IDLE;

                    /* Entry 'IDLE': '<S82>:3' */
                    Extraction_B.VariantMergeForOutportExtr_HighSpd_EnableCEM =
                        false;
                    Extraction_B.VariantMergeForOutportExtr_HighSpd_EnableLoadMassLimitCheck
                        = false;
                }
                else if ((*GESE_Extr_SpinProfileConfig).High_Speed_Enabled != 0 &&
                         !((*GESE_Extr_SpinProfileConfig).
                           Factory_Calibration_Enabled != 0) &&
                         Extr_HighSpd_RampFrictionDelta >=
                         Extr_HighSpd_RampFrictionThreshold)
                {
                    /* Transition: '<S82>:139' */
                    /* Transition: '<S82>:141' */
                    /* Transition: '<S82>:100' */
                    is_RAMPING_l = Extraction_IN_NO_ACTIVE_CHILD_l5;

                    /* Exit 'RAMPING': '<S82>:5' */
                    if (Extr_HighSpd_RampFrictionDelta < 0.0F)
                    {
                        Extr_HighSpd_RampFrictionDelta = 0.0F;
                    }

                    /* Event: '<S82>:142' */
                    Extr_HighSpd_EvFrictionLimitExceededEventCounter_l++;
                    is_c8_Extraction_l = Extraction_IN_IDLE_iz;
                    Extr_HighSpd_RampState_l = HIGH_SPEED_RAMP_STATE_IDLE;

                    /* Entry 'IDLE': '<S82>:3' */
                    Extraction_B.VariantMergeForOutportExtr_HighSpd_EnableCEM =
                        false;
                    Extraction_B.VariantMergeForOutportExtr_HighSpd_EnableLoadMassLimitCheck
                        = false;
                }
                else
                {
                    if ((*GESE_Extr_SpinProfileConfig).High_Speed_Enabled != 0)
                    {
                        Extr_HighSpd_RampViscousThreshold =
                            Extraction_CalculateInterpolatedThreshold((float32)
                            Extr_HighSpd_TargetSpeed, (float32)
                            Extraction_DW.Extr_HighSpd_RampLogic_Enabled.PrevTargetSpeed,
                            Extraction_B.OutportBufferForExtr_HighSpd_PulseConfig.Ramp_Viscous_Friction_Threshold,
                            Extraction_B.OutportBufferForExtr_HighSpd_PrevPulseConfig.Ramp_Viscous_Friction_Threshold,
                            Extr_Inputs_MotorSignals.Speed);
                        Extr_HighSpd_RampFrictionThreshold =
                            Extraction_CalculateInterpolatedThreshold((float32)
                            Extr_HighSpd_TargetSpeed, (float32)
                            Extraction_DW.Extr_HighSpd_RampLogic_Enabled.PrevTargetSpeed,
                            Extr_HighSpd_BaselineFriction * (1.0F + (float32)
                            Extraction_B.OutportBufferForExtr_HighSpd_PulseConfig.Plateau_Hold_Threshold
                            / 100.0F), Extr_HighSpd_PrevBaselineFriction * (1.0F
                            + (float32)
                            Extraction_B.OutportBufferForExtr_HighSpd_PrevPulseConfig.Plateau_Hold_Threshold
                            / 100.0F), Extr_Inputs_MotorSignals.Speed);
                        Extr_HighSpd_RampFrictionDelta =
                            Extr_Inputs_ParamEstData.Friction -
                            Extr_HighSpd_ReferenceFriction;
                    }
                }
                break;

              case Extraction_IN_WAITING_PE_COV_RESET_TIME:
                /* During 'WAITING_PE_COV_RESET_TIME': '<S82>:19' */
                tmp_lp = (uint32)(uint16)(sint32)roundf((float32)
                    Extraction_B.OutportBufferForExtr_HighSpd_ParamEstAppSpecificSetup.Cov_Reset_Wait_x10
                    / 10.0F) * ((uint16)FS_EXTRACTION);
                if (tmp_lp > 65535U)
                {
                    tmp_lp = 65535U;
                }

                if (*sfEvent_l == Extraction_event_ticks_eh &&
                        temporalCounter_i1_l >= tmp_lp)
                {
                    /* Transition: '<S82>:32' */
                    is_RAMPING_l = Extraction_IN_MONITORING;
                    Extr_HighSpd_RampState_l = HIGH_SPEED_RAMP_STATE_MONITORING;
                    Extraction_enter_atomic_MONITORING();
                }
                break;

              default:
                /* During 'WAITING_STABILIZATION_TIME': '<S82>:22' */
                tmp_lp = (uint32)
                    Extraction_B.OutportBufferForExtr_HighSpd_ParamEstAppSpecificSetup.Stabilization_Time
                    * ((uint16)FS_EXTRACTION);
                if (tmp_lp > 65535U)
                {
                    tmp_lp = 65535U;
                }

                if (*sfEvent_l == Extraction_event_ticks_eh &&
                        temporalCounter_i1_l >= tmp_lp)
                {
                    /* Transition: '<S82>:20' */
                    /* Exit 'WAITING_STABILIZATION_TIME': '<S82>:22' */
                    if ((*GESE_Extr_SpinProfileConfig).High_Speed_Enabled != 0)
                    {
                        Extraction_SelectParamEstOutputs(b_l);
                        Extraction_ParamEstControl(PE_COV_RESET);
                    }

                    is_RAMPING_l = Extraction_IN_WAITING_PE_COV_RESET_TIME;
                    temporalCounter_i1_l = 0U;
                    Extr_HighSpd_RampState_l =
                        HIGH_SPEED_RAMP_STATE_WAITING_PE_COV_RESET_TIME;
                }
                break;
            }
        }

        if (guard1)
        {
            /* Exit Internal 'RAMPING': '<S82>:5' */
            if (is_RAMPING_l == Extraction_IN_WAITING_STABILIZATION_TIME)
            {
                /* Exit 'WAITING_STABILIZATION_TIME': '<S82>:22' */
                if ((*GESE_Extr_SpinProfileConfig).High_Speed_Enabled != 0)
                {
                    Extraction_SelectParamEstOutputs(b_l);
                    Extraction_ParamEstControl(PE_COV_RESET);
                }

                is_RAMPING_l = Extraction_IN_NO_ACTIVE_CHILD_l5;
            }
            else
            {
                is_RAMPING_l = Extraction_IN_NO_ACTIVE_CHILD_l5;
            }

            /* Exit 'RAMPING': '<S82>:5' */
            if (Extr_HighSpd_RampFrictionDelta < 0.0F)
            {
                Extr_HighSpd_RampFrictionDelta = 0.0F;
            }

            is_c8_Extraction_l = Extraction_IN_IDLE_iz;
            Extr_HighSpd_RampState_l = HIGH_SPEED_RAMP_STATE_IDLE;

            /* Entry 'IDLE': '<S82>:3' */
            Extraction_B.VariantMergeForOutportExtr_HighSpd_EnableCEM = false;
            Extraction_B.VariantMergeForOutportExtr_HighSpd_EnableLoadMassLimitCheck
                = false;
        }
    }

    Extraction_DW.Extr_HighSpd_RampLogic_Enabled.is_RAMPING = is_RAMPING_l;
    Extraction_DW.Extr_HighSpd_RampLogic_Enabled.is_c8_Extraction =
        is_c8_Extraction_l;
    Extraction_DW.Extr_HighSpd_RampLogic_Enabled.temporalCounter_i1 =
        temporalCounter_i1_l;
    Extr_HighSpd_RampState = Extr_HighSpd_RampState_l;
    Extraction_DW.Extr_HighSpd_RampLogic_Enabled.is_active_c8_Extraction =
        is_active_c8_Extraction_l;
    Extraction_DW.Extr_HighSpd_RampLogic_Enabled.Extr_HighSpd_EvRampCompleteEventCounter
        = Extr_HighSpd_EvRampCompleteEventCounter_l;
    Extraction_DW.Extr_HighSpd_RampLogic_Enabled.Extr_HighSpd_EvViscousLimitExceededEventCounter
        = Extr_HighSpd_EvViscousLimitExceededEventCounter_l;
    Extraction_DW.Extr_HighSpd_RampLogic_Enabled.Extr_HighSpd_EvFrictionLimitExceededEventCounter
        = Extr_HighSpd_EvFrictionLimitExceededEventCounter_l;
}

#endif                                 /* EXTR_VARIANT_HIGH_SPEED_PE_ENABLED */

/* Output and update for function-call system: '<S180>/DistUtilsWrapperControl' */
void Extraction_DistUtilsWrapperControl(DIST_UTILS_COMMAND_TYPE
    rtu_distUtilsCommand, Extraction_B_DistUtilsWrapperControl_TYPE *localB)
{
    /* DataTypeConversion: '<S185>/Data Type Conversion' */
    localB->DataTypeConversion = (uint8)rtu_distUtilsCommand;

    /* S-Function (DistUtils_sfun): '<S185>/DistUtils' incorporates:
     *  Constant: '<S185>/Constant'
     *  Constant: '<S185>/Constant1'
     */
    DistUtils_sfun_Outputs_wrapper(&localB->DataTypeConversion,
        &rtCP_Constant_Value_g3, &rtCP_Constant1_Value_lc, &localB->DistUtils_o1,
        &localB->DistUtils_o2, &localB->DistUtils_o3, &localB->DistUtils_o4);
}

/* Output and update for function-call system: '<S180>/REBALANCE.RUNNING.DISTRIBUTION_RAMP.P2PTorqueThresholdCalc' */
void Extraction_REBALANCERUNNINGDISTRIBUTION_RAMPP2PTorqueThresholdCalc(float32
    rtu_mass, float32 rtu_currentSpeed,
    Extraction_B_REBALANCERUNNINGDISTRIBUTION_RAMPP2PTorqueThresholdCalc_TYPE
    *localB)
{
    uint32 rtb_loadMassPrelookup_o1;
    uint32 rtb_speedRefPrelookup_o1;
    float32 rtb_p2pTorqueInterpolation;
    float32 frac[2];
    uint32 bpIndex[2];
    uint32 rtb_loadMassPrelookup_o2;
    uint32 rtb_speedRefPrelookup_o2;

    /* PreLookup: '<S187>/loadMassPrelookup' incorporates:
     *  DataTypeConversion: '<S187>/Data Type Conversion1'
     *  Gain: '<S187>/x10'
     */
    rtb_loadMassPrelookup_o1 = plook_u32u16u32n31_binc_f((uint16)(10.0F *
        rtu_mass), (&(GESE_Extr_P2PTorqueLoadMassX10_BP.BP[0])),
        GESE_Extr_P2PTorqueLoadMassX10_BP.N - 1U, &rtb_loadMassPrelookup_o2);

    /* PreLookup: '<S187>/speedRefPrelookup' incorporates:
     *  DataTypeConversion: '<S187>/Data Type Conversion2'
     */
    rtb_speedRefPrelookup_o1 = plook_u32u16u32n31_binc_f((uint16)
        rtu_currentSpeed, (&(GESE_Extr_FastDistRampTargetSpeed_BP.BP[0])),
        GESE_Extr_FastDistRampTargetSpeed_BP.N - 1U, &rtb_speedRefPrelookup_o2);

    /* Interpolation_n-D: '<S187>/p2pTorqueInterpolation' incorporates:
     *  Constant: '<S187>/p2pTorqueLutData'
     */
    frac[0] = (float32)rtb_loadMassPrelookup_o2 * 4.65661287E-10F;
    frac[1] = (float32)rtb_speedRefPrelookup_o2 * 4.65661287E-10F;
    bpIndex[0] = rtb_loadMassPrelookup_o1;
    bpIndex[1] = rtb_speedRefPrelookup_o1;
    rtb_p2pTorqueInterpolation = intrp2d_u16ffu32fl(bpIndex, frac,
        (&(GESE_Extr_P2PTorqueX100_LUT[0])), GESE_Extr_P2PTorqueLoadMassX10_BP.N);

    /* Gain: '<S187>/Gain' */
    localB->Gain = 0.01F * rtb_p2pTorqueInterpolation;
}

/* Output and update for Simulink Function: '<S12>/FindInitialDistributionPulse' */
void Extraction_FindInitialDistributionPulse(float32 rtu_speed, uint8 *rty_index)
{
    uint8 b_index;
    uint32 qY;
    uint8 GESE_Extr_DistProfPulseCnt_0;
    boolean_T exitg1;

    /* MATLAB Function: '<S181>/MATLAB Function' */
    GESE_Extr_DistProfPulseCnt_0 = GESE_Extr_DistProfPulseCnt;

    /* MATLAB Function 'Extraction/Extr_Rebalance/FindInitialDistributionPulse/MATLAB Function': '<S188>:1' */
    /*  Req 825208 */
    qY = GESE_Extr_DistProfPulseCnt_0 - 1U;
    if (qY > GESE_Extr_DistProfPulseCnt_0)
    {
        qY = 0U;
    }

    *rty_index = 0U;

    /* '<S188>:1:7' */
    b_index = 0U;
    exitg1 = false;
    while (!exitg1 && b_index <= (uint8)qY)
    {
        /* '<S188>:1:7' */
        *rty_index = b_index;
        if ((float32)GESE_Extr_DistProf[b_index].Target_Speed_Div2 * 2.0F >
                rtu_speed - 3.0F)
        {
            /* '<S188>:1:8' */
            exitg1 = true;
        }
        else
        {
            b_index++;
        }
    }

    /* End of MATLAB Function: '<S181>/MATLAB Function' */
}

/* Output and update for Simulink Function: '<S12>/LoadRebalanceProfile' */
void Extraction_LoadRebalanceProfile(uint8 rtu_index, boolean_T *rty_pass)
{
    /* MATLAB Function: '<S182>/MATLAB Function' incorporates:
     *  SignalConversion: '<S182>/TmpSignal ConversionAtindexOutport1'
     */
    /* MATLAB Function 'Extraction/Extr_Rebalance/LoadRebalanceProfile/MATLAB Function': '<S189>:1' */
    /* '<S189>:1:8' */
    *rty_pass = ExtractionParams__LoadDistributionProfile(rtu_index);
}

/* Output and update for Simulink Function: '<S12>/LoadRebalanceProfileIndex' */
void Extraction_LoadRebalanceProfileIndex(uint8 rtu_rebal_count, uint8
    *rty_index)
{
    uint8 ii;
    uint8 GESE_Extr_RebalTableCount_0;
    uint8 Extr_Rebal_Index_0;
    boolean_T exitg1;

    /* MATLAB Function: '<S183>/MATLAB Function' */
    GESE_Extr_RebalTableCount_0 = GESE_Extr_RebalTableCount;

    /* MATLAB Function 'Extraction/Extr_Rebalance/LoadRebalanceProfileIndex/MATLAB Function': '<S190>:1' */
    /* '<S190>:1:6' */
    Extr_Rebal_Index_0 = GESE_Extr_RebalTable[GESE_Extr_RebalTableCount_0 - 1].
        Rebalance_Profile_Index;

    /* '<S190>:1:8' */
    ii = 1U;
    exitg1 = false;
    while (!exitg1 && ii <= GESE_Extr_RebalTableCount_0)
    {
        /* '<S190>:1:8' */
        if (rtu_rebal_count < GESE_Extr_RebalTable[ii - 1].Rebalance_Attempts)
        {
            /* '<S190>:1:9' */
            /* '<S190>:1:10' */
            Extr_Rebal_Index_0 = GESE_Extr_RebalTable[ii - 1].
                Rebalance_Profile_Index;
            exitg1 = true;
        }
        else
        {
            ii++;
        }
    }

    /* SignalConversion: '<S183>/TmpSignal ConversionAtindexInport1' */
    *rty_index = Extr_Rebal_Index_0;
    Extr_Rebal_Index = Extr_Rebal_Index_0;
}

/* Output and update for Simulink Function: '<S30>/AccelerometerControl' */
#if EXTR_VARIANT_ACCELEROMETER_ENABLED

void Extraction_AccelerometerControl(boolean_T rtu_enabled)
{
    /* MATLAB Function: '<S37>/AccelerometerControlMacro' incorporates:
     *  SignalConversion: '<S37>/TmpSignal ConversionAtenabledOutport1'
     */
    /* MATLAB Function 'Extraction/Extr_Accel/Extr_Accel_Enabled/Extr_Accel_Enabled/Extr_Accel_Supervisor/AccelerometerControl/AccelerometerControlMacro': '<S48>:1' */
    if (rtu_enabled)
    {
        /* '<S48>:1:3' */
        EXTRACTION_ENABLE_ACCELEROMETER();
    }
    else
    {
        EXTRACTION_DISABLE_ACCELEROMETER();
    }

    /* End of MATLAB Function: '<S37>/AccelerometerControlMacro' */
}

#endif                                 /* EXTR_VARIANT_ACCELEROMETER_ENABLED */

/* Output and update for Simulink Function: '<S30>/GetAccelBaseline' */
#if EXTR_VARIANT_ACCELEROMETER_ENABLED

void Extraction_GetAccelBaseline(float32 rty_Accel_Baseline[3])
{
    /* MATLAB Function: '<S39>/GetAccelBaselineMacro' */
    /* MATLAB Function 'Extraction/Extr_Accel/Extr_Accel_Enabled/Extr_Accel_Enabled/Extr_Accel_Supervisor/GetAccelBaseline/GetAccelBaselineMacro': '<S49>:1' */
    // Retrieve the accelerometer baseline (X,Y,Z vector)	();
    EXTRACTION_GET_ACCELEROMETER_BASELINE(Extr_Inputs_AccelBaselineRaw);

    /* SignalConversion: '<S39>/TmpSignal ConversionAtAccel_BaselineInport1' incorporates:
     *  Gain: '<S39>/AccelBaselineFloat'
     */
    rty_Accel_Baseline[0] = 0.00390625F * (float32)Extr_Inputs_AccelBaselineRaw
        [0];
    rty_Accel_Baseline[1] = 0.00390625F * (float32)Extr_Inputs_AccelBaselineRaw
        [1];
    rty_Accel_Baseline[2] = 0.00390625F * (float32)Extr_Inputs_AccelBaselineRaw
        [2];
}

#endif                                 /* EXTR_VARIANT_ACCELEROMETER_ENABLED */

/* Output and update for Simulink Function: '<S30>/GetAccelBaselineStatus' */
#if EXTR_VARIANT_ACCELEROMETER_ENABLED

void Extraction_GetAccelBaselineStatus(ACCEL_BASELINE_STATUS_TYPE
    *rty_Accel_Baseline_Status)
{
    /* MATLAB Function: '<S40>/GetAccelBaselineStatusMacro' */
    /* MATLAB Function 'Extraction/Extr_Accel/Extr_Accel_Enabled/Extr_Accel_Enabled/Extr_Accel_Supervisor/GetAccelBaselineStatus/GetAccelBaselineStatusMacro': '<S50>:1' */
    // Retrieve the accelerometer baseline status.	();
    /* '<S50>:1:4' */
    *rty_Accel_Baseline_Status = EXTRACTION_GET_ACCEL_BASELINE_STATUS();
}

#endif                                 /* EXTR_VARIANT_ACCELEROMETER_ENABLED */

/* Output and update for Simulink Function: '<S30>/GetAccelStatus' */
#if EXTR_VARIANT_ACCELEROMETER_ENABLED

void Extraction_GetAccelStatus(ACCELEROMETER_STATUS_TYPE *rty_Accel_Status)
{
    /* MATLAB Function: '<S42>/GetAccelStatusMacro' */
    /* MATLAB Function 'Extraction/Extr_Accel/Extr_Accel_Enabled/Extr_Accel_Enabled/Extr_Accel_Supervisor/GetAccelStatus/GetAccelStatusMacro': '<S52>:1' */
    // Retrieve the accelerometer status.	();
    /* '<S52>:1:4' */
    *rty_Accel_Status = EXTRACTION_GET_ACCEL_STATUS();
}

#endif                                 /* EXTR_VARIANT_ACCELEROMETER_ENABLED */

/* Output and update for Simulink Function: '<S30>/MeasureBaseline' */
#if EXTR_VARIANT_ACCELEROMETER_ENABLED

void Extraction_MeasureBaseline(void)
{
    /* MATLAB Function: '<S43>/MeasureBaselineMacro' */
    /* MATLAB Function 'Extraction/Extr_Accel/Extr_Accel_Enabled/Extr_Accel_Enabled/Extr_Accel_Supervisor/MeasureBaseline/MeasureBaselineMacro': '<S53>:1' */
    // Retrieve the accelerometer baseline status.	();
    EXTRACTION_MEASURE_ACCEL_BASELINE();
}

#endif                                 /* EXTR_VARIANT_ACCELEROMETER_ENABLED */

/* Output and update for function-call system: '<S56>/CalcFloorDispThreshold' */
#if EXTR_VARIANT_ACCELEROMETER_ENABLED

void Extraction_CalcFloorDispThreshold(const MOTOR_OUTPUT_TYPE
    *rtu_motor_signals, const VIB_MAP_FLOOR_DISPL_THRESHOLD_TYPE
    *rtu_fd_threshold_setup, Extraction_B_CalcFloorDispThreshold_TYPE *localB)
{
    float32 rtb_Abs_fw;

    /* Abs: '<S58>/Abs' */
    rtb_Abs_fw = fabsf(rtu_fd_threshold_setup->Displacement_Threshold_B);

    /* Fcn: '<S58>/Fcn' */
    if (rtu_motor_signals->Speed < 0.0F && rtb_Abs_fw > floorf(rtb_Abs_fw))
    {
        rtb_Abs_fw = -powf(-rtu_motor_signals->Speed, rtb_Abs_fw);
    }
    else
    {
        rtb_Abs_fw = powf(rtu_motor_signals->Speed, rtb_Abs_fw);
    }

    /* End of Fcn: '<S58>/Fcn' */

    /* Sum: '<S58>/Sum' incorporates:
     *  Abs: '<S58>/Abs1'
     *  Abs: '<S58>/Abs2'
     *  Product: '<S58>/Product'
     */
    localB->Sum = rtb_Abs_fw * fabsf
        (rtu_fd_threshold_setup->Displacement_Threshold_A) + fabsf
        (rtu_fd_threshold_setup->Displacement_Threshold_C);
}

#endif                                 /* EXTR_VARIANT_ACCELEROMETER_ENABLED */

/* Function for Chart: '<S29>/Extr_Accel_CabHitLogic' */
#if EXTR_VARIANT_ACCELEROMETER_ENABLED

static void Extraction_RUNNING_nh(const boolean_T *CabHitEnabled_j, const sint32
    *sfEvent_j)
{
    boolean_T SoftThresholdCrossed_j;
    sint16 tmp_j;
    sint16 tmp_jb;
    sint16 tmp_jbh;
    sint32 tmp_jbhc;
    uint32 tmp_jbhc0;
    uint8 is_MONITORING_ig_j;
    CAB_HIT_STATE Extr_Accel_CabHitState_j;
    uint8 is_RUNNING_f3_j;
    uint8 Extr_Accel_SoftCabHitCounter_j;
    boolean_T HardThresholdCrossed_j;
    uint16 Extr_Accel_HardCabHitCounter_j;
    boolean_T CleanWasherThresholdCrossed_j;
    uint16 Extr_Accel_SoftCabHitCounterOverall_j;
    uint32 Extr_Accel_EvCleanWasherExceptionEventCounter_j;
    uint32 Extr_Accel_EvCabHitEventCounter_j;
    boolean_T guard1 = false;
    boolean_T guard2 = false;
    Extr_Accel_EvCabHitEventCounter_j =
        Extraction_DW.Extr_Accel_Enabled_gt.Extr_Accel_EvCabHitEventCounter;
    Extr_Accel_EvCleanWasherExceptionEventCounter_j =
        Extraction_DW.Extr_Accel_Enabled_gt.Extr_Accel_EvCleanWasherExceptionEventCounter;
    Extr_Accel_SoftCabHitCounterOverall_j = Extr_Accel_SoftCabHitCounterOverall;
    CleanWasherThresholdCrossed_j =
        Extraction_DW.Extr_Accel_Enabled_gt.CleanWasherThresholdCrossed;
    Extr_Accel_HardCabHitCounter_j = Extr_Accel_HardCabHitCounter;
    HardThresholdCrossed_j =
        Extraction_DW.Extr_Accel_Enabled_gt.HardThresholdCrossed;
    Extr_Accel_SoftCabHitCounter_j = Extr_Accel_SoftCabHitCounter;
    is_RUNNING_f3_j = Extraction_DW.Extr_Accel_Enabled_gt.is_RUNNING_f3;
    Extr_Accel_CabHitState_j = Extr_Accel_CabHitState;
    is_MONITORING_ig_j = Extraction_DW.Extr_Accel_Enabled_gt.is_MONITORING_ig;

    /* During 'RUNNING': '<S34>:8' */
    guard1 = false;
    guard2 = false;
    if (Extr_Accel_SoftCabHitCounter_j > (*GESE_Extr_CabHitSetup).
            Soft_Counter_Threshold)
    {
        /* Transition: '<S34>:161' */
        Extraction_SetFault(EXTR_FAULT_ACCEL_CAB_HIT_COUNT_EXCEEDED);
        guard2 = true;
    }
    else if (!*CabHitEnabled_j)
    {
        /* Transition: '<S34>:26' */
        guard1 = true;
    }
    else if (HardThresholdCrossed_j)
    {
        /* Transition: '<S34>:169' */
        tmp_jbhc0 = Extr_Accel_HardCabHitCounter_j + 1U;
        if (tmp_jbhc0 > 65535U)
        {
            tmp_jbhc0 = 65535U;
        }

        Extr_Accel_HardCabHitCounter_j = (uint16)tmp_jbhc0;
        Extraction_SetFault(EXTR_FAULT_ACCEL_CAB_HIT_DETECTED);

        /* Transition: '<S34>:170' */
        guard2 = true;
    }
    else if (CleanWasherThresholdCrossed_j)
    {
        /* Transition: '<S34>:178' */
        Extraction_SetFault(EXTR_FAULT_CLEAN_WASHER_ACCEL_EXCEEDED);

        /* Transition: '<S34>:179' */
        /* Event: '<S34>:180' */
        Extr_Accel_EvCleanWasherExceptionEventCounter_j++;
        guard1 = true;
    }
    else
    {
        if (*sfEvent_j == Extraction_event_Extr_Sup_EvResetSoftCabHitCnt)
        {
            Extr_Accel_SoftCabHitCounter_j = 0U;
        }

        switch (is_RUNNING_f3_j)
        {
          case Extraction_IN_FILTER_WARM_UP:
            /* During 'FILTER_WARM_UP': '<S34>:181' */
            tmp_jbhc0 = 3U * ((uint16)FS_EXTRACTION);
            if (tmp_jbhc0 > 65535U)
            {
                tmp_jbhc0 = 65535U;
            }

            if (*sfEvent_j == Extraction_event_Trigger_l4 &&
                    Extraction_DW.Extr_Accel_Enabled_gt.temporalCounter_i1_cz >=
                tmp_jbhc0)
            {
                /* Transition: '<S34>:183' */
                is_RUNNING_f3_j = Extraction_IN_MONITORING_SPEED;
                Extr_Accel_CabHitState_j = CAB_HIT_STATE_MONITORING_SPEED;
            }
            break;

          case Extraction_IN_MONITORING_co:
            /* During 'MONITORING': '<S34>:156' */
            if (fabsf(Extr_Inputs_MotorSignals.Speed) < (float32)
                    (*GESE_Extr_CabHitSetup).Detection_Start_Rpm_Div10 * 10.0F ||
                fabsf(Extr_Inputs_MotorSignals.Speed) > (float32)
                    (*GESE_Extr_CabHitSetup).Detection_Stop_Rpm_Div10 * 10.0F)
            {
                /* Transition: '<S34>:63' */
                /* Transition: '<S34>:185' */
                /* Exit Internal 'MONITORING': '<S34>:156' */
                is_MONITORING_ig_j = Extraction_IN_NO_ACTIVE_CHILD_pf;
                is_RUNNING_f3_j = Extraction_IN_MONITORING_SPEED;
                Extr_Accel_CabHitState_j = CAB_HIT_STATE_MONITORING_SPEED;
            }
            else
            {
                if ((*GESE_Extr_CabHitSetup).Soft_Detection_Threshold_X_x250 < 0)
                {
                    tmp_jbhc = -(*GESE_Extr_CabHitSetup).
                        Soft_Detection_Threshold_X_x250;
                    if (tmp_jbhc > 32767)
                    {
                        tmp_jbhc = 32767;
                    }

                    tmp_j = (sint16)tmp_jbhc;
                }
                else
                {
                    tmp_j = (*GESE_Extr_CabHitSetup).
                        Soft_Detection_Threshold_X_x250;
                }

                if ((*GESE_Extr_CabHitSetup).Soft_Detection_Threshold_Y_x250 < 0)
                {
                    tmp_jbhc = -(*GESE_Extr_CabHitSetup).
                        Soft_Detection_Threshold_Y_x250;
                    if (tmp_jbhc > 32767)
                    {
                        tmp_jbhc = 32767;
                    }

                    tmp_jb = (sint16)tmp_jbhc;
                }
                else
                {
                    tmp_jb = (*GESE_Extr_CabHitSetup).
                        Soft_Detection_Threshold_Y_x250;
                }

                if ((*GESE_Extr_CabHitSetup).Soft_Detection_Threshold_Z_x250 < 0)
                {
                    tmp_jbhc = -(*GESE_Extr_CabHitSetup).
                        Soft_Detection_Threshold_Z_x250;
                    if (tmp_jbhc > 32767)
                    {
                        tmp_jbhc = 32767;
                    }

                    tmp_jbh = (sint16)tmp_jbhc;
                }
                else
                {
                    tmp_jbh = (*GESE_Extr_CabHitSetup).
                        Soft_Detection_Threshold_Z_x250;
                }

                SoftThresholdCrossed_j = Extr_Accel_CabHitFiltAccel[0] >
                    (float32)tmp_j / 250.0F || Extr_Accel_CabHitFiltAccel[1] >
                    (float32)tmp_jb / 250.0F || Extr_Accel_CabHitFiltAccel[2] >
                    (float32)tmp_jbh / 250.0F;
                if ((*GESE_Extr_CabHitSetup).Hard_Detection_Threshold_X_x250 < 0)
                {
                    tmp_jbhc = -(*GESE_Extr_CabHitSetup).
                        Hard_Detection_Threshold_X_x250;
                    if (tmp_jbhc > 32767)
                    {
                        tmp_jbhc = 32767;
                    }

                    tmp_j = (sint16)tmp_jbhc;
                }
                else
                {
                    tmp_j = (*GESE_Extr_CabHitSetup).
                        Hard_Detection_Threshold_X_x250;
                }

                if ((*GESE_Extr_CabHitSetup).Hard_Detection_Threshold_Y_x250 < 0)
                {
                    tmp_jbhc = -(*GESE_Extr_CabHitSetup).
                        Hard_Detection_Threshold_Y_x250;
                    if (tmp_jbhc > 32767)
                    {
                        tmp_jbhc = 32767;
                    }

                    tmp_jb = (sint16)tmp_jbhc;
                }
                else
                {
                    tmp_jb = (*GESE_Extr_CabHitSetup).
                        Hard_Detection_Threshold_Y_x250;
                }

                if ((*GESE_Extr_CabHitSetup).Hard_Detection_Threshold_Z_x250 < 0)
                {
                    tmp_jbhc = -(*GESE_Extr_CabHitSetup).
                        Hard_Detection_Threshold_Z_x250;
                    if (tmp_jbhc > 32767)
                    {
                        tmp_jbhc = 32767;
                    }

                    tmp_jbh = (sint16)tmp_jbhc;
                }
                else
                {
                    tmp_jbh = (*GESE_Extr_CabHitSetup).
                        Hard_Detection_Threshold_Z_x250;
                }

                HardThresholdCrossed_j = Extr_Accel_CabHitFiltAccel[0] >
                    (float32)tmp_j / 250.0F || Extr_Accel_CabHitFiltAccel[1] >
                    (float32)tmp_jb / 250.0F || Extr_Accel_CabHitFiltAccel[2] >
                    (float32)tmp_jbh / 250.0F;
                if ((*GESE_Extr_Setup).CWLD_Enabled != 0)
                {
                    CleanWasherThresholdCrossed_j = Extr_Accel_CabHitFiltAccel[0]
                        > (float32)(*GESE_Extr_CleanWasherSetup).
                        Accel_Threshold_x250 / 250.0F ||
                        Extr_Accel_CabHitFiltAccel[1] > (float32)
                        (*GESE_Extr_CleanWasherSetup).Accel_Threshold_x250 /
                        250.0F || Extr_Accel_CabHitFiltAccel[2] > (float32)
                        (*GESE_Extr_CleanWasherSetup).Accel_Threshold_x250 /
                        250.0F;
                }

                if (is_MONITORING_ig_j == Extraction_IN_MONITORING_SOFT_CAB_HIT)
                {
                    /* During 'MONITORING_SOFT_CAB_HIT': '<S34>:56' */
                    /* % REQ 304198 - Calculate floor displacement
                       % REQ 292850 - Calculate floor displacement threshold
                       % REQ 309738 - Check if reference displacement has not been calculated
                       % REQ 304107 - Update vibration mapping reference displacement for the current spin pulse index */
                    if (SoftThresholdCrossed_j)
                    {
                        /* Transition: '<S34>:154' */
                        is_MONITORING_ig_j =
                            Extraction_IN_SOFT_THRESHOLD_CROSSED;
                        Extr_Accel_CabHitState_j =
                            CAB_HIT_STATE_SOFT_THRESHOLD_CROSSED;

                        /* Entry 'SOFT_THRESHOLD_CROSSED': '<S34>:153' */
                        tmp_jbhc = (sint32)(Extr_Accel_SoftCabHitCounter_j + 1U);
                        if ((uint32)tmp_jbhc > 255U)
                        {
                            tmp_jbhc = 255;
                        }

                        Extr_Accel_SoftCabHitCounter_j = (uint8)tmp_jbhc;
                        tmp_jbhc0 = Extr_Accel_SoftCabHitCounterOverall_j + 1U;
                        if (tmp_jbhc0 > 65535U)
                        {
                            tmp_jbhc0 = 65535U;
                        }

                        Extr_Accel_SoftCabHitCounterOverall_j = (uint16)
                            tmp_jbhc0;
                    }
                }
                else
                {
                    /* During 'SOFT_THRESHOLD_CROSSED': '<S34>:153' */
                    /* % REQ 304198 - Calculate floor displacement
                       % REQ 292850 - Calculate floor displacement threshold
                       % REQ 309738 - Check if reference displacement has not been calculated
                       % REQ 304107 - Update vibration mapping reference displacement for the current spin pulse index */
                    if (!SoftThresholdCrossed_j)
                    {
                        /* Transition: '<S34>:155' */
                        is_MONITORING_ig_j =
                            Extraction_IN_MONITORING_SOFT_CAB_HIT;
                        Extr_Accel_CabHitState_j =
                            CAB_HIT_STATE_MONITORING_SOFT_CAB_HIT;
                    }
                }
            }
            break;

          default:
            /* During 'MONITORING_SPEED': '<S34>:54' */
            if (fabsf(Extr_Inputs_MotorSignals.Speed) >= (float32)
                    (*GESE_Extr_CabHitSetup).Detection_Start_Rpm_Div10 * 10.0F &&
                fabsf(Extr_Inputs_MotorSignals.Speed) <= (float32)
                    (*GESE_Extr_CabHitSetup).Detection_Stop_Rpm_Div10 * 10.0F)
            {
                /* Transition: '<S34>:57' */
                is_RUNNING_f3_j = Extraction_IN_MONITORING_co;

                /* Entry 'MONITORING': '<S34>:156' */
                CleanWasherThresholdCrossed_j = false;
                if ((*GESE_Extr_CabHitSetup).Hard_Detection_Threshold_X_x250 < 0)
                {
                    tmp_jbhc = -(*GESE_Extr_CabHitSetup).
                        Hard_Detection_Threshold_X_x250;
                    if (tmp_jbhc > 32767)
                    {
                        tmp_jbhc = 32767;
                    }

                    tmp_j = (sint16)tmp_jbhc;
                }
                else
                {
                    tmp_j = (*GESE_Extr_CabHitSetup).
                        Hard_Detection_Threshold_X_x250;
                }

                if ((*GESE_Extr_CabHitSetup).Hard_Detection_Threshold_Y_x250 < 0)
                {
                    tmp_jbhc = -(*GESE_Extr_CabHitSetup).
                        Hard_Detection_Threshold_Y_x250;
                    if (tmp_jbhc > 32767)
                    {
                        tmp_jbhc = 32767;
                    }

                    tmp_jb = (sint16)tmp_jbhc;
                }
                else
                {
                    tmp_jb = (*GESE_Extr_CabHitSetup).
                        Hard_Detection_Threshold_Y_x250;
                }

                if ((*GESE_Extr_CabHitSetup).Hard_Detection_Threshold_Z_x250 < 0)
                {
                    tmp_jbhc = -(*GESE_Extr_CabHitSetup).
                        Hard_Detection_Threshold_Z_x250;
                    if (tmp_jbhc > 32767)
                    {
                        tmp_jbhc = 32767;
                    }

                    tmp_jbh = (sint16)tmp_jbhc;
                }
                else
                {
                    tmp_jbh = (*GESE_Extr_CabHitSetup).
                        Hard_Detection_Threshold_Z_x250;
                }

                HardThresholdCrossed_j = Extr_Accel_CabHitFiltAccel[0] >
                    (float32)tmp_j / 250.0F || Extr_Accel_CabHitFiltAccel[1] >
                    (float32)tmp_jb / 250.0F || Extr_Accel_CabHitFiltAccel[2] >
                    (float32)tmp_jbh / 250.0F;
                if ((*GESE_Extr_Setup).CWLD_Enabled != 0)
                {
                    CleanWasherThresholdCrossed_j = Extr_Accel_CabHitFiltAccel[0]
                        > (float32)(*GESE_Extr_CleanWasherSetup).
                        Accel_Threshold_x250 / 250.0F ||
                        Extr_Accel_CabHitFiltAccel[1] > (float32)
                        (*GESE_Extr_CleanWasherSetup).Accel_Threshold_x250 /
                        250.0F || Extr_Accel_CabHitFiltAccel[2] > (float32)
                        (*GESE_Extr_CleanWasherSetup).Accel_Threshold_x250 /
                        250.0F;
                }

                /* Entry Internal 'MONITORING': '<S34>:156' */
                /* Transition: '<S34>:157' */
                is_MONITORING_ig_j = Extraction_IN_MONITORING_SOFT_CAB_HIT;
                Extr_Accel_CabHitState_j = CAB_HIT_STATE_MONITORING_SOFT_CAB_HIT;
            }
            break;
        }
    }

    if (guard2)
    {
        /* Transition: '<S34>:171' */
        /* Event: '<S34>:162' */
        Extr_Accel_EvCabHitEventCounter_j++;
        guard1 = true;
    }

    if (guard1)
    {
        /* Exit Internal 'RUNNING': '<S34>:8' */
        is_RUNNING_f3_j = Extraction_IN_NO_ACTIVE_CHILD_pf;

        /* Exit Internal 'MONITORING': '<S34>:156' */
        is_MONITORING_ig_j = Extraction_IN_NO_ACTIVE_CHILD_pf;
        Extraction_DW.Extr_Accel_Enabled_gt.is_c33_Extraction =
            Extraction_IN_IDLE_ii;
        Extr_Accel_CabHitState_j = CAB_HIT_STATE_IDLE;

        /* Entry 'IDLE': '<S34>:2' */
        Extr_Accel_SoftCabHitCounter_j = 0U;
        HardThresholdCrossed_j = false;
        CleanWasherThresholdCrossed_j = false;
    }

    Extraction_DW.Extr_Accel_Enabled_gt.is_MONITORING_ig = is_MONITORING_ig_j;
    Extr_Accel_CabHitState = Extr_Accel_CabHitState_j;
    Extraction_DW.Extr_Accel_Enabled_gt.is_RUNNING_f3 = is_RUNNING_f3_j;
    Extr_Accel_SoftCabHitCounter = Extr_Accel_SoftCabHitCounter_j;
    Extraction_DW.Extr_Accel_Enabled_gt.HardThresholdCrossed =
        HardThresholdCrossed_j;
    Extr_Accel_HardCabHitCounter = Extr_Accel_HardCabHitCounter_j;
    Extraction_DW.Extr_Accel_Enabled_gt.CleanWasherThresholdCrossed =
        CleanWasherThresholdCrossed_j;
    Extr_Accel_SoftCabHitCounterOverall = Extr_Accel_SoftCabHitCounterOverall_j;
    Extraction_DW.Extr_Accel_Enabled_gt.Extr_Accel_EvCleanWasherExceptionEventCounter
        = Extr_Accel_EvCleanWasherExceptionEventCounter_j;
    Extraction_DW.Extr_Accel_Enabled_gt.Extr_Accel_EvCabHitEventCounter =
        Extr_Accel_EvCabHitEventCounter_j;
}

#endif                                 /* EXTR_VARIANT_ACCELEROMETER_ENABLED */

/* Function for Chart: '<S29>/Extr_Accel_CabHitLogic' */
#if EXTR_VARIANT_ACCELEROMETER_ENABLED

static void Extraction_chartstep_c33_Extraction(const boolean_T *CabHitEnabled_j,
    const sint32 *sfEvent_j)
{
    /* During: Extraction/Extr_Accel/Extr_Accel_Enabled/Extr_Accel_Enabled/Extr_Accel_CabHit/Extr_Accel_CabHitLogic */
    if (Extraction_DW.Extr_Accel_Enabled_gt.is_c33_Extraction ==
            Extraction_IN_IDLE_ii)
    {
        /* During 'IDLE': '<S34>:2' */
        if (*CabHitEnabled_j)
        {
            /* Transition: '<S34>:7' */
            /* Exit 'IDLE': '<S34>:2' */
            Extr_Accel_SoftCabHitCounter = 0U;
            Extraction_DW.Extr_Accel_Enabled_gt.HardThresholdCrossed = false;
            Extraction_DW.Extr_Accel_Enabled_gt.CleanWasherThresholdCrossed =
                false;
            Extraction_DW.Extr_Accel_Enabled_gt.is_c33_Extraction =
                Extraction_IN_RUNNING_iz;

            /* Entry Internal 'RUNNING': '<S34>:8' */
            /* Transition: '<S34>:182' */
            Extraction_DW.Extr_Accel_Enabled_gt.is_RUNNING_f3 =
                Extraction_IN_FILTER_WARM_UP;
            Extraction_DW.Extr_Accel_Enabled_gt.temporalCounter_i1_cz = 0U;
            Extr_Accel_CabHitState = CAB_HIT_STATE_FILTER_WARM_UP;
        }
    }
    else
    {
        Extraction_RUNNING_nh(CabHitEnabled_j, sfEvent_j);
    }
}

#endif                                 /* EXTR_VARIANT_ACCELEROMETER_ENABLED */

/* Function for Chart: '<S31>/Extr_Accel_VibMapLogic' */
#if EXTR_VARIANT_ACCELEROMETER_ENABLED

static float32 Extraction_CalcFloorDisplacement(const float32 accel_data_j[3],
    float32 floor_disp_setup_FD_Calc_Friction_Coeff_j, float32
    floor_disp_setup_FD_Calc_Velocity_Ref_Coeff_j, float32
    floor_disp_setup_FD_Calc_Velocity_Actual_Coeff_j, float32
    floor_disp_setup_FD_Calc_Accel_X_Coeff_j, float32
    floor_disp_setup_FD_Calc_Accel_Y_Coeff_j, float32
    floor_disp_setup_FD_Calc_Accel_XX_Coeff_j, float32
    floor_disp_setup_FD_Calc_Accel_YX_Coeff_j, float32
    floor_disp_setup_FD_Calc_Accel_ZX_Coeff_j, float32
    floor_disp_setup_FD_Calc_Accel_YY_Coeff_j, float32
    floor_disp_setup_FD_Calc_Accel_ZY_Coeff_j, float32
    floor_disp_setup_FD_Calc_Accel_ZZ_Coeff_j, float32
    floor_disp_setup_FD_Calc_Intercept_j, float32 pe_friction_j, float32
    motor_signals_Speed_j, float32 motor_signals_Speed_Ref_j)
{
    /* MATLAB Function 'CalcFloorDisplacement': '<S56>:46' */
    /*  304198: The control shall set VM_Floor_Displacement_mm equal to */
    /*  ( VM_FDCALC_INTERCEPT */
    /*  + VM_FDCALC_FRICTION_COEFF * Parameter_Estimation_Friction_Nm */
    /*  + VM_FDCALC_VELOCITY_REF_COEFF * Current Drum Target Velocity  */
    /*  + VM_FDCALC_VELOCITY_ACTUAL_COEFF * Current Drum Actual Velocity  */
    /*  + VM_FDCALC_ACCEL_X_COEFF * VM_Filtered_Accelerometer_X */
    /*  + VM_FDCALC_ACCEL_Y_COEFF * VM_Filtered_Accelerometer_Y */
    /*  + VM_FDCALC_ACCEL_Z_COEFF * VM_Filtered_Accelerometer_Z */
    /*  + VM_FDCALC_ACCEL_XX_COEFF * VM_Filtered_Accelerometer_X2 */
    /*  + VM_FDCALC_ACCEL_YX_COEFF * VM_Filtered_Accelerometer_Y * VM_Filtered_Accelerometer_X */
    /*  + VM_FDCALC_ACCEL_ZX_COEFF * VM_Filtered_Accelerometer_Z * VM_Filtered_Accelerometer_X */
    /*  + VM_FDCALC_ACCEL_YY_COEFF * VM_Filtered_Accelerometer_Y2 */
    /*  + VM_FDCALC_ACCEL_ZY_COEFF * VM_Filtered_Accelerometer_Z * VM_Filtered_Accelerometer_Y */
    /*  + VM_FDCALC_ACCEL_ZZ_COEFF * VM_Filtered_Accelerometer_Z2 ) */
    /* '<S56>:46:17' */
    return floor_disp_setup_FD_Calc_Intercept_j +
        floor_disp_setup_FD_Calc_Friction_Coeff_j * pe_friction_j +
        floor_disp_setup_FD_Calc_Velocity_Ref_Coeff_j *
        motor_signals_Speed_Ref_j +
        floor_disp_setup_FD_Calc_Velocity_Actual_Coeff_j * motor_signals_Speed_j
        + floor_disp_setup_FD_Calc_Accel_X_Coeff_j * accel_data_j[0] +
        floor_disp_setup_FD_Calc_Accel_Y_Coeff_j * accel_data_j[1] +
        floor_disp_setup_FD_Calc_Accel_XX_Coeff_j * (accel_data_j[0] *
        accel_data_j[0]) + floor_disp_setup_FD_Calc_Accel_YX_Coeff_j *
        accel_data_j[1] * accel_data_j[0] +
        floor_disp_setup_FD_Calc_Accel_ZX_Coeff_j * accel_data_j[2] *
        accel_data_j[0] + floor_disp_setup_FD_Calc_Accel_YY_Coeff_j *
        (accel_data_j[1] * accel_data_j[1]) +
        floor_disp_setup_FD_Calc_Accel_ZY_Coeff_j * accel_data_j[2] *
        accel_data_j[1] + floor_disp_setup_FD_Calc_Accel_ZZ_Coeff_j *
        (accel_data_j[2] * accel_data_j[2]);
}

#endif                                 /* EXTR_VARIANT_ACCELEROMETER_ENABLED */

/* Function for Chart: '<S31>/Extr_Accel_VibMapLogic' */
#if EXTR_VARIANT_ACCELEROMETER_ENABLED

static void Extraction_UpdateVibMapRefFloorDisp(uint8
    vib_map_setup_Percentage_Of_Current_j, uint8
    vib_map_setup_Reference_Update_Limit_x100_j, float32 floor_disp_j, uint8
    spin_pulse_Target_Speed_Div10_j)
{
    uint8 ii_j;
    uint32 qY_j;
    float32 tmp_j;
    uint16 tmp_jb;

    /* MATLAB Function 'UpdateVibMapRefFloorDisp': '<S56>:142' */
    /*  Search for existing ref speed and update the corresponding floor disp value */
    /* '<S56>:142:7' */
    for (ii_j = 1U; ii_j <= GESE_Extr_VibMapRefSpeedArray_div10.N; ii_j++)
    {
        /* '<S56>:142:7' */
        if (GESE_Extr_VibMapRefSpeedArray_div10.BP[ii_j - 1] ==
                spin_pulse_Target_Speed_Div10_j)
        {
            /* '<S56>:142:8' */
            if (ExtractionParams__GetVibMapRefDisp( ii_j - 1 ) == 65535)
            {
                /* '<S56>:142:9' */
                /* '<S56>:142:10' */
                tmp_j = roundf(100.0F * floor_disp_j);
                if (tmp_j < 65536.0F)
                {
                    if (tmp_j >= 0.0F)
                    {
                        ExtractionParams__SetVibMapRefDisp( ii_j - 1 , (uint16)
                            tmp_j);
                    }
                    else
                    {
                        ExtractionParams__SetVibMapRefDisp( ii_j - 1 , 0U);
                    }
                }
                else
                {
                    ExtractionParams__SetVibMapRefDisp( ii_j - 1 , MAX_uint16_T);
                }
            }
            else
            {
                tmp_j = roundf(floor_disp_j * 100.0F);
                if (tmp_j < 65536.0F)
                {
                    if (tmp_j >= 0.0F)
                    {
                        tmp_jb = (uint16)tmp_j;
                    }
                    else
                    {
                        tmp_jb = 0U;
                    }
                }
                else
                {
                    tmp_jb = MAX_uint16_T;
                }

                qY_j = (uint32)ExtractionParams__GetVibMapRefDisp( ii_j - 1 ) -
                    tmp_jb;
                if (qY_j > ExtractionParams__GetVibMapRefDisp( ii_j - 1 ))
                {
                    qY_j = 0U;
                }

                if ((sint32)qY_j <= vib_map_setup_Reference_Update_Limit_x100_j)
                {
                    /* '<S56>:142:12' */
                    /* '<S56>:142:14' */
                    tmp_j = roundf((float32)
                                   vib_map_setup_Percentage_Of_Current_j *
                                   (floor_disp_j - (float32)
                                    ExtractionParams__GetVibMapRefDisp( ii_j - 1
                                     ) / 100.0F));
                    if (tmp_j < 65536.0F)
                    {
                        if (tmp_j >= 0.0F)
                        {
                            tmp_jb = (uint16)tmp_j;
                        }
                        else
                        {
                            tmp_jb = 0U;
                        }
                    }
                    else
                    {
                        tmp_jb = MAX_uint16_T;
                    }

                    qY_j = (uint32)ExtractionParams__GetVibMapRefDisp( ii_j - 1 )
                        + tmp_jb;
                    if (qY_j > 65535U)
                    {
                        qY_j = 65535U;
                    }

                    ExtractionParams__SetVibMapRefDisp( ii_j - 1 , (uint16)qY_j);
                }
            }
        }
    }
}

#endif                                 /* EXTR_VARIANT_ACCELEROMETER_ENABLED */

/* Function for Chart: '<S31>/Extr_Accel_VibMapLogic' */
#if EXTR_VARIANT_ACCELEROMETER_ENABLED

static uint8 Extraction_CalcSafeSpeed(float32 th_cross_speed_j, uint8
    vib_map_setup_Max_Delta_Rpm_Div10_j, const uint16 floor_disp_array_x100_j[25],
    uint8 speed_ref_array_div10_N_j, const uint8 speed_ref_array_div10_BP_j[25],
    uint8 spin_pulse_Target_Speed_Div10_j)
{
    uint8 speed_div10_j;
    uint16 min_disp_j;
    uint8 ii_j;
    float32 tmp_j;
    uint32 qY_j;

    /* MATLAB Function 'CalcSafeSpeed': '<S56>:127' */
    /* '<S56>:127:3' */
    min_disp_j = MAX_uint16_T;

    /* '<S56>:127:4' */
    speed_div10_j = spin_pulse_Target_Speed_Div10_j;

    /* '<S56>:127:5' */
    qY_j = (uint32)spin_pulse_Target_Speed_Div10_j -
        vib_map_setup_Max_Delta_Rpm_Div10_j;
    if (qY_j > spin_pulse_Target_Speed_Div10_j)
    {
        qY_j = 0U;
    }

    if (fabsf((float32)spin_pulse_Target_Speed_Div10_j * 10.0F -
              th_cross_speed_j) <= (float32)vib_map_setup_Max_Delta_Rpm_Div10_j *
        10.0F)
    {
        /* '<S56>:127:7' */
        /* '<S56>:127:8' */
        tmp_j = roundf(th_cross_speed_j / 10.0F);
        if (tmp_j < 256.0F)
        {
            if (tmp_j >= 0.0F)
            {
                speed_div10_j = (uint8)tmp_j;
            }
            else
            {
                speed_div10_j = 0U;
            }
        }
        else
        {
            speed_div10_j = MAX_uint8_T;
        }
    }
    else
    {
        /*     %% Search for minimum displacement among valid speeds > target speed - max delta speed */
        /* '<S56>:127:11' */
        for (ii_j = 1U; ii_j <= speed_ref_array_div10_N_j; ii_j++)
        {
            /* '<S56>:127:11' */
            if (speed_ref_array_div10_BP_j[ii_j - 1] != 255 &&
                    floor_disp_array_x100_j[ii_j - 1] != 65535 &&
                    speed_ref_array_div10_BP_j[ii_j - 1] >= (uint8)qY_j &&
                    floor_disp_array_x100_j[ii_j - 1] < min_disp_j)
            {
                /* '<S56>:127:12' */
                /* '<S56>:127:13' */
                /* '<S56>:127:14' */
                min_disp_j = floor_disp_array_x100_j[ii_j - 1];

                /* '<S56>:127:15' */
                speed_div10_j = speed_ref_array_div10_BP_j[ii_j - 1];
            }
        }
    }

    return speed_div10_j;
}

#endif                                 /* EXTR_VARIANT_ACCELEROMETER_ENABLED */

/* Function for Chart: '<S31>/Extr_Accel_VibMapLogic' */
#if EXTR_VARIANT_ACCELEROMETER_ENABLED

static uint16 Extraction_CalcPlateauTimeIncrement(uint8 target_speed_div10_j,
    uint8 safe_speed_div10_j, sint8 vib_map_setup_Plateau_Duration_Ext_M_x10_j,
    sint8 vib_map_setup_Plateau_Duration_Ext_B_Div10_j)
{
    uint16 extra_time_j;
    float32 tmp_j;
    uint32 qY_j;

    /* MATLAB Function 'CalcPlateauTimeIncrement': '<S56>:137' */
    /* '<S56>:137:3' */
    qY_j = (uint32)target_speed_div10_j - safe_speed_div10_j;
    if (qY_j > target_speed_div10_j)
    {
        qY_j = 0U;
    }

    tmp_j = roundf(10.0F * (float32)(sint32)qY_j * ((float32)
                    vib_map_setup_Plateau_Duration_Ext_M_x10_j / 10.0F) +
                   (float32)vib_map_setup_Plateau_Duration_Ext_B_Div10_j * 10.0F);
    if (tmp_j >= 0.0F)
    {
        extra_time_j = (uint16)tmp_j;
    }
    else
    {
        extra_time_j = 0U;
    }

    return extra_time_j;
}

#endif                                 /* EXTR_VARIANT_ACCELEROMETER_ENABLED */

/* Output and update for Simulink Function: '<Root>/ClearFault' */
void Extraction__ClearFault(void)
{
    /* DataStoreWrite: '<S1>/ExtrFaultOut' incorporates:
     *  Constant: '<S1>/NULL'
     */
    Extr_Sup_Fault = 0U;
}

/* Output and update for Simulink Function: '<Root>/GetFault' */
uint32 Extraction__GetFault(void)
{
    /* SignalConversion: '<S3>/TmpSignal ConversionAtfaultInport1' incorporates:
     *  DataStoreRead: '<S3>/ExtrFault'
     */
    return Extr_Sup_Fault;
}

/* Output and update for Simulink Function: '<Root>/GetFloat' */
float32 Extraction__GetFloat(EXTR_FLOAT_FB_ENUM index)
{
    float32 rty_ret_value_0;

    /* SwitchCase: '<S4>/Switch Case' incorporates:
     *  SignalConversion: '<S4>/TmpSignal ConversionAtindexOutport1'
     */
    switch (index)
    {
      case EXTR_FLOAT_FB_ENUM_LOAD_MASS_LOW_SPEED:
        /* Outputs for IfAction SubSystem: '<S4>/If Action Subsystem' incorporates:
         *  ActionPort: '<S265>/Action Port'
         */
        /* SignalConversion: '<S4>/TmpSignal ConversionAtmassInport1' incorporates:
         *  DataStoreRead: '<S265>/Data Store Read'
         */
        rty_ret_value_0 = Extr_LowSpd_LoadMass;

        /* End of Outputs for SubSystem: '<S4>/If Action Subsystem' */
        break;

      case EXTR_FLOAT_FB_ENUM_LOAD_MASS_HIGH_SPEED:
        /* Outputs for IfAction SubSystem: '<S4>/If Action Subsystem1' incorporates:
         *  ActionPort: '<S266>/Action Port'
         */
        /* SignalConversion: '<S4>/TmpSignal ConversionAtmassInport1' incorporates:
         *  DataStoreRead: '<S266>/Data Store Read'
         */
        rty_ret_value_0 = Extr_Sup_LoadMass;

        /* End of Outputs for SubSystem: '<S4>/If Action Subsystem1' */
        break;

      case EXTR_FLOAT_FB_ENUM_UNBALANCE_LOW_SPEED:
        /* Outputs for IfAction SubSystem: '<S4>/If Action Subsystem3' incorporates:
         *  ActionPort: '<S268>/Action Port'
         */
        /* SignalConversion: '<S4>/TmpSignal ConversionAtmassInport1' incorporates:
         *  DataTypeConversion: '<S268>/Data Type Conversion'
         */
        rty_ret_value_0 = Extr_LowSpd_Unbalance;

        /* End of Outputs for SubSystem: '<S4>/If Action Subsystem3' */
        break;

      case EXTR_FLOAT_FB_ENUM_FRICTION_HIGH_SPEED:
        /* Outputs for IfAction SubSystem: '<S4>/If Action Subsystem4' incorporates:
         *  ActionPort: '<S269>/Action Port'
         */
        /* SignalConversion: '<S4>/TmpSignal ConversionAtmassInport1' incorporates:
         *  DataStoreRead: '<S269>/Data Store Read'
         */
        rty_ret_value_0 = Extr_HighSpd_Friction;

        /* End of Outputs for SubSystem: '<S4>/If Action Subsystem4' */
        break;

      case EXTR_FLOAT_FB_ENUM_VISCOUS_FRICTION_HIGH_SPEED:
        /* Outputs for IfAction SubSystem: '<S4>/If Action Subsystem5' incorporates:
         *  ActionPort: '<S270>/Action Port'
         */
        /* SignalConversion: '<S4>/TmpSignal ConversionAtmassInport1' incorporates:
         *  DataStoreRead: '<S270>/Data Store Read'
         */
        rty_ret_value_0 = Extr_HighSpd_ViscousFriction;

        /* End of Outputs for SubSystem: '<S4>/If Action Subsystem5' */
        break;

      case EXTR_FLOAT_FB_ENUM_MAX_CABINET_ACCELERATION:
        /* Outputs for IfAction SubSystem: '<S4>/If Action Subsystem6' incorporates:
         *  ActionPort: '<S271>/Action Port'
         */
        /* SignalConversion: '<S4>/TmpSignal ConversionAtmassInport1' incorporates:
         *  DataStoreRead: '<S271>/Data Store Read'
         */
        rty_ret_value_0 = Extr_Accel_MaxAccel;

        /* End of Outputs for SubSystem: '<S4>/If Action Subsystem6' */
        break;

      case EXTR_FLOAT_FB_ENUM_SPEED_AT_MAX_CABINET_ACCELERATION:
        /* Outputs for IfAction SubSystem: '<S4>/If Action Subsystem7' incorporates:
         *  ActionPort: '<S272>/Action Port'
         */
        /* SignalConversion: '<S4>/TmpSignal ConversionAtmassInport1' incorporates:
         *  DataStoreRead: '<S272>/Data Store Read'
         */
        rty_ret_value_0 = Extr_Accel_MaxAccelSpeed;

        /* End of Outputs for SubSystem: '<S4>/If Action Subsystem7' */
        break;

      case EXTR_FLOAT_FB_ENUM_MAX_FLOOR_DISPLACEMENT:
        /* Outputs for IfAction SubSystem: '<S4>/If Action Subsystem8' incorporates:
         *  ActionPort: '<S273>/Action Port'
         */
        /* SignalConversion: '<S4>/TmpSignal ConversionAtmassInport1' incorporates:
         *  DataStoreRead: '<S273>/Data Store Read'
         */
        rty_ret_value_0 = Extr_Accel_MaxVibMapFloorDisp;

        /* End of Outputs for SubSystem: '<S4>/If Action Subsystem8' */
        break;

      default:
        /* Outputs for IfAction SubSystem: '<S4>/If Action Subsystem2' incorporates:
         *  ActionPort: '<S267>/Action Port'
         */
        /* SignalConversion: '<S4>/TmpSignal ConversionAtmassInport1' incorporates:
         *  Constant: '<S267>/Constant'
         *  SignalConversion: '<S267>/OutportBufferForret_value'
         */
        rty_ret_value_0 = 0.0F;

        /* End of Outputs for SubSystem: '<S4>/If Action Subsystem2' */
        break;
    }

    /* End of SwitchCase: '<S4>/Switch Case' */
    return rty_ret_value_0;
}

/* Output and update for Simulink Function: '<Root>/GetInteger' */
sint32 Extraction__GetInteger(EXTR_INT32_FB_ENUM index)
{
    sint32 rty_ret_value_0;

    /* SwitchCase: '<S5>/Switch Case' incorporates:
     *  SignalConversion: '<S5>/TmpSignal ConversionAtindexOutport1'
     */
    switch (index)
    {
      case EXTR_INT32_FB_ENUM_REBAL_CNT_LOW_SPEED:
        /* Outputs for IfAction SubSystem: '<S5>/If Action Subsystem' incorporates:
         *  ActionPort: '<S274>/Action Port'
         */
        /* SignalConversion: '<S5>/TmpSignal ConversionAtret_valueInport1' incorporates:
         *  DataTypeConversion: '<S274>/sint32'
         */
        rty_ret_value_0 = Extr_LowSpd_RebalCnt;

        /* End of Outputs for SubSystem: '<S5>/If Action Subsystem' */
        break;

      case EXTR_INT32_FB_ENUM_REBAL_CNT_HIGH_SPEED:
        /* Outputs for IfAction SubSystem: '<S5>/If Action Subsystem1' incorporates:
         *  ActionPort: '<S275>/Action Port'
         */
        /* SignalConversion: '<S5>/TmpSignal ConversionAtret_valueInport1' incorporates:
         *  DataTypeConversion: '<S275>/sint32'
         */
        rty_ret_value_0 = Extr_HighSpd_RebalCnt;

        /* End of Outputs for SubSystem: '<S5>/If Action Subsystem1' */
        break;

      case EXTR_INT32_FB_ENUM_CEM_EVENT_COUNT:
        /* Outputs for IfAction SubSystem: '<S5>/If Action Subsystem3' incorporates:
         *  ActionPort: '<S277>/Action Port'
         */
        /* SignalConversion: '<S5>/TmpSignal ConversionAtret_valueInport1' incorporates:
         *  DataTypeConversion: '<S277>/sint32'
         */
        rty_ret_value_0 = Extr_HighSpd_CEMEventCounter;

        /* End of Outputs for SubSystem: '<S5>/If Action Subsystem3' */
        break;

      case EXTR_INT32_FB_ENUM_SOFT_CAB_HIT_COUNT:
        /* Outputs for IfAction SubSystem: '<S5>/If Action Subsystem4' incorporates:
         *  ActionPort: '<S278>/Action Port'
         */
        /* SignalConversion: '<S5>/TmpSignal ConversionAtret_valueInport1' incorporates:
         *  DataTypeConversion: '<S278>/sint32'
         */
        rty_ret_value_0 = Extr_Accel_SoftCabHitCounterOverall;

        /* End of Outputs for SubSystem: '<S5>/If Action Subsystem4' */
        break;

      case EXTR_INT32_FB_ENUM_HARD_CAB_HIT_COUNT:
        /* Outputs for IfAction SubSystem: '<S5>/If Action Subsystem5' incorporates:
         *  ActionPort: '<S279>/Action Port'
         */
        /* SignalConversion: '<S5>/TmpSignal ConversionAtret_valueInport1' incorporates:
         *  DataTypeConversion: '<S279>/sint32'
         */
        rty_ret_value_0 = Extr_Accel_HardCabHitCounter;

        /* End of Outputs for SubSystem: '<S5>/If Action Subsystem5' */
        break;

      case EXTR_INT32_FB_ENUM_CURRENT_SPIN_PULSE_INDEX:
        /* Outputs for IfAction SubSystem: '<S5>/If Action Subsystem6' incorporates:
         *  ActionPort: '<S280>/Action Port'
         */
        /* SignalConversion: '<S5>/TmpSignal ConversionAtret_valueInport1' incorporates:
         *  DataTypeConversion: '<S280>/sint32'
         */
        rty_ret_value_0 = Extr_HighSpd_CurrPulseInd;

        /* End of Outputs for SubSystem: '<S5>/If Action Subsystem6' */
        break;

      case EXTR_INT32_FB_ENUM_HIGH_SPEED_STATUS:
        /* Outputs for IfAction SubSystem: '<S5>/If Action Subsystem7' incorporates:
         *  ActionPort: '<S281>/Action Port'
         */
        /* SignalConversion: '<S5>/TmpSignal ConversionAtret_valueInport1' incorporates:
         *  DataTypeConversion: '<S281>/sint32'
         */
        rty_ret_value_0 = Extr_HighSpd_State;

        /* End of Outputs for SubSystem: '<S5>/If Action Subsystem7' */
        break;

      default:
        /* Outputs for IfAction SubSystem: '<S5>/If Action Subsystem2' incorporates:
         *  ActionPort: '<S276>/Action Port'
         */
        /* SignalConversion: '<S5>/TmpSignal ConversionAtret_valueInport1' incorporates:
         *  Constant: '<S276>/Constant'
         *  SignalConversion: '<S276>/OutportBufferForret_value'
         */
        rty_ret_value_0 = 0;

        /* End of Outputs for SubSystem: '<S5>/If Action Subsystem2' */
        break;
    }

    /* End of SwitchCase: '<S5>/Switch Case' */
    return rty_ret_value_0;
}

/* Output and update for Simulink Function: '<Root>/GetState' */
EXTRACTION_STATE Extraction__GetState(void)
{
    /* SignalConversion: '<S6>/TmpSignal ConversionAtstateInport1' */
    return Extr_Sup_State;
}

/* Output and update for Simulink Function: '<Root>/Initialize' */
void Extraction__Initialize(void)
{
    /* MATLAB Function: '<S7>/ExtractionInitialize' */
    /* MATLAB Function 'Initialize/ExtractionInitialize': '<S282>:1' */
    ClearInernalStates();
    Extraction_initialize();
}

/* Output and update for Simulink Function: '<Root>/IsRebalInProgress' */
void Extraction__IsRebInProgress(boolean_T *rty_rebal)
{
    /* SignalConversion: '<S8>/TmpSignal ConversionAtrebalInport1' */
    *rty_rebal = Extr_Rebal_RebInProgress;
}

/* Function for Chart: '<S193>/Extr_Sup_Logic' */
static void Extraction_RUNNING_oa(const sint32 *sfEvent_ha)
{
    uint32 tmp;

    /* During 'RUNNING': '<S201>:41' */
    Extraction_GetMciFault(&tmp);
    if (tmp != 0U)
    {
        /* Transition: '<S201>:140' */
        Extraction_DW.FaultDetected = true;
        Extraction_SetFault(EXTR_FAULT_MCI_FAULT_DETECTED);

        /* Transition: '<S201>:206' */
        /* Exit Internal 'RUNNING': '<S201>:41' */
        switch (Extraction_DW.is_RUNNING_kz)
        {
          case Extraction_IN_EXTRACTION_gg:
            /* Exit Internal 'EXTRACTION': '<S201>:6' */
            Extraction_DW.is_EXTRACTION_hb = 0U;

            /* Exit 'EXTRACTION': '<S201>:6' */
            Extraction_ParamEstControl(PE_STOP);
            Extraction_ParamEstExcControl(EXC_DISABLE);
            Extraction_DW.is_RUNNING_kz = 0U;
            break;

          case Extraction_IN_REBALANCE_hg:
            /* Exit 'REBALANCE': '<S201>:36' */
            Extraction_DW.is_RUNNING_kz = 0U;
            break;

          default:
            Extraction_DW.is_RUNNING_kz = 0U;
            break;
        }

        /* Exit 'RUNNING': '<S201>:41' */
        /* Event: '<S201>:54' */
        Extraction_DW.Extr_Sup_EvStopAllEventCounter++;

        /* Event: '<S201>:72' */
        Extraction_DW.Extr_Sup_ResetRebalCntEventCounter++;
        Extraction_B.Extr_Sup_CabHitEnabled = false;
        Extraction_B.Extr_Sup_VibMapEnabled = false;
        Extraction_DW.is_EXTRACTION = Extraction_IN_MOTOR_STOPPING;
        Extraction_B.Extr_Sup_State_d3 = EXTRACTION_STATE_MOTOR_STOPPING;

        /* Entry 'MOTOR_STOPPING': '<S201>:141' */
        Extraction_StopMotor();

        /* Outputs for Function Call SubSystem: '<S2>/Extr_UpdateInputs' */

        /* Event: '<S201>:103' */
        Extraction_Extr_UpdateInputs();

        /* End of Outputs for SubSystem: '<S2>/Extr_UpdateInputs' */
    }
    else
    {
        /* Outputs for Function Call SubSystem: '<S2>/Extr_UpdateInputs' */

        /* Event: '<S201>:103' */
        Extraction_Extr_UpdateInputs();

        /* End of Outputs for SubSystem: '<S2>/Extr_UpdateInputs' */
        if (*sfEvent_ha == Extraction_event_ACU_NotifFoamDetec)
        {
            /* Transition: '<S201>:95' */
            /* Exit Internal 'RUNNING': '<S201>:41' */
            switch (Extraction_DW.is_RUNNING_kz)
            {
              case Extraction_IN_EXTRACTION_gg:
                /* Exit Internal 'EXTRACTION': '<S201>:6' */
                Extraction_DW.is_EXTRACTION_hb = 0U;

                /* Exit 'EXTRACTION': '<S201>:6' */
                Extraction_ParamEstControl(PE_STOP);
                Extraction_ParamEstExcControl(EXC_DISABLE);
                Extraction_DW.is_RUNNING_kz = 0U;
                break;

              case Extraction_IN_REBALANCE_hg:
                /* Exit 'REBALANCE': '<S201>:36' */
                Extraction_DW.is_RUNNING_kz = 0U;
                break;

              default:
                Extraction_DW.is_RUNNING_kz = 0U;
                break;
            }

            /* Event: '<S201>:96' */
            Extraction_DW.Extr_Sup_EvFoamDetectedEventCounter++;
            Extraction_SetFault(EXTR_FAULT_ANTIFOAM_NOTIFICATION);
            Extraction_DW.is_RUNNING_kz = Extraction_IN_EXECUTING_ANTIFOAM;
            Extraction_B.Extr_Sup_State_d3 = EXTRACTION_STATE_EXECUTING_ANTIFOAM;
        }
        else
        {
            switch (Extraction_DW.is_RUNNING_kz)
            {
              case Extraction_IN_EXECUTING_ANTIFOAM:
                /* During 'EXECUTING_ANTIFOAM': '<S201>:97' */
                if (*sfEvent_ha == Extraction_event_ACU_ExtrResume)
                {
                    /* Transition: '<S201>:102' */
                    Extraction_DW.is_RUNNING_kz = Extraction_IN_EXTRACTION_gg;

                    /* Entry Internal 'EXTRACTION': '<S201>:6' */
                    /* Transition: '<S201>:16' */
                    Extraction_DW.is_EXTRACTION_hb = Extraction_IN_LOW_SPEED;
                    Extraction_B.Extr_Sup_State_d3 = EXTRACTION_STATE_LOW_SPEED;

                    /* Entry 'LOW_SPEED': '<S201>:15' */
                    /* Event: '<S201>:183' */
                    Extraction_DW.Extr_Sup_EvResetSoftCabHitCntEventCounter++;

                    /* Event: '<S201>:11' */
                    Extraction_DW.Extr_Sup_EvLowSpdStartEventCounter++;
                }
                break;

              case Extraction_IN_EXTRACTION_gg:
                /* During 'EXTRACTION': '<S201>:6' */
                if (*sfEvent_ha == Extraction_event_Extr_LowSpd_EvRebalStart || *
                        sfEvent_ha == Extraction_event_Extr_HighSpd_EvRebalStart
                    || *sfEvent_ha ==
                        Extraction_event_Extr_Rebal_EvShortRedistExceeded ||
                        *sfEvent_ha ==
                        Extraction_event_Extr_Rebal_P2PTorqueFault)
                {
                    /* Transition: '<S201>:37' */
                    /* Exit Internal 'EXTRACTION': '<S201>:6' */
                    Extraction_DW.is_EXTRACTION_hb = 0U;

                    /* Exit 'EXTRACTION': '<S201>:6' */
                    Extraction_ParamEstControl(PE_STOP);
                    Extraction_ParamEstExcControl(EXC_DISABLE);
                    Extraction_DW.is_RUNNING_kz = Extraction_IN_REBALANCE_hg;
                    Extraction_B.Extr_Sup_State_d3 = EXTRACTION_STATE_REBALANCE;

                    /* Entry 'REBALANCE': '<S201>:36' */
                    /* Event: '<S201>:60' */
                    Extraction_DW.Extr_Sup_EvRebalStartEventCounter++;
                }
                else if (Extraction_DW.is_EXTRACTION_hb ==
                         Extraction_IN_HIGH_SPEED)
                {
                    /* During 'HIGH_SPEED': '<S201>:19' */
                    if (*sfEvent_ha == Extraction_event_Extr_HighSpd_EvComplete)
                    {
                        /* Transition: '<S201>:26' */
                        /* Transition: '<S201>:125' */
                        Extraction_DW.ExtrComplete = true;
                        if ((*GESE_Extr_Setup).ESM_Disabled != 0)
                        {
                            /* Transition: '<S201>:186' */
                            /*  true = Keep the motor running when extraction ends */
                            /* Transition: '<S201>:190' */
                            if (Extraction_DW.FaultDetected)
                            {
                                /* Transition: '<S201>:152' */
                                Extraction_DW.is_EXTRACTION_hb = 0U;

                                /* Exit 'EXTRACTION': '<S201>:6' */
                                Extraction_ParamEstControl(PE_STOP);
                                Extraction_ParamEstExcControl(EXC_DISABLE);
                                Extraction_DW.is_RUNNING_kz = 0U;

                                /* Exit 'RUNNING': '<S201>:41' */
                                /* Event: '<S201>:54' */
                                Extraction_DW.Extr_Sup_EvStopAllEventCounter++;

                                /* Event: '<S201>:72' */
                                Extraction_DW.Extr_Sup_ResetRebalCntEventCounter
                                    ++;
                                Extraction_B.Extr_Sup_CabHitEnabled = false;
                                Extraction_B.Extr_Sup_VibMapEnabled = false;
                                Extraction_DW.is_EXTRACTION =
                                    Extraction_IN_CONFIGURING;
                                Extraction_B.Extr_Sup_State_d3 =
                                    EXTRACTION_STATE_CONFIGURING;

                                /* Entry 'CONFIGURING': '<S201>:122' */
                                Extraction__ClearFault();

                                /* Event: '<S201>:120' */
                                Extraction_DW.Extr_Sup_ClearFaultEventCounter++;

                                /* Event: '<S201>:72' */
                                Extraction_DW.Extr_Sup_ResetRebalCntEventCounter
                                    ++;
                                if (((boolean_T)EXTR_FEATURE_ACCEL_ENABLED))
                                {
                                    Extraction_B.Extr_Sup_CabHitEnabled = true;
                                    Extraction_B.Extr_Sup_VibMapEnabled = true;
                                }

                                /* Outputs for Function Call SubSystem: '<S2>/Extr_UpdateInputs' */

                                /* Event: '<S201>:103' */
                                Extraction_Extr_UpdateInputs();

                                /* End of Outputs for SubSystem: '<S2>/Extr_UpdateInputs' */
                            }
                            else
                            {
                                /* Transition: '<S201>:153' */
                                if (Extraction_DW.ExtrComplete)
                                {
                                    /* Transition: '<S201>:145' */
                                    Extraction_DW.is_EXTRACTION_hb = 0U;

                                    /* Exit 'EXTRACTION': '<S201>:6' */
                                    Extraction_ParamEstControl(PE_STOP);
                                    Extraction_ParamEstExcControl(EXC_DISABLE);
                                    Extraction_DW.is_RUNNING_kz = 0U;

                                    /* Exit 'RUNNING': '<S201>:41' */
                                    /* Event: '<S201>:54' */
                                    Extraction_DW.Extr_Sup_EvStopAllEventCounter
                                        ++;

                                    /* Event: '<S201>:72' */
                                    Extraction_DW.Extr_Sup_ResetRebalCntEventCounter
                                        ++;
                                    Extraction_B.Extr_Sup_CabHitEnabled = false;
                                    Extraction_B.Extr_Sup_VibMapEnabled = false;
                                    Extraction_DW.is_EXTRACTION =
                                        Extraction_IN_EXTRACTION_COMPLETE;
                                    Extraction_B.Extr_Sup_State_d3 =
                                        EXTRACTION_STATE_EXTRACTION_COMPLETE;
                                }
                                else
                                {
                                    /* Transition: '<S201>:144' */
                                    /* Transition: '<S201>:158' */
                                    Extraction_DW.is_EXTRACTION_hb = 0U;

                                    /* Exit 'EXTRACTION': '<S201>:6' */
                                    Extraction_ParamEstControl(PE_STOP);
                                    Extraction_ParamEstExcControl(EXC_DISABLE);
                                    Extraction_DW.is_RUNNING_kz = 0U;

                                    /* Exit 'RUNNING': '<S201>:41' */
                                    /* Event: '<S201>:54' */
                                    Extraction_DW.Extr_Sup_EvStopAllEventCounter
                                        ++;

                                    /* Event: '<S201>:72' */
                                    Extraction_DW.Extr_Sup_ResetRebalCntEventCounter
                                        ++;
                                    Extraction_B.Extr_Sup_CabHitEnabled = false;
                                    Extraction_B.Extr_Sup_VibMapEnabled = false;
                                    Extraction_DW.is_EXTRACTION =
                                        Extraction_IN_IDLE_kf;
                                    Extraction_B.Extr_Sup_State_d3 =
                                        EXTRACTION_STATE_IDLE;
                                }
                            }
                        }
                        else
                        {
                            /* Transition: '<S201>:205' */
                            /* Transition: '<S201>:206' */
                            Extraction_DW.is_EXTRACTION_hb = 0U;

                            /* Exit 'EXTRACTION': '<S201>:6' */
                            Extraction_ParamEstControl(PE_STOP);
                            Extraction_ParamEstExcControl(EXC_DISABLE);
                            Extraction_DW.is_RUNNING_kz = 0U;

                            /* Exit 'RUNNING': '<S201>:41' */
                            /* Event: '<S201>:54' */
                            Extraction_DW.Extr_Sup_EvStopAllEventCounter++;

                            /* Event: '<S201>:72' */
                            Extraction_DW.Extr_Sup_ResetRebalCntEventCounter++;
                            Extraction_B.Extr_Sup_CabHitEnabled = false;
                            Extraction_B.Extr_Sup_VibMapEnabled = false;
                            Extraction_DW.is_EXTRACTION =
                                Extraction_IN_MOTOR_STOPPING;
                            Extraction_B.Extr_Sup_State_d3 =
                                EXTRACTION_STATE_MOTOR_STOPPING;

                            /* Entry 'MOTOR_STOPPING': '<S201>:141' */
                            Extraction_StopMotor();

                            /* Outputs for Function Call SubSystem: '<S2>/Extr_UpdateInputs' */

                            /* Event: '<S201>:103' */
                            Extraction_Extr_UpdateInputs();

                            /* End of Outputs for SubSystem: '<S2>/Extr_UpdateInputs' */
                        }
                    }
                }
                else
                {
                    /* During 'LOW_SPEED': '<S201>:15' */
                    if (*sfEvent_ha ==
                            Extraction_event_Extr_LowSpd_EvComplete_jg)
                    {
                        /* Transition: '<S201>:18' */
                        Extraction_DW.is_EXTRACTION_hb =
                            Extraction_IN_HIGH_SPEED;
                        Extraction_B.Extr_Sup_State_d3 =
                            EXTRACTION_STATE_HIGH_SPEED;

                        /* Entry 'HIGH_SPEED': '<S201>:19' */
                        Extraction_ResetInitialPulseConfig();

                        /* Event: '<S201>:22' */
                        Extraction_DW.Extr_Sup_EvHighSpdStartEventCounter++;
                    }
                }
                break;

              default:
                /* During 'REBALANCE': '<S201>:36' */
                if (*sfEvent_ha == Extraction_event_Extr_Rebal_EvComplete)
                {
                    /* Transition: '<S201>:38' */
                    /* Exit 'REBALANCE': '<S201>:36' */
                    Extraction_DW.is_RUNNING_kz = Extraction_IN_EXTRACTION_gg;

                    /* Entry Internal 'EXTRACTION': '<S201>:6' */
                    /* Transition: '<S201>:16' */
                    Extraction_DW.is_EXTRACTION_hb = Extraction_IN_LOW_SPEED;
                    Extraction_B.Extr_Sup_State_d3 = EXTRACTION_STATE_LOW_SPEED;

                    /* Entry 'LOW_SPEED': '<S201>:15' */
                    /* Event: '<S201>:183' */
                    Extraction_DW.Extr_Sup_EvResetSoftCabHitCntEventCounter++;

                    /* Event: '<S201>:11' */
                    Extraction_DW.Extr_Sup_EvLowSpdStartEventCounter++;
                }
                break;
            }
        }
    }
}

/* Function for Chart: '<S193>/Extr_Sup_Logic' */
static void Extraction_exit_internal_EXTRACTION(void)
{
    uint8 is_RUNNING_kz;
    uint32 Extr_Sup_EvStopAllEventCounter;
    uint32 Extr_Sup_ResetRebalCntEventCounter;
    uint8 is_EXTRACTION;
    is_EXTRACTION = Extraction_DW.is_EXTRACTION;
    Extr_Sup_ResetRebalCntEventCounter =
        Extraction_DW.Extr_Sup_ResetRebalCntEventCounter;
    Extr_Sup_EvStopAllEventCounter =
        Extraction_DW.Extr_Sup_EvStopAllEventCounter;
    is_RUNNING_kz = Extraction_DW.is_RUNNING_kz;

    /* Exit Internal 'EXTRACTION': '<S201>:74' */
    switch (is_EXTRACTION)
    {
      case Extraction_IN_MOTOR_STOPPING:
        /* Exit 'MOTOR_STOPPING': '<S201>:141' */
        Extraction_ReleaseMotion();
        is_EXTRACTION = 0U;
        break;

      case Extraction_IN_RUNNING_hm:
        /* Exit Internal 'RUNNING': '<S201>:41' */
        switch (is_RUNNING_kz)
        {
          case Extraction_IN_EXTRACTION_gg:
            /* Exit Internal 'EXTRACTION': '<S201>:6' */
            Extraction_DW.is_EXTRACTION_hb = 0U;

            /* Exit 'EXTRACTION': '<S201>:6' */
            Extraction_ParamEstControl(PE_STOP);
            Extraction_ParamEstExcControl(EXC_DISABLE);
            is_RUNNING_kz = 0U;
            break;

          case Extraction_IN_REBALANCE_hg:
            /* Exit 'REBALANCE': '<S201>:36' */
            is_RUNNING_kz = 0U;
            break;

          default:
            is_RUNNING_kz = 0U;
            break;
        }

        /* Exit 'RUNNING': '<S201>:41' */
        /* Event: '<S201>:54' */
        Extr_Sup_EvStopAllEventCounter++;

        /* Event: '<S201>:72' */
        Extr_Sup_ResetRebalCntEventCounter++;
        Extraction_B.Extr_Sup_CabHitEnabled = false;
        Extraction_B.Extr_Sup_VibMapEnabled = false;
        is_EXTRACTION = 0U;
        break;

      default:
        is_EXTRACTION = 0U;
        break;
    }

    Extraction_DW.is_RUNNING_kz = is_RUNNING_kz;
    Extraction_DW.Extr_Sup_EvStopAllEventCounter =
        Extr_Sup_EvStopAllEventCounter;
    Extraction_DW.Extr_Sup_ResetRebalCntEventCounter =
        Extr_Sup_ResetRebalCntEventCounter;
    Extraction_DW.is_EXTRACTION = is_EXTRACTION;
}

/* Function for Chart: '<S193>/Extr_Sup_Logic' */
static void Extraction_chartstep_c7_Extraction(const sint32 *sfEvent_ha)
{
    uint32 tmp;
    boolean_T tmp_0;
    boolean_T guard1 = false;

    /* During: Extraction/Extr_Supervisor/Extr_Supervisor/Extr_Sup_Logic */
    /* During 'EXTRACTION': '<S201>:74' */
    if (*sfEvent_ha == Extraction_event_ACU_ResetLoadMass)
    {
        /* Event: '<S201>:64' */
        Extraction_DW.Extr_Sup_ResetLoadMassEventCounter++;
    }

    guard1 = false;
    if (*sfEvent_ha == Extraction_event_ACU_ExtrStop)
    {
        /* Transition: '<S201>:75' */
        Extraction_DW.ExtrComplete = false;

        /* Transition: '<S201>:210' */
        guard1 = true;
    }
    else if (*sfEvent_ha == Extraction_event_ACU_ExtrEnd)
    {
        /* Transition: '<S201>:202' */
        Extraction_DW.ExtrComplete = false;
        Extraction_IsMotorRunning(&tmp_0);
        if ((*GESE_Extr_Setup).ESM_Disabled != 0 || !tmp_0)
        {
            /* Transition: '<S201>:200' */
            Extraction_ReleaseMotion();

            /* Transition: '<S201>:158' */
            Extraction_exit_internal_EXTRACTION();
            Extraction_DW.is_EXTRACTION = Extraction_IN_IDLE_kf;
            Extraction_B.Extr_Sup_State_d3 = EXTRACTION_STATE_IDLE;
        }
        else
        {
            /* Transition: '<S201>:203' */
            /* Transition: '<S201>:210' */
            guard1 = true;
        }
    }
    else if (*sfEvent_ha == Extraction_event_ACU_ExtrStart)
    {
        /* Transition: '<S201>:212' */
        Extraction_exit_internal_EXTRACTION();
        Extraction_DW.is_EXTRACTION = Extraction_IN_CONFIGURING;
        Extraction_B.Extr_Sup_State_d3 = EXTRACTION_STATE_CONFIGURING;

        /* Entry 'CONFIGURING': '<S201>:122' */
        Extraction__ClearFault();

        /* Event: '<S201>:120' */
        Extraction_DW.Extr_Sup_ClearFaultEventCounter++;

        /* Event: '<S201>:72' */
        Extraction_DW.Extr_Sup_ResetRebalCntEventCounter++;
        if (((boolean_T)EXTR_FEATURE_ACCEL_ENABLED))
        {
            Extraction_B.Extr_Sup_CabHitEnabled = true;
            Extraction_B.Extr_Sup_VibMapEnabled = true;
        }

        /* Outputs for Function Call SubSystem: '<S2>/Extr_UpdateInputs' */

        /* Event: '<S201>:103' */
        Extraction_Extr_UpdateInputs();

        /* End of Outputs for SubSystem: '<S2>/Extr_UpdateInputs' */
    }
    else if (*sfEvent_ha == Extraction_event_Extr_Rebal_EvCleanWasherException ||
             *sfEvent_ha == Extraction_event_Extr_Accel_EvCleanWasherException ||
             *sfEvent_ha == Extraction_event_Extr_LowSpd_EvCleanWasherException)
    {
        /* Transition: '<S201>:160' */
        Extraction_DW.ExtrComplete = true;
        guard1 = true;
    }
    else
    {
        switch (Extraction_DW.is_EXTRACTION)
        {
          case Extraction_IN_CONFIGURING:
            /* During 'CONFIGURING': '<S201>:122' */
            Extraction_GetMciFault(&tmp);
            if (!(tmp != 0U))
            {
                /* Transition: '<S201>:191' */
                Extraction_DW.is_EXTRACTION = Extraction_IN_RUNNING_hm;

                /* Entry 'RUNNING': '<S201>:41' */
                /*  Reset stats */
                Extr_Accel_MaxAccel = 0.0F;
                Extr_Accel_MaxAccelSpeed = 0.0F;
                Extr_Accel_HardCabHitCounter = 0U;
                Extr_Accel_SoftCabHitCounterOverall = 0U;
                Extr_Accel_MaxVibMapFloorDisp = 0.0F;
                Extr_HighSpd_CEMEventCounter = 0U;
                Extraction_DW.FaultDetected = false;
                Extraction_DW.ExtrComplete = false;

                /* Outputs for Function Call SubSystem: '<S2>/Extr_UpdateInputs' */

                /* Event: '<S201>:103' */
                Extraction_Extr_UpdateInputs();

                /* End of Outputs for SubSystem: '<S2>/Extr_UpdateInputs' */

                /* Entry Internal 'RUNNING': '<S201>:41' */
                /* Transition: '<S201>:44' */
                Extraction_DW.is_RUNNING_kz = Extraction_IN_EXTRACTION_gg;

                /* Entry Internal 'EXTRACTION': '<S201>:6' */
                /* Transition: '<S201>:16' */
                Extraction_DW.is_EXTRACTION_hb = Extraction_IN_LOW_SPEED;
                Extraction_B.Extr_Sup_State_d3 = EXTRACTION_STATE_LOW_SPEED;

                /* Entry 'LOW_SPEED': '<S201>:15' */
                /* Event: '<S201>:183' */
                Extraction_DW.Extr_Sup_EvResetSoftCabHitCntEventCounter++;

                /* Event: '<S201>:11' */
                Extraction_DW.Extr_Sup_EvLowSpdStartEventCounter++;
            }
            else
            {
                /* Outputs for Function Call SubSystem: '<S2>/Extr_UpdateInputs' */

                /* Event: '<S201>:103' */
                Extraction_Extr_UpdateInputs();

                /* End of Outputs for SubSystem: '<S2>/Extr_UpdateInputs' */
            }
            break;

          case Extraction_IN_EXTRACTION_COMPLETE:
            /* During 'EXTRACTION_COMPLETE': '<S201>:66' */
            break;

          case Extraction_IN_IDLE_kf:
            /* During 'IDLE': '<S201>:5' */
            break;

          case Extraction_IN_MOTOR_STOPPING:
            /* During 'MOTOR_STOPPING': '<S201>:141' */
            Extraction_IsMotorRunning(&tmp_0);
            if (!tmp_0)
            {
                /* Transition: '<S201>:150' */
                if (Extraction_DW.FaultDetected)
                {
                    /* Transition: '<S201>:152' */
                    /* Exit 'MOTOR_STOPPING': '<S201>:141' */
                    Extraction_ReleaseMotion();
                    Extraction_DW.is_EXTRACTION = Extraction_IN_CONFIGURING;
                    Extraction_B.Extr_Sup_State_d3 =
                        EXTRACTION_STATE_CONFIGURING;

                    /* Entry 'CONFIGURING': '<S201>:122' */
                    Extraction__ClearFault();

                    /* Event: '<S201>:120' */
                    Extraction_DW.Extr_Sup_ClearFaultEventCounter++;

                    /* Event: '<S201>:72' */
                    Extraction_DW.Extr_Sup_ResetRebalCntEventCounter++;
                    if (((boolean_T)EXTR_FEATURE_ACCEL_ENABLED))
                    {
                        Extraction_B.Extr_Sup_CabHitEnabled = true;
                        Extraction_B.Extr_Sup_VibMapEnabled = true;
                    }

                    /* Outputs for Function Call SubSystem: '<S2>/Extr_UpdateInputs' */

                    /* Event: '<S201>:103' */
                    Extraction_Extr_UpdateInputs();

                    /* End of Outputs for SubSystem: '<S2>/Extr_UpdateInputs' */
                }
                else
                {
                    /* Transition: '<S201>:153' */
                    if (Extraction_DW.ExtrComplete)
                    {
                        /* Transition: '<S201>:145' */
                        /* Exit 'MOTOR_STOPPING': '<S201>:141' */
                        Extraction_ReleaseMotion();
                        Extraction_DW.is_EXTRACTION =
                            Extraction_IN_EXTRACTION_COMPLETE;
                        Extraction_B.Extr_Sup_State_d3 =
                            EXTRACTION_STATE_EXTRACTION_COMPLETE;
                    }
                    else
                    {
                        /* Transition: '<S201>:144' */
                        /* Transition: '<S201>:158' */
                        /* Exit 'MOTOR_STOPPING': '<S201>:141' */
                        Extraction_ReleaseMotion();
                        Extraction_DW.is_EXTRACTION = Extraction_IN_IDLE_kf;
                        Extraction_B.Extr_Sup_State_d3 = EXTRACTION_STATE_IDLE;
                    }
                }
            }
            else
            {
                /* Outputs for Function Call SubSystem: '<S2>/Extr_UpdateInputs' */

                /* Event: '<S201>:103' */
                Extraction_Extr_UpdateInputs();

                /* End of Outputs for SubSystem: '<S2>/Extr_UpdateInputs' */
            }
            break;

          default:
            Extraction_RUNNING_oa(sfEvent_ha);
            break;
        }
    }

    if (guard1)
    {
        /* Transition: '<S201>:207' */
        Extraction_exit_internal_EXTRACTION();
        Extraction_DW.is_EXTRACTION = Extraction_IN_MOTOR_STOPPING;
        Extraction_B.Extr_Sup_State_d3 = EXTRACTION_STATE_MOTOR_STOPPING;

        /* Entry 'MOTOR_STOPPING': '<S201>:141' */
        Extraction_StopMotor();

        /* Outputs for Function Call SubSystem: '<S2>/Extr_UpdateInputs' */

        /* Event: '<S201>:103' */
        Extraction_Extr_UpdateInputs();

        /* End of Outputs for SubSystem: '<S2>/Extr_UpdateInputs' */
    }
}

/* Function for Chart: '<S193>/Extr_Sup_MotorSpeedCheck' */
static void Extraction_enter_atomic_MOTOR_ON(const uint8
    *Extr_Inputs_MaxSpeedError_iv, const uint8 *Extr_Inputs_RampTimeout_dy,
    const MOTOR_OUTPUT_TYPE *Extr_Inputs_MotorSignals_os, sint16
    Extr_Sup_SpdCommand_hvd[2])
{
    boolean_T tmp;
    sint16 Extr_Sup_SpdCommand_hvd_0;
    sint32 tmp_0;
    float32 MaxSpeedError;
    float32 Extr_Sup_RampTimeout_0;

    /* Entry 'MOTOR_ON': '<S202>:10' */
    Extr_Sup_RampTimeout_0 = *Extr_Inputs_RampTimeout_dy;
    if (Extr_Sup_SpdCommand_hvd[1] < 0)
    {
        tmp_0 = -Extr_Sup_SpdCommand_hvd[1];
        if (tmp_0 > 32767)
        {
            tmp_0 = 32767;
        }

        Extr_Sup_SpdCommand_hvd_0 = (sint16)tmp_0;
    }
    else
    {
        Extr_Sup_SpdCommand_hvd_0 = Extr_Sup_SpdCommand_hvd[1];
    }

    if (Extr_Sup_SpdCommand_hvd_0 > 0)
    {
        Extr_Sup_RampTimeout_0 += fabsf((float32)Extr_Sup_SpdCommand_hvd[0] -
            Extr_Inputs_MotorSignals_os->Speed) / fabsf((float32)
            Extr_Sup_SpdCommand_hvd[1]);
    }

    Extraction_IsMotorRunning(&tmp);
    if (!tmp)
    {
        Extr_Sup_RampTimeout_0 += 10.0F;

        /*  Add motor startup delay */
    }

    Extr_Sup_SpdMonitorPaused = fabsf((float32)Extr_Sup_SpdCommand_hvd[0]) <
        fabsf(Extr_Inputs_MotorSignals_os->Speed) ||
        Extraction_B.OutportBufferForExtr_LowSpd_PauseSpeedMonitor;
    Extr_Sup_RampTime = 0.0F;
    if (*Extr_Inputs_MaxSpeedError_iv == 255)
    {
        MaxSpeedError = 2.0F;
    }
    else
    {
        MaxSpeedError = *Extr_Inputs_MaxSpeedError_iv;
    }

    Extraction_DW.MaxSpeedError = MaxSpeedError;
    Extr_Sup_RampTimeout = Extr_Sup_RampTimeout_0;
}

/* Function for Chart: '<S193>/Extr_Sup_MotorSpeedCheck' */
static void Extraction_chartstep_c47_Extraction(const uint8
    *Extr_Inputs_MaxSpeedError_iv, const uint8 *Extr_Inputs_RampTimeout_dy,
    const MOTOR_OUTPUT_TYPE *Extr_Inputs_MotorSignals_os, uint8
    *Extr_Sup_SpdComCounter_cy, sint16 Extr_Sup_SpdCommand_hvd[2], sint32
    *sfEvent_pq)
{
    uint8 Extr_Sup_SpdComCounter_start;
    uint8 Extr_Sup_SpdComCounter_start_0;
    uint8 is_active_c47_Extraction;
    EXTR_SUP_SPEED_ERROR_MONITOR_STATE Extr_Sup_SpeedErrorMonitorState_0;
    uint8 is_c47_Extraction;
    uint32 Extr_Sup_EvSpeedErrorTimeoutEventCounter;
    uint32 Extr_Sup_EvSpeedReachedEventCounter;
    boolean_T guard1 = false;
    Extr_Sup_EvSpeedReachedEventCounter =
        Extraction_DW.Extr_Sup_EvSpeedReachedEventCounter;
    Extr_Sup_EvSpeedErrorTimeoutEventCounter =
        Extraction_DW.Extr_Sup_EvSpeedErrorTimeoutEventCounter;
    is_c47_Extraction = Extraction_DW.is_c47_Extraction;
    Extr_Sup_SpeedErrorMonitorState_0 = Extr_Sup_SpeedErrorMonitorState;
    is_active_c47_Extraction = Extraction_DW.is_active_c47_Extraction;
    Extr_Sup_SpdComCounter_start_0 = Extraction_DW.Extr_Sup_SpdComCounter_start;
    Extr_Sup_SpdComCounter_start = *Extr_Sup_SpdComCounter_cy;

    /* During: Extraction/Extr_Supervisor/Extr_Supervisor/Extr_Sup_MotorSpeedCheck */
    if (is_active_c47_Extraction == 0U)
    {
        /* Entry: Extraction/Extr_Supervisor/Extr_Supervisor/Extr_Sup_MotorSpeedCheck */
        is_active_c47_Extraction = 1U;

        /* Entry Internal: Extraction/Extr_Supervisor/Extr_Supervisor/Extr_Sup_MotorSpeedCheck */
        /* Transition: '<S202>:9' */
        is_c47_Extraction = Extraction_IN_MONITORING_SPEED_COMMAND;
        Extr_Sup_SpeedErrorMonitorState_0 =
            EXTR_SUP_SPEED_ERROR_MONITOR_STATE_MONITORING_SPEED_COMMAND;
    }
    else if (is_c47_Extraction == Extraction_IN_MONITORING_SPEED_COMMAND)
    {
        /* During 'MONITORING_SPEED_COMMAND': '<S202>:8' */
        if (Extr_Sup_SpdComCounter_start_0 != Extr_Sup_SpdComCounter_start)
        {
            /* Transition: '<S202>:63' */
            is_c47_Extraction = Extraction_IN_MOTOR_ON;
            Extr_Sup_SpeedErrorMonitorState_0 =
                EXTR_SUP_SPEED_ERROR_MONITOR_STATE_MOTOR_ON;
            Extraction_enter_atomic_MOTOR_ON(Extr_Inputs_MaxSpeedError_iv,
                Extr_Inputs_RampTimeout_dy, Extr_Inputs_MotorSignals_os,
                Extr_Sup_SpdCommand_hvd);
        }
    }
    else
    {
        /* During 'MOTOR_ON': '<S202>:10' */
        guard1 = false;
        if (*sfEvent_pq == Extraction_event_Extr_Sup_EvStopAll_g1)
        {
            /* Transition: '<S202>:69' */
            guard1 = true;
        }
        else if (fabsf((float32)Extr_Sup_SpdCommand_hvd[0] -
                       Extr_Inputs_MotorSignals_os->Speed) <=
                 Extraction_DW.MaxSpeedError)
        {
            /* Transition: '<S202>:45' */
            /* Event: '<S202>:52' */
            Extr_Sup_EvSpeedReachedEventCounter++;
            guard1 = true;
        }
        else if (Extr_Sup_RampTime > Extr_Sup_RampTimeout &&
                 !Extr_Sup_SpdMonitorPaused)
        {
            /* Transition: '<S202>:36' */
            Extraction_SetFault(EXTR_FAULT_SPEED_ERROR_EXCEEDED);

            /* Event: '<S202>:26' */
            Extr_Sup_EvSpeedErrorTimeoutEventCounter++;
            guard1 = true;
        }
        else if (Extr_Sup_SpdComCounter_start_0 != Extr_Sup_SpdComCounter_start)
        {
            /* Transition: '<S202>:68' */
            is_c47_Extraction = Extraction_IN_MOTOR_ON;
            Extr_Sup_SpeedErrorMonitorState_0 =
                EXTR_SUP_SPEED_ERROR_MONITOR_STATE_MOTOR_ON;
            Extraction_enter_atomic_MOTOR_ON(Extr_Inputs_MaxSpeedError_iv,
                Extr_Inputs_RampTimeout_dy, Extr_Inputs_MotorSignals_os,
                Extr_Sup_SpdCommand_hvd);
        }
        else
        {
            Extr_Sup_RampTime += 1.0F / (float32)((uint16)FS_EXTRACTION);
        }

        if (guard1)
        {
            is_c47_Extraction = Extraction_IN_MONITORING_SPEED_COMMAND;
            Extr_Sup_SpeedErrorMonitorState_0 =
                EXTR_SUP_SPEED_ERROR_MONITOR_STATE_MONITORING_SPEED_COMMAND;
        }
    }

    Extr_Sup_SpdComCounter_start_0 = Extr_Sup_SpdComCounter_start;
    Extraction_DW.Extr_Sup_SpdComCounter_start = Extr_Sup_SpdComCounter_start_0;
    Extraction_DW.is_active_c47_Extraction = is_active_c47_Extraction;
    Extr_Sup_SpeedErrorMonitorState = Extr_Sup_SpeedErrorMonitorState_0;
    Extraction_DW.is_c47_Extraction = is_c47_Extraction;
    Extraction_DW.Extr_Sup_EvSpeedErrorTimeoutEventCounter =
        Extr_Sup_EvSpeedErrorTimeoutEventCounter;
    Extraction_DW.Extr_Sup_EvSpeedReachedEventCounter =
        Extr_Sup_EvSpeedReachedEventCounter;
}

/* Function for Chart: '<S122>/Extr_LowSpd_SupervisorLogic' */
static void Extraction_enter_atomic_LOW_SPEED_COMPLETED(void)
{
    sint32 b_previousEvent;

    /* Entry 'LOW_SPEED_COMPLETED': '<S171>:85' */
    /* Event: '<S171>:353' */
    Extraction_DW.Extr_LowSpd_EvResetShortRedistEventCounter++;

    /* Event: '<S171>:123' */
    Extraction_DW.Extr_LowSpd_EvCompleteEventCounter++;

    /* Event: '<S171>:143' */
    b_previousEvent = Extraction_DW.sfEvent_f2;
    Extraction_DW.sfEvent_f2 = Extraction_event_EvStop_o5;
    Extraction_c15_Extraction();
    Extraction_DW.sfEvent_f2 = b_previousEvent;
}

/* Function for Chart: '<S122>/Extr_LowSpd_SupervisorLogic' */
static void Extraction_RUNNING_lv(void)
{
    boolean_T guard1 = false;
    boolean_T guard2 = false;

    /* During 'RUNNING': '<S171>:23' */
    guard1 = false;
    guard2 = false;
    if (Extraction_DW.sfEvent_f2 ==
            Extraction_event_Extr_Sup_EvSpeedErrorTimeout_cn ||
            Extraction_DW.sfEvent_f2 == Extraction_event_Extr_Accel_EvCabHit_kw)
    {
        /* Transition: '<S171>:249' */
        guard2 = true;
    }
    else if (Extraction_DW.sfEvent_f2 ==
             Extraction_event_Extr_Sup_EvFoamDetected_l0)
    {
        /* Transition: '<S171>:253' */
        guard1 = true;
    }
    else if (Extraction_DW.sfEvent_f2 ==
             Extraction_event_Extr_Sup_EvRebalStart_jz)
    {
        /* Transition: '<S171>:364' */
        Extraction_B.Extr_LowSpd_RebalCnt_e4++;
        guard1 = true;
    }
    else if (Extraction_DW.sfEvent_f2 ==
             Extraction_event_Extr_LowSpd_EvUnbChkRebalStart)
    {
        /* Transition: '<S171>:411' */
        Extraction_SetFault(EXTR_FAULT_LOW_SPD_HIGH_UNB_DETECTED);

        /* Transition: '<S171>:418' */
        guard2 = true;
    }
    else
    {
        if (Extraction_DW.sfEvent_f2 == Extraction_event_Extr_Accel_EvCabHit_kw)
        {
            Extraction_SetFault(EXTR_FAULT_LOW_SPD_CAB_HIT_DETECTED);
        }

        switch (Extraction_DW.is_RUNNING_jv)
        {
          case Extraction_IN_BASIC_UNBALANCE_CHECK:
            /* During 'BASIC_UNBALANCE_CHECK': '<S171>:346' */
            if (Extraction_DW.sfEvent_f2 ==
                    Extraction_event_Extr_LowSpd_EvUnbChkComplete)
            {
                /* Transition: '<S171>:347' */
                if ((*GESE_Extr_Setup).MDDP_Enabled != 0 &&
                        !Extraction_B.Extr_LowSpd_MddpExecuted_mb)
                {
                    /* Transition: '<S171>:282' */
                    Extraction_DW.is_RUNNING_jv = Extraction_IN_MDDP;
                    Extr_LowSpd_State = LOW_SPEED_STATE_MDDP;

                    /* Entry 'MDDP': '<S171>:65' */
                    Extraction_ParamEstExcControl(EXC_DISABLE);

                    /* Event: '<S171>:122' */
                    Extraction_DW.Extr_LowSpd_EvMddpStartEventCounter++;
                }
                else
                {
                    /* Transition: '<S171>:286' */
                    Extraction_DW.is_RUNNING_jv =
                        Extraction_IN_LOW_SPEED_COMPLETED;
                    Extr_LowSpd_State = LOW_SPEED_STATE_LOW_SPEED_COMPLETED;
                    Extraction_enter_atomic_LOW_SPEED_COMPLETED();
                }
            }
            break;

          case Extraction_IN_DISTRIBUTION:
            /* During 'DISTRIBUTION': '<S171>:126' */
            switch (Extraction_DW.sfEvent_f2)
            {
              case Extraction_event_Extr_Rebal_EvComplete_j3:
                /* Transition: '<S171>:127' */
                if ((*GESE_Extr_SpinProfileConfig).Factory_Calibration_Enabled
                        != 0)
                {
                    /* Transition: '<S171>:396' */
                    /* Transition: '<S171>:399' */
                    /* Transition: '<S171>:398' */
                    Extraction_DW.is_RUNNING_jv =
                        Extraction_IN_LOW_SPEED_COMPLETED;
                    Extr_LowSpd_State = LOW_SPEED_STATE_LOW_SPEED_COMPLETED;
                    Extraction_enter_atomic_LOW_SPEED_COMPLETED();
                }
                else
                {
                    /* Transition: '<S171>:394' */
                    Extraction_DW.is_RUNNING_jv = Extraction_IN_LOAD_MEASUREMENT;
                    Extr_LowSpd_State = LOW_SPEED_STATE_LOAD_MEASUREMENT;

                    /* Entry 'LOAD_MEASUREMENT': '<S171>:48' */
                    /* Event: '<S171>:120' */
                    Extraction_DW.Extr_LowSpd_EvLoadMeasStartEventCounter++;
                }
                break;

              case Extraction_event_Extr_Rebal_EvShortRedistExceeded_bq:
                /* Transition: '<S171>:338' */
                Extraction_B.Extr_LowSpd_RebalCnt_e4++;
                Extraction_DW.is_RUNNING_jv = 0U;
                if (Extraction_DW.is_LOW_SPEED_EXTRACTION !=
                        Extraction_IN_RUNNING_e4)
                {
                }
                else
                {
                    /* Exit 'RUNNING': '<S171>:23' */
                    Extraction_ParamEstExcControl(EXC_DISABLE);
                    Extraction_ParamEstControl(PE_STOP);
                    Extraction_DW.is_LOW_SPEED_EXTRACTION = 0U;
                }

                if (Extraction_DW.is_LOW_SPEED_EXTRACTION ==
                        Extraction_IN_IDLE_jy)
                {
                }
                else
                {
                    Extraction_DW.is_LOW_SPEED_EXTRACTION =
                        Extraction_IN_IDLE_jy;
                    Extr_LowSpd_State = LOW_SPEED_STATE_IDLE;

                    /* Entry 'IDLE': '<S171>:13' */
                    Extraction_DW.DistributionRequested = false;
                    Extraction_DW.AntifoamRequested = false;

                    /* Event: '<S171>:254' */
                    Extraction_DW.Extr_LowSpd_EvStopEventCounter++;
                }
                break;
            }
            break;

          case Extraction_IN_LOAD_MEASUREMENT:
            /* During 'LOAD_MEASUREMENT': '<S171>:48' */
            if (Extraction_DW.sfEvent_f2 ==
                    Extraction_event_Extr_LowSpd_EvLoadMeasComplete)
            {
                /* Transition: '<S171>:52' */
                /* Transition: '<S171>:431' */
                if ((*GESE_Extr_Setup).OR_Enabled != 0 && ((boolean_T)
                        EXTR_FEATURE_OPT_RAMP_ENABLED) && (*GESE_Extr_Setup).
                        PE_Enabled != 0)
                {
                    /* Transition: '<S171>:237' */
                    Extraction_DW.is_RUNNING_jv = Extraction_IN_OPTIMAL_RAMP;
                    Extr_LowSpd_State = LOW_SPEED_STATE_OPTIMAL_RAMP;

                    /* Entry 'OPTIMAL_RAMP': '<S171>:50' */
                    if (!((*GESE_Extr_Setup).MDDP_Enabled != 0))
                    {
                        Extraction_B.Extr_LowSpd_MddpExecuted_mb = true;
                    }

                    /* Event: '<S171>:121' */
                    Extraction_DW.Extr_LowSpd_EvOptRampStartEventCounter++;
                }
                else
                {
                    /* Transition: '<S171>:272' */
                    /* Transition: '<S171>:283' */
                    Extraction_DW.is_RUNNING_jv =
                        Extraction_IN_BASIC_UNBALANCE_CHECK;
                    Extr_LowSpd_State = LOW_SPEED_STATE_BASIC_UNBALANCE_CHECK;

                    /* Entry 'BASIC_UNBALANCE_CHECK': '<S171>:346' */
                    if (!((*GESE_Extr_Setup).MDDP_Enabled != 0))
                    {
                        Extraction_B.Extr_LowSpd_MddpExecuted_mb = true;
                    }

                    /* Event: '<S171>:343' */
                    Extraction_DW.Extr_LowSpd_EvStartBasicUnbChkEventCounter++;
                }
            }
            break;

          case Extraction_IN_LOW_SPEED_COMPLETED:
            /* During 'LOW_SPEED_COMPLETED': '<S171>:85' */
            break;

          case Extraction_IN_MDDP:
            /* During 'MDDP': '<S171>:65' */
            if (Extraction_DW.sfEvent_f2 ==
                    Extraction_event_Extr_LowSpd_EvMddpComplete)
            {
                /* Transition: '<S171>:263' */
                if (Extr_LowSpd_MddpDur <= 0)
                {
                    /* Transition: '<S171>:427' */
                    /* Transition: '<S171>:431' */
                    if ((*GESE_Extr_Setup).OR_Enabled != 0 && ((boolean_T)
                            EXTR_FEATURE_OPT_RAMP_ENABLED) && (*GESE_Extr_Setup)
                            .PE_Enabled != 0)
                    {
                        /* Transition: '<S171>:237' */
                        if (Extraction_DW.is_RUNNING_jv != Extraction_IN_MDDP)
                        {
                        }
                        else
                        {
                            /* Exit 'MDDP': '<S171>:65' */
                            Extraction_B.Extr_LowSpd_MddpExecuted_mb = true;
                            Extraction_DW.is_RUNNING_jv = 0U;
                        }

                        if (Extraction_DW.is_RUNNING_jv ==
                                Extraction_IN_OPTIMAL_RAMP)
                        {
                        }
                        else
                        {
                            Extraction_DW.is_RUNNING_jv =
                                Extraction_IN_OPTIMAL_RAMP;
                            Extr_LowSpd_State = LOW_SPEED_STATE_OPTIMAL_RAMP;

                            /* Entry 'OPTIMAL_RAMP': '<S171>:50' */
                            if (!((*GESE_Extr_Setup).MDDP_Enabled != 0))
                            {
                                Extraction_B.Extr_LowSpd_MddpExecuted_mb = true;
                            }

                            /* Event: '<S171>:121' */
                            Extraction_DW.Extr_LowSpd_EvOptRampStartEventCounter
                                ++;
                        }
                    }
                    else
                    {
                        /* Transition: '<S171>:272' */
                        /* Transition: '<S171>:283' */
                        if (Extraction_DW.is_RUNNING_jv != Extraction_IN_MDDP)
                        {
                        }
                        else
                        {
                            /* Exit 'MDDP': '<S171>:65' */
                            Extraction_B.Extr_LowSpd_MddpExecuted_mb = true;
                            Extraction_DW.is_RUNNING_jv = 0U;
                        }

                        if (Extraction_DW.is_RUNNING_jv ==
                                Extraction_IN_BASIC_UNBALANCE_CHECK)
                        {
                        }
                        else
                        {
                            Extraction_DW.is_RUNNING_jv =
                                Extraction_IN_BASIC_UNBALANCE_CHECK;
                            Extr_LowSpd_State =
                                LOW_SPEED_STATE_BASIC_UNBALANCE_CHECK;

                            /* Entry 'BASIC_UNBALANCE_CHECK': '<S171>:346' */
                            if (!((*GESE_Extr_Setup).MDDP_Enabled != 0))
                            {
                                Extraction_B.Extr_LowSpd_MddpExecuted_mb = true;
                            }

                            /* Event: '<S171>:343' */
                            Extraction_DW.Extr_LowSpd_EvStartBasicUnbChkEventCounter
                                ++;
                        }
                    }
                }
                else
                {
                    /* Transition: '<S171>:429' */
                    /* Transition: '<S171>:195' */
                    if (Extraction_DW.is_RUNNING_jv != Extraction_IN_MDDP)
                    {
                    }
                    else
                    {
                        /* Exit 'MDDP': '<S171>:65' */
                        Extraction_B.Extr_LowSpd_MddpExecuted_mb = true;
                        Extraction_DW.is_RUNNING_jv = 0U;
                    }

                    if (Extraction_DW.is_RUNNING_jv ==
                            Extraction_IN_LOAD_MEASUREMENT)
                    {
                    }
                    else
                    {
                        Extraction_DW.is_RUNNING_jv =
                            Extraction_IN_LOAD_MEASUREMENT;
                        Extr_LowSpd_State = LOW_SPEED_STATE_LOAD_MEASUREMENT;

                        /* Entry 'LOAD_MEASUREMENT': '<S171>:48' */
                        /* Event: '<S171>:120' */
                        Extraction_DW.Extr_LowSpd_EvLoadMeasStartEventCounter++;
                    }
                }
            }
            break;

          case Extraction_IN_OPTIMAL_RAMP:
            /* During 'OPTIMAL_RAMP': '<S171>:50' */
            switch (Extraction_DW.sfEvent_f2)
            {
              case Extraction_event_Ext_LowSpd_EvOptRampComplete:
                /* Transition: '<S171>:66' */
                Extraction_DW.is_RUNNING_jv =
                    Extraction_IN_BASIC_UNBALANCE_CHECK;
                Extr_LowSpd_State = LOW_SPEED_STATE_BASIC_UNBALANCE_CHECK;

                /* Entry 'BASIC_UNBALANCE_CHECK': '<S171>:346' */
                if (!((*GESE_Extr_Setup).MDDP_Enabled != 0))
                {
                    Extraction_B.Extr_LowSpd_MddpExecuted_mb = true;
                }

                /* Event: '<S171>:343' */
                Extraction_DW.Extr_LowSpd_EvStartBasicUnbChkEventCounter++;
                break;

              case Extraction_event_Extr_LowSpd_EvOptRampRedist:
                /* Transition: '<S171>:133' */
                Extr_LowSpd_Fault = EXTR_LOW_SPD_FAULT_OPT_RAMP_HIGH_UNBALANCE;
                Extraction_SetFault(EXTR_FAULT_LOW_SPD_HIGH_UNB_DETECTED);

                /* Transition: '<S171>:268' */
                /* Transition: '<S171>:269' */
                Extraction_DW.is_RUNNING_jv = Extraction_IN_DISTRIBUTION;
                Extr_LowSpd_State = LOW_SPEED_STATE_DISTRIBUTION;

                /* Entry 'DISTRIBUTION': '<S171>:126' */
                Extraction_ParamEstExcControl(EXC_DISABLE);

                /* Event: '<S171>:119' */
                Extraction_DW.Extr_LowSpd_EvDistributionStartEventCounter++;
                break;
            }
            break;
        }
    }

    if (guard2)
    {
        /* Transition: '<S171>:361' */
        /* Event: '<S171>:116' */
        Extraction_DW.Extr_LowSpd_EvRebalStartEventCounter++;
        Extraction_B.Extr_LowSpd_RebalCnt_e4++;
        guard1 = true;
    }

    if (guard1)
    {
        /* Exit Internal 'RUNNING': '<S171>:23' */
        if (Extraction_DW.is_RUNNING_jv == Extraction_IN_MDDP)
        {
            /* Exit 'MDDP': '<S171>:65' */
            Extraction_B.Extr_LowSpd_MddpExecuted_mb = true;
            Extraction_DW.is_RUNNING_jv = 0U;
        }
        else
        {
            Extraction_DW.is_RUNNING_jv = 0U;
        }

        if (Extraction_DW.is_LOW_SPEED_EXTRACTION != Extraction_IN_RUNNING_e4)
        {
        }
        else
        {
            /* Exit 'RUNNING': '<S171>:23' */
            Extraction_ParamEstExcControl(EXC_DISABLE);
            Extraction_ParamEstControl(PE_STOP);
            Extraction_DW.is_LOW_SPEED_EXTRACTION = 0U;
        }

        if (Extraction_DW.is_LOW_SPEED_EXTRACTION == Extraction_IN_IDLE_jy)
        {
        }
        else
        {
            Extraction_DW.is_LOW_SPEED_EXTRACTION = Extraction_IN_IDLE_jy;
            Extr_LowSpd_State = LOW_SPEED_STATE_IDLE;

            /* Entry 'IDLE': '<S171>:13' */
            Extraction_DW.DistributionRequested = false;
            Extraction_DW.AntifoamRequested = false;

            /* Event: '<S171>:254' */
            Extraction_DW.Extr_LowSpd_EvStopEventCounter++;
        }
    }
}

/* Function for Chart: '<S122>/Extr_LowSpd_SupervisorLogic' */
static void Extraction_exit_internal_LOW_SPEED_EXTRACTION(void)
{
    uint8 is_RUNNING_jv;
    uint8 is_LOW_SPEED_EXTRACTION;
    is_LOW_SPEED_EXTRACTION = Extraction_DW.is_LOW_SPEED_EXTRACTION;
    is_RUNNING_jv = Extraction_DW.is_RUNNING_jv;

    /* Exit Internal 'LOW_SPEED_EXTRACTION': '<S171>:155' */
    switch (is_LOW_SPEED_EXTRACTION)
    {
      case Extraction_IN_DROPPING_TO_DIST_SPEED:
        /* Exit Internal 'DROPPING_TO_DIST_SPEED': '<S171>:321' */
        Extraction_DW.is_DROPPING_TO_DIST_SPEED = 0U;

        /* Exit 'DROPPING_TO_DIST_SPEED': '<S171>:321' */
        Extraction_B.Extr_LowSpd_PauseSpeedMonitor = false;
        is_LOW_SPEED_EXTRACTION = 0U;
        break;

      case Extraction_IN_IDLE_jy:
        /* Exit 'IDLE': '<S171>:13' */
        /*  Disable PE Excitation */
        Extraction_ParamEstExcControl(EXC_DISABLE);
        Extraction_LoadDistributionSpeed(&Extraction_B.Extr_LowSpd_DistSpeed_lt);
        is_LOW_SPEED_EXTRACTION = 0U;
        break;

      case Extraction_IN_RUNNING_e4:
        /* Exit Internal 'RUNNING': '<S171>:23' */
        if (is_RUNNING_jv == Extraction_IN_MDDP)
        {
            /* Exit 'MDDP': '<S171>:65' */
            Extraction_B.Extr_LowSpd_MddpExecuted_mb = true;
            is_RUNNING_jv = 0U;
        }
        else
        {
            is_RUNNING_jv = 0U;
        }

        /* Exit 'RUNNING': '<S171>:23' */
        Extraction_ParamEstExcControl(EXC_DISABLE);
        Extraction_ParamEstControl(PE_STOP);
        is_LOW_SPEED_EXTRACTION = 0U;
        break;

      default:
        is_LOW_SPEED_EXTRACTION = 0U;
        break;
    }

    Extraction_DW.is_RUNNING_jv = is_RUNNING_jv;
    Extraction_DW.is_LOW_SPEED_EXTRACTION = is_LOW_SPEED_EXTRACTION;
}

/* Function for Chart: '<S122>/Extr_LowSpd_SupervisorLogic' */
static void Extraction_c15_Extraction(void)
{
    sint32 b_previousEvent;
    boolean_T d_sf_internal_predicateOutput;
    uint8 tmp[8];
    boolean_T guard1 = false;

    /* During: Extraction/Extr_LowSpd/Extr_LowSpd_Supervisor/Extr_LowSpd_SupervisorLogic */
    if (Extraction_DW.is_c15_Extraction != Extraction_IN_LOW_SPEED_EXTRACTION)
    {
    }
    else
    {
        /* During 'LOW_SPEED_EXTRACTION': '<S171>:155' */
        if (Extraction_DW.sfEvent_f2 ==
                Extraction_event_Extr_Sup_EvResetLoadMass)
        {
            Extraction_B.Extr_LowSpd_MddpExecuted_mb = false;

            /* Event: '<S171>:241' */
            Extraction_DW.Extr_LowSpd_EvResetLoadMassEventCounter++;
        }

        if (Extraction_DW.sfEvent_f2 ==
                Extraction_event_Extr_Sup_ResetRebalCnt_ay)
        {
            Extraction_B.Extr_LowSpd_RebalCnt_e4 = 0U;
        }

        if (Extraction_DW.sfEvent_f2 == Extraction_event_Extr_Sup_ClearFault_ba)
        {
            Extr_LowSpd_Fault = EXTR_LOW_SPD_FAULT_NONE;
        }

        guard1 = false;
        if (Extraction_DW.sfEvent_f2 == Extraction_event_EvStop_o5 ||
                Extraction_DW.sfEvent_f2 ==
                Extraction_event_Extr_Sup_EvStopAll_ne)
        {
            /* Transition: '<S171>:425' */
            guard1 = true;
        }
        else if (Extraction_DW.sfEvent_f2 ==
                 Extraction_event_Extr_Sup_EvFoamDetected_l0)
        {
            /* Transition: '<S171>:371' */
            Extraction_DW.AntifoamRequested = true;
            Extraction_DW.DistributionRequested = false;

            /* Event: '<S171>:254' */
            Extraction_DW.Extr_LowSpd_EvStopEventCounter++;
            if (Extraction_B.Extr_LowSpd_DistSpeed_lt > ((sint16)EXTR_MIN_SPEED))
            {
                if (fabsf(Extr_Inputs_MotorSignals.Speed) > (sint16)
                        (Extraction_B.Extr_LowSpd_DistSpeed_lt - 10))
                {
                    Extraction_IsMotorRunning(&d_sf_internal_predicateOutput);
                }
                else
                {
                    d_sf_internal_predicateOutput = false;
                }
            }
            else
            {
                d_sf_internal_predicateOutput = false;
            }

            if (d_sf_internal_predicateOutput)
            {
                /* Transition: '<S171>:376' */
                Extraction_exit_internal_LOW_SPEED_EXTRACTION();
                if (Extraction_DW.is_LOW_SPEED_EXTRACTION ==
                        Extraction_IN_DROPPING_TO_DIST_SPEED)
                {
                }
                else
                {
                    Extraction_DW.is_LOW_SPEED_EXTRACTION =
                        Extraction_IN_DROPPING_TO_DIST_SPEED;

                    /* Entry 'DROPPING_TO_DIST_SPEED': '<S171>:321' */
                    Extraction_B.Extr_LowSpd_PauseSpeedMonitor = true;
                    if ((*GESE_Extr_Setup).PE_Enabled != 0)
                    {
                        Extraction_ParamEstControl(PE_STOP);
                        Extraction_ParamEstExcControl(EXC_DISABLE);
                    }
                }

                /* Entry Internal 'DROPPING_TO_DIST_SPEED': '<S171>:321' */
                /* Transition: '<S171>:331' */
                if (Extraction_DW.is_DROPPING_TO_DIST_SPEED ==
                        Extraction_IN_SETTING_DIST_SPEED)
                {
                }
                else
                {
                    Extraction_DW.is_DROPPING_TO_DIST_SPEED =
                        Extraction_IN_SETTING_DIST_SPEED;
                    Extr_LowSpd_State = LOW_SPEED_STATE_SETTING_DIST_SPEED;

                    /* Entry 'SETTING_DIST_SPEED': '<S171>:330' */
                    Extraction_LoadDistributionSpeed
                        (&Extraction_B.Extr_LowSpd_DistSpeed_lt);
                    Extraction_SetSpeedCommand
                        (Extraction_B.Extr_LowSpd_DistSpeed_lt, 50);
                }
            }
            else
            {
                /* Transition: '<S171>:374' */
                /* Transition: '<S171>:375' */
                guard1 = true;
            }
        }
        else if (Extraction_DW.sfEvent_f2 ==
                 Extraction_event_Extr_Sup_EvLowSpdStart)
        {
            /* Transition: '<S171>:45' */
            Extraction_LoadDistributionSpeed
                (&Extraction_B.Extr_LowSpd_DistSpeed_lt);
            if (Extraction_DW.DistributionRequested &&
                    (*GESE_Extr_SpinProfileConfig).Spin_Pulse_Count > 0 &&
                    Extraction_B.VariantMergeForOutportExtr_HighSpd_CurrPulseInd
                >= (uint8)((*GESE_Extr_SpinProfileConfig).Spin_Pulse_Count - 1U))
            {
                /* Transition: '<S171>:403' */
                Extraction_DW.DistributionRequested = false;

                /* Transition: '<S171>:405' */
                /* Transition: '<S171>:423' */
                /* Transition: '<S171>:406' */
                Extraction_exit_internal_LOW_SPEED_EXTRACTION();
                if (Extraction_DW.is_LOW_SPEED_EXTRACTION ==
                        Extraction_IN_RUNNING_e4)
                {
                }
                else
                {
                    Extraction_DW.is_LOW_SPEED_EXTRACTION =
                        Extraction_IN_RUNNING_e4;

                    /* Entry 'RUNNING': '<S171>:23' */
                    Extraction_B.Extr_LowSpd_PauseSpeedMonitor = false;
                    Extraction_RetrieveMaxTargetSpeed();
                    if ((*GESE_Extr_Setup).PE_Enabled != 0)
                    {
                        /*  Start PE to warm the filters up */
                        tmp[0] = 0U;
                        tmp[1] = 0U;
                        tmp[2] = 0U;
                        tmp[3] = 0U;
                        tmp[4] = 0U;
                        tmp[5] = 0U;
                        tmp[6] = 0U;
                        tmp[7] = 0U;
                        Extraction_SelectParamEstOutputs(tmp);
                        Extraction_ParamEstControl(PE_RESET);
                    }
                }

                if (Extraction_DW.is_RUNNING_jv ==
                        Extraction_IN_LOW_SPEED_COMPLETED)
                {
                }
                else
                {
                    Extraction_DW.is_RUNNING_jv =
                        Extraction_IN_LOW_SPEED_COMPLETED;
                    Extr_LowSpd_State = LOW_SPEED_STATE_LOW_SPEED_COMPLETED;
                    Extraction_enter_atomic_LOW_SPEED_COMPLETED();
                }
            }
            else
            {
                /* Transition: '<S171>:417' */
                Extraction_exit_internal_LOW_SPEED_EXTRACTION();
                if (Extraction_DW.is_LOW_SPEED_EXTRACTION ==
                        Extraction_IN_PE_INIT)
                {
                }
                else
                {
                    Extraction_DW.is_LOW_SPEED_EXTRACTION =
                        Extraction_IN_PE_INIT;
                    Extr_LowSpd_State = LOW_SPEED_STATE_PE_INIT;

                    /* Entry 'PE_INIT': '<S171>:230' */
                    if ((*GESE_Extr_Setup).PE_Enabled != 0)
                    {
                        Extraction_UpdateParamEstSetup
                            ((*GESE_Extr_ParamEstLowSpeedSetup).
                             Param_Est_Setup_Index);

                        /* Outputs for Function Call SubSystem: '<S122>/UpdatePEAppSpecificSetup' */

                        /* Event: '<S171>:246' */
                        Extraction_UpdatePEAppSpecificSetup
                            (&Extraction_B.UpdatePEAppSpecificSetup);

                        /* End of Outputs for SubSystem: '<S122>/UpdatePEAppSpecificSetup' */
                    }
                }
            }
        }
        else
        {
            switch (Extraction_DW.is_LOW_SPEED_EXTRACTION)
            {
              case Extraction_IN_DROPPING_TO_DIST_SPEED:
                /* During 'DROPPING_TO_DIST_SPEED': '<S171>:321' */
                if (Extraction_DW.sfEvent_f2 == Extraction_event_EvDistCompleted)
                {
                    /* Transition: '<S171>:324' */
                    if (Extraction_DW.DistributionRequested &&
                            (*GESE_Extr_SpinProfileConfig).Spin_Pulse_Count > 0 &&
                        Extraction_B.VariantMergeForOutportExtr_HighSpd_CurrPulseInd
                        >= (uint8)((*GESE_Extr_SpinProfileConfig).
                                   Spin_Pulse_Count - 1U))
                    {
                        /* Transition: '<S171>:403' */
                        Extraction_DW.DistributionRequested = false;

                        /* Transition: '<S171>:405' */
                        /* Transition: '<S171>:423' */
                        /* Transition: '<S171>:406' */
                        /* Exit Internal 'DROPPING_TO_DIST_SPEED': '<S171>:321' */
                        Extraction_DW.is_DROPPING_TO_DIST_SPEED = 0U;
                        if (Extraction_DW.is_LOW_SPEED_EXTRACTION !=
                                Extraction_IN_DROPPING_TO_DIST_SPEED)
                        {
                        }
                        else
                        {
                            /* Exit 'DROPPING_TO_DIST_SPEED': '<S171>:321' */
                            Extraction_B.Extr_LowSpd_PauseSpeedMonitor = false;
                            Extraction_DW.is_LOW_SPEED_EXTRACTION = 0U;
                        }

                        if (Extraction_DW.is_LOW_SPEED_EXTRACTION ==
                                Extraction_IN_RUNNING_e4)
                        {
                        }
                        else
                        {
                            Extraction_DW.is_LOW_SPEED_EXTRACTION =
                                Extraction_IN_RUNNING_e4;

                            /* Entry 'RUNNING': '<S171>:23' */
                            Extraction_B.Extr_LowSpd_PauseSpeedMonitor = false;
                            Extraction_RetrieveMaxTargetSpeed();
                            if ((*GESE_Extr_Setup).PE_Enabled != 0)
                            {
                                /*  Start PE to warm the filters up */
                                tmp[0] = 0U;
                                tmp[1] = 0U;
                                tmp[2] = 0U;
                                tmp[3] = 0U;
                                tmp[4] = 0U;
                                tmp[5] = 0U;
                                tmp[6] = 0U;
                                tmp[7] = 0U;
                                Extraction_SelectParamEstOutputs(tmp);
                                Extraction_ParamEstControl(PE_RESET);
                            }
                        }

                        if (Extraction_DW.is_RUNNING_jv ==
                                Extraction_IN_LOW_SPEED_COMPLETED)
                        {
                        }
                        else
                        {
                            Extraction_DW.is_RUNNING_jv =
                                Extraction_IN_LOW_SPEED_COMPLETED;
                            Extr_LowSpd_State =
                                LOW_SPEED_STATE_LOW_SPEED_COMPLETED;
                            Extraction_enter_atomic_LOW_SPEED_COMPLETED();
                        }
                    }
                    else
                    {
                        /* Transition: '<S171>:417' */
                        /* Exit Internal 'DROPPING_TO_DIST_SPEED': '<S171>:321' */
                        Extraction_DW.is_DROPPING_TO_DIST_SPEED = 0U;
                        if (Extraction_DW.is_LOW_SPEED_EXTRACTION !=
                                Extraction_IN_DROPPING_TO_DIST_SPEED)
                        {
                        }
                        else
                        {
                            /* Exit 'DROPPING_TO_DIST_SPEED': '<S171>:321' */
                            Extraction_B.Extr_LowSpd_PauseSpeedMonitor = false;
                            Extraction_DW.is_LOW_SPEED_EXTRACTION = 0U;
                        }

                        if (Extraction_DW.is_LOW_SPEED_EXTRACTION ==
                                Extraction_IN_PE_INIT)
                        {
                        }
                        else
                        {
                            Extraction_DW.is_LOW_SPEED_EXTRACTION =
                                Extraction_IN_PE_INIT;
                            Extr_LowSpd_State = LOW_SPEED_STATE_PE_INIT;

                            /* Entry 'PE_INIT': '<S171>:230' */
                            if ((*GESE_Extr_Setup).PE_Enabled != 0)
                            {
                                Extraction_UpdateParamEstSetup
                                    ((*GESE_Extr_ParamEstLowSpeedSetup).
                                     Param_Est_Setup_Index);

                                /* Outputs for Function Call SubSystem: '<S122>/UpdatePEAppSpecificSetup' */

                                /* Event: '<S171>:246' */
                                Extraction_UpdatePEAppSpecificSetup
                                    (&Extraction_B.UpdatePEAppSpecificSetup);

                                /* End of Outputs for SubSystem: '<S122>/UpdatePEAppSpecificSetup' */
                            }
                        }
                    }
                }
                else
                {
                    switch (Extraction_DW.is_DROPPING_TO_DIST_SPEED)
                    {
                      case Extraction_IN_EXECUTING_ANTIFOAM_ROUTINE:
                        /* During 'EXECUTING_ANTIFOAM_ROUTINE': '<S171>:382' */
                        break;

                      case Extraction_IN_SETTING_DIST_SPEED:
                        /* During 'SETTING_DIST_SPEED': '<S171>:330' */
                        if (Extraction_DW.sfEvent_f2 ==
                                Extraction_event_Extr_Sup_EvSpeedReached_pu)
                        {
                            /* Transition: '<S171>:334' */
                            if (Extraction_DW.AntifoamRequested)
                            {
                                /* Transition: '<S171>:380' */
                                /* Transition: '<S171>:383' */
                                Extraction_DW.is_DROPPING_TO_DIST_SPEED =
                                    Extraction_IN_EXECUTING_ANTIFOAM_ROUTINE;
                                Extr_LowSpd_State =
                                    LOW_SPEED_STATE_EXECUTING_ANTIFOAM_ROUTINE;

                                /* Entry 'EXECUTING_ANTIFOAM_ROUTINE': '<S171>:382' */
                            }
                            else
                            {
                                /* Transition: '<S171>:381' */
                                Extraction_DW.is_DROPPING_TO_DIST_SPEED =
                                    Extraction_IN_WAITING_DIST_SPEED_TIMEOUT;
                                Extraction_DW.temporalCounter_i1_jq = 0U;
                                Extr_LowSpd_State =
                                    LOW_SPEED_STATE_WAITING_DIST_SPEED_TIMEOUT;

                                /* Entry 'WAITING_DIST_SPEED_TIMEOUT': '<S171>:333' */
                            }
                        }
                        break;

                      case Extraction_IN_WAITING_DIST_SPEED_TIMEOUT:
                        /* During 'WAITING_DIST_SPEED_TIMEOUT': '<S171>:333' */
                        if (Extraction_DW.sfEvent_f2 ==
                                Extraction_event_Trigger_o1 &&
                                Extraction_DW.temporalCounter_i1_jq >= (uint16)
                                ((uint32)
                                 Extraction_B.VariantMergeForOutportExtr_HighSpd_DistSpeedTimeout
                                 * ((uint16)FS_EXTRACTION)))
                        {
                            /* Transition: '<S171>:336' */
                            /* Event: '<S171>:329' */
                            b_previousEvent = Extraction_DW.sfEvent_f2;
                            Extraction_DW.sfEvent_f2 =
                                Extraction_event_EvDistCompleted;
                            Extraction_c15_Extraction();
                            Extraction_DW.sfEvent_f2 = b_previousEvent;
                        }
                        break;
                    }
                }
                break;

              case Extraction_IN_IDLE_jy:
                /* During 'IDLE': '<S171>:13' */
                if (Extraction_DW.sfEvent_f2 ==
                        Extraction_event_Extr_HighSpd_EvDropToDistSpeed)
                {
                    /* Transition: '<S171>:326' */
                    Extraction_DW.AntifoamRequested = false;
                    Extraction_DW.DistributionRequested = true;

                    /* Transition: '<S171>:327' */
                    if (Extraction_DW.is_LOW_SPEED_EXTRACTION !=
                            Extraction_IN_IDLE_jy)
                    {
                    }
                    else
                    {
                        /* Exit 'IDLE': '<S171>:13' */
                        /*  Disable PE Excitation */
                        Extraction_ParamEstExcControl(EXC_DISABLE);
                        Extraction_LoadDistributionSpeed
                            (&Extraction_B.Extr_LowSpd_DistSpeed_lt);
                        Extraction_DW.is_LOW_SPEED_EXTRACTION = 0U;
                    }

                    if (Extraction_DW.is_LOW_SPEED_EXTRACTION ==
                            Extraction_IN_DROPPING_TO_DIST_SPEED)
                    {
                    }
                    else
                    {
                        Extraction_DW.is_LOW_SPEED_EXTRACTION =
                            Extraction_IN_DROPPING_TO_DIST_SPEED;

                        /* Entry 'DROPPING_TO_DIST_SPEED': '<S171>:321' */
                        Extraction_B.Extr_LowSpd_PauseSpeedMonitor = true;
                        if ((*GESE_Extr_Setup).PE_Enabled != 0)
                        {
                            Extraction_ParamEstControl(PE_STOP);
                            Extraction_ParamEstExcControl(EXC_DISABLE);
                        }
                    }

                    /* Entry Internal 'DROPPING_TO_DIST_SPEED': '<S171>:321' */
                    /* Transition: '<S171>:331' */
                    if (Extraction_DW.is_DROPPING_TO_DIST_SPEED ==
                            Extraction_IN_SETTING_DIST_SPEED)
                    {
                    }
                    else
                    {
                        Extraction_DW.is_DROPPING_TO_DIST_SPEED =
                            Extraction_IN_SETTING_DIST_SPEED;
                        Extr_LowSpd_State = LOW_SPEED_STATE_SETTING_DIST_SPEED;

                        /* Entry 'SETTING_DIST_SPEED': '<S171>:330' */
                        Extraction_LoadDistributionSpeed
                            (&Extraction_B.Extr_LowSpd_DistSpeed_lt);
                        Extraction_SetSpeedCommand
                            (Extraction_B.Extr_LowSpd_DistSpeed_lt, 50);
                    }
                }
                break;

              case Extraction_IN_PE_INIT:
                /* During 'PE_INIT': '<S171>:230' */
                if (ExtractionParams__IsExtrConfigValid() != 0)
                {
                    /* Transition: '<S171>:221' */
                    /* Transition: '<S171>:413' */
                    if (Extraction_B.Extr_LowSpd_DistSpeed_lt > ((sint16)
                            EXTR_MIN_SPEED))
                    {
                        if (fabsf(Extr_Inputs_MotorSignals.Speed) > (sint16)
                                (Extraction_B.Extr_LowSpd_DistSpeed_lt + 25))
                        {
                            Extraction_IsMotorRunning
                                (&d_sf_internal_predicateOutput);
                        }
                        else
                        {
                            d_sf_internal_predicateOutput = false;
                        }
                    }
                    else
                    {
                        d_sf_internal_predicateOutput = false;
                    }

                    if (d_sf_internal_predicateOutput)
                    {
                        /* Transition: '<S171>:421' */
                        /* Transition: '<S171>:424' */
                        /* Transition: '<S171>:406' */
                        Extraction_DW.is_LOW_SPEED_EXTRACTION =
                            Extraction_IN_RUNNING_e4;

                        /* Entry 'RUNNING': '<S171>:23' */
                        Extraction_B.Extr_LowSpd_PauseSpeedMonitor = false;
                        Extraction_RetrieveMaxTargetSpeed();
                        if ((*GESE_Extr_Setup).PE_Enabled != 0)
                        {
                            /*  Start PE to warm the filters up */
                            tmp[0] = 0U;
                            tmp[1] = 0U;
                            tmp[2] = 0U;
                            tmp[3] = 0U;
                            tmp[4] = 0U;
                            tmp[5] = 0U;
                            tmp[6] = 0U;
                            tmp[7] = 0U;
                            Extraction_SelectParamEstOutputs(tmp);
                            Extraction_ParamEstControl(PE_RESET);
                        }

                        if (Extraction_DW.is_RUNNING_jv ==
                                Extraction_IN_LOW_SPEED_COMPLETED)
                        {
                        }
                        else
                        {
                            Extraction_DW.is_RUNNING_jv =
                                Extraction_IN_LOW_SPEED_COMPLETED;
                            Extr_LowSpd_State =
                                LOW_SPEED_STATE_LOW_SPEED_COMPLETED;
                            Extraction_enter_atomic_LOW_SPEED_COMPLETED();
                        }
                    }
                    else
                    {
                        /* Transition: '<S171>:312' */
                        Extraction_DW.is_LOW_SPEED_EXTRACTION =
                            Extraction_IN_RUNNING_e4;

                        /* Entry 'RUNNING': '<S171>:23' */
                        Extraction_B.Extr_LowSpd_PauseSpeedMonitor = false;
                        Extraction_RetrieveMaxTargetSpeed();
                        if ((*GESE_Extr_Setup).PE_Enabled != 0)
                        {
                            /*  Start PE to warm the filters up */
                            tmp[0] = 0U;
                            tmp[1] = 0U;
                            tmp[2] = 0U;
                            tmp[3] = 0U;
                            tmp[4] = 0U;
                            tmp[5] = 0U;
                            tmp[6] = 0U;
                            tmp[7] = 0U;
                            Extraction_SelectParamEstOutputs(tmp);
                            Extraction_ParamEstControl(PE_RESET);
                        }

                        /* Entry Internal 'RUNNING': '<S171>:23' */
                        /* Transition: '<S171>:128' */
                        if (Extraction_DW.is_RUNNING_jv ==
                                Extraction_IN_DISTRIBUTION)
                        {
                        }
                        else
                        {
                            Extraction_DW.is_RUNNING_jv =
                                Extraction_IN_DISTRIBUTION;
                            Extr_LowSpd_State = LOW_SPEED_STATE_DISTRIBUTION;

                            /* Entry 'DISTRIBUTION': '<S171>:126' */
                            Extraction_ParamEstExcControl(EXC_DISABLE);

                            /* Event: '<S171>:119' */
                            Extraction_DW.Extr_LowSpd_EvDistributionStartEventCounter
                                ++;
                        }
                    }
                }
                else
                {
                    /* Transition: '<S171>:228' */
                    Extr_LowSpd_Fault = EXTR_LOW_SPD_FAULT_INVALID_CONFIG;
                    Extraction_SetFault(EXTR_FAULT_LOW_SPD_INVALID_CONFIG);
                    Extraction_DW.is_LOW_SPEED_EXTRACTION =
                        Extraction_IN_IDLE_jy;
                    Extr_LowSpd_State = LOW_SPEED_STATE_IDLE;

                    /* Entry 'IDLE': '<S171>:13' */
                    Extraction_DW.DistributionRequested = false;
                    Extraction_DW.AntifoamRequested = false;

                    /* Event: '<S171>:254' */
                    Extraction_DW.Extr_LowSpd_EvStopEventCounter++;
                }
                break;

              case Extraction_IN_RUNNING_e4:
                Extraction_RUNNING_lv();
                break;
            }
        }

        if (guard1)
        {
            Extraction_exit_internal_LOW_SPEED_EXTRACTION();
            if (Extraction_DW.is_LOW_SPEED_EXTRACTION == Extraction_IN_IDLE_jy)
            {
            }
            else
            {
                Extraction_DW.is_LOW_SPEED_EXTRACTION = Extraction_IN_IDLE_jy;
                Extr_LowSpd_State = LOW_SPEED_STATE_IDLE;

                /* Entry 'IDLE': '<S171>:13' */
                Extraction_DW.DistributionRequested = false;
                Extraction_DW.AntifoamRequested = false;

                /* Event: '<S171>:254' */
                Extraction_DW.Extr_LowSpd_EvStopEventCounter++;
            }
        }
    }
}

/* Function for Chart: '<S122>/Extr_LowSpd_SupervisorLogic' */
static void Extraction_chartstep_c15_Extraction(void)
{
    Extraction_c15_Extraction();
}

/* Function for Chart: '<S125>/Extr_LowSpd_LoadMeasLogic' */
static float32 Extraction_CalculateAverageLoadMass(const float32 inertia_buf[15],
    uint8 n_samp)
{
    float32 inertia_avg;
    float32 accum;
    uint8 avg_count;
    float32 MaxJ;
    float32 MinJ;
    uint8 c_ii;

    /* MATLAB Function 'CalculateAverageLoadMass': '<S145>:338' */
    /* '<S145>:338:3' */
    accum = 0.0F;

    /* '<S145>:338:4' */
    avg_count = 0U;
    if (n_samp >= 3)
    {
        /* '<S145>:338:6' */
        /* '<S145>:338:8' */
        MaxJ = -3.402823466E+38F;

        /* '<S145>:338:9' */
        MinJ = 3.402823466E+38F;

        /* '<S145>:338:11' */
        for (c_ii = 1U; c_ii <= n_samp; c_ii++)
        {
            /* '<S145>:338:11' */
            if (inertia_buf[c_ii - 1] < MinJ)
            {
                /* '<S145>:338:12' */
                /* '<S145>:338:13' */
                MinJ = inertia_buf[c_ii - 1];
            }

            if (inertia_buf[c_ii - 1] > MaxJ)
            {
                /* '<S145>:338:15' */
                /* '<S145>:338:16' */
                MaxJ = inertia_buf[c_ii - 1];
            }
        }

        /* '<S145>:338:20' */
        for (c_ii = 1U; c_ii <= n_samp; c_ii++)
        {
            /* '<S145>:338:20' */
            if (inertia_buf[c_ii - 1] != MaxJ && inertia_buf[c_ii - 1] != MinJ)
            {
                /* '<S145>:338:21' */
                /* '<S145>:338:22' */
                accum += inertia_buf[c_ii - 1];

                /* '<S145>:338:23' */
                avg_count++;
            }
        }

        if (avg_count == 0)
        {
            /* '<S145>:338:27' */
            /* '<S145>:338:28' */
            inertia_avg = 0.0F;
        }
        else
        {
            /* '<S145>:338:32' */
            inertia_avg = accum / (float32)avg_count;
        }
    }
    else
    {
        /* '<S145>:338:34' */
        for (c_ii = 1U; c_ii <= n_samp; c_ii++)
        {
            /* '<S145>:338:34' */
            /* '<S145>:338:35' */
            accum += inertia_buf[c_ii - 1];

            /* '<S145>:338:36' */
            avg_count++;
        }

        /* '<S145>:338:39' */
        inertia_avg = accum / (float32)avg_count;
    }

    return inertia_avg;
}

/* Function for Chart: '<S125>/Extr_LowSpd_LoadMeasLogic' */
static float32 Extraction_CalculateLoadMass_o3(float32 J, float32 coeffs_A,
    float32 coeffs_B, float32 coeffs_C)
{
    float32 mass;

    /* MATLAB Function 'CalculateLoadMass': '<S145>:325' */
    /*  Converts the ienrtia measurement to mass in Kg. */
    /* '<S145>:325:4' */
    mass = coeffs_A * (J * J) + coeffs_B * J + coeffs_C;
    if (mass < 0.0F)
    {
        /* '<S145>:325:6' */
        /* '<S145>:325:7' */
        mass = 0.0F;
    }

    return mass;
}

/* Function for Chart: '<S125>/Extr_LowSpd_LoadMeasLogic' */
static float32 Extraction_ConvertMgrToGrams_ey(float32 ub, float32 coeffs_K2,
    float32 coeffs_K4, sint16 coeffs_K1, sint16 coeffs_K3)
{
    /* MATLAB Function 'ConvertMgrToGrams': '<S145>:350' */
    /*  Converts the unbalance measurement from moment (m*g*r) in N*m to mass in grams. */
    /* '<S145>:350:4' */
    return ((float32)coeffs_K1 + 1000.0F * coeffs_K2 * ub) * coeffs_K4 +
        (float32)coeffs_K3;
}

/* Function for Chart: '<S125>/Extr_LowSpd_LoadMeasLogic' */
static void Extraction_c9_Extraction(void)
{
    uint8 ii;
    sint32 b_previousEvent;
    static const uint8 b[8] =
    {
        1U, 1U, 0U, 0U, 0U, 0U, 1U, 1U
    };

    uint8 tmp[8];

    /* During: Extraction/Extr_LowSpd/Extr_LowSpd_Algorithms/Extr_LowSpd_LoadMeas/Extr_LowSpd_LoadMeasLogic */
    switch (Extraction_DW.is_c9_Extraction)
    {
      case Extraction_IN_IDLE_gt:
        /* During 'IDLE': '<S145>:398' */
        switch (Extraction_DW.sfEvent_ak)
        {
          case Extraction_event_Extr_LowSpd_EvLoadMeasStart:
            /* Transition: '<S145>:176' */
            if ((*GESE_Extr_Setup).PE_Enabled != 0)
            {
                /* Transition: '<S145>:357' */
                Extraction_DW.is_c9_Extraction =
                    Extraction_IN_RUNNING_PE_MEASUREMENT;

                /* Entry 'RUNNING_PE_MEASUREMENT': '<S145>:171' */
                Extraction_DW.InertiaSampleCount = 0U;
                for (ii = 1U; ii <= (*GESE_Extr_Setup).Load_Mass_Count_Average;
                        ii++)
                {
                    Extraction_DW.InertiaBuffer[ii - 1] = 0.0F;
                }

                /* Entry Internal 'RUNNING_PE_MEASUREMENT': '<S145>:171' */
                /* Transition: '<S145>:272' */
                if (Extraction_DW.is_RUNNING_PE_MEASUREMENT ==
                        Extraction_IN_PARAM_EST_SETUP)
                {
                }
                else
                {
                    Extraction_DW.is_RUNNING_PE_MEASUREMENT =
                        Extraction_IN_PARAM_EST_SETUP;
                    Extraction_DW.temporalCounter_i1_pw = 0U;
                    Extr_LowSpd_LoadMeasState = LOAD_MEAS_STATE_PARAM_EST_SETUP;

                    /* Entry 'PARAM_EST_SETUP': '<S145>:208' */
                    tmp[0] = 0U;
                    tmp[1] = 0U;
                    tmp[2] = 0U;
                    tmp[3] = 0U;
                    tmp[4] = 0U;
                    tmp[5] = 0U;
                    tmp[6] = 0U;
                    tmp[7] = 0U;
                    Extraction_SelectParamEstOutputs(tmp);
                    Extraction_ParamEstControl(PE_RESET);
                }
            }
            else
            {
                /* Transition: '<S145>:375' */
                Extraction_DW.is_c9_Extraction =
                    Extraction_IN_WAITING_BALANCE_INDEX;
                Extr_LowSpd_LoadMeasState =
                    LOAD_MEAS_STATE_WAITING_BALANCE_INDEX;
            }
            break;

          case Extraction_event_Extr_LowSpd_EvResetLoadMass:
            Extraction_B.Extr_LowSpd_LoadMass_ec = -1.0F;
            break;
        }
        break;

      case Extraction_IN_RUNNING_PE_MEASUREMENT:
        /* During 'RUNNING_PE_MEASUREMENT': '<S145>:171' */
        if (Extraction_DW.sfEvent_ak == Extraction_event_Extr_LowSpd_EvStop ||
                Extraction_DW.sfEvent_ak == Extraction_event_LocalStop)
        {
            /* Transition: '<S145>:199' */
            /* Exit Internal 'RUNNING_PE_MEASUREMENT': '<S145>:171' */
            if (Extraction_DW.is_RUNNING_PE_MEASUREMENT ==
                    Extraction_IN_AVERAGING_MEASUREMENTS)
            {
                /* Exit 'AVERAGING_MEASUREMENTS': '<S145>:385' */
                Extraction_DW.InertiaSampleCount++;
                Extraction_DW.InertiaBuffer[Extraction_DW.InertiaSampleCount - 1]
                    = Extraction_DW.InertiaSum / (float32)
                    Extraction_DW.SampleCnt;
                Extraction_DW.is_RUNNING_PE_MEASUREMENT = 0U;
            }
            else
            {
                Extraction_DW.is_RUNNING_PE_MEASUREMENT = 0U;
            }

            Extraction_DW.is_c9_Extraction = Extraction_IN_VERIFYING_LIMITS;
            Extr_LowSpd_LoadMeasState = LOAD_MEAS_STATE_VERIFYING_LIMITS;

            /* Entry 'VERIFYING_LIMITS': '<S145>:172' */
            if ((*GESE_Extr_Setup).CWLD_Enabled != 0)
            {
                if (Extraction_B.Extr_LowSpd_LoadMass_ec > (float32)
                        (*GESE_Extr_CleanWasherSetup).Load_Mass_Threshold_x10 /
                        10.0F)
                {
                    /* Event: '<S145>:397' */
                    Extraction_DW.Extr_LowSpd_EvCleanWasherExceptionEventCounter
                        ++;
                    Extraction_SetFault
                        (EXTR_FAULT_CLEAN_WASHER_LOAD_MASS_EXCEEDED);
                }

                if (Extraction_B.Extr_LowSpd_Unbalance_oe >
                        (*GESE_Extr_CleanWasherSetup).Unbalance_Threshold)
                {
                    /* Event: '<S145>:397' */
                    Extraction_DW.Extr_LowSpd_EvCleanWasherExceptionEventCounter
                        ++;
                    Extraction_SetFault
                        (EXTR_FAULT_CLEAN_WASHER_UNBALANCE_EXCEEDED);
                }
            }
        }
        else if (Extraction_DW.sfEvent_ak ==
                 Extraction_event_Extr_LowSpd_EvResetLoadMass)
        {
            /* Transition: '<S145>:330' */
            /* Exit Internal 'RUNNING_PE_MEASUREMENT': '<S145>:171' */
            if (Extraction_DW.is_RUNNING_PE_MEASUREMENT ==
                    Extraction_IN_AVERAGING_MEASUREMENTS)
            {
                /* Exit 'AVERAGING_MEASUREMENTS': '<S145>:385' */
                Extraction_DW.InertiaSampleCount++;
                Extraction_DW.InertiaBuffer[Extraction_DW.InertiaSampleCount - 1]
                    = Extraction_DW.InertiaSum / (float32)
                    Extraction_DW.SampleCnt;
                Extraction_DW.is_RUNNING_PE_MEASUREMENT = 0U;
            }
            else
            {
                Extraction_DW.is_RUNNING_PE_MEASUREMENT = 0U;
            }

            Extraction_B.Extr_LowSpd_LoadMass_ec = 0.0F;
            Extraction_DW.is_c9_Extraction = Extraction_IN_IDLE_gt;
            Extr_LowSpd_LoadMeasState = LOAD_MEAS_STATE_IDLE;
        }
        else
        {
            switch (Extraction_DW.is_RUNNING_PE_MEASUREMENT)
            {
              case Extraction_IN_AVERAGING_MEASUREMENTS:
                /* During 'AVERAGING_MEASUREMENTS': '<S145>:385' */
                if (Extraction_DW.sfEvent_ak == Extraction_event_ticks_hm &&
                        Extraction_DW.temporalCounter_i1_pw >= (uint16)(3U *
                        ((uint16)FS_EXTRACTION)) || ((*GESE_Extr_Setup).
                        OR_Enabled != 0 && ((boolean_T)
                                            EXTR_FEATURE_OPT_RAMP_ENABLED) ||
                        (*GESE_Extr_Setup).LMS_Enabled != 0))
                {
                    /* Transition: '<S145>:386' */
                    /* Transition: '<S145>:391' */
                    /* Transition: '<S145>:390' */
                    /* Transition: '<S145>:388' */
                    if (!((*GESE_Extr_Setup).LMS_Enabled != 0) ||
                            !(Extraction_DW.InertiaSampleCount < (uint8)
                              ((*GESE_Extr_Setup).Load_Mass_Count_Average - 1U)))
                    {
                        /* Transition: '<S145>:321' */
                        /* Transition: '<S145>:354' */
                        /* Transition: '<S145>:410' */
                        if (Extraction_DW.is_RUNNING_PE_MEASUREMENT !=
                                Extraction_IN_AVERAGING_MEASUREMENTS)
                        {
                        }
                        else
                        {
                            /* Exit 'AVERAGING_MEASUREMENTS': '<S145>:385' */
                            Extraction_DW.InertiaSampleCount++;
                            Extraction_DW.InertiaBuffer[Extraction_DW.InertiaSampleCount
                                - 1] = Extraction_DW.InertiaSum / (float32)
                                Extraction_DW.SampleCnt;
                            Extraction_DW.is_RUNNING_PE_MEASUREMENT = 0U;
                        }

                        if (Extraction_DW.is_RUNNING_PE_MEASUREMENT ==
                                Extraction_IN_MEASUREMENT_COMPLETE)
                        {
                        }
                        else
                        {
                            Extraction_DW.is_RUNNING_PE_MEASUREMENT =
                                Extraction_IN_MEASUREMENT_COMPLETE;
                            Extr_LowSpd_LoadMeasState =
                                LOAD_MEAS_STATE_MEASUREMENT_COMPLETE;

                            /* Entry 'MEASUREMENT_COMPLETE': '<S145>:90' */
                            Extraction_B.Extr_LowSpd_LoadMass_ec =
                                Extraction_CalculateLoadMass_o3
                                (Extraction_CalculateAverageLoadMass
                                 (Extraction_DW.InertiaBuffer,
                                  Extraction_DW.InertiaSampleCount),
                                 (*GESE_Extr_MassConvCoeffs).A,
                                 (*GESE_Extr_MassConvCoeffs).B,
                                 (*GESE_Extr_MassConvCoeffs).C);
                            Extraction_B.Extr_LowSpd_Unbalance_oe = (uint16)
                                roundf(Extraction_ConvertMgrToGrams_ey
                                       (Extraction_DW.UbSum / (float32)
                                        Extraction_DW.SampleCnt,
                                        (*GESE_Extr_UnbConvCoeffs).K2,
                                        (*GESE_Extr_UnbConvCoeffs).K4,
                                        (*GESE_Extr_UnbConvCoeffs).K1,
                                        (*GESE_Extr_UnbConvCoeffs).K3));

                            /* Event: '<S145>:213' */
                            Extraction_DW.Extr_LowSpd_EvLoadMeasCompleteEventCounter
                                ++;

                            /* Event: '<S145>:278' */
                            b_previousEvent = Extraction_DW.sfEvent_ak;
                            Extraction_DW.sfEvent_ak =
                                Extraction_event_LocalStop;
                            Extraction_c9_Extraction();
                            Extraction_DW.sfEvent_ak = b_previousEvent;
                        }
                    }
                    else
                    {
                        /* Transition: '<S145>:335' */
                        /* Transition: '<S145>:355' */
                        /* Transition: '<S145>:412' */
                        if (Extraction_DW.is_RUNNING_PE_MEASUREMENT !=
                                Extraction_IN_AVERAGING_MEASUREMENTS)
                        {
                        }
                        else
                        {
                            /* Exit 'AVERAGING_MEASUREMENTS': '<S145>:385' */
                            Extraction_DW.InertiaSampleCount++;
                            Extraction_DW.InertiaBuffer[Extraction_DW.InertiaSampleCount
                                - 1] = Extraction_DW.InertiaSum / (float32)
                                Extraction_DW.SampleCnt;
                            Extraction_DW.is_RUNNING_PE_MEASUREMENT = 0U;
                        }

                        if (Extraction_DW.is_RUNNING_PE_MEASUREMENT ==
                                Extraction_IN_WAITING_CONVERGENCE)
                        {
                        }
                        else
                        {
                            Extraction_DW.is_RUNNING_PE_MEASUREMENT =
                                Extraction_IN_WAITING_CONVERGENCE;
                            Extraction_DW.temporalCounter_i1_pw = 0U;
                            Extr_LowSpd_LoadMeasState =
                                LOAD_MEAS_STATE_WAITING_CONVERGENCE;

                            /* Entry 'WAITING_CONVERGENCE': '<S145>:264' */
                            Extraction_ParamEstControl(PE_COV_RESET);
                            Extraction_ParamEstExcControl(EXC_ENABLE);
                            Extraction_SelectParamEstOutputs(b);
                            Extraction_DW.SampleCnt = 0U;
                            Extraction_DW.InertiaSum = 0.0F;
                            Extraction_DW.UbSum = 0.0F;
                        }
                    }
                }
                else
                {
                    Extraction_DW.SampleCnt++;
                    Extraction_DW.InertiaSum += Extr_Inputs_ParamEstData.Inertia;
                    Extraction_DW.UbSum += Extr_Inputs_ParamEstData.UB;
                }
                break;

              case Extraction_IN_MEASUREMENT_COMPLETE:
                /* During 'MEASUREMENT_COMPLETE': '<S145>:90' */
                break;

              case Extraction_IN_PARAM_EST_SETUP:
                /* During 'PARAM_EST_SETUP': '<S145>:208' */
                if (Extraction_DW.sfEvent_ak == Extraction_event_ticks_hm &&
                        Extraction_DW.temporalCounter_i1_pw >= (uint16)((uint32)
                        Extraction_B.UpdatePEAppSpecificSetup.PEAppSpecificConfig.Stabilization_Time
                     * ((uint16)FS_EXTRACTION)))
                {
                    /* Transition: '<S145>:154' */
                    Extraction_DW.is_RUNNING_PE_MEASUREMENT =
                        Extraction_IN_WAITING_CONVERGENCE;
                    Extraction_DW.temporalCounter_i1_pw = 0U;
                    Extr_LowSpd_LoadMeasState =
                        LOAD_MEAS_STATE_WAITING_CONVERGENCE;

                    /* Entry 'WAITING_CONVERGENCE': '<S145>:264' */
                    Extraction_ParamEstControl(PE_COV_RESET);
                    Extraction_ParamEstExcControl(EXC_ENABLE);
                    Extraction_SelectParamEstOutputs(b);
                    Extraction_DW.SampleCnt = 0U;
                    Extraction_DW.InertiaSum = 0.0F;
                    Extraction_DW.UbSum = 0.0F;
                }
                break;

              case Extraction_IN_WAITING_CONVERGENCE:
                /* During 'WAITING_CONVERGENCE': '<S145>:264' */
                if (Extraction_DW.sfEvent_ak == Extraction_event_ticks_hm &&
                        Extraction_DW.temporalCounter_i1_pw >= (uint16)((uint32)
                        roundf((float32)
                               Extraction_B.UpdatePEAppSpecificSetup.PEAppSpecificConfig.Cov_Reset_Wait_x10
                               / 10.0F) * ((uint16)FS_EXTRACTION)))
                {
                    /* Transition: '<S145>:265' */
                    Extraction_DW.is_RUNNING_PE_MEASUREMENT =
                        Extraction_IN_AVERAGING_MEASUREMENTS;
                    Extraction_DW.temporalCounter_i1_pw = 0U;
                    Extr_LowSpd_LoadMeasState =
                        LOAD_MEAS_STATE_AVERAGING_MEASUREMENTS;

                    /* Entry 'AVERAGING_MEASUREMENTS': '<S145>:385' */
                    Extraction_DW.SampleCnt++;
                    Extraction_DW.InertiaSum += Extr_Inputs_ParamEstData.Inertia;
                    Extraction_DW.UbSum += Extr_Inputs_ParamEstData.UB;
                }
                break;
            }
        }
        break;

      case Extraction_IN_VERIFYING_LIMITS:
        /* During 'VERIFYING_LIMITS': '<S145>:172' */
        /* Transition: '<S145>:405' */
        /* Transition: '<S145>:407' */
        /* Transition: '<S145>:408' */
        Extraction_DW.is_c9_Extraction = Extraction_IN_IDLE_gt;
        Extr_LowSpd_LoadMeasState = LOAD_MEAS_STATE_IDLE;
        break;

      case Extraction_IN_WAITING_BALANCE_INDEX:
        /* During 'WAITING_BALANCE_INDEX': '<S145>:374' */
        if (Extr_Inputs_UnbalanceIndex > 0)
        {
            /* Transition: '<S145>:377' */
            if ((*GESE_Extr_Setup).CM_Enabled != 0)
            {
                /* Transition: '<S145>:381' */
                /*  Converted balance index */
                /* Transition: '<S145>:384' */
                /* Simulink Function 'ConvertBalIndexToGrams': '<S145>:371' */
                Extraction_B.ub = Extr_Inputs_UnbalanceIndex;

                /* Outputs for Function Call SubSystem: '<S145>/ConvertBalIndexToGrams' */
                /* Lookup_n-D: '<S146>/BalanceIndexToGrams' */
                Extraction_B.BalanceIndexToGrams = look1_iu16lfyu16DfIf_binlcas
                    (Extraction_B.ub,
                     GESE_Extr_BalanceIndexConversionLut.Raw_Values,
                     GESE_Extr_BalanceIndexConversionLut.Converted_Values,
                     GESE_Extr_BalanceIndexConversionLut.Count - 1U);

                /* End of Outputs for SubSystem: '<S145>/ConvertBalIndexToGrams' */
                Extraction_B.Extr_LowSpd_Unbalance_oe =
                    Extraction_B.BalanceIndexToGrams;
            }
            else
            {
                /* Transition: '<S145>:379' */
                Extraction_B.Extr_LowSpd_Unbalance_oe =
                    Extr_Inputs_UnbalanceIndex;

                /* Transition: '<S145>:401' */
            }

            /* Transition: '<S145>:403' */
            /* Transition: '<S145>:361' */
            /* Event: '<S145>:213' */
            Extraction_DW.Extr_LowSpd_EvLoadMeasCompleteEventCounter++;
            Extraction_DW.is_c9_Extraction = Extraction_IN_VERIFYING_LIMITS;
            Extr_LowSpd_LoadMeasState = LOAD_MEAS_STATE_VERIFYING_LIMITS;

            /* Entry 'VERIFYING_LIMITS': '<S145>:172' */
            if ((*GESE_Extr_Setup).CWLD_Enabled != 0)
            {
                if (Extraction_B.Extr_LowSpd_LoadMass_ec > (float32)
                        (*GESE_Extr_CleanWasherSetup).Load_Mass_Threshold_x10 /
                        10.0F)
                {
                    /* Event: '<S145>:397' */
                    Extraction_DW.Extr_LowSpd_EvCleanWasherExceptionEventCounter
                        ++;
                    Extraction_SetFault
                        (EXTR_FAULT_CLEAN_WASHER_LOAD_MASS_EXCEEDED);
                }

                if (Extraction_B.Extr_LowSpd_Unbalance_oe >
                        (*GESE_Extr_CleanWasherSetup).Unbalance_Threshold)
                {
                    /* Event: '<S145>:397' */
                    Extraction_DW.Extr_LowSpd_EvCleanWasherExceptionEventCounter
                        ++;
                    Extraction_SetFault
                        (EXTR_FAULT_CLEAN_WASHER_UNBALANCE_EXCEEDED);
                }
            }
        }
        else
        {
            if (Extraction_DW.sfEvent_ak == Extraction_event_Extr_LowSpd_EvStop)
            {
                /* Transition: '<S145>:378' */
                Extraction_DW.is_c9_Extraction = Extraction_IN_IDLE_gt;
                Extr_LowSpd_LoadMeasState = LOAD_MEAS_STATE_IDLE;
            }
        }
        break;
    }
}

/* Function for Chart: '<S125>/Extr_LowSpd_LoadMeasLogic' */
static void Extraction_chartstep_c9_Extraction(void)
{
    Extraction_c9_Extraction();
}

/* Function for Chart: '<S124>/Extr_LowSpd_BasicUnbCheck' */
static void Extraction_broadcast_Extr_LowSpd_EvUnbChkComplete_local(void)
{
    sint32 b_previousEvent;

    /* Event: '<S141>:56' */
    b_previousEvent = Extraction_DW.sfEvent_k0;
    Extraction_DW.sfEvent_k0 =
        Extraction_event_Extr_LowSpd_EvUnbChkComplete_local;
    Extraction_c42_Extraction();
    Extraction_DW.sfEvent_k0 = b_previousEvent;
}

/* Function for Chart: '<S124>/Extr_LowSpd_BasicUnbCheck' */
static void Extraction_enter_atomic_COMPLETE_ST(void)
{
    /* Entry 'COMPLETE_ST': '<S141>:21' */
    /* Event: '<S141>:22' */
    Extraction_DW.Extr_LowSpd_EvUnbChkCompleteEventCounter++;
    Extraction_broadcast_Extr_LowSpd_EvUnbChkComplete_local();
}

/* Function for Chart: '<S124>/Extr_LowSpd_BasicUnbCheck' */
static void Extraction_c42_Extraction(void)
{
    uint8 b_index;
    uint16 tmp[25];
    sint32 i;
    sint16 tmp_0;
    boolean_T guard1 = false;
    boolean_T guard2 = false;
    sint32 exitg1;

    /* During: Extraction/Extr_LowSpd/Extr_LowSpd_Algorithms/Extr_LowSpd_BasicUnbChk/Extr_LowSpd_BasicUnbCheck */
    if (Extraction_DW.is_active_c42_Extraction == 0U)
    {
        /* Entry: Extraction/Extr_LowSpd/Extr_LowSpd_Algorithms/Extr_LowSpd_BasicUnbChk/Extr_LowSpd_BasicUnbCheck */
        Extraction_DW.is_active_c42_Extraction = 1U;

        /* Entry Internal: Extraction/Extr_LowSpd/Extr_LowSpd_Algorithms/Extr_LowSpd_BasicUnbChk/Extr_LowSpd_BasicUnbCheck */
        /* Transition: '<S141>:2' */
        Extraction_DW.is_c42_Extraction = Extraction_IN_IDLE_jy;
    }
    else
    {
        guard1 = false;
        guard2 = false;
        switch (Extraction_DW.is_c42_Extraction)
        {
          case Extraction_IN_BASIC_UNB_CHK:
            /* During 'BASIC_UNB_CHK': '<S141>:3' */
            if (Extraction_DW.sfEvent_k0 ==
                    Extraction_event_Extr_LowSpd_EvUnbChkComplete_local ||
                    Extraction_DW.sfEvent_k0 ==
                    Extraction_event_Extr_HighSpd_EvStopAll_ks)
            {
                /* Transition: '<S141>:39' */
                /* Exit Internal 'BASIC_UNB_CHK': '<S141>:3' */
                Extraction_DW.is_BASIC_UNB_CHK = 0U;
                Extraction_DW.is_c42_Extraction = Extraction_IN_IDLE_jy;
            }
            else
            {
                switch (Extraction_DW.is_BASIC_UNB_CHK)
                {
                  case Extraction_IN_COMPLETE_ST:
                    /* During 'COMPLETE_ST': '<S141>:21' */
                    break;

                  case Extraction_IN_START_REBAL_ST:
                    /* During 'START_REBAL_ST': '<S141>:36' */
                    /* Transition: '<S141>:38' */
                    /* Transition: '<S141>:60' */
                    Extraction_DW.is_BASIC_UNB_CHK = Extraction_IN_COMPLETE_ST;
                    Extraction_enter_atomic_COMPLETE_ST();
                    break;
                }
            }
            break;

          case Extraction_IN_IDLE_jy:
            /* During 'IDLE': '<S141>:1' */
            if (Extraction_DW.sfEvent_k0 ==
                    Extraction_event_Extr_LowSpd_StartBasicUnbChkEv)
            {
                /* Transition: '<S141>:4' */
                Extraction_DW.is_c42_Extraction = Extraction_IN_BASIC_UNB_CHK;

                /* Entry 'BASIC_UNB_CHK': '<S141>:3' */
                Extraction_CalculateUnbalanceTargetArray(tmp);
                for (i = 0; i < 25; i++)
                {
                    Extr_LowSpd_UnbTargetArray[i] = tmp[i];
                }

                Extr_LowSpd_UnbChkSpdLimit = MAX_uint16_T;

                /* Entry Internal 'BASIC_UNB_CHK': '<S141>:3' */
                /* Transition: '<S141>:6' */
                /* 70786 */
                if (Extraction_B.Extr_LowSpd_RebalCnt_lp >
                        GESE_Extr_RebalAttemptsArray[GESE_Extr_OptRampUnbLut_Size
                        [0] - 1])
                {
                    /* Transition: '<S141>:9' */
                    tmp_0 = Extr_LowSpd_DistSpeed;
                    if (Extr_LowSpd_DistSpeed < 0)
                    {
                        tmp_0 = 0;
                    }

                    Extr_LowSpd_UnbChkSpdLimit = (uint16)tmp_0;

                    /* Transition: '<S141>:70' */
                    guard1 = true;
                }
                else
                {
                    /* Transition: '<S141>:10' */
                    /*  select the right row  */
                    /* Transition: '<S141>:13' */
                    b_index = 0U;
                    do
                    {
                        exitg1 = 0;
                        if (b_index <= GESE_Extr_OptRampUnbLut_Size[0] - 1)
                        {
                            /* Transition: '<S141>:11' */
                            /* Transition: '<S141>:12' */
                            /* 70782 */
                            if (Extraction_B.Extr_LowSpd_Unbalance_oe <=
                                    Extr_LowSpd_UnbTargetArray[b_index] &&
                                    b_index == 0)
                            {
                                /* Transition: '<S141>:24' */
                                guard1 = true;
                                exitg1 = 1;
                            }
                            else if (Extraction_B.Extr_LowSpd_Unbalance_oe <=
                                     Extr_LowSpd_UnbTargetArray[b_index] &&
                                     b_index > 0)
                            {
                                /* Transition: '<S141>:26' */
                                /* 70784 */
                                if (Extraction_B.Extr_LowSpd_RebalCnt_lp >=
                                        GESE_Extr_RebalAttemptsArray[b_index - 1])
                                {
                                    /* Constant: '<S124>/OptRampUnbLutSize' */
                                    /* Transition: '<S141>:28' */
                                    Extr_LowSpd_UnbChkSpdLimit = (uint16)
                                        (GESE_Extr_TargetSpeedArray_Div10[GESE_Extr_OptRampUnbLut_Size
                                         [0] - b_index] * 10);

                                    /* Transition: '<S141>:62' */
                                    guard1 = true;
                                }
                                else
                                {
                                    /* Transition: '<S141>:37' */
                                    /* Transition: '<S141>:68' */
                                    guard2 = true;
                                }

                                exitg1 = 1;
                            }
                            else
                            {
                                /* Transition: '<S141>:14' */
                                b_index++;
                            }
                        }
                        else
                        {
                            /* Transition: '<S141>:15' */
                            /* req70783 */
                            /* Transition: '<S141>:66' */
                            /* Transition: '<S141>:65' */
                            guard2 = true;
                            exitg1 = 1;
                        }
                    }
                    while (exitg1 == 0);
                }
            }
            break;
        }

        if (guard2)
        {
            if (Extraction_DW.is_BASIC_UNB_CHK == Extraction_IN_START_REBAL_ST)
            {
            }
            else
            {
                Extraction_DW.is_BASIC_UNB_CHK = Extraction_IN_START_REBAL_ST;

                /* Entry 'START_REBAL_ST': '<S141>:36' */
                /* Event: '<S141>:31' */
                Extraction_DW.Extr_LowSpd_EvUnbChkRebalStartEventCounter++;
                Extraction_broadcast_Extr_LowSpd_EvUnbChkComplete_local();
            }
        }

        if (guard1)
        {
            if (Extraction_DW.is_BASIC_UNB_CHK == Extraction_IN_COMPLETE_ST)
            {
            }
            else
            {
                Extraction_DW.is_BASIC_UNB_CHK = Extraction_IN_COMPLETE_ST;
                Extraction_enter_atomic_COMPLETE_ST();
            }
        }
    }
}

/* Function for Chart: '<S124>/Extr_LowSpd_BasicUnbCheck' */
static void Extraction_chartstep_c42_Extraction(void)
{
    Extraction_c42_Extraction();
}

/* Function for Chart: '<S126>/MDDPLogic' */
static void Extraction_enter_atomic_PLATEAU_END(void)
{
    sint32 b_previousEvent;

    /* Entry 'PLATEAU_END': '<S147>:228' */
    /* Event: '<S147>:292' */
    Extraction_DW.Extr_LowSpd_EvMddpCompleteEventCounter++;

    /* Event: '<S147>:256' */
    b_previousEvent = Extraction_DW.sfEvent_o4;
    Extraction_DW.sfEvent_o4 = Extraction_event_localStop_ix;
    Extraction_c11_Extraction();
    Extraction_DW.sfEvent_o4 = b_previousEvent;
}

/* Function for Chart: '<S126>/MDDPLogic' */
static void Extraction_c11_Extraction(void)
{
    float32 finalAccum;
    uint8 rtb_loadMassPrelookup_k;
    float32 rtb_loadMassPrelookup_f;
    uint16 tmp;

    /* During: Extraction/Extr_LowSpd/Extr_LowSpd_Algorithms/Extr_LowSpd_MDDP/MDDPLogic */
    switch (Extraction_DW.is_c11_Extraction)
    {
      case Extraction_IN_IDLE_gt:
        /* During 'IDLE': '<S147>:172' */
        if (Extraction_DW.sfEvent_o4 == Extraction_event_evStart_eo)
        {
            /* Transition: '<S147>:176' */
            Extraction_DW.is_c11_Extraction = Extraction_IN_STATE_RUNNING;

            /* Entry Internal 'STATE_RUNNING': '<S147>:171' */
            /* Transition: '<S147>:302' */
            if (Extraction_DW.is_STATE_RUNNING ==
                    Extraction_IN_CALC_MDDP_PARAMETERS)
            {
            }
            else
            {
                Extraction_DW.is_STATE_RUNNING =
                    Extraction_IN_CALC_MDDP_PARAMETERS;
                Extr_LowSpd_MddpState = MDDP_STATE_CALC_MDDP_PARAMETERS;

                /* Entry 'CALC_MDDP_PARAMETERS': '<S147>:253' */
                /* Simulink Function 'PlateauSetup': '<S147>:230' */
                Extraction_B.mass_m2 = Extraction_B.Extr_LowSpd_LoadMass_ec;

                /* Outputs for Function Call SubSystem: '<S147>/PlateauSetup' */
                /* Outputs for Atomic SubSystem: '<S148>/CalcMDDPParameters' */
                /* Gain: '<S149>/x10' */
                rtb_loadMassPrelookup_f = 10.0F * Extraction_B.mass_m2;
                if (rtb_loadMassPrelookup_f < 65536.0F)
                {
                    if (rtb_loadMassPrelookup_f >= 0.0F)
                    {
                        tmp = (uint16)rtb_loadMassPrelookup_f;
                    }
                    else
                    {
                        tmp = 0U;
                    }
                }
                else
                {
                    tmp = MAX_uint16_T;
                }

                /* End of Gain: '<S149>/x10' */

                /* PreLookup: '<S149>/loadMassPrelookup' */
                rtb_loadMassPrelookup_k = plook_u8u16f_binc(tmp,
                    (&(GESE_Extr_MddpLoadMassX10_BP.BP[0])),
                    GESE_Extr_MddpLoadMassX10_BP.N - 1U,
                    &rtb_loadMassPrelookup_f);

                /* Interpolation_n-D: '<S149>/mddpDurationLUT' incorporates:
                 *  Constant: '<S149>/mddpDurationLUTData'
                 */
                finalAccum = intrp1d_u8ffu8fl(rtb_loadMassPrelookup_k,
                    rtb_loadMassPrelookup_f, (&(GESE_Extr_MddpDuration_LUT[0])));
                Extraction_B.mddpDurationLUT = (uint8)finalAccum;

                /* Interpolation_n-D: '<S149>/mddpRampLUT' incorporates:
                 *  Constant: '<S149>/mddpRampLUTData'
                 */
                finalAccum = intrp1d_u8ffu8fl(rtb_loadMassPrelookup_k,
                    rtb_loadMassPrelookup_f, (&(GESE_Extr_MddpRamp_LUT[0])));
                Extraction_B.mddpRampLUT = (uint8)finalAccum;

                /* Interpolation_n-D: '<S149>/mddpSpeedLUT' incorporates:
                 *  Constant: '<S149>/mddpSpeedLUTData'
                 */
                finalAccum = intrp1d_u16ffu8fl(rtb_loadMassPrelookup_k,
                    rtb_loadMassPrelookup_f, (&(GESE_Extr_MddpSpeed_LUT[0])));
                Extraction_B.mddpSpeedLUT = (uint16)finalAccum;

                /* End of Outputs for SubSystem: '<S148>/CalcMDDPParameters' */
                /* End of Outputs for SubSystem: '<S147>/PlateauSetup' */
                Extr_LowSpd_MddpRmp = Extraction_B.mddpRampLUT;
                Extr_LowSpd_MddpSpd = Extraction_B.mddpSpeedLUT;
                Extraction_B.Extr_LowSpd_MddpDur_fy =
                    Extraction_B.mddpDurationLUT;
            }
        }
        break;

      case Extraction_IN_STATE_RUNNING:
        /* During 'STATE_RUNNING': '<S147>:171' */
        if (Extraction_DW.sfEvent_o4 == Extraction_event_evStop_m2 ||
                Extraction_DW.sfEvent_o4 == Extraction_event_localStop_ix)
        {
            /* Transition: '<S147>:199' */
            /* Exit Internal 'STATE_RUNNING': '<S147>:171' */
            Extraction_DW.is_STATE_RUNNING = 0U;
            Extraction_DW.is_c11_Extraction = Extraction_IN_IDLE_gt;
            Extr_LowSpd_MddpState = MDDP_STATE_IDLE;
        }
        else
        {
            switch (Extraction_DW.is_STATE_RUNNING)
            {
              case Extraction_IN_CALC_MDDP_PARAMETERS:
                /* During 'CALC_MDDP_PARAMETERS': '<S147>:253' */
                if (Extraction_B.Extr_LowSpd_MddpDur_fy <= 0)
                {
                    /* Transition: '<S147>:301' */
                    /* Transition: '<S147>:254' */
                    /* Transition: '<S147>:300' */
                    Extraction_DW.is_STATE_RUNNING = Extraction_IN_PLATEAU_END;
                    Extr_LowSpd_MddpState = MDDP_STATE_PLATEAU_END;
                    Extraction_enter_atomic_PLATEAU_END();
                }
                else
                {
                    /* Transition: '<S147>:222' */
                    Extraction_DW.is_STATE_RUNNING = Extraction_IN_RAMPING_ce;
                    Extr_LowSpd_MddpState = MDDP_STATE_RAMPING;

                    /* Entry 'RAMPING': '<S147>:303' */
                    Extraction_SetSpeedCommand((sint16)Extr_LowSpd_MddpSpd,
                        Extr_LowSpd_MddpRmp);
                }
                break;

              case Extraction_IN_PLATEAU_it:
                /* During 'PLATEAU': '<S147>:221' */
                if (Extraction_DW.sfEvent_o4 == Extraction_event_ticks_hm &&
                        Extraction_DW.temporalCounter_i1_f4 >= (uint16)((uint32)
                        Extraction_B.Extr_LowSpd_MddpDur_fy * ((uint16)
                        FS_EXTRACTION)))
                {
                    /* Transition: '<S147>:225' */
                    if ((*GESE_Extr_MddpSetup).Decelerate_To_Dist_Speed)
                    {
                        /* Transition: '<S147>:249' */
                        Extraction_DW.is_STATE_RUNNING =
                            Extraction_IN_RAMPING_DOWN;
                        Extr_LowSpd_MddpState = MDDP_STATE_RAMPING_DOWN;

                        /* Entry 'RAMPING_DOWN': '<S147>:248' */
                        Extraction_SetSpeedCommand(Extr_LowSpd_DistSpeed,
                            Extr_LowSpd_MddpRmp);
                    }
                    else
                    {
                        /* Transition: '<S147>:251' */
                        Extraction_DW.is_STATE_RUNNING =
                            Extraction_IN_PLATEAU_END;
                        Extr_LowSpd_MddpState = MDDP_STATE_PLATEAU_END;
                        Extraction_enter_atomic_PLATEAU_END();
                    }
                }
                break;

              case Extraction_IN_PLATEAU_END:
                /* During 'PLATEAU_END': '<S147>:228' */
                break;

              case Extraction_IN_RAMPING_ce:
                /* During 'RAMPING': '<S147>:303' */
                if (Extraction_DW.sfEvent_o4 ==
                        Extraction_event_Extr_Sup_EvSpeedReached_fi)
                {
                    /* Transition: '<S147>:304' */
                    Extraction_DW.is_STATE_RUNNING = Extraction_IN_PLATEAU_it;
                    Extraction_DW.temporalCounter_i1_f4 = 0U;
                    Extr_LowSpd_MddpState = MDDP_STATE_PLATEAU;
                }
                break;

              case Extraction_IN_RAMPING_DOWN:
                /* During 'RAMPING_DOWN': '<S147>:248' */
                if (Extraction_DW.sfEvent_o4 ==
                        Extraction_event_Extr_Sup_EvSpeedReached_fi)
                {
                    /* Transition: '<S147>:250' */
                    Extraction_DW.is_STATE_RUNNING = Extraction_IN_PLATEAU_END;
                    Extr_LowSpd_MddpState = MDDP_STATE_PLATEAU_END;
                    Extraction_enter_atomic_PLATEAU_END();
                }
                break;
            }
        }
        break;
    }
}

/* Function for Chart: '<S126>/MDDPLogic' */
static void Extraction_chartstep_c11_Extraction(void)
{
    Extraction_c11_Extraction();
}

/* Function for Chart: '<S64>/Extr_HighSpd_ExtUnbCheckLogic' */
static uint16 Extraction_GetUnbalanceTarget(const EXT_UNB_CHK_TARGET_TYPE
    unb_targ_set[25], uint8 b_index)
{
    sint32 tmp;

    /* MATLAB Function 'GetUnbalanceTarget': '<S71>:192' */
    /* '<S71>:192:3' */
    tmp = (sint32)(b_index + 1U);
    if ((uint32)tmp > 255U)
    {
        tmp = 255;
    }

    return unb_targ_set[tmp - 1].Unbalance_Level;
}

/* Function for Chart: '<S64>/Extr_HighSpd_ExtUnbCheckLogic' */
static uint8 Extraction_GetSpeedTarget(const EXT_UNB_CHK_TARGET_TYPE
    unb_targ_set[25], uint8 b_index)
{
    sint32 tmp;

    /* MATLAB Function 'GetSpeedTarget': '<S71>:199' */
    /* '<S71>:199:3' */
    tmp = (sint32)(b_index + 1U);
    if ((uint32)tmp > 255U)
    {
        tmp = 255;
    }

    return unb_targ_set[tmp - 1].Target_Speed_Div10;
}

/* Function for Chart: '<S64>/Extr_HighSpd_ExtUnbCheckLogic' */
static void Extraction_c45_Extraction(void)
{
    sint32 b_previousEvent;
    boolean_T guard1 = false;
    sint32 exitg1;

    /* During: Extraction/Extr_HighSpd/Extr_HighSpd_Algorithms/Extr_HighSpd_ExtUnbChk/Extr_HighSpd_ExtUnbCheckLogic */
    if (!Extraction_DW.doneDoubleBufferReInit)
    {
        Extraction_DW.doneDoubleBufferReInit = 1;
        Extraction_DW.MCI_RawUnbalanceCnt_prev = Extr_Inputs_UnbalanceIndexCnt;
        Extraction_DW.MCI_RawUnbalanceCnt_start = Extr_Inputs_UnbalanceIndexCnt;
    }

    guard1 = false;
    switch (Extraction_DW.is_c45_Extraction)
    {
      case Extraction_IN_IDLE_gt:
        /* During 'IDLE': '<S71>:1' */
        if (Extraction_DW.sfEvent_pr ==
                Extraction_event_Extr_HighSpd_EvStartExtndUnbChkEv)
        {
            /* Transition: '<S71>:4' */
            Extraction_DW.is_c45_Extraction = Extraction_IN_RUNNING_eh;

            /* Entry 'RUNNING': '<S71>:70' */
            Extraction_B.Extr_HighSpd_ExtUnbCalculated = false;

            /* Entry Internal 'RUNNING': '<S71>:70' */
            /* Transition: '<S71>:206' */
            Extraction_DW.is_RUNNING_im = Extraction_IN_SEARCHING_SPEED;
            Extr_HighSpd_ExtUnbState = EXT_UNB_CHECK_STATE_SEARCHING_SPEED;
        }
        break;

      case Extraction_IN_RUNNING_eh:
        /* During 'RUNNING': '<S71>:70' */
        if (Extraction_DW.sfEvent_pr ==
                Extraction_event_Extr_HighSpd_EvPlateauComplete_dk ||
                Extraction_DW.sfEvent_pr ==
                Extraction_event_Extr_HighSpd_EvStopAll_pq ||
                Extraction_DW.sfEvent_pr == Extraction_event_EvNoMatchFound ||
                Extraction_DW.sfEvent_pr == Extraction_event_EvStop_o5)
        {
            /* Transition: '<S71>:171' */
            /* Transition: '<S71>:182' */
            /* Transition: '<S71>:78' */
            /* Transition: '<S71>:39' */
            /* Exit Internal 'RUNNING': '<S71>:70' */
            /* Exit Internal 'MONITORING_UNBALANCE': '<S71>:93' */
            Extraction_DW.is_MONITORING_UNBALANCE = 0U;
            Extraction_DW.is_RUNNING_im = 0U;
            Extraction_DW.is_c45_Extraction = Extraction_IN_IDLE_gt;
            Extr_HighSpd_ExtUnbState = EXT_UNB_CHECK_STATE_IDLE;

            /* Entry 'IDLE': '<S71>:1' */
            Extraction_DW.tgt = 0U;
            Extraction_B.Extr_HighSpd_ExtUnbCalculated = true;
        }
        else
        {
            switch (Extraction_DW.is_RUNNING_im)
            {
              case Extraction_IN_MONITORING_UNBALANCE:
                /* During 'MONITORING_UNBALANCE': '<S71>:93' */
                switch (Extraction_DW.is_MONITORING_UNBALANCE)
                {
                  case Extraction_IN_CALCULATING_AVG_UNB:
                    /* During 'CALCULATING_AVG_UNB': '<S71>:142' */
                    if (Extr_HighSpd_ExtUnbSampleCnt == (*GESE_Extr_ExtUnbSetup)
                            .Sample_Count)
                    {
                        /* Transition: '<S71>:138' */
                        Extraction_B.Extr_HighSpd_ExtUnbCalculated = true;
                        if (Extr_HighSpd_ExtUnbAvg >
                                Extr_HighSpd_ExtUnbAvgThreshold)
                        {
                            /* Transition: '<S71>:141' */
                            /* Event: '<S71>:130' */
                            Extraction_DW.Extr_HighSpd_EvExtndChkFailedEventCounter
                                ++;

                            /* Event: '<S71>:181' */
                            b_previousEvent = Extraction_DW.sfEvent_pr;
                            Extraction_DW.sfEvent_pr =
                                Extraction_event_EvStop_o5;
                            Extraction_c45_Extraction();
                            Extraction_DW.sfEvent_pr = b_previousEvent;
                        }
                        else
                        {
                            /* Transition: '<S71>:143' */
                            /* Transition: '<S71>:203' */
                            Extr_HighSpd_ExtUnbSampleCnt = 0U;
                            Extraction_DW.ExtUnbSum = 0.0F;

                            /*  if the check does not fail, the check is
                               repeated for the entire duration of TSpin. */
                            guard1 = true;
                        }
                    }
                    else
                    {
                        if (Extraction_DW.MCI_RawUnbalanceCnt_prev !=
                                Extraction_DW.MCI_RawUnbalanceCnt_start)
                        {
                            /* Transition: '<S71>:213' */
                            b_previousEvent = Extr_HighSpd_ExtUnbSampleCnt + 1;
                            if (b_previousEvent > 255)
                            {
                                b_previousEvent = 255;
                            }

                            Extr_HighSpd_ExtUnbSampleCnt = (uint8)
                                b_previousEvent;
                            Extraction_DW.ExtUnbSum += (float32)
                                Extr_Inputs_UnbalanceIndex;
                            Extr_HighSpd_ExtUnbAvg = Extraction_DW.ExtUnbSum /
                                (float32)Extr_HighSpd_ExtUnbSampleCnt;
                            guard1 = true;
                        }
                    }
                    break;

                  case Extraction_IN_SKIPPING_SAMPLES:
                    /* During 'SKIPPING_SAMPLES': '<S71>:131' */
                    if (Extr_HighSpd_ExtUnbSampleCnt == (*GESE_Extr_ExtUnbSetup)
                            .Skipped_Samples)
                    {
                        /* Transition: '<S71>:134' */
                        Extr_HighSpd_ExtUnbSampleCnt = 0U;
                        Extraction_DW.ExtUnbSum = 0.0F;
                        Extr_HighSpd_ExtUnbAvgThreshold =
                            Extraction_GetUnbalanceTarget
                            ((&(GESE_Extr_ExtUnbTargetSet[0])),
                             Extraction_DW.tgt);
                        Extraction_DW.is_MONITORING_UNBALANCE =
                            Extraction_IN_CALCULATING_AVG_UNB;
                        Extr_HighSpd_ExtUnbState =
                            EXT_UNB_CHECK_STATE_CALCULATING_AVG_UNB;
                    }
                    else
                    {
                        b_previousEvent = Extr_Inputs_UnbalanceIndexCnt -
                            Extraction_DW.InitialUnbalanceCnt;
                        if (b_previousEvent < 0)
                        {
                            b_previousEvent = 0;
                        }
                        else
                        {
                            if (b_previousEvent > 255)
                            {
                                b_previousEvent = 255;
                            }
                        }

                        Extr_HighSpd_ExtUnbSampleCnt = (uint8)b_previousEvent;
                    }
                    break;
                }
                break;

              case Extraction_IN_SEARCHING_SPEED:
                /* During 'SEARCHING_SPEED': '<S71>:205' */
                /* Transition: '<S71>:207' */
                do
                {
                    exitg1 = 0;
                    if (Extraction_DW.tgt < GESE_Extr_ExtUnbTargetCount)
                    {
                        /* Transition: '<S71>:57' */
                        /* Transition: '<S71>:58' */
                        if (Extr_HighSpd_TargetSpeed ==
                                Extraction_GetSpeedTarget
                                ((&(GESE_Extr_ExtUnbTargetSet[0])),
                                 Extraction_DW.tgt) * 10)
                        {
                            /* Transition: '<S71>:67' */
                            Extraction_DW.is_RUNNING_im =
                                Extraction_IN_MONITORING_UNBALANCE;

                            /* Entry 'MONITORING_UNBALANCE': '<S71>:93' */
                            Extraction_DW.InitialUnbalanceCnt =
                                Extr_Inputs_UnbalanceIndexCnt;
                            Extr_HighSpd_ExtUnbAvg = 0.0F;

                            /* Entry Internal 'MONITORING_UNBALANCE': '<S71>:93' */
                            /* Transition: '<S71>:132' */
                            if (Extraction_DW.is_MONITORING_UNBALANCE ==
                                    Extraction_IN_SKIPPING_SAMPLES)
                            {
                            }
                            else
                            {
                                Extraction_DW.is_MONITORING_UNBALANCE =
                                    Extraction_IN_SKIPPING_SAMPLES;
                                Extr_HighSpd_ExtUnbState =
                                    EXT_UNB_CHECK_STATE_SKIPPING_SAMPLES;

                                /* Entry 'SKIPPING_SAMPLES': '<S71>:131' */
                                b_previousEvent = Extr_Inputs_UnbalanceIndexCnt
                                    - Extraction_DW.InitialUnbalanceCnt;
                                if (b_previousEvent < 0)
                                {
                                    b_previousEvent = 0;
                                }
                                else
                                {
                                    if (b_previousEvent > 255)
                                    {
                                        b_previousEvent = 255;
                                    }
                                }

                                Extr_HighSpd_ExtUnbSampleCnt = (uint8)
                                    b_previousEvent;
                            }

                            exitg1 = 1;
                        }
                        else
                        {
                            /* Transition: '<S71>:60' */
                            b_previousEvent = Extraction_DW.tgt + 1;
                            if (b_previousEvent > 255)
                            {
                                b_previousEvent = 255;
                            }

                            Extraction_DW.tgt = (uint8)b_previousEvent;
                        }
                    }
                    else
                    {
                        /* Transition: '<S71>:176' */
                        /* Event: '<S71>:74' */
                        b_previousEvent = Extraction_DW.sfEvent_pr;
                        Extraction_DW.sfEvent_pr =
                            Extraction_event_EvNoMatchFound;
                        Extraction_c45_Extraction();
                        Extraction_DW.sfEvent_pr = b_previousEvent;
                        exitg1 = 1;
                    }
                }
                while (exitg1 == 0);
                break;
            }
        }
        break;
    }

    if (guard1)
    {
        Extraction_DW.is_MONITORING_UNBALANCE =
            Extraction_IN_CALCULATING_AVG_UNB;
        Extr_HighSpd_ExtUnbState = EXT_UNB_CHECK_STATE_CALCULATING_AVG_UNB;
    }
}

/* Function for Chart: '<S64>/Extr_HighSpd_ExtUnbCheckLogic' */
static void Extraction_chartstep_c45_Extraction(void)
{
    Extraction_DW.MCI_RawUnbalanceCnt_prev =
        Extraction_DW.MCI_RawUnbalanceCnt_start;
    Extraction_DW.MCI_RawUnbalanceCnt_start = Extr_Inputs_UnbalanceIndexCnt;
    Extraction_c45_Extraction();
}

/* Function for Chart: '<S12>/Extr_Rebal_Logic' */
static void Extraction_broadcast_EvLocalStop(void)
{
    sint32 b_previousEvent;

    /* Event: '<S180>:145' */
    b_previousEvent = Extraction_DW.sfEvent;
    Extraction_DW.sfEvent = Extraction_event_EvLocalStop;
    Extraction_c24_Extraction();
    Extraction_DW.sfEvent = b_previousEvent;
}

/* Function for Chart: '<S12>/Extr_Rebal_Logic' */
static void Extraction_enter_atomic_DISTRIBUTION_RAMP(void)
{
    sint16 tmp;
    sint32 tmp_0;
    float32 u0;

    /* Entry 'DISTRIBUTION_RAMP': '<S180>:69' */
    if (Extr_Rebal_PulseIndex < GESE_Extr_DistProfPulseCnt)
    {
        if (Extr_Rebal_PulseIndex >= 1)
        {
            Extraction_B.prev_speed = (sint16)
                (GESE_Extr_DistProf[Extr_Rebal_PulseIndex - 1].Target_Speed_Div2
                 * 2);
        }
        else
        {
            Extraction_B.prev_speed = 0;
        }

        tmp_0 = (sint32)(Extr_Rebal_PulseIndex + 1U);
        if ((uint32)tmp_0 > 255U)
        {
            tmp_0 = 255;
        }

        Extr_Rebal_PulseIndex = (uint8)tmp_0;
        Extraction_DW.CurrentPulse = GESE_Extr_DistProf[Extr_Rebal_PulseIndex -
            1];
        Extraction_DW.TargetSpeed = (sint16)
            (Extraction_DW.CurrentPulse.Target_Speed_Div2 * 2);

        /* Simulink Function 'CalculateRampRate': '<S180>:104' */
        Extraction_B.current_pulse = Extraction_DW.CurrentPulse;

        /* Outputs for Function Call SubSystem: '<S180>/REBALANCE.RUNNING.DISTRIBUTION_RAMP.CalculateRampRate' */
        /* Product: '<S186>/Divide1' incorporates:
         *  Constant: '<S186>/Constant'
         */
        u0 = (float32)Extraction_B.current_pulse.Ramp_Time_x10 / 10.0F;

        /* Saturate: '<S186>/Saturation' */
        if (u0 < 0.1F)
        {
            u0 = 0.1F;
        }

        /* End of Saturate: '<S186>/Saturation' */

        /* DataTypeConversion: '<S186>/Round' incorporates:
         *  Gain: '<S186>/Gain'
         *  Product: '<S186>/Divide'
         *  Sum: '<S186>/Subtract'
         */
        Extraction_B.Abs = (sint16)((float32)
            (Extraction_B.current_pulse.Target_Speed_Div2 * 2 -
             Extraction_B.prev_speed) / u0);

        /* Abs: '<S186>/Abs' */
        if (Extraction_B.Abs < 0)
        {
            /* DataTypeConversion: '<S186>/Round' */
            Extraction_B.Abs = (sint16)-Extraction_B.Abs;
        }

        /* End of Abs: '<S186>/Abs' */
        /* End of Outputs for SubSystem: '<S180>/REBALANCE.RUNNING.DISTRIBUTION_RAMP.CalculateRampRate' */
        Extraction_DW.RampAccel = Extraction_B.Abs;
        if (Extraction_DW.TargetSpeed < 0)
        {
            tmp_0 = -Extraction_DW.TargetSpeed;
            if (tmp_0 > 32767)
            {
                tmp_0 = 32767;
            }

            tmp = (sint16)tmp_0;
        }
        else
        {
            tmp = Extraction_DW.TargetSpeed;
        }

        if (tmp < ((sint16)EXTR_MIN_SPEED))
        {
            Extraction_DW.TargetSpeed = 0;
        }

        Extraction_DW.P2PTorqueDelay = (*GESE_Extr_FastDistRampSetup).
            P2P_Torque_Updates_Initial;
        Extr_Rebal_DistP2PTorque = 0.0F;
    }
    else
    {
        /* Event: '<S180>:16' */
        Extraction_DW.Extr_Rebal_EvCompleteEventCounter++;
        Extraction_broadcast_EvLocalStop();
    }
}

/* Function for Chart: '<S12>/Extr_Rebal_Logic' */
static void Extraction_enter_internal_RUNNING_lp(void)
{
    sint16 tmp;
    sint32 tmp_0;

    /* Entry Internal 'RUNNING': '<S180>:8' */
    /* Transition: '<S180>:70' */
    if (Extraction_DW.is_RUNNING == Extraction_IN_DISTRIBUTION_RAMP)
    {
    }
    else
    {
        Extraction_DW.is_RUNNING = Extraction_IN_DISTRIBUTION_RAMP;
        Extraction_enter_atomic_DISTRIBUTION_RAMP();
    }

    if (Extraction_DW.is_RUNNING != Extraction_IN_DISTRIBUTION_RAMP)
    {
    }
    else
    {
        /* Entry Internal 'DISTRIBUTION_RAMP': '<S180>:69' */
        /* Transition: '<S180>:164' */
        if (Extraction_DW.TargetSpeed < 0)
        {
            tmp_0 = -Extraction_DW.TargetSpeed;
            if (tmp_0 > 32767)
            {
                tmp_0 = 32767;
            }

            tmp = (sint16)tmp_0;
        }
        else
        {
            tmp = Extraction_DW.TargetSpeed;
        }

        if (tmp > 0)
        {
            /* Transition: '<S180>:290' */
            if (Extraction_DW.is_DISTRIBUTION_RAMP ==
                    Extraction_IN_MOTOR_STARTING)
            {
            }
            else
            {
                Extraction_DW.is_DISTRIBUTION_RAMP =
                    Extraction_IN_MOTOR_STARTING;
                Extr_Rebal_State = EXTR_REBAL_STATE_MOTOR_STARTING;

                /* Entry 'MOTOR_STARTING': '<S180>:163' */
                Extraction_SetSpeedCommand(Extraction_DW.TargetSpeed,
                    Extraction_DW.RampAccel);
            }
        }
        else
        {
            /* Transition: '<S180>:291' */
            if (Extraction_DW.is_DISTRIBUTION_RAMP ==
                    Extraction_IN_MOTOR_STOPPING_lj)
            {
            }
            else
            {
                Extraction_DW.is_DISTRIBUTION_RAMP =
                    Extraction_IN_MOTOR_STOPPING_lj;
                Extr_Rebal_State = EXTR_REBAL_STATE_MOTOR_STOPPING;

                /* Entry 'MOTOR_STOPPING': '<S180>:286' */
                Extraction_StopMotor();
            }
        }
    }
}

/* Function for Chart: '<S12>/Extr_Rebal_Logic' */
static void Extraction_broadcast_EvRampComplete(void)
{
    sint32 b_previousEvent;

    /* Event: '<S180>:162' */
    b_previousEvent = Extraction_DW.sfEvent;
    Extraction_DW.sfEvent = Extraction_event_EvRampComplete;
    Extraction_c24_Extraction();
    Extraction_DW.sfEvent = b_previousEvent;
}

/* Function for Chart: '<S12>/Extr_Rebal_Logic' */
static sint16 Extraction_CalcDropSpeed(uint8 setup_Short_Redist_Delta_Speed_Drop,
    uint8 setup_Short_Redist_Min_Velocity, float32 satelizationSpeed, float32
    speedRef)
{
    sint16 speedTarget;
    float32 tmp;
    sint16 tmp_0;

    /* MATLAB Function 'CalcDropSpeed': '<S180>:128' */
    /*  Decelerate the drum to a lower rpm to redistribute the load when */
    /*  the satelization speed is greater than the minimum limit. */
    /* '<S180>:128:5' */
    tmp = roundf(fabsf(satelizationSpeed));
    if (tmp < 32768.0F)
    {
        tmp_0 = (sint16)tmp;
    }
    else
    {
        tmp_0 = MAX_int16_T;
    }

    speedTarget = (sint16)(tmp_0 - setup_Short_Redist_Delta_Speed_Drop);
    if (tmp < 32768.0F)
    {
        tmp_0 = (sint16)tmp;
    }
    else
    {
        tmp_0 = MAX_int16_T;
    }

    if ((sint16)(tmp_0 - setup_Short_Redist_Delta_Speed_Drop) <
            setup_Short_Redist_Min_Velocity)
    {
        /* '<S180>:128:7' */
        /*  The calculated drop speed is less than the minimum speed */
        /* '<S180>:128:8' */
        speedTarget = setup_Short_Redist_Min_Velocity;

        /*  reassign the drop speed to the minimum */
    }

    /*  Assign the direction to the calc_drop_speed which is currently + = cw */
    if (speedRef < 0.0F)
    {
        /* '<S180>:128:12' */
        /* '<S180>:128:13' */
        speedTarget = (sint16)-speedTarget;
    }

    return speedTarget;
}

/* Function for Chart: '<S12>/Extr_Rebal_Logic' */
static void Extraction_DISTRIBUTION_RAMP(void)
{
    boolean_T tmp;
    sint16 tmp_0;
    sint32 tmp_1;
    boolean_T guard1 = false;

    /* During 'DISTRIBUTION_RAMP': '<S180>:69' */
    if (Extraction_DW.sfEvent == Extraction_event_EvRampComplete)
    {
        /* Transition: '<S180>:73' */
        if (Extraction_DW.CurrentPulse.On_Time > 0)
        {
            /* Transition: '<S180>:168' */
            /* Exit Internal 'DISTRIBUTION_RAMP': '<S180>:69' */
            Extraction_DW.is_DISTRIBUTION_RAMP = 0U;

            /* Exit Internal 'RAMPING': '<S180>:198' */
            Extraction_DW.is_RAMPING = 0U;
            if (Extraction_DW.is_RUNNING != Extraction_IN_DISTRIBUTION_RAMP)
            {
            }
            else
            {
                /* Exit 'DISTRIBUTION_RAMP': '<S180>:69' */
                Extraction_DW.is_RUNNING = 0U;
            }

            if (Extraction_DW.is_RUNNING == Extraction_IN_PLATEAU_p1)
            {
            }
            else
            {
                Extraction_DW.is_RUNNING = Extraction_IN_PLATEAU_p1;
                Extraction_DW.temporalCounter_i1 = 0U;
                Extr_Rebal_State = EXTR_REBAL_STATE_PLATEAU;

                /* Entry 'PLATEAU': '<S180>:71' */
                if (Extraction_DW.CurrentPulse.On_Time_Scaling > 0)
                {
                    Extraction_DW.MotionDelay = (uint16)
                        (Extraction_DW.CurrentPulse.On_Time * 5);
                }
                else
                {
                    Extraction_DW.MotionDelay =
                        Extraction_DW.CurrentPulse.On_Time;
                }
            }
        }
        else
        {
            /* Transition: '<S180>:167' */
            /* Transition: '<S180>:172' */
            /* Transition: '<S180>:171' */
            /* Transition: '<S180>:74' */
            if (Extraction_DW.CurrentPulse.Off_Time > 0)
            {
                /* Transition: '<S180>:76' */
                /* Exit Internal 'DISTRIBUTION_RAMP': '<S180>:69' */
                Extraction_DW.is_DISTRIBUTION_RAMP = 0U;

                /* Exit Internal 'RAMPING': '<S180>:198' */
                Extraction_DW.is_RAMPING = 0U;
                if (Extraction_DW.is_RUNNING != Extraction_IN_DISTRIBUTION_RAMP)
                {
                }
                else
                {
                    /* Exit 'DISTRIBUTION_RAMP': '<S180>:69' */
                    Extraction_DW.is_RUNNING = 0U;
                }

                if (Extraction_DW.is_RUNNING == Extraction_IN_MOTOR_OFF)
                {
                }
                else
                {
                    Extraction_DW.is_RUNNING = Extraction_IN_MOTOR_OFF;
                    Extraction_DW.temporalCounter_i1 = 0U;
                    Extr_Rebal_State = EXTR_REBAL_STATE_MOTOR_OFF;

                    /* Entry 'MOTOR_OFF': '<S180>:72' */
                    if (Extraction_DW.CurrentPulse.On_Time_Scaling > 0)
                    {
                        Extraction_DW.MotionDelay = (uint16)
                            (Extraction_DW.CurrentPulse.Off_Time * 5);
                    }
                    else
                    {
                        Extraction_DW.MotionDelay =
                            Extraction_DW.CurrentPulse.Off_Time;
                    }

                    Extraction_StopMotor();
                }
            }
            else
            {
                /* Transition: '<S180>:87' */
                /* Transition: '<S180>:85' */
                /* Transition: '<S180>:84' */
                /* Exit Internal 'DISTRIBUTION_RAMP': '<S180>:69' */
                Extraction_DW.is_DISTRIBUTION_RAMP = 0U;

                /* Exit Internal 'RAMPING': '<S180>:198' */
                Extraction_DW.is_RAMPING = 0U;
                if (Extraction_DW.is_RUNNING != Extraction_IN_DISTRIBUTION_RAMP)
                {
                }
                else
                {
                    /* Exit 'DISTRIBUTION_RAMP': '<S180>:69' */
                    Extraction_DW.is_RUNNING = 0U;
                }

                if (Extraction_DW.is_RUNNING == Extraction_IN_DISTRIBUTION_RAMP)
                {
                }
                else
                {
                    Extraction_DW.is_RUNNING = Extraction_IN_DISTRIBUTION_RAMP;
                    Extraction_enter_atomic_DISTRIBUTION_RAMP();
                }

                if (Extraction_DW.is_RUNNING != Extraction_IN_DISTRIBUTION_RAMP)
                {
                }
                else
                {
                    /* Entry Internal 'DISTRIBUTION_RAMP': '<S180>:69' */
                    /* Transition: '<S180>:164' */
                    if (Extraction_DW.TargetSpeed < 0)
                    {
                        tmp_1 = -Extraction_DW.TargetSpeed;
                        if (tmp_1 > 32767)
                        {
                            tmp_1 = 32767;
                        }

                        tmp_0 = (sint16)tmp_1;
                    }
                    else
                    {
                        tmp_0 = Extraction_DW.TargetSpeed;
                    }

                    if (tmp_0 > 0)
                    {
                        /* Transition: '<S180>:290' */
                        if (Extraction_DW.is_DISTRIBUTION_RAMP ==
                                Extraction_IN_MOTOR_STARTING)
                        {
                        }
                        else
                        {
                            Extraction_DW.is_DISTRIBUTION_RAMP =
                                Extraction_IN_MOTOR_STARTING;
                            Extr_Rebal_State = EXTR_REBAL_STATE_MOTOR_STARTING;

                            /* Entry 'MOTOR_STARTING': '<S180>:163' */
                            Extraction_SetSpeedCommand(Extraction_DW.TargetSpeed,
                                Extraction_DW.RampAccel);
                        }
                    }
                    else
                    {
                        /* Transition: '<S180>:291' */
                        if (Extraction_DW.is_DISTRIBUTION_RAMP ==
                                Extraction_IN_MOTOR_STOPPING_lj)
                        {
                        }
                        else
                        {
                            Extraction_DW.is_DISTRIBUTION_RAMP =
                                Extraction_IN_MOTOR_STOPPING_lj;
                            Extr_Rebal_State = EXTR_REBAL_STATE_MOTOR_STOPPING;

                            /* Entry 'MOTOR_STOPPING': '<S180>:286' */
                            Extraction_StopMotor();
                        }
                    }
                }
            }
        }
    }
    else
    {
        guard1 = false;
        if (Extr_Rebal_DistP2PTorque > (float32)(*GESE_Extr_FastDistRampSetup).
                Max_Torque_Disturbance_x100 / 100.0F)
        {
            Extraction_SetFault(EXTR_FAULT_REBAL_P2P_TORQUE_EXCEEDED);

            /* Event: '<S180>:146' */
            Extraction_DW.Extr_Rebal_EvP2PTorqFaultEventCounter++;
            Extraction_broadcast_EvLocalStop();
            if (Extraction_DW.is_RUNNING != Extraction_IN_DISTRIBUTION_RAMP)
            {
            }
            else
            {
                guard1 = true;
            }
        }
        else
        {
            guard1 = true;
        }

        if (guard1)
        {
            if ((*GESE_Extr_Setup).CWLD_Enabled != 0 && Extr_Rebal_DistP2PTorque
                > (float32)(*GESE_Extr_CleanWasherSetup).
                    Torque_P2P_Threshold_x100 / 100.0F)
            {
                Extraction_SetFault(EXTR_FAULT_CLEAN_WASHER_P2P_TORQUE_EXCEEDED);

                /* Event: '<S180>:231' */
                Extraction_DW.Extr_Rebal_EvCleanWasherExceptionEventCounter++;
                Extraction_broadcast_EvLocalStop();
            }
        }

        if (Extraction_DW.is_RUNNING != Extraction_IN_DISTRIBUTION_RAMP)
        {
        }
        else
        {
            switch (Extraction_DW.is_DISTRIBUTION_RAMP)
            {
              case Extraction_IN_DROPPING_SPEED:
                /* During 'DROPPING_SPEED': '<S180>:106' */
                if (Extraction_DW.sfEvent ==
                        Extraction_event_Extr_Sup_EvSpeedReached_ok)
                {
                    /* Transition: '<S180>:113' */
                    /* Transition: '<S180>:152' */
                    /* Transition: '<S180>:120' */
                    Extraction_DW.is_DISTRIBUTION_RAMP =
                        Extraction_IN_MOTOR_STARTING;
                    Extr_Rebal_State = EXTR_REBAL_STATE_MOTOR_STARTING;

                    /* Entry 'MOTOR_STARTING': '<S180>:163' */
                    Extraction_SetSpeedCommand(Extraction_DW.TargetSpeed,
                        Extraction_DW.RampAccel);
                }
                break;

              case Extraction_IN_MOTOR_STARTING:
                /* During 'MOTOR_STARTING': '<S180>:163' */
                Extraction_IsMotorRunning(&tmp);
                if (tmp)
                {
                    /* Transition: '<S180>:124' */
                    if ((*GESE_Extr_Setup).DR_Enabled != 0 &&
                            Extraction_DW.CurrentPulse.Dist_Ramp_Enabled != 0)
                    {
                        /* Transition: '<S180>:158' */
                        /* Transition: '<S180>:159' */
                        Extraction_DW.is_DISTRIBUTION_RAMP =
                            Extraction_IN_RAMPING_ce;
                        if (Extraction_DW.is_RAMPING ==
                                Extraction_IN_DETECTING_PEAKS_AND_TROUGHS)
                        {
                        }
                        else
                        {
                            Extraction_DW.is_RAMPING =
                                Extraction_IN_DETECTING_PEAKS_AND_TROUGHS;
                            Extr_Rebal_State =
                                EXTR_REBAL_STATE_DETECTING_PEAKS_AND_TROUGHS;

                            /* Entry 'DETECTING_PEAKS_AND_TROUGHS': '<S180>:107' */
                            /* Simulink Function 'DistUtilsWrapperControl': '<S180>:186' */
                            Extraction_B.distUtilsCommand =
                                DIST_UTILS_ENABLE_SATELIZATION;

                            /* Outputs for Function Call SubSystem: '<S180>/DistUtilsWrapperControl' */
                            Extraction_DistUtilsWrapperControl
                                (Extraction_B.distUtilsCommand,
                                 &Extraction_B.DistUtilsWrapperControl);

                            /* End of Outputs for SubSystem: '<S180>/DistUtilsWrapperControl' */

                            /* Simulink Function 'DistUtilsWrapperControl': '<S180>:186' */
                            Extraction_B.distUtilsCommand =
                                DIST_UTILS_ENABLE_DXDT;

                            /* Outputs for Function Call SubSystem: '<S180>/DistUtilsWrapperControl' */
                            Extraction_DistUtilsWrapperControl
                                (Extraction_B.distUtilsCommand,
                                 &Extraction_B.DistUtilsWrapperControl);

                            /* End of Outputs for SubSystem: '<S180>/DistUtilsWrapperControl' */

                            /* Simulink Function 'DistUtilsWrapperControl': '<S180>:186' */
                            Extraction_B.distUtilsCommand =
                                DIST_UTILS_RESET_COUNTERS;

                            /* Outputs for Function Call SubSystem: '<S180>/DistUtilsWrapperControl' */
                            Extraction_DistUtilsWrapperControl
                                (Extraction_B.distUtilsCommand,
                                 &Extraction_B.DistUtilsWrapperControl);

                            /* End of Outputs for SubSystem: '<S180>/DistUtilsWrapperControl' */
                        }
                    }
                    else
                    {
                        /* Transition: '<S180>:200' */
                        Extraction_DW.is_DISTRIBUTION_RAMP =
                            Extraction_IN_RAMPING_ce;
                        Extraction_DW.is_RAMPING =
                            Extraction_IN_EXECUTING_STATIC_PROFILE;
                        Extr_Rebal_State =
                            EXTR_REBAL_STATE_EXECUTING_STATIC_PROFILE;
                    }
                }
                break;

              case Extraction_IN_MOTOR_STOPPING_lj:
                /* During 'MOTOR_STOPPING': '<S180>:286' */
                Extraction_IsMotorRunning(&tmp);
                if (!tmp)
                {
                    /* Transition: '<S180>:287' */
                    /* Transition: '<S180>:298' */
                    /* Transition: '<S180>:299' */
                    /* Transition: '<S180>:116' */
                    Extraction_broadcast_EvRampComplete();
                    if (Extraction_DW.is_DISTRIBUTION_RAMP !=
                            Extraction_IN_MOTOR_STOPPING_lj)
                    {
                    }
                    else
                    {
                        Extraction_DW.is_DISTRIBUTION_RAMP = 0U;

                        /* Entry Internal 'DISTRIBUTION_RAMP': '<S180>:69' */
                        /* Transition: '<S180>:164' */
                        if (Extraction_DW.TargetSpeed < 0)
                        {
                            tmp_1 = -Extraction_DW.TargetSpeed;
                            if (tmp_1 > 32767)
                            {
                                tmp_1 = 32767;
                            }

                            tmp_0 = (sint16)tmp_1;
                        }
                        else
                        {
                            tmp_0 = Extraction_DW.TargetSpeed;
                        }

                        if (tmp_0 > 0)
                        {
                            /* Transition: '<S180>:290' */
                            if (Extraction_DW.is_DISTRIBUTION_RAMP ==
                                    Extraction_IN_MOTOR_STARTING)
                            {
                            }
                            else
                            {
                                Extraction_DW.is_DISTRIBUTION_RAMP =
                                    Extraction_IN_MOTOR_STARTING;
                                Extr_Rebal_State =
                                    EXTR_REBAL_STATE_MOTOR_STARTING;

                                /* Entry 'MOTOR_STARTING': '<S180>:163' */
                                Extraction_SetSpeedCommand
                                    (Extraction_DW.TargetSpeed,
                                     Extraction_DW.RampAccel);
                            }
                        }
                        else
                        {
                            /* Transition: '<S180>:291' */
                            if (Extraction_DW.is_DISTRIBUTION_RAMP ==
                                    Extraction_IN_MOTOR_STOPPING_lj)
                            {
                            }
                            else
                            {
                                Extraction_DW.is_DISTRIBUTION_RAMP =
                                    Extraction_IN_MOTOR_STOPPING_lj;
                                Extr_Rebal_State =
                                    EXTR_REBAL_STATE_MOTOR_STOPPING;

                                /* Entry 'MOTOR_STOPPING': '<S180>:286' */
                                Extraction_StopMotor();
                            }
                        }
                    }
                }
                break;

              case Extraction_IN_RAMPING_ce:
                /* During 'RAMPING': '<S180>:198' */
                if (Extraction_DW.sfEvent ==
                        Extraction_event_Extr_Sup_EvSpeedReached_ok)
                {
                    /* Transition: '<S180>:296' */
                    /* Transition: '<S180>:116' */
                    Extraction_broadcast_EvRampComplete();
                    if (Extraction_DW.is_DISTRIBUTION_RAMP !=
                            Extraction_IN_RAMPING_ce)
                    {
                    }
                    else
                    {
                        /* Exit Internal 'RAMPING': '<S180>:198' */
                        Extraction_DW.is_RAMPING = 0U;
                        Extraction_DW.is_DISTRIBUTION_RAMP = 0U;

                        /* Entry Internal 'DISTRIBUTION_RAMP': '<S180>:69' */
                        /* Transition: '<S180>:164' */
                        if (Extraction_DW.TargetSpeed < 0)
                        {
                            tmp_1 = -Extraction_DW.TargetSpeed;
                            if (tmp_1 > 32767)
                            {
                                tmp_1 = 32767;
                            }

                            tmp_0 = (sint16)tmp_1;
                        }
                        else
                        {
                            tmp_0 = Extraction_DW.TargetSpeed;
                        }

                        if (tmp_0 > 0)
                        {
                            /* Transition: '<S180>:290' */
                            if (Extraction_DW.is_DISTRIBUTION_RAMP ==
                                    Extraction_IN_MOTOR_STARTING)
                            {
                            }
                            else
                            {
                                Extraction_DW.is_DISTRIBUTION_RAMP =
                                    Extraction_IN_MOTOR_STARTING;
                                Extr_Rebal_State =
                                    EXTR_REBAL_STATE_MOTOR_STARTING;

                                /* Entry 'MOTOR_STARTING': '<S180>:163' */
                                Extraction_SetSpeedCommand
                                    (Extraction_DW.TargetSpeed,
                                     Extraction_DW.RampAccel);
                            }
                        }
                        else
                        {
                            /* Transition: '<S180>:291' */
                            if (Extraction_DW.is_DISTRIBUTION_RAMP ==
                                    Extraction_IN_MOTOR_STOPPING_lj)
                            {
                            }
                            else
                            {
                                Extraction_DW.is_DISTRIBUTION_RAMP =
                                    Extraction_IN_MOTOR_STOPPING_lj;
                                Extr_Rebal_State =
                                    EXTR_REBAL_STATE_MOTOR_STOPPING;

                                /* Entry 'MOTOR_STOPPING': '<S180>:286' */
                                Extraction_StopMotor();
                            }
                        }
                    }
                }
                else
                {
                    switch (Extraction_DW.is_RAMPING)
                    {
                      case Extraction_IN_DETECTING_PEAKS_AND_TROUGHS:
                        /* During 'DETECTING_PEAKS_AND_TROUGHS': '<S180>:107' */
                        tmp_1 = (sint32)((uint32)
                                         Extr_Inputs_DistUtilsData.Dxdt.PeakCount
                                         + Extr_Inputs_DistUtilsData.Dxdt.TroughCount);
                        if ((uint32)tmp_1 > 255U)
                        {
                            tmp_1 = 255;
                        }

                        if (tmp_1 >= Extraction_DW.P2PTorqueDelay)
                        {
                            /* Transition: '<S180>:114' */
                            Extraction_DW.is_RAMPING =
                                Extraction_IN_VERIFYING_LOAD_DISTRIBUTION;
                            Extr_Rebal_State =
                                EXTR_REBAL_STATE_VERIFYING_LOAD_DISTRIBUTION;

                            /* Entry 'VERIFYING_LOAD_DISTRIBUTION': '<S180>:108' */
                            /* Simulink Function 'P2PTorqueThresholdCalc': '<S180>:134' */
                            Extraction_B.mass =
                                Extraction_B.Extr_LowSpd_LoadMass_gp;
                            Extraction_B.currentSpeed =
                                Extr_Inputs_MotorSignals.Speed_Ref;

                            /* Outputs for Function Call SubSystem: '<S180>/REBALANCE.RUNNING.DISTRIBUTION_RAMP.P2PTorqueThresholdCalc' */
                            Extraction_REBALANCERUNNINGDISTRIBUTION_RAMPP2PTorqueThresholdCalc
                                (Extraction_B.mass, Extraction_B.currentSpeed,
                                 &Extraction_B.REBALANCERUNNINGDISTRIBUTION_RAMPP2PTorqueThresholdCalc);

                            /* End of Outputs for SubSystem: '<S180>/REBALANCE.RUNNING.DISTRIBUTION_RAMP.P2PTorqueThresholdCalc' */
                            Extr_Rebal_DistP2PThresh =
                                Extraction_B.REBALANCERUNNINGDISTRIBUTION_RAMPP2PTorqueThresholdCalc.Gain;
                            Extr_Rebal_DistP2PTorque = fabsf
                                (Extr_Inputs_DistUtilsData.Dxdt.PeakValue -
                                 Extr_Inputs_DistUtilsData.Dxdt.TroughValue);
                        }
                        else
                        {
                            Extr_Rebal_DistP2PTorque = fabsf
                                (Extr_Inputs_DistUtilsData.Dxdt.PeakValue -
                                 Extr_Inputs_DistUtilsData.Dxdt.TroughValue);
                        }
                        break;

                      case Extraction_IN_EXECUTING_STATIC_PROFILE:
                        /* During 'EXECUTING_STATIC_PROFILE': '<S180>:153' */
                        break;

                      case Extraction_IN_VERIFYING_LOAD_DISTRIBUTION:
                        /* During 'VERIFYING_LOAD_DISTRIBUTION': '<S180>:108' */
                        if (Extr_Rebal_DistP2PTorque >= Extr_Rebal_DistP2PThresh)
                        {
                            /* Transition: '<S180>:115' */
                            Extraction_DW.is_RAMPING = 0U;
                            Extraction_DW.is_DISTRIBUTION_RAMP =
                                Extraction_IN_DROPPING_SPEED;
                            Extr_Rebal_State = EXTR_REBAL_STATE_DROPPING_SPEED;

                            /* Entry 'DROPPING_SPEED': '<S180>:106' */
                            tmp_1 = (sint32)(Extr_Rebal_ShrtRedistCnt + 1U);
                            if ((uint32)tmp_1 > 255U)
                            {
                                tmp_1 = 255;
                            }

                            Extr_Rebal_ShrtRedistCnt = (uint8)tmp_1;
                            Extraction_DW.P2PTorqueDelay =
                                (*GESE_Extr_FastDistRampSetup).
                                P2P_Torque_Updates_Short_Redist;
                            Extraction_SetSpeedCommand(Extraction_CalcDropSpeed
                                ((*GESE_Extr_FastDistRampSetup).
                                 Short_Redist_Delta_Speed_Drop,
                                 (*GESE_Extr_FastDistRampSetup).
                                 Short_Redist_Min_Velocity,
                                 Extr_Inputs_DistUtilsData.satelizationSpeed,
                                 Extr_Inputs_MotorSignals.Speed_Ref),
                                Extraction_DW.RampAccel);
                        }
                        else
                        {
                            /* Simulink Function 'P2PTorqueThresholdCalc': '<S180>:134' */
                            Extraction_B.mass =
                                Extraction_B.Extr_LowSpd_LoadMass_gp;
                            Extraction_B.currentSpeed =
                                Extr_Inputs_MotorSignals.Speed_Ref;

                            /* Outputs for Function Call SubSystem: '<S180>/REBALANCE.RUNNING.DISTRIBUTION_RAMP.P2PTorqueThresholdCalc' */
                            Extraction_REBALANCERUNNINGDISTRIBUTION_RAMPP2PTorqueThresholdCalc
                                (Extraction_B.mass, Extraction_B.currentSpeed,
                                 &Extraction_B.REBALANCERUNNINGDISTRIBUTION_RAMPP2PTorqueThresholdCalc);

                            /* End of Outputs for SubSystem: '<S180>/REBALANCE.RUNNING.DISTRIBUTION_RAMP.P2PTorqueThresholdCalc' */
                            Extr_Rebal_DistP2PThresh =
                                Extraction_B.REBALANCERUNNINGDISTRIBUTION_RAMPP2PTorqueThresholdCalc.Gain;
                            Extr_Rebal_DistP2PTorque = fabsf
                                (Extr_Inputs_DistUtilsData.Dxdt.PeakValue -
                                 Extr_Inputs_DistUtilsData.Dxdt.TroughValue);
                        }
                        break;
                    }
                }
                break;
            }
        }
    }
}

/* Function for Chart: '<S12>/Extr_Rebal_Logic' */
static void Extraction_RUNNING_bk(void)
{
    boolean_T DistProfileLoaded;
    uint8 tmp;
    sint16 tmp_0;
    sint32 tmp_1;
    uint32 tmp_2;
    boolean_T guard1 = false;

    /* During 'RUNNING': '<S180>:8' */
    guard1 = false;
    if (Extraction_DW.sfEvent ==
            Extraction_event_Extr_Sup_EvSpeedErrorTimeout_le ||
            Extraction_DW.sfEvent == Extraction_event_Extr_Accel_EvCabHit_ii)
    {
        /* Transition: '<S180>:245' */
        /* Transition: '<S180>:305' */
        /* Transition: '<S180>:247' */
        /* Transition: '<S180>:248' */
        Extraction_LoadRebalanceProfileIndex(Extr_Rebal_RebalCnt, &tmp);
        Extraction_LoadRebalanceProfile(tmp, &DistProfileLoaded);
        tmp_1 = (sint32)(Extr_Rebal_RebalCnt + 1U);
        if ((uint32)tmp_1 > 255U)
        {
            tmp_1 = 255;
        }

        Extr_Rebal_RebalCnt = (uint8)tmp_1;
        Extr_Rebal_RebInProgress = true;
        Extr_Rebal_ShrtRedistCnt = 0U;
        Extr_Rebal_PulseIndex = 0U;

        /* Transition: '<S180>:238' */
        if (!DistProfileLoaded || GESE_Extr_DistProfPulseCnt <= 0)
        {
            /* Transition: '<S180>:63' */
            Extr_Rebal_Fault = EXTR_REBAL_FAULT_INVALID_DIST_PROFILE;
            Extraction_SetFault(EXTR_FAULT_REBAL_INVALID_CONFIG);
            guard1 = true;
        }
        else
        {
            Extraction_IsMotorRunning(&DistProfileLoaded);
            tmp_1 = Extr_LowSpd_DistSpeed - 10;
            if (tmp_1 < -32768)
            {
                tmp_1 = -32768;
            }

            if (DistProfileLoaded && (Extr_LowSpd_DistSpeed > ((sint16)
                    EXTR_MIN_SPEED) && Extr_Inputs_MotorSignals.Speed >= tmp_1))
            {
                /* Transition: '<S180>:226' */
                /* Transition: '<S180>:225' */
                /* Exit Internal 'RUNNING': '<S180>:8' */
                if (Extraction_DW.is_RUNNING == Extraction_IN_DISTRIBUTION_RAMP)
                {
                    /* Exit Internal 'DISTRIBUTION_RAMP': '<S180>:69' */
                    Extraction_DW.is_DISTRIBUTION_RAMP = 0U;

                    /* Exit Internal 'RAMPING': '<S180>:198' */
                    Extraction_DW.is_RAMPING = 0U;

                    /* Exit 'DISTRIBUTION_RAMP': '<S180>:69' */
                    Extraction_DW.is_RUNNING = 0U;
                }
                else
                {
                    Extraction_DW.is_RUNNING = 0U;
                }

                Extraction_DW.is_REBALANCE = Extraction_IN_RAMPING_TO_DIST_SPEED;
                Extr_Rebal_State = EXTR_REBAL_STATE_RAMPING_TO_DIST_SPEED;

                /* Entry 'RAMPING_TO_DIST_SPEED': '<S180>:52' */
                Extraction_SetSpeedCommand(Extr_LowSpd_DistSpeed, 100);
            }
            else
            {
                /* Transition: '<S180>:236' */
                /* Exit Internal 'RUNNING': '<S180>:8' */
                if (Extraction_DW.is_RUNNING == Extraction_IN_DISTRIBUTION_RAMP)
                {
                    /* Exit Internal 'DISTRIBUTION_RAMP': '<S180>:69' */
                    Extraction_DW.is_DISTRIBUTION_RAMP = 0U;

                    /* Exit Internal 'RAMPING': '<S180>:198' */
                    Extraction_DW.is_RAMPING = 0U;

                    /* Exit 'DISTRIBUTION_RAMP': '<S180>:69' */
                    Extraction_DW.is_RUNNING = 0U;
                }
                else
                {
                    Extraction_DW.is_RUNNING = 0U;
                }

                Extraction_DW.is_REBALANCE = Extraction_IN_RUNNING_jg;
                Extraction_enter_internal_RUNNING_lp();
            }
        }
    }
    else if (Extraction_DW.sfEvent == Extraction_event_EvLocalStop)
    {
        /* Transition: '<S180>:302' */
        guard1 = true;
    }
    else
    {
        switch (Extraction_DW.is_RUNNING)
        {
          case Extraction_IN_DISTRIBUTION_RAMP:
            Extraction_DISTRIBUTION_RAMP();
            break;

          case Extraction_IN_MOTOR_OFF:
            /* During 'MOTOR_OFF': '<S180>:72' */
            tmp_2 = (uint32)Extraction_DW.MotionDelay * ((uint16)FS_EXTRACTION);
            if (tmp_2 > 65535U)
            {
                tmp_2 = 65535U;
            }

            if (Extraction_DW.sfEvent == Extraction_event_Trigger_o1 &&
                    Extraction_DW.temporalCounter_i1 >= tmp_2)
            {
                /* Transition: '<S180>:88' */
                /* Transition: '<S180>:89' */
                /* Transition: '<S180>:90' */
                Extraction_DW.is_RUNNING = Extraction_IN_DISTRIBUTION_RAMP;
                Extraction_enter_atomic_DISTRIBUTION_RAMP();
                if (Extraction_DW.is_RUNNING != Extraction_IN_DISTRIBUTION_RAMP)
                {
                }
                else
                {
                    /* Entry Internal 'DISTRIBUTION_RAMP': '<S180>:69' */
                    /* Transition: '<S180>:164' */
                    if (Extraction_DW.TargetSpeed < 0)
                    {
                        tmp_1 = -Extraction_DW.TargetSpeed;
                        if (tmp_1 > 32767)
                        {
                            tmp_1 = 32767;
                        }

                        tmp_0 = (sint16)tmp_1;
                    }
                    else
                    {
                        tmp_0 = Extraction_DW.TargetSpeed;
                    }

                    if (tmp_0 > 0)
                    {
                        /* Transition: '<S180>:290' */
                        if (Extraction_DW.is_DISTRIBUTION_RAMP ==
                                Extraction_IN_MOTOR_STARTING)
                        {
                        }
                        else
                        {
                            Extraction_DW.is_DISTRIBUTION_RAMP =
                                Extraction_IN_MOTOR_STARTING;
                            Extr_Rebal_State = EXTR_REBAL_STATE_MOTOR_STARTING;

                            /* Entry 'MOTOR_STARTING': '<S180>:163' */
                            Extraction_SetSpeedCommand(Extraction_DW.TargetSpeed,
                                Extraction_DW.RampAccel);
                        }
                    }
                    else
                    {
                        /* Transition: '<S180>:291' */
                        if (Extraction_DW.is_DISTRIBUTION_RAMP ==
                                Extraction_IN_MOTOR_STOPPING_lj)
                        {
                        }
                        else
                        {
                            Extraction_DW.is_DISTRIBUTION_RAMP =
                                Extraction_IN_MOTOR_STOPPING_lj;
                            Extr_Rebal_State = EXTR_REBAL_STATE_MOTOR_STOPPING;

                            /* Entry 'MOTOR_STOPPING': '<S180>:286' */
                            Extraction_StopMotor();
                        }
                    }
                }
            }
            break;

          case Extraction_IN_PLATEAU_p1:
            /* During 'PLATEAU': '<S180>:71' */
            tmp_2 = (uint32)Extraction_DW.MotionDelay * ((uint16)FS_EXTRACTION);
            if (tmp_2 > 65535U)
            {
                tmp_2 = 65535U;
            }

            if (Extraction_DW.sfEvent == Extraction_event_Trigger_o1 &&
                    Extraction_DW.temporalCounter_i1 >= tmp_2)
            {
                /* Transition: '<S180>:173' */
                /* Transition: '<S180>:74' */
                if (Extraction_DW.CurrentPulse.Off_Time > 0)
                {
                    /* Transition: '<S180>:76' */
                    Extraction_DW.is_RUNNING = Extraction_IN_MOTOR_OFF;
                    Extraction_DW.temporalCounter_i1 = 0U;
                    Extr_Rebal_State = EXTR_REBAL_STATE_MOTOR_OFF;

                    /* Entry 'MOTOR_OFF': '<S180>:72' */
                    if (Extraction_DW.CurrentPulse.On_Time_Scaling > 0)
                    {
                        Extraction_DW.MotionDelay = (uint16)
                            (Extraction_DW.CurrentPulse.Off_Time * 5);
                    }
                    else
                    {
                        Extraction_DW.MotionDelay =
                            Extraction_DW.CurrentPulse.Off_Time;
                    }

                    Extraction_StopMotor();
                }
                else
                {
                    /* Transition: '<S180>:87' */
                    /* Transition: '<S180>:85' */
                    /* Transition: '<S180>:84' */
                    Extraction_DW.is_RUNNING = Extraction_IN_DISTRIBUTION_RAMP;
                    Extraction_enter_atomic_DISTRIBUTION_RAMP();
                    if (Extraction_DW.is_RUNNING !=
                            Extraction_IN_DISTRIBUTION_RAMP)
                    {
                    }
                    else
                    {
                        /* Entry Internal 'DISTRIBUTION_RAMP': '<S180>:69' */
                        /* Transition: '<S180>:164' */
                        if (Extraction_DW.TargetSpeed < 0)
                        {
                            tmp_1 = -Extraction_DW.TargetSpeed;
                            if (tmp_1 > 32767)
                            {
                                tmp_1 = 32767;
                            }

                            tmp_0 = (sint16)tmp_1;
                        }
                        else
                        {
                            tmp_0 = Extraction_DW.TargetSpeed;
                        }

                        if (tmp_0 > 0)
                        {
                            /* Transition: '<S180>:290' */
                            if (Extraction_DW.is_DISTRIBUTION_RAMP ==
                                    Extraction_IN_MOTOR_STARTING)
                            {
                            }
                            else
                            {
                                Extraction_DW.is_DISTRIBUTION_RAMP =
                                    Extraction_IN_MOTOR_STARTING;
                                Extr_Rebal_State =
                                    EXTR_REBAL_STATE_MOTOR_STARTING;

                                /* Entry 'MOTOR_STARTING': '<S180>:163' */
                                Extraction_SetSpeedCommand
                                    (Extraction_DW.TargetSpeed,
                                     Extraction_DW.RampAccel);
                            }
                        }
                        else
                        {
                            /* Transition: '<S180>:291' */
                            if (Extraction_DW.is_DISTRIBUTION_RAMP ==
                                    Extraction_IN_MOTOR_STOPPING_lj)
                            {
                            }
                            else
                            {
                                Extraction_DW.is_DISTRIBUTION_RAMP =
                                    Extraction_IN_MOTOR_STOPPING_lj;
                                Extr_Rebal_State =
                                    EXTR_REBAL_STATE_MOTOR_STOPPING;

                                /* Entry 'MOTOR_STOPPING': '<S180>:286' */
                                Extraction_StopMotor();
                            }
                        }
                    }
                }
            }
            break;
        }
    }

    if (guard1)
    {
        /* Exit Internal 'RUNNING': '<S180>:8' */
        if (Extraction_DW.is_RUNNING == Extraction_IN_DISTRIBUTION_RAMP)
        {
            /* Exit Internal 'DISTRIBUTION_RAMP': '<S180>:69' */
            Extraction_DW.is_DISTRIBUTION_RAMP = 0U;

            /* Exit Internal 'RAMPING': '<S180>:198' */
            Extraction_DW.is_RAMPING = 0U;

            /* Exit 'DISTRIBUTION_RAMP': '<S180>:69' */
            Extraction_DW.is_RUNNING = 0U;
        }
        else
        {
            Extraction_DW.is_RUNNING = 0U;
        }

        Extraction_DW.is_REBALANCE = Extraction_IN_IDLE_gt;
        Extr_Rebal_State = EXTR_REBAL_STATE_IDLE;

        /* Entry 'IDLE': '<S180>:2' */
        /* Simulink Function 'DistUtilsWrapperControl': '<S180>:186' */
        Extraction_B.distUtilsCommand = DIST_UTILS_DISABLE;

        /* Outputs for Function Call SubSystem: '<S180>/DistUtilsWrapperControl' */
        Extraction_DistUtilsWrapperControl(Extraction_B.distUtilsCommand,
            &Extraction_B.DistUtilsWrapperControl);

        /* End of Outputs for SubSystem: '<S180>/DistUtilsWrapperControl' */
        Extr_Rebal_RebInProgress = false;
    }
}

/* Function for Chart: '<S12>/Extr_Rebal_Logic' */
static void Extraction_REBALANCE(void)
{
    boolean_T DistProfileLoaded;
    uint8 tmp;
    sint32 tmp_0;
    boolean_T guard1 = false;
    boolean_T guard2 = false;

    /* During 'REBALANCE': '<S180>:38' */
    if (Extr_Rebal_ShrtRedistCnt > (*GESE_Extr_FastDistRampSetup).
            Short_Redist_Count_Max)
    {
        Extr_Rebal_ShrtRedistCnt = 0U;
        Extr_Rebal_Fault = EXTR_REBAL_FAULT_SHORT_DIST_COUNT_EXCEEDED;
        Extraction_SetFault(EXTR_FAULT_REBAL_SHORT_DIST_LIMIT_EXCEEDED);

        /* Event: '<S180>:182' */
        Extraction_DW.Extr_Rebal_EvShortRedistExceededEventCounter++;
    }

    if (Extraction_DW.sfEvent == Extraction_event_Extr_Sup_ResetRebalCnt_d5)
    {
        Extr_Rebal_RebalCnt = 0U;
        Extr_Rebal_ShrtRedistCnt = 0U;
    }

    if (Extraction_DW.sfEvent == Extraction_event_Extr_LowSpd_EvResetShortRedist)
    {
        Extr_Rebal_ShrtRedistCnt = 0U;
    }

    if (Extraction_DW.sfEvent == Extraction_event_Extr_Sup_ClearFault_oj)
    {
        Extr_Rebal_Fault = EXTR_REBAL_FAULT_NONE;
    }

    guard1 = false;
    guard2 = false;
    if (Extraction_DW.sfEvent == Extraction_event_Extr_Sup_StopAll ||
            Extraction_DW.sfEvent == Extraction_event_Extr_Sup_EvFoamDetected_f3)
    {
        /* Transition: '<S180>:26' */
        /* Transition: '<S180>:301' */
        guard1 = true;
    }
    else if (Extraction_DW.sfEvent == Extraction_event_Extr_Sup_EvRebalStart_dl)
    {
        /* Transition: '<S180>:219' */
        /* Transition: '<S180>:248' */
        Extraction_LoadRebalanceProfileIndex(Extr_Rebal_RebalCnt, &tmp);
        Extraction_LoadRebalanceProfile(tmp, &DistProfileLoaded);
        tmp_0 = (sint32)(Extr_Rebal_RebalCnt + 1U);
        if ((uint32)tmp_0 > 255U)
        {
            tmp_0 = 255;
        }

        Extr_Rebal_RebalCnt = (uint8)tmp_0;
        Extr_Rebal_RebInProgress = true;
        Extr_Rebal_ShrtRedistCnt = 0U;
        Extr_Rebal_PulseIndex = 0U;

        /* Transition: '<S180>:238' */
        if (!DistProfileLoaded || GESE_Extr_DistProfPulseCnt <= 0)
        {
            /* Transition: '<S180>:63' */
            Extr_Rebal_Fault = EXTR_REBAL_FAULT_INVALID_DIST_PROFILE;
            Extraction_SetFault(EXTR_FAULT_REBAL_INVALID_CONFIG);
            guard1 = true;
        }
        else
        {
            Extraction_IsMotorRunning(&DistProfileLoaded);
            tmp_0 = Extr_LowSpd_DistSpeed - 10;
            if (tmp_0 < -32768)
            {
                tmp_0 = -32768;
            }

            if (DistProfileLoaded && (Extr_LowSpd_DistSpeed > ((sint16)
                    EXTR_MIN_SPEED) && Extr_Inputs_MotorSignals.Speed >= tmp_0))
            {
                /* Transition: '<S180>:226' */
                /* Transition: '<S180>:225' */
                /* Exit Internal 'REBALANCE': '<S180>:38' */
                if (Extraction_DW.is_REBALANCE == Extraction_IN_RUNNING_jg)
                {
                    /* Exit Internal 'RUNNING': '<S180>:8' */
                    if (Extraction_DW.is_RUNNING ==
                            Extraction_IN_DISTRIBUTION_RAMP)
                    {
                        /* Exit Internal 'DISTRIBUTION_RAMP': '<S180>:69' */
                        Extraction_DW.is_DISTRIBUTION_RAMP = 0U;

                        /* Exit Internal 'RAMPING': '<S180>:198' */
                        Extraction_DW.is_RAMPING = 0U;

                        /* Exit 'DISTRIBUTION_RAMP': '<S180>:69' */
                        Extraction_DW.is_RUNNING = 0U;
                    }
                    else
                    {
                        Extraction_DW.is_RUNNING = 0U;
                    }

                    Extraction_DW.is_REBALANCE = 0U;
                }
                else
                {
                    Extraction_DW.is_REBALANCE = 0U;
                }

                if (Extraction_DW.is_REBALANCE ==
                        Extraction_IN_RAMPING_TO_DIST_SPEED)
                {
                }
                else
                {
                    Extraction_DW.is_REBALANCE =
                        Extraction_IN_RAMPING_TO_DIST_SPEED;
                    Extr_Rebal_State = EXTR_REBAL_STATE_RAMPING_TO_DIST_SPEED;

                    /* Entry 'RAMPING_TO_DIST_SPEED': '<S180>:52' */
                    Extraction_SetSpeedCommand(Extr_LowSpd_DistSpeed, 100);
                }
            }
            else
            {
                /* Transition: '<S180>:236' */
                /* Exit Internal 'REBALANCE': '<S180>:38' */
                if (Extraction_DW.is_REBALANCE == Extraction_IN_RUNNING_jg)
                {
                    /* Exit Internal 'RUNNING': '<S180>:8' */
                    if (Extraction_DW.is_RUNNING ==
                            Extraction_IN_DISTRIBUTION_RAMP)
                    {
                        /* Exit Internal 'DISTRIBUTION_RAMP': '<S180>:69' */
                        Extraction_DW.is_DISTRIBUTION_RAMP = 0U;

                        /* Exit Internal 'RAMPING': '<S180>:198' */
                        Extraction_DW.is_RAMPING = 0U;

                        /* Exit 'DISTRIBUTION_RAMP': '<S180>:69' */
                        Extraction_DW.is_RUNNING = 0U;
                    }
                    else
                    {
                        Extraction_DW.is_RUNNING = 0U;
                    }

                    Extraction_DW.is_REBALANCE = 0U;
                }
                else
                {
                    Extraction_DW.is_REBALANCE = 0U;
                }

                Extraction_DW.is_REBALANCE = Extraction_IN_RUNNING_jg;
                Extraction_enter_internal_RUNNING_lp();
            }
        }
    }
    else
    {
        switch (Extraction_DW.is_REBALANCE)
        {
          case Extraction_IN_IDLE_gt:
            /* During 'IDLE': '<S180>:2' */
            if (Extraction_DW.sfEvent ==
                    Extraction_event_Extr_LowSpd_EvDistributionStart)
            {
                /* Transition: '<S180>:177' */
                Extraction_LoadRebalanceProfile((*GESE_Extr_Setup).
                    Distribution_Profile_Index, &DistProfileLoaded);
                tmp_0 = (sint32)(Extr_Rebal_ShrtRedistCnt + 1U);
                if ((uint32)tmp_0 > 255U)
                {
                    tmp_0 = 255;
                }

                Extr_Rebal_ShrtRedistCnt = (uint8)tmp_0;
                Extraction_FindInitialDistributionPulse
                    (Extr_Inputs_MotorSignals.Speed_Ref, &Extr_Rebal_PulseIndex);

                /* Transition: '<S180>:238' */
                if (!DistProfileLoaded || GESE_Extr_DistProfPulseCnt <= 0)
                {
                    /* Transition: '<S180>:63' */
                    Extr_Rebal_Fault = EXTR_REBAL_FAULT_INVALID_DIST_PROFILE;
                    Extraction_SetFault(EXTR_FAULT_REBAL_INVALID_CONFIG);
                    Extraction_DW.is_REBALANCE = Extraction_IN_IDLE_gt;
                    Extr_Rebal_State = EXTR_REBAL_STATE_IDLE;

                    /* Entry 'IDLE': '<S180>:2' */
                    /* Simulink Function 'DistUtilsWrapperControl': '<S180>:186' */
                    Extraction_B.distUtilsCommand = DIST_UTILS_DISABLE;

                    /* Outputs for Function Call SubSystem: '<S180>/DistUtilsWrapperControl' */
                    Extraction_DistUtilsWrapperControl
                        (Extraction_B.distUtilsCommand,
                         &Extraction_B.DistUtilsWrapperControl);

                    /* End of Outputs for SubSystem: '<S180>/DistUtilsWrapperControl' */
                    Extr_Rebal_RebInProgress = false;
                }
                else
                {
                    Extraction_IsMotorRunning(&DistProfileLoaded);
                    tmp_0 = Extr_LowSpd_DistSpeed - 10;
                    if (tmp_0 < -32768)
                    {
                        tmp_0 = -32768;
                    }

                    if (DistProfileLoaded && (Extr_LowSpd_DistSpeed > ((sint16)
                            EXTR_MIN_SPEED) && Extr_Inputs_MotorSignals.Speed >=
                         tmp_0))
                    {
                        /* Transition: '<S180>:226' */
                        /* Transition: '<S180>:225' */
                        Extraction_DW.is_REBALANCE =
                            Extraction_IN_RAMPING_TO_DIST_SPEED;
                        Extr_Rebal_State =
                            EXTR_REBAL_STATE_RAMPING_TO_DIST_SPEED;

                        /* Entry 'RAMPING_TO_DIST_SPEED': '<S180>:52' */
                        Extraction_SetSpeedCommand(Extr_LowSpd_DistSpeed, 100);
                    }
                    else
                    {
                        /* Transition: '<S180>:236' */
                        Extraction_DW.is_REBALANCE = Extraction_IN_RUNNING_jg;
                        Extraction_enter_internal_RUNNING_lp();
                    }
                }
            }
            break;

          case Extraction_IN_RAMPING_TO_DIST_SPEED:
            /* During 'RAMPING_TO_DIST_SPEED': '<S180>:52' */
            if (Extraction_DW.sfEvent ==
                    Extraction_event_Extr_Sup_EvSpeedReached_ok)
            {
                /* Transition: '<S180>:196' */
                guard2 = true;
            }
            else
            {
                if (Extraction_DW.sfEvent ==
                        Extraction_event_Extr_Sup_EvSpeedErrorTimeout_le ||
                        Extraction_DW.sfEvent ==
                        Extraction_event_Extr_Accel_EvCabHit_ii)
                {
                    /* Transition: '<S180>:304' */
                    /* Transition: '<S180>:305' */
                    /* Transition: '<S180>:247' */
                    /* Transition: '<S180>:248' */
                    Extraction_LoadRebalanceProfileIndex(Extr_Rebal_RebalCnt,
                        &tmp);
                    Extraction_LoadRebalanceProfile(tmp, &DistProfileLoaded);
                    tmp_0 = (sint32)(Extr_Rebal_RebalCnt + 1U);
                    if ((uint32)tmp_0 > 255U)
                    {
                        tmp_0 = 255;
                    }

                    Extr_Rebal_RebalCnt = (uint8)tmp_0;
                    Extr_Rebal_RebInProgress = true;
                    Extr_Rebal_ShrtRedistCnt = 0U;
                    Extr_Rebal_PulseIndex = 0U;

                    /* Transition: '<S180>:238' */
                    if (!DistProfileLoaded || GESE_Extr_DistProfPulseCnt <= 0)
                    {
                        /* Transition: '<S180>:63' */
                        Extr_Rebal_Fault = EXTR_REBAL_FAULT_INVALID_DIST_PROFILE;
                        Extraction_SetFault(EXTR_FAULT_REBAL_INVALID_CONFIG);
                        Extraction_DW.is_REBALANCE = Extraction_IN_IDLE_gt;
                        Extr_Rebal_State = EXTR_REBAL_STATE_IDLE;

                        /* Entry 'IDLE': '<S180>:2' */
                        /* Simulink Function 'DistUtilsWrapperControl': '<S180>:186' */
                        Extraction_B.distUtilsCommand = DIST_UTILS_DISABLE;

                        /* Outputs for Function Call SubSystem: '<S180>/DistUtilsWrapperControl' */
                        Extraction_DistUtilsWrapperControl
                            (Extraction_B.distUtilsCommand,
                             &Extraction_B.DistUtilsWrapperControl);

                        /* End of Outputs for SubSystem: '<S180>/DistUtilsWrapperControl' */
                        Extr_Rebal_RebInProgress = false;
                    }
                    else
                    {
                        Extraction_IsMotorRunning(&DistProfileLoaded);
                        tmp_0 = Extr_LowSpd_DistSpeed - 10;
                        if (tmp_0 < -32768)
                        {
                            tmp_0 = -32768;
                        }

                        if (DistProfileLoaded && (Extr_LowSpd_DistSpeed >
                                                  ((sint16)EXTR_MIN_SPEED) &&
                                                  Extr_Inputs_MotorSignals.Speed
                             >= tmp_0))
                        {
                            /* Transition: '<S180>:226' */
                            /* Transition: '<S180>:225' */
                            Extraction_DW.is_REBALANCE =
                                Extraction_IN_RAMPING_TO_DIST_SPEED;
                            Extr_Rebal_State =
                                EXTR_REBAL_STATE_RAMPING_TO_DIST_SPEED;

                            /* Entry 'RAMPING_TO_DIST_SPEED': '<S180>:52' */
                            Extraction_SetSpeedCommand(Extr_LowSpd_DistSpeed,
                                100);
                        }
                        else
                        {
                            /* Transition: '<S180>:236' */
                            guard2 = true;
                        }
                    }
                }
            }
            break;

          case Extraction_IN_RUNNING_jg:
            Extraction_RUNNING_bk();
            break;
        }
    }

    if (guard2)
    {
        Extraction_DW.is_REBALANCE = Extraction_IN_RUNNING_jg;
        Extraction_enter_internal_RUNNING_lp();
    }

    if (guard1)
    {
        /* Exit Internal 'REBALANCE': '<S180>:38' */
        if (Extraction_DW.is_REBALANCE == Extraction_IN_RUNNING_jg)
        {
            /* Exit Internal 'RUNNING': '<S180>:8' */
            if (Extraction_DW.is_RUNNING == Extraction_IN_DISTRIBUTION_RAMP)
            {
                /* Exit Internal 'DISTRIBUTION_RAMP': '<S180>:69' */
                Extraction_DW.is_DISTRIBUTION_RAMP = 0U;

                /* Exit Internal 'RAMPING': '<S180>:198' */
                Extraction_DW.is_RAMPING = 0U;

                /* Exit 'DISTRIBUTION_RAMP': '<S180>:69' */
                Extraction_DW.is_RUNNING = 0U;
            }
            else
            {
                Extraction_DW.is_RUNNING = 0U;
            }

            Extraction_DW.is_REBALANCE = 0U;
        }
        else
        {
            Extraction_DW.is_REBALANCE = 0U;
        }

        if (Extraction_DW.is_REBALANCE == Extraction_IN_IDLE_gt)
        {
        }
        else
        {
            Extraction_DW.is_REBALANCE = Extraction_IN_IDLE_gt;
            Extr_Rebal_State = EXTR_REBAL_STATE_IDLE;

            /* Entry 'IDLE': '<S180>:2' */
            /* Simulink Function 'DistUtilsWrapperControl': '<S180>:186' */
            Extraction_B.distUtilsCommand = DIST_UTILS_DISABLE;

            /* Outputs for Function Call SubSystem: '<S180>/DistUtilsWrapperControl' */
            Extraction_DistUtilsWrapperControl(Extraction_B.distUtilsCommand,
                &Extraction_B.DistUtilsWrapperControl);

            /* End of Outputs for SubSystem: '<S180>/DistUtilsWrapperControl' */
            Extr_Rebal_RebInProgress = false;
        }
    }
}

/* Function for Chart: '<S12>/Extr_Rebal_Logic' */
static void Extraction_c24_Extraction(void)
{
    /* During: Extraction/Extr_Rebalance/Extr_Rebal_Logic */
    if (Extraction_DW.is_active_c24_Extraction == 0U)
    {
        /* Entry: Extraction/Extr_Rebalance/Extr_Rebal_Logic */
        Extraction_DW.is_active_c24_Extraction = 1U;

        /* Entry Internal: Extraction/Extr_Rebalance/Extr_Rebal_Logic */
        /* Transition: '<S180>:212' */
        if (Extraction_DW.is_c24_Extraction == Extraction_IN_REBALANCE_bo)
        {
        }
        else
        {
            Extraction_DW.is_c24_Extraction = Extraction_IN_REBALANCE_bo;

            /* Entry 'REBALANCE': '<S180>:38' */
            Extr_Rebal_RebalCnt = 0U;
            Extr_Rebal_ShrtRedistCnt = 0U;
            Extr_Rebal_RebInProgress = false;
        }

        /* Entry Internal 'REBALANCE': '<S180>:38' */
        /* Transition: '<S180>:39' */
        if (Extraction_DW.is_REBALANCE == Extraction_IN_IDLE_gt)
        {
        }
        else
        {
            Extraction_DW.is_REBALANCE = Extraction_IN_IDLE_gt;
            Extr_Rebal_State = EXTR_REBAL_STATE_IDLE;

            /* Entry 'IDLE': '<S180>:2' */
            /* Simulink Function 'DistUtilsWrapperControl': '<S180>:186' */
            Extraction_B.distUtilsCommand = DIST_UTILS_DISABLE;

            /* Outputs for Function Call SubSystem: '<S180>/DistUtilsWrapperControl' */
            Extraction_DistUtilsWrapperControl(Extraction_B.distUtilsCommand,
                &Extraction_B.DistUtilsWrapperControl);

            /* End of Outputs for SubSystem: '<S180>/DistUtilsWrapperControl' */
            Extr_Rebal_RebInProgress = false;
        }
    }
    else if (Extraction_DW.is_c24_Extraction != Extraction_IN_REBALANCE_bo)
    {
    }
    else
    {
        Extraction_REBALANCE();
    }
}

/* Function for Chart: '<S12>/Extr_Rebal_Logic' */
static void Extraction_chartstep_c24_Extraction(void)
{
    Extraction_c24_Extraction();
}

/* Model step function */
void Extraction__Handler5ms(void)
{
    /* RootInportFunctionCallGenerator: '<Root>/RootFcnCall_InsertedFor_Extraction__Handler5ms_at_outport_1' incorporates:
     *  SubSystem: '<S13>/Extr_Supervisor'
     */
    /* SignalConversion: '<S193>/TmpLatchAtACU_MaxSpeedErrorOutport1' */
    Extraction_B.Extr_Inputs_MaxSpeedError_iv = Extr_Inputs_MaxSpeedError;

    /* SignalConversion: '<S193>/TmpLatchAtACU_RampTimeoutOutport1' */
    Extraction_B.Extr_Inputs_RampTimeout_dy = Extr_Inputs_RampTimeout;

    /* SignalConversion: '<S193>/TmpLatchAtMCI_MotorSignalsOutport1' */
    Extraction_B.Extr_Inputs_MotorSignals_os = Extr_Inputs_MotorSignals;

    /* SignalConversion: '<S193>/TmpLatchAtExtr_Sup_SpdComCounterOutport1' */
    Extraction_B.Extr_Sup_SpdComCounter_cy = Extraction_B.FixPtSum1;

    /* SignalConversion: '<S193>/TmpLatchAtExtr_Sup_SpdCommandOutport1' */
    Extraction_B.Extr_Sup_SpdCommand_hvd[0] = Extr_Sup_SpdCommand[0];
    Extraction_B.Extr_Sup_SpdCommand_hvd[1] = Extr_Sup_SpdCommand[1];

    /* Chart: '<S204>/TriggerGen' */
    Extraction_TriggerGen(&Extraction_B.sf_TriggerGen,
                          &Extraction_DW.sf_TriggerGen);

    /* SignalConversion: '<S193>/HiddenBuf_InsertedFor_Extr_Sup_Logic_at_inport_0' */
    Extraction_B.HiddenBuf_InsertedFor_Extr_Sup_Logic_at_inport_0[0] =
        Extraction_B.sf_TriggerGen.trigger;
    Extraction_B.HiddenBuf_InsertedFor_Extr_Sup_Logic_at_inport_0[1] =
        Extraction_B.OutportBufferForACU_ExtrStart;
    Extraction_B.HiddenBuf_InsertedFor_Extr_Sup_Logic_at_inport_0[2] =
        Extraction_B.OutportBufferForACU_ExtrStop;
    Extraction_B.HiddenBuf_InsertedFor_Extr_Sup_Logic_at_inport_0[3] =
        Extraction_B.OutportBufferForACU_ExtrResume;
    Extraction_B.HiddenBuf_InsertedFor_Extr_Sup_Logic_at_inport_0[4] =
        Extraction_B.OutportBufferForACU_ExtrEnd;
    Extraction_B.HiddenBuf_InsertedFor_Extr_Sup_Logic_at_inport_0[5] =
        Extraction_B.OutportBufferForACU_ResetLoadMass;
    Extraction_B.HiddenBuf_InsertedFor_Extr_Sup_Logic_at_inport_0[6] =
        Extraction_B.OutportBufferForACU_NotifyFoamDetection;
    Extraction_B.HiddenBuf_InsertedFor_Extr_Sup_Logic_at_inport_0[7] =
        Extraction_B.OutportBufferForExtr_LowSpd_EvRebalStart;
    Extraction_B.HiddenBuf_InsertedFor_Extr_Sup_Logic_at_inport_0[8] =
        Extraction_B.OutportBufferForExtr_Rebal_EvComplete;
    Extraction_B.HiddenBuf_InsertedFor_Extr_Sup_Logic_at_inport_0[9] =
        Extraction_B.OutportBufferForExtr_LowSpd_EvComplete;
    Extraction_B.HiddenBuf_InsertedFor_Extr_Sup_Logic_at_inport_0[10] =
        Extraction_B.VariantMergeForOutportExtr_HighSpd_EvComplete;
    Extraction_B.HiddenBuf_InsertedFor_Extr_Sup_Logic_at_inport_0[11] =
        Extraction_B.VariantMergeForOutportExtr_HighSpd_EvStartRebal;
    Extraction_B.HiddenBuf_InsertedFor_Extr_Sup_Logic_at_inport_0[12] =
        Extraction_B.OutportBufferForExtr_Rebal_EvP2PTorqFault;
    Extraction_B.HiddenBuf_InsertedFor_Extr_Sup_Logic_at_inport_0[13] =
        Extraction_B.OutportBufferForExtr_Rebal_EvShortRedistExceeded;
    Extraction_B.HiddenBuf_InsertedFor_Extr_Sup_Logic_at_inport_0[14] =
        Extraction_B.OutportBufferForExtr_Rebal_EvCleanWasherException;
    Extraction_B.HiddenBuf_InsertedFor_Extr_Sup_Logic_at_inport_0[15] =
        Extraction_B.VariantMergeForOutportExtr_Accel_EvCleanWasherException;
    Extraction_B.HiddenBuf_InsertedFor_Extr_Sup_Logic_at_inport_0[16] =
        Extraction_B.OutportBufferForExtr_LowSpd_EvCleanWasherException;
    Extraction_B.HiddenBuf_InsertedFor_Extr_Sup_Logic_at_inport_0[17] =
        Extraction_B.VariantMergeForOutportExtr_Accel_EvCabHit;

    /* Chart: '<S193>/Extr_Sup_Logic' incorporates:
     *  TriggerPort: '<S201>/input events'
     */
    Extraction_B.zcEvent_idx_0 = false;
    for (Extraction_B.i = 0; Extraction_B.i < 18; Extraction_B.i++)
    {
        Extraction_B.zcEvent_idx_1 = (sint32)
            Extraction_B.HiddenBuf_InsertedFor_Extr_Sup_Logic_at_inport_0[Extraction_B.i]
            != (Extraction_PrevZCX.Extr_Sup_Logic_Trig_ZCE[Extraction_B.i] ==
                POS_ZCSIG) &&
            Extraction_PrevZCX.Extr_Sup_Logic_Trig_ZCE[Extraction_B.i] !=
            UNINITIALIZED_ZCSIG;
        Extraction_B.zcEvent_idx_0 = Extraction_B.zcEvent_idx_0 ||
            Extraction_B.zcEvent_idx_1;
        Extraction_B.zcEvent_mb[Extraction_B.i] = Extraction_B.zcEvent_idx_1;
    }

    if (Extraction_B.zcEvent_idx_0)
    {
        for (Extraction_B.i = 0; Extraction_B.i < 18; Extraction_B.i++)
        {
            Extraction_B.inputevents_n2[Extraction_B.i] = (sint8)
                (Extraction_B.zcEvent_mb[Extraction_B.i] ?
                 Extraction_B.HiddenBuf_InsertedFor_Extr_Sup_Logic_at_inport_0[Extraction_B.i]
                 ? RISING_ZCEVENT : FALLING_ZCEVENT : NO_ZCEVENT);
        }

        /* Gateway: Extraction/Extr_Supervisor/Extr_Supervisor/Extr_Sup_Logic */
        Extraction_B.i = 0;
        if (Extraction_B.inputevents_n2[0U] != 0)
        {
            Extraction_B.i = 1;

            /* Event: '<S201>:1' */
            Extraction_B.sfEvent_ha = Extraction_event_ticks_ml;
            Extraction_chartstep_c7_Extraction(&Extraction_B.sfEvent_ha);
        }

        if (Extraction_B.inputevents_n2[1U] != 0)
        {
            Extraction_B.i = 1;

            /* Event: '<S201>:2' */
            Extraction_B.sfEvent_ha = Extraction_event_ACU_ExtrStart;
            Extraction_chartstep_c7_Extraction(&Extraction_B.sfEvent_ha);
        }

        if (Extraction_B.inputevents_n2[2U] != 0)
        {
            Extraction_B.i = 1;

            /* Event: '<S201>:3' */
            Extraction_B.sfEvent_ha = Extraction_event_ACU_ExtrStop;
            Extraction_chartstep_c7_Extraction(&Extraction_B.sfEvent_ha);
        }

        if (Extraction_B.inputevents_n2[3U] != 0)
        {
            Extraction_B.i = 1;

            /* Event: '<S201>:219' */
            Extraction_B.sfEvent_ha = Extraction_event_ACU_ExtrResume;
            Extraction_chartstep_c7_Extraction(&Extraction_B.sfEvent_ha);
        }

        if (Extraction_B.inputevents_n2[4U] != 0)
        {
            Extraction_B.i = 1;

            /* Event: '<S201>:198' */
            Extraction_B.sfEvent_ha = Extraction_event_ACU_ExtrEnd;
            Extraction_chartstep_c7_Extraction(&Extraction_B.sfEvent_ha);
        }

        if (Extraction_B.inputevents_n2[5U] != 0)
        {
            Extraction_B.i = 1;

            /* Event: '<S201>:65' */
            Extraction_B.sfEvent_ha = Extraction_event_ACU_ResetLoadMass;
            Extraction_chartstep_c7_Extraction(&Extraction_B.sfEvent_ha);
        }

        if (Extraction_B.inputevents_n2[6U] != 0)
        {
            Extraction_B.i = 1;

            /* Event: '<S201>:56' */
            Extraction_B.sfEvent_ha = Extraction_event_ACU_NotifFoamDetec;
            Extraction_chartstep_c7_Extraction(&Extraction_B.sfEvent_ha);
        }

        if (Extraction_B.inputevents_n2[7U] != 0)
        {
            Extraction_B.i = 1;

            /* Event: '<S201>:21' */
            Extraction_B.sfEvent_ha = Extraction_event_Extr_LowSpd_EvRebalStart;
            Extraction_chartstep_c7_Extraction(&Extraction_B.sfEvent_ha);
        }

        if (Extraction_B.inputevents_n2[8U] != 0)
        {
            Extraction_B.i = 1;

            /* Event: '<S201>:12' */
            Extraction_B.sfEvent_ha = Extraction_event_Extr_Rebal_EvComplete;
            Extraction_chartstep_c7_Extraction(&Extraction_B.sfEvent_ha);
        }

        if (Extraction_B.inputevents_n2[9U] != 0)
        {
            Extraction_B.i = 1;

            /* Event: '<S201>:13' */
            Extraction_B.sfEvent_ha = Extraction_event_Extr_LowSpd_EvComplete_jg;
            Extraction_chartstep_c7_Extraction(&Extraction_B.sfEvent_ha);
        }

        if (Extraction_B.inputevents_n2[10U] != 0)
        {
            Extraction_B.i = 1;

            /* Event: '<S201>:28' */
            Extraction_B.sfEvent_ha = Extraction_event_Extr_HighSpd_EvComplete;
            Extraction_chartstep_c7_Extraction(&Extraction_B.sfEvent_ha);
        }

        if (Extraction_B.inputevents_n2[11U] != 0)
        {
            Extraction_B.i = 1;

            /* Event: '<S201>:88' */
            Extraction_B.sfEvent_ha = Extraction_event_Extr_HighSpd_EvRebalStart;
            Extraction_chartstep_c7_Extraction(&Extraction_B.sfEvent_ha);
        }

        if (Extraction_B.inputevents_n2[12U] != 0)
        {
            Extraction_B.i = 1;

            /* Event: '<S201>:107' */
            Extraction_B.sfEvent_ha = Extraction_event_Extr_Rebal_P2PTorqueFault;
            Extraction_chartstep_c7_Extraction(&Extraction_B.sfEvent_ha);
        }

        if (Extraction_B.inputevents_n2[13U] != 0)
        {
            Extraction_B.i = 1;

            /* Event: '<S201>:108' */
            Extraction_B.sfEvent_ha =
                Extraction_event_Extr_Rebal_EvShortRedistExceeded;
            Extraction_chartstep_c7_Extraction(&Extraction_B.sfEvent_ha);
        }

        if (Extraction_B.inputevents_n2[14U] != 0)
        {
            Extraction_B.i = 1;

            /* Event: '<S201>:161' */
            Extraction_B.sfEvent_ha =
                Extraction_event_Extr_Accel_EvCleanWasherException;
            Extraction_chartstep_c7_Extraction(&Extraction_B.sfEvent_ha);
        }

        if (Extraction_B.inputevents_n2[15U] != 0)
        {
            Extraction_B.i = 1;

            /* Event: '<S201>:162' */
            Extraction_B.sfEvent_ha =
                Extraction_event_Extr_Rebal_EvCleanWasherException;
            Extraction_chartstep_c7_Extraction(&Extraction_B.sfEvent_ha);
        }

        if (Extraction_B.inputevents_n2[16U] != 0)
        {
            Extraction_B.i = 1;

            /* Event: '<S201>:163' */
            Extraction_B.sfEvent_ha =
                Extraction_event_Extr_LowSpd_EvCleanWasherException;
            Extraction_chartstep_c7_Extraction(&Extraction_B.sfEvent_ha);
        }

        if (Extraction_B.inputevents_n2[17U] != 0)
        {
            Extraction_B.i = 1;

            /* Event: '<S201>:184' */
            Extraction_B.sfEvent_ha = Extraction_event_Extr_Accel_EvCabHit_ja;
            Extraction_chartstep_c7_Extraction(&Extraction_B.sfEvent_ha);
        }

        if (Extraction_B.i != 0 &&
                Extraction_DW.Extr_Sup_EvLowSpdStartEventCounter > 0U)
        {
            Extraction_B.Extr_Sup_EvLowSpdStart =
                !Extraction_B.Extr_Sup_EvLowSpdStart;
            Extraction_DW.Extr_Sup_EvLowSpdStartEventCounter--;
        }

        if (Extraction_B.i != 0 &&
                Extraction_DW.Extr_Sup_EvHighSpdStartEventCounter > 0U)
        {
            Extraction_B.Extr_Sup_EvHighSpdStart =
                !Extraction_B.Extr_Sup_EvHighSpdStart;
            Extraction_DW.Extr_Sup_EvHighSpdStartEventCounter--;
        }

        if (Extraction_B.i != 0 && Extraction_DW.Extr_Sup_EvStopAllEventCounter >
            0U)
        {
            Extraction_B.Extr_Sup_EvStopAll = !Extraction_B.Extr_Sup_EvStopAll;
            Extraction_DW.Extr_Sup_EvStopAllEventCounter--;
        }

        if (Extraction_B.i != 0 &&
                Extraction_DW.Extr_Sup_ResetLoadMassEventCounter > 0U)
        {
            Extraction_B.Extr_Sup_ResetLoadMass =
                !Extraction_B.Extr_Sup_ResetLoadMass;
            Extraction_DW.Extr_Sup_ResetLoadMassEventCounter--;
        }

        if (Extraction_B.i != 0 &&
                Extraction_DW.Extr_Sup_EvRebalStartEventCounter > 0U)
        {
            Extraction_B.Extr_Sup_EvRebalStart =
                !Extraction_B.Extr_Sup_EvRebalStart;
            Extraction_DW.Extr_Sup_EvRebalStartEventCounter--;
        }

        if (Extraction_B.i != 0 &&
                Extraction_DW.Extr_Sup_ResetRebalCntEventCounter > 0U)
        {
            Extraction_B.Extr_Sup_ResetRebalCnt =
                !Extraction_B.Extr_Sup_ResetRebalCnt;
            Extraction_DW.Extr_Sup_ResetRebalCntEventCounter--;
        }

        if (Extraction_B.i != 0 &&
                Extraction_DW.Extr_Sup_EvFoamDetectedEventCounter > 0U)
        {
            Extraction_B.Extr_Sup_EvFoamDetected =
                !Extraction_B.Extr_Sup_EvFoamDetected;
            Extraction_DW.Extr_Sup_EvFoamDetectedEventCounter--;
        }

        if (Extraction_B.i != 0 && Extraction_DW.Extr_Sup_ClearFaultEventCounter
            > 0U)
        {
            Extraction_B.Extr_Sup_ClearFault = !Extraction_B.Extr_Sup_ClearFault;
            Extraction_DW.Extr_Sup_ClearFaultEventCounter--;
        }

        if (Extraction_B.i != 0 &&
                Extraction_DW.Extr_Sup_EvResetSoftCabHitCntEventCounter > 0U)
        {
            Extraction_B.Extr_Sup_EvResetSoftCabHitCnt =
                !Extraction_B.Extr_Sup_EvResetSoftCabHitCnt;
            Extraction_DW.Extr_Sup_EvResetSoftCabHitCntEventCounter--;
        }
    }

    for (Extraction_B.i = 0; Extraction_B.i < 18; Extraction_B.i++)
    {
        Extraction_PrevZCX.Extr_Sup_Logic_Trig_ZCE[Extraction_B.i] =
            Extraction_B.HiddenBuf_InsertedFor_Extr_Sup_Logic_at_inport_0[Extraction_B.i];
    }

    /* DataStoreWrite: '<S193>/Data Store Write' */
    Extr_Sup_State = Extraction_B.Extr_Sup_State_d3;

    /* Chart: '<S193>/Extr_Sup_MotorSpeedCheck' incorporates:
     *  TriggerPort: '<S202>/input events'
     */
    Extraction_B.zcEvent_idx_0 = (sint32)Extraction_B.sf_TriggerGen.trigger !=
        (Extraction_PrevZCX.Extr_Sup_MotorSpeedCheck_Trig_ZCE[0] == POS_ZCSIG) &&
        Extraction_PrevZCX.Extr_Sup_MotorSpeedCheck_Trig_ZCE[0] !=
        UNINITIALIZED_ZCSIG;
    Extraction_B.zcEvent_idx_1 = (sint32)Extraction_B.Extr_Sup_EvStopAll !=
        (Extraction_PrevZCX.Extr_Sup_MotorSpeedCheck_Trig_ZCE[1] == POS_ZCSIG) &&
        Extraction_PrevZCX.Extr_Sup_MotorSpeedCheck_Trig_ZCE[1] !=
        UNINITIALIZED_ZCSIG;
    if (Extraction_B.zcEvent_idx_0 || Extraction_B.zcEvent_idx_1)
    {
        /* Gateway: Extraction/Extr_Supervisor/Extr_Supervisor/Extr_Sup_MotorSpeedCheck */
        Extraction_B.i = 0;
        if ((sint8)(Extraction_B.zcEvent_idx_0 ?
                    Extraction_B.sf_TriggerGen.trigger ? RISING_ZCEVENT :
                    FALLING_ZCEVENT : NO_ZCEVENT) != 0)
        {
            Extraction_B.i = 1;

            /* Event: '<S202>:71' */
            Extraction_B.sfEvent_ha = Extraction_event_Trigger_d3;
            Extraction_chartstep_c47_Extraction
                (&Extraction_B.Extr_Inputs_MaxSpeedError_iv,
                 &Extraction_B.Extr_Inputs_RampTimeout_dy,
                 &Extraction_B.Extr_Inputs_MotorSignals_os,
                 &Extraction_B.Extr_Sup_SpdComCounter_cy,
                 Extraction_B.Extr_Sup_SpdCommand_hvd, &Extraction_B.sfEvent_ha);
        }

        if ((sint8)(Extraction_B.zcEvent_idx_1 ? Extraction_B.Extr_Sup_EvStopAll
                    ? RISING_ZCEVENT : FALLING_ZCEVENT : NO_ZCEVENT) != 0)
        {
            Extraction_B.i = 1;

            /* Event: '<S202>:70' */
            Extraction_B.sfEvent_ha = Extraction_event_Extr_Sup_EvStopAll_g1;
            Extraction_chartstep_c47_Extraction
                (&Extraction_B.Extr_Inputs_MaxSpeedError_iv,
                 &Extraction_B.Extr_Inputs_RampTimeout_dy,
                 &Extraction_B.Extr_Inputs_MotorSignals_os,
                 &Extraction_B.Extr_Sup_SpdComCounter_cy,
                 Extraction_B.Extr_Sup_SpdCommand_hvd, &Extraction_B.sfEvent_ha);
        }

        if (Extraction_B.i != 0 &&
                Extraction_DW.Extr_Sup_EvSpeedErrorTimeoutEventCounter > 0U)
        {
            Extraction_B.Extr_Sup_EvSpeedErrorTimeout_ov =
                !Extraction_B.Extr_Sup_EvSpeedErrorTimeout_ov;
            Extraction_DW.Extr_Sup_EvSpeedErrorTimeoutEventCounter--;
        }

        if (Extraction_B.i != 0 &&
                Extraction_DW.Extr_Sup_EvSpeedReachedEventCounter > 0U)
        {
            Extraction_B.Extr_Sup_EvSpeedReached_fh =
                !Extraction_B.Extr_Sup_EvSpeedReached_fh;
            Extraction_DW.Extr_Sup_EvSpeedReachedEventCounter--;
        }
    }

    Extraction_PrevZCX.Extr_Sup_MotorSpeedCheck_Trig_ZCE[0] =
        Extraction_B.sf_TriggerGen.trigger;
    Extraction_PrevZCX.Extr_Sup_MotorSpeedCheck_Trig_ZCE[1] =
        Extraction_B.Extr_Sup_EvStopAll;

    /* SignalConversion: '<S193>/OutportBufferForExtr_Sup_CabHitEnabled' */
    Extraction_B.OutportBufferForExtr_Sup_CabHitEnabled =
        Extraction_B.Extr_Sup_CabHitEnabled;

    /* SignalConversion: '<S193>/OutportBufferForExtr_Sup_ClearFault' */
    Extraction_B.OutportBufferForExtr_Sup_ClearFault =
        Extraction_B.Extr_Sup_ClearFault;

    /* SignalConversion: '<S193>/OutportBufferForExtr_Sup_EvFoamDetected' */
    Extraction_B.OutportBufferForExtr_Sup_EvFoamDetected =
        Extraction_B.Extr_Sup_EvFoamDetected;

    /* SignalConversion: '<S193>/OutportBufferForExtr_Sup_EvHighSpdStart' */
    Extraction_B.OutportBufferForExtr_Sup_EvHighSpdStart =
        Extraction_B.Extr_Sup_EvHighSpdStart;

    /* SignalConversion: '<S193>/OutportBufferForExtr_Sup_EvLowSpdStart' */
    Extraction_B.zcEvent_idx_1 = Extraction_B.Extr_Sup_EvLowSpdStart;

    /* SignalConversion: '<S193>/OutportBufferForExtr_Sup_EvRebalStart' */
    Extraction_B.OutportBufferForExtr_Sup_EvRebalStart =
        Extraction_B.Extr_Sup_EvRebalStart;

    /* SignalConversion: '<S193>/OutportBufferForExtr_Sup_EvResetSoftCabHitCnt' */
    Extraction_B.OutportBufferForExtr_Sup_EvResetSoftCabHitCnt =
        Extraction_B.Extr_Sup_EvResetSoftCabHitCnt;

    /* SignalConversion: '<S193>/OutportBufferForExtr_Sup_EvSpeedErrorTimeout' */
    Extraction_B.Extr_Sup_EvSpeedErrorTimeout =
        Extraction_B.Extr_Sup_EvSpeedErrorTimeout_ov;

    /* SignalConversion: '<S193>/OutportBufferForExtr_Sup_EvSpeedReached' */
    Extraction_B.Extr_Sup_EvSpeedReached =
        Extraction_B.Extr_Sup_EvSpeedReached_fh;

    /* SignalConversion: '<S193>/OutportBufferForExtr_Sup_EvStopAll' */
    Extraction_B.OutportBufferForExtr_Sup_EvStopAll =
        Extraction_B.Extr_Sup_EvStopAll;

    /* SignalConversion: '<S193>/OutportBufferForExtr_Sup_ResetLoadMass' */
    Extraction_B.OutportBufferForExtr_Sup_ResetLoadMass =
        Extraction_B.Extr_Sup_ResetLoadMass;

    /* SignalConversion: '<S193>/OutportBufferForExtr_Sup_ResetRebalCnt' */
    Extraction_B.OutportBufferForExtr_Sup_ResetRebalCnt =
        Extraction_B.Extr_Sup_ResetRebalCnt;

    /* SignalConversion: '<S193>/OutportBufferForExtr_Sup_VibMapEnabled' */
    Extraction_B.OutportBufferForExtr_Sup_VibMapEnabled =
        Extraction_B.Extr_Sup_VibMapEnabled;

    /* Chart: '<S193>/Extr_Sup_LoadMassMerge' */
    /* Gateway: Extraction/Extr_Supervisor/Extr_Supervisor/Extr_Sup_LoadMassMerge */
    Extraction_B.Extr_LowSpd_LoadMass_prev =
        Extraction_DW.Extr_LowSpd_LoadMass_start;
    Extraction_DW.Extr_LowSpd_LoadMass_start =
        Extraction_B.Extr_LowSpd_LoadMass_gp;
    Extraction_B.Extr_HighSpd_LoadMass_prev =
        Extraction_DW.Extr_HighSpd_LoadMass_start;
    Extraction_DW.Extr_HighSpd_LoadMass_start =
        Extraction_B.VariantMergeForOutportExtr_HighSpd_LoadMass;

    /* During: Extraction/Extr_Supervisor/Extr_Supervisor/Extr_Sup_LoadMassMerge */
    if (Extraction_DW.is_active_c20_Extraction == 0U)
    {
        /* Entry: Extraction/Extr_Supervisor/Extr_Supervisor/Extr_Sup_LoadMassMerge */
        Extraction_DW.is_active_c20_Extraction = 1U;

        /* Entry Internal: Extraction/Extr_Supervisor/Extr_Supervisor/Extr_Sup_LoadMassMerge */
        /* Transition: '<S200>:5' */
        /* Entry 'RUNNING': '<S200>:4' */
        if (Extraction_B.Extr_LowSpd_LoadMass_gp !=
                Extraction_DW.Extr_LowSpd_LoadMass_start)
        {
            Extr_Sup_LoadMass = Extraction_B.Extr_LowSpd_LoadMass_gp;
        }

        if (Extraction_B.VariantMergeForOutportExtr_HighSpd_LoadMass !=
                Extraction_DW.Extr_HighSpd_LoadMass_start)
        {
            Extr_Sup_LoadMass =
                Extraction_B.VariantMergeForOutportExtr_HighSpd_LoadMass;
        }
    }
    else
    {
        /* During 'RUNNING': '<S200>:4' */
        if (Extraction_B.Extr_LowSpd_LoadMass_prev !=
                Extraction_DW.Extr_LowSpd_LoadMass_start)
        {
            Extr_Sup_LoadMass = Extraction_B.Extr_LowSpd_LoadMass_gp;
        }

        if (Extraction_B.Extr_HighSpd_LoadMass_prev !=
                Extraction_DW.Extr_HighSpd_LoadMass_start)
        {
            Extr_Sup_LoadMass =
                Extraction_B.VariantMergeForOutportExtr_HighSpd_LoadMass;
        }
    }

    /* End of Chart: '<S193>/Extr_Sup_LoadMassMerge' */

    /* RootInportFunctionCallGenerator: '<Root>/RootFcnCall_InsertedFor_Extraction__Handler5ms_at_outport_1' incorporates:
     *  SubSystem: '<S11>/Extr_LowSpd_Supervisor'
     */
    /* Chart: '<S174>/TriggerGen' */
    Extraction_TriggerGen(&Extraction_B.sf_TriggerGen_ih,
                          &Extraction_DW.sf_TriggerGen_ih);

    /* Chart: '<S122>/Extr_LowSpd_SupervisorLogic' incorporates:
     *  TriggerPort: '<S171>/input events'
     */
    Extraction_B.zcEvent[0] = (sint32)Extraction_B.sf_TriggerGen_ih.trigger !=
        (Extraction_PrevZCX.Extr_LowSpd_SupervisorLogic_Trig_ZCE[0] == POS_ZCSIG)
        && Extraction_PrevZCX.Extr_LowSpd_SupervisorLogic_Trig_ZCE[0] !=
        UNINITIALIZED_ZCSIG;

    /* End of Outputs for RootInportFunctionCallGenerator: '<Root>/RootFcnCall_InsertedFor_Extraction__Handler5ms_at_outport_1' */

    /* RootInportFunctionCallGenerator: '<Root>/RootFcnCall_InsertedFor_Extraction__Handler5ms_at_outport_1' incorporates:
     *  SubSystem: '<S13>/Extr_Supervisor'
     */
    /* SignalConversion: '<S193>/OutportBufferForExtr_Sup_EvLowSpdStart' */
    Extraction_B.zcEvent[1] = (sint32)Extraction_B.Extr_Sup_EvLowSpdStart !=
        (Extraction_PrevZCX.Extr_LowSpd_SupervisorLogic_Trig_ZCE[1] == POS_ZCSIG)
        && Extraction_PrevZCX.Extr_LowSpd_SupervisorLogic_Trig_ZCE[1] !=
        UNINITIALIZED_ZCSIG;
    Extraction_B.zcEvent[2] = (sint32)
        Extraction_B.OutportBufferForExtr_Rebal_EvComplete !=
        (Extraction_PrevZCX.Extr_LowSpd_SupervisorLogic_Trig_ZCE[2] == POS_ZCSIG)
        && Extraction_PrevZCX.Extr_LowSpd_SupervisorLogic_Trig_ZCE[2] !=
        UNINITIALIZED_ZCSIG;
    Extraction_B.zcEvent[3] = (sint32)
        Extraction_B.OutportBufferForExtr_LowSpd_EvLoadMeasComplete !=
        (Extraction_PrevZCX.Extr_LowSpd_SupervisorLogic_Trig_ZCE[3] == POS_ZCSIG)
        && Extraction_PrevZCX.Extr_LowSpd_SupervisorLogic_Trig_ZCE[3] !=
        UNINITIALIZED_ZCSIG;
    Extraction_B.zcEvent[4] = (sint32)
        Extraction_B.VariantMergeForOutportExt_LowSpd_EvOptRampComplete !=
        (Extraction_PrevZCX.Extr_LowSpd_SupervisorLogic_Trig_ZCE[4] == POS_ZCSIG)
        && Extraction_PrevZCX.Extr_LowSpd_SupervisorLogic_Trig_ZCE[4] !=
        UNINITIALIZED_ZCSIG;
    Extraction_B.zcEvent[5] = (sint32)
        Extraction_B.VariantMergeForOutportExt_LowSpd_EvRedist !=
        (Extraction_PrevZCX.Extr_LowSpd_SupervisorLogic_Trig_ZCE[5] == POS_ZCSIG)
        && Extraction_PrevZCX.Extr_LowSpd_SupervisorLogic_Trig_ZCE[5] !=
        UNINITIALIZED_ZCSIG;
    Extraction_B.zcEvent[6] = (sint32)
        Extraction_B.OutportBufferForExtr_LowSpd_EvMddpComplete !=
        (Extraction_PrevZCX.Extr_LowSpd_SupervisorLogic_Trig_ZCE[6] == POS_ZCSIG)
        && Extraction_PrevZCX.Extr_LowSpd_SupervisorLogic_Trig_ZCE[6] !=
        UNINITIALIZED_ZCSIG;
    Extraction_B.zcEvent[7] = (sint32)
        Extraction_B.OutportBufferForExtr_Sup_EvStopAll !=
        (Extraction_PrevZCX.Extr_LowSpd_SupervisorLogic_Trig_ZCE[7] == POS_ZCSIG)
        && Extraction_PrevZCX.Extr_LowSpd_SupervisorLogic_Trig_ZCE[7] !=
        UNINITIALIZED_ZCSIG;

    /* End of Outputs for RootInportFunctionCallGenerator: '<Root>/RootFcnCall_InsertedFor_Extraction__Handler5ms_at_outport_1' */

    /* RootInportFunctionCallGenerator: '<Root>/RootFcnCall_InsertedFor_Extraction__Handler5ms_at_outport_1' incorporates:
     *  SubSystem: '<S13>/Extr_Supervisor'
     */
    /* SignalConversion: '<S193>/OutportBufferForExtr_Sup_ResetLoadMass' */
    Extraction_B.zcEvent[8] = (sint32)Extraction_B.Extr_Sup_ResetLoadMass !=
        (Extraction_PrevZCX.Extr_LowSpd_SupervisorLogic_Trig_ZCE[8] == POS_ZCSIG)
        && Extraction_PrevZCX.Extr_LowSpd_SupervisorLogic_Trig_ZCE[8] !=
        UNINITIALIZED_ZCSIG;
    Extraction_B.zcEvent[9] = (sint32)
        Extraction_B.VariantMergeForOutportExtr_Accel_EvCabHit !=
        (Extraction_PrevZCX.Extr_LowSpd_SupervisorLogic_Trig_ZCE[9] == POS_ZCSIG)
        && Extraction_PrevZCX.Extr_LowSpd_SupervisorLogic_Trig_ZCE[9] !=
        UNINITIALIZED_ZCSIG;
    Extraction_B.zcEvent[10] = (sint32)
        Extraction_B.OutportBufferForExtr_Sup_EvFoamDetected !=
        (Extraction_PrevZCX.Extr_LowSpd_SupervisorLogic_Trig_ZCE[10] ==
         POS_ZCSIG) && Extraction_PrevZCX.Extr_LowSpd_SupervisorLogic_Trig_ZCE
        [10] != UNINITIALIZED_ZCSIG;
    Extraction_B.zcEvent[11] = (sint32)
        Extraction_B.VariantMergeForOutportExtr_HighSpd_EvDropToDistSpeed !=
        (Extraction_PrevZCX.Extr_LowSpd_SupervisorLogic_Trig_ZCE[11] ==
         POS_ZCSIG) && Extraction_PrevZCX.Extr_LowSpd_SupervisorLogic_Trig_ZCE
        [11] != UNINITIALIZED_ZCSIG;
    Extraction_B.zcEvent[12] = (sint32)
        Extraction_B.OutportBufferForExtr_Rebal_EvShortRedistExceeded !=
        (Extraction_PrevZCX.Extr_LowSpd_SupervisorLogic_Trig_ZCE[12] ==
         POS_ZCSIG) && Extraction_PrevZCX.Extr_LowSpd_SupervisorLogic_Trig_ZCE
        [12] != UNINITIALIZED_ZCSIG;
    Extraction_B.zcEvent[13] = (sint32)
        Extraction_B.OutportBufferForExtr_LowSpd_EvUnbChkComplete !=
        (Extraction_PrevZCX.Extr_LowSpd_SupervisorLogic_Trig_ZCE[13] ==
         POS_ZCSIG) && Extraction_PrevZCX.Extr_LowSpd_SupervisorLogic_Trig_ZCE
        [13] != UNINITIALIZED_ZCSIG;
    Extraction_B.zcEvent[14] = (sint32)
        Extraction_B.OutportBufferForExtr_LowSpd_EvUnbChkRebalStart !=
        (Extraction_PrevZCX.Extr_LowSpd_SupervisorLogic_Trig_ZCE[14] ==
         POS_ZCSIG) && Extraction_PrevZCX.Extr_LowSpd_SupervisorLogic_Trig_ZCE
        [14] != UNINITIALIZED_ZCSIG;
    Extraction_B.zcEvent[15] = (sint32)
        Extraction_B.OutportBufferForExtr_Sup_ResetRebalCnt !=
        (Extraction_PrevZCX.Extr_LowSpd_SupervisorLogic_Trig_ZCE[15] ==
         POS_ZCSIG) && Extraction_PrevZCX.Extr_LowSpd_SupervisorLogic_Trig_ZCE
        [15] != UNINITIALIZED_ZCSIG;
    Extraction_B.zcEvent[16] = (sint32)
        Extraction_B.OutportBufferForExtr_Sup_ClearFault !=
        (Extraction_PrevZCX.Extr_LowSpd_SupervisorLogic_Trig_ZCE[16] ==
         POS_ZCSIG) && Extraction_PrevZCX.Extr_LowSpd_SupervisorLogic_Trig_ZCE
        [16] != UNINITIALIZED_ZCSIG;
    Extraction_B.zcEvent[17] = (sint32)
        Extraction_B.OutportBufferForExtr_Sup_EvRebalStart !=
        (Extraction_PrevZCX.Extr_LowSpd_SupervisorLogic_Trig_ZCE[17] ==
         POS_ZCSIG) && Extraction_PrevZCX.Extr_LowSpd_SupervisorLogic_Trig_ZCE
        [17] != UNINITIALIZED_ZCSIG;
    Extraction_B.zcEvent[18] = (sint32)Extraction_B.Extr_Sup_EvSpeedErrorTimeout
        != (Extraction_PrevZCX.Extr_LowSpd_SupervisorLogic_Trig_ZCE[18] ==
            POS_ZCSIG) &&
        Extraction_PrevZCX.Extr_LowSpd_SupervisorLogic_Trig_ZCE[18] !=
        UNINITIALIZED_ZCSIG;
    Extraction_B.zcEvent[19] = (sint32)Extraction_B.Extr_Sup_EvSpeedReached !=
        (Extraction_PrevZCX.Extr_LowSpd_SupervisorLogic_Trig_ZCE[19] ==
         POS_ZCSIG) && Extraction_PrevZCX.Extr_LowSpd_SupervisorLogic_Trig_ZCE
        [19] != UNINITIALIZED_ZCSIG;
    Extraction_B.zcEvent_idx_0 = false;
    for (Extraction_B.i = 0; Extraction_B.i < 20; Extraction_B.i++)
    {
        Extraction_B.zcEvent_idx_0 = Extraction_B.zcEvent_idx_0 ||
            Extraction_B.zcEvent[Extraction_B.i];
    }

    if (Extraction_B.zcEvent_idx_0)
    {
        /* Outputs for Function Call SubSystem: '<S13>/Extr_Supervisor' */
        /* SignalConversion: '<S193>/OutportBufferForExtr_Sup_EvLowSpdStart' */
        Extraction_B.rtb_inputevents_mn_idx_1 = (sint8)(Extraction_B.zcEvent[1] ?
            Extraction_B.Extr_Sup_EvLowSpdStart ? RISING_ZCEVENT :
            FALLING_ZCEVENT : NO_ZCEVENT);

        /* End of Outputs for SubSystem: '<S13>/Extr_Supervisor' */
        Extraction_B.rtb_inputevents_mn_idx_2 = (sint8)(Extraction_B.zcEvent[2] ?
            Extraction_B.OutportBufferForExtr_Rebal_EvComplete ? RISING_ZCEVENT :
            FALLING_ZCEVENT : NO_ZCEVENT);
        Extraction_B.rtb_inputevents_mn_idx_3 = (sint8)(Extraction_B.zcEvent[3] ?
            Extraction_B.OutportBufferForExtr_LowSpd_EvLoadMeasComplete ?
            RISING_ZCEVENT : FALLING_ZCEVENT : NO_ZCEVENT);
        Extraction_B.rtb_inputevents_mn_idx_4 = (sint8)(Extraction_B.zcEvent[4] ?
            Extraction_B.VariantMergeForOutportExt_LowSpd_EvOptRampComplete ?
            RISING_ZCEVENT : FALLING_ZCEVENT : NO_ZCEVENT);
        Extraction_B.rtb_inputevents_mn_idx_5 = (sint8)(Extraction_B.zcEvent[5] ?
            Extraction_B.VariantMergeForOutportExt_LowSpd_EvRedist ?
            RISING_ZCEVENT : FALLING_ZCEVENT : NO_ZCEVENT);
        Extraction_B.rtb_inputevents_mn_idx_6 = (sint8)(Extraction_B.zcEvent[6] ?
            Extraction_B.OutportBufferForExtr_LowSpd_EvMddpComplete ?
            RISING_ZCEVENT : FALLING_ZCEVENT : NO_ZCEVENT);
        Extraction_B.rtb_inputevents_mn_idx_7 = (sint8)(Extraction_B.zcEvent[7] ?
            Extraction_B.OutportBufferForExtr_Sup_EvStopAll ? RISING_ZCEVENT :
            FALLING_ZCEVENT : NO_ZCEVENT);

        /* Outputs for Function Call SubSystem: '<S13>/Extr_Supervisor' */
        /* SignalConversion: '<S193>/OutportBufferForExtr_Sup_ResetLoadMass' */
        Extraction_B.rtb_inputevents_mn_idx_8 = (sint8)(Extraction_B.zcEvent[8] ?
            Extraction_B.Extr_Sup_ResetLoadMass ? RISING_ZCEVENT :
            FALLING_ZCEVENT : NO_ZCEVENT);

        /* End of Outputs for SubSystem: '<S13>/Extr_Supervisor' */
        Extraction_B.rtb_inputevents_mn_idx_9 = (sint8)(Extraction_B.zcEvent[9] ?
            Extraction_B.VariantMergeForOutportExtr_Accel_EvCabHit ?
            RISING_ZCEVENT : FALLING_ZCEVENT : NO_ZCEVENT);
        Extraction_B.rtb_inputevents_mn_idx_10 = (sint8)(Extraction_B.zcEvent[10]
            ? Extraction_B.OutportBufferForExtr_Sup_EvFoamDetected ?
            RISING_ZCEVENT : FALLING_ZCEVENT : NO_ZCEVENT);
        Extraction_B.rtb_inputevents_mn_idx_11 = (sint8)(Extraction_B.zcEvent[11]
            ? Extraction_B.VariantMergeForOutportExtr_HighSpd_EvDropToDistSpeed ?
            RISING_ZCEVENT : FALLING_ZCEVENT : NO_ZCEVENT);
        Extraction_B.rtb_inputevents_mn_idx_12 = (sint8)(Extraction_B.zcEvent[12]
            ? Extraction_B.OutportBufferForExtr_Rebal_EvShortRedistExceeded ?
            RISING_ZCEVENT : FALLING_ZCEVENT : NO_ZCEVENT);
        Extraction_B.rtb_inputevents_mn_idx_13 = (sint8)(Extraction_B.zcEvent[13]
            ? Extraction_B.OutportBufferForExtr_LowSpd_EvUnbChkComplete ?
            RISING_ZCEVENT : FALLING_ZCEVENT : NO_ZCEVENT);
        Extraction_B.rtb_inputevents_mn_idx_14 = (sint8)(Extraction_B.zcEvent[14]
            ? Extraction_B.OutportBufferForExtr_LowSpd_EvUnbChkRebalStart ?
            RISING_ZCEVENT : FALLING_ZCEVENT : NO_ZCEVENT);
        Extraction_B.rtb_inputevents_mn_idx_15 = (sint8)(Extraction_B.zcEvent[15]
            ? Extraction_B.OutportBufferForExtr_Sup_ResetRebalCnt ?
            RISING_ZCEVENT : FALLING_ZCEVENT : NO_ZCEVENT);
        Extraction_B.rtb_inputevents_mn_idx_16 = (sint8)(Extraction_B.zcEvent[16]
            ? Extraction_B.OutportBufferForExtr_Sup_ClearFault ? RISING_ZCEVENT :
            FALLING_ZCEVENT : NO_ZCEVENT);
        Extraction_B.rtb_inputevents_mn_idx_17 = (sint8)(Extraction_B.zcEvent[17]
            ? Extraction_B.OutportBufferForExtr_Sup_EvRebalStart ?
            RISING_ZCEVENT : FALLING_ZCEVENT : NO_ZCEVENT);
        Extraction_B.rtb_inputevents_mn_idx_18 = (sint8)(Extraction_B.zcEvent[18]
            ? Extraction_B.Extr_Sup_EvSpeedErrorTimeout ? RISING_ZCEVENT :
            FALLING_ZCEVENT : NO_ZCEVENT);
        Extraction_B.rtb_inputevents_mn_idx_19 = (sint8)(Extraction_B.zcEvent[19]
            ? Extraction_B.Extr_Sup_EvSpeedReached ? RISING_ZCEVENT :
            FALLING_ZCEVENT : NO_ZCEVENT);

        /* Gateway: Extraction/Extr_LowSpd/Extr_LowSpd_Supervisor/Extr_LowSpd_SupervisorLogic */
        Extraction_B.i = 0;
        if ((sint8)(Extraction_B.zcEvent[0] ?
                    Extraction_B.sf_TriggerGen_ih.trigger ? RISING_ZCEVENT :
                    FALLING_ZCEVENT : NO_ZCEVENT) != 0)
        {
            Extraction_B.i = 1;
            if (Extraction_DW.temporalCounter_i1_jq < MAX_uint32_T)
            {
                Extraction_DW.temporalCounter_i1_jq++;
            }

            /* Event: '<S171>:247' */
            Extraction_DW.sfEvent_f2 = Extraction_event_Trigger_o1;
            Extraction_chartstep_c15_Extraction();
        }

        if (Extraction_B.rtb_inputevents_mn_idx_1 != 0)
        {
            Extraction_B.i = 1;

            /* Event: '<S171>:114' */
            Extraction_DW.sfEvent_f2 = Extraction_event_Extr_Sup_EvLowSpdStart;
            Extraction_chartstep_c15_Extraction();
        }

        if (Extraction_B.rtb_inputevents_mn_idx_2 != 0)
        {
            Extraction_B.i = 1;

            /* Event: '<S171>:129' */
            Extraction_DW.sfEvent_f2 = Extraction_event_Extr_Rebal_EvComplete_j3;
            Extraction_chartstep_c15_Extraction();
        }

        if (Extraction_B.rtb_inputevents_mn_idx_3 != 0)
        {
            Extraction_B.i = 1;

            /* Event: '<S171>:117' */
            Extraction_DW.sfEvent_f2 =
                Extraction_event_Extr_LowSpd_EvLoadMeasComplete;
            Extraction_chartstep_c15_Extraction();
        }

        if (Extraction_B.rtb_inputevents_mn_idx_4 != 0)
        {
            Extraction_B.i = 1;

            /* Event: '<S171>:115' */
            Extraction_DW.sfEvent_f2 =
                Extraction_event_Ext_LowSpd_EvOptRampComplete;
            Extraction_chartstep_c15_Extraction();
        }

        if (Extraction_B.rtb_inputevents_mn_idx_5 != 0)
        {
            Extraction_B.i = 1;

            /* Event: '<S171>:134' */
            Extraction_DW.sfEvent_f2 =
                Extraction_event_Extr_LowSpd_EvOptRampRedist;
            Extraction_chartstep_c15_Extraction();
        }

        if (Extraction_B.rtb_inputevents_mn_idx_6 != 0)
        {
            Extraction_B.i = 1;

            /* Event: '<S171>:118' */
            Extraction_DW.sfEvent_f2 =
                Extraction_event_Extr_LowSpd_EvMddpComplete;
            Extraction_chartstep_c15_Extraction();
        }

        if (Extraction_B.rtb_inputevents_mn_idx_7 != 0)
        {
            Extraction_B.i = 1;

            /* Event: '<S171>:54' */
            Extraction_DW.sfEvent_f2 = Extraction_event_Extr_Sup_EvStopAll_ne;
            Extraction_chartstep_c15_Extraction();
        }

        if (Extraction_B.rtb_inputevents_mn_idx_8 != 0)
        {
            Extraction_B.i = 1;

            /* Event: '<S171>:158' */
            Extraction_DW.sfEvent_f2 = Extraction_event_Extr_Sup_EvResetLoadMass;
            Extraction_chartstep_c15_Extraction();
        }

        if (Extraction_B.rtb_inputevents_mn_idx_9 != 0)
        {
            Extraction_B.i = 1;

            /* Event: '<S171>:250' */
            Extraction_DW.sfEvent_f2 = Extraction_event_Extr_Accel_EvCabHit_kw;
            Extraction_chartstep_c15_Extraction();
        }

        if (Extraction_B.rtb_inputevents_mn_idx_10 != 0)
        {
            Extraction_B.i = 1;

            /* Event: '<S171>:299' */
            Extraction_DW.sfEvent_f2 =
                Extraction_event_Extr_Sup_EvFoamDetected_l0;
            Extraction_chartstep_c15_Extraction();
        }

        if (Extraction_B.rtb_inputevents_mn_idx_11 != 0)
        {
            Extraction_B.i = 1;

            /* Event: '<S171>:328' */
            Extraction_DW.sfEvent_f2 =
                Extraction_event_Extr_HighSpd_EvDropToDistSpeed;
            Extraction_chartstep_c15_Extraction();
        }

        if (Extraction_B.rtb_inputevents_mn_idx_12 != 0)
        {
            Extraction_B.i = 1;

            /* Event: '<S171>:339' */
            Extraction_DW.sfEvent_f2 =
                Extraction_event_Extr_Rebal_EvShortRedistExceeded_bq;
            Extraction_chartstep_c15_Extraction();
        }

        if (Extraction_B.rtb_inputevents_mn_idx_13 != 0)
        {
            Extraction_B.i = 1;

            /* Event: '<S171>:344' */
            Extraction_DW.sfEvent_f2 =
                Extraction_event_Extr_LowSpd_EvUnbChkComplete;
            Extraction_chartstep_c15_Extraction();
        }

        if (Extraction_B.rtb_inputevents_mn_idx_14 != 0)
        {
            Extraction_B.i = 1;

            /* Event: '<S171>:345' */
            Extraction_DW.sfEvent_f2 =
                Extraction_event_Extr_LowSpd_EvUnbChkRebalStart;
            Extraction_chartstep_c15_Extraction();
        }

        if (Extraction_B.rtb_inputevents_mn_idx_15 != 0)
        {
            Extraction_B.i = 1;

            /* Event: '<S171>:355' */
            Extraction_DW.sfEvent_f2 =
                Extraction_event_Extr_Sup_ResetRebalCnt_ay;
            Extraction_chartstep_c15_Extraction();
        }

        if (Extraction_B.rtb_inputevents_mn_idx_16 != 0)
        {
            Extraction_B.i = 1;

            /* Event: '<S171>:356' */
            Extraction_DW.sfEvent_f2 = Extraction_event_Extr_Sup_ClearFault_ba;
            Extraction_chartstep_c15_Extraction();
        }

        if (Extraction_B.rtb_inputevents_mn_idx_17 != 0)
        {
            Extraction_B.i = 1;

            /* Event: '<S171>:362' */
            Extraction_DW.sfEvent_f2 = Extraction_event_Extr_Sup_EvRebalStart_jz;
            Extraction_chartstep_c15_Extraction();
        }

        if (Extraction_B.rtb_inputevents_mn_idx_18 != 0)
        {
            Extraction_B.i = 1;

            /* Event: '<S171>:368' */
            Extraction_DW.sfEvent_f2 =
                Extraction_event_Extr_Sup_EvSpeedErrorTimeout_cn;
            Extraction_chartstep_c15_Extraction();
        }

        if (Extraction_B.rtb_inputevents_mn_idx_19 != 0)
        {
            Extraction_B.i = 1;

            /* Event: '<S171>:369' */
            Extraction_DW.sfEvent_f2 =
                Extraction_event_Extr_Sup_EvSpeedReached_pu;
            Extraction_chartstep_c15_Extraction();
        }

        if (Extraction_B.i != 0 &&
                Extraction_DW.Extr_LowSpd_EvDistributionStartEventCounter > 0U)
        {
            Extraction_B.Extr_LowSpd_EvDistributionStart =
                !Extraction_B.Extr_LowSpd_EvDistributionStart;
            Extraction_DW.Extr_LowSpd_EvDistributionStartEventCounter--;
        }

        if (Extraction_B.i != 0 &&
                Extraction_DW.Extr_LowSpd_EvLoadMeasStartEventCounter > 0U)
        {
            Extraction_B.Extr_LowSpd_EvLoadMeasStart =
                !Extraction_B.Extr_LowSpd_EvLoadMeasStart;
            Extraction_DW.Extr_LowSpd_EvLoadMeasStartEventCounter--;
        }

        if (Extraction_B.i != 0 &&
                Extraction_DW.Extr_LowSpd_EvOptRampStartEventCounter > 0U)
        {
            Extraction_B.Extr_LowSpd_EvOptRampStart =
                !Extraction_B.Extr_LowSpd_EvOptRampStart;
            Extraction_DW.Extr_LowSpd_EvOptRampStartEventCounter--;
        }

        if (Extraction_B.i != 0 &&
                Extraction_DW.Extr_LowSpd_EvMddpStartEventCounter > 0U)
        {
            Extraction_B.Extr_LowSpd_EvMddpStart =
                !Extraction_B.Extr_LowSpd_EvMddpStart;
            Extraction_DW.Extr_LowSpd_EvMddpStartEventCounter--;
        }

        if (Extraction_B.i != 0 &&
                Extraction_DW.Extr_LowSpd_EvResetLoadMassEventCounter > 0U)
        {
            Extraction_B.Extr_LowSpd_EvResetLoadMass =
                !Extraction_B.Extr_LowSpd_EvResetLoadMass;
            Extraction_DW.Extr_LowSpd_EvResetLoadMassEventCounter--;
        }

        if (Extraction_B.i != 0 &&
                Extraction_DW.Extr_LowSpd_EvRebalStartEventCounter > 0U)
        {
            Extraction_B.Extr_LowSpd_EvRebalStart =
                !Extraction_B.Extr_LowSpd_EvRebalStart;
            Extraction_DW.Extr_LowSpd_EvRebalStartEventCounter--;
        }

        if (Extraction_B.i != 0 &&
                Extraction_DW.Extr_LowSpd_EvCompleteEventCounter > 0U)
        {
            Extraction_B.Extr_LowSpd_EvComplete =
                !Extraction_B.Extr_LowSpd_EvComplete;
            Extraction_DW.Extr_LowSpd_EvCompleteEventCounter--;
        }

        if (Extraction_B.i != 0 && Extraction_DW.Extr_LowSpd_EvStopEventCounter >
            0U)
        {
            Extraction_B.Extr_LowSpd_EvStop = !Extraction_B.Extr_LowSpd_EvStop;
            Extraction_DW.Extr_LowSpd_EvStopEventCounter--;
        }

        if (Extraction_B.i != 0 &&
                Extraction_DW.Extr_LowSpd_EvStartBasicUnbChkEventCounter > 0U)
        {
            Extraction_B.Extr_LowSpd_EvStartBasicUnbChk =
                !Extraction_B.Extr_LowSpd_EvStartBasicUnbChk;
            Extraction_DW.Extr_LowSpd_EvStartBasicUnbChkEventCounter--;
        }

        if (Extraction_B.i != 0 &&
                Extraction_DW.Extr_LowSpd_EvResetShortRedistEventCounter > 0U)
        {
            Extraction_B.Extr_LowSpd_EvResetShortRedist =
                !Extraction_B.Extr_LowSpd_EvResetShortRedist;
            Extraction_DW.Extr_LowSpd_EvResetShortRedistEventCounter--;
        }
    }

    Extraction_PrevZCX.Extr_LowSpd_SupervisorLogic_Trig_ZCE[0] =
        Extraction_B.sf_TriggerGen_ih.trigger;
    Extraction_PrevZCX.Extr_LowSpd_SupervisorLogic_Trig_ZCE[1] =
        Extraction_B.zcEvent_idx_1;
    Extraction_PrevZCX.Extr_LowSpd_SupervisorLogic_Trig_ZCE[2] =
        Extraction_B.OutportBufferForExtr_Rebal_EvComplete;
    Extraction_PrevZCX.Extr_LowSpd_SupervisorLogic_Trig_ZCE[3] =
        Extraction_B.OutportBufferForExtr_LowSpd_EvLoadMeasComplete;
    Extraction_PrevZCX.Extr_LowSpd_SupervisorLogic_Trig_ZCE[4] =
        Extraction_B.VariantMergeForOutportExt_LowSpd_EvOptRampComplete;
    Extraction_PrevZCX.Extr_LowSpd_SupervisorLogic_Trig_ZCE[5] =
        Extraction_B.VariantMergeForOutportExt_LowSpd_EvRedist;
    Extraction_PrevZCX.Extr_LowSpd_SupervisorLogic_Trig_ZCE[6] =
        Extraction_B.OutportBufferForExtr_LowSpd_EvMddpComplete;
    Extraction_PrevZCX.Extr_LowSpd_SupervisorLogic_Trig_ZCE[7] =
        Extraction_B.OutportBufferForExtr_Sup_EvStopAll;
    Extraction_PrevZCX.Extr_LowSpd_SupervisorLogic_Trig_ZCE[8] =
        Extraction_B.OutportBufferForExtr_Sup_ResetLoadMass;
    Extraction_PrevZCX.Extr_LowSpd_SupervisorLogic_Trig_ZCE[9] =
        Extraction_B.VariantMergeForOutportExtr_Accel_EvCabHit;
    Extraction_PrevZCX.Extr_LowSpd_SupervisorLogic_Trig_ZCE[10] =
        Extraction_B.OutportBufferForExtr_Sup_EvFoamDetected;
    Extraction_PrevZCX.Extr_LowSpd_SupervisorLogic_Trig_ZCE[11] =
        Extraction_B.VariantMergeForOutportExtr_HighSpd_EvDropToDistSpeed;
    Extraction_PrevZCX.Extr_LowSpd_SupervisorLogic_Trig_ZCE[12] =
        Extraction_B.OutportBufferForExtr_Rebal_EvShortRedistExceeded;
    Extraction_PrevZCX.Extr_LowSpd_SupervisorLogic_Trig_ZCE[13] =
        Extraction_B.OutportBufferForExtr_LowSpd_EvUnbChkComplete;
    Extraction_PrevZCX.Extr_LowSpd_SupervisorLogic_Trig_ZCE[14] =
        Extraction_B.OutportBufferForExtr_LowSpd_EvUnbChkRebalStart;
    Extraction_PrevZCX.Extr_LowSpd_SupervisorLogic_Trig_ZCE[15] =
        Extraction_B.OutportBufferForExtr_Sup_ResetRebalCnt;
    Extraction_PrevZCX.Extr_LowSpd_SupervisorLogic_Trig_ZCE[16] =
        Extraction_B.OutportBufferForExtr_Sup_ClearFault;
    Extraction_PrevZCX.Extr_LowSpd_SupervisorLogic_Trig_ZCE[17] =
        Extraction_B.OutportBufferForExtr_Sup_EvRebalStart;
    Extraction_PrevZCX.Extr_LowSpd_SupervisorLogic_Trig_ZCE[18] =
        Extraction_B.Extr_Sup_EvSpeedErrorTimeout;
    Extraction_PrevZCX.Extr_LowSpd_SupervisorLogic_Trig_ZCE[19] =
        Extraction_B.Extr_Sup_EvSpeedReached;

    /* DataStoreWrite: '<S122>/Data Store Write' */
    Extr_LowSpd_RebalCnt = Extraction_B.Extr_LowSpd_RebalCnt_e4;

    /* SignalConversion: '<S122>/DistSpeed' */
    Extr_LowSpd_DistSpeed = Extraction_B.Extr_LowSpd_DistSpeed_lt;

    /* SignalConversion: '<S122>/OutportBufferForExtr_LowSpd_EvComplete' */
    Extraction_B.OutportBufferForExtr_LowSpd_EvComplete =
        Extraction_B.Extr_LowSpd_EvComplete;

    /* SignalConversion: '<S122>/OutportBufferForExtr_LowSpd_EvDistributionStart' */
    Extraction_B.OutportBufferForExtr_Sup_ResetLoadMass =
        Extraction_B.Extr_LowSpd_EvDistributionStart;

    /* SignalConversion: '<S122>/OutportBufferForExtr_LowSpd_EvMddpStart' */
    Extraction_B.OutportBufferForExtr_LowSpd_EvMddpStart =
        Extraction_B.Extr_LowSpd_EvMddpStart;

    /* SignalConversion: '<S122>/OutportBufferForExtr_LowSpd_EvOptRampStart' */
    Extraction_B.OutportBufferForExtr_LowSpd_EvOptRampStart =
        Extraction_B.Extr_LowSpd_EvOptRampStart;

    /* SignalConversion: '<S122>/OutportBufferForExtr_LowSpd_EvRebalStart' */
    Extraction_B.OutportBufferForExtr_LowSpd_EvRebalStart =
        Extraction_B.Extr_LowSpd_EvRebalStart;

    /* SignalConversion: '<S122>/OutportBufferForExtr_LowSpd_EvResetShortRedist' */
    Extraction_B.OutportBufferForExtr_LowSpd_EvResetShortRedist =
        Extraction_B.Extr_LowSpd_EvResetShortRedist;

    /* SignalConversion: '<S122>/OutportBufferForExtr_LowSpd_EvStartBasicUnbChk' */
    Extraction_B.OutportBufferForExtr_LowSpd_EvStartBasicUnbChk =
        Extraction_B.Extr_LowSpd_EvStartBasicUnbChk;

    /* SignalConversion: '<S122>/OutportBufferForExtr_LowSpd_EvStop' */
    Extraction_B.OutportBufferForExtr_LowSpd_EvStop =
        Extraction_B.Extr_LowSpd_EvStop;

    /* SignalConversion: '<S122>/OutportBufferForExtr_LowSpd_MddpExecuted' */
    Extraction_B.Extr_LowSpd_MddpExecuted_p5 =
        Extraction_B.Extr_LowSpd_MddpExecuted_mb;

    /* SignalConversion: '<S122>/OutportBufferForExtr_LowSpd_PauseSpeedMonitor' */
    Extraction_B.OutportBufferForExtr_LowSpd_PauseSpeedMonitor =
        Extraction_B.Extr_LowSpd_PauseSpeedMonitor;

    /* SignalConversion: '<S122>/OutportBufferForExtr_LowSpd_RebalCnt' */
    Extraction_B.Extr_LowSpd_RebalCnt_lp = Extraction_B.Extr_LowSpd_RebalCnt_e4;

    /* RootInportFunctionCallGenerator: '<Root>/RootFcnCall_InsertedFor_Extraction__Handler5ms_at_outport_1' incorporates:
     *  SubSystem: '<S11>/Extr_LowSpd_Algorithms'
     */
    /* Chart: '<S130>/TriggerGen' */
    Extraction_TriggerGen(&Extraction_B.sf_TriggerGen_kb,
                          &Extraction_DW.sf_TriggerGen_kb);

    /* Chart: '<S125>/Extr_LowSpd_LoadMeasLogic' incorporates:
     *  TriggerPort: '<S145>/input events'
     */
    Extraction_B.zcEvent_idx_0 = (sint32)Extraction_B.sf_TriggerGen_kb.trigger
        != (Extraction_PrevZCX.Extr_LowSpd_LoadMeasLogic_Trig_ZCE[0] ==
            POS_ZCSIG) && Extraction_PrevZCX.Extr_LowSpd_LoadMeasLogic_Trig_ZCE
        [0] != UNINITIALIZED_ZCSIG;

    /* End of Outputs for RootInportFunctionCallGenerator: '<Root>/RootFcnCall_InsertedFor_Extraction__Handler5ms_at_outport_1' */

    /* RootInportFunctionCallGenerator: '<Root>/RootFcnCall_InsertedFor_Extraction__Handler5ms_at_outport_1' incorporates:
     *  SubSystem: '<S11>/Extr_LowSpd_Supervisor'
     */
    /* SignalConversion: '<S122>/OutportBufferForExtr_LowSpd_EvLoadMeasStart' */
    Extraction_B.zcEvent_idx_1 = (sint32)
        Extraction_B.Extr_LowSpd_EvLoadMeasStart !=
        (Extraction_PrevZCX.Extr_LowSpd_LoadMeasLogic_Trig_ZCE[1] == POS_ZCSIG) &&
        Extraction_PrevZCX.Extr_LowSpd_LoadMeasLogic_Trig_ZCE[1] !=
        UNINITIALIZED_ZCSIG;
    Extraction_B.zcEvent_idx_2 = (sint32)
        Extraction_B.OutportBufferForExtr_LowSpd_EvStop !=
        (Extraction_PrevZCX.Extr_LowSpd_LoadMeasLogic_Trig_ZCE[2] == POS_ZCSIG) &&
        Extraction_PrevZCX.Extr_LowSpd_LoadMeasLogic_Trig_ZCE[2] !=
        UNINITIALIZED_ZCSIG;

    /* End of Outputs for RootInportFunctionCallGenerator: '<Root>/RootFcnCall_InsertedFor_Extraction__Handler5ms_at_outport_1' */

    /* RootInportFunctionCallGenerator: '<Root>/RootFcnCall_InsertedFor_Extraction__Handler5ms_at_outport_1' incorporates:
     *  SubSystem: '<S11>/Extr_LowSpd_Supervisor'
     */
    /* SignalConversion: '<S122>/OutportBufferForExtr_LowSpd_EvResetLoadMass' */
    Extraction_B.zcEvent_idx_3 = (sint32)
        Extraction_B.Extr_LowSpd_EvResetLoadMass !=
        (Extraction_PrevZCX.Extr_LowSpd_LoadMeasLogic_Trig_ZCE[3] == POS_ZCSIG) &&
        Extraction_PrevZCX.Extr_LowSpd_LoadMeasLogic_Trig_ZCE[3] !=
        UNINITIALIZED_ZCSIG;
    if (Extraction_B.zcEvent_idx_0 || Extraction_B.zcEvent_idx_1 ||
            Extraction_B.zcEvent_idx_2 || Extraction_B.zcEvent_idx_3)
    {
        /* Gateway: Extraction/Extr_LowSpd/Extr_LowSpd_Algorithms/Extr_LowSpd_LoadMeas/Extr_LowSpd_LoadMeasLogic */
        Extraction_B.i = 0;
        if ((sint8)(Extraction_B.zcEvent_idx_0 ?
                    Extraction_B.sf_TriggerGen_kb.trigger ? RISING_ZCEVENT :
                    FALLING_ZCEVENT : NO_ZCEVENT) != 0)
        {
            Extraction_B.i = 1;
            if (Extraction_DW.temporalCounter_i1_pw < MAX_uint32_T)
            {
                Extraction_DW.temporalCounter_i1_pw++;
            }

            /* Event: '<S145>:48' */
            Extraction_DW.sfEvent_ak = Extraction_event_ticks_hm;
            Extraction_chartstep_c9_Extraction();
        }

        /* Outputs for Function Call SubSystem: '<S11>/Extr_LowSpd_Supervisor' */
        /* SignalConversion: '<S122>/OutportBufferForExtr_LowSpd_EvLoadMeasStart' */
        if ((sint8)(Extraction_B.zcEvent_idx_1 ?
                    Extraction_B.Extr_LowSpd_EvLoadMeasStart ? RISING_ZCEVENT :
                    FALLING_ZCEVENT : NO_ZCEVENT) != 0)
        {
            Extraction_B.i = 1;

            /* Event: '<S145>:212' */
            Extraction_DW.sfEvent_ak =
                Extraction_event_Extr_LowSpd_EvLoadMeasStart;
            Extraction_chartstep_c9_Extraction();
        }

        /* End of Outputs for SubSystem: '<S11>/Extr_LowSpd_Supervisor' */
        if ((sint8)(Extraction_B.zcEvent_idx_2 ?
                    Extraction_B.OutportBufferForExtr_LowSpd_EvStop ?
                    RISING_ZCEVENT : FALLING_ZCEVENT : NO_ZCEVENT) != 0)
        {
            Extraction_B.i = 1;

            /* Event: '<S145>:195' */
            Extraction_DW.sfEvent_ak = Extraction_event_Extr_LowSpd_EvStop;
            Extraction_chartstep_c9_Extraction();
        }

        /* Outputs for Function Call SubSystem: '<S11>/Extr_LowSpd_Supervisor' */
        /* SignalConversion: '<S122>/OutportBufferForExtr_LowSpd_EvResetLoadMass' */
        if ((sint8)(Extraction_B.zcEvent_idx_3 ?
                    Extraction_B.Extr_LowSpd_EvResetLoadMass ? RISING_ZCEVENT :
                    FALLING_ZCEVENT : NO_ZCEVENT) != 0)
        {
            Extraction_B.i = 1;

            /* Event: '<S145>:329' */
            Extraction_DW.sfEvent_ak =
                Extraction_event_Extr_LowSpd_EvResetLoadMass;
            Extraction_chartstep_c9_Extraction();
        }

        /* End of Outputs for SubSystem: '<S11>/Extr_LowSpd_Supervisor' */
        if (Extraction_B.i != 0 &&
                Extraction_DW.Extr_LowSpd_EvLoadMeasCompleteEventCounter > 0U)
        {
            Extraction_B.Extr_LowSpd_EvLoadMeasComplete =
                !Extraction_B.Extr_LowSpd_EvLoadMeasComplete;
            Extraction_DW.Extr_LowSpd_EvLoadMeasCompleteEventCounter--;
        }

        if (Extraction_B.i != 0 &&
                Extraction_DW.Extr_LowSpd_EvCleanWasherExceptionEventCounter >
                0U)
        {
            Extraction_B.Extr_LowSpd_EvCleanWasherException =
                !Extraction_B.Extr_LowSpd_EvCleanWasherException;
            Extraction_DW.Extr_LowSpd_EvCleanWasherExceptionEventCounter--;
        }
    }

    Extraction_PrevZCX.Extr_LowSpd_LoadMeasLogic_Trig_ZCE[0] =
        Extraction_B.sf_TriggerGen_kb.trigger;

    /* End of Outputs for RootInportFunctionCallGenerator: '<Root>/RootFcnCall_InsertedFor_Extraction__Handler5ms_at_outport_1' */

    /* RootInportFunctionCallGenerator: '<Root>/RootFcnCall_InsertedFor_Extraction__Handler5ms_at_outport_1' incorporates:
     *  SubSystem: '<S11>/Extr_LowSpd_Supervisor'
     */
    /* SignalConversion: '<S122>/OutportBufferForExtr_LowSpd_EvLoadMeasStart' */
    Extraction_PrevZCX.Extr_LowSpd_LoadMeasLogic_Trig_ZCE[1] =
        Extraction_B.Extr_LowSpd_EvLoadMeasStart;
    Extraction_PrevZCX.Extr_LowSpd_LoadMeasLogic_Trig_ZCE[2] =
        Extraction_B.OutportBufferForExtr_LowSpd_EvStop;

    /* End of Outputs for RootInportFunctionCallGenerator: '<Root>/RootFcnCall_InsertedFor_Extraction__Handler5ms_at_outport_1' */

    /* RootInportFunctionCallGenerator: '<Root>/RootFcnCall_InsertedFor_Extraction__Handler5ms_at_outport_1' incorporates:
     *  SubSystem: '<S11>/Extr_LowSpd_Supervisor'
     */
    /* SignalConversion: '<S122>/OutportBufferForExtr_LowSpd_EvResetLoadMass' */
    Extraction_PrevZCX.Extr_LowSpd_LoadMeasLogic_Trig_ZCE[3] =
        Extraction_B.Extr_LowSpd_EvResetLoadMass;

    /* Switch: '<S121>/LoadMassSwitch' */
    if (Extraction_B.Extr_LowSpd_LoadMass_ec >= 0.0F)
    {
        Extraction_B.Extr_LowSpd_LoadMass_gp =
            Extraction_B.Extr_LowSpd_LoadMass_ec;
    }
    else
    {
        Extraction_B.Extr_LowSpd_LoadMass_gp = Extr_Inputs_AcuLoadMass;
    }

    /* End of Switch: '<S121>/LoadMassSwitch' */

    /* DataStoreWrite: '<S121>/Data Store Write' */
    Extr_LowSpd_LoadMass = Extraction_B.Extr_LowSpd_LoadMass_gp;

    /* DataStoreWrite: '<S125>/Data Store Write' */
    Extr_LowSpd_Unbalance = Extraction_B.Extr_LowSpd_Unbalance_oe;

    /* Chart: '<S124>/Extr_LowSpd_BasicUnbCheck' incorporates:
     *  TriggerPort: '<S141>/input events'
     */
    Extraction_B.zcEvent_idx_0 = (sint32)Extraction_B.sf_TriggerGen_kb.trigger
        != (Extraction_PrevZCX.Extr_LowSpd_BasicUnbCheck_Trig_ZCE[0] ==
            POS_ZCSIG) && Extraction_PrevZCX.Extr_LowSpd_BasicUnbCheck_Trig_ZCE
        [0] != UNINITIALIZED_ZCSIG;

    /* End of Outputs for RootInportFunctionCallGenerator: '<Root>/RootFcnCall_InsertedFor_Extraction__Handler5ms_at_outport_1' */

    /* RootInportFunctionCallGenerator: '<Root>/RootFcnCall_InsertedFor_Extraction__Handler5ms_at_outport_1' incorporates:
     *  SubSystem: '<S11>/Extr_LowSpd_Supervisor'
     */
    /* SignalConversion: '<S122>/OutportBufferForExtr_LowSpd_EvStartBasicUnbChk' */
    Extraction_B.zcEvent_idx_1 = (sint32)
        Extraction_B.Extr_LowSpd_EvStartBasicUnbChk !=
        (Extraction_PrevZCX.Extr_LowSpd_BasicUnbCheck_Trig_ZCE[1] == POS_ZCSIG) &&
        Extraction_PrevZCX.Extr_LowSpd_BasicUnbCheck_Trig_ZCE[1] !=
        UNINITIALIZED_ZCSIG;
    Extraction_B.zcEvent_idx_2 = (sint32)
        Extraction_B.OutportBufferForExtr_LowSpd_EvStop !=
        (Extraction_PrevZCX.Extr_LowSpd_BasicUnbCheck_Trig_ZCE[2] == POS_ZCSIG) &&
        Extraction_PrevZCX.Extr_LowSpd_BasicUnbCheck_Trig_ZCE[2] !=
        UNINITIALIZED_ZCSIG;
    if (Extraction_B.zcEvent_idx_0 || Extraction_B.zcEvent_idx_1 ||
            Extraction_B.zcEvent_idx_2)
    {
        /* Outputs for Function Call SubSystem: '<S11>/Extr_LowSpd_Supervisor' */
        /* SignalConversion: '<S122>/OutportBufferForExtr_LowSpd_EvStartBasicUnbChk' */
        Extraction_B.rtb_inputevents_mn_idx_1 = (sint8)
            (Extraction_B.zcEvent_idx_1 ?
             Extraction_B.Extr_LowSpd_EvStartBasicUnbChk ? RISING_ZCEVENT :
             FALLING_ZCEVENT : NO_ZCEVENT);

        /* End of Outputs for SubSystem: '<S11>/Extr_LowSpd_Supervisor' */
        Extraction_B.rtb_inputevents_mn_idx_2 = (sint8)
            (Extraction_B.zcEvent_idx_2 ?
             Extraction_B.OutportBufferForExtr_LowSpd_EvStop ? RISING_ZCEVENT :
             FALLING_ZCEVENT : NO_ZCEVENT);

        /* Gateway: Extraction/Extr_LowSpd/Extr_LowSpd_Algorithms/Extr_LowSpd_BasicUnbChk/Extr_LowSpd_BasicUnbCheck */
        Extraction_B.i = 0;
        if ((sint8)(Extraction_B.zcEvent_idx_0 ?
                    Extraction_B.sf_TriggerGen_kb.trigger ? RISING_ZCEVENT :
                    FALLING_ZCEVENT : NO_ZCEVENT) != 0)
        {
            Extraction_B.i = 1;

            /* Event: '<S141>:48' */
            Extraction_DW.sfEvent_k0 = Extraction_event_Trigger_db;
            Extraction_chartstep_c42_Extraction();
        }

        if (Extraction_B.rtb_inputevents_mn_idx_1 != 0)
        {
            Extraction_B.i = 1;

            /* Event: '<S141>:5' */
            Extraction_DW.sfEvent_k0 =
                Extraction_event_Extr_LowSpd_StartBasicUnbChkEv;
            Extraction_chartstep_c42_Extraction();
        }

        if (Extraction_B.rtb_inputevents_mn_idx_2 != 0)
        {
            Extraction_B.i = 1;

            /* Event: '<S141>:49' */
            Extraction_DW.sfEvent_k0 =
                Extraction_event_Extr_HighSpd_EvStopAll_ks;
            Extraction_chartstep_c42_Extraction();
        }

        if (Extraction_B.i != 0 &&
                Extraction_DW.Extr_LowSpd_EvUnbChkRebalStartEventCounter > 0U)
        {
            Extraction_B.Extr_LowSpd_EvUnbChkRebalStart =
                !Extraction_B.Extr_LowSpd_EvUnbChkRebalStart;
            Extraction_DW.Extr_LowSpd_EvUnbChkRebalStartEventCounter--;
        }

        if (Extraction_B.i != 0 &&
                Extraction_DW.Extr_LowSpd_EvUnbChkCompleteEventCounter > 0U)
        {
            Extraction_B.Extr_LowSpd_EvUnbChkComplete =
                !Extraction_B.Extr_LowSpd_EvUnbChkComplete;
            Extraction_DW.Extr_LowSpd_EvUnbChkCompleteEventCounter--;
        }
    }

    Extraction_PrevZCX.Extr_LowSpd_BasicUnbCheck_Trig_ZCE[0] =
        Extraction_B.sf_TriggerGen_kb.trigger;
    Extraction_PrevZCX.Extr_LowSpd_BasicUnbCheck_Trig_ZCE[1] =
        Extraction_B.OutportBufferForExtr_LowSpd_EvStartBasicUnbChk;
    Extraction_PrevZCX.Extr_LowSpd_BasicUnbCheck_Trig_ZCE[2] =
        Extraction_B.OutportBufferForExtr_LowSpd_EvStop;

    /* Chart: '<S126>/MDDPLogic' incorporates:
     *  TriggerPort: '<S147>/input events'
     */
    Extraction_B.zcEvent_idx_0 = (sint32)Extraction_B.sf_TriggerGen_kb.trigger
        != (Extraction_PrevZCX.MDDPLogic_Trig_ZCE[0] == POS_ZCSIG) &&
        Extraction_PrevZCX.MDDPLogic_Trig_ZCE[0] != UNINITIALIZED_ZCSIG;
    Extraction_B.zcEvent_idx_1 = (sint32)
        Extraction_B.OutportBufferForExtr_LowSpd_EvMddpStart !=
        (Extraction_PrevZCX.MDDPLogic_Trig_ZCE[1] == POS_ZCSIG) &&
        Extraction_PrevZCX.MDDPLogic_Trig_ZCE[1] != UNINITIALIZED_ZCSIG;
    Extraction_B.zcEvent_idx_2 = (sint32)
        Extraction_B.OutportBufferForExtr_LowSpd_EvStop !=
        (Extraction_PrevZCX.MDDPLogic_Trig_ZCE[2] == POS_ZCSIG) &&
        Extraction_PrevZCX.MDDPLogic_Trig_ZCE[2] != UNINITIALIZED_ZCSIG;
    Extraction_B.zcEvent_idx_3 = (sint32)Extraction_B.Extr_Sup_EvSpeedReached !=
        (Extraction_PrevZCX.MDDPLogic_Trig_ZCE[3] == POS_ZCSIG) &&
        Extraction_PrevZCX.MDDPLogic_Trig_ZCE[3] != UNINITIALIZED_ZCSIG;
    if (Extraction_B.zcEvent_idx_0 || Extraction_B.zcEvent_idx_1 ||
            Extraction_B.zcEvent_idx_2 || Extraction_B.zcEvent_idx_3)
    {
        /* Gateway: Extraction/Extr_LowSpd/Extr_LowSpd_Algorithms/Extr_LowSpd_MDDP/MDDPLogic */
        Extraction_B.i = 0;
        if ((sint8)(Extraction_B.zcEvent_idx_0 ?
                    Extraction_B.sf_TriggerGen_kb.trigger ? RISING_ZCEVENT :
                    FALLING_ZCEVENT : NO_ZCEVENT) != 0)
        {
            Extraction_B.i = 1;
            if (Extraction_DW.temporalCounter_i1_f4 < MAX_uint32_T)
            {
                Extraction_DW.temporalCounter_i1_f4++;
            }

            /* Event: '<S147>:48' */
            Extraction_DW.sfEvent_o4 = Extraction_event_ticks_hm;
            Extraction_chartstep_c11_Extraction();
        }

        if ((sint8)(Extraction_B.zcEvent_idx_1 ?
                    Extraction_B.OutportBufferForExtr_LowSpd_EvMddpStart ?
                    RISING_ZCEVENT : FALLING_ZCEVENT : NO_ZCEVENT) != 0)
        {
            Extraction_B.i = 1;

            /* Event: '<S147>:242' */
            Extraction_DW.sfEvent_o4 = Extraction_event_evStart_eo;
            Extraction_chartstep_c11_Extraction();
        }

        if ((sint8)(Extraction_B.zcEvent_idx_2 ?
                    Extraction_B.OutportBufferForExtr_LowSpd_EvStop ?
                    RISING_ZCEVENT : FALLING_ZCEVENT : NO_ZCEVENT) != 0)
        {
            Extraction_B.i = 1;

            /* Event: '<S147>:195' */
            Extraction_DW.sfEvent_o4 = Extraction_event_evStop_m2;
            Extraction_chartstep_c11_Extraction();
        }

        if ((sint8)(Extraction_B.zcEvent_idx_3 ?
                    Extraction_B.Extr_Sup_EvSpeedReached ? RISING_ZCEVENT :
                    FALLING_ZCEVENT : NO_ZCEVENT) != 0)
        {
            Extraction_B.i = 1;

            /* Event: '<S147>:306' */
            Extraction_DW.sfEvent_o4 =
                Extraction_event_Extr_Sup_EvSpeedReached_fi;
            Extraction_chartstep_c11_Extraction();
        }

        if (Extraction_B.i != 0 &&
                Extraction_DW.Extr_LowSpd_EvMddpCompleteEventCounter > 0U)
        {
            Extraction_B.Extr_LowSpd_EvMddpComplete =
                !Extraction_B.Extr_LowSpd_EvMddpComplete;
            Extraction_DW.Extr_LowSpd_EvMddpCompleteEventCounter--;
        }
    }

    Extraction_PrevZCX.MDDPLogic_Trig_ZCE[0] =
        Extraction_B.sf_TriggerGen_kb.trigger;
    Extraction_PrevZCX.MDDPLogic_Trig_ZCE[1] =
        Extraction_B.OutportBufferForExtr_LowSpd_EvMddpStart;
    Extraction_PrevZCX.MDDPLogic_Trig_ZCE[2] =
        Extraction_B.OutportBufferForExtr_LowSpd_EvStop;
    Extraction_PrevZCX.MDDPLogic_Trig_ZCE[3] =
        Extraction_B.Extr_Sup_EvSpeedReached;

    /* SignalConversion: '<S126>/MddpDuration' */
    Extr_LowSpd_MddpDur = Extraction_B.Extr_LowSpd_MddpDur_fy;

    /* MinMax: '<S121>/MaxTargetSpeed' */
    if (Extraction_B.x10 < Extr_Inputs_MaxSpeed)
    {
        Extraction_B.MaxTargetSpeed = Extraction_B.x10;
    }
    else
    {
        Extraction_B.MaxTargetSpeed = Extr_Inputs_MaxSpeed;
    }

    /* End of MinMax: '<S121>/MaxTargetSpeed' */

    /* Outputs for Atomic SubSystem: '<S121>/Extr_LowSpd_OptRamp' */
#if EXTR_VARIANT_OPT_RAMP_ENABLED

    /* Output and update for atomic system: '<S127>/Extr_LowSpd_OptRamp_Enabled' */
    {
        sint32 inputEventFiredFlag_h;
        boolean_T zcEvent_idx_0_h;
        boolean_T zcEvent_idx_1_h;
        boolean_T zcEvent_idx_2_h;
        sint8 rtb_inputevents_dv_idx_1_h;
        sint8 rtb_inputevents_dv_idx_2_h;

        /* Chart: '<S150>/OptRampLogic' incorporates:
         *  TriggerPort: '<S151>/input events'
         */
        zcEvent_idx_0_h = (sint32)Extraction_B.sf_TriggerGen_kb.trigger !=
            (Extraction_PrevZCX.Extr_LowSpd_OptRamp_Enabled.OptRampLogic_Trig_ZCE
             [0] == POS_ZCSIG) &&
            Extraction_PrevZCX.Extr_LowSpd_OptRamp_Enabled.OptRampLogic_Trig_ZCE[
            0] != UNINITIALIZED_ZCSIG;
        zcEvent_idx_1_h = (sint32)
            Extraction_B.OutportBufferForExtr_LowSpd_EvOptRampStart !=
            (Extraction_PrevZCX.Extr_LowSpd_OptRamp_Enabled.OptRampLogic_Trig_ZCE
             [1] == POS_ZCSIG) &&
            Extraction_PrevZCX.Extr_LowSpd_OptRamp_Enabled.OptRampLogic_Trig_ZCE[
            1] != UNINITIALIZED_ZCSIG;
        zcEvent_idx_2_h = (sint32)
            Extraction_B.OutportBufferForExtr_LowSpd_EvStop !=
            (Extraction_PrevZCX.Extr_LowSpd_OptRamp_Enabled.OptRampLogic_Trig_ZCE
             [2] == POS_ZCSIG) &&
            Extraction_PrevZCX.Extr_LowSpd_OptRamp_Enabled.OptRampLogic_Trig_ZCE[
            2] != UNINITIALIZED_ZCSIG;
        if (zcEvent_idx_0_h || zcEvent_idx_1_h || zcEvent_idx_2_h)
        {
            rtb_inputevents_dv_idx_1_h = (sint8)(zcEvent_idx_1_h ?
                Extraction_B.OutportBufferForExtr_LowSpd_EvOptRampStart ?
                RISING_ZCEVENT : FALLING_ZCEVENT : NO_ZCEVENT);
            rtb_inputevents_dv_idx_2_h = (sint8)(zcEvent_idx_2_h ?
                Extraction_B.OutportBufferForExtr_LowSpd_EvStop ? RISING_ZCEVENT
                : FALLING_ZCEVENT : NO_ZCEVENT);

            /* Gateway: Extraction/Extr_LowSpd/Extr_LowSpd_Algorithms/Extr_LowSpd_OptRamp/Extr_LowSpd_OptRamp_Enabled/OptRampLogic */
            inputEventFiredFlag_h = 0;
            if ((sint8)(zcEvent_idx_0_h ? Extraction_B.sf_TriggerGen_kb.trigger ?
                        RISING_ZCEVENT : FALLING_ZCEVENT : NO_ZCEVENT) != 0)
            {
                inputEventFiredFlag_h = 1;
                if (Extraction_DW.Extr_LowSpd_OptRamp_Enabled.temporalCounter_i1
                    < MAX_uint32_T)
                {
                    Extraction_DW.Extr_LowSpd_OptRamp_Enabled.temporalCounter_i1
                        ++;
                }

                /* Event: '<S151>:48' */
                Extraction_DW.Extr_LowSpd_OptRamp_Enabled.sfEvent =
                    Extraction_event_ticks;
                Extraction_chartstep_c13_Extraction();
            }

            if (rtb_inputevents_dv_idx_1_h != 0)
            {
                inputEventFiredFlag_h = 1;

                /* Event: '<S151>:306' */
                Extraction_DW.Extr_LowSpd_OptRamp_Enabled.sfEvent =
                    Extraction_event_evStart;
                Extraction_chartstep_c13_Extraction();
            }

            if (rtb_inputevents_dv_idx_2_h != 0)
            {
                inputEventFiredFlag_h = 1;

                /* Event: '<S151>:195' */
                Extraction_DW.Extr_LowSpd_OptRamp_Enabled.sfEvent =
                    Extraction_event_evStop;
                Extraction_chartstep_c13_Extraction();
            }

            if (inputEventFiredFlag_h != 0 &&
                    Extraction_DW.Extr_LowSpd_OptRamp_Enabled.evOptRampEventCounter
                > 0U)
            {
                Extraction_B.VariantMergeForOutportExt_LowSpd_EvOptRampComplete =
                    !Extraction_B.VariantMergeForOutportExt_LowSpd_EvOptRampComplete;
                Extraction_DW.Extr_LowSpd_OptRamp_Enabled.evOptRampEventCounter
                    --;
            }

            if (inputEventFiredFlag_h != 0 &&
                    Extraction_DW.Extr_LowSpd_OptRamp_Enabled.evRedistEventCounter
                > 0U)
            {
                Extraction_B.VariantMergeForOutportExt_LowSpd_EvRedist =
                    !Extraction_B.VariantMergeForOutportExt_LowSpd_EvRedist;
                Extraction_DW.Extr_LowSpd_OptRamp_Enabled.evRedistEventCounter--;
            }
        }

        Extraction_PrevZCX.Extr_LowSpd_OptRamp_Enabled.OptRampLogic_Trig_ZCE[0] =
            Extraction_B.sf_TriggerGen_kb.trigger;
        Extraction_PrevZCX.Extr_LowSpd_OptRamp_Enabled.OptRampLogic_Trig_ZCE[1] =
            Extraction_B.OutportBufferForExtr_LowSpd_EvOptRampStart;
        Extraction_PrevZCX.Extr_LowSpd_OptRamp_Enabled.OptRampLogic_Trig_ZCE[2] =
            Extraction_B.OutportBufferForExtr_LowSpd_EvStop;
    }

#endif                                 /* EXTR_VARIANT_OPT_RAMP_ENABLED */

    /* End of Outputs for SubSystem: '<S121>/Extr_LowSpd_OptRamp' */

    /* SignalConversion: '<S121>/OutportBufferForExtr_LowSpd_EvCleanWasherException' */
    Extraction_B.OutportBufferForExtr_LowSpd_EvCleanWasherException =
        Extraction_B.Extr_LowSpd_EvCleanWasherException;

    /* SignalConversion: '<S121>/OutportBufferForExtr_LowSpd_EvLoadMeasComplete' */
    Extraction_B.OutportBufferForExtr_LowSpd_EvLoadMeasComplete =
        Extraction_B.Extr_LowSpd_EvLoadMeasComplete;

    /* SignalConversion: '<S121>/OutportBufferForExtr_LowSpd_EvMddpComplete' */
    Extraction_B.OutportBufferForExtr_LowSpd_EvMddpComplete =
        Extraction_B.Extr_LowSpd_EvMddpComplete;

    /* SignalConversion: '<S121>/OutportBufferForExtr_LowSpd_EvUnbChkComplete' */
    Extraction_B.OutportBufferForExtr_LowSpd_EvUnbChkComplete =
        Extraction_B.Extr_LowSpd_EvUnbChkComplete;

    /* SignalConversion: '<S121>/OutportBufferForExtr_LowSpd_EvUnbChkRebalStart' */
    Extraction_B.OutportBufferForExtr_LowSpd_EvUnbChkRebalStart =
        Extraction_B.Extr_LowSpd_EvUnbChkRebalStart;

    /* MinMax: '<S121>/SpeedLimit' */
    if (Extraction_B.MaxTargetSpeed < Extr_LowSpd_UnbChkSpdLimit)
    {
        Extr_LowSpd_MaxTargetSpeed = Extraction_B.MaxTargetSpeed;
    }
    else
    {
        Extr_LowSpd_MaxTargetSpeed = Extr_LowSpd_UnbChkSpdLimit;
    }

    /* End of MinMax: '<S121>/SpeedLimit' */

    /* RootInportFunctionCallGenerator: '<Root>/RootFcnCall_InsertedFor_Extraction__Handler5ms_at_outport_1' incorporates:
     *  SubSystem: '<S62>/Extr_HighSpd_Supervisor'
     */
    /* Chart: '<S88>/TriggerGen' */
    Extraction_TriggerGen(&Extraction_B.sf_TriggerGen_md,
                          &Extraction_DW.sf_TriggerGen_md);

    /* Outputs for Atomic SubSystem: '<S85>/Extr_HighSpd_SupervisorLogic' */
#if EXTR_VARIANT_HIGH_SPEED_PE_DISABLED

    /* Output and update for atomic system: '<S86>/Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled' */
    {
        sint32 inputEventFiredFlag_b;
        boolean_T zcEvent_b[19];
        boolean_T tmp_b;
        sint8 rtb_inputevents_mk_idx_1_b;
        sint8 rtb_inputevents_mk_idx_2_b;
        sint8 rtb_inputevents_mk_idx_3_b;
        sint8 rtb_inputevents_mk_idx_4_b;
        sint8 rtb_inputevents_mk_idx_5_b;
        sint8 rtb_inputevents_mk_idx_6_b;
        sint8 rtb_inputevents_mk_idx_7_b;
        sint8 rtb_inputevents_mk_idx_8_b;
        sint8 rtb_inputevents_mk_idx_9_b;
        sint8 rtb_inputevents_mk_idx_10_b;
        sint8 rtb_inputevents_mk_idx_11_b;
        sint8 rtb_inputevents_mk_idx_12_b;
        sint8 rtb_inputevents_mk_idx_13_b;
        sint8 rtb_inputevents_mk_idx_14_b;
        sint8 rtb_inputevents_mk_idx_15_b;
        sint8 rtb_inputevents_mk_idx_16_b;
        sint8 rtb_inputevents_mk_idx_17_b;
        sint8 rtb_inputevents_mk_idx_18_b;

        /* Chart: '<S92>/Extr_HighSpd_SupervisorLogic' incorporates:
         *  TriggerPort: '<S94>/input events'
         */
        zcEvent_b[0] = (sint32)Extraction_B.sf_TriggerGen_md.trigger !=
            (Extraction_PrevZCX.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.Extr_HighSpd_SupervisorLogic_Trig_ZCE_cb
             [0] == POS_ZCSIG) &&
            Extraction_PrevZCX.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.Extr_HighSpd_SupervisorLogic_Trig_ZCE_cb
            [0] != UNINITIALIZED_ZCSIG;
        zcEvent_b[1] = (sint32)
            Extraction_B.OutportBufferForExtr_Sup_EvHighSpdStart !=
            (Extraction_PrevZCX.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.Extr_HighSpd_SupervisorLogic_Trig_ZCE_cb
             [1] == POS_ZCSIG) &&
            Extraction_PrevZCX.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.Extr_HighSpd_SupervisorLogic_Trig_ZCE_cb
            [1] != UNINITIALIZED_ZCSIG;
        zcEvent_b[2] = (sint32)Extraction_B.OutportBufferForExtr_Sup_EvStopAll
            !=
            (Extraction_PrevZCX.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.Extr_HighSpd_SupervisorLogic_Trig_ZCE_cb
             [2] == POS_ZCSIG) &&
            Extraction_PrevZCX.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.Extr_HighSpd_SupervisorLogic_Trig_ZCE_cb
            [2] != UNINITIALIZED_ZCSIG;
        zcEvent_b[3] = (sint32)
            Extraction_B.OutportBufferForExtr_Sup_EvFoamDetected !=
            (Extraction_PrevZCX.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.Extr_HighSpd_SupervisorLogic_Trig_ZCE_cb
             [3] == POS_ZCSIG) &&
            Extraction_PrevZCX.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.Extr_HighSpd_SupervisorLogic_Trig_ZCE_cb
            [3] != UNINITIALIZED_ZCSIG;
        zcEvent_b[4] = (sint32)
            Extraction_B.VariantMergeForOutportExtr_HighSpd_EvRampComplete !=
            (Extraction_PrevZCX.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.Extr_HighSpd_SupervisorLogic_Trig_ZCE_cb
             [4] == POS_ZCSIG) &&
            Extraction_PrevZCX.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.Extr_HighSpd_SupervisorLogic_Trig_ZCE_cb
            [4] != UNINITIALIZED_ZCSIG;
        zcEvent_b[5] = (sint32)
            Extraction_B.VariantMergeForOutportExtr_HighSpd_EvPlateauComplete !=
            (Extraction_PrevZCX.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.Extr_HighSpd_SupervisorLogic_Trig_ZCE_cb
             [5] == POS_ZCSIG) &&
            Extraction_PrevZCX.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.Extr_HighSpd_SupervisorLogic_Trig_ZCE_cb
            [5] != UNINITIALIZED_ZCSIG;
        zcEvent_b[6] = (sint32)
            Extraction_B.VariantMergeForOutportExtr_HighSpd_EvViscousLimitExceeded
            !=
            (Extraction_PrevZCX.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.Extr_HighSpd_SupervisorLogic_Trig_ZCE_cb
             [6] == POS_ZCSIG) &&
            Extraction_PrevZCX.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.Extr_HighSpd_SupervisorLogic_Trig_ZCE_cb
            [6] != UNINITIALIZED_ZCSIG;
        zcEvent_b[7] = (sint32)
            Extraction_B.VariantMergeForOutportExtr_HighSpd_EvCEMFailureDetected
            !=
            (Extraction_PrevZCX.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.Extr_HighSpd_SupervisorLogic_Trig_ZCE_cb
             [7] == POS_ZCSIG) &&
            Extraction_PrevZCX.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.Extr_HighSpd_SupervisorLogic_Trig_ZCE_cb
            [7] != UNINITIALIZED_ZCSIG;
        zcEvent_b[8] = (sint32)
            Extraction_B.VariantMergeForOutportExtr_Accel_VibMapEvExcessiveFloorDisp
            !=
            (Extraction_PrevZCX.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.Extr_HighSpd_SupervisorLogic_Trig_ZCE_cb
             [8] == POS_ZCSIG) &&
            Extraction_PrevZCX.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.Extr_HighSpd_SupervisorLogic_Trig_ZCE_cb
            [8] != UNINITIALIZED_ZCSIG;
        zcEvent_b[9] = (sint32)
            Extraction_B.VariantMergeForOutportExtr_HighSpd_EvLoadMassExceeded
            !=
            (Extraction_PrevZCX.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.Extr_HighSpd_SupervisorLogic_Trig_ZCE_cb
             [9] == POS_ZCSIG) &&
            Extraction_PrevZCX.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.Extr_HighSpd_SupervisorLogic_Trig_ZCE_cb
            [9] != UNINITIALIZED_ZCSIG;
        zcEvent_b[10] = (sint32)
            Extraction_B.VariantMergeForOutportExtr_Accel_EvCabHit !=
            (Extraction_PrevZCX.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.Extr_HighSpd_SupervisorLogic_Trig_ZCE_cb
             [10] == POS_ZCSIG) &&
            Extraction_PrevZCX.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.Extr_HighSpd_SupervisorLogic_Trig_ZCE_cb
            [10] != UNINITIALIZED_ZCSIG;
        zcEvent_b[11] = (sint32)
            Extraction_B.OutportBufferForExtr_LowSpd_EvComplete !=
            (Extraction_PrevZCX.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.Extr_HighSpd_SupervisorLogic_Trig_ZCE_cb
             [11] == POS_ZCSIG) &&
            Extraction_PrevZCX.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.Extr_HighSpd_SupervisorLogic_Trig_ZCE_cb
            [11] != UNINITIALIZED_ZCSIG;
        zcEvent_b[12] = (sint32)
            Extraction_B.OutportBufferForExtr_HighSpd_EvExtndChkFailed !=
            (Extraction_PrevZCX.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.Extr_HighSpd_SupervisorLogic_Trig_ZCE_cb
             [12] == POS_ZCSIG) &&
            Extraction_PrevZCX.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.Extr_HighSpd_SupervisorLogic_Trig_ZCE_cb
            [12] != UNINITIALIZED_ZCSIG;
        zcEvent_b[13] = (sint32)
            Extraction_B.OutportBufferForExtr_Sup_ResetRebalCnt !=
            (Extraction_PrevZCX.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.Extr_HighSpd_SupervisorLogic_Trig_ZCE_cb
             [13] == POS_ZCSIG) &&
            Extraction_PrevZCX.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.Extr_HighSpd_SupervisorLogic_Trig_ZCE_cb
            [13] != UNINITIALIZED_ZCSIG;
        zcEvent_b[14] = (sint32)Extraction_B.OutportBufferForExtr_Sup_ClearFault
            !=
            (Extraction_PrevZCX.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.Extr_HighSpd_SupervisorLogic_Trig_ZCE_cb
             [14] == POS_ZCSIG) &&
            Extraction_PrevZCX.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.Extr_HighSpd_SupervisorLogic_Trig_ZCE_cb
            [14] != UNINITIALIZED_ZCSIG;
        zcEvent_b[15] = (sint32)
            Extraction_B.OutportBufferForExtr_Sup_EvRebalStart !=
            (Extraction_PrevZCX.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.Extr_HighSpd_SupervisorLogic_Trig_ZCE_cb
             [15] == POS_ZCSIG) &&
            Extraction_PrevZCX.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.Extr_HighSpd_SupervisorLogic_Trig_ZCE_cb
            [15] != UNINITIALIZED_ZCSIG;
        zcEvent_b[16] = (sint32)Extraction_B.Extr_Sup_EvSpeedErrorTimeout !=
            (Extraction_PrevZCX.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.Extr_HighSpd_SupervisorLogic_Trig_ZCE_cb
             [16] == POS_ZCSIG) &&
            Extraction_PrevZCX.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.Extr_HighSpd_SupervisorLogic_Trig_ZCE_cb
            [16] != UNINITIALIZED_ZCSIG;
        zcEvent_b[17] = (sint32)Extraction_B.Extr_Sup_EvSpeedReached !=
            (Extraction_PrevZCX.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.Extr_HighSpd_SupervisorLogic_Trig_ZCE_cb
             [17] == POS_ZCSIG) &&
            Extraction_PrevZCX.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.Extr_HighSpd_SupervisorLogic_Trig_ZCE_cb
            [17] != UNINITIALIZED_ZCSIG;
        zcEvent_b[18] = (sint32)
            Extraction_B.VariantMergeForOutportExtr_HighSpd_EvFrictionLimitExceeded
            !=
            (Extraction_PrevZCX.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.Extr_HighSpd_SupervisorLogic_Trig_ZCE_cb
             [18] == POS_ZCSIG) &&
            Extraction_PrevZCX.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.Extr_HighSpd_SupervisorLogic_Trig_ZCE_cb
            [18] != UNINITIALIZED_ZCSIG;
        tmp_b = false;
        for (inputEventFiredFlag_b = 0; inputEventFiredFlag_b < 19;
                inputEventFiredFlag_b++)
        {
            tmp_b = tmp_b || zcEvent_b[inputEventFiredFlag_b];
        }

        if (tmp_b)
        {
            rtb_inputevents_mk_idx_1_b = (sint8)(zcEvent_b[1] ?
                Extraction_B.OutportBufferForExtr_Sup_EvHighSpdStart ?
                RISING_ZCEVENT : FALLING_ZCEVENT : NO_ZCEVENT);
            rtb_inputevents_mk_idx_2_b = (sint8)(zcEvent_b[2] ?
                Extraction_B.OutportBufferForExtr_Sup_EvStopAll ? RISING_ZCEVENT
                : FALLING_ZCEVENT : NO_ZCEVENT);
            rtb_inputevents_mk_idx_3_b = (sint8)(zcEvent_b[3] ?
                Extraction_B.OutportBufferForExtr_Sup_EvFoamDetected ?
                RISING_ZCEVENT : FALLING_ZCEVENT : NO_ZCEVENT);
            rtb_inputevents_mk_idx_4_b = (sint8)(zcEvent_b[4] ?
                Extraction_B.VariantMergeForOutportExtr_HighSpd_EvRampComplete ?
                RISING_ZCEVENT : FALLING_ZCEVENT : NO_ZCEVENT);
            rtb_inputevents_mk_idx_5_b = (sint8)(zcEvent_b[5] ?
                Extraction_B.VariantMergeForOutportExtr_HighSpd_EvPlateauComplete
                ? RISING_ZCEVENT : FALLING_ZCEVENT : NO_ZCEVENT);
            rtb_inputevents_mk_idx_6_b = (sint8)(zcEvent_b[6] ?
                Extraction_B.VariantMergeForOutportExtr_HighSpd_EvViscousLimitExceeded
                ? RISING_ZCEVENT : FALLING_ZCEVENT : NO_ZCEVENT);
            rtb_inputevents_mk_idx_7_b = (sint8)(zcEvent_b[7] ?
                Extraction_B.VariantMergeForOutportExtr_HighSpd_EvCEMFailureDetected
                ? RISING_ZCEVENT : FALLING_ZCEVENT : NO_ZCEVENT);
            rtb_inputevents_mk_idx_8_b = (sint8)(zcEvent_b[8] ?
                Extraction_B.VariantMergeForOutportExtr_Accel_VibMapEvExcessiveFloorDisp
                ? RISING_ZCEVENT : FALLING_ZCEVENT : NO_ZCEVENT);
            rtb_inputevents_mk_idx_9_b = (sint8)(zcEvent_b[9] ?
                Extraction_B.VariantMergeForOutportExtr_HighSpd_EvLoadMassExceeded
                ? RISING_ZCEVENT : FALLING_ZCEVENT : NO_ZCEVENT);
            rtb_inputevents_mk_idx_10_b = (sint8)(zcEvent_b[10] ?
                Extraction_B.VariantMergeForOutportExtr_Accel_EvCabHit ?
                RISING_ZCEVENT : FALLING_ZCEVENT : NO_ZCEVENT);
            rtb_inputevents_mk_idx_11_b = (sint8)(zcEvent_b[11] ?
                Extraction_B.OutportBufferForExtr_LowSpd_EvComplete ?
                RISING_ZCEVENT : FALLING_ZCEVENT : NO_ZCEVENT);
            rtb_inputevents_mk_idx_12_b = (sint8)(zcEvent_b[12] ?
                Extraction_B.OutportBufferForExtr_HighSpd_EvExtndChkFailed ?
                RISING_ZCEVENT : FALLING_ZCEVENT : NO_ZCEVENT);
            rtb_inputevents_mk_idx_13_b = (sint8)(zcEvent_b[13] ?
                Extraction_B.OutportBufferForExtr_Sup_ResetRebalCnt ?
                RISING_ZCEVENT : FALLING_ZCEVENT : NO_ZCEVENT);
            rtb_inputevents_mk_idx_14_b = (sint8)(zcEvent_b[14] ?
                Extraction_B.OutportBufferForExtr_Sup_ClearFault ?
                RISING_ZCEVENT : FALLING_ZCEVENT : NO_ZCEVENT);
            rtb_inputevents_mk_idx_15_b = (sint8)(zcEvent_b[15] ?
                Extraction_B.OutportBufferForExtr_Sup_EvRebalStart ?
                RISING_ZCEVENT : FALLING_ZCEVENT : NO_ZCEVENT);
            rtb_inputevents_mk_idx_16_b = (sint8)(zcEvent_b[16] ?
                Extraction_B.Extr_Sup_EvSpeedErrorTimeout ? RISING_ZCEVENT :
                FALLING_ZCEVENT : NO_ZCEVENT);
            rtb_inputevents_mk_idx_17_b = (sint8)(zcEvent_b[17] ?
                Extraction_B.Extr_Sup_EvSpeedReached ? RISING_ZCEVENT :
                FALLING_ZCEVENT : NO_ZCEVENT);
            rtb_inputevents_mk_idx_18_b = (sint8)(zcEvent_b[18] ?
                Extraction_B.VariantMergeForOutportExtr_HighSpd_EvFrictionLimitExceeded
                ? RISING_ZCEVENT : FALLING_ZCEVENT : NO_ZCEVENT);

            /* Gateway: Extraction/Extr_HighSpd/Extr_HighSpd_Supervisor/Extr_HighSpd_Supervisor/Extr_HighSpd_SupervisorLogic/Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled/Extr_HighSpd_SupervisorLogic */
            inputEventFiredFlag_b = 0;
            if ((sint8)(zcEvent_b[0] ? Extraction_B.sf_TriggerGen_md.trigger ?
                        RISING_ZCEVENT : FALLING_ZCEVENT : NO_ZCEVENT) != 0)
            {
                inputEventFiredFlag_b = 1;
                if (Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.temporalCounter_i1
                    < MAX_uint32_T)
                {
                    Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.temporalCounter_i1
                        ++;
                }

                /* Event: '<S94>:145' */
                Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.sfEvent
                    = Extraction_event_Trigger;
                Extraction_chartstep_c48_Extraction();
            }

            if (rtb_inputevents_mk_idx_1_b != 0)
            {
                inputEventFiredFlag_b = 1;

                /* Event: '<S94>:1' */
                Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.sfEvent
                    = Extraction_event_Extr_Sup_EvHighSpdStart;
                Extraction_chartstep_c48_Extraction();
            }

            if (rtb_inputevents_mk_idx_2_b != 0)
            {
                inputEventFiredFlag_b = 1;

                /* Event: '<S94>:2' */
                Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.sfEvent
                    = Extraction_event_Extr_Sup_EvStopAll;
                Extraction_chartstep_c48_Extraction();
            }

            if (rtb_inputevents_mk_idx_3_b != 0)
            {
                inputEventFiredFlag_b = 1;

                /* Event: '<S94>:231' */
                Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.sfEvent
                    = Extraction_event_Extr_Sup_EvFoamDetected;
                Extraction_chartstep_c48_Extraction();
            }

            if (rtb_inputevents_mk_idx_4_b != 0)
            {
                inputEventFiredFlag_b = 1;

                /* Event: '<S94>:28' */
                Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.sfEvent
                    = Extraction_event_Extr_HighSpd_EvRampComplete;
                Extraction_chartstep_c48_Extraction();
            }

            if (rtb_inputevents_mk_idx_5_b != 0)
            {
                inputEventFiredFlag_b = 1;

                /* Event: '<S94>:29' */
                Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.sfEvent
                    = Extraction_event_Extr_HighSpd_EvPlateauComplete;
                Extraction_chartstep_c48_Extraction();
            }

            if (rtb_inputevents_mk_idx_6_b != 0)
            {
                inputEventFiredFlag_b = 1;

                /* Event: '<S94>:53' */
                Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.sfEvent
                    = Extraction_event_Extr_HighSpd_EvViscousLimitExceeded;
                Extraction_chartstep_c48_Extraction();
            }

            if (rtb_inputevents_mk_idx_7_b != 0)
            {
                inputEventFiredFlag_b = 1;

                /* Event: '<S94>:153' */
                Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.sfEvent
                    = Extraction_event_Extr_HighSpd_EvCEMFailureDetected;
                Extraction_chartstep_c48_Extraction();
            }

            if (rtb_inputevents_mk_idx_8_b != 0)
            {
                inputEventFiredFlag_b = 1;

                /* Event: '<S94>:216' */
                Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.sfEvent
                    = Extraction_event_Extr_Accel_VibMapEvExcessiveFloorDisp;
                Extraction_chartstep_c48_Extraction();
            }

            if (rtb_inputevents_mk_idx_9_b != 0)
            {
                inputEventFiredFlag_b = 1;

                /* Event: '<S94>:232' */
                Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.sfEvent
                    = Extraction_event_Extr_HighSpd_EvLoadMassExceeded;
                Extraction_chartstep_c48_Extraction();
            }

            if (rtb_inputevents_mk_idx_10_b != 0)
            {
                inputEventFiredFlag_b = 1;

                /* Event: '<S94>:241' */
                Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.sfEvent
                    = Extraction_event_Extr_Accel_EvCabHit;
                Extraction_chartstep_c48_Extraction();
            }

            if (rtb_inputevents_mk_idx_11_b != 0)
            {
                inputEventFiredFlag_b = 1;

                /* Event: '<S94>:256' */
                Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.sfEvent
                    = Extraction_event_Extr_LowSpd_EvComplete;
                Extraction_chartstep_c48_Extraction();
            }

            if (rtb_inputevents_mk_idx_12_b != 0)
            {
                inputEventFiredFlag_b = 1;

                /* Event: '<S94>:284' */
                Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.sfEvent
                    = Extraction_event_Extr_HighSpd_EvExtndChkFailed;
                Extraction_chartstep_c48_Extraction();
            }

            if (rtb_inputevents_mk_idx_13_b != 0)
            {
                inputEventFiredFlag_b = 1;

                /* Event: '<S94>:286' */
                Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.sfEvent
                    = Extraction_event_Extr_Sup_ResetRebalCnt;
                Extraction_chartstep_c48_Extraction();
            }

            if (rtb_inputevents_mk_idx_14_b != 0)
            {
                inputEventFiredFlag_b = 1;

                /* Event: '<S94>:289' */
                Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.sfEvent
                    = Extraction_event_Extr_Sup_ClearFault;
                Extraction_chartstep_c48_Extraction();
            }

            if (rtb_inputevents_mk_idx_15_b != 0)
            {
                inputEventFiredFlag_b = 1;

                /* Event: '<S94>:293' */
                Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.sfEvent
                    = Extraction_event_Extr_Sup_EvRebalStart;
                Extraction_chartstep_c48_Extraction();
            }

            if (rtb_inputevents_mk_idx_16_b != 0)
            {
                inputEventFiredFlag_b = 1;

                /* Event: '<S94>:310' */
                Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.sfEvent
                    = Extraction_event_Extr_Sup_EvSpeedErrorTimeout;
                Extraction_chartstep_c48_Extraction();
            }

            if (rtb_inputevents_mk_idx_17_b != 0)
            {
                inputEventFiredFlag_b = 1;

                /* Event: '<S94>:319' */
                Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.sfEvent
                    = Extraction_event_Extr_Sup_EvSpeedReached;
                Extraction_chartstep_c48_Extraction();
            }

            if (rtb_inputevents_mk_idx_18_b != 0)
            {
                inputEventFiredFlag_b = 1;

                /* Event: '<S94>:325' */
                Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.sfEvent
                    = Extraction_event_Extr_HighSpd_EvFrictionLimitExceeded1;
                Extraction_chartstep_c48_Extraction();
            }

            if (inputEventFiredFlag_b != 0 &&
                    Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.Extr_HighSpd_EvStartPlateauEventCounter
                > 0U)
            {
                Extraction_B.VariantMergeForOutportExtr_HighSpd_EvStartPlateau =
                    !Extraction_B.VariantMergeForOutportExtr_HighSpd_EvStartPlateau;
                Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.Extr_HighSpd_EvStartPlateauEventCounter
                    --;
            }

            if (inputEventFiredFlag_b != 0 &&
                    Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.Extr_HighSpd_EvStartRampEventCounter
                > 0U)
            {
                Extraction_B.VariantMergeForOutportExtr_HighSpd_EvStartRamp =
                    !Extraction_B.VariantMergeForOutportExtr_HighSpd_EvStartRamp;
                Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.Extr_HighSpd_EvStartRampEventCounter
                    --;
            }

            if (inputEventFiredFlag_b != 0 &&
                    Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.Extr_HighSpd_EvCompleteEventCounter
                > 0U)
            {
                Extraction_B.VariantMergeForOutportExtr_HighSpd_EvComplete =
                    !Extraction_B.VariantMergeForOutportExtr_HighSpd_EvComplete;
                Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.Extr_HighSpd_EvCompleteEventCounter
                    --;
            }

            if (inputEventFiredFlag_b != 0 &&
                    Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.Extr_HighSpd_EvStartRebalEventCounter
                > 0U)
            {
                Extraction_B.VariantMergeForOutportExtr_HighSpd_EvStartRebal =
                    !Extraction_B.VariantMergeForOutportExtr_HighSpd_EvStartRebal;
                Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.Extr_HighSpd_EvStartRebalEventCounter
                    --;
            }

            if (inputEventFiredFlag_b != 0 &&
                    Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.Extr_HighSpd_EvStopAllEventCounter
                > 0U)
            {
                Extraction_B.VariantMergeForOutportExtr_HighSpd_EvStopAll =
                    !Extraction_B.VariantMergeForOutportExtr_HighSpd_EvStopAll;
                Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.Extr_HighSpd_EvStopAllEventCounter
                    --;
            }

            if (inputEventFiredFlag_b != 0 &&
                    Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.Extr_HighSpd_EvDropToDistSpeedEventCounter
                > 0U)
            {
                Extraction_B.VariantMergeForOutportExtr_HighSpd_EvDropToDistSpeed
                    =
                    !Extraction_B.VariantMergeForOutportExtr_HighSpd_EvDropToDistSpeed;
                Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.Extr_HighSpd_EvDropToDistSpeedEventCounter
                    --;
            }
        }

        Extraction_PrevZCX.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.Extr_HighSpd_SupervisorLogic_Trig_ZCE_cb
            [0] = Extraction_B.sf_TriggerGen_md.trigger;
        Extraction_PrevZCX.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.Extr_HighSpd_SupervisorLogic_Trig_ZCE_cb
            [1] = Extraction_B.OutportBufferForExtr_Sup_EvHighSpdStart;
        Extraction_PrevZCX.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.Extr_HighSpd_SupervisorLogic_Trig_ZCE_cb
            [2] = Extraction_B.OutportBufferForExtr_Sup_EvStopAll;
        Extraction_PrevZCX.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.Extr_HighSpd_SupervisorLogic_Trig_ZCE_cb
            [3] = Extraction_B.OutportBufferForExtr_Sup_EvFoamDetected;
        Extraction_PrevZCX.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.Extr_HighSpd_SupervisorLogic_Trig_ZCE_cb
            [4] = Extraction_B.VariantMergeForOutportExtr_HighSpd_EvRampComplete;
        Extraction_PrevZCX.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.Extr_HighSpd_SupervisorLogic_Trig_ZCE_cb
            [5] =
            Extraction_B.VariantMergeForOutportExtr_HighSpd_EvPlateauComplete;
        Extraction_PrevZCX.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.Extr_HighSpd_SupervisorLogic_Trig_ZCE_cb
            [6] =
            Extraction_B.VariantMergeForOutportExtr_HighSpd_EvViscousLimitExceeded;
        Extraction_PrevZCX.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.Extr_HighSpd_SupervisorLogic_Trig_ZCE_cb
            [7] =
            Extraction_B.VariantMergeForOutportExtr_HighSpd_EvCEMFailureDetected;
        Extraction_PrevZCX.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.Extr_HighSpd_SupervisorLogic_Trig_ZCE_cb
            [8] =
            Extraction_B.VariantMergeForOutportExtr_Accel_VibMapEvExcessiveFloorDisp;
        Extraction_PrevZCX.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.Extr_HighSpd_SupervisorLogic_Trig_ZCE_cb
            [9] =
            Extraction_B.VariantMergeForOutportExtr_HighSpd_EvLoadMassExceeded;
        Extraction_PrevZCX.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.Extr_HighSpd_SupervisorLogic_Trig_ZCE_cb
            [10] = Extraction_B.VariantMergeForOutportExtr_Accel_EvCabHit;
        Extraction_PrevZCX.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.Extr_HighSpd_SupervisorLogic_Trig_ZCE_cb
            [11] = Extraction_B.OutportBufferForExtr_LowSpd_EvComplete;
        Extraction_PrevZCX.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.Extr_HighSpd_SupervisorLogic_Trig_ZCE_cb
            [12] = Extraction_B.OutportBufferForExtr_HighSpd_EvExtndChkFailed;
        Extraction_PrevZCX.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.Extr_HighSpd_SupervisorLogic_Trig_ZCE_cb
            [13] = Extraction_B.OutportBufferForExtr_Sup_ResetRebalCnt;
        Extraction_PrevZCX.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.Extr_HighSpd_SupervisorLogic_Trig_ZCE_cb
            [14] = Extraction_B.OutportBufferForExtr_Sup_ClearFault;
        Extraction_PrevZCX.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.Extr_HighSpd_SupervisorLogic_Trig_ZCE_cb
            [15] = Extraction_B.OutportBufferForExtr_Sup_EvRebalStart;
        Extraction_PrevZCX.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.Extr_HighSpd_SupervisorLogic_Trig_ZCE_cb
            [16] = Extraction_B.Extr_Sup_EvSpeedErrorTimeout;
        Extraction_PrevZCX.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.Extr_HighSpd_SupervisorLogic_Trig_ZCE_cb
            [17] = Extraction_B.Extr_Sup_EvSpeedReached;
        Extraction_PrevZCX.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.Extr_HighSpd_SupervisorLogic_Trig_ZCE_cb
            [18] =
            Extraction_B.VariantMergeForOutportExtr_HighSpd_EvFrictionLimitExceeded;

        /* DataStoreWrite: '<S92>/Data Store Write' */
        Extr_HighSpd_CurrPulseInd =
            Extraction_B.VariantMergeForOutportExtr_HighSpd_CurrPulseInd;

        /* DataStoreWrite: '<S92>/Data Store Write1' */
        Extr_HighSpd_State =
            Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.Extr_HighSpd_State_lz;

        /* SignalConversion: '<S92>/TargetSpeed' */
        Extr_HighSpd_TargetSpeed =
            Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.Extr_HighSpd_TargetSpeed_ln;
    }

#elif EXTR_VARIANT_HIGH_SPEED_PE_ENABLED

    /* Chart: '<S93>/Extr_HighSpd_SupervisorLogic' incorporates:
     *  TriggerPort: '<S97>/input events'
     */
    Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.zcEvent[0]
        = (sint32)Extraction_B.sf_TriggerGen_md.trigger !=
        (Extraction_PrevZCX.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.Extr_HighSpd_SupervisorLogic_Trig_ZCE
         [0] == POS_ZCSIG) &&
        Extraction_PrevZCX.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.Extr_HighSpd_SupervisorLogic_Trig_ZCE
        [0] != UNINITIALIZED_ZCSIG;
    Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.zcEvent[1]
        = (sint32)Extraction_B.OutportBufferForExtr_Sup_EvHighSpdStart !=
        (Extraction_PrevZCX.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.Extr_HighSpd_SupervisorLogic_Trig_ZCE
         [1] == POS_ZCSIG) &&
        Extraction_PrevZCX.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.Extr_HighSpd_SupervisorLogic_Trig_ZCE
        [1] != UNINITIALIZED_ZCSIG;
    Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.zcEvent[2]
        = (sint32)Extraction_B.OutportBufferForExtr_Sup_EvStopAll !=
        (Extraction_PrevZCX.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.Extr_HighSpd_SupervisorLogic_Trig_ZCE
         [2] == POS_ZCSIG) &&
        Extraction_PrevZCX.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.Extr_HighSpd_SupervisorLogic_Trig_ZCE
        [2] != UNINITIALIZED_ZCSIG;
    Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.zcEvent[3]
        = (sint32)Extraction_B.OutportBufferForExtr_Sup_EvFoamDetected !=
        (Extraction_PrevZCX.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.Extr_HighSpd_SupervisorLogic_Trig_ZCE
         [3] == POS_ZCSIG) &&
        Extraction_PrevZCX.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.Extr_HighSpd_SupervisorLogic_Trig_ZCE
        [3] != UNINITIALIZED_ZCSIG;
    Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.zcEvent[4]
        = (sint32)Extraction_B.VariantMergeForOutportExtr_HighSpd_EvRampComplete
        !=
        (Extraction_PrevZCX.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.Extr_HighSpd_SupervisorLogic_Trig_ZCE
         [4] == POS_ZCSIG) &&
        Extraction_PrevZCX.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.Extr_HighSpd_SupervisorLogic_Trig_ZCE
        [4] != UNINITIALIZED_ZCSIG;
    Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.zcEvent[5]
        = (sint32)
        Extraction_B.VariantMergeForOutportExtr_HighSpd_EvPlateauComplete !=
        (Extraction_PrevZCX.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.Extr_HighSpd_SupervisorLogic_Trig_ZCE
         [5] == POS_ZCSIG) &&
        Extraction_PrevZCX.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.Extr_HighSpd_SupervisorLogic_Trig_ZCE
        [5] != UNINITIALIZED_ZCSIG;
    Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.zcEvent[6]
        = (sint32)
        Extraction_B.VariantMergeForOutportExtr_HighSpd_EvViscousLimitExceeded
        !=
        (Extraction_PrevZCX.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.Extr_HighSpd_SupervisorLogic_Trig_ZCE
         [6] == POS_ZCSIG) &&
        Extraction_PrevZCX.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.Extr_HighSpd_SupervisorLogic_Trig_ZCE
        [6] != UNINITIALIZED_ZCSIG;
    Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.zcEvent[7]
        = (sint32)
        Extraction_B.VariantMergeForOutportExtr_HighSpd_EvCEMFailureDetected !=
        (Extraction_PrevZCX.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.Extr_HighSpd_SupervisorLogic_Trig_ZCE
         [7] == POS_ZCSIG) &&
        Extraction_PrevZCX.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.Extr_HighSpd_SupervisorLogic_Trig_ZCE
        [7] != UNINITIALIZED_ZCSIG;
    Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.zcEvent[8]
        = (sint32)
        Extraction_B.VariantMergeForOutportExtr_Accel_VibMapEvExcessiveFloorDisp
        !=
        (Extraction_PrevZCX.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.Extr_HighSpd_SupervisorLogic_Trig_ZCE
         [8] == POS_ZCSIG) &&
        Extraction_PrevZCX.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.Extr_HighSpd_SupervisorLogic_Trig_ZCE
        [8] != UNINITIALIZED_ZCSIG;
    Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.zcEvent[9]
        = (sint32)
        Extraction_B.VariantMergeForOutportExtr_HighSpd_EvLoadMassExceeded !=
        (Extraction_PrevZCX.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.Extr_HighSpd_SupervisorLogic_Trig_ZCE
         [9] == POS_ZCSIG) &&
        Extraction_PrevZCX.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.Extr_HighSpd_SupervisorLogic_Trig_ZCE
        [9] != UNINITIALIZED_ZCSIG;
    Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.zcEvent
        [10] = (sint32)Extraction_B.VariantMergeForOutportExtr_Accel_EvCabHit !=
        (Extraction_PrevZCX.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.Extr_HighSpd_SupervisorLogic_Trig_ZCE
         [10] == POS_ZCSIG) &&
        Extraction_PrevZCX.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.Extr_HighSpd_SupervisorLogic_Trig_ZCE
        [10] != UNINITIALIZED_ZCSIG;
    Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.zcEvent
        [11] = (sint32)Extraction_B.OutportBufferForExtr_LowSpd_EvComplete !=
        (Extraction_PrevZCX.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.Extr_HighSpd_SupervisorLogic_Trig_ZCE
         [11] == POS_ZCSIG) &&
        Extraction_PrevZCX.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.Extr_HighSpd_SupervisorLogic_Trig_ZCE
        [11] != UNINITIALIZED_ZCSIG;
    Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.zcEvent
        [12] = (sint32)
        Extraction_B.OutportBufferForExtr_HighSpd_EvExtndChkFailed !=
        (Extraction_PrevZCX.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.Extr_HighSpd_SupervisorLogic_Trig_ZCE
         [12] == POS_ZCSIG) &&
        Extraction_PrevZCX.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.Extr_HighSpd_SupervisorLogic_Trig_ZCE
        [12] != UNINITIALIZED_ZCSIG;
    Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.zcEvent
        [13] = (sint32)Extraction_B.OutportBufferForExtr_Sup_ResetRebalCnt !=
        (Extraction_PrevZCX.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.Extr_HighSpd_SupervisorLogic_Trig_ZCE
         [13] == POS_ZCSIG) &&
        Extraction_PrevZCX.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.Extr_HighSpd_SupervisorLogic_Trig_ZCE
        [13] != UNINITIALIZED_ZCSIG;
    Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.zcEvent
        [14] = (sint32)Extraction_B.OutportBufferForExtr_Sup_ClearFault !=
        (Extraction_PrevZCX.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.Extr_HighSpd_SupervisorLogic_Trig_ZCE
         [14] == POS_ZCSIG) &&
        Extraction_PrevZCX.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.Extr_HighSpd_SupervisorLogic_Trig_ZCE
        [14] != UNINITIALIZED_ZCSIG;
    Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.zcEvent
        [15] = (sint32)Extraction_B.OutportBufferForExtr_Sup_EvRebalStart !=
        (Extraction_PrevZCX.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.Extr_HighSpd_SupervisorLogic_Trig_ZCE
         [15] == POS_ZCSIG) &&
        Extraction_PrevZCX.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.Extr_HighSpd_SupervisorLogic_Trig_ZCE
        [15] != UNINITIALIZED_ZCSIG;
    Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.zcEvent
        [16] = (sint32)Extraction_B.Extr_Sup_EvSpeedErrorTimeout !=
        (Extraction_PrevZCX.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.Extr_HighSpd_SupervisorLogic_Trig_ZCE
         [16] == POS_ZCSIG) &&
        Extraction_PrevZCX.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.Extr_HighSpd_SupervisorLogic_Trig_ZCE
        [16] != UNINITIALIZED_ZCSIG;
    Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.zcEvent
        [17] = (sint32)Extraction_B.Extr_Sup_EvSpeedReached !=
        (Extraction_PrevZCX.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.Extr_HighSpd_SupervisorLogic_Trig_ZCE
         [17] == POS_ZCSIG) &&
        Extraction_PrevZCX.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.Extr_HighSpd_SupervisorLogic_Trig_ZCE
        [17] != UNINITIALIZED_ZCSIG;
    Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.zcEvent
        [18] = (sint32)
        Extraction_B.VariantMergeForOutportExtr_HighSpd_EvFrictionLimitExceeded
        !=
        (Extraction_PrevZCX.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.Extr_HighSpd_SupervisorLogic_Trig_ZCE
         [18] == POS_ZCSIG) &&
        Extraction_PrevZCX.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.Extr_HighSpd_SupervisorLogic_Trig_ZCE
        [18] != UNINITIALIZED_ZCSIG;
    Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.b0 =
        false;
    for (Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.inputEventFiredFlag
         = 0;
            Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.inputEventFiredFlag
         < 19;
            Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.inputEventFiredFlag
            ++)
    {
        Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.b0 =
            Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.b0
            ||
            Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.zcEvent
            [Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.inputEventFiredFlag];
    }

    if (Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.b0)
    {
        Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.rtb_inputevents_hu_idx_1
            = (sint8)
            (Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.zcEvent
             [1] ? Extraction_B.OutportBufferForExtr_Sup_EvHighSpdStart ?
             RISING_ZCEVENT : FALLING_ZCEVENT : NO_ZCEVENT);
        Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.rtb_inputevents_hu_idx_2
            = (sint8)
            (Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.zcEvent
             [2] ? Extraction_B.OutportBufferForExtr_Sup_EvStopAll ?
             RISING_ZCEVENT : FALLING_ZCEVENT : NO_ZCEVENT);
        Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.rtb_inputevents_hu_idx_3
            = (sint8)
            (Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.zcEvent
             [3] ? Extraction_B.OutportBufferForExtr_Sup_EvFoamDetected ?
             RISING_ZCEVENT : FALLING_ZCEVENT : NO_ZCEVENT);
        Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.rtb_inputevents_hu_idx_4
            = (sint8)
            (Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.zcEvent
             [4] ?
             Extraction_B.VariantMergeForOutportExtr_HighSpd_EvRampComplete ?
             RISING_ZCEVENT : FALLING_ZCEVENT : NO_ZCEVENT);
        Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.rtb_inputevents_hu_idx_5
            = (sint8)
            (Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.zcEvent
             [5] ?
             Extraction_B.VariantMergeForOutportExtr_HighSpd_EvPlateauComplete ?
             RISING_ZCEVENT : FALLING_ZCEVENT : NO_ZCEVENT);
        Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.rtb_inputevents_hu_idx_6
            = (sint8)
            (Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.zcEvent
             [6] ?
             Extraction_B.VariantMergeForOutportExtr_HighSpd_EvViscousLimitExceeded
             ? RISING_ZCEVENT : FALLING_ZCEVENT : NO_ZCEVENT);
        Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.rtb_inputevents_hu_idx_7
            = (sint8)
            (Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.zcEvent
             [7] ?
             Extraction_B.VariantMergeForOutportExtr_HighSpd_EvCEMFailureDetected
             ? RISING_ZCEVENT : FALLING_ZCEVENT : NO_ZCEVENT);
        Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.rtb_inputevents_hu_idx_8
            = (sint8)
            (Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.zcEvent
             [8] ?
             Extraction_B.VariantMergeForOutportExtr_Accel_VibMapEvExcessiveFloorDisp
             ? RISING_ZCEVENT : FALLING_ZCEVENT : NO_ZCEVENT);
        Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.rtb_inputevents_hu_idx_9
            = (sint8)
            (Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.zcEvent
             [9] ?
             Extraction_B.VariantMergeForOutportExtr_HighSpd_EvLoadMassExceeded ?
             RISING_ZCEVENT : FALLING_ZCEVENT : NO_ZCEVENT);
        Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.rtb_inputevents_hu_idx_10
            = (sint8)
            (Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.zcEvent
             [10] ? Extraction_B.VariantMergeForOutportExtr_Accel_EvCabHit ?
             RISING_ZCEVENT : FALLING_ZCEVENT : NO_ZCEVENT);
        Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.rtb_inputevents_hu_idx_11
            = (sint8)
            (Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.zcEvent
             [11] ? Extraction_B.OutportBufferForExtr_LowSpd_EvComplete ?
             RISING_ZCEVENT : FALLING_ZCEVENT : NO_ZCEVENT);
        Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.rtb_inputevents_hu_idx_12
            = (sint8)
            (Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.zcEvent
             [12] ? Extraction_B.OutportBufferForExtr_HighSpd_EvExtndChkFailed ?
             RISING_ZCEVENT : FALLING_ZCEVENT : NO_ZCEVENT);
        Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.rtb_inputevents_hu_idx_13
            = (sint8)
            (Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.zcEvent
             [13] ? Extraction_B.OutportBufferForExtr_Sup_ResetRebalCnt ?
             RISING_ZCEVENT : FALLING_ZCEVENT : NO_ZCEVENT);
        Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.rtb_inputevents_hu_idx_14
            = (sint8)
            (Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.zcEvent
             [14] ? Extraction_B.OutportBufferForExtr_Sup_ClearFault ?
             RISING_ZCEVENT : FALLING_ZCEVENT : NO_ZCEVENT);
        Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.rtb_inputevents_hu_idx_15
            = (sint8)
            (Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.zcEvent
             [15] ? Extraction_B.OutportBufferForExtr_Sup_EvRebalStart ?
             RISING_ZCEVENT : FALLING_ZCEVENT : NO_ZCEVENT);
        Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.rtb_inputevents_hu_idx_16
            = (sint8)
            (Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.zcEvent
             [16] ? Extraction_B.Extr_Sup_EvSpeedErrorTimeout ? RISING_ZCEVENT :
             FALLING_ZCEVENT : NO_ZCEVENT);
        Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.rtb_inputevents_hu_idx_17
            = (sint8)
            (Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.zcEvent
             [17] ? Extraction_B.Extr_Sup_EvSpeedReached ? RISING_ZCEVENT :
             FALLING_ZCEVENT : NO_ZCEVENT);
        Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.rtb_inputevents_hu_idx_18
            = (sint8)
            (Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.zcEvent
             [18] ?
             Extraction_B.VariantMergeForOutportExtr_HighSpd_EvFrictionLimitExceeded
             ? RISING_ZCEVENT : FALLING_ZCEVENT : NO_ZCEVENT);

        /* Gateway: Extraction/Extr_HighSpd/Extr_HighSpd_Supervisor/Extr_HighSpd_Supervisor/Extr_HighSpd_SupervisorLogic/Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled/Extr_HighSpd_SupervisorLogic */
        Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.inputEventFiredFlag
            = 0;
        if ((sint8)
                (Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.zcEvent
                 [0] ? Extraction_B.sf_TriggerGen_md.trigger ? RISING_ZCEVENT :
                 FALLING_ZCEVENT : NO_ZCEVENT) != 0)
        {
            Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.inputEventFiredFlag
                = 1;

            /* Event: '<S97>:145' */
            Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.sfEvent
                = Extraction_event_Trigger_o5;
            Extraction_chartstep_c17_Extraction();
        }

        if (Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.rtb_inputevents_hu_idx_1
            != 0)
        {
            Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.inputEventFiredFlag
                = 1;

            /* Event: '<S97>:1' */
            Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.sfEvent
                = Extraction_event_Extr_Sup_EvHighSpdStart_px;
            Extraction_chartstep_c17_Extraction();
        }

        if (Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.rtb_inputevents_hu_idx_2
            != 0)
        {
            Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.inputEventFiredFlag
                = 1;

            /* Event: '<S97>:2' */
            Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.sfEvent
                = Extraction_event_Extr_Sup_EvStopAll_dp;
            Extraction_chartstep_c17_Extraction();
        }

        if (Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.rtb_inputevents_hu_idx_3
            != 0)
        {
            Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.inputEventFiredFlag
                = 1;

            /* Event: '<S97>:231' */
            Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.sfEvent
                = Extraction_event_Extr_Sup_EvFoamDetected_md;
            Extraction_chartstep_c17_Extraction();
        }

        if (Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.rtb_inputevents_hu_idx_4
            != 0)
        {
            Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.inputEventFiredFlag
                = 1;

            /* Event: '<S97>:28' */
            Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.sfEvent
                = Extraction_event_Extr_HighSpd_EvRampComplete_b3;
            Extraction_chartstep_c17_Extraction();
        }

        if (Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.rtb_inputevents_hu_idx_5
            != 0)
        {
            Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.inputEventFiredFlag
                = 1;

            /* Event: '<S97>:29' */
            Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.sfEvent
                = Extraction_event_Extr_HighSpd_EvPlateauComplete_mw;
            Extraction_chartstep_c17_Extraction();
        }

        if (Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.rtb_inputevents_hu_idx_6
            != 0)
        {
            Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.inputEventFiredFlag
                = 1;

            /* Event: '<S97>:53' */
            Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.sfEvent
                = Extraction_event_Extr_HighSpd_EvViscousLimitExceeded_gz;
            Extraction_chartstep_c17_Extraction();
        }

        if (Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.rtb_inputevents_hu_idx_7
            != 0)
        {
            Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.inputEventFiredFlag
                = 1;

            /* Event: '<S97>:153' */
            Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.sfEvent
                = Extraction_event_Extr_HighSpd_EvCEMFailureDetected_gz;
            Extraction_chartstep_c17_Extraction();
        }

        if (Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.rtb_inputevents_hu_idx_8
            != 0)
        {
            Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.inputEventFiredFlag
                = 1;

            /* Event: '<S97>:216' */
            Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.sfEvent
                = Extraction_event_Extr_Accel_VibMapEvExcessiveFloorDisp_ed;
            Extraction_chartstep_c17_Extraction();
        }

        if (Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.rtb_inputevents_hu_idx_9
            != 0)
        {
            Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.inputEventFiredFlag
                = 1;

            /* Event: '<S97>:232' */
            Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.sfEvent
                = Extraction_event_Extr_HighSpd_EvLoadMassExceeded_ha;
            Extraction_chartstep_c17_Extraction();
        }

        if (Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.rtb_inputevents_hu_idx_10
            != 0)
        {
            Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.inputEventFiredFlag
                = 1;

            /* Event: '<S97>:241' */
            Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.sfEvent
                = Extraction_event_Extr_Accel_EvCabHit_ns;
            Extraction_chartstep_c17_Extraction();
        }

        if (Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.rtb_inputevents_hu_idx_11
            != 0)
        {
            Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.inputEventFiredFlag
                = 1;

            /* Event: '<S97>:256' */
            Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.sfEvent
                = Extraction_event_Extr_LowSpd_EvComplete_kv;
            Extraction_chartstep_c17_Extraction();
        }

        if (Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.rtb_inputevents_hu_idx_12
            != 0)
        {
            Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.inputEventFiredFlag
                = 1;

            /* Event: '<S97>:284' */
            Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.sfEvent
                = Extraction_event_Extr_HighSpd_EvExtndChkFailed_fo;
            Extraction_chartstep_c17_Extraction();
        }

        if (Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.rtb_inputevents_hu_idx_13
            != 0)
        {
            Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.inputEventFiredFlag
                = 1;

            /* Event: '<S97>:286' */
            Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.sfEvent
                = Extraction_event_Extr_Sup_ResetRebalCnt_fl;
            Extraction_chartstep_c17_Extraction();
        }

        if (Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.rtb_inputevents_hu_idx_14
            != 0)
        {
            Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.inputEventFiredFlag
                = 1;

            /* Event: '<S97>:289' */
            Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.sfEvent
                = Extraction_event_Extr_Sup_ClearFault_kr;
            Extraction_chartstep_c17_Extraction();
        }

        if (Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.rtb_inputevents_hu_idx_15
            != 0)
        {
            Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.inputEventFiredFlag
                = 1;

            /* Event: '<S97>:293' */
            Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.sfEvent
                = Extraction_event_Extr_Sup_EvRebalStart_ba;
            Extraction_chartstep_c17_Extraction();
        }

        if (Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.rtb_inputevents_hu_idx_16
            != 0)
        {
            Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.inputEventFiredFlag
                = 1;

            /* Event: '<S97>:410' */
            Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.sfEvent
                = Extraction_event_Extr_Sup_EvSpeedErrorTimeout_o2;
            Extraction_chartstep_c17_Extraction();
        }

        if (Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.rtb_inputevents_hu_idx_17
            != 0)
        {
            Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.inputEventFiredFlag
                = 1;

            /* Event: '<S97>:419' */
            Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.sfEvent
                = Extraction_event_Extr_Sup_EvSpeedReached_a1;
            Extraction_chartstep_c17_Extraction();
        }

        if (Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.rtb_inputevents_hu_idx_18
            != 0)
        {
            Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.inputEventFiredFlag
                = 1;

            /* Event: '<S97>:432' */
            Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.sfEvent
                = Extraction_event_Extr_HighSpd_EvFrictionLimitExceeded;
            Extraction_chartstep_c17_Extraction();
        }

        if (Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.inputEventFiredFlag
            != 0 &&
                Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.Extr_HighSpd_EvStartPlateauEventCounter
            > 0U)
        {
            Extraction_B.VariantMergeForOutportExtr_HighSpd_EvStartPlateau =
                !Extraction_B.VariantMergeForOutportExtr_HighSpd_EvStartPlateau;
            Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.Extr_HighSpd_EvStartPlateauEventCounter
                --;
        }

        if (Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.inputEventFiredFlag
            != 0 &&
                Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.Extr_HighSpd_EvStartRampEventCounter
            > 0U)
        {
            Extraction_B.VariantMergeForOutportExtr_HighSpd_EvStartRamp =
                !Extraction_B.VariantMergeForOutportExtr_HighSpd_EvStartRamp;
            Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.Extr_HighSpd_EvStartRampEventCounter
                --;
        }

        if (Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.inputEventFiredFlag
            != 0 &&
                Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.Extr_HighSpd_EvCompleteEventCounter
            > 0U)
        {
            Extraction_B.VariantMergeForOutportExtr_HighSpd_EvComplete =
                !Extraction_B.VariantMergeForOutportExtr_HighSpd_EvComplete;
            Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.Extr_HighSpd_EvCompleteEventCounter
                --;
        }

        if (Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.inputEventFiredFlag
            != 0 &&
                Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.Extr_HighSpd_EvStartRebalEventCounter
            > 0U)
        {
            Extraction_B.VariantMergeForOutportExtr_HighSpd_EvStartRebal =
                !Extraction_B.VariantMergeForOutportExtr_HighSpd_EvStartRebal;
            Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.Extr_HighSpd_EvStartRebalEventCounter
                --;
        }

        if (Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.inputEventFiredFlag
            != 0 &&
                Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.Extr_HighSpd_EvStopAllEventCounter
            > 0U)
        {
            Extraction_B.VariantMergeForOutportExtr_HighSpd_EvStopAll =
                !Extraction_B.VariantMergeForOutportExtr_HighSpd_EvStopAll;
            Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.Extr_HighSpd_EvStopAllEventCounter
                --;
        }

        if (Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.inputEventFiredFlag
            != 0 &&
                Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.Extr_HighSpd_EvDropToDistSpeedEventCounter
            > 0U)
        {
            Extraction_B.VariantMergeForOutportExtr_HighSpd_EvDropToDistSpeed =
                !Extraction_B.VariantMergeForOutportExtr_HighSpd_EvDropToDistSpeed;
            Extraction_DW.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.Extr_HighSpd_EvDropToDistSpeedEventCounter
                --;
        }
    }

    Extraction_PrevZCX.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.Extr_HighSpd_SupervisorLogic_Trig_ZCE
        [0] = Extraction_B.sf_TriggerGen_md.trigger;
    Extraction_PrevZCX.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.Extr_HighSpd_SupervisorLogic_Trig_ZCE
        [1] = Extraction_B.OutportBufferForExtr_Sup_EvHighSpdStart;
    Extraction_PrevZCX.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.Extr_HighSpd_SupervisorLogic_Trig_ZCE
        [2] = Extraction_B.OutportBufferForExtr_Sup_EvStopAll;
    Extraction_PrevZCX.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.Extr_HighSpd_SupervisorLogic_Trig_ZCE
        [3] = Extraction_B.OutportBufferForExtr_Sup_EvFoamDetected;
    Extraction_PrevZCX.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.Extr_HighSpd_SupervisorLogic_Trig_ZCE
        [4] = Extraction_B.VariantMergeForOutportExtr_HighSpd_EvRampComplete;
    Extraction_PrevZCX.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.Extr_HighSpd_SupervisorLogic_Trig_ZCE
        [5] = Extraction_B.VariantMergeForOutportExtr_HighSpd_EvPlateauComplete;
    Extraction_PrevZCX.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.Extr_HighSpd_SupervisorLogic_Trig_ZCE
        [6] =
        Extraction_B.VariantMergeForOutportExtr_HighSpd_EvViscousLimitExceeded;
    Extraction_PrevZCX.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.Extr_HighSpd_SupervisorLogic_Trig_ZCE
        [7] =
        Extraction_B.VariantMergeForOutportExtr_HighSpd_EvCEMFailureDetected;
    Extraction_PrevZCX.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.Extr_HighSpd_SupervisorLogic_Trig_ZCE
        [8] =
        Extraction_B.VariantMergeForOutportExtr_Accel_VibMapEvExcessiveFloorDisp;
    Extraction_PrevZCX.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.Extr_HighSpd_SupervisorLogic_Trig_ZCE
        [9] = Extraction_B.VariantMergeForOutportExtr_HighSpd_EvLoadMassExceeded;
    Extraction_PrevZCX.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.Extr_HighSpd_SupervisorLogic_Trig_ZCE
        [10] = Extraction_B.VariantMergeForOutportExtr_Accel_EvCabHit;
    Extraction_PrevZCX.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.Extr_HighSpd_SupervisorLogic_Trig_ZCE
        [11] = Extraction_B.OutportBufferForExtr_LowSpd_EvComplete;
    Extraction_PrevZCX.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.Extr_HighSpd_SupervisorLogic_Trig_ZCE
        [12] = Extraction_B.OutportBufferForExtr_HighSpd_EvExtndChkFailed;
    Extraction_PrevZCX.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.Extr_HighSpd_SupervisorLogic_Trig_ZCE
        [13] = Extraction_B.OutportBufferForExtr_Sup_ResetRebalCnt;
    Extraction_PrevZCX.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.Extr_HighSpd_SupervisorLogic_Trig_ZCE
        [14] = Extraction_B.OutportBufferForExtr_Sup_ClearFault;
    Extraction_PrevZCX.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.Extr_HighSpd_SupervisorLogic_Trig_ZCE
        [15] = Extraction_B.OutportBufferForExtr_Sup_EvRebalStart;
    Extraction_PrevZCX.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.Extr_HighSpd_SupervisorLogic_Trig_ZCE
        [16] = Extraction_B.Extr_Sup_EvSpeedErrorTimeout;
    Extraction_PrevZCX.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.Extr_HighSpd_SupervisorLogic_Trig_ZCE
        [17] = Extraction_B.Extr_Sup_EvSpeedReached;
    Extraction_PrevZCX.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.Extr_HighSpd_SupervisorLogic_Trig_ZCE
        [18] =
        Extraction_B.VariantMergeForOutportExtr_HighSpd_EvFrictionLimitExceeded;

    /* DataStoreWrite: '<S93>/Data Store Write' */
    Extr_HighSpd_CurrPulseInd =
        Extraction_B.VariantMergeForOutportExtr_HighSpd_CurrPulseInd;

    /* DataStoreWrite: '<S93>/Data Store Write1' */
    Extr_HighSpd_State =
        Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.Extr_HighSpd_State_la;

    /* SignalConversion: '<S93>/TargetSpeed' */
    Extr_HighSpd_TargetSpeed =
        Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.Extr_HighSpd_TargetSpeed_ep;

#endif                                 /* EXTR_VARIANT_HIGH_SPEED_PE_DISABLED */

    /* End of Outputs for SubSystem: '<S85>/Extr_HighSpd_SupervisorLogic' */

    /* SignalConversion: '<S85>/OutportBufferForExtr_HighSpd_LoadMassLimitConfig' */
    Extraction_B.OutportBufferForExtr_HighSpd_LoadMassLimitConfig =
        Extraction_B.VariantMergeForOutportExtr_HighSpd_LoadMassLimitConfig;

    /* SignalConversion: '<S85>/OutportBufferForExtr_HighSpd_ParamEstAppSpecificSetup' */
    Extraction_B.OutportBufferForExtr_HighSpd_ParamEstAppSpecificSetup =
        Extraction_B.VariantMergeForOutportExtr_HighSpd_ParamEstAppSpecificSetup;

    /* SignalConversion: '<S85>/OutportBufferForExtr_HighSpd_PrevLoadMassLimitConfig' */
    Extraction_B.OutportBufferForExtr_HighSpd_PrevLoadMassLimitConfig =
        Extraction_B.VariantMergeForOutportExtr_HighSpd_PrevLoadMassLimitConfig;

    /* SignalConversion: '<S85>/OutportBufferForExtr_HighSpd_PrevPulseConfig' */
    Extraction_B.OutportBufferForExtr_HighSpd_PrevPulseConfig =
        Extraction_B.VariantMergeForOutportExtr_HighSpd_PrevPulseConfig;

    /* SignalConversion: '<S85>/OutportBufferForExtr_HighSpd_PulseConfig' */
    Extraction_B.OutportBufferForExtr_HighSpd_PulseConfig =
        Extraction_B.VariantMergeForOutportExtr_HighSpd_PulseConfig;

    /* SignalConversion: '<S85>/OutportBufferForExtr_HighSpd_PrevSpinPulse' */
    Extraction_B.OutportBufferForExtr_HighSpd_PrevSpinPulse =
        Extraction_B.PrevSpinPulse;

    /* SignalConversion: '<S85>/OutportBufferForExtr_HighSpd_SpinPulse' */
    Extraction_B.Extr_HighSpd_SpinPulse = Extraction_B.Selector1;

    /* RootInportFunctionCallGenerator: '<Root>/RootFcnCall_InsertedFor_Extraction__Handler5ms_at_outport_1' incorporates:
     *  SubSystem: '<S10>/Extr_HighSpd_Algorithms'
     */
    /* Chart: '<S68>/TriggerGen' */
    Extraction_TriggerGen(&Extraction_B.sf_TriggerGen_bc,
                          &Extraction_DW.sf_TriggerGen_bc);

    /* UnitDelay: '<S61>/Unit Delay' */
    Extraction_B.UnitDelay = Extraction_DW.UnitDelay_DSTATE;

    /* Outputs for Atomic SubSystem: '<S61>/Extr_HighSpd_LoadMassLimitCheck' */
#if EXTR_VARIANT_HIGH_SPEED_PE_ENABLED

    /* Output and update for atomic system: '<S65>/Extr_HighSpd_LoadMassLimitCheck_Enabled' */
    {
        sint32 inputEventFiredFlag_b;
        sint32 sfEvent_b;
        boolean_T zcEvent_idx_0_b;
        boolean_T zcEvent_idx_1_b;
        boolean_T trigger_b;

        /* Chart: '<S72>/Extr_HighSpd_LoadMassLimitCheckLogic' incorporates:
         *  TriggerPort: '<S73>/input events'
         */
        trigger_b = Extraction_B.sf_TriggerGen_bc.trigger;
        zcEvent_idx_0_b = (sint32)trigger_b !=
            (Extraction_PrevZCX.Extr_HighSpd_LoadMassLimitCheck_Enabled.Extr_HighSpd_LoadMassLimitCheckLogic_Trig_ZCE
             [0] == POS_ZCSIG) &&
            Extraction_PrevZCX.Extr_HighSpd_LoadMassLimitCheck_Enabled.Extr_HighSpd_LoadMassLimitCheckLogic_Trig_ZCE
            [0] != UNINITIALIZED_ZCSIG;
        zcEvent_idx_1_b = (sint32)
            Extraction_B.VariantMergeForOutportExtr_HighSpd_EvStopAll !=
            (Extraction_PrevZCX.Extr_HighSpd_LoadMassLimitCheck_Enabled.Extr_HighSpd_LoadMassLimitCheckLogic_Trig_ZCE
             [1] == POS_ZCSIG) &&
            Extraction_PrevZCX.Extr_HighSpd_LoadMassLimitCheck_Enabled.Extr_HighSpd_LoadMassLimitCheckLogic_Trig_ZCE
            [1] != UNINITIALIZED_ZCSIG;
        if (zcEvent_idx_0_b || zcEvent_idx_1_b)
        {
            /* Gateway: Extraction/Extr_HighSpd/Extr_HighSpd_Algorithms/Extr_HighSpd_LoadMassLimitCheck/Extr_HighSpd_LoadMassLimitCheck_Enabled/Extr_HighSpd_LoadMassLimitCheckLogic */
            inputEventFiredFlag_b = 0;
            if ((sint8)(zcEvent_idx_0_b ? trigger_b ? RISING_ZCEVENT :
                        FALLING_ZCEVENT : NO_ZCEVENT) != 0)
            {
                inputEventFiredFlag_b = 1;
                if (Extraction_DW.Extr_HighSpd_LoadMassLimitCheck_Enabled.temporalCounter_i1
                    < MAX_uint32_T)
                {
                    Extraction_DW.Extr_HighSpd_LoadMassLimitCheck_Enabled.temporalCounter_i1
                        ++;
                }

                /* Event: '<S73>:17' */
                sfEvent_b = Extraction_event_Trigger_cm;
                Extraction_chartstep_c36_Extraction(&sfEvent_b);
            }

            if ((sint8)(zcEvent_idx_1_b ?
                        Extraction_B.VariantMergeForOutportExtr_HighSpd_EvStopAll
                        ? RISING_ZCEVENT : FALLING_ZCEVENT : NO_ZCEVENT) != 0)
            {
                inputEventFiredFlag_b = 1;

                /* Event: '<S73>:36' */
                sfEvent_b = Extraction_event_Extr_HighSpd_EvStopAll_e4;
                Extraction_chartstep_c36_Extraction(&sfEvent_b);
            }

            if (inputEventFiredFlag_b != 0 &&
                    Extraction_DW.Extr_HighSpd_LoadMassLimitCheck_Enabled.Extr_HighSpd_EvLoadMassExceededEventCounter
                > 0U)
            {
                Extraction_B.VariantMergeForOutportExtr_HighSpd_EvLoadMassExceeded
                    =
                    !Extraction_B.VariantMergeForOutportExtr_HighSpd_EvLoadMassExceeded;
                Extraction_DW.Extr_HighSpd_LoadMassLimitCheck_Enabled.Extr_HighSpd_EvLoadMassExceededEventCounter
                    --;
            }
        }

        Extraction_PrevZCX.Extr_HighSpd_LoadMassLimitCheck_Enabled.Extr_HighSpd_LoadMassLimitCheckLogic_Trig_ZCE
            [0] = trigger_b;
        Extraction_PrevZCX.Extr_HighSpd_LoadMassLimitCheck_Enabled.Extr_HighSpd_LoadMassLimitCheckLogic_Trig_ZCE
            [1] = Extraction_B.VariantMergeForOutportExtr_HighSpd_EvStopAll;
    }

#endif                                 /* EXTR_VARIANT_HIGH_SPEED_PE_ENABLED */

    /* End of Outputs for SubSystem: '<S61>/Extr_HighSpd_LoadMassLimitCheck' */

    /* Memory: '<S61>/HighSpdExtUnbCalculated' */
    Extraction_B.HighSpdExtUnbCalculated =
        Extraction_DW.HighSpdExtUnbCalculated_PreviousInput;

    /* Outputs for Atomic SubSystem: '<S61>/Extr_HighSpd_PlateauLogic' */
#if EXTR_VARIANT_HIGH_SPEED_PE_DISABLED

    /* Output and update for atomic system: '<S66>/Extr_HighSpd_PlateauLogic_Disabled' */
    {
        sint32 inputEventFiredFlag_j;
        boolean_T zcEvent_idx_0_j;
        boolean_T zcEvent_idx_1_j;
        boolean_T zcEvent_idx_2_j;
        boolean_T zcEvent_idx_3_j;
        boolean_T zcEvent_idx_4_j;
        boolean_T trigger_j;

        /* Chart: '<S74>/Extr_HighSpd_PlateauLogic' incorporates:
         *  TriggerPort: '<S76>/input events'
         */
        trigger_j = Extraction_B.sf_TriggerGen_bc.trigger;
        zcEvent_idx_0_j = (sint32)trigger_j !=
            (Extraction_PrevZCX.Extr_HighSpd_PlateauLogic_Disabled.Extr_HighSpd_PlateauLogic_Trig_ZCE_lg
             [0] == POS_ZCSIG) &&
            Extraction_PrevZCX.Extr_HighSpd_PlateauLogic_Disabled.Extr_HighSpd_PlateauLogic_Trig_ZCE_lg
            [0] != UNINITIALIZED_ZCSIG;
        zcEvent_idx_1_j = (sint32)
            Extraction_B.VariantMergeForOutportExtr_HighSpd_EvStartPlateau !=
            (Extraction_PrevZCX.Extr_HighSpd_PlateauLogic_Disabled.Extr_HighSpd_PlateauLogic_Trig_ZCE_lg
             [1] == POS_ZCSIG) &&
            Extraction_PrevZCX.Extr_HighSpd_PlateauLogic_Disabled.Extr_HighSpd_PlateauLogic_Trig_ZCE_lg
            [1] != UNINITIALIZED_ZCSIG;
        zcEvent_idx_2_j = (sint32)
            Extraction_B.VariantMergeForOutportExtr_HighSpd_EvStopAll !=
            (Extraction_PrevZCX.Extr_HighSpd_PlateauLogic_Disabled.Extr_HighSpd_PlateauLogic_Trig_ZCE_lg
             [2] == POS_ZCSIG) &&
            Extraction_PrevZCX.Extr_HighSpd_PlateauLogic_Disabled.Extr_HighSpd_PlateauLogic_Trig_ZCE_lg
            [2] != UNINITIALIZED_ZCSIG;
        zcEvent_idx_3_j = (sint32)
            Extraction_B.VariantMergeForOutportExtr_Accel_VibMapEvExcessiveFloorDisp
            !=
            (Extraction_PrevZCX.Extr_HighSpd_PlateauLogic_Disabled.Extr_HighSpd_PlateauLogic_Trig_ZCE_lg
             [3] == POS_ZCSIG) &&
            Extraction_PrevZCX.Extr_HighSpd_PlateauLogic_Disabled.Extr_HighSpd_PlateauLogic_Trig_ZCE_lg
            [3] != UNINITIALIZED_ZCSIG;
        zcEvent_idx_4_j = (sint32)
            Extraction_B.VariantMergeForOutportExtr_HighSpd_EvLoadMassExceeded
            !=
            (Extraction_PrevZCX.Extr_HighSpd_PlateauLogic_Disabled.Extr_HighSpd_PlateauLogic_Trig_ZCE_lg
             [4] == POS_ZCSIG) &&
            Extraction_PrevZCX.Extr_HighSpd_PlateauLogic_Disabled.Extr_HighSpd_PlateauLogic_Trig_ZCE_lg
            [4] != UNINITIALIZED_ZCSIG;
        if (zcEvent_idx_0_j || zcEvent_idx_1_j || zcEvent_idx_2_j ||
                zcEvent_idx_3_j || zcEvent_idx_4_j)
        {
            /* Gateway: Extraction/Extr_HighSpd/Extr_HighSpd_Algorithms/Extr_HighSpd_PlateauLogic/Extr_HighSpd_PlateauLogic_Disabled/Extr_HighSpd_PlateauLogic */
            inputEventFiredFlag_j = 0;
            if ((sint8)(zcEvent_idx_0_j ? trigger_j ? RISING_ZCEVENT :
                        FALLING_ZCEVENT : NO_ZCEVENT) != 0)
            {
                inputEventFiredFlag_j = 1;
                if (Extraction_DW.Extr_HighSpd_PlateauLogic_Disabled.temporalCounter_i1
                    < MAX_uint32_T)
                {
                    Extraction_DW.Extr_HighSpd_PlateauLogic_Disabled.temporalCounter_i1
                        ++;
                }

                /* Event: '<S76>:1' */
                Extraction_DW.Extr_HighSpd_PlateauLogic_Disabled.sfEvent =
                    Extraction_event_ticks_bn;
                Extraction_chartstep_c50_Extraction();
            }

            if ((sint8)(zcEvent_idx_1_j ?
                        Extraction_B.VariantMergeForOutportExtr_HighSpd_EvStartPlateau
                        ? RISING_ZCEVENT : FALLING_ZCEVENT : NO_ZCEVENT) != 0)
            {
                inputEventFiredFlag_j = 1;

                /* Event: '<S76>:132' */
                Extraction_DW.Extr_HighSpd_PlateauLogic_Disabled.sfEvent =
                    Extraction_event_Extr_HighSpd_EvStartPlateau;
                Extraction_chartstep_c50_Extraction();
            }

            if ((sint8)(zcEvent_idx_2_j ?
                        Extraction_B.VariantMergeForOutportExtr_HighSpd_EvStopAll
                        ? RISING_ZCEVENT : FALLING_ZCEVENT : NO_ZCEVENT) != 0)
            {
                inputEventFiredFlag_j = 1;

                /* Event: '<S76>:68' */
                Extraction_DW.Extr_HighSpd_PlateauLogic_Disabled.sfEvent =
                    Extraction_event_Extr_HighSpd_EvStopAll_d3;
                Extraction_chartstep_c50_Extraction();
            }

            if ((sint8)(zcEvent_idx_3_j ?
                        Extraction_B.VariantMergeForOutportExtr_Accel_VibMapEvExcessiveFloorDisp
                        ? RISING_ZCEVENT : FALLING_ZCEVENT : NO_ZCEVENT) != 0)
            {
                inputEventFiredFlag_j = 1;

                /* Event: '<S76>:205' */
                Extraction_DW.Extr_HighSpd_PlateauLogic_Disabled.sfEvent =
                    Extraction_event_Extr_Accel_VibMapEvExcessiveFloorDisp_h3;
                Extraction_chartstep_c50_Extraction();
            }

            if ((sint8)(zcEvent_idx_4_j ?
                        Extraction_B.VariantMergeForOutportExtr_HighSpd_EvLoadMassExceeded
                        ? RISING_ZCEVENT : FALLING_ZCEVENT : NO_ZCEVENT) != 0)
            {
                inputEventFiredFlag_j = 1;

                /* Event: '<S76>:216' */
                Extraction_DW.Extr_HighSpd_PlateauLogic_Disabled.sfEvent =
                    Extraction_event_Extr_HighSpd_EvLoadMassExceeded_in;
                Extraction_chartstep_c50_Extraction();
            }

            if (inputEventFiredFlag_j != 0 &&
                    Extraction_DW.Extr_HighSpd_PlateauLogic_Disabled.Extr_HighSpd_EvPlateauCompleteEventCounter
                > 0U)
            {
                Extraction_B.VariantMergeForOutportExtr_HighSpd_EvPlateauComplete
                    =
                    !Extraction_B.VariantMergeForOutportExtr_HighSpd_EvPlateauComplete;
                Extraction_DW.Extr_HighSpd_PlateauLogic_Disabled.Extr_HighSpd_EvPlateauCompleteEventCounter
                    --;
            }
        }

        Extraction_PrevZCX.Extr_HighSpd_PlateauLogic_Disabled.Extr_HighSpd_PlateauLogic_Trig_ZCE_lg
            [0] = trigger_j;
        Extraction_PrevZCX.Extr_HighSpd_PlateauLogic_Disabled.Extr_HighSpd_PlateauLogic_Trig_ZCE_lg
            [1] = Extraction_B.VariantMergeForOutportExtr_HighSpd_EvStartPlateau;
        Extraction_PrevZCX.Extr_HighSpd_PlateauLogic_Disabled.Extr_HighSpd_PlateauLogic_Trig_ZCE_lg
            [2] = Extraction_B.VariantMergeForOutportExtr_HighSpd_EvStopAll;
        Extraction_PrevZCX.Extr_HighSpd_PlateauLogic_Disabled.Extr_HighSpd_PlateauLogic_Trig_ZCE_lg
            [3] =
            Extraction_B.VariantMergeForOutportExtr_Accel_VibMapEvExcessiveFloorDisp;
        Extraction_PrevZCX.Extr_HighSpd_PlateauLogic_Disabled.Extr_HighSpd_PlateauLogic_Trig_ZCE_lg
            [4] =
            Extraction_B.VariantMergeForOutportExtr_HighSpd_EvLoadMassExceeded;

        /* SignalConversion: '<S74>/PlateauDecision' */
        Extr_HighSpd_PlateauDecision =
            Extraction_B.Extr_HighSpd_PlateauLogic_Disabled.Extr_HighSpd_PlateauDecision_m0;
    }

#elif EXTR_VARIANT_HIGH_SPEED_PE_ENABLED

    /* Output and update for atomic system: '<S66>/Extr_HighSpd_PlateauLogic_Enabled' */
    {
        sint32 inputEventFiredFlag_g;
        boolean_T zcEvent_idx_0_g;
        boolean_T zcEvent_idx_1_g;
        boolean_T zcEvent_idx_2_g;
        boolean_T zcEvent_idx_3_g;
        boolean_T zcEvent_idx_4_g;
        boolean_T trigger_g;

        /* Chart: '<S75>/Extr_HighSpd_PlateauLogic' incorporates:
         *  TriggerPort: '<S77>/input events'
         */
        trigger_g = Extraction_B.sf_TriggerGen_bc.trigger;
        zcEvent_idx_0_g = (sint32)trigger_g !=
            (Extraction_PrevZCX.Extr_HighSpd_PlateauLogic_Enabled.Extr_HighSpd_PlateauLogic_Trig_ZCE
             [0] == POS_ZCSIG) &&
            Extraction_PrevZCX.Extr_HighSpd_PlateauLogic_Enabled.Extr_HighSpd_PlateauLogic_Trig_ZCE
            [0] != UNINITIALIZED_ZCSIG;
        zcEvent_idx_1_g = (sint32)
            Extraction_B.VariantMergeForOutportExtr_HighSpd_EvStartPlateau !=
            (Extraction_PrevZCX.Extr_HighSpd_PlateauLogic_Enabled.Extr_HighSpd_PlateauLogic_Trig_ZCE
             [1] == POS_ZCSIG) &&
            Extraction_PrevZCX.Extr_HighSpd_PlateauLogic_Enabled.Extr_HighSpd_PlateauLogic_Trig_ZCE
            [1] != UNINITIALIZED_ZCSIG;
        zcEvent_idx_2_g = (sint32)
            Extraction_B.VariantMergeForOutportExtr_HighSpd_EvStopAll !=
            (Extraction_PrevZCX.Extr_HighSpd_PlateauLogic_Enabled.Extr_HighSpd_PlateauLogic_Trig_ZCE
             [2] == POS_ZCSIG) &&
            Extraction_PrevZCX.Extr_HighSpd_PlateauLogic_Enabled.Extr_HighSpd_PlateauLogic_Trig_ZCE
            [2] != UNINITIALIZED_ZCSIG;
        zcEvent_idx_3_g = (sint32)
            Extraction_B.VariantMergeForOutportExtr_Accel_VibMapEvExcessiveFloorDisp
            !=
            (Extraction_PrevZCX.Extr_HighSpd_PlateauLogic_Enabled.Extr_HighSpd_PlateauLogic_Trig_ZCE
             [3] == POS_ZCSIG) &&
            Extraction_PrevZCX.Extr_HighSpd_PlateauLogic_Enabled.Extr_HighSpd_PlateauLogic_Trig_ZCE
            [3] != UNINITIALIZED_ZCSIG;
        zcEvent_idx_4_g = (sint32)
            Extraction_B.VariantMergeForOutportExtr_HighSpd_EvLoadMassExceeded
            !=
            (Extraction_PrevZCX.Extr_HighSpd_PlateauLogic_Enabled.Extr_HighSpd_PlateauLogic_Trig_ZCE
             [4] == POS_ZCSIG) &&
            Extraction_PrevZCX.Extr_HighSpd_PlateauLogic_Enabled.Extr_HighSpd_PlateauLogic_Trig_ZCE
            [4] != UNINITIALIZED_ZCSIG;
        if (zcEvent_idx_0_g || zcEvent_idx_1_g || zcEvent_idx_2_g ||
                zcEvent_idx_3_g || zcEvent_idx_4_g)
        {
            /* Gateway: Extraction/Extr_HighSpd/Extr_HighSpd_Algorithms/Extr_HighSpd_PlateauLogic/Extr_HighSpd_PlateauLogic_Enabled/Extr_HighSpd_PlateauLogic */
            inputEventFiredFlag_g = 0;
            if ((sint8)(zcEvent_idx_0_g ? trigger_g ? RISING_ZCEVENT :
                        FALLING_ZCEVENT : NO_ZCEVENT) != 0)
            {
                inputEventFiredFlag_g = 1;
                if (Extraction_DW.Extr_HighSpd_PlateauLogic_Enabled.temporalCounter_i1
                    < MAX_uint32_T)
                {
                    Extraction_DW.Extr_HighSpd_PlateauLogic_Enabled.temporalCounter_i1
                        ++;
                }

                if (Extraction_DW.Extr_HighSpd_PlateauLogic_Enabled.temporalCounter_i2
                    < MAX_uint32_T)
                {
                    Extraction_DW.Extr_HighSpd_PlateauLogic_Enabled.temporalCounter_i2
                        ++;
                }

                if (Extraction_DW.Extr_HighSpd_PlateauLogic_Enabled.temporalCounter_i3
                    < MAX_uint32_T)
                {
                    Extraction_DW.Extr_HighSpd_PlateauLogic_Enabled.temporalCounter_i3
                        ++;
                }

                /* Event: '<S77>:1' */
                Extraction_DW.Extr_HighSpd_PlateauLogic_Enabled.sfEvent =
                    Extraction_event_ticks_pn;
                Extraction_chartstep_c3_Extraction();
            }

            if ((sint8)(zcEvent_idx_1_g ?
                        Extraction_B.VariantMergeForOutportExtr_HighSpd_EvStartPlateau
                        ? RISING_ZCEVENT : FALLING_ZCEVENT : NO_ZCEVENT) != 0)
            {
                inputEventFiredFlag_g = 1;

                /* Event: '<S77>:132' */
                Extraction_DW.Extr_HighSpd_PlateauLogic_Enabled.sfEvent =
                    Extraction_event_Extr_HighSpd_EvStartPlateau_i3;
                Extraction_chartstep_c3_Extraction();
            }

            if ((sint8)(zcEvent_idx_2_g ?
                        Extraction_B.VariantMergeForOutportExtr_HighSpd_EvStopAll
                        ? RISING_ZCEVENT : FALLING_ZCEVENT : NO_ZCEVENT) != 0)
            {
                inputEventFiredFlag_g = 1;

                /* Event: '<S77>:68' */
                Extraction_DW.Extr_HighSpd_PlateauLogic_Enabled.sfEvent =
                    Extraction_event_Extr_HighSpd_EvStopAll_bu;
                Extraction_chartstep_c3_Extraction();
            }

            if ((sint8)(zcEvent_idx_3_g ?
                        Extraction_B.VariantMergeForOutportExtr_Accel_VibMapEvExcessiveFloorDisp
                        ? RISING_ZCEVENT : FALLING_ZCEVENT : NO_ZCEVENT) != 0)
            {
                inputEventFiredFlag_g = 1;

                /* Event: '<S77>:205' */
                Extraction_DW.Extr_HighSpd_PlateauLogic_Enabled.sfEvent =
                    Extraction_event_Extr_Accel_VibMapEvExcessiveFloorDisp_af;
                Extraction_chartstep_c3_Extraction();
            }

            if ((sint8)(zcEvent_idx_4_g ?
                        Extraction_B.VariantMergeForOutportExtr_HighSpd_EvLoadMassExceeded
                        ? RISING_ZCEVENT : FALLING_ZCEVENT : NO_ZCEVENT) != 0)
            {
                inputEventFiredFlag_g = 1;

                /* Event: '<S77>:216' */
                Extraction_DW.Extr_HighSpd_PlateauLogic_Enabled.sfEvent =
                    Extraction_event_Extr_HighSpd_EvLoadMassExceeded_js;
                Extraction_chartstep_c3_Extraction();
            }

            if (inputEventFiredFlag_g != 0 &&
                    Extraction_DW.Extr_HighSpd_PlateauLogic_Enabled.Extr_HighSpd_EvPlateauCompleteEventCounter
                > 0U)
            {
                Extraction_B.VariantMergeForOutportExtr_HighSpd_EvPlateauComplete
                    =
                    !Extraction_B.VariantMergeForOutportExtr_HighSpd_EvPlateauComplete;
                Extraction_DW.Extr_HighSpd_PlateauLogic_Enabled.Extr_HighSpd_EvPlateauCompleteEventCounter
                    --;
            }
        }

        Extraction_PrevZCX.Extr_HighSpd_PlateauLogic_Enabled.Extr_HighSpd_PlateauLogic_Trig_ZCE
            [0] = trigger_g;
        Extraction_PrevZCX.Extr_HighSpd_PlateauLogic_Enabled.Extr_HighSpd_PlateauLogic_Trig_ZCE
            [1] = Extraction_B.VariantMergeForOutportExtr_HighSpd_EvStartPlateau;
        Extraction_PrevZCX.Extr_HighSpd_PlateauLogic_Enabled.Extr_HighSpd_PlateauLogic_Trig_ZCE
            [2] = Extraction_B.VariantMergeForOutportExtr_HighSpd_EvStopAll;
        Extraction_PrevZCX.Extr_HighSpd_PlateauLogic_Enabled.Extr_HighSpd_PlateauLogic_Trig_ZCE
            [3] =
            Extraction_B.VariantMergeForOutportExtr_Accel_VibMapEvExcessiveFloorDisp;
        Extraction_PrevZCX.Extr_HighSpd_PlateauLogic_Enabled.Extr_HighSpd_PlateauLogic_Trig_ZCE
            [4] =
            Extraction_B.VariantMergeForOutportExtr_HighSpd_EvLoadMassExceeded;

        /* SignalConversion: '<S75>/PlateauDecision' */
        Extr_HighSpd_PlateauDecision =
            Extraction_B.Extr_HighSpd_PlateauLogic_Enabled.Extr_HighSpd_PlateauDecision_pf;
    }

#endif                                 /* EXTR_VARIANT_HIGH_SPEED_PE_DISABLED */

    /* End of Outputs for SubSystem: '<S61>/Extr_HighSpd_PlateauLogic' */

    /* Outputs for Atomic SubSystem: '<S61>/Extr_HighSpd_RampLogic' */
#if EXTR_VARIANT_HIGH_SPEED_PE_DISABLED

    /* Output and update for atomic system: '<S67>/Extr_HighSpd_RampLogic_Disabled' */
    {
        sint32 inputEventFiredFlag_a;
        sint32 sfEvent_a;
        boolean_T zcEvent_idx_0_a;
        boolean_T zcEvent_idx_1_a;
        boolean_T zcEvent_idx_2_a;
        boolean_T zcEvent_idx_3_a;
        boolean_T zcEvent_idx_4_a;
        boolean_T zcEvent_idx_5_a;
        boolean_T trigger_a;

        /* Chart: '<S79>/Extr_HighSpd_RampLogic' incorporates:
         *  TriggerPort: '<S81>/input events'
         */
        trigger_a = Extraction_B.sf_TriggerGen_bc.trigger;
        zcEvent_idx_0_a = (sint32)trigger_a !=
            (Extraction_PrevZCX.Extr_HighSpd_RampLogic_Disabled.Extr_HighSpd_RampLogic_Trig_ZCE_ir
             [0] == POS_ZCSIG) &&
            Extraction_PrevZCX.Extr_HighSpd_RampLogic_Disabled.Extr_HighSpd_RampLogic_Trig_ZCE_ir
            [0] != UNINITIALIZED_ZCSIG;
        zcEvent_idx_1_a = (sint32)
            Extraction_B.VariantMergeForOutportExtr_HighSpd_EvStartRamp !=
            (Extraction_PrevZCX.Extr_HighSpd_RampLogic_Disabled.Extr_HighSpd_RampLogic_Trig_ZCE_ir
             [1] == POS_ZCSIG) &&
            Extraction_PrevZCX.Extr_HighSpd_RampLogic_Disabled.Extr_HighSpd_RampLogic_Trig_ZCE_ir
            [1] != UNINITIALIZED_ZCSIG;
        zcEvent_idx_2_a = (sint32)
            Extraction_B.VariantMergeForOutportExtr_HighSpd_EvStopAll !=
            (Extraction_PrevZCX.Extr_HighSpd_RampLogic_Disabled.Extr_HighSpd_RampLogic_Trig_ZCE_ir
             [2] == POS_ZCSIG) &&
            Extraction_PrevZCX.Extr_HighSpd_RampLogic_Disabled.Extr_HighSpd_RampLogic_Trig_ZCE_ir
            [2] != UNINITIALIZED_ZCSIG;
        zcEvent_idx_3_a = (sint32)
            Extraction_B.VariantMergeForOutportExtr_HighSpd_EvLoadMassExceeded
            !=
            (Extraction_PrevZCX.Extr_HighSpd_RampLogic_Disabled.Extr_HighSpd_RampLogic_Trig_ZCE_ir
             [3] == POS_ZCSIG) &&
            Extraction_PrevZCX.Extr_HighSpd_RampLogic_Disabled.Extr_HighSpd_RampLogic_Trig_ZCE_ir
            [3] != UNINITIALIZED_ZCSIG;
        zcEvent_idx_4_a = (sint32)Extraction_B.Extr_Sup_EvSpeedErrorTimeout !=
            (Extraction_PrevZCX.Extr_HighSpd_RampLogic_Disabled.Extr_HighSpd_RampLogic_Trig_ZCE_ir
             [4] == POS_ZCSIG) &&
            Extraction_PrevZCX.Extr_HighSpd_RampLogic_Disabled.Extr_HighSpd_RampLogic_Trig_ZCE_ir
            [4] != UNINITIALIZED_ZCSIG;
        zcEvent_idx_5_a = (sint32)Extraction_B.Extr_Sup_EvSpeedReached !=
            (Extraction_PrevZCX.Extr_HighSpd_RampLogic_Disabled.Extr_HighSpd_RampLogic_Trig_ZCE_ir
             [5] == POS_ZCSIG) &&
            Extraction_PrevZCX.Extr_HighSpd_RampLogic_Disabled.Extr_HighSpd_RampLogic_Trig_ZCE_ir
            [5] != UNINITIALIZED_ZCSIG;
        if (zcEvent_idx_0_a || zcEvent_idx_1_a || zcEvent_idx_2_a ||
                zcEvent_idx_3_a || zcEvent_idx_4_a || zcEvent_idx_5_a)
        {
            /* Gateway: Extraction/Extr_HighSpd/Extr_HighSpd_Algorithms/Extr_HighSpd_RampLogic/Extr_HighSpd_RampLogic_Disabled/Extr_HighSpd_RampLogic */
            inputEventFiredFlag_a = 0;
            if ((sint8)(zcEvent_idx_0_a ? trigger_a ? RISING_ZCEVENT :
                        FALLING_ZCEVENT : NO_ZCEVENT) != 0)
            {
                inputEventFiredFlag_a = 1;

                /* Event: '<S81>:1' */
                sfEvent_a = Extraction_event_ticks_gd;
                Extraction_chartstep_c49_Extraction(&sfEvent_a);
            }

            if ((sint8)(zcEvent_idx_1_a ?
                        Extraction_B.VariantMergeForOutportExtr_HighSpd_EvStartRamp
                        ? RISING_ZCEVENT : FALLING_ZCEVENT : NO_ZCEVENT) != 0)
            {
                inputEventFiredFlag_a = 1;

                /* Event: '<S81>:2' */
                sfEvent_a = Extraction_event_Extr_HighSpd_EvStartRamp;
                Extraction_chartstep_c49_Extraction(&sfEvent_a);
            }

            if ((sint8)(zcEvent_idx_2_a ?
                        Extraction_B.VariantMergeForOutportExtr_HighSpd_EvStopAll
                        ? RISING_ZCEVENT : FALLING_ZCEVENT : NO_ZCEVENT) != 0)
            {
                inputEventFiredFlag_a = 1;

                /* Event: '<S81>:68' */
                sfEvent_a = Extraction_event_Extr_HighSpd_EvStopAll_du;
                Extraction_chartstep_c49_Extraction(&sfEvent_a);
            }

            if ((sint8)(zcEvent_idx_3_a ?
                        Extraction_B.VariantMergeForOutportExtr_HighSpd_EvLoadMassExceeded
                        ? RISING_ZCEVENT : FALLING_ZCEVENT : NO_ZCEVENT) != 0)
            {
                inputEventFiredFlag_a = 1;

                /* Event: '<S81>:95' */
                sfEvent_a = Extraction_event_Extr_HighSpd_EvLoadMassExceeded_oi;
                Extraction_chartstep_c49_Extraction(&sfEvent_a);
            }

            if ((sint8)(zcEvent_idx_4_a ?
                        Extraction_B.Extr_Sup_EvSpeedErrorTimeout ?
                        RISING_ZCEVENT : FALLING_ZCEVENT : NO_ZCEVENT) != 0)
            {
                inputEventFiredFlag_a = 1;

                /* Event: '<S81>:118' */
                sfEvent_a = Extraction_event_Extr_Sup_EvSpeedErrorTimeout_p2;
                Extraction_chartstep_c49_Extraction(&sfEvent_a);
            }

            if ((sint8)(zcEvent_idx_5_a ? Extraction_B.Extr_Sup_EvSpeedReached ?
                        RISING_ZCEVENT : FALLING_ZCEVENT : NO_ZCEVENT) != 0)
            {
                inputEventFiredFlag_a = 1;

                /* Event: '<S81>:119' */
                sfEvent_a = Extraction_event_Extr_Sup_EvSpeedReached_c4;
                Extraction_chartstep_c49_Extraction(&sfEvent_a);
            }

            if (inputEventFiredFlag_a != 0 &&
                    Extraction_DW.Extr_HighSpd_RampLogic_Disabled.Extr_HighSpd_EvRampCompleteEventCounter
                > 0U)
            {
                Extraction_B.VariantMergeForOutportExtr_HighSpd_EvRampComplete =
                    !Extraction_B.VariantMergeForOutportExtr_HighSpd_EvRampComplete;
                Extraction_DW.Extr_HighSpd_RampLogic_Disabled.Extr_HighSpd_EvRampCompleteEventCounter
                    --;
            }
        }

        Extraction_PrevZCX.Extr_HighSpd_RampLogic_Disabled.Extr_HighSpd_RampLogic_Trig_ZCE_ir
            [0] = trigger_a;
        Extraction_PrevZCX.Extr_HighSpd_RampLogic_Disabled.Extr_HighSpd_RampLogic_Trig_ZCE_ir
            [1] = Extraction_B.VariantMergeForOutportExtr_HighSpd_EvStartRamp;
        Extraction_PrevZCX.Extr_HighSpd_RampLogic_Disabled.Extr_HighSpd_RampLogic_Trig_ZCE_ir
            [2] = Extraction_B.VariantMergeForOutportExtr_HighSpd_EvStopAll;
        Extraction_PrevZCX.Extr_HighSpd_RampLogic_Disabled.Extr_HighSpd_RampLogic_Trig_ZCE_ir
            [3] =
            Extraction_B.VariantMergeForOutportExtr_HighSpd_EvLoadMassExceeded;
        Extraction_PrevZCX.Extr_HighSpd_RampLogic_Disabled.Extr_HighSpd_RampLogic_Trig_ZCE_ir
            [4] = Extraction_B.Extr_Sup_EvSpeedErrorTimeout;
        Extraction_PrevZCX.Extr_HighSpd_RampLogic_Disabled.Extr_HighSpd_RampLogic_Trig_ZCE_ir
            [5] = Extraction_B.Extr_Sup_EvSpeedReached;
    }

#elif EXTR_VARIANT_HIGH_SPEED_PE_ENABLED

    /* Output and update for atomic system: '<S67>/Extr_HighSpd_RampLogic_Enabled' */
    {
        sint32 inputEventFiredFlag_l;
        sint32 sfEvent_l;
        boolean_T zcEvent_idx_0_l;
        boolean_T zcEvent_idx_1_l;
        boolean_T zcEvent_idx_2_l;
        boolean_T zcEvent_idx_3_l;
        boolean_T zcEvent_idx_4_l;
        boolean_T zcEvent_idx_5_l;
        boolean_T trigger_l;

        /* Chart: '<S80>/Extr_HighSpd_RampLogic' incorporates:
         *  TriggerPort: '<S82>/input events'
         */
        trigger_l = Extraction_B.sf_TriggerGen_bc.trigger;
        zcEvent_idx_0_l = (sint32)trigger_l !=
            (Extraction_PrevZCX.Extr_HighSpd_RampLogic_Enabled.Extr_HighSpd_RampLogic_Trig_ZCE
             [0] == POS_ZCSIG) &&
            Extraction_PrevZCX.Extr_HighSpd_RampLogic_Enabled.Extr_HighSpd_RampLogic_Trig_ZCE
            [0] != UNINITIALIZED_ZCSIG;
        zcEvent_idx_1_l = (sint32)
            Extraction_B.VariantMergeForOutportExtr_HighSpd_EvStartRamp !=
            (Extraction_PrevZCX.Extr_HighSpd_RampLogic_Enabled.Extr_HighSpd_RampLogic_Trig_ZCE
             [1] == POS_ZCSIG) &&
            Extraction_PrevZCX.Extr_HighSpd_RampLogic_Enabled.Extr_HighSpd_RampLogic_Trig_ZCE
            [1] != UNINITIALIZED_ZCSIG;
        zcEvent_idx_2_l = (sint32)
            Extraction_B.VariantMergeForOutportExtr_HighSpd_EvStopAll !=
            (Extraction_PrevZCX.Extr_HighSpd_RampLogic_Enabled.Extr_HighSpd_RampLogic_Trig_ZCE
             [2] == POS_ZCSIG) &&
            Extraction_PrevZCX.Extr_HighSpd_RampLogic_Enabled.Extr_HighSpd_RampLogic_Trig_ZCE
            [2] != UNINITIALIZED_ZCSIG;
        zcEvent_idx_3_l = (sint32)
            Extraction_B.VariantMergeForOutportExtr_HighSpd_EvLoadMassExceeded
            !=
            (Extraction_PrevZCX.Extr_HighSpd_RampLogic_Enabled.Extr_HighSpd_RampLogic_Trig_ZCE
             [3] == POS_ZCSIG) &&
            Extraction_PrevZCX.Extr_HighSpd_RampLogic_Enabled.Extr_HighSpd_RampLogic_Trig_ZCE
            [3] != UNINITIALIZED_ZCSIG;
        zcEvent_idx_4_l = (sint32)Extraction_B.Extr_Sup_EvSpeedErrorTimeout !=
            (Extraction_PrevZCX.Extr_HighSpd_RampLogic_Enabled.Extr_HighSpd_RampLogic_Trig_ZCE
             [4] == POS_ZCSIG) &&
            Extraction_PrevZCX.Extr_HighSpd_RampLogic_Enabled.Extr_HighSpd_RampLogic_Trig_ZCE
            [4] != UNINITIALIZED_ZCSIG;
        zcEvent_idx_5_l = (sint32)Extraction_B.Extr_Sup_EvSpeedReached !=
            (Extraction_PrevZCX.Extr_HighSpd_RampLogic_Enabled.Extr_HighSpd_RampLogic_Trig_ZCE
             [5] == POS_ZCSIG) &&
            Extraction_PrevZCX.Extr_HighSpd_RampLogic_Enabled.Extr_HighSpd_RampLogic_Trig_ZCE
            [5] != UNINITIALIZED_ZCSIG;
        if (zcEvent_idx_0_l || zcEvent_idx_1_l || zcEvent_idx_2_l ||
                zcEvent_idx_3_l || zcEvent_idx_4_l || zcEvent_idx_5_l)
        {
            /* Gateway: Extraction/Extr_HighSpd/Extr_HighSpd_Algorithms/Extr_HighSpd_RampLogic/Extr_HighSpd_RampLogic_Enabled/Extr_HighSpd_RampLogic */
            inputEventFiredFlag_l = 0;
            if ((sint8)(zcEvent_idx_0_l ? trigger_l ? RISING_ZCEVENT :
                        FALLING_ZCEVENT : NO_ZCEVENT) != 0)
            {
                inputEventFiredFlag_l = 1;
                if (Extraction_DW.Extr_HighSpd_RampLogic_Enabled.temporalCounter_i1
                    < MAX_uint32_T)
                {
                    Extraction_DW.Extr_HighSpd_RampLogic_Enabled.temporalCounter_i1
                        ++;
                }

                /* Event: '<S82>:1' */
                sfEvent_l = Extraction_event_ticks_eh;
                Extraction_chartstep_c8_Extraction(&sfEvent_l);
            }

            if ((sint8)(zcEvent_idx_1_l ?
                        Extraction_B.VariantMergeForOutportExtr_HighSpd_EvStartRamp
                        ? RISING_ZCEVENT : FALLING_ZCEVENT : NO_ZCEVENT) != 0)
            {
                inputEventFiredFlag_l = 1;

                /* Event: '<S82>:2' */
                sfEvent_l = Extraction_event_Extr_HighSpd_EvStartRamp_oc;
                Extraction_chartstep_c8_Extraction(&sfEvent_l);
            }

            if ((sint8)(zcEvent_idx_2_l ?
                        Extraction_B.VariantMergeForOutportExtr_HighSpd_EvStopAll
                        ? RISING_ZCEVENT : FALLING_ZCEVENT : NO_ZCEVENT) != 0)
            {
                inputEventFiredFlag_l = 1;

                /* Event: '<S82>:68' */
                sfEvent_l = Extraction_event_Extr_HighSpd_EvStopAll_gt;
                Extraction_chartstep_c8_Extraction(&sfEvent_l);
            }

            if ((sint8)(zcEvent_idx_3_l ?
                        Extraction_B.VariantMergeForOutportExtr_HighSpd_EvLoadMassExceeded
                        ? RISING_ZCEVENT : FALLING_ZCEVENT : NO_ZCEVENT) != 0)
            {
                inputEventFiredFlag_l = 1;

                /* Event: '<S82>:95' */
                sfEvent_l = Extraction_event_Extr_HighSpd_EvLoadMassExceeded_ef;
                Extraction_chartstep_c8_Extraction(&sfEvent_l);
            }

            if ((sint8)(zcEvent_idx_4_l ?
                        Extraction_B.Extr_Sup_EvSpeedErrorTimeout ?
                        RISING_ZCEVENT : FALLING_ZCEVENT : NO_ZCEVENT) != 0)
            {
                inputEventFiredFlag_l = 1;

                /* Event: '<S82>:130' */
                sfEvent_l = Extraction_event_Extr_Sup_EvSpeedErrorTimeout_nh;
                Extraction_chartstep_c8_Extraction(&sfEvent_l);
            }

            if ((sint8)(zcEvent_idx_5_l ? Extraction_B.Extr_Sup_EvSpeedReached ?
                        RISING_ZCEVENT : FALLING_ZCEVENT : NO_ZCEVENT) != 0)
            {
                inputEventFiredFlag_l = 1;

                /* Event: '<S82>:131' */
                sfEvent_l = Extraction_event_Extr_Sup_EvSpeedReached_ix;
                Extraction_chartstep_c8_Extraction(&sfEvent_l);
            }

            if (inputEventFiredFlag_l != 0 &&
                    Extraction_DW.Extr_HighSpd_RampLogic_Enabled.Extr_HighSpd_EvRampCompleteEventCounter
                > 0U)
            {
                Extraction_B.VariantMergeForOutportExtr_HighSpd_EvRampComplete =
                    !Extraction_B.VariantMergeForOutportExtr_HighSpd_EvRampComplete;
                Extraction_DW.Extr_HighSpd_RampLogic_Enabled.Extr_HighSpd_EvRampCompleteEventCounter
                    --;
            }

            if (inputEventFiredFlag_l != 0 &&
                    Extraction_DW.Extr_HighSpd_RampLogic_Enabled.Extr_HighSpd_EvViscousLimitExceededEventCounter
                > 0U)
            {
                Extraction_B.VariantMergeForOutportExtr_HighSpd_EvViscousLimitExceeded
                    =
                    !Extraction_B.VariantMergeForOutportExtr_HighSpd_EvViscousLimitExceeded;
                Extraction_DW.Extr_HighSpd_RampLogic_Enabled.Extr_HighSpd_EvViscousLimitExceededEventCounter
                    --;
            }

            if (inputEventFiredFlag_l != 0 &&
                    Extraction_DW.Extr_HighSpd_RampLogic_Enabled.Extr_HighSpd_EvFrictionLimitExceededEventCounter
                > 0U)
            {
                Extraction_B.VariantMergeForOutportExtr_HighSpd_EvFrictionLimitExceeded
                    =
                    !Extraction_B.VariantMergeForOutportExtr_HighSpd_EvFrictionLimitExceeded;
                Extraction_DW.Extr_HighSpd_RampLogic_Enabled.Extr_HighSpd_EvFrictionLimitExceededEventCounter
                    --;
            }
        }

        Extraction_PrevZCX.Extr_HighSpd_RampLogic_Enabled.Extr_HighSpd_RampLogic_Trig_ZCE
            [0] = trigger_l;
        Extraction_PrevZCX.Extr_HighSpd_RampLogic_Enabled.Extr_HighSpd_RampLogic_Trig_ZCE
            [1] = Extraction_B.VariantMergeForOutportExtr_HighSpd_EvStartRamp;
        Extraction_PrevZCX.Extr_HighSpd_RampLogic_Enabled.Extr_HighSpd_RampLogic_Trig_ZCE
            [2] = Extraction_B.VariantMergeForOutportExtr_HighSpd_EvStopAll;
        Extraction_PrevZCX.Extr_HighSpd_RampLogic_Enabled.Extr_HighSpd_RampLogic_Trig_ZCE
            [3] =
            Extraction_B.VariantMergeForOutportExtr_HighSpd_EvLoadMassExceeded;
        Extraction_PrevZCX.Extr_HighSpd_RampLogic_Enabled.Extr_HighSpd_RampLogic_Trig_ZCE
            [4] = Extraction_B.Extr_Sup_EvSpeedErrorTimeout;
        Extraction_PrevZCX.Extr_HighSpd_RampLogic_Enabled.Extr_HighSpd_RampLogic_Trig_ZCE
            [5] = Extraction_B.Extr_Sup_EvSpeedReached;
    }

#endif                                 /* EXTR_VARIANT_HIGH_SPEED_PE_DISABLED */

    /* End of Outputs for SubSystem: '<S61>/Extr_HighSpd_RampLogic' */

    /* Logic: '<S61>/EnableCEM' */
    Extraction_B.Extr_HighSpd_EnableCEM =
        Extraction_B.VariantMergeForOutportExtr_HighSpd_EnableCEM_j4 ||
        Extraction_B.VariantMergeForOutportExtr_HighSpd_EnableCEM;

    /* Logic: '<S61>/EnableLoadMassLimitCheck' */
    Extraction_DW.UnitDelay_DSTATE =
        Extraction_B.VariantMergeForOutportExtr_HighSpd_EnableLoadMassLimitCheck_hb
        ||
        Extraction_B.VariantMergeForOutportExtr_HighSpd_EnableLoadMassLimitCheck;

    /* Outputs for Atomic SubSystem: '<S61>/Extr_HighSpd_CEM' */
#if EXTR_VARIANT_HIGH_SPEED_PE_ENABLED

    /* Output and update for atomic system: '<S63>/Extr_HighSpd_CEM_Enabled' */
    {
        sint32 inputEventFiredFlag_p;
        boolean_T zcEvent_idx_0_p;
        boolean_T zcEvent_idx_1_p;
        boolean_T trigger_p;

        /* Chart: '<S69>/Extr_HighSpd_CEMLogic' incorporates:
         *  TriggerPort: '<S70>/input events'
         */
        trigger_p = Extraction_B.sf_TriggerGen_bc.trigger;
        zcEvent_idx_0_p = (sint32)trigger_p !=
            (Extraction_PrevZCX.Extr_HighSpd_CEM_Enabled.Extr_HighSpd_CEMLogic_Trig_ZCE
             [0] == POS_ZCSIG) &&
            Extraction_PrevZCX.Extr_HighSpd_CEM_Enabled.Extr_HighSpd_CEMLogic_Trig_ZCE
            [0] != UNINITIALIZED_ZCSIG;
        zcEvent_idx_1_p = (sint32)
            Extraction_B.VariantMergeForOutportExtr_HighSpd_EvStopAll !=
            (Extraction_PrevZCX.Extr_HighSpd_CEM_Enabled.Extr_HighSpd_CEMLogic_Trig_ZCE
             [1] == POS_ZCSIG) &&
            Extraction_PrevZCX.Extr_HighSpd_CEM_Enabled.Extr_HighSpd_CEMLogic_Trig_ZCE
            [1] != UNINITIALIZED_ZCSIG;
        if (zcEvent_idx_0_p || zcEvent_idx_1_p)
        {
            /* Gateway: Extraction/Extr_HighSpd/Extr_HighSpd_Algorithms/Extr_HighSpd_CEM/Extr_HighSpd_CEM_Enabled/Extr_HighSpd_CEMLogic */
            inputEventFiredFlag_p = 0;
            if ((sint8)(zcEvent_idx_0_p ? trigger_p ? RISING_ZCEVENT :
                        FALLING_ZCEVENT : NO_ZCEVENT) != 0)
            {
                inputEventFiredFlag_p = 1;
                if (Extraction_DW.Extr_HighSpd_CEM_Enabled.temporalCounter_i1 <
                        65535U)
                {
                    Extraction_DW.Extr_HighSpd_CEM_Enabled.temporalCounter_i1++;
                }

                /* Event: '<S70>:1' */
                Extraction_DW.Extr_HighSpd_CEM_Enabled.sfEvent =
                    Extraction_event_ticks_nt;
                Extraction_chartstep_c21_Extraction();
            }

            if ((sint8)(zcEvent_idx_1_p ?
                        Extraction_B.VariantMergeForOutportExtr_HighSpd_EvStopAll
                        ? RISING_ZCEVENT : FALLING_ZCEVENT : NO_ZCEVENT) != 0)
            {
                inputEventFiredFlag_p = 1;

                /* Event: '<S70>:68' */
                Extraction_DW.Extr_HighSpd_CEM_Enabled.sfEvent =
                    Extraction_event_Extr_HighSpd_EvStopAll;
                Extraction_chartstep_c21_Extraction();
            }

            if (inputEventFiredFlag_p != 0 &&
                    Extraction_DW.Extr_HighSpd_CEM_Enabled.Extr_HighSpd_EvCEMFailureDetectedEventCounter
                > 0U)
            {
                Extraction_B.VariantMergeForOutportExtr_HighSpd_EvCEMFailureDetected
                    =
                    !Extraction_B.VariantMergeForOutportExtr_HighSpd_EvCEMFailureDetected;
                Extraction_DW.Extr_HighSpd_CEM_Enabled.Extr_HighSpd_EvCEMFailureDetectedEventCounter
                    --;
            }
        }

        Extraction_PrevZCX.Extr_HighSpd_CEM_Enabled.Extr_HighSpd_CEMLogic_Trig_ZCE
            [0] = trigger_p;
        Extraction_PrevZCX.Extr_HighSpd_CEM_Enabled.Extr_HighSpd_CEMLogic_Trig_ZCE
            [1] = Extraction_B.VariantMergeForOutportExtr_HighSpd_EvStopAll;
    }

#endif                                 /* EXTR_VARIANT_HIGH_SPEED_PE_ENABLED */

    /* End of Outputs for SubSystem: '<S61>/Extr_HighSpd_CEM' */

    /* Chart: '<S64>/Extr_HighSpd_ExtUnbCheckLogic' incorporates:
     *  TriggerPort: '<S71>/input events'
     */
    Extraction_B.zcEvent_idx_0 = (sint32)Extraction_B.sf_TriggerGen_bc.trigger
        != (Extraction_PrevZCX.Extr_HighSpd_ExtUnbCheckLogic_Trig_ZCE[0] ==
            POS_ZCSIG) &&
        Extraction_PrevZCX.Extr_HighSpd_ExtUnbCheckLogic_Trig_ZCE[0] !=
        UNINITIALIZED_ZCSIG;
    Extraction_B.zcEvent_idx_1 = (sint32)
        Extraction_B.VariantMergeForOutportExtr_HighSpd_EvStartPlateau !=
        (Extraction_PrevZCX.Extr_HighSpd_ExtUnbCheckLogic_Trig_ZCE[1] ==
         POS_ZCSIG) &&
        Extraction_PrevZCX.Extr_HighSpd_ExtUnbCheckLogic_Trig_ZCE[1] !=
        UNINITIALIZED_ZCSIG;
    Extraction_B.zcEvent_idx_2 = (sint32)
        Extraction_B.VariantMergeForOutportExtr_HighSpd_EvStopAll !=
        (Extraction_PrevZCX.Extr_HighSpd_ExtUnbCheckLogic_Trig_ZCE[2] ==
         POS_ZCSIG) &&
        Extraction_PrevZCX.Extr_HighSpd_ExtUnbCheckLogic_Trig_ZCE[2] !=
        UNINITIALIZED_ZCSIG;
    Extraction_B.zcEvent_idx_3 = (sint32)
        Extraction_B.VariantMergeForOutportExtr_HighSpd_EvPlateauComplete !=
        (Extraction_PrevZCX.Extr_HighSpd_ExtUnbCheckLogic_Trig_ZCE[3] ==
         POS_ZCSIG) &&
        Extraction_PrevZCX.Extr_HighSpd_ExtUnbCheckLogic_Trig_ZCE[3] !=
        UNINITIALIZED_ZCSIG;
    if (Extraction_B.zcEvent_idx_0 || Extraction_B.zcEvent_idx_1 ||
            Extraction_B.zcEvent_idx_2 || Extraction_B.zcEvent_idx_3)
    {
        /* Gateway: Extraction/Extr_HighSpd/Extr_HighSpd_Algorithms/Extr_HighSpd_ExtUnbChk/Extr_HighSpd_ExtUnbCheckLogic */
        Extraction_B.i = 0;
        if ((sint8)(Extraction_B.zcEvent_idx_0 ?
                    Extraction_B.sf_TriggerGen_bc.trigger ? RISING_ZCEVENT :
                    FALLING_ZCEVENT : NO_ZCEVENT) != 0)
        {
            Extraction_B.i = 1;

            /* Event: '<S71>:184' */
            Extraction_DW.sfEvent_pr = Extraction_event_Trigger_o1;
            Extraction_chartstep_c45_Extraction();
        }

        if ((sint8)(Extraction_B.zcEvent_idx_1 ?
                    Extraction_B.VariantMergeForOutportExtr_HighSpd_EvStartPlateau
                    ? RISING_ZCEVENT : FALLING_ZCEVENT : NO_ZCEVENT) != 0)
        {
            Extraction_B.i = 1;

            /* Event: '<S71>:172' */
            Extraction_DW.sfEvent_pr =
                Extraction_event_Extr_HighSpd_EvStartExtndUnbChkEv;
            Extraction_chartstep_c45_Extraction();
        }

        if ((sint8)(Extraction_B.zcEvent_idx_2 ?
                    Extraction_B.VariantMergeForOutportExtr_HighSpd_EvStopAll ?
                    RISING_ZCEVENT : FALLING_ZCEVENT : NO_ZCEVENT) != 0)
        {
            Extraction_B.i = 1;

            /* Event: '<S71>:161' */
            Extraction_DW.sfEvent_pr =
                Extraction_event_Extr_HighSpd_EvStopAll_pq;
            Extraction_chartstep_c45_Extraction();
        }

        if ((sint8)(Extraction_B.zcEvent_idx_3 ?
                    Extraction_B.VariantMergeForOutportExtr_HighSpd_EvPlateauComplete
                    ? RISING_ZCEVENT : FALLING_ZCEVENT : NO_ZCEVENT) != 0)
        {
            Extraction_B.i = 1;

            /* Event: '<S71>:162' */
            Extraction_DW.sfEvent_pr =
                Extraction_event_Extr_HighSpd_EvPlateauComplete_dk;
            Extraction_chartstep_c45_Extraction();
        }

        if (Extraction_B.i != 0 &&
                Extraction_DW.Extr_HighSpd_EvExtndChkFailedEventCounter > 0U)
        {
            Extraction_B.Extr_HighSpd_EvExtndChkFailed =
                !Extraction_B.Extr_HighSpd_EvExtndChkFailed;
            Extraction_DW.Extr_HighSpd_EvExtndChkFailedEventCounter--;
        }
    }

    Extraction_PrevZCX.Extr_HighSpd_ExtUnbCheckLogic_Trig_ZCE[0] =
        Extraction_B.sf_TriggerGen_bc.trigger;
    Extraction_PrevZCX.Extr_HighSpd_ExtUnbCheckLogic_Trig_ZCE[1] =
        Extraction_B.VariantMergeForOutportExtr_HighSpd_EvStartPlateau;
    Extraction_PrevZCX.Extr_HighSpd_ExtUnbCheckLogic_Trig_ZCE[2] =
        Extraction_B.VariantMergeForOutportExtr_HighSpd_EvStopAll;
    Extraction_PrevZCX.Extr_HighSpd_ExtUnbCheckLogic_Trig_ZCE[3] =
        Extraction_B.VariantMergeForOutportExtr_HighSpd_EvPlateauComplete;

    /* SignalConversion: '<S61>/OutportBufferForExtr_HighSpd_EvExtndChkFailed' */
    Extraction_B.OutportBufferForExtr_HighSpd_EvExtndChkFailed =
        Extraction_B.Extr_HighSpd_EvExtndChkFailed;

    /* Update for Memory: '<S61>/HighSpdExtUnbCalculated' */
    Extraction_DW.HighSpdExtUnbCalculated_PreviousInput =
        Extraction_B.Extr_HighSpd_ExtUnbCalculated;

    /* RootInportFunctionCallGenerator: '<Root>/RootFcnCall_InsertedFor_Extraction__Handler5ms_at_outport_1' incorporates:
     *  SubSystem: '<S2>/Extr_Rebalance'
     */
    /* Chart: '<S184>/TriggerGen' */
    Extraction_TriggerGen(&Extraction_B.sf_TriggerGen_dq,
                          &Extraction_DW.sf_TriggerGen_dq);

    /* Chart: '<S12>/Extr_Rebal_Logic' incorporates:
     *  TriggerPort: '<S180>/input events'
     */
    Extraction_B.zcEvent_idx_0 = (sint32)Extraction_B.sf_TriggerGen_dq.trigger
        != (Extraction_PrevZCX.Extr_Rebal_Logic_Trig_ZCE[0] == POS_ZCSIG) &&
        Extraction_PrevZCX.Extr_Rebal_Logic_Trig_ZCE[0] != UNINITIALIZED_ZCSIG;
    Extraction_B.zcEvent_idx_1 = (sint32)
        Extraction_B.OutportBufferForExtr_Sup_EvRebalStart !=
        (Extraction_PrevZCX.Extr_Rebal_Logic_Trig_ZCE[1] == POS_ZCSIG) &&
        Extraction_PrevZCX.Extr_Rebal_Logic_Trig_ZCE[1] != UNINITIALIZED_ZCSIG;
    Extraction_B.zcEvent_idx_2 = (sint32)
        Extraction_B.OutportBufferForExtr_Sup_EvStopAll !=
        (Extraction_PrevZCX.Extr_Rebal_Logic_Trig_ZCE[2] == POS_ZCSIG) &&
        Extraction_PrevZCX.Extr_Rebal_Logic_Trig_ZCE[2] != UNINITIALIZED_ZCSIG;
    Extraction_B.zcEvent_idx_3 = (sint32)
        Extraction_B.OutportBufferForExtr_Sup_ResetRebalCnt !=
        (Extraction_PrevZCX.Extr_Rebal_Logic_Trig_ZCE[3] == POS_ZCSIG) &&
        Extraction_PrevZCX.Extr_Rebal_Logic_Trig_ZCE[3] != UNINITIALIZED_ZCSIG;
    Extraction_B.OutportBufferForExtr_LowSpd_EvMddpStart = (sint32)
        Extraction_B.OutportBufferForExtr_Sup_ResetLoadMass !=
        (Extraction_PrevZCX.Extr_Rebal_Logic_Trig_ZCE[4] == POS_ZCSIG) &&
        Extraction_PrevZCX.Extr_Rebal_Logic_Trig_ZCE[4] != UNINITIALIZED_ZCSIG;
    Extraction_B.OutportBufferForExtr_LowSpd_EvStartBasicUnbChk = (sint32)
        Extraction_B.OutportBufferForExtr_LowSpd_EvResetShortRedist !=
        (Extraction_PrevZCX.Extr_Rebal_Logic_Trig_ZCE[5] == POS_ZCSIG) &&
        Extraction_PrevZCX.Extr_Rebal_Logic_Trig_ZCE[5] != UNINITIALIZED_ZCSIG;
    Extraction_B.zcEvent_idx_6 = (sint32)
        Extraction_B.OutportBufferForExtr_Sup_EvFoamDetected !=
        (Extraction_PrevZCX.Extr_Rebal_Logic_Trig_ZCE[6] == POS_ZCSIG) &&
        Extraction_PrevZCX.Extr_Rebal_Logic_Trig_ZCE[6] != UNINITIALIZED_ZCSIG;
    Extraction_B.zcEvent_idx_7 = (sint32)
        Extraction_B.OutportBufferForExtr_Sup_ClearFault !=
        (Extraction_PrevZCX.Extr_Rebal_Logic_Trig_ZCE[7] == POS_ZCSIG) &&
        Extraction_PrevZCX.Extr_Rebal_Logic_Trig_ZCE[7] != UNINITIALIZED_ZCSIG;
    Extraction_B.zcEvent_idx_8 = (sint32)Extraction_B.Extr_Sup_EvSpeedReached !=
        (Extraction_PrevZCX.Extr_Rebal_Logic_Trig_ZCE[8] == POS_ZCSIG) &&
        Extraction_PrevZCX.Extr_Rebal_Logic_Trig_ZCE[8] != UNINITIALIZED_ZCSIG;
    Extraction_B.zcEvent_idx_9 = (sint32)
        Extraction_B.Extr_Sup_EvSpeedErrorTimeout !=
        (Extraction_PrevZCX.Extr_Rebal_Logic_Trig_ZCE[9] == POS_ZCSIG) &&
        Extraction_PrevZCX.Extr_Rebal_Logic_Trig_ZCE[9] != UNINITIALIZED_ZCSIG;
    Extraction_B.zcEvent_idx_10 = (sint32)
        Extraction_B.VariantMergeForOutportExtr_Accel_EvCabHit !=
        (Extraction_PrevZCX.Extr_Rebal_Logic_Trig_ZCE[10] == POS_ZCSIG) &&
        Extraction_PrevZCX.Extr_Rebal_Logic_Trig_ZCE[10] != UNINITIALIZED_ZCSIG;
    if (Extraction_B.zcEvent_idx_0 || Extraction_B.zcEvent_idx_1 ||
            Extraction_B.zcEvent_idx_2 || Extraction_B.zcEvent_idx_3 ||
            Extraction_B.OutportBufferForExtr_LowSpd_EvMddpStart ||
            Extraction_B.OutportBufferForExtr_LowSpd_EvStartBasicUnbChk ||
            Extraction_B.zcEvent_idx_6 || Extraction_B.zcEvent_idx_7 ||
            Extraction_B.zcEvent_idx_8 || Extraction_B.zcEvent_idx_9 ||
            Extraction_B.zcEvent_idx_10)
    {
        Extraction_B.rtb_inputevents_mn_idx_1 = (sint8)
            (Extraction_B.zcEvent_idx_1 ?
             Extraction_B.OutportBufferForExtr_Sup_EvRebalStart ? RISING_ZCEVENT
             : FALLING_ZCEVENT : NO_ZCEVENT);
        Extraction_B.rtb_inputevents_mn_idx_2 = (sint8)
            (Extraction_B.zcEvent_idx_2 ?
             Extraction_B.OutportBufferForExtr_Sup_EvStopAll ? RISING_ZCEVENT :
             FALLING_ZCEVENT : NO_ZCEVENT);
        Extraction_B.rtb_inputevents_mn_idx_3 = (sint8)
            (Extraction_B.zcEvent_idx_3 ?
             Extraction_B.OutportBufferForExtr_Sup_ResetRebalCnt ?
             RISING_ZCEVENT : FALLING_ZCEVENT : NO_ZCEVENT);
        Extraction_B.rtb_inputevents_mn_idx_4 = (sint8)
            (Extraction_B.zcEvent_idx_6 ?
             Extraction_B.OutportBufferForExtr_Sup_EvFoamDetected ?
             RISING_ZCEVENT : FALLING_ZCEVENT : NO_ZCEVENT);
        Extraction_B.rtb_inputevents_mn_idx_5 = (sint8)
            (Extraction_B.zcEvent_idx_7 ?
             Extraction_B.OutportBufferForExtr_Sup_ClearFault ? RISING_ZCEVENT :
             FALLING_ZCEVENT : NO_ZCEVENT);
        Extraction_B.rtb_inputevents_mn_idx_6 = (sint8)
            (Extraction_B.zcEvent_idx_8 ? Extraction_B.Extr_Sup_EvSpeedReached ?
             RISING_ZCEVENT : FALLING_ZCEVENT : NO_ZCEVENT);
        Extraction_B.rtb_inputevents_mn_idx_7 = (sint8)
            (Extraction_B.zcEvent_idx_9 ?
             Extraction_B.Extr_Sup_EvSpeedErrorTimeout ? RISING_ZCEVENT :
             FALLING_ZCEVENT : NO_ZCEVENT);
        Extraction_B.rtb_inputevents_mn_idx_8 = (sint8)
            (Extraction_B.zcEvent_idx_10 ?
             Extraction_B.VariantMergeForOutportExtr_Accel_EvCabHit ?
             RISING_ZCEVENT : FALLING_ZCEVENT : NO_ZCEVENT);

        /* Gateway: Extraction/Extr_Rebalance/Extr_Rebal_Logic */
        Extraction_B.i = 0;
        if ((sint8)(Extraction_B.zcEvent_idx_0 ?
                    Extraction_B.sf_TriggerGen_dq.trigger ? RISING_ZCEVENT :
                    FALLING_ZCEVENT : NO_ZCEVENT) != 0)
        {
            Extraction_B.i = 1;
            if (Extraction_DW.temporalCounter_i1 < MAX_uint32_T)
            {
                Extraction_DW.temporalCounter_i1++;
            }

            /* Event: '<S180>:10' */
            Extraction_DW.sfEvent = Extraction_event_Trigger_o1;
            Extraction_chartstep_c24_Extraction();
        }

        if (Extraction_B.rtb_inputevents_mn_idx_1 != 0)
        {
            Extraction_B.i = 1;

            /* Event: '<S180>:1' */
            Extraction_DW.sfEvent = Extraction_event_Extr_Sup_EvRebalStart_dl;
            Extraction_chartstep_c24_Extraction();
        }

        if (Extraction_B.rtb_inputevents_mn_idx_2 != 0)
        {
            Extraction_B.i = 1;

            /* Event: '<S180>:27' */
            Extraction_DW.sfEvent = Extraction_event_Extr_Sup_StopAll;
            Extraction_chartstep_c24_Extraction();
        }

        if (Extraction_B.rtb_inputevents_mn_idx_3 != 0)
        {
            Extraction_B.i = 1;

            /* Event: '<S180>:37' */
            Extraction_DW.sfEvent = Extraction_event_Extr_Sup_ResetRebalCnt_d5;
            Extraction_chartstep_c24_Extraction();
        }

        if ((sint8)(Extraction_B.OutportBufferForExtr_LowSpd_EvMddpStart ?
                    Extraction_B.OutportBufferForExtr_Sup_ResetLoadMass ?
                    RISING_ZCEVENT : FALLING_ZCEVENT : NO_ZCEVENT) != 0)
        {
            Extraction_B.i = 1;

            /* Event: '<S180>:176' */
            Extraction_DW.sfEvent =
                Extraction_event_Extr_LowSpd_EvDistributionStart;
            Extraction_chartstep_c24_Extraction();
        }

        if ((sint8)(Extraction_B.OutportBufferForExtr_LowSpd_EvStartBasicUnbChk ?
                    Extraction_B.OutportBufferForExtr_LowSpd_EvResetShortRedist ?
                    RISING_ZCEVENT : FALLING_ZCEVENT : NO_ZCEVENT) != 0)
        {
            Extraction_B.i = 1;

            /* Event: '<S180>:206' */
            Extraction_DW.sfEvent =
                Extraction_event_Extr_LowSpd_EvResetShortRedist;
            Extraction_chartstep_c24_Extraction();
        }

        if (Extraction_B.rtb_inputevents_mn_idx_4 != 0)
        {
            Extraction_B.i = 1;

            /* Event: '<S180>:213' */
            Extraction_DW.sfEvent = Extraction_event_Extr_Sup_EvFoamDetected_f3;
            Extraction_chartstep_c24_Extraction();
        }

        if (Extraction_B.rtb_inputevents_mn_idx_5 != 0)
        {
            Extraction_B.i = 1;

            /* Event: '<S180>:215' */
            Extraction_DW.sfEvent = Extraction_event_Extr_Sup_ClearFault_oj;
            Extraction_chartstep_c24_Extraction();
        }

        if (Extraction_B.rtb_inputevents_mn_idx_6 != 0)
        {
            Extraction_B.i = 1;

            /* Event: '<S180>:233' */
            Extraction_DW.sfEvent = Extraction_event_Extr_Sup_EvSpeedReached_ok;
            Extraction_chartstep_c24_Extraction();
        }

        if (Extraction_B.rtb_inputevents_mn_idx_7 != 0)
        {
            Extraction_B.i = 1;

            /* Event: '<S180>:234' */
            Extraction_DW.sfEvent =
                Extraction_event_Extr_Sup_EvSpeedErrorTimeout_le;
            Extraction_chartstep_c24_Extraction();
        }

        if (Extraction_B.rtb_inputevents_mn_idx_8 != 0)
        {
            Extraction_B.i = 1;

            /* Event: '<S180>:306' */
            Extraction_DW.sfEvent = Extraction_event_Extr_Accel_EvCabHit_ii;
            Extraction_chartstep_c24_Extraction();
        }

        if (Extraction_B.i != 0 &&
                Extraction_DW.Extr_Rebal_EvCompleteEventCounter > 0U)
        {
            Extraction_B.Extr_Rebal_EvComplete =
                !Extraction_B.Extr_Rebal_EvComplete;
            Extraction_DW.Extr_Rebal_EvCompleteEventCounter--;
        }

        if (Extraction_B.i != 0 &&
                Extraction_DW.Extr_Rebal_EvP2PTorqFaultEventCounter > 0U)
        {
            Extraction_B.Extr_Rebal_EvP2PTorqFault =
                !Extraction_B.Extr_Rebal_EvP2PTorqFault;
            Extraction_DW.Extr_Rebal_EvP2PTorqFaultEventCounter--;
        }

        if (Extraction_B.i != 0 &&
                Extraction_DW.Extr_Rebal_EvShortRedistExceededEventCounter > 0U)
        {
            Extraction_B.Extr_Rebal_EvShortRedistExceeded =
                !Extraction_B.Extr_Rebal_EvShortRedistExceeded;
            Extraction_DW.Extr_Rebal_EvShortRedistExceededEventCounter--;
        }

        if (Extraction_B.i != 0 &&
                Extraction_DW.Extr_Rebal_EvCleanWasherExceptionEventCounter > 0U)
        {
            Extraction_B.Extr_Rebal_EvCleanWasherException =
                !Extraction_B.Extr_Rebal_EvCleanWasherException;
            Extraction_DW.Extr_Rebal_EvCleanWasherExceptionEventCounter--;
        }
    }

    Extraction_PrevZCX.Extr_Rebal_Logic_Trig_ZCE[0] =
        Extraction_B.sf_TriggerGen_dq.trigger;
    Extraction_PrevZCX.Extr_Rebal_Logic_Trig_ZCE[1] =
        Extraction_B.OutportBufferForExtr_Sup_EvRebalStart;
    Extraction_PrevZCX.Extr_Rebal_Logic_Trig_ZCE[2] =
        Extraction_B.OutportBufferForExtr_Sup_EvStopAll;
    Extraction_PrevZCX.Extr_Rebal_Logic_Trig_ZCE[3] =
        Extraction_B.OutportBufferForExtr_Sup_ResetRebalCnt;
    Extraction_PrevZCX.Extr_Rebal_Logic_Trig_ZCE[4] =
        Extraction_B.OutportBufferForExtr_Sup_ResetLoadMass;
    Extraction_PrevZCX.Extr_Rebal_Logic_Trig_ZCE[5] =
        Extraction_B.OutportBufferForExtr_LowSpd_EvResetShortRedist;
    Extraction_PrevZCX.Extr_Rebal_Logic_Trig_ZCE[6] =
        Extraction_B.OutportBufferForExtr_Sup_EvFoamDetected;
    Extraction_PrevZCX.Extr_Rebal_Logic_Trig_ZCE[7] =
        Extraction_B.OutportBufferForExtr_Sup_ClearFault;
    Extraction_PrevZCX.Extr_Rebal_Logic_Trig_ZCE[8] =
        Extraction_B.Extr_Sup_EvSpeedReached;
    Extraction_PrevZCX.Extr_Rebal_Logic_Trig_ZCE[9] =
        Extraction_B.Extr_Sup_EvSpeedErrorTimeout;
    Extraction_PrevZCX.Extr_Rebal_Logic_Trig_ZCE[10] =
        Extraction_B.VariantMergeForOutportExtr_Accel_EvCabHit;

    /* SignalConversion: '<S12>/OutportBufferForExtr_Rebal_EvCleanWasherException' */
    Extraction_B.OutportBufferForExtr_Rebal_EvCleanWasherException =
        Extraction_B.Extr_Rebal_EvCleanWasherException;

    /* SignalConversion: '<S12>/OutportBufferForExtr_Rebal_EvComplete' */
    Extraction_B.OutportBufferForExtr_Rebal_EvComplete =
        Extraction_B.Extr_Rebal_EvComplete;

    /* SignalConversion: '<S12>/OutportBufferForExtr_Rebal_EvP2PTorqFault' */
    Extraction_B.OutportBufferForExtr_Rebal_EvP2PTorqFault =
        Extraction_B.Extr_Rebal_EvP2PTorqFault;

    /* SignalConversion: '<S12>/OutportBufferForExtr_Rebal_EvShortRedistExceeded' */
    Extraction_B.OutportBufferForExtr_Rebal_EvShortRedistExceeded =
        Extraction_B.Extr_Rebal_EvShortRedistExceeded;

    /* RootInportFunctionCallGenerator: '<Root>/RootFcnCall_InsertedFor_Extraction__Handler5ms_at_outport_1' incorporates:
     *  SubSystem: '<S2>/Extr_Accel'
     */

    /* Chart: '<S27>/TriggerGen' */
    Extraction_TriggerGen(&Extraction_B.sf_TriggerGen_gl,
                          &Extraction_DW.sf_TriggerGen_gl);

    /* Outputs for Atomic SubSystem: '<S9>/Extr_Accel_Enabled' */
#if EXTR_VARIANT_ACCELEROMETER_ENABLED

    /* Output and update for atomic system: '<S26>/Extr_Accel_Enabled' */
    {
        float32 maxV_j;
        sint32 inputEventFiredFlag_j;
        sint8 s45_iter_j;
        boolean_T loopCond_j;
        float32 rtb_Extr_Accel_VibMapFiltAccel_le[3];
        sint32 sfEvent_j;
        boolean_T zcEvent_idx_0_j;
        boolean_T zcEvent_idx_1_j;
        uint32 tmp_j;
        boolean_T trigger_j;
        boolean_T Extr_Accel_Acquiring_j;
        uint16 Extr_Accel_ExtPlateauTimeIncrement_nd_j;
        uint8 Extr_Accel_VibMapSafeSpeed_div10_p5_j;
        uint8 is_active_c18_Extraction_j;
        uint8 is_c18_Extraction_j;
        EXTR_ACCEL_STATE Extr_Accel_State_j;
        uint8 is_SUPERVISOR_ENABLED_j;
        uint16 Extr_Accel_CommErrors_j;
        float32 Extr_Accel_MaxAccel_j;
        uint32 temporalCounter_i1_j;
        uint8 is_RUNNING_j;
        VIB_MAP_STATE Extr_Accel_VibMapState_j;
        uint8 is_c26_Extraction_j;
        uint8 is_MONITORING_j;
        float32 Extr_Accel_VibMapFloorDisp_j;
        float32 Extr_Accel_MaxVibMapFloorDisp_j;
        uint32 Extr_Accel_EvExcessiveFloorDispEventCounter_j;
        float32 Extr_Accel_VibMapFloorDispThreshold_j;
        sint32 i_j;
        sint32 i_jb;
        Extr_Accel_VibMapFloorDispThreshold_j =
            Extr_Accel_VibMapFloorDispThreshold;
        Extr_Accel_EvExcessiveFloorDispEventCounter_j =
            Extraction_DW.Extr_Accel_Enabled_gt.Extr_Accel_EvExcessiveFloorDispEventCounter;
        Extr_Accel_MaxVibMapFloorDisp_j = Extr_Accel_MaxVibMapFloorDisp;
        Extr_Accel_VibMapFloorDisp_j = Extr_Accel_VibMapFloorDisp;
        is_MONITORING_j = Extraction_DW.Extr_Accel_Enabled_gt.is_MONITORING;
        is_c26_Extraction_j =
            Extraction_DW.Extr_Accel_Enabled_gt.is_c26_Extraction;
        Extr_Accel_VibMapState_j = Extr_Accel_VibMapState;
        is_RUNNING_j = Extraction_DW.Extr_Accel_Enabled_gt.is_RUNNING;
        temporalCounter_i1_j =
            Extraction_DW.Extr_Accel_Enabled_gt.temporalCounter_i1;
        Extr_Accel_MaxAccel_j = Extr_Accel_MaxAccel;
        Extr_Accel_CommErrors_j = Extr_Accel_CommErrors;
        is_SUPERVISOR_ENABLED_j =
            Extraction_DW.Extr_Accel_Enabled_gt.is_SUPERVISOR_ENABLED;
        Extr_Accel_State_j = Extr_Accel_State;
        is_c18_Extraction_j =
            Extraction_DW.Extr_Accel_Enabled_gt.is_c18_Extraction;
        is_active_c18_Extraction_j =
            Extraction_DW.Extr_Accel_Enabled_gt.is_active_c18_Extraction;
        Extr_Accel_VibMapSafeSpeed_div10_p5_j =
            Extraction_B.Extr_Accel_Enabled_gt.Extr_Accel_VibMapSafeSpeed_div10_p5;
        Extr_Accel_ExtPlateauTimeIncrement_nd_j =
            Extraction_B.Extr_Accel_Enabled_gt.Extr_Accel_ExtPlateauTimeIncrement_nd;
        Extr_Accel_Acquiring_j = Extr_Accel_Acquiring;

        /* Chart: '<S29>/Extr_Accel_CabHitLogic' incorporates:
         *  TriggerPort: '<S34>/input events'
         */
        trigger_j = Extraction_B.sf_TriggerGen_gl.trigger;

        /* Chart: '<S30>/Extr_Accel_Supervisor' */
        /* Gateway: Extraction/Extr_Accel/Extr_Accel_Enabled/Extr_Accel_Enabled/Extr_Accel_Supervisor/Extr_Accel_Supervisor */
        /* During: Extraction/Extr_Accel/Extr_Accel_Enabled/Extr_Accel_Enabled/Extr_Accel_Supervisor/Extr_Accel_Supervisor */
        if (is_active_c18_Extraction_j == 0U)
        {
            /* Entry: Extraction/Extr_Accel/Extr_Accel_Enabled/Extr_Accel_Enabled/Extr_Accel_Supervisor/Extr_Accel_Supervisor */
            is_active_c18_Extraction_j = 1U;

            /* Entry Internal: Extraction/Extr_Accel/Extr_Accel_Enabled/Extr_Accel_Enabled/Extr_Accel_Supervisor/Extr_Accel_Supervisor */
            /* Transition: '<S38>:2' */
            is_c18_Extraction_j = Extraction_IN_SUPERVISOR_IDLE;
            Extr_Accel_State_j = EXTR_ACCEL_STATE_SUPERVISOR_IDLE;
        }
        else
        {
            switch (is_c18_Extraction_j)
            {
              case Extraction_IN_SUPERVISOR_DISABLED:
                /* During 'SUPERVISOR_DISABLED': '<S38>:45' */
                break;

              case Extraction_IN_SUPERVISOR_ENABLED:
                /* During 'SUPERVISOR_ENABLED': '<S38>:3' */
                switch (is_SUPERVISOR_ENABLED_j)
                {
                  case Extraction_IN_ACCELEROMETER_IDLE:
                    /* During 'ACCELEROMETER_IDLE': '<S38>:36' */
                    if (Extraction_B.OutportBufferForExtr_Sup_VibMapEnabled ||
                            Extraction_B.OutportBufferForExtr_Sup_CabHitEnabled)
                    {
                        /* Transition: '<S38>:37' */
                        is_SUPERVISOR_ENABLED_j =
                            Extraction_IN_STARTING_ACCELEROMETER;
                        Extr_Accel_State_j =
                            EXTR_ACCEL_STATE_STARTING_ACCELEROMETER;

                        /* Entry 'STARTING_ACCELEROMETER': '<S38>:24' */
                        Extraction_AccelerometerControl(true);
                        Extr_Accel_CommErrors_j = 0U;
                    }
                    break;

                  case Extraction_IN_DISABLING_ACCELEROMETER:
                    /* During 'DISABLING_ACCELEROMETER': '<S38>:7' */
                    if (Extraction_DW.Extr_Accel_Enabled_gt.AccelStatus ==
                            ACCELEROMETER_DISABLED)
                    {
                        /* Transition: '<S38>:11' */
                        is_SUPERVISOR_ENABLED_j = Extraction_IN_SENDING_COMMAND;
                        Extr_Accel_State_j = EXTR_ACCEL_STATE_SENDING_COMMAND;

                        /* Entry 'SENDING_COMMAND': '<S38>:10' */
                        Extraction_MeasureBaseline();
                        Extraction_GetAccelBaselineStatus
                            (&Extraction_DW.Extr_Accel_Enabled_gt.AccelBaselineStatus);
                    }
                    else
                    {
                        Extraction_GetAccelStatus
                            (&Extraction_DW.Extr_Accel_Enabled_gt.AccelStatus);
                    }
                    break;

                  case Extraction_IN_MEASURING_ACCELERATION:
                    /* During 'MEASURING_ACCELERATION': '<S38>:28' */
                    if (!(Extraction_B.OutportBufferForExtr_Sup_VibMapEnabled ||
                          Extraction_B.OutportBufferForExtr_Sup_CabHitEnabled))
                    {
                        /* Transition: '<S38>:39' */
                        /* Transition: '<S38>:41' */
                        /* Transition: '<S38>:42' */
                        is_SUPERVISOR_ENABLED_j =
                            Extraction_IN_ACCELEROMETER_IDLE;
                        Extr_Accel_State_j = EXTR_ACCEL_STATE_ACCELEROMETER_IDLE;

                        /* Entry 'ACCELEROMETER_IDLE': '<S38>:36' */
                        Extraction_GetAccelBaseline(Extr_Accel_Baseline);
                        Extr_Accel_Acquiring_j = false;
                    }
                    else
                    {
                        Extraction_GetAccelStatus
                            (&Extraction_DW.Extr_Accel_Enabled_gt.AccelStatus);
                        if (Extraction_DW.Extr_Accel_Enabled_gt.AccelStatus !=
                                ACCELEROMETER_ACQUIRING)
                        {
                            tmp_j = Extr_Accel_CommErrors_j + 1U;
                            if (tmp_j > 65535U)
                            {
                                tmp_j = 65535U;
                            }

                            Extr_Accel_CommErrors_j = (uint16)tmp_j;
                        }
                    }
                    break;

                  case Extraction_IN_MEASURING_BASELINE:
                    /* During 'MEASURING_BASELINE': '<S38>:18' */
                    if (Extraction_DW.Extr_Accel_Enabled_gt.AccelBaselineStatus ==
                        ACCEL_IDLE)
                    {
                        /* Transition: '<S38>:14' */
                        is_SUPERVISOR_ENABLED_j =
                            Extraction_IN_ACCELEROMETER_IDLE;
                        Extr_Accel_State_j = EXTR_ACCEL_STATE_ACCELEROMETER_IDLE;

                        /* Entry 'ACCELEROMETER_IDLE': '<S38>:36' */
                        Extraction_GetAccelBaseline(Extr_Accel_Baseline);
                        Extr_Accel_Acquiring_j = false;
                    }
                    else
                    {
                        Extraction_GetAccelBaselineStatus
                            (&Extraction_DW.Extr_Accel_Enabled_gt.AccelBaselineStatus);
                    }
                    break;

                  case Extraction_IN_SENDING_COMMAND:
                    /* During 'SENDING_COMMAND': '<S38>:10' */
                    if (Extraction_DW.Extr_Accel_Enabled_gt.AccelBaselineStatus ==
                        ACCEL_MEASURING_BASELINE)
                    {
                        /* Transition: '<S38>:19' */
                        is_SUPERVISOR_ENABLED_j =
                            Extraction_IN_MEASURING_BASELINE;
                        Extr_Accel_State_j = EXTR_ACCEL_STATE_MEASURING_BASELINE;

                        /* Entry 'MEASURING_BASELINE': '<S38>:18' */
                        Extraction_GetAccelBaselineStatus
                            (&Extraction_DW.Extr_Accel_Enabled_gt.AccelBaselineStatus);
                    }
                    else
                    {
                        Extraction_GetAccelBaselineStatus
                            (&Extraction_DW.Extr_Accel_Enabled_gt.AccelBaselineStatus);
                    }
                    break;

                  default:
                    /* During 'STARTING_ACCELEROMETER': '<S38>:24' */
                    if (Extraction_DW.Extr_Accel_Enabled_gt.AccelStatus ==
                            ACCELEROMETER_ACQUIRING)
                    {
                        /* Transition: '<S38>:29' */
                        is_SUPERVISOR_ENABLED_j =
                            Extraction_IN_MEASURING_ACCELERATION;
                        Extr_Accel_State_j =
                            EXTR_ACCEL_STATE_MEASURING_ACCELERATION;

                        /* Entry 'MEASURING_ACCELERATION': '<S38>:28' */
                        Extr_Accel_Acquiring_j = true;
                        Extraction_GetAccelStatus
                            (&Extraction_DW.Extr_Accel_Enabled_gt.AccelStatus);
                        if (Extraction_DW.Extr_Accel_Enabled_gt.AccelStatus !=
                                ACCELEROMETER_ACQUIRING)
                        {
                            tmp_j = Extr_Accel_CommErrors_j + 1U;
                            if (tmp_j > 65535U)
                            {
                                tmp_j = 65535U;
                            }

                            Extr_Accel_CommErrors_j = (uint16)tmp_j;
                        }
                    }
                    else
                    {
                        Extraction_GetAccelStatus
                            (&Extraction_DW.Extr_Accel_Enabled_gt.AccelStatus);
                    }
                    break;
                }
                break;

              default:
                /* During 'SUPERVISOR_IDLE': '<S38>:1' */
                /* Transition: '<S38>:59' */
                if (!(ExtractionParams__IsCabHitEnabled() != 0) &&
                        !(ExtractionParams__IsVibMapEnabled() != 0))
                {
                    /* Transition: '<S38>:61' */
                    is_c18_Extraction_j = Extraction_IN_SUPERVISOR_DISABLED;
                    Extr_Accel_State_j = EXTR_ACCEL_STATE_SUPERVISOR_DISABLED;
                }
                else
                {
                    /* Transition: '<S38>:60' */
                    is_c18_Extraction_j = Extraction_IN_SUPERVISOR_ENABLED;

                    /* Entry 'SUPERVISOR_ENABLED': '<S38>:3' */
                    Extr_Accel_Acquiring_j = false;
                    Extr_Accel_Baseline[0] = 0.0F;
                    Extr_Accel_Baseline[1] = 0.0F;
                    Extr_Accel_Baseline[2] = 0.0F;

                    /* Entry Internal 'SUPERVISOR_ENABLED': '<S38>:3' */
                    /* Transition: '<S38>:8' */
                    is_SUPERVISOR_ENABLED_j =
                        Extraction_IN_DISABLING_ACCELEROMETER;
                    Extr_Accel_State_j =
                        EXTR_ACCEL_STATE_DISABLING_ACCELEROMETER;

                    /* Entry 'DISABLING_ACCELEROMETER': '<S38>:7' */
                    Extraction_AccelerometerControl(false);
                    Extraction_GetAccelStatus
                        (&Extraction_DW.Extr_Accel_Enabled_gt.AccelStatus);
                }
                break;
            }
        }

        /* End of Chart: '<S30>/Extr_Accel_Supervisor' */

        /* Outputs for Enabled SubSystem: '<S30>/AccelSampleReader' incorporates:
         *  EnablePort: '<S36>/Enable'
         */
        if (Extr_Accel_Acquiring_j)
        {
            /* Outputs for Iterator SubSystem: '<S36>/Subsystem' incorporates:
             *  WhileIterator: '<S45>/While Iterator'
             */
            s45_iter_j = 1;

            /* Constant: '<S30>/AccelWhileIC' */
            loopCond_j = true;
            while (loopCond_j && s45_iter_j <= 8)
            {
                /* MATLAB Function: '<S45>/GetAccelData' */
                /* MATLAB Function 'Extraction/Extr_Accel/Extr_Accel_Enabled/Extr_Accel_Enabled/Extr_Accel_Supervisor/AccelSampleReader/Subsystem/GetAccelData': '<S47>:1' */
                // Retrieve a sample of accelerometer raw data (X,Y,Z vector)	();
                /* '<S47>:1:6' */
                loopCond_j = EXTRACTION_GET_ACCELEROMETER_RAW_DATA
                    (Extr_Inputs_AccelRawData);

                /* Gain: '<S45>/AccelDataFloat' */
                Extraction_B.Extr_Accel_Enabled_gt.Abs_iy[0] = 0.00390625F *
                    (float32)Extr_Inputs_AccelRawData[0];
                Extraction_B.Extr_Accel_Enabled_gt.Abs_iy[1] = 0.00390625F *
                    (float32)Extr_Inputs_AccelRawData[1];
                Extraction_B.Extr_Accel_Enabled_gt.Abs_iy[2] = 0.00390625F *
                    (float32)Extr_Inputs_AccelRawData[2];

                /* MATLAB Function: '<S45>/FilterAccelerometerData' */
                /* MATLAB Function 'Extraction/Extr_Accel/Extr_Accel_Enabled/Extr_Accel_Enabled/Extr_Accel_Supervisor/AccelSampleReader/Subsystem/FilterAccelerometerData': '<S46>:1' */
                // Filter accelerometer raw data (X,Y,Z vector)();
                ExtractionParams__FilterAccelData
                    (Extraction_B.Extr_Accel_Enabled_gt.Abs_iy,
                     Extr_Accel_FiltData);
                s45_iter_j++;
            }

            /* End of Outputs for SubSystem: '<S36>/Subsystem' */
        }

        /* End of Outputs for SubSystem: '<S30>/AccelSampleReader' */

        /* If: '<S28>/If' */
        if (Extr_Accel_Acquiring_j)
        {
            /* Outputs for IfAction SubSystem: '<S28>/UpdateMaximumAcceleration' incorporates:
             *  ActionPort: '<S32>/Action Port'
             */
            /* MinMax: '<S32>/MinMax' incorporates:
             *  Abs: '<S32>/Abs'
             *  Sum: '<S32>/Subtract'
             */
            maxV_j = fmaxf(fmaxf(fabsf(Extr_Accel_FiltData[0] -
                             Extr_Accel_Baseline[0]), fabsf(Extr_Accel_FiltData
                             [1] - Extr_Accel_Baseline[1])), fabsf
                           (Extr_Accel_FiltData[2] - Extr_Accel_Baseline[2]));

            /* If: '<S32>/If' incorporates:
             *  MinMax: '<S32>/MinMax'
             *  RelationalOperator: '<S32>/Relational Operator'
             */
            if (maxV_j > Extr_Accel_MaxAccel_j)
            {
                /* Outputs for IfAction SubSystem: '<S32>/If Action Subsystem' incorporates:
                 *  ActionPort: '<S59>/Action Port'
                 */
                /* DataStoreWrite: '<S59>/Data Store Write' */
                Extr_Accel_MaxAccel_j = maxV_j;

                /* DataStoreWrite: '<S59>/Data Store Write1' */
                Extr_Accel_MaxAccelSpeed = Extr_Inputs_MotorSignals.Speed;

                /* End of Outputs for SubSystem: '<S32>/If Action Subsystem' */
            }

            /* End of If: '<S32>/If' */
            /* End of Outputs for SubSystem: '<S28>/UpdateMaximumAcceleration' */
        }

        /* End of If: '<S28>/If' */

        /* Logic: '<S29>/CabHitFiltData' incorporates:
         *  DataStoreRead: '<S29>/IsAccelEnabled'
         */
        loopCond_j = Extr_Accel_Acquiring_j && ExtractionParams__IsCabHitEnabled
            () != 0;

        /* Outputs for Enabled SubSystem: '<S29>/AccelInputFilter' incorporates:
         *  EnablePort: '<S33>/Enable'
         */
        if (loopCond_j)
        {
            /* Sum: '<S33>/Subtract' */
            Extraction_B.Extr_Accel_Enabled_gt.Abs_iy[0] = Extr_Accel_FiltData[0]
                - Extr_Accel_Baseline[0];
            Extraction_B.Extr_Accel_Enabled_gt.Abs_iy[1] = Extr_Accel_FiltData[1]
                - Extr_Accel_Baseline[1];
            Extraction_B.Extr_Accel_Enabled_gt.Abs_iy[2] = Extr_Accel_FiltData[2]
                - Extr_Accel_Baseline[2];

            /* MATLAB Function: '<S33>/FilterAccelerometerData' */
            /* MATLAB Function 'Extraction/Extr_Accel/Extr_Accel_Enabled/Extr_Accel_Enabled/Extr_Accel_CabHit/AccelInputFilter/FilterAccelerometerData': '<S35>:1' */
            // Filter accelerometer raw data (X,Y,Z vector)();
            ExtractionParams__FilterCabHitAccel
                (Extraction_B.Extr_Accel_Enabled_gt.Abs_iy,
                 rtb_Extr_Accel_VibMapFiltAccel_le);

            /* Math: '<S33>/SquaredAcceleration'
             *
             * About '<S33>/SquaredAcceleration':
             *  Operator: magnitude^2
             */
            Extr_Accel_CabHitFiltAccel[0] = rtb_Extr_Accel_VibMapFiltAccel_le[0]
                * rtb_Extr_Accel_VibMapFiltAccel_le[0];
            Extr_Accel_CabHitFiltAccel[1] = rtb_Extr_Accel_VibMapFiltAccel_le[1]
                * rtb_Extr_Accel_VibMapFiltAccel_le[1];
            Extr_Accel_CabHitFiltAccel[2] = rtb_Extr_Accel_VibMapFiltAccel_le[2]
                * rtb_Extr_Accel_VibMapFiltAccel_le[2];
        }

        /* End of Outputs for SubSystem: '<S29>/AccelInputFilter' */

        /* Logic: '<S29>/CabHitEnabled' */
        loopCond_j = Extraction_B.OutportBufferForExtr_Sup_CabHitEnabled &&
            loopCond_j;

        /* Chart: '<S29>/Extr_Accel_CabHitLogic' incorporates:
         *  TriggerPort: '<S34>/input events'
         */
        zcEvent_idx_0_j = (sint32)trigger_j !=
            (Extraction_PrevZCX.Extr_Accel_Enabled_gt.Extr_Accel_CabHitLogic_Trig_ZCE
             [0] == POS_ZCSIG) &&
            Extraction_PrevZCX.Extr_Accel_Enabled_gt.Extr_Accel_CabHitLogic_Trig_ZCE
            [0] != UNINITIALIZED_ZCSIG;
        zcEvent_idx_1_j = (sint32)
            Extraction_B.OutportBufferForExtr_Sup_EvResetSoftCabHitCnt !=
            (Extraction_PrevZCX.Extr_Accel_Enabled_gt.Extr_Accel_CabHitLogic_Trig_ZCE
             [1] == POS_ZCSIG) &&
            Extraction_PrevZCX.Extr_Accel_Enabled_gt.Extr_Accel_CabHitLogic_Trig_ZCE
            [1] != UNINITIALIZED_ZCSIG;
        if (zcEvent_idx_0_j || zcEvent_idx_1_j)
        {
            /* Gateway: Extraction/Extr_Accel/Extr_Accel_Enabled/Extr_Accel_Enabled/Extr_Accel_CabHit/Extr_Accel_CabHitLogic */
            inputEventFiredFlag_j = 0;
            if ((sint8)(zcEvent_idx_0_j ? trigger_j ? RISING_ZCEVENT :
                        FALLING_ZCEVENT : NO_ZCEVENT) != 0)
            {
                inputEventFiredFlag_j = 1;
                if (Extraction_DW.Extr_Accel_Enabled_gt.temporalCounter_i1_cz <
                        MAX_uint32_T)
                {
                    Extraction_DW.Extr_Accel_Enabled_gt.temporalCounter_i1_cz++;
                }

                /* Event: '<S34>:187' */
                sfEvent_j = Extraction_event_Trigger_l4;
                Extraction_chartstep_c33_Extraction(&loopCond_j, &sfEvent_j);
            }

            if ((sint8)(zcEvent_idx_1_j ?
                        Extraction_B.OutportBufferForExtr_Sup_EvResetSoftCabHitCnt
                        ? RISING_ZCEVENT : FALLING_ZCEVENT : NO_ZCEVENT) != 0)
            {
                inputEventFiredFlag_j = 1;

                /* Event: '<S34>:188' */
                sfEvent_j = Extraction_event_Extr_Sup_EvResetSoftCabHitCnt;
                Extraction_chartstep_c33_Extraction(&loopCond_j, &sfEvent_j);
            }

            if (inputEventFiredFlag_j != 0 &&
                    Extraction_DW.Extr_Accel_Enabled_gt.Extr_Accel_EvCabHitEventCounter
                > 0U)
            {
                Extraction_B.VariantMergeForOutportExtr_Accel_EvCabHit =
                    !Extraction_B.VariantMergeForOutportExtr_Accel_EvCabHit;
                Extraction_DW.Extr_Accel_Enabled_gt.Extr_Accel_EvCabHitEventCounter
                    --;
            }

            if (inputEventFiredFlag_j != 0 &&
                    Extraction_DW.Extr_Accel_Enabled_gt.Extr_Accel_EvCleanWasherExceptionEventCounter
                > 0U)
            {
                Extraction_B.VariantMergeForOutportExtr_Accel_EvCleanWasherException
                    =
                    !Extraction_B.VariantMergeForOutportExtr_Accel_EvCleanWasherException;
                Extraction_DW.Extr_Accel_Enabled_gt.Extr_Accel_EvCleanWasherExceptionEventCounter
                    --;
            }
        }

        Extraction_PrevZCX.Extr_Accel_Enabled_gt.Extr_Accel_CabHitLogic_Trig_ZCE[
            0] = trigger_j;
        Extraction_PrevZCX.Extr_Accel_Enabled_gt.Extr_Accel_CabHitLogic_Trig_ZCE[
            1] = Extraction_B.OutportBufferForExtr_Sup_EvResetSoftCabHitCnt;

        /* Logic: '<S31>/VibMapFiltData' incorporates:
         *  DataStoreRead: '<S31>/IsVibMapEnabled'
         */
        loopCond_j = Extr_Accel_Acquiring_j && ExtractionParams__IsVibMapEnabled
            () != 0;

        /* Outputs for Enabled SubSystem: '<S31>/AccelInputFilter' incorporates:
         *  EnablePort: '<S55>/Enable'
         */
        if (loopCond_j)
        {
            /* Sum: '<S55>/Subtract' */
            Extraction_B.Extr_Accel_Enabled_gt.Abs_iy[0] = Extr_Accel_FiltData[0]
                - Extr_Accel_Baseline[0];
            Extraction_B.Extr_Accel_Enabled_gt.Abs_iy[1] = Extr_Accel_FiltData[1]
                - Extr_Accel_Baseline[1];
            Extraction_B.Extr_Accel_Enabled_gt.Abs_iy[2] = Extr_Accel_FiltData[2]
                - Extr_Accel_Baseline[2];

            /* MATLAB Function: '<S55>/FilterAccelerometerData' */
            /* MATLAB Function 'Extraction/Extr_Accel/Extr_Accel_Enabled/Extr_Accel_Enabled/Extr_Accel_VibMap/AccelInputFilter/FilterAccelerometerData': '<S57>:1' */
            // Filter accelerometer raw data (X,Y,Z vector)();
            ExtractionParams__FilterVibMapAccel
                (Extraction_B.Extr_Accel_Enabled_gt.Abs_iy,
                 rtb_Extr_Accel_VibMapFiltAccel_le);

            /* Math: '<S55>/SquaredAcceleration'
             *
             * About '<S55>/SquaredAcceleration':
             *  Operator: magnitude^2
             */
            Extr_Accel_VibMapFiltAccel[0] = rtb_Extr_Accel_VibMapFiltAccel_le[0]
                * rtb_Extr_Accel_VibMapFiltAccel_le[0];
            Extr_Accel_VibMapFiltAccel[1] = rtb_Extr_Accel_VibMapFiltAccel_le[1]
                * rtb_Extr_Accel_VibMapFiltAccel_le[1];
            Extr_Accel_VibMapFiltAccel[2] = rtb_Extr_Accel_VibMapFiltAccel_le[2]
                * rtb_Extr_Accel_VibMapFiltAccel_le[2];
        }

        /* End of Outputs for SubSystem: '<S31>/AccelInputFilter' */

        /* Logic: '<S31>/VibMapEnabled' */
        loopCond_j = Extraction_B.OutportBufferForExtr_Sup_VibMapEnabled &&
            loopCond_j;

        /* Chart: '<S31>/Extr_Accel_VibMapLogic' */
        /* Gateway: Extraction/Extr_Accel/Extr_Accel_Enabled/Extr_Accel_Enabled/Extr_Accel_VibMap/Extr_Accel_VibMapLogic */
        if (temporalCounter_i1_j < MAX_uint32_T)
        {
            temporalCounter_i1_j++;
        }

        /* During: Extraction/Extr_Accel/Extr_Accel_Enabled/Extr_Accel_Enabled/Extr_Accel_VibMap/Extr_Accel_VibMapLogic */
        if (is_c26_Extraction_j == Extraction_IN_IDLE_ii)
        {
            /* During 'IDLE': '<S56>:2' */
            if (loopCond_j)
            {
                /* Transition: '<S56>:7' */
                is_c26_Extraction_j = Extraction_IN_RUNNING_iz;

                /* Entry 'RUNNING': '<S56>:8' */
                Extr_Accel_ExtPlateauTimeIncrement_nd_j = 0U;

                /* Entry Internal 'RUNNING': '<S56>:8' */
                /* Transition: '<S56>:55' */
                is_RUNNING_j = Extraction_IN_MONITORING_SPEED_dr;
                Extr_Accel_VibMapState_j = VIB_MAP_STATE_MONITORING_SPEED;
            }
        }
        else
        {
            /* During 'RUNNING': '<S56>:8' */
            if (!loopCond_j)
            {
                /* Transition: '<S56>:26' */
                /* Exit Internal 'RUNNING': '<S56>:8' */
                /* Exit Internal 'MONITORING': '<S56>:56' */
                is_MONITORING_j = Extraction_IN_NO_ACTIVE_CHILD_pf;
                is_RUNNING_j = Extraction_IN_NO_ACTIVE_CHILD_pf;
                is_c26_Extraction_j = Extraction_IN_IDLE_ii;
                Extr_Accel_VibMapState_j = VIB_MAP_STATE_IDLE;
            }
            else if (is_RUNNING_j == Extraction_IN_MONITORING_my)
            {
                /* During 'MONITORING': '<S56>:56' */
                /* % REQ 304198 - Calculate floor displacement
                   % REQ 292850 - Calculate floor displacement threshold
                   % REQ 309738 - Check if reference displacement has not been calculated
                   % REQ 304107 - Update vibration mapping reference displacement for the current spin pulse index */
                if (Extr_Inputs_MotorSignals.Speed < (float32)
                        (*GESE_Extr_VibMapSetup).Velocity_Enable_Rpm_Div10 *
                        10.0F)
                {
                    /* Transition: '<S56>:63' */
                    /* Exit Internal 'MONITORING': '<S56>:56' */
                    is_MONITORING_j = Extraction_IN_NO_ACTIVE_CHILD_pf;
                    is_RUNNING_j = Extraction_IN_MONITORING_SPEED_dr;
                    Extr_Accel_VibMapState_j = VIB_MAP_STATE_MONITORING_SPEED;
                }
                else
                {
                    loopCond_j = Extr_Accel_VibMapFiltAccel[0] > (float32)
                        (*GESE_Extr_VibMapSetup).Accel_Threshold_X_x250 / 250.0F
                        || Extr_Accel_VibMapFiltAccel[1] > (float32)
                        (*GESE_Extr_VibMapSetup).Accel_Threshold_Y_x250 / 250.0F
                        || Extr_Accel_VibMapFiltAccel[2] > (float32)
                        (*GESE_Extr_VibMapSetup).Accel_Threshold_Z_x250 / 250.0F;

                    /* Outputs for Function Call SubSystem: '<S56>/CalcFloorDispThreshold' */

                    /* Simulink Function 'CalcFloorDispThreshold': '<S56>:154' */
                    Extraction_CalcFloorDispThreshold(&Extr_Inputs_MotorSignals,
                        (&((*GESE_Extr_VibMapFDThreshold))),
                        &Extraction_B.Extr_Accel_Enabled_gt.CalcFloorDispThreshold);

                    /* End of Outputs for SubSystem: '<S56>/CalcFloorDispThreshold' */
                    Extr_Accel_VibMapFloorDispThreshold_j =
                        Extraction_B.Extr_Accel_Enabled_gt.CalcFloorDispThreshold.Sum;
                    switch (is_MONITORING_j)
                    {
                      case Extraction_IN_FD_THRESHOLD_CROSSED:
                        /* During 'FD_THRESHOLD_CROSSED': '<S56>:159' */
                        /* Transition: '<S56>:160' */
                        is_MONITORING_j =
                            Extraction_IN_MONITORING_FLOOR_DISPLACEMENT;
                        Extr_Accel_VibMapState_j =
                            VIB_MAP_STATE_MONITORING_FLOOR_DISPLACEMENT;

                        /* Entry 'MONITORING_FLOOR_DISPLACEMENT': '<S56>:68' */
                        Extr_Accel_VibMapFloorDisp_j =
                            Extraction_CalcFloorDisplacement
                            (Extr_Accel_VibMapFiltAccel,
                             (*GESE_Extr_FloorDispSetup).FD_Calc_Friction_Coeff,
                             (*GESE_Extr_FloorDispSetup).
                             FD_Calc_Velocity_Ref_Coeff,
                             (*GESE_Extr_FloorDispSetup).
                             FD_Calc_Velocity_Actual_Coeff,
                             (*GESE_Extr_FloorDispSetup).FD_Calc_Accel_X_Coeff,
                             (*GESE_Extr_FloorDispSetup).FD_Calc_Accel_Y_Coeff,
                             (*GESE_Extr_FloorDispSetup).FD_Calc_Accel_XX_Coeff,
                             (*GESE_Extr_FloorDispSetup).FD_Calc_Accel_YX_Coeff,
                             (*GESE_Extr_FloorDispSetup).FD_Calc_Accel_ZX_Coeff,
                             (*GESE_Extr_FloorDispSetup).FD_Calc_Accel_YY_Coeff,
                             (*GESE_Extr_FloorDispSetup).FD_Calc_Accel_ZY_Coeff,
                             (*GESE_Extr_FloorDispSetup).FD_Calc_Accel_ZZ_Coeff,
                             (*GESE_Extr_FloorDispSetup).FD_Calc_Intercept,
                             Extr_Inputs_ParamEstData.Friction,
                             Extr_Inputs_MotorSignals.Speed,
                             Extr_Inputs_MotorSignals.Speed_Ref);
                        if (Extr_Accel_VibMapFloorDisp_j >
                                Extr_Accel_MaxVibMapFloorDisp_j)
                        {
                            Extr_Accel_MaxVibMapFloorDisp_j =
                                Extr_Accel_VibMapFloorDisp_j;
                        }

                        Extraction_UpdateVibMapRefFloorDisp
                            ((*GESE_Extr_VibMapSetup).Percentage_Of_Current,
                             (*GESE_Extr_VibMapSetup).
                             Reference_Update_Limit_x100,
                             Extr_Accel_VibMapFloorDisp_j,
                             Extraction_B.Extr_HighSpd_SpinPulse.Target_Speed_Div10);
                        break;

                      case Extraction_IN_FILTER_WARM_UP_ig:
                        /* During 'FILTER_WARM_UP': '<S56>:162' */
                        tmp_j = 3U * ((uint16)FS_EXTRACTION);
                        if (tmp_j > 65535U)
                        {
                            tmp_j = 65535U;
                        }

                        if (temporalCounter_i1_j >= tmp_j)
                        {
                            /* Transition: '<S56>:163' */
                            is_MONITORING_j =
                                Extraction_IN_MONITORING_ACCELERATION;
                            Extr_Accel_VibMapState_j =
                                VIB_MAP_STATE_MONITORING_ACCELERATION;
                        }
                        break;

                      case Extraction_IN_MONITORING_ACCELERATION:
                        /* During 'MONITORING_ACCELERATION': '<S56>:70' */
                        if (loopCond_j)
                        {
                            /* Transition: '<S56>:69' */
                            is_MONITORING_j =
                                Extraction_IN_MONITORING_FLOOR_DISPLACEMENT;
                            Extr_Accel_VibMapState_j =
                                VIB_MAP_STATE_MONITORING_FLOOR_DISPLACEMENT;

                            /* Entry 'MONITORING_FLOOR_DISPLACEMENT': '<S56>:68' */
                            Extr_Accel_VibMapFloorDisp_j =
                                Extraction_CalcFloorDisplacement
                                (Extr_Accel_VibMapFiltAccel,
                                 (*GESE_Extr_FloorDispSetup).
                                 FD_Calc_Friction_Coeff,
                                 (*GESE_Extr_FloorDispSetup).
                                 FD_Calc_Velocity_Ref_Coeff,
                                 (*GESE_Extr_FloorDispSetup).
                                 FD_Calc_Velocity_Actual_Coeff,
                                 (*GESE_Extr_FloorDispSetup).
                                 FD_Calc_Accel_X_Coeff,
                                 (*GESE_Extr_FloorDispSetup).
                                 FD_Calc_Accel_Y_Coeff,
                                 (*GESE_Extr_FloorDispSetup).
                                 FD_Calc_Accel_XX_Coeff,
                                 (*GESE_Extr_FloorDispSetup).
                                 FD_Calc_Accel_YX_Coeff,
                                 (*GESE_Extr_FloorDispSetup).
                                 FD_Calc_Accel_ZX_Coeff,
                                 (*GESE_Extr_FloorDispSetup).
                                 FD_Calc_Accel_YY_Coeff,
                                 (*GESE_Extr_FloorDispSetup).
                                 FD_Calc_Accel_ZY_Coeff,
                                 (*GESE_Extr_FloorDispSetup).
                                 FD_Calc_Accel_ZZ_Coeff,
                                 (*GESE_Extr_FloorDispSetup).FD_Calc_Intercept,
                                 Extr_Inputs_ParamEstData.Friction,
                                 Extr_Inputs_MotorSignals.Speed,
                                 Extr_Inputs_MotorSignals.Speed_Ref);
                            if (Extr_Accel_VibMapFloorDisp_j >
                                    Extr_Accel_MaxVibMapFloorDisp_j)
                            {
                                Extr_Accel_MaxVibMapFloorDisp_j =
                                    Extr_Accel_VibMapFloorDisp_j;
                            }

                            Extraction_UpdateVibMapRefFloorDisp
                                ((*GESE_Extr_VibMapSetup).Percentage_Of_Current,
                                 (*GESE_Extr_VibMapSetup).
                                 Reference_Update_Limit_x100,
                                 Extr_Accel_VibMapFloorDisp_j,
                                 Extraction_B.Extr_HighSpd_SpinPulse.Target_Speed_Div10);
                        }
                        break;

                      default:
                        /* During 'MONITORING_FLOOR_DISPLACEMENT': '<S56>:68' */
                        if (!loopCond_j)
                        {
                            /* Transition: '<S56>:74' */
                            is_MONITORING_j =
                                Extraction_IN_MONITORING_ACCELERATION;
                            Extr_Accel_VibMapState_j =
                                VIB_MAP_STATE_MONITORING_ACCELERATION;
                        }
                        else if (Extr_Accel_VibMapFloorDisp_j >
                                 Extraction_B.Extr_Accel_Enabled_gt.CalcFloorDispThreshold.Sum)
                        {
                            /* Transition: '<S56>:87' */
                            is_MONITORING_j = Extraction_IN_FD_THRESHOLD_CROSSED;
                            Extr_Accel_VibMapState_j =
                                VIB_MAP_STATE_FD_THRESHOLD_CROSSED;

                            /* Entry 'FD_THRESHOLD_CROSSED': '<S56>:159' */
                            ExtractionParams__SetVibMapThrshCrossSpd
                                (Extr_Inputs_MotorSignals.Speed - (float32)
                                 (*GESE_Extr_VibMapSetup).
                                 Threshold_Cross_Offset_Rpm_Div10 * 10.0F);
                            for (i_j = 0; i_j < 25; i_j++)
                            {
                                Extraction_B.Extr_Accel_Enabled_gt.tmpFor_Extr_Accel_VibMapRefDispArray_x100
                                    [i_j] = ExtractionParams__GetVibMapRefDisp
                                    ( i_j );
                            }

                            Extr_Accel_VibMapSafeSpeed_div10_p5_j =
                                Extraction_CalcSafeSpeed
                                (ExtractionParams__GetVibMapThrshCrossSpd(),
                                 (*GESE_Extr_VibMapSetup).Max_Delta_Rpm_Div10,
                                 Extraction_B.Extr_Accel_Enabled_gt.tmpFor_Extr_Accel_VibMapRefDispArray_x100,
                                 GESE_Extr_VibMapRefSpeedArray_div10.N,
                                 (&(GESE_Extr_VibMapRefSpeedArray_div10.BP[0])),
                                 Extraction_B.Extr_HighSpd_SpinPulse.Target_Speed_Div10);
                            for (i_jb = 0; i_jb < 25; i_jb++)
                            {
                                ExtractionParams__SetVibMapRefDisp( i_jb ,
                                    Extraction_B.Extr_Accel_Enabled_gt.tmpFor_Extr_Accel_VibMapRefDispArray_x100
                                    [i_jb]);
                            }

                            Extr_Accel_ExtPlateauTimeIncrement_nd_j =
                                Extraction_CalcPlateauTimeIncrement
                                (Extraction_B.Extr_HighSpd_SpinPulse.Target_Speed_Div10,
                                 Extr_Accel_VibMapSafeSpeed_div10_p5_j,
                                 (*GESE_Extr_VibMapSetup).
                                 Plateau_Duration_Ext_M_x10,
                                 (*GESE_Extr_VibMapSetup).
                                 Plateau_Duration_Ext_B_Div10);
                            Extraction_SetFault
                                (EXTR_FAULT_ACCEL_VIB_MAP_FLOOR_DISP_EXCEEDED);

                            /* Event: '<S56>:88' */
                            Extr_Accel_EvExcessiveFloorDispEventCounter_j++;
                        }
                        else
                        {
                            Extr_Accel_VibMapFloorDisp_j =
                                Extraction_CalcFloorDisplacement
                                (Extr_Accel_VibMapFiltAccel,
                                 (*GESE_Extr_FloorDispSetup).
                                 FD_Calc_Friction_Coeff,
                                 (*GESE_Extr_FloorDispSetup).
                                 FD_Calc_Velocity_Ref_Coeff,
                                 (*GESE_Extr_FloorDispSetup).
                                 FD_Calc_Velocity_Actual_Coeff,
                                 (*GESE_Extr_FloorDispSetup).
                                 FD_Calc_Accel_X_Coeff,
                                 (*GESE_Extr_FloorDispSetup).
                                 FD_Calc_Accel_Y_Coeff,
                                 (*GESE_Extr_FloorDispSetup).
                                 FD_Calc_Accel_XX_Coeff,
                                 (*GESE_Extr_FloorDispSetup).
                                 FD_Calc_Accel_YX_Coeff,
                                 (*GESE_Extr_FloorDispSetup).
                                 FD_Calc_Accel_ZX_Coeff,
                                 (*GESE_Extr_FloorDispSetup).
                                 FD_Calc_Accel_YY_Coeff,
                                 (*GESE_Extr_FloorDispSetup).
                                 FD_Calc_Accel_ZY_Coeff,
                                 (*GESE_Extr_FloorDispSetup).
                                 FD_Calc_Accel_ZZ_Coeff,
                                 (*GESE_Extr_FloorDispSetup).FD_Calc_Intercept,
                                 Extr_Inputs_ParamEstData.Friction,
                                 Extr_Inputs_MotorSignals.Speed,
                                 Extr_Inputs_MotorSignals.Speed_Ref);
                            if (Extr_Accel_VibMapFloorDisp_j >
                                    Extr_Accel_MaxVibMapFloorDisp_j)
                            {
                                Extr_Accel_MaxVibMapFloorDisp_j =
                                    Extr_Accel_VibMapFloorDisp_j;
                            }

                            Extraction_UpdateVibMapRefFloorDisp
                                ((*GESE_Extr_VibMapSetup).Percentage_Of_Current,
                                 (*GESE_Extr_VibMapSetup).
                                 Reference_Update_Limit_x100,
                                 Extr_Accel_VibMapFloorDisp_j,
                                 Extraction_B.Extr_HighSpd_SpinPulse.Target_Speed_Div10);
                        }
                        break;
                    }
                }
            }
            else
            {
                /* During 'MONITORING_SPEED': '<S56>:54' */
                if (Extr_Inputs_MotorSignals.Speed >= (float32)
                        (*GESE_Extr_VibMapSetup).Velocity_Enable_Rpm_Div10 *
                        10.0F)
                {
                    /* Transition: '<S56>:57' */
                    is_RUNNING_j = Extraction_IN_MONITORING_my;

                    /* Outputs for Function Call SubSystem: '<S56>/CalcFloorDispThreshold' */

                    /* Entry 'MONITORING': '<S56>:56' */
                    /* Simulink Function 'CalcFloorDispThreshold': '<S56>:154' */
                    Extraction_CalcFloorDispThreshold(&Extr_Inputs_MotorSignals,
                        (&((*GESE_Extr_VibMapFDThreshold))),
                        &Extraction_B.Extr_Accel_Enabled_gt.CalcFloorDispThreshold);

                    /* End of Outputs for SubSystem: '<S56>/CalcFloorDispThreshold' */
                    Extr_Accel_VibMapFloorDispThreshold_j =
                        Extraction_B.Extr_Accel_Enabled_gt.CalcFloorDispThreshold.Sum;

                    /* Entry Internal 'MONITORING': '<S56>:56' */
                    /* Transition: '<S56>:73' */
                    is_MONITORING_j = Extraction_IN_FILTER_WARM_UP_ig;
                    temporalCounter_i1_j = 0U;
                    Extr_Accel_VibMapState_j = VIB_MAP_STATE_FILTER_WARM_UP;
                }
            }
        }

        if (Extr_Accel_EvExcessiveFloorDispEventCounter_j > 0U)
        {
            Extraction_B.VariantMergeForOutportExtr_Accel_VibMapEvExcessiveFloorDisp
                =
                !Extraction_B.VariantMergeForOutportExtr_Accel_VibMapEvExcessiveFloorDisp;
            Extr_Accel_EvExcessiveFloorDispEventCounter_j--;
        }

        /* End of Chart: '<S31>/Extr_Accel_VibMapLogic' */

        /* SignalConversion: '<S31>/ExtPlateauTimeIncrement' */
        Extr_Accel_ExtPlateauTimeIncrement =
            Extr_Accel_ExtPlateauTimeIncrement_nd_j;

        /* SignalConversion: '<S31>/VibMapSafeSpeed' */
        Extr_Accel_VibMapSafeSpeed_div10 = Extr_Accel_VibMapSafeSpeed_div10_p5_j;
        Extr_Accel_Acquiring = Extr_Accel_Acquiring_j;
        Extraction_B.Extr_Accel_Enabled_gt.Extr_Accel_ExtPlateauTimeIncrement_nd
            = Extr_Accel_ExtPlateauTimeIncrement_nd_j;
        Extraction_B.Extr_Accel_Enabled_gt.Extr_Accel_VibMapSafeSpeed_div10_p5 =
            Extr_Accel_VibMapSafeSpeed_div10_p5_j;
        Extraction_DW.Extr_Accel_Enabled_gt.is_active_c18_Extraction =
            is_active_c18_Extraction_j;
        Extraction_DW.Extr_Accel_Enabled_gt.is_c18_Extraction =
            is_c18_Extraction_j;
        Extr_Accel_State = Extr_Accel_State_j;
        Extraction_DW.Extr_Accel_Enabled_gt.is_SUPERVISOR_ENABLED =
            is_SUPERVISOR_ENABLED_j;
        Extr_Accel_CommErrors = Extr_Accel_CommErrors_j;
        Extr_Accel_MaxAccel = Extr_Accel_MaxAccel_j;
        Extraction_DW.Extr_Accel_Enabled_gt.temporalCounter_i1 =
            temporalCounter_i1_j;
        Extraction_DW.Extr_Accel_Enabled_gt.is_RUNNING = is_RUNNING_j;
        Extr_Accel_VibMapState = Extr_Accel_VibMapState_j;
        Extraction_DW.Extr_Accel_Enabled_gt.is_c26_Extraction =
            is_c26_Extraction_j;
        Extraction_DW.Extr_Accel_Enabled_gt.is_MONITORING = is_MONITORING_j;
        Extr_Accel_VibMapFloorDisp = Extr_Accel_VibMapFloorDisp_j;
        Extr_Accel_MaxVibMapFloorDisp = Extr_Accel_MaxVibMapFloorDisp_j;
        Extraction_DW.Extr_Accel_Enabled_gt.Extr_Accel_EvExcessiveFloorDispEventCounter
            = Extr_Accel_EvExcessiveFloorDispEventCounter_j;
        Extr_Accel_VibMapFloorDispThreshold =
            Extr_Accel_VibMapFloorDispThreshold_j;
    }

#endif                                 /* EXTR_VARIANT_ACCELEROMETER_ENABLED */

    /* End of Outputs for SubSystem: '<S9>/Extr_Accel_Enabled' */

    /* End of Outputs for RootInportFunctionCallGenerator: '<Root>/RootFcnCall_InsertedFor_Extraction__Handler5ms_at_outport_1' */
}

/* Model step function */
void Extraction__Start(void)
{
    boolean_T ACU_ExtrStart;
    uint32 ACU_ExtrStartEventCounter;
    ACU_ExtrStartEventCounter = Extraction_DW.ACU_ExtrStartEventCounter;
    ACU_ExtrStart = Extraction_B.ACU_ExtrStart;

    /* RootInportFunctionCallGenerator: '<Root>/RootFcnCall_InsertedFor_Extraction__Start_at_outport_1' incorporates:
     *  SubSystem: '<S13>/StartCommand'
     */
    /* Chart: '<S197>/EventGen' */
    /* Gateway: Extraction/Extr_Supervisor/StartCommand/EventGen */
    /* During: Extraction/Extr_Supervisor/StartCommand/EventGen */
    /* Entry Internal: Extraction/Extr_Supervisor/StartCommand/EventGen */
    /* Transition: '<S211>:2' */
    /* Event: '<S211>:3' */
    ACU_ExtrStartEventCounter++;
    if (ACU_ExtrStartEventCounter > 0U)
    {
        ACU_ExtrStart = !ACU_ExtrStart;
        ACU_ExtrStartEventCounter--;
    }

    /* End of Chart: '<S197>/EventGen' */

    /* SignalConversion: '<S197>/OutportBufferForACU_ExtrStart' */
    Extraction_B.OutportBufferForACU_ExtrStart = ACU_ExtrStart;

    /* End of Outputs for RootInportFunctionCallGenerator: '<Root>/RootFcnCall_InsertedFor_Extraction__Start_at_outport_1' */
    Extraction_B.ACU_ExtrStart = ACU_ExtrStart;
    Extraction_DW.ACU_ExtrStartEventCounter = ACU_ExtrStartEventCounter;
}

/* Model step function */
void Extraction__Stop(void)
{
    boolean_T ACU_ExtrStop;
    uint32 ACU_ExtrStopEventCounter;
    ACU_ExtrStopEventCounter = Extraction_DW.ACU_ExtrStopEventCounter;
    ACU_ExtrStop = Extraction_B.ACU_ExtrStop;

    /* RootInportFunctionCallGenerator: '<Root>/RootFcnCall_InsertedFor_Extraction__Stop_at_outport_1' incorporates:
     *  SubSystem: '<S13>/StopCommand'
     */
    /* Chart: '<S198>/EventGen' */
    /* Gateway: Extraction/Extr_Supervisor/StopCommand/EventGen */
    /* During: Extraction/Extr_Supervisor/StopCommand/EventGen */
    /* Entry Internal: Extraction/Extr_Supervisor/StopCommand/EventGen */
    /* Transition: '<S212>:2' */
    /* Event: '<S212>:3' */
    ACU_ExtrStopEventCounter++;
    if (ACU_ExtrStopEventCounter > 0U)
    {
        ACU_ExtrStop = !ACU_ExtrStop;
        ACU_ExtrStopEventCounter--;
    }

    /* End of Chart: '<S198>/EventGen' */

    /* SignalConversion: '<S198>/OutportBufferForACU_ExtrStop' */
    Extraction_B.OutportBufferForACU_ExtrStop = ACU_ExtrStop;

    /* End of Outputs for RootInportFunctionCallGenerator: '<Root>/RootFcnCall_InsertedFor_Extraction__Stop_at_outport_1' */
    Extraction_B.ACU_ExtrStop = ACU_ExtrStop;
    Extraction_DW.ACU_ExtrStopEventCounter = ACU_ExtrStopEventCounter;
}

/* Model step function */
void Extraction__Resume(void)
{
    boolean_T ACU_ExtrResume;
    uint32 ACU_ExtrResumeEventCounter;
    ACU_ExtrResumeEventCounter = Extraction_DW.ACU_ExtrResumeEventCounter;
    ACU_ExtrResume = Extraction_B.ACU_ExtrResume;

    /* RootInportFunctionCallGenerator: '<Root>/RootFcnCall_InsertedFor_Extraction__Resume_at_outport_1' incorporates:
     *  SubSystem: '<S13>/ResumeCommand'
     */
    /* Chart: '<S196>/EventGen' */
    /* Gateway: Extraction/Extr_Supervisor/ResumeCommand/EventGen */
    /* During: Extraction/Extr_Supervisor/ResumeCommand/EventGen */
    /* Entry Internal: Extraction/Extr_Supervisor/ResumeCommand/EventGen */
    /* Transition: '<S210>:2' */
    /* Event: '<S210>:3' */
    ACU_ExtrResumeEventCounter++;
    if (ACU_ExtrResumeEventCounter > 0U)
    {
        ACU_ExtrResume = !ACU_ExtrResume;
        ACU_ExtrResumeEventCounter--;
    }

    /* End of Chart: '<S196>/EventGen' */

    /* SignalConversion: '<S196>/OutportBufferForACU_ExtrResume' */
    Extraction_B.OutportBufferForACU_ExtrResume = ACU_ExtrResume;

    /* End of Outputs for RootInportFunctionCallGenerator: '<Root>/RootFcnCall_InsertedFor_Extraction__Resume_at_outport_1' */
    Extraction_B.ACU_ExtrResume = ACU_ExtrResume;
    Extraction_DW.ACU_ExtrResumeEventCounter = ACU_ExtrResumeEventCounter;
}

/* Model step function */
void Extraction__End(void)
{
    boolean_T ACU_ExtrEnd;
    uint32 ACU_ExtrEndEventCounter;
    ACU_ExtrEndEventCounter = Extraction_DW.ACU_ExtrEndEventCounter;
    ACU_ExtrEnd = Extraction_B.ACU_ExtrEnd;

    /* RootInportFunctionCallGenerator: '<Root>/RootFcnCall_InsertedFor_Extraction__End_at_outport_1' incorporates:
     *  SubSystem: '<S13>/EndCommand'
     */
    /* Chart: '<S192>/EventGen' */
    /* Gateway: Extraction/Extr_Supervisor/EndCommand/EventGen */
    /* During: Extraction/Extr_Supervisor/EndCommand/EventGen */
    /* Entry Internal: Extraction/Extr_Supervisor/EndCommand/EventGen */
    /* Transition: '<S199>:2' */
    /* Event: '<S199>:3' */
    ACU_ExtrEndEventCounter++;
    if (ACU_ExtrEndEventCounter > 0U)
    {
        ACU_ExtrEnd = !ACU_ExtrEnd;
        ACU_ExtrEndEventCounter--;
    }

    /* End of Chart: '<S192>/EventGen' */

    /* SignalConversion: '<S192>/OutportBufferForACU_ExtrEnd' */
    Extraction_B.OutportBufferForACU_ExtrEnd = ACU_ExtrEnd;

    /* End of Outputs for RootInportFunctionCallGenerator: '<Root>/RootFcnCall_InsertedFor_Extraction__End_at_outport_1' */
    Extraction_B.ACU_ExtrEnd = ACU_ExtrEnd;
    Extraction_DW.ACU_ExtrEndEventCounter = ACU_ExtrEndEventCounter;
}

/* Model step function */
void Extraction__ResetLoadMass(void)
{
    boolean_T ACU_ResetLoadMass;
    uint32 ACU_ResetLoadMassEventCounter;
    ACU_ResetLoadMassEventCounter = Extraction_DW.ACU_ResetLoadMassEventCounter;
    ACU_ResetLoadMass = Extraction_B.ACU_ResetLoadMass;

    /* RootInportFunctionCallGenerator: '<Root>/RootFcnCall_InsertedFor_Extraction__ResetLoadMass_at_outport_1' incorporates:
     *  SubSystem: '<S13>/ResetLoadMass'
     */
    /* Chart: '<S195>/EventGen' */
    /* Gateway: Extraction/Extr_Supervisor/ResetLoadMass/EventGen */
    /* During: Extraction/Extr_Supervisor/ResetLoadMass/EventGen */
    /* Entry Internal: Extraction/Extr_Supervisor/ResetLoadMass/EventGen */
    /* Transition: '<S209>:2' */
    /* Event: '<S209>:3' */
    ACU_ResetLoadMassEventCounter++;
    if (ACU_ResetLoadMassEventCounter > 0U)
    {
        ACU_ResetLoadMass = !ACU_ResetLoadMass;
        ACU_ResetLoadMassEventCounter--;
    }

    /* End of Chart: '<S195>/EventGen' */

    /* SignalConversion: '<S195>/OutportBufferForACU_ResetLoadMass' */
    Extraction_B.OutportBufferForACU_ResetLoadMass = ACU_ResetLoadMass;

    /* End of Outputs for RootInportFunctionCallGenerator: '<Root>/RootFcnCall_InsertedFor_Extraction__ResetLoadMass_at_outport_1' */
    Extraction_B.ACU_ResetLoadMass = ACU_ResetLoadMass;
    Extraction_DW.ACU_ResetLoadMassEventCounter = ACU_ResetLoadMassEventCounter;
}

/* Model step function */
void Extraction__NotifyFoamDetection(void)
{
    boolean_T NotifyFoamDetection;
    uint32 NotifyFoamDetectionEventCounter;
    NotifyFoamDetectionEventCounter =
        Extraction_DW.NotifyFoamDetectionEventCounter;
    NotifyFoamDetection = Extraction_B.NotifyFoamDetection;

    /* RootInportFunctionCallGenerator: '<Root>/RootFcnCall_InsertedFor_Extraction__NotifyFoamDetection_at_outport_1' incorporates:
     *  SubSystem: '<S13>/NotifyFoamDetection'
     */
    /* Chart: '<S194>/EventGen' */
    /* Gateway: Extraction/Extr_Supervisor/NotifyFoamDetection/EventGen */
    /* During: Extraction/Extr_Supervisor/NotifyFoamDetection/EventGen */
    /* Entry Internal: Extraction/Extr_Supervisor/NotifyFoamDetection/EventGen */
    /* Transition: '<S208>:2' */
    /* Event: '<S208>:3' */
    NotifyFoamDetectionEventCounter++;
    if (NotifyFoamDetectionEventCounter > 0U)
    {
        NotifyFoamDetection = !NotifyFoamDetection;
        NotifyFoamDetectionEventCounter--;
    }

    /* End of Chart: '<S194>/EventGen' */

    /* SignalConversion: '<S194>/OutportBufferForACU_NotifyFoamDetection' */
    Extraction_B.OutportBufferForACU_NotifyFoamDetection = NotifyFoamDetection;

    /* End of Outputs for RootInportFunctionCallGenerator: '<Root>/RootFcnCall_InsertedFor_Extraction__NotifyFoamDetection_at_outport_1' */
    Extraction_B.NotifyFoamDetection = NotifyFoamDetection;
    Extraction_DW.NotifyFoamDetectionEventCounter =
        NotifyFoamDetectionEventCounter;
}

/* Model initialize function */
void Extraction_initialize(void)
{
    {
        sint32 i;
        for (i = 0; i < 18; i++)
        {
            Extraction_PrevZCX.Extr_Sup_Logic_Trig_ZCE[i] = UNINITIALIZED_ZCSIG;
        }

        Extraction_PrevZCX.Extr_Sup_MotorSpeedCheck_Trig_ZCE[0] =
            UNINITIALIZED_ZCSIG;
        Extraction_PrevZCX.Extr_Sup_MotorSpeedCheck_Trig_ZCE[1] =
            UNINITIALIZED_ZCSIG;
        for (i = 0; i < 20; i++)
        {
            Extraction_PrevZCX.Extr_LowSpd_SupervisorLogic_Trig_ZCE[i] =
                UNINITIALIZED_ZCSIG;
        }

        Extraction_PrevZCX.Extr_LowSpd_BasicUnbCheck_Trig_ZCE[0] =
            UNINITIALIZED_ZCSIG;
        Extraction_PrevZCX.Extr_LowSpd_BasicUnbCheck_Trig_ZCE[1] =
            UNINITIALIZED_ZCSIG;
        Extraction_PrevZCX.Extr_LowSpd_BasicUnbCheck_Trig_ZCE[2] =
            UNINITIALIZED_ZCSIG;
        Extraction_PrevZCX.Extr_LowSpd_LoadMeasLogic_Trig_ZCE[0] =
            UNINITIALIZED_ZCSIG;
        Extraction_PrevZCX.MDDPLogic_Trig_ZCE[0] = UNINITIALIZED_ZCSIG;
        Extraction_PrevZCX.Extr_HighSpd_ExtUnbCheckLogic_Trig_ZCE[0] =
            UNINITIALIZED_ZCSIG;
        Extraction_PrevZCX.Extr_LowSpd_LoadMeasLogic_Trig_ZCE[1] =
            UNINITIALIZED_ZCSIG;
        Extraction_PrevZCX.MDDPLogic_Trig_ZCE[1] = UNINITIALIZED_ZCSIG;
        Extraction_PrevZCX.Extr_HighSpd_ExtUnbCheckLogic_Trig_ZCE[1] =
            UNINITIALIZED_ZCSIG;
        Extraction_PrevZCX.Extr_LowSpd_LoadMeasLogic_Trig_ZCE[2] =
            UNINITIALIZED_ZCSIG;
        Extraction_PrevZCX.MDDPLogic_Trig_ZCE[2] = UNINITIALIZED_ZCSIG;
        Extraction_PrevZCX.Extr_HighSpd_ExtUnbCheckLogic_Trig_ZCE[2] =
            UNINITIALIZED_ZCSIG;
        Extraction_PrevZCX.Extr_LowSpd_LoadMeasLogic_Trig_ZCE[3] =
            UNINITIALIZED_ZCSIG;
        Extraction_PrevZCX.MDDPLogic_Trig_ZCE[3] = UNINITIALIZED_ZCSIG;
        Extraction_PrevZCX.Extr_HighSpd_ExtUnbCheckLogic_Trig_ZCE[3] =
            UNINITIALIZED_ZCSIG;
        Extraction_PrevZCX.Extr_Rebal_Logic_Trig_ZCE[0] = UNINITIALIZED_ZCSIG;
        Extraction_PrevZCX.Extr_Rebal_Logic_Trig_ZCE[1] = UNINITIALIZED_ZCSIG;
        Extraction_PrevZCX.Extr_Rebal_Logic_Trig_ZCE[2] = UNINITIALIZED_ZCSIG;
        Extraction_PrevZCX.Extr_Rebal_Logic_Trig_ZCE[3] = UNINITIALIZED_ZCSIG;
        Extraction_PrevZCX.Extr_Rebal_Logic_Trig_ZCE[4] = UNINITIALIZED_ZCSIG;
        Extraction_PrevZCX.Extr_Rebal_Logic_Trig_ZCE[5] = UNINITIALIZED_ZCSIG;
        Extraction_PrevZCX.Extr_Rebal_Logic_Trig_ZCE[6] = UNINITIALIZED_ZCSIG;
        Extraction_PrevZCX.Extr_Rebal_Logic_Trig_ZCE[7] = UNINITIALIZED_ZCSIG;
        Extraction_PrevZCX.Extr_Rebal_Logic_Trig_ZCE[8] = UNINITIALIZED_ZCSIG;
        Extraction_PrevZCX.Extr_Rebal_Logic_Trig_ZCE[9] = UNINITIALIZED_ZCSIG;
        Extraction_PrevZCX.Extr_Rebal_Logic_Trig_ZCE[10] = UNINITIALIZED_ZCSIG;

#if EXTR_VARIANT_ACCELEROMETER_ENABLED

        Extraction_PrevZCX.Extr_Accel_Enabled_gt.Extr_Accel_CabHitLogic_Trig_ZCE[
            0] = UNINITIALIZED_ZCSIG;
        Extraction_PrevZCX.Extr_Accel_Enabled_gt.Extr_Accel_CabHitLogic_Trig_ZCE[
            1] = UNINITIALIZED_ZCSIG;

#endif                                 /* EXTR_VARIANT_ACCELEROMETER_ENABLED */

#if EXTR_VARIANT_HIGH_SPEED_PE_ENABLED

        Extraction_PrevZCX.Extr_HighSpd_RampLogic_Enabled.Extr_HighSpd_RampLogic_Trig_ZCE
            [0] = UNINITIALIZED_ZCSIG;
        Extraction_PrevZCX.Extr_HighSpd_RampLogic_Enabled.Extr_HighSpd_RampLogic_Trig_ZCE
            [1] = UNINITIALIZED_ZCSIG;
        Extraction_PrevZCX.Extr_HighSpd_RampLogic_Enabled.Extr_HighSpd_RampLogic_Trig_ZCE
            [2] = UNINITIALIZED_ZCSIG;
        Extraction_PrevZCX.Extr_HighSpd_RampLogic_Enabled.Extr_HighSpd_RampLogic_Trig_ZCE
            [3] = UNINITIALIZED_ZCSIG;
        Extraction_PrevZCX.Extr_HighSpd_RampLogic_Enabled.Extr_HighSpd_RampLogic_Trig_ZCE
            [4] = UNINITIALIZED_ZCSIG;
        Extraction_PrevZCX.Extr_HighSpd_RampLogic_Enabled.Extr_HighSpd_RampLogic_Trig_ZCE
            [5] = UNINITIALIZED_ZCSIG;

#endif                                 /* EXTR_VARIANT_HIGH_SPEED_PE_ENABLED */

#if EXTR_VARIANT_HIGH_SPEED_PE_DISABLED

        Extraction_PrevZCX.Extr_HighSpd_RampLogic_Disabled.Extr_HighSpd_RampLogic_Trig_ZCE_ir
            [0] = UNINITIALIZED_ZCSIG;
        Extraction_PrevZCX.Extr_HighSpd_RampLogic_Disabled.Extr_HighSpd_RampLogic_Trig_ZCE_ir
            [1] = UNINITIALIZED_ZCSIG;
        Extraction_PrevZCX.Extr_HighSpd_RampLogic_Disabled.Extr_HighSpd_RampLogic_Trig_ZCE_ir
            [2] = UNINITIALIZED_ZCSIG;
        Extraction_PrevZCX.Extr_HighSpd_RampLogic_Disabled.Extr_HighSpd_RampLogic_Trig_ZCE_ir
            [3] = UNINITIALIZED_ZCSIG;
        Extraction_PrevZCX.Extr_HighSpd_RampLogic_Disabled.Extr_HighSpd_RampLogic_Trig_ZCE_ir
            [4] = UNINITIALIZED_ZCSIG;
        Extraction_PrevZCX.Extr_HighSpd_RampLogic_Disabled.Extr_HighSpd_RampLogic_Trig_ZCE_ir
            [5] = UNINITIALIZED_ZCSIG;

#endif                                 /* EXTR_VARIANT_HIGH_SPEED_PE_DISABLED */

#if EXTR_VARIANT_HIGH_SPEED_PE_ENABLED

        Extraction_PrevZCX.Extr_HighSpd_PlateauLogic_Enabled.Extr_HighSpd_PlateauLogic_Trig_ZCE
            [0] = UNINITIALIZED_ZCSIG;
        Extraction_PrevZCX.Extr_HighSpd_PlateauLogic_Enabled.Extr_HighSpd_PlateauLogic_Trig_ZCE
            [1] = UNINITIALIZED_ZCSIG;
        Extraction_PrevZCX.Extr_HighSpd_PlateauLogic_Enabled.Extr_HighSpd_PlateauLogic_Trig_ZCE
            [2] = UNINITIALIZED_ZCSIG;
        Extraction_PrevZCX.Extr_HighSpd_PlateauLogic_Enabled.Extr_HighSpd_PlateauLogic_Trig_ZCE
            [3] = UNINITIALIZED_ZCSIG;
        Extraction_PrevZCX.Extr_HighSpd_PlateauLogic_Enabled.Extr_HighSpd_PlateauLogic_Trig_ZCE
            [4] = UNINITIALIZED_ZCSIG;

#endif                                 /* EXTR_VARIANT_HIGH_SPEED_PE_ENABLED */

#if EXTR_VARIANT_HIGH_SPEED_PE_DISABLED

        Extraction_PrevZCX.Extr_HighSpd_PlateauLogic_Disabled.Extr_HighSpd_PlateauLogic_Trig_ZCE_lg
            [0] = UNINITIALIZED_ZCSIG;
        Extraction_PrevZCX.Extr_HighSpd_PlateauLogic_Disabled.Extr_HighSpd_PlateauLogic_Trig_ZCE_lg
            [1] = UNINITIALIZED_ZCSIG;
        Extraction_PrevZCX.Extr_HighSpd_PlateauLogic_Disabled.Extr_HighSpd_PlateauLogic_Trig_ZCE_lg
            [2] = UNINITIALIZED_ZCSIG;
        Extraction_PrevZCX.Extr_HighSpd_PlateauLogic_Disabled.Extr_HighSpd_PlateauLogic_Trig_ZCE_lg
            [3] = UNINITIALIZED_ZCSIG;
        Extraction_PrevZCX.Extr_HighSpd_PlateauLogic_Disabled.Extr_HighSpd_PlateauLogic_Trig_ZCE_lg
            [4] = UNINITIALIZED_ZCSIG;

#endif                                 /* EXTR_VARIANT_HIGH_SPEED_PE_DISABLED */

#if EXTR_VARIANT_HIGH_SPEED_PE_ENABLED

        Extraction_PrevZCX.Extr_HighSpd_LoadMassLimitCheck_Enabled.Extr_HighSpd_LoadMassLimitCheckLogic_Trig_ZCE
            [0] = UNINITIALIZED_ZCSIG;
        Extraction_PrevZCX.Extr_HighSpd_CEM_Enabled.Extr_HighSpd_CEMLogic_Trig_ZCE
            [0] = UNINITIALIZED_ZCSIG;
        Extraction_PrevZCX.Extr_HighSpd_LoadMassLimitCheck_Enabled.Extr_HighSpd_LoadMassLimitCheckLogic_Trig_ZCE
            [1] = UNINITIALIZED_ZCSIG;
        Extraction_PrevZCX.Extr_HighSpd_CEM_Enabled.Extr_HighSpd_CEMLogic_Trig_ZCE
            [1] = UNINITIALIZED_ZCSIG;
        for (i = 0; i < 19; i++)
        {
            Extraction_PrevZCX.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.Extr_HighSpd_SupervisorLogic_Trig_ZCE
                [i] = UNINITIALIZED_ZCSIG;
        }

#endif                                 /* EXTR_VARIANT_HIGH_SPEED_PE_ENABLED */

#if EXTR_VARIANT_HIGH_SPEED_PE_DISABLED

        for (i = 0; i < 19; i++)
        {
            Extraction_PrevZCX.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.Extr_HighSpd_SupervisorLogic_Trig_ZCE_cb
                [i] = UNINITIALIZED_ZCSIG;
        }

#endif                                 /* EXTR_VARIANT_HIGH_SPEED_PE_DISABLED */

#if EXTR_VARIANT_OPT_RAMP_ENABLED

        Extraction_PrevZCX.Extr_LowSpd_OptRamp_Enabled.OptRampLogic_Trig_ZCE[0] =
            UNINITIALIZED_ZCSIG;
        Extraction_PrevZCX.Extr_LowSpd_OptRamp_Enabled.OptRampLogic_Trig_ZCE[1] =
            UNINITIALIZED_ZCSIG;
        Extraction_PrevZCX.Extr_LowSpd_OptRamp_Enabled.OptRampLogic_Trig_ZCE[2] =
            UNINITIALIZED_ZCSIG;

#endif                                 /* EXTR_VARIANT_OPT_RAMP_ENABLED */

        /* SystemInitialize for RootInportFunctionCallGenerator: '<Root>/RootFcnCall_InsertedFor_Extraction__Handler5ms_at_outport_1' incorporates:
         *  SystemInitialize for SubSystem: '<S13>/Extr_Supervisor'
         */
        /* SystemInitialize for Chart: '<S193>/Extr_Sup_Logic' */
        /* Entry: Extraction/Extr_Supervisor/Extr_Supervisor/Extr_Sup_Logic */
        /* Entry Internal: Extraction/Extr_Supervisor/Extr_Supervisor/Extr_Sup_Logic */
        /* Transition: '<S201>:87' */
        /* Entry 'EXTRACTION': '<S201>:74' */
        /* Entry Internal 'EXTRACTION': '<S201>:74' */
        /* Transition: '<S201>:10' */
        Extraction_DW.is_EXTRACTION = Extraction_IN_IDLE_kf;
        Extraction_B.Extr_Sup_State_d3 = EXTRACTION_STATE_IDLE;

        /* SystemInitialize for Chart: '<S193>/Extr_Sup_MotorSpeedCheck' */
        Extr_Sup_SpeedErrorMonitorState =
            EXTR_SUP_SPEED_ERROR_MONITOR_STATE_NONE;

        /* SystemInitialize for RootInportFunctionCallGenerator: '<Root>/RootFcnCall_InsertedFor_Extraction__Handler5ms_at_outport_1' incorporates:
         *  SystemInitialize for SubSystem: '<S11>/Extr_LowSpd_Supervisor'
         */
        /* SystemInitialize for Chart: '<S122>/Extr_LowSpd_SupervisorLogic' */
        Extraction_DW.sfEvent_f2 = -1;

        /* SystemInitialize for RootInportFunctionCallGenerator: '<Root>/RootFcnCall_InsertedFor_Extraction__Handler5ms_at_outport_1' incorporates:
         *  SystemInitialize for SubSystem: '<S11>/Extr_LowSpd_Algorithms'
         */
        /* SystemInitialize for Chart: '<S125>/Extr_LowSpd_LoadMeasLogic' */
        /* Entry: Extraction/Extr_LowSpd/Extr_LowSpd_Supervisor/Extr_LowSpd_SupervisorLogic */
        /* Entry Internal: Extraction/Extr_LowSpd/Extr_LowSpd_Supervisor/Extr_LowSpd_SupervisorLogic */
        /* Transition: '<S171>:212' */
        /* Entry 'LOW_SPEED_EXTRACTION': '<S171>:155' */
        /* Entry Internal 'LOW_SPEED_EXTRACTION': '<S171>:155' */
        /* Transition: '<S171>:21' */
        /* Entry 'IDLE': '<S171>:13' */
        /* Event: '<S171>:254' */
        Extraction_DW.sfEvent_ak = -1;

        /* Entry: Extraction/Extr_LowSpd/Extr_LowSpd_Algorithms/Extr_LowSpd_LoadMeas/Extr_LowSpd_LoadMeasLogic */
        /* Entry Internal: Extraction/Extr_LowSpd/Extr_LowSpd_Algorithms/Extr_LowSpd_LoadMeas/Extr_LowSpd_LoadMeasLogic */
        /* Transition: '<S145>:173' */
        Extraction_B.Extr_LowSpd_LoadMass_ec = -1.0F;
        Extraction_DW.is_c9_Extraction = Extraction_IN_IDLE_gt;
        Extr_LowSpd_LoadMeasState = LOAD_MEAS_STATE_IDLE;

        /* SystemInitialize for Chart: '<S124>/Extr_LowSpd_BasicUnbCheck' */
        Extr_LowSpd_UnbChkSpdLimit = MAX_uint16_T;

        /* SystemInitialize for Chart: '<S126>/MDDPLogic' */
        Extraction_DW.sfEvent_o4 = -1;

        /* Entry: Extraction/Extr_LowSpd/Extr_LowSpd_Algorithms/Extr_LowSpd_MDDP/MDDPLogic */
        /* Entry Internal: Extraction/Extr_LowSpd/Extr_LowSpd_Algorithms/Extr_LowSpd_MDDP/MDDPLogic */
        /* Transition: '<S147>:173' */
        Extraction_DW.is_c11_Extraction = Extraction_IN_IDLE_gt;
        Extr_LowSpd_MddpState = MDDP_STATE_IDLE;

        /* SystemInitialize for Atomic SubSystem: '<S121>/Extr_LowSpd_OptRamp' */
#if EXTR_VARIANT_OPT_RAMP_ENABLED

        /* System initialize for atomic system: '<S127>/Extr_LowSpd_OptRamp_Enabled' */

        /* SystemInitialize for Chart: '<S150>/OptRampLogic' */
        Extraction_DW.Extr_LowSpd_OptRamp_Enabled.sfEvent = -1;

        /* Entry: Extraction/Extr_LowSpd/Extr_LowSpd_Algorithms/Extr_LowSpd_OptRamp/Extr_LowSpd_OptRamp_Enabled/OptRampLogic */
        /* Entry Internal: Extraction/Extr_LowSpd/Extr_LowSpd_Algorithms/Extr_LowSpd_OptRamp/Extr_LowSpd_OptRamp_Enabled/OptRampLogic */
        /* Transition: '<S151>:173' */
        Extraction_DW.Extr_LowSpd_OptRamp_Enabled.is_c13_Extraction =
            Extraction_IN_IDLE;
        Extr_LowSpd_OptRampState = OPT_RAMP_STATE_IDLE;

#endif                                 /* EXTR_VARIANT_OPT_RAMP_ENABLED */

        /* End of SystemInitialize for SubSystem: '<S121>/Extr_LowSpd_OptRamp' */

        /* SystemInitialize for RootInportFunctionCallGenerator: '<Root>/RootFcnCall_InsertedFor_Extraction__Handler5ms_at_outport_1' incorporates:
         *  SystemInitialize for SubSystem: '<S62>/Extr_HighSpd_Supervisor'
         */

        /* SystemInitialize for Atomic SubSystem: '<S85>/Extr_HighSpd_SupervisorLogic' */
#if EXTR_VARIANT_HIGH_SPEED_PE_DISABLED

        /* System initialize for atomic system: '<S86>/Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled' */

        /* SystemInitialize for Chart: '<S92>/Extr_HighSpd_SupervisorLogic' */
        Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstDisabled.Extr_HighSpd_State_lz
            = HIGH_SPEED_STATE_NONE;
        Extraction_B.VariantMergeForOutportExtr_HighSpd_EvStartPlateau = false;
        Extraction_B.VariantMergeForOutportExtr_HighSpd_EvStartRamp = false;
        Extraction_B.VariantMergeForOutportExtr_HighSpd_EvComplete = false;
        Extraction_B.VariantMergeForOutportExtr_HighSpd_EvStartRebal = false;
        Extraction_B.VariantMergeForOutportExtr_HighSpd_EvStopAll = false;
        Extraction_B.VariantMergeForOutportExtr_HighSpd_EvDropToDistSpeed =
            false;

#elif EXTR_VARIANT_HIGH_SPEED_PE_ENABLED

        /* System initialize for atomic system: '<S86>/Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled' */

        /* SystemInitialize for Chart: '<S93>/Extr_HighSpd_SupervisorLogic' */
        Extraction_B.Extr_HighSpd_SupervisorLogic_HighSpeedParamEstEnabled.Extr_HighSpd_State_la
            = HIGH_SPEED_STATE_NONE;
        Extraction_B.VariantMergeForOutportExtr_HighSpd_EvStartPlateau = false;
        Extraction_B.VariantMergeForOutportExtr_HighSpd_EvStartRamp = false;
        Extraction_B.VariantMergeForOutportExtr_HighSpd_EvComplete = false;
        Extraction_B.VariantMergeForOutportExtr_HighSpd_EvStartRebal = false;
        Extraction_B.VariantMergeForOutportExtr_HighSpd_EvStopAll = false;
        Extraction_B.VariantMergeForOutportExtr_HighSpd_EvDropToDistSpeed =
            false;

#endif                                 /* EXTR_VARIANT_HIGH_SPEED_PE_DISABLED */

        /* End of SystemInitialize for SubSystem: '<S85>/Extr_HighSpd_SupervisorLogic' */

        /* SystemInitialize for RootInportFunctionCallGenerator: '<Root>/RootFcnCall_InsertedFor_Extraction__Handler5ms_at_outport_1' incorporates:
         *  SystemInitialize for SubSystem: '<S10>/Extr_HighSpd_Algorithms'
         */

        /* SystemInitialize for Atomic SubSystem: '<S61>/Extr_HighSpd_PlateauLogic' */
#if EXTR_VARIANT_HIGH_SPEED_PE_DISABLED

        /* System initialize for atomic system: '<S66>/Extr_HighSpd_PlateauLogic_Disabled' */

        /* SystemInitialize for Chart: '<S74>/Extr_HighSpd_PlateauLogic' */
        Extr_HighSpd_PlateauState = HIGH_SPEED_PLATEAU_STATE_NONE;
        Extraction_B.VariantMergeForOutportExtr_HighSpd_EvPlateauComplete =
            false;

#elif EXTR_VARIANT_HIGH_SPEED_PE_ENABLED

        /* System initialize for atomic system: '<S66>/Extr_HighSpd_PlateauLogic_Enabled' */

        /* SystemInitialize for Chart: '<S75>/Extr_HighSpd_PlateauLogic' */
        Extr_HighSpd_PlateauState = HIGH_SPEED_PLATEAU_STATE_NONE;
        Extraction_B.VariantMergeForOutportExtr_HighSpd_EvPlateauComplete =
            false;

#endif                                 /* EXTR_VARIANT_HIGH_SPEED_PE_DISABLED */

        /* End of SystemInitialize for SubSystem: '<S61>/Extr_HighSpd_PlateauLogic' */

        /* SystemInitialize for Atomic SubSystem: '<S61>/Extr_HighSpd_RampLogic' */
#if EXTR_VARIANT_HIGH_SPEED_PE_DISABLED

        /* System initialize for atomic system: '<S67>/Extr_HighSpd_RampLogic_Disabled' */

        /* SystemInitialize for Chart: '<S79>/Extr_HighSpd_RampLogic' */
        Extr_HighSpd_RampState = HIGH_SPEED_RAMP_STATE_NONE;
        Extraction_B.VariantMergeForOutportExtr_HighSpd_EvRampComplete = false;

#elif EXTR_VARIANT_HIGH_SPEED_PE_ENABLED

        /* System initialize for atomic system: '<S67>/Extr_HighSpd_RampLogic_Enabled' */

        /* SystemInitialize for Chart: '<S80>/Extr_HighSpd_RampLogic' */
        Extr_HighSpd_RampState = HIGH_SPEED_RAMP_STATE_NONE;
        Extraction_B.VariantMergeForOutportExtr_HighSpd_EvRampComplete = false;

#endif                                 /* EXTR_VARIANT_HIGH_SPEED_PE_DISABLED */

        /* End of SystemInitialize for SubSystem: '<S61>/Extr_HighSpd_RampLogic' */

        /* SystemInitialize for Atomic SubSystem: '<S61>/Extr_HighSpd_CEM' */
#if EXTR_VARIANT_HIGH_SPEED_PE_ENABLED

        /* System initialize for atomic system: '<S63>/Extr_HighSpd_CEM_Enabled' */

        /* SystemInitialize for Chart: '<S69>/Extr_HighSpd_CEMLogic' */
        Extr_HighSpd_CEMState = CEM_STATE_NONE;

#endif                                 /* EXTR_VARIANT_HIGH_SPEED_PE_ENABLED */

        /* End of SystemInitialize for SubSystem: '<S61>/Extr_HighSpd_CEM' */

        /* SystemInitialize for Chart: '<S64>/Extr_HighSpd_ExtUnbCheckLogic' */
        Extraction_DW.doneDoubleBufferReInit = 0;
        Extraction_DW.sfEvent_pr = -1;

        /* SystemInitialize for RootInportFunctionCallGenerator: '<Root>/RootFcnCall_InsertedFor_Extraction__Handler5ms_at_outport_1' incorporates:
         *  SystemInitialize for SubSystem: '<S2>/Extr_Rebalance'
         */
        /* SystemInitialize for Chart: '<S12>/Extr_Rebal_Logic' */
        /* Entry: Extraction/Extr_HighSpd/Extr_HighSpd_Algorithms/Extr_HighSpd_ExtUnbChk/Extr_HighSpd_ExtUnbCheckLogic */
        /* Entry Internal: Extraction/Extr_HighSpd/Extr_HighSpd_Algorithms/Extr_HighSpd_ExtUnbChk/Extr_HighSpd_ExtUnbCheckLogic */
        /* Transition: '<S71>:2' */
        /* Entry 'IDLE': '<S71>:1' */
        Extr_Rebal_State = EXTR_REBAL_STATE_NONE;

        /* SystemInitialize for RootInportFunctionCallGenerator: '<Root>/RootFcnCall_InsertedFor_Extraction__Handler5ms_at_outport_1' incorporates:
         *  SystemInitialize for SubSystem: '<S2>/Extr_Accel'
         */

        /* SystemInitialize for Atomic SubSystem: '<S9>/Extr_Accel_Enabled' */
#if EXTR_VARIANT_ACCELEROMETER_ENABLED

        /* System initialize for atomic system: '<S26>/Extr_Accel_Enabled' */

        /* SystemInitialize for Chart: '<S30>/Extr_Accel_Supervisor' */
        Extr_Accel_State = EXTR_ACCEL_STATE_NONE;

        /* SystemInitialize for Chart: '<S29>/Extr_Accel_CabHitLogic' */
        /* Entry: Extraction/Extr_Accel/Extr_Accel_Enabled/Extr_Accel_Enabled/Extr_Accel_CabHit/Extr_Accel_CabHitLogic */
        /* Entry Internal: Extraction/Extr_Accel/Extr_Accel_Enabled/Extr_Accel_Enabled/Extr_Accel_CabHit/Extr_Accel_CabHitLogic */
        /* Transition: '<S34>:3' */
        Extraction_DW.Extr_Accel_Enabled_gt.is_c33_Extraction =
            Extraction_IN_IDLE_ii;
        Extr_Accel_CabHitState = CAB_HIT_STATE_IDLE;

        /* SystemInitialize for Chart: '<S31>/Extr_Accel_VibMapLogic' */
        /* Entry 'IDLE': '<S34>:2' */
        /* Entry: Extraction/Extr_Accel/Extr_Accel_Enabled/Extr_Accel_Enabled/Extr_Accel_VibMap/Extr_Accel_VibMapLogic */
        /* Entry Internal: Extraction/Extr_Accel/Extr_Accel_Enabled/Extr_Accel_Enabled/Extr_Accel_VibMap/Extr_Accel_VibMapLogic */
        /* Transition: '<S56>:3' */
        Extraction_DW.Extr_Accel_Enabled_gt.is_c26_Extraction =
            Extraction_IN_IDLE_ii;
        Extr_Accel_VibMapState = VIB_MAP_STATE_IDLE;

#endif                                 /* EXTR_VARIANT_ACCELEROMETER_ENABLED */

        /* End of SystemInitialize for SubSystem: '<S9>/Extr_Accel_Enabled' */

        /* End of SystemInitialize for RootInportFunctionCallGenerator: '<Root>/RootFcnCall_InsertedFor_Extraction__Handler5ms_at_outport_1' */

        /* SystemInitialize for S-Function (sfun_private_function_caller): '<S2>/sfcn_inserted_server10' incorporates:
         *  SystemInitialize for SubSystem: '<S2>/UpdateParamEstSetup'
         */
        Extraction_UpdateParamEstSetup_Init();

        /* End of SystemInitialize for S-Function (sfun_private_function_caller): '<S2>/sfcn_inserted_server10' */

        /* SystemInitialize for RootInportFunctionCallGenerator: '<Root>/RootFcnCall_InsertedFor_Extraction__Handler5ms_at_outport_1' incorporates:
         *  SystemInitialize for SubSystem: '<S11>/Extr_LowSpd_Supervisor'
         */
        /* SystemInitialize for Chart: '<S122>/Extr_LowSpd_SupervisorLogic' */
        Extraction_DW.is_LOW_SPEED_EXTRACTION = Extraction_IN_IDLE_jy;
        Extraction_DW.is_c15_Extraction = Extraction_IN_LOW_SPEED_EXTRACTION;
        Extraction_DW.Extr_LowSpd_EvStopEventCounter = 1U;
        Extr_LowSpd_State = LOW_SPEED_STATE_IDLE;

        /* SystemInitialize for RootInportFunctionCallGenerator: '<Root>/RootFcnCall_InsertedFor_Extraction__Handler5ms_at_outport_1' incorporates:
         *  SystemInitialize for SubSystem: '<S10>/Extr_HighSpd_Algorithms'
         */
        /* SystemInitialize for Chart: '<S64>/Extr_HighSpd_ExtUnbCheckLogic' */
        Extraction_DW.is_c45_Extraction = Extraction_IN_IDLE_gt;
        Extr_HighSpd_ExtUnbState = EXT_UNB_CHECK_STATE_IDLE;
        Extraction_B.Extr_HighSpd_ExtUnbCalculated = true;

        /* End of SystemInitialize for RootInportFunctionCallGenerator: '<Root>/RootFcnCall_InsertedFor_Extraction__Handler5ms_at_outport_1' */
    }
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
