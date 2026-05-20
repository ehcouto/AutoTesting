/**
 *  @file
 *  @brief      Module to provide a standardized method of implementing callbacks.
 *
 *  @details    A callback is a function that is passed as an argument to another software module,
 *              which is expected to execute (call back) the argument at some convenient time.
 *
 *              This module implements synchronous callbacks. This means that when the event occurs,
 *              the callbacks are executed immediately. Because of the immediate nature,
 *              the designer should carefully consider what actions are taken at the time that the
 *              callback is executed. The user should avoid calling these functions from an interrupt service routine.
 *
 *              Register, Unregister and Notify functions call ChecksumOK to test the control registry
 *              before attempting any operations using the control. If ChecksumOK detects a corrupted
 *              registry it will re-intialize the registry and throw a CALLBACK_REGISTRY_CORRUPTED exception.
 *
 *  @par        Module Owner
 *              Daniel Mark Putnam
 *
 *  $Header: Callback.c 1.7 2013/12/12 10:55:27EST Daniel Putnam (PUTNADM) Exp  $
 *
 *  @copyright  Copyright 2013 - $Date: 2013/12/12 10:55:27EST $  Whirlpool Corporation.  All rights reserved - CONFIDENTIAL.
 */

//-------------------------------------- Include Files ----------------------------------------------------------------
#include "C_Extensions.h"
#include "Callback.h"
#include "Callback_prv.h"

//-------------------------------------- PUBLIC (Variables) -----------------------------------------------------------

//-------------------------------------- PRIVATE (Variables, Constants & Defines) -------------------------------------
//! Macro to get the number of handlers referenced by the control structure.
#define NUM_OF_HANDLERS     (control->Registry_Size)

//! Macro to get a pointer to the first handler referenced by the control structure.
#define HANDLERS            ((CALLBACK_HANDLER_TYPE *)(control->Registry))

//! Macro to access last byte of registry used to store the registry checksum.
#define CONTROL_CHECKSUM    (control->Registry[(control->Registry_Size * sizeof(CALLBACK_HANDLER_TYPE))])

//-------------------------------------- PRIVATE (Function Prototypes) ------------------------------------------------
static void UpdateChecksum(CALLBACK_CONTROL_TYPE const * control);
static BOOL_TYPE ChecksumOK(CALLBACK_CONTROL_TYPE const * control);

//=====================================================================================================================
//-------------------------------------- Public Functions -------------------------------------------------------------
//=====================================================================================================================
//---------------------------------------------------------------------------------------------------------------------
/**
 * Function used to initialize control.
 * Set all control registry locations to NULL and initialize registry checksum.
 *
 * @param control = Reference to the control structure used to implement the callback functionality.
 */
