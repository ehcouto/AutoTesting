/**
 *  @file       MciSetWms.c
 *
 *  @brief      Indesit Wm Mci Setting Algorithms module
 *
 *  $Header: $
 *
 *  @copyright  Copyright 2016-$Date$. Whirlpool Corporation. All rights reserved - CONFIDENTIAL
 */
//---------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------


//-------------------------------------- Include Files ----------------------------------------------------------------
#include "MciSetWm.h"
#include "MciSensorsWm.h"
#include "Mci.h"
#include "MathCalc.h"


#if (MCI_SET_PARAMETERS_INTERNAL == ENABLED)
    #include "MciSetWm_prv.h"  //Mca parameters
#else
#if (PLATFORM_USED == NUCLEUS_BASED)
    #include "string.h"
    #include "SettingFile.h"
#else
    #include "Params.h"
#endif
#endif
//-------------------------------------- PUBLIC (Variables) -----------------------------------------------------------

//-------------------------------------- PRIVATE (Variables, Constants & Defines) -------------------------------------
#define SPEED_CONV_FACTOR       1.0f/(2.0f * 19800.0f)  // RPM<<16   to  IQ15 p.u.
#define TORQUE_CONV_FACTOR      2.0f * 4.0f             // IQ15 p.u. to  Nm<<16

static BOOL_TYPE ParamsReady;
static MCISETWM_PARAMS_TYPE *MciSetWm_Params;

#if (CIM_SOFT_START == ENABLED)
static uint32 Soft_Start_Max_Cnt;
static sint16 Previous_End_Speed;
#endif

#if (DYNAMIC_TORQUE_LIMITS == ENABLED)
static uint16 Vdc_Period_Cnt;
static sint16 Vdc_Min;
static sint16 Vdc_Min_Tmp;
static BOOL_TYPE LimitNorm_Request;
static BOOL_TYPE VdcNorm_Flag;
#endif

#if (UPDATE_SPEED_REGULATOR == ENABLED)
static sint32 Previous_Speed_Ref;
#endif

#if (MCI_SET_PARAMETERS_INTERNAL == ENABLED)

#else


#if (PLATFORM_USED == NUCLEUS_BASED)
    #define MCA_SET_WM_PARAMETERS_SIZE                  sizeof(MCISETWM_PARAMS_TYPE)
    SETTINGFILE_LOADER_TYPE Mca_Set_Wm_Prm_Flash_Ptr;
    uint8 Mca_Set_Wm_Prm_Ram[MCA_SET_WM_PARAMETERS_SIZE];
#else
    PARAMS_LOADER_TYPE Mca_Set_Wm_Prm_Ram;
#endif
#endif
//-------------------------------------- PRIVATE (Function Prototypes) ------------------------------------------------
#ifdef MOTOR_CIM_TYPE

#if (CIM_SOFT_START == ENABLED)
static void UpdateSoftStartInit(void);
static void UpdateSoftStartOnStart(void);
static void UpdateSoftStartOnStop(void);
static void SetMaxSoftStart(void);
#endif

#endif


#if (UPDATE_SPEED_FIR_TAPS == ENABLED)
static void UpdateSpeedFirTaps(void);
#endif

#if (UPDATE_SPEED_REGULATOR == ENABLED)
static void UpdateSpeedRegulator(void);
#endif

#if (DYNAMIC_TORQUE_LIMITS == ENABLED)
static void DynamicTorqueLimits(void);
#endif
//=====================================================================================================================
//-------------------------------------- Public Functions -------------------------------------------------------------
//=====================================================================================================================
//---------------------------------------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------------------------------------
/**
 *  @brief      It Initializes the module MciSetWms and its variables.
 *
 */
