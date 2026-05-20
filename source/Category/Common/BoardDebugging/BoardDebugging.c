/**
 *  @file       BoardDebugging.c
 *
 *  @brief      Debugging features
 *
 *  @details
 *
 *  @section
 *
 *  @copyright  Copyright 2013-$Date: 2016/01/11 12:30:13CET $. Whirlpool Corporation. All rights reserved - CONFIDENTIAL
 */
//---------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------

//-------------------------------------- Include Files ----------------------------------------------------------------

#include "BoardDebugging.h"
#include "BoardDebugging_prv.h"
#include "mci.h"
#include "SpeedRefGen.h"

#ifndef BD_RESET
    #define BD_RESET                 DISABLED
#endif

#ifndef BD_MCI_CMD
    #define BD_MCI_CMD               DISABLED
#endif

#ifndef BD_ADC_NOISE_ASSESSMENT
    #define BD_ADC_NOISE_ASSESSMENT  DISABLED
#endif

#ifndef BD_PWM_ACCESS
    #define BD_PWM_ACCESS            DISABLED
#endif

#ifndef BD_ADC_SYNCH_ADJUST
    #define BD_ADC_SYNCH_ADJUST      DISABLED
#endif

#ifndef DEBUG_MASTERCOMMANDER
    #define DEBUG_MASTERCOMMANDER    DISABLED
#endif

#ifndef BD_LED
    #define BD_LED                   DISABLED
#endif

#ifndef BD_INTERRUPT_ANALYSIS
    #define BD_INTERRUPT_ANALYSIS    DISABLED
#endif

#if (BD_LED == ENABLED)
    #define LED_INIT_COUNTER         (uint16)(LED_INIT_COUNTER_MS / 25)
    #define LED_TIMER                (uint16)(LED_TIMER_MS / 25)
#endif
//-------------------------------------- PRIVATE (Variables, Constants & Defines) -------------------------------------
#if (DEBUG_MASTERCOMMANDER == ENABLED)
    #include "MasterCommander.h"
#endif


#if (BD_MCI_CMD == ENABLED)
    typedef enum
    {
        SELECT_STOP,
        SELECT_SPEED,
        SELECT_MANUAL,
        SELECT_PWM,
        SELECT_DISABLED,
        SELECT_PLTSPECIFIC,
    } SELECT_SPD_MANUAL_TYPE;

    static sint8 BD_Update_Cmd;

    static SELECT_SPD_MANUAL_TYPE BD_Select_Method;

    static MOTOR_ENUM_TYPE BD_Motor;

    static sint16 BD_Target_Speed;
    static sint16 BD_Target_Accel;
    static sint16 BD_Target_Stop;
    static MCI_INJECTION_TYPE BD_Manual_Method;
    static sint32 BD_Level_x32;
    static sint32 BD_Level_Rate_x32;
    static sint32 BD_Param_x32;
    static sint32 BD_Param_Rate_x32;

    #ifdef BD_PLT_SPECIFIC
    static uint8 BD_Plt_Specific_Buffer_Size;
    static uint8 BD_Plt_Specific_Buffer[10];
    #endif
    #ifndef BD_CUSTOM_CMD
        // define here macros for standard MCI commands
        #define BD_RUN(motor_index, target_speed, target_accel)                                              Mci__Run(motor_index, target_speed, target_accel)
        #define BD_MANUAL_INJ(motor_index, method, level_x32, level_rate_x32, param_x32, param_rate_x32)     Mci__Manual_Injection(motor_index, method, level_x32, level_rate_x32, param_x32, param_rate_x32)
        #define BD_STOP(motor_index, target_stop)                                                            Mci__Stop(motor_index, target_stop)
        // otherwise macros definition will be in prv file (for instance for Dishwasher application)

    #endif
#endif

#if (BD_RESET == ENABLED)
    #include "Micro.h"
    static uint8   BD_Reset = 255;  //  force reset from Board Debugging
