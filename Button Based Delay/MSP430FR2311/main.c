#include <msp430fr2311.h>

#define LED1    BIT0    // Defines the BIt corresponding to the LED used P1.0
#define BUTTON  BIT1    // Defines the BIT corresponding to the LED used P1.1
/**
 * Nathan Sulzer
 * Into to Embedded Systems
 * main.c
 */
int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;           // stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5;               // Disable GPIO default high-impedance mode
    P1DIR  |= LED1;                     // Set P1.6 Green LED to output
    P1SEL0 = 0x00;                      //sets P1 to GPIO
    P1SEL1 = 0x00;
    P1REN  = BUTTON;                   // Enables resistor for BUTTON P1.3
    P1OUT  = BUTTON;                    // Makes resistor P1.3 a pull up
    P1IES |= BUTTON;                     // Interrupts when BUTTON is pressed or low to high transition
    P1IE |= BUTTON;                     // Interrupt enable set to BUTTON
    P1IFG &= ~BUTTON;                   // Clear interrupt flag


    __bis_SR_register(GIE); //General interrupt enable

    TB0CCTL0 = CCIE;                    // Enables Capture/compare register for interrupt of timer A0
    TB0CCR0 = 6000;                     //register that timer will compare itself too
    TB0CTL = TBSSEL_1 + MC_1 + TBCLR;

}

#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void){
    if(!(P1IN & BUTTON))              // Check for button Press
    {
        P1IES &= ~BUTTON; //sets interupt to trigger on rising edge (button release)
        TB0CTL = TBSSEL_1 + MC_2 + TBCLR;           // SMCLK, divide clock by 8, continous mode, clear TAR
        P1IFG &= ~BUTTON; //clear interupt flag
    }
    else
    {
        P1IES |= BUTTON; //sets interupt to trigger on falling edge (button press)
        TB0CCR0 = TB0R; //changes ccr0 to value of timer
        TB0CTL = TBSSEL_1 + MC_1 + TBCLR;           // SMCLK, divide clock by 8, up mode, clear TAR
        P1IFG &= ~BUTTON; //clears interupt flag
     }
}

#pragma vector=TIMER0_B0_VECTOR
__interrupt void TIMER0_A0_ISR(void)
{
            P1OUT ^= LED1; //toggles LED when interupt triggered
}
