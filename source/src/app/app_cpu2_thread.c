/*
 * app_cpu2_thread.c
 *
 *  Created on: 2024. 5. 9.
 *      Author: ajg10
 */
#include "main.h"

#ifdef CPU2

//  상태머신 flag
Uint16 step_flag = 0;

float32 cpu_res = 0.0;
Uint16 finshFlag_0 = 0;

float32 cla_res = 0.0;
Uint16 finshFlag_1 = 0;

void thread_core(void);
void thread_cla_start(void);
void thread_cla_end(void);

Uint16 cpu2thread_counter = 0;
void cpu2_thread(void) {
    cpu2thread_counter++;
    cpu2_to_cpu1.debug = cpu2thread_counter;
    switch(step_flag) {
    //  대기상태. 지속적으로 CPU1에서 날라온 data 감시
    case 0:
        //  set CLA data buffer
        if(cpu1_to_cpu2.flag == 1){
            cpu2_to_cpu1.flag = 0;
            memcpy(cpu_to_cla.input, shareBuffer.cla_input, sizeof(float32)*cpu1_to_cpu2.input_count);
            memcpy(cpu_to_cla.weight, shareBuffer.cla_weight, sizeof(float32)*cpu1_to_cpu2.input_count);
            cpu_to_cla.input_count = cpu1_to_cpu2.input_count;
            thread_cla_start();
            step_flag = 1;
        }
        break;
    case 1:
        thread_core();
        while(1){
            thread_cla_end();
            if((finshFlag_0 == 1) && (finshFlag_1 == 1)) {
                step_flag = 2;
                break;
            }
        }
        break;
    case 2:
        cpu2_to_cpu1.output[0] = cpu_res;
        cpu2_to_cpu1.output[1] = cla_res;
        cpu2_to_cpu1.flag = 1;
        step_flag = 0;
        break;
    }
}

void thread_cla_start(void) {
    //  CPU1.CLA start
    asm("  IACK  #0x0001");
    asm("  RPT #3 || NOP");
}

void thread_cla_end(void) {
    //  CPU1.CLA end
    if (Cla1Regs.MIRUN.bit.INT1 != 1) {
        cla_res = cla_to_cpu.output;
        finshFlag_1 = 1;
    }
}

void thread_core(void) {
    Uint32 counter = 0;
    float32 tmp = 0.0;
    for(counter = 0 ; counter < cpu1_to_cpu2.input_count ; counter++) {
        tmp = tmp + (shareBuffer.cpu_input[counter] * shareBuffer.cpu_weight[counter]);
    }
    cpu_res = tmp;
    finshFlag_0 = 1;
}

void send_flag_to_cpu1(void) {
    cpu2_to_cpu1.flag = 1;
}

#endif
