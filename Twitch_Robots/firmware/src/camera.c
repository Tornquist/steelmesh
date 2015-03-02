#include "app.h"

/* Initialize fields of CAM_DATA structure */
bool cam_data_initialize(CAM_DATA* cam_data) {
    int i;

    ANSELECLR = 0xFFFF;
    ANSELGCLR = 0xFFFF;
    TRISGbits.TRISG7 = 0;
    TRISEbits.TRISE8 = 0;
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

/* Wake up the camera by sending SYNC commands to it until an ACK is received */
bool cam_wake(CAM_DATA* cam_data) {
    int i;
    int bytesProcessed;
    uint8_t cam_cmd_sync [CAM_COMMAND_LENGTH] = {0x00, 0x00, 0x00, 0x00, 0x0D, 0xAA};

    // send SYNC commands until wake
    for (i = 0; i < CAM_MAX_SYNC_ATTEMPTS; i++) {
        cam_send_sync(cam_data);
    }

    // check if ACK received
    if (cam_receive_ack(cam_data) != CAM_CMD_ID_SYNC) {
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

    while (true) {
    bytesProcessed = 0;
    do {
        bytesProcessed += DRV_USART_Write(cam_data->cam_handle, cam_cmd_sync + bytesProcessed, CAM_COMMAND_LENGTH - bytesProcessed);
    } while (bytesProcessed < CAM_COMMAND_LENGTH);
    }
    return CAM_SUCCESS;
}

// send an ACK command to the camera
bool cam_send_ack(CAM_DATA* cam_data, uint8_t command_id, uint8_t package_id_byte_0, uint8_t package_id_byte_1) {
    int bytesProcessed = 0;
    uint8_t cam_cmd_ack [CAM_COMMAND_LENGTH] = {package_id_byte_1, package_id_byte_0, 0x00, command_id, CAM_CMD_ID_ACK, 0xAA};

    bytesProcessed = 0;
    do {
        bytesProcessed += DRV_USART_Write(cam_data->cam_handle, cam_cmd_ack + bytesProcessed, CAM_COMMAND_LENGTH - bytesProcessed);
    } while (bytesProcessed < CAM_COMMAND_LENGTH);

    return CAM_SUCCESS;
}

/* Send an INITIAL command to the camera to set it to JPEG and set its resolution */
bool cam_send_initial(CAM_DATA* cam_data) {
    int bytesProcessed = 0;
    uint8_t cam_cmd_initial [CAM_COMMAND_LENGTH] = {CAM_RESOLUTION_160x128, 0x00, CAM_INITIAL_JPEG, 0x00, CAM_CMD_ID_INITIAL, 0xAA};

    // send INITIAL
    bytesProcessed = 0;
    do {
        bytesProcessed += DRV_USART_Write(cam_data->cam_handle, cam_cmd_initial + bytesProcessed, CAM_COMMAND_LENGTH - bytesProcessed);
    } while (bytesProcessed < CAM_COMMAND_LENGTH);

    // check receipt by camera with ACK response
    if (cam_receive_ack(cam_data) != CAM_CMD_ID_INITIAL) {
        return CAM_FAIL;
    } else {
        return CAM_SUCCESS;
    }
}

/* Send a SET PACKAGE SIZE command to the camera */
bool cam_send_package_size(CAM_DATA* cam_data) {
    int bytesProcessed = 0;
    uint8_t cam_cmd_package_size [CAM_COMMAND_LENGTH] = {0x00, CAM_SIZE_512_HIGH_BYTE, CAM_SIZE_512_LOW_BYTE, 0x08, CAM_CMD_ID_SET_PACKAGE_SIZE, 0xAA};

    // send command
    bytesProcessed = 0;
    do {
        bytesProcessed += DRV_USART_Write(cam_data->cam_handle, cam_cmd_package_size + bytesProcessed, CAM_COMMAND_LENGTH - bytesProcessed);
    } while (bytesProcessed < CAM_COMMAND_LENGTH);

    // check receipt by camera with ACK response
    if (cam_receive_ack(cam_data) != CAM_CMD_ID_SET_PACKAGE_SIZE) {
        return CAM_FAIL;
    } else {
        return CAM_SUCCESS;
    }
}

/* Send a GET PICTURE command to receive a DATA packet and multiple image data packages */
bool cam_send_get_picture(CAM_DATA* cam_data) {
    int bytesProcessed = 0;
    uint8_t cam_cmd_get_picture [CAM_COMMAND_LENGTH] = {0x00, 0x00, 0x00, CAM_GET_PICTURE_JPEG, CAM_CMD_ID_GET_PICTURE, 0xAA};

    // send command
    bytesProcessed = 0;
    do {
        bytesProcessed += DRV_USART_Write(cam_data->cam_handle, cam_cmd_get_picture + bytesProcessed, CAM_COMMAND_LENGTH - bytesProcessed);
    } while (bytesProcessed < CAM_COMMAND_LENGTH);

    // check receipt by camera with ACK response
    if (cam_receive_ack(cam_data) != CAM_CMD_ID_GET_PICTURE) {
        return CAM_FAIL;
    }

    // receive DATA response and get number of packages into cam_data
    if (cam_receive_data_cmd(cam_data) != CAM_CMD_ID_DATA) {
        return CAM_FAIL;
    }

    // send an ACK with the package number, then receive the package
    // when all packages have been received send an ACK with {F0, F0} as package ID
    for (cam_data->cam_data_package_counter = 0;
            cam_data->cam_data_package_counter < cam_data->cam_data_package_number; cam_data->cam_data_package_counter++) {
        cam_send_ack(cam_data, CAM_CMD_ID_ACK, (uint8_t)cam_data->cam_data_package_counter, (uint8_t)cam_data->cam_data_package_counter >> 8);
        if (cam_receive_package(cam_data) != CAM_SUCCESS) {
            return CAM_FAIL;
        }
    }

    // when all packages have been received send an ACK with {F0, F0} as package ID
    cam_send_ack(cam_data, CAM_CMD_ID_ACK, CAM_PACKAGE_FINAL, CAM_PACKAGE_FINAL);
    
    return CAM_SUCCESS;
}

/* Wait until an ACK is received or for a number of tries to allow processing time */
uint8_t cam_receive_ack(CAM_DATA* cam_data) {
    int bytesProcessed;
    int attempts;

    // check if ACK received
    bytesProcessed = 0;
    attempts = 0;
    do {
        bytesProcessed += DRV_USART_Read(cam_data->cam_handle, cam_data->cam_received_command + bytesProcessed, CAM_COMMAND_LENGTH - bytesProcessed);
        attempts++;
        if (attempts > CAM_MAX_ACK_RECEIVE_ATTEMPTS) {
            return CAM_ACK_FAILED;
        }
    } while (bytesProcessed < CAM_COMMAND_LENGTH);

    // return command ID
    return cam_data->cam_received_command[2];
}

/* Receive a DATA response and return the number of packages to expect */
bool cam_receive_data_cmd(CAM_DATA* cam_data) {
    int bytesProcessed;

    // check if ACK received
    bytesProcessed = 0;
    do {
        bytesProcessed += DRV_USART_Read(cam_data->cam_handle, cam_data->cam_received_command + bytesProcessed, CAM_COMMAND_LENGTH - bytesProcessed);
    } while (bytesProcessed < CAM_COMMAND_LENGTH);

    if (cam_data->cam_received_command[1] != CAM_CMD_ID_DATA) {
        return CAM_FAIL;
    }

    // package size bytes into an integer
    cam_data->cam_data_package_number = 0;
    cam_data->cam_data_package_number |= (int)cam_data->cam_received_command[4];
    cam_data->cam_data_package_number |= (int)cam_data->cam_received_command[5] << 8;
    cam_data->cam_data_package_number |= (int)cam_data->cam_received_command[6] << 16;

    return CAM_SUCCESS;
}

// send get picture, get ack, get data, send ack with 0 package id to get package 0
// send ack with next package id to get the next package
/* Receive an image data package and put it into the data array */
bool cam_receive_package(CAM_DATA* cam_data) {
    // ID: bytes 0 to 1
    // Data size: bytes 5 to (package size - 6)
    // Verify code: bytes N-1 to N
    int bytesProcessed;
    int package_id;
    int data_size;
    int verify_code;

    // receive ID bytes
    bytesProcessed = 0;
    do {
        bytesProcessed += DRV_USART_Read(cam_data->cam_handle, cam_data->cam_received_command + bytesProcessed, CAM_PACKAGE_ID_SIZE - bytesProcessed);
    } while (bytesProcessed < CAM_PACKAGE_ID_SIZE);

    // assemble package id
    package_id = 0;
    package_id |= (int)cam_data->cam_received_command[0];
    package_id |= (int)cam_data->cam_received_command[1] << 8;

    if (package_id != cam_data->cam_data_package_counter) {
        return CAM_FAIL;
    }

    // receive data size bytes
    bytesProcessed = 0;
    do {
        bytesProcessed += DRV_USART_Read(cam_data->cam_handle, cam_data->cam_received_command + bytesProcessed, CAM_PACKAGE_DATA_SIZE - bytesProcessed);
    } while (bytesProcessed < CAM_PACKAGE_DATA_SIZE);

    // assemble data size
    data_size = 0;
    data_size |= (int)cam_data->cam_received_command[0];
    data_size |= (int)cam_data->cam_received_command[1] << 8;

    // receive all image data bytes into array
    bytesProcessed = 0;
    do {
        bytesProcessed += DRV_USART_Read(cam_data->cam_handle, cam_data->cam_data_array + bytesProcessed, data_size - bytesProcessed);
    } while (bytesProcessed < data_size);

    // update received pointer
    cam_data->cam_data_receive_index += bytesProcessed;

    // receive verify code (add checks later)
    bytesProcessed = 0;
    do {
        bytesProcessed += DRV_USART_Read(cam_data->cam_handle, cam_data->cam_received_command + bytesProcessed, CAM_PACKAGE_VERIFY_SIZE - bytesProcessed);
    } while (bytesProcessed < CAM_PACKAGE_VERIFY_SIZE);

    // assemble verify code
    verify_code = 0;
    verify_code |= (int)cam_data->cam_received_command[0];
    verify_code |= (int)cam_data->cam_received_command[1] << 8;

    return CAM_SUCCESS;
}