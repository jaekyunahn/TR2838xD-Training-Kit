/*
 * bsp_adc.h
 *
 *  Created on: 2024. 7. 10.
 *      Author: User
 */

#ifndef INC_BSP_BSP_ADC_H_
#define INC_BSP_BSP_ADC_H_

void init_adc(void);
void init_adca(void);
void init_adcc(void);
void init_adcd(void);
void readADC(void);
Uint16 getADCval(Uint16 caseNum);

#endif /* INC_BSP_BSP_ADC_H_ */
