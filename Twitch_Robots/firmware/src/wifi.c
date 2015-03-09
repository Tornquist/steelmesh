/*
 * File:   wifi.h
 * Author: Josh Hannan
 *
 * Created on February 28, 2015.
 */

#include "app.h"

/* Initialize fields of WIFI_DATA structure */
bool wifi_data_initialize(WIFI_DATA* wifi_data) {
    int i;

    wifi_data->wifi_handle = DRV_USART_Open(WIFI_USART_ID, DRV_IO_INTENT_READWRITE | DRV_IO_INTENT_NONBLOCKING);
    if (wifi_data->wifi_handle == DRV_HANDLE_INVALID) {
        return WIFI_FAIL;
    }

    for (i = 0; i < WIFI_COMMAND_LENGTH; i++) {
        wifi_data->wifi_received_command[i] = 0;
    }

    for (i = 0; i < 3; i++) {
        wifi_data->wifi_data_array[i] = 0;
    }

    
    return WIFI_SUCCESS;
}

// enter command mode in the wifi module
bool wifi_enter_cmd_mode(WIFI_DATA* wifi_data) {
    unsigned int total;
    unsigned int count;
    char wifi_cmd_start [WIFI_CMD_ENTER_LENGTH] = { '$', '$', '$'};

    total = 0;
    do {
        count = DRV_USART_Write(wifi_data->wifi_handle, &wifi_cmd_start[total], WIFI_CMD_ENTER_LENGTH - total);
        if(total == DRV_USART_WRITE_ERROR) {
            wifi_data->test = 0;
        }
        total += count;

    } while (total < WIFI_CMD_ENTER_LENGTH);

    return WIFI_SUCCESS;
}

// function to send any command over UART  i want to be able to pass this a command
/*bool wifi_send_cmd(WIFI_DATA *wifi_data) {
    int bytesProcessed;
    uint8_t ENTER_CMD_MODE [3] = {0x24, 0x24, 0x24};
    uint8_t SET_UART_BAUD [19] = {0x73, 0x65, 0x74, 0x20, 0x75, 0x61, 0x72, 0x74, 0x20, 0x62, 0x61, 0x75, 0x64, 0x20, 0x39, 0x36, 0x30, 0x30, 0x0D};

    bytesProcessed = 0;
    do {
        bytesProcessed += DRV_USART_Write(wifi_data->wifi_handle, SET_UART_BAUD + bytesProcessed, 19 - bytesProcessed);
    } while (bytesProcessed < WIFI_CMD_ENTER_LENGTH);

    return WIFI_SUCCESS;
}*/

// receive a response from the wifi module
bool wifi_receive_response(WIFI_DATA* wifi_data) {
    unsigned int count = 0;
    unsigned int total;
    volatile int nothing;
    char myBuffer[3] = {0x00, 0x00, 0x00};
    //uint8_t CMD_RESPONSE [3] = {0x43, 0x4D, 0x44};
    //uint8_t wifi_cmd_start [WIFI_CMD_ENTER_LENGTH] = {0x24, 0x24, 0x24};

    total = 0;
    do
    {
        count  = DRV_USART_Read(wifi_data->wifi_handle, &myBuffer[total], WIFI_CMD_ENTER_LENGTH-total);
        if(count == DRV_USART_READ_ERROR)
        {
            return WIFI_FAIL;
            // There was an error. The DRV_USART_ErrorGet() function
           // can be called to find the exact error.
        }
        total += count;

    } while( total < WIFI_CMD_ENTER_LENGTH );

    wifi_data->wifi_data_array[0] = myBuffer[0];
    wifi_data->wifi_data_array[1] = myBuffer[1];
    wifi_data->wifi_data_array[2] = myBuffer[2];

    //if(wifi_data->wifi_data_array == wifi_cmd_start) {
            //if(wifi_data->wifi_data_array[1] == 0x4D) {
            //    if(wifi_data->wifi_data_array[2] == 0x44) {
        //printf("YAY");
        //return WIFI_SUCCESS;
                    //appData.state = STATE_WIFI_INIT;
                    //sync_command_signal = false;
            //    }
            //}
            //wifi_data->wifi_received_command[0] = 0x43;
    //}

/*
        if((wifi_data->wifi_data_array[0] == 0x41) | (wifi_data->wifi_data_array[0] == 0x45)) {
            wifi_data->wifi_received_command[0] = 0x41;
            //if(wifi_data->wifi_data_array[1] == 0x4F) {
            //    if(wifi_data->wifi_data_array[2] == 0x4B) {
            //        nothing = 1;
            //        return WIFI_SUCCESS;
                    //appData.state = STATE_WIFI_INIT;
                    //sync_command_signal = false;
            //    }
            //}
        }

         // Do something else...
*/
    return WIFI_FAIL;
}

