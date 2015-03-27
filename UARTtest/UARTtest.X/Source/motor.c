/*
 * File:   motor.c
 * Author: Josh Hannan
 *
 * Created on March 26, 2015, 10:53 AM
 */

#include "motor.h"


void turn_right(void)
{
    int i;
    // set pwm for right motor to go forward
    // set pwm for left motor to not move

    // wait for a short amount of time

    // set them back to zero
}

void turn_left(void)
{
    int i;
    // set pwm for left motor to go forward
    // set pwm for right motor to not move

    // wait for a short amount of time

    // set them back to zero

}

void forward(void)
{
    int i;
    // decrease period of pwm and set GPIO pin for left motor to go forward
    // decrease period of pwm and set GPIO pin for right motor to go forward

    // wait for a short amount of time

    // set them back to idle

}

void backward(void)
{
    int i;
    // set pwm for left motor to go backward
    // set pwm for right motor to go backward

    // wait for a short amount of time

    // set them back to zero

}

void paddle_kick(void)
{
    int i;
    // set output to rotate paddle to kick

    // reset angle of paddle

    // set paddle motor to zero
}

int command_execute(char *command)
{
    if (command == 'left') {
        turn_left();
    } else if (command == 'right') {
        turn_right();
    } else if (command == 'forward') {
        forward();
    } else if (command == 'backward') {
        backward();
    } else if (command == 'kick') {
        paddle_kick();
    } else {
        return MOTOR_FAIL;
    }
}

