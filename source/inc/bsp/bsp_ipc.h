/*
 * bsp_ipc.h
 *
 *  Created on: 2024. 5. 9.
 *      Author: ajg10
 */

#ifndef INC_BSP_BSP_IPC_H_
#define INC_BSP_BSP_IPC_H_

void init_ipc(void);
void clear_ipc_cpu2_to_cpu1(void);
void clear_ipc_cpu1_to_cpu2(void);
void share_cpu2_mode(void);
void share_cpu1_mode(void);
extern struct CPU_SHARE_DATA_MASTER_TO_SLAVE cpu1_to_cpu2;
extern struct CPU_SHARE_DATA_SLAVE_TO_MASTER cpu2_to_cpu1;
extern struct CPU_SHARE_DATA_buffer shareBuffer;

#endif /* INC_BSP_BSP_IPC_H_ */
