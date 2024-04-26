#include <msp430.h> 
#include <stdint.h>
#include <stdbool.h>
#include "initialize.h"
#include "servo.h"

/**
 * main.c
 */

int ir_read() {
    int x = 0;

    ADC10CTL0 = SREF_0|ADC10SHT_2|ADC10ON;
    ADC10CTL1 = INCH_3|SHS_0|ADC10DIV_0|ADC10SSEL_0|CONSEQ_0;
    ADC10AE0 |= BIT3;  //P1.3 also above, INCH_3
    ADC10CTL0 |= ENC;

    ADC10CTL0 |= ADC10SC;
    while(ADC10CTL1 & ADC10BUSY);
    x = ADC10MEM;

    return x;
}

int motion_read() {
    int y = 0;

    ADC10CTL0 = SREF_0|ADC10SHT_2|ADC10ON;
    ADC10CTL1 = INCH_0|SHS_0|ADC10DIV_0|ADC10SSEL_0|CONSEQ_0;
    ADC10AE0 |= BIT0;  //P1.3 also above, INCH_3
    ADC10CTL0 |= ENC;

    ADC10CTL0 |= ADC10SC;
    while(ADC10CTL1 & ADC10BUSY);
    y = ADC10MEM;

    return y;
}

//

void init_serial() {
    // WDTCTL = WDTPW | WDTHOLD;
    BCSCTL1 = CALBC1_1MHZ;
    DCOCTL = CALDCO_1MHZ;
    // Set up WDT interrupt for helpfulness
    WDTCTL = WDT_ADLY_250;       // WDT interrupt
    IE1 |= WDTIE;               // Enable WDT interrupt

    P1SEL = BIT1|BIT2;      // Configure the data in and data out for SPI on USCIA0
    P1SEL2 = BIT1|BIT2;

    UCA0CTL1 |= UCSWRST+UCSSEL_2;
    UCA0BR0 = 52;  //settings for 19200 baud
    UCA0BR1 = 0;
    UCA0MCTL = UCBRS_0;
    UCA0CTL1 &= ~UCSWRST;
}

// Iteration variable
int i;
// Character strings used in various messages:
char charturn[] = "Current Turn: ";
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

/* itoa:  convert n to characters in s */
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

// Takes an input integer value that is converted to a character and output to the terminal
// as described in the rubric.
void output_to_terminal(int value) {
    init_serial();

    int memval = value;

    itoa(memval, mv_char);
    ser_output(charmemval);
    ser_output(mv_char);
    ser_output(charreturn);

    init_main();
}

// Function to print specific lines to the terminal:
// If select == -1, print a return character,
// else, take select + 1 and convert that to a serial message that states
// what the current turn of play is. This is assuming the user passes sequence_counter to
// the function as select.
void terminal_return(int select) {
    init_serial();

    if (select == -1) {
        ser_output(charreturn);
    } else {
        select++;

        itoa(select, mv_char);
        ser_output(charturn);
        ser_output(mv_char);
        ser_output(charreturn);
    }

    init_main();
}

int ir_counter, motion_counter = 0;
int timer_counter, button_pressed = 0;
int timer_wakeup_flag, button_wakeup_flag = 0;
int switch_flag = 0;
const int TIMEOUT_THRESHOLD =639;//71;//.711765 seconds per tick; 2 minutes is approx. 169 ticks
const int IR_THRESHOLD = 3;
//int double_time_counter = 0;

int main(void)
{
    init_main();
    init_adc();
	
    _enable_interrupts();

    enum state_enum{DOWN=0, UP=1} state; // enum to describe state of system

    state = DOWN;

    volatile float ir_voltage, motion_voltage;

    //MoveArm(90);

    while(1) {

        // Timer Interrupt Handler here:
        if (timer_wakeup_flag) {
            timer_counter++;
/*
            if (timer_counter % 2 == 1) {
                double_time_counter++;
            }
*/
            timer_wakeup_flag = 0;
        }


        // Button Interrupt Handler here:
        if (button_wakeup_flag) {
            timer_counter = 0;
            switch_flag = 1;
            button_wakeup_flag = 0;
        }


        //WDTCTL = WDT_ADLY_250;    // WDT 16ms (~43.3ms since clk 12khz), ACLK, interval timer


        // Poll the two sensors here:
        ir_voltage = ir_read();
        //output_to_terminal(ir_voltage);

        motion_voltage = motion_read();
        //output_to_terminal(motion_voltage);
/*
        if (double_time_counter >= TIMEOUT_THRESHOLD) {
            if (motion_counter >= 1) {
                double_time_counter = 0;
            } else {
                motion_counter = 0;
            }
        }
*/
        if (ir_voltage <= 10) {
            ir_counter++;
            //switch_flag = 1;
        }

        if (motion_voltage >= 700) {
            motion_counter++;
        }


        // State Transition Logic Here:
        if ((ir_counter >= 2) && (timer_counter >= IR_THRESHOLD)) {
            timer_counter = 0;
            //motion_counter = 0;
            ir_counter = 0;

            switch_flag = 1;
        }

        if (motion_counter >= 2) {
            motion_counter = 0;
            timer_counter = 0;
        }

        if (timer_counter >= TIMEOUT_THRESHOLD) {
            timer_counter = 0;
            if (state == UP){
                switch_flag = 1;
                //MoveArm(65);
                //P2OUT &=~ BIT5;
                //state = DOWN;
            }
            //switch_flag = 1;
        }

        if (switch_flag) {
            if (state == DOWN) {
                MoveArm(100);
                P2OUT |= BIT5;//light off
                state = UP;

            } else {
                MoveArm(65);
                state = DOWN;
                P2OUT &=~ BIT5;//light on
            }

            switch_flag = 0;
        }

        __bis_SR_register(LPM3_bits + GIE);
    }

	return 0;
}

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
    // Every WDT tick, assert a timer_wakeup_flag to the FSM
    timer_wakeup_flag = 1;

    __bic_SR_register_on_exit(LPM3_bits); // exit LPM3 when returning to program (clear LPM3 bits)
}


// Watchdog Timer interrupt service routine
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=PORT2_VECTOR
__interrupt void button(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(PORT2_VECTOR))) button (void)
#else
#error Compiler not supported!
#endif
{
    // Buttons designed to issue two interrupts, one for pressing down and
    // one on release.

    // Detects particular button input:
    // Pressing the buttons clears its particular P2IN port, so P2IN & BIT0
    // will detect if that particular button was pressed.
    // Set button_pressed appropriately.

    // Assert the button_wakeup_flag to process the button input
    button_wakeup_flag = 1;

    // Toggle edge select to make the buttons responsive. When unpressed, interrupt checks
    // for a press, and when pressed it toggles IES to interrupt on release, then toggling
    // IES back to the default where it'll wait for a press.
    //P2IES ^= (BIT0 + BIT2 + BIT3 + BIT4);
    // Clear the interrupt flags
    P2IFG &= ~(BIT4);
    __bic_SR_register_on_exit(LPM3_bits); // exit LPM3 when returning to program (clear LPM3 bits)
}
