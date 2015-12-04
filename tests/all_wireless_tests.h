#include "osObjects.h" 

#include "stm32f4xx.h"                  // Device header
#include "stm32f4xx_conf.h"
#include <stdio.h>

#include "cc2500.h"

#define WIRELESS_TRY(val, exp) \
        if (val != exp) { \
				    printf(#val " was %d instead of expected " #exp " (%d)\n", val, exp); \
				}

void test_control_read(void);
void test_read_write(void);
void test_wireless(void);
void test_transmit(void);
