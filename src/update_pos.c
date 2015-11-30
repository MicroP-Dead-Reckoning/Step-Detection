#include "update_pos.h"

void updatePos(){
	distance += STEP_SIZE;
}

int sendPos(int direction){
	if (dist_last == distance)
		return 0;
	
	dist_last = distance;
	

}
