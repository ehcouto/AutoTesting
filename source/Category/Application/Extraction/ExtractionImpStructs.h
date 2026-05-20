/**
 *  @file
 *
 *  @brief   Extraction hard-coded parameters
 *
 *
 *  @copyright  Copyright 2017  $  Whirlpool Corporation.  All rights reserved - CONFIDENTIAL.
 */

#ifndef EXTRACTIONIMPSTRUCTS_H
#define EXTRACTIONIMPSTRUCTS_H

#include "C_Extensions.h"
#include "Compiler_defs.h"

//=====================================================================================================================
//-------------------------------------- PUBLIC (Extern Variables, Constants & Defines) -------------------------------
//=====================================================================================================================

//! Configuration structure for the distribution pulse parameters.
typedef PACKED struct
{
	sint8 Target_Speed_Div2;	    //! Target speed divided by 2.
	uint8 Ramp_Time_x10;		    //! Ramp Time in seconds.
	uint8 On_Time:7;			    //! On time in seconds. Time where the motor will be spinning at Target_Speed.
	uint8 On_Time_Scaling:1;	    //! Prescaler bit for the On Time
									//! 0 = On Time is in 1 s increments
									//! 1 = On Time is in 5 s increments
	uint8 Off_Time:7;			    //! Off time in seconds. Time where the motor will be stopped by the end of the pulse.
	uint8 Off_Time_Scaling:1;	    //! Prescaler bit for the Off Time
									//! 0 = On Time is in 1 s increments
									//! 1 = On Time is in 5 s increments
	uint8 Dist_Ramp_Enabled:1;    	//! Enable/Disable Distribution Ramp for current pulse
	uint8 Unused:7;			    	//! Unused bits
} DISTRIBUTION_PULSE_TYPE;

//! Extraction setup structure.
typedef PACKED struct
{
	uint8 Distribution_Profile_Index;			//!< Displacement for the Distribution Profile.
	uint8 Spin_Profile_Index;					//!< Displacement for the Spin Profile data.
	uint8 Rebalance_Table_Index;				//!< Displacement for the Rebalance Table.
	uint8 DR_Enabled      :1;					//!< Enable/Disable Distribution Ramp for the Extraction Set..
	uint8 OR_Enabled      :1;					//!< Enable/Disable Optimum Ramp for the Extraction Set.
	uint8 MDDP_Enabled    :1;                   //!< Enable/Disable MDDP for the Extraction Set.
	uint8 LMS_Enabled     :1;					//!< Enable/Disable averaged low speed load mass measurement.
	uint8 CWLD_Enabled    :1;                   //!< Enable/Disable Clean Washer Load Detection
	uint8 ESM_Disabled	  :1;                   //!< Keep the motor running when extraction ends
												//!< 0 = Stop motor
												//!< 1 = Don't stop motor
	uint8 CM_Enabled      :1;                   //!< The type of source that should be used to measure the unbalance level
	uint8 PE_Enabled      :1;					//!< 0 = Raw Motor Measurement (PE = 0, CM = 0)
												//!< 1 = Converted Motor Measurement (PE = 0, CM = 1)
												//!< 2 = Parameter Estimation Measurement (PE = 1, CM = 0)
	uint8 Basic_Unbalance_Table_Index;
	uint8 Extended_Unbalance_Table_Index;
	uint8 Optimum_Ramp_Unbalance_Table_Index;
	uint8 Load_Mass_Count_Average :4;			//!< The number of PE measurements to be taken to calculate load mass.
	uint8 Padding_Byte_7          :4;			//!< 4-bit padding.
} EXTRACTION_SETUP_TYPE;

