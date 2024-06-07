/*
 * bsp_spi.c
 *
 *  Created on: 2024. 3. 27.
 *      Author: ajg10
 */
#include "main.h"

//#define 8MHZ    8000000
#define SPI_BRR        ((200000000 / 4) / 8000000) - 1

/**
 * @brief Initializes the SPI interface.
 *
 * This function initializes the SPI-B interface.
 */
void init_spi(void) {
    init_spib();
}

/**
 * @brief Configures GPIO pins for SPI functionality.
 *
 * This function sets up the GPIO pins for SPI-A by enabling pull-ups,
 * setting the pins to asynchronous mode, and configuring them for SPI functionality.
 */
void bsp_spi_gpio(void) {
    EALLOW;

    //
    // Enable internal pull-up for the selected pins
    // Pull-ups can be enabled or disabled by the user.
    // This will enable the pullups for the specified pins.
    //
    GpioCtrlRegs.GPAPUD.bit.GPIO24 = 0;  // Enable pull-up on GPIO24 (SPIPICOA)
    GpioCtrlRegs.GPAPUD.bit.GPIO25 = 0;  // Enable pull-up on GPIO25 (SPIPOCIA)
    GpioCtrlRegs.GPAPUD.bit.GPIO26 = 1;  // Enable pull-up on GPIO26 (SPICLKA)
    GpioCtrlRegs.GPAPUD.bit.GPIO27 = 0;  // Enable pull-up on GPIO27 (SPIPTEA)

    //
    // Set qualification for selected pins to asynch only
    // This will select asynch (no qualification) for the selected pins.
    //
    GpioCtrlRegs.GPAQSEL2.bit.GPIO24 = 3; // Asynch input GPIO24 (SPIPICOA)
    GpioCtrlRegs.GPAQSEL2.bit.GPIO25 = 3; // Asynch input GPIO25 (SPIPOCIA)
    GpioCtrlRegs.GPAQSEL2.bit.GPIO26 = 3; // Asynch input GPIO26 (SPICLKA)
    GpioCtrlRegs.GPAQSEL2.bit.GPIO27 = 3; // Asynch input GPIO27 (SPIPTEA)

    //
    // Configure SPI-A pins
    // This specifies which of the possible GPIO pins will be SPI functional pins.
    //
    GpioCtrlRegs.GPAMUX2.bit.GPIO24 = 2; // Configure GPIO24 as SPIPICOA
    GpioCtrlRegs.GPAMUX2.bit.GPIO25 = 2; // Configure GPIO25 as SPIPOCIA
    GpioCtrlRegs.GPAMUX2.bit.GPIO26 = 2; // Configure GPIO26 as SPICLKA
    GpioCtrlRegs.GPAMUX2.bit.GPIO27 = 2; // Configure GPIO27 as SPIPTEA

    //
    // Configure SPI-A pins
    // This specifies which of the possible GPIO pins will be SPI functional pins.
    //
    GpioCtrlRegs.GPAGMUX2.bit.GPIO24 = 1; // Configure GPIO24 as SPIPICOA
    GpioCtrlRegs.GPAGMUX2.bit.GPIO25 = 1; // Configure GPIO25 as SPIPOCIA
    GpioCtrlRegs.GPAGMUX2.bit.GPIO26 = 1; // Configure GPIO26 as SPICLKA
    GpioCtrlRegs.GPAGMUX2.bit.GPIO27 = 1; // Configure GPIO27 as SPIPTEA

    EDIS;
}

/**
 * @brief Initializes the SPI-B interface.
 *
 * This function initializes the core SPI-B registers, sets the baud rate, and configures
 * SPI-B for master mode, 8-bit character length, and other settings.
 */
void init_spib(void) {
    //
    // Initialize core SPI registers
    // Set reset low before configuration changes
    // Clock polarity (0 == rising, 1 == falling)
    // 8-bit character
    // Disable loop-back
    //
    SpibRegs.SPICCR.bit.SPISWRESET = 0;
    SpibRegs.SPICCR.bit.CLKPOLARITY = 1;
    SpibRegs.SPICCR.bit.SPICHAR = 7;
    SpibRegs.SPICCR.bit.SPILBK = 0;

    //
    // Enable master (0 == slave, 1 == master)
    // Enable transmission (Talk)
    // Clock phase (0 == normal, 1 == delayed)
    // SPI interrupts are disabled
    //
    SpibRegs.SPICTL.bit.MASTER_SLAVE = 1;
    SpibRegs.SPICTL.bit.TALK = 1;
    SpibRegs.SPICTL.bit.CLK_PHASE = 0;
    SpibRegs.SPICTL.bit.SPIINTENA = 0;

    // Set the baud rate
    SpibRegs.SPIBRR.bit.SPI_BIT_RATE = SPI_BRR;

    //
    // Set FREE bit
    // Halting on a breakpoint will not halt the SPI
    //
    SpibRegs.SPIPRI.bit.FREE = 1;

    // Release the SPI from reset
    SpibRegs.SPICCR.bit.SPISWRESET = 1;
}

/**
 * @brief SPI data transfer function.
 *
 * This function sends a 16-bit data word through the SPI-B interface and
 * returns the received data.
 *
 * @param Data The 16-bit data word to be transmitted.
 * @return The 16-bit data word received.
 */
Uint16 SpibDriver(Uint16 Data) {
    Uint16 Read, Temp;
    Temp = Data << 8;
    Temp = Temp & 0xFF00;
    while (SpibRegs.SPISTS.bit.BUFFULL_FLAG);
    SpibRegs.SPITXBUF = Temp;
    while(SpibRegs.SPISTS.bit.INT_FLAG !=1);
    Read = SpibRegs.SPIRXBUF;
    return Read;
}
