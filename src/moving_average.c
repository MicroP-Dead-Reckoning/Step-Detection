#include "moving_average.h"


/*!
	adds a value to the filter
	@param f value to be added to buffer
 */
int add_value(FilterBuffer *buf, float f) {
	buf->sum -= buf->buffer[buf->replace % BUFF_SIZE];
	buf->sum += (buf->buffer[buf->replace % BUFF_SIZE] = f);
	buf->avg = buf->replace++ < BUFF_SIZE 
											? buf->sum/buf->replace 
											: buf->sum/BUFF_SIZE;
	return 0;
}

/*!
	@return average value
 */
float get_value(FilterBuffer *buf) {
	return buf->avg;
}
