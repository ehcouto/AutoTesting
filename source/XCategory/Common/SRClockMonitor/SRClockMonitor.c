/**
 *  @file
 *
 *  @defgroup CLASS_B
 *
 *  @brief      This module compares an internal clock source given by the 250us timer interrupt with an external clock of source.
 *
 *  @details    This module generates exceptions in case of the clocks get disaligned
 *
 *  $Header: SRClockMonitor.c 1.10 2012/06/18 18:04:25EDT MANOEL GARBUIO DE SOUZA (SOUZAMG) Exp  $
 *
 *  @copyright  Copyright 2010-$Date: 2012/06/18 18:04:25EDT $. Whirlpool Corporation. All rights reserved - CONFIDENTIAL
 */

//-------------------------------------- Include Files ----------------------------------------------------------------
#include "SRSystemConfig.h"
#include "SRClockMonitor.h"
#include "SRClockMonitor_prv.h"
#include "Micro.h"
#include "SRException.h"
#include "SRData.h"
//-------------------------------------- PUBLIC (Variables) -----------------------------------------------------------

#ifndef SRCLOCKMONITOR_FEATURE
    #define SRCLOCKMONITOR_FEATURE      ENABLED
#endif


//-------------------------------------- PRIVATE (Variables, Constants & Defines) -------------------------------------


#if (SRCLOCKMONITOR_FEATURE == ENABLED)
    static uint16 SRClockMonitor_Counter;
    static uint8 SRClock_First_Reading;
    static uint8 SRClockMonitor_ErrorCount;
    static uint16 N_SRClockMonitor_Counter;
    static uint8 N_SRClock_First_Reading;
    static uint8 N_SRClockMonitor_ErrorCount;

    #ifndef SRCLOCKMONITOR_MAX_ERROR_OCCURENCE
        #define SRCLOCKMONITOR_MAX_ERROR_OCCURENCE          4
    #endif

    #ifndef SRCLOCKMONITOR_INITIAL_DISREGARDED_READINGS
        #define SRCLOCKMONITOR_INITIAL_DISREGARDED_READINGS 4
    #endif

#endif


//-------------------------------------- PRIVATE (Function Prototypes) ------------------------------------------------


//=====================================================================================================================
//-------------------------------------- Public Functions -------------------------------------------------------------
//=====================================================================================================================

//---------------------------------------------------------------------------------------------------------------------
/**
 *  @brief      It Initializes the module SRClockMonitor and its variables
 *
 */
void SRClockMonitor__Initialize(void)
{

#if (SRCLOCKMONITOR_FEATURE == ENABLED)
    SRCLOCKMONITOR_INIT_FLOW();
    SRData__UpdateShort(&SRClockMonitor_Counter,&N_SRClockMonitor_Counter,0);
    SRData__UpdateByte(&SRClock_First_Reading,&N_SRClock_First_Reading,0);
    SRData__UpdateByte(&SRClockMonitor_ErrorCount,&N_SRClockMonitor_ErrorCount,0);
#endif
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Monitor 250us
 *  This method is responsible by the TimerInterrupot clock monitoring.
 *  It counts ticks based on timerInterruption for the MonitorExtclk compare with the external clock source.
 */
void SRClockMonitor__Monitor1ms(void)
{

#if (SRCLOCKMONITOR_FEATURE == ENABLED)
    SRCLOCKMONITOR_1MS_FLOW();
    if (SRData__CheckShort(&SRClockMonitor_Counter,&N_SRClockMonitor_Counter) == SRDATA_OK)
    {
        if (SRClockMonitor_Counter < 0xFFFF)
        {
            SRData__UpdateShort(&SRClockMonitor_Counter,&N_SRClockMonitor_Counter,SRClockMonitor_Counter+1);
            if (SRClockMonitor_Counter >= SRCLOCK_MAX_ABSOLUT_COUNTER)
            {
                SRException__Queue(SREXCEPTION_SRCLOCK_OVERRUN,(uint32)SRClockMonitor_Counter,0);
                SRData__UpdateShort(&SRClockMonitor_Counter,&N_SRClockMonitor_Counter,0);
            }
        }
    }
#endif
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief This method is responsible to check the internal clock by comparing the ticks counted by the Monitor250us
 * with the ext_ticks from the external source of clock.
 * @param ext_ticks numer of ticks expected to happen in the 250us interrupt since the last call of this method.
 */
void SRClockMonitor__MonitorExtClk(uint16 ext_ticks)
{
#if (SRCLOCKMONITOR_FEATURE == ENABLED)
    uint16 max;
    uint16 min;


    SRCLOCKMONITOR_EXT_FLOW();
    if (SRData__CheckByte(&SRClock_First_Reading,&N_SRClock_First_Reading) != SRDATA_OK)
    {
        SRData__UpdateByte(&SRClock_First_Reading,&N_SRClock_First_Reading,0);
    }

    //To avoid the lack of precision in the first interaction it ignores the first interactions,
    //after that we do expect the system to be in sync
    if (SRClock_First_Reading < SRCLOCKMONITOR_INITIAL_DISREGARDED_READINGS )
    {
        SRData__UpdateByte(&SRClock_First_Reading,&N_SRClock_First_Reading,SRClock_First_Reading+1);
    }
    else
    {
        if ((SRData__CheckShort(&SRClockMonitor_Counter,&N_SRClockMonitor_Counter) == SRDATA_OK) &&
            (SRData__CheckByte(&SRClockMonitor_ErrorCount,&N_SRClockMonitor_ErrorCount) == SRDATA_OK))
        {
            max = 0xFFFF;
            if ((0xFFFF - SRCLOCKMONITOR_CLK_TICKS_ABOVE) > ext_ticks)
            {
                max = ext_ticks + SRCLOCKMONITOR_CLK_TICKS_ABOVE;
            }
            min = 0;
            if (ext_ticks > SRCLOCKMONITOR_CLK_TICKS_BELOW)
            {
                min = ext_ticks -  SRCLOCKMONITOR_CLK_TICKS_BELOW;
            }

            if ((SRClockMonitor_Counter < min ) ||
                (SRClockMonitor_Counter > max))
            {

                SRData__UpdateByte(&SRClockMonitor_ErrorCount,&N_SRClockMonitor_ErrorCount,SRClockMonitor_ErrorCount+1);
                if (SRClockMonitor_ErrorCount >= SRCLOCKMONITOR_MAX_ERROR_OCCURENCE)
                {
                    // Out of Range
                    SRException__Queue(SREXCEPTION_SRCLOCK_TEST,(uint32)SRClockMonitor_Counter,(uint32)ext_ticks);
                }

            }
            else
            {
                SRData__UpdateByte(&SRClockMonitor_ErrorCount,&N_SRClockMonitor_ErrorCount,0);
            }

        }
    }
    SRData__UpdateShort(&SRClockMonitor_Counter,&N_SRClockMonitor_Counter,0);
#endif

}

//=====================================================================================================================
//-------------------------------------- Private Functions ------------------------------------------------------------
//=====================================================================================================================


