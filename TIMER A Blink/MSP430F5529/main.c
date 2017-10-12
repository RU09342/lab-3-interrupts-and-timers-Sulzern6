#include <msp430F5529.h>

#define LED1    BIT0    // Defines the LED at P1.0
#define LED2   BIT7     // Defines the LED at P4.7
#define BUTTON  BIT1    // Defines BUTTON  at P1.1
/**
 * Nathan Sulzer
 * Into to Embedded Systems
 * main.c
 */
void main(void)
{
    WDTCTL = WDTPW + WDTHOLD;   // Stop watchdog timer
    P1SEL &= ~(LED1 + LED2);    // Select I/O direction of Port 1
    P1DIR |= LED1;              // Set P1.0  to output direction
    P4DIR |= LED2;
    P1OUT &= ~(LED1);           // Output the LEDs
    P4OUT &= ~(LED2);

    TA0CCTL0 = 0x0010;          // Enables Capture/compare register for interrupt of timer B0
    TA0CCR0  = 60000;
    TA0CTL |= TASSEL_2 + MC_1;  // Set the timer A to SMCLCK, Continuous
                                // Clear the timer and enable timer interrupt
    __enable_interrupt();

    __bis_SR_register(LPM0 + GIE); // LPM0 with interrupts enabled

}


// Timer A0 interrupt service routine
#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A0 (void)
{
    P1OUT ^= LED1; // Toggle the LEDs
    P4OUT ^= LED2;
}
