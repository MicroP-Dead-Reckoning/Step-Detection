#include "update_pos.h"

void updatePos(){
	distance =+ STEP_SIZE;
}

int sendPos(uint8_t direction){
	if (dist_last == distance)
		return 0;
	
	dist_last = distance;
	dir_last = direction;
	
	uint8_t dist_send[2] = {distance >> 8, (distance & 0x00FF)};
	
	CC2500_Write(dist_send, 	CC2500_FIFO_REG, 2);
	CC2500_Write(&direction, CC2500_FIFO_REG, 1);
	return 1;
}
