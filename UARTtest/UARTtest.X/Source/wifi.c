/*
 * File:   wifi.c
 * Author: Josh Hannan
 *
 * Contains helper functions for communicating with the WiFi module
 *
 * Created on March 22, 2015, 11:46 AM
 *
 */

#include "wifi.h"

//-------------------------------------------------
//  setup_config
//  sets ports as digital outputs and disables timers
//-------------------------------------------------
void setup_config(void)
{
    // disable watchdog and deadman timers
    WDTCONbits.ON = 0;
    DMTCONbits.ON = 0;

    // disable jtag
    CFGCONbits.JTAGEN = 0;

    // set all ports as outputs
    TRISACLR = 0xFFFF;
    TRISBCLR = 0xFFFF;
    TRISGCLR = 0xFFFF;

    // set all ports as digital
    ANSELACLR = 0xFFFF;
    ANSELBCLR = 0xFFFF;
    ANSELGCLR = 0xFFFF;
}

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
    // set UART ports as inputs and outputs
    TRISGbits.TRISG8 = 1;  // RX
    TRISGbits.TRISG7 = 0;  // TX

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
void tx_string(char *string)
{
    int i;
    for(i = 0; i < strlen(string); i++) {
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

//-------------------------------------------------
//  rx_string
//  receives a string of length ascii characters
//  over UART
//-------------------------------------------------
int rx_string(int length)
{
    int i;
    int valid = 0;
    char RESPONSE [60] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
    for(i = 0; i < length; i++) {
        RESPONSE[i] = rx_char();
        if (RESPONSE[i] == 'A' || RESPONSE[i] == 'O' || RESPONSE[i] == 'K') {
            valid++;
        } else if (RESPONSE[i] == 0x00) {
            return WIFI_FAIL;
        }
    }

    if(valid == 3) {
        return WIFI_SUCCESS;
    }
    return WIFI_SUCCESS;
}
//-------------------------------------------------
//  rx_char
//  receives one ascii character over UART
//-------------------------------------------------
char rx_char()
{
    char c;
    int i = 0;
    // wait for recieve buffer to be full
    while(U1STAbits.URXDA == 0) {
        i=i+1;
        // check for receive buffer overflow
        if(U1STAbits.OERR == 1) {
            // clear the overflow flag and the buffer
            U1STAbits.OERR = 0;
        }
        // if it has been waiting a really long time, exit
        if(i == 100000000) {
            return 0x00;
        }
    }
    // read the receive register
    c = U1RXREG;
    return c;
}

void fail_wait(void)
{
    while(1)
    {
        continue;
    }
}

void wifi_config(void)
{
    int i = 0;
    char bot_id = '1';
    char RESPONSE [50] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
    // commands to enter and exit command mode
    char *ENTER_CMD = "$$$";
    char *EXIT_CMD = "exit\r";
    char *REBOOT = "reboot\r";  // reboots the module
    char *SAVE = "save twitch\r";  // saves settings to a file
    char *LOAD = "load twitch\r";  // loads settings from a file
    // used for testing, might need later
    char SET_COMM [13] = {'s','e','t',' ','c','o','m','m',' ','$',' ','m',0x0d};
    char SET_COMM_OPEN [19] = {'s','e','t',' ','c','o','m','m',' ','o','p','e','n',' ','p','o','o','p',0x0d};
    char GET_APMODE [12] = {'g','e','t',' ','a','p','m','o','d','e',0x0d};
    // commands we will need for the design
    char *SET_WLAN_PHRASE = "set wlan phrase wittytrain\r";
    char *JOIN_STEELMESH = "join steelmesh\r";
    char *SET_IP_PROTO = "set ip proto 18\r";
    char *SET_DNS_NAME = "set dns name 192.168.1.7\r";
    char *SET_IP_ADDRESS = "set ip address \r"; //ADD IP ADDRESS
    char *SET_IP_REMOTE = "set ip remote 3000\r"; // PORT NUMBER
    char *SET_COM_REMOTE = "set com remote GET$/feeds/1/start\r"; // string that begins every data transmission
    char *SET_IP_HOST = "set ip host 0\r";  // turns on DNS
    char *SET_UART_MODE = "set uart mode 2\r"; // UART data trigger mode
    char *SET_OPTION_FORMAT = "set option format 0\r"; //send html header
    char *SET_SYS_AUTO = "set sys auto 10\r"; //automatically connect every 10 seconds

    // enter command mode
    tx_string(ENTER_CMD);
    // receive CMD from WiFi module.
    RESPONSE[0] = rx_char();
    RESPONSE[1] = rx_char();
    RESPONSE[2] = rx_char();

    // set the password for the network
    tx_string(SET_WLAN_PHRASE);

    if(rx_string(strlen(SET_WLAN_PHRASE)) == WIFI_FAIL) {
        fail_wait();
    }

    // join the network
    tx_string(JOIN_STEELMESH);

    if(rx_string(strlen(JOIN_STEELMESH)+49) == WIFI_FAIL) {
        fail_wait();
    }

    // send a config command to WiFi module
    tx_string(SET_IP_PROTO);


    // receive AOK response
    if(rx_string(20) == WIFI_FAIL) {
        fail_wait();
    }

    tx_string(SET_DNS_NAME);  //need to add webserver name

    if(rx_string(strlen(SET_DNS_NAME)) == WIFI_FAIL) {
        fail_wait();
    }

    // turn on DNS
    tx_string(SET_IP_HOST);

    // receive AOK response
    if(rx_string(strlen(SET_IP_HOST)+8) == WIFI_FAIL) {
        fail_wait();
    }

    // set webserver port, 80 is standard, but may be changed
    tx_string(SET_IP_REMOTE);

    if(rx_string(strlen(SET_IP_REMOTE)+10) == WIFI_FAIL) {
        fail_wait();
    }

    // set the string that begins every data transmission
    tx_string(SET_COM_REMOTE);

    // receive AOK response
    if(rx_string(strlen(SET_COM_REMOTE)-8) == WIFI_FAIL) {
        fail_wait();
    }

    //automatically connect every 10 seconds
    tx_string(SET_SYS_AUTO);  

    if(rx_string(strlen(SET_SYS_AUTO)+8) == WIFI_FAIL) {
        fail_wait();
    }

    // send HTML header
    tx_string(SET_OPTION_FORMAT);

    // receive AOK response
    if(rx_string(strlen(SET_OPTION_FORMAT)+8) == WIFI_FAIL) {
        fail_wait();
    }

    // set to data trigger mode
    tx_string(SET_UART_MODE);  

    if(rx_string(strlen(SET_UART_MODE)+8) == WIFI_FAIL) {
        fail_wait();
    }

    // save the settings to a file named "twitch"
    tx_string(SAVE);

    // receive AOK response
    if(rx_string(strlen(SAVE)+28) == WIFI_FAIL) {
        fail_wait();
    }

    tx_string(REBOOT);   //reboot the module for the settings to take effect

    if(rx_string(strlen(REBOOT)+10) == WIFI_FAIL) {
        fail_wait();
    }

    // wait for reboot
    while(i<10000) {
        i++;
    }

    // enter command mode again to load config file, may not be necessary
    tx_string(ENTER_CMD);

    if(rx_string(3) == WIFI_FAIL) {
        fail_wait();
    }

    // load "twitch" config file
    tx_string(LOAD);

    if(rx_string(strlen(LOAD)+4) == WIFI_FAIL) {
        fail_wait();
    }

    // wait some more, for funsies
    i=0;
    while(i<10000) {
        i++;
    }

    // exit command mode to start transmitting data
    tx_string(EXIT_CMD);

    if(rx_string(strlen(EXIT_CMD)+1) == WIFI_FAIL) {
        fail_wait();
    }

    // waiting is really fun
    i=0;
    while(i<10000) {
        i++;
    }

}