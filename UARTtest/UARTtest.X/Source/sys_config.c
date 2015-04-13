/*
 * File:   sys_config.h
 * Author: Josh Hannan
 *
 * Created on March 26, 2015, 12:59 PM
 */


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
    //TRISACLR = 0xFFFF;
    TRISBCLR = 0xFFFF;
    TRISGCLR = 0xFFFF;
    TRISDCLR = 0xFFFF;
    TRISFCLR = 0xFFFF;


    // set all ports as digital
    //ANSELACLR = 0xFFFF;
    ANSELBCLR = 0xFFFF;
    ANSELGCLR = 0xFFFF;
    ANSELDCLR = 0xFFFF;
    ANSELFCLR = 0xFFFF;

    //THESE LINES DON'T SEEM TO DO ANYTHING
    TRISFbits.TRISF3 = 0;
    TRISBbits.TRISB6 = 0;

    //TRISFbits.TRISF3 = 0;
    // disable jtag
    CFGCONbits.JTAGEN = 0;

    //COMPILES UNCOMMENTED, WE WERE JUST TRYING TO GET IT TO NOT SUCK
    /*
    // configure Output Compare (PWM)
    OC1CON = 0x0000;
    T2CON = 0x0000;
    //OC1CONbits.OC32 = 1;
    //T2CONSET = 0x0038;
    TMR2 = 0x0;
    OC1R = 0x7FFFF;        // set initial duty cycle
    OC1RS = 0x7FFFF;       // set duty cycle
    OC1CON = 0x0006;      // set mode to PWM
    PR2 = 0xFFFFF;         // sets period of PWM
    
    T2CONSET = 0x8000;    // enable timer 2
    OC1CONSET = 0x8000;   // enable Output Compare (PWM)


    // output compare 2
    OC2CON = 0x0000;
    OC2R = 0x0064;        // set initial duty cycle
    OC2RS = 0x0064;       // set duty cycle
    OC2CON = 0x0006;      // set mode to PWM
    PR3 = 0x00C7;         // sets period of PWM

    T3CONSET = 0x8000;    // enable timer 2
    OC2CONSET = 0x8000;   // enable Output Compare (PWM)

    // output compare 3
    OC3CON = 0x0000;
    OC3R = 0x007F;        // set initial duty cycle
    OC3RS = 0x007F;       // set duty cycle
    OC3CON = 0x0006;      // set mode to PWM
    PR4 = 0x00FF;         // sets period of PWM

    T4CONSET = 0x8000;    // enable timer 2
    OC3CONSET = 0x8000;   // enable Output Compare (PWM)
    */

    __builtin_enable_interrupts();
}

//------------------------------------------------------------------------------
//  setup_pps
//  sets configuration registers for peripheral pin
//  select
//------------------------------------------------------------------------------
void setup_pps(void)
{
    int i = 0;

    SYSKEY = 0x0;         //i dont know what these do
    SYSKEY = 0xAA996655;
    SYSKEY = 0x556699AA;

    CFGCONbits.IOLOCK = 0;    // unlocks config
    U1RXRbits.U1RXR = 0b0001; // Set RX to RPG8
    RPG7Rbits.RPG7R = 0b0001; // Set TX to RPG7

    //I ADDED THESE LINES THIS MORNING (4/11/15)
    RPF3Rbits.RPF3R = 0b0000;
    RPB6Rbits.RPB6R = 0b0000;

    //I CHANGED THE BELOW ONE THIS MORNING (4/11/15)
    //RPF3Rbits.RPF3R = 0b1100; // set OC1 PWM output to RD0
    RPB14Rbits.RPB14R = 0b1011; // set OC2 PWM output to RB14
    RPB9Rbits.RPB9R = 0b1011;   // set OC4 PWM output to RB9
    RPB7Rbits.RPB7R = 0x000C;
    CFGCONbits.IOLOCK = 1;    // locks config

    ANSELGbits.ANSG7 = 0;     // sets G7 to digital
    ANSELGbits.ANSG8 = 0;     // sets G8 to digital

    SYSKEY = 0x0;


    // wait for settings to take effect
    while(i<2000){
        i++;
    }
    i = 0;
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
