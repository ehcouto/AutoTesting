/**
 *  @file
 *
 *  @brief   Extraction hard-coded parameters
 *
 *
 *  @copyright  Copyright 2017  $  Whirlpool Corporation.  All rights reserved - CONFIDENTIAL.
 */

#include "C_Extensions.h"
#include "ExtractionParams.h"

//=====================================================================================================================
//-------------------------------------- PUBLIC (Extern Variables, Constants & Defines) -------------------------------
//=====================================================================================================================

//-------------------------------------- PRIVATE (Variables, Constants & Defines) -------------------------------------


//static const MCISENSORSWM_PARAMS_TYPE MciSensorsWm_Params_SF =
//{
//		.K_Turns = 699,		/*K_Turns  = 2^15 / 60 / Turns Base / Algo Frequency * 32768 = Speedbase[Hz] / 128 / Algo Frequency * 32768 =*/
//		.Mean_Over_Drum_Rev = 256,		/*Mean_Over_Drum_Rev = 1.0 * Transmission Ratio / Turns Base * 32768 = 1.0 * Transmission Ratio / 128 * 32768 =*/
//		.Spinning_Thr_Speed = 80		/*Spinning_Thr_Speed = Threshold Distribution * Transmission Ratio  =*/
//};

#if (EXTRACTION_ATHLETE_PARAMETERS_IN_PLACE == 0)

static const EXT_UNB_CHK_SETUP_TYPE ExtUnbSetup = {
		.Sample_Count = 10,
		.Skipped_Samples = 10,
};

static const EXT_UNB_CHK_TARGET_TYPE ExtUnbTargetSet[] = {
		{
				.Target_Speed_Div10 = 20,
				.Unbalance_Level = 30000
		},
		{
				.Target_Speed_Div10 = 60,
				.Unbalance_Level = 32000
		},
};

static const REBALANCE_TABLE_TYPE RebalTable[] = {
		{
				.Rebalance_Attempts = 255,
				.Rebalance_Profile_Index = 1
		}
};

#endif // (EXTRACTION_ATHLETE_PARAMETERS_IN_PLACE == 0)

//! Section: setup structures used by extraction ----------------------------------------------------------------------
#if ((EXTRACTION_LOW_SPEED_PARAMETERS_IN_PLACE == 0) || (EXTRACTION_HIGH_SPEED_PARAMETERS_IN_PLACE == 0))

static const PARAM_EST_SETUP_TYPE Param_Est_Setup_Bank[] = {
		{
				.BB_Lag = 0.97f,
				.Forgetting = 1.0f,
				.Kalman_Vector =	{ 0.003162,    	/* PARAM_EST_KALMAN_INERTIA */
						0.0,        				/* PARAM_EST_KALMAN_VISCOUS */
						0.3162,        				/* PARAM_EST_KALMAN_COULOMB */
						0.03162,      				/* PARAM_EST_KALMAN_UNBALANCE */
						0.07071 },    				/* PARAM_EST_KALMAN_BALLBALANCER */
						.Model_Config.Struct.Adapt_Type = PARAM_EST_ADAPTATION_KALMAN,
						.Model_Config.Struct.Model = PARAM_EST_MODEL_BK_EULER_B0_BB,
						.Filter_Configuration_Index = (0 + PE_HARD_CODED_INDEXES_OFFSET)	// For embedded PE parameters indexes add
																							// PE_HARD_CODED_INDEXES_OFFSET to the real index

		},
		{
				.BB_Lag = 0.97f,
				.Forgetting = 0.998f,
				.Kalman_Vector =	{ 0,    		/* PARAM_EST_KALMAN_INERTIA */
						0,        					/* PARAM_EST_KALMAN_VISCOUS */
						0,     						/* PARAM_EST_KALMAN_COULOMB */
						0.05,      					/* PARAM_EST_KALMAN_UNBALANCE */
						0},    						/* PARAM_EST_KALMAN_BALLBALANCER */

						.Model_Config.Struct.Adapt_Type = PARAM_EST_ADAPTATION_BOTH,
						.Model_Config.Struct.Model = PARAM_EST_MODEL_HYB_ACCEL,
						.Filter_Configuration_Index = (1 + PE_HARD_CODED_INDEXES_OFFSET)	// For embedded PE parameters indexes add
																							// PE_HARD_CODED_INDEXES_OFFSET to the real index
		}
};

