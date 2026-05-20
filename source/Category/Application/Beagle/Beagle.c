/**********************************************************************************************************************
 *  @file       Beagle.c
 *  @brief      Beagle module sends high speed data through SPI port with DMA operation.
 *
 *  @details    The protocol does not implement handshake.
 *
 *---------------------------------------------------------------------------------------------------------------------
 *------------------- Copyright 2013.  Whirlpool Corporation.  All rights reserved - CONFIDENTIAL ---------------------
 *---------------------------------------------------------------------------------------------------------------------
 */
#include "C_Extensions.h"
#if(__ICCRX__)
#include "Dtc.h"
#endif
#include "Spi.h"
#include "Gpio.h"
#include "Beagle.h"


#if( (defined BEAGLE_PORT) && (defined BEAGLE_PIN) && (defined BEAGLE_SPI_PORT) && (defined BEAGLE_BUFFER_SIZE))

    #define BEAGLE_MODULE_IS_ENABLED

    #define SET_FLAG_SSL_BIT()          GPIO__PIN_SET(BEAGLE_PORT, BEAGLE_PIN)
    #define RESET_FLAG_SSL_BIT()        GPIO__PIN_CLR(BEAGLE_PORT, BEAGLE_PIN)
    #define CONFIG_FLAG_SSL_BIT()       Gpio__PinConfig(BEAGLE_PORT, BEAGLE_PIN, OUTPUT_PUSHPULL)

    typedef enum
    {
        BEAGLE_START_TX = 0,
        BEAGLE_IGNORE_CALLS,
        BEAGLE_IDLE_TX,
        BEAGLE_ALL_STEPS
    } BEAGLE_STATE_TYPE;

    static BEAGLE_STATE_TYPE Beagle_State;
    static BEAGLE_DATA_TYPE *Beagle_Data_Pointer;

    static unsigned short Beagle_Ignore_Counter;
    static unsigned short Beagle_Skip_Counter;

#else
    #warning Beagle is compiled, but the parameters are not set in the parameter file, e.g. no data shall be sent.
#endif




//---------------------------------------------------------------------------------------------------------------------
/**
 *  @brief      Initialize the Beagle module and all of its variable.
 *
 *  @param      none.
 *  @return     none.
 */
void Beagle__Init(BEAGLE_DATA_TYPE *beagle_data_buffer)
{
#if defined BEAGLE_MODULE_IS_ENABLED
    CONFIG_FLAG_SSL_BIT();
    RESET_FLAG_SSL_BIT();

#if(__ICCRX__)
    Dtc__Initialize();
#endif

    Spi__Initialize(BEAGLE_SPI_PORT);

#if(__ICCARM__)
    Spi__SetConfiguration(BEAGLE_SPI_PORT, SPI_POLARITY_SCK_IDLE_0, SPI_EDGE_FIRST, SPI_ODER_MSB_FIRST);
    Spi__Enable(BEAGLE_SPI_PORT);
#endif

    Beagle_State = BEAGLE_IDLE_TX;

    Beagle_Skip_Counter = BEAGLE_SKIP_CALLS;        // Loads the default value to the timer.

    Beagle_Data_Pointer = beagle_data_buffer;
#endif
}



//---------------------------------------------------------------------------------------------------------------------
/**
 *  @brief      Load floating data to the buffer only when the previous buffer is already sent.
 *              In the case of sending data at top rate, call the load right before the Beagle__HighSpeedHandler.
 *              The BEAGLE_DATA_TYPE shall be "float" or "signed long"
 *
 *  @param      none.
 *  @return     none.
 */
BOOL_TYPE Beagle__TriggerTXData(void)
{
#if defined BEAGLE_MODULE_IS_ENABLED
    BOOL_TYPE ret_val = FALSE;

    if(Beagle_State == BEAGLE_IDLE_TX)
    {
        // Reset the ssl bit, just in case.
        RESET_FLAG_SSL_BIT();

        // Start the Beagle TX, after copying the buffer
        Beagle_State = BEAGLE_START_TX;
    }

    return(ret_val);
#else
    return(FALSE);
#endif
}


//---------------------------------------------------------------------------------------------------------------------
/**
 *  @brief      Handler to be called at high rate according to the application.
 *              It might be called at 16kHz or 4kHz or any other rate the user needs.
 *
 *  @param      none.
 *  @return     none.
 */
void Beagle__HighSpeedHandler(void)
{
#if defined BEAGLE_MODULE_IS_ENABLED

    if(Beagle_State == BEAGLE_IDLE_TX)
    {
        RESET_FLAG_SSL_BIT();
    }
    else if(Beagle_State == BEAGLE_START_TX)
    {
        SET_FLAG_SSL_BIT();
        Spi__Write(BEAGLE_SPI_PORT, (unsigned char *)Beagle_Data_Pointer, (unsigned short)(BEAGLE_BUFFER_SIZE*4));

        if(Beagle_Skip_Counter >= 1)
        {
            Beagle_State = BEAGLE_IGNORE_CALLS;
            Beagle_Ignore_Counter = Beagle_Skip_Counter;
        }
        else
        {
            Beagle_State = BEAGLE_IDLE_TX;
        }
    }
    else if(Beagle_State == BEAGLE_IGNORE_CALLS)
    {
        if(Beagle_Ignore_Counter)
        {
            Beagle_Ignore_Counter--;
        }
        else
        {
            Beagle_State = BEAGLE_IDLE_TX;
        }
    }

#endif
}



//---------------------------------------------------------------------------------------------------------------------
/**
 *  @brief      Update or change the rate of the Beagle sending rate.
 *
 *  @param      new_rate: define the new rate, the Beagle rate will depend up on the rate
 *              of the method Beagle__HighSpeedHandler()
 *  @return     none.
 */
void Beagle__ChangeRate(unsigned short new_rate)
{
#if defined BEAGLE_MODULE_IS_ENABLED
    Beagle_Skip_Counter = new_rate;
#endif
}
