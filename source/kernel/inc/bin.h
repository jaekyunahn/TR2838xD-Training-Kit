/*
 * bin.h
 *
 *  Created on: 2024. 5. 6.
 *      Author: User
 */

#ifndef KERNEL_INC_BIN_H_
#define KERNEL_INC_BIN_H_

#include "cmd.h"

void bin(void);
void update_val(int index, int argc, char argv[CMDLINE_MAX_ARGS][LIMIT_CMD_LEN]);
void init_bin_buffer(void);
void Cmd_test(void);
void Cmd_help(void);
void Cmd_reset(void);
void Cmd_ls(void);
void Cmd_cd(void);
void Cmd_rm(void);
void Cmd_mkdir(void);
void Cmd_temp(void);

#endif /* KERNEL_INC_BIN_H_ */
