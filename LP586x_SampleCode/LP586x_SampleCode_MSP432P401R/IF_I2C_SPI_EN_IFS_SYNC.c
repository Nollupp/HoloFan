/* --COPYRIGHT--,BSD
 * Copyright (c) 2021, Texas Instruments Incorporated
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
// IF_I2C_SPI_EN_IFS_SYNC.c - Hardware abstraction layer for Interface between LP586x with MSP432P401R
// Create time: 3.18.2021
// Author: Hardy Wu
//
//****************************************************************************

#include <IF_I2C_SPI_EN_IFS_SYNC.h>
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <stdio.h>
#include <stdint.h>

/* Timer configuration */
#define CPU_F ((double)48000000)
#define delay_us(x) __delay_cycles((long)(CPU_F*(double)x/1000000.0))
#define delay_ms(x) __delay_cycles((long)(CPU_F*(double)x/1000.0))

/* I2C master configuration parameter */
const eUSCI_I2C_MasterConfig i2cConfig =
{
        EUSCI_B_I2C_CLOCKSOURCE_SMCLK,          // SMCLK Clock Source
        3000000,                                // SMCLK = 3MHz
        EUSCI_B_I2C_SET_DATA_RATE_400KBPS,      // Desired I2C Clock of 400khz
        0,                                      // No byte counter threshold
        EUSCI_B_I2C_NO_AUTO_STOP                // No Autostop
};

/***************************************************************************//**
 * @brief  Initialize GPIO for I2C
 * @param  none
 * @return none
 ******************************************************************************/
void Init_I2C_GPIO()
{
    /* Select I2C function for I2C_SCL(P6.5) & I2C_SDA(P6.4) */
    GPIO_setAsPeripheralModuleFunctionOutputPin(
            GPIO_PORT_P6,
            GPIO_PIN5,
            GPIO_PRIMARY_MODULE_FUNCTION);

    GPIO_setAsPeripheralModuleFunctionOutputPin(
            GPIO_PORT_P6,
            GPIO_PIN4,
            GPIO_PRIMARY_MODULE_FUNCTION);
}


/***************************************************************************//**
 * @brief  Configures I2C
 * @param  none
 * @return none
 ******************************************************************************/
void I2C_init(void)
{
    /* Initialize USCI_B0 and I2C Master to communicate with slave devices*/
    I2C_initMaster(EUSCI_B1_BASE, &i2cConfig);

    /* Disable I2C module to make changes */
    I2C_disableModule(EUSCI_B1_BASE);

    /* Enable I2C Module to start operations */
    I2C_enableModule(EUSCI_B1_BASE);

    return;
}

/***************************************************************************//**
 * @brief  Set slave address
 * @param  slaveAdr_5bit: slave address you want to control
 * @param  regAdr_10bit: register address you want to write
 * @return none
 ******************************************************************************/
void I2C_setSlave(unsigned short slaveAdr_5bit,
                  unsigned short regAdr_10bit)
{
    /* Transfer slave address to I2C standard format */
    unsigned char slaveAdr_8bit;
    slaveAdr_8bit = (slaveAdr_5bit<<2) + (regAdr_10bit>>8);

    /* Specify slave address for I2C */
    I2C_setSlaveAddress(EUSCI_B1_BASE,
                        slaveAdr_8bit);

    /* Enable and clear the interrupt flag */
    I2C_clearInterruptFlag(EUSCI_B1_BASE,
        EUSCI_B_I2C_TRANSMIT_INTERRUPT0 + EUSCI_B_I2C_RECEIVE_INTERRUPT0);
    return;
}

/***************************************************************************//**
 * @brief  Writes data to LED driver
 * @param  regAdr_10bit: register address you want to write
 * @param  writeByte: Data to be written
 * @return none
 ******************************************************************************/
void I2C_write (unsigned short regAdr_10bit,
                uint8_t writeByte)
{
    /* Transfer data format to I2C standard*/
    unsigned char regAdr_8bit;
    regAdr_8bit = regAdr_10bit;

    /* Set master to transmit mode */
    I2C_setMode(EUSCI_B1_BASE,
        EUSCI_B_I2C_TRANSMIT_MODE);

    /* Clear any existing interrupt flag */
    I2C_clearInterruptFlag(EUSCI_B1_BASE,
       EUSCI_B_I2C_TRANSMIT_INTERRUPT0);

    /* Wait until ready to write */
    while (I2C_isBusBusy(EUSCI_B1_BASE));

    /* Initiate start and send first character */
    I2C_masterSendMultiByteStart(EUSCI_B1_BASE,
                                 regAdr_8bit);

    /* Send the data */
    I2C_masterSendMultiByteFinish(EUSCI_B1_BASE,
                                  writeByte);

    delay_us(5);

}

