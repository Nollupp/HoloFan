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
// main.c - MSP-EXP432P401R + Educational Boosterpack MkII - Light Sensor
//
//          Displays lux value measured by the OPT3001 Digital Ambient
//          Light Sensor on the colored LCD. The MSP432 communicates
//          with the sensor through I2C.
//
//          The ambient light measurement is also used to automatically
//          adjust the LCD backlight.
//
//      *** Make sure J5 jumper on the BOOSTXL-EDUMKII is connected ***
//          to 3.LCD BACKLT
//
//****************************************************************************

#include <IF_I2C_SPI_EN_IFS_SYNC.h>
#include <ti/devices/msp432p4xx/inc/msp.h>
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <ti/grlib/grlib.h>
#include "LP586x.h"
#include "LED_Animation.h"
#include "LED_Animation_withSYNC.h"
#include <stdio.h>
#include <stdint.h>

/*
 * Main function
 */
int main(void)
{
    /* Halting WDT and disabling master interrupts */
    MAP_WDT_A_holdTimer();
    MAP_Interrupt_disableMaster();

    /* Initializes Clock System*/
    //MAP_CS_setDCOCenteredFrequency(CS_DCO_FREQUENCY_48);
    MAP_CS_initClockSignal(CS_MCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1);
    MAP_CS_initClockSignal(CS_HSMCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1);
    MAP_CS_initClockSignal(CS_SMCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1);
    MAP_CS_initClockSignal(CS_ACLK, CS_REFOCLK_SELECT, CS_CLOCK_DIVIDER_1);

    /* Enabling SRAM Bank Retention */
    MAP_SysCtl_enableSRAMBankRetention(SYSCTL_SRAM_BANK1);

    /* Initialize I2C communication */
    Init_I2C_GPIO();
    I2C_init();

    Disable_EN();
    Enable_EN();
    IFS_I2C();
    Initial_SYNC();
    delay_ms(1);

    Chip_Normal();
    delay_ms(100);

    Device_Initial(Max_Line_11, Mode_1, PWM_Frequency_125k);
    //Device_Configuration_1(SW_BLK_1us, PWM_Scale_Exponential, PWM_PhaseShift_on, CS_Delay_on);
    Device_Configuration_2(Comp_Group3_2clock, Comp_Group2_Off, Comp_Group1_Off, LOD_Removal_Off, LSD_Removal_Off);

    //Breathing();
    //Rainbow_test();

    //SmileFace();
    //Snake_Loop(200);
    //Snake_Single(250);

    //Helix_Single(255);
    //Firework(255);

    Count(250);
    Snake_Single_withLP5860(250);
    delay_ms(500);
    Helix_Night(250);
    Firework_Night(250);
    Rainbow();

}
