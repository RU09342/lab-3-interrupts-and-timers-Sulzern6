# Nathan Sulzer
# Embedded Systems
# Lab 3
# Button Interrupt
The purpose of this task was to write a code that works just as the Button Blink code had without blinking instead this code uses the button to toggle the LED and implements this in a way that saves processor power. Unlilke prior codes this funcion utilizes the interrupt registers that only run the code when the button is pressed and a change is nessesary running in low power mode when no inputs are being enter.
# Example Code
This code functions by toggling an LED on and off on the press of a button and remaining in low power mode when no button is pressed.
```c
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


    WDTCTL = WDTPW | WDTHOLD;           // stop watchdog timer
    P1DIR  |= LED1;                     // Set P1.0 LED to output
    P1REN  |= BUTTON;                   // Enables resistor for BUTTON P1.3
    P1OUT  = BUTTON;                    // Makes resistor P1.3 a pull up
    P1IES = 0x0000;                     // Interrupts when BUTTON is pressed or low to high transition
    P1IE |= BUTTON;                     // Interrupt enable set to BUTTON
    P1IFG &= ~BUTTON;                   // Clear interrupt flag


    _BIS_SR(LPM4_bits + GIE);           // General Interrupt enable and low power mode


}

                                        //Port 1 Interrupt
#pragma vector = PORT1_VECTOR
__interrupt void PORT_1(void)
{
        P1OUT ^= LED1;                   // toggle LED
       P1IFG &= ~BUTTON;                 // Clear interrupt flag
}
```