#include <msp430fr6989.h>

#define LED1    BIT0    // Defines the LED at P1.0
#define BUTTON  BIT1    // Defines the BUTTON at P1.1
/**
 * Nathan Sulzer
 * Into to Embedded Systems
 * main.c
 */
void main(void) {
                                     //WDTCTL is watchdog timer control
                                    //WDTPW watchdog timer + password, is an interrupt
                                     //WDTHOLD watchdog timer +hold, if 1 watchdog timer is stopped
                                     // Stop watchdog timer
    WDTCTL = WDTPW | WDTHOLD;

                                      // Disable the GPIO power-on default high-impedance mode
                                       // to activate previously configured port settings
                                      //PM5CTL0 = Power mode 5 control register 0
                                       //LOCKLPM5 = Locks I/O pin, bit is reset by a power cycle
                                        //~LOCKLPM5=8'b0 and by anding PM5CTL0 it clears the register
    PM5CTL0 &= ~LOCKLPM5;



                                        //resistor enabled
    P1REN |= BUTTON;

                                            //Sets Resistor to pullup, 1.0 is low, has to use = to initialize
    P1OUT = BUTTON;

                                          //Enables port 1.0 as output, has to use = to initialize
    P1DIR =  LED1;
    P1IE |=  BUTTON;                           //enable interrupt on port 1.1
    P1IES |= BUTTON;                            //sets as falling edge
    P1IFG &=~(BUTTON);                     //clear interrupt flag

                                        //enter LPM4 mode and enable global interrupt
    _BIS_SR(LPM4_bits + GIE);


}

                                                   //Port 1 ISR
#pragma vector = PORT1_VECTOR
__interrupt void PORT_1(void)                        //double __
{
        P1OUT ^= LED1;                            // P1.0 = toggle
       P1IFG &= ~BUTTON;                           // P1.1 IFG cleared
}
