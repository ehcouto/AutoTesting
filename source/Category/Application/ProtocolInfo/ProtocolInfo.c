/**
 *  @file       ProtocolInfo.c
 *
 *  @brief      Indesit Legacy protocol application specific serial buffer information
 *
 *  $Header: $
 *
 *  @copyright  Copyright 2016-$Date$. Whirlpool Corporation. All rights reserved - CONFIDENTIAL
 */
//---------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------


//-------------------------------------- Include Files ----------------------------------------------------------------
#include "ProtocolInfo.h"
#include "C_Extensions.h"
#include "Mci.h"
//#include "SensorsAlgorithms.h"
#include "MciSensorsWm.h"
#include "FaultManager.h"
#include "Params.h"
// LUT includes
#include "CommandManager.h"
#include "Mcl.h"
#include "MclDqCtrl.h"
#include "MclSpeedCtrl.h"
#include "MclDqRef.h"
#include "MclObserver.h"
#include "BoardDebugging.h"
#include "SRSignature.h"

#if (APPLIANCE_TYPE == APPLIANCE_TYPE_WASHER)
#include "SRMotorSafetyInfoParamsMgr.h"
static SR_MOTOR_SAFETY_INFO_LIST_TYPE Protocol_SafetyInfo;
#endif
//-------------------------------------- PUBLIC (Variables) -----------------------------------------------------------

//-------------------------------------- PRIVATE (Variables, Constants & Defines) -------------------------------------
// symbol MICRO_FLASH_CRC created by the IELF tool, this holds the calculated CRC value on the flash memory.
extern uint16 MICRO_FLASH_CRC;
static uint8 ClassB_Signature[32];    // Required to copy Class B Signature in RAM
static uint16 Micro_Flash_Crc_Holder; // Holder required to copy Flash CRC in RAM
static PROTOCOL_INFO_TYPE Protocol_Info;


static uint16 LUT_Data_Sizes[] = {
    sizeof(SF_Application_Params),
    sizeof(Mcl_Quantities),
#if (MCI_FVT_TESTS == ENABLED)
    sizeof(FvtResult),
#else
    sizeof(uint16),
#endif
#if	(PLATFORM_USED == PLATFORM_2_5_BASED)
    sizeof(uint16),
#else
	sizeof(uint16),
#endif
    sizeof(uint16),
    sizeof(uint16),
    sizeof(uint16),
    sizeof(uint16), // Dummy placeholder - sRampParams
    sizeof(uint16), // Dummy placeholder - sMiscs
    sizeof(uint16),
    sizeof(Mci_ClassB_Prm),
    sizeof(ClassB_Signature),
    sizeof(Micro_Flash_Crc_Holder),
};

// Init in run time
static uint16 LUT_Pointers[15];

#define LktSize         sizeof(LUT_Pointers)/2

#define RPM_TO_IQ_PLT_2_5(value)        (signed long)IQMATH__REAL2IQ((value), 19800, IQ15)

static IDENTTABLE_TYPE Ident_Table;

//-------------------------------------- PRIVATE (Function Prototypes) ------------------------------------------------


//=====================================================================================================================
//-------------------------------------- Public Functions -------------------------------------------------------------
//=====================================================================================================================

//---------------------------------------------------------------------------------------------------------------------
/**
 *  @brief      It Initializes the module ProtocolInfo and its variables
 *
 */
