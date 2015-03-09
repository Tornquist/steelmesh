/* 
 * File:   wifi.h
 * Author: Josh Hannan
 *
 * Created on February 28, 2015.
 */

#ifndef WIFI_H
#define	WIFI_H

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "system_config.h"
#include "system_definitions.h"

// *****************************************************************************
// *****************************************************************************
// Section: Constant Definitions
// *****************************************************************************
// *****************************************************************************

#define WIFI_CMD_ENTER_LENGTH 3
#define WIFI_COMMAND_LENGTH 30
#define WIFI_USART_ID USART_ID_1
#define WIFI_SUCCESS true
#define WIFI_FAIL false




// *****************************************************************************
// *****************************************************************************
// Section: Type Definitions
// *****************************************************************************
// *****************************************************************************

typedef struct {
    /* USART handle for wifi module */
    DRV_HANDLE wifi_handle;
    
    /* command store from wifi module */
    char wifi_data_array[3];
    uint8_t test;
    /* Number of packages required to send complete image */
    //int cam_data_package_number;
    
    /* Current position in image array when sending */
    //int cam_data_send_index;

    /* Current position in image array when receiving */
    //int cam_data_receive_index;

    /* Tracks image package id to use in ACK response */
    //int cam_data_package_counter;

    /* Command response received from the wifi module */
    uint8_t wifi_received_command [WIFI_COMMAND_LENGTH];
    
} WIFI_DATA;

// *****************************************************************************
// *****************************************************************************
// Section: Function Prototypes
// *****************************************************************************
// *****************************************************************************

/* Initialize fields of WIFI_DATA structure */
bool wifi_data_initialize(WIFI_DATA* wifi_data);

/* Send $$$ to wifi module to enter command mode */
bool wifi_enter_cmd_mode(WIFI_DATA* wifi_data);

/* Receive status responses from wifi module every time a command is sent */
bool wifi_receive_response(WIFI_DATA* wifi_data);


#endif	/* WIFI_H */

