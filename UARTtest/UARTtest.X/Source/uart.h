/* 
 * File:   uart.h
 * Author: Josh Hannan
 *
 * Created on March 26, 2015, 12:56 PM
 */

#ifndef UART_H
#define	UART_H

#define UART_FAIL 0
#define UART_SUCCESS 1

#define ROBOT_NOP 0
#define ROBOT_FORWARD 1
#define ROBOT_BACK 2
#define ROBOT_LEFT 3
#define ROBOT_RIGHT 4
#define ROBOT_KICK 5

void tx_string(char *string);

void tx_char(unsigned char c);

int rx_command();

int rx_string(int length);

char rx_char();



#endif	/* UART_H */

