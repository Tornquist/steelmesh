/* 
 * File:   motor.h
 * Author: Josh Hannan
 *
 * Created on March 26, 2015, 10:53 AM
 */

#ifndef MOTOR_H
#define	MOTOR_H

#include "system_include.h"

#define MOTOR_SUCCESS 1
#define MOTOR_FAIL 0

void robot_move(int lDir, int rDir, int time);
void robot_nop(void);
void robot_right(void);
void robot_left(void);
void robot_forward(void);
void robot_back(void);
void robot_kick(void);
void idle_delay(int value);

#endif	/* MOTOR_H */

