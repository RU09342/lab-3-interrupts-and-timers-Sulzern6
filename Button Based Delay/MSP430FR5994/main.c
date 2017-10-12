#include <msp430fr5994.h>

#define LED1   BIT0     // Defines the LED at P1.0
#define BUTTON  BIT6    // Defines the BUTTON at P5.6
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
    P5REN = BUTTON;                       // Enables resistor for button P5.6
    P5OUT = BUTTON;                       // Makes resistor P5.6 a pull up
    P5IES |= BIT6;                     // Interrupts when BUTTON is pressed or low to high transition
    P5IE |= BIT6;                     // Interrupt enable set to BUTTON
    P5IFG &= ~BIT6;                   // Clear interrupt flag


    __bis_SR_register(GIE); //General interrupt enable

    TA0CCTL0 = CCIE;                    // Enables Capture/compare register for interrupt of timer A0
    TA0CCR0 = 250;                     //register that timer will compare itself too
    TA0CTL = TASSEL_1 + MC_1 + TACLR;

}

#pragma vector=PORT5_VECTOR
__interrupt void Port_5(void){
    if(!(P5IN & BUTTON))              // Check for button Press
    {
        P5IES &= ~BIT6; //sets interupt to trigger on rising edge (button release)
        TA0CTL = TASSEL_1 + MC_2 + TACLR;           // SMCLK, divide clock by 8, continous mode, clear TAR
        P5IFG = BIT0; //clear interupt flag
    }
    else
    {
        P5IES |= BIT6; //sets interupt to trigger on falling edge (button press)
        TA0CCR0 = TA0R; //changes ccr0 to value of timer
        TA0CTL = TASSEL_1 + MC_1 + TACLR;           // SMCLK, divide clock by 8, up mode, clear TAR
        P5IFG = BIT0; //clears interupt flag
     }
}

#pragma vector=TIMER0_A0_VECTOR
__interrupt void TIMER0_A0_ISR(void)
{
            P1OUT ^= LED1; //toggles LED when interupt triggered
}
