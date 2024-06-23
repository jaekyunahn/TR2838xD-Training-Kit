/*
 * init_app.c
 *
 *  Created on: 2023. 3. 18.
 *      Author: ajg10
 */
#include "main.h"

void init_application(void) {
#ifdef CPU1
    //  Flash Programing function initialize
    init_program_flash();
    //  LED Driver initialize
    init_app_LED_Driver();
    //  Initialize fileSystem array buffers
    init_fileSystem_buffer();

    #ifdef APP
    //  Initialize global array buffers
    init_bin_buffer();
    init_cmd_buffer();

    UARTprintf("\n");
    UARTprintf("------------HW info------------\n");
    UARTprintf("DEVICE:          \033[33m %s\033[0m\n",DEVICE);
    UARTprintf("CPU:             \033[33m %s\033[0m\n",DSP_NAME);
    UARTprintf("MIPS:            \033[33m %s\033[0m\n",DSP_MIPS);
    UARTprintf("Core Clock:      \033[33m %s\033[0m\n",DSP_SPEED);
    UARTprintf("Core UNIT:       \033[33m %s\033[0m\n",DSP_UNIT);
    UARTprintf("number of cores: \033[33m %s\033[0m\n",DSP_CORE);
    UARTprintf("RAM:             \033[33m %s\033[0m\n",DSP_RAM);
    UARTprintf("Flash:           \033[33m %s\033[0m\n",DSP_FLASH);
    scia_write('\n');
    UARTprintf("------------FW info------------\n");
    UARTprintf("OS Version:         \033[36m %s\033[0m\n",KERNEL_VERSION);
    UARTprintf("FileSystem Version: \033[36m %s\033[0m\n",FILESYSTEM_VERSION);
    scia_write('\n');
    #else
    UARTprintf("\033c");
    DELAY_US(100000);
    UARTprintf("Start Bootloader\n");
    #endif

#endif

    //  CPU <> CLA data initialize
    cpu_to_cla.output_case = 0;
    cpu_to_cla.input_count = 0;
    cpu_to_cla.bias = 0.0;
    cla_to_cpu.output = 0.0;
    memset(cpu_to_cla.input, 0.0, sizeof(cpu_to_cla.input));
    memset(cpu_to_cla.weight, 0.0, sizeof(cpu_to_cla.weight));

    //  Initialization of data exchange flag between CPU1 and CPU2
    clear_ipc_cpu1_to_cpu2();
    clear_ipc_cpu2_to_cpu1();
}

