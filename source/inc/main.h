/*
 * main.h
 *
 *  Created on: 2023. 3. 18.
 *      Author: ajg10
 */

#ifndef USER_INC_MAIN_H_
#define USER_INC_MAIN_H_

//  Compiler Default Header File
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>

//  TI Header File
#include "f28x_project.h"
#include "device.h"
#include "F021_F2838x_C28x.h"

//  FreeRTOS
#include "cmd.h"
#include "bin.h"
#include "user.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

//  User Header File
#include "_define.h"
#include "_init_app.h"
#include "_sequence.h"
#include "_interrupt.h"
#include "_init_board.h"

#include "uartstdio.h"
#include "ustdlib.h"

//  third_party
#include "ff.h"
#include "ffconf.h"
#include "diskio.h"
#include "tms320f2837x_mmc_driver.h"

//  CLA
#include "cla_app.h"

//  User lib
#include "ai_lib.h"

//  User include
#include "bsp_spi.h"
#include "bsp_led.h"
#include "bsp_cla.h"
#include "bsp_sci.h"
#include "bsp_ipc.h"
#include "bsp_gpio.h"
#include "bsp_rtos.h"
#include "bsp_cpuTimer.h"
#include "bsp_filesystem.h"
#include "bsp_flash.h"

#include "app_ai.h"
#include "app_function.h"
#include "app_LED_Driver.h"
#include "app_cpu2_thread.h"
#include "app_multicoreTest.h"
#include "app_FirmwareUpdate.h"

#endif /* USER_INC_MAIN_H_ */
