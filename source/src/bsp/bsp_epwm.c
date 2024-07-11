/*
 * bsp_epwm.c
 *
 *  Created on: 2024. 7. 10.
 *      Author: User
 */
#include "main.h"

#define period  2000 // 50kHz
void init_epwm(void)
{
    CpuSysRegs.PCLKCR2.bit.EPWM1=1;

    EALLOW;
    CpuSysRegs.PCLKCR0.bit.TBCLKSYNC = 0;
    EDIS;

    //  init of ePWM1A
    setup_epwm();

    EALLOW;
    CpuSysRegs.PCLKCR0.bit.TBCLKSYNC = 1;
    EDIS;

#if 0
    EALLOW;
    PieVectTable.EPWM1_INT = &EPwm1Isr;  // EPWM1_INT 인터럽트 벡터에 EPwmIsr( ) 함수 연결
    EDIS;

    PieCtrlRegs.PIEIER3.bit.INTx1 = 1;  // CPU 인터럽트 3번에서 확장된 주변회로 인터럽트 벡터 그룹에서 1번 벡터 활성화 (EPWM1_INT)
    IER |= M_INT3;
#endif
}

void setup_epwm(void)
{
    EALLOW;

    //  Disable internal pull-up for the selected output pins for reduced power consumption. Pull-ups can be enabled or disabled by the user.
    //  Comment out other unwanted lines.
    GpioCtrlRegs.GPAPUD.bit.GPIO0 = 1;   // Disable pull-up on GPIO0 (EPWM1A)
    GpioCtrlRegs.GPAPUD.bit.GPIO1 = 1;   // Disable pull-up on GPIO1 (EPWM1B)

    //  Configure EPWM-1 pins using GPIO regs. This specifies which of the possible GPIO pins will be EPWM1 functional pins.
    //  Comment out other unwanted lines.
    GpioCtrlRegs.GPAMUX1.bit.GPIO0 = 1;   // Configure GPIO0 as EPWM1A
    GpioCtrlRegs.GPAMUX1.bit.GPIO1 = 1;   // Configure GPIO1 as EPWM1B

    EDIS;

    //  Setup TBCLK
    EPwm1Regs.TBPRD = period;       // Set timer period 801 TBCLKs
    EPwm1Regs.TBPHS.bit.TBPHS = 0x0000;        // Phase is 0
    EPwm1Regs.TBCTR = 0x0000;                  // Clear counter

    //  Setup counter mode
    EPwm1Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN; // Count up and down
    EPwm1Regs.TBCTL.bit.PHSEN = TB_DISABLE;        // Disable phase loading
    EPwm1Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;       // Clock ratio to SYSCLKOUT
    EPwm1Regs.TBCTL.bit.CLKDIV = TB_DIV1;

    //  Setup shadowing
    EPwm1Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
    EPwm1Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
    EPwm1Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO; // Load on Zero
    EPwm1Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;

    //  Set actions
    EPwm1Regs.AQCTLA.bit.CAU = AQ_SET;            // Set PWM1A on event A, up count
    EPwm1Regs.AQCTLA.bit.CAD = AQ_CLEAR;          // Clear PWM1A on event A, down count
    EPwm1Regs.AQCTLB.bit.CBU = AQ_SET;            // Set PWM1B on event B, up count
    EPwm1Regs.AQCTLB.bit.CBD = AQ_CLEAR;          // Clear PWM1B on event B, down count

#if 0
    //  Interrupt where we will change the Compare Values
    EPwm1Regs.ETSEL.bit.INTSEL = ET_CTR_ZERO;     // Select INT on Zero event
    EPwm1Regs.ETSEL.bit.INTEN = 1;                // Enable INT
    EPwm1Regs.ETPS.bit.INTPRD = 1;           // Generate INT on 3rd event
#endif

    //  Duty 50% Start
    EPwm1Regs.CMPA.bit.CMPA = period / 2;
}
