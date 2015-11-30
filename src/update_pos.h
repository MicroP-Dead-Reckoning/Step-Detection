#define osObjectsPublic                     // define objects in main module
#include "osObjects.h"                      // RTOS object definitions
#include "stm32f4xx.h"                  		// Device header
#include "stm32f4xx_conf.h"
#include <stdio.h>
#include <stdlib.h>
#include "cc2500.h"
#include "gyroscope.h"

#define STEP_SIZE	1		//cm

uint16_t distance = 0;
uint16_t dist_last = 0;
uint8_t dir_last = 0;

void UpdatePos(void const *arguments);
int sendPos(uint8_t direction);
