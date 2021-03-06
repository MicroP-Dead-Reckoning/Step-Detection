#include "update_pos.h"
#include <stdio.h>
#include "cc2500.h"

#define TOTAL_STEPS 100

uint8_t direction[TOTAL_STEPS];
uint8_t distance[TOTAL_STEPS];
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
	for (int i = 0; i <= steps; i++) {
		printf("%d: dir: %d, dist: %d\n", i, direction[i], distance[i]);
	}
	
	CC2500_Write(direction, CC2500_FIFO_REG, 1);
	CC2500_Write(distance, CC2500_FIFO_REG, 1);
	return 0;
}
