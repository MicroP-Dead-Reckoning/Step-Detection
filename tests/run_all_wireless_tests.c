#include "all_wireless_tests.h"

/*
 * main: initialize and start the system
 */
int main (void) {
	
	int i;
	for (i=0; i<168000000/24; i++);
	CC2500_SPI_INIT(CC2500_TRANS);
	for (i=0; i<168000000/24; i++);
	test_control_read();
	test_read_write();
	//test_wireless();
	test_transmit();
}
