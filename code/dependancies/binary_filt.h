/**
* @author Robert Page
* @date 2/21/2019
*/

#ifndef BINARY_FILT_H_
#define BINARY_FILT_H_

#include <stdint.h>

//dynamic window filter settings
#define MAX_DATA_HISTORY 100         //Maximum window (max 256)
#define MAX_PULSEWIDTH_HISTORY 5	//Max history of pulse widths

//pulse width median filter settings
#define MEDIAN_WIDTH 5

/**
* @brief binary filter with a dynamic window
* 
* uses a history of the previous input pulse widths to identify an optimum window width.
* Then uses a median filter with that width to filter the output.
* 
* @param input input to the filter, usually from a sampled GPIO pin
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
* @warning needs revision, two buffers or better tracking of last value.
*/
uint8_t calc_period(uint8_t input);

#endif /* BINARY_FILT_H_ */
