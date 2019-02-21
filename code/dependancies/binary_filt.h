/*
 * binary_filt.h
 *
 *  Created on: Feb 19, 2019
 *      Author: Jack
 */

#ifndef BINARY_FILT_H_
#define BINARY_FILT_H_

#include <stdint.h>

//filter settings
#define MAX_DATA_HISTORY 250
#define MAX_PULSEWIDTH_HISTORY 5	

uint8_t dyn_window_filt(uint8_t input);

#endif /* BINARY_FILT_H_ */
