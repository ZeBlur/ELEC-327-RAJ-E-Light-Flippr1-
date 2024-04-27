#include <msp430.h> 
#include <stdint.h>
#include <stdbool.h>
#include "initialize.h"
#include "servo.h"

/**
 * FLIPPER.c
 *
 * The following code implements a simple state machine to control
 * the logic for our automated light switch flipper. It relies on
 * several helper functions that are explained below.
 *
 *    Date: April 24, 2024
 *  Author: RAJ Group LLC
 *
 */

int ir_read() {
    // This function provides the functionality for reading the IR
    // sensor voltage anytime the function is called.

    // Temporary variable to store the IR sensor voltage value.
    int x = 0;

    // Configure the ADC to read from pin 1.3 of the MSP430
    ADC10CTL0 = SREF_0|ADC10SHT_2|ADC10ON;
    ADC10CTL1 = INCH_3|SHS_0|ADC10DIV_0|ADC10SSEL_0|CONSEQ_0;
    ADC10AE0 |= BIT3;  //P1.3 also above, INCH_3
    ADC10CTL0 |= ENC;

    // Wait for the measurement to finish and store the value
    ADC10CTL0 |= ADC10SC;
    while(ADC10CTL1 & ADC10BUSY);
    x = ADC10MEM;

    // Return the ADC reading of the IR sensor
    return x;
}

int motion_read() {
    // This function provides the functionality for reading the
    // motion sensor voltage anytime the function is called.

    // Temporary variable to store the motion sensor voltage value.
    int y = 0;

    // Configure the ADC to read from pin 1.0 of the MSP430
    ADC10CTL0 = SREF_0|ADC10SHT_2|ADC10ON;
    ADC10CTL1 = INCH_0|SHS_0|ADC10DIV_0|ADC10SSEL_0|CONSEQ_0;
    ADC10AE0 |= BIT0;  //P1.3 also above, INCH_3
    ADC10CTL0 |= ENC;

    // Wait for the measurement to finish and store the value
    ADC10CTL0 |= ADC10SC;
    while(ADC10CTL1 & ADC10BUSY);
    y = ADC10MEM;

    // Return the ADC reading of the motion sensor
    return y;
}

//

void init_serial() {
    // Reconfigure the clock
    BCSCTL1 = CALBC1_1MHZ;
    DCOCTL = CALDCO_1MHZ;

    // Set up WDT interrupt
    WDTCTL = WDT_ADLY_250;       // WDT interrupt
    IE1 |= WDTIE;               // Enable WDT interrupt

    P1SEL = BIT1|BIT2;      // Configure the data in and data out for SPI on USCIA0
    P1SEL2 = BIT1|BIT2;

    // Configure UCA0 for serial output
    UCA0CTL1 |= UCSWRST+UCSSEL_2;
    UCA0BR0 = 52;  //settings for 19200 baud
    UCA0BR1 = 0;
    UCA0MCTL = UCBRS_0;
    UCA0CTL1 &= ~UCSWRST;
}

// Iteration variable
int i;
// Character strings used in various messages:
char charmemval[] = "Sensor Voltage: ";
// Return Character
char charreturn[] = "\r\n\r\n";

// Variables used to generate the serial message below
char mv_char[5];
void ser_output(char *str);
void itoa(int n, char s[]);
void reverse(char s[]);

// Add message to serial output buffer so it can be sent
void ser_output(char *str){
    while(*str != 0) {
        while (!(IFG2&UCA0TXIFG));
        UCA0TXBUF = *str++;
        }
}

/* itoa:  convert int n to characters stored in s */
void itoa(int n, char s[])
{
    int i, sign;

    if ((sign = n) < 0)  /* record sign */
        n = -n;          /* make n positive */
    i = 0;
    do {       /* generate digits in reverse order */
        s[i++] = n % 10 + '0';   /* get next digit */
    } while ((n /= 10) > 0);     /* delete it */
    if (sign < 0)
        s[i++] = '-';
    s[i] = '\0';
    reverse(s);
}

// Reverses a character string for serial output
void reverse(char s[])
{
    int i, j;
    char c;

    for (i = 0, j = strlen(s)-1; i<j; i++, j--) {
        c = s[i];
        s[i] = s[j];
        s[j] = c;
    }
}

// Takes an input integer value that is converted to a character and
// output to the terminal along with additional text.
void output_to_terminal(int value) {
    // Initialize the ports for serial output.
    init_serial();

    // Set up the input integer for char conversion
    int memval = value;

    // Serial conversions
    itoa(memval, mv_char);
    ser_output(charmemval);
    ser_output(mv_char);
    ser_output(charreturn);

    // Re-Initialize the MSP430 for main functionality
    init_main();
}

// Variables that are incremented every time their respective sensors are tripped.
int ir_counter, motion_counter = 0;
// Timer counter variable that is used to keep a running total of the time that passes.
int timer_counter = 0;
// Initialize flags that handle timer and button interrupts
int timer_wakeup_flag, button_wakeup_flag = 0;
// Initialize the flag that toggles the servo motion
int switch_flag = 0;