/* Setup structure for Fast Distribution Ramp. */
typedef PACKED struct
{
    /* Max peak to peak torque value. */
    uint16 Max_Torque_Disturbance_x100;

    /* Defines the number of torque updates before starting to verify if the load is distributed. */
    uint8 P2P_Torque_Updates_Initial;

    /* Defines the number of torque updates before starting to verify if the load is distributed after a short redistribution is executed. */
    uint8 P2P_Torque_Updates_Short_Redist;

    /* Delta drop speed in rpm. */
    uint8 Short_Redist_Delta_Speed_Drop;

    /* Minimum speed when trying to distribute the load. */
    uint8 Short_Redist_Min_Velocity;

    /* Maximum number of short distributions. When the counter reaches this value it means the FDR module failed to distribute the load. */
    uint8 Short_Redist_Count_Max;

    /* Displacement index for the DISTR_RAMP_LIMIT_DRUM_VELOCITY Table */
	uint8 Limit_Drum_Velocity_Table;

    /* Displacement index for the DISTR_RAMP_LIMIT_LOAD_MASS_kg Table */
    uint8 Limit_Load_Mass_Kg_Table;

    /* Displacement index for the DISTR_RAMP_DISTURBANCE_THRESHOLD Table */
    uint8 Disturbance_Threshold_Table;
} FDR_SETUP_TYPE;

/* Setup structure for Optimum Ramp. */
typedef PACKED struct
{
    /* Minimum ball balancer moment (converted to a mass spinning over a nominal radius)
       needed to ramp at optimum time. */
    uint16 BB_Magnitude_Threshold;

    /* Displacement for the OPT_RAMP_LIMIT_SUMP_TEMP_C Table */
    uint8 Limit_Sump_Temp_C_Table;

    /* Displacement for the OPT_RAMP_BB_PHASE_ADVANCE_TARGET Table */
	uint8 BB_Phase_Adv_Target_Table;

	/* Displacement for the OPT_RAMP_BB_PHASE_ADVANCE_TOLERANCE Table */
	uint8 BB_Phase_Adv_Tolerance_Table;

	/* Displacement for the OPT_RAMP_LIMIT_TIME_PLATEAU_CONSTANT Table */
	uint8 Limit_Time_Plateau_Const_Table;

	/* Displacement for the OPT_RAMP_LIMIT_TIME_CONVERGENCE Table */
	uint8 Limit_Time_Convergence_Table;

	/* Padding byte */
    uint8 Padding;
} OPT_RAMP_SETUP_TYPE;

/* Setup structure for Mass Dependent Dewatering Plateau. */
typedef PACKED struct
{
    /* Decelerate to distribution speed after the MDDP plateau or proceed
       directly to the next step. */
    uint8 Decelerate_To_Dist_Speed:1;

    uint8 Unused:7;

    /* Displacement for the MDDP_PLATEAU_LOAD_MASS_kg Table */
	uint8 Plateau_Load_Mass_Kg_Table;

    /* Displacement for the MDDP_PLATEAU_DURATION_s Table */
	uint8 Plateau_Duration_S_Table;

	/* Displacement for the MDDP_PLATEAU_VELOCITY_rpm Table */
	uint8 Plateau_Velocity_Rpm_Table;

	/* Displacement for the MDDP_RAMP_RATE_rpm/s Table */
	uint8 Ramp_Rate_Rpm_S_Table;

	uint8 Padding;
} MDDP_SETUP_TYPE;

//! Prelookup data structure for unsigned short arrays.
typedef struct
{
	uint16 N;				//! Number of elements on pre-lookup array
	uint16 *BP;		//! Pointer to the pre-lookup data
} PRE_LOOKUP_TYPE_UINT16;

//! Prelookup data structure for unsigned char arrays.
typedef struct
{
	uint8 N;				//! Number of elements on pre-lookup array
	uint8 *BP;		//! Pointer to the pre-lookup data
} PRE_LOOKUP_TYPE_UINT8;

typedef struct
{
    uint16 Count;
    uint16 *Converted_Values;
    uint16 *Raw_Values;
} BALANCE_INDEX_CONVERSION_TABLE_TYPE;

