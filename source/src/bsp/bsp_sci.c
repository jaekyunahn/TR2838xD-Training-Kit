/*
 * bsp_sci.c
 *
 *  Created on: 2024. 5. 9.
 *      Author: ajg10
 */
#include "main.h"

#define BIT0        (1<<0)
#define BIT1        (1<<1)
#define BIT2        (1<<2)
#define BIT3        (1<<3)
#define BIT4        (1<<4)
#define BIT5        (1<<5)
#define BIT6        (1<<6)
#define BIT7        (1<<7)

#define CPU_CLK     200000000L          // 200MHz
#define LSP_CLK     (CPU_CLK/4)         // default at reset

#define BAUDRATE_A  115200L

/**
 * @brief Initializes the SCI interface.
 *
 * This function initializes the SCI-A interface.
 */
void init_sci(void) {
    init_scia();
}

/**
 * @brief Initializes the SCI-A interface.
 *
 * This function configures the necessary registers and settings
 * for SCI-A communication, including baud rate, control settings,
 * and enabling the SCI-A peripheral.
 */
void init_scia(void) {
    int32 i = 0;

    //  SCI-A to CPU1
    //  This register must be configured prior to enabling the peripheral clocks.
    EALLOW;
    DevCfgRegs.CPUSEL5.bit.SCI_A = 0;
    EDIS;

    // Enable SCI-A clock
    EALLOW;
    CpuSysRegs.PCLKCR7.bit.SCI_A = 1;
    EDIS;

    SciaRegs.SCICCR.all = 0x7;                   // 1 stop bit, no parity, 8-bit character, no loopback
    SciaRegs.SCICTL1.all = BIT1 | BIT0;          // Enable RX-ERR interrupt, TX and RX operation

    // Calculate and set the baud rate
    i = (int32)(((float32)LSP_CLK / ((float32)BAUDRATE_A * 8.0) - 1) + 0.5);
    SciaRegs.SCIHBAUD.all = i >> 8;
    SciaRegs.SCILBAUD.all = i & 0xFF;

    SciaRegs.SCICTL2.all = 0x0;                  // Enable RX/BK INT, TX INT
    SciaRegs.SCICTL1.all = BIT5 | BIT1 | BIT0;   // Relinquish SCI from Reset
}

/**
 * @brief Configures GPIO pins for SCI functionality.
 *
 * This function sets up the GPIO pins for SCI-A, configuring them
 * for input/output and setting their multiplexing options.
 */
void bsp_sci_gpio(void) {
    //  SCI_A
    GPIO_SetupPinMux(28, GPIO_MUX_CPU1, 1);
    GPIO_SetupPinOptions(28, GPIO_INPUT, GPIO_PUSHPULL);
    GPIO_SetupPinMux(29, GPIO_MUX_CPU1, 1);
    GPIO_SetupPinOptions(29, GPIO_OUTPUT, GPIO_ASYNC);
}

/**
 * @brief Transmits data via SCI-A.
 *
 * This function waits until the transmit buffer is empty,
 * then sends the provided data.
 *
 * @param data The data to be transmitted.
 */
#pragma CODE_SECTION(scia_write, ".TI.ramfunc");
void scia_write(Uint16 data) {
    while (SciaRegs.SCICTL2.bit.TXEMPTY == 0);
    SciaRegs.SCITXBUF.all = data;
}

/**
 * @brief Receives data via SCI-A.
 *
 * This function waits until data is received, then returns the received data.
 *
 * @return The received data.
 */
Uint16 scia_read(void) {
    Uint16 res = 0;
    while (SciaRegs.SCIRXST.bit.RXRDY == 0); // wait for data
    res = SciaRegs.SCIRXBUF.all;
    return res;
}

Uint16 scia_read_nowait(void) {
    Uint16 res = 0;
    if(SciaRegs.SCIRXST.bit.RXRDY == 1) {
        res = SciaRegs.SCIRXBUF.all;
    }
    return res;
}

