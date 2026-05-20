/**
 *  @file       MciSensorsWm.c
 *
 *  @brief      Mci Sensors Algorithms for Indesit legacy appliances
 *
 *  $Header: $
 *
 *  @copyright  Copyright 2016-$Date$. Whirlpool Corporation. All rights reserved - CONFIDENTIAL
 */
//---------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------


//-------------------------------------- Include Files ----------------------------------------------------------------
#include "MciSensorsWm.h"
#include "Mci.h"
#include "MathCalc.h"



#if (MCI_SENSORS_PARAMETERS_INTERNAL == ENABLED)
    #include "MciSensorsWm_prv.h"  //Mca parameters
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
#ifndef MIN_SINT32
    #define MIN_SINT32  -2147483648L
#endif

#if (MCI_INVERT_DIRECTION == ENABLED)
    #define     MOTOR_DIRECTION     -1.0f
#else
    #define     MOTOR_DIRECTION      1.0f
#endif

static BOOL_TYPE ParamsReady;
static MCISENSORSWM_PARAMS_TYPE *MciSensorsWm_Params;

#if (MCI_MEAN_VALUE_CALC == ENABLED)
// Torque Meaning and Unbalance Index Variables
static uint16 Balance;
static sint16 Torque_Mean;
static float Torque_Mean_F;
static uint16 Mean_Samples_Cnt;
static sint32 Torque_Acc, Vibra_Acc;
static float Torque_Acc_F;
static sint32 Turns_Counter;
static uint16 Unbalance_Cnt;
static uint16 Vibration_Index;
#endif

#if (MCI_OVERHEATING_WASH == ENABLED)
//Washing Overheating Index Variables
static sint32 IsMotor2_Acc;
static float IsMotor2_Acc_F;
static uint16 OH_Washing_Cnt;
static uint16 OH_Washing_Stop_Flag;
static uint16 OH_Washing_Movement_Flag;
static sint16 OH_Washing_Index;
static float OH_Washing_Index_F;
#endif


#if (MCI_POWER_METER == ENABLED)
//Power Meter Variables
static sint16 Motor_Line_Power; 		//W
static sint16 Motor_Line_Energy_Low; 	//Ws
static sint16 Motor_Line_Energy_High; 	//Ws
static sint32 Energy_BackUp; 			//Ws //TODO: put it in back section
static sint32 Energy_K; 				//Ws
static sint32 Energy_K_1; 				//Ws
#endif


#if (MCI_DRUM_LOAD_AVERAGE == ENABLED)
// Drum Load Average Variables
static sint64 Drum_Load_Torque_Acc;
static uint32 Drum_Load_Torque_Samples_Cnt;
static float Drum_Load_Torque;
#endif


#if (MCI_SENSORS_PARAMETERS_INTERNAL == ENABLED)

#else
#if (PLATFORM_USED == NUCLEUS_BASED)
#define MCA_SENSORS_WM_PARAMETERS_SIZE              sizeof(MCISENSORSWM_PARAMS_TYPE)

SETTINGFILE_LOADER_TYPE Mca_Sensors_Wm_Prm_Flash_Ptr;
uint8 Mca_Sensors_Wm_Prm_Ram[MCA_SENSORS_WM_PARAMETERS_SIZE];
#else
PARAMS_LOADER_TYPE Mca_Sensors_Wm_Prm_Ram;
#endif
#endif
//-------------------------------------- PRIVATE (Function Prototypes) ------------------------------------------------
#if (MCI_MEAN_VALUE_CALC == ENABLED)
static void MeanValueCalc(void);
static void MeanValueCalc_Reset(void);
#endif

#if (MCI_OVERHEATING_WASH == ENABLED)
static void OverHeating_Washing(void);
#endif

#if (MCI_POWER_METER == ENABLED)
static void PowerMeter(void);
#endif

#if (MCI_DRUM_LOAD_AVERAGE == ENABLED)
static void Drum_Load_Average(void);
#endif
//=====================================================================================================================
//-------------------------------------- Public Functions -------------------------------------------------------------
//=====================================================================================================================


