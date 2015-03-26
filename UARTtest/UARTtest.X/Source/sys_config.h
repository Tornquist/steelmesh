/* 
 * File:   sys_config.h
 * Author: Josh Hannan
 *
 * Created on March 26, 2015, 12:59 PM
 */

#ifndef SYS_CONFIG_H
#define	SYS_CONFIG_H

#include <stdio.h>
#include <stdlib.h>
#include "proc/p32mz2048ech100.h"
#include "p32xxxx.h"
#include <string.h>

#define FPB 99000000L // Frequency Peripheral Bus = 99MHz
#define BAUDRATE 9600

void setup_config(void);

void setup_pps(void);

void setup_uart(void);


#endif	/* SYS_CONFIG_H */

