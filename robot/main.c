#include <msp430.h>
#define PERIOD 1000     // PWM period in clock cycles
#define DUTY_CYCLE_2 250  // PWM duty cycle for P2.5
#define LEFT 1000       // Left position duty cycle (need to change this)
#define CENTER 1500     // Center position duty cycle (need to change this)
#define RIGHT 2000      // Right position duty cycle (need to change this)
unsigned int move = 0;
unsigned int turn = 0;

/**
 * main.c
 */
int main(void)
{
    WDTCTL = WDTPW + WDTHOLD;   // Stop WDT
    BCSCTL3 |= LFXT1S_2;                    // ACLK = VLO

    // Input ports (add P1.1 later)
    P1DIR |= BIT0;  // Configure P1.0 and P1.1 as input ports
    P1REN |= BIT0;    // Enable pull-up/down resistors on P1.0 and P1.1
    P1OUT |= BIT0;  // Set pull-up resistors on P1.0 and P1.1

    // Output ports (for controller and servo)
    P2DIR |= BIT5 + BIT3 + BIT0; // We need 2.5, 2.3, and 2.0 connected!
    TA0CCR0 = PERIOD - 1;      // Set PWM period
    TA0CCR1 = CENTER;           // Set initial duty cycle for P2.0
    TA0CCR2 = DUTY_CYCLE_2 - 1; // Initial Value for P2.5; Move forward
    TA0CCTL1 = OUTMOD_7;        // Set output mode to reset/set for P2.0
    TA0CCTL2 = OUTMOD_7;       // Set output mode to reset/set for P2.5
    TA0CTL = TASSEL_2 + MC_1;  // Set timer source to SMCLK and count up mode

    while (1) {
        __bis_SR_register(LPM3_bits + GIE);       // Enter LPM3 w/ interrupt
        switch (move){
            case 0: // Forward State
                TA0CCR2 = DUTY_CYCLE_2 - 1; // Move forward; P2.5
                P2OUT |= BIT3;  // Indicate move forward
                if ((P1IN & BIT1) == 0){ // Check if P1IN is high
                    move = 1;
                    __delay_cycles(500000);  // Wait for 0.5 second
                }
                break;
            case 1: // IDLE State
                TA0CCR2 = 0; // Stop; P2.5
                P2OUT &= ~BIT3;  // Indicate turn
                switch (turn){
                        case 0: // Turn left, need to add more stuff in here
                            TA0CCR1 = LEFT;
                        case 1: // Turn right, need to add more stuff in here
                            TA0CCR1 = RIGHT;
                            break;
                    }
                if ((P1IN & BIT0) == 0){ // Check if P1IN is low
                    move = 0;
                    __delay_cycles(500000);  // Wait for 0.5 second
                }
                break;
        }
    }

    return 0;
}