/* Configuration structure that holds the spin pulse parameters. */
typedef PACKED struct
{
    /* Target speed divided by 10. */
    uint8 Target_Speed_Div10;
    uint8 Dist_Time;

    /* Ramp Time in seconds.
       This vector contains the min and max times, which will be interpolated based on the load mass. */
    uint8 Ramp_Time[2];

    /* Plateau time in seconds. Time where the motor will remain in the target speed.
       This vector contains the min and max times, which will be interpolated based on the load mass. */
    uint8 Plateau_Time[2];

    /* Parameter Estimation configuration index. */
    uint8 Param_Est_Config_Set_Index;

    /* Parameter Estimation application-specific configuration index. */
    uint8 Param_Est_App_Specific_Config_Index;

    /* High speed algorithms configuration index. */
    uint8 High_Speed_Config_Index;

    /* If Parameter_Estimation_Friction_Nm is greater than this percentage of
       High_Speed_Friction_Baseline_Nm the control abandons the spin. */
    uint8 CEM_Plateau_Threshold;

    /* Baseline update configuration index. */
    uint8 Baseline_Config_Index;

    /* Displacement index for the Load Mass Limit.
     *  Check Pulse Configuration Table in MCU_LOAD_MASS_LIMIT_CHECK_DATA_PTR */
    uint8 Load_Mass_Config_Index;
} SPIN_PULSE_TYPE;

typedef PACKED struct
{
	/* Byte 0 */
	/* Padding bits. */
    uint8 Padding_Bits_01:7;

    /* Enable/Disable Factory Calibration. */
    uint8 Factory_Calibration_Enabled:1;

	/* Byte 1 */
    /* Enable/Disable High Speed Extraction. */
    uint8 High_Speed_Enabled:1;

    /* Enable/Disable Inertia Dependent Ramp and Plateau. */
    uint8 Inertia_Dependent_Profile_Enabled:1;

    /* Enable/Disable load Mass Limit Check */
    uint8 Load_Mass_Limit_Check_Enabled:1;

    /* Padding bits. */
    uint8 Padding_Bits_02:5;

	/* Byte 2 */
    /* Number of spin pulses in the profile. */
    uint8 Spin_Pulse_Count;

	/* Byte 3 */
    uint8 Unused;

    /* Vector consisting of two load mass values [min;max] multiplied by 10.
       Those values are used for plateau and ramp interpolation. */
    uint16 Load_Mass_x10[2];
} SPIN_PROFILE_TYPE;

/* Setup structure for high speed extraction. It holds plateau and ramp configuration. */
typedef PACKED struct
{
    /* The value of Parameter_Estimation_Viscous above which the control will
       execute Ramp Logic. */
    float32 Ramp_Viscous_Friction_Threshold;

    /* The maximum range of HIGH_SPEED_FRICTION_AVERAGE_SAMPLES samples
       of High_Speed_Friction_Window_Average_Nmunder which the Plateau Logic can
       make a decision to advance, monitor, hold, etc. */
    uint16 Plateau_Settling_Threshold_x1000;

    /* The percent of High_Speed_Friction_Baseline_Nm that
       Parameter_Estimation_Friction_Nm must be under for the control
       to proceed to the next Pulse in the Spin Profile. */
    uint8 Plateau_Advance_Threshold;

    /* The percent of High_Speed_Friction_Baseline_Nm that
       Parameter_Estimation_Friction_Nm must be under for the control to
       continue to monitor for the friction to fall below the advance threshold. */
    uint8 Plateau_Monitor_Threshold;

    /* The percent of High_Speed_Friction_Baseline_Nm that
       Parameter_Estimation_Friction_Nm must be under for the control to
       hold the current drum velocity. */
    uint8 Plateau_Hold_Threshold;

    /* The maximum updates to High_Speed_Friction_Average_Nm before the plateau times out. */
    uint8 Plateau_Max_Averages;

    /* The number of samples of High_Speed_Friction_Delta_Nm to
	   wait before proceeding to the final plateau. */
    uint8 Plateau_Monitor_Samples;
} HIGH_SPEED_PULSE_CONFIG_TYPE;

/* Defines the polynomial coefficients to convert inertia to mass,
   according tothe equation: mass = A^2*J + B*J + C */
typedef PACKED struct
{
    /* First coefficient in the load mass calculation equation. */
    float32 A;

    /* Second coefficient in the load mass calculation equation. */
    float32 B;

    /* Third coefficient in the load mass calculation equation. */
    float32 C;
} MASS_CONVERSION_COEFFS_TYPE;

