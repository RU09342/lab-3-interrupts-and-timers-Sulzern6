#include <msp430g2553.h>

#define LED1    BIT0    // Defines LED at P1.0
#define LED2    BIT6    // Defines LED at P1.6
#define BUTTON  BIT3    // Defines BUTTON at P1.3
/**
 * Nathan Sulzer
 * Into to Embedded Systems
 * main.c
 */
int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;       // stop watchdog timer
    P1SEL = 0x00;                   //sets P1 to GPIO
    P1SEL2 = 0x00;
    P1DIR = LED1;                   //sets LED at P1.0 to output
    P1REN = BUTTON;                 //Resistor enabled for button P1.3
    P1OUT = BUTTON;                 //Sets resistor to pull up for button P1.3 when not pressed
    P1IE |= BUTTON;                 //interrupt enable for button P1.3
    P1IES |= BUTTON;                //sets interrupt for Low/high transition
    P1IFG &= ~BUTTON;               //Clears interrupt flag register

    __bis_SR_register(GIE);         //General interrupt enable

    TA0CCTL0 = CCIE;                // Enables Capture/compare register for interrupt of timer A0
    TA0CCR0 = 6000;                 // register that timer will compare itself too
    TA0CTL = TASSEL_1 + MC_1 + TACLR;           // ACLK, continous mode, clear TAR


}

#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{
    if(!(P1IN & BUTTON))                 // Check for button Press
    {
    P1IES &= ~BUTTON;                   //sets interrupt to low/high transition
    TA0CTL = TASSEL_1 + MC_2 + TACLR;   // ACLK, up mode, clear TAR
    P1IFG = 0x0000;                     //clear interrupt flag
    }
    else
    {
    P1IES |= BUTTON;                    //sets interrupt to trigger on falling edge (button press)
    TA0CCR0 = TA0R;                     //Sets the timer for the button press to CCR0
    TA0CTL = TASSEL_1 + MC_1 + TACLR;   // ACLK, up mode, clear TAR
    P1IFG &= ~BUTTON;                   //clears interrupt flag
    }
}

#pragma vector=TIMER0_A0_VECTOR
__interrupt void TIMER0_A0_ISR(void)
{
            P1OUT ^= LED1;              //Toggle LED
}
