/*
 * user.h
 *
 *  Created on: 2024. 5. 6.
 *      Author: User
 */

#ifndef KERNEL_USER_H_
#define KERNEL_USER_H_

#define KERNEL_VERSION  "FreeRTOS V11.1.0"
#define FILESYSTEM_VERSION  "FATFS R0.15"

#define DEVICE  "TR2838xD-Training-Kit"
#define USER_NAME   "admin"

#define USER_0_ID "user"
#define USER_0_PW "1234"
#define USER_0_ID_LEN 4
#define USER_0_PW_LEN 4

#define DSP_NAME    "TMS320F28384D"
#define DSP_MIPS    "925MIPS"
#define DSP_SPEED   "C28:200MHz,CLA:200MHz,CortexM4:125MHz"
#define DSP_UNIT    "C28/CLA/FP64/TMU/VCRC/FINTDIV"
#define DSP_RAM     "338KB"
#define DSP_FLASH   "1536KB"
#define DSP_CORE    "C28(x2),CLA(x2),M4(x1)"

#endif /* KERNEL_USER_H_ */
