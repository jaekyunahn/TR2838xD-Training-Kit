/*
 * app_LedBlink.c
 *
 *  Created on: 2024. 5. 9.
 *      Author: ajg10
 */
#include "main.h"

/**
 * @brief Controls the state of LEDs connected to GPIO pins.
 *
 * This function sets or clears the specified GPIO pin based on the
 * provided address and data. Each address corresponds to a specific
 * GPIO pin, and the data determines whether the pin is set (1) or cleared (0).
 *
 * @param address The GPIO pin address (0-7).
 * @param data The state to set the pin (0 for clear, 1 for set).
 */
//#pragma CODE_SECTION(app_LED_Driver, ".TI.ramfunc");
void app_LED_Driver(Uint16 address, Uint16 data) {
    switch(address) {
    case 0: if(data == 0) GpioDataRegs.GPBCLEAR.bit.GPIO32 = 1; else GpioDataRegs.GPBSET.bit.GPIO32 = 1; break;
    case 1: if(data == 0) GpioDataRegs.GPBCLEAR.bit.GPIO33 = 1; else GpioDataRegs.GPBSET.bit.GPIO33 = 1; break;
    case 2: if(data == 0) GpioDataRegs.GPBCLEAR.bit.GPIO34 = 1; else GpioDataRegs.GPBSET.bit.GPIO34 = 1; break;
    case 3: if(data == 0) GpioDataRegs.GPBCLEAR.bit.GPIO35 = 1; else GpioDataRegs.GPBSET.bit.GPIO35 = 1; break;
    case 4: if(data == 0) GpioDataRegs.GPBCLEAR.bit.GPIO36 = 1; else GpioDataRegs.GPBSET.bit.GPIO36 = 1; break;
    case 5: if(data == 0) GpioDataRegs.GPBCLEAR.bit.GPIO37 = 1; else GpioDataRegs.GPBSET.bit.GPIO37 = 1; break;
    case 6: if(data == 0) GpioDataRegs.GPBCLEAR.bit.GPIO38 = 1; else GpioDataRegs.GPBSET.bit.GPIO38 = 1; break;
    case 7: if(data == 0) GpioDataRegs.GPBCLEAR.bit.GPIO39 = 1; else GpioDataRegs.GPBSET.bit.GPIO39 = 1; break;
    }
}

/**
 * @brief Configures GPIO pins for LED control.
 *
 * This function sets up the GPIO pins (32-39) for LED control by disabling
 * pull-up resistors, setting the pins to high, configuring the multiplexer
 * to GPIO function, and setting the pins as outputs.
 */
void app_LED_Driver_set_gpio(void) {
    EALLOW; // Enable access to protected registers

    GpioCtrlRegs.GPBPUD.bit.GPIO32 = 0;
    GpioCtrlRegs.GPBPUD.bit.GPIO33 = 0;
    GpioCtrlRegs.GPBPUD.bit.GPIO34 = 0;
    GpioCtrlRegs.GPBPUD.bit.GPIO35 = 0;
    GpioCtrlRegs.GPBPUD.bit.GPIO36 = 0;
    GpioCtrlRegs.GPBPUD.bit.GPIO37 = 0;
    GpioCtrlRegs.GPBPUD.bit.GPIO38 = 0;
    GpioCtrlRegs.GPBPUD.bit.GPIO39 = 0;

    GpioDataRegs.GPBSET.bit.GPIO32 = 1;
    GpioDataRegs.GPBSET.bit.GPIO33 = 1;
    GpioDataRegs.GPBSET.bit.GPIO34 = 1;
    GpioDataRegs.GPBSET.bit.GPIO35 = 1;
    GpioDataRegs.GPBSET.bit.GPIO36 = 1;
    GpioDataRegs.GPBSET.bit.GPIO37 = 1;
    GpioDataRegs.GPBSET.bit.GPIO38 = 1;
    GpioDataRegs.GPBSET.bit.GPIO39 = 1;

    GpioCtrlRegs.GPBMUX1.bit.GPIO32 = 0;
    GpioCtrlRegs.GPBMUX1.bit.GPIO33 = 0;
    GpioCtrlRegs.GPBMUX1.bit.GPIO34 = 0;
    GpioCtrlRegs.GPBMUX1.bit.GPIO35 = 0;
    GpioCtrlRegs.GPBMUX1.bit.GPIO36 = 0;
    GpioCtrlRegs.GPBMUX1.bit.GPIO37 = 0;
    GpioCtrlRegs.GPBMUX1.bit.GPIO38 = 0;
    GpioCtrlRegs.GPBMUX1.bit.GPIO39 = 0;

    GpioCtrlRegs.GPBDIR.bit.GPIO32 = 1;
    GpioCtrlRegs.GPBDIR.bit.GPIO33 = 1;
    GpioCtrlRegs.GPBDIR.bit.GPIO34 = 1;
    GpioCtrlRegs.GPBDIR.bit.GPIO35 = 1;
    GpioCtrlRegs.GPBDIR.bit.GPIO36 = 1;
    GpioCtrlRegs.GPBDIR.bit.GPIO37 = 1;
    GpioCtrlRegs.GPBDIR.bit.GPIO38 = 1;
    GpioCtrlRegs.GPBDIR.bit.GPIO39 = 1;

    EDIS; // Disable access to protected registers
}

/**
 * @brief Initializes all LEDs to off state.
 *
 * This function turns off all LEDs connected to GPIO pins 32-39 by setting
 * their states to 0.
 */
void init_app_LED_Driver(void) {
    Uint16 i = 0;
    for(i = 0 ; i < 8 ; i++) {
        app_LED_Driver(i,0);
    }
}
