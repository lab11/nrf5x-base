// Global libraries
#include <stdint.h>

// Nordic libraries
#include "ble_advdata.h"

// nrf5x-base libraries
#include "simple_ble.h"
#include "simple_adv.h"

// Define constants about this beacon.
#define DEVICE_NAME "advSMtest"

// Manufacturer specific data setup
#define UMICH_COMPANY_IDENTIFIER 0x02E0
uint8_t mdata[2] = {0x01, 0x02};

// Intervals for advertising and connections
static simple_ble_config_t ble_config = {
    .platform_id       = 0x00,              // used as 4th octect in device BLE address
    .device_id         = DEVICE_ID_DEFAULT,
    .adv_name          = DEVICE_NAME,       // used in advertisements if there is room
    .adv_interval      = MSEC_TO_UNITS(500, UNIT_0_625_MS),
    .min_conn_interval = MSEC_TO_UNITS(500, UNIT_1_25_MS),
    .max_conn_interval = MSEC_TO_UNITS(1000, UNIT_1_25_MS)
};

static ble_uuid_t TEST_SERVICE_UUID = {0xBEAF, BLE_UUID_TYPE_BLE};

// main is essentially two library calls to setup all of the Nordic SDK
// API calls.
int main(void) {

    // Setup BLE
    simple_ble_init(&ble_config);

    ble_advdata_manuf_data_t mandata;
    mandata.company_identifier = UMICH_COMPANY_IDENTIFIER;
    mandata.data.p_data = mdata;
    mandata.data.size   = 2;

    // Setup our advertisement
    simple_adv_service_manuf_data(&TEST_SERVICE_UUID, &mandata);

    while (1) {
        power_manage();
    }
}
