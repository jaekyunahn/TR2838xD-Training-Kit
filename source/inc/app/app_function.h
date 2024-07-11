/*
 * app_function.h
 *
 *  Created on: 2024. 5. 21.
 *      Author: User
 */

#ifndef INC_APP_APP_FUNCTION_H_
#define INC_APP_APP_FUNCTION_H_

void convert_c28_to_x86(Uint16 x86[2], Uint16 c28[4]);
void convert_x86_to_c28(Uint16 x86[2], Uint16 c28[4]);
Uint16 calcrc(Uint16 *data, Uint32 count);

#endif /* INC_APP_APP_FUNCTION_H_ */
