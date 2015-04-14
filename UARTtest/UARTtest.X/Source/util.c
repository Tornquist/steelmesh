/*
 * File:   util.c
 * Author: Nathan Tornquist
 *
 * Created on April 14, 2015, 6:45 PM
 */

#include "util.h"

void idle_delay(int value) {
    int i = 0;
    while(i< value){
        i++;
    }
}

char destructive_device_id_read() {
    int i = 0;
    int j = 0;
    char id = ' ';
    TRISBSET = 0xFFFF; // input
    idle_delay(1000);

    i = PORTBbits.RB4;
    idle_delay(10);
    j = PORTBbits.RB3;
    idle_delay(10);

    // ji
    if ((j == 0) && (i == 1)) {
        id = '1';
    } else if ((j == 1) && (i == 0)) {
        id = '2';
    }else if ((j == 1) && (i == 1)) {
        id = '3';
    } else {
        id = '0';
    }

    TRISBCLR = 0xFFFF; // output
    idle_delay(1000);
    return id;
}

