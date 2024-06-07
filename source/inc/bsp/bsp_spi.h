/*
 * bsp_spi.h
 *
 *  Created on: 2024. 3. 27.
 *      Author: ajg10
 */

#ifndef INC_BSP_BSP_SPI_H_
#define INC_BSP_BSP_SPI_H_

void init_spi(void);
void bsp_spi_gpio(void);

void init_spib(void);
Uint16 SpibDriver(Uint16 Data);

#endif /* INC_BSP_BSP_SPI_H_ */
