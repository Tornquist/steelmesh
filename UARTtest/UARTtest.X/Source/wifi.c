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
idle_delay(100000);
    // Turn on HTTP mode and receive response
    tx_string(SET_IP_PROTO);
    if(rx_string(strlen(SET_IP_PROTO) + 14) == WIFI_FAIL) { fail_wait(); }
idle_delay(100000);
    tx_string(SET_WLAN_PHRASE);
    if(rx_string(strlen(SET_WLAN_PHRASE) + 14) == WIFI_FAIL) { fail_wait(); }
idle_delay(100000);
    tx_string(SET_WLAN_SSID);
    if(rx_string(strlen(SET_WLAN_SSID) + 14) == WIFI_FAIL) { fail_wait(); }
idle_delay(100000);

    // set webserver port to 3000
    tx_string(SET_IP_REMOTE);
    if(rx_string(strlen(SET_IP_REMOTE) + 14) == WIFI_FAIL) { fail_wait(); }
idle_delay(100000);
    // turn on DNS, or maybe set webserver IP address. I dont know
    tx_string(SET_IP_HOST);
    if(rx_string(strlen(SET_IP_HOST) + 6) == WIFI_FAIL) { fail_wait(); }
idle_delay(100000);
    // Set the WiFi module to open a connection when UART data is received
    tx_string(SET_UART_MODE);
    if(rx_string(strlen(SET_UART_MODE) + 14) == WIFI_FAIL) { fail_wait(); }
idle_delay(1000000);
    // set the string that begins every data transmission
    tx_string(SET_COM_REMOTE_START);
    if(rx_string(strlen(SET_COM_REMOTE_START)-7) == WIFI_FAIL) { fail_wait(); }
idle_delay(1000000);
    // Set the WiFi module to open a connection when UART data is received
    tx_string(JOIN);
    if(rx_string(strlen(JOIN) + 100) == WIFI_FAIL) { fail_wait(); }
idle_delay(100000);
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
idle_delay(100000);
    tx_string("factory RESET\r");
    if(rx_string(47) == WIFI_FAIL) { fail_wait(); }
idle_delay(100000);
    tx_string(REBOOT);
    if(rx_string(strlen(REBOOT)+10) == WIFI_FAIL) { fail_wait(); }
idle_delay(10000000);
    
}