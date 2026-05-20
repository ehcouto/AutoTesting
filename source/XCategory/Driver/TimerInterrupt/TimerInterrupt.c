/**
 *	  @file
 *    @brief    Timer Interrupt GDM module for MKV03X implements the
 *              Timer Interrupt Standard API (Application Programming Interface)
 *    @details  Timer Interrupt module gives the Timer interrupt service to the application,
 *              basically it is initialized and after that every defined interval an interruption is generated
 *              which drives the slotted Architecture where user can execute the Application
 *
 *  $Header: TimerInterrupt.c 1.3 2015/08/11 08:04:23EDT Natak Rahul (NATAKRB) Exp  $
 *  @copyright  Copyright 2012-$Date: 2015/08/11 08:04:23EDT $. Whirlpool Corporation. All rights reserved - CONFIDENTIAL
 *
 */
//-------------------------------------- Include Files ----------------------------------------------------------------
#include "C_Extensions.h"
#include "Micro.h"
#include "TimerInterrupt.h"
#include "string.h"

//-------------------------------------- PRIVATE (Variables, Constants & Defines) -------------------------------------
#ifndef SYSTICK_PREEMPTION_PRIORITY
    #define SYSTICK_PREEMPTION_PRIORITY 1
#endif

#ifndef SYSTICK_SUB_PRIORITY
    #define SYSTICK_SUB_PRIORITY        1
#endif

#define DEF_1MHZ                			1000000			//It generates a Timer clock freq = 1MHz
#define DEF_1KHZ                            1000			//It generates a Timer clock freq = 1KHz

#define TIMER_RELOAD_COUNT()            (Micro__GetClock()->HCLK_Frequency / (DEF_1MHZ/INTERRUPT_INTERVAL));
#define TIMER_RELOAD_LONG_COUNT()       (Micro__GetClock()->HCLK_Frequency / (DEF_1MHZ/TIMERITERUPT_LONG_INTERVAL_IN_MS));
#define HCLK_IN_MHZ()                   (Micro__GetClock()->HCLK_Frequency / DEF_1MHZ)
//=====================================================================================================================
//-------------------------------------- Public Functions -------------------------------------------------------------
//=====================================================================================================================
//---------------------------------------------------------------------------------------------------------------------
/**
 *    @brief    This method initializes the Timer interrupt module
 *    @details  1. This method should be called after initialization of all the modules like micro, Gpio, Hmi, Lsi,
 *    				Wide etc.
 *
 *              2. This function initializes the system tick timer and its interrupt and start the system tick timer.
 *		           Counter is in free running mode to generate periodical interrupts.
 *
 *		        3. You can change the SysTick IRQ priority by changing the below macro -
 *		        	#define SYSTICK_PREEMPTION_PRIORITY		0
 *
 *		        4. SysTick time base uses the below formula:
 *		                 Reload Value = SysTick Counter Clock (Hz) x  Desired Time base (s)
 */
void TimerInterrupt__Initialize(void)
{

	//Disable the System Core timer
	SYSTICK.CTRL &= SYSTICK_COUNTER_DISABLE;

	 //AHB clock divided by 8 selected as SysTick clock source.
	SYSTICK.LOAD  = TIMER_RELOAD_COUNT();

	//Set Priority for SysTick_IRQ_CHANNEL
	Micro__NVICEnableIRQ (SYSTICK_IRQ_CHANNEL, SYSTICK_PREEMPTION_PRIORITY, SYSTICK_SUB_PRIORITY);

	SYSTICK.VAL   = 0;

	// Enable SysTick IRQ CLock source and SysTick Timer
	SYSTICK.CTRL  = SYSTICK_CTRL_CLKSOURCE_MSK | SYSTICK_CTRL_TICKINT_MSK | SYSTICK_CTRL_ENABLE_MSK;

}


//---------------------------------------------------------------------------------------------------------------------
/**
 *    @brief    It sets the timer interrupt for long duration
 *    @details  Long interval value is depends on the 'TIMERITERUPT_LONG_INTERVAL_IN_MS' macro value
 */
void TimerInterrupt__SetLongInterval(void)
{
	//Disable the System Core timer
	SYSTICK.CTRL &= SYSTICK_COUNTER_DISABLE;

	 //AHB clock divided by 8 selected as SysTick clock source.
    SYSTICK.LOAD  = TIMER_RELOAD_LONG_COUNT();

    //Configure the SysTick handler priority
    Micro__NVICEnableIRQ (SYSTICK_IRQ_CHANNEL, SYSTICK_PREEMPTION_PRIORITY, SYSTICK_SUB_PRIORITY);

    SYSTICK.VAL   = 0;

    // Enable SysTick IRQ CLock source and SysTick Timer
    SYSTICK.CTRL  = SYSTICK_CTRL_CLKSOURCE_MSK | SYSTICK_CTRL_TICKINT_MSK | SYSTICK_CTRL_ENABLE_MSK;

    SERVICE_WATCHDOG();
}

