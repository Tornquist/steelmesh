/*
 * File:   motor.c
 * Author: Josh Hannan
 *
 * Created on March 26, 2015, 10:53 AM
 */

#include "motor.h"
#include "util.h"

void robot_nop(void) { idle_delay(100000); }

void robot_move(int lDir, int rDir, int time)
{
    int j = 0;
    LATBbits.LATB8 = lDir;
    LATBbits.LATB10 = 1; // set !reset to 1

    LATBbits.LATB13 = rDir;
    LATBbits.LATB5 = 1; // set !reset to 1

    while(j < time) {
        LATBbits.LATB14 = 1; // OC2, pin 29
        LATBbits.LATB9 = 1; // OC3, pin 22
        idle_delay(10000);

        LATBbits.LATB14 = 0; // OC2, pin 29
        LATBbits.LATB9 = 0; // OC3, pin 22
        idle_delay(10000);

        j = j + 1;
    }
}

void robot_right(void) { robot_move(0, 0, 400); }

void robot_left(void) { robot_move(1, 1, 400); }

void robot_forward(void) { robot_move(1, 0, 800); }

void robot_back(void) { robot_move(0, 1, 800); }

void robot_kick(char id) {
    // 400 000 Total for entire kick cycle
    // High 32000 and Low 368000 = 90
    // High 10000 and Low 390000 = 0 = Down Completely
    int j = 0;

    if (id == '1') {
        while (j < 20) {
            LATBbits.LATB2 = 1; // OC1, pin 14
            idle_delay(32000);

            LATBbits.LATB2 = 0; // OC1, pin 14
            idle_delay(368000);

            j = j + 1;
        }

        j = 0;
        while (j < 40) {
            LATBbits.LATB2 = 1; // OC1, pin 14
            idle_delay(10000);

            LATBbits.LATB2 = 0; // OC1, pin 14
            idle_delay(360000);
            j = j + 1;
        }
    }
    else {
        while (j < 20) {
            LATBbits.LATB2 = 1; // OC1, pin 14
            idle_delay(32000);

            LATBbits.LATB2 = 0; // OC1, pin 14
            idle_delay(368000);

            j = j + 1;
        }

        j = 0;
        while (j < 40) {
            LATBbits.LATB2 = 1; // OC1, pin 14
            idle_delay(50000);

            LATBbits.LATB2 = 0; // OC1, pin 14
            idle_delay(390000);
            j = j + 1;
        } 
    }
}