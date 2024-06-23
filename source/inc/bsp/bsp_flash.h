/*
 * bsp_flash.h
 *
 *  Created on: 2024. 6. 16.
 *      Author: User
 */

#ifndef INC_BSP_BSP_FLASH_H_
#define INC_BSP_BSP_FLASH_H_

void init_program_flash(void);
Uint16 erase_flash(Uint16 sectorNum);
Uint16 ProgramUsingAutoECC(Uint16 sectorNum, Uint16 *Buffer, Uint32 startIndex, Uint16 writeSize);
Uint16 ConvertAddressToSectorNumber(uint32_t address);

#endif /* INC_BSP_BSP_FLASH_H_ */
