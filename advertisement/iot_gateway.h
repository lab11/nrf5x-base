#ifndef __IOT_GATEWAY_H
#define __IOT_GATEWAY_H

#include "ble_advdata.h"

// Flags for which sensors the gateway should sample when forwarding data
#define IOT_GATEWAY_SENSORS_TIME          0x80
#define IOT_GATEWAY_SENSORS_GPS           0x40
#define IOT_GATEWAY_SENSORS_ACCELEROMETER 0x20
#define IOT_GATEWAY_SENSORS_AMBIENT_LIGHT 0x10
#define IOT_GATEWAY_SENSORS_TEXT_INPUT    0x08
#define IOT_GATEWAY_SENSORS_CAMERA        0x04
#define IOT_GATEWAY_SENSORS_WEB_UI        0x02
#define IOT_GATEWAY_SENSORS_IP_OVER_BLE   0x01

// defines for the advertising packet format
#define IOT_GATEWAY_PKT_URL_MAX_LEN 14
#define IOT_GATEWAY_PKT_HEADER_LEN 16
#define IOT_GATEWAY_PKT_PAYLOAD_MAX_LEN 10
#define IOT_GATEWAY_PKT_MAX_LEN IOT_GATEWAY_PKT_HEADER_LEN+IOT_GATEWAY_PKT_PAYLOAD_MAX_LEN

// Functions
uint32_t iot_gateway_adv (char* post_url_str,
                          uint8_t incentive_program_level,
                          uint8_t reliability_level,
                          uint8_t sensors,
                          uint8_t* data,
                          uint8_t data_len,
                          const ble_advdata_t* scan_response_data);


#endif
