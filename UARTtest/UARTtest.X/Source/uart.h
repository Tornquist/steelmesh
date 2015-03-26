/* 
 * File:   uart.h
 * Author: Josh Hannan
 *
 * Created on March 26, 2015, 12:56 PM
 */

#ifndef UART_H
#define	UART_H

void tx_string(char *string);

void tx_char(unsigned char c);

int rx_string(int length);

char rx_char();



#endif	/* UART_H */

