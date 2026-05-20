/**
 *  @file       MciSetWmFloat.c
 *
 *  @brief      Indesit Wm Mci Setting Algorithms module
 *
 *  $Header: $
 *
 *  @copyright  Copyright 2016-$Date$. Whirlpool Corporation. All rights reserved - CONFIDENTIAL
 */
//---------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------


//-------------------------------------- Include Files ----------------------------------------------------------------
#include "MciSetWmFloat.h"
#include "Mci.h"
#include "MathCalc.h"


#if (MCI_SET_PARAMETERS_INTERNAL == ENABLED)
    #include "MciSetWmFloat_prv.h"  //Mca parameters
#else
    #include "string.h"
    #include "SettingFile.h"
#endif
//-------------------------------------- PUBLIC (Variables) -----------------------------------------------------------

//-------------------------------------- PRIVATE (Variables, Constants & Defines) -------------------------------------
static BOOL_TYPE ParamsReady;
static MCISETWMFLOAT_PARAMS_TYPE *MciSetWmFloat_Params;


#define MCA_SET_WM_FLOAT_PARAMETERS_SIZE                  sizeof(MCISETWMFLOAT_PARAMS_TYPE)
SETTINGFILE_LOADER_TYPE Mca_Set_Wm_Prm_Flash_Ptr;
uint8 Mca_Set_Wm_Prm_Ram[MCA_SET_WM_FLOAT_PARAMETERS_SIZE];


//-------------------------------------- PRIVATE (Function Prototypes) ------------------------------------------------



#if (DYNAMIC_TORQUE_LIMITS == ENABLED)
static void DynamicTorqueLimits(void);
#endif
//=====================================================================================================================
//-------------------------------------- Public Functions -------------------------------------------------------------
//=====================================================================================================================
//---------------------------------------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------------------------------------
/**
 *  @brief      It Initializes the module MciSetWmFloats and its variables.
 *
 */
void MciSetWmFloat__Initialize(void)
{
    ParamsReady = FALSE;

#if (MCI_SET_PARAMETERS_INTERNAL == ENABLED)
    MciSetWmFloat_Params = (MCISETWM_PARAMS_TYPE *) (&MciSetWmFloat_Params_SF[0]);
    ParamsReady = TRUE;
#else
    if((SettingFile__IsValid() == TRUE) &&
        (SettingFile__BasicLoader(SF_PTR_MCU_MCA, SF_DISPL_MOTOR_MCI_SET_WM_FLOAT, &Mca_Set_Wm_Prm_Flash_Ptr) == PASS))
    {
        memcpy(&Mca_Set_Wm_Prm_Ram, Mca_Set_Wm_Prm_Flash_Ptr.Data, Mca_Set_Wm_Prm_Flash_Ptr.Length); //Copy from Flash to RAM
        MciSetWmFloat_Params = (MCISETWMFLOAT_PARAMS_TYPE *) (&Mca_Set_Wm_Prm_Ram);
        ParamsReady = TRUE;;
    }
#endif

}


//---------------------------------------------------------------------------------------------------------------------
/**
 *  @brief      Feeds back if Mci Set module parameters are loaded.
 *
 *  @param      none
 *  @return     TRUE: parameters are loaded / FALSE: parameters are not loaded
 */
BOOL_TYPE MciSetWmFloat__IsParamsReady(void)
{
    return(ParamsReady);
}



//---------------------------------------------------------------------------------------------------------------------
/**
 *  @brief      Handle Wm Set Mci events of 5ms.
 *
 *  @param
 *  @return
 */
void MciSetWmFloat__5msHandler(void)
{
	if(ParamsReady == TRUE)
	{
#if (DYNAMIC_TORQUE_LIMITS == ENABLED)
		DynamicTorqueLimits();
#endif
	}

    else //parameters not loaded
    {
        #if (MCI_SET_PARAMETERS_INTERNAL == ENABLED)

        MciSetWmFloat_Params = (MCISETWM_PARAMS_TYPE *) (&MciSetWmFloat_Params_SF[0]);
        ParamsReady = TRUE;

        #else

        if((SettingFile__IsValid() == TRUE) &&
            (SettingFile__BasicLoader(SF_PTR_MCU_MCA, SF_DISPL_MOTOR_MCI_SET_WM_FLOAT, &Mca_Set_Wm_Prm_Flash_Ptr) == PASS))
        {
            memcpy(&Mca_Set_Wm_Prm_Ram, Mca_Set_Wm_Prm_Flash_Ptr.Data, Mca_Set_Wm_Prm_Flash_Ptr.Length); //Copy from Flash to RAM
            MciSetWmFloat_Params = (MCISETWMFLOAT_PARAMS_TYPE *) (&Mca_Set_Wm_Prm_Ram);
            ParamsReady = TRUE;;
        }
        #endif
    }
}



#if (DYNAMIC_TORQUE_LIMITS == ENABLED)

//---------------------------------------------------------------------------------------------------------------------
/**
 *  @brief     Dynamic torque limits
 *  @details   Calculation of the torque spinning limits.
 *  @param[in]
 *  @param[out]
 *  @return
 */
static void DynamicTorqueLimits(void)
{
    float32  torque_limit; //Q17.15
    float32  speed;
    uint16  i;
    float32*  torque_limit_table;
    float32*  m_limit_table;


    torque_limit_table = &MciSetWmFloat_Params->TorqueLimit[0];
    m_limit_table = &MciSetWmFloat_Params->MLimit[0];


    if(Mci__GetDigital(MOTOR0, MCI_DI_MOTOR_RUNNING) == TRUE)
    {
        //---------------------------------------------------------------------------------------------------------------------
        //                   Dynamic Torque Limits
        //       assign torque limit according to Speed ranges
        //     [ sSetFileParams.f16Speed_Limit[TORQUE_LIM_LENGHT] ]
        //     [ sSetFileParams.f16Torque_Limit[TORQUE_LIM_LENGHT] ]
        //                [ #define TORQUE_LIM_LENGHT 8 ]
        //---------------------------------------------------------------------------------------------------------------------
    	speed = (float32) (1.0f/65536.0f) * Mci__GetAnalog(MOTOR0, MCI_AI_ACTUAL_SPEED_S16);


        for (i = 0; i < TORQUE_LIM_LENGHT; i++)
        {
            if (MciSetWmFloat_Params->SpeedLimit[i]!=0.0f)
            {
                if(speed < MciSetWmFloat_Params->SpeedLimit[i])
                {
                    break;
                }
            }
            else
            {
                break;
            }
        }


        if (i==0)
        {
            torque_limit=0.0f;
        }
        else
        {
            // to avoid no limits on overspeed
            if (MciSetWmFloat_Params->SpeedLimit[i]==0.0f)
            {
                i--;
            }

            //Linear Limits
            torque_limit = torque_limit_table[i] + m_limit_table[i]*(speed-MciSetWmFloat_Params->SpeedLimit[i]);

        }


        torque_limit = torque_limit * 65536.0f;  // torque Nm <<16

        Mci__SetAnalog(MOTOR0, MCI_AO_SPEED_LIMIT_INDEX, (sint32)torque_limit);

    }

}
#endif