#endif

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////// --- ADC NOISE ASSESMENT --/////////////////////////////////////////
#if (BD_ADC_NOISE_ASSESSMENT == ENABLED)
    #include "MotorSafetyMgr.h"
    #include "MathCalc.h"
    #include "Mcl.h"

// structure declaration
    typedef struct{
        sint16 Signal_K_1;
        sint16 Signal_K_2;
        uint16 Noise_Cnt;
        uint16 Noise_Ampl_Max;
    } NOISE_SIGNAL_TYPE;

    static NOISE_SIGNAL_TYPE Current_A;
    static NOISE_SIGNAL_TYPE Current_B;
    static NOISE_SIGNAL_TYPE Current_C;
    static NOISE_SIGNAL_TYPE DcBus;

    static uint16 Current_Noise_Thr;
    static uint16 DcBus_Noise_Thr;
    static uint16 Noise_Measure_Window;
    static uint16 Noise_Measure_Window_Cnt;

#endif
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////


    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////// --- PWM SIGNALS DEBUG ---//////////////////////////////////////////


#if (BD_PWM_ACCESS == ENABLED)
    #include "MclBasicTypes.h"
    #include "MotorSafetyMgr.h"
    #include "Mcl.h"
    static uint8 Pwm_Turn_On_Flag;
    static ABC_COOR_SYST_TYPE Pwm_Duty_Hw_Dbg;

    extern MCI_CONTROL_STATE_TYPE Mci_Control_State;           //!< Define the states used in this controller

#endif


#if (BD_ADC_SYNCH_ADJUST == ENABLED)
    static uint8  Adc_Conv_Synch_Adjust;
    static volatile sint16 Adc_Conv_Synch_Value;
#endif

#if (BD_LED == ENABLED)
	#include "Gpio.h"
    static BOOL_TYPE Is_Led_Blinking;
    static uint16 Led_Counter;
#endif


#if (BD_INTERRUPT_ANALYSIS == ENABLED)
    // CPU processing time and overload
    static volatile float32 BD_Fast_Isr_Ovl_Perc;
    static volatile uint16  BD_Fast_Isr_Max_us;
    static volatile uint16  BD_Fast_Isr_Duration_us;
    static volatile float32 BD_Pwm_Isr_Ovl_Perc;
    static volatile float32 BD_Pwm_Isr_Max_us;
    static volatile float32 BD_Pwm_Isr_Duration_us;
#endif
//-------------------------------------- PUBLIC (Variables) -----------------------------------------------------------




//-------------------------------------- PRIVATE (Function Prototypes) ------------------------------------------------

#if (BD_ADC_NOISE_ASSESSMENT == ENABLED)
static void AdcNoiseInit();
static void AdcNoiseHandler();
static void AdcNoiseMeasure(sint16 signal, NOISE_SIGNAL_TYPE* noise_signal, uint16 noise_thr);
#endif



//=====================================================================================================================
//-------------------------------------- Public Functions -------------------------------------------------------------
//=====================================================================================================================

uint8  My_MC_Cycle;
uint16 My_MC_Cycle_Counter;
uint16 My_MC_Cycle_Counter_On;
uint16 My_MC_Cycle_Counter_Off;
uint8  My_MC_Cycle_Phase;
uint32 My_MC_Cycle_Counter_Duration;
uint16 My_MC_Cycle_Duration_Min;

uint8 My_MC_Braking;
uint16 My_MC_Braking_Phase;
uint16 My_MC_Braking_Runs;
sint16 My_MC_Braking_Decel;
uint16 My_MC_Braking_Runs_Total;
uint32 My_MC_Braking_Counter;
uint8 My_MC_Braking_Region;

uint8 My_MC_Spin;
uint8 My_MC_Spin_Phase;
uint32 My_MC_Spin_Counter;
sint16 My_MC_Spin_Decel_Stop;

sint32 Clear_Fault_Timer;

#include "ThermalModel.h"

//---------------------------------------------------------------------------------------------------------------------
/**
 *  @brief      It Initializes the module BoardDebugging and its variables
 *
 *  @details    The selected debugging features are initialized
 *
 */
