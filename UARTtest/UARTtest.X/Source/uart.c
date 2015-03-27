/*
 * File:   uart.h
 * Author: Josh Hannan
 *
 * Created on March 26, 2015, 12:56 PM
 */

#include "wifi.h"
#include "uart.h"

//------------------------------------------------------------------------------
//  tx_string
//  transmits a string of length ascii characters
//  over UART
//------------------------------------------------------------------------------
void tx_string(char *string)
{
    int i;
    for(i = 0; i < strlen(string); i++) {
        tx_char(string[i]);
    }
}

//------------------------------------------------------------------------------
//  tx_char
//  transmits one ascii character over UART
//------------------------------------------------------------------------------
void tx_char(unsigned char c)
{
    int i = 0;
    // wait for transmit register to be empty
    while(U1STAbits.UTXBF);
    // place a character in transmit register
    U1TXREG = c;
}

//------------------------------------------------------------------------------
//  rx_string
//  receives a string of length ascii characters
//  over UART
//------------------------------------------------------------------------------
int rx_string(int length)
{
    int i;
    int valid = 0;
    char RESPONSE [120] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
    for(i = 0; i < length; i++) {
        RESPONSE[i] = rx_char();
        if (RESPONSE[i] == 'A' || RESPONSE[i] == 'O' || RESPONSE[i] == 'K') {
            valid++;
        } else if (RESPONSE[i] == 0x00) {
            return UART_FAIL;
        }
    }

    if(valid == 3) {
        return WIFI_SUCCESS;
    }
    return UART_SUCCESS;
}
//------------------------------------------------------------------------------
//  rx_char
//  receives one ascii character over UART
//------------------------------------------------------------------------------
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