/* Defines the coefficients used to convert unbalance moment (mgr) to grams (UB)
   according to the equation: UB = (K1 + (1000*K2*mgr))*K4+K3. */
typedef PACKED struct
{
    /* Drum radius (nominal value). */
    float32 K2;

    /* A scaling factor to apply to the normalized offset Parameter_Estimation_UBMag. */
    float32 K4;

    /* Offset of Parameter Estimation output Parameter_Estimation_UBMag. */
    sint16 K1;

    /* An offset to apply a safety buffer to the normalized, scaled Parameter_Estimation_UBMag. */
    sint16 K3;
} UB_CONVERSION_COEFFS_TYPE;

typedef PACKED struct
{
    /* Displacement for the selected Parameter Estimation Configuration
       Set Table (in MCU_PARAMETER_ESTIMATION_PTR). */
    uint8 Param_Est_Setup_Index;

    /* Displacement for the selected Parameter Estimation Application-Specific
       Configuration Set Table (in MCU_PARAMETER_ESTIMATION_PTR). */
    uint8 Param_Est_App_Specific_Setup_Index;
} PARAM_EST_SETUP_INDEX_TYPE;

typedef PACKED struct
{
    /* The value with which to replace the Parameter Estimation covariance matrix main diagonal when a covariance reset is executed. */
    uint16 Covariance_Matrix_Default;

    /* The time the control waits after resetting covariances prior to evaluating Parameter Estimation outputs */
    uint8 Cov_Reset_Wait_x10;

    /* The time the control waits after resetting covariances prior to evaluating Parameter Estimation outputs */
    uint8 Stabilization_Time;
} PARAM_EST_APP_SPECIFIC_SETUP_TYPE;

/* Setup structure for high speed extraction. It holds plateau and ramp configuration. */
typedef PACKED struct
{
    /* The number of samples of High_Speed_Friction_Window_Average_Nm
       included in the average for High_Speed_Friction_Average_Nm. */
    uint8 Friction_Average_Samples;

    /* The number of drum revolutions worth of Parameter_Estimation_Friction_Nm
       included in the average for High_Speed_Friction_Window_Average_Nm. */
    uint8 Friction_Drum_Revolutions;

    /* The drum velocity the control will drop once it detects that
       Parameter_Estimation_Viscous is greater than or equal to
       HIGH_SPEED_RAMP_VISCOUS_THRESHOLD. */
    uint8 Delta_Speed_Drop;

    /* The velocity to be reduced if there is a need in the Extended Plateau. */
    uint8 Ext_Plateau_Delta_Speed_Drop_Div5;

    /* Reference velocity for the calculation of High_Speed_Friction_Average_Reference_Nm
       used for the calculation of High_Speed_Friction_Delta_Nm */
    uint8 Reference_Plateau_Velocity_Div10;
} HIGH_SPEED_CONFIG_TYPE;

/* Encapsulates the configuration parameters for updating the friction baseline. */
typedef PACKED struct
{
    /* Specifies the maximum value for High_Speed_Friction_Baseline_Nm.
       Any calculation that exceeds this value is clamped to this value. */
    uint16 Friction_Max_x1000;

    /* Specifies the minimum value for High_Speed_Friction_Baseline_Nm.
       Any calculation that falls below this value is clamped to this value. */
    uint16 Friction_Min_x1000;

    /* If Parameter_Estimation_Friction_Nmis underneath this percentage of
       High_Speed_Friction_Baseline_Nm then High_Speed_Friction_Baseline_Nm is updated. */
    uint8 Baseline_Update_Threshold;

    /* The percentage of the difference between High_Speed_Friction_Baseline_N
       and High_Speed_Friction_Average_Nm used for the update to
       High_Speed_Friction_Baseline_Nm. */
    uint8 Percent_Of_Current_Threshold;

    /* The maximum number of redistributions from the same pulse
       before forcing an update to High_Speed_Friction_Baseline_Nm. */
    uint8 Max_Redistributions;
} HIGH_SPEED_BASELINE_CONFIG_TYPE;

