//! a moving average filter of adjustable length
#ifndef __INCLUDE_MOVING_AVERAGE_H_
#define __INCLUDE_MOVING_AVERAGE_H_

typedef struct {
	int size;
	float* buffer;
	int replace;
	float avg;
	float sum;
} FilterBuffer;

/*!
	\brief	adds a value to the filter
	\param	*buf the FilterBuffer to add f to
	\param	f newly sampled data 
 */
float add_value(FilterBuffer *buf, float f);

/*!
	\brief	returns average value
	\param	*buf the buffer for which average is needed 
 */
float get_value(FilterBuffer *buf);

#endif //__INCLUDE_MOVING_AVERAGE_H_
