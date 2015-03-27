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

//------------------------------------------------------------------------------
//  join_steelmesh
//  sets WPA passphrase and joins the steelmesh WiFi network
//
//------------------------------------------------------------------------------
void wifi_join_steelmesh()
{
    char *SET_WLAN_PHRASE = "set wlan phrase wittytrain\r";
    char *JOIN_STEELMESH = "join steelmesh\r";
    char *ENTER_CMD = "$$$";
    char *EXIT_CMD = "exit\r";

    // enter command mode
    tx_string(ENTER_CMD);
    if(rx_string(3) == WIFI_FAIL) {
        fail_wait();
    }

    // set the password for the network
    tx_string(SET_WLAN_PHRASE);
    if(rx_string(strlen(SET_WLAN_PHRASE)+5) == WIFI_FAIL) {
        fail_wait();
    }

    // join the network
    tx_string(JOIN_STEELMESH);
    if(rx_string(strlen(JOIN_STEELMESH)+95) == WIFI_FAIL) {
        fail_wait();
    }

    // exit command mode to go to data mode
    tx_string(EXIT_CMD);
    if(rx_string(strlen(EXIT_CMD)+1) == WIFI_FAIL) {
        fail_wait();
    }
}

//------------------------------------------------------------------------------
//  wifi_open_config
//  Enters Command mode and opens a file that contains saved configuration
//  settings and applies them
//------------------------------------------------------------------------------
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

//------------------------------------------------------------------------------
//  wifi_update_device
//  Updates the firmware
//
//------------------------------------------------------------------------------
void wifi_update_device(void)
{
    char *ENTER_CMD = "$$$";
    char *EXIT_CMD = "exit\r";
    char *SET_DNS = "set dns backup rn.microchip.com\r";
    char *SET_FTP_USER = "set ftp user roving\r";
    char *SET_FTP_PASS = "set ftp pass Pass123\r";
    char *SET_FTP_DIR = "set ftp dir ./public\r";
    char *SET_FTP_FILE = "ftp update wifly7-400.mif\r";

    // enter command mode to load file
    tx_string(ENTER_CMD);
    if(rx_string(3) == WIFI_FAIL) {
        fail_wait();
    }

    tx_string(SET_DNS);
    if(rx_string(strlen(SET_DNS)+2) == WIFI_FAIL) { fail_wait(); }

    tx_string(SET_FTP_USER);
    if(rx_string(strlen(SET_FTP_USER)+2) == WIFI_FAIL) { fail_wait(); }

    tx_string(SET_FTP_PASS);
    if(rx_string(strlen(SET_FTP_PASS)+2) == WIFI_FAIL) { fail_wait(); }

    tx_string(SET_FTP_DIR);
    if(rx_string(strlen(SET_FTP_DIR)+2) == WIFI_FAIL) { fail_wait(); }
    
    tx_string(SET_FTP_FILE);
    if(rx_string(strlen(SET_FTP_FILE)+2) == WIFI_FAIL) { fail_wait(); }

    // exit command mode to go to data mode
    tx_string(EXIT_CMD);
    if(rx_string(strlen(EXIT_CMD)+1) == WIFI_FAIL) {
        fail_wait();
    }
}

//------------------------------------------------------------------------------
//  wifi_open_connection
//  opens a connection with the webserver
//
//------------------------------------------------------------------------------
void wifi_open_connection(void)
{
    char *ENTER_CMD = "$$$";
    char *EXIT_CMD = "exit\r";
    char *OPEN = "open 192.168.1.7 3000\r";
    char *RUN = "run web_app\r";

    // enter command mode to load file
    tx_string(ENTER_CMD);
    if(rx_string(3) == WIFI_FAIL) {
        fail_wait();
    }

    // open the twitch configuration file and update settings
    tx_string(RUN);
    if(rx_string(strlen(RUN)+20) == WIFI_FAIL) {
    //if(rx_string(strlen(OPEN)+42) == WIFI_FAIL) {
        fail_wait();
    }

    // exit command mode to go to data mode
    tx_string(EXIT_CMD);
    if(rx_string(strlen(EXIT_CMD)+1) == WIFI_FAIL) {
        fail_wait();
    }
}