//---------------------------------------------------------------------------------------------------------------------
/**
 *  @brief      It Initializes the module MciSensorsWm and its variables.
 *
 */
void MciSensorsWm__Initialize(void)
{
    ParamsReady = FALSE;

#if (MCI_SENSORS_PARAMETERS_INTERNAL == ENABLED)
    MciSensorsWm_Params = (MCISENSORSWM_PARAMS_TYPE *) (&MciSensorsWm_Params_SF[0]);
    ParamsReady = TRUE;
#else
    #if (PLATFORM_USED == NUCLEUS_BASED)
    if((SettingFile__IsValid() == TRUE) &&
        (SettingFile__BasicLoader(SF_PTR_MCU_MCA, SF_DISPL_MOTOR_MCI_SENSORS_WM, &Mca_Sensors_Wm_Prm_Flash_Ptr) == PASS))
    {
        memcpy(&Mca_Sensors_Wm_Prm_Ram, Mca_Sensors_Wm_Prm_Flash_Ptr.Data, Mca_Sensors_Wm_Prm_Flash_Ptr.Length); //Copy from Flash to RAM
        MciSensorsWm_Params = (MCISENSORSWM_PARAMS_TYPE *) (&Mca_Sensors_Wm_Prm_Ram);
        ParamsReady = TRUE;
    }
    #else
    if(Params__BasicLoader(SF_PTR_MCU_MCA, SF_DISPL_MOTOR_MCI_SENSORS_WM, &Mca_Sensors_Wm_Prm_Ram) == PASS)
    {
        MciSensorsWm_Params = (MCISENSORSWM_PARAMS_TYPE *) (Mca_Sensors_Wm_Prm_Ram.Data);
        ParamsReady = TRUE;
    }
    #endif
#endif

#if (MCI_MEAN_VALUE_CALC == ENABLED)
	MeanValueCalc_Reset();
#endif

#if (MCI_OVERHEATING_WASH == ENABLED)
    //Overheating washing initialize
    IsMotor2_Acc = 0;
    IsMotor2_Acc_F = 0.0f;
    OH_Washing_Index = 0;
    OH_Washing_Index_F = 0.0f;
    OH_Washing_Cnt = 0;
    OH_Washing_Stop_Flag = 0;
    OH_Washing_Movement_Flag = 0;
#endif

#if (MCI_POWER_METER == ENABLED)
    Energy_BackUp = 0;
#endif


#if (MCI_DRUM_LOAD_AVERAGE == ENABLED)
    Drum_Load_Torque_Acc = 0;
    Drum_Load_Torque_Samples_Cnt = 0;
    Drum_Load_Torque = 0.0f;
#endif

}


//---------------------------------------------------------------------------------------------------------------------
/**
 *  @brief      Feeds back if Mci Set module parameters are loaded.
 *
 *  @param      none
 *  @return     TRUE: parameters are loaded / FALSE: parameters are not loaded
 */
BOOL_TYPE MciSensorsWm__IsParamsReady(void)
{
    return(ParamsReady);
}



//---------------------------------------------------------------------------------------------------------------------
/**
 *  @brief      Mci Sensors Wm  1ms handler
 *
 */
void MciSensorsWm__1msHandler(void)
{
	static unsigned char routine_post_scaler = 0;

	routine_post_scaler++;

	if (ParamsReady == TRUE)
    {

    	if(routine_post_scaler & 0x01)
    	{
    		// 2 ms handler
    		#if (MCI_MEAN_VALUE_CALC == ENABLED)
    		{
    			MeanValueCalc();
    		}
    		#endif
    	}
    }
}


//---------------------------------------------------------------------------------------------------------------------
/**
 *  @brief      Sensors and Algorithms 5ms handler
 *
 */
