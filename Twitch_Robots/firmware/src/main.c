/*******************************************************************************
  MPLAB Harmony Project Main Source File

  Company:
    Microchip Technology Inc.
  
  File Name:
    main.c

  Summary:
    This file contains the "main" function for an MPLAB Harmony project.

  Description:
    This file contains the "main" function for an MPLAB Harmony project.  The
    "main" function calls the "SYS_Initialize" function to initialize the state 
    machines of all MPLAB Harmony modules in the system and it calls the 
    "SYS_Tasks" function from within a system-wide "super" loop to maintain 
    their correct operation. These two functions are implemented in 
    configuration-specific files (usually "system_init.c" and "system_tasks.c")
    in a configuration-specific folder under the "src/system_config" folder 
    within this project's top-level folder.  An MPLAB Harmony project may have
    more than one configuration, each contained within it's own folder under
    the "system_config" folder.
 *******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c) 2013-2014 released Microchip Technology Inc.  All rights reserved.

//Microchip licenses to you the right to use, modify, copy and distribute
Software only when embedded on a Microchip microcontroller or digital signal
controller that is integrated into your product or third party product
(pursuant to the sublicense terms in the accompanying license agreement).

You should refer to the license agreement accompanying this Software for
additional information regarding your rights and obligations.

SOFTWARE AND DOCUMENTATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF
MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE.
IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER
CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR
OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR
CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT OF
SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
(INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.
 *******************************************************************************/
// DOM-IGNORE-END


// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include <stddef.h>                     // Defines NULL
#include <stdbool.h>                    // Defines true
#include <stdlib.h>                     // Defines EXIT_FAILURE
#include "system/common/sys_module.h"   // SYS function prototypes

#include "app.h"

// *****************************************************************************
// *****************************************************************************
// Section: Main Entry Point
// *****************************************************************************
// *****************************************************************************

int main ( void )
{
    U1MODE = 5;
    int bytesProcessed;
    int count;
    uint8_t cam_cmd_sync [CAM_COMMAND_LENGTH] = {0x00, 0x00, 0x00, 0x00, 0x0D, 0xAA};
    uint8_t cam_rcv [CAM_COMMAND_LENGTH] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    DRV_HANDLE cam_handle;
    
    /* Initialize all MPLAB Harmony modules, including application(s). */
    SYS_Initialize ( NULL );


    while ( true )
    {
        /* Maintain state machines of all polled MPLAB Harmony modules. */
        //SYS_Tasks ( );

        cam_handle = DRV_USART_Open(CAM_USART_ID, DRV_IO_INTENT_READWRITE | DRV_IO_INTENT_NONBLOCKING);

        bytesProcessed = 0;
        count = 0;
        do {
            count = DRV_USART_Write(cam_handle, cam_cmd_sync + bytesProcessed, CAM_COMMAND_LENGTH - bytesProcessed);
            if (count == DRV_USART_WRITE_ERROR) {
                return CAM_FAIL;
            }
            else {
                bytesProcessed += count;
            }
        } while (bytesProcessed < CAM_COMMAND_LENGTH);

        bytesProcessed = 0;
        count = 0;
        do {
            count = DRV_USART_Read(cam_handle, cam_rcv + bytesProcessed, CAM_COMMAND_LENGTH - bytesProcessed);
            if (count == DRV_USART_READ_ERROR) {
                return CAM_FAIL;
            }
            else {
                bytesProcessed += count;
            }
        } while (bytesProcessed < CAM_COMMAND_LENGTH);

        DRV_USART_Close(cam_handle);
    }

    /* Execution should not come here during normal operation */

    return ( EXIT_FAILURE );
}


/*******************************************************************************
 End of File
*/

