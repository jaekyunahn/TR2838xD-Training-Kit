/*
 * _sequence.c
 *
 *  Created on: 2023. 3. 18.
 *      Author: ajg10
 */
#include "main.h"

Uint16 main_loop_counter = 0;
Uint16 control_loop_counter = 0;

void control_loop(void) {
    control_loop_counter++;
}

void main_loop(void) {
    main_loop_counter++;
#ifdef CPU2
    cpu2_thread();
#endif
}

#pragma CODE_SECTION(topProcess, ".TI.ramfunc");
Uint16 topProcess(void) {
    //  Not use
    return 0;
}

//
//  End file
//
