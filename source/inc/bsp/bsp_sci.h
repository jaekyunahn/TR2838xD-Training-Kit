/*
 * bsp_sci.h
 *
 *  Created on: 2024. 5. 9.
 *      Author: ajg10
 */

#ifndef INC_BSP_BSP_SCI_H_
#define INC_BSP_BSP_SCI_H_

void init_sci(void);
void init_scia(void);
void bsp_sci_gpio(void);

Uint16 scia_read(void);
void scia_write(Uint16 data);
Uint16 scia_read_nowait(void);

#endif /* INC_BSP_BSP_SCI_H_ */
