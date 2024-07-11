/*
 * app_Xmodem.c
 *
 *  Created on: 2024. 7. 5.
 *      Author: User
 */
#include "main.h"

#define SOH 0x01
#define EOT 0x04
#define ACK 0x06
#define NAK 0x15
#define ETB 0x17
#define CAN 0x18
#define RDY 0x43

Uint16 thread_loop_flag = 0;

Uint16 receiveBuffer[135];
Uint16 dataBuffer[128];
Uint16 receiveBufferIndex = 0;
Uint16 iTimeOutCounter = 0;//ms
//  for Flash
Uint16 UART_CombinationBuffer[64];
Uint16 EraseErrorCount = 0;

//  debug
Uint16 debugtimeoutCounter = 0;
Uint16 debugLastdata = 0;

struct UART_DATA uart_recive_Data;

Uint16 flowStatus = 0;
void Xmodem_thread(void) {
    int32 res = 0;
    Uint32 x, i;
    Uint16 tmpIndex = 0;
    //  Flash Sector Index & Address
    Uint16 fwFlashSectorIndex = 0, fwFlashSectorIndexOld = APPLICATION_START_SECTOR;
    Uint32 fwFlashSectorAddressIndex = 0;
    //  Now Flash Address
    uint32_t flashAddress = (uint32_t)APPLICATION_ADDRESS;

    //
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //

    //  init
    uart_recive_Data.flag = 0;
    memset(receiveBuffer,0x00,sizeof(receiveBuffer));
    receiveBufferIndex=0;

    //  w
    thread_loop_flag = 1;
    flowStatus = 0;

    //  Download case
#ifdef APP
    //  DownLoad and Save SD-Card : TBD
#else
    //  Erase and Download Flash
    UARTprintf("Start Erase Flash\n");
    for(i = 5 ; i < 14; i++) {
        res = erase_flash(i);
        if (res != 0) {
            EraseErrorCount++;
        }
    }
    UARTprintf("End Erase Flash (Error Count:%d)\n",EraseErrorCount);
#endif

    //  X-Modem Thread
    while(thread_loop_flag) {
        //  check Time Out Counter
        iTimeOutCounter = getUpdateTimerCount();
        //  check revData
        uart_recive_Data = scia_read_struct();
        if(uart_recive_Data.flag == 1){
            receiveBuffer[receiveBufferIndex] = uart_recive_Data.data;
            debugLastdata = uart_recive_Data.data;
            setUpdateTimerCount(0);//iTimeOutCounter = getUpdateTimerCount();
            receiveBufferIndex++;
        }

        switch(flowStatus) {
        case 0:
            //  send char 'C'
            scia_write(RDY);
            //  start time out counter
            setUpdateTimerCount(0);
            //  wait revData
            flowStatus = 1;
            break;

        case 1:
            //  time out 3s
            if(iTimeOutCounter >= 3000) {
                flowStatus = 0;
                memset(receiveBuffer,0x00,sizeof(receiveBuffer));
                receiveBufferIndex=0;
                //  time out count
                debugtimeoutCounter++;
            }

            switch(receiveBuffer[0]) {
            case SOH:
                //  incoming Data
                if(receiveBufferIndex >= 133) {
#ifdef APP
                    //
                    //  SD-Card Write
                    //
#else
                    //
                    //  Flash Write
                    //

                    //  copy rx buffer to write buffer
                    memset(dataBuffer,0x00,sizeof(dataBuffer));
                    memcpy(dataBuffer, receiveBuffer+3, sizeof(dataBuffer));

                    //  buffer remap
                    tmpIndex = 0;
                    for(x = 0 ; x < 64 ; x++) {
                        UART_CombinationBuffer[x] = dataBuffer[tmpIndex] + ( dataBuffer[tmpIndex + 1] << 8 );
                        tmpIndex += 2;
                    }

                    //  fwFlashSectorIndex = TMS320F28 Flash Sector Number
                    fwFlashSectorIndex = ConvertAddressToSectorNumber(flashAddress);
                    if(fwFlashSectorIndexOld != fwFlashSectorIndex ) {
                        //UARTprintf("fwFlashSectorIndex=%d\n",fwFlashSectorIndex);
                        fwFlashSectorIndexOld = fwFlashSectorIndex;
                        fwFlashSectorAddressIndex = 0;
                    }
                    //  fwFlashSectorAddressIndex = TMS320F28 Flash Sector n relative Address
                    //UARTprintf("Readindex=%6l, FlashSectorAddressIndex=%6l, FlashAddress=0x%8x, ReadSize=%6l, data=0x%2x\n", fwReadindex, fwFlashSectorAddressIndex, flashAddress, res, CombinationBuffer[0]);
                    res = ProgramUsingAutoECC(fwFlashSectorIndex, UART_CombinationBuffer, fwFlashSectorAddressIndex, 64);

                    if((res != 0)&&(res != 3)) {
                        //UARTprintf("Flash write fail:%l\n",res);
                    }
                    flashAddress += 64;
                    fwFlashSectorAddressIndex += 64;
#endif
                    //
                    scia_write(ACK);
                    memset(receiveBuffer,0x00,sizeof(receiveBuffer));
                    receiveBufferIndex=0;
                }
                break;

                //
                //  End Char : EOT or ETB
                //

            case EOT:
                scia_write(ACK);
                flowStatus = 2;
                break;

            case ETB:
                scia_write(ACK);
                flowStatus = 2;
                break;

            default:
                memset(receiveBuffer,0x00,sizeof(receiveBuffer));
                receiveBufferIndex=0;
                break;
            }

            break;

        case 2:
            //  Enter Key TimeOut
            if(iTimeOutCounter >= 3000) {
                UARTprintf("Please press Enter Key\n");
                setUpdateTimerCount(0);
            }
            //  예외처리 >> 규정상 EOT, ETB문자가 순서대로 와야 하는데 순서가 뒤집히는 경우도 있어서 이때 ACK 처리해줘야 상대도 통신을 마칠 수 있음
            if((uart_recive_Data.data == EOT)||(uart_recive_Data.data == ETB)) {
                scia_write(ACK);
            }
            //  Enter Key Press
            if(uart_recive_Data.data == 13) {
                thread_loop_flag = 0;
            }
            break;
        }
    }
}