/* Encapsulates the configuration parameters for Load Mass Limit Check algorithm. */
typedef PACKED struct
{
    /* Load_Mass_kg limit above which, during the Spin Profile, the control will abandon
       the Pulse, drop the drum velocity and execute the Extended Plateau. */
    uint16 Threshold_x10;

    /* The period of time between calculations of Load_Mass_kg during the Spin Profile. */
    uint8 Delta_Time_x10;

    /* Padding byte. */
    uint8 Padding;
} LOAD_MASS_LIMIT_SETUP_TYPE;

typedef PACKED struct
{
    /* The highest value of Rebalance_Spin_Attempts before the control
       executes the EXTRACTION_RECOVERY_PROFILE_INDEX, or,
       if High-Speed Extraction is enabled, the threshold in which the control
       accepts a lower spin speed in favor of mitigating cycle time. */
    uint8 Max_Redistributions;
} HIGH_SPEED_UNB_SETUP_TYPE;

typedef PACKED struct
{
    /* The number of samples of Parameter_Estimation_Friction_Nm
       to use in order to set the value of Baseline_Calibration_Friction_Nm. */
    uint8 Plateau_Samples;

    /* The number of samples of High_Speed_Friction_Window_Average_Nm
       included in the average for High_Speed_Friction_Average_Nm
       when Factory Calibration is enabled. */
    uint8 Average_Samples;

    /* The number of drum revolutions worth of Parameter_Estimation_Friction_Nm
       included in the average for High_Speed_Friction_Window_Average_Nm
       when Factory Calibration is enabled. */
    uint8 Friction_Drum_Revolutions;

	/* Displacement index for the BASELINE_REFERENCE_VELOCITY Table */
	uint8 Reference_Velocity_Table;

	/* Displacement index for the BASELINE_FACTORY_FRICTION_SELECTION_INDEX Table */
	uint8 Factory_Friction_Sel_Index_Table;

	/* Displacement index for the BASELINE_UPDATE_DEFAULT_FRICTION Table */
	uint8 Default_Friction_Table;

	/* Displacement index for the BASELINE_FACTORY_CALIBRATION_SCALING_FACTOR Table */
	uint8 Factory_Cal_Scaling_Factor_Table;

	uint8 Padding;
} BASELINE_FACTORY_CALIBRATION_SETUP_TYPE;

typedef PACKED struct
{
    /* The drum velocity above which a CEM event is likely triggered by drum-to-tub contact instead of clothing drag. */
    uint8 Clothing_Drag_Max_Velocity_Div10;

    /* The number of drum revolutions worth of Parameter_Estimation_Friction_Nm
       included in the average for CEM_Friction_Window_Average_Nm. */
    uint8 Friction_Window_Drum_Rev;

    /* The number of samples of High_Speed_Friction_Window_Average_Nm
       included in the average for CEM_Friction_Average_Nm. */
    uint8 Friction_Avg_Samples;

    /* A static percentage (x10) to reduce High_Speed_Friction_Baseline_Nm
       as a result of a CEM trigger. */
    uint8 Static_Decrement_x10;

    /* A static percentage (x10) to increase High_Speed_Friction_Baseline_Nm
       as a result of a CEM trigger. */
    uint8 Static_Increment_x10;
} CEM_SETUP_TYPE;

typedef PACKED struct
{
	/* The coefficient of the Floor Displacement Threshold Characteristic. */
	float32 Displacement_Threshold_A;

	/* The exponent of the Floor Displacement Threshold Characteristic. */
	float32 Displacement_Threshold_B;

	/* The offset of the Floor Displacement Threshold Characteristic. */
	float32 Displacement_Threshold_C;
} VIB_MAP_FLOOR_DISPL_THRESHOLD_TYPE;