void ProtocolInfo__Initialize(void)
{
    uint8* temp;
    uint8 i;


#if (APPLIANCE_TYPE == APPLIANCE_TYPE_WASHER)

    SRMotorSafetyInfoParmsMgr__Initialize();

	// Protocol Info Buffer initialization
    Protocol_Info.Active_Power = 0;
    Protocol_Info.Balance = 0;
    Protocol_Info.Balance_Window = 0;
    Protocol_Info.Crc_Safety = 0;
    Protocol_Info.Fault = 0;
    Protocol_Info.Fdb_Flags = 0;
    Protocol_Info.Me_Mean = 0;
    Protocol_Info.Me_Mean_Window = 0;
    Protocol_Info.OH_Motor_Index = 0;
    Protocol_Info.OH_Spinning_Index = 0;
    Protocol_Info.OH_Washing_Index = 0;
    Protocol_Info.Omega = 0;
    Protocol_Info.Safety_Cnt = 0;
    Protocol_Info.Safety_Flags = 0;
    Protocol_Info.Total_Energy_Hi = 0;
    Protocol_Info.Total_Energy_Lo = 0;
    Protocol_Info.Version = 0;
#elif (APPLIANCE_TYPE == APPLIANCE_TYPE_DISHWASHER)
	// Protocol Info Buffer initialization
    Protocol_Info.Fault = 0;
    Protocol_Info.Omega = 0;
    Protocol_Info.Virtual_Sensor_State_Machine = 0;
    Protocol_Info.OH_Motor_Index= 0;
    Protocol_Info.Omega = 0;
    Protocol_Info.Mean = 0;
    Protocol_Info.Fdb_Flags = 0;
    Protocol_Info.Active_Power = 0;
    Protocol_Info.Total_Energy_Hi = 0;
    Protocol_Info.Total_Energy_Lo = 0;
    Protocol_Info.Vdc_Max_Copy = 0;
#endif
    // Copy from Flash to RAM both CRC and Signature
    Micro_Flash_Crc_Holder = MICRO_FLASH_CRC;
    temp = SRSignature__GetClassBSignature();
    for (i = 0; i < 32; i++)
    {
        ClassB_Signature[i] = *temp++;
    }

    // LUT Pointers initialization - Addresses in Word format
    LUT_Pointers[0] = ((uint16)(&SF_Application_Params) >> 1);
    LUT_Pointers[1] = ((uint16)(&Mcl_Quantities) >> 1);
#if (MCI_FVT_TESTS == ENABLED)
    LUT_Pointers[2] = ((uint16)(&FvtResult) >> 1);
#else
    LUT_Pointers[2] = ((uint16)(0));
#endif
#if (PLATFORM_USED == PLATFORM_2_5_BASED)
    LUT_Pointers[3] = ((uint16)0);
#else
    LUT_Pointers[3] = ((uint16)(0));
#endif

    LUT_Pointers[4] = ((uint16)(0));
    LUT_Pointers[5] = ((uint16)(0));
    LUT_Pointers[6] = ((uint16)(0));
    LUT_Pointers[7] = ((uint16)0);
    LUT_Pointers[8] = ((uint16)0); // Dummy placeholder - sRampParams
    LUT_Pointers[9] = ((uint16)0); // Dummy placeholder - sMiscs
    LUT_Pointers[10] = ((uint16)0);
    LUT_Pointers[11] = ((uint16)(&Mci_ClassB_Prm) >> 1);
    LUT_Pointers[12] = ((uint16)(&ClassB_Signature[0]) >> 1);
    LUT_Pointers[13] = ((uint16)(&Micro_Flash_Crc_Holder) >> 1);
    LUT_Pointers[14] = ((uint16)(&LUT_Data_Sizes) >> 1);
    // Ident Table initialization
    Ident_Table.Ident_Table_0 = (uint16)(((uP_MODEL<<4)+uPManufacturer)<<8)|((BoardType<<4)+APPLIANCE_TYPE);
    Ident_Table.Ident_Table_1 = (uint16)(((((uint16)ExtendedAddr<<7)+(WriteBlockSize<<6)+(IdentFormat<<5)+(Alignment<<4)+(ErrorChecking<<3)+ProjectCode)<<8)|(FW_VERSION_A));
    Ident_Table.Ident_Table_2 = (uint16)((FW_VERSION_B<<8)|(FW_VERSION_C));
    Ident_Table.Ident_Table_3 = (uint16)((RamAddrHi<<8)|(RamAddrLo));
    Ident_Table.Ident_Table_4 = (uint16)((EEPAddrHi<<8)|(EEPAddrLo));
    Ident_Table.LUT_Pointers_Address = ((uint16)(&LUT_Pointers) >> 1); // address in word format
    Ident_Table.Ident_Table_5 = (uint16)((EEPSize<<8)|(LktSize));
    Ident_Table.Ident_Table_6 = 0;
    Ident_Table.Ident_Table_7 = 0;
    Ident_Table.Ident_Table_8 = 0;
    Ident_Table.Ident_Table_9 = 0;
    Ident_Table.Ident_Table_10 = 0;
    Ident_Table.Ident_Table_11 = 0;
    Ident_Table.Ident_Table_12 = 0;
    Ident_Table.Ident_Table_13 = 0;

    FaultManager__Reset();
}


//---------------------------------------------------------------------------------------------------------------------
/**
 *  @brief      ProtocolInfo 2 ms Handler
 *
 *  @details    Populate the InfoBuffer with the latest information
 *
 *  @remarks    The update of the fault information is performed here
 *
 */
