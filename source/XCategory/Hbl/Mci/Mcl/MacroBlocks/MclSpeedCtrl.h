/**
 *  @file
 *  @brief       Motor Control Loop 3-phase motors: Speed Controller Macro Block.
 *  @details     This module implements the speed controller module.
 *  @author      alessio.beato/luigi.fagnano  (only temporary, since it is not integrated in MKS)
 *  $Header: FOC/Mcl.h 1.3 2015/11/05 11:53:55CET Marcelo Silva (SILVAMC) Exp  $
 * @copyright Copyright 2012 - $Date: 2015/11/05 11:53:55CET $. Whirlpool Corporation. All rights reserved � CONFIDENTIAL
*/
/*
 *
 *---------------------------------------------------------------------------------------------------------------------
 *---------------------------------------------------------------------------------------------------------------------
 */
#ifndef MCL_SPEED_CTRL_H_
    #define MCL_SPEED_CTRL_H_


//-------------------------------------- Include Files ----------------------------------------------------------------
#include "C_Extensions.h"
#include "MclCrossTypes.h"
#include "MclConfig.h"




//=====================================================================================================================
//-------------------------------------- PUBLIC (Extern Variables, Constants & Defines) -------------------------------
//=====================================================================================================================

//! Define the type of variables used by speed controller.
typedef struct
{
    float32 Max_Squared_Inv_Current;                //!< [A^2] - Maximum current that inverter is able to deliver
    float32 Max_Design_Motor_Torque;                //!< [Nm] - Maximum torque according to the motor design
    float32 Rotor_Flux_Nominal;                     //!< [Vs/rad] - Rotor magnets flux constant
    uint32 Force_Default_Gains_Time;                //!< [x Ts] - time after starting to force the default gains

    struct
    {
        float32 Max_Cos;                            //!< [n/a] - define cosine of maximum load angle
        float32 Kp;
        float32 Ki;
    } load_angle;

    struct
    {
        float32 kp;                                 //!< define the speed kp gain
        float32 ki;                                 //!< define the speed ki gain
        float32 speed_abs;                          //!< define below what speed the kp and ki are used, this one
                                                    //!< defines which set of gains are valid!
        sint32 transition_time;                     //!< [x 25ms] - Time to transitate between regions
    } speed_gains_table[SPEEDCTRL__SPEED_REGIONS];  //!< speed gains table, used for speed gains scheduling

    struct
    {
        float32 Max_Braking_Current;                 //!< Max braking current
        float32 Max_Braking_Torque;                  //!< Max Braking Torque available
    }Braking_Parameters;

    float32  pole_pairs_factor;
} MCL_SPEED_CTRL_PARAMS_TYPE;


typedef struct
{
    MCL_SPEED_CTRL_PARAMS_TYPE  *SpeedCtrlPrm;
    MATHCALC_LUT_EXT_F_TYPE       MTPVPrm;
}MCLSPEEDCTRL_JOINT_PARAMS_TYPE;





//=====================================================================================================================
//-------------------------------------- PUBLIC (Function Prototypes) -------------------------------------------------
//=====================================================================================================================

void MclSpeedCtrl__ResetState(MCLSPEEDCTRL_JOINT_PARAMS_TYPE *params);
void MclSpeedCtrl__Initialize(MCLSPEEDCTRL_JOINT_PARAMS_TYPE *params);
void MclSpeedCtrl__RunningHandler(MCL_SPEED_CTRL_IO_F_TYPE *io, MCLSPEEDCTRL_JOINT_PARAMS_TYPE* params);
void MclSpeedCtrl__1msHandler(MCL_SPEED_CTRL_IO_F_TYPE *io, MCLSPEEDCTRL_JOINT_PARAMS_TYPE* params);
void MclSpeedCtrl__25msHandler(MCL_SPEED_CTRL_IO_F_TYPE *io, MCLSPEEDCTRL_JOINT_PARAMS_TYPE* params);

BOOL_TYPE MclSpeedCtrl__SetSpeedRegLimit(sint32 value);
BOOL_TYPE MclSpeedCtrl__SetSpeedGainIndex(sint32 speed_gains_index, MCLSPEEDCTRL_JOINT_PARAMS_TYPE *params);
BOOL_TYPE MclSpeedCtrl__SetDeltaSpeedRef(sint32 delta_speed_x65536, MCL_SPEED_CTRL_IO_F_TYPE *io);

#endif // MCL_SPEED_CTRL_H_
