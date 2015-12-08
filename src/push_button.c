#include "push_button.h"

#include <stdio.h>

#include "osObjects.h"                      // RTOS object definitions
#include "stm32f4xx.h"                  		// Device header
#include "stm32f4xx_conf.h"
#include "update_pos.h"

/*!
	\brief Sets up the user button the stm407 board

	Sets up the `User Button` on the STM407 discoveryboard, 
	including Power, GPIO, NVIC and EXTI setup
 */
void setup_button() {
	
	GPIO_InitTypeDef gpio_init_s;

	/* setup PA0 (User Button) */
	//Enable clock for GPOIA
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	gpio_init_s.GPIO_Pin = GPIO_Pin_0;
	//Mode input
	gpio_init_s.GPIO_Mode = GPIO_Mode_IN;
	gpio_init_s.GPIO_OType = GPIO_OType_PP;
	gpio_init_s.GPIO_PuPd = GPIO_PuPd_DOWN;
	gpio_init_s.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &gpio_init_s);
	
		/* setup exti */

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE); //CHANGE?
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE); // Enable

	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOD, EXTI_PinSource1);

	EXTI_InitTypeDef exti_init;

	exti_init.EXTI_Line = EXTI_Line0; //EXTI Line 0

	exti_init.EXTI_LineCmd = ENABLE; //Enable
	exti_init.EXTI_Mode = EXTI_Mode_Interrupt; //Use for interupts
	exti_init.EXTI_Trigger = EXTI_Trigger_Rising;

	EXTI_Init(&exti_init);

	/* setup nvic */
	NVIC_InitTypeDef nvic_init;

	nvic_init.NVIC_IRQChannel = EXTI0_IRQn; //EXTIO

	nvic_init.NVIC_IRQChannelCmd = ENABLE;
	nvic_init.NVIC_IRQChannelPreemptionPriority = 0x00;
	nvic_init.NVIC_IRQChannelSubPriority = 0x01;
	NVIC_Init(&nvic_init);
}

/*!
 *  \brief User Button Interrupt Handler. Begins Transmission
 *  
 *  calls the `send_pos` function and then waits. Originally, we intended to
 *  signal a thread which would then start sending data. However, due to time 
 *  constraints we elected to use a fast and dirty solution
 */	
void EXTI0_IRQHandler(void) {
	if (EXTI_GetITStatus(EXTI_Line0) != RESET) {
		EXTI_ClearITPendingBit(EXTI_Line0); 			/* Clear interrupt flag */
	}
	send_pos();
	printf("Hello World\n");
	for (uint32_t i = 0; i < 1680000; i++);
}
