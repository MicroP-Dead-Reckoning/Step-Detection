#ifndef _INCLUDE_UPDATE_POS_H_
#define _INCLUDE_UPDATE_POS_H_

#include "stm32f4xx.h"                  		// Device header
#include "stm32f4xx_conf.h"

void update_pos(uint8_t new_direction);
int send_pos(void);

#endif // _INCLUDE_UPDATE_POS_H_
