/*
 * cmd.h
 *
 *  Created on: 2024. 5. 6.
 *      Author: User
 */

#ifndef KERNEL_INC_CMD_H_
#define KERNEL_INC_CMD_H_

//! Defines the value that is returned if the command is not found.
#define CMDLINE_BAD_CMD         (-1)
//! Defines the value that is returned if there are too many arguments.
#define CMDLINE_TOO_MANY_ARGS   (-2)
//! Defines the value that is returned if there are too few arguments.
#define CMDLINE_TOO_FEW_ARGS   (-3)
//! Defines the value that is returned if an argument is invalid.
#define CMDLINE_INVALID_ARG   (-4)
#define CMDLINE_MAX_ARGS    8
#define LIMIT_CMD_LEN       32

int string_macth(char *source, char *target, int size);
int login(void);
void cmd(void);
Uint32 CmdLineProcess(char *pcCmdLine);
int string_macth(char *source, char *target, int size);
void init_cmd_buffer(void);
void set_run_flag(Uint16 data);
void set_dir_address(char *address);
void get_now_dir(char *data);
void autoLogin(void);
int cmd_size(char *cmd);

#endif /* KERNEL_INC_CMD_H_ */
