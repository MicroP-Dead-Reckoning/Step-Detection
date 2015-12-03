#ifndef __INCLUDE_GYROSCOPE_H_
#define __INCLUDE_GYROSCOPE_H_

/*!
	Initializes the gyroscope on the lsm9ds1 board
 */
void init_gyroscope(void);

/*!
	Gyroscope Updating thread. Get signaled by IRQ from the lsm9ds1
 */
void Gyroscope(void const *argument);

#endif //__INCLUDE_GYROSCOPE_H_
