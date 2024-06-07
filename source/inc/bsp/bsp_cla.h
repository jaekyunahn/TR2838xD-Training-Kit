/*
 * bsp_cla.h
 *
 *  Created on: 2024. 2. 4.
 *      Author: ajg10
 */

#ifndef USER_INC_BSP_BSP_CLA_H_
#define USER_INC_BSP_BSP_CLA_H_

extern uint32_t Cla1funcsRunStart, Cla1funcsLoadStart, Cla1funcsLoadSize;
extern uint32_t Cla1ConstRunStart, Cla1ConstLoadStart, Cla1ConstLoadSize;

void init_cla(void);

__interrupt void cla1Isr1 (void);
__interrupt void cla1Isr2 (void);
__interrupt void cla1Isr3 (void);
__interrupt void cla1Isr4 (void);
__interrupt void cla1Isr5 (void);
__interrupt void cla1Isr6 (void);
__interrupt void cla1Isr7 (void);
__interrupt void cla1Isr8 (void);

#endif /* USER_INC_BSP_BSP_CLA_H_ */
