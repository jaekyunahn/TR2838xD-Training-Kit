/*
 * bsp_ipc.c
 *
 *  Created on: 2024. 5. 9.
 *      Author: ajg10
 */
#include "main.h"

//
//  공유 메모리. 직접 읽거나 쓰지말기.
//
struct CPU_SHARE_DATA_MASTER_TO_SLAVE cpu1_to_cpu2;
#pragma DATA_SECTION(cpu1_to_cpu2, "MSGRAM_CPU1TOCPU2");
struct CPU_SHARE_DATA_SLAVE_TO_MASTER cpu2_to_cpu1;
#pragma DATA_SECTION(cpu2_to_cpu1, "MSGRAM_CPU2TOCPU1");

struct CPU_SHARE_DATA_MASTER_TO_SLAVE cpu1_to_cm;
#pragma DATA_SECTION(cpu1_to_cm, "MSGRAM_CPU_TO_CM");
struct CPU_SHARE_DATA_SLAVE_TO_MASTER cm_to_cpu1;
#pragma DATA_SECTION(cm_to_cpu1, "MSGRAM_CM_TO_CPU");

struct CPU_SHARE_DATA_buffer shareBuffer;
#pragma DATA_SECTION(shareBuffer, "shareMem");

/**
 * @brief Initializes the IPC structures.
 *
 * This function initializes the IPC structures by setting their fields to default values.
 */
void init_ipc(void) {
    cpu1_to_cpu2.flag = 0;
    cpu1_to_cpu2.target = 0;
    cpu1_to_cpu2.out_case = 0;
    cpu1_to_cpu2.input_count = 0;
    memset(cpu1_to_cpu2.bias, 0, sizeof(cpu1_to_cpu2.bias));

    memset(shareBuffer.cpu_input, 0, sizeof(shareBuffer.cpu_input));
    memset(shareBuffer.cpu_weight, 0, sizeof(shareBuffer.cpu_weight));
    memset(shareBuffer.cla_input, 0, sizeof(shareBuffer.cla_input));
    memset(shareBuffer.cla_weight, 0, sizeof(shareBuffer.cla_weight));

    cpu2_to_cpu1.flag = 0;
    memset(cpu2_to_cpu1.output, 0, sizeof(cpu2_to_cpu1.output));
}

/**
 * @brief Clears the IPC flags for the specified core.
 *
 * @param from_core The core from which the IPC flags should be cleared.
 */
void clear_ipc(Uint16 from_core) {
    switch(from_core) {
        case CPU2_CPU:
            cpu1_to_cpu2.flag = 0;
            cpu2_to_cpu1.flag = 0;
            break;
        case CM:
            // Add handling for the Control Module if needed
            break;
    }
}

/**
 * @brief Clears the IPC flag from CPU1 to CPU2.
 */
void clear_ipc_cpu1_to_cpu2(void) {
    cpu1_to_cpu2.flag = 0;
}

/**
 * @brief Clears the IPC flag from CPU2 to CPU1.
 */
void clear_ipc_cpu2_to_cpu1(void) {
    cpu2_to_cpu1.flag = 0;
}

/**
 * @brief Sets the shared memory mode to CPU1.
 *
 * This function configures the shared memory sections to be accessed by CPU1.
 */
void share_cpu1_mode(void) {
    EALLOW;
    MemCfgRegs.GSxMSEL.bit.MSEL_GS10 = 0;
    MemCfgRegs.GSxMSEL.bit.MSEL_GS11 = 0;
    MemCfgRegs.GSxMSEL.bit.MSEL_GS12 = 0;
    MemCfgRegs.GSxMSEL.bit.MSEL_GS13 = 0;
    MemCfgRegs.GSxMSEL.bit.MSEL_GS14 = 0;
    MemCfgRegs.GSxMSEL.bit.MSEL_GS15 = 0;
    EDIS;
}

/**
 * @brief Sets the shared memory mode to CPU2.
 *
 * This function configures the shared memory sections to be accessed by CPU2.
 */
void share_cpu2_mode(void) {
    EALLOW;
    MemCfgRegs.GSxMSEL.bit.MSEL_GS10 = 1;
    MemCfgRegs.GSxMSEL.bit.MSEL_GS11 = 1;
    MemCfgRegs.GSxMSEL.bit.MSEL_GS12 = 1;
    MemCfgRegs.GSxMSEL.bit.MSEL_GS13 = 1;
    MemCfgRegs.GSxMSEL.bit.MSEL_GS14 = 1;
    MemCfgRegs.GSxMSEL.bit.MSEL_GS15 = 1;
    EDIS;
}
