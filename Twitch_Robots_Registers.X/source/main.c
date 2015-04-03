/* 
 * File:   main.c
 * Author: TJ
 *
 * Created on March 11, 2015, 12:32 PM
 */

#include "../include/camera.h"
#include <sys/attribs.h>
#include <xc.h>
#include <string.h>

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

int cam_data_index;
char cam_data_buff[1024];

int main(int argc, char** argv) {
    int i;
    CAM_DATA cam_data;

    // initialize
    //asm volatile("di");
    //asm volatile("ehb");
    init();
    asm volatile("ei");

    // wake up the camera, set its mode, and set its package size
    //while (cam_wake(&cam_data) != CAM_SUCCESS);
    //cam_send_initial(&cam_data);
    //cam_send_package_size(&cam_data);

    // receive a picture from the camera
    //cam_send_get_picture(&cam_data);
    
    uart2_tx_string("TEST\n");

    i = i + 1;
    return (EXIT_SUCCESS);
}

void __ISR (_UART2_RX_VECTOR, IPL7SRS) Uart2RXIntHandler(void) {
    //while (U2STAbits.URXDA);
    cam_data_buff[cam_data_index] = U2RXREG;
    cam_data_index = cam_data_index + 1;
    IFS4CLR = _IFS4_U2RXIF_MASK;
}