static const PARAM_EST_EXC_SETUP_TYPE Param_Est_Exc_Setup_Bank[] = {
		{
				.Method = PARAM_EXC_METHOD_PRBS,
				.Amplitude = 9.0f,
				.Time = 0.7f
		},
		{
				.Method = PARAM_EXC_METHOD_PRBS,
				.Amplitude = 5.0f,
				.Time = 0.3f
		}
};

static const PARAM_EST_APP_SPECIFIC_SETUP_TYPE Param_Est_App_Specific_Setup_Bank[] = {
		{
				.Covariance_Matrix_Default = 1000,
				.Cov_Reset_Wait_x10 = 80,
				.Stabilization_Time = 0
		},
		{
				.Covariance_Matrix_Default = 1000,
				.Cov_Reset_Wait_x10 = 50,
				.Stabilization_Time = 5
		}
};
#endif // ((EXTRACTION_LOW_SPEED_PARAMETERS_IN_PLACE == 0) || (EXTRACTION_HIGH_SPEED_PARAMETERS_IN_PLACE == 0))

#if (EXTRACTION_LOW_SPEED_PARAMETERS_IN_PLACE == 0)
static const EXTRACTION_SETUP_TYPE ExtractionSetup = {
			.Distribution_Profile_Index = 0,
		    .Spin_Profile_Index = 0,
		    .Rebalance_Table_Index = 0,
		    .DR_Enabled = 1,
		    .OR_Enabled = 0,
		    .MDDP_Enabled = 0,
		    .LMS_Enabled = 0,
			.CLWD_Enabled = 0,
			.Unused = 0,
		    .CM_Enabled = 0,
		    .PE_Enabled = 1,
		    .Basic_Unbalance_Table_Index = 0,
		    .Extended_Unbalance_Table_Index = 0,
		    .Optimum_Ramp_Unbalance_Table_Index = 0,
			.Load_Mass_Count_Average = 1,
			.Padding_Byte_7 = 0
};

static const UB_CONVERSION_COEFFS_TYPE Extraction_UB_Coeffs = {
		.K2 = 0.3924246f,
		.K4 = 1.0f,
		.K1 = 0U,
		.K3 = 0U
};

static const MASS_CONVERSION_COEFFS_TYPE Extraction_Mass_Coeffs = {
		.A = 0.0f,
		.B = 10.0f,
		.C = 0.0f
};

static const FDR_SETUP_TYPE FDR_Setup = {
		.Max_Torque_Disturbance_x100 = 5000,
		.P2P_Torque_Updates_Initial = 2,
		.P2P_Torque_Updates_Short_Redist = 2,
		.Short_Redist_Delta_Speed_Drop = 10,
		.Short_Redist_Min_Velocity = 40,
		.Short_Redist_Count_Max = 3
};

static const P2P_TORQUE_SETUP_TYPE P2P_Torque_Setup =
{
		.SNRD_Sample_Rate_Div_10 = 5
};

static const SATELLIZATION_SETUP_TYPE Satellization_Setup = {
		.Window_Power_Threshold = 2000.0f,
		.Window_Power_Num_Readings = 10,
		.Window_Power_Num_Elements = 10,
		.Max_Velocity = 95,
		.HPF_Selection_Speed = 65
};

static const OPT_RAMP_SETUP_TYPE Opt_Ramp_Setup = {
		.BB_Magnitude_Threshold = 1.0f,
};

static const MDDP_SETUP_TYPE MDDP_Setup = {
		.Decelerate_To_Dist_Speed = TRUE
};

