/**
 * @file bsp_cpuTimer.c
 * @brief CPU Timer initialization and configuration
 *
 * Created on: 2024. 4. 2.
 * Author: ajg10795
 */

#include "main.h"

/**
 * @brief Initialize CPU Timer1
 *
 * This function registers the interrupt for CPU Timer1, sets the timer period,
 * prescaler, and starts the timer.
 */
void init_cpuTimer(void) {
    Interrupt_register(INT_TIMER1, &timer1_ISR);
    CPUTimer_setPeriod(CPUTIMER1_BASE, 0xFFFFFFFF);
    CPUTimer_setPreScaler(CPUTIMER1_BASE, 0);
    CPUTimer_stopTimer(CPUTIMER1_BASE);
    CPUTimer_reloadTimerCounter(CPUTIMER1_BASE);
    configCPUTimer(CPUTIMER1_BASE, 1000); // 1ms
    CPUTimer_enableInterrupt(CPUTIMER1_BASE);
    Interrupt_enable(INT_TIMER1);
    CPUTimer_startTimer(CPUTIMER1_BASE);
}

/**
 * @brief Configure the CPU Timer
 *
 * This function sets the period, prescaler, and emulation mode for the CPU Timer.
 *
 * @param cpuTimer The base address of the CPU Timer to be configured
 * @param timer_period The timer period in microseconds
 */
void configCPUTimer(uint32_t cpuTimer, uint32_t timer_period) {
    uint32_t temp, timer_freq = 200000000;
    temp = ((timer_freq / 1000000) * timer_period);
    CPUTimer_setPeriod(cpuTimer, temp);
    CPUTimer_setPreScaler(cpuTimer, 0);
    CPUTimer_stopTimer(cpuTimer);
    CPUTimer_reloadTimerCounter(cpuTimer);
    CPUTimer_setEmulationMode(cpuTimer, CPUTIMER_EMULATIONMODE_STOPAFTERNEXTDECREMENT);
    CPUTimer_enableInterrupt(cpuTimer);
}

/**
 * @brief Timer1 ISR
 *
 * This interrupt service routine is called when Timer1 overflows.
 */
__interrupt void timer1_ISR(void) {
    ai_tick_timer();
    rtos_timer_isr_function();
}