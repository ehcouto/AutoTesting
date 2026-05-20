/**
 *  @file       CommandManager.h
 *
 *  @brief      Header file for the Command Manager module
 *
 *  $Header: $
 *
 *  @copyright  Copyright 2016-$Date$. Whirlpool Corporation. All rights reserved - CONFIDENTIAL
 */
//---------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------
#ifndef COMMANDMANAGER_H_
    #define COMMANDMANAGER_H_

//-------------------------------------- Include Files ----------------------------------------------------------------
#include "C_Extensions.h"
#include "CommandManager_prm.h"
//=====================================================================================================================
//-------------------------------------- PUBLIC (Extern Variables, Constants & Defines) -------------------------------
//=====================================================================================================================

//=====================================================================================================================
//-------------------------------------- PUBLIC (Function Prototypes) -------------------------------------------------
//=====================================================================================================================
void CommandManager__Initialize(void);
void CommandManager__UnLockAcceleration(void);
void CommandManager__DecodeMasterCommand(sint16 speed_rpm, sint16 ramp_rpm_per_s, uint16 flags);
void CommandManager__5msHandler(void);
uint16 CommandManager__GetFault(void);

#endif /* COMMANDMANAGER_H_ */
