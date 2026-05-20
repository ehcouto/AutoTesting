/**
 *  @file       Gpio.h
 *  @defgroup   CLASS_B
 *
 *  @brief      Generic Gpio API define
 *
 *  $Header: Gpio.h 1.5 2015/06/11 17:15:30EDT MANOEL GARBUIO DE SOUZA (SOUZAMG) Exp  $
 *
 *------------------- Copyright 2011.  Whirlpool Corporation.  All rights reserved - CONFIDENTIAL ---------------------
 *
 */
 
#ifndef GPIO_H                                      // Ensure this file is not compiled again
    #define GPIO_H                                  // after we compile it the first time
#include "C_Extensions.h"
#include "uc.h"
#include "Gpio_def.h"
#include "Gpio_prm.h"
#include "Gpio_macros.h"

//=====================================================================================================================
//-------------------------------------- PUBLIC (Extern Variables, Constants & Defines) -------------------------------
//=====================================================================================================================

typedef enum
{
    INPUT_FLOAT,
    INPUT_PULLUP,
    INPUT_PULLDOWN,
    INPUT_ANALOG,
    SPECIAL_FUNCTION_OPEN_DRAIN,
    SPECIAL_FUNCTION_PUSH_PULL,
    OUTPUT_PUSHPULL,
    OUTPUT_OPEN_DRAIN,
    PIN_TYPE_NUM
} GPIO_PIN_MODE_TYPE;




PACKED typedef struct
{
    GPIO_PORT_TYPE port;
	uint8 pin;
	GPIO_PIN_MODE_TYPE type;
}GPIO_PORTBIT_TYPE;

//=====================================================================================================================
//-------------------------------------- PUBLIC (Function Prototypes) -------------------------------------------------
//=====================================================================================================================
void Gpio__Initialize(void);
void Gpio__RefreshConfig(void);

PASS_FAIL_TYPE Gpio__Validate(GPIO_PORT_TYPE port,uint8 pin);

void Gpio__PinConfig(GPIO_PORT_TYPE port,uint8 pin,GPIO_PIN_MODE_TYPE type);
void Gpio__PinWrite(GPIO_PORT_TYPE port,uint8 pin,BOOL_TYPE value);
BOOL_TYPE Gpio__PinRead(GPIO_PORT_TYPE port,uint8 pin);


void Gpio__PortConfig(GPIO_PORT_TYPE port,GPIO_PIN_MODE_TYPE type);
void Gpio__PortWrite(GPIO_PORT_TYPE port,PORT_DATA_TYPE mask, PORT_DATA_TYPE value);
PORT_DATA_TYPE Gpio__PortRead(GPIO_PORT_TYPE port);

BOOL_TYPE Gpio__CheckConfig(GPIO_PORT_TYPE port,uint8 pin,GPIO_PIN_MODE_TYPE type, BOOL_TYPE state);

#endif