void MciSensorsWm__5msHandler(void)
{
    if (ParamsReady == TRUE)
    {

#if (MCI_OVERHEATING_WASH == ENABLED)
        OverHeating_Washing();
#endif

#if (MCI_POWER_METER == ENABLED)
        PowerMeter();
#endif

#if (MCI_DRUM_LOAD_AVERAGE == ENABLED)
        Drum_Load_Average();
#endif
    }
	else //parameters not loaded
    {
        #if (MCI_SENSORS_PARAMETERS_INTERNAL == ENABLED)

        MciSensorsWm_Params = (MCISENSORSWM_PARAMS_TYPE *) (&MciSensorsWm_Params_SF[0]);
        ParamsReady = TRUE;

        #else
        #if (PLATFORM_USED == NUCLEUS_BASED)
        if((SettingFile__IsValid() == TRUE) &&
            (SettingFile__BasicLoader(SF_PTR_MCU_MCA, SF_DISPL_MOTOR_MCI_SENSORS_WM, &Mca_Sensors_Wm_Prm_Flash_Ptr) == PASS))
        {
            memcpy(&Mca_Sensors_Wm_Prm_Ram, Mca_Sensors_Wm_Prm_Flash_Ptr.Data, Mca_Sensors_Wm_Prm_Flash_Ptr.Length); //Copy from Flash to RAM
            MciSensorsWm_Params = (MCISENSORSWM_PARAMS_TYPE *) (&Mca_Sensors_Wm_Prm_Ram);
            ParamsReady = TRUE;
        }
        #else
        if(Params__BasicLoader(SF_PTR_MCU_MCA, SF_DISPL_MOTOR_MCI_SENSORS_WM, &Mca_Sensors_Wm_Prm_Ram) == PASS)
        {

            MciSensorsWm_Params = (MCISENSORSWM_PARAMS_TYPE *) (Mca_Sensors_Wm_Prm_Ram.Data);
            ParamsReady = TRUE;
        }
        #endif
        #endif
    }
}


//---------------------------------------------------------------------------------------------------------------------
/**
 *  @brief      Read an integer analog value
 *
 *  @param[in]  read_data Index to the parameter. See the @ref MciSensorsWm.h file
 *
 *  @return     the parameter value.
 *              The default return is zero.
 *              Not implemented functions returns -1
 */
sint16 MciSensorsWm__GetIndesitAnalog(MCISENSORSWM_AI_TYPE read_data)
{
    sint16 ret_val = -32768;

    switch(read_data)
    {

#if (MCI_MEAN_VALUE_CALC == ENABLED)
    	case MCISENSORSWM_AI_TORQUE_MEAN:
            ret_val = (sint16)(Torque_Mean);
            break;

        case MCISENSORSWM_AI_BALANCE:
            ret_val = (sint16)(Balance);
            break;
#endif

#if (MCI_OVERHEATING_WASH == ENABLED)
        case MCISENSORSWM_AI_OVERHEATING_WASHING:
            ret_val = (sint16)(OH_Washing_Index);
            break;
#endif

#if (MCI_POWER_METER == ENABLED)
        case MCISENSORSWM_AI_MOTOR_LINE_POWER:
            ret_val = (sint16)(Motor_Line_Power);
            break;

        case MCISENSORSWM_AI_MOTOR_LINE_POWER_HI:
        	ret_val = (sint16)(Motor_Line_Energy_High);
        	break;

        case MCISENSORSWM_AI_MOTOR_LINE_POWER_LO:
		   ret_val = (sint16)(Motor_Line_Energy_Low);
		   break;
#endif

        default:
            //
            break;
    }

    return(ret_val);
}



//---------------------------------------------------------------------------------------------------------------------
/**
 *  @brief      Read an integer analog value
 *
 *  @param[in]  read_data Index to the parameter. See the @ref MciSensorsWm.h file
 *
 *  @return     the parameter value.
 *              The default return is zero.
 *              Not implemented functions returns -1
 */
