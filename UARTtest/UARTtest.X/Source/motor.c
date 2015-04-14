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
    while(i<10000000){
            i++;
        }
}

void robot_right(void)
{
    int i = 0;
    int j = 0;
    LATBbits.LATB8 = 0; // set left direction to 0
    LATBbits.LATB10 = 1; // set !reset to 1

    LATBbits.LATB13 = 0; // set right direction to 0
    LATBbits.LATB5 = 1;

    while(j<400) {
        j++;
        // set all I/O pins
        LATBbits.LATB4 = 1; // dip 1, pin 12
        LATBbits.LATB3 = 1; // dip 2, pin 13

        // set all OCx pins

        LATBbits.LATB14 = 1; // OC2, pin 29
        LATBbits.LATB9 = 1; // OC3, pin 22


        i = 0;
        while(i<1000000 / 100){
            i++;
        }

        LATBbits.LATB4 = 0; // dip 1, pin 12
        LATBbits.LATB3 = 0; // dip 2, pin 13

        // clear all OCx pins
        LATBbits.LATB14 = 0; // OC2, pin 29
        LATBbits.LATB9 = 0; // OC3, pin 22


        i = 0;
        while(i<1000000 / 100){
            i++;
        }
    }
}

void robot_left(void)
{
    int i = 0;
    int j = 0;
    LATBbits.LATB8 = 1; // set left direction to 0
    LATBbits.LATB10 = 1; // set !reset to 1

    LATBbits.LATB13 = 1; // set right direction to 0
    LATBbits.LATB5 = 1;

    while(j<400) {
        j++;
        // set all I/O pins
        LATBbits.LATB4 = 1; // dip 1, pin 12
        LATBbits.LATB3 = 1; // dip 2, pin 13

        // set all OCx pins

        LATBbits.LATB14 = 1; // OC2, pin 29
        LATBbits.LATB9 = 1; // OC3, pin 22


        i = 0;
        while(i<1000000 / 100){
            i++;
        }

        LATBbits.LATB4 = 0; // dip 1, pin 12
        LATBbits.LATB3 = 0; // dip 2, pin 13

        // clear all OCx pins
        LATBbits.LATB14 = 0; // OC2, pin 29
        LATBbits.LATB9 = 0; // OC3, pin 22


        i = 0;
        while(i<1000000 / 100){
            i++;
        }
    }
}

void robot_forward(void)
{
    int i = 0;
    int j = 0;
    LATBbits.LATB8 = 1; // set left direction to 1
    LATBbits.LATB10 = 1; // set !reset to 1

    LATBbits.LATB13 = 0; // set right direction to 1
    LATBbits.LATB5 = 1;
    
    while(j<800) {
        j++;
        // set all I/O pins
        LATBbits.LATB4 = 1; // dip 1, pin 12
        LATBbits.LATB3 = 1; // dip 2, pin 13

        // set all OCx pins

        LATBbits.LATB14 = 1; // OC2, pin 29
        LATBbits.LATB9 = 1; // OC3, pin 22


        i = 0;
        while(i<1000000 / 100){
            i++;
        }

        LATBbits.LATB4 = 0; // dip 1, pin 12
        LATBbits.LATB3 = 0; // dip 2, pin 13

        // clear all OCx pins
        LATBbits.LATB14 = 0; // OC2, pin 29
        LATBbits.LATB9 = 0; // OC3, pin 22


        i = 0;
        while(i<1000000 / 100){
            i++;
        }
    }
}

void robot_back(void)
{
    int i = 0;
    int j = 0;
    LATBbits.LATB8 = 0; // set left direction to 0
    LATBbits.LATB10 = 1; // set !reset to 1

    LATBbits.LATB13 = 1; // set right direction to 0
    LATBbits.LATB5 = 1;

    while(j<800) {
        j++;
        // set all I/O pins
        LATBbits.LATB4 = 1; // dip 1, pin 12
        LATBbits.LATB3 = 1; // dip 2, pin 13

        // set all OCx pins

        LATBbits.LATB14 = 1; // OC2, pin 29
        LATBbits.LATB9 = 1; // OC3, pin 22


        i = 0;
        while(i<1000000 / 100){
            i++;
        }

        LATBbits.LATB4 = 0; // dip 1, pin 12
        LATBbits.LATB3 = 0; // dip 2, pin 13

        // clear all OCx pins
        LATBbits.LATB14 = 0; // OC2, pin 29
        LATBbits.LATB9 = 0; // OC3, pin 22


        i = 0;
        while(i<1000000 / 100){
            i++;
        }
    }
}

void robot_kick(void)
{
    int i = 0;
    int j = 0;
    while (j < 20) {
        LATBbits.LATB2 = 1; // OC1, pin 14
        while(i< 32000){
            i++;
        }
        i = 0;
        LATBbits.LATB2 = 0; // OC1, pin 14

        while(i< 368000){
            i++;
        }
        i = 0;
        j = j + 1;
    }
            j = 0;
    while (j < 250) {
        LATBbits.LATB2 = 1; // OC1, pin 14
        while(i< 10000){
            i++;
        }
        i = 0;
        LATBbits.LATB2 = 0; // OC1, pin 14
        while(i< 390000){
            i++;
        }
        i = 0;
        j = j + 1;
    }
}



