/*#include "app.h"*/

/* Initialize fields of CAM_DATA structure */
/*
bool cam_data_initialize(CAM_DATA* cam_data) {
    int i;

    cam_data->cam_handle = DRV_USART_Open(CAM_USART_ID, DRV_IO_INTENT_READWRITE | DRV_IO_INTENT_NONBLOCKING);
    if (cam_data->cam_handle == DRV_HANDLE_INVALID) {
        return CAM_FAIL;
    }
    cam_data->cam_data_send_index = 0;
    cam_data->cam_data_receive_index = 0;
    cam_data->cam_data_package_counter = 0;
    for (i = 0; i < CAM_COMMAND_LENGTH; i++) {
        cam_data->cam_received_command[i] = 0;
    }
    for (i = 0; i < CAM_DATA_ARRAY_SIZE; i++) {
        cam_data->cam_data_array[i] = 0;
    }
    return CAM_SUCCESS;
}

// Wake up the camera by sending SYNC commands to it until an ACK is received
bool cam_wake(CAM_DATA* cam_data) {
    int i;
    int bytesProcessed;
    uint8_t cam_cmd_sync [CAM_COMMAND_LENGTH] = {0x00, 0x00, 0x00, 0x00, 0x0D, 0xAA};

    // send SYNC commands until wake
    for (i = 0; i < CAM_MAX_SYNC_ATTEMPTS; i++) {
        cam_send_sync(cam_data);
    }

    // check if ACK received
    bytesProcessed = 0;
    bytesProcessed += DRV_USART_Read(cam_data->cam_handle, cam_data->cam_received_command + bytesProcessed, CAM_COMMAND_LENGTH - bytesProcessed);

    // if no response, fail
    if (bytesProcessed == 0) {
        return CAM_FAIL;
    }

    // receive SYNC packet on success
    bytesProcessed = 0;
    do {
        bytesProcessed += DRV_USART_Read(cam_data->cam_handle, cam_data->cam_received_command + bytesProcessed, CAM_COMMAND_LENGTH - bytesProcessed);
    } while (bytesProcessed < CAM_COMMAND_LENGTH);

    // send ACK command to verify
    cam_send_ack(cam_data, CAM_CMD_ID_SYNC, 0x00, 0x00);

    return CAM_SUCCESS;
}

// send a SYNC command to the camera
bool cam_send_sync(CAM_DATA* cam_data) {
    int bytesProcessed;
    uint8_t cam_cmd_sync [CAM_COMMAND_LENGTH] = {0x00, 0x00, 0x00, 0x00, 0x0D, 0xAA};

    bytesProcessed = 0;
    do {
        bytesProcessed += DRV_USART_Write(cam_data->cam_handle, cam_cmd_sync + bytesProcessed, CAM_COMMAND_LENGTH - bytesProcessed);
    } while (bytesProcessed < CAM_COMMAND_LENGTH);

    return CAM_SUCCESS;
}

// send an ACK command to the camera
bool cam_send_ack(CAM_DATA* cam_data, uint8_t command_id, uint8_t package_id_byte_0, uint8_t package_id_byte_1) {
    int bytesProcessed = 0;
    uint8_t cam_cmd_ack [CAM_COMMAND_LENGTH] = {package_id_byte_1, package_id_byte_0, 0x00, command_id, 0x0E, 0xAA};

    bytesProcessed = 0;
    do {
        bytesProcessed += DRV_USART_Write(cam_data->cam_handle, cam_cmd_ack + bytesProcessed, CAM_COMMAND_LENGTH - bytesProcessed);
    } while (bytesProcessed < CAM_COMMAND_LENGTH);

    return CAM_SUCCESS;
}*/