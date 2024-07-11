/*
 * app_function.c
 *
 *  Created on: 2024. 5. 21.
 *      Author: User
 */
#include "main.h"

/**
 * @brief Converts a short array from x86 format to TMS320C2000 series memory format.
 *
 * This function takes a 2-element short array in x86 format and converts it
 * to a 4-element array in TMS320C2000 series memory format.
 *
 * @param[in] x86 The input array in x86 format.
 * @param[out] c28 The output array in TMS320C2000 series memory format.
 */
void convert_x86_to_c28(Uint16 x86[2], Uint16 c28[4]) {
    c28[0] = x86[0] & 0x00FF;
    c28[1] = (x86[0] & 0xFF00) >> 8;
    c28[2] = x86[1] & 0x00FF;
    c28[3] = (x86[1] & 0xFF00) >> 8;
}

/**
 * @brief Converts a short array from TMS320C2000 series memory format to x86 format.
 *
 * This function takes a 4-element array in TMS320C2000 series memory format
 * and converts it to a 2-element short array in x86 format.
 *
 * @param[out] x86 The output array in x86 format.
 * @param[in] c28 The input array in TMS320C2000 series memory format.
 */
void convert_c28_to_x86(Uint16 x86[2], Uint16 c28[4]) {
    x86[0] = (c28[0] & 0x00FF) + ((c28[1] & 0x00FF) << 8);
    x86[1] = (c28[2] & 0x00FF) + ((c28[3] & 0x00FF) << 8);
}


Uint16 calcrc(Uint16 *data, Uint32 count){
    Uint16  crc;
    Uint16 i;
    crc = 0;
    while (--count >= 0) {
        crc = crc ^ (Uint16) *data++ << 8;
        i = 8;
        do {
            if (crc & 0x8000){
                crc = crc << 1 ^ 0x1021;
            }
            else{
                crc = crc << 1;
            }
        } while(--i);
    }
    return crc;
}

//
//
//
