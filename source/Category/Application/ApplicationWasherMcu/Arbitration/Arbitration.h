/**
 *  @file
 *  @brief      Public interface to the Arbitration module.
 *
 *  @details    Refer to the documentation for Arbitration.c for more details.
 *
 *  $Header: Arbitration.h 1.7 2015/08/19 10:33:36EDT Nelson Ferragut II (FERRANJ) Exp  $
 *
 *  @copyright  Copyright 2013 - $Date: 2015/08/19 10:33:36EDT $  Whirlpool Corporation.  All rights reserved - CONFIDENTIAL.
 */
#ifndef ARBITRATION_H_
#define ARBITRATION_H_

//  --- Include Files -------------------------------------------------------------------------------------------------

// -- This Module --

// -- Other Modules --


//=====================================================================================================================
//  --- Public Properties ---------------------------------------------------------------------------------------------
//=====================================================================================================================


// -- Public Type Declarations --

//! Arbitration control structure. Pointer to this structure is passed to all public functions.
typedef const struct ARB_CONTROL_STRUCT
{
    //! The action to use when all requestors have released control of the output.
    const void *Default_Action;

    //! The number of requestors that can request a change in the output.
    unsigned char Number_Of_Requestors;

    //! The number of bytes consumed by each action.
    unsigned char Size_Of_Action_Type;

    //! An array of actions requested by each requestor.
    void *Actions;

    //! A bit for each requestor to keep track of whether or not they have released control.
    //! 0 = Requestor has released control of the output.
    //! 1 = Requestor has requested control of the output.
    uint8 *Active_Bits;
} ARB_CONTROL_TYPE;


//=====================================================================================================================
//  --- Public Methods ------------------------------------------------------------------------------------------------
//=====================================================================================================================


// -- Public Macro Declarations --


//---------------------------------------------------------------------------------------------------------------------
/**
 * Macro used to define a control structure for a subject module.
 *
 * @image html arbitration_sq_initialize.png "Typical initialization sequence when using the Arbitration module."
 *
 * @param arbitration_control_name = The name of the ARB_CONTROL_TYPE control structure variable
 *          to create. A pointer to the control structure must be passed to every call to
 *          Arbitration module functions.
 * @param action_type = The data type used by requestors when setting an action for the Arbitrated Output.
 * @param default_action = The default action for the Arbitrated Output. The Arbitration module
 *          sets the Arbitrated Output to this value if none of the Requestors request control of
 *          the output.
 * @param requestor_count = The number of unique requestors that can request an action. This is
 *          the same as the number of Requestors that are declared in the list of Requestors.
 */
#define DEFINE_ARBITRATION_CONTROL_STRUCTURE(arbitration_control_name, action_type, default_action, requestor_count) \
    static const action_type arbitration_control_name##_Default = default_action; \
    static action_type arbitration_control_name##_Actions[requestor_count]; \
    static unsigned char arbitration_control_name##_Active_Bits[((requestor_count) + 7) / 8]; \
    static ARB_CONTROL_TYPE arbitration_control_name = \
    { \
        &arbitration_control_name##_Default, \
        requestor_count, \
        sizeof(action_type), \
        arbitration_control_name##_Actions, \
        arbitration_control_name##_Active_Bits \
    }


// -- Public Function Prototypes --

void Arbitration__Initialize(ARB_CONTROL_TYPE *control);
void Arbitration__GetPrioritizedAction(ARB_CONTROL_TYPE *control, void *action);
void Arbitration__GetPrioritizedActionAndRequestor(ARB_CONTROL_TYPE *control, void *action, unsigned char *requestor);
void Arbitration__ReleaseControl(ARB_CONTROL_TYPE *control, unsigned char requestor);
void Arbitration__RequestControl(ARB_CONTROL_TYPE *control, void *action, unsigned char requestor);


#endif  // ARBITRATION_H_

