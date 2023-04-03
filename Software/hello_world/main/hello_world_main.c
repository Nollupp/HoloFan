/* Hello World Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "driver/spi_master.h"
#include "driver/gpio.h"

#define PIN_NUM_MISO 6
#define PIN_NUM_MOSI 4
#define PIN_NUM_CLK  5
#define PIN_NUM_CS   7


void LEDDriver_data(spi_device_handle_t spi, const uint8_t *data, int len)
{
    esp_err_t ret;
    spi_transaction_t t;
    if (len==0) return;             //no need to send anything
    memset(&t, 0, sizeof(t));       //Zero out the transaction
    t.length=len*8;                 //Len is in bytes, transaction length is in bits.
    t.tx_buffer=data;               //Data
    t.user=(void*)1;                //D/C needs to be set to 1
    ret=spi_device_polling_transmit(spi, &t);  //Transmit!
    assert(ret==ESP_OK);            //Should have had no issues.
}

void app_main(void)
{
    printf("Hello world!\n");
    esp_err_t ret;
    spi_device_handle_t spi;
    spi_bus_config_t buscfg={
        .miso_io_num=PIN_NUM_MISO,
        .mosi_io_num=PIN_NUM_MOSI,
        .sclk_io_num=PIN_NUM_CLK,
        .quadwp_io_num=-1,
        .quadhd_io_num=-1,
    };
    spi_device_interface_config_t devcfg={
        .command_bits=0,
        .address_bits=0,
        .dummy_bits=0,
        .clock_speed_hz=5000000,
        .duty_cycle_pos=128,        //50% duty cycle
        .mode=0,
        .spics_io_num=PIN_NUM_CS,
        .cs_ena_posttrans=3,        //Keep the CS low 3 cycles after transaction, to stop slave from missing the last bit when CS has less propagation delay than CLK
        .queue_size=3
    };

    int n=0;
    uint8_t sendbuf[2] = {0x01,0x00};
    uint8_t recvbuf[2] = {1};
    spi_transaction_t t;
    memset(&t, 0, sizeof(t));

    //Initialize the SPI bus
    ret=spi_bus_initialize(SPI2_HOST, &buscfg, SPI_DMA_CH_AUTO);
    //Attach the LCD to the SPI bus
    printf("Here!\n");
    assert(ret==ESP_OK);
    ret=spi_bus_add_device(SPI2_HOST, &devcfg, &spi);
    assert(ret==ESP_OK);
    printf("Here 2!\n");
    //lcd_data(spi, lcd_init_cmds[cmd].data, lcd_init_cmds[cmd].databytes&0x1F);
    while(1)
    {
        vTaskDelay(3000 / portTICK_RATE_MS);
        t.length=16;
        t.tx_buffer=sendbuf;
        t.rx_buffer=recvbuf;
        //Wait for slave to be ready for next byte before sending
        ret=spi_device_polling_transmit(spi, &t);
        printf("Received: %d\n", recvbuf[0]);
        n++;
    }
}
