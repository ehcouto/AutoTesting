/**
 *  @file
 *
 *  @brief      This file contains the source code for the Extraction AggregateP2PTorque module.
 *
 *  @details    Module calculates the Aggregate Peak To Peak Torque.  Value is used in the Extraction algorithm to
 *              determine if the load is balanced.
 *
 *  @par Module Owner
 *              Paul Spindler
 *
 *  $Header: AggregateP2PTorque.c 1.2 2016/01/19 12:54:46EST Paul F Spindler (SPINDPF) Exp  $
 *
 *  @copyright  Copyright 2012 - $Date: 2016/01/19 12:54:46EST $  Whirlpool Corporation.  All rights reserved - CONFIDENTIAL.
 */

//-------------------------------------- Include Files ----------------------------------------------------------------
#include "C_Extensions.h"

#include "AggregateP2PTorque.h"

#include "Dxdt.h"

//-------------------------------------- PRIVATE (Variables, Constants & Defines) -------------------------------------

typedef enum
{
    AGG_P2P_TORQ_STATE_INACTIVE = 0,
    AGG_P2P_TORQ_STATE_INITIAL_VALUE,
    AGG_P2P_TORQ_STATE_RUNNING
} AGGREGATE_P2P_TORQUE_STATE_TYPE;

//! module static variables
static struct
{
    unsigned short                  Num_Of_Samples;
    signed short                    Sum;                // tenths of Newton Meters
    signed short                    Value;              // tenths of Newton Meters
    AGGREGATE_P2P_TORQUE_STATE_TYPE State;
} Agg_P2p_Torq;

//-------------------------------------- PRIVATE (Function Prototypes) ------------------------------------------------

static AVERAGE_TORQUE_TYPE CalculateP2pTorque(void);
static void DxdtEventHandler(DXDT_EVENT_TYPE event);

//=====================================================================================================================
//-------------------------------------- Public Functions -------------------------------------------------------------
//=====================================================================================================================

//---------------------------------------------------------------------------------------------------------------------
/**
 *  @brief      Function initializes the module.
 *
 */
void AggregateP2PTorque__Initialize(void)
{
    AggregateP2PTorque__Clear();
    AggregateP2PTorque__Disable();
}


//---------------------------------------------------------------------------------------------------------------------
/**
 *  @brief      Function clears the task variables.
 *
 */
void AggregateP2PTorque__Clear(void)
{
    Agg_P2p_Torq.Num_Of_Samples = 0;
    Agg_P2p_Torq.Sum            = 0;
    Agg_P2p_Torq.Value          = 0;
}


//---------------------------------------------------------------------------------------------------------------------
/**
 *  @brief      Function disables the task.
 *
 */
void AggregateP2PTorque__Disable(void)
{
    Agg_P2p_Torq.State = AGG_P2P_TORQ_STATE_INACTIVE;
    Dxdt__UnregisterEventHandler(&DxdtEventHandler);
}


//---------------------------------------------------------------------------------------------------------------------
/**
 *  @brief      Function enables the task.
 *
 */
void AggregateP2PTorque__Enable(void)
{
    if ( Agg_P2p_Torq.State == AGG_P2P_TORQ_STATE_INACTIVE )
    {
        Dxdt__RegisterEventHandler(&DxdtEventHandler);
        Agg_P2p_Torq.State = AGG_P2P_TORQ_STATE_INITIAL_VALUE;
    }
    // else Agg_P2p_Torq.State == AGG_P2P_TORQ_STATE_INITIAL_VALUE || Agg_P2p_Torq.State == AGG_P2P_TORQ_STATE_RUNNING
    // task is already running

}


//---------------------------------------------------------------------------------------------------------------------
/**
 *  @brief      Function returns the current calculated value of the Aggregate Peak To Peak Torque.
 *
 *  @return     Aggregate Peak To Peak Torque
 */
AVERAGE_TORQUE_TYPE AggregateP2PTorque__GetValue(void)
{
    return ( Agg_P2p_Torq.Value );
}


//=====================================================================================================================
//-------------------------------------- Private Functions ------------------------------------------------------------
//=====================================================================================================================

//---------------------------------------------------------------------------------------------------------------------
/**
 *  @brief      Function calculates the current peak to peak value of the torque using the TpsBuf module for the max
 *              and min.
 *
 *  @return     Calculated peak to peak torque
 */
static AVERAGE_TORQUE_TYPE CalculateP2pTorque(void)
{
    DXDT_TORQUE_DATA_TYPE   dxdt;
    AVERAGE_TORQUE_TYPE     p2p_torque;

    Dxdt__GetData(&dxdt);
    p2p_torque = dxdt.Max - dxdt.Min;

    return (p2p_torque);
}


//---------------------------------------------------------------------------------------------------------------------
/**
 *  @brief      Function handles callback events from the Dxdt module.
 *  @details    Function calculates the Aggregate Peak To Peak Torque.
 *
 *  @param      event   Event posted from the Dxdt module
 *
 */
static void DxdtEventHandler(DXDT_EVENT_TYPE event)
{
    switch ( event )
    {
        case DXDT_EV_MAX_TORQUE_DETECTED:
        case DXDT_EV_MIN_TORQUE_DETECTED:
            switch (Agg_P2p_Torq.State)
            {
                case AGG_P2P_TORQ_STATE_INACTIVE:
                    // do nothing
                    break;

                case AGG_P2P_TORQ_STATE_INITIAL_VALUE:
                    // no previous peak nor trough to use, initial value passed.
                    Agg_P2p_Torq.State = AGG_P2P_TORQ_STATE_RUNNING;
                    break;

                case AGG_P2P_TORQ_STATE_RUNNING:
                    // calculate the aggregate of the torque
                    Agg_P2p_Torq.Sum += CalculateP2pTorque();                                               // add the new peak to peak value to the aggregate summation
                    Agg_P2p_Torq.Num_Of_Samples++;                                                          // increment the number of aggregate samples
                    Agg_P2p_Torq.Value = Agg_P2p_Torq.Sum / (signed short)(Agg_P2p_Torq.Num_Of_Samples);    // calculate the average
                    break;

                default:
                    break;
            }
            break;
        default:
            break;
    }
}
