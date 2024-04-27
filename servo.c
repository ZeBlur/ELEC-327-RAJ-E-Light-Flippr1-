/*
 * servo.c
 *
 *  Created on: April 24, 2024
 *      Author: RAJ Group LLC
 */

#include "msp430g2553.h"
#include <stdint.h>
#include <stdbool.h>
#include "servo.h"

// Below is a function to rotate the our servo arm in a controlled manner
// Input: int degrees     -        number of degrees to set and hold the arm at
void MoveArm(int degrees){
    // translation of degree input to PWM range
     int temp;
     int pwm_denominator;

    // Conversion factor found online
     temp = degrees * 0.1167;
     temp = (temp + 23) % 181;

     pwm_denominator = temp;

     // Set the pwm denominator and configure pwm mode 3
     TA1CCR1 = pwm_denominator;
     // Configure Timer A for PWM Set/Reset Mode as required for the servo
     TA1CCTL1 = OUTMOD_3;
}
