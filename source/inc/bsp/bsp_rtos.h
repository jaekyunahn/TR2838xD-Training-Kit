/*
 * bsp_rtos.h
 *
 *  Created on: 2024. 3. 22.
 *      Author: ajg10
 */

#ifndef INC_BSP_BSP_RTOS_H_
#define INC_BSP_BSP_RTOS_H_

void rtos_timer_isr_function(void);
void rtos_timer_isr_function(void);
void vApplicationStackOverflowHook( TaskHandle_t xTask, char *pcTaskName );
void init_thread(void);
void init_freeRTOS(void);
void main_task(void * pvParameters);
void cla_task(void * pvParameters);
void cmd_task(void * pvParameters);
void BlinkLED_task(void * pvParameters);

#endif /* INC_BSP_BSP_RTOS_H_ */
