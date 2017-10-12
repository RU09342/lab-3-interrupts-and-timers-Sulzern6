#include <msp430fr2311.h>

#define LED   BIT0
/**
 * main.c
 */
void main(void)
{
    WDTCTL = WDTPW + WDTHOLD; // Stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5;               // Disable GPIO default high-impedance mode
    P1SEL0 &= ~(LED);                   // Select I/O direction for P1.0 and P1.1
    P1SEL1 &= ~(LED);
    P1DIR  = LED;                       // Set  LED on p1 to output
    P2DIR  = LED;                       // Set  LED on p2 to output
    P1OUT &= ~(LED); // Clear the LEDs
    P2OUT &= ~(LED);
    TB0CCTL0 = 0x0010;                  // Enables Capture/compare register for interrupt of timer B0
    TB0CCR0  = 60000;
    TB0CTL |= TBSSEL_2 + MC_1; // Set the timer B to SMCLCK/8, Continuous
    // Clear the timer and enable timer interrupt
    __enable_interrupt();

    __bis_SR_register(LPM0 + GIE); // LPM0 with interrupts enabled

}


// Timer B0 interrupt service routine
#pragma vector=TIMER0_B0_VECTOR
__interrupt void Timer_B0 (void)
{
    P1OUT ^= LED; // Toggle the LED
    P2OUT ^= LED; // Toggle the LED
}
