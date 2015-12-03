#define osObjectsPublic                     // define objects in main module
#include "osObjects.h"                      // RTOS object definitions
#include "stm32f4xx.h"                  		// Device header
#include "stm32f4xx_conf.h"
#include <stdio.h>
#include <stdlib.h>
#include "cc2500.h"
#include "gyroscope.h"

extern uint8_t distance;
extern uint8_t dist_last;
extern uint8_t dir_last;


int send_pos(uint8_t direction);
