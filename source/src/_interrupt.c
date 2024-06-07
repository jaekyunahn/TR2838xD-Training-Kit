/*
 * interrupt.c
 *
 *  Created on: 2024. 2. 3.
 *      Author: ajg10
 */
#include "main.h"

//  PWM interrupt
//  EPWM1 모듈 인터럽트 서비스 루틴
__interrupt void EPwm1Isr(void) {
    //  To do
    control_loop();
    //  EPWM1 모듈의 인터럽트 플래그 비트 클리어
    EPwm1Regs.ETCLR.bit.INT = 1;
    //  EPWM1 인터럽트 벡터가 포함된 CPU 인터럽트 확장그룹 3번의 Acknowledge 비트 클리어
    PieCtrlRegs.PIEACK.bit.ACK3 = 1;
}


//
//  End file
//
