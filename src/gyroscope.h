//! Controls communication with the lsm9ds1 gyroscope

#ifndef __INCLUDE_GYROSCOPE_H_
#define __INCLUDE_GYROSCOPE_H_

/*!
 *  \brief Initializes the gyroscope on the lsm9ds1 board
 *
 *  Sets up the EXTI, NVIC and SPI needed by the lsm9ds1
 *  The lsm9ds1 triggers an interrupt whenever the gyroscope has data
 *  available.
 */
void init_gyroscope(void);

/*!
 * \brief Gyroscope Updating thread. Get signaled by IRQ from the lsm9ds1
 *
 * Waits on the lsm9ds1 data ready interrupt. Once data is ready, we read
 * the gyroscope to update the FilterBuffer containing it's values. This
 * updates the current heading and detects steps.
 */
void Gyroscope(void const *argument);

#endif //__INCLUDE_GYROSCOPE_H_