void BoardDebugging__Initialize(void)
{

#if (DEBUG_MASTERCOMMANDER == ENABLED)
    MasterCommander__Initialize();
#endif

#if (BD_ADC_NOISE_ASSESSMENT == ENABLED)
    AdcNoiseInit();
#endif

#if (BD_RESET == ENABLED)
    BD_Reset = 255;
#endif

#if (BD_MCI_CMD == ENABLED)

    BD_Select_Method = SELECT_SPEED;
    BD_Update_Cmd = 0;

    BD_Motor = BD_MOTOR_INIT;

    BD_Target_Speed = BD_SPEED_INIT;
    BD_Target_Accel = BD_ACCEL_INIT;
    BD_Target_Stop  = BD_STOP_INIT;

    BD_Manual_Method  = BD_MANUAL_METHOD_INIT;
    BD_Level_x32      = (sint32)(BD_LEVEL_INIT      * 32.0f);
    BD_Level_Rate_x32 = (sint32)(BD_LEVEL_RATE_INIT * 32.0f);
    BD_Param_x32      = (sint32)(BD_PARAM_INIT      * 32.0f);
    BD_Param_Rate_x32 = (sint32)(BD_PARAM_RATE_INIT * 32.0f);

#endif

#if (BD_PWM_ACCESS == ENABLED)
    Pwm_Duty_Hw_Dbg.A = 16384;
    Pwm_Duty_Hw_Dbg.B = 16384;
    Pwm_Duty_Hw_Dbg.C = 16384;
#endif

#if (BD_LED == ENABLED)
    Is_Led_Blinking = FALSE;
    Led_Counter = LED_INIT_COUNTER;
    Gpio__PinConfig(LED_PORT,LED_PIN,OUTPUT_PUSHPULL);
    GPIO__PIN_SET(LED_PORT,LED_PIN);
#endif

    My_MC_Cycle = (uint8)FALSE;
    My_MC_Cycle_Counter = 1;
    My_MC_Cycle_Counter_On = 10; //Secs
    My_MC_Cycle_Counter_Off = 5; //Secs
    My_MC_Cycle_Phase = 0;
    My_MC_Cycle_Counter_Duration = (uint32)60*40*3600; //1hr
    My_MC_Cycle_Duration_Min = 60; //60 minutes

    My_MC_Braking = (uint8)FALSE;
    My_MC_Braking_Phase = 0;
    My_MC_Braking_Runs = 0;
    My_MC_Braking_Decel = 20*15; //20 rpms/s
    My_MC_Braking_Runs_Total = 20;
    My_MC_Braking_Counter = 0;
    My_MC_Braking_Region = 1;

    My_MC_Spin = (uint8)FALSE;
    My_MC_Spin_Phase = 0;
    My_MC_Spin_Counter = 0;
    My_MC_Spin_Decel_Stop = 20*15;

    Clear_Fault_Timer = 3*40; //3 secs.
}


//---------------------------------------------------------------------------------------------------------------------
/**
 *  @brief      Pwm handler for MasterCommander. Rate: every 62.5 us
 *
 */


void BoardDebugging__PwmHandler(void)
{
#if (BD_ADC_NOISE_ASSESSMENT == ENABLED)
    // Adc noise calculation
    // make sure this is executed after offset calculation
    if (MotorSafetyMgr__IsInIdle() || (Mci__GetAnalog(MOTOR0, MCI_AI_INTERNAL_MCI_STATE) >= 3)) // >= MCI_IDLE
    {
        AdcNoiseHandler();
    }
#endif


#if (BD_PWM_ACCESS == ENABLED)
    if(Pwm_Turn_On_Flag && (MotorSafetyMgr__DoesClassAHaveAccessToPwm() == TRUE))
    {
        if (BD_Update_Cmd != 0)
        {
            // Apply the PWM
            MotorSafetyMgr__UpdatePwm(Pwm_Duty_Hw_Dbg.A, Pwm_Duty_Hw_Dbg.B, Pwm_Duty_Hw_Dbg.C);

            BD_Update_Cmd = 0;
        }
    }
#endif


#if (BD_ADC_SYNCH_ADJUST == ENABLED)
    if (Adc_Conv_Synch_Adjust != 0)
    {
        SRMCAtod__SetSyncDelay(Adc_Conv_Synch_Value);
        Adc_Conv_Synch_Adjust  = 0;
    }
#endif

#if (DEBUG_MASTERCOMMANDER == ENABLED)
    MasterCommander__PwmHandler();
#endif
}

