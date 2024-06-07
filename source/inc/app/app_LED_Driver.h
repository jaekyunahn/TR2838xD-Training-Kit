/*
 * app_LedBlink.h
 *
 *  Created on: 2024. 5. 9.
 *      Author: ajg10
 */

#ifndef INC_APP_APP_LED_DRIVER_H_
#define INC_APP_APP_LED_DRIVER_H_

void init_app_LED_Driver(void);
void app_LED_Driver_set_gpio(void);
void app_LED_Driver(Uint16 address, Uint16 data);

#endif /* INC_APP_APP_LED_DRIVER_H_ */
