#include <msp430.h> 

unsigned int move = 0;
unsigned int turn = 0;

/**
 * main.c
 */
int main(void)
{
	BCSCTL3 |= LFXT1S_2;                    // ACLK = VLO

	// Input ports (add P1.1 later)
	P1DIR |= BIT0;  // Configure P1.0 and P1.1 as input ports
	P1REN |= BIT0 ;    // Enable pull-up/down resistors on P1.0
    P1OUT |= BIT0; 	// Set pull-up resistors on P1.0


    // Set up WDT interrupt for helpfulness
    WDTCTL = WDTPW + WDTHOLD;
    IE1 |= WDTIE;               // Enable WDT interrupt

	// Output ports (for controller and servo)
	P2DIR |= BIT5 + BIT0; // We need 2.5 and 2.0 connected!
    P2SEL |= BIT5; // P2.5 is TA1.2 PWM output
	P2SEL2 |= BIT0; // P2.0 is TA1.0 PWM output
	TA1CCR2 = 0;
	TA1CCR0 = 0;
    TA1CCTL2 = OUTMOD_7;
    TA1CTL = TASSEL_2 + MC_1; // SMCLK, upmode

	while (1) {
		__bis_SR_register(LPM3_bits + GIE);       // Enter LPM3 w/ interrupt
		switch (move){
			case 0:	// Forward State
				TA1CCR0 = 1; // Move forward
				if ((P1IN & BIT1) == 0) // Check if P1IN is high
					move = 1;
				break;
			case 1:	// IDLE State
				TA1CCR0 = 0; // Stop
			switch (turn){
					case 0:	// Turn left

						break;
					case 1: // Turn right
					
						break;
				}
				if ((P1IN & BIT0) == 0) // Check if P1IN is low
					move = 0;
				break;
		}
	}
	
	return 0;
}