void MciSetWm__Initialize(void)
{
    ParamsReady = FALSE;

#if (MCI_SET_PARAMETERS_INTERNAL == ENABLED)
    MciSetWm_Params = (MCISETWM_PARAMS_TYPE *) (&MciSetWm_Params_SF[0]);
    ParamsReady = TRUE;
#else
    #if (PLATFORM_USED == NUCLEUS_BASED)
    if((SettingFile__IsValid() == TRUE) &&
        (SettingFile__BasicLoader(SF_PTR_MCU_MCA, SF_DISPL_MOTOR_MCI_SET_WM, &Mca_Set_Wm_Prm_Flash_Ptr) == PASS))
    {
        memcpy(&Mca_Set_Wm_Prm_Ram, Mca_Set_Wm_Prm_Flash_Ptr.Data, Mca_Set_Wm_Prm_Flash_Ptr.Length); //Copy from Flash to RAM
        MciSetWm_Params = (MCISETWM_PARAMS_TYPE *) (&Mca_Set_Wm_Prm_Ram);
        ParamsReady = TRUE;;
    }
    #else
    if(Params__BasicLoader(SF_PTR_MCU_MCA, SF_DISPL_MOTOR_MCI_SET_WM, &Mca_Set_Wm_Prm_Ram) == PASS)
    {
        MciSetWm_Params = (MCISETWM_PARAMS_TYPE *) (Mca_Set_Wm_Prm_Ram.Data);
        ParamsReady = TRUE;;
    }
    #endif
#endif

#if (DYNAMIC_TORQUE_LIMITS == ENABLED)
    Vdc_Min = 0x7FFF;
    Vdc_Min_Tmp = 0x7FFF;
    Vdc_Period_Cnt = 0;
#endif
}


//---------------------------------------------------------------------------------------------------------------------
/**
 *  @brief      Feeds back if Mci Set module parameters are loaded.
 *
 *  @param      none
 *  @return     TRUE: parameters are loaded / FALSE: parameters are not loaded
 */
BOOL_TYPE MciSetWm__IsParamsReady(void)
{
    return(ParamsReady);
}



//---------------------------------------------------------------------------------------------------------------------
/**
 *  @brief      Handle Wm Set Mci events of 5ms.
 *
 *  @param
 *  @return
 */
void MciSetWm__5msHandler(void)
{
	if(ParamsReady == TRUE)
	{
#if (UPDATE_SPEED_FIR_TAPS == ENABLED)
		UpdateSpeedFirTaps();
#endif

#if (UPDATE_SPEED_REGULATOR == ENABLED)
		UpdateSpeedRegulator();
#endif

#if (DYNAMIC_TORQUE_LIMITS == ENABLED)
		DynamicTorqueLimits();
#endif
	}

    else //parameters not loaded
    {
        #if (MCI_SET_PARAMETERS_INTERNAL == ENABLED)

        MciSetWm_Params = (MCISETWM_PARAMS_TYPE *) (&MciSetWm_Params_SF[0]);
        ParamsReady = TRUE;

        #else
        #if (PLATFORM_USED == NUCLEUS_BASED)
        if((SettingFile__IsValid() == TRUE) &&
            (SettingFile__BasicLoader(SF_PTR_MCU_MCA, SF_DISPL_MOTOR_MCI_SET_WM, &Mca_Set_Wm_Prm_Flash_Ptr) == PASS))
        {
            memcpy(&Mca_Set_Wm_Prm_Ram, Mca_Set_Wm_Prm_Flash_Ptr.Data, Mca_Set_Wm_Prm_Flash_Ptr.Length); //Copy from Flash to RAM
            MciSetWm_Params = (MCISETWM_PARAMS_TYPE *) (&Mca_Set_Wm_Prm_Ram);
            ParamsReady = TRUE;;
        }
        #else
        if(Params__BasicLoader(SF_PTR_MCU_MCA, SF_DISPL_MOTOR_MCI_SET_WM, &Mca_Set_Wm_Prm_Ram) == PASS)
        {
           MciSetWm_Params = (MCISETWM_PARAMS_TYPE *) (Mca_Set_Wm_Prm_Ram.Data);
           ParamsReady = TRUE;;
        }
        #endif
        #endif
    }
}


#if (UPDATE_SPEED_REGULATOR == ENABLED)
/**
 *  @brief     Update PI speed gains according the target speed
 *  @details   input must be always positive; usually the absolute value of SpeedRef is used
 *
 *
 *  @param[in]  speed_ref = reference speed used to update the PI speed gains (must be positive)
 *  @param[out]
 *  @return
 */
