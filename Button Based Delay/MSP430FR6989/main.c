#include <msp430fr6989.h>

#define LED1    BIT0    // Defines the LED at P1.0
#define BUTTON  BIT1    // Defines the BUTTON at P1.1
/**
 * Nathan Sulzer
 * Into to Embedded Systems
 * main.c
 */
int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;           // stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5;               // Disable GPIO default high-impedance mode
    P1DIR  |= LEDG;                     // Set P1.6 Green LED to output
    P1SEL0 = 0x00;                      //sets P1 to GPIO
    P1SEL1 = 0x00;
    P1REN  |= BUTTON;                   // Enables resistor for BUTTON P1.3
    P1OUT  = BUTTON;                    // Makes resistor P1.3 a pull up
    P1IES = 0x0000;                     // Interrupts when BUTTON is pressed or low to high transition
    P1IE |= BUTTON;                     // Interrupt enable set to BUTTON
    P1IFG &= ~BUTTON;                   // Clear interrupt flag


    __bis_SR_register(GIE); //General interrupt enable

    TA0CCTL0 = CCIE;                    // Enables Capture/compare register for interrupt of timer A0
    TA0CCR0 = 6000;                     //register that timer will compare itself too
    TA0CTL = TASSEL_1 + MC_1 + TACLR;

}

#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void){
    if(!(P1IN & BUTTON))              // Check for button Press
    {
        P1IES &= ~BUTTON; //sets interupt to trigger on rising edge (button release)
        TA0CTL = TASSEL_1 + MC_2 + TACLR;           // SMCLK, divide clock by 8, continous mode, clear TAR
        P1IFG = 0x0000; //clear interupt flag
    }
    else
    {
        P1IES |= BUTTON; //sets interupt to trigger on falling edge (button press)
        TA0CCR0 = TA0R; //changes ccr0 to value of timer
        TA0CTL = TASSEL_1 + MC_1 + TACLR;           // SMCLK, divide clock by 8, up mode, clear TAR
        P1IFG = 0x0000; //clears interupt flag
     }
}

#pragma vector=TIMER0_A0_VECTOR
__interrupt void TIMER0_A0_ISR(void)
{
            P1OUT ^= LEDG; //toggles LED when interupt triggered
}
