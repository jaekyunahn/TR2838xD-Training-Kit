/*
 * ai_lib.h
 *
 *  Created on: 2024. 6. 6.
 *      Author: User
 */

#ifndef AI_LIB_H_
#define AI_LIB_H_

typedef int                 int16;
typedef long                int32;
typedef long long           int64;
typedef unsigned int        Uint16;
typedef unsigned long       Uint32;
typedef unsigned long long  Uint64;
typedef float               float32;
typedef long double         float64;

float32 relu(float32 x);
void softmax(float32* input, float32* output, Uint16 size);
float32 perceptron(float32* input, float32* weight, Uint32 input_size);

#endif /* AI_LIB_H_ */
