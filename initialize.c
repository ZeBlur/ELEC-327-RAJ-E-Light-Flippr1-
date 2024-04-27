/*
 * initialize.c
 *
 *  Created on: March 25, 2024
 *      Author: Adam Swartz (aas23)
 *
 *      Source and Header files for initializing all necessary modules
 *
 */


#include "msp430g2553.h"
#include <stdint.h>
#include <stdbool.h>
#include "initialize.h"

/* ---------------------------------------------------------------- */

// Initialize the Watch Dog Timer
void init_wdt(void){
    BCSCTL3 |= LFXT1S_2;     // ACLK = VLO
    WDTCTL = WDT_ADLY_16;    // WDT 16ms (~43.3ms since clk 12khz), ACLK, interval timer
    IE1 |= WDTIE;            // Enable WDT interrupt
}

// Initialize the various buttons used in the code (only button on pin 2.4 in this case).
void init_buttons() {
    P2DIR &= ~(BIT4); // set to input
    P2DIR |= BIT5; // Set LED on 2.5 to output.
    P2REN = BIT4; // enable pullup/down resistors
    P2OUT = BIT4; // set resistors to pull up

    /* Uncomment the following code if you want to use interrupts to detect button presses */

    P2IES = BIT4; // listen for high to low transitions
    P2IFG &=  ~(BIT4); // clear any pending interrupts
    P2IE = BIT4; // enable interrupts for these pins
}

// Initialize the ADC to be used for reading voltage values on pins
// ir_read() and motion_read() do this.
void init_adc() {
    ADC10CTL0 = SREF_0|ADC10SHT_2|ADC10ON;
    ADC10CTL1 = INCH_3|SHS_0|ADC10DIV_0|ADC10SSEL_0|CONSEQ_0;
    ADC10CTL0 |= ENC;
}

/* ---------------------------------------------------------------- */

// Function to be called in the main.c function to initialize all appropriate values
// by calling init_wdt() and init_buttons() to drive the program forward.

void init_main() {

    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    DCOCTL = 0;                 // Select lowest DCOx and MODx settings
    BCSCTL1 = CALBC1_1MHZ;     // Set range
    DCOCTL = CALDCO_1MHZ;      // Set DCO step + modulation
    BCSCTL3 |= LFXT1S_2;        // ACLK = VLO - This is also called in the init_wdt() function


    P2DIR |= BIT1;                        // Set PWM pin direction
    P2SEL |= BIT1;                        // Set PWM pin direction

    // Enable timers used for generating PWM signal for servo actuation
    TA1CTL = TASSEL_1 + MC_1;                              // ACLK, upmode
    TA1CCR0 = 50;                                          // Set PWM frequency ~12 kHz / 20

    // Initialize all the modules
    init_wdt();
    init_buttons();
}
