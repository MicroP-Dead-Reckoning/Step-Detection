#include "all_wireless_tests.h"


void test_wireless(void){

	uint8_t res2[3];
	
	CC2500_Read(res2, CC2500_FREQ_REG, 3);
	printf("result = %x, %x, %x\n", res2[0], res2[1], res2[2]);

	CC2500_INT_INIT();
	for (int i=0; i<168000000/6; i++);
	
	while(1);
	
}	

uint8_t recv_buffer[20];
uint8_t num;
int TEST_VALUE = 0;
void EXTI15_10_IRQHandler() {
	int i;
	if (EXTI_GetITStatus(CC2500_SPI_INT1_EXTI_LINE) != RESET){
		CC2500_Read_SR(&num, 0x3B);
		CC2500_Read_RX(recv_buffer, num);
		for(i = 0; i < num; i++){
			printf("%d, ", recv_buffer[i]);
		}
		printf("\n");
		EXTI_ClearITPendingBit(CC2500_SPI_INT1_EXTI_LINE);
	}
}
