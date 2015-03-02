/* 
 * File:   camera.h
 * Author: TJ
 *
 * Created on February 26, 2015, 4:32 PM
 */

#ifndef CAMERA_H
#define	CAMERA_H

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

#define CAM_COMMAND_LENGTH 6
#define CAM_DATA_ARRAY_SIZE 512 * 10
#define CAM_USART_ID USART_ID_2
#define CAM_SUCCESS true
#define CAM_FAIL false

#define CAM_MAX_SYNC_ATTEMPTS 50
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


// *****************************************************************************
// *****************************************************************************
// Section: Type Definitions
// *****************************************************************************
// *****************************************************************************

typedef struct {
    /* USART handle for camera module */
    DRV_HANDLE cam_handle;
    
    /* Image data store from camera */
    uint8_t cam_data_array[CAM_DATA_ARRAY_SIZE];

    /* Number of packages required to send complete image */
    int cam_data_package_number;
    
    /* Current position in image array when sending */
    int cam_data_send_index;

    /* Current position in image array when receiving */
    int cam_data_receive_index;

    /* Tracks image package id to use in ACK response */
    int cam_data_package_counter;

    /* Command response received from the camera */
    uint8_t cam_received_command [CAM_COMMAND_LENGTH];
    
} CAM_DATA;

// *****************************************************************************
// *****************************************************************************
// Section: Function Prototypes
// *****************************************************************************
// *****************************************************************************

/* Initialize fields of CAM_DATA structure */
bool cam_data_initialize(CAM_DATA* cam_data);

/* Wake up the camera by sending SYNC commands to it until an ACK is received */
bool cam_wake(CAM_DATA* cam_data);

/* Send one SYNC command to the camera */
bool cam_send_sync(CAM_DATA* cam_data);

/* Send an ACK command to the camera with irrelevant args set to 0x00 */
bool cam_send_ack(CAM_DATA* cam_data, uint8_t command_id, uint8_t package_id_byte_0, uint8_t package_id_byte_1);

/* Send an INITIAL command to the camera to set it to JPEG and set its resolution */
bool cam_send_initial(CAM_DATA* cam_data);

/* Send a SET PACKAGE SIZE command to the camera */
bool cam_send_package_size(CAM_DATA* cam_data);

/* Send a GET PICTURE command to receive a DATA packet and multiple image data packages */
bool cam_send_get_picture(CAM_DATA* cam_data);

/* Wait until an ACK is received or for a number of tries to allow processing time */
uint8_t cam_receive_ack(CAM_DATA* cam_data);

/* Receive a DATA response and return the number of packages to expect */
bool cam_receive_data_cmd(CAM_DATA* cam_data);

/* Receive an image data package and put it into the data array */
bool cam_receive_package(CAM_DATA* cam_data);

#endif	/* CAMERA_H */