static const PARAM_EST_SETUP_INDEX_TYPE LowSpeedSetup = {
		.Param_Est_Setup_Index = (0 + PE_HARD_CODED_INDEXES_OFFSET),				// For embedded PE parameters indexes add
																					// PE_HARD_CODED_INDEXES_OFFSET to the real index
		.Param_Est_App_Specific_Setup_Index = (0 + PE_HARD_CODED_INDEXES_OFFSET)	// For embedded PE parameters indexes add
																					// PE_HARD_CODED_INDEXES_OFFSET to the real index
};

// Number of second-order sections
#define HP_FILTER_NUM_SOS_SECTIONS	4

// HPF #1 - stop 75rpm, pass 130rpm, fs 100Hz
static DIGITAL_FILTER_BIQUAD_COEFF_TYPE Param_Est_HS_Filter_01_Coeffs[] =
{
		/*Section 01*/
		{
				.Num = {-1.99406898021698f, 1.0f},
				.Den = {-1.9538109302520752f, 0.96695202589035034f},
				.Gain = 0.98164629936218262f
		},
		/*Section 02*/
		{
				.Num = {-1.9957356452941895f, 1.0f},
				.Den = {-1.8974725008010864f, 0.90863937139511108f},
				.Gain = 0.95254349708557129f
		},
		/*Section 03*/
		{
				.Num = {-1.9980950355529785f, 1.0f},
				.Den = {-1.8575031757354736f, 0.86622864007949829f},
				.Gain = 0.93137651681900024f
		},
		/*Section 04*/
		{
				.Num = {-1.9997650384902954f, 1.0f},
				.Den = {-1.8369312286376953f, 0.84401875734329224f},
				.Gain = 0.92029160261154175f
		}
};

// HPF #2 - stop 95rpm, pass 165rpm, fs 100Hz
static DIGITAL_FILTER_BIQUAD_COEFF_TYPE Param_Est_HS_Filter_02_Coeffs[] =
{
		/*Section 01*/
		{
				.Num = {-1.990486622f, 1.0f},
				.Den = {-1.937429547f, 0.9583925605f},
				.Gain = 0.9762774706f
		},
		/*Section 02*/
		{
				.Num = {-1.993158221f, 1.0f},
				.Den = {-1.868144155f, 0.8858264089f},
				.Gain = 0.9401006103f
		},
		/*Section 03*/
		{
				.Num = {-1.99694252f, 1.0f},
				.Den = {-1.819949389f, 0.8336938024f},
				.Gain = 0.9141095281f
		},
		/*Section 04*/
		{
				.Num = {-1.999622703f, 1.0f},
				.Den = {-1.795463204f, 0.8065980077f},
				.Gain = 0.9006002545f
		},
};

#define HARD_CODED_DIST_PROFILES_COUNT	2

static const DISTRIBUTION_PULSE_TYPE Distribution_Profile_Set[2][3] = {
		{
				// FDR profile - 0-90RPM @7RPM/s
				{
						.Target_Speed_Div2 = 45,
						.Ramp_Time_x10 = 20,
						.On_Time = 3,
						.Off_Time = 0,
						.Dist_Ramp_Enabled = 0
				},
				{
						.Target_Speed_Div2 = 30,
						.Ramp_Time_x10 = 86,
						.On_Time = 0,
						.Off_Time = 0,
						.Dist_Ramp_Enabled = 0
				},
				{
						.Target_Speed_Div2 = 45,
						.Ramp_Time_x10 = 80,
						.On_Time = 2,
						.Off_Time = 0,
						.Dist_Ramp_Enabled = 1
				}
		},
		{
				// Rebalance profile
				{
						.Target_Speed_Div2 = 25,
						.Ramp_Time_x10 = 1,
						.On_Time = 10,
						.Off_Time = 10,
						.Dist_Ramp_Enabled = 0
				},
				{
						.Target_Speed_Div2 = -25,
						.Ramp_Time_x10 = 1,
						.On_Time = 10,
						.Off_Time = 10,
						.Dist_Ramp_Enabled = 0
				},
		}
};

