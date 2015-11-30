/*----------------------------------------------------------------------------
 * osObjects.h: CMSIS-RTOS global object definitions for an application
 *----------------------------------------------------------------------------
 *
 * This header file defines global RTOS objects used throughout a project
 *
 * #define osObjectsPublic indicates that objects are defined; without that
 * definition the objects are defined as external symbols.
 *
 *--------------------------------------------------------------------------*/

#ifndef __osObjects
#define __osObjects

#if (!defined (osObjectsPublic))
#define osObjectsExternal          // define RTOS objects with extern attribute
#endif

#include <cmsis_os.h>              // CMSIS RTOS header file

//signal values for signaling
typedef enum SIGNALS{
	SIGNAL_GYROSCOPE = 0x0001,
	SIGNAL_TEMP_ACCEL = 0x0002,
	SIGNAL_ACCELEROMETER = 0x0003,
	SIGNAL_DISPLAY = 0x0004
}SIGNALS;

// global 'thread' functions ---------------------------------------------------
/* 
*/

extern osThreadId gyroscope_thread;


#endif  // __osObjects
