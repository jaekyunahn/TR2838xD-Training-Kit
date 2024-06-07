/**
 * @file cmd.c
 * @brief Command line interface for handling user input and executing commands.
 *
 * Created on: 2024. 5. 6.
 * Author: User
 */

#include "main.h"

// An array to hold the pointers to the command line arguments.
static char g_ppcArgv[CMDLINE_MAX_ARGS][LIMIT_CMD_LEN];
#pragma DATA_SECTION(g_ppcArgv, "ramgs0");
char g_cCmdBuf[CMD_BUF_SIZE];
#pragma DATA_SECTION(g_cCmdBuf, "ramgs0");
char g_cCwdBuf[PATH_BUF_SIZE] = "/";
#pragma DATA_SECTION(g_cCwdBuf, "ramgs0");
char id[32];
#pragma DATA_SECTION(id, "ramgs0");
char pw[32];
#pragma DATA_SECTION(pw, "ramgs0");

#define CMD_COUNT 9
const char cmd_table[CMD_COUNT][LIMIT_CMD_LEN] = {
    "help", "test", "reboot", "ls", "cd", "rm", "mkdir", "coretest", "ai"
};

/**
 * @brief Initialize command buffers.
 *
 * This function clears the command buffers used for storing command line input and current directory path.
 */
void init_cmd_buffer(void) {
    memset(g_cCmdBuf, 0x00, sizeof(g_cCmdBuf));
    memset(g_cCwdBuf, 0x00, sizeof(g_cCwdBuf));
    memset(id, 0x00, sizeof(id));
    memset(pw, 0x00, sizeof(pw));
    g_cCwdBuf[0] = '/';
}

// Application run flag, set to 0 when the application is not running.
Uint16 run_flag = 0;

/**
 * @brief Compare two strings for a specified size.
 *
 * @param source Source string to compare.
 * @param target Target string to compare.
 * @param size Number of characters to compare.
 * @return int 1 if strings match, -1 otherwise.
 */
int string_macth(char *source, char *target, int size) {
    int x = 0;
    int res = 1;
    for(x = 0 ; x < size ; x++) {
        if (source[x] != target[x]) {
            res = -1;
        }
    }
    return res;
}

/**
 * @brief Handle user login.
 *
 * This function prompts the user for an ID and password, and checks them against predefined values.
 * @return int 0 if login is successful, -1 otherwise.
 */
int login(void) {
    int res = 0;
    int rxIDsize = 0;
    int rxPWsize = 0;
    UARTprintf("####Login####\n");
ID_enter:
    UARTprintf("id:");
    rxIDsize = UARTgets(id, sizeof(id));
    if (rxIDsize == 0){
        UARTprintf("Please enter your ID\n");
        //  ID를 입력하지 않은 경우 다시 입력
        goto ID_enter;
    }
PW_enter:
    UARTprintf("pw:");
    rxPWsize = UARTPWgets(pw, sizeof(pw));
    if (rxPWsize == 0){
        UARTprintf("Please enter your PW\n");
        //  PW를 입력하지 않은 경우 다시 입력
        goto PW_enter;
    }
    //  ID / PW를 모두 일치 시켜야만 Login 허가
    res += string_macth(id, USER_0_ID, USER_0_ID_LEN);
    res += string_macth(pw, USER_0_PW, USER_0_PW_LEN);
    if (res == 2) {
        res = 0;
        scia_write('\n');
    }
    else {
        UARTprintf("\nLogin Fail\n\n");
        res = -1;
    }
    return res;
}

/**
 * @brief Execute the command line interface.
 *
 * This function processes user input and executes the corresponding command.
 */
void cmd(void) {
    Uint32 nStatus;
    if(run_flag == 0){
        UARTprintf("\033[32m%s\033[0m@%s:\033[96m%s\033[0m # ", id, DEVICE, g_cCwdBuf);
        UARTgets(g_cCmdBuf, sizeof(g_cCmdBuf));
        nStatus = CmdLineProcess(g_cCmdBuf);
        if(nStatus == (Uint32)CMDLINE_BAD_CMD) {
            UARTprintf("Bad command!\n");
        }
        else if(nStatus == (Uint32)CMDLINE_TOO_MANY_ARGS) {
            UARTprintf("Too many arguments for command processor!\n");
        }
        else if(nStatus != 0) {
            UARTprintf("Command returned error code %d\n", nStatus);
        }
    }
}

/**
 * @brief Process a command line string.
 *
 * This function parses a command line string into arguments and executes the corresponding command.
 * @param pcCmdLine Command line string to process.
 * @return Uint32 Status code indicating success or failure.
 */
Uint32 CmdLineProcess(char *pcCmdLine) {
    Uint16 i = 0;
    Uint16 char_index = 0;
    Uint16 cmd_find = 1;
    char *pcChar;
    uint_fast8_t ui8Argc;
    bool bFindArg = true;
    ui8Argc = 0;
    pcChar = pcCmdLine;
    //  이 부분은 실행 명령어를 분리하는 부분으로 ' ' , 즉 Space를 기준으로 파싱하는 구조로 되어있음
    while(*pcChar) {
        if(*pcChar == ' ') {
            *pcChar = 0;
            bFindArg = true;
        }
        else {
            if(bFindArg) {
                if(ui8Argc < CMDLINE_MAX_ARGS) {
                    memcpy(g_ppcArgv[ui8Argc], pcChar, sizeof(g_ppcArgv[ui8Argc]));
                    ui8Argc++;
                    bFindArg = false;
                }
                else {
                    return((Uint32)CMDLINE_TOO_MANY_ARGS);
                }
            }
        }
        pcChar++;
    }
    //  파싱한 명령어와 Arg데이터를 Main Task에 전달하는 부분
    if(ui8Argc) {
        for(i = 0 ; i < CMD_COUNT ; i++) {
            cmd_find = 1;
            for(char_index = 0; char_index < cmd_size(cmd_table[i]); char_index++) {
                if(g_ppcArgv[0][char_index] != cmd_table[i][char_index]) {
                    cmd_find = 0;
                }
            }
            //  등록한 명령어를 발견하면 run_flag를 1로 세팅하여 Main Task가 동작하도록 한다
            if(cmd_find == 1) {
                update_val(i, ui8Argc, g_ppcArgv);
                run_flag = 1;
                return 0;
            }
        }
    }
    return((Uint32)CMDLINE_BAD_CMD);
}

/**
 * @brief Set the run flag.
 *
 * This function sets the run flag to indicate whether the application is running.
 * @param data Value to set the run flag to.
 */
void set_run_flag(Uint16 data) {
    run_flag = data;
}

/**
 * @brief Set the current directory path.
 *
 * This function sets the current working directory path.
 * @param address Address of the new directory path.
 */
void set_dir_address(char *address) {
    sprintf(g_cCwdBuf, "%s", address);
}

/**
 * @brief Get the current directory path.
 *
 * This function gets the current working directory path.
 * @param data Buffer to store the current directory path.
 */
void get_now_dir(char *data) {
    sprintf(data, "%s", g_cCwdBuf);
}

/**
 * @brief Automatically login with default credentials.
 *
 * This function sets the ID to "admin" for automatic login.
 */
void autoLogin(void) {
    sprintf(id, "admin");
}

/**
 * @brief Get the size of a command string.
 *
 * This function returns the length of a command string.
 * @param cmd Command string to measure.
 * @return int Length of the command string.
 */
int cmd_size(char *cmd) {
    int i = 0;
    for(i = 0 ; i < LIMIT_CMD_LEN ; i++) {
        if(cmd[i] == 0x00) {
            return i;
        }
    }
    return 0;
}

// End of file
