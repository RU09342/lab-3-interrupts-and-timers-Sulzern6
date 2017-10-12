#include <msp430fr5994.h>

#define LED1   BIT0     // Defines the LED at P1.0
#define LED2   BIT1     // Defines the LED at P1.1
#define BUTTON  BIT6    // Defines the BUTTON at P5.6
/**
 * Nathan Sulzer
 * Into to Embedded Systems
 * main.c
 */
void main(void)
{
    WDTCTL = WDTPW + WDTHOLD; // Stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5;     // Disable GPIO

    P1SEL0 &= ~(LED1 + LED2); // Select I/O direction for P1.0 and P1.1
    P1SEL1 &= ~(LED1 + LED2);
    P1DIR |= LED1 + LED2; // Set P1.0 and P1.1 to output direction
    P1OUT &= ~(LED1 + LED2); // Clear the LEDs

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
    P1OUT ^= LED1 + LED2; // Toggle the LEDs
}
