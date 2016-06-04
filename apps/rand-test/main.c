/*
 * Create an example service for providing 4 bytes of random data 
 *  Demonstrates use of RNG
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
#include "nrf_drv_rng.h"

// Define constants about this beacon.
#define DEVICE_NAME "RANDOM"

// LED pin number
#define LED0 18

// Length of random data to notify
#define RAND_NOTIFY_LEN 4

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
//  16-bit short uuid is 0xdc36 (bytes 12 and 13 of 128-bit UUID)
static simple_ble_service_t rand_service = {
    .uuid128 = {{0xe9, 0xc9, 0xc8, 0xa1, 0x0a, 0x3d, 0x78, 0xb8,
                 0xa3, 0x4a, 0x20, 0x41, 0xdc, 0x36, 0xf3, 0x30}}
};
static simple_ble_char_t rand_update_char = {.uuid16 = 0xdc37};
static simple_ble_char_t rand_notify_char = {.uuid16 = 0xdc38};
static uint8_t random [RAND_NOTIFY_LEN] = {0x76, 0x54, 0x32, 0x10};
static uint8_t rand_update_value = 0;

void ble_error(uint32_t error_code) {
  led_on(LED0);
}

// called automatically by simple_ble_init
void services_init (void) {
    // add rand service
    simple_ble_add_service(&rand_service);

    // add update characteristic
    simple_ble_add_characteristic(0, 1, 0, 0, // read, write, notify, vlen
            1, (uint8_t *) &rand_update_value,
            &rand_service, &rand_update_char);
    
    // add notify characteristic
    simple_ble_add_characteristic(1, 0, 1, 0, // read, write, notify, vlen
            RAND_NOTIFY_LEN, (uint8_t *) random,
            &rand_service, &rand_notify_char);
}

void ble_evt_write(ble_evt_t* p_ble_evt) {

    if (simple_ble_is_char_event(p_ble_evt, &rand_update_char)) {
        // user wrote to rand characteristic
        // use the blocking function to ensure randomness
        nrf_drv_rng_block_rand(random, RAND_NOTIFY_LEN);
        
        // notify new random value
        simple_ble_notify_char(&rand_notify_char);

    }
}

// main is essentially two library calls to setup all of the Nordic SDK
// API calls.
int main(void) {

    // Setup BLE
    simple_ble_init(&ble_config);
    led_init(LED0);
   
    // initialize rng
    nrf_drv_rng_init(NULL);

    // Advertise name
    simple_adv_only_name();

    while (1) {
        power_manage();
    }
}

