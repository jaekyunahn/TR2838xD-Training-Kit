/**
 * @file bsp_cla.c
 * @brief CLA initialization and interrupt service routines
 *
 * Created on: 2024. 2. 4.
 * Author: ajg10
 */

#include "main.h"

#define myCLA0_BASE CLA1_BASE
#define INT_myCLA01 INT_CLA1_1
#define INT_myCLA01_INTERRUPT_ACK_GROUP INTERRUPT_ACK_GROUP11

/** @brief Structure for data from CPU to CLA */
struct CPU_CLA_DATA cpu_to_cla;

/** @brief Structure for data from CLA to CPU */
struct CLA_CPU_DATA cla_to_cpu;

#pragma DATA_SECTION(cpu_to_cla, "CpuToCla1MsgRAM");
#pragma DATA_SECTION(cla_to_cpu, "Cla1ToCpuMsgRAM");

uint32_t Cla1funcsRunStart, Cla1funcsLoadStart, Cla1funcsLoadSize;
uint32_t Cla1ConstRunStart, Cla1ConstLoadStart, Cla1ConstLoadSize;

/**
 * @brief Initialize CLA
 *
 * This function sets up the CLA by configuring the program and data memory,
 * initializing message RAMs, and setting up task vectors and interrupts.
 */
void init_cla(void) {
    // The following code sets up the program and data memory for the CLA.
    // Note: Once the CLA program memory is configured, the CPU cannot access it.
    EALLOW;

#ifdef _FLASH
    // Copy the program and constants from FLASH to RAM before configuring the CLA
    memcpy((uint32_t *)&Cla1funcsRunStart, (uint32_t *)&Cla1funcsLoadStart, (uint32_t)&Cla1funcsLoadSize );
    memcpy((uint32_t *)&Cla1ConstRunStart, (uint32_t *)&Cla1ConstLoadStart, (uint32_t)&Cla1ConstLoadSize );
#endif

    // Initialize and wait for CLA1ToCPUMsgRAM
    MemCfgRegs.MSGxINIT.bit.INIT_CLA1TOCPU = 1;
    while(MemCfgRegs.MSGxINITDONE.bit.INITDONE_CLA1TOCPU != 1){};

    // Initialize and wait for CPUToCLA1MsgRAM
    MemCfgRegs.MSGxINIT.bit.INIT_CPUTOCLA1 = 1;
    while(MemCfgRegs.MSGxINITDONE.bit.INITDONE_CPUTOCLA1 != 1){};

    // Configure LSxRAM as program and data space for the CLA
    MemCfgRegs.LSxMSEL.bit.MSEL_LS4 = 1;
    MemCfgRegs.LSxMSEL.bit.MSEL_LS5 = 1;
    MemCfgRegs.LSxCLAPGM.bit.CLAPGM_LS4 = 1;
    MemCfgRegs.LSxCLAPGM.bit.CLAPGM_LS5 = 1;

    MemCfgRegs.LSxMSEL.bit.MSEL_LS6 = 1;
    MemCfgRegs.LSxMSEL.bit.MSEL_LS7 = 1;
    MemCfgRegs.LSxCLAPGM.bit.CLAPGM_LS6 = 0;
    MemCfgRegs.LSxCLAPGM.bit.CLAPGM_LS7 = 0;

    // Set CLA task vectors
#pragma diag_suppress=770
    Cla1Regs.MVECT1 = (uint16_t)(&Cla1Task1);
    //Cla1Regs.MVECT2 = (uint16_t)(&Cla1Task2);
    //Cla1Regs.MVECT3 = (uint16_t)(&Cla1Task3);
    //Cla1Regs.MVECT4 = (uint16_t)(&Cla1Task4);
    //Cla1Regs.MVECT5 = (uint16_t)(&Cla1Task5);
    //Cla1Regs.MVECT6 = (uint16_t)(&Cla1Task6);
    //Cla1Regs.MVECT7 = (uint16_t)(&Cla1Task7);
    //Cla1Regs.MVECT8 = (uint16_t)(&Cla1Task8);
#pragma diag_warning=770

    // Enable IACK instruction to start CLA tasks by software
    Cla1Regs.MCTL.bit.IACKE = 1;

    // Set trigger sources for CLA tasks
    // Setting trigger source to none, tasks will be started by software only
    DmaClaSrcSelRegs.CLA1TASKSRCSEL1.bit.TASK1 = CLA_TRIG_NOPERPH;
    //DmaClaSrcSelRegs.CLA1TASKSRCSEL1.bit.TASK2 = CLA_TRIG_NOPERPH;
    //DmaClaSrcSelRegs.CLA1TASKSRCSEL1.bit.TASK3 = CLA_TRIG_NOPERPH;
    //DmaClaSrcSelRegs.CLA1TASKSRCSEL1.bit.TASK4 = CLA_TRIG_NOPERPH;
    //DmaClaSrcSelRegs.CLA1TASKSRCSEL2.bit.TASK5 = CLA_TRIG_NOPERPH;
    //DmaClaSrcSelRegs.CLA1TASKSRCSEL2.bit.TASK6 = CLA_TRIG_NOPERPH;
    //DmaClaSrcSelRegs.CLA1TASKSRCSEL2.bit.TASK7 = CLA_TRIG_NOPERPH;
    //DmaClaSrcSelRegs.CLA1TASKSRCSEL2.bit.TASK8 = CLA_TRIG_NOPERPH;

    // Enable/Disable CLA tasks
    Cla1Regs.MIER.bit.INT1 = CLA_INT_ENABLE;    // Enable Task1
    Cla1Regs.MIER.bit.INT2 = CLA_INT_DISABLE;   // Disable Task2
    Cla1Regs.MIER.bit.INT3 = CLA_INT_DISABLE;   // Disable Task3
    Cla1Regs.MIER.bit.INT4 = CLA_INT_DISABLE;   // Disable Task4
    Cla1Regs.MIER.bit.INT5 = CLA_INT_DISABLE;   // Disable Task5
    Cla1Regs.MIER.bit.INT6 = CLA_INT_DISABLE;   // Disable Task6
    Cla1Regs.MIER.bit.INT7 = CLA_INT_DISABLE;   // Disable Task7
    Cla1Regs.MIER.bit.INT8 = CLA_INT_DISABLE;   // Disable Task8

    // Vector remapping for CLA interrupts
    PieVectTable.CLA1_1_INT = &cla1Isr1;
    //PieVectTable.CLA1_2_INT = &cla1Isr2;
    //PieVectTable.CLA1_3_INT = &cla1Isr3;
    //PieVectTable.CLA1_4_INT = &cla1Isr4;
    //PieVectTable.CLA1_5_INT = &cla1Isr5;
    //PieVectTable.CLA1_6_INT = &cla1Isr6;
    //PieVectTable.CLA1_7_INT = &cla1Isr7;
    //PieVectTable.CLA1_8_INT = &cla1Isr8;

    PieCtrlRegs.PIEIER11.bit.INTx1 = 1;  // Enable INT 11.1 in the PIE (CLA Task1)
    //PieCtrlRegs.PIEIER11.bit.INTx2 = 1;  // Enable INT 11.2 in the PIE
    //PieCtrlRegs.PIEIER11.bit.INTx3 = 1;  // Enable INT 11.3 in the PIE
    //PieCtrlRegs.PIEIER11.bit.INTx4 = 1;  // Enable INT 11.4 in the PIE
    //PieCtrlRegs.PIEIER11.bit.INTx5 = 1;  // Enable INT 11.5 in the PIE
    //PieCtrlRegs.PIEIER11.bit.INTx6 = 1;  // Enable INT 11.6 in the PIE
    //PieCtrlRegs.PIEIER11.bit.INTx7 = 1;  // Enable INT 11.7 in the PIE
    //PieCtrlRegs.PIEIER11.bit.INTx8 = 1;  // Enable INT 11.8 in the PIE

    IER |= M_INT11;  // Enable CPU Interrupt 11

    EDIS;
}

