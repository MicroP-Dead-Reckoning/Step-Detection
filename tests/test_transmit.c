#include "all_wireless_tests.h"

void test_transmit(void){

	uint8_t res2[3];
	uint8_t to_send = 0;
	CC2500_Read(res2, CC2500_FREQ_REG, 3);
	printf("result = %x, %x, %x\n", res2[0], res2[1], res2[2]);
	while(1){
		CC2500_Write(&to_send, 	CC2500_FIFO_REG, 1);
		to_send = (to_send + 2) % 10;
		printf("sent = %d\n", to_send);
		for (int i=0; i<168000000/12; i++);
	}
	
}	