void UpdateSpeedRegulator(void)
{
    sint32 speed_ref;

	speed_ref = MATHCALC__ABS((Mci__GetAnalog(MOTOR0, MCI_AI_TARGET_SPEED_RPM_S16)>>16));

	if(speed_ref != Previous_Speed_Ref)
	{
		if(speed_ref > MciSetWm_Params->High_Regulators_Thr_Speed)
		{
			// [ applying very high regulator ]
			Mci__SetAnalog(MOTOR0, MCI_AO_SPEED_LOOP_GAIN_TABLE_INDEX, (sint32)&MciSetWm_Params->Prop_Gain_Speed_Very_High_Lo);
		}
		else if(speed_ref > MciSetWm_Params->Med_Regulators_Thr_Speed)
		{
			// [ applying high regulator ]
			Mci__SetAnalog(MOTOR0, MCI_AO_SPEED_LOOP_GAIN_TABLE_INDEX, (sint32)&MciSetWm_Params->Prop_Gain_Speed_High_Lo);
		}
		else if(speed_ref > MciSetWm_Params->Low_Regulators_Thr_Speed)
		{
			// [ applying med regulator ]
			Mci__SetAnalog(MOTOR0, MCI_AO_SPEED_LOOP_GAIN_TABLE_INDEX, (sint32)&MciSetWm_Params->Prop_Gain_Speed_Med_Lo);
		}
		else
		{
			// [ applying low regulator]
			Mci__SetAnalog(MOTOR0, MCI_AO_SPEED_LOOP_GAIN_TABLE_INDEX, (sint32)&MciSetWm_Params->Prop_Gain_Speed_Low_Lo);
		}
	}

	Previous_Speed_Ref = speed_ref;

}
#endif

#if (DYNAMIC_TORQUE_LIMITS == ENABLED)
//---------------------------------------------------------------------------------------------------------------------
/**
 *  @brief      Request meaning
 *
 *  @param      mean_request = meaning request state
 *
 */
void MciSetWm__LimitNormRequest(BOOL_TYPE limitnorm_request)
{
    LimitNorm_Request = limitnorm_request;
}

#endif


#ifdef MOTOR_CIM_TYPE

#if (UPDATE_SPEED_FIR_TAPS == ENABLED)
//---------------------------------------------------------------------------------------------------------------------
/**
 *  @brief     Update Taps for FIR filter used in speed calculation
 *  @details   input must be always positive; usually the absolute value of SpeedRef is used
 *
 *
 *  @param[in]  speed_ref = reference speed used to update the desired Taps of the Speed FIR Filter (must be positive)
 *  @param[out]
 *  @return
 */
void UpdateSpeedFirTaps(void)
{
 	sint32 speed_set_point;

	speed_set_point = MATHCALC__ABS((Mci__GetAnalog(MOTOR0, MCI_AI_SPEED_REFERENCE_RPM_S16)>>16));

	if(speed_set_point > MciSetWm_Params->Med_Regulators_Thr_Speed)
	{// [ applying high taps ]
		Mci__SetAnalog(MOTOR0, MCI_AO_SPEED_FIR_TAPS_TABLE_INDEX, MciSetWm_Params->Speed_Taps_High);

	}
	else if(speed_set_point> MciSetWm_Params->Low_Regulators_Thr_Speed)
	{// [ applying med taps ]
		Mci__SetAnalog(MOTOR0, MCI_AO_SPEED_FIR_TAPS_TABLE_INDEX, MciSetWm_Params->Speed_Taps_Med);

	}
	else if(speed_set_point > STARTUP_THR_SPEED_PERC)
	{// [ applying low taps]
		Mci__SetAnalog(MOTOR0, MCI_AO_SPEED_FIR_TAPS_TABLE_INDEX, MciSetWm_Params->Speed_Taps_Low);

	}
	else
	{
		Mci__SetAnalog(MOTOR0, MCI_AO_SPEED_FIR_TAPS_TABLE_INDEX, SPEED_TAPS_STARTUP);
	}

}


#endif



#if (CIM_SOFT_START == ENABLED)
//---------------------------------------------------------------------------------------------------------------------
/**
 *  @brief     Soft Start Update
 *  @details
 *
 *
 *  @param[in]
 *  @param[out]
 *  @return
 */
uint8 MciSetWm__SoftStart(SOFT_START_STATE_ENUM_T soft_start_state)
{
    uint8 ret_val = FALSE;

    if(ParamsReady == TRUE)
    {
	   switch(soft_start_state)
	   {

			case SOFT_START_INITIALIZATION:		// Init the softstart counter
				UpdateSoftStartInit();
				ret_val = TRUE;
				break;

			case SOFT_START_ON_START:			// Update the softstart counter at the motor starting based on the previous speed
				UpdateSoftStartOnStart();
				ret_val = TRUE;
				break;

			case SOFT_START_ON_STOP:			// Update the softstart counter at the motor stopping based on the previous torque
				UpdateSoftStartOnStop();
				ret_val = TRUE;
				break;

			case SOFT_START_MAX_REQUEST:		// max soft start request for 20 starts
				SetMaxSoftStart();
				ret_val = TRUE;
				break;


			default:
				//
				break;
		}
    }

    return(ret_val);
}


//---------------------------------------------------------------------------------------------------------------------
/**
 *  @brief     Soft Start Update
 *  @details   Init the softstart counter
 *
 *
 *  @param[in]  )
 *  @param[out]
 *  @return
 */
