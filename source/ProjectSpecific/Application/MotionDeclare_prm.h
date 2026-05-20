/**
 *  @file
 *
 *  @brief      Public parameters declarations for the Motion module.
 *
 *  $Header: MotionDeclare.h 1.6 2016/03/02 10:38:23EST Paul F Spindler (SPINDPF) Exp  $
 *
 *  @copyright  Copyright 2015-$Date: 2016/03/02 10:38:23EST $. Whirlpool Corporation. All rights reserved - CONFIDENTIAL
 */
//---------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------
#ifndef MOTIONDECLARE_PRM_H_
#define MOTIONDECLARE_PRM_H_

//=====================================================================================================================
//-------------------------------------- PUBLIC (Extern Variables, Constants & Defines) -------------------------------
//=====================================================================================================================


#define MOTION__OPTIONAL_INITIALIZE() //{ MciSetWm__SoftStart(SOFT_START_INITIALIZATION); }

#define MOTION__POSTPROCESS_RUN()     // MciSetWm__SoftStart(SOFT_START_ON_START); }

#define MOTION__PREPROCESS_STOP()     //{ MciSetWm__SoftStart(SOFT_START_ON_STOP); }

#define SPINDRUM_MAX_NEGATIVE_DRUM_SPEED    90
#define SPINDRUM_MAX_START_DRUM_SPEED       127

#endif // MOTIONDECLARE_PRM_H_


