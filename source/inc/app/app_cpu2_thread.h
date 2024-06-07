/*
 * app_cpu2_thread.h
 *
 *  Created on: 2024. 5. 9.
 *      Author: ajg10
 */

#ifndef INC_APP_APP_CPU2_THREAD_H_
#define INC_APP_APP_CPU2_THREAD_H_

#ifdef CPU2
void cpu2_thread(void);
void send_flag_to_cpu1(void);
#endif

#endif /* INC_APP_APP_CPU2_THREAD_H_ */
