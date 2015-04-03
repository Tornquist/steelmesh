#include "../include/camera.h"

void init() {
    init_config();
    init_pps();
    init_uart_cam();
}
void init_config() {
    // disable watchdog and deadman timers
    WDTCONbits.ON = 0;
    DMTCONbits.ON = 0;

    // disable jtag
    CFGCONbits.JTAGEN = 0;

    // set all ports as outputs
    TRISACLR = 0xFFFF;
    TRISBCLR = 0xFFFF;
    TRISECLR = 0xFFFF;
    TRISGCLR = 0xFFFF;

    TRISEbits.TRISE8 = 0; // U2TX
    TRISEbits.TRISE9 = 1; // U2RX
    TRISGbits.TRISG7 = 0; // U1TX
    TRISGbits.TRISG8 = 1; // U1RX

    // set all ports as digital
    ANSELACLR = 0xFFFF;
    ANSELBCLR = 0xFFFF;
    ANSELECLR = 0xFFFF;
    ANSELGCLR = 0xFFFF;

    // initialize UART2 receive interrupt
    INTCONbits.MVEC = 1; // enable multi-vectored interrupt operation
    IEC4bits.U2RXIE = 0;     // disable interrupt
    IPC36bits.U2RXIP = 7;   // set priority
    IPC36bits.U2RXIS = 0;   // set subpriority
    IFS4bits.U2RXIF = 0;    // clear interrupt flag
    IEC4bits.U2RXIE = 1;    // enable interrupt
    //IEC4SET = _IEC4_U2RXIE_MASK; // enable interrupt
    
}
void init_pps() {
    // setup PPS
    SYSKEY = 0x0;
    SYSKEY = 0xAA996655;
    SYSKEY = 0x556699AA;

    CFGCONbits.IOLOCK = 0;    // unlocks config
    U1RXRbits.U1RXR = 0b0001; // Set U1RX to RPG8
    RPG7Rbits.RPG7R = 0b0001; // Set U1TX to RPG7
    U2RXRbits.U2RXR = 0b1101; // set U2RX to RPE9
    RPE8Rbits.RPE8R = 0b0010; // set U2TX to RPE8
    ANSELGbits.ANSG7 = 0;     // sets G7 to digital
    ANSELGbits.ANSG8 = 0;     // sets G8 to digital
    ANSELEbits.ANSE8 = 0;
    ANSELEbits.ANSE9 = 0;
    CFGCONbits.IOLOCK = 1;    // locks config

    SYSKEY = 0x0; // lock PPS
}
void init_uart_cam() {
    // setup UART 2
    U2BRG = FPB/(4.0*BAUDRATE)-1;  // maybe
    U2MODEbits.BRGH = 1;           // uses eq 2 for BR
    U2MODEbits.PDSEL0 = 0;         // 8 bits, no parity
    U2MODEbits.PDSEL1 = 0;
    U2MODEbits.STSEL = 0;          // 1 stop bit

    U2MODEbits.SIDL = 0;
    U2MODEbits.IREN = 0;
    U2MODEbits.RTSMD = 1;

    U2STAbits.URXISEL = 0b00; // set interrupt mode

    U2STAbits.URXEN = 1;           // enable RX
    U2STAbits.UTXEN = 1;           // enable TX

    U2MODEbits.ON = 1;             // enables uart module
}
void cam_tx_char(char c) {
    while (U2STAbits.UTXBF);
    U2TXREG = c;
}
char cam_rx_char() {
    char c;
    int i = 0;
    // wait for recieve buffer to be full
    while(U2STAbits.URXDA == 0) {
        i=i+1;
        // check for receive buffer overflow
        
        // if it has been waiting a really long time, exit
        if(i == 1000000) {
            return 0x00;
        }

        if(U2STAbits.OERR == 1) {
            // clear the overflow flag and the buffer
            U2STAbits.OERR = 0;
        }
    }
    // read the receive register
    c = U2RXREG;

    
    return c;
}

/* Initialize fields of CAM_DATA structure */
int cam_data_initialize(CAM_DATA* cam_data) {
    int i;
    cam_data->cam_data_send_index = 0;
    cam_data->cam_data_receive_index = 0;
    cam_data->cam_data_package_counter = 0;
    for (i = 0; i < CAM_COMMAND_LENGTH; i++) {
        cam_data->cam_received_command[i] = 0;
    }
    for (i = 0; i < CAM_DATA_ARRAY_SIZE; i++) {
        cam_data->cam_data_array[i] = 0;
    }
    for (i = 0; i < CAM_BUFFER_SIZE; i++) {
        cam_data->cam_buffer[i] = 0;
    }
    cam_data->cam_buffer_start = 0;
    cam_data->cam_buffer_end = 0;

    return CAM_SUCCESS;
}

