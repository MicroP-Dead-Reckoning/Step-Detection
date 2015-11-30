#include "moving_average.h"


/*!
	adds a value to the filter
	@param f value to be added to buffer
 */
float add_value(FilterBuffer *buf, float f) {
	buf->sum -= buf->buffer[buf->replace % buf->size];
	buf->sum += (buf->buffer[buf->replace % buf->size] = f);
	buf->avg = buf->replace++ < buf->size 
											? buf->sum/buf->replace 
											: buf->sum/buf->size;
	return buf->avg;
}

/*!
	@return average value
 */
float get_value(FilterBuffer *buf) {
	return buf->avg;
}
