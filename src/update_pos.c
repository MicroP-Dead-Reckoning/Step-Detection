#include "update_pos.h"

uint8_t distance = 0;
uint8_t dist_last = 0;
uint8_t dir_last = 0;

/*!
 Function handles the updating of distance (number of steps)
*/
void update_pos(){
	distance++;
}
/*!
 Function handles the sending of data
*/
int send_pos(uint8_t direction){
	if (dir_last == direction)
	{
		update_pos();
		return 0;
	}else{
		CC2500_Write(&distance, CC2500_FIFO_REG, 1);
		CC2500_Write(&dir_last, CC2500_FIFO_REG, 1);
		distance = 1;
		dir_last = direction;
	}
	
	return 0;
}
