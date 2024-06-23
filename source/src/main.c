//###########################################################################
// FILE:    main.c
// TITLE:   TMS320F28384D program.
//###########################################################################
//  Release Date: 24.05.31
//###########################################################################
#include "main.h"

/**
 * @brief Main Function
 *
 * This function initializes the board and the application, and starts the main loop.
 * If compiled with CPU1, it also initializes the FreeRTOS kernel.
 */
uint32_t main(void) {
    //  Initialize Board
    init_board();
    //  Initialize Application or Variable
    init_application();
#ifdef CPU1
    #ifdef APP
    //  RTOS kernel Initialize
    init_freeRTOS();
    #else
    //  Bootloader Timer
    init_cpuTimer0();
    uint32_t jumpToAddress = 0x00000000;
    jumpToAddress = app_FWupdate();
    if(jumpToAddress == 0) {
        __asm("    ESTOP0");
    }
    return jumpToAddress;
    #endif
#endif
    //  Start Main loop
    while(1) {
        main_loop();
    }
    return 0;
}
