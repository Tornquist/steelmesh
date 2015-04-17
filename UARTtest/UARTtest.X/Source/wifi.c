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

void fail_wait(void) {
    while(1)
    {
        continue;
    }
}

void wifi_open_config(void) {
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
    if(rx_string(strlen(LOAD)+16) == WIFI_FAIL) {
        fail_wait();
    }

//    tx_string(REBOOT);   //reboot the module for the settings to take effect
//    if(rx_string(strlen(REBOOT)+10) == WIFI_FAIL) {
//        fail_wait();
//    }

    // exit command mode to go to data mode
    tx_string(EXIT_CMD);
    if(rx_string(strlen(EXIT_CMD)+1) == WIFI_FAIL) {
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
//------------------------------------------------------------------------------
void wifi_update_device(void) {
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
//   wifi_set_real_config
//   sets the configuration settings to connect to the wifi network and webserver
//------------------------------------------------------------------------------
void wifi_set_real_config(char id) {
    char *ENTER_CMD = "$$$";
    char *EXIT_CMD = "exit\r";
    char *SET_IP_PROTO = "set i p 18\r";
    char *SET_WLAN_SSID = "set w s steelmesh\r";
    char *SET_WLAN_PHRASE = "set w p wittytrain\r";
    char *SET_IP_REMOTE = "set ip remote 3000\r"; // PORT NUMBER
    char *SET_IP_HOST = "set ip host 192.168.1.7\r";  // turns on DNS
    char *SET_UART_MODE = "set u m 2\r"; // UART data trigger mode
    char *JOIN = "join\r";
    char *SET_COM_REMOTE_START = "set comm remote GET$/command/1?data=\r"; // string that begins every data transmission
    SET_COM_REMOTE_START[29] = id;
    
    // enter command mode
    tx_string(ENTER_CMD);
    if(rx_string(3) == WIFI_FAIL) { fail_wait(); }

    // Turn on HTTP mode and receive response
    tx_string(SET_IP_PROTO);
    if(rx_string(strlen(SET_IP_PROTO) + 14) == WIFI_FAIL) { fail_wait(); }

    tx_string(SET_WLAN_PHRASE);
    if(rx_string(strlen(SET_WLAN_PHRASE) + 14) == WIFI_FAIL) { fail_wait(); }

    tx_string(SET_WLAN_SSID);
    if(rx_string(strlen(SET_WLAN_SSID) + 14) == WIFI_FAIL) { fail_wait(); }


    // set webserver port to 3000
    tx_string(SET_IP_REMOTE);
    if(rx_string(strlen(SET_IP_REMOTE) + 14) == WIFI_FAIL) { fail_wait(); }

    // turn on DNS, or maybe set webserver IP address. I dont know
    tx_string(SET_IP_HOST);
    if(rx_string(strlen(SET_IP_HOST) + 6) == WIFI_FAIL) { fail_wait(); }

    // Set the WiFi module to open a connection when UART data is received
    tx_string(SET_UART_MODE);
    if(rx_string(strlen(SET_UART_MODE) + 14) == WIFI_FAIL) { fail_wait(); }

    // set the string that begins every data transmission
    tx_string(SET_COM_REMOTE_START);
    if(rx_string(strlen(SET_COM_REMOTE_START)-7) == WIFI_FAIL) { fail_wait(); }

    // Set the WiFi module to open a connection when UART data is received
    tx_string(JOIN);
    if(rx_string(strlen(JOIN) + 100) == WIFI_FAIL) { fail_wait(); }

    // exit command mode to go to data mode
    tx_string(EXIT_CMD);
    if(rx_string(strlen(EXIT_CMD)+1) == WIFI_FAIL) {
        fail_wait();
    }


}


void wifi_load_twitch() {
    char *ENTER_CMD = "$$$";
    char *EXIT_CMD = "exit\r";
    char *SET_IP_PROTO = "set i p 18\r";
    char *SET_WLAN_SSID = "set w s steelmesh\r";
    char *SET_WLAN_PHRASE = "set w p wittytrain\r";
    char *SET_IP_REMOTE = "set ip remote 3000\r"; // PORT NUMBER
    char *SET_IP_HOST = "set ip host 192.168.1.7\r";  // turns on DNS
    char *SET_UART_MODE = "set u m 2\r"; // UART data trigger mode
    char *JOIN = "join\r";
    char *SET_COM_REMOTE_START = "set comm remote GET$/command/1?data=\r"; // string that begins every data transmission

    char * LOAD_TWITCH = "load twitch\r";

    // enter command mode
    tx_string(ENTER_CMD);
    if(rx_string(3) == WIFI_FAIL) { fail_wait(); }

    tx_string(LOAD_TWITCH);
    if(rx_string(strlen(LOAD_TWITCH)+10) == WIFI_FAIL) { fail_wait(); }

    // Set the WiFi module to open a connection when UART data is received
    tx_string(JOIN);
    if(rx_string(strlen(JOIN) + 125) == WIFI_FAIL) { fail_wait(); }


    // exit command mode to go to data mode
    tx_string(EXIT_CMD);
    if(rx_string(strlen(EXIT_CMD)+1) == WIFI_FAIL) {
        fail_wait();
    }

    
}

//------------------------------------------------------------------------------
//   wifi_reboot
//   performs a factory reset on the wifi module and reboots it
//------------------------------------------------------------------------------
void wifi_reboot() {
    char *ENTER_CMD = "$$$";
    char *EXIT_CMD = "exit\r";
    char *REBOOT = "reboot\r";
    // enter command mode
    tx_string(ENTER_CMD);
    
    if(rx_string(3) == WIFI_FAIL) { fail_wait(); }

    tx_string("factory RESET\r");
    if(rx_string(47) == WIFI_FAIL) { fail_wait(); }

    tx_string(REBOOT);
    if(rx_string(strlen(REBOOT)+10) == WIFI_FAIL) { fail_wait(); }

    // wait for reboot
    int i = 0;
    while(i<10000000) {
        i++;
    }
    
}

void wifi_data_start(void) {
    char *ENTER_CMD = "$$$";
    char *EXIT_CMD = "exit\r";
    char *SET_COM_REMOTE_START = "set comm remote GET$/feeds/1/start?data=\r"; // string that begins every data transmission
    
    // enter command mode
    tx_string(ENTER_CMD);
    if(rx_string(3) == WIFI_FAIL) { fail_wait(); }

    // set the string that begins every data transmission
    tx_string(SET_COM_REMOTE_START);
    if(rx_string(strlen(SET_COM_REMOTE_START)-7) == WIFI_FAIL) { fail_wait(); }

    // exit command mode to go to data mode
    tx_string(EXIT_CMD);
    if(rx_string(strlen(EXIT_CMD)+1) == WIFI_FAIL) {
        fail_wait();
    }

}

void wifi_data(void) {
    char *ENTER_CMD = "$$$";
    char *EXIT_CMD = "exit\r";
    char *SET_COM_REMOTE = "set c r GET$/feeds/1/image?data=\r"; // string that begins every data transmission
    
    // enter command mode
    tx_string(ENTER_CMD);
    if(rx_string(3) == WIFI_FAIL) { fail_wait(); }

    // set the string that begins every data transmission
    tx_string(SET_COM_REMOTE);
    if(rx_string(strlen(SET_COM_REMOTE)+2) == WIFI_FAIL) { fail_wait(); }
    
    // exit command mode to go to data mode
    tx_string(EXIT_CMD);
    if(rx_string(strlen(EXIT_CMD)+1) == WIFI_FAIL) {
        fail_wait();
    }

}

void wifi_data_end(void) {
    char *ENTER_CMD = "$$$";
    char *EXIT_CMD = "exit\r";
    char *SET_COM_REMOTE_END = "set c r GET$/feeds/1/end?data=\r"; // string that begins every data transmission

    // enter command mode
    tx_string(ENTER_CMD);
    if(rx_string(3) == WIFI_FAIL) { fail_wait(); }

    // set the string that begins every data transmission
    tx_string(SET_COM_REMOTE_END);
    if(rx_string(strlen(SET_COM_REMOTE_END)+2) == WIFI_FAIL) { fail_wait(); }

    // exit command mode to go to data mode
    tx_string(EXIT_CMD);
    if(rx_string(strlen(EXIT_CMD)+1) == WIFI_FAIL) {
        fail_wait();
    }

    tx_string("1234");

}

