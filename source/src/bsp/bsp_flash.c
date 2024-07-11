/*
 * bsp_flash.c
 *
 *  Created on: 2024. 6. 16.
 *      Author: User
 */
#include "main.h"

#pragma CODE_SECTION(init_program_flash, ".TI.ramfunc");
#pragma CODE_SECTION(erase_flash, ".TI.ramfunc");

void init_program_flash(void) {

    Fapi_StatusType  oReturnCheck;

#ifdef CPU1
    //  At 200MHz, execution wait-states for external oscillator is 3. Modify the wait-states when the system clock frequency is changed.
    Flash_initModule(FLASH0CTRL_BASE, FLASH0ECC_BASE, 3);
    //  Pump access must be gained by the core using pump semaphore
    Flash_claimPumpSemaphore(FLASH_CPU1_WRAPPER);
#endif

    //  Initialize the Flash API by providing the Flash register base address and operating frequency(in MHz).
    //  This function is required to initialize the Flash API based on System frequency before any other Flash API operation can be performed.
    //  This function must also be called whenever System frequency or RWAIT is changed.
    oReturnCheck = Fapi_initializeAPI(F021_CPU0_BASE_ADDRESS, DEVICE_SYSCLK_FREQ/1000000U);
    if(oReturnCheck != Fapi_Status_Success) {
        //  Check Flash API documentation for possible errors
        return;
    }

    //  Initialize the Flash banks and FMC for erase and program operations.
    //  Fapi_setActiveFlashBank() function sets the Flash banks and FMC for further Flash operations to be performed on the banks.
    oReturnCheck = Fapi_setActiveFlashBank(Fapi_FlashBank0);
    if(oReturnCheck != Fapi_Status_Success) {
        //  Check Flash API documentation for possible errors
        return;
    }
}

Uint16 erase_flash(Uint16 sectorNum) {
    Uint16 res = 0;
    Fapi_StatusType  oReturnCheck;
    Fapi_FlashStatusType  oFlashStatus;
    Fapi_FlashStatusWordType  oFlashStatusWord;

    uint32 sectorAddress = 0x00000000;
    uint32 sectorSize = 0x0000;

    switch(sectorNum) {
    case 0: sectorAddress = Bzero_Sector0_start; sectorSize = Sector16KB_u32length; break;
    case 1: sectorAddress = Bzero_Sector1_start; sectorSize = Sector16KB_u32length; break;
    case 2: sectorAddress = Bzero_Sector2_start; sectorSize = Sector16KB_u32length; break;
    case 3: sectorAddress = Bzero_Sector3_start; sectorSize = Sector16KB_u32length; break;
    case 4: sectorAddress = Bzero_Sector4_start; sectorSize = Sector64KB_u32length; break;
    case 5: sectorAddress = Bzero_Sector5_start; sectorSize = Sector64KB_u32length; break;
    case 6: sectorAddress = Bzero_Sector6_start; sectorSize = Sector64KB_u32length; break;
    case 7: sectorAddress = Bzero_Sector7_start; sectorSize = Sector64KB_u32length; break;
    case 8: sectorAddress = Bzero_Sector8_start; sectorSize = Sector64KB_u32length; break;
    case 9: sectorAddress = Bzero_Sector9_start; sectorSize = Sector64KB_u32length; break;
    case 10: sectorAddress = Bzero_Sector10_start; sectorSize = Sector16KB_u32length; break;
    case 11: sectorAddress = Bzero_Sector11_start; sectorSize = Sector16KB_u32length; break;
    case 12: sectorAddress = Bzero_Sector12_start; sectorSize = Sector16KB_u32length; break;
    case 13: sectorAddress = Bzero_Sector13_start; sectorSize = Sector16KB_u32length; break;
    }

    //  Erase the sector that is programmed in the above example Erase Sector6
    oReturnCheck = Fapi_issueAsyncCommandWithAddress(Fapi_EraseSector, (uint32 *)sectorAddress);

    //  Wait until FSM is done with erase sector operation
    while (Fapi_checkFsmForReady() != Fapi_Status_FsmReady);
    if(oReturnCheck != Fapi_Status_Success) {
        //  Check Flash API documentation for possible errors
        res = 1;
        return res;
    }

    //  Read FMSTAT register contents to know the status of FSM after erase command to see if there are any erase operation related errors
    oFlashStatus = Fapi_getFsmStatus();
    if(oFlashStatus != 0) {
        //  Check Flash API documentation for FMSTAT and debug accordingly Fapi_getFsmStatus() function gives the FMSTAT register contents.
        //  Check to see if any of the EV bit, ESUSP bit, CSTAT bit or VOLTSTAT bit is set (Refer to API documentation for more details).
        res = 2;
        return res;
    }

    //  Verify that Sector6 is erased
    oReturnCheck = Fapi_doBlankCheck((uint32 *)sectorAddress, sectorSize, &oFlashStatusWord);
    if(oReturnCheck != Fapi_Status_Success) {
        //  Check Flash API documentation for possible errors
        res = 3;
        return res;
    }

    return res;
}

