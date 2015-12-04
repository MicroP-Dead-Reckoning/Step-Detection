#include "all_wireless_tests.h"


void test_read_write(void){
	
	uint8_t test_values[3] = {0x1F,0xDE,0xAD};
	uint8_t orig[3], res[3];
	
	CC2500_Read(orig, 0x00, 3);
	
	WIRELESS_TRY(orig[0], VAL_CC2500_IOCFG2);
	WIRELESS_TRY(orig[1], VAL_CC2500_IOCFG1);
	WIRELESS_TRY(orig[2], VAL_CC2500_IOCFG0);
	//CC2500_Read(&result, 0x00, 1);
	//printf("result = %d\n", result);
	
	//uint8_t val2[3] = {65,80,43};
	CC2500_Write(test_values, 0x00, 3);
	CC2500_Read(res, 0x00, 3);
	
	WIRELESS_TRY(res[0], test_values[0]);
	WIRELESS_TRY(res[1], test_values[1]);
	WIRELESS_TRY(res[2], test_values[2]);
	
	CC2500_Write(orig, 0x00, 3);
	CC2500_Read(res, 0x00, 3);
	WIRELESS_TRY(res[0], VAL_CC2500_IOCFG2);
	WIRELESS_TRY(res[1], VAL_CC2500_IOCFG1);
	WIRELESS_TRY(res[2], VAL_CC2500_IOCFG0);
	
	printf("result = %d, %d, %d\n", res[0], res[1], res[2]);
}	
