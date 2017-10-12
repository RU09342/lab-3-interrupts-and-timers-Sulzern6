#include <msp430fr2311.h>

#define LED1    BIT0    // Defines the BIt corresponding to the LED used P1.0
#define BUTTON  BIT1    // Defines the BIT corresponding to the LED used P1.1
/**
 * Nathan Sulzer
 * Into to Embedded Systems
 * main.c
 */
void main(void)
{


    WDTCTL = WDTPW | WDTHOLD;           // stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5;               // Disable GPIO default high-impedance mode
    P1DIR  |= LED1;                     // Set P1.0 LED to output
    P1REN  |= BUTTON;                   // Enables resistor for BUTTON P1.1
    P1OUT  = BUTTON;                    // Makes resistor P1.1 a pull up
    P1IES &= ~0x10;                     // Interrupts when BUTTON is pressed or low to high transition
    P1IE |= BUTTON;                     // Interrupt enable set to BUTTON
    P1IFG &= ~BUTTON;                   // Clear interrupt flag


    _BIS_SR(LPM4_bits + GIE);           //low power mode and General Interrupt Enable


}

                                                  //Port 1 Interrupt
#pragma vector = PORT1_VECTOR
__interrupt void PORT_1(void)                     //double __
{
       P1OUT ^= LED1;                             // Toggle LED at P1.0
       P1IFG &= ~BUTTON;                          // Clear interrupt flag
}
