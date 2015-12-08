#ifndef _INCLUDE_UPDATE_POS_H_
#define _INCLUDE_UPDATE_POS_H_

#include "stm32f4xx.h"                  		// Device header
#include "stm32f4xx_conf.h"
/*!
 *  \brief record that a new step occured
 *  \param new_direction the direction a step was taken
 *
 *  Whenever a step is taken, we have two cases.
 *	1. Same direction as the last step
 *	2. A new direction
 *  If the step was in the same direction, we say that 
 *  we went one unit farther in that direction
 *
 *  Otherwise, we say that we've taken one step in the new
 *  direction
 */
void update_pos(uint8_t new_direction);

/*!
 *  \brief transmits the recorded data via the CC2500
 */
int send_pos(void);

#endif // _INCLUDE_UPDATE_POS_H_
