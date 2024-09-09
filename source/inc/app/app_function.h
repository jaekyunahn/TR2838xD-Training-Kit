/*
 * app_function.h
 *
 *  Created on: 2024. 5. 21.
 *      Author: User
 */

#ifndef INC_APP_APP_FUNCTION_H_
#define INC_APP_APP_FUNCTION_H_

#define K_ALLPASS   0
#define K_LPF       1
#define K_HPF       2
#define K_BPF       3
#define K_NOTCH     4

#define IIR1DEFINE(type, w0, Ts)        {(int)(type), (float)(w0), (float)(Ts), 0, 0, 0, 0}
#define IIR2DEFINE(type, w0, zeta,Ts)   {(int)(type), (float)(w0), (float)(zeta), (float)(Ts), 0, 0, 0, 0, 0, 0, 0}

typedef struct  {
    int type;
    float w0;
    float delT;
    float coeff[3], reg;
}   IIR1;

void convert_c28_to_x86(Uint16 x86[2], Uint16 c28[4]);
void convert_x86_to_c28(Uint16 x86[2], Uint16 c28[4]);
Uint16 calcrc(Uint16 *data, Uint32 count);
void IIR1CoeffInit(IIR1 *p_gIIR, float w0 );
void IIR1Init(IIR1 *p_gIIR, float w0 );
float IIR1Update(IIR1 *p_gIIR, const float input );

#endif /* INC_APP_APP_FUNCTION_H_ */
