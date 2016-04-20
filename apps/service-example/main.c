/*
 * Create an example service for controlling an LED
 *  Demonstrates reading, writing, and notifications
 */

// Global libraries
#include <stdint.h>

// Nordic libraries
#include "ble_advdata.h"

// nrf5x-base libraries
#include "simple_ble.h"
#include "simple_adv.h"
#include "led.h"
#include "device_info_service.h"

// Define constants about this beacon.
#define DEVICE_NAME "LED Service"

// LED pin number
#define LED0 13

// Intervals for advertising and connections
static simple_ble_config_t ble_config = {
    .platform_id       = 0x00,              // used as 4th octect in device BLE address
    .device_id         = DEVICE_ID_DEFAULT,
    .adv_name          = DEVICE_NAME,       // used in advertisements if there is room
    .adv_interval      = MSEC_TO_UNITS(500, UNIT_0_625_MS),
    .min_conn_interval = MSEC_TO_UNITS(500, UNIT_1_25_MS),
    .max_conn_interval = MSEC_TO_UNITS(1000, UNIT_1_25_MS)
};

// service and characteristic handles
//  UUID created by `uuidgen -r`
//  16-bit short uuid is 0x890f (bytes 12 and 13 of 128-bit UUID)
static simple_ble_service_t led_service = {
    .uuid128 = {{0x87, 0xa4, 0xde, 0xa0, 0x96, 0xea, 0x4e, 0xe6,
                 0x87, 0x45, 0x83, 0x28, 0x89, 0x0f, 0xad, 0x7b}}
};
static simple_ble_char_t led_on_char = {.uuid16 = 0x8910};
static simple_ble_char_t led_off_char = {.uuid16 = 0x8911};
static simple_ble_char_t led_state_char = {.uuid16 = 0x8912};
static uint8_t led_on_value = 0;
static uint8_t led_off_value = 0;
static uint8_t led_state_value = 0;

// called automatically by simple_ble_init
void services_init (void) {
    // add led service
    simple_ble_add_service(&led_service);

    // add led_on characteristic
    simple_ble_add_characteristic(0, 1, 0, 0, // read, write, notify, vlen
            1, (uint8_t*)&led_on_value,
            &led_service, &led_on_char);

    // add led_off characteristic
    simple_ble_add_characteristic(0, 1, 0, 0, // read, write, notify, vlen
            1, (uint8_t*)&led_off_value,
            &led_service, &led_off_char);

    // add led_state characteristic
    simple_ble_add_characteristic(1, 0, 1, 0, // read, write, notify, vlen
            1, (uint8_t*)&led_state_value,
            &led_service, &led_state_char);
}

void ble_evt_write(ble_evt_t* p_ble_evt) {

    if (simple_ble_is_char_event(p_ble_evt, &led_on_char)) {
        // user wrote to led_on characteristic
        led_on(LED0);

        // update led state and notify
        led_state_value = 1;
        simple_ble_notify_char(&led_state_char);

    } else if (simple_ble_is_char_event(p_ble_evt, &led_off_char)) {
        // user wrote to led_off characteristic
        led_off(LED0);

        // update led state and notify
        led_state_value = 0;
        simple_ble_notify_char(&led_state_char);
    }
}

// main is essentially two library calls to setup all of the Nordic SDK
// API calls.
int main(void) {

    // Setup BLE
    simple_ble_init(&ble_config);
    led_init(LED0);

    // Add device information service
    simple_ble_device_info_service("1.2.0", "3.4.5", "1.0.1");

    // Advertise name
    simple_adv_only_name();

    while (1) {
        power_manage();
    }
}

