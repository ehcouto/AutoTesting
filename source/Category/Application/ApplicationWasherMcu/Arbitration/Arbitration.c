/**
 * @file
 * @brief       Manages conflicting requests from multiple controllers to set a single output.
 *
 * @details
 *
 * The Arbitration module has the following interface:
 *
 * @image html arbitration_cl.png "Class diagram for the Arbitration Module."
 *
 * The Arbitration module provides a service to Subject modules that control an output. The
 * Subject module establishes a priority scheme for the requests made by external controlling
 * modules. The arbitration module uses the priority scheme to resolve all the requests to a
 * single prioritized output. The following sequence diagram shows an example:
 *
 * @image html arbitration_sq_conflicting_requests.png "Arbitration of conflicting requests for outputs"
 *
 * Multiple Subject modules can use the Arbitration module simultaneously since the RAM is located
 * in the scope of the Subject modules.
 *
 *  $Header: Arbitration.c 1.4 2015/08/19 10:32:24EDT Nelson Ferragut II (FERRANJ) Exp  $
 *
 *  @copyright  Copyright 2013 - $Date: 2015/08/19 10:32:24EDT $  Whirlpool Corporation.  All rights reserved - CONFIDENTIAL.
 */

//  --- Include Files -------------------------------------------------------------------------------------------------

// -- This Module --
#include "C_Extensions.h"
#include "Arbitration.h"
#include "Arbitration_prv.h"

// -- Other Modules --
#include <string.h>
#include "Utilities.h"


//  --- Public Variables ----------------------------------------------------------------------------------------------


//  --- Private Properties --------------------------------------------------------------------------------------------


//  --- Private Function Prototypes -----------------------------------------------------------------------------------

static void *GetActionPointer(ARB_CONTROL_TYPE *control, unsigned char requestor);
static void GetPrioritizedAction(ARB_CONTROL_TYPE *control, void *action, unsigned char *requestor);

//=====================================================================================================================
//  --- Public Functions ----------------------------------------------------------------------------------------------
//=====================================================================================================================


//---------------------------------------------------------------------------------------------------------------------
/**
 * Initialize the arbitration control structure.
 *
 * After initialization, the control structure has all requestors deferring to the default output.
 *
 * @param control = Pointer to the arbitration control structure to initialize.
 */
void Arbitration__Initialize(ARB_CONTROL_TYPE *control)
{
    if (control != NULL)
    {
        // Set all requestors for controlling the output to inactive.
        memset(control->Active_Bits, FALSE, (control->Number_Of_Requestors + 7) / 8);
    }
    else
    {
        DEBUG_UNINITIALIZED_POINTER(Arbitration__Initialize);
    }
}


//---------------------------------------------------------------------------------------------------------------------
/**
 * Get the current action that should be taken based on the load arbitration.
 *
 * @param control = Pointer to the arbitration control structure.
 * @param action = Pointer to a structure to fill with the current arbitrated action for the load.
 */
void Arbitration__GetPrioritizedAction(ARB_CONTROL_TYPE *control, void *action)
{
    unsigned char requestor;

    if (( control == NULL ) ||
        ( action == NULL ))
    {
        DEBUG_UNINITIALIZED_POINTER(Arbitration__GetPrioritizedAction);
    }
    else
    {
        GetPrioritizedAction(control, action, &requestor);
    }
}


//---------------------------------------------------------------------------------------------------------------------
/**
 * Get the current action that should be taken based on the load arbitration.
 *
 * @param control = Pointer to the arbitration control structure.
 * @param action = Pointer to a structure to fill with the current arbitrated action for the load.
 * @param requestor = Pointer to the variable to write the current active requestor to.
 */
void Arbitration__GetPrioritizedActionAndRequestor(ARB_CONTROL_TYPE *control, void *action, unsigned char *requestor)
{
    if (( control == NULL ) ||
        ( action == NULL ) ||
        ( requestor == NULL ))
    {
        DEBUG_UNINITIALIZED_POINTER(Arbitration__GetPrioritizedActionAndRequestor);
    }
    else
    {
        GetPrioritizedAction(control, action, requestor);
    }
}


//---------------------------------------------------------------------------------------------------------------------
/**
 * Update the arbitration based on a new release of control of the output action.
 *
 * @param control = Pointer to the arbitration control structure.
 * @param requestor = The requestor releasing control of the output.
 */
void Arbitration__ReleaseControl(ARB_CONTROL_TYPE *control, unsigned char requestor)
{
    if (control != NULL)
    {
        if (requestor < control->Number_Of_Requestors)
        {
            // Release control of any action by the 'requestor'.
            Utilities__SetBitInArray(control->Active_Bits, requestor, FALSE);
        }
        else
        {
            DEBUG_INVALID_REQUESTOR(requestor);
        }
    }
    else
    {
        DEBUG_UNINITIALIZED_POINTER(Arbitration__ReleaseControl);
    }
}


//---------------------------------------------------------------------------------------------------------------------
/**
 * Update the arbitration based on a new request for control of the output action.
 *
 * @param control = Pointer to the arbitration control structure.
 * @param action = Pointer to a structure that contains the requested action.
 * @param requestor = The requestor asking for control of the output.
 */
void Arbitration__RequestControl(ARB_CONTROL_TYPE *control, void *action, unsigned char requestor)
{
    if ((control != NULL) && (action != NULL))
    {
        if (requestor < control->Number_Of_Requestors)
        {
            // Record the request of the 'action' for the 'requestor' given.
            Utilities__SetBitInArray(control->Active_Bits, requestor, TRUE);
            memcpy(GetActionPointer(control, requestor), action, control->Size_Of_Action_Type);
        }
        else
        {
            DEBUG_INVALID_REQUESTOR(requestor);
        }
    }
    else
    {
        DEBUG_UNINITIALIZED_POINTER(Arbitration__RequestControl);
    }
}


//=====================================================================================================================
//  --- Private Functions ---------------------------------------------------------------------------------------------
//=====================================================================================================================

//---------------------------------------------------------------------------------------------------------------------
/**
 * Get the current action that should be taken based on the load arbitration.
 *
 * @param control = Pointer to the arbitration control structure.
 * @param action = Pointer to a structure to fill with the current arbitrated action for the load.
 * @param requestor = Pointer to the variable to write the current active requestor to.
 */
static void GetPrioritizedAction(ARB_CONTROL_TYPE *control, void *action, unsigned char *requestor)
{
    unsigned char req;

    // Assume that there is no active request and the default action will be used.
    for (req = 0; req < control->Number_Of_Requestors; req++)
    {
        // Look for the first (highest priority) active request
        if (Utilities__GetBitInArray(control->Active_Bits, req) == TRUE)
        {
            // Determine the address of the prioritized action.
            memcpy(action, GetActionPointer(control, req), control->Size_Of_Action_Type);
            *requestor = req;
            return;
        }
    }
    // All requestors released control -- use the default action.
    memcpy(action, control->Default_Action, control->Size_Of_Action_Type);
    *requestor = control->Number_Of_Requestors;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * Get the address of the last action requested by the specified requestor.
 *
 * Note, this function is for internal use only.
 * It does not check whether or not the requestor has released control.
 *
 * @param control = Pointer to the arbitration control structure.
 * @param requestor = The requestor that asked for an action.
 */
static void *GetActionPointer(ARB_CONTROL_TYPE *control, unsigned char requestor)
{
    return ((unsigned char*)(control->Actions) + (requestor * control->Size_Of_Action_Type));
}


