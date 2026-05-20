/**
 * @file        Api221FilterMciErr.c
 * @brief       Api221 Filter mci errors - module to be placed between MCI and API221 to filter
 *              error messages. E.g., when ACU disables motor control via class B and mci class A
 *              throws an error.
 *
 * @copyright   Copyright 2017 Whirlpool Corporation.  All rights reserved - CONFIDENTIAL.
 *
 */

//-------------------------------------- Include Files ----------------------------------------------------------------
#include "Compiler_defs.h"                          //!< Compiler definition
#include "C_Extensions.h"                           //!< C extension
#include "SRAPI020Safety_prm.h"                     //!< Class B API020 (Safety relevant communication - parameters)
#include "SRAPI020Safety.h"                         //!< Class B API020 (Safety relevant communication)
#include "Mci.h"                                    //!< Mci - Motor Control Interface
#include "Api221FilterMciErr.h"                     //!< Include the api221 filter header file




//---------------------------------------------------------------------------------------------------------------------
/*
 *  @brief      Get all MCI Errors list but force2stop in the case it was disabled by SRAPI020
 *  @param      Motor indexer
 *
@startuml
    title Force2Stop Bit Management
    hide footbox
    autonumber

    box "ACU" #DDDDFF
        participant "SRAPI020Safety" as ACU020
        participant "API221MotionCtrl" as ACU221
    end box
    box "MCU" #DDFFDD
        participant "SRAPI020Safety" as MCU020
        participant "API221MotionCtrl" as MCU221
        participant "Api221FilterMciErr" as TBD
        participant "MotorActivity And\nMotion Modules" as MA
        participant "SR Motor\nModules" as SRMCU
        participant MCI
    end box

    == Stop Using API020 ==

    ACU020 --> MCU020 : Disable Motor
    MCU020 <- SRMCU : SRAPI020Safety__GetRxValue()
    MCU020 --> SRMCU : Ok, RxValue (Disabled)
    SRMCU -> MCI : Disable Motor
    MCU221 -> TBD : Get MCU Errors
    TBD -> MCI : Get MCI Errors
    MCI --> TBD : All MCI Errors
    'TBD -> SRMCU : Was Disabled Received?
    'TBD <-- SRMCU : Yes/No
    MCU020 <- TBD : SRAPI020Safety__GetRxValue()
    alt disabled
        MCU020 --> TBD : Ok, RxValue (Disabled)
        MCU221 <-- TBD : No Errors
        TBD -> MA : Clear Errors
    else else
        MCU020 --> TBD : Not Ok or RxValue (Enabled)
        loop for each potential MCI Error
            TBD -> MCI : Mci__GetError()
            TBD <-- MCI : FALSE
        end loop
        MCU221 <-- TBD : Errors
    end alt

    == Normal Run Operation ==

    ACU221 -> MCU221 : Run
    MCU221 -> MA : Run
    MA -> MCI : Run
    loop during normal operation
        alt if API020 sends a value
            ACU020 --> MCU020 : Enable Motor
        end alt
        MCU020 <- SRMCU : SRAPI020Safety__GetRxValue()
        MCU020 --> SRMCU : Ok, RxValue (Enabled)
        SRMCU -> MCI : Enable Motor
        MCU221 -> TBD : Get MCU Errors
        loop for each potential MCI Error
            TBD -> MCI : Mci__GetError()
            TBD <-- MCI : FALSE
        end loop
        MCU221 <-- TBD : Errors
    end loop

    == Failed API020 Communications ==

    alt if API020 sends a value
        ACU020 --> MCU020 : Corrupt Message
        MCU020 -> MCU020 : Corrupt RxValue
    else if API020 timeout expires
        MCU020 -> MCU020 : Corrupt RxValue
    end alt
    MCU020 <- SRMCU : SRAPI020Safety__GetRxValue()
    MCU020 --> SRMCU : Not Ok
    SRMCU -> MCI : Disable Motor

@enduml
 *
 */
uint32 Api221FilterMciErr__GetMCUErrorList(MOTOR_ENUM_TYPE motor)
{
    uint8 srapi20_alive;
    SRAPI020_SPEED_MONITOR_RX_BIT_TYPE speed_monitor_status_api20_rx;
    uint32 mci_errors;
    uint32 temp_u32;

    // Get all errors from mci
    mci_errors = Mci__GetErrorList(motor);

    // alive means CRC matched, timeout not expired and packet sequence is correct.
    srapi20_alive = SRAPI020Safety__GetRxValue(API020_RX_SPEED_MONITOR_STATUS, &speed_monitor_status_api20_rx);

    // Check if safe communication is still alive
    if(srapi20_alive)
    {
        // If motor is enabled from ACU via class B
        if(BIT_TEST(speed_monitor_status_api20_rx,SRAPI020_SPEED_MONITOR__RX_ENABLED))
        {
            // left blank intentionally by doing nothing here, just let
            // the mci_error go thru with all errors read from mci
        }
        else // If motor is disabled from ACU via class B
        {
            if(mci_errors)
            {
                // Return all errors from Mci but force2stop
                temp_u32 = mci_errors & ((1UL<<MCI_ERROR_ANY_ERROR_FOUND) + (1UL<<MCI_ERROR_CLASS_B_FORCE2STOP));
                if(temp_u32 == mci_errors)
                {
                    // Clears all errors in Mci in the case that force2stop here is the only fault
                    Mci__ClearError(motor);
                }

                // Clear bit in case, no need to check if it was set (save computing time)
                BIT_CLR(mci_errors, MCI_ERROR_CLASS_B_FORCE2STOP);

                // Check if bit any error found was set due to force stop error
                if(mci_errors == (1<<MCI_ERROR_ANY_ERROR_FOUND))
                {
                    // clear all error variable because the only error found
                    // was the force 2 stop which is already cleared in mci_error variable
                    mci_errors = 0;
                }
            }
        }
    }

    return(mci_errors);
}




//---------------------------------------------------------------------------------------------------------------------
/*
 * @brief       Get a specific error
 * @details     Force 2 stop error shall not be reported in the case SRAPI020 disabled the motor
 * @return      TRUE: specific error was found; FALSE: no error found
 *
 */
uint8 Api221FilterMciErr__GetError(MOTOR_ENUM_TYPE motor, MCI_ERROR_TYPE error)
{
    uint32 mci_error;
    uint32 all_mci_errors;

    // Get all mci errors but force 2 stop according to srapi020 to disable
    all_mci_errors = Api221FilterMciErr__GetMCUErrorList(motor);

    mci_error = FALSE;
    if(BIT_TEST(all_mci_errors, error))
    {
        mci_error = TRUE;
    }


    return(mci_error);
}




//---------------------------------------------------------------------------------------------------------------------
/*
 * @brief   Request to Mci to clear all its internal errors
 * @return  TRUE: all errors were cleared
 *          FALSE: at least one error could not be cleared
 */
uint8 Api221FilterMciErr__ClearError(MOTOR_ENUM_TYPE motor)
{
    return(Mci__ClearError(motor));
}





