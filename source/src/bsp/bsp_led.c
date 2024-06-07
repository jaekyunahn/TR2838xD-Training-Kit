/*
 * bsp_led.c
 *
 *  Created on: 2024. 5. 9.
 *      Author: ajg10
 */
#include "main.h"

/**
 * @brief Toggles the state of an LED.
 *
 * This function toggles the state of the LED connected to GPIO31.
 */
void bsp_led_blink(void) {
    static Uint16 led_control = 0;
    if (led_control == 0) {
        GpioDataRegs.GPASET.bit.GPIO31 = 1;
        led_control = 1;
    } else {
        GpioDataRegs.GPACLEAR.bit.GPIO31 = 1;
        led_control = 0;
    }
}

/**
 * @brief Sets up GPIO for LED blinking.
 *
 * This function configures GPIO31 for output and enables the pull-up resistor.
 */
void bsp_led_blink_set_gpio(void) {
    EALLOW; // Enable access to protected registers

    GpioCtrlRegs.GPAPUD.bit.GPIO31 = 0;   // Enable pull-up on GPIO31
    GpioDataRegs.GPASET.bit.GPIO31 = 1;   // Set GPIO31 output latch
    GpioCtrlRegs.GPAMUX2.bit.GPIO31 = 0;  // Configure GPIO31 as GPIO
    GpioCtrlRegs.GPADIR.bit.GPIO31 = 1;   // Set GPIO31 as output

    EDIS; // Disable access to protected registers
}