/***************************************************************************//**
 * @brief  Reads data from the sensor
 * @param  writeByte Address of register to read from
 * @return Register contents
 ******************************************************************************/
int I2C_read_16bit(unsigned char regAdr_10bit)
{
    /* Transfer data format to I2C standard*/
    unsigned char regAdr_8bit;
    regAdr_8bit = regAdr_10bit;

    volatile int val = 0;
    volatile int valScratch = 0;

    /* Set master to transmit mode PL */
    I2C_setMode(EUSCI_B1_BASE,
        EUSCI_B_I2C_TRANSMIT_MODE);

    /* Clear any existing interrupt flag PL */
    I2C_clearInterruptFlag(EUSCI_B1_BASE,
        EUSCI_B_I2C_TRANSMIT_INTERRUPT0);

    /* Wait until ready to write PL */
    while (I2C_isBusBusy(EUSCI_B1_BASE));

    /* Initiate start and send first character */
    I2C_masterSendMultiByteStart(EUSCI_B1_BASE, regAdr_8bit);

    /* Wait for TX to finish */
    while(!(I2C_getInterruptStatus(EUSCI_B1_BASE,
        EUSCI_B_I2C_TRANSMIT_INTERRUPT0)));

    /* Initiate stop only */
    I2C_masterSendMultiByteStop(EUSCI_B1_BASE);

    /* Wait for Stop to finish */
    while(!I2C_getInterruptStatus(EUSCI_B1_BASE,
        EUSCI_B_I2C_STOP_INTERRUPT));

    /*
     * Generate Start condition and set it to receive mode.
     * This sends out the slave address and continues to read
     * until you issue a STOP
     */
    I2C_masterReceiveStart(EUSCI_B1_BASE);

    /* Wait for RX buffer to fill */
    while(!(I2C_getInterruptStatus(EUSCI_B1_BASE,
        EUSCI_B_I2C_RECEIVE_INTERRUPT0)));

    /* Read from I2C RX register */
    val = I2C_masterReceiveMultiByteNext(EUSCI_B1_BASE);

    /* Receive second byte then send STOP condition */
    valScratch = I2C_masterReceiveMultiByteFinish(EUSCI_B1_BASE);

    /* Shift val to top MSB */
    val = (val << 8);

    /* Read from I2C RX Register and write to LSB of val */
    val |= valScratch;

    /* Return temperature value */
    return (int16_t)val;
}

/***************************************************************************//**
 * @brief  Pull down GPIO to disable IC
 * @param  none
 * @return none
 ******************************************************************************/
void Disable_EN()
{
    /* Pull down P5.6*/
    GPIO_setAsOutputPin(GPIO_PORT_P5, GPIO_PIN6);
    GPIO_setOutputLowOnPin(GPIO_PORT_P5, GPIO_PIN6);//Set Enable to Low to enter SHUTDOWN mode
}

/***************************************************************************//**
 * @brief  Pull high GPIO to enable IC
 * @param  none
 * @return none
 ******************************************************************************/
void Enable_EN()
{
    /* Pull high P5.6*/
    GPIO_setAsOutputPin(GPIO_PORT_P5, GPIO_PIN6);
    GPIO_setOutputHighOnPin(GPIO_PORT_P5, GPIO_PIN6);//Set Enable to High to enter INITIALIZATION mode
}

/***************************************************************************//**
 * @brief  IFS pin set to I2C
 * @param  none
 * @return none
 ******************************************************************************/
void IFS_I2C()
{
    /* Pull low P2.4*/
    GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN4);
    GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN4);//Set IFS to low to use I2C interface
}

/***************************************************************************//**
 * @brief  IFS pin set to SPI
 * @param  none
 * @return none
 ******************************************************************************/
void IFS_SPI()
{
    /* Pull high P2.4*/
    GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN4);
    GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN4);//Set IFS to high to use I2C interface
}

/***************************************************************************//**
 * @brief  Initial SYNC pin
 * @param  none
 * @return none
 ******************************************************************************/
void Initial_SYNC()
{
    /* Using P2.6 to send SYNC signal */
    GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN6);
}

/***************************************************************************//**
 * @brief  Send sync signal
 * @param  none
 * @return none
 ******************************************************************************/
void Send_SYNC()
{
    /* Using P2.6 to send SYNC signal */
    GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN6);
    delay_us(200);
    GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN6);
}