void Callback__Initialize(CALLBACK_CONTROL_TYPE const * control)
{
    unsigned char index;

    if (control == NULL)
    {
        CALLBACK_CONTROL_IS_NULL();
    }
    else
    {
        for (index = 0; index < NUM_OF_HANDLERS; index++)
        {
            HANDLERS[index] = NULL;
        }

        UpdateChecksum(control);
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * Function used to register handler with control (Add handler to control registry).
 *
 * If handler is already registered in control, then this function will call the
 * HANDLER_IS_ALREADY_REGISTERED() debug macro. If the callback handler cannot be registered
 * because of lack of space, then this function will call the NO_SPACE_TO_REGISTER_HANDLER() debug
 * macro.
 *
 * @param control = Reference to the control structure used to implement the callback functionality.
 * @param handler = The function handler to be registered in the control structure.
 */
void Callback__Register(CALLBACK_CONTROL_TYPE const * control, CALLBACK_HANDLER_TYPE handler)
{
    unsigned char available_slot;
    unsigned char index;

    if (control == NULL)
    {
        CALLBACK_CONTROL_IS_NULL();
    }
    else if (handler == NULL)
    {
        ATTEMPT_TO_REGISTER_NULL_HANDLER();
    }
    else
    {
        // Check integrity of control structure before use
        if (ChecksumOK(control))
        {
            // Scan the whole list of handlers.
            available_slot = NUM_OF_HANDLERS;
            for (index = 0; index < NUM_OF_HANDLERS; index++)
            {
                // Check if the handler being registered is already registered.
                if (HANDLERS[index] == handler)
                {
                    // Application-specific debug macro
                    HANDLER_IS_ALREADY_REGISTERED(handler);
                    return;
                }

                // Look for an available slot to put the handler that is being registered.
                if (HANDLERS[index] == NULL)
                {
                    available_slot = index;
                }
            }

            if (available_slot < NUM_OF_HANDLERS)
            {
                // Register the handler
                HANDLERS[available_slot] = handler;
                UpdateChecksum(control);
            }
            else
            {
                // Application-specific debug macro
                NO_SPACE_TO_REGISTER_HANDLER(control);
            }
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * Function used to unregister handler from control (Remove handler from control registry).
 *
 * If the callback handler is not already registered, then this function will call the
 * HANDLER_IS_ALREADY_UNREGISTERED() debug macro.
 *
 * @param control = Reference to the control structure used to implement the callback functionality.
 * @param handler = The function handler that should be unregistered from the control structure.
 */
void Callback__Unregister(CALLBACK_CONTROL_TYPE const * control, CALLBACK_HANDLER_TYPE handler)
{
    unsigned char index;

    if (control == NULL)
    {
        CALLBACK_CONTROL_IS_NULL();
    }
    else if (handler == NULL)
    {
        ATTEMPT_TO_UNREGISTER_NULL_HANDLER();
    }
    else
    {
        // Check integrity of control structure before use
        if (ChecksumOK(control))
        {
            for (index = 0; index < NUM_OF_HANDLERS; index++)
            {
                if (HANDLERS[index] == handler)
                {
                    // Unregister the handler
                    HANDLERS[index] = NULL;

                    UpdateChecksum(control);
                    return;
                }
            }
        }
        // Application-specific debug macro
        HANDLER_IS_ALREADY_UNREGISTERED(handler);
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * Function used to notify registered callback handlers.
 * All handlers in control registry are called with event as an argument.
 *
 * @param control = Reference to the control structure used to implement the callback functionality.
 * @param event = The type of action or occurrence detected by the caller.
 */
void Callback__Notify(CALLBACK_CONTROL_TYPE const * control, CALLBACK_EVENT_TYPE event)
{
    unsigned char index;

    if (control == NULL)
    {
        CALLBACK_CONTROL_IS_NULL();
    }
    else
    {
        // Check integrity of control structure before use
        if (ChecksumOK(control))
        {
            // Call each registered callback and pass the event as a parameter.
            for (index = 0; index < NUM_OF_HANDLERS; index++)
            {
                if (HANDLERS[index] != NULL )
                {
                    HANDLERS[index](event);
                }
            }
        }
    }
}

//=====================================================================================================================
//-------------------------------------- Private Functions ------------------------------------------------------------
//=====================================================================================================================
//---------------------------------------------------------------------------------------------------------------------
/**
 * Function to test control registry checksum.
 * Throws CALLBACK_REGISTRY_CORRUPTED exception if detected.
 * Initializes control registry if corruption detected.
 *
 * @param control = Reference to the control structure used to implement the callback functionality.
 */
static BOOL_TYPE ChecksumOK(CALLBACK_CONTROL_TYPE* control)
{
    unsigned char index;
    unsigned char checksum;
    BOOL_TYPE checksum_ok;

    checksum_ok = FALSE;

    checksum = 0;
    // Sum all the bytes (including checksum byte) of the registry, keeping only the least significant byte.
    for (index = 0; index < ((control->Registry_Size * sizeof(CALLBACK_HANDLER_TYPE)) + 1); index++)
    {
        checksum += control->Registry[index];
    }

    if (checksum == 0xFF)
    {
        checksum_ok = TRUE;
    }
    else
    {
        CALLBACK_REGISTRY_CORRUPTED(control);
        Callback__Initialize(control);
    }

    return (checksum_ok);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * Function to update control registry checksum. Uses same checksum algorithm (Ones Complement) as Motorola S-Record.
 *
 * @param control = Reference to the control structure used to implement the callback functionality.
 */
static void UpdateChecksum(CALLBACK_CONTROL_TYPE* control)
{
    unsigned char index;

    CONTROL_CHECKSUM = 0xFF;

    // Subtract all the bytes of the registry callbacks, keeping only the least significant byte.
    for (index = 0; index < control->Registry_Size * sizeof(CALLBACK_HANDLER_TYPE); index++)
    {
        CONTROL_CHECKSUM -= control->Registry[index];
    }
}
