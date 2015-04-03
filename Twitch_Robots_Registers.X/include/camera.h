/* 
 * File:   camera.h
 * Author: TJ
 *
 * Created on March 23, 2015, 3:06 PM
 */

#ifndef CAMERA_H
#define	CAMERA_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/proc/p32mz2048ech100.h"

// *****************************************************************************
// *****************************************************************************
// Section: Constant Definitions
// *****************************************************************************
// *****************************************************************************

#define CAM_COMMAND_LENGTH 6
#define CAM_DATA_ARRAY_SIZE 512 * 10
#define CAM_BUFFER_SIZE 512 * 2
#define CAM_USART_ID USART_ID_2
#define CAM_SUCCESS 0
#define CAM_FAIL 1

#define CAM_MAX_SYNC_ATTEMPTS 60
#define CAM_MAX_ACK_RECEIVE_ATTEMPTS 10
#define CAM_ACK_FAILED 0xFF

#define CAM_CMD_ID_INITIAL 0x01
#define CAM_CMD_ID_GET_PICTURE 0x04
#define CAM_CMD_ID_SET_PACKAGE_SIZE 0x06
#define CAM_CMD_ID_DATA 0x0A
#define CAM_CMD_ID_SYNC 0x0D
#define CAM_CMD_ID_ACK 0x0E

#define CAM_SIZE_512_LOW_BYTE 0x00
#define CAM_SIZE_512_HIGH_BYTE 0x02

#define CAM_RESOLUTION_160x128 0x03
#define CAM_RESOLUTION_320x240 0x05
#define CAM_RESOLUTION_640x480 0x07

#define CAM_INITIAL_JPEG 0x07
#define CAM_GET_PICTURE_JPEG 0x05

#define CAM_PACKAGE_ID_SIZE 2
#define CAM_PACKAGE_DATA_SIZE 2
#define CAM_PACKAGE_IMAGE_SIZE 512 - 6
#define CAM_PACKAGE_VERIFY_SIZE 2

#define CAM_PACKAGE_FINAL 0xF0

#define FPB 99000000L // Frequency Peripheral Bus = 99MHz
#define BAUDRATE 9600

// *****************************************************************************
// *****************************************************************************
// Section: Type Definitions
// *****************************************************************************
// *****************************************************************************

typedef struct {
    /* Image data store from camera */
    char cam_data_array[CAM_DATA_ARRAY_SIZE];

    /* Number of packages required to send complete image */
    int cam_data_package_number;

    /* Current position in image array when sending */
    int cam_data_send_index;

    /* Current position in image array when receiving */
    int cam_data_receive_index;

    /* Tracks image package id to use in ACK response */
    int cam_data_package_counter;

    /* Command response received from the camera */
    char cam_received_command [6];

    /* Place for received data from camera */
    char cam_buffer [CAM_BUFFER_SIZE];
    int cam_buffer_start;
    int cam_buffer_end;

} CAM_DATA;

// *****************************************************************************
// *****************************************************************************
// Section: Function Prototypes
// *****************************************************************************
// *****************************************************************************

// Configure registers, PPS, and UART
void init();
void init_config();
void init_pps();
void init_uart_cam();

// UART driver functions
void cam_tx_char(char c);
char cam_rx_char();

/* Initialize fields of CAM_DATA structure */
int cam_data_initialize(CAM_DATA* cam_data);

/* Wake up the camera by sending SYNC commands to it until an ACK is received */
int cam_wake(CAM_DATA* cam_data);

int cam_send_reset(CAM_DATA* cam_data);

/* Send one SYNC command to the camera */
int cam_send_sync(CAM_DATA* cam_data);

/* Send an ACK command to the camera with irrelevant args set to 0x00 */
int cam_send_ack(CAM_DATA* cam_data, char command_id, char package_id_byte_0, char package_id_byte_1);

/* Send an INITIAL command to the camera to set it to JPEG and set its resolution */
int cam_send_initial(CAM_DATA* cam_data);

/* Send a SET PACKAGE SIZE command to the camera */
int cam_send_package_size(CAM_DATA* cam_data);

/* Send a GET PICTURE command to receive a DATA packet and multiple image data packages */
int cam_send_get_picture(CAM_DATA* cam_data);

/* Wait until an ACK is received or for a number of tries to allow processing time */
char cam_receive_ack(CAM_DATA* cam_data);

/* Receive a DATA response and return the number of packages to expect */
int cam_receive_data_cmd(CAM_DATA* cam_data);

/* Receive an image data package and put it into the data array */
int cam_receive_package(CAM_DATA* cam_data);

#endif	/* CAMERA_H */