//---------------------------------------------------------------------------------------------------------------------
/**
 *  @brief      Board Debugging handler for 250 us
 *
 */
void BoardDebugging__250usHandler(void)
{
#if (DEBUG_MASTERCOMMANDER == ENABLED)
    MasterCommander__FastHandler();
#endif
}


//---------------------------------------------------------------------------------------------------------------------
/**
 *  @brief      Board Debugging handler for 25 ms
 *
 */
#pragma optimize = none
void BoardDebugging__25msHandler(void)
{
#if (BD_RESET == ENABLED)
    if (BD_Reset != 255)
    {
        Micro__ForceReset((MICRO_RESET_MODE_TYPE)BD_Reset);
    }
#endif

#if (BD_PWM_ACCESS == ENABLED)
    if ((BD_Select_Method == SELECT_PWM) && (BD_Update_Cmd != 0))
    {
        if (Pwm_Turn_On_Flag == 0)
        {
            Pwm_Turn_On_Flag = 1;
            MotorSafetyMgr__Prepare2StartMotor();
            // put MCI in debug state
            Mci_Control_State = MCI_DEBUGGING;
            Mcl__SetDigital(MCL_WRITE_INJECTION_DC_VOLTAGE);
        }
    }
    if((BD_Select_Method == SELECT_STOP) && Pwm_Turn_On_Flag == 1)
    {
        Pwm_Turn_On_Flag = 0;
        MotorSafetyMgr__Prepare2StopMotor();
        BD_Select_Method = SELECT_DISABLED;
        Mci_Control_State = MCI_IDLE;
    }
#endif

#if (BD_MCI_CMD == ENABLED)
    if((BD_Select_Method == SELECT_SPEED) && (BD_Update_Cmd != 0))
    {
        if(BD_Target_Speed != 0)
        {
            BD_Update_Cmd = BD_RUN(BD_Motor, BD_Target_Speed, BD_Target_Accel);
        }
        else
        {
            BD_Update_Cmd = BD_STOP(BD_Motor, BD_Target_Accel);
        }
    }
    else if((BD_Select_Method == SELECT_MANUAL) && (BD_Update_Cmd != 0))
    {
        BD_Update_Cmd = BD_MANUAL_INJ(BD_Motor, BD_Manual_Method, BD_Level_x32, BD_Level_Rate_x32, BD_Param_x32, BD_Param_Rate_x32);
    }
    else if(BD_Select_Method == SELECT_STOP)
    {
        BD_STOP(BD_Motor, BD_Target_Stop);
        BD_Select_Method = SELECT_DISABLED;
    }
    else if(BD_Select_Method == SELECT_DISABLED)
    {
        BD_Update_Cmd = 0;
        BD_Select_Method = SELECT_SPEED;
    }
#ifdef BD_PLT_SPECIFIC
    else if ((BD_Select_Method == SELECT_PLTSPECIFIC) && (BD_Update_Cmd != 0))
    {
        BD_Update_Cmd = 0;
        BD_PLT_SPECIFIC(BD_Motor, BD_Plt_Specific_Buffer);
    }
#endif

    if (BD_Update_Cmd > 0)
    {
        BD_Update_Cmd = 0;
    }
#endif


#if (BD_ADC_NOISE_ASSESSMENT == ENABLED)
    Noise_Measure_Window_Cnt++;
    if(Noise_Measure_Window_Cnt == Noise_Measure_Window)
    {
        Noise_Measure_Window_Cnt = 0;
        Current_A.Noise_Cnt = 0;
        Current_B.Noise_Cnt = 0;
        Current_C.Noise_Cnt = 0;
        DcBus.Noise_Cnt     = 0;
    }
#endif

#if (BD_LED == ENABLED)
    if(Is_Led_Blinking == FALSE)
    {
        GPIO__PIN_SET(LED_PORT,LED_PIN);
        Led_Counter--;
        if(!Led_Counter)
        {
            Led_Counter = LED_TIMER;
            Is_Led_Blinking = TRUE;
        }
    }
    else
    {
        Led_Counter--;
        if(!Led_Counter)
        {
            GPIO__PIN_TOGGLE(LED_PORT,LED_PIN);
            Led_Counter = LED_TIMER;
            Is_Led_Blinking = TRUE;
        }
    }
#endif

    uint8 temp;

    temp = My_MC_Cycle + My_MC_Braking + My_MC_Spin;

    if(temp > 1) //Sanity check. Disable cycles if more than one is active.
    {
    	BD_STOP(BD_Motor, 10000);
    	My_MC_Cycle = (uint8)FALSE;
    	My_MC_Braking = (uint8)FALSE;
    	My_MC_Spin = (uint8)FALSE;
    }

    // --- Cycle Design ---

    if(My_MC_Cycle == (uint8)TRUE)
    {
    	if(My_MC_Cycle_Phase == 0) //Init MC Cycle...
    	{
        	Mci__ClearError(MOTOR0);
    		My_MC_Cycle_Counter_Duration = (uint32)((uint32)My_MC_Cycle_Duration_Min * 60 * 40)+1;
        	My_MC_Cycle_Phase = 4;
        	My_MC_Cycle_Counter = 1; //Start Immediately...
        	BD_Target_Stop = BD_Target_Accel;
    	}
    	else if(My_MC_Cycle_Phase == 1) //Check for stop...
    	{
    		My_MC_Cycle_Counter--;
    		if(!My_MC_Cycle_Counter)
    		{
        		BD_STOP(BD_Motor, BD_Target_Stop);
        		My_MC_Cycle_Phase = 2;
        		My_MC_Cycle_Counter = My_MC_Cycle_Counter_Off * 40; //Off Time
    		}
    	}
    	else if(My_MC_Cycle_Phase == 2) //Init Negative Cycle
    	{
    		My_MC_Cycle_Counter--;
    		if(!My_MC_Cycle_Counter)
    		{
        		BD_RUN(BD_Motor, -BD_Target_Speed, BD_Target_Accel);
        		My_MC_Cycle_Phase = 3;
        		My_MC_Cycle_Counter = My_MC_Cycle_Counter_On * 40; //On Time
    		}
    	}
    	else if(My_MC_Cycle_Phase == 3) //Cycle Negative...
    	{
    		My_MC_Cycle_Counter--;
    		if(!My_MC_Cycle_Counter)
    		{
        		BD_STOP(BD_Motor, BD_Target_Stop);
        		My_MC_Cycle_Phase = 4;
        		My_MC_Cycle_Counter = My_MC_Cycle_Counter_Off * 40; //Off Time
    		}
    	}
    	else if(My_MC_Cycle_Phase == 4) //Init Positive Cycle
    	{
    		My_MC_Cycle_Counter--;
    		if(!My_MC_Cycle_Counter)
    		{
        		BD_RUN(BD_Motor, BD_Target_Speed, BD_Target_Accel);
        		My_MC_Cycle_Phase = 1;
        		My_MC_Cycle_Counter = My_MC_Cycle_Counter_On * 40; //On Time
    		}
    	}

    	//Stop cycle
    	if((My_MC_Cycle_Phase == 5) ||
    	   (ThermalModel__GetStatorTemperature() > 140.0f))//Force Cycle Stop by user or temperature...
    	{
    		My_MC_Cycle_Counter_Duration = 1; //will force the stop on next cycle over check...

    	}

    	//Check if total cycle is over...
    	My_MC_Cycle_Counter_Duration--;
    	if(!My_MC_Cycle_Counter_Duration)
    	{
    		BD_STOP(BD_Motor, BD_Target_Stop);
    		My_MC_Cycle_Phase = 0;
    		My_MC_Cycle = (uint8)FALSE;
    	}


    }


    // --- Braking Testing ---

    if(My_MC_Braking == (uint8)TRUE)
    {
    	if(My_MC_Braking_Phase == 0) //Init MC Cycle...
    	{
    		My_MC_Braking_Runs = 1;
    		My_MC_Braking_Counter = 5 * 40; //5 secs in plateau..
        	My_MC_Braking_Phase = 1;
        	Mci__ClearError(MOTOR0); //Clear any previous fault...

        	if(My_MC_Braking_Region == 2)
        	{
        		BD_RUN(BD_Motor, 500*15, 300);
        	}
        	else if(My_MC_Braking_Region == 1)
        	{
        		BD_RUN(BD_Motor, 150*15, 300);
        	}
    	}
    	else if(My_MC_Braking_Phase == 1) //Check plateau... and Brake.
    	{
    		if(SpeedRefGen__GetStatus() == SPEED_REF_AT_PLATEAU)
    		{
    			My_MC_Braking_Counter--;
    			if(!My_MC_Braking_Counter)
    			{
    	        	if(My_MC_Braking_Region == 2)
    	        	{
    	        		BD_RUN(BD_Motor, 300*15, My_MC_Braking_Decel);
    	        	}
    	        	else if(My_MC_Braking_Region == 1)
    	        	{
    	        		BD_RUN(BD_Motor, 100*15, My_MC_Braking_Decel);
    	        	}
    	        	My_MC_Braking_Phase = 2;
    	        	My_MC_Braking_Counter = 5 * 40; //5 secs in plateau..
    			}
    		}
    	}
    	else if(My_MC_Braking_Phase == 2) //Wait plateau after braking...
    	{
    		if(SpeedRefGen__GetStatus() == SPEED_REF_AT_PLATEAU)
    		{
    			My_MC_Braking_Counter--;
    			if(!My_MC_Braking_Counter)
    			{
    	        	My_MC_Braking_Phase = 3;
    			}
    		}
    	}
    	else if(My_MC_Braking_Phase == 3) //Keep Testing...
    	{
    		My_MC_Braking_Runs++;
    		My_MC_Braking_Counter = 5 * 40;
        	My_MC_Braking_Phase = 1;

        	if(My_MC_Braking_Region == 2)
        	{
        		BD_RUN(BD_Motor, 500*15, 300);
        	}
        	else if(My_MC_Braking_Region == 1)
        	{
        		BD_RUN(BD_Motor, 150*15, 300);
        	}
    	}


    	//Stop cycle
    	if((My_MC_Braking_Phase == 10) ||
    	   (ThermalModel__GetStatorTemperature() > 140.0f))//Force Cycle Stop by user or temperature...
    	{
    		My_MC_Braking_Runs = 100;
    	}

    	//Check if total cycle is over...
    	if(My_MC_Braking_Runs > My_MC_Braking_Runs_Total)
    	{
    		BD_STOP(BD_Motor, BD_Target_Stop);
    		My_MC_Braking_Phase = 0;
    		My_MC_Braking = (uint8)FALSE;
    	}
    }




    // --- Spin Testing ---

        if(My_MC_Spin == (uint8)TRUE)
        {
        	if(My_MC_Spin_Phase == 0) //Init MC Cycle...
        	{
        		My_MC_Spin_Counter = 30 * 40; //30 secs in plateau..
            	My_MC_Spin_Phase = 1;
            	Mci__ClearError(MOTOR0); //Clear any previous fault...

           		BD_RUN(BD_Motor, 350*15, 25*15); //350rpm
        	}
        	else if(My_MC_Spin_Phase == 1) //Check plateau... and Brake.
        	{
        		if(SpeedRefGen__GetStatus() == SPEED_REF_AT_PLATEAU)
        		{
        			My_MC_Spin_Counter--;
        			if(!My_MC_Spin_Counter)
        			{
       	        		BD_RUN(BD_Motor, 500*15, 20*15);
        	        	My_MC_Spin_Phase = 2;
        	        	My_MC_Spin_Counter = 30 * 40; //5 secs in plateau..
        			}
        		}
        	}
        	else if(My_MC_Spin_Phase == 2) //Check plateau... and Brake.
        	{
        		if(SpeedRefGen__GetStatus() == SPEED_REF_AT_PLATEAU)
        		{
        			My_MC_Spin_Counter--;
        			if(!My_MC_Spin_Counter)
        			{
       	        		BD_RUN(BD_Motor, 800*15, 20*15);
        	        	My_MC_Spin_Phase = 3;
        	        	My_MC_Spin_Counter = 30 * 40; //5 secs in plateau..
        			}
        		}
        	}
        	else if(My_MC_Spin_Phase == 3) //Check plateau... and Brake.
        	{
        		if(SpeedRefGen__GetStatus() == SPEED_REF_AT_PLATEAU)
        		{
        			My_MC_Spin_Counter--;
        			if(!My_MC_Spin_Counter)
        			{
       	        		BD_RUN(BD_Motor, 1000*15, 10*15);
        	        	My_MC_Spin_Phase = 4;
        	        	My_MC_Spin_Counter = 30 * 40; //5 secs in plateau..
        			}
        		}
        	}
        	else if(My_MC_Spin_Phase == 4) //Check plateau... and Brake.
        	{
        		if(SpeedRefGen__GetStatus() == SPEED_REF_AT_PLATEAU)
        		{
        			My_MC_Spin_Counter--;
        			if(!My_MC_Spin_Counter)
        			{
       	        		BD_RUN(BD_Motor, 1200*15, 5*15);
        	        	My_MC_Spin_Phase = 5;
        	        	My_MC_Spin_Counter = 30 * 40; //5 secs in plateau..
        			}
        		}
        	}
        	else if(My_MC_Spin_Phase == 5) //Check plateau... and Brake.
        	{
        		if(SpeedRefGen__GetStatus() == SPEED_REF_AT_PLATEAU)
        		{
        			My_MC_Spin_Counter--;
        			if(!My_MC_Spin_Counter)
        			{
       	        		My_MC_Spin_Phase = 10; //Go to Stop...
        			}
        		}
        	}


        	//Stop cycle
        	if((My_MC_Spin_Phase == 10) ||
        	   (ThermalModel__GetStatorTemperature() > 140.0f))//Force Cycle Stop by user or temperature...
        	{
        		BD_STOP(BD_Motor, My_MC_Spin_Decel_Stop);
        		My_MC_Spin_Phase = 0;
        		My_MC_Spin = (uint8)FALSE;
        	}
        }


        //Check and Clear Faults...
        if(Mci__GetErrorList(MOTOR0)) //Any fault detected?
        {
        	Clear_Fault_Timer--;
        	if(Clear_Fault_Timer <= 0)
        	{
        		if(Mci__ClearError(MOTOR0) == TRUE)
        		{
        			Clear_Fault_Timer = 3*40; //3 secs
        		}
        	}
        }
}

