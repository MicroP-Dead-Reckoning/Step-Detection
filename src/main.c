/*----------------------------------------------------------------------------
 * CMSIS-RTOS 'main' function template
 *---------------------------------------------------------------------------*/

#define osObjectsPublic                     // define objects in main module
#include "osObjects.h"                      // RTOS object definitions
#include "stm32f4xx.h"                  		// Device header
#include "stm32f4xx_conf.h"
#include <stdio.h>

#include "gyroscope.h"

osThreadId gyroscope_thread;
osThreadId update_dir;
osThreadId update_xy;

osThreadDef(Gyroscope, osPriorityNormal, 1, 0);
//osThreadDef();
//osThreadDef();

/*
 * main: initialize and start the system
 */
 int main (void) {
  osKernelInitialize ();                    // initialize CMSIS-RTOS
	init_gyroscope();
	for (uint32_t i = 0; i < 16800000; i++);
	 
	gyroscope_thread = osThreadCreate(osThread(Gyroscope), NULL);
	
	EXTI_GenerateSWInterrupt(EXTI_Line0);
	osKernelStart ();                         // start thread execution 
}

