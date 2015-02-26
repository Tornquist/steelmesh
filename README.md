# Twitch Plays ECE477 -- Collaborative Robot Project

## Robot MCU Configuration
The system clock is set to 200MHz.
The periperhal bus clock #2 (UART) is set to 33.33MHz to minimize baud rate calculation error.

### Robot MCU WiFi (UART1) Configuation
The baud rate is chosen to be 115,200bps. This module is set for no interrupts and uses the Read/Write model.

### Robot MCU Camera (UART2) Configuation
The baud rate is chosen to be 115,200bps. This module is set for no interrupts and uses the Read/Write model.

## Robot MCU 100-pin choices
WiFi uses UART1. U1TX assigned to pin 11 (RG7). U1RX assigned to pin 12 (RG8).
These pins stay the same between the device and the PIM. 

Camera uses UART2. U2TX assigned to pin 18 (RE8). U2RX assigned to pin 19 (RE9).
These pins stay the same between the device and the PIM.