/* 
 * File:   newmain.c
 * Author: Josh Hannan
 *
 * When the WiFi module gets a command, it sends a carriage return, newline,
 * the command that was sent, another carriage return, newline, AOK, then
 * another carriage return and newline, then "<2.45> " which is the firmware
 * version number plus a space.
 * 15 or 16 more characters than the original command.
 *
 * In configuration commands, spaces in the command are still spaces, but spaces
 * in the parameters of the commands need to be $.
 * 
 *
 * Created on March 11, 2015, 1:51 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include "proc/p32mz2048ech100.h"
#include "p32xxxx.h"
#include "wifi.h"

#pragma config DEBUG =      ON
#pragma config JTAGEN =     OFF
#pragma config ICESEL =     ICS_PGx2
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
    char *HELLO_WORLD = "Hello World";

    // call configuration settings for uart and pps
    setup_config();
    setup_uart();
    setup_pps();

    // wait for settings to take effect
    while(i<2000){
        i++;
    }

    wifi_config();

    while(1) {
        
        // this data should be sent over wifi to the webserver
        tx_string(HELLO_WORLD);       
      
    }

    return (EXIT_SUCCESS);
}