sint32 MciSensorsWm__GetAnalog(MOTOR_ENUM_TYPE motor, MCISENSORSWM_AI_TYPE read_data)
{
    sint32 ret_val = MIN_SINT32;

    if ((MCI_AI_TYPE) read_data < MCI_AI_NR_OF_AI)
    {
        ret_val = Mci__GetAnalog(motor, (MCI_AI_TYPE) read_data);
    }

    switch(read_data)
    {

        case MCISENSORSWM_AI_TARGET_SPEED_RPM_S16:
        case MCISENSORSWM_AI_SPEED_REFERENCE_RPM_S16:
        case MCISENSORSWM_AI_ACTUAL_SPEED_S16:
        case MCISENSORSWM_AI_SPEED_ERROR_RPM_S16:
        case MCISENSORSWM_AI_MEAN_SPEED_S16:
            {
                float32 new_val = ret_val / MciSensorsWm__GetTransmissionRatio();
                ret_val = (sint32) new_val;
            }
            break;

        case MCISENSORSWM_AI_LOAD_TORQUE_S16:
            {
                float32 new_val = ret_val * MciSensorsWm__GetTransmissionRatio();
                ret_val = (sint32) new_val;
            }
            break;

#if (MCI_MEAN_VALUE_CALC == ENABLED)
        case MCISENSORSWM_AI_TORQUE_MEAN:
            ret_val = (sint32)(Torque_Mean_F*65536.0f);
            break;

        case MCISENSORSWM_AI_BALANCE:
            ret_val = (sint32)(Balance << 16);
            break;

        case MCISENSORSWM_AI_BALANCE_COUNT:
        	ret_val = (sint32)Unbalance_Cnt;
        	break;
#endif

#if (MCI_OVERHEATING_WASH == ENABLED)
        case MCISENSORSWM_AI_OVERHEATING_WASHING:
            ret_val = (sint32)(OH_Washing_Index_F*65536.0f);
            break;
#endif

#if (MCI_DRUM_LOAD_AVERAGE == ENABLED)
        case MCISENSORSWM_AI_DRUM_LOAD_AVERAGE:
            ret_val = (sint32)(Drum_Load_Torque*65536.0f);
            break;
#endif

        default:
            //
            break;
    }

    return(ret_val);
}


//---------------------------------------------------------------------------------------------------------------------
/**
 *  @brief      Read an float analog value
 *
 *  @param[in]  read_data Index to the parameter. See the @ref MciSensorsWm.h file
 *
 *  @return     the parameter value.
 *              The default return is zero.
 *              Not implemented functions returns -1
 */
