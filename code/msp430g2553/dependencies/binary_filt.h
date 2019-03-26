/**
* @author Robert Page
* @date 2/21/2019
*/

#ifndef BINARY_FILT_H_
#define BINARY_FILT_H_

#ifndef __STM8S_H	// "stm8s.h" already includes "stdint.h"
	#include <stdint.h>
#endif

//dynamic window filter settings
#define MAX_DATA_HISTORY 100         //Maximum window (max 256)
#define MAX_PULSEWIDTH_HISTORY 5	//Max history of pulse widths

//pulse width median filter settings
#define MEDIAN_WIDTH 5

/**
* @brief binary filter, sets dynamic window to 50% of the pulse width
*
* designed to take input from a GPIO pin, where the GPIO pin is receiving a noisy square wave. 
* The filter will establish a buffer of the previous MAX_DATA_HISTORY samples, 
* and keep a buffer of the previous MAX_PULSEWIDTH_HISTORY pulse widths
*
* @param input input desired to be filtered, designed for only binary inputs
*
* @author Robert Page
* @date 2/21/2019
*/
uint8_t dyn_window_filt(uint8_t input);

/**
* @brief takes a sampled input and returns period
* 
* counts number of samples between 0->1 transitions
* 
* @param input input to calculator
* @return period of input in unit of samples
*
* @warning max period is 255 because of uint8_t 
*/
uint8_t calc_period(uint8_t input);

#endif /* BINARY_FILT_H_ */
