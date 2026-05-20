/**
 *  @file       ProtocolMainExternal.
 *
 *  @brief      Protocol Main External header file
 *
 *  @copyright  Copyright 2016. Whirlpool Corporation. All rights reserved - CONFIDENTIAL
 */
//---------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------
#ifndef PROTOCOLMAINEXTERNAL_H_
    #define PROTOCOLMAINEXTERNAL_H_

//-------------------------------------- Include Files ----------------------------------------------------------------
#include "ProtocolInfo.h"
#include "C_Extensions.h"

//=====================================================================================================================
//-------------------------------------- PUBLIC (Extern Variables, Constants & Defines) -------------------------------
//=====================================================================================================================


// Cmd_Flags Mask
#define FREE0                   BIT0
#define VDC_NORM_REQUEST        BIT1
#define COMMON_BUFFER_REQUEST   BIT2
#define MEANING_REQUEST         BIT3

typedef struct
{
    sint16 Speed;
    sint16 Ramp_Time;
    uint16 Cmd_Flags;
    uint16 Data_Type;
}COMMAND_DATA_TYPE;

typedef enum
{
    LOAD_0,
	LOAD_1,
	LOAD_2,
} LOAD_ENUM_TYPE;

//=====================================================================================================================
//-------------------------------------- PUBLIC (Function Prototypes) -------------------------------------------------
//=====================================================================================================================

void ProtocolMainExternal__Initialize(void);
void ProtocolMainExternal__250usHandler(void);
void ProtocolMainExternal__1msHandler(void);
uint16 ProtocolMainExternal__GetFault(void);
COMMAND_DATA_TYPE* ProtocolMainExternal__GetCommandData(LOAD_ENUM_TYPE load);
#if (APPLIANCE_TYPE == APPLIANCE_TYPE_DISHWASHER)
    //void ProtocolMainExternal__SetIPMType(IPM_TYPE ipm_type)
#endif
    
#endif // PROTOCOLMAINEXTERNAL_H_
