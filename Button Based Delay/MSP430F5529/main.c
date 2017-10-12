#include <msp430F5529.h>

#define LED1    BIT0    // Defines the LED at P1.0
#define BUTTON  BIT1    // Defines BUTTON  at  P1.1
/**
 * Nathan Sulzer
 * Into to Embedded Systems
 * main.c
 */
int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;           // stop watchdog timer
    P1DIR  |= LED1;                     // Output Direction
    P1REN  |= BUTTON;                   // Enables resistor
    P1OUT  = BUTTON;                    // Makes resistor a pull up
    P1SEL = 0x00;

                                        // Interrupt Setting
    P1IES &= ~BUTTON;                   // Port 1 interrupt Edge Select: Sets the interrupt for BUTTON press
    P1IE |= BUTTON;                     // Interrupt enable set to BUTTON
    P1IFG &= ~BUTTON;                   // Clear interrupt flag


    __bis_SR_register(gie);             //general interrupts enabled


    TA0CCTL0 = 0x0010;                  // Enables Capture/compare register for interrupt of timer A0
    TA0CCR0 = 250;                      // Set value for Timer A0 to count up to
    TA0CTL = TASSEL_1 + MC_1 + TACLR;   // ACLK, timer stopped, Clear TAR


}


#pragma vector = PORT1_VECTOR
__interrupt void Interrupt_1(void)
{
    if(!(P1IN & BUTTON))                   // Check for button Press
    {
        P1IES = BIT1;                      // Interrupts when BUTTON is pressed or high to low transition
        TA0CTL = TASSEL_1 + MC_2 + TACLR;  // ACLK, timer stopped, Clear TAR
        P1IFG = 0x0000;                    // Clear interrupt flag

    }
    else
    {
        TA0CCR0 = TA0R;
        P1IFG = ~BIT1;                     // Clear interrupt flag
        TA0CTL = TASSEL_1 + MC_1 + TACLR;  // ACLK, timer stopped, Clear TAR
        P1IES = 0x0001;                    // Interrupts when BUTTON is pressed or low to high transition
    }
}


#pragma vector=TIMER0_A0_VECTOR
__interrupt void Interrupt_2(void)
{
    P1OUT ^= LED1;                      // Toggle LED
}