/* Wake up the camera by sending SYNC commands to it until an ACK is received */
int cam_wake(CAM_DATA* cam_data) {
    int i;
    int j;
    char c;

    // send SYNC commands until wake
    for (i = 0; i < CAM_MAX_SYNC_ATTEMPTS; i++) {
        cam_send_sync(cam_data);
    }

    // check if ACK received
    if (cam_receive_ack(cam_data) != CAM_CMD_ID_SYNC) {
        return CAM_FAIL;
    }

    // receive SYNC packet on success
    for (i = 0; i < CAM_COMMAND_LENGTH; i++) {
        cam_data->cam_received_command[i] = cam_rx_char();
    }

    // send ACK command to verify
    cam_send_ack(cam_data, CAM_CMD_ID_SYNC, 0x00, 0x00);

    return CAM_SUCCESS;
}

int cam_send_reset(CAM_DATA* cam_data) {
    int i;
    char cam_cmd_reset [CAM_COMMAND_LENGTH] = {0xAA, 0x08, 0x00, 0x00, 0x00, 0xFF};

    for (i = 0; i < CAM_COMMAND_LENGTH; i++) {
        cam_tx_char(cam_cmd_reset[i]);
    }

    return CAM_SUCCESS;
}

// send a SYNC command to the camera
int cam_send_sync(CAM_DATA* cam_data) {
    int i;
    char cam_cmd_sync [CAM_COMMAND_LENGTH] = {0xAA, 0x0D, 0x00, 0x00, 0x00, 0x00};

    for (i = 0; i < CAM_COMMAND_LENGTH; i++) {
        cam_tx_char(cam_cmd_sync[i]);
    }

    return CAM_SUCCESS;
}

// send an ACK command to the camera
int cam_send_ack(CAM_DATA* cam_data, char command_id, char package_id_byte_0, char package_id_byte_1) {
    int i;
    char cam_cmd_ack [CAM_COMMAND_LENGTH] = {0xAA, CAM_CMD_ID_ACK, command_id, 0x00, package_id_byte_0, package_id_byte_1};

    for (i = 0; i < CAM_COMMAND_LENGTH; i++) {
        cam_tx_char(cam_cmd_ack[i]);
    }

    return CAM_SUCCESS;
}

/* Send an INITIAL command to the camera to set it to JPEG and set its resolution */
int cam_send_initial(CAM_DATA* cam_data) {
    int i;
    char cam_cmd_initial [CAM_COMMAND_LENGTH] = {0xAA, CAM_CMD_ID_INITIAL, 0x00, CAM_INITIAL_JPEG, 0x00, CAM_RESOLUTION_160x128};

    for (i = 0; i < CAM_COMMAND_LENGTH; i++) {
        cam_tx_char(cam_cmd_initial[i]);
    }

    // check receipt by camera with ACK response
    if (cam_receive_ack(cam_data) != CAM_CMD_ID_INITIAL) {
        return CAM_FAIL;
    } else {
        return CAM_SUCCESS;
    }
}

/* Send a SET PACKAGE SIZE command to the camera */
int cam_send_package_size(CAM_DATA* cam_data) {
    int i;
    char cam_cmd_package_size [CAM_COMMAND_LENGTH] = {0xAA, CAM_CMD_ID_SET_PACKAGE_SIZE, 0x08, CAM_SIZE_512_LOW_BYTE, CAM_SIZE_512_HIGH_BYTE, 0x00};

    for (i = 0; i < CAM_COMMAND_LENGTH; i++) {
        cam_tx_char(cam_cmd_package_size[i]);
    }

    // check receipt by camera with ACK response
    if (cam_receive_ack(cam_data) != CAM_CMD_ID_SET_PACKAGE_SIZE) {
        return CAM_FAIL;
    } else {
        return CAM_SUCCESS;
    }
}

