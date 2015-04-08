/*
 * File:   motor.c
 * Author: Josh Hannan
 *
 * Created on March 26, 2015, 10:53 AM
 */

#include "motor.h"


void robot_nop(void)
{
    int i = 0;
    // do nothing
    while(i<50000000){
            i++;
        }
}

void robot_right(void)
{
    int i = 0;
    // set pwm for right motor to go forward
    // set pwm for left motor to not move

    // wait for a short amount of time

    // set them back to zero
    PR2 = 0x00C7;
    OC1CONSET = 0x8000;

    PR3 = 0x00A3;
    OC2CONSET = 0x8000;
    
    while (i < 50000000) { i++; }
    OC1CONCLR = 0x8000;
    OC2CONCLR = 0x8000;
}

void robot_left(void)
{
    int i = 0;
    // set pwm for left motor to go forward
    // set pwm for right motor to not move

    // wait for a short amount of time

    // set them back to zero
while(i<50000000){
            i++;
        }
}

void robot_forward(void)
{
    int i = 0;
    // decrease period of pwm and set GPIO pin for left motor to go forward
    // decrease period of pwm and set GPIO pin for right motor to go forward

    // wait for a short amount of time

    // set them back to idle
    while(i<50000000){
            i++;
        }
}

void robot_back(void)
{
    int i = 0;
    // set pwm for left motor to go backward
    // set pwm for right motor to go backward

    // wait for a short amount of time

    // set them back to zero
while(i<50000000){
            i++;
        }
}

void robot_kick(void)
{
    int i = 0;
    // set output to rotate paddle to kick

    // reset angle of paddle
    OC3R = 0x0046;
    OC3RS = 0x0046;


    // set paddle motor to zero
    while(i<50000000){
            i++;
    }
    OC3R = 0x006F;
    OC3RS = 0x006F;
}



