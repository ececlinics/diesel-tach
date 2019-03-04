/**
* @author Robert Page
* @date 2/21/2019
*/

#include <stdint.h>
#include <msp430.h>
#include "binary_filt.h"
#include "binary_filt.c"

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    P1DIR |= BIT1; //1.1 data output
    P1DIR &= ~BIT0; //1.0 data input

    //16MHz clock
    BCSCTL1 = CALBC1_16MHZ; // Set range
    DCOCTL = CALDCO_16MHZ; // Set DCO step + modulation*/

    // TIMER INTERRUPT
    TA1CCTL0 = CCIE;            // CCR0 interrupt enabled
    TA1CTL = TASSEL_2 + MC_1;   // SMCLK/8, upmode
    TA1CCR0 =  500;							// 12.5 Hz

    _BIS_SR(CPUOFF + GIE);      // Enter LPM0 w/ interrupt

}

// Timer 1 A0 interrupt service routine
#pragma vector = TIMER1_A0_VECTOR
__interrupt void Timer1_A0_ISR( void )
    P1OUT = (P1OUT & ~BIT1)
					|(dyn_window_filt(P1IN & BIT0) << 1 & BIT1);

