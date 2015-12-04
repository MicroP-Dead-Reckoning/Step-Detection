#include "update_pos.h"
#include "cc2500.h"

#define NUM_STEPS 100

//uint8_t distance = 0;
//uint8_t dist_last = 0;
uint8_t direction[NUM_STEPS];
uint8_t distance[NUM_STEPS];
int8_t steps = -1;

/*!
 Function handles the updating of distance (number of steps)
*/
void update_pos(uint8_t new_direction){
	if (direction[steps] != new_direction) {
		steps++;
		direction[steps] = new_direction;
		distance[steps] = 0;
	}
	distance[steps]++;
}

/*!
 Function handles the sending of data
*/
int send_pos(){
	CC2500_Write(direction, CC2500_FIFO_REG, 1);
	CC2500_Write(distance, CC2500_FIFO_REG, 1);
	return 0;
}
