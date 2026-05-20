/**
 *  @file       
 *  @defgroup   CLASS_B
 *  @brief      Basic description of file contents
 *
 *  @section    Applicable_Documents
 *					List here all the applicable documents if needed. <tr>	
 *
 *  $Header: $
 *
 *  @copyright  Copyright 2015-$Date$. Whirlpool Corporation. All rights reserved - CONFIDENTIAL
 */
//---------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------
#ifndef SRPOWERMGR_H
#define SRPOWERMGR_H

#include "SRSystemConfig.h"
#include "C_Extensions.h"

#if (LOW_POWER_FEATURE == ENABLED)

#include "SRPowerMgr_prm.h"
#include "Gpio.h"

//=====================================================================================================================
//-------------------------------------- PUBLIC (Extern Variables, Constants & Defines) -------------------------------
//=====================================================================================================================
typedef enum
{
    SRPOWERMGR_NORMAL_SPEED_MODE = 0,
    SRPOWERMGR_DEEP_SLEEP_MODE,
    SRPOWERMGR_AUTO_WAKEUP_MODE,
    SRPOWERMGR_LOW_SPEED_MODE
}SRPOWERMGR_MODE;

typedef void (*const ACTIVATION_FUNC)(void);
#define SRPOWER_ACTIVATION_FUNC_NULL    (ACTIVATION_FUNC)NULL

typedef struct
{
    uint8 WakeUp_Node;
    GPIO_PORT_TYPE WakeUp_Port;
    uint8 WakeUp_Pin;
    BOOL_TYPE WakeUp_Value;
    ACTIVATION_FUNC WakeUp_Func;
}SRPOWERMGR_WAKEUP_TYPE;


//=====================================================================================================================
//-------------------------------------- PUBLIC (Function Prototypes) -------------------------------------------------
//=====================================================================================================================
void SRPowerMgr__Initialize(void);
BOOL_TYPE SRPowerMgr__GoLowPowerMode(void);
void SRPowerMgr__GoNormalMode(void);
SRPOWERMGR_MODE SRPowerMgr__GetMode(void);
void SRPowerMgr__Handler(void);
BOOL_TYPE SRPowerMgr__IsThisNodeWakeUpSource(void);
void SRPowerMgr__ReceivedWakeUpNodeID(uint8 node);

#endif  //LOW_POWER_FEATURE

#endif // SRPOWERMGR_H