// This is the threshold timer_counter must exceed before registering inactivity and turning
// off the lights.
//
// We have experimentally determined 0.711765 seconds per tick; 2 minutes is approx. 169 ticks
const int TIMEOUT_THRESHOLD =639;

// This is the threshold number of times the IR Sensor must be tripped to register activity.
// The IR sensor is quite sensitive so this threshold serves as a method to "debounce" it.
const int IR_THRESHOLD = 3;

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
 *      MAIN FUNCTION FSM BEGINS HERE:
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */

int main(void)
{
    /* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
     *      Begin Initialization:
     * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
     */

    // Call the function to initialize the WDT, Button Pins, Timers, and Clock Sources necessary for
    // the main function.
    init_main();
    // Call the function to initialize the ADC once before ir_read() and motion_read() reinitialize it.
    init_adc();

    // Enable Interrupts! Important, we use both WDT and Button interrupts.
    _enable_interrupts();

    // Create state vectors! Our FSM is only ever in two states, up or down.
    enum state_enum{DOWN=0, UP=1} state;

    // Initialize the FSM to the Down position
    state = DOWN;

    // Initialize volatile floats to store the sensor readings.
    volatile float ir_voltage, motion_voltage;

    /* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
     *      Begin Main While Loop that Handles State Transitions:
     * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
     */

    while(1) {

        // Timer Interrupt Handler:
        if (timer_wakeup_flag) {
            // Increment the running timer counter
            timer_counter++;

            // Clear the timer interrupt flag
            timer_wakeup_flag = 0;
        }


        // Button Interrupt Handler:
        if (button_wakeup_flag) {
            // Triggers a state transition, so reset the timer and assert the servo toggle flag
            timer_counter = 0;
            switch_flag = 1;

            // Clear the button interrupt flag
            button_wakeup_flag = 0;
        }



        // Poll the two sensors here:
        ir_voltage = ir_read();

        motion_voltage = motion_read();



        // IR Sensor is Active High, so check for any low readings indicating remote activity.
        if (ir_voltage <= 10) {
            // If activity is detected, register it by incrementing the counter
            ir_counter++;
        }

        // Motion Sensor is Active Low, so check for any high readings indicating motion activity.
        if (motion_voltage >= 700) {
            // If activity is detected, register it by incrementing the counter
            motion_counter++;
        }


        // If the IR Sensor consistently reports activity:
        if ((ir_counter >= 2) && (timer_counter >= IR_THRESHOLD)) {
            // Reset the counters
            timer_counter = 0;
            ir_counter = 0;

            // Set the servo toggle flag
            switch_flag = 1;
        }

        // If the motion sensor consistently reports activity:
        if (motion_counter >= 2) {
            // Reset the timer to register motion,
            timer_counter = 0;
            // Reset the motion activity counter
            motion_counter = 0;
        }



        // If enough time passes such that the timeout threshold is exceeded:
        if (timer_counter >= TIMEOUT_THRESHOLD) {
            // Reset the timer counter
            timer_counter = 0;

            // If the lights are on:
            if (state == UP){
                // Turn them off.
                switch_flag = 1;
            }
        }



        // If the servo toggle flag is asserted:
        if (switch_flag) {
            // If lights are off:
            if (state == DOWN) {
                // Move the servo arm to turn the lights on
                MoveArm(100);
                // Turn off MSP430 Board Light used for debugging
                P2OUT |= BIT5;
                // Lights are on, register the system in the UP state
                state = UP;
            } else {
                // Move the servo arm to turn the lights off
                MoveArm(65);
                // Turn on the MSP430 Board Light used for debugging.
                P2OUT &=~ BIT5;
                // Lights are off, register the system in the DOWN state
                state = DOWN;
            }

            // Clear the servo toggle flag
            switch_flag = 0;
        }

        // ENTER LOW POWER MODE!
        __bis_SR_register(LPM3_bits + GIE);
    }

    // Never reached hehe.
    return 0;
}


/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *      Begin Interrupt Service Routines:
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */


// Watchdog Timer interrupt service routine
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=WDT_VECTOR
__interrupt void watchdog_timer(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(WDT_VECTOR))) watchdog_timer (void)
#else
#error Compiler not supported!
#endif
{
    // Every WDT tick, assert a timer_wakeup_flag to the FSM to increment the counter
    timer_wakeup_flag = 1;

    __bic_SR_register_on_exit(LPM3_bits); // exit LPM3 when returning to program (clear LPM3 bits)
}


// Button interrupt service routine
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=PORT2_VECTOR
__interrupt void button(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(PORT2_VECTOR))) button (void)
#else
#error Compiler not supported!
#endif
{
    // On button press, assert the button_wakeup_flag to process the button input
    button_wakeup_flag = 1;

    // Clear the interrupt flags (only button is on pin 2.4)
    P2IFG &= ~(BIT4);
    __bic_SR_register_on_exit(LPM3_bits); // exit LPM3 when returning to program (clear LPM3 bits)
}