typedef PACKED struct
{
	/* The minimum value of Accelerometer_Filtered_X for the Floor Displacement Limit Check. */
	uint16 Accel_Threshold_X_x250;

	/* The minimum value of Accelerometer_Filtered_Y for the Floor Displacement Limit Check. */
	uint16 Accel_Threshold_Y_x250;

	/* The minimum value of Accelerometer_Filtered_Z for the Floor Displacement Limit Check. */
	uint16 Accel_Threshold_Z_x250;

	/* The maximum RPM delta for dropping the drum velocity in Extended Plateau as a result of the
	   Floor Displacement Limit Check.*/
	uint8 Max_Delta_Rpm_Div10;

	/* The 1st-order coefficient for the determination of the additional Extended Plateau duration. */
	sint8 Plateau_Duration_Ext_M_x10;

	/* The offset for the determination of the additional Extended Plateau duration. */
	sint8 Plateau_Duration_Ext_B_Div10;

	/* The percentage of the existing value of VM_Displacement_Reference_mm and the new value
	   of VM_Floor_Displacement_mm for the calculation of the new value of the former. */
	uint8 Percentage_Of_Current;

	/* The maximum difference between VM_Displacement_Reference_mm and VM_Floor_Displacement_mm
	   for the control to update the former.*/
	uint8 Reference_Update_Limit_x100;

	/* The delta velocity from the velocity at which the control recognizes that VM_Floor_Displacement_mm
	   exceeds VM_Displacement_Threshold_mm.*/
	uint8 Threshold_Cross_Offset_Rpm_Div10;

	/* The drum velocity that the drum must be at or above in order to evaluate the
	   Floor Displacement Limit Check.*/
	uint8 Velocity_Enable_Rpm_Div10;

	/* The velocity below which the control will dwell at the current velocity if VM_Displacement_Reference_mm
	   is greater than VM_Displacement_Threshold_mm in the next Pulse.*/
	uint8 Velocity_Dwell_Threshold_Div10;

	/* Displacement index for the Vibration Mapping Acceleromenter Filter Configuration (in MCU_EXTRACTION_ DYNAMIC_TABLES_PTR).
	   255 means filter disabled. */
	uint8 Accel_Filter_Config_Index;

	/* Padding byte */
	uint8 Padding;
} VIBRATION_MAPPING_SETUP_TYPE;

typedef PACKED struct
{
	/* The Parameter_Estimation_Friction_Nm coefficient for the VM_Floor_Displacement_mm calculation. */
	float32 FD_Calc_Friction_Coeff;

	/* The Target Velocity coefficient for the VM_Floor_Displacement_mm calculation. */
	float32 FD_Calc_Velocity_Ref_Coeff;

	/* The Actual Drum Velocity coefficient for the VM_Floor_Displacement_mm calculation. */
	float32 FD_Calc_Velocity_Actual_Coeff;

	/* The coefficient applied to VM_Filtered_Accelerometer_X for the VM_Floor_Displacement_mm calculation. */
	float32 FD_Calc_Accel_X_Coeff;

	/* The coefficient applied to VM_Filtered_Accelerometer_Y for the VM_Floor_Displacement_mm calculation. */
	float32 FD_Calc_Accel_Y_Coeff;

	/* The coefficient applied to VM_Filtered_Accelerometer_Z for the VM_Floor_Displacement_mm calculation. */
	float32 FD_Calc_Accel_Z_Coeff;

	/* The coefficient applied to VM_Filtered_Accelerometer_X2 for the VM_Floor_Displacement_mm calculation. */
	float32 FD_Calc_Accel_XX_Coeff;

	/* The coefficient applied to VM_Filtered_Accelerometer_Y*VM_Filtered_Accelerometer_X for the
	   VM_Floor_Displacement_mm calculation. */
	float32 FD_Calc_Accel_YX_Coeff;

	/* The coefficient applied to VM_Filtered_Accelerometer_Z*VM_Filtered_Accelerometer_X for the
	   VM_Floor_Displacement_mm calculation. */
	float32 FD_Calc_Accel_ZX_Coeff;

	/* The coefficient applied to VM_Filtered_Accelerometer_Y2 for the VM_Floor_Displacement_mm calculation. */
	float32 FD_Calc_Accel_YY_Coeff;

	/* The coefficient applied to VM_Filtered_Accelerometer_Y*VM_Filtered_Accelerometer_Z for the
	   VM_Floor_Displacement_mm calculation. */
	float32 FD_Calc_Accel_ZY_Coeff;

	/* The coefficient applied to VM_Filtered_Accelerometer_Z2 for the VM_Floor_Displacement_mm calculation. */
	float32 FD_Calc_Accel_ZZ_Coeff;

	/* The static offset for the VM_Floor_Displacement_mm calculation. */
	sint16 FD_Calc_Intercept_x100;

} FLOOR_DISPLACEMENT_SETUP_TYPE;

