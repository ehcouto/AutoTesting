/**
 *  @file       MciParametersLoader_prv.h
 *
 *  @brief      Load Mci parameters
 *
 *  $Header: $
 *
 *  @copyright  Copyright 2016-$Date$. Whirlpool Corporation. All rights reserved - CONFIDENTIAL
 */
//---------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------
#ifndef MCIPARAMETERSLOADER_PRV_H
    #define MCIPARAMETERSLOADER_PRV_H


//-------------------------------------- Include Files ----------------------------------------------------------------
#include "C_Extensions.h"
#include "Mcl.h"
#include "SRMotorSafetyMgr_prv.h"
#include "mci_prm.h"

// Motor Selection for INTERNAL MCL parameters
// list of available motors
#define HEFEI_H27        0
#define NIDEC_H38        1
#define HEFEI_H27_HIGH   2
#define HEFEI_H22        3
#define HEFEI_H17        4
#define HEFEI_DD_TM2     5
#define ORCA_H24         6
#define ORCA_H34         7


// selected motor
#define MCL_MOTOR   ORCA_H24

//Some Mci Class-A displacements are not from setting file
#if (MCL_MOTOR == HEFEI_H27)
    #if (WINDY_STRIP_BOARD == 1)
        #define INTERNAL_CLASS_A_PARAMS_INCLUDE_FILE "Mcl_prm_Hefei_H27.h"
        #define INTERNAL_CLASS_B_PARAMS_INCLUDE_FILE "MotorSr_prm.h"
    #else
        #define INTERNAL_CLASS_A_PARAMS_INCLUDE_FILE "Mcl_prm_Hefei_H27_Windy_Int.h"
        #define INTERNAL_CLASS_B_PARAMS_INCLUDE_FILE "MotorSr_prm.h"
    #endif
#elif (MCL_MOTOR == NIDEC_H38)
    #define INTERNAL_CLASS_A_PARAMS_INCLUDE_FILE "Mcl_prm_Nidec_H38.h"
    #define INTERNAL_CLASS_B_PARAMS_INCLUDE_FILE "MotorSr_prm.h"
#elif (MCL_MOTOR == HEFEI_H27_HIGH)
    #define INTERNAL_CLASS_A_PARAMS_INCLUDE_FILE "Mcl_prm_Hefei_H27_High.h"
    #define INTERNAL_CLASS_B_PARAMS_INCLUDE_FILE "MotorSr_prm.h"
#elif (MCL_MOTOR == HEFEI_H22)
    #define INTERNAL_CLASS_A_PARAMS_INCLUDE_FILE "Mcl_prm_Hefei_H22.h"
    #define INTERNAL_CLASS_B_PARAMS_INCLUDE_FILE "MotorSr_prm.h"
#elif (MCL_MOTOR == HEFEI_H17)
    #define INTERNAL_CLASS_A_PARAMS_INCLUDE_FILE "Mcl_prm_Hefei_H17.h"
    #define INTERNAL_CLASS_B_PARAMS_INCLUDE_FILE "MotorSr_prm.h"
#elif (MCL_MOTOR == HEFEI_DD_TM2)
    #define INTERNAL_CLASS_A_PARAMS_INCLUDE_FILE "Mcl_prm_Hefei_DD_TM2.h"
    #define INTERNAL_CLASS_B_PARAMS_INCLUDE_FILE "MotorSr_prm_Hefei_DD_TM2.h"
#elif (MCL_MOTOR == ORCA_H24)
    #define INTERNAL_CLASS_A_PARAMS_INCLUDE_FILE "Mcl_prm_Orca_H24.h"
    #define INTERNAL_CLASS_B_PARAMS_INCLUDE_FILE "Mcl_prm_Orca_H24_SR.h"
#elif (MCL_MOTOR == ORCA_H34)
    #define INTERNAL_CLASS_A_PARAMS_INCLUDE_FILE "Mcl_prm_Orca_H34.h"
    #define INTERNAL_CLASS_B_PARAMS_INCLUDE_FILE "Mcl_prm_Orca_H34_SR.h"
#endif

