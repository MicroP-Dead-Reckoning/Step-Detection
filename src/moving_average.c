#include "moving_average.h"

/*!
	\brief	adds a value to the filter
	\param	*buf the FilterBuffer to add f to
	\param	f newly sampled data 
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
	\brief	returns average value
	\param	*buf the buffer for which average is needed 
 */
float get_value(FilterBuffer *buf) {
	return buf->avg;
}