static const uint8 Distribution_Profile_Count_Set[] = {1,2};

#endif // (EXTRACTION_LOW_SPEED_PARAMETERS_IN_PLACE == 0)

#if (EXTRACTION_HIGH_SPEED_PARAMETERS_IN_PLACE == 0)
static const HIGH_SPEED_PULSE_CONFIG_TYPE High_Speed_Pulse_Config_Set[] = {
		{
				.Plateau_Advance_Threshold = 20,
				.Plateau_Monitor_Threshold = 255,
				.Plateau_Hold_Threshold = 255,
				.Plateau_Settling_Threshold_x1000 = 2000,
				.Plateau_Max_Averages = 12,
				.Plateau_Monitor_Samples = 15,
				.Ramp_Viscous_Friction_Threshold = 1000000.0f
		}
};

static const HIGH_SPEED_CONFIG_TYPE High_Speed_Config = {
		.Friction_Average_Samples = 3,
		.Friction_Drum_Revolutions = 5,
		.Delta_Speed_Drop = 100,
		.Ext_Plateau_Delta_Speed_Drop_Div5 = 40,
		.Reference_Plateau_Velocity_Div10 = 60
};


static const SPIN_PROFILE_TYPE Spin_Profile = {
		.Factory_Calibration_Enabled = 0,
		.High_Speed_Enabled = 1,
		.Inertia_Dependent_Profile_Enabled = 1,
		.Load_Mass_Limit_Check_Enabled = 0,
		.Spin_Pulse_Count = 1,
		.Load_Mass_x10 = {50, 300}
};

static const SPIN_PULSE_TYPE Spin_Pulse_Set[] = {
		{
				.Target_Speed_Div10 = 120,
				.Dist_Time = 0,
				.Ramp_Time = {110,110}, //90-500RPM @30RPM/s
				.Plateau_Time = {0,0},
				.Param_Est_Config_Set_Index = (1 + PE_HARD_CODED_INDEXES_OFFSET),			// For embedded PE parameters indexes add
																							// PE_HARD_CODED_INDEXES_OFFSET to the real index
				.Param_Est_App_Specific_Config_Index = (1 + PE_HARD_CODED_INDEXES_OFFSET),	// For embedded PE parameters indexes add
																							// PE_HARD_CODED_INDEXES_OFFSET to the real index
				.High_Speed_Config_Index = 0,
				.CEM_Plateau_Threshold = 255,
				.Baseline_Config_Index = 0,
				.Load_Mass_Config_Index = 0
		},
		{
				.Target_Speed_Div10 = 50,
				.Dist_Time = 10,
				.Ramp_Time = {15,15},
				.Plateau_Time = {10,10},
				.Param_Est_Config_Set_Index = (1 + PE_HARD_CODED_INDEXES_OFFSET),			// For embedded PE parameters indexes add
																							// PE_HARD_CODED_INDEXES_OFFSET to the real index
				.Param_Est_App_Specific_Config_Index = (1 + PE_HARD_CODED_INDEXES_OFFSET),	// For embedded PE parameters indexes add
																							// PE_HARD_CODED_INDEXES_OFFSET to the real index
				.High_Speed_Config_Index = 0,
				.CEM_Plateau_Threshold = 255,
				.Baseline_Config_Index = 0,
				.Load_Mass_Config_Index = 0
		},
		{
				.Target_Speed_Div10 = 80,
				.Dist_Time = 0,
				.Ramp_Time = {15,15},
				.Plateau_Time = {15,15},
				.Param_Est_Config_Set_Index = (1 + PE_HARD_CODED_INDEXES_OFFSET),			// For embedded PE parameters indexes add
																							// PE_HARD_CODED_INDEXES_OFFSET to the real index
				.Param_Est_App_Specific_Config_Index = (1 + PE_HARD_CODED_INDEXES_OFFSET),	// For embedded PE parameters indexes add
																							// PE_HARD_CODED_INDEXES_OFFSET to the real index
				.High_Speed_Config_Index = 0,
				.CEM_Plateau_Threshold = 255,
				.Baseline_Config_Index = 0,
				.Load_Mass_Config_Index = 0
		}
};

