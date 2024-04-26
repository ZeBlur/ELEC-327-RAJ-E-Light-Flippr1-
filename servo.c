/*
 * servo.c
 *
 *  Created on: Apr 24, 2024
 *      Author: RAJ Group LLC
 */

#include "msp430g2553.h"
#include <stdint.h>
#include <stdbool.h>
#include "servo.h"

void MoveArm(int degrees){
    int temp;
    int pwm_denominator;
    temp = degrees * 0.1167;
    temp = (temp + 23) % 181;
    //temp = (temp + 24) % 180;
    pwm_denominator = temp;     // translation of degree input to PWM range

     //TA1CTL = TASSEL_1 + MC_1;                              // ACLK, upmode
     //TA1CCR0 = 50;                                          // Set PWM frequency ~12 kHz / 20
     TA1CCR1 = pwm_denominator;                             // 180 degrees
     TA1CCTL1 = OUTMOD_3;                                   // Set PWM mode
}


