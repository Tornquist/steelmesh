/*
 * File:   wifi.h
 * Author: Josh Hannan
 *
 * Created on March 22, 2015, 11:47 AM
 */

#ifndef WIFI_H
#define	WIFI_H

#include <stdio.h>
#include <stdlib.h>
#include "proc/p32mz2048ech100.h"
#include "p32xxxx.h"
#include <string.h>

#define FPB 99000000L // Frequency Peripheral Bus = 99MHz
#define BAUDRATE 9600

#define WIFI_SUCCESS 1
#define WIFI_FAIL 0

void setup_config(void);

void setup_pps(void);

void setup_uart(void);

void tx_string(char *string);

void tx_char(unsigned char c);

int rx_string(int length);

char rx_char();

void fail_wait(void);

void wifi_config(void);



#endif	/* WIFI_H */


