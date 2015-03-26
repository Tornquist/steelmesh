/*
 * File:   wifi.c
 * Author: Josh Hannan
 *
 * Contains helper functions for communicating with the WiFi module
 *
 * Created on March 22, 2015, 11:46 AM
 *
 */

#include "wifi.h"

void fail_wait(void)
{
    while(1)
    {
        continue;
    }
}
void join_steelmesh()
{
    char *SET_WLAN_PHRASE = "set wlan phrase wittytrain\r";
    char *JOIN_STEELMESH = "join steelmesh\r";
    // set the password for the network
    tx_string(SET_WLAN_PHRASE);

    if(rx_string(strlen(SET_WLAN_PHRASE)) == WIFI_FAIL) {
        fail_wait();
    }
    // join the network
    tx_string(JOIN_STEELMESH);

    if(rx_string(strlen(JOIN_STEELMESH)+89) == WIFI_FAIL) {
        fail_wait();
    }
}

void wifi_set_config()
{
    char RESPONSE [50] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
    // commands to enter and exit command mode
    char *ENTER_CMD = "$$$";
    char *EXIT_CMD = "exit\r";
    char *SET_IP_PROTO = "set ip proto 18\r";
    char *SET_DNS_NAME = "set dns name 192.168.1.5\r";
    char *SET_IP_ADDRESS = "set ip address \r"; //ADD IP ADDRESS
    char *SET_IP_REMOTE = "set ip remote 3000\r"; // PORT NUMBER
    char *SET_COM_REMOTE_2 = "set com remote GET$/feeds/1/start\r"; // string that begins every data transmission
    char *SET_COM_REMOTE = "set com remote 0\r";
    char *SET_IP_HOST = "set ip host 192.168.1.4\r";  // turns on DNS
    char *SET_UART_MODE = "set uart mode 2\r"; // UART data trigger mode
    char *SET_OPTION_FORMAT = "set option format 0\r"; //send html header
    char *SET_SYS_AUTO = "set sys auto 10\r"; //automatically connect every 10 seconds
    char *REBOOT = "reboot\r";  // reboots the module
    char *SAVE = "save twitch\r";  // saves settings to a file
    int i = 0;

    // enter command mode
    tx_string(ENTER_CMD);
    // receive CMD from WiFi module.
    RESPONSE[0] = rx_char();
    RESPONSE[1] = rx_char();
    RESPONSE[2] = rx_char();

    // send a config command to WiFi module
    tx_string(SET_IP_PROTO);


    // receive AOK response
    if(rx_string(20) == WIFI_FAIL) {
        fail_wait();
    }

    //tx_string(SET_DNS_NAME);  //need to add webserver name

    //if(rx_string(strlen(SET_DNS_NAME)) == WIFI_FAIL) {
    //    fail_wait();
    //}

    // turn on DNS
    tx_string(SET_IP_HOST);

    // receive AOK response
    if(rx_string(strlen(SET_IP_HOST)+8) == WIFI_FAIL) {
        fail_wait();
    }

    // set webserver port, 80 is standard, but may be changed
    tx_string(SET_IP_REMOTE);

    if(rx_string(strlen(SET_IP_REMOTE)+10) == WIFI_FAIL) {
        fail_wait();
    }

    // set the string that begins every data transmission
    tx_string(SET_COM_REMOTE);

    // receive AOK response
    if(rx_string(strlen(SET_COM_REMOTE)-8) == WIFI_FAIL) {
        fail_wait();
    }

    //automatically connect every 10 seconds
    tx_string(SET_SYS_AUTO);

    if(rx_string(strlen(SET_SYS_AUTO)+8) == WIFI_FAIL) {
        fail_wait();
    }

    // send HTML header
    tx_string(SET_OPTION_FORMAT);

    // receive AOK response
    if(rx_string(strlen(SET_OPTION_FORMAT)+8) == WIFI_FAIL) {
        fail_wait();
    }

    // set to data trigger mode
    tx_string(SET_UART_MODE);

    if(rx_string(strlen(SET_UART_MODE)+8) == WIFI_FAIL) {
        fail_wait();
    }

    // save the settings to a file named "twitch"
    tx_string(SAVE);

    // receive AOK response
    if(rx_string(strlen(SAVE)+28) == WIFI_FAIL) {
        fail_wait();
    }

    tx_string(REBOOT);   //reboot the module for the settings to take effect

    if(rx_string(strlen(REBOOT)+10) == WIFI_FAIL) {
        fail_wait();
    }

    // wait for reboot
    while(i<10000) {
        i++;
    }
}

void wifi_open_connection(void)
{
    char *ENTER_CMD = "$$$";
    char *EXIT_CMD = "exit\r";
    char *OPEN = "open 192.168.1.4 3000\r";

    // enter command mode again to load config file, may not be necessary
    tx_string(ENTER_CMD);

    if(rx_string(3) == WIFI_FAIL) {
        fail_wait();
    }

    tx_string(OPEN);

    if(rx_string(strlen(OPEN)+8) == WIFI_FAIL) {
        fail_wait();
    }

    tx_string(EXIT_CMD);

    if(rx_string(strlen(EXIT_CMD)+1) == WIFI_FAIL) {
        fail_wait();
    }
}

void wifi_open_config(void)
{
    int i = 0;
    char bot_id = '1';
    char RESPONSE [50] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
    // commands to enter and exit command mode
    char *ENTER_CMD = "$$$";
    char *EXIT_CMD = "exit\r";
    char *REBOOT = "reboot\r";  // reboots the module
    char *SAVE = "save twitch\r";  // saves settings to a file
    char *LOAD = "load twitch\r";  // loads settings from a file
    // commands we will need for the design
    
    char *START = "GET$/feeds/1/start";

    // enter command mode again to load config file, may not be necessary
    tx_string(ENTER_CMD);

    if(rx_string(3) == WIFI_FAIL) {
        fail_wait();
    }

    // load "twitch" config file
    tx_string(LOAD);

    if(rx_string(strlen(LOAD)+4) == WIFI_FAIL) {
        fail_wait();
    }

    tx_string(REBOOT);   //reboot the module for the settings to take effect

    if(rx_string(strlen(REBOOT)+10) == WIFI_FAIL) {
        fail_wait();
    }

    // wait for reboot
    while(i<10000) {
        i++;
    }
}