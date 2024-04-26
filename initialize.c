/*
 * initialize.c
 *
 *  Created on: April 24, 2024
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

// Initialize the various buttons used in the code
void init_buttons() {
    P2DIR &= ~(BIT4); // set to input
    P2DIR |= BIT5;
    P2REN = BIT4; // enable pullup/down resistors
    P2OUT = BIT4; // set resistors to pull up

    /* Uncomment the following code if you want to use interrupts to detect button presses */

    P2IES = BIT4; // listen for high to low transitions
    P2IFG &=  ~(BIT4); // clear any pending interrupts
    P2IE = BIT4; // enable interrupts for these pins
}

// Initialize the ADC to be used for reading random voltage values from Pin 1.3
void init_adc() {
    ADC10CTL0 = SREF_0|ADC10SHT_2|ADC10ON;
    ADC10CTL1 = INCH_3|SHS_0|ADC10DIV_0|ADC10SSEL_0|CONSEQ_0;
    //ADC10AE0 |= BIT0;  //P1.3 also above, INCH_3
    ADC10CTL0 |= ENC;
}

/* ---------------------------------------------------------------- */

// Function to be called in the main.c function to initialize all appropriate values
// by calling init_wdt(), rgb_init_spi() for LED strip usage, init_buttons() for input,
// and init_adc() for generating a random seed.
void init_main() {

    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    DCOCTL = 0;                 // Select lowest DCOx and MODx settings
    BCSCTL1 = CALBC1_1MHZ;     // Set range
    DCOCTL = CALDCO_1MHZ;      // Set DCO step + modulation
    BCSCTL3 |= LFXT1S_2;        // ACLK = VLO - This is also called in the init_wdt() function


    P2DIR |= BIT1;                        // Set PWM pin direction
    P2SEL |= BIT1;                        // Set PWM pin direction

    TA1CTL = TASSEL_1 + MC_1;                              // ACLK, upmode
    TA1CCR0 = 50;                                          // Set PWM frequency ~12 kHz / 20



    // Initialize all the modules
    init_wdt();
    init_buttons();
    //init_adc();
}
