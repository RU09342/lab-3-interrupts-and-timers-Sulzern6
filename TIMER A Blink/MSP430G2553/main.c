#include <msp430g2553.h>

#define LED1    BIT0    // Defines LED at P1.0
#define LED2    BIT6    // Defines LED at P1.6
#define BUTTON  BIT3    // Defines BUTTON at P1.3
/**
 * Nathan Sulzer
 * Into to Embedded Systems
 * main.c
 */
void main(void)
{
    WDTCTL = WDTPW + WDTHOLD;       // Stop watchdog timer

    P1SEL &= ~(LED1 + LED2);        // Select I/O direction for P1.0 and P1.1
    P1SEL2 &= ~(LED1 + LED2);
    P1DIR |= LED1 + LED2;           // Set P1.0 and P1.1 to output direction
    P1OUT &= ~(LED1 + LED2);        // Clear the LEDs

    TA0CCTL0 = 0x0010;               // Enables Capture/compare register for interrupt of timer B0
    TA0CCR0  = 60000;
    TA0CTL |= TASSEL_2 + MC_1;      // Set the timer B to SMCLCK/8, Continuous
                                    // Clear the timer and enable timer interrupt
    __enable_interrupt();

    __bis_SR_register(LPM0 + GIE);  // LPM0 with interrupts enabled

}


                                    // Timer A0 interrupt service routine
#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A0 (void)
{
    P1OUT ^= LED1 + LED2;           // Toggle the LEDs
}
