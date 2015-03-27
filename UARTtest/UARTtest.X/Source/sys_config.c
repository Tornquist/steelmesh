/*
 * File:   sys_config.h
 * Author: Josh Hannan
 *
 * Created on March 26, 2015, 12:59 PM
 */

#include "wifi.h"
#include "sys_config.h"

//------------------------------------------------------------------------------
//  setup_config
//  sets ports as digital outputs, disables timers, configures output compare,
//  and enables timers
//------------------------------------------------------------------------------
void setup_config(void)
{
    __builtin_disable_interrupts();
    // disable watchdog and deadman timers
    WDTCONbits.ON = 0;
    DMTCONbits.ON = 0;

    // set all ports as outputs
    TRISACLR = 0xFFFF;
    TRISBCLR = 0xFFFF;
    TRISGCLR = 0xFFFF;
    TRISDCLR = 0xFFFF;

    // set all ports as digital
    ANSELACLR = 0xFFFF;
    ANSELBCLR = 0xFFFF;
    ANSELGCLR = 0xFFFF;
    ANSELDCLR = 0xFFFF;

    // disable jtag
    CFGCONbits.JTAGEN = 0;

    // configure Output Compare (PWM)
    OC1CON = 0x0000;
    OC1R = 0x0064;        // set initial duty cycle
    OC1RS = 0x0064;       // set duty cycle
    OC1CON = 0x0006;      // set mode to PWM
    PR2 = 0x00C7;         // sets period of PWM

    T2CONSET = 0x8000;    // enable timer 2
    OC1CONSET = 0x8000;   // enable Output Compare (PWM)

    __builtin_enable_interrupts();
}

//------------------------------------------------------------------------------
//  setup_pps
//  sets configuration registers for peripheral pin
//  select
//------------------------------------------------------------------------------
void setup_pps(void)
{
    SYSKEY = 0x0;         //i dont know what these do
    SYSKEY = 0xAA996655;
    SYSKEY = 0x556699AA;

    CFGCONbits.IOLOCK = 0;    // unlocks config
    U1RXRbits.U1RXR = 0b0001; // Set RX to RPG8
    RPG7Rbits.RPG7R = 0b0001; // Set TX to RPG7
    RPD0Rbits.RPD0R = 0b1100; // set OC1 PWM output to RD0
    //RPB7Rbits.RPB7R = 0x000C;
    CFGCONbits.IOLOCK = 1;    // locks config

    ANSELGbits.ANSG7 = 0;     // sets G7 to digital
    ANSELGbits.ANSG8 = 0;     // sets G8 to digital

    SYSKEY = 0x0;
}

//------------------------------------------------------------------------------
//  setup_uart
//  sets configuration registers for UART
//------------------------------------------------------------------------------
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
