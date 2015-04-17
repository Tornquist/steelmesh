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
#include "util.h"


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

int main(int argc, char** argv) {
    int i = 0;
    int j = 0;
    char id = '0';
    int action;

    // Call Configuration Settings for UART and PPS
    setup_config();
    setup_uart();
    setup_pps();

    id = destructive_device_id_read();

    robot_kick(id);

    idle_delay(10000000);

    // Set Configuration for WiFi
    wifi_reboot();
    wifi_set_real_config(id);
    //wifi_load_twitch();

    idle_delay(10000000);
/*
    //Robot Demo
    while(1) {
        robot_forward();
        robot_nop();
        robot_back();
        robot_nop();
        robot_left();
        robot_nop();
        robot_right();
        robot_nop();
        robot_kick();
        robot_nop();
    }
*/

    while (1) {
        action = rx_command();
        //action = ROBOT_KICK;
        
        idle_delay(1000);

        switch(action) {
            case ROBOT_NOP:
                robot_nop();
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
                robot_kick(id);
                break;
            default:
                robot_nop();
                break;
        }

    }
    return (EXIT_SUCCESS);
}
