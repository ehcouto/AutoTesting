/**
 *  @file       ProtocolInfo.h
 *
 *  @brief      Header file for the ProtocolInfo module
 *
 *  $Header: $
 *
 *  @copyright  Copyright 2016-$Date$. Whirlpool Corporation. All rights reserved - CONFIDENTIAL
 */
//---------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------
#ifndef PROTOCOLINFO_PROTOCOLINFO_H_
#define PROTOCOLINFO_PROTOCOLINFO_H_

//-------------------------------------- Include Files ----------------------------------------------------------------
#include "ProtocolInfo_prm.h"
#include "C_Extensions.h"


//=====================================================================================================================
//-------------------------------------- PUBLIC (Extern Variables, Constants & Defines) -------------------------------
//=====================================================================================================================
// Fdb_Flags Mask
#define TRIPHASE_PARAMS_READ   	BIT0  //!< bTriphaseParamsRead
#define FLAGFREE1             	BIT1
#define FLAGFREE2               BIT2
#define FLAGFREE3               BIT3
#define FLAGFREE4         		BIT4
#define FLAGFREE5           	BIT5
#define FLAGFREE6           	BIT6
#define FLAGFREE7               BIT7
#define FLAGFREE8               BIT8
#define FLAGFREE9              	BIT9
#define FLAGFREE10            	BIT10
#define FLAGFREE11       		BIT11
#define FLAGFREE12             	BIT12
#define FLAGFREE13         		BIT13
#define FLAGFREE14              BIT14
#define FLAGFREE15              BIT15


#pragma pack(2)
typedef struct
{
    uint16  Ident_Table_0;
    uint16  Ident_Table_1;
    uint16  Ident_Table_2;
    uint16  Ident_Table_3;
    uint16  Ident_Table_4;
    uint16  LUT_Pointers_Address;
    uint16  Ident_Table_5;
    uint16  Ident_Table_6;
    uint16  Ident_Table_7;
    uint16  Ident_Table_8;
    uint16  Ident_Table_9;
    uint16  Ident_Table_10;
    uint16  Ident_Table_11;
    uint16  Ident_Table_12;
    uint16  Ident_Table_13;
}IDENTTABLE_TYPE;


//=====================================================================================================================
//-------------------------------------- PUBLIC (Function Prototypes) -------------------------------------------------
//=====================================================================================================================
void ProtocolInfo__Initialize(void);
void ProtocolInfo__1msHandler(void);
PROTOCOL_INFO_TYPE* ProtocolInfo__GetInfo(void);
IDENTTABLE_TYPE* ProtocolInfo__GetIdent(void);

#endif // PROTOCOLINFO_PROTOCOLINFO_H_
