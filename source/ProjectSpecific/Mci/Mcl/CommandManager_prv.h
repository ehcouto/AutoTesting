/**
 *  @file        CommandManager_prv.h
 *  @brief       Application Parameters for 3-phase motors.
 *
 *               ==== AUTOMATIC GENERATED FILE! DO NOT TOUCH IT, USE EXCEL MACRO! ====
 *
 *  @details     This module defines all application parameters. Created in 20/11/2017 15:20:30
 *               Setting File Parameters for 
 *  @copyright   Copyright 2016.  Whirlpool Corporation.  All rights reserved - CONFIDENTIAL
 *
 */

#ifndef COMMANDMANAGER_PRV_H
#define COMMANDMANAGER_PRV_H


//=====================================================================================================================
//-------------------------------------- PUBLIC (Extern Variables, Constants & Defines) -------------------------------
//=====================================================================================================================


#include "Mci_prm.h" 


#ifdef MCI_INTERNAL_PARAMS
//Motor Control Application Parameters

//Algorithm 
//Algo Frequency = 200 Hz

//Speed Controller (SC)
//Threshold Wash = 60 rpm_D
//Threshold Distribution = 88 rpm_D
//Threshold Spin = 1280 rpm_D
//Max Speed Motor = 19800 rpm_M
//Kp (rpm<wash) = 8 Nmm/rpm
//Ki (rpm<wash) = 6.5 Nmm/(rpm s)
//Kp (rpm>wash) = 1.3 Nmm/rpm
//Ki(rpm>wash) = 5.9 Nmm/(rpm s)
//Kp (rpm<spin) = 1.3 Nmm/rpm
//Ki (rpm<spin) = 5.9 Nmm/(rpm s)
//Kp (rpm>spin) = 0.5 Nmm/rpm
//Ki (rpm>spin) = 1.9 Nmm/(rpm s)
//Transmission Ratio = 11.7 adm

//Is Max
//Is Max Spin = 0 Apeak
//Is Max Tumbling = 7 Apeak
//Is Max Thr Speed = 6000 rpm_M


signed short int Command_Params_SF[10] =
{
/*Transmission_Ratio = Transmission Ratio * 256 =*/                     2995,\
/*Free_Down_Gain = Algo Frequency /(Threshold Wash * Transmission Ratio)  *32768 =*/                     9336,\
/*Max_Allowed_Speed = 0.93 * Base Speed / Transmission Ratio =*/                     1740,\
/*Low_Regulators_Thr_Speed = Threshold Wash * Transmission Ratio =*/                     702,\
/*Med_Regulators_Thr_Speed = Threshold Distribution * Transmission Ratio  =*/                     1030,\
/*High_Regulators_Thr_Speed = Threshold Spin * Transmission Ratio  =*/                     14976,\
/*Max_Deceleration_Low_Speed = Max Deceleration Low Speed * Transmission Ratio =*/                     585,\
/*Max_Deceleration_High_Speed = Max Deceleration High Speed * Transmission Ratio =*/                     176,\
/*Max_Acceleration_Low_Speed = Max Acceleration Low Speed * Transmission Ratio =*/                     1544,\
/*Max_Acceleration_High_Speed = Max Acceleration High Speed * Transmission Ratio =*/                     293,\
};

#if 0
//DEBUG_FAGNAL
signed short int OldSettingFile[] =
{
 2     ,\
 15867 ,\
 12177 ,\
 100   ,\
 4934  ,\
 1700  ,\
 2560  ,\
 145   ,\
 14167 ,\
 1581  ,\
 2500  ,\
 25000 ,\
 50    ,\
 31130 ,\
 -2404 ,\
 2500  ,\
 -21987,\
 26444 ,\
 14723 ,\
 4064  ,\
 256   ,\
 444   ,\
 8418  ,\
 40775 ,\
 17000 ,\
 200   ,\
 650   ,\
 0     ,\
 0     ,\
 12529 ,\
 15762 ,\
 252   ,\
 32354 ,\
 6693  ,\
 16065 ,\
 8497  ,\
 438   ,\
 438   ,\
 -15508,\
 8372  ,\
 8417  ,\
 -780  ,\
 8648  ,\
 11456 ,\
 -8    ,\
 14853 ,\
 -1    ,\
 8674  ,\
 7040  ,\
 3012  ,\
 10    ,\
 5461  ,\
 12741 ,\
 3840  ,\
 6023  ,\
 11567 ,\
 10364 ,\
 -7    ,\
 14853 ,\
 -1    ,\
 1946  ,\
 1000  ,\
 386   ,\
 0     ,\
 29544 ,\
 289   ,\
 17442 ,\
 14255 ,\
 764   ,\
 25600 ,\
 32000 ,\
 15944 ,\
 16327 ,\
 -522  ,\
 0     ,\
 -19463,\
 9965  ,\
 15561 ,\
 -260  ,\
 22938 ,\
 24576 ,\
 21907 ,\
 11622 ,\
 1020  ,\
 0     ,\
 2621  ,\
 3476  ,\
 16384 ,\
 1157  ,\
 1696  ,\
 6939  ,\
 20679 ,\
 13235 ,\
 1278  ,\
 20679 ,\
 13235 ,\
 1278  ,\
 16544 ,\
 8470  ,\
 1277  ,\
 22106 ,\
 15924 ,\
 1529  ,\
 23356 ,\
 19463 ,\
 -4768 ,\
 8192  ,\
 32702 ,\
 0     ,\
 8     ,\
 6939  ,\
 7903  ,\
 9830  ,\
 11758 ,\
 19468 ,\
 22938 ,\
 27371 ,\
 0     ,\
 4179  ,\
 4179  ,\
 3857  ,\
 3536  ,\
 3375  ,\
 3295  ,\
 3697  ,\
 0     ,\
 5658  ,\
 5658  ,\
 5658  ,\
 5658  ,\
 5658  ,\
 5127  ,\
 4179  ,\
 0     ,\
 0     ,\
 0     ,\
 -5465 ,\
 -5465 ,\
 -683  ,\
 -759  ,\
 2970  ,\
 0     ,\
 0     ,\
 0     ,\
 0     ,\
 0     ,\
 0     ,\
 -5009 ,\
 -7009 ,\
 0     ,\
 22962 ,\
 10987 ,\
 14013 ,\
 14013 ,\
 5     ,\
 5812  ,\
 0     ,\
 63161 ,\
 1     ,\
 60787 ,\
 0     ,\
 37897 ,\
 10853 ,\
 80    ,\
 1284  ,\
 1053  ,\
};
#endif
#endif //


#endif //COMMANDMANAGER_PRV_H