///////////////////////////// MCI - CLASS A Section  ////////////////////////////////////////
// DEFINITION OF DISPLACEMENT AVAILABLE ON THE MCI CLASS A POINTER //
#define DISPL_DTC_CONTROL               1
#define DISPL_MOTOR_LUT                 3
#define DISPL_SPEED_CTRL                4
#define DISPL_MTPA_LUT                  9
#define DISPL_MTPV_LUT                  10
#define DISPL_FLUX_LUT                  11

// For each used displacement declare its size here
#define DISPL_DTC_CONTROL_SIZE               sizeof(MCL_PARAMS_DISPL1_TYPE)
#define DISPL_MOTOR_LUT_SIZE                 sizeof(MCL_PARAMS_DISPL3_TYPE)
#define DISPL_SPEED_CTRL_SIZE                sizeof(MCL_PARAMS_DISPL4_TYPE)
#define DISPL_MTPA_LUT_SIZE                  sizeof(MCL_PARAMS_DISPL9_TYPE)
#define DISPL_MTPV_LUT_SIZE                  sizeof(MCL_PARAMS_DISPL10_TYPE)
#define DISPL_FLUX_LUT_SIZE                  sizeof(MCL_PARAMS_DISPL11_TYPE)

// Define here the the memory location of the displacements.
// For each memory location define comma separated displacements in the right order.
// REMARK: Only define the used memory locations. Leave the unused locations undefined
#if (!defined MCI_INTERNAL_PARAMS)
#define SF_RAM_DISPLS           {DISPL_DTC_CONTROL, DISPL_MOTOR_LUT, DISPL_SPEED_CTRL, DISPL_MTPA_LUT, DISPL_MTPV_LUT, DISPL_FLUX_LUT}
//#define INTERNAL_DISPLS         {DISPL_FLUX_LUT}
#else
#define INTERNAL_DISPLS         {DISPL_DTC_CONTROL, DISPL_MOTOR_LUT, DISPL_SPEED_CTRL, DISPL_MTPA_LUT, DISPL_MTPV_LUT,DISPL_FLUX_LUT}
#endif
//#define SF_FLASH_DISPLS       {}
//#define INTERNAL_DISPLS

// For internal displacements only, define the sizes, in the same order internal
// displacements definition. Values must be comma separated.
#define INTERNAL_DISPL_SIZES  {DISPL_DTC_CONTROL_SIZE, DISPL_MOTOR_LUT_SIZE, DISPL_SPEED_CTRL_SIZE, DISPL_MTPA_LUT_SIZE, DISPL_MTPV_LUT_SIZE,DISPL_FLUX_LUT}

// sum all displacement sizes selected for RAM buffering
#define  MCI_CLASS_A_PARAMETERS_SIZE_RAM     DISPL_DTC_CONTROL_SIZE + DISPL_MOTOR_LUT_SIZE + DISPL_SPEED_CTRL_SIZE + DISPL_MTPA_LUT_SIZE + DISPL_MTPV_LUT_SIZE + DISPL_FLUX_LUT_SIZE

///////////////////////////// MCI - CLASS B Section  ////////////////////////////////////////
// DISPLACEMENT AVAILABLE ON THE MCI CLASS B POINTER (SR) //
#define DISPL_SR_MOTOR           1

// For each used displacement declare its size here
#define DISPL_SR_MOTOR_SIZE            SAFETY_MOTOR_PARAMETERS_NUM

// Define here the the memory location of the displacements.
// For each memory location define comma separated displacements in the right order.
// REMARK: Only define the used memory locations. Leave the unused locations undefined
#if (!defined MCI_INTERNAL_PARAMS)
    #define SF_CLASSB_RAM_DISPLS          {DISPL_SR_MOTOR}
#else
    #define INTERNAL_CLASSB_DISPLS        {DISPL_SR_MOTOR}
#endif
//#define SF_CLASSB_FLASH_DISPLS        {}

// For internal displacements only, define the sizes, in the same order internal
// displacements definition. Values must be comma separated.
#define INTERNAL_CLASSB_DISPL_SIZES    {DISPL_SR_MOTOR_SIZE}

// sum all displacement sizes selected for RAM buffering
#define  MCI_CLASS_B_PARAMETERS_SIZE_RAM     DISPL_SR_MOTOR_SIZE

//=====================================================================================================================
//-------------------------------------- PUBLIC (Function Prototypes) -------------------------------------------------
//=====================================================================================================================


#endif /* MCIPARAMETERSLOADER_PRV_H */

