/*
 * init_board.c
 *
 *  Created on: 2023. 3. 18.
 *      Author: ajg10
 */
#include "main.h"
/**
 * @brief initialize C28x Board
 */
void init_board(void) {
#ifdef CPU1
    //  CPU2 Using GSRAM6~9
    EALLOW;
    MemCfgRegs.GSxMSEL.bit.MSEL_GS6    = 1;
    MemCfgRegs.GSxMSEL.bit.MSEL_GS7    = 1;
    MemCfgRegs.GSxMSEL.bit.MSEL_GS8    = 1;
    MemCfgRegs.GSxMSEL.bit.MSEL_GS9    = 1;
    EDIS;
    //  Step 1. Initialize System Control:
    //  PLL, WatchDog, enable Peripheral Clocks
    //  This example function is found in the F2837xD_SysCtrl.c file.
    InitSysCtrl();
    //  CPU2 Boot
#ifdef _FLASH
    //  Flash Boot
    //  CPU2에 프로그램이 없으면 헛돌음.
    Device_bootCPU2(BOOTMODE_BOOT_TO_FLASH_SECTOR0);
#endif
    //  Step 2. Initialize GPIO:
    //  This example function is found in the F2837xD_Gpio.c file and
    //  illustrates how to set the GPIO to it's default state.
    InitGpio();
    //  CPU1 <-> CPU2 data communication
    init_ipc();
#endif
    //  CLA init
    init_cla();
    //  Step 3. Clear all interrupts and initialize PIE vector table:
    //  Disable CPU interrupts
    DINT;
    //  Initialize the PIE control registers to their default state.
    //  The default state is all PIE interrupts disabled and flags
    //  are cleared.
    //  This function is found in the F2837xD_PieCtrl.c file.
    InitPieCtrl();
    IER = 0x0000;   //  CPU 인터럽트 벡터 비 활성화
    IFR = 0x0000;   //  CPU 인터럽트 플래그 클리어
    //  Initialize the PIE vector table with pointers to the shell Interrupt
    //  Service Routines (ISR).
    //  This will populate the entire table, even if the interrupt
    //  is not used in this example.  This is useful for debug purposes.
    //  The shell ISR routines are found in F2837xD_DefaultIsr.c.
    //  This function is found in F2837xD_PieVect.c.
    InitPieVectTable();
    //  Initialize peripheral interface
#ifdef CPU1
    init_gpio();
    init_can();
    init_sci();
    init_spi();
    init_adc();
    init_epwm();
#endif
    //  Enable global Interrupts and higher priority real-time debug events:
    EINT;  // Enable Global interrupt INTM
    ERTM;  // Enable Global realtime interrupt DBGM
}

