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
void main(void) {
    //  Initialize Board
    init_board();

    //  Initialize Application or Variable
    init_application();

#ifdef CPU1
    //  RTOS kernel Initialize
    init_freeRTOS();
#endif

    //  Start Main loop
    while(1) {
        main_loop();
    }
}
