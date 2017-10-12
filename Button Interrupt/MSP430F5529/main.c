#include <msp430F5529.h>

#define LED1    BIT0    // Defines the LED at 1.0
#define BUTTON  BIT1    // Defines BUTTON at  1.1
/**
 * Nathan Sulzer
 * Into to Embedded Systems
 * main.c
 */
void main(void)
{
    WDTCTL = WDTPW + WDTHOLD;                 // Stop watchdog timer
    P1DIR |=  LED1;                           // Set LED at P1.0 to output direction
    P1IE  |=  BUTTON;                         // Interrupt enabled for Port 1
    P1IES |=  BIT0;                           // Sets interrupt for low/high transition
    P1REN |=  BUTTON;                         // Enable resistor for BUTTON at P1.1
    P1OUT |=  BUTTON;                         // Sets Resistor at P1.1 to Pull up
    P1IFG &= ~BUTTON;                         // P1.1 Interrupt Flag cleared
                                              //BIT1 on Port 1 can be used as Switch2

    __bis_SR_register(LPM4_bits + GIE);       // Low power mode and General Interrupt Enable

}

  // Port 1 interrupt service routine
  #pragma vector=PORT1_VECTOR
  __interrupt void Port_1(void)

  {
    P1OUT ^= LED1;                             // Toggle LED at P1.0
    P1IFG &= ~BUTTON;                          // Clear the interrupt Flag
}
