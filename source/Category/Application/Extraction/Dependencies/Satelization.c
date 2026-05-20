/**
 *  @file
 *
 *  @brief      Calculates the Satelization Speed during the Extraction Distribution Algorithm
 *
 *  @details    Uses the WindowPower to determine if the load has Satelized.
 *
 *  @par        Module Owner
 *              Paul Spindler
 *
 *  $Header: Satelization.c 1.2 2016/01/19 12:57:49EST Paul F Spindler (SPINDPF) Exp  $
 *
 *  @copyright  Copyright 2012 - $Date: 2016/01/19 12:57:49EST $  Whirlpool Corporation.  All rights reserved - CONFIDENTIAL.
 */

//-------------------------------------- Include Files ----------------------------------------------------------------
#include "C_Extensions.h"

#include "Satelization.h"
#include "Satelization_prv.h"

#include "WindowPower.h"

#include "string.h"

//-------------------------------------- PRIVATE (Variables, Constants & Defines) -------------------------------------

typedef enum
{
    STATE_INACTIVE = 0,
    STATE_CALC_SAT_SPEED,
    STATE_LOAD_SATELIZED
} SATELIZATION_STATE_TYPE;

//! structure containing the satelization module variables
static struct
{
    signed short            RpmX16;         // tenths of rpm
    SATELIZATION_STATE_TYPE State;
} Satelization;

//-------------------------------------- PRIVATE (Function Prototypes) ------------------------------------------------

static void ClearVariablesForNewCalculation(void);
static void WindowPowerEventHandler(WINDOW_POWER_EVENT_TYPE event);

//=====================================================================================================================
//-------------------------------------- Public Functions -------------------------------------------------------------
//=====================================================================================================================

//---------------------------------------------------------------------------------------------------------------------
/**
 *  @brief      Initialize the Satelization module.
 *
 */
void Satelization__Initialize(void)
{
    memset(&Satelization, 0, sizeof(Satelization));
    Satelization.RpmX16 = INVALID_SATELIZATION_RPM;
}

void Satelization__Disable(void)
{
    WindowPower__UnregisterEventHandler(&WindowPowerEventHandler);
    Satelization.State = STATE_INACTIVE;
}

void Satelization__Enable(void)
{
    switch ( Satelization.State )
    {
        case STATE_INACTIVE:
            ClearVariablesForNewCalculation();
            WindowPower__RegisterEventHandler(&WindowPowerEventHandler);
            break;
        case STATE_CALC_SAT_SPEED:
        case STATE_LOAD_SATELIZED:
            // do nothing, module already active
            break;
        default:
            break;
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 *  @brief      Returns the satelization speed.
 *
 *  @return     satelization rpm in tenths of rpm
 *
 */
signed short Satelization__GetRpmX16(void)
{
    return ( Satelization.RpmX16 );
}

//=====================================================================================================================
//-------------------------------------- Private Functions ------------------------------------------------------------
//=====================================================================================================================

static void ClearVariablesForNewCalculation(void)
{
    Satelization.RpmX16 = INVALID_SATELIZATION_RPM;
    Satelization.State  = STATE_CALC_SAT_SPEED;
}

static void WindowPowerEventHandler(WINDOW_POWER_EVENT_TYPE event)
{
    signed short reference_rpm_x16 = INPUT_REFERENCE_SPEED;

    switch ( event )
    {
        case WINDOW_POWER_EV_NEW_VALUE_CALCULATED:
            switch ( Satelization.State )
            {
                case STATE_CALC_SAT_SPEED:
                    if (( WindowPower__GetValue() < SATELIZATION_WINDOW_POWER_THRESHOLD ) &&
                        ( reference_rpm_x16 < MAX_SATELIZATION_RPM_X16 ))
                    {
                        Satelization.RpmX16 = reference_rpm_x16;                     // satelization has occurred
                        Satelization.State = STATE_LOAD_SATELIZED;
                    }
                    // else load has not satelized
                    break;
                case STATE_LOAD_SATELIZED:
                    if ( WindowPower__GetValue() > SATELIZATION_WINDOW_POWER_THRESHOLD )
                    {                                                                   // load has become unsatelized
                        ClearVariablesForNewCalculation();
                    }
                    // else load is still satelized
                    break;
                default:
                    // should never entered
                    ClearVariablesForNewCalculation();
                    break;
            }
            break;
        default:
            break;
    }
}