#if (BD_INTERRUPT_ANALYSIS == ENABLED)
/*
 *   BD_Get_Fast_Duration_us(), BD_Fast_Overload(), BD_Get_PWM_Duration_us(), BD_PWM_Overload() are
 *   private functions. Implementation is micro specific, so they are placed in the BoardDebugging_prv.h file
 *   with the pragma inline
 */

void BoardDebugging__FastIsrAnalysis(void)
{
    uint16 temp;
    temp = BD_Get_Fast_Duration_us();
    BD_Fast_Isr_Duration_us = temp;

    if (temp > BD_Fast_Isr_Max_us)
    {   // update the max counter
        BD_Fast_Isr_Max_us = BD_Fast_Isr_Duration_us;
    }
    // CPU overload
    BD_Fast_Overload(BD_Fast_Isr_Ovl_Perc);
}

void BoardDebugging__PwmIsrAnalysis(void)
{
    float32 temp;
    temp = BD_Get_PWM_Duration_us();
    BD_Pwm_Isr_Duration_us = temp;
    if (temp > BD_Pwm_Isr_Max_us)
    {   // update the max counter
        BD_Pwm_Isr_Max_us = BD_Pwm_Isr_Duration_us;
    }

    // CPU overload
    BD_PWM_Overload(BD_Pwm_Isr_Ovl_Perc);
}
#endif



