/*
 * Send an advertisement periodically
 */

#include <stdbool.h>
#include <stdint.h>
#include "nrf_gpio.h"
#include "ble_advdata.h"
#include "nordic_common.h"
#include "led.h"
#include "sdk_config.h"

#include "simple_ble.h"
#include "simple_adv.h"

// Custom UTF-8 name. Flash this app and scan to see it!
char name[20] = {0xE2, 0x9D, 0x84, 0xE2, 0x9D,
                 0x85, 0xE2, 0x9D, 0x86, 0xE2,
                 0x9C, 0x8B, 0xE2, 0x87, 0x92,
                 0xF0, 0x9F, 0x8D, 0xBB, 0x00};


// Intervals for advertising and connections
static simple_ble_config_t ble_config = {
    .platform_id       = 0x00,              // used as 4th octect in device BLE address
    .device_id         = DEVICE_ID_DEFAULT,
    .adv_name          = name,
    .adv_interval      = MSEC_TO_UNITS(500, UNIT_0_625_MS),
    .min_conn_interval = MSEC_TO_UNITS(500, UNIT_1_25_MS),
    .max_conn_interval = MSEC_TO_UNITS(1000, UNIT_1_25_MS)
};


int main(void) {
    uint32_t err_code;

    // Setup BLE
    simple_ble_init(&ble_config);

    // Advertise because why not
    simple_adv_only_name();

    while (1) {
        power_manage();
    }
}
