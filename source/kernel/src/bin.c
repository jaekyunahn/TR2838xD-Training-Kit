/**
 * @file bin.c
 * @brief File operations and command handling.
 *
 * Created on: 2024. 5. 6.
 * Author: User
 */

#include "main.h"

#define NUM_LIST_STRINGS        48
#define MAX_FILENAME_STRING_LEN (4 + 8 + 1 + 3 + 1)

//
// Storage for the names of the files in the current directory. Filenames
// are stored in format "(D) filename.ext" for directories or
// "(F) filename.ext" for files.
//
char g_pcFilenames[NUM_LIST_STRINGS][MAX_FILENAME_STRING_LEN];
#pragma DATA_SECTION(g_pcFilenames, "ramgs0");
char cmd_dir_buffer[PATH_BUF_SIZE] = "/";
#pragma DATA_SECTION(cmd_dir_buffer, "ramgs0");
char cmd_temp_buffer[PATH_BUF_SIZE];
#pragma DATA_SECTION(cmd_temp_buffer, "ramgs0")
char Gloval_argv[CMDLINE_MAX_ARGS][LIMIT_CMD_LEN];
#pragma DATA_SECTION(Gloval_argv, "ramgs0");

static FATFS g_sFatFs;
#pragma DATA_SECTION(g_sFatFs, "ramgs0");
static DIR g_sDirObject;
#pragma DATA_SECTION(g_sDirObject, "ramgs0");
static FILINFO g_sFileInfo;
#pragma DATA_SECTION(g_sFileInfo, "ramgs0");

int Gloval_argc = 0;
int Gloval_index = 0;
int Gloval_opflag = 0;
int mutexHandle;

/**
 * @brief Initialize command buffers.
 *
 * This function clears the command buffers used for storing directory and
 * temporary paths.
 */
void init_bin_buffer(void) {
    memset(cmd_dir_buffer, 0x00, sizeof(cmd_dir_buffer));
    memset(cmd_temp_buffer, 0x00, sizeof(cmd_temp_buffer));
}

/**
 * @brief Execute the command based on global flags and index.
 *
 * This function checks the global operation flag and executes the command
 * corresponding to the global index.
 */
void bin(void) {
    if(Gloval_opflag == 1) {
        switch(Gloval_index) {
            case 0: Cmd_help(); break;
            case 1: Cmd_test(); break;
            case 2: Cmd_reset(); break;
            case 3: Cmd_ls(); break;
            case 4: Cmd_cd(); break;
            case 5: Cmd_rm(); break;
            case 6: Cmd_mkdir(); break;
            case 7: app_core_test(); break;
            case 8: app_ai(); break;
            default:
                UARTprintf("fail commands\n");
                break;
        }
        Gloval_opflag = 0;
    }
}

/**
 * @brief Update global variables for command execution.
 *
 * @param index Index of the command to be executed.
 * @param argc Number of arguments.
 * @param argv Array of arguments.
 */
void update_val(int index, int argc, char argv[CMDLINE_MAX_ARGS][LIMIT_CMD_LEN]) {
    Uint16 x, y;
    for(x = 0 ; x < CMDLINE_MAX_ARGS ; x++ ) {
        for(y = 0 ; y < LIMIT_CMD_LEN ; y++ ) {
            Gloval_argv[x][y] = argv[x][y];
        }
    }
    Gloval_argc = argc;
    Gloval_index = index;
    Gloval_opflag = 1;
}

/**
 * @brief Display help information.
 *
 * This function prints the list of available commands.
 */
void Cmd_help(void) {
    // Print some header text.
    UARTprintf("----------------------------------------\n");
    UARTprintf("help        - commands list\n");
    UARTprintf("----------------------------------------\n");

    // Return success.
    set_run_flag(0);
    return;
}

/**
 * @brief Test command.
 *
 * This function prints a test message and tests FATFS operations.
 */
void Cmd_test(void) {
    // Print some header text.
    UARTprintf("test read sdcard\n");

    test_fatfs();

    // Return success.
    set_run_flag(0);
    return;
}

/**
 * @brief Reset the device.
 *
 * This function prints a reset message and resets the device.
 */
void Cmd_reset(void) {
    // Print some header text.
    UARTprintf("reboot Device\n\n");

    SysCtl_resetDevice();

    // Return success.
    set_run_flag(0);
    return;
}

/**
 * @brief List directory contents.
 *
 * This function lists the contents of the current directory.
 */
