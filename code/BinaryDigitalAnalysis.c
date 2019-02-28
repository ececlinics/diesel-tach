#include <msp430.h>
<<<<<<< HEAD
#include "BDA.h"
#include <stdint.h>
=======
#include <stdint.h>
#include "BDA.h"
#define MAX_DATA_HISTORY 500
#define MAX_PULSEWIDTH_HISTORY 5
>>>>>>> 5d83ede4b4cf5c533ee60fe09463de7bf7d62531

int BinaryDigitalAnalysis(uint8_t output);

int main(void)
{
<<<<<<< HEAD
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    P1DIR |= BIT1; //1.1 data output
    P1OUT |= BIT1; //1.1 data outpu
    P1DIR &= ~DR; //1.0 data input

    while(1){
        //BinaryDigitalAnalysis(P1IN & ~DR);
        if(dyn_window_filt(P1IN & DR))
        {
            P1OUT |= BIT1;
        }
        else
        {
            P1OUT &= ~BIT1;
        }

    }
    return 0;
=======
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	P1DIR |= BIT1; //1.1 data output
	P1DIR &= ~DR; //1.0 data input

	while(1)
	    BinaryDigitalAnalysis(P1IN & ~DR);

	return 0;
>>>>>>> 5d83ede4b4cf5c533ee60fe09463de7bf7d62531
}

int BinaryDigitalAnalysis(uint8_t input)
{
    //static uint8_t window = 180;
    //std::vector<uint8_t> history(window)
    static uint8_t history[window];
    static uint8_t *oldData = history + window; // Setup pointers for the first
    static uint8_t *newData = history;          // and last elements in the array.
    static uint8_t sum;
    *newData = DR;      // writes to the array
    sum = sum + *newData - *oldData;

    if(newData != window - 1)
        newData++;
    else
        newData = 0;

    if(oldData != window - 1)
        oldData++;
    else
        oldData = 0;

    return sum;
}

uint8_t dyn_window_filt(uint8_t input){

    static uint8_t data_history[MAX_DATA_HISTORY];
    static uint8_t pulse_history[MAX_PULSEWIDTH_HISTORY];
    static uint8_t next_data_index = 0, next_pulse_index = 0, pulse_sum = 0;
<<<<<<< HEAD
    uint8_t i,pulse_width = 0, dyn_sum = 0;

=======
    uint8_t i, pulse_width = 0, dyn_sum = 0;
    
>>>>>>> 5d83ede4b4cf5c533ee60fe09463de7bf7d62531
    //update history array
    data_history[next_data_index] = input;

    //calculate current pulse width
    if(input == 1)
        pulse_sum += 1;
<<<<<<< HEAD
    else{
=======
    else
    {
>>>>>>> 5d83ede4b4cf5c533ee60fe09463de7bf7d62531
        pulse_history[next_pulse_index]=pulse_sum;
        pulse_sum = 0;

        //increment pulse history index
        if(next_data_index==MAX_PULSEWIDTH_HISTORY-1)
            next_pulse_index += 1;
        else
            next_pulse_index = 0;
    }


    //find max pulse width out of buffer
    for(i = 0; i < MAX_PULSEWIDTH_HISTORY; i++){
        if(pulse_width < pulse_history[i])
            pulse_width = pulse_history[i];
    }

    // sum dynamic window
    if(next_data_index >= pulse_width - 1)  //dynamic window does not wrap around
    {
        // sum the dynamic window
        for(i = next_data_index - pulse_width + 1; i < next_data_index + 1; i++)
            dyn_sum += data_history[i];
    }
    else    //dynamic window does wrap around
    {
        // sum the dynamic window
        for(i = MAX_DATA_HISTORY - 1 + next_data_index - pulse_width; i != next_data_index + 1; i++)
        {
            if(i = MAX_DATA_HISTORY)
                i = 0;
            dyn_sum += data_history[i];
        }
    }


    //increment data history index
<<<<<<< HEAD
    if(next_data_index==MAX_DATA_HISTORY-1)
        next_data_index += 1;
=======
    if(next_data_index == MAX_DATA_HISTORY - 1)
        next_data += 1;
>>>>>>> 5d83ede4b4cf5c533ee60fe09463de7bf7d62531
    else
        next_data_index = 0;


    // return data 1 or 0
    if(dyn_sum > pulse_width / 2)
        return 1;
    else
        return 0;
}