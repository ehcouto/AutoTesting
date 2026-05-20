/**
 *  @file
 *  @brief      Params
 *
 *  @details    Params
 *
 *
 *  @copyright  Copyright 2016. Whirlpool Corporation. All rights reserved - CONFIDENTIAL
 */
//---------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------
//-------------------------------------- Include Files ----------------------------------------------------------------
#include "C_Extensions.h"
#include "Params.h"
#include "SettingFile.h"


//-------------------------------------- PUBLIC (Variables) -----------------------------------------------------------
//Variable setting using communication with main uC
BOOL_TYPE Parms_Ready;

SF_APPLICATION_PARAMS_TYPE SF_Application_Params; //To be backward compatible with EOL -> Max acceleration key unlock.

uint8 Mci_ClassA_Dtc[sizeof(MCL_PARAMS_DISPL1_TYPE)];
uint8 Mci_ClassA_Motor_Lut[sizeof(MCL_PARAMS_DISPL3_TYPE)];
uint8 Mci_ClassA_Speed_Ctrl[sizeof(MCL_PARAMS_DISPL4_TYPE)];
uint8 Mci_ClassA_Mtpa[sizeof(MCL_PARAMS_DISPL9_TYPE)];
uint8 Mci_ClassA_Mtpv[sizeof(MCL_PARAMS_DISPL10_TYPE)];
uint8 Mci_ClassA_Ote[51*4];

uint8 Mci_ClassB_Prm[SAFETY_MOTOR_PARAMETERS_NUM];

uint8 Mca_Set_Wm[70*2];
uint8 Mca_Sensor_Wm[6*2];

//-------------------------------------- PRIVATE (Variables, Constants & Defines) -------------------------------------

//-------------------------------------- PRIVATE (Function Prototypes) ------------------------------------------------


//=====================================================================================================================
//-------------------------------------- Public Functions -------------------------------------------------------------
//=====================================================================================================================

//---------------------------------------------------------------------------------------------------------------------
/**
 *  @brief      It Initializes the module Params and its variables
 *
 */
void Params__Initialize(void)
{

}



//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief   Loads setting file data for a displacement within a pointer.
 * @param   pointer_id: See PARAMS_LOADER_TYPE
 * @param   displacement: The displacement (or offset) into the pointer where the data is located
 * @param   sf_data_ptr: Pointer to the setting file data (see SETTINGFILE_LOADER_TYPE)
 * @return  TRUE if valid data was found at the given pointer and displacement.
 */
PASS_FAIL_TYPE Params__BasicLoader(SETTINGFILE_OFFSET_TYPE pointer_id, uint16 displacement, PARAMS_LOADER_TYPE * sf_data_ptr)
{

    PASS_FAIL_TYPE ret_val = PASS;

    if(Params__IsReady() == TRUE)
    {
        switch (pointer_id)
        {
            case SF_PTR_MCU_CLASS_A_MCI:

            //////////////////////////////////////////////////////////////////
            /////////////////// Class A Motor PARAMETERS /////////////////////
            //////////////////////////////////////////////////////////////////
            switch (displacement)
            {
                case SF_DISPL_MOTOR_PARAMS_DTC:
                    sf_data_ptr->Data = Mci_ClassA_Dtc;
                    sf_data_ptr->Length = sizeof(Mci_ClassA_Dtc);
                    break;

                case SF_DISPL_MOTOR_PARAMS_LUT:
                    sf_data_ptr->Data = Mci_ClassA_Motor_Lut;
                    sf_data_ptr->Length = sizeof(Mci_ClassA_Motor_Lut);
                    break;

                case SF_DISPL_MOTOR_SPEED_CTRL:
                    sf_data_ptr->Data = Mci_ClassA_Speed_Ctrl;
                    sf_data_ptr->Length = sizeof(Mci_ClassA_Speed_Ctrl);
                    break;

                case SF_DISPL_MTPA_LUT:
                    sf_data_ptr->Data = Mci_ClassA_Mtpa;
                    sf_data_ptr->Length = sizeof(Mci_ClassA_Mtpa);
                    break;

                case SF_DISPL_MTPV_LUT:
                    sf_data_ptr->Data = Mci_ClassA_Mtpv;
                    sf_data_ptr->Length = sizeof(Mci_ClassA_Mtpv);
                    break;

                case SF_DISPL_MOTOR_OTE:
                    sf_data_ptr->Data = Mci_ClassA_Ote;
                    sf_data_ptr->Length = sizeof(Mci_ClassA_Ote);
                    break;

                default:
                    ret_val = FAIL;
                    break;
            }
            break;
            //////////////////////////////////////////////////////////////////
            /////////////////// Class A Motor PARAMETERS /////////////////////
            //////////////////////////////////////////////////////////////////



            case SF_PTR_MCU_CLASS_B_MCI:
                if(displacement == SF_DISPL_MOTOR_SR_PARAMS)
                {
                    sf_data_ptr->Data = Mci_ClassB_Prm;
                    sf_data_ptr->Length = sizeof(Mci_ClassB_Prm);
                }
                else
                {
                    ret_val = FAIL;
                }
            break;



            case SF_PTR_MCU_MCA:
               if(displacement == SF_DISPL_MOTOR_MCI_SET_WM)
               {
                   sf_data_ptr->Data = Mca_Set_Wm;
                   sf_data_ptr->Length = sizeof(Mca_Set_Wm);
               }
               else if(displacement == SF_DISPL_MOTOR_MCI_SENSORS_WM)
               {
                   sf_data_ptr->Data = Mca_Sensor_Wm;
                   sf_data_ptr->Length = sizeof(Mca_Sensor_Wm);
               }
               else
               {
                   ret_val = FAIL;
               }
            break;

            default:
                ret_val = FAIL;
            break;

        }
    }
    else
    {//Parameters are not loaded
        ret_val = FAIL;
    }

    return ret_val;
}

//=====================================================================================================================
//-------------------------------------- Private Functions ------------------------------------------------------------
//=====================================================================================================================
