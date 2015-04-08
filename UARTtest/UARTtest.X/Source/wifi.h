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
#include "uart.h"

#define FPB 99000000L // Frequency Peripheral Bus = 99MHz
#define BAUDRATE 9600

#define WIFI_SUCCESS 1
#define WIFI_FAIL 0

void fail_wait(void);

void wifi_set_config(void);

void wifi_set_real_config(void);

void wifi_open_connection(void);

void wifi_open_config(void);

void wifi_update_device(void);

void wifi_reboot(void);

void wifi_data_start(void);

void wifi_data(void);

void wifi_data_end(void);

#endif	/* WIFI_H */


