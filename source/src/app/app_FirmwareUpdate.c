/*
 * app_FirmwareUpdate.c
 *
 *  Created on: 2024. 6. 18.
 *      Author: User
 */
#include "main.h"
#ifdef APP
#else
#define APPLICATION_ADDRESS 0x090000
#define FLASH_SIZE  0x2E000
#define TEMP_READ_MEMORY_SIZE   256
#define TEMP_WRITE_MEMORY_SIZE   128
#define APPLICATION_START_SECTOR    5

Uint16 fwUpdateFlag = 0;
Uint16 selectTimerCount = 0;

//  FW Data Buffer
Uint16 FwBuffer[TEMP_READ_MEMORY_SIZE];
Uint16 read_file_address[32];

//  Combination Buffer
Uint16 CombinationBuffer[TEMP_WRITE_MEMORY_SIZE];

//
/////////////////////////////////////////////////////////////////////////////////////////
//

void UpdateTimerCount(void) {
    selectTimerCount++;
}

void setFwUpdateFlag(Uint16 data) {
    fwUpdateFlag = data;
}

Uint16 getFwUpdateFlag(void) {
    return fwUpdateFlag;
}

void app_FWupdate_start(void) {
    setFwUpdateFlag(1);
    //  Return success.
    set_run_flag(0);
    return ;
}

