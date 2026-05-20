/**
 *  @file
 *  @brief      Contains controller specific definition for STM32F0x
 *
 *  $Header: Spi_Defs.h 1.2 2014/05/14 14:10:09IST Natak Rahul (NATAKRB) Exp  $
 *
 *   @copyright  Copyright 2007-2012-$Date: 2014/05/14 14:10:09IST $. Whirlpool Corporation. All rights reserved - CONFIDENTIAL
 */
//-------------------------------------- Include Files ----------------------------------------------------------------
#ifndef SPI_DEFS_H_
#define SPI_DEFS_H_

void Spi__Interrupt0(void);
void Spi__Interrupt1(void);

void Spi__TXDmaInterrupt0(void);
void Spi__RXDmaInterrupt0(void);
void Spi__TXDmaInterrupt1(void);
void Spi__RXDmaInterrupt1(void);

#endif // SPI_DEFS_H_


