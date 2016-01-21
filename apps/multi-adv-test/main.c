/*
 * Switch advertisements between Eddystone, device name, and custom data
 */

// Global libraries
#include <stdint.h>

// Nordic libraries
#include "ble_advdata.h"

// nrf5x-base libraries
#include "simple_ble.h"
#include "eddystone.h"
#include "simple_adv.h"
#include "multi_adv.h"


// Define constants about this beacon.
#define DEVICE_NAME "nRFtest"
#define PHYSWEB_URL "goo.gl/aaaaaa"

// How many milliseconds between switching advertisements
#define ADV_SWITCH_MS 1000

// Custom UTF-8 name. Flash this app and scan to see it!
char name[14] = {0xE2, 0x9C, 0xAE, 0x6D, 0xE2, 0x9A, 0x9B,
                 0x61, 0x64, 0x76, 0xE2, 0x9C, 0xAE, 0x00};

// Manufacturer specific data setup
#define UMICH_COMPANY_IDENTIFIER 0x02E0
uint8_t mdata[2] = {0x01, 0x02};

// Intervals for advertising and connections
static simple_ble_config_t ble_config = {
    .platform_id       = 0x00,              // used as 4th octect in device BLE address
    .device_id         = DEVICE_ID_DEFAULT,
    .adv_name          = name,              // used in advertisements if there is room
    .adv_interval      = MSEC_TO_UNITS(500, UNIT_0_625_MS),
    .min_conn_interval = MSEC_TO_UNITS(500, UNIT_1_25_MS),
    .max_conn_interval = MSEC_TO_UNITS(1000, UNIT_1_25_MS)
};

static void adv_config_eddystone () {
    eddystone_adv(PHYSWEB_URL, NULL);
}

static void adv_config_name () {
    simple_adv_only_name();
}

static void adv_config_data () {
    ble_advdata_manuf_data_t mandata;

    // Increment so the manufac data changes
    mdata[1]++;

    mandata.company_identifier = UMICH_COMPANY_IDENTIFIER;
    mandata.data.p_data = mdata;
    mandata.data.size   = 2;

    simple_adv_manuf_data(&mandata);
}

// main is essentially two library calls to setup all of the Nordic SDK
// API calls.
int main(void) {

    // Setup BLE
    simple_ble_init(&ble_config);

    // Need to init multi adv
    multi_adv_init(ADV_SWITCH_MS);

    // Now register our advertisement configure functions
    multi_adv_register_config(adv_config_eddystone);
    multi_adv_register_config(adv_config_name);
    multi_adv_register_config(adv_config_data);

    // Start rotating
    multi_adv_start();

    while (1) {
        power_manage();
    }
}
