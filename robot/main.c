#include <msp430.h>
#define DELAY 1000     // PWM period in clock cycles
#define LEFT 10       // Left position duty cycle
#define CENTER 15     // Center position duty cycle
#define RIGHT 20      // Right position duty cycle
unsigned int move = 0;
unsigned int turn = 0;

/**
 * main.c
 */
int main(void)
{
    WDTCTL = WDTPW + WDTHOLD;   // Stop WDT

    // Input ports (add P1.1 later)
    P1DIR |= BIT0 + BIT1;  // Configure P1.0 and P1.1 as input ports (P1.0 for moving, P1.1 for turning)
    P1REN |= BIT0 + BIT1;    // Enable pull-up/down resistors on P1.0 and P1.1
    P1OUT |= BIT0 + BIT1;  // Set pull-up resistors on P1.0 and P1.1

    TA0CTL = TASSEL_1 + MC_1;  // ACLK, upmode
    TA0CCR0 = 10000;     // You'll need to set this to achieve 1 second interrupts!
    TA0CCTL0 = CCIE;    // CCR0 interrupt enabled

    TA1CTL = TASSEL_1 + MC_1;  // ACLK, upmode
    TA1CCR0 = CENTER; // 1.5ms duty cycle (center, default)
    TA1CCTL0 = CCIE;    // CCR0 interrupt enabled

    // Output ports (for controller and servo)
    P2DIR |= BIT5 + BIT3 + BIT1 + BIT0; // We need 2.5, 2.1, and 2.0 connected!

    P2SEL = 0;
    P2SEL2 = 0;


    while (1) {
        __bis_SR_register(LPM3_bits + GIE);       // Enter LPM3 w/ interrupt
        switch (move){
            case 0: // Forward State
                P2OUT ^= BIT0;  // Move Forward
                __delay_cycles(DELAY);
                if ((P1IN & BIT0)){ // Check if P1.0 is high
                    move = 1;
                    __delay_cycles(500000);  // Wait for 0.5 second for state transition
                }
                break;
            case 1: // IDLE State
                P2OUT &= ~BIT0;  // Stop Moving
                switch (turn){
                        case 0: // Turn left, need to add more stuff in here
                            TA0CCR1 = LEFT;
                            if ((P1IN & BIT1))  // Check if P1.1 is high
                                turn = 1;
                            break;
                        case 1: // Turn right, need to add more stuff in here
                            TA0CCR1 = RIGHT;
                            if (!(P1IN & BIT1)) // Check if P1.1 is low
                                turn = 0;
                            break;
                    }
                if (!(P1IN & BIT0)){ // Check if P1.0 is low
                    move = 0;
                    TA0CCR1 = CENTER;   //Reset to center
                    __delay_cycles(500000);  // Wait for 0.5 second
                }
                break;
        }
    }

    return 0;
}
