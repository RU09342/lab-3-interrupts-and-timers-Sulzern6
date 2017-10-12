#include <msp430fr6989.h>

#define LED1    BIT0    // Defines the LED at P1.0
#define BUTTON  BIT1    // Defines the BUTTON at P1.1
#define LED2    BIT7    // Defines the LED at P9.2
/**
 * Nathan Sulzer
 * Into to Embedded Systems
 * main.c
 */
void main(void)
{
    WDTCTL = WDTPW + WDTHOLD; // Stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5;               // Disable GPIO default high-impedance mode
    P1SEL0 &= ~(LED1); // Select I/O direction for P1.0 and P1.1
    P1SEL1 &= ~(LED1);
    P1DIR  = LED1;               // Set  LED1 to output
    P9DIR  = LED2;               // Sets LED2 to output
    P1OUT &= ~(LED1); // Clear the LEDs
    P9OUT &= ~(LED2); // Clear the LEDs

    TA0CCTL0 = 0x0010;                  // Enables Capture/compare register for interrupt of timer B0
    TA0CCR0  = 60000;
    TA0CTL |= TASSEL_2 + MC_1; // Set the timer B to SMCLCK/8, Continuous
    // Clear the timer and enable timer interrupt
    __enable_interrupt();

    __bis_SR_register(LPM0 + GIE); // LPM0 with interrupts enabled

}


// Timer B0 interrupt service routine
#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A0 (void)
{
    P1OUT ^= LED1; // Toggle the LEDs
    P9OUT ^= LED2; // Toggle the LEDs
}