/**
 * @brief CLA Task 1 ISR
 *
 * This interrupt service routine is called when CLA Task 1 completes.
 */
__interrupt void cla1Isr1 (void) {
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP11;  // Acknowledge interrupt to PIE
}

/**
 * @brief CLA Task 2 ISR
 *
 * This interrupt service routine is called when CLA Task 2 completes.
 */
__interrupt void cla1Isr2 (void) {
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP11;  // Acknowledge interrupt to PIE
}

/**
 * @brief CLA Task 3 ISR
 *
 * This interrupt service routine is called when CLA Task 3 completes.
 */
__interrupt void cla1Isr3 (void) {
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP11;  // Acknowledge interrupt to PIE
}

/**
 * @brief CLA Task 4 ISR
 *
 * This interrupt service routine is called when CLA Task 4 completes.
 */
__interrupt void cla1Isr4 (void) {
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP11;  // Acknowledge interrupt to PIE
}

/**
 * @brief CLA Task 5 ISR
 *
 * This interrupt service routine is called when CLA Task 5 completes.
 */
__interrupt void cla1Isr5 (void) {
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP11;  // Acknowledge interrupt to PIE
}

/**
 * @brief CLA Task 6 ISR
 *
 * This interrupt service routine is called when CLA Task 6 completes.
 */
__interrupt void cla1Isr6 (void) {
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP11;  // Acknowledge interrupt to PIE
}

/**
 * @brief CLA Task 7 ISR
 *
 * This interrupt service routine is called when CLA Task 7 completes.
 */
__interrupt void cla1Isr7 (void) {
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP11;  // Acknowledge interrupt to PIE
}

/**
 * @brief CLA Task 8 ISR
 *
 * This interrupt service routine is called when CLA Task 8 completes.
 */
__interrupt void cla1Isr8 (void) {
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP11;  // Acknowledge interrupt to PIE
}

// End of file