static const HIGH_SPEED_BASELINE_CONFIG_TYPE High_Speed_Baseline_Set[] = {
		{
				.Baseline_Update_Threshold = 90,
				.Percent_Of_Current_Threshold = 100,
				.Friction_Max_x1000 = 1000,
				.Friction_Min_x1000 = 500,
				.Max_Redistributions = 3,
		}
};

static const LOAD_MASS_LIMIT_SETUP_TYPE High_Speed_LMLC_Set[] = {
		{
				.Threshold_x10 = 10000,
				.Delta_Time_x10 = 10,
		}
};

static const HIGH_SPEED_UNB_SETUP_TYPE High_Speed_Unb_Config = {
		.Max_Redistributions = 5
};

static const BASELINE_FACTORY_CALIBRATION_SETUP_TYPE Factory_Calibration_Setup = {
		.Plateau_Samples = 5,
		.Average_Samples = 3,
		.Friction_Drum_Revolutions = 4
};

static const CEM_SETUP_TYPE CEM_Setup = {
		.Clothing_Drag_Max_Velocity_Div10 = 50,
		.Friction_Avg_Samples = 3,
		.Friction_Window_Drum_Rev = 2,
		.Static_Decrement_x10 = 1,
		.Static_Increment_x10 = 1
};

#endif // (EXTRACTION_HIGH_SPEED_PARAMETERS_IN_PLACE == 0)

#if (EXTRACTION_LOW_SPEED_PARAMETERS_IN_PLACE == 0)

//! Section: Pre-lookup data used by extraction ----------------------------------------------------------------

static const uint16 FastDistRampTargetSpeed_BP[] = { 50U, 60U, 70U, 80U, 90U, 100U };
static const uint16 MddpLoadMassX10_BP[] = { 0U, 170U, 254U, 255U, 300U, 530U };
static const uint16 OptRampLoadMassX100_BP[] = { 300U, 900U, 1800U, 2700U, 3600U, 4500U };
static const uint16 P2PTorqueLoadMassX10_BP[] = { 300U, 900U, 1800U, 2700U, 3600U, 4500U };
static const uint8 RebalAttempts_BP[] = { 3U, 5U, 7U, 11U, 16U, 21U, 26U, 29U };
static const uint8 TargetSpeedDiv10_BP[] = { 14U, 60U, 80U, 100U, 106U, 116U, 117U, 118U };
static const uint16 WaterTemp_BP[] = { 10U, 25U, 40U, 50U, 60U, 70U };

//! Section: LUT data used by extraction -----------------------------------------------------------------------

