/*
 * cla.h
 *
 *  Created on: 2023. 11. 10.
 *      Author: ajg10
 */

#ifndef CLA_CLA_APP_H_
#define CLA_CLA_APP_H_

extern struct CPU_CLA_DATA cpu_to_cla;
extern struct CLA_CPU_DATA cla_to_cpu;

//CLA C Tasks
__interrupt void Cla1Task1(void);
__interrupt void Cla1Task2(void);
__interrupt void Cla1Task3(void);
__interrupt void Cla1Task4(void);
__interrupt void Cla1Task5(void);
__interrupt void Cla1Task6(void);
__interrupt void Cla1Task7(void);
__interrupt void Cla1Task8(void);

#endif /* CLA_CLA_APP_H_ */
