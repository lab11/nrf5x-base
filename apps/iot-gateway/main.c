/*
 * Advertise with the IoT-Gateway format.
 *
 * https://github.com/lab11/iot-gateway
 */

// Global libraries
#include <stdint.h>

// Nordic libraries
#include "ble_advdata.h"

// nrf5x-base libraries
#include "simple_ble.h"
#include "iot_gateway.h"

// Define constants about this beacon.
#define DEVICE_NAME "iotgw"
#define POST_URL "goo.gl/aaaaaa"

// Intervals for advertising and connections
static simple_ble_config_t ble_config = {
    .platform_id       = 0x4D,              // used as 4th octect in device BLE address
    .device_id         = DEVICE_ID_DEFAULT,
    .adv_name          = DEVICE_NAME,       // used in advertisements if there is room
    .adv_interval      = MSEC_TO_UNITS(500, UNIT_0_625_MS),
    .min_conn_interval = MSEC_TO_UNITS(500, UNIT_1_25_MS),
    .max_conn_interval = MSEC_TO_UNITS(1000, UNIT_1_25_MS)
};

// main is essentially two library calls to setup all of the Nordic SDK
// API calls.
int main(void) {

    uint8_t adv_data[4] = {0xBA, 0xCE, 0xD0, 0x11};

    // Setup BLE
    simple_ble_init(&ble_config);

    // Advertise a URL
    iot_gateway_adv(POST_URL,
                    7, // Incentive level (default to middle)
                    8, // Reliability (just picked 8)
                    IOT_GATEWAY_SENSORS_TIME | IOT_GATEWAY_SENSORS_AMBIENT_LIGHT, // Request time and ambient light from the gateway
                    adv_data, // App specific data
                    4, // Data length
                    NULL // No scan response
                    );

    while (1) {
        power_manage();
    }
}
