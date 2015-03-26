/* 
 * File:   motor.h
 * Author: Josh Hannan
 *
 * Created on March 26, 2015, 10:53 AM
 */

#ifndef MOTOR_H
#define	MOTOR_H

#include <stdio.h>
#include <stdlib.h>
//#include <plib.h>
#include "proc/p32mz2048ech100.h"
#include "p32xxxx.h"
#include <string.h>

#define FPB 99000000L // Frequency Peripheral Bus = 99MHz
#define MOTOR_SUCCESS 1
#define MOTOR_FAIL 0

void turn_right(void);

void turn_left(void);

void forward(void);

void backward(void);

void paddle_kick(void);

int command_execute(char *command);


#endif	/* MOTOR_H */

