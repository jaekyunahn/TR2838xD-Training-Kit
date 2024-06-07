/*
 * bsp_cpuTimer.h
 *
 *  Created on: 2024. 4. 2.
 *      Author: ajg10795
 */

#ifndef USER_INC_BSP_BSP_CPUTIMER_H_
#define USER_INC_BSP_BSP_CPUTIMER_H_

void init_cpuTimer(void);
void configCPUTimer(uint32_t cpuTimer, uint32_t timer_period);
__interrupt void timer1_ISR( void );

#endif /* USER_INC_BSP_BSP_CPUTIMER_H_ */