typedef PACKED struct
{
	/* The value of Cabinet_Hit_Filtered_Accelerometer_X beyond which the control
	   increments Cabinet_Hit_Soft_Counter. */
	sint16 Soft_Detection_Threshold_X_x250;

	/* The value of Cabinet_Hit_Filtered_Accelerometer_Y beyond which the control
	   increments Cabinet_Hit_Soft_Counter. */
	sint16 Soft_Detection_Threshold_Y_x250;

	/* The value of Cabinet_Hit_Filtered_Accelerometer_Z beyond which the control
	   increments Cabinet_Hit_Soft_Counter. */
	sint16 Soft_Detection_Threshold_Z_x250;

	/* The value of Cabinet_Hit_Filtered_Accelerometer_X beyond which the control stops
	   the motor and executes a Rebalance Profile. */
	sint16 Hard_Detection_Threshold_X_x250;

	/* The value of Cabinet_Hit_Filtered_Accelerometer_Y beyond which the control stops
	   the motor and executes a Rebalance Profile. */
	sint16 Hard_Detection_Threshold_Y_x250;

	/* The value of Cabinet_Hit_Filtered_Accelerometer_Z beyond which the control stops
	   the motor and executes a Rebalance Profile. */
	sint16 Hard_Detection_Threshold_Z_x250;

	/* The drum velocity above which a Cabinet Hit Detection would illicit a behavioral
	   response from the control. */
	uint8 Detection_Start_Rpm_Div10;

	/* The drum velocity below which a Cabinet Hit Detection would illicit a behavioral
	   response from the control. */
	uint8 Detection_Stop_Rpm_Div10;

	/* The number of soft hit detections at which the control runs the Rebalance Profile. */
	uint8 Soft_Counter_Threshold;

	/* Displacement index for the Cabinet Hit Accelerometer Filter Configuration (in MCU_EXTRACTION_ DYNAMIC_TABLES_PTR).
	   255 means filter disabled. */
	uint8 Accel_Filter_Config_Index;

} CABINET_HIT_SETUP_TYPE;

typedef PACKED struct
{
    /* The number of samples to exclude in the arithmetic
       mean calculation of the unbalance measurement average. */
    uint8 Skipped_Samples;

    /* The number of samples to include in the arithmetic mean
       calculation of unbalance measurement average. */
    uint8 Sample_Count;
} EXT_UNB_CHK_SETUP_TYPE;

typedef PACKED struct
{
    /* The maximum accepted measured unbalance level
       associated with the specified target velocity. */
    uint16 Unbalance_Level;

    /* The target velocity allowed with the corresponding
       maximum unbalance extended level */
    uint8 Target_Speed_Div10;

    uint8 Padding_Byte;
} EXT_UNB_CHK_TARGET_TYPE;

typedef PACKED struct
{
    /* The number of attempts that have been executed
     * by extraction. */
    uint8 Rebalance_Attempts;

    /* Displacement for the Rebalance Profile. */
    uint8 Rebalance_Profile_Index;
} REBALANCE_TABLE_TYPE;

typedef PACKED struct
{
    /* The threshold for any accelerometer axis beyond which the control shall halt the clean washer cycle. */
    uint16 Accel_Threshold_x250;

    /* The threshold for Load_Mass_kg beyond which the control shall halt the clean washer cycle. */
    uint16 Load_Mass_Threshold_x10;

    /* The threshold for Unbalance_Measurement beyond which the control shall halt the clean washer cycle. */
    uint16 Unbalance_Threshold;

    /* The threshold for Load_Mass_kg beyond which the control shall halt the clean washer cycle. */
    uint16 Torque_P2P_Threshold_x100;
} CLEAN_WASHER_SETUP_TYPE;

#endif