void Cmd_ls(void) {
    unsigned long ulTotalSize, ulItemCount, ulFileCount, ulDirCount;
    FRESULT fresult;
    FATFS *pFatFs;
    get_now_dir(cmd_dir_buffer);
    fresult = f_mount(&g_sFatFs, "",0);
    // Open the current directory for access.
    fresult = f_opendir(&g_sDirObject, cmd_dir_buffer);
    // Check for error and return if there is a problem.
    if(fresult != FR_OK) {
        // Fail
        UARTprintf("f_opendir fail\n");
        set_run_flag(0);
        return;
    }
    ulTotalSize = 0;
    ulFileCount = 0;
    ulDirCount = 0;
    ulItemCount = 0;
    // Give an extra blank line before the listing.
    UARTprintf("\n");
    // Enter loop to enumerate through all directory entries.
    for(;;) {
        // Read an entry from the directory.
        fresult = f_readdir(&g_sDirObject, &g_sFileInfo);
        // Check for error and return if there is a problem.
        if(fresult != FR_OK) {
            UARTprintf("f_readdir fail:%d\n", fresult);
            set_run_flag(0);
            return;
        }
        // If the file name is blank, then this is the end of the listing.
        if(!g_sFileInfo.fname[0]) {
            break;
        }
        // Print the entry information on a single line with formatting to show the attributes, date, time, size, and name.
        UARTprintf("%c%c%c%c%c %u/%02u/%02u %02u:%02u %9u  %s\n",
                   (g_sFileInfo.fattrib & AM_DIR) ? (uint32_t)'D' : (uint32_t)'-',
                   (g_sFileInfo.fattrib & AM_RDO) ? (uint32_t)'R' : (uint32_t)'-',
                   (g_sFileInfo.fattrib & AM_HID) ? (uint32_t)'H' : (uint32_t)'-',
                   (g_sFileInfo.fattrib & AM_SYS) ? (uint32_t)'S' : (uint32_t)'-',
                   (g_sFileInfo.fattrib & AM_ARC) ? (uint32_t)'A' : (uint32_t)'-',
                   (uint32_t)((g_sFileInfo.fdate >> 9) + 1980),
                   (uint32_t)((g_sFileInfo.fdate >> 5) & 15),
                   (uint32_t)(g_sFileInfo.fdate & 31),
                   (uint32_t)((g_sFileInfo.ftime >> 11)),
                   (uint32_t)((g_sFileInfo.ftime >> 5) & 63),
                   (uint32_t)(g_sFileInfo.fsize),
                   g_sFileInfo.fname);
        // Add the information as a line in the listbox widget.
        if(ulItemCount < NUM_LIST_STRINGS) {
            usprintf(g_pcFilenames[ulItemCount], "(%c) %12s", (g_sFileInfo.fattrib & AM_DIR) ? 'D' : 'F', g_sFileInfo.fname);
        }
        // If the attribute is directory, then increment the directory count.
        if(g_sFileInfo.fattrib & AM_DIR) {
            ulDirCount++;
        }
        // Otherwise, it is a file. Increment the file count, and add in the file size to the total.
        else {
            ulFileCount++;
            ulTotalSize += g_sFileInfo.fsize;
        }
        // Move to the next entry in the item array we use to populate the list box.
        ulItemCount++;
    }
    // Print summary lines showing the file, dir, and size totals.
    UARTprintf("\n%4u File(s),%10u bytes total\n%4u Dir(s)", ulFileCount, ulTotalSize, ulDirCount);
    // Get the free space.
    fresult = f_getfree("/", &ulTotalSize, &pFatFs);
    // Check for error and return if there is a problem.
    if(fresult != FR_OK) {
        // Fail
        UARTprintf("f_getfree fail:%d\n", fresult);
        set_run_flag(0);
        return;
    }
    // Display the amount of free space that was calculated.
    UARTprintf(", %10uK bytes free\n", ulTotalSize * g_sFatFs.csize / 2);
    // Return success.
    set_run_flag(0);
    return;
}

/**
 * @brief Change directory.
 *
 * This function changes the current directory to the one specified in the arguments.
 */
void Cmd_cd(void) {
    unsigned long ulReason;
    FRESULT fresult;
    get_now_dir(cmd_dir_buffer);
    fresult = ChangeToDirectory(Gloval_argv[1], cmd_dir_buffer, &ulReason);
    if(fresult != FR_OK) {
        switch(ulReason) {
            case OPENDIR_ERROR:
                UARTprintf("Error opening new directory.\n");
                break;
            case NAME_TOO_LONG_ERROR:
                UARTprintf("Resulting path name is too long.\n");
                break;
            default:
                UARTprintf("An unrecognized error was reported.\n");
                break;
        }
    }
    set_dir_address(cmd_dir_buffer);
    // Return success.
    set_run_flag(0);
    return;
}

/**
 * @brief Remove a file or directory.
 *
 * This function removes the file or directory specified in the arguments.
 */
void Cmd_rm(void) {
    UARTprintf("f_unlink <= %s\n", Gloval_argv[1]);
    FRESULT fresult = f_unlink(Gloval_argv[1]);
    if(fresult != FR_OK) {
        UARTprintf("[Error] remove file or directory.[%d]\n", fresult);
    }
    // Return success.
    set_run_flag(0);
    return;
}

/**
 * @brief Create a directory.
 *
 * This function creates a directory with the name specified in the arguments.
 */
void Cmd_mkdir(void) {
    sprintf(cmd_temp_buffer, "%s/%s", cmd_dir_buffer, Gloval_argv[1]);
    UARTprintf("f_mkdir <= %s\n", cmd_temp_buffer);
    FRESULT fresult = f_mkdir(cmd_temp_buffer);
    if(fresult != FR_OK) {
        UARTprintf("[Error] make directory.[%d]\n", fresult);
    }
    // Return success.
    set_run_flag(0);
    return;
}
