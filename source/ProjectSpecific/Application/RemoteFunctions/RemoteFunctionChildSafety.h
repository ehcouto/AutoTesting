/**
 * @file
 * @brief       Public interface to the RemoteFunctionChildSafety module.
 *
 * @details     Refer to the RemoteFunctionChildSafety.c source file for more detailed information.
 *
 * @copyright   Copyright 2016 Whirlpool Corporation.  All rights reserved - CONFIDENTIAL.
 */

#ifndef REMOTE_FUNCTION_CHILD_SAFETY_H_
#define REMOTE_FUNCTION_CHILD_SAFETY_H_


//  --- Include Files -------------------------------------------------------------------------------------------------

// -- This Module --

// -- Other Modules --
#include "C_Extensions.h"
#include "RemoteFunctionInterface.h"

//=====================================================================================================================
//  --- Public Properties ---------------------------------------------------------------------------------------------
//=====================================================================================================================


// -- Public Constant Declarations --


// -- Public Enumerated Constant Declarations --

// -- Public Type Declarations --


//=====================================================================================================================
//  --- Public Methods ------------------------------------------------------------------------------------------------
//=====================================================================================================================


void RemoteFunctionChildSafety__Initialize (void);
ACTIVE_TYPE RemoteFunctionChildSafety__Background(void);
REMOTE_FUNCTION_CONTROL_RESPONSE_TYPE  RemoteFunctionChildSafety__DoCommand(uint8 command, uint8* params, uint8 size);
void RemoteFunctionChildSafety__GetStatus(uint8** status, uint8* size);
#endif      // REMOTE_FUNCTION_CHILD_SAFETY_H_