//------------------------------------------------------------------------------
//  wifi_set_config
//  sends configuration commands to the Wifi
//  module
//------------------------------------------------------------------------------
void wifi_set_config()
{
    char RESPONSE [50] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
    // commands to enter and exit command mode
    char *ENTER_CMD = "$$$";
    char *EXIT_CMD = "exit\r";
    char *SET_IP_PROTO = "set ip proto 18\r";
    char *SET_DNS_NAME = "set dns name 192.168.1.7\r";
    char *SET_IP_ADDRESS = "set ip address 192.168.1.3\r"; //ADD IP ADDRESS (WiFly IP)
    char *SET_IP_REMOTE = "set ip remote 3000\r"; // PORT NUMBER
    char *SET_DHCP_MODE = "set ip dhcp 0\r"; //Disable DHCP
    char *SET_COM_REMOTE = "set com remote GET$/feeds/1/start?data=\r"; // string that begins every data transmission
    char *SET_IP_HOST = "set ip host 192.168.1.7\r";  // turns on DNS
    char *SET_IP_GATEWAY = "set ip gateway 192.168.1.1\r"; //Self explanatory
    char *SET_IP_NETMASK = "set ip gateway 255.255.255.0\r"; //Self explanatory
    char *SET_UART_MODE = "set uart mode 2\r"; // UART data trigger mode
    char *SET_OPTION_FORMAT = "set option format 1\r"; //send html header
    char *SET_SYS_AUTO = "set sys auto 10\r"; //automatically connect every 10 seconds
    char *REBOOT = "reboot\r";  // reboots the module
    char *SAVE = "save twitch\r";  // saves settings to a file
    int i = 0;

    // enter command mode
    tx_string(ENTER_CMD);
    if(rx_string(3) == WIFI_FAIL) { fail_wait(); }

    // Turn on HTTP mode and receive response
    tx_string(SET_IP_PROTO);
    if(rx_string(strlen(SET_IP_PROTO)+13) == WIFI_FAIL) { fail_wait(); }

    // Sets the name of the webserver
    //tx_string(SET_DNS_NAME);
    //if(rx_string(strlen(SET_DNS_NAME)) == WIFI_FAIL) { fail_wait(); }

    // turn on DNS, or maybe set webserver IP address. I dont know
    tx_string(SET_IP_HOST);
    if(rx_string(strlen(SET_IP_HOST)+8) == WIFI_FAIL) { fail_wait(); }

    // set webserver port to 3000
    tx_string(SET_IP_REMOTE);
    if(rx_string(strlen(SET_IP_REMOTE)+10) == WIFI_FAIL) { fail_wait(); }

    tx_string(SET_IP_GATEWAY);
    if(rx_string(strlen(SET_IP_GATEWAY)+3) == WIFI_FAIL) { fail_wait(); }

    tx_string(SET_IP_NETMASK);
    if(rx_string(strlen(SET_IP_NETMASK)+3) == WIFI_FAIL) { fail_wait(); }

    tx_string(SET_DHCP_MODE);
    if(rx_string(strlen(SET_DHCP_MODE)+3) == WIFI_FAIL) { fail_wait(); }
    // set the string that begins every data transmission
    tx_string(SET_COM_REMOTE);
    //if(rx_string(strlen(SET_COM_REMOTE)+3) == WIFI_FAIL) { fail_wait(); }

    // Configures to autoconnect every 10 seconds
    tx_string(SET_SYS_AUTO);
    if(rx_string(strlen(SET_SYS_AUTO)+8) == WIFI_FAIL) { fail_wait(); }

    // send an HTML header?? -- Correct. This is right Josh.
    tx_string(SET_OPTION_FORMAT);
    if(rx_string(strlen(SET_OPTION_FORMAT)+12) == WIFI_FAIL) { fail_wait(); }

    // Set the WiFi module to open a connection when UART data is received
    //tx_string(SET_UART_MODE);
    //if(rx_string(strlen(SET_UART_MODE)+8) == WIFI_FAIL) { fail_wait(); }

    // save the settings to a file named "twitch"
    tx_string(SAVE);
    if(rx_string(strlen(SAVE)+28) == WIFI_FAIL) { fail_wait(); }

    // Reboot the module for the settings to take effect
    tx_string(REBOOT);
    if(rx_string(strlen(REBOOT)+10) == WIFI_FAIL) { fail_wait(); }

    // wait for reboot
    while(i<10000) {
        i++;
    }
}

void wifi_reboot()
{
    char *ENTER_CMD = "$$$";
    char *REBOOT = "reboot\r";
    // enter command mode
    tx_string(ENTER_CMD);
    if(rx_string(3) == WIFI_FAIL) { fail_wait(); }

    tx_string(REBOOT);
    if(rx_string(strlen(REBOOT)+10) == WIFI_FAIL) { fail_wait(); }

    // wait for reboot
    int i = 0;
    while(i<10000) {
        i++;
    }
}

