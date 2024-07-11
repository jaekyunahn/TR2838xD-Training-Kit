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
    //  CPU 동작을 나타내기 위한 LED의 MUX 설정
    bsp_led_blink_set_gpio();
    //  8개의 배열로 구성된 LED MUX 서렂ㅇ
    app_LED_Driver_set_gpio();
    //  SCI GPIO MUX 설정
    bsp_sci_gpio();
    //  SPI GPIO MUX 설정
    bsp_spi_gpio();
    //  CAN GPIO MUX 설정
    bsp_can_gpio();
}

