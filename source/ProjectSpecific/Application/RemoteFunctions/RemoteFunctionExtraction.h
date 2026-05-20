/**
 * @file
 * @brief       Public interface to the RemoteFunctionExtraction module.
 *
 * @details     Refer to the RemoteFunctionExtraction.c source file for more detailed information.
 *
 * @copyright   Copyright 2016 Whirlpool Corporation.  All rights reserved - CONFIDENTIAL.
 */

#ifndef REMOTE_FUNCTION_EXTRACTION_H_
#define REMOTE_FUNCTION_EXTRACTION_H_


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


void RemoteFunctionExtraction__Initialize (void);
ACTIVE_TYPE RemoteFunctionExtraction__Background(void);
REMOTE_FUNCTION_CONTROL_RESPONSE_TYPE  RemoteFunctionExtraction__DoCommand(uint8 command, uint8* params, uint8 size);
void RemoteFunctionExtraction__GetStatus(uint8** status, uint8* size);
uint8 RemoteFunctionExtraction__GetTemperature(void);
float32 RemoteFunctionExtraction__GetLoadMass(void);
uint16 RemoteFunctionExtraction__HSVelocityMinReq(void);
uint16 RemoteFunctionExtraction__HSVelocityMaxLim(void);
uint16 RemoteFunctionExtraction__HSExtdPlateauDur(void);
uint8 RemoteFunctionExtraction__ParMaxAllowRampTime (void);
uint8 RemoteFunctionExtraction__ParDrumTargVelErr (void);

#endif      // REMOTE_FUNCTION_EXTRACTION_H_
