#define osObjectsPublic                     // define objects in main module
#include "osObjects.h"                      // RTOS object definitions
#include "stm32f4xx.h"                  		// Device header
#include "stm32f4xx_conf.h"
#include <stdio.h>

#include "gyroscope.h"

#define STEP_SIZE	1		//cm

int distance = 0;
int dist_last = 0;
int dir_last = 0;

void UpdatePos(void const *arguments);