//=====================================================================================================================
//-------------------------------------- Private Functions ------------------------------------------------------------
//=====================================================================================================================

#if (BD_ADC_NOISE_ASSESSMENT == ENABLED)
static void AdcNoiseInit()
{
    //Adc Noise counters initialization
    Current_A.Signal_K_1     = 0;
    Current_B.Signal_K_1     = 0;
    Current_C.Signal_K_1     = 0;
    DcBus.Signal_K_1         = 0;

    Current_A.Signal_K_2     = 0;
    Current_B.Signal_K_2     = 0;
    Current_C.Signal_K_2     = 0;
    DcBus.Signal_K_2         = 0;


    Current_A.Noise_Ampl_Max = 0;
    Current_B.Noise_Ampl_Max = 0;
    Current_C.Noise_Ampl_Max = 0;
    DcBus.Noise_Ampl_Max     = 0;

    Current_A.Noise_Cnt      = 0;
    Current_B.Noise_Cnt      = 0;
    Current_C.Noise_Cnt      = 0;
    DcBus.Noise_Cnt          = 0;

    Current_Noise_Thr = BD_CURRENT_NOISE_THR;
    DcBus_Noise_Thr = BD_DC_BUS_NOISE_THR;

    Noise_Measure_Window = BD_NOISE_WINDOW;
    Noise_Measure_Window_Cnt = 0;

}

