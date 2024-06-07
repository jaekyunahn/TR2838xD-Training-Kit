/*
 * FreeRTOS Kernel V11.1.0
 * Copyright (C) 2021 Amazon.com, Inc. or its affiliates. All Rights Reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * https://www.FreeRTOS.org
 * https://github.com/FreeRTOS
 *
 */

/* Scheduler includes. */
#include "FreeRTOS.h"
#include "task.h"

/* Constants required for hardware setup.  The tick ISR runs off the ACLK, not the MCLK. */
/* 하드웨어 설정에 필요한 상수입니다. 틱 ISR은 MCLK가 아닌 ACLK에서 실행됩니다. */
//  기존 V10.x에서 가져옴.
#if 0
/*------------------------------------------------------------------------
 * Implementation of functions defined in portable.h for the MSP430X port.
 *-----------------------------------------------------------------------*/
#define portACLK_FREQUENCY_HZ           ( ( TickType_t ) 32768 )
#define portINITIAL_CRITICAL_NESTING    ( ( uint16_t ) 10 )
#define portFLAGS_INT_ENABLED           ( ( StackType_t ) 0x08 )
#else
//-------------------------------------------------------------------------------------------------
// Implementation of functions defined in portable.h for the C28x port.
//-------------------------------------------------------------------------------------------------
#define portINITIAL_CRITICAL_NESTING  ( ( uint16_t ) 10 )
#define portFLAGS_INT_ENABLED         ( ( StackType_t ) 0x08 )
#if defined(__TMS320C28XX_FPU64__)
# define AUX_REGISTERS_TO_SAVE        27 // XAR + FPU registers
# define XAR4_REGISTER_POSITION       6  // XAR4 position in AUX registers array
# define STF_REGISTER_POSITION        10 // STF position in AUX registers array
#elif defined(__TMS320C28XX_FPU32__)
# define AUX_REGISTERS_TO_SAVE        19 // XAR + FPU registers
# define XAR4_REGISTER_POSITION       6  // XAR4 position in AUX registers array
# define STF_REGISTER_POSITION        10 // STF position in AUX registers array
#else
# define AUX_REGISTERS_TO_SAVE        9  // XAR registers only
# define XAR4_REGISTER_POSITION       5  // XAR4 position in AUX registers array
#endif
#endif

//==========================================================================================================================

/* We require the address of the pxCurrentTCB variable, but don't want to know any details of its type. */
/* pxCurrentTCB 변수의 주소가 필요하지만 해당 유형에 대한 세부 정보를 알고 싶지 않습니다. */
typedef void TCB_t;
extern volatile TCB_t * volatile pxCurrentTCB;

/* Each task maintains a count of the critical section nesting depth.  Each time a critical section is entered the count is incremented.  
Each time a critical section is exited the count is decremented - with interrupts only being re-enabled if the count is zero. 
usCriticalNesting will get set to zero when the scheduler starts, but must not be initialised to zero as this will cause problems during the startup sequence. */
/* 각 작업은 임계 섹션 중첩 깊이의 개수를 유지합니다. 중요 섹션에 들어갈 때마다 개수가 증가합니다. 
중요 섹션이 종료될 때마다 카운트가 감소하며, 카운트가 0인 경우에만 인터럽트가 다시 활성화됩니다. 
usCriticalNesting은 스케줄러가 시작될 때 0으로 설정되지만 시작 시퀀스 중에 문제가 발생할 수 있으므로 0으로 초기화하면 안 됩니다. */
volatile uint16_t usCriticalNesting = portINITIAL_CRITICAL_NESTING;
/*-----------------------------------------------------------*/

/* Sets up the periodic ISR used for the RTOS tick.  This uses timer 0, but could have alternatively used the watchdog timer or timer 1. */
/* RTOS 틱에 사용되는 주기적 ISR을 설정합니다. 이는 타이머 0을 사용하지만, 대안으로 워치독 타이머나 타이머 1을 사용할 수도 있습니다. */
void vPortSetupTimerInterrupt( void );
/*-----------------------------------------------------------*/

//
//  추가한 변수 
// 

// Each task maintains a count of the critical section nesting depth.  Each
// time a critical section is entered the count is incremented.  Each time a
// critical section is exited the count is decremented - with interrupts only
// being re-enabled if the count is zero.
//
// ulCriticalNesting will get set to zero when the scheduler starts, but must
// not be initialised to zero as this will cause problems during the startup
// sequence.
// ulCriticalNesting should be 32 bit value to keep stack alignment unchanged.
volatile uint32_t ulCriticalNesting = portINITIAL_CRITICAL_NESTING;
volatile uint16_t bYield = 0;
volatile uint16_t bPreemptive = 0;

