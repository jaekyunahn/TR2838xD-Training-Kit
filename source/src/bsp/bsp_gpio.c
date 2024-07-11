/*
 * bsp_gpio.c
 *
 *  Created on: 2023. 3. 18.
 *      Author: ajg10
 */
#include "main.h"

/**
 * @brief GPIO MUX setting
 */
void init_gpio(void) {
    //  CPU ������ ��Ÿ���� ���� LED�� MUX ����
    bsp_led_blink_set_gpio();
    //  8���� �迭�� ������ LED MUX ������
    app_LED_Driver_set_gpio();
    //  SCI GPIO MUX ����
    bsp_sci_gpio();
    //  SPI GPIO MUX ����
    bsp_spi_gpio();
    //  CAN GPIO MUX ����
    bsp_can_gpio();
}

