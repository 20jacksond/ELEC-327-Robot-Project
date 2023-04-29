#include <msp430.h>
#define PERIOD 1000     // PWM period in clock cycles
/**
 * main.c
 */
int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // Stop WDT

    TA0CTL = TASSEL_1 + MC_1;  // ACLK, upmode
    TA0CCR0 = 10000;     // You'll need to set this to achieve 1 second interrupts!
    TA0CCTL0 = CCIE;    // CCR0 interrupt enabled

    TA1CTL = TASSEL_1 + MC_1;  // ACLK, upmode
    TA1CCR0 = 10; // 1ms duty cycle (right)
    TA1CCTL0 = CCIE;    // CCR0 interrupt enabled

    P2DIR |= BIT5; // Need to set P2.0, P2.1 HIGH

    return 0;
}
