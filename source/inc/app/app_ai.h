/*
 * app_ai.h
 *
 *  Created on: 2024. 5. 18.
 *      Author: User
 */

#ifndef INC_APP_APP_AI_H_
#define INC_APP_APP_AI_H_

void app_ai(void);
//float32 relu(float32 x);
//void softmax(float32* input, float32* output, Uint16 size) ;
//float32 perceptron(float32* input, float32* weight, Uint32 input_size) ;
void dence_layer( Uint16 layer_case, Uint16 layer_index, Uint16 input_layer_count, Uint16 output_layer_count );
void ai_tick_timer(void);
void convertBuffer_charTofloat(Uint16 *source, float32 *output, Uint16 count);
void CLA_start(void);
void CLA_end(void);
void CPU2_start(void);
void CPU2_end(void);

#endif /* INC_APP_APP_AI_H_ */
