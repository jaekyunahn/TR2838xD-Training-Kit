/*
 * bsp_rtos.c
 *
 *  Created on: 2024. 3. 22.
 *      Author: ajg10
 */
#include "main.h"

// Main Thread Stack Size
#define MAIN_STACK_SIZE 3072U
// cmd Thread Stack Size
#define CMD_STACK_SIZE  2048U
// Led Thread Stack Size
#define LED_STACK_SIZE  256U
#define NULL_PARAM      0x00000000

#define AUTO_LOGIN  1

static SemaphoreHandle_t xSemaphore = NULL;

static StaticTask_t main_taskBuffer;
static StackType_t  main_taskStack[MAIN_STACK_SIZE];
static StaticTask_t cmd_taskBuffer;
static StackType_t  cmd_taskStack[CMD_STACK_SIZE];
static StaticTask_t blinkLED_taskBuffer;
static StackType_t  blinkLED_taskStack[LED_STACK_SIZE];

TaskHandle_t main_taskHandle;

// For debugging and checking the operation of the Main Thread
Uint16 main_task_counter = 0;
uint16_t mainTaskStackSize = 0;

Uint16 BlinkLED_task_counter = 0;
Uint16 cmd_task_counter = 0;

/**
 * @brief Timer ISR function.
 *
 * This function is called when the timer ISR occurs. It gives the semaphore
 * and yields from ISR if a higher priority task was woken.
 */
void rtos_timer_isr_function(void) {
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    xSemaphoreGiveFromISR(xSemaphore, &xHigherPriorityTaskWoken);
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

/**
 * @brief Initializes FreeRTOS.
 *
 * This function initializes FreeRTOS, sets up the CPU timer, and starts
 * the threads.
 */
void init_freeRTOS(void) {
    UARTprintf("start OS kernel\n");
    // FreeRTOS using CPU Timer 3
    init_cpuTimer();
    // Create and start threads
    init_thread();
}

/**
 * @brief Hook function for stack overflow.
 *
 * This function is called if a stack overflow occurs in a task.
 *
 * @param xTask Task handle
 * @param pcTaskName Task name
 */
void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName) {
    UARTprintf("Application StackOverflow!\n");
    while(1);
}

/**
 * @brief Initializes and starts threads.
 *
 * This function creates the main, command, and LED blink threads,
 * and starts the FreeRTOS scheduler.
 */
void init_thread(void) {
    main_taskHandle = xTaskCreateStatic(
        main_task,                  // Function that implements the task
        "main_task",                // Task name
        MAIN_STACK_SIZE,            // Stack size
        (void *)NULL_PARAM,         // Parameter passed into the task
        tskIDLE_PRIORITY + 2,       // Task priority
        main_taskStack,             // Array to use as the task's stack
        &main_taskBuffer            // Variable to hold the task's data structure
    );

    xTaskCreateStatic(
        cmd_task,                   // Function that implements the task
        "cmd_task",                 // Task name
        CMD_STACK_SIZE,             // Stack size
        (void *)NULL_PARAM,         // Parameter passed into the task
        tskIDLE_PRIORITY + 2,       // Task priority
        cmd_taskStack,              // Array to use as the task's stack
        &cmd_taskBuffer             // Variable to hold the task's data structure
    );

    xTaskCreateStatic(
        BlinkLED_task,              // Function that implements the task
        "BlinkLED_task",            // Task name
        LED_STACK_SIZE,             // Stack size
        (void *)NULL_PARAM,         // Parameter passed into the task
        tskIDLE_PRIORITY + 2,       // Task priority
        blinkLED_taskStack,         // Array to use as the task's stack
        &blinkLED_taskBuffer        // Variable to hold the task's data structure
    );

    // Start scheduler
    vTaskStartScheduler();
}

/**
 * @brief Main application thread.
 *
 * This thread is the main application thread that performs periodic tasks.
 *
 * @param pvParameters Task parameters
 */
void main_task(void *pvParameters) {
    TickType_t xLastWakeTime;
    const TickType_t xFrequency = 1;

    while(1) {
        bin();
        main_task_counter++;
        mainTaskStackSize = uxTaskGetStackHighWaterMark(main_taskHandle);
        vTaskDelayUntil(&xLastWakeTime, xFrequency);
    }
}

/**
 * @brief Command process thread.
 *
 * This thread handles command processing, with an optional auto-login feature.
 *
 * @param pvParameters Task parameters
 */
void cmd_task(void *pvParameters) {
    TickType_t xLastWakeTime;
    const TickType_t xFrequency = 100;

#if AUTO_LOGIN
    autoLogin();
#else
    int res = -1;
    while(1) {
        res = login();
        if (res == 0) {
            break;
        }
    }
#endif

    while(1) {
        cmd();
        cmd_task_counter++;
        vTaskDelayUntil(&xLastWakeTime, xFrequency);
    }
}

/**
 * @brief LED blink thread.
 *
 * This thread handles LED blinking.
 *
 * @param pvParameters Task parameters
 */
void BlinkLED_task(void *pvParameters) {
    TickType_t xLastWakeTime;
    const TickType_t xFrequency = 1000;

    while(1) {
        bsp_led_blink();
        coolingSystem();
        BlinkLED_task_counter++;
        vTaskDelayUntil(&xLastWakeTime, xFrequency);
    }
}

// Specifies that Flash programs should be processed from RAM
#ifdef _FLASH
#pragma CODE_SECTION(main_task, ".TI.ramfunc");
#pragma CODE_SECTION(BlinkLED_task, ".TI.ramfunc");
#pragma CODE_SECTION(cmd_task, ".TI.ramfunc");
#endif
