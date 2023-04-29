#include <msp430.h>
#define PERIOD 1000     // PWM period in clock cycles
/**
 * main.c
 */
int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // Stop WDT
    P2DIR |= BIT1 + BIT0; // Need to set P2.0, P2.1 HIGH
    P2SEL = 0;
    P2SEL2 = 0;
    while (1) {
        P2OUT ^= BIT0;
        __delay_cycles(100000);
    }

    return 0;
}
