/*
 * bsp_filesystem.h
 *
 *  Created on: 2024. 5. 11.
 *      Author: User
 */

#ifndef INC_BSP_BSP_FILESYSTEM_H_
#define INC_BSP_BSP_FILESYSTEM_H_

void test_fatfs(void);
FRESULT ChangeToDirectory(char *pcDirectory, char *now_dir, unsigned long *pulReason);
void init_fileSystem_buffer(void);
int16 read_file(Uint16 *address, Uint16 *buffer, Uint16 read_size, Uint16 index);

#endif /* INC_BSP_BSP_FILESYSTEM_H_ */
