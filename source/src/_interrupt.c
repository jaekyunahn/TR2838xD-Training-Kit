/*
 * interrupt.c
 *
 *  Created on: 2024. 2. 3.
 *      Author: ajg10
 */
#include "main.h"

//  PWM interrupt
//  EPWM1 ��� ���ͷ�Ʈ ���� ��ƾ
__interrupt void EPwm1Isr(void) {
    //  To do
    control_loop();
    //  EPWM1 ����� ���ͷ�Ʈ �÷��� ��Ʈ Ŭ����
    EPwm1Regs.ETCLR.bit.INT = 1;
    //  EPWM1 ���ͷ�Ʈ ���Ͱ� ���Ե� CPU ���ͷ�Ʈ Ȯ��׷� 3���� Acknowledge ��Ʈ Ŭ����
    PieCtrlRegs.PIEACK.bit.ACK3 = 1;
}


//
//  End file
//
