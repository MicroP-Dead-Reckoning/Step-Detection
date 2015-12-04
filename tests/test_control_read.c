#include "all_wireless_tests.h"

const uint8_t PARTNUM = 128, VERSION = 3;

void test_control_read(void) {
	uint8_t x;
	CC2500_Read_SR(&x, CC2500_PARTNAME);
	WIRELESS_TRY(x, PARTNUM);
	
	CC2500_Read_SR(&x, CC2500_VERSION);
	WIRELESS_TRY(x, VERSION);
}
