/**
 *  @file       ProtocolMainExternal_prv.h
 *
 *  @brief
 *
 *  @section
 *
 *  $Header: ProtocolMainExternal_prv.h 1.4 2015/12/22 15:12:18CET Maciej  Kobuszewski (KOBUSM) Exp  $
 *
 *  @copyright  Copyright 2013-$Date: 2015/12/22 15:12:18CET $. Whirlpool Corporation. All rights reserved - CONFIDENTIAL
 */
//---------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------
#ifndef PROTOCOLMAINEXTERNAL_PRV_H_
    #define PROTOCOLMAINEXTERNAL_PRV_H_

    #define PROTOCOLMAINEXTERNAL_BAUDRATE   B9600
    #define PROTOCOLMAINEXTERNAL_COMPORT    COM0



	#if (APPLIANCE_TYPE == APPLIANCE_TYPE_DISHWASHER)
	#define PILOT_TYPE_WASH			34
	#define PILOT_TYPE_DRAIN		35
	#endif
#endif // MAINBOARDCOMMANDER_PRV_H_

