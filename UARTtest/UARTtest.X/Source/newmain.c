/* 
 * File:   newmain.c
 * Author: EE63PC1-user
 *
 * Created on March 11, 2015, 1:51 PM
 */



#include <stdio.h>
#include <stdlib.h>
#include "proc/p32mz2048ech100.h"
#include "p32xxxx.h"
//#include <peripheral/uart.h>

#define FPB 99000000L // Frequency Peripheral Bus = 40MHz
#define BAUDRATE 9600

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

//-------------------------------------------------
//  setup_pps
//  sets configuration registers for peripheral pin
//  select
//-------------------------------------------------
void setup_pps(void)
{
    SYSKEY = 0x0;         //i dont know what these do
    SYSKEY = 0xAA996655;
    SYSKEY = 0x556699AA;

    CFGCONbits.IOLOCK = 0;    // unlocks config
    U1RXRbits.U1RXR = 0b0001; // Set RX to RPG8
    RPG7Rbits.RPG7R = 0b0001; // Set TX to RPG7
    ANSELGbits.ANSG7 = 0;     // sets G7 to digital
    ANSELGbits.ANSG8 = 0;     // sets G8 to digital
    CFGCONbits.IOLOCK = 1;    // locks config

    SYSKEY = 0x0;
}

//-------------------------------------------------
//  setup_uart
//  sets configuration registers for UART
//-------------------------------------------------
void setup_uart(void)
{
    // how to set the baud rate of the uart? BRG = 25 for 4MHz peripheral bus clock
    U1BRG = FPB/(4.0*BAUDRATE)-1;  // maybe
    U1MODEbits.ON = 1;             // enables uart module
    U1MODEbits.BRGH = 1;           // uses eq 2 for BR
    U1MODEbits.PDSEL0 = 0;         // 8 bits, no parity
    U1MODEbits.PDSEL1 = 0;
    U1MODEbits.STSEL = 0;          // 1 stop bit

    U1STAbits.URXEN = 1;           // enable RX
    U1STAbits.UTXEN = 1;           // enable TX

    U1MODEbits.SIDL = 0;           
    U1MODEbits.IREN = 0;
    U1MODEbits.RTSMD = 1;
}

//-------------------------------------------------
//  tx_string
//  transmits a string of length ascii characters
//  over UART
//-------------------------------------------------
void tx_string(char *string, int length)
{
    int i;
    for(i = 0; i < length; i++) {
        tx_char(string[i]);
    }
}

//-------------------------------------------------
//  tx_char
//  transmits one ascii character over UART
//-------------------------------------------------
void tx_char(unsigned char c)
{
    int i = 0;
    // wait for transmit register to be empty
    while(U1STAbits.UTXBF);
    // place a character in transmit register
    U1TXREG = c;              
}

/*void rx_string(char *RESPONSE)
{
    int i;
    for(i = 0; i < 20; i++) {
        RESPONSE[i] = rx_char();
    }
}*/
//-------------------------------------------------
//  rx_char
//  receives one ascii character over UART
//-------------------------------------------------
char rx_char()
{
    char c;
    // wait for recieve buffer to be full
    while(U1STAbits.URXDA == 0) {   
        continue;
    }
    // read the receive register
    c = U1RXREG;
    return c;
}

/*
 * 
 */
int main(int argc, char** argv) {
    int i = 0;
    //char RESPONSE [6] = {0x00,0x00,0x00,0x00,0x00,0x00};
    char RESPONSE [50] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
    char ENTER_CMD [3] = {'$', '$', '$'};
    char EXIT_CMD [5] = {'e','x','i','t',0x0d};
    char SET_IP_PROTO [16] = {'s','e','t','$','i','p','$','p','r','o','t','o','$','1','8',0x0D};
    char SET_COMM [14] = {'s','e','t','$','c','o','m','m','$','$','$','m',0x0D};
    char SET_COMM_OPEN [19] = {'s','e','t',' ','c','o','m','m',' ','o','p','e','n',' ','p','o','o','p',0x0d};
    char GET_APMODE [12] = {'g','e','t',' ','a','p','m','o','d','e',0x0d};

    // disable watchdog and deadman timers
    WDTCONbits.ON = 0;
    DMTCONbits.ON = 0;

    // set all ports as outputs
    TRISACLR = 0xFFFF;
    TRISBCLR = 0xFFFF;
    TRISGCLR = 0xFFFF;

    // set all ports as digital
    ANSELACLR = 0xFFFF;
    ANSELBCLR = 0xFFFF;
    ANSELGCLR = 0xFFFF;

    // disable jtag
    CFGCONbits.JTAGEN = 0;

    // set UART ports as inputs and outputs
    TRISGbits.TRISG8 = 1;  // RX
    TRISGbits.TRISG7 = 0;  // TX

    // call configuration settings for uart and pps
    setup_uart();    
    setup_pps();

    // wait for settings to take effect
    while(i<2000){
        i++;
    }

    while(1) {
        // enter command mode
        tx_string(ENTER_CMD, 3);
        // receive CMD from WiFi module.
        RESPONSE[0] = rx_char();
        RESPONSE[1] = rx_char();
        RESPONSE[2] = rx_char();

        // send a config command to WiFi module
        tx_string(SET_COMM_OPEN, 19);
        // receive AOK response
        //rx_string(RESPONSE);
        for(i = 0; i < 16; i++) {
            RESPONSE[i] = rx_char();
        }
        /*RESPONSE[0] = rx_char();
        RESPONSE[1] = rx_char();
        RESPONSE[2] = rx_char();
        RESPONSE[3] = rx_char();
        RESPONSE[4] = rx_char();
        RESPONSE[5] = rx_char();*/

        tx_string(SET_IP_PROTO, 16);
      
    }

    return (EXIT_SUCCESS);
}

