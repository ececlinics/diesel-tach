#include <stdint.h>

int BinaryDigitalAnalysis(uint8_t output);

int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	P1DIR |= BIT1; //1.1 data output
	P1DIR &= ~DR; //1.0 data input

	while(1){
	    BinaryDigitalAnalysis(P1IN & ~DR);
	}
	return 0;
}
int BinaryDigitalAnalysis(uint8_t input)
{
    //static uint8_t window = 180;
    //std::vector<uint8_t> histo5ry(window)
    static uint8_t history[window];
    static uint8_t *oldData = history + window, *newData = history; //Setup pointers for the first (oldData) and last (newData) elements in the array,
    static uint8_t sum;
    *newData = DR; //writes to the array
    sum = sum + *newData - *oldData;
    if(newData != window - 1)
    {
        newData++;
    }
    else{
        newData = 0;
    }
    if(oldData != window - 1)
        {
            oldData++;
        }
        else{
            oldData = 0;
        }

    return sum;
}