Fapi_StatusType oReturnCheck;
Uint16 ProgramUsingAutoECC(Uint16 sectorNum, Uint16 *Buffer, Uint32 startIndex, Uint16 writeSize) {
    uint32 *Buffer32 = (uint32 *)Buffer;

    Uint16 res = 0;

    uint32 u32Index = 0, i = 0;
    Fapi_FlashStatusType        oFlashStatus;
    Fapi_FlashStatusWordType    oFlashStatusWord;

    uint32 sectorAddress = 0x00000000;

    switch(sectorNum) {
    case 0: sectorAddress  = Bzero_Sector0_start;  break;
    case 1: sectorAddress  = Bzero_Sector1_start;  break;
    case 2: sectorAddress  = Bzero_Sector2_start;  break;
    case 3: sectorAddress  = Bzero_Sector3_start;  break;
    case 4: sectorAddress  = Bzero_Sector4_start;  break;
    case 5: sectorAddress  = Bzero_Sector5_start;  break;
    case 6: sectorAddress  = Bzero_Sector6_start;  break;
    case 7: sectorAddress  = Bzero_Sector7_start;  break;
    case 8: sectorAddress  = Bzero_Sector8_start;  break;
    case 9: sectorAddress  = Bzero_Sector9_start;  break;
    case 10: sectorAddress = Bzero_Sector10_start; break;
    case 11: sectorAddress = Bzero_Sector11_start; break;
    case 12: sectorAddress = Bzero_Sector12_start; break;
    case 13: sectorAddress = Bzero_Sector13_start; break;
    }

    //  Start Address º¸Á¤
    sectorAddress = sectorAddress + startIndex;
    //UARTprintf("[bsp_flash]sectorAddress=0x%x,startIndex=%l\n",sectorAddress,startIndex);

    for( i=0, u32Index = sectorAddress ; (u32Index < (sectorAddress + writeSize)) ; i+= 8, u32Index+= 8) {
        //
        //UARTprintf("[bsp_flash]u32Index=0x%x,target=0x%x,i=%l\n",u32Index,(sectorAddress + writeSize),i);

        //  Flash Programing
        oReturnCheck = Fapi_issueProgrammingCommand((uint32 *)u32Index, Buffer+i, 8, 0, 0, Fapi_AutoEccGeneration);

        //  Wait until the Flash program operation is over
        while(Fapi_checkFsmForReady() == Fapi_Status_FsmBusy);
        if(oReturnCheck != Fapi_Status_Success) {
            res = 1;
            return res;
        }

        //  Read FMSTAT register contents to know the status of FSM after program command to see if there are any program operation related errors
        oFlashStatus = Fapi_getFsmStatus();
        if((oFlashStatus != 0) && (oFlashStatus != 0x30)) {
            UARTprintf("oFlashStatus:%l\n",oFlashStatus);
            res = 2;
            return res;
        }

        //  Verify the programmed values.  Check for any ECC errors.
        oReturnCheck = Fapi_doVerify((uint32 *)u32Index, 4, Buffer32+(i/2), &oFlashStatusWord);
        if(oReturnCheck != Fapi_Status_Success) {
            Uint32 ioReturnCheck = oReturnCheck;
            switch(ioReturnCheck)
            {
            case Fapi_Status_FsmBusy:
            case Fapi_Status_FsmReady:
            case Fapi_Status_AsyncBusy:
            case Fapi_Status_AsyncComplete:
            case Fapi_Error_Fail:
            case Fapi_Error_StateMachineTimeout:
            case Fapi_Error_OtpChecksumMismatch:
            case Fapi_Error_InvalidDelayValue:
            case Fapi_Error_InvalidHclkValue:
            case Fapi_Error_InvalidCpu:
            case Fapi_Error_InvalidBank:
            case Fapi_Error_InvalidAddress:
            case Fapi_Error_InvalidReadMode:
            case Fapi_Error_AsyncIncorrectDataBufferLength:
            case Fapi_Error_AsyncIncorrectEccBufferLength:
            case Fapi_Error_AsyncDataEccBufferLengthMismatch:
            case Fapi_Error_FeatureNotAvailable:
            case Fapi_Error_FlashRegsNotWritable:
            case Fapi_Error_InvalidCPUID:
                UARTprintf("Fapi_doVerify:%l\n",oReturnCheck);
                break;
            }
        }
    }

    //  OK
    return res;
}


// Convert Flash memory Address to Sector Number
Uint16 ConvertAddressToSectorNumber(uint32_t address) {
    Uint16 res = 0;
    if( ( address >= 0x080000 ) && ( address < 0x082000 ) ) res = 0;
    else if( ( address >= 0x082000 ) && ( address < 0x084000 ) ) res = 1;
    else if( ( address >= 0x084000 ) && ( address < 0x086000 ) ) res = 2;
    else if( ( address >= 0x086000 ) && ( address < 0x088000 ) ) res = 3;
    else if( ( address >= 0x088000 ) && ( address < 0x090000 ) ) res = 4;
    else if( ( address >= 0x090000 ) && ( address < 0x098000 ) ) res = 5;
    else if( ( address >= 0x098000 ) && ( address < 0x0A0000 ) ) res = 6;
    else if( ( address >= 0x0A0000 ) && ( address < 0x0A8000 ) ) res = 7;
    else if( ( address >= 0x0A8000 ) && ( address < 0x0B0000 ) ) res = 8;
    else if( ( address >= 0x0B0000 ) && ( address < 0x0B8000 ) ) res = 9;
    else if( ( address >= 0x0B8000 ) && ( address < 0x0BA000 ) ) res = 10;
    else if( ( address >= 0x0BA000 ) && ( address < 0x0BC000 ) ) res = 11;
    else if( ( address >= 0x0BC000 ) && ( address < 0x0BE000 ) ) res = 12;
    else if( ( address >= 0x0BE000 ) && ( address < 0x0C0000 ) ) res = 13;
    return res;
}


