static void AdcNoiseHandler()
{
    sint16 current_a;
    sint16 current_b;
    sint16 current_c;
    sint16 dc_bus;

    // convert floating point mcl currents and DCbus voltage back to 1.15 notation
    // (to be used, for instance, in case of third current reconstruction)
    current_a = (sint16) BD_CURRENT(Mcl_IO.Is_ABC.A);
    current_b = (sint16) BD_CURRENT(Mcl_IO.Is_ABC.B);
    current_c = (sint16) BD_CURRENT(Mcl_IO.Is_ABC.C);
    dc_bus    = (sint16) BD_VOLTAGE(Mcl_IO.Vdc);


    //--------------- Motor Current Adc Noise measurement -----------------------
    AdcNoiseMeasure(current_a, &Current_A, Current_Noise_Thr);
    AdcNoiseMeasure(current_b, &Current_B, Current_Noise_Thr);
    AdcNoiseMeasure(current_c, &Current_C, Current_Noise_Thr);

    //--------------- Dc Bus  Noise measurement -------------------------
    AdcNoiseMeasure(dc_bus, &DcBus, DcBus_Noise_Thr);

}


static void AdcNoiseMeasure(sint16 signal, NOISE_SIGNAL_TYPE* noise_signal, uint16 noise_thr)
{
    sint16 signal_mean;
    sint16 signal_delta;


    signal_mean = (signal>>1) + (noise_signal->Signal_K_2 >>1);

    signal_delta = MATHCALC__ABS(noise_signal->Signal_K_1 - signal_mean);

    if(signal_delta > noise_thr)
    {
        if(noise_signal->Noise_Ampl_Max < signal_delta)
        {
            noise_signal->Noise_Ampl_Max = signal_delta;
        }

        noise_signal->Noise_Cnt++;
    }

    noise_signal->Signal_K_2 = noise_signal->Signal_K_1;
    noise_signal->Signal_K_1 = signal;

}


#endif
