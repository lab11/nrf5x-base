#ifndef DFU_H
#define DFU_H

//#ifndef DEVICE_NAME
//  #define DEVICE_NAME "DFUTarg"
//#endif /*DEVICE_NAME*/

#include <stdint.h>

extern uint8_t MAC_ADDR[6];
#define PLATFORM_ID_BYTE 0xDF

// LED
#define ERROR_LED 13
#define BSP_LED_0 ERROR_LED 
#define BSP_LED_1 ERROR_LED 

#endif /*DFU_H*/