static void UpdateSoftStartInit(void)
{
	Previous_End_Speed = 0;

	if(Soft_Start_Max_Cnt == 0)
	{
		Mci__SetAnalog(MOTOR0, MCI_AO_SOFTSTART_COUNTER_INDEX, SOFT_START_APP_MED_COUNTER);
	}
	else
	{
		Mci__SetAnalog(MOTOR0, MCI_AO_SOFTSTART_COUNTER_INDEX, SOFT_START_APP_MAX_COUNTER);
	}
}




//---------------------------------------------------------------------------------------------------------------------
/**
 *  @brief     Soft Start Update
 *  @details   Update the softstart counter at the motor starting based on the previous speed
 *
 *
 *  @param[in]
 *  @param[out]
 *  @return
 */
static void UpdateSoftStartOnStart(void)
{
    uint16 soft_start_cnt;
    sint32 speed_set_point_abs;
    sint32 delta_speed_abs;
    sint32 ramp_end_speed_abs;

    speed_set_point_abs = MATHCALC__ABS((Mci__GetAnalog(MOTOR0, MCI_AI_SPEED_REFERENCE_RPM_S16)>>16));

	ramp_end_speed_abs = Mci__GetAnalog(MOTOR0, MCI_AI_TARGET_SPEED_RPM_S16)>>16;
	ramp_end_speed_abs = MATHCALC__ABS(ramp_end_speed_abs);

    if(speed_set_point_abs == 0)
    {
    	delta_speed_abs = ramp_end_speed_abs - Previous_End_Speed;
        delta_speed_abs = MATHCALC__ABS(delta_speed_abs);

        if((delta_speed_abs > SOFT_START_SPEED_TOLLERANCE)&&(Previous_End_Speed != 0))
        {// [ delta previous speed > tollerance --> re-initialize the SoftStart_Counter

            soft_start_cnt = SOFT_START_APP_MAX_COUNTER;

            Mci__SetAnalog(MOTOR0, MCI_AO_SOFTSTART_COUNTER_INDEX, soft_start_cnt);
        }
    }

    Previous_End_Speed = ramp_end_speed_abs;
}



//---------------------------------------------------------------------------------------------------------------------
/**
 *  @brief     Soft Start Update
 *  @details   Update the softstart counter at the motor stopping based on the previous torque
 *
 *
 *  @param[in]
 *  @param[out]
 *  @return
 */
static void UpdateSoftStartOnStop(void)
{
    uint16 soft_start_cnt;
    sint16 me_mean;
    sint32 temp32;
    sint32 ramp_end_speed_abs;

    if ((ParamsReady == TRUE)&&(Mci__GetAnalog(MOTOR0, MCI_AI_INTERNAL_MCI_STATE)>2))
    {
		me_mean = MciSensorsWm__GetIndesitAnalog(MCISENSORSWM_AI_TORQUE_MEAN);
		me_mean = MATHCALC__ABS(me_mean);

		/****** SoftStart Counter Calculation - begin ************/
		if (me_mean)
		{

			temp32 = (sint32)(((MciSetWm_Params->Soft_Start_Rel)<<IQ15)/(MciSetWm_Params->Torque_Ref_Max_Startup));


			soft_start_cnt = (uint16) IQMATH__Q15MULQ15(me_mean,(sint16)temp32);


			if (soft_start_cnt > SOFT_START_APP_MAX_COUNTER)
			{
				soft_start_cnt = SOFT_START_APP_MAX_COUNTER;
			}
			else if (soft_start_cnt < SOFT_START_APP_MIN_COUNTER)
			{
				soft_start_cnt = SOFT_START_APP_MIN_COUNTER;
			}

		}
		else
		{
			if(Mci__GetDigital(MOTOR0, MCI_DI_IDLE) == TRUE)
			{
				soft_start_cnt = SOFT_START_APP_MED_COUNTER;
			}
			else
			{
				soft_start_cnt = SOFT_START_APP_MAX_COUNTER;
			}
		}
		/****** SoftStart Counter Calculation - end ************/

		ramp_end_speed_abs = Mci__GetAnalog(MOTOR0, MCI_AI_TARGET_SPEED_RPM_S16)>>16;
		ramp_end_speed_abs = MATHCALC__ABS(ramp_end_speed_abs);

		if((Soft_Start_Max_Cnt != 0)||(ramp_end_speed_abs > MciSetWm_Params->Low_Regulators_Thr_Speed))
		{
			if(Soft_Start_Max_Cnt != 0) //decrease the counter
			{
				Soft_Start_Max_Cnt--;
			}

			soft_start_cnt = SOFT_START_APP_MAX_COUNTER;
		}

		Mci__SetAnalog(MOTOR0, MCI_AO_SOFTSTART_COUNTER_INDEX, soft_start_cnt);
    }
}