/* Send a GET PICTURE command to receive a DATA packet and multiple image data packages */
int cam_send_get_picture(CAM_DATA* cam_data) {
    int i;
    char cam_cmd_get_picture [CAM_COMMAND_LENGTH] = {0xAA, CAM_CMD_ID_GET_PICTURE, CAM_GET_PICTURE_JPEG, 0x00, 0x00, 0x00};

    for (i = 0; i < CAM_COMMAND_LENGTH; i++) {
        cam_tx_char(cam_cmd_get_picture[i]);
    }

    /*// check receipt by camera with ACK response
    if (cam_receive_ack(cam_data) != CAM_CMD_ID_GET_PICTURE) {
        return CAM_FAIL;
    }

    for (i = 0; i < 10000000; i++) {
        
    }*/

    // receive DATA response and get number of packages into cam_data
    if (cam_receive_data_cmd(cam_data) != CAM_SUCCESS) {
        return CAM_FAIL;
    }

    // send an ACK with the package number, then receive the package
    // when all packages have been received send an ACK with {F0, F0} as package ID
    for (cam_data->cam_data_package_counter = 0;
            cam_data->cam_data_package_counter < cam_data->cam_data_package_number; cam_data->cam_data_package_counter++) {
        cam_send_ack(cam_data, CAM_CMD_ID_ACK, (char)cam_data->cam_data_package_counter, (char)cam_data->cam_data_package_counter >> 8);
        if (cam_receive_package(cam_data) != CAM_SUCCESS) {
            return CAM_FAIL;
        }
    }

    // when all packages have been received send an ACK with {F0, F0} as package ID
    cam_send_ack(cam_data, CAM_CMD_ID_ACK, CAM_PACKAGE_FINAL, CAM_PACKAGE_FINAL);

    return CAM_SUCCESS;
}

/* Wait until an ACK is received or for a number of tries to allow processing time */
char cam_receive_ack(CAM_DATA* cam_data) {
    int i;

    if (!(cam_data->cam_buffer_start + 6 == cam_data->cam_buffer_end)) {
        return 0;
    }

    for (i = 0; i < CAM_COMMAND_LENGTH; i++) {
        cam_data->cam_received_command[i] = cam_data->cam_buffer[cam_data->cam_buffer_start + i];
        cam_data->cam_buffer_start = (cam_data->cam_buffer_start + 1) % CAM_BUFFER_SIZE;
    }

    // return command ID
    return cam_data->cam_received_command[2];
}

/* Receive a DATA response and return the number of packages to expect */
int cam_receive_data_cmd(CAM_DATA* cam_data) {
    int i;

    // check if ACK received
    for (i = 0; i < CAM_COMMAND_LENGTH; i++) {
        cam_data->cam_received_command[i] = cam_rx_char();
    }
    if (cam_data->cam_received_command[1] != CAM_CMD_ID_GET_PICTURE) {
        return CAM_FAIL;
    }

    // receive data package
    for (i = 0; i < CAM_COMMAND_LENGTH; i++) {
        cam_data->cam_received_command[i] = cam_rx_char();
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
int cam_receive_package(CAM_DATA* cam_data) {
    // ID: bytes 0 to 1
    // Data size: bytes 5 to (package size - 6)
    // Verify code: bytes N-1 to N
    int package_id;
    int data_size;
    int verify_code;
    int i;

    // receive ID bytes
    for (i = 0; i < CAM_PACKAGE_ID_SIZE; i++) {
        cam_data->cam_received_command[i] = cam_rx_char();
    }

    // assemble package id
    package_id = 0;
    package_id |= (int)cam_data->cam_received_command[0];
    package_id |= (int)cam_data->cam_received_command[1] << 8;

    if (package_id != cam_data->cam_data_package_counter) {
        return CAM_FAIL;
    }

    // receive data size bytes
    for (i = 0; i < CAM_PACKAGE_DATA_SIZE; i++) {
        cam_data->cam_received_command[i] = cam_rx_char();
    }

    // assemble data size
    data_size = 0;
    data_size |= (int)cam_data->cam_received_command[0];
    data_size |= (int)cam_data->cam_received_command[1] << 8;

    // receive all image data bytes into array
    for (i = cam_data->cam_data_receive_index; i < data_size; i++) {
        cam_data->cam_data_array[i] = cam_rx_char();
    }

    // update received pointer
    cam_data->cam_data_receive_index += data_size;

    // receive verify code (add checks later)
    for (i = 0; i < CAM_PACKAGE_VERIFY_SIZE; i++) {
        cam_data->cam_received_command[i] = cam_rx_char();
    }

    // assemble verify code
    verify_code = 0;
    verify_code |= (int)cam_data->cam_received_command[0];
    verify_code |= (int)cam_data->cam_received_command[1] << 8;

    return CAM_SUCCESS;
}

