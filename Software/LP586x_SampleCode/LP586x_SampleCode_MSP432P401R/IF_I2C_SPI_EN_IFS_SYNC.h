/* --COPYRIGHT--,BSD
 * Copyright (c) 2015, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * --/COPYRIGHT--*/
//****************************************************************************
//
// IF_I2C_SPI_EN_IFS_SYNC.h - Prototypes of hardware abstraction layer for I2C between
//             between LP586x with MSP432P401R
//
//****************************************************************************

#include <stdio.h>
#include <stdint.h>

#ifndef __IF_I2C_SPI_EN_IFS_SYNC_H_
#define __IF_I2C_SPI_EN_IFS_SYNC_H_

#define CPU_F ((double)48000000)
#define delay_us(x) __delay_cycles((long)(CPU_F*(double)x/1000000.0))
#define delay_ms(x) __delay_cycles((long)(CPU_F*(double)x/1000.0))

extern void Init_I2C_GPIO(void);
extern void I2C_init(void);
extern void I2C_setSlave(unsigned short slaveAdr_5bit,
                         unsigned short regAdr_10bit);
extern void I2C_write (unsigned short regAdr_10bit,
                       uint8_t writeByte);
extern int I2C_read_16bit(unsigned char regAdr_10bit);
extern void Disable_EN();
extern void Enable_EN();
extern void IFS_I2C();
extern void IFS_SPI();
extern void Initial_SYNC();
extern void Send_SYNC();

#endif /* __IF_I2C_SPI_EN_IFS_SYNC_H_ */
