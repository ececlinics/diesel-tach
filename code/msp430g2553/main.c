/**
* @author Robert Page
* @date 2/21/2019
*/


#include <stdint.h>
#include <msp430.h>
#include "binary_filt.h"

uint8_t i = 0, period;

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    P1DIR |= BIT1; //1.1 data output
    P1DIR &= ~BIT0; //1.0 data input

    // initialize port 2 and 3
    P2DIR = 0xFF;
    P2OUT = 0x00;
    P3DIR = 0xFF;
    P3OUT = 0x00;

    //16MHz clock
    BCSCTL1 = CALBC1_16MHZ; // Set range
    DCOCTL = CALDCO_16MHZ; // Set DCO step + modulation*/

    // TIMER INTERRUPT
    TA1CCTL0 = CCIE;                             // CCR0 interrupt enabled
    TA1CTL = TASSEL_2 + MC_1;           // SMCLK/8, upmode
    TA1CCR0 =  500;                     // 12.5 Hz

    _BIS_SR(CPUOFF + GIE);          // Enter LPM0 w/ interrupt

}

// Timer 1 A0 interrupt service routine
#pragma vector = TIMER1_A0_VECTOR
__interrupt void Timer1_A0_ISR( void ){
    period = calc_period(dyn_window_filt(P1IN & BIT0));

    // software PWM
    if(i>=period>>1)
        P1OUT &= ~BIT1;
    else
        P1OUT |= BIT1;

    //increment i
    if(i>period-1)
        i = 0;
    else
        i++;

    //P1OUT = (P1OUT & ~BIT1)|(dyn_window_filt(P1IN & BIT0)<<1 & BIT1);
}


