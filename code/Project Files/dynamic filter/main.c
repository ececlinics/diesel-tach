#include <stdint.h>
#include <msp430.h>
#include "binary_filt.h"

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    P1DIR |= BIT1; //1.1 data output
    P1DIR &= ~BIT0; //1.0 data input

    while(1){
        if(dyn_window_filt(P1IN & BIT0)){
            P1OUT |= BIT1;
        }
        else{
            P1OUT &= ~BIT1;
        }
    }
}
