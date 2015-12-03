#include "update_pos.h"

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
		CC2500_Write(&direction, CC2500_FIFO_REG, 1);
		distance = 0;
		dir_last = direction;
	}
	
	return 0;
}
