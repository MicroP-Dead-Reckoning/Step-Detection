#define osObjectsPublic                     // define objects in main module
#include "osObjects.h"                      // RTOS object definitions
#include "stm32f4xx.h"                  		// Device header
#include "stm32f4xx_conf.h"
#include <stdio.h>
#include <stdlib.h>
#include "cc2500.h"
#include "gyroscope.h"

uint8_t distance = 0;
uint8_t dist_last = 0;
uint8_t dir_last = 0;


int send_pos(uint8_t direction);
