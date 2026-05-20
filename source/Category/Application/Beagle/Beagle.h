/**********************************************************************************************************************
 *  @file       Beagle.h
 *  @brief      Beagle module sends high speed data through SPI port with DMA operation.
 *
 *  @details    The protocol does not implement handshake.
 *
 *---------------------------------------------------------------------------------------------------------------------
 *------------------- Copyright 2013.  Whirlpool Corporation.  All rights reserved - CONFIDENTIAL ---------------------
 *---------------------------------------------------------------------------------------------------------------------
 */

#ifndef BEAGLE_H_
#define BEAGLE_H_

#include "Beagle_prm.h"


void Beagle__Init(BEAGLE_DATA_TYPE *beagle_data_buffer);
BOOL_TYPE Beagle__TriggerTXData(void);
void Beagle__HighSpeedHandler(void);
void Beagle__ChangeRate(unsigned short new_rate);


#endif
