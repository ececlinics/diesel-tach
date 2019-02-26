/**
* @author Robert Page
* @date 2/21/2019
*/

#ifndef BINARY_FILT_H_
#define BINARY_FILT_H_

#include <stdint.h>

//filter settings
#define MAX_DATA_HISTORY 80         //Maximum window (max 256)
#define MAX_PULSEWIDTH_HISTORY 5	//Max history of pulse widths

uint8_t dyn_window_filt(uint8_t input);

#endif /* BINARY_FILT_H_ */