static const uint8 MddpDuration_LUT[] = { 0U, 0U, 0U, 15U, 30U, 45U };
static const uint8 MddpRamp_LUT[] = { 10U, 10U, 10U, 10U, 10U, 10U };
static const uint16 MddpSpeed_LUT[] = { 140U, 140U, 140U, 140U, 140U, 140U };
static const uint8 OptRampConvTime_LUT[] = { 20U, 20U, 20U, 20U, 20U, 20U };
static const sint16 OptRampLowerOffset_LUT[] = { -5000, -5000, -5000, -5000, -5000, -5000 };
static const uint16 OptRampPhaseTolerance_LUT[] = { 10U, 8U, 5U, 5U, 5U, 5U };
static const uint16 OptRampPhsAdvTgt_LUT[] = { 10U, 15U, 30U, 30U, 30U, 30U };
static const uint8 OptRampPlateauTime_LUT[] = { 20U, 20U, 20U, 20U, 20U, 20U };
static const uint16 OptRampUnb_LUT[] =
{
	5000U, 5000U, 5000U, 5000U, 5000U, 5000U, 5000U, 5000U, 5000U, 5000U, 5000U, 5000U,
	5000U, 5000U, 5000U, 5000U, 5000U, 5000U, 5000U, 5000U, 5000U, 5000U, 5000U, 5000U,
	5000U, 5000U, 5000U, 5000U, 5000U, 5000U, 5000U, 5000U, 5000U, 5000U, 5000U, 5000U,
	5000U, 5000U, 5000U, 5000U, 5000U, 5000U, 5000U, 5000U, 5000U, 5000U, 5000U, 5000U
};
static const sint16 OptRampUpperOffset_LUT[] = { 5000U, 5000U, 5000U, 5000U, 5000U, 5000U };
static const uint16 P2PTorqueX100_LUT[] =
{
	5000U, 5000U, 5000U, 5000U, 5000U, 5000U, 5000U, 5000U, 5000U, 5000U, 5000U, 5000U, 5000U,
	5000U, 5000U, 5000U, 5000U, 5000U, 5000U, 5000U, 5000U, 5000U, 5000U, 5000U, 5000U, 5000U,
	5000U, 5000U, 5000U, 5000U, 5000U, 5000U, 5000U, 5000U, 5000U
};
#endif // (EXTRACTION_LOW_SPEED_PARAMETERS_IN_PLACE == 0)

#if (EXTRACTION_HIGH_SPEED_PARAMETERS_IN_PLACE == 0)
static const uint16 BaselineFrictionDefaultValuesHC_x1000[BASELINE_ARRAY_MAX_SIZE] = { 3000, 3100, 3200, 3300 };
static const uint8 BaselineFrictionReferenceSpeedsHC_Div10[BASELINE_ARRAY_MAX_SIZE] = { 30U, 60U, 90U, 100U };
static const uint16 FactoryCalScalingFactorArrayHC[BASELINE_ARRAY_MAX_SIZE] = {120, 140, 160, 180};
static const uint8 FactoryFrictionSelIndexArrayHC[BASELINE_ARRAY_MAX_SIZE] = {0, 0, 0, 0};
#endif // (EXTRACTION_HIGH_SPEED_PARAMETERS_IN_PLACE == 0)

#if (EXTRACTION_ACCEL_PARAMETERS_IN_PLACE == 0)

#define ACCEL_LP_FILTER_NUM_SOS_SECTIONS	3

static DIGITAL_FILTER_BIQUAD_COEFF_TYPE Accel_LF_Filter_Coeffs[] =
{
		/*Section 01*/
		{
				.Num = {-1.8741014f, 1.0f},
				.Den = {-1.912623525f, 0.9231058359f},
				.Gain = 0.216047436f
		},
		/*Section 02*/
		{
				.Num = {-1.248931646f, 1.0f},
				.Den = {-1.881327629f, 0.8860738873f},
				.Gain = 0.09751286358f
		},
		/*Section 03*/
		{
				.Num = {-1.928299546f, 1.0f},
				.Den = {-1.958030343f, 0.9737803936f},
				.Gain = 0.005454508122f
		},
};

//! Accelerometer related setup structs
static const FLOOR_DISPLACEMENT_SETUP_TYPE FloorDispSetup;
static const VIB_MAP_FLOOR_DISPL_THRESHOLD_TYPE VibMapFDThreshold;
static const VIBRATION_MAPPING_SETUP_TYPE VibMapSetup;
static const CABINET_HIT_SETUP_TYPE CabHitSetup = {
		.Soft_Detection_Threshold_X_x250 = 1000,
		.Soft_Detection_Threshold_Y_x250 = 1000,
		.Soft_Detection_Threshold_Z_x250 = 1000,
		.Hard_Detection_Threshold_X_x250 = 2000,
		.Hard_Detection_Threshold_Y_x250 = 2000,
		.Hard_Detection_Threshold_Z_x250 = 2000,
		.Soft_Counter_Threshold = 8,
		.Detection_Start_Rpm_Div10 = 4,
		.Detection_Stop_Rpm_Div10 = 160,
		.Accel_Filter_Config_Index = 0xFF
};

