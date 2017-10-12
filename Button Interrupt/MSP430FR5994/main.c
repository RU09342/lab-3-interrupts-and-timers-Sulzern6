#include <msp430fr5994.h>

#define LED1   BIT0     // Defines the LED at P1.0
#define BUTTON  BIT6    // Defines the BUTTON at P5.6
/**
 * Nathan Sulzer
 * Into to Embedded Systems
 * main.c
 */
void main(void)
{
    WDTCTL = WDTPW | WDTHOLD;             // stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5;                 // Disable GPIO default high-impedance mode
    P1DIR  = LED1;                        // Set P1.0 RED LED to output
    P5REN = BUTTON;                       // Enables resistor for button P5.6
    P5OUT = BUTTON;                       // Makes resistor P5.6 a pull up

    P5IES = 0x0000;                     // Interrupts when BUTTON is pressed or low to high transition
    P5IE |= BUTTON;                     // Interrupt enable set to BUTTON
    P5IFG &= ~BUTTON;                   // Clear interrupt flag

                                        //enter LPM4 mode and enable global interrupt
    _BIS_SR(LPM4_bits + GIE);


}

                                                   //Port 1 ISR
#pragma vector = PORT5_VECTOR
__interrupt void PORT_5(void)                        //double __
{
       P1OUT ^= LED1;                            // P1.0 = toggle
       P5IFG &= ~BUTTON;                           // P1.1 IFG cleared
}