float MciSensorsWm__GetFloatAnalog(MCISENSORSWM_FLOAT_AI_TYPE read_data)
{
    float ret_val;

    ret_val = -32768.0f;

    switch(read_data)
    {

#if (MCI_MEAN_VALUE_CALC == ENABLED)
        case MCISENSORSWM_FLOAT_AI_TORQUE_MEAN:
            ret_val = Torque_Mean_F;
            break;
#endif

#if (MCI_OVERHEATING_WASH == ENABLED)
        case MCISENSORSWM_FLOAT_AI_OVERHEATING_WASHING:
            ret_val = OH_Washing_Index_F;
            break;
#endif


#if (MCI_DRUM_LOAD_AVERAGE == ENABLED)
        case MCISENSORSWM_FLOAT_AI_DRUM_LOAD_AVERAGE:
            ret_val = Drum_Load_Torque;
            break;
#endif
        default:
            //
            break;
    }

    return ret_val;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 *  @brief      Returns Tramsission Ratio
 *
 *  @return     Transmission Ratio value with default motor direction (1 or -1)
 *              If no parameters are read, it return 1.0 or -1.0
 */
float32 MciSensorsWm__GetTransmissionRatio(void)
{
    float32 ratio = MOTOR_DIRECTION;

    if (ParamsReady == TRUE)
    {
        ratio *= MciSensorsWm_Params->Transmission_Ratio;
    }

    return(ratio);
}


//=====================================================================================================================
//-------------------------------------- Private Functions ------------------------------------------------------------
//=====================================================================================================================
#if (MCI_MEAN_VALUE_CALC == ENABLED)
static void MeanValueCalc(void)
{
    sint16 torque;                  // torque = result of torque estimator
    float torque_f;                 // torque = result of torque estimator
    sint16 torque_mean;
    float torque_mean_f;
    sint16 mean_over_drum_rev;
    uint16 temp_vibration_index;
    
    
    if(Mci__GetDigital(MOTOR0, MCI_DI_MOTOR_RUNNING) == TRUE)
    {
        torque = Mci__GetAnalog(MOTOR0, MCI_AI_LOAD_TORQUE_S16)>>TORQUE_NM256_2_PU_SHIFT; //TODO: put the shift into parameters
        torque_f = (1.0f/65536.0f) * Mci__GetAnalog(MOTOR0, MCI_AI_LOAD_TORQUE_S16) * (MciSensorsWm_Params->Transmission_Ratio * MOTOR_DIRECTION); //Nm_Drum

        // Accumulator update
        Torque_Acc = Torque_Acc + torque;
        Torque_Acc_F = Torque_Acc_F + torque_f;
        Vibra_Acc = Vibra_Acc + MATHCALC__ABS(torque - Torque_Mean);

        Turns_Counter += IQMATH__I1Q31MULI1Q15(MciSensorsWm_Params->K_Turns, MATHCALC__ABS((Mci__GetAnalog(MOTOR0, MCI_AI_ACTUAL_SPEED_S16)>>16)));
        
        Mean_Samples_Cnt++;

        // check spinning region
		if((Mci__GetAnalog(MOTOR0, MCI_AI_TARGET_SPEED_RPM_S16)>>16) > MciSensorsWm_Params->Spinning_Thr_Speed)
		{
			mean_over_drum_rev = MciSensorsWm_Params->Mean_Over_Drum_Rev << 2;
		}
		else
		{
			mean_over_drum_rev = MciSensorsWm_Params->Mean_Over_Drum_Rev;
		}

        if (IQMATH__IQ2IQ(Turns_Counter, IQ31, IQ15) >= mean_over_drum_rev)
        {
            // clear turns counter
            Turns_Counter = 0;

            //--------------------- Torque mean value calculation - begin ---------------------
            torque_mean = (Torque_Acc)/Mean_Samples_Cnt;
            torque_mean_f = (Torque_Acc_F)/((float)Mean_Samples_Cnt);

            // check spinning region
            if((Mci__GetAnalog(MOTOR0, MCI_AI_SPEED_REFERENCE_RPM_S16)>>16) >= MciSensorsWm_Params->Spinning_Thr_Speed)
            {
                // IIR filter for spinning region
                torque_mean = (sint16)(((sint32)torque_mean + (sint32) Torque_Mean)>>1);

                torque_mean_f = (torque_mean_f + Torque_Mean_F)/(2.0f);
            }

            // updating of torque_mean
            Torque_Mean = torque_mean;
            Torque_Mean_F = torque_mean_f;
            //--------------------- Torque mean value calculation - end ---------------------
            
            //--------------------- Unbalance Index calculation - begin ---------------------
            temp_vibration_index = (Vibra_Acc)/Mean_Samples_Cnt;
            
            // Vibration Index calculation - IIR filter
            Vibration_Index = (sint16)(((sint32)temp_vibration_index + (sint32) Vibration_Index)>>1);

            if(TRUE == Mci__GetDigital(MOTOR0, MCI_DI_REACHED_TARGET_SPEED))
            {
                if(Unbalance_Cnt < 65535)
                {
                    ++Unbalance_Cnt;
                }

                if(Unbalance_Cnt>=RATE_FOR_UNBALANCE_COUNTER)
                {
                    Balance = Vibration_Index;
                }
            }
            else    // [ not in steady state ]
            {
                Balance = 0;
                Unbalance_Cnt = 0;
            }
            //--------------------- Unbalance Index calculation - end ---------------------
            
            // clear accumulators
            Torque_Acc = 0;
            Torque_Acc_F = 0.0f;
            Vibra_Acc = 0;
            Mean_Samples_Cnt = 0;
        }
    }
    else
    {
    	MeanValueCalc_Reset();
    }
   
}


static void MeanValueCalc_Reset(void)
{
    // clear accumulators
    Torque_Acc = 0;
    Torque_Acc_F = 0.0f;
    Vibra_Acc = 0;
    Mean_Samples_Cnt = 0;
    Turns_Counter = 0;

    // clear sensors variables
    Torque_Mean = 0;
    Torque_Mean_F = 0.0f;
    Balance = 0;
    Unbalance_Cnt = 0;
    Vibration_Index = 0;
}
#endif


#if (MCI_OVERHEATING_WASH == ENABLED)
//---------------------------------------------------------------------------------------------------------------------
/**
 *  @brief     Overheating Washing
 *  @details   Calculation of Washing overheating index in [A^2].
 *  @param[in]
 *  @param[out]
 *  @return
 */
static void OverHeating_Washing(void)
{
	sint32 temp_a;
	sint32 temp_b;
	sint32 temp_c;

    float temp_a_f;
    float temp_b_f;
    float temp_c_f;

    float temp_sum;

	sint64 temp64;

	sint32 is_motor_2;


	if(Mci__GetAnalog(MOTOR0, MCI_AI_INTERNAL_MCI_STATE) > 2) //> MCI_INITIALIZING
	{
        temp_a = Mci__GetAnalog(MOTOR0, MCI_AI_CURR_PHASE_A_S16)>>1;
        temp_b = Mci__GetAnalog(MOTOR0, MCI_AI_CURR_PHASE_B_S16)>>1;
        temp_c = Mci__GetAnalog(MOTOR0, MCI_AI_CURR_PHASE_C_S16)>>1;

        temp_a = (sint32)IQMATH__REAL2IQ(temp_a,BASE_CURRENT,0); //A pu 17.15
        temp_b = (sint32)IQMATH__REAL2IQ(temp_b,BASE_CURRENT,0); //A pu 17.15
        temp_c = (sint32)IQMATH__REAL2IQ(temp_c,BASE_CURRENT,0); //A pu 17.15

        //square Ia^2 Ib^2 Ic^2
        temp_a = (sint64)IQMATH__I2Q30MULI17Q15(temp_a,temp_a); //A^2 pu 2.30
        temp_b = (sint64)IQMATH__I2Q30MULI17Q15(temp_b,temp_b); //A^2 pu 2.30
        temp_c = (sint64)IQMATH__I2Q30MULI17Q15(temp_c,temp_c); //A^2 pu 2.30


        temp64 = temp_a + temp_b + temp_c;
        temp64 = temp64>>IQ15; //2.30 -> 17.15


        //saturation
        if (temp64 > (sint64)MAX_31BIT_POSITIVE)
        {
            is_motor_2  = (sint32)MAX_31BIT_POSITIVE;
        }
        else if (temp64 < -(sint64)MAX_31BIT_NEGATIVE)
        {
            is_motor_2  = (sint32)MAX_31BIT_NEGATIVE;
        }
        else
        {
            is_motor_2  = (sint32)temp64;
        }

        //Update Accumulator
        IsMotor2_Acc = IsMotor2_Acc + is_motor_2;



        //Floating point calculation - begin
        temp_a_f  = (1.0f/65536.0f) * Mci__GetAnalog(MOTOR0, MCI_AI_CURR_PHASE_A_S16);
        temp_b_f  = (1.0f/65536.0f) * Mci__GetAnalog(MOTOR0, MCI_AI_CURR_PHASE_B_S16);
        temp_c_f  = (1.0f/65536.0f) * Mci__GetAnalog(MOTOR0, MCI_AI_CURR_PHASE_C_S16);


        //square Ia^2 Ib^2 Ic^2
        temp_a_f = temp_a_f*temp_a_f;
        temp_b_f = temp_b_f*temp_b_f;
        temp_c_f = temp_c_f*temp_c_f;

        temp_sum = temp_a_f + temp_b_f + temp_c_f;

        IsMotor2_Acc_F = IsMotor2_Acc_F + temp_sum;
        //Floating point calculation - begin


        //Update Counter
        OH_Washing_Cnt++;

        // Motor Stop Detection
        if(Mci__GetDigital(MOTOR0, MCI_DI_MOTOR_RUNNING) == FALSE)
        {
            if(OH_Washing_Movement_Flag == 1)
            {
                OH_Washing_Stop_Flag = 1;
                OH_Washing_Movement_Flag = 0;
            }
        }
        else
        {
            OH_Washing_Movement_Flag = 1;
        }


        if ((OH_Washing_Stop_Flag) || (OH_Washing_Cnt > OH_WASHING_MAX_PERIOD))
        {
            if(OH_Washing_Cnt > 0)
            {
                OH_Washing_Index = (sint16)((IsMotor2_Acc/(OH_Washing_Cnt*3))<<1); //multiplication of 2/3
                OH_Washing_Index_F = ((IsMotor2_Acc_F/((float)OH_Washing_Cnt*3.0f))*2.0f); //multiplication of 2/3
                OH_Washing_Stop_Flag = 0;
                OH_Washing_Cnt = 0;
                IsMotor2_Acc = 0;
                IsMotor2_Acc_F = 0.0f;
            }
        }
	}
}
/***************************************************************************/
#endif


#if (MCI_POWER_METER == ENABLED)
//---------------------------------------------------------------------------------------------------------------------
/**
 *  @brief     Power Meter
 *  @details   Calculation of Power and Energy line assorbed by inverter/motor
 *  @param[in]
 *  @param[out]
 *  @return
 */
static void PowerMeter(void)
{
	sint32 temp32;
	sint32 temp32_2;

    //----------- Electrical Power Calculation---------
	temp32 = Mci__GetAnalog(MOTOR0, MCI_AI_ACTIVE_POWER_S16)>>12;

	if(temp32 < 0)
	{
		temp32 = 0;
	}


	temp32 = (sint32)IQMATH__REAL2IQ(temp32,(sint16)(BASE_POWER*16),IQ15); //W pu 17.15
	//-------------------------------------------------

    //----------- Electrical Energy Calculation--------

	Energy_K_1 = Energy_K>>IQ15;
	Energy_K = Energy_K + IQMATH__I2Q30MULI17Q15(MciSensorsWm_Params->KEnergy,temp32);

	//delta energy
	temp32_2 = (Energy_K>>IQ15) - Energy_K_1;


	//power integration
	Energy_BackUp = Energy_BackUp + temp32_2;  //17.15

	if(Energy_BackUp < 0)
	{
		Energy_BackUp = 0;

	}
	//-------------------------------------------------


	//Sensors values update
	temp32_2 = IQMATH__I17Q15MULI17Q15(Energy_BackUp,MciSensorsWm_Params->EnergyConversion); //Ws

	Motor_Line_Power       =  IQMATH__Q15MULQ15(temp32,MciSensorsWm_Params->PowerConversion);
	Motor_Line_Energy_Low  =  (sint16)(temp32_2); //Ws
	Motor_Line_Energy_High =   (sint16)(temp32_2>>IQ15); //Ws

}
/***************************************************************************/
#endif


#if (MCI_DRUM_LOAD_AVERAGE == ENABLED)
static void Drum_Load_Average(void)
{
    sint32 torque;                  // torque = result of torque estimator
    float temp_float_acc;

    if(Mci__GetDigital(MOTOR0, MCI_DI_MOTOR_RUNNING) == TRUE)
    {
        torque = Mci__GetAnalog(MOTOR0, MCI_AI_LOAD_TORQUE_S16)>>6; //Nm * 1024

        // Accumulator update
        Drum_Load_Torque_Acc = Drum_Load_Torque_Acc + torque;

        Drum_Load_Torque_Samples_Cnt++;
    }
    else // Motor is stopped -> Calculate the drum load average
    {
        if(Drum_Load_Torque_Samples_Cnt > 0)
        {

            temp_float_acc = (float)(Drum_Load_Torque_Acc*MciSensorsWm_Params->Transmission_Ratio); // Nm_Motor_Acc_x1024 -> Nm_Drum_Acc_x1024;

            Drum_Load_Torque_Samples_Cnt = Drum_Load_Torque_Samples_Cnt<<10;  // Samples_Cnt * 1024

            Drum_Load_Torque =  temp_float_acc/((float)Drum_Load_Torque_Samples_Cnt);   // Drum_Load_Torque = Nm_Drum_Acc_x1024/(Samples_Cnt * 1024)


            //Reset Counters
            Drum_Load_Torque_Samples_Cnt = 0;
            Drum_Load_Torque_Acc = 0;
        }
    }
}
#endif

