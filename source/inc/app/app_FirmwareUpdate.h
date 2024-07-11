/*
 * app_FirmwareUpdate.h
 *
 *  Created on: 2024. 6. 18.
 *      Author: User
 */

#ifndef INC_APP_APP_FIRMWAREUPDATE_H_
#define INC_APP_APP_FIRMWAREUPDATE_H_

uint32_t app_FWupdate(void);
void setFwUpdateFlag(Uint16 data);
Uint16 getFwUpdateFlag(void);
void app_FWupdate_start(void);
void UpdateTimerCount(void);
Uint16 getUpdateTimerCount(void);
void setUpdateTimerCount(Uint16 data);

#endif /* INC_APP_APP_FIRMWAREUPDATE_H_ */