void ProtocolInfo__1msHandler(void)
{
    sint16 speed;

    FaultManager__UpdateFault();

    Protocol_Info.Fault = FaultManager__GetFaultToPublish();

    // Speed Measure Sign Imposition
    if((FALSE == Mci__GetDigital(MOTOR0, MCI_DI_MOTOR_RUNNING)) && (FaultManager__GetFaultToPublish() == NO_FAULT))
    {// No Currents -> Speed is Zero
        Protocol_Info.Omega = 0;
    }
    else
    {// Current -> Speed is NOT Zero
        speed = RPM_TO_IQ_PLT_2_5(Mci__GetAnalog(MOTOR0, MCI_AI_ACTUAL_SPEED_S16)>>16);

        if(speed != 0)
        {
            Protocol_Info.Omega = speed;
        }
        else
        {
            Protocol_Info.Omega = 1;
        }
    }

#ifndef INTERNAL_PARAMS
	if(Params__IsReady() == TRUE)
	{
		BIT_CLR(Protocol_Info.Fdb_Flags, TRIPHASE_PARAMS_READ);
	}
	else
#endif
	{
		BIT_SET(Protocol_Info.Fdb_Flags, TRIPHASE_PARAMS_READ);
	}


#if (APPLIANCE_TYPE == APPLIANCE_TYPE_WASHER)
    Protocol_Info.Me_Mean = MciSensorsWm__GetIndesitAnalog(MCISENSORSWM_AI_TORQUE_MEAN);  //lint !e63
    //Protocol_Info.Me_Mean_Window = MciSensorsWm__GetAnalog(SENSORS_AI_TORQUE_MEAN_WINDOW);

    Protocol_Info.Balance = MciSensorsWm__GetIndesitAnalog(MCISENSORSWM_AI_BALANCE);
    //Protocol_Info.Balance_Window = SensorsAlgorithms__GetAnalog(SENSORS_AI_BALANCE_WINDOW);




    //--------------------------------Global Data--------------------------------------
    //Power Meter
//	Protocol_Info.Active_Power = MciSensorsWm__GetAnalog(SENSORS_AI_MOTOR_LINE_POWER);
//	Protocol_Info.Total_Energy_Hi = MciSensorsWm__GetAnalog(SENSORS_AI_MOTOR_LINE_POWER_HI);
//	Protocol_Info.Total_Energy_Lo = MciSensorsWm__GetAnalog(SENSORS_AI_MOTOR_LINE_POWER_LO);

    //Overheating Indexes
//	Protocol_Info.OH_Spinning_Index = 0;
    Protocol_Info.OH_Washing_Index = MciSensorsWm__GetIndesitAnalog(MCISENSORSWM_AI_OVERHEATING_WASHING);
    Protocol_Info.OH_Motor_Index = 0;

    // Fw Version Init according to ident content
    Protocol_Info.Version = (FW_VERSION_A << 12) + (FW_VERSION_B << 8) + FW_VERSION_PROTOCOL;
    //--------------------------------Global Data--------------------------------------
#elif (APPLIANCE_TYPE == APPLIANCE_TYPE_DISHWASHER)

    //--------------------------------Global Data--------------------------------------
      //Power Meter
  	Protocol_Info.Active_Power = SensorsAlgorithms__GetAnalog(SENSORS_AI_MOTOR_LINE_POWER);
  	Protocol_Info.Total_Energy_Hi = SensorsAlgorithms__GetAnalog(SENSORS_AI_MOTOR_LINE_POWER_HI);
  	Protocol_Info.Total_Energy_Lo = SensorsAlgorithms__GetAnalog(SENSORS_AI_MOTOR_LINE_POWER_LO);

    //Overheating Indexes
	Protocol_Info.OH_Motor_Index = 0;

	// Fw Version Init according to ident content
	Protocol_Info.Vdc_Max_Copy = Mci__GetAnalog(MOTOR0, MCI_AI_BUS_VOLTAGE); //TODO: check if aligned with Dea701 requirement
      //--------------------------------Global Data--------------------------------------


#endif
}

//---------------------------------------------------------------------------------------------------------------------
/**
 *  @brief      Get a pointer to the InfoBuffer structure
 *
 *  @return     Reference to the InfoBuffer structure. See the @ref ProtocolInfo.h
 *              file for reference.
 *
 */
PROTOCOL_INFO_TYPE* ProtocolInfo__GetInfo(void)
{

#if (APPLIANCE_TYPE == APPLIANCE_TYPE_WASHER)
	//Safety Information
	Protocol_SafetyInfo = SRMotorSafetyInfoParmsMgr__GetInfo();
	Protocol_Info.Safety_Cnt = Protocol_SafetyInfo.Counter;
	Protocol_Info.Safety_Flags = Protocol_SafetyInfo.Info_Flags;
	Protocol_Info.Crc_Safety = Protocol_SafetyInfo.Crc;
#endif
	return &Protocol_Info;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 *  @brief      Get a pointer to the Ident_Table structure
 *
 *  @return     Reference to the Ident_Table structure.
 *
 */
IDENTTABLE_TYPE* ProtocolInfo__GetIdent(void)
{
    return &Ident_Table;
}
//=====================================================================================================================
//-------------------------------------- Private Functions ------------------------------------------------------------
//=====================================================================================================================