uint32_t app_FWupdate(void) {
    uint32_t jumpToAddress = (uint32_t)APPLICATION_ADDRESS;
#if 0
    //
    //  MCU Flash Erase & Write Test
    //

    //  erase Test
    Uint16 i = 0;
    Uint16 res = 0;
    Uint16 count = 0;
    UARTprintf("Start Erase Flash Test\n");
    for(i = 5 ; i < 14; i++) {
        res = erase_flash(i);
        if (res == 0) {
            count++;
        }
    }
    UARTprintf("End Erase Flash Test [count=%d]\n",count);

    //  Write Test
    memset(FwBuffer,0x00,sizeof(FwBuffer));
    for(i = 0 ; i < 1024 ; i++) {
        FwBuffer[i] = i;
    }
    ProgramUsingAutoECC(5,FwBuffer,0,256);
    ProgramUsingAutoECC(6,FwBuffer,0,256);
    ProgramUsingAutoECC(7,FwBuffer,0,256);
    ProgramUsingAutoECC(8,FwBuffer,0,256);
    ProgramUsingAutoECC(9,FwBuffer,0,256);
    ProgramUsingAutoECC(10,FwBuffer,0,256);
    ProgramUsingAutoECC(11,FwBuffer,0,256);
    ProgramUsingAutoECC(12,FwBuffer,0,256);

    //  End
    //  Release the pump access
    Flash_releasePumpSemaphore();
#else

    //  FW file Size
    int32 fwSize = 0;
    //  FW file read count
    Uint32 fwCount = (Uint32)FLASH_SIZE / (Uint32)TEMP_WRITE_MEMORY_SIZE;
    //  FW file read index
    Uint32 fwReadindex = 0;
    //  Flash Sector Index & Address
    Uint16 fwFlashSectorIndex = 0, fwFlashSectorIndexOld = APPLICATION_START_SECTOR;
    Uint32 fwFlashSectorAddressIndex = 0;
    //  Now Flash Address
    uint32_t flashAddress = (uint32_t)APPLICATION_ADDRESS;

    //  Select Loop Control Flag
    Uint16 menuSelectLoopFlag = 1;
    //  Select Wait Time (S)
    Uint16 count_Timer = 5;

    //  Variable
    int32 res = 0;
    Uint16 getChar = 0;
    Uint16 error_count = 0;
    Uint16 count_Timer_tmp = 0;
    Uint16 count_Timer_tmp_old = 0;
    Uint16 tmpIndex = 0;
    Uint32 i = 0, x = 0;

    //
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //

    //  Select Update Mode
    Uint16 fwUpdateMode = 0;
    UARTprintf("Select boot method\n");
    UARTprintf("    0: Nomal Boot (MCU Flash) -> Default\n");
    UARTprintf("    1: SD-Card Update\n");
    //  TBD
    //UARTprintf("    2: UART Update\n");
    //UARTprintf("    3: CAN Update\n");

    selectTimerCount = 0;

    //  Wait user select
    UARTprintf("%d",count_Timer-(selectTimerCount/1000));
    while(menuSelectLoopFlag) {
        //  Time out
        if(selectTimerCount >= 5000) {
            UARTprintf("\n");
            // loop out
            menuSelectLoopFlag = 0;
            break;
        }
        else{
            //  ms CountVal convert to s'
            count_Timer_tmp = count_Timer-(selectTimerCount/1000);
            if(count_Timer_tmp_old != count_Timer_tmp) {
                count_Timer_tmp_old = count_Timer_tmp;
                //  terminal cusor left 1 move
                UARTprintf("\033[1D");
                UARTprintf("%d",count_Timer-(selectTimerCount/1000));
            }
            //  Get User KeyVal
            getChar = scia_read_nowait();
            //  Key Data incoming
            if(getChar != 0){
                //  Key Data is 0~9
                if((getChar >= 0x30)&&(getChar <= 0x39)){
                    //  char format to int
                    fwUpdateMode = getChar - 0x30;
                    UARTprintf("\nselect Number=%d\n",fwUpdateMode);
                    // loop out
                    menuSelectLoopFlag = 0;
                    break;
                }
            }
        }
    }

    //  Select Wait시 App으로 넘어갈때 FreeRTOS가 멈추는 현상 발생. Timer2를 정지하지 않으면 뭔가 APP쪽에서 꼬이는듯
    CPUTimer_stopTimer(CPUTIMER2_BASE);

    //  Select Boot Menu
    switch(fwUpdateMode) {
    case 1:
        //  init FW file name
        memset(read_file_address, 0x00, sizeof(read_file_address));
        sprintf(read_file_address,"CPU1_APP");

        //  check FW file
        fwSize = read_file(read_file_address, FwBuffer, sizeof(FwBuffer), 0);
        if(fwSize != -1) {
        UARTprintf("fwCount=%l\n",fwCount);
            //  Erase Flash
            UARTprintf("Start Erase Flash\n");
            for(i = 5 ; i < 14; i++) {
                res = erase_flash(i);
                if (res != 0) {
                    error_count++;
                }
            }
            if (error_count == 0) {
                UARTprintf("End Erase Flash\n");
            }
            else {
                UARTprintf("Erase Flash Error [error_count=%d]\n",error_count);
                return 0;
            }

            //  Read & write Data
            UARTprintf("Start Flash Programing\n");
            for(i = 0 ; i < fwCount ; i++) {
                res = read_file(read_file_address, FwBuffer, sizeof(FwBuffer), fwReadindex);
                if(res == -1) {
                    //  read fail count, if fail count up to 5 that is stop.
                    error_count++;
                    if (error_count >= 5) {
                        UARTprintf("read fwFile fail\n");
                        return 0;
                    }
                }
                else {
                    //  Convert MCU Flash Format
                    tmpIndex = 0;
                    for(x = 0 ; x < TEMP_WRITE_MEMORY_SIZE ; x++) {
                        CombinationBuffer[x] = FwBuffer[tmpIndex] + ( FwBuffer[tmpIndex + 1] << 8 );
                        tmpIndex += 2;
                    }
                    //  fwFlashSectorIndex = TMS320F28 Flash Sector Number
                    fwFlashSectorIndex = ConvertAddressToSectorNumber(flashAddress);
                    if(fwFlashSectorIndexOld != fwFlashSectorIndex ) {
                        UARTprintf("fwFlashSectorIndex=%d\n",fwFlashSectorIndex);
                        fwFlashSectorIndexOld = fwFlashSectorIndex;
                        fwFlashSectorAddressIndex = 0;
                    }
                    //  fwFlashSectorAddressIndex = TMS320F28 Flash Sector n relative Address
                    //UARTprintf("Readindex=%6l, FlashSectorAddressIndex=%6l, FlashAddress=0x%8x, ReadSize=%6l, data=0x%2x\n", fwReadindex, fwFlashSectorAddressIndex, flashAddress, res, CombinationBuffer[0]);
                    res = ProgramUsingAutoECC(fwFlashSectorIndex, CombinationBuffer, fwFlashSectorAddressIndex, TEMP_WRITE_MEMORY_SIZE);

                    if((res != 0)&&(res != 3)) {
                        UARTprintf("Flash write fail:%l\n",res);
                    }
                    fwReadindex += TEMP_READ_MEMORY_SIZE;
                    flashAddress += TEMP_WRITE_MEMORY_SIZE;
                    fwFlashSectorAddressIndex += TEMP_WRITE_MEMORY_SIZE;
                }
            }

            if (error_count == 0) {
                UARTprintf("End Flash Programing\n");
            }
            else {
                UARTprintf("Flash Programing Error [error_count=%d]\n",error_count);
                return 0;
            }

            //  Release the pump access
            Flash_releasePumpSemaphore();

            //  delet file > f_unlink
            //  TBD
            //f_unlink("app");
        }
        else {
            UARTprintf("Can not Find Update FW file\n");
        }
        break;
    }

#endif
    return jumpToAddress;
}
#endif

//
//
//
