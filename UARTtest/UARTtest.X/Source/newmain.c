/* 
 * File:   newmain.c
 * Author: Josh Hannan
 *
 * When the WiFi module gets a regular command, it sends a carriage return, newline,
 * the command that was sent, another carriage return, newline, AOK/ERR, then
 * another carriage return and newline, then "<4.00> " which is the firmware
 * version number plus a space.
 * usually 14 more characters than the original command.
 *
 * In configuration commands, spaces in the command are still spaces, but spaces
 * in the parameters of the commands need to be $.
 * 
 *
 * Created on March 11, 2015, 1:51 PM
 */

#include "system_include.h"
#include "wifi.h"
#include "motor.h"
#include "uart.h"


#pragma config DEBUG =      ON
#pragma config JTAGEN =     OFF
#pragma config ICESEL =     ICS_PGx1
#pragma config TRCEN =      ON
#pragma config BOOTISA =    MIPS32
#pragma config FECCCON =    OFF_UNLOCKED
#pragma config FSLEEP =     OFF
#pragma config DBGPER =     PG_ALL
#pragma config EJTAGBEN =   NORMAL
#pragma config CP =         OFF

/*** DEVCFG1 ***/

#pragma config FNOSC =      SPLL
#pragma config DMTINTV =    WIN_127_128
#pragma config FSOSCEN =    ON
#pragma config IESO =       ON
#pragma config POSCMOD =    OFF
#pragma config OSCIOFNC =   OFF
#pragma config FCKSM =      CSECME
#pragma config WDTPS =      PS1048576
#pragma config WDTSPGM =    STOP
#pragma config FWDTEN =     OFF
#pragma config WINDIS =     NORMAL
#pragma config FWDTWINSZ =  WINSZ_25
#pragma config DMTCNT =     DMT31
#pragma config FDMTEN =     OFF

/*** DEVCFG2 ***/

#pragma config FPLLIDIV =   DIV_1
#pragma config FPLLRNG =    RANGE_5_10_MHZ
#pragma config FPLLICLK =   PLL_FRC
#pragma config FPLLMULT =   MUL_50
#pragma config FPLLODIV =   DIV_2
#pragma config UPLLFSEL =   FREQ_24MHZ
#pragma config UPLLEN =     ON

/*** DEVCFG3 ***/

#pragma config USERID =     0xffff
#pragma config FMIIEN =     ON
#pragma config FETHIO =     ON
#pragma config PGL1WAY =    ON
#pragma config PMDL1WAY =   ON
#pragma config IOL1WAY =    ON
#pragma config FUSBIDIO =   ON

/*** BF1SEQ0 ***/

#pragma config TSEQ =       0xffff
#pragma config CSEQ =       0xffff



/*
 * 
 */
int main(int argc, char** argv) {
    int i = 0;
    int j = 0;
    int action;

    // call configuration settings for uart and pps
    setup_config();
    setup_uart();
    setup_pps();

    // set configuration for wifi
    //wifi_reboot();
    //wifi_set_real_config();
    
    while(i<10000000){
            i++;
    }
    i = 0;

    //T2CONSET = 0x8000;    // enable timer 2
    //OC1CONSET = 0x8000;   // enable Output Compare (PWM)

    LATBbits.LATB8 = 1; // set left direction to 1
    LATBbits.LATB10 = 1; // set !reset to 1

    LATBbits.LATB13 = 0; // set right direction to 1
    LATBbits.LATB5 = 1;

    //400 000 Total for entire kick cycle
    // High 32000 and Low 368000 = 90
    // High 10000 and Low 390000 = 0 = Down Completely
    while(1) {
        j = 0;
        while (j < 30) {
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


    //THIS DOESN'T DO ANYTHING BECAUSE EVERYTHING IS BROKEN
    while(1) {
        // set all I/O pins
        //LATBbits.LATB13 = 1; // right stepper direction, pin 28
        //LATBbits.LATB5 = 1; // right stepper !reset, pin 11
        //LATBbits.LATB8 = 1; // left stepper direction, pin 21
        //LATBbits.LATB10 = 1; // left stepper !reset, pin 23
        LATBbits.LATB4 = 1; // dip 1, pin 12
        LATBbits.LATB3 = 1; // dip 2, pin 13

        // set all OCx pins
        
        //LATBbits.LATB14 = 1; // OC2, pin 29
        //LATBbits.LATB9 = 1; // OC3, pin 22

        LATFbits.LATF3 = 1; // old servo

        i = 0;
        while(i<1000000 / 100){
            i++;
        }

        // clear all I/O pins
        //LATBbits.LATB13 = 0; // right stepper direction, pin 28
        //LATBbits.LATB5 = 0; // right stepper !reset, pin 11
        //LATBbits.LATB8 = 0; // left stepper direction, pin 21
        //LATBbits.LATB10 = 0; // left stepper !reset, pin 23
        LATBbits.LATB4 = 0; // dip 1, pin 12
        LATBbits.LATB3 = 0; // dip 2, pin 13

        // clear all OCx pins
        LATBbits.LATB2 = 0; // OC1, pin 14
        //LATBbits.LATB14 = 0; // OC2, pin 29
        //LATBbits.LATB9 = 0; // OC3, pin 22

        LATFbits.LATF3 = 0; // old servo
        
        i = 0;
        while(i<1000000 / 100){
            i++;
        }
         
        
    }
    j = 0;
    while (j < 2000) {
        //action = rx_command();
        action = ROBOT_KICK;
        //while(i<10000000){
        //while(i<1000){
        //    i++;
        //}
        //i = 0;
        
        switch(action) {
            case ROBOT_NOP:
                robot_kick();
                break;
            case ROBOT_FORWARD:
                robot_forward();
                break;
            case ROBOT_BACK:
                robot_back();
                break;
            case ROBOT_LEFT:
                robot_left();
                break;
            case ROBOT_RIGHT:
                robot_right();
                break;
            case ROBOT_KICK:
                robot_kick();
                break;
            default:
                robot_kick();
                break;
        }

        i = 0;
        j = j + 1;
        if (j > 1950) { j = 0; }
    }
    return (EXIT_SUCCESS);
}