//---------------------------------------------------------------------------------------------------------------------
/**
 *  @brief     Request a Max Soft Start Counter for 20 starts
 *  @details
 *
 *
 *  @param[in]
 *  @param[out]
 *  @return
 */
static void SetMaxSoftStart(void)
{
	Soft_Start_Max_Cnt = SOFT_START_MAX_CNT_PERIOD;
}

#endif

#endif

#if (DYNAMIC_TORQUE_LIMITS == ENABLED)
//---------------------------------------------------------------------------------------------------------------------
/**
 *  @brief     Dynamic torque limits
 *  @details   Calculation of the torque spinning limits.
 *  @param[in]
 *  @param[out]
 *  @return
 */
static void DynamicTorqueLimits(void)
{
    sint32  torque_limit; //Q17.15
    sint16  speed;
    uint16  i;
    sint16*  torque_limit_table;
    sint16*  m_limit_table;
    sint16   vdc;

    vdc = Mci__GetAnalog(MOTOR0, MCI_AI_BUS_VOLTAGE_S16)>>16;

    Vdc_Period_Cnt++;
    if(Vdc_Period_Cnt > VDC_PERIOD_MIN)
    {
        Vdc_Period_Cnt = 0;

        Vdc_Min = Vdc_Min_Tmp;

        Vdc_Min_Tmp = 0x7FFF;
    }
    else
    {
        if(Vdc_Min_Tmp > vdc)
        {
            Vdc_Min_Tmp = vdc;
        }
    }

    if(Mci__GetDigital(MOTOR0, MCI_DI_MOTOR_STOP) == TRUE)
	{
		// update VdcNorm (management for torque limits for normative cycles)
		if (Vdc_Min >= MciSetWm_Params->Vdc_Norm)
		{ // Torque Limits for Normative Cycles can be applied
			VdcNorm_Flag = TRUE;
		}
		else
		{ // Always apply standard torque limits
			VdcNorm_Flag = FALSE;
		}
	}

    if ((LimitNorm_Request) && (VdcNorm_Flag))
    {//
        torque_limit_table = &MciSetWm_Params->TorqueLimit_Norm[0];
        m_limit_table = &MciSetWm_Params->MLimit_Norm[0];
    }
    else
    {//
        torque_limit_table = &MciSetWm_Params->TorqueLimit[0];
        m_limit_table = &MciSetWm_Params->MLimit[0];
    }

    if(Mci__GetDigital(MOTOR0, MCI_DI_MOTOR_RUNNING) == TRUE)
    {
        //---------------------------------------------------------------------------------------------------------------------
        //                   Dynamic Torque Limits
        //       assign torque limit according to Speed ranges
        //     [ sSetFileParams.f16Speed_Limit[TORQUE_LIM_LENGHT] ]
        //     [ sSetFileParams.f16Torque_Limit[TORQUE_LIM_LENGHT] ]
        //                [ #define TORQUE_LIM_LENGHT 8 ]
        //---------------------------------------------------------------------------------------------------------------------
    	speed = (sint16) ((float32)(Mci__GetAnalog(MOTOR0, MCI_AI_ACTUAL_SPEED_S16)) * SPEED_CONV_FACTOR);

        for (i = 0; i < TORQUE_LIM_LENGHT; i++)
        {
            if (MciSetWm_Params->SpeedLimit[i]!=0)
            {
                if(speed < MciSetWm_Params->SpeedLimit[i])
                {
                    break;
                }
            }
            else
            {
                break;
            }
        }


        if (i==0)
        {
            torque_limit=0;
        }
        else
        {
            // to avoid no limits on overspeed
            if (MciSetWm_Params->SpeedLimit[i]==0)
            {
                i--;
            }

            //Linear Limits
            torque_limit = torque_limit_table[i] + IQMATH__Q15MULQ15(m_limit_table[i],(speed-MciSetWm_Params->SpeedLimit[i]));

        }

        // translate the limit from Indesit to DTC torque base system
        torque_limit = (sint32)((float)torque_limit * TORQUE_CONV_FACTOR);  // torque Nm <<16

        Mci__SetAnalog(MOTOR0, MCI_AO_SPEED_LIMIT_INDEX, torque_limit);

    }

}
#endif
