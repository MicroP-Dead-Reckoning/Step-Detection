/*----------------------------------------------------------------------------
 * CMSIS-RTOS 'main' function template
 *---------------------------------------------------------------------------*/

#define osObjectsPublic                     // define objects in main module
#include "osObjects.h"                      // RTOS object definitions
#include "stm32f4xx.h"                  		// Device header
#include "stm32f4xx_conf.h"
#include <stdio.h>
#include "cc2500.h"
#include "gyroscope.h"
#include "all_wireless_tests.h"


osThreadId gyroscope_thread;

osThreadDef(Gyroscope, osPriorityNormal, 1, 0);

/*
 * main: initialize and start the system
 */

int main (void) {
  osKernelInitialize ();                    // initialize CMSIS-RTOS

	CC2500_SPI_INIT(CC2500_TRANS);
	osDelay(250);
	CC2500_INT_INIT();
	osDelay(250);
	test_control_read();
	//test_transmit();

	init_gyroscope();
	for (uint32_t i = 0; i < 16800000; i++);
	 
	gyroscope_thread = osThreadCreate(osThread(Gyroscope), NULL);

	EXTI_GenerateSWInterrupt(EXTI_Line1);
	osKernelStart ();                         // start thread execution 
}