/* Initialise the stack of a task to look exactly as if a call to portSAVE_CONTEXT had been called. See the header file portable.h. */
/* portSAVE_CONTEXT에 대한 호출이 호출된 것처럼 정확하게 보이도록 작업 스택을 초기화합니다. 헤더 파일 Portable.h를 참조하세요. */
StackType_t *pxPortInitialiseStack( StackType_t *pxTopOfStack, TaskFunction_t pxCode, void *pvParameters )
{
    uint16_t i;
    uint16_t base = 0;

    pxTopOfStack[base++] = 0x0080;  // ST0. PSM = 0(No shift)
    pxTopOfStack[base++] = 0x0000;  // T
    pxTopOfStack[base++] = 0x0000;  // AL
    pxTopOfStack[base++] = 0x0000;  // AH
    pxTopOfStack[base++] = 0xFFFF;  // PL
    pxTopOfStack[base++] = 0xFFFF;  // PH
    pxTopOfStack[base++] = 0xFFFF;  // AR0
    pxTopOfStack[base++] = 0xFFFF;  // AR1
    pxTopOfStack[base++] = 0x8A08;  // ST1
    pxTopOfStack[base++] = 0x0000;  // DP
    pxTopOfStack[base++] = 0x0000;  // IER
    pxTopOfStack[base++] = 0x0000;  // DBGSTAT
    pxTopOfStack[base++] = ((uint32_t)pxCode) & 0xFFFFU;       // PCL
    pxTopOfStack[base++] = ((uint32_t)pxCode >> 16) & 0x00FFU; // PCH
    pxTopOfStack[base++] = 0xAAAA;  // Alignment
    pxTopOfStack[base++] = 0xBBBB;  // Alignment

    // Fill the rest of the registers with dummy values.
    for (i = 0; i < (2 * AUX_REGISTERS_TO_SAVE); i++)
    {
        uint16_t low = 0x0000;
        uint16_t high = 0x0000;

        if (i == (2 * XAR4_REGISTER_POSITION))
        {
            low = ((uint32_t)pvParameters) & 0xFFFFU;
            high = ((uint32_t)pvParameters >> 16) & 0xFFFFU;
        }
#if defined(__TMS320C28XX_FPU32__)
        if (i == (2 * STF_REGISTER_POSITION))
        {
            uint32_t stf = getSTF();

            low = stf & 0xFFFFU;
            high = (stf >> 16) & 0xFFFFU;
        }
#endif
        pxTopOfStack[base + i] = low;
        i++;
        pxTopOfStack[base + i] = high;
    }

    base += i;

    // Reserve place for ST1 which will be used in context switch
    // to set correct SPA bit ASAP.
    pxTopOfStack[base++] = 0x8A18;  // ST1 with SPA bit set
    pxTopOfStack[base++] = 0x0000;  // DP
    pxTopOfStack[base++] = 0x0000;  // placeholder for 32 bit ulCriticalNesting
    pxTopOfStack[base++] = 0x0000;

    // Return a pointer to the top of the stack we have generated so this can
    // be stored in the task control block for the task.
    return pxTopOfStack + base;
}
/*-----------------------------------------------------------*/

void vPortEndScheduler( void )
{
    /* It is unlikely that the MSP430 port will get stopped. If required simply disable the tick interrupt here. */
    /* MSP430 포트가 중지될 가능성은 거의 없습니다. 필요한 경우 여기에서 틱 인터럽트를 비활성화하면 됩니다. */
}
/*-----------------------------------------------------------*/

//  추가한 함수 -->
BaseType_t xPortStartScheduler(void)
{
    vPortSetupTimerInterrupt();
    ulCriticalNesting = 0;
#if(configUSE_PREEMPTION == 1)
    bPreemptive = 1;
#else
    bPreemptive = 0;
#endif
    portENABLE_INTERRUPTS();
    portRESTORE_FIRST_CONTEXT();
    //  Should not get here!
    return pdFAIL;
}

void vPortEnterCritical(void)
{
    portDISABLE_INTERRUPTS();
    ulCriticalNesting++;
}

void vPortExitCritical(void)
{
    ulCriticalNesting--;
    if (ulCriticalNesting == 0)
    {
        portENABLE_INTERRUPTS();
    }
}

void vApplicationSetupTimerInterrupt(void)
{
    // Initialize timer period:
    CPUTimer_setPeriod(CPUTIMER2_BASE, configCPU_CLOCK_HZ / configTICK_RATE_HZ);

    // Set pre-scale counter to divide by 1 (SYSCLKOUT):
    CPUTimer_setPreScaler(CPUTIMER2_BASE, 0);

    // Initializes timer control register. The timer is stopped, reloaded,
    // free run disabled, and interrupt enabled.
    // Additionally, the free and soft bits are set
    CPUTimer_stopTimer(CPUTIMER2_BASE);
    CPUTimer_reloadTimerCounter(CPUTIMER2_BASE);
    CPUTimer_setEmulationMode(CPUTIMER2_BASE, CPUTIMER_EMULATIONMODE_STOPAFTERNEXTDECREMENT);

    // Enable interrupt and start timer
    CPUTimer_enableInterrupt(CPUTIMER2_BASE);
    Interrupt_register(INT_TIMER2, &portTICK_ISR);
    Interrupt_enable(INT_TIMER2);
    CPUTimer_startTimer(CPUTIMER2_BASE);
}

//  <-- 추가한 함수

/* Hardware initialisation to generate the RTOS tick. */
/* RTOS 틱을 생성하기 위한 하드웨어 초기화. */
#pragma WEAK( vPortSetupTimerInterrupt )
void vPortSetupTimerInterrupt( void )
{
    vApplicationSetupTimerInterrupt();
}

/*-----------------------------------------------------------*/

#if 0
#pragma vector=configTICK_VECTOR
interrupt void vTickISREntry( void )
{
extern void vPortTickISR( void );

    __bic_SR_register_on_exit( SCG1 + SCG0 + OSCOFF + CPUOFF );
    #if configUSE_PREEMPTION == 1
        extern void vPortPreemptiveTickISR( void );
        vPortPreemptiveTickISR();
    #else
        extern void vPortCooperativeTickISR( void );
        vPortCooperativeTickISR();
    #endif
}
#endif

