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

#if 0
float32 PIcontroller(float32 in, float32 ref, struct PI_CTRL *PIcon, struct PI_GAIN gain){
    PIcon->err = ref - in;
    PIcon->prop = gain.Kp * PIcon->err;
    PIcon->integ = PIcon->integ + gain.KiT * (PIcon->err - gain.Ka*PIcon->out_err);
    PIcon->out = PIcon->prop + PIcon->integ;

    if(PIcon->out > gain.out_max) PIcon->out_sat = gain.out_max;
    else if (PIcon->out < 0.) PIcon->out_sat = 0.;
    else PIcon->out_sat = PIcon->out;

    PIcon->out_err = PIcon->out - PIcon->out_sat;

    return PIcon->out_sat;
}
#endif

void IIR1CoeffInit(IIR1 *p_gIIR, float w0 )
{
    float a0, b0, b1;
    float INV_alpha, dt;
    int type;

    // Continuous-time Filter Coefficients
    p_gIIR->w0 = w0 ;
    type = p_gIIR->type;
    dt = p_gIIR->delT;

    a0 = w0;
    switch(type)
    {
        case K_LPF:
            b0 = w0;
            b1 = 0;
            break;
        case K_HPF:
            b0 = 0;
            b1 = (float)1;
            break;
        default:
        case K_ALLPASS:
            b0 = -w0;
            b1 = (float)1;
    }

    // Discrete-time Filter Coefficients
    INV_alpha = (float)1./((float)2 + dt*a0);
    p_gIIR->coeff[0] = ((float)2*b1 + dt*b0)*INV_alpha;
    p_gIIR->coeff[1] = (-(float)2*b1 + dt*b0)*INV_alpha;
    p_gIIR->coeff[2] = -(-(float)2 + dt*a0)*INV_alpha;

    return;
}

void IIR1Init(IIR1 *p_gIIR, float w0 )
{
    // Initialize Filter Coefficients
    IIR1CoeffInit(p_gIIR, w0);

    // Initialize Storage Elements
    p_gIIR->reg = 0;

    return;
}

float IIR1Update(IIR1 *p_gIIR, const float x)
{
    float y;

    y = p_gIIR->reg + p_gIIR->coeff[0]*x;
    p_gIIR->reg = p_gIIR->coeff[1]*x + p_gIIR->coeff[2]*y;

    return(y);
}
//
//
//
