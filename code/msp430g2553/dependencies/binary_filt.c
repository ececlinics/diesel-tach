#include "binary_filt.h"

static uint8_t median_filt(uint8_t * start);

uint8_t dyn_window_filt(uint8_t input){
	
	static uint8_t data_history[MAX_DATA_HISTORY];
	static uint8_t pulse_history[MAX_PULSEWIDTH_HISTORY];
	static uint8_t next_data_index = 0, next_pulse_index = 0, pulse_sum = 0;
	uint8_t i,pulse_width = 0, dyn_sum = 0;
	
	//update history array
	data_history[next_data_index] = input;
	
	//calculate current pulse width
	if(input==1){
	    if(pulse_sum<=MAX_DATA_HISTORY)
	        pulse_sum += 1;
	}
	else if (pulse_sum>0){
		pulse_history[next_pulse_index]=pulse_sum>>1;
		pulse_sum = 0;
		
		//increment pulse history index
		if(next_pulse_index==MAX_PULSEWIDTH_HISTORY-1)
		    next_pulse_index = 0;
		else
			next_pulse_index += 1;
	}
	
	
	//find max pulse width out of buffer
	for(i = 0;i<MAX_PULSEWIDTH_HISTORY;i++){
		if(pulse_width<pulse_history[i])
			//set dynamic window to one pulse width
			pulse_width = pulse_history[i];
	}
	
	// sum dynamic window
	if(next_data_index>=pulse_width-1){//dynamic window does not wrap around
		for(i=next_data_index-pulse_width+1;i<next_data_index+1;i++){//sums the dynamic window
			dyn_sum += data_history[i];
		}
	}
	else if(pulse_width<MAX_DATA_HISTORY){//dynamic window does wrap around
		for(i=MAX_DATA_HISTORY+1+next_data_index-pulse_width;i!=next_data_index+1;i++){//sums the dynamic window
			if(i == MAX_DATA_HISTORY)
				i = 0;
			dyn_sum += data_history[i];
		}
	}
	
	//increment data history index
	if(next_data_index==MAX_DATA_HISTORY-1)
	    next_data_index = 0;
	else
		next_data_index += 1;
	
	
	// return data 1 or 0
	if(dyn_sum>pulse_width>>1)
		return 1;
	else
		return 0;
}

uint8_t calc_period(uint8_t input){
	static uint8_t sum_period = 0, period_history[MEDIAN_WIDTH], last_input = 0, next_data_index = 0;
	uint8_t i;
	
	//sums the period and 
	if(last_input == 0 && input == 1){
	    if(next_data_index==0){
			if(period_history[MEDIAN_WIDTH-1]==0){
				period_history[next_data_index] = sum_period;
                sum_period = 0;
			}			
            else if(sum_period<period_history[MEDIAN_WIDTH-1]>>1){
                period_history[next_data_index] = period_history[MEDIAN_WIDTH-1]-2;
                sum_period = 0;
            }
            else{
                period_history[next_data_index] = sum_period;
                sum_period = 0;
            }
	    }
	   else{
	        if(period_history[next_data_index-1]==0){
	            period_history[next_data_index] = sum_period;
	            sum_period = 0;
	        }	        			
			else if(sum_period<period_history[next_data_index-1]>>1){
	            period_history[next_data_index] = period_history[next_data_index-1]-2;
	            sum_period = 0;
	        }
	        else{
	            period_history[next_data_index] = sum_period;
	            sum_period = 0;
	        }
	    }
		
		if (next_data_index == MEDIAN_WIDTH-1)
			next_data_index = 0;
		else
			next_data_index++;
	}
	else{
	    if(sum_period<254)
	        sum_period++;
	    else{
	        for(i=0;i<MEDIAN_WIDTH;i++){
	            period_history[i] = 0;
	        }
	    }
	}	
	
	last_input = input;
	
	return median_filt(period_history);
}

static uint8_t median_filt(uint8_t * start){
	uint8_t i, j, temp, buffer[MEDIAN_WIDTH];
	
	//copy array
	for(i=0;i<MEDIAN_WIDTH;i++){
		buffer[i] = start[i];
	}
	
	//sort buffer
    for(i=0; i<MEDIAN_WIDTH-1; i++) {
        for(j=i+1; j<MEDIAN_WIDTH; j++) {
            if(buffer[j] < buffer[i]) {
                // swap elements
                temp = buffer[i];
                buffer[i] = buffer[j];
                buffer[j] = temp;
            }
        }
    }
	
	return buffer[MEDIAN_WIDTH>>1];
}
