/**
 *  @file
 *
 *  @brief      Source file for the Speed Maintain module.
 *
 *  @details    This module is a motion requestor that will maintain the
 *				running where there is no active motion requestor.
 **/
 
#ifndef SPEED_MAINTAIN_H
#define SPEED_MAINTAIN_H

//=====================================================================================================================
//-------------------------------------- Public Functions -------------------------------------------------------------
//=====================================================================================================================

void SpeedMaintain__Initialize(void);
void SpeedMaintain__SetSpinActivity(MOTION_PROFILE_TYPE motion_prof);
void SpeedMaintain__ReleaseMotion(void);
void SpeedMaintain__Handler25ms(void);

#endif
