/*
 * @file bsp_filesystem.c
 *
 * @brief File system management for SD card operations.
 *
 *  Created on: 2024. 5. 11.
 *      Author: User
 */
#include "main.h"

// Temporary data buffer used for manipulating file paths or reading data from the SD card.
static char g_cTmpBuf[PATH_BUF_SIZE];
#pragma DATA_SECTION(g_cTmpBuf, "ramgs0");    // map the TX data to memory

/**
 * @brief Initialize the file system buffer.
 */
void init_fileSystem_buffer(void) {
    memset(g_cTmpBuf, 0x00, sizeof(g_cTmpBuf));
}

/**
 * @brief Test function to read data from a file and print the result.
 */
void test_fatfs(void) {
    Uint16 read_data[10];
    FRESULT res;
    res = read_file("read_me.txt", read_data, 10, 0);
    UARTprintf("res=%d\n", res);
    UARTprintf("read data=%s\n", read_data);
}

/**
 * @brief Change to the specified directory.
 *
 * @param pcDirectory Pointer to the directory name.
 * @param now_dir Pointer to the current directory path.
 * @param pulReason Pointer to a variable to store the reason for failure, if any.
 *
 * @return FRESULT Result of the directory change operation.
 */
FRESULT ChangeToDirectory(char *pcDirectory, char *now_dir, unsigned long *pulReason) {
    static DIR sDirObject;
    unsigned int uIdx;
    FRESULT fresult;

    // Copy the current working path into a temporary buffer so it can be manipulated.
    strcpy(g_cTmpBuf, now_dir);

    // If the first character is '/', then this is a fully specified path, and it should just be used as-is.
    if(pcDirectory[0] == '/') {
        // Make sure the new path is not bigger than the cwd buffer.
        if(strlen(pcDirectory) + 1 > sizeof(now_dir)) {
            *pulReason = NAME_TOO_LONG_ERROR;
            return(FR_OK);
        }
        // If the new path name (in argv[1]) is not too long, then copy it into the temporary buffer so it can be checked.
        else {
            strncpy(g_cTmpBuf, pcDirectory, sizeof(g_cTmpBuf));
        }
    }
    // If the argument is ".." then attempt to remove the lowest level on the CWD.
    else if(!strcmp(pcDirectory, "..")) {
        // Get the index to the last character in the current path.
        uIdx = strlen(g_cTmpBuf) - 1;

        // Back up from the end of the path name until a separator (/) is found, or until we bump up to the start of the path.
        while((g_cTmpBuf[uIdx] != '/') && (uIdx > 1)) {
            // Back up one character.
            uIdx--;
        }

        // Now we are either at the lowest level separator in the current path, or at the beginning of the string (root).
        // So set the new end of string here, effectively removing that last part of the path.
        g_cTmpBuf[uIdx] = 0;
    }
    // Otherwise, this is just a normal path name from the current directory, and it needs to be appended to the current path.
    else {
        // Test to make sure that when the new additional path is added on to the current path,
        // there is room in the buffer for the full new path. It needs to include a new separator, and a trailing null character.
        if(cmd_size(g_cTmpBuf) + cmd_size(pcDirectory) + 1 + 1 < cmd_size(now_dir)) {
            *pulReason = NAME_TOO_LONG_ERROR;
            return(FR_INVALID_OBJECT);
        }
        // The new path is okay, so add the separator and then append the new directory to the path.
        else {
            // If not already at the root level, then append a '/'
            if(strcmp(g_cTmpBuf, "/")) {
                strcat(g_cTmpBuf, "/");
            }
            // Append the new directory to the path.
            strcat(g_cTmpBuf, pcDirectory);
        }
    }

    // At this point, a candidate new directory path is in g_cTmpBuf. Try to open it to make sure it is valid.
    fresult = f_opendir(&sDirObject, g_cTmpBuf);

    // If it can't be opened, then it is a bad path. Inform user and return.
    if(fresult != FR_OK) {
        *pulReason = OPENDIR_ERROR;
        return(fresult);
    }
    // Otherwise, it is a valid new path, so copy it into the CWD and update the screen.
    else {
        strncpy(now_dir, g_cTmpBuf, sizeof(g_cTmpBuf));
    }

    // Return success.
    return(FR_OK);
}

/**
 * @brief Read data from a file.
 *
 * @param address Pointer to the file path.
 * @param buffer Pointer to the buffer to store read data.
 * @param read_size Number of bytes to read.
 * @param index Offset to start reading from.
 *
 * @return int16 Number of bytes read, or -1 if an error occurred.
 */
int16 read_file(Uint16 *address, Uint16 *buffer, Uint16 read_size, Uint32 index) {
    int16 res = 0;
    UINT br;
    FSIZE_t file_size;
    FIL fil;
    FRESULT fresult = FR_OK;
    Uint16 readSize = read_size;
    FATFS FatFs;

    f_mount(&FatFs, "", 0);
    fresult = f_open(&fil, address, FA_READ);

    if(fresult == FR_OK) {
        // Check the file size
        file_size = f_size(&fil);
        //UARTprintf("file_size= %lBytes\n",(Uint32)file_size);
        // If the file size is smaller than the specified read size
        if((Uint16)file_size < read_size) {
            readSize = (Uint16)file_size;
        }

        // Set the file read pointer
        fresult = f_lseek(&fil, (FSIZE_t)index);

        if(fresult == FR_OK) {
            fresult = f_read(&fil, buffer, (UINT)readSize, &br);
            if(fresult == FR_OK) {
                f_close(&fil);
            }
        }
    }

    if(fresult == FR_OK) {
        res = br;
    } else {
        res = -1;
    }

    return res;
}

//
// End file
//