static const uint8 VibMapRefSpeedArray_div10[] = {50,80,116};

#endif


//-------------------------------------- PRIVATE (Function Prototypes) ------------------------------------------------

static void ExtractionEmbeddedParams__Initialize()
{
#if (EXTRACTION_ATHLETE_PARAMETERS_IN_PLACE == 0)
	GESE_Extr_ExtUnbSetup = &ExtUnbSetup;
	GESE_Extr_ExtUnbTargetSet = &ExtUnbTargetSet[0];
	GESE_Extr_ExtUnbTargetCount = 2U;
	GESE_Extr_RebalTable = RebalTable;
	GESE_Extr_RebalTableCount = 1;
#endif // (EXTRACTION_ATHLETE_PARAMETERS_IN_PLACE == 0)

#if (EXTRACTION_LOW_SPEED_PARAMETERS_IN_PLACE == 0)

	//Init setup pointers here
	GESE_Extr_MassConvCoeffs = &Extraction_Mass_Coeffs;
	GESE_Extr_UnbConvCoeffs = &Extraction_UB_Coeffs;
	GESE_Extr_FastDistRampSetup = &FDR_Setup;
	GESE_Extr_OptRampSetup = &Opt_Ramp_Setup;
	GESE_Extr_MddpSetup = &MDDP_Setup;
	GESE_Extr_P2PTorqueSetup = &P2P_Torque_Setup;
	GESE_Extr_SatellizationSetup = &Satellization_Setup;
	GESE_Extr_ParamEstLowSpeedSetup = &LowSpeedSetup;
	GESE_Extr_Setup = &ExtractionSetup;

	GESE_Extr_HighPassFilterSetup[0].Num_Sections = HP_FILTER_NUM_SOS_SECTIONS;
	GESE_Extr_HighPassFilterSetup[0].Coeffs = Param_Est_HS_Filter_01_Coeffs;
	GESE_Extr_HighPassFilterSetup[1].Num_Sections = HP_FILTER_NUM_SOS_SECTIONS;
	GESE_Extr_HighPassFilterSetup[1].Coeffs = Param_Est_HS_Filter_02_Coeffs;

	//Init pre-lookup pointers here
	GESE_Extr_FastDistRampTargetSpeed_BP.N = 6U;
	GESE_Extr_FastDistRampTargetSpeed_BP.BP = FastDistRampTargetSpeed_BP;
	GESE_Extr_MddpLoadMassX10_BP.N = 6U;
	GESE_Extr_MddpLoadMassX10_BP.BP = MddpLoadMassX10_BP;
	GESE_Extr_OptRampLoadMassX100_BP.N = 6U;
	GESE_Extr_OptRampLoadMassX100_BP.BP = OptRampLoadMassX100_BP;
	GESE_Extr_P2PTorqueLoadMassX10_BP.N = 6U;
	GESE_Extr_P2PTorqueLoadMassX10_BP.BP = P2PTorqueLoadMassX10_BP;
	GESE_Extr_RebalAttempts_BP.N = 8U;
	GESE_Extr_RebalAttempts_BP.BP = RebalAttempts_BP;
	GESE_Extr_TargetSpeedDiv10_BP.N = 8U;
	GESE_Extr_TargetSpeedDiv10_BP.BP = TargetSpeedDiv10_BP;
	GESE_Extr_WaterTemp_BP.N = 6U;
	GESE_Extr_WaterTemp_BP.BP = WaterTemp_BP;
	GESE_Extr_RebalAttemptsArray = RebalAttempts_BP;
	GESE_Extr_TargetSpeedArray_Div10 = TargetSpeedDiv10_BP;

	//Init LUT pointers here
	GESE_Extr_MddpDuration_LUT = MddpDuration_LUT;
	GESE_Extr_MddpRamp_LUT = MddpRamp_LUT;
	GESE_Extr_MddpSpeed_LUT = MddpSpeed_LUT;
	GESE_Extr_OptRampConvTime_LUT = OptRampConvTime_LUT;
	GESE_Extr_OptRampPhaseTolerance_LUT = OptRampPhaseTolerance_LUT;
	GESE_Extr_OptRampPhsAdvTgt_LUT = OptRampPhsAdvTgt_LUT;
	GESE_Extr_OptRampPlateauTime_LUT = OptRampPlateauTime_LUT;
	GESE_Extr_P2PTorqueX100_LUT = P2PTorqueX100_LUT;
	GESE_Extr_OptRampUnbBeforeMDDP_LUT = OptRampUnb_LUT;
	GESE_Extr_OptRampUnbAfterMDDP_LUT = OptRampUnb_LUT;
	GESE_Extr_OptRampUpperOffsetBeforeMDDP_LUT = OptRampUpperOffset_LUT;
	GESE_Extr_OptRampUpperOffsetAfterMDDP_LUT = OptRampUpperOffset_LUT;
	GESE_Extr_OptRampLowerOffsetBeforeMDDP_LUT = OptRampLowerOffset_LUT;
	GESE_Extr_OptRampLowerOffsetAfterMDDP_LUT = OptRampLowerOffset_LUT;

	GESE_Extr_DistProfPulseCnt = 2;
	GESE_Extr_OptRampUnbLut_Size[0] = 8;
	GESE_Extr_OptRampUnbLut_Size[1] = 6;

#endif // (EXTRACTION_LOW_SPEED_PARAMETERS_IN_PLACE == 0)


#if (EXTRACTION_HIGH_SPEED_PARAMETERS_IN_PLACE == 0)

	GESE_Extr_HighSpdConfig = &High_Speed_Config;
	GESE_Extr_SpinPulses = Spin_Pulse_Set;
	GESE_Extr_HighSpdUnbConfig = &High_Speed_Unb_Config;
	GESE_Extr_SpinProfileConfig = &Spin_Profile;
	GESE_Extr_BaselineFrictionRefSpeed_BP.N = 4;
	GESE_Extr_BaselineDefaultFrictionArray_x1000 = BaselineFrictionDefaultValuesHC_x1000;
	GESE_Extr_BaselineFrictionRefSpeed_BP.BP = BaselineFrictionReferenceSpeedsHC_Div10;
	GESE_Extr_BaselineFrictionRefSpeed_Sig = GESE_Extr_BaselineFrictionRefSpeed_BP;
	GESE_Extr_BaselineFactoryCalibrationSetup = &Factory_Calibration_Setup;
	GESE_Extr_CEMSetup = &CEM_Setup;
	GESE_Extr_FactoryFrictionSelIndexArray = FactoryFrictionSelIndexArrayHC;
	GESE_Extr_FactoryCalScalingFactorArray = FactoryCalScalingFactorArrayHC;

#endif // (EXTRACTION_HIGH_SPEED_PARAMETERS_IN_PLACE == 0)

#if (EXTRACTION_ACCEL_PARAMETERS_IN_PLACE == 0)

	GESE_Extr_VibMapFDThreshold = NULL;
	GESE_Extr_VibMapSetup = NULL;
	GESE_Extr_FloorDispSetup = NULL;
	GESE_Extr_VibMapRefSpeedArray_div10.N = 0;
	GESE_Extr_VibMapRefSpeedArray_div10.BP = NULL; //VibMapRefSpeedArray_div10;
	GESE_Extr_CabHitSetup = NULL;

#endif
}

#if (EXTRACTION_HIGH_SPEED_PARAMETERS_IN_PLACE == 0)
uint16 *ExtractionEmbeddedParams__GetFactoryFriction()
{
	return (uint16*)(void*)BaselineFrictionDefaultValuesHC_x1000;
}
#endif // (EXTRACTION_HIGH_SPEED_PARAMETERS_IN_PLACE == 0)
