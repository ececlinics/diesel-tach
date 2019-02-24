#include <stdint.h>
#include <msp430.h>
#include "binary_filt.h"

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    P1DIR |= BIT1; //1.1 data output
    P1DIR &= ~BIT0; //1.0 data input

    //16MHz clock
    BCSCTL1 = CALBC1_16MHZ; // Set range
    DCOCTL = CALDCO_16MHZ; // Set DCO step + modulation*/

    //PWM
    P1DIR |= BIT2;
    P1SEL |= BIT2;
    TA0CCTL1 = OUTMOD_7;
    TA0CCR0 = (10000 - 1);
    TA0CCR1 = 5000;
    TA0CTL = (TASSEL_2 | MC_1);

    while(1){
        if(dyn_window_filt(P1IN & BIT0)){
            P1OUT |= BIT1;
        }
        else{
            P1OUT &= ~BIT1;
        }
    }
}
