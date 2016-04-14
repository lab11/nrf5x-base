/*
 * Send an advertisement periodically while scanning for advertisements.
 */

#include <stdbool.h>
#include <stdint.h>
#include "nrf_gpio.h"
#include "ble_advdata.h"
#include "nordic_common.h"
#include "softdevice_handler.h"
#include "ble_debug_assert_handler.h"
#include "led.h"

#include "simple_ble.h"
#include "simple_adv.h"


#define LED0 13

// Intervals for advertising and connections
static simple_ble_config_t ble_config = {
    .platform_id       = 0xd9,              // used as 4th octect in device BLE address
    .device_id         = DEVICE_ID_DEFAULT,
    .adv_name          = "ScanSPAM",
    .adv_interval      = MSEC_TO_UNITS(500, UNIT_0_625_MS),
    .min_conn_interval = MSEC_TO_UNITS(500, UNIT_1_25_MS),
    .max_conn_interval = MSEC_TO_UNITS(1000, UNIT_1_25_MS)
};


void ble_error(uint32_t error_code) {
    led_on(LED0);
}

void ble_evt_adv_report (ble_evt_t* p_ble_evt) {
    led_toggle(LED0);
}

int main(void) {
    led_init(LED0);
    led_off(LED0);

    // Setup BLE
    simple_ble_init(&ble_config);

    // Advertise because why not
    simple_adv_only_name();

    // And scan at the same time
    simple_ble_scan_start();

    while (1) {
        power_manage();
    }
}
