/**
 *  @file       
 *
 *  @brief      Source code for the AverageTorque module.
 *
 *  @details    Calculates an average of the Mci module torque.
 *
 *  $Header: AverageTorque.c 1.2 2016/01/19 12:55:30EST Paul F Spindler (SPINDPF) Exp  $
 *
 *  @copyright  Copyright 2014-$Date: 2016/01/19 12:55:30EST $. Whirlpool Corporation. All rights reserved - CONFIDENTIAL
 */
//---------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------


//-------------------------------------- Include Files ----------------------------------------------------------------
#include "C_Extensions.h"

#include "AverageTorque.h"
#include "AverageTorque_prv.h"

#include "Callback.h"
#include "string.h"

//-------------------------------------- PRIVATE (Variables, Constants & Defines) -------------------------------------

//! List of module states
typedef enum
{
    STATE_INACTIVE = 0,
    STATE_ACTIVE
} AVERAGE_TORQUE_STATE_TYPE;

//! declaration of variables used to calculate the average torque reading
typedef struct
{
    unsigned char           Num_Of_Readings;        // number of 1msec readings
    signed long             Sum_Of_Readings;        // sum of torque readings read from the Mci module at 1msec rate
} BUFFER_TYPE;

//! declaration of variables that hold the calculated value of the average torque
typedef struct
{
    AVERAGE_TORQUE_TYPE     Interrupt;              // value calculated in a interrupt
    AVERAGE_TORQUE_TYPE     Non_Interrupt;          // value calculated in a non-interrupt
} CALCULATE_VALUE_TYPE;


//! define the callback control structure module static variable
DEFINE_CALLBACK_CONTROL_STRUCTURE(Average_Torque_Callback_Control_Structure, SIZE_OF_CALLBACK_REGISTER);


//! definition of module static variables
static struct
{
    BUFFER_TYPE                 Buffer;
    CALCULATE_VALUE_TYPE        Calculated_Value;      // the current calculated average torque value
    unsigned char               New_Calculated_Value:1; // 1 = a new Calculated_Value is available; 0 = new Calculated_Value IS NOT available
    AVERAGE_TORQUE_STATE_TYPE   State;
} Aver_Torq;

//-------------------------------------- PRIVATE (Function Prototypes) ------------------------------------------------

static void ClearBuffer(void);

//=====================================================================================================================
//-------------------------------------- Public Functions -------------------------------------------------------------
//=====================================================================================================================

//---------------------------------------------------------------------------------------------------------------------
/**
 *  @brief      It Initializes the module AverageTorque and its variables
 *
 */
void AverageTorque__Initialize(void)
{
    memset(&Aver_Torq, 0, sizeof(Aver_Torq));
    Callback__Initialize(&Average_Torque_Callback_Control_Structure);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 *  @brief      Module background functions to be executed at a 1msec rate.
 */
void AverageTorque__Handler1msec(void)
{
    switch ( Aver_Torq.State )
    {
        default:
        case STATE_INACTIVE:
            break;
        case STATE_ACTIVE:
            Aver_Torq.Buffer.Sum_Of_Readings += (signed long)INPUT_TORQUE;     // add the current torque reading to the sum
            Aver_Torq.Buffer.Num_Of_Readings++;
            if (Aver_Torq.Buffer.Num_Of_Readings >= CALCULATED_AVERAGE_TORQUE_DATA_RATE )
            {                                           // target number of torque readings has been reached
                Aver_Torq.Calculated_Value.Interrupt = (AVERAGE_TORQUE_TYPE)(Aver_Torq.Buffer.Sum_Of_Readings / (signed long)Aver_Torq.Buffer.Num_Of_Readings);
                ClearBuffer();
                Aver_Torq.New_Calculated_Value = TRUE;           // set flag to TRUE so 5msec slot knows to post event that a new value is available
            }
            break;
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 *  @brief      Module background functions to be executed at a 5msec rate.
 */
void AverageTorque__Handler5msec(void)
{
    switch ( Aver_Torq.State )
    {
        default:
        case STATE_INACTIVE:
            break;
        case STATE_ACTIVE:
            DISABLE_INTERRUPTS();
            if ( Aver_Torq.New_Calculated_Value == TRUE )
            {
                Aver_Torq.New_Calculated_Value = FALSE;
                Aver_Torq.Calculated_Value.Non_Interrupt = Aver_Torq.Calculated_Value.Interrupt;
                ENABLE_INTERRUPTS();
                Callback__Notify(&Average_Torque_Callback_Control_Structure, (CALLBACK_EVENT_TYPE)AVERAGE_TORQUE_EV_NEW_VALUE_CALCULATED);
            }
            else
            {
                ENABLE_INTERRUPTS();
            }
            break;
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 *  @brief      Disables the module.
 *  @details    Designed to be called from a non-interrupt
 */
void AverageTorque__Disable(void)
{
    Aver_Torq.State = STATE_INACTIVE;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 *  @brief      Enables the module.
 *  @details    Designed to be called from a non-interrupt
 */

void AverageTorque__Enable(void)
{
    switch ( Aver_Torq.State )
    {
        default:
        case STATE_INACTIVE:
            DISABLE_INTERRUPTS();
            ClearBuffer();
            Aver_Torq.New_Calculated_Value = FALSE;
            ENABLE_INTERRUPTS();
            Aver_Torq.State = STATE_ACTIVE;
            break;
        case STATE_ACTIVE:
            // do nothing, module already active
            break;
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 *  @brief      Returns the current value of the average torque
 *
 *  return      AVERAGE_TORQUE_TYPE
 */
AVERAGE_TORQUE_TYPE AverageTorque__GetValue(void)
{
    return ( Aver_Torq.Calculated_Value.Non_Interrupt);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 *  @brief      Registers a callback to the module
 *
 *  @param      callback_address   The callback function to register
 */
void AverageTorque__RegisterCallback(AVERAGE_TORQUE_EVENT_HANDLER_TYPE event_handler)
{
    Callback__Register(&Average_Torque_Callback_Control_Structure, (CALLBACK_HANDLER_TYPE)event_handler);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 *  @brief      unregisters a callback with the module
 *
 *  @param      callback_address    The callback function to register
 */
void AverageTorque__UnregisterCallback(AVERAGE_TORQUE_EVENT_HANDLER_TYPE event_handler)
{
    Callback__Unregister(&Average_Torque_Callback_Control_Structure, (CALLBACK_HANDLER_TYPE)event_handler);
}


//=====================================================================================================================
//-------------------------------------- Private Functions ------------------------------------------------------------
//=====================================================================================================================

//---------------------------------------------------------------------------------------------------------------------
/**
 *  @brief      Clears buffer used to calculate the average torque value.
 *  @details    NOTE: variables in this function are written to in a interrupt
 */
static void ClearBuffer(void)
{
    memset(&Aver_Torq.Buffer, 0, sizeof(Aver_Torq.Buffer));
}